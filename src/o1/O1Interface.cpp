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

/* This file contains the interfaces for ODU-High to interact with the 
   O1 module  
*/

#include "O1Interface.h"
#include "O1App.hpp"
#include "GlobalDefs.hpp"
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
   for( int i = 0; i < 5 ; i++)
   {
      if( O1App::instance().getStartupStatus() == true)
      {
         return O1::SUCCESS;
      }
      else
      {
         sleep(1);
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

   if(O1App::instance().start() == false){
         O1_LOG("\nO1 O1Interface : Start failed");
      return O1::FAILURE;
   }
   
   /* TO DO 
   if(O1App::instance().setAffinity(0))
   {
      O1_LOG("\nO1 O1Interface : CPU affinity set" );
      O1App::instance().getAffinity();
   }
   */
   return check_O1_module_status();
}


/**********************************************************************
         End of file
**********************************************************************/
