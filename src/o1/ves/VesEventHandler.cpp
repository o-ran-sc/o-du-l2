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

/* This file contains functions to support the preparation of Common Header part
   of VES Event*/


#include "VesEventHandler.hpp"
#include "PnfRegistration.hpp"

/*******************************************************************
 *
 * @brief Constructor
 *
 * @details
 *
 *    Function : VesEventHandler
 *
 *    Functionality:
 *      - Constructor intialization
 *
 * @params[in] NULL
 * @return None
 ******************************************************************/
VesEventHandler::VesEventHandler() : mVesEvent(NULL)
{

}

/*******************************************************************
 *
 * @brief Destructor
 *
 * @details
 *
 *    Function : ~VesEventHandler
 *
 *    Functionality:
 *      - Destructor
 *
 * @params[in] None
 * @return None
 ******************************************************************/
VesEventHandler::~VesEventHandler()
{
   if( mVesEvent != NULL )
      delete mVesEvent;
}

/*******************************************************************
 *
 * @brief Prepare VES Message
 *
 * @details
 *
 *    Function : prepare
 *
 *    Functionality:
 *      - prepare VES event
 *
 * @params[in] void
 * @return true     - success
 *         false    - failure
 *
 * ****************************************************************/

bool VesEventHandler::prepare(VesEventType evtType)
{
   //check event type and call funtions accordingly
   bool ret = true;
   switch(evtType)
   {
      case VesEventType::PNF_REGISTRATION:
         {
            O1_LOG("\nO1 VesEventHandler : Preparing PNF registration");
            mVesEvent = new PnfRegistration;
            break;
         }
      case VesEventType::FAULT_NOTIFICATION:
         O1_LOG("\nO1 VesEventHandler : Preparing VES fault notification");
         break;
      case VesEventType::PM_NOTIFICATION:
         O1_LOG("\nO1 VesEventHandler : Preparing VES pm notification");
         break;
      case VesEventType::HEARTBEAT:
         O1_LOG("\nO1 VesEventHandler : Preparing VES heartbeat");
         break;
      default:
         O1_LOG("\nO1 VesEventHandler : VES msg Type is not available");
         ret = false;
         break;
   }
   if(!mVesEvent->prepare()) {
      O1_LOG("\nO1 VesEventHandler : Failed to prepare VES message");
      ret = false;
   }
   return ret;
}

/*******************************************************************
 *
 * @brief Send Ves Message
 *
 * @details
 *
 *    Function : send
 *
 *    Functionality:
 *      - Send VES event to SMO
 *
 * @params[in] void
 * @return true     - success
 *         false    - failure
 *
 * ****************************************************************/

bool VesEventHandler::send()
{
   if (!mVesEvent->send()) {
      O1_LOG("\nO1 VesEventHandler : Failed to send VES event");
      return false;
   }
   return true;
}
/**********************************************************************
  End of file
 **********************************************************************/
