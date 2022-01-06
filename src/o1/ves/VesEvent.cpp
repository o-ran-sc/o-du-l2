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
#include "Message.hpp"

/* Constructor */
VesEvent::VesEvent(VesEventType eventType) 
               : mVesEventType(eventType) {

          
}; 


/* Default Destructor*/
VesEvent::~VesEvent()
{
   if ( mHttpClient != NULL )
   {
      delete mHttpClient;
   }
   free(mSendData);
}


/*******************************************************************
 *
 * @brief Initialize the Ves Event
 *
 * @details
 *
 *    Function : init
 *
 *    Functionality:
 *      - Intialize the Ves event with configuration
 *        Sets the URL
 *        Instantiates the http client
 *
 * @params[in] IN   - void
 * @return void
 *
 * ****************************************************************/

void VesEvent::init(){

   getConfig();
   createUrl();
   mHttpClient = new HttpClient(mVesUrl, mVesServerUsername, mVesServerPassword);

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

bool VesEvent::prepare(const Message* msg)
{

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
      JsonHelper::deleteNode(rootNode);
      return false;
   }
   VesCommonHeader vesCommHdr;
   if(vesCommHdr.prepare(commHdrNode, mVesEventType))
   {
      if(JsonHelper::addJsonNodeToObject(event, "commonEventHeader", commHdrNode) == 0) 
      {
         O1_LOG("\nO1 VesEvent : could not add commonEventHeader object");
         JsonHelper::deleteNode(rootNode);
         return false;
      }
      else {
         //add header into the message and create pnfFields
         mVesEventFields = JsonHelper::createNode();
         if(mVesEventFields == 0) {
            O1_LOG("\nO1 VesEvent : could not create Ves Event Fields JSON object");
            JsonHelper::deleteNode(rootNode);
            return false;
         }
         if(!prepareEventFields(msg)) {
            O1_LOG("\nO1 VesEvent : could not prepare Ves Event Fields Node");
            JsonHelper::deleteNode(rootNode);
            return false;
         }
         if(JsonHelper::addJsonNodeToObject(event, getEventFieldName().c_str(), mVesEventFields) == 0) {
            O1_LOG("\nO1 VesEvent : could not add mVesEventFields object");
            JsonHelper::deleteNode(rootNode);
            return false;
         }
         mSendData = JsonHelper::printUnformatted(rootNode);
         char* rootNode_string = JsonHelper::print(rootNode);
	 O1_LOG("\nO1 VesEvent : VES request : -- \n%s\n", rootNode_string);
	 free(rootNode_string);
         JsonHelper::deleteNode(rootNode);  //deleting the rootNode here; (after getting the string version of the json created)
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

/*******************************************************************
 *
 * @brief Send the Ves event over Http
 *
 * @details
 *
 *    Function : send
 *
 *    Functionality:
 *      - Sends the Ves event over http
 *
 * @params[in] IN   - void
 * @return true     - success
 *         false    - failure
 *
 * ****************************************************************/

bool VesEvent::send()
{
   return mHttpClient->send(mSendData);
}

/*******************************************************************
 *
 * @brief gets Event Type name from VesEventType Enum
 *
 * @details
 *
 *    Function : getEventFieldName
 *
 *    Functionality:
 *      - returns VesEvent name
 *
 *
 * @params[in] void
 * @return string : Ves Event Name
 ******************************************************************/

string VesEvent::getEventFieldName() 
{

   switch(mVesEventType)
   {
      case VesEventType::PNF_REGISTRATION:
      {
         return "pnfRegistrationFields";
      }
      case VesEventType::FAULT_NOTIFICATION:
      {
         return "faultFields";
      }
      case VesEventType::PM_SLICE:
      {
         return "measurementFields";
      }
      case VesEventType::HEARTBEAT:
      {
         return "heartbeatFields";
      }
      default:
         return "eventFields";
   }
}

/*******************************************************************
 *
 * @brief Get Ves Collector configuration 
 *
 * @details
 *
 *    Function : getConfig
 *
 *    Functionality:
 *      - Gets Ves Collector configuration
 *
 *
 * @params[in] void
 * @return void
 ******************************************************************/
void VesEvent::getConfig()
{
    mVesServerIp = ConfigLoader::instance().getOamConfigFile().getVesServerIp();
    mVesServerPort = ConfigLoader::instance().getOamConfigFile().getVesServerPort();
    mVesServerUsername = ConfigLoader::instance().getOamConfigFile().getVesServerUsername();
    mVesServerPassword = ConfigLoader::instance().getOamConfigFile().getVesServerPassword();
}

/*******************************************************************
 *
 * @brief Create the URL for sending VES messages
 *
 * @details
 *
 *    Function : createUrl
 *
 *    Functionality:
 *      - Creates the VES URL
 *
 *
 * @params[in] void
 * @return void
 ******************************************************************/
void VesEvent::createUrl()
{
   mVesUrl = "https://" + mVesServerIp + ":" + mVesServerPort + "/eventListener/v7";
}



/**********************************************************************
  End of file
 **********************************************************************/

