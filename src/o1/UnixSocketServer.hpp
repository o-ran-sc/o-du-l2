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

/* This file contains UnixSocketServer class that listens for Netconf Alarm messages
   on a Unix socket from ODU. It calls the AlarmManager functions for raising 
   or clearing the alarms based on the actions received  
*/
 
#ifndef __UNIX_SOCKET_SERVER_HPP__
#define __UNIX_SOCKET_SERVER_HPP__
#include <string>
#include <pthread.h>
#include "Thread.hpp"

using std::string;
#define BUFLEN 512

class UnixSocketServer : public Thread
{

   private:
   int mSock;
   string mSockPath;
   int readMessage(int);
   int makeSocket();
   
   protected:
   bool run();
   bool mIsRunning;

   public:
   UnixSocketServer(const string& sockPath);    
   ~UnixSocketServer();
   bool isRunning() const;   
   virtual void cleanUp(void);

};

#endif


/**********************************************************************
         End of file
**********************************************************************/
