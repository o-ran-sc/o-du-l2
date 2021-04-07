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

/* This file contains functions to connect to unix socket server and send/recv
   messages 
*/


#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/un.h>

#include "UnixSocketClient.hpp"
#include "GlobalDefs.hpp"


/*******************************************************************
 *
 * @brief Constructor
 *
 * @details
 *
 *    Function : UnixSocketClient
 *
 *    Functionality:
 *      - Constructor intialization
 *
 * @params[in] socket path
 * @return None
 ******************************************************************/
UnixSocketClient::UnixSocketClient(const string& path) 
                                  : mSockPath(path)
{

}

/*******************************************************************
 *
 * @brief Destructor
 *
 * @details
 *
 *    Function : ~UnixSocketClient
 *
 *    Functionality:
 *      - Destructor
 *
 * @params[in] None
 * @return None
 ******************************************************************/
 UnixSocketClient::~UnixSocketClient()
 {

 }

/*******************************************************************
 *
 * @brief Open a Unix socket
 *
 * @details
 *
 *    Function : openSocket
 *
 *    Functionality:
 *      - Opens a Unix socket
 *
 * @params[in] void 
 * @return O1::SUCCESS - success
 *         O1::FAILURE - failure
 ******************************************************************/
uint8_t UnixSocketClient::openSocket()
{
   /* Create the socket. */
   mSock = socket (AF_UNIX, SOCK_STREAM, 0);
   if (mSock < 0)
   {
      O1_LOG("\nO1 UnixSocketClient : Error opening socket");
      return O1::FAILURE;
   }

   /* Init the sockaddr_un structure */
   mSockName.sun_family = AF_UNIX;
   strcpy(mSockName.sun_path, mSockPath.c_str());

   /* Connect to the server */
   if (0 > connect (mSock,
                   (struct sockaddr *)&mSockName,
                   sizeof (mSockName)))
   {
      O1_LOG("\nO1 UnixSocketClient : Error connecting");
      return O1::FAILURE;
   }

   return O1::SUCCESS;

}
/*******************************************************************
 *
 * @brief Send message over Unix socket
 *
 * @details
 *
 *    Function : sendData
 *
 *    Functionality:
 *      - Sends message over Unix socket
 *
 * @params[in] message, size of the message
 * @return Number of bytes sent
 *
 ******************************************************************/
int UnixSocketClient::sendData(void* data, const int size)
{
   int nbytes = write (mSock, data, size);
   if (nbytes < 0)
   {
      O1_LOG("\nO1 UnixSocketClient : Error writing. %d bytes sent", nbytes);
   }
   return nbytes;
}

/*******************************************************************
 *
 * @brief Recieve message over Unix socket
 *
 * @details
 *
 *    Function : receiveData
 *
 *    Functionality:
 *      - Recieves message over Unix socket
 *
 * @params[in] message, size of the message
 * @return Number of bytes received
 *
 ******************************************************************/
int UnixSocketClient::receiveData(void* data, const int size)
{
   int nbytes = read (mSock, data, size);
   if (nbytes < 0)
   {
      O1_LOG("\nO1 UnixSocketClient : Error reading. %d bytes sent", nbytes);
   }
   return nbytes;
}

/*******************************************************************
 *
 * @brief Close the Unix socket
 *
 * @details
 *
 *    Function : closeSocket
 *
 *    Functionality:
 *      - Closes the Unix socket
 *
 * @params[in] void
 * @return O1::SUCCESS - success
 *         O1::FAILURE - failure
 ******************************************************************/
uint8_t UnixSocketClient::closeSocket()
{
   if( close(mSock) != 0 )
      return O1::FAILURE;
   return O1::SUCCESS;
}

/**********************************************************************
         End of file
**********************************************************************/
