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

/* This file contains functions to connect to TCP server and send massages */


#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include "TcpClient.h"
#include "GlobalDefs.h"
#include "ssi.h"

static int s_sock;
static struct sockaddr_in s_serverName;
static uint16_t s_port;
static const char* s_hostName = NULL;


/*******************************************************************
 *
 * @brief Initilize the sockadd_in structure
 *
 * @details
 *
 *    Function : initSockaddr
 *
 *    Functionality:
 *      - Initilizes the sockadd_in structure
 *
 * @params[in] void 
 * @return ROK     - success
 *         RFAILED - failure
 ******************************************************************/
static uint8_t initSockaddr()
{

   struct hostent *hostInfo;
   struct sockaddr_in *name = &s_serverName;
   bzero(&s_serverName, sizeof(s_serverName));
   name->sin_family = AF_INET;
   name->sin_port = htons (s_port);
   hostInfo = gethostbyname (s_hostName);
   if (hostInfo == NULL)
   {
      O1_LOG("\nO1 TcpClient : Unknown host %s", s_hostName);
      return RFAILED;
   }
   name->sin_addr = *(struct in_addr *) hostInfo->h_addr;
   return ROK;
}


/*******************************************************************
 *
 * @brief Open a TCP socket
 *
 * @details
 *
 *    Function : openSocket
 *
 *    Functionality:
 *      - Opens a TCP socket
 *
 * @params[in] hostname, port 
 * @return ROK     - success
 *         RFAILED - failure
 ******************************************************************/
uint8_t openSocket(const char* hostName, const uint16_t port)
{
   /* Create the socket. */
   s_port = port;
   s_hostName = hostName;
   s_sock = socket (PF_INET, SOCK_STREAM, 0);
   if (s_sock < 0)
   {
      O1_LOG("\nO1 TcpClient : Error opening socket");
      return RFAILED;
   }

   /* Init the sockaddr_in structure */
   if (initSockaddr() == RFAILED)
   {
      return RFAILED;
   }
   /* Connect to the server */
   if (0 > connect (s_sock,
                   (struct sockaddr_in *)&s_serverName ,
                   sizeof (s_serverName)))
   {
      O1_LOG("\nO1 TcpClient : Error connecting");
      return RFAILED;
   }

   return ROK;

}

/*******************************************************************
 *
 * @brief Send message over TCP socket
 *
 * @details
 *
 *    Function : sendData
 *
 *    Functionality:
 *      - Sends message over TCP socket
 *
 * @params[in] message, size of the message
 * @return Number of bytes sent
 *
 ******************************************************************/
int sendData(void* data, const int size)
{
   int nbytes = write (s_sock, data, size);
   if (nbytes < 0)
   {
      O1_LOG("\nO1 TcpClient : Error writing. %d bytes sent", nbytes);
   }
   return nbytes;
}

/*******************************************************************
 *
 * @brief Recieve message over TCP socket
 *
 * @details
 *
 *    Function : receiveData
 *
 *    Functionality:
 *      - Recieves message over TCP socket
 *
 * @params[in] message, size of the message
 * @return Number of bytes received
 *
 ******************************************************************/
int receiveData(void* data, const int size)
{
   int nbytes = read (s_sock, data, size);
   if (nbytes < 0)
   {
      O1_LOG("\nO1 TcpClient : Error reading. %d bytes sent", nbytes);
   }
   return nbytes;
}

/*******************************************************************
 *
 * @brief Close the TCP socket
 *
 * @details
 *
 *    Function : closeSocket
 *
 *    Functionality:
 *      - Closes the TCP socket
 *
 * @params[in] message, size of the message
 * @return ROK     - success
 *         RFAILED - failure
 ******************************************************************/
uint8_t closeSocket()
{
   if( close(s_sock) != 0 )
      return RFAILED;
   return ROK;
}

/**********************************************************************
         End of file
**********************************************************************/
