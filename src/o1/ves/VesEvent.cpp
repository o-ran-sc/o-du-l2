/*******************************************************************************
################################################################################
#   Copyright (c) [2021] [HCL Technologies Ltd.]                               #
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

/* This file contains functions to support the preparation of VES Event 
   It is the parent class of Ves Event, Every event need to override at 
   least prepareEventFields function and fill specific values of event 
   fields */

#include "VesEvent.hpp"
#include "HttpClient.hpp"

/*******************************************************************
 *
 * @brief prepare Ves Event Fields
 *
 * @details
 *
 *    Function : prepare
 *
 *    Functionality:
 *      - prepare Ves Event Fields in json format
 *
 * @params[in] IN   - void 
 * @return true     - success
 *         false    - failure
 *
 * ****************************************************************/


bool VesEvent::prepare()
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
       JsonHelper::deleteNode(rootNode);
       return false;
    }

    if(JsonHelper::addJsonNodeToObject(rootNode, "event", event) == 0) {
       O1_LOG("O1 VES : could not add event Object\n");
       JsonHelper::deleteNode(rootNode);
       return false;
    }

   cJSON *commHdrNode = JsonHelper::createNode();
   if(commHdrNode == 0) {
      O1_LOG("O1 VES : could not create common header Node JSON object");
      return false;
   }
 
   VesCommonHeader vesCommHdr;

   if(vesCommHdr.prepare(commHdrNode, mVesEventType))
   {
       if(JsonHelper::addJsonNodeToObject(event, "commonEventHeader", \
                                commHdrNode) == 0) {
       O1_LOG("O1 VES : could not add commonEventHeader object\n");
       JsonHelper::deleteNode(rootNode);
       return false;
      }
      else {
           
         //add header into the message and create pnfFields
         mVesEventFields = JsonHelper::createNode();
         if(mVesEventFields == 0) {
            O1_LOG("O1 VES : could not create Ves Event Fields JSON object\n");
            return false;
         }

         if(!prepareEventFields()) {
         O1_LOG("O1 VES : could not prepare Ves Event Fields Node \n");
         JsonHelper::deleteNode(rootNode);
         return false;
         }
    
         if(JsonHelper::addJsonNodeToObject(event, "pnfRegistrationFields", mVesEventFields) == 0) {
            O1_LOG("O1 VES : could not add mVesEventFields object\n");
            JsonHelper::deleteNode(rootNode);
            return false;
         }

      mSendData = JsonHelper::printUnformatted(rootNode);
      O1_LOG("O1 VES : final ves request : -- \n%s\n", JsonHelper::print(rootNode));
      }
   }
   else
   {
      O1_LOG("O1 VES : preparePnfRegistration common header preparation failed\n");
      JsonHelper::deleteNode(rootNode);
      return false;
   }
   return true; 
}

bool VesEvent::send()
{
   if(!getVesCollectorDetails()) {
      O1_LOG("O1 VES : Could not get successfully details of Ves Collector\n");
      return false; 
   }
   
   HttpClient httpClient(mVesServerIp, mVesServerPort, mVesServerUsername, \
                         mVesServerPassword);

   return httpClient.send(mSendData);
}

/*******************************************************************
 *
 * @brief Gets the Ves Collector Server details
 *
 * @details
 *
 *    Function : getVesCollectorDetails
 *
 *    Functionality:
 *      - Gets the Ves Collector Server details by reading config file
 *
 * @params[in] IN   - pointer to pnfFields 
 * @return true     - success
 *         false    - failure
 *
 * ****************************************************************/


bool VesEvent::getVesCollectorDetails()
{
   return readConfigFile();
}

/*******************************************************************
 *
 * @brief Read Ves Collector config json file
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

bool VesEvent::readConfigFile()
{
   cJSON *json = JsonHelper::read(SMO_CONFIG);
   if(json == NULL) {
       O1_LOG("O1 VES : Config file reading error is  :%s\n", JsonHelper::getError());
    return false;
   }
   else {
      cJSON *rootNode = NULL;
      rootNode = JsonHelper::getNode(json, "smoConfig");
      if(rootNode) {
         O1_LOG("O1 VES : Reading smoConfig.json file\n");
         mVesServerIp = JsonHelper::getValue(rootNode, "smoV4IpAddress");
         mVesServerPort = JsonHelper::getValue(rootNode, "smoPort");
         mVesServerUsername = JsonHelper::getValue(rootNode, "username");
         mVesServerPassword = JsonHelper::getValue(rootNode, "password");
      }
      else {
         O1_LOG("O1 VES : smoConfig Object is not availbale in config file\n");
         return false;
      }
   }  
   JsonHelper::deleteNode(json);
   return true;    
}
