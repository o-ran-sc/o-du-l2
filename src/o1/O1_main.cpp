/*******************************************************************************
################################################################################
#   Copyright (c) [2020] [HCL Technologies Ltd.]                               #
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

/* This file contains O1 main. 
   Starts the Netopeer and TCP server
*/

#include "NetconfManager.hpp"
#include "TcpServer.hpp"
#include <signal.h>

/********************************************************************** 
   Description : Main function. Start of O1 module.
   Params[In]  : None 
   Return      : EXIT_SUCCESS 
               : EXIT_FAILURE
**********************************************************************/

int main(int argc, char **argv)
{
   TcpServer tcpServer(O1::TCP_PORT);
   /*SIGINT handling*/
   //signal(SIGINT,  NetconfManager::sigintHandler);
   /* Start Netconf server and subscribe to yang modules */
   try
   {
      NetconfManager::instancePtr()->init();
      O1_LOG("\nO1 O1_main : NetconfManager init successful");

   }
   catch( const std::exception& e ) 
   {
      O1_LOG("\nO1 O1_main : Exception : %s", e.what());
      return EXIT_FAILURE;
   }
   /* Start the TCP Server to listen for alarm messages */
   if( tcpServer.start() )
   {
      O1_LOG("\nO1 O1_main : TCP server started\n");
      /* Wait for the TcpServer thread to end*/
      tcpServer.wait();
   }
   else
   {
      O1_LOG("\nO1 O1_main : Failed to start TCP server");
      return EXIT_FAILURE;
   }
   return EXIT_SUCCESS;
}

/**********************************************************************
         End of file
**********************************************************************/
