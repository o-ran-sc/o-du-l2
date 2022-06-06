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
#include "PnfRegistrationEvent.hpp"
#include "SliceMeasurementEvent.hpp"
#include "Message.hpp"
#include "CellStateChange.hpp"
#include "CellStateChangeStdDef.hpp"
#include "SliceMeasurementEventStdDef.hpp"

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

bool VesEventHandler::prepare(VesEventType evtType, const Message* msg)
{
   //check event type and call funtions accordingly
   bool ret = true;
   switch(evtType)
   {
      case VesEventType::PNF_REGISTRATION:
      {
         O1_LOG("\nO1 VesEventHandler : Preparing PNF registration");
         mVesEvent = new PnfRegistrationEvent();
         break;
      }
      case VesEventType::PM_SLICE:
      {
         #ifdef StdDef
         mVesEvent = new SliceMeasurementEventStdDef;
         O1_LOG("\nO1 VesEventHandler : Preparing Standard VES PM Slice");
         #else
         mVesEvent = new SliceMeasurementEvent;
         O1_LOG("\nO1 VesEventHandler : Preparing VES PM Slice");
         #endif
         break;
      }
      case VesEventType::FAULT_NOTIFICATION:
      {
         #ifdef StdDef
         O1_LOG("\nO1 VesEventHandler : Preparing Standard VES fault notification");
         mVesEvent = new CellStateChangeStdDef();
         #else
         O1_LOG("\nO1 VesEventHandler : Preparing VES fault notification");
         mVesEvent = new CellStateChange();
         #endif
         break;
      }

      default:
         O1_LOG("\nO1 VesEventHandler : VES message type does not exist ");
         ret = false;
         break;
   }
   mVesEvent->init();
   if(!mVesEvent->prepare(msg)) {
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
