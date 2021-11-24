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

/* This file contains the O1App class which is responsible for running/starting
   all the O1 modules in a thread. It inherits the Thread class and Singleton
   class.
*/

#include "O1App.hpp"
#include "GlobalDefs.hpp"
#include "SessionHandler.hpp"
#include "CmInterface.h"
#include <unistd.h>

#include "VesUtils.hpp"
#include "VesEventHandler.hpp"


/*******************************************************************
 *
 * @brief Constructor
 *
 * @details
 *
 *    Function : O1App
 *
 *    Functionality:
 *      - Constructor intialization
 *
 * @params[in] None
 * @return None
 ******************************************************************/

O1App::O1App() : mUxSocketServer(O1::ALARM_SOCK_PATH)
{

}

/*******************************************************************
 *
 * @brief Destructor
 *
 * @details
 *
 *    Function : ~O1App
 *
 *    Functionality:
 *      - Destructor
 *
 * @params[in] None
 * @return None
 ******************************************************************/

O1App::~O1App()
{

}

/*******************************************************************
 *
 * @brief Runs the O1 modules as a thread 
 *
 * @details
 *
 *    Function : run
 *
 *    Functionality:
 *      - Implements the "run" function of Thread class.
 *        Starts all the O1 modules.
 *
 *
 * @params[in] void
 * @return true  : success
 *         false : failure
 ******************************************************************/

bool O1App::run()
{
   const int SLEEP_INTERVAL = 2;
   const int DEFAUL_CELL_ID = 1;
   SessionHandler sessHdlr;

   /*setting default cell state disabled*/
   setCellOpState(DEFAUL_CELL_ID, DISABLED, INACTIVE);

   /* Start Netconf session and subscribe to yang modules */
   try
   {
      if( !sessHdlr.init() )
      {
         O1_LOG("\nO1 O1App : SessionHandler initialization failed ");         
         return false;
      }
   }
   catch( const std::exception& e ) 
   {
      O1_LOG("\nO1 O1App : Exception : %s", e.what());
      return false;
   }
   
   /* Start the Unix Socket Server to listen for alarm messages */
   if( mUxSocketServer.start() )
   {  
      
      if(mUxSocketServer.setAffinity(O1::CPU_CORE))
      {
         O1_LOG("\nO1 O1App : CPU affinity set for UnixSocketServer thread to " );
         mUxSocketServer.printAffinity();
      }
      
      sleep(SLEEP_INTERVAL);
      if( mUxSocketServer.isRunning() )  
      {
         mStartupStatus = true;
         O1_LOG("\nO1 O1App : Unix Socket server started");
      }
      else
      {
         O1_LOG("\nO1 O1App : Unix Socket server failed to start");
         return false;
      }
      /* Wait for the Unix Socket Server thread to end*/
      mUxSocketServer.join();
   }
   else
   {
      O1_LOG("\nO1 O1App : Unix Socket server failed to start");
      return false;
   }
   return true;
}

/*******************************************************************
 *
 * @brief Check if the O1 Module is fully up and running
 *
 * @details
 *
 *    Function : getStartupStatus
 *
 *    Functionality:
 *      - Returns the status of O1App whether it is fully up
 *
 *
 * @params[in] void
 * @return true  : started
 *         false : not started
 ******************************************************************/

bool O1App::getStartupStatus() const
{
   return mStartupStatus;
}

/*******************************************************************
 *
 * @brief Cleanup O1App
 *
 * @details
 *
 *    Function : cleanUp
 *
 *    Functionality:
 *      - Implements the "cleanUp" function of Thread class to
 *        take care of any clean up required for O1 components 
 *        before stopping the thread.
 *
 * @params[in] void
 * @return void
 ******************************************************************/

void O1App::cleanUp(void)
{
   /* Stop the socket server thread */
   mUxSocketServer.stop();   
}
