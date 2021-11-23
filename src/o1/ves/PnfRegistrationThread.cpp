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

/* This file contains the class which runs as a thread to send the PNF
   registration message.
*/


#include "PnfRegistrationThread.hpp"
#include "VesUtils.hpp"
#include "VesEventHandler.hpp"

#include <unistd.h>


bool PnfRegistrationThread::run()
{

   const int N_RETRY = 2;
   const int RETRY_DELAY = 10;
   /* Prepare VES PNF registration request */
   VesEventHandler vesEvtHdr;
   if(vesEvtHdr.prepare(VesEventType::PNF_REGISTRATION))
   {
      /* Send VES PNF registration request */
      for( int i = 0; i < N_RETRY; i++)
      {
         sleep(RETRY_DELAY);
         O1_LOG("\nO1 PnfRegistrationThread : Sending PNF registration. Attempt %d\n", i );
         vesEvtHdr.send();
      }
   }
   return true;

}

/**********************************************************************
  End of file
 **********************************************************************/