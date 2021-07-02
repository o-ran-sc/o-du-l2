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

/* This file contains functions to support the preparation of VES Event
   It is the parent class of Ves Event, Every event need to override at
   least prepareEventFields function and fill specific values of event
   fields */

#include "VesEvent.hpp"


/* Default constructor*/
VesEvent::VesEvent()
{
   mHttpClient = NULL;
}


/* Default Destructor*/
VesEvent::~VesEvent()
{
   if(mHttpClient != NULL)
   {
      delete mHttpClient;
   }
}

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

   if(!readConfigFile()) {
      O1_LOG("\nO1 VesEvent : Could not get SMO details");
      return false;
   }

   mHttpClient = new HttpClient(mVesServerIp, mVesServerPort, mVesServerUsername, \
                         mVesServerPassword);

   cJSON *rootNode = JsonHelper::createNode();
   if(rootNode == 0) {
       O1_LOG("\nO1 VesEvent : could not create cJSON root Node object");
       return false;
   }

   cJSON *event = JsonHelper::createNode();
   if(event == 0) {
      O1_LOG("\nO1 VesEvent : could not create event cJSON object");
      JsonHelper::deleteNode(rootNode);
      return false;
   }

   if(JsonHelper::addJsonNodeToObject(rootNode, "event", event) == 0) {
      O1_LOG("\nO1 VesEvent : could not add event Object");
      JsonHelper::deleteNode(rootNode);
      return false;
   }

   cJSON *commHdrNode = JsonHelper::createNode();
   if(commHdrNode == 0) {
      O1_LOG("\nO1 VesEvent : could not create common header Node JSON object");
      return false;
   }

   VesCommonHeader vesCommHdr;

   if(vesCommHdr.prepare(commHdrNode, mVesEventType))
   {
       if(JsonHelper::addJsonNodeToObject(event, "commonEventHeader", \
                                commHdrNode) == 0) {
       O1_LOG("\nO1 VesEvent : could not add commonEventHeader object");
       JsonHelper::deleteNode(rootNode);
       return false;
      }
      else {

         //add header into the message and create pnfFields
         mVesEventFields = JsonHelper::createNode();
         if(mVesEventFields == 0) {
            O1_LOG("\nO1 VesEvent : could not create Ves Event Fields JSON object");
            return false;
         }

         if(!prepareEventFields()) {
         O1_LOG("\nO1 VesEvent : could not prepare Ves Event Fields Node");
         JsonHelper::deleteNode(rootNode);
         return false;
         }

         if(JsonHelper::addJsonNodeToObject(event, "pnfRegistrationFields", mVesEventFields) == 0) {
            O1_LOG("\nO1 VesEvent : could not add mVesEventFields object");
            JsonHelper::deleteNode(rootNode);
            return false;
         }

      mSendData = JsonHelper::printUnformatted(rootNode);
      O1_LOG("\nO1 VesEvent : VES request : -- \n%s\n", JsonHelper::print(rootNode));
      }
   }
   else
   {
      O1_LOG("\nO1 VesEvent : Failed to prepare preparePnfRegistration common header");
      JsonHelper::deleteNode(rootNode);
      return false;
   }
   return true;
}

bool VesEvent::send()
{
   return mHttpClient->send(mSendData);
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
   cJSON *json = JsonHelper::read(VES_CONFIG);
   if(json == NULL) {
       O1_LOG("\nO1 VesEvent : Error reading config file  :%s", JsonHelper::getError());
       return false;
   }
   else {
      cJSON *rootNode = NULL;
      rootNode = JsonHelper::getNode(json, "vesConfig");
      if(rootNode) {
         O1_LOG("\nO1 VesEvent : Reading smoConfig.json file\n");
         mVesServerIp = JsonHelper::getValue(rootNode, "vesV4IpAddress");
         mVesServerPort = JsonHelper::getValue(rootNode, "vesPort");
         mVesServerUsername = JsonHelper::getValue(rootNode, "username");
         mVesServerPassword = JsonHelper::getValue(rootNode, "password");
      }
      else {
         O1_LOG("\nO1 VesEvent : smoConfig Object is not available in config file");
         return false;
      }
   }
   JsonHelper::deleteNode(json);
   return true;
}
