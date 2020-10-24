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

#include "TcpClient.h"
#include "GlobalDefs.h"
#include "ssi.h"

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>


static int s_sock;
static struct sockaddr_in s_serverName;
static uint16_t s_port;
static const char* s_hostName = NULL;


/********************************************************************** 
   Description : Initilize the sockadd_in structure 
   Params[In]  : None
   Return      : ROK     - success
                 RFAILED - failure
**********************************************************************/
static int initSockaddr()
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


/********************************************************************** 
   Description : Open a TCP socket 
   Params[In]  : hostName, port
   Return      : ROK     - success
                 RFAILED - failure
**********************************************************************/
int openSocket(const char* hostName, const int port)
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

/********************************************************************** 
   Description : Send the message on TCP socket 
   Params[In]  : message, size of the message
   Return      : Number of bytes sent
**********************************************************************/
int sendData(void* data, const int size)
{
   int nbytes = write (s_sock, data, size);
   if (nbytes < 0)
   {
      O1_LOG("\nO1 TcpClient : Error writing. %d bytes sent", nbytes);
   }
   return nbytes;
}

/********************************************************************** 
   Description : Close the TCP socket 
   Params[In]  : None
   Return      : ROK     - success
                 RFAILED - failure
**********************************************************************/
int closeSocket()
{
   if( close(s_sock) != 0 )
      return RFAILED;
   return ROK;
}

/**********************************************************************
         End of file
**********************************************************************/
