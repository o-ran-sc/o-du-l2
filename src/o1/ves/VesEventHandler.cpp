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

/* This file contains functions to support the preparation of Common Header part of VES Event*/


#include "VesEventHandler.hpp"
#include "PnfRegistration.hpp"
#include "HttpClient.hpp"

/* Default constructor */
VesEventHandler::VesEventHandler()
{

}

/* Destructor */
VesEventHandler::~VesEventHandler()
{

}

/*******************************************************************
 *
 * @brief prepare and send  Ves Message
 *
 * @details
 *
 *    Function : sendVesEvent
 *
 *    Functionality:
 *      - prepare VES event and send to oam 
 *
 * @params[in] void
 * @return true     - success
 *         false    - failure
 *
 * ****************************************************************/

bool VesEventHandler::sendVesEvent(VesEventType evtType)
{
   //check event type and call funtions accordingly
   bool ret = true;
   char *sendData;
   O1_LOG("O1 VES : sendVesEvent started\n");
   switch(evtType)
   {
      case VesEventType::pnfRegistration:
         {
            O1_LOG("O1 VES : send PNP registration\n");
            PnfRegistration pnfReg;
            pnfReg.preparePnfRegistrationReq(&sendData);
            break;
         }
      case VesEventType::alarmNotification:
         O1_LOG("O1 VES : send VES alarm notification\n");
         break;
      case VesEventType::pmNotification:
         O1_LOG("O1 VES : send VES pm notification\n");
         break;
      case VesEventType::none:
      default:
         O1_LOG("O1 VES : send VES msg Type is not avilable\n");
         ret = false;
         break;
   }
               
   HttpClient httpSend;
   if(!httpSend.sendVesEvent(sendData))
   {
      O1_LOG("O1 VES : could not send VES Event\n");
      ret = false; 
   };
   return ret;
}
/**********************************************************************
  End of file
 **********************************************************************/
