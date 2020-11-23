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

/* This file contains TcpServer class that listens for Netconf Alarm messages
   on a TCP socket from ODU. It calls the AlarmManager functions for raising 
   or clearing the alarms based on the actions received  
*/
 
#ifndef __TCP_SERVER_HPP__
#define __TCP_SERVER_HPP__
#include <string>
#include <pthread.h>

using std::string;

class TcpServer
{

   private:
   pthread_t mThreadId;
   int mSock;
   const int16_t mPort;
   int readMessage(int);
   int makeSocket();
   bool run();
   static void* task(void*);

   public:
   bool start();
   bool wait();
   TcpServer(const uint16_t port) : mPort(port){};    
   ~TcpServer();

};

#endif


/**********************************************************************
         End of file
**********************************************************************/
