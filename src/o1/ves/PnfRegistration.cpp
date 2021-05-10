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

/* This file contains macros and functions to support the preparation of pnf
   Registration VES Event*/

#include <ctime>
#include <string.h>
#include <stdlib.h>
#include <sstream>
#include "VesCommonHeader.hpp"
#include "PnfRegistration.hpp"
#include "JsonHelper.hpp"
#include "VesUtils.hpp"
 
/* Default constructor */
PnfRegistration::PnfRegistration()
{

}

/* Default Destructor */
PnfRegistration::~PnfRegistration()
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

bool PnfRegistration::getCurrentDate(char *dateStr)
{
   time_t t = time(0);
   std::ostringstream oss; 
   strftime(dateStr, MAX_TIME_STR, "%F", localtime(&t));
   //strftime(dateStr, MAX_TIME_STR, "%y-%m-%d", localtime(&t));
   return true;
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

string PnfRegistration::getNetconfV4ServerIP()
{
   if(mNetconfIp != "") {
      return mNetconfIp;
   }
   else {
      return NETCONF_V4_SERVER_IP;
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

string PnfRegistration::getNetconfPort()
{
   if(mNetconfPort != "") {
      return mNetconfPort;
   }
   else {
      return OAM_PORT;
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


string PnfRegistration::getUsername()
{
   if(mNetconfPort != "") {
      return mUsername;
   }
   else {
      return USERNAME;
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

string PnfRegistration::getPassword()
{
   if(mNetconfPort != "") {
      return mPassword;
   }
   else {
      return PASSWORD;
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

string PnfRegistration::getNetconfV6ServerIP()
{
   return NETCONF_V6_SERVER_IP;
}

/*******************************************************************
 *
 * @brief check as if PNF additional fields are required
 *
 * @details
 *
 *    Function : isPnfAdditionalFieldsRequired
 *
 *    Functionality:
 *      - check that pnf additional fields are required or not
 *
 * @params[in] void
 * @return true - success
 *
 * ****************************************************************/

bool PnfRegistration::isPnfAdditionalFieldsRequired() 
{
   return true; //default is true ... will implement further for such 
                //cases when additinal fields are not required
}

/*******************************************************************
 *
 * @brief prepare PNF registration event
 *
 * @details
 *
 *    Function : preparePnfRegistrationReq
 *
 *    Functionality:
 *      - prepare PNF registration event fields in json formate
 *
 * @params[in] OUT - pointer to sendData 
 * @return true     - success
 *         false    - failure
 *
 * ****************************************************************/

bool PnfRegistration::preparePnfRegistrationReq(char ** sendData)
{
    O1_LOG("O1 VES : prepare PNF Registration start\n");
    cJSON *rootNode = JsonHelper::createNode();
    if(rootNode == 0) {
        O1_LOG("O1 VES : could not create cJSON root Node object\n");
        return false;
    }

    cJSON *event = JsonHelper::createNode();
    if(event == 0) {
       O1_LOG("O1 VES : could not create event cJSON object\n");
       cJSON_Delete(rootNode);
       return false;
    }

    if(cJSON_AddItemToObject(rootNode, "event", event) == 0) {
       O1_LOG("O1 VES : could not add event Object\n");
       cJSON_Delete(rootNode);
       return false;
    }

   cJSON *commHdrNode = JsonHelper::createNode();
   if(commHdrNode == 0) {
      O1_LOG("O1 VES : could not create common header Node JSON object");
      return false;
   }
 
   VesCommonHeader vesCommHdr;

   if(vesCommHdr.prepareCommonHeader(commHdrNode, \
                                     VesEventType::pnfRegistration))
   {
         if(cJSON_AddItemToObject(event, "commonEventHeader", \
                                    commHdrNode) == 0) {
         O1_LOG("O1 VES : could not add commonEventHeader object\n");
         cJSON_Delete(rootNode);
         return false;
      }
      else {
           
       //add header into the message and create pnfFields
         cJSON* pnfFieldsNode = JsonHelper::createNode();
         if(pnfFieldsNode == 0) {
            O1_LOG("O1 VES : could not create Pnf Registration Fields JSON object\n");
            return false;
         }

         if(!preparePnfRegistrationFields(pnfFieldsNode) || \
                                          (pnfFieldsNode ==0)) {
         O1_LOG("O1 VES : could not create pnfFieldsNode \n");
         cJSON_Delete(rootNode);
         return false;
         }
    
         if(cJSON_AddItemToObject(event, "pnfRegistrationFields", pnfFieldsNode) == 0) {
            O1_LOG("O1 VES : could not add pnfFieldsNode object\n");
            cJSON_Delete(rootNode);
            return false;
         }

      *sendData = cJSON_PrintUnformatted(rootNode);
       O1_LOG("O1 VES : final ves request : -- \n%s\n", cJSON_Print(rootNode));
      }
   }
   else
   {
      O1_LOG("O1 VES : preparePnfRegistration common header preparation failed\n");
      cJSON_Delete(rootNode);
      return false;
   }
   return true; 
}

/*******************************************************************
 *
 * @brief prepare PNF registration Fields
 *
 * @details
 *
 *    Function : preparePnfRegistrationFields
 *
 *    Functionality:
 *      - prepare PNF registration Fields in json format
 *
 * @params[in] IN   - pointer to pnfFields 
 * @return true     - success
 *         false    - failure
 *
 * ****************************************************************/

bool PnfRegistration::preparePnfRegistrationFields(cJSON* pnfFields)
{
   bool ret = true;
   char dateStr[MAX_TIME_STR];
   
   if(!readConfigFile())
   {
      O1_LOG("O1 VES : config file reading failed\n");
      //return false;
   }
   if(JsonHelper::addNodeToObject(pnfFields, "pnfRegistrationFieldsVersion", \
                                       PNF_REGISTRATION_VERSION) == 0) {
      ret = false;
   }
   else if(getCurrentDate(dateStr) && (JsonHelper::addNodeToObject(pnfFields, \
				   "lastServiceDate", dateStr) == 0)) {
      ret = false;
   }
   else if(JsonHelper::addNodeToObject(pnfFields, "macAddress", \
                                       MAC_ADDR) == 0) {
      ret = false;
   }
   if(getCurrentDate(dateStr) && (JsonHelper::addNodeToObject(pnfFields, \
				   "manufactureDate", dateStr) == 0)) {
      ret = false;
   }
   else if(JsonHelper::addNodeToObject(pnfFields, "modelNumber", \
                                       MODEL_NUMBER) == 0) {
      ret = false;
   }
   else if(JsonHelper::addNodeToObject(pnfFields, "oamV4IpAddress", \
                                       getNetconfV4ServerIP().c_str()) == 0) {
      ret = false;
   }
   else if(JsonHelper::addNodeToObject(pnfFields, "oamV6IpAddress", \
                                       NETCONF_V6_SERVER_IP) == 0) {
      ret = false;
   }
   else if(JsonHelper::addNodeToObject(pnfFields, "serialNumber", \
                                       SERIAL_NUMBER) == 0) {
      ret = false;
   }
   else if(JsonHelper::addNodeToObject(pnfFields, "softwareVersion", \
                                       SOFTWARE_VERSION) == 0) {
      ret = false;
   }
   else if(JsonHelper::addNodeToObject(pnfFields, "unitFamily", \
                                       UNIT_FAMILY) == 0) {
      ret = false;
   }
   else if(JsonHelper::addNodeToObject(pnfFields, "unitType", \
                                       UNIT_TYPE) == 0) {
      ret = false;
   }
   else if(JsonHelper::addNodeToObject(pnfFields, "vendorName", \
                                       VENDER_NAME) == 0) {
      ret = false;
   }
   else
   {
      cJSON *addFields = JsonHelper::createNode();
      if(addFields == 0) {
         O1_LOG("O1 VES : could not create additional fields JSON object\n");
         return false;
      }

      if(isPnfAdditionalFieldsRequired() && \
         preparePnfAdditionalFields(addFields))
      {
         O1_LOG("O1 VES : PNF parameter prepration done adding additional parameters \
if required \n");
      if(addFields == 0) {
         O1_LOG("O1 VES : could not prepare additional fields cJSON object\n");
         cJSON_Delete(pnfFields);
         return false;
         }
 
      if(cJSON_AddItemToObject(pnfFields, "additionalFields", \
                               addFields) == 0) {
         O1_LOG("O1 VES : could not add additional fileds\n");
         cJSON_Delete(pnfFields);
         return false;
         }
       }
       O1_LOG("O1 VES : Preparation on PNF registration additional fields done \
successfully \n");
   }
   return ret;
}

/*******************************************************************
 *
 * @brief prepare PNF registration additional Fields
 *
 * @details
 *
 *    Function : preparePnfAdditionalFields
 *
 *    Functionality:
 *      - prepare PNF registration additional Fields in json formate
 *
 * @params[in] IN   - pointer to addField 
 * @return true     - success
 *         false    - failure
 *
 * ****************************************************************/

bool PnfRegistration::preparePnfAdditionalFields(cJSON *addFields)
{
   bool ret = true;
   if(JsonHelper::addNodeToObject(addFields, "oamPort", getNetconfPort().c_str()) == 0) {
      ret = false;
   }
   else if(JsonHelper::addNodeToObject(addFields, "protocol", \
                                       OAM_PROTOCOL) == 0) {
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
                                       RECONNECT_ON_SCHEMA_CHANGE) == 0) {
      ret = false;
   }
   else if(JsonHelper::addNodeToObject(addFields, "sleep-factor", \
                                       SLEEP_FACTOR) == 0) {
      ret = false;
   }
   else if(JsonHelper::addNodeToObject(addFields, "tcpOnly", \
                                       TCP_ONLY) == 0) {
      ret = false;
   }
   else if(JsonHelper::addNodeToObject(addFields, "connectionTimeout", \
                                       CONNECTION_TIMEOUT) == 0) {
      ret = false;
   }
   else if(JsonHelper::addNodeToObject(addFields, "maxConnectionAttempts", \
                                       MAX_CONNECTION_ATTEMPTS) == 0) {
      ret = false;
   }
   else if(JsonHelper::addNodeToObject(addFields, "betweenAttemptsTimeout", \
                                       BETWEEN_ATTEMPTS_TIMEOUT) == 0) {
      ret = false;
   }
   else if(JsonHelper::addNodeToObject(addFields, "keepaliveDelay", \
                                       KEEPALIVE_DELAY) == 0) {
      ret = false;
   }
   O1_LOG("O1 VES : additonal field preparation of PNF done successfully \n");
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
 *      - Reads json file.
 *
 *
 * @params[in] void
 * @return true  : success
 *         false : failure
 ******************************************************************/

bool PnfRegistration::readConfigFile()
{
   cJSON *json = JsonHelper::readConfig(NETCONF_CONFIG);
   if(json == NULL) {
       O1_LOG("O1 VES : Config file reading error is  :%s\n", cJSON_GetErrorPtr());
    return false;
    }
    else {
       if(!parseConfig(json))
       {
          O1_LOG("O1 VES : Config file parsing error is  :%s\n", cJSON_GetErrorPtr());
          return false;
       }
   }
   cJSON_free(json);
   return true;    
}

/*******************************************************************
 *
 * @brief parse json configuration file 
 *
 * @details
 *
 *    Function : parseConfig
 *
 *    Functionality:
 *      - parse smo json configuration file.
 *
 *
 * @params[in] pointer to cJson
 * @return true  : success
 *         false : failure
 ******************************************************************/


bool PnfRegistration::parseConfig(cJSON *json)
{
   cJSON *rootNode = NULL;
   rootNode = cJSON_GetObjectItem(json, "NetconfServer");
   if(rootNode) {
      O1_LOG("O1 VES : Reading NetconfServer config file\n");
      mNetconfIp = JsonHelper::getObject(rootNode, "NetconfServerIp");
      mNetconfPort = JsonHelper::getObject(rootNode, "NetconfPort");
      mUsername = JsonHelper::getObject(rootNode, "NetconfUsername");
      mPassword = JsonHelper::getObject(rootNode, "NetconfPassword");
      }
   else {
      O1_LOG("O1 VES : smoConfig Object is not availbale in config file\n");
      return false;
   }
   return true;
}

/**********************************************************************
  End of file
 **********************************************************************/
