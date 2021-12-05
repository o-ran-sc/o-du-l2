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

/* This file contains the C interface for ODU to start the O1 module */

#include "O1Interface.h"
#include "O1App.hpp"
#include "GlobalDefs.hpp"
#include "PnfRegistrationThread.hpp"
#include "ConfigLoader.hpp"

#include <signal.h>
#include <unistd.h>


/*******************************************************************
 *
 * @brief Wait for the O1 Module to start
 *
 * @details
 *
 *    Function : check_O1_module_status
 *
 *    Functionality:
 *      - Checks if the O1App has started
 *
 *
 * @params[in] void
 * @return O1::SUCCESS - success
 *         O1::FAILURE - failure
 ******************************************************************/

int static check_O1_module_status(void){

   const int N_RETRY = 5;
   const int RETRY_DELAY = 1;

   for( int i = 0; i < N_RETRY; i++)
   {
      if( O1App::instance().getStartupStatus() == true)
      {
         return O1::SUCCESS;
      }
      else
      {
         sleep(RETRY_DELAY);
      }
   }

   return O1::FAILURE;
}

/*******************************************************************
 *
 * @brief Starts the O1 Module
 *
 * @details
 *
 *    Function : start_O1_module
 *
 *    Functionality:
 *      - Starts the O1 module by instantiating the O1App instance
 *        Invoked from the ODU-High main function
 *
 *
 * @params[in] void
 * @return O1::SUCCESS - success
 *         O1::FAILURE - failure
 ******************************************************************/

int start_O1_module(void)
{

   if( !ConfigLoader::instance().loadConfigurations() )
      return O1::FAILURE;

   if(O1App::instance().start() == false){
      O1_LOG("\nO1 O1Interface : Failed to start");
      return O1::FAILURE;
   }
   
   if(O1App::instance().setAffinity(O1::CPU_CORE))
   {
      O1_LOG("\nO1 O1Interface : CPU affinity set for O1App thread to" );
      O1App::instance().printAffinity();
   }
   return check_O1_module_status();
}

/*******************************************************************
 *
 * @brief Send VES PNF Registration Request
 *
 * @details
 *
 *    Function : sendPnfRegistration
 *
 *    Functionality:
 *      - Create a thread and send VES PNF Registration Request
 *
 *
 * @params[in] void
 * @return void
 ******************************************************************/

void sendPnfRegistration(void)
{
   PnfRegistrationThread::instance().start();
   if(PnfRegistrationThread::instance().setAffinity(O1::CPU_CORE))
   {
      O1_LOG("\nO1 O1Interface : CPU affinity set for PnfRegistration thread to " );
      PnfRegistrationThread::instance().printAffinity();
   }
}


/**********************************************************************
         End of file
**********************************************************************/
