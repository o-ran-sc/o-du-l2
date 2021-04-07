/*******************************************************************************
################################################################################
#   Copyright (c) [2020-2021] [HCL Technologies Ltd.]                          #
#                                                                              #
#   Licensed under th`e Apache License, Version 2.0 (the "License");            #
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

/* This file contains functions to connect to a unix socket server and send/recv
   messages 
*/

#ifndef __UNIX_SOCKET_CLIENT_HPP__
#define __UNIX_SOCKET_CLIENT_HPP__

#include <stdint.h>
#include <string>
#include <sys/un.h>

using std::string;

class UnixSocketClient
{
   private:

   int mSock;
   string mSockPath;
   struct sockaddr_un mSockName;
   
   public:
   UnixSocketClient(const string& path);
   ~UnixSocketClient();
   uint8_t openSocket();
   int sendData(void*, const int);
   int receiveData(void* data, const int size);
   uint8_t closeSocket();

};

#endif

/**********************************************************************
         End of file
**********************************************************************/
