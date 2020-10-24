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

#include "TcpServer.hpp"
#include "Alarm.hpp"
#include "AlarmManager.hpp"
#include "GlobalDefs.hpp"
#include <iostream>
#include <cstdio>
#include <cerrno>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

using std::map;
using std::pair;

/* Default constructor */
TcpServer::~TcpServer()
{

}


/********************************************************************** 
   Description : Read the data from the connected client application 
   Params[In]  : fd - File descriptor
   Return      : int - No of bytes read 
**********************************************************************/
int TcpServer::readMessage(int fd)
{
   AlarmRecord alrmRec;
   bzero(&alrmRec,sizeof(alrmRec));
   int nbytes = read (fd, &alrmRec, sizeof(alrmRec));
   if (nbytes > 0)
   {
      Alarm alrm;
      int alrmId;
      O1_LOG("\nO1 TcpServer :\nAction %d\nalarm ID %s\n%d\n%s\n%d\n%s\n%s\nbytes %d",
                     alrmRec.msgHeader.action,
                     alrmRec.alarmId,
                     alrmRec.perceivedSeverity,
                     alrmRec.additionalText,
                     alrmRec.eventType,
                     alrmRec.specificProblem,
                     alrmRec.additionalInfo,
                     nbytes
                     );
      /*Fill the alarm structure */
      sscanf(alrmRec.alarmId,"%d",&alrmId);
      alrm.setAlarmId(alrmId);
      alrm.setPerceivedSeverity(alrmRec.perceivedSeverity);
      alrm.setAdditionalText(alrmRec.additionalText);
      alrm.setEventType(alrmRec.eventType);
      alrm.setSpecificProblem(alrmRec.specificProblem);
      alrm.setAdditionalInfo(alrmRec.additionalInfo);

      switch(alrmRec.msgHeader.action)
      {
         case RAISE: 
                     if(AlarmManager::instance().raiseAlarm(alrm))
                     {
                        O1_LOG("\nO1 TcpServer : Alarm raised");
                     }
                     else
                     {
                        O1_LOG("\nO1 TcpServer : Error in raising alarm");
                     }
                     break;  
         case CLEAR: 
                     if(AlarmManager::instance().clearAlarm(alrm))
                     {
                        O1_LOG("\nO1 TcpServer : Alarm cleared");
                     }
                     else
                     {
                        O1_LOG("\nO1 TcpServer : Error in clearing alarm");
                     }
                     break;
         default:    
                     O1_LOG("\nO1 TcpServer : No action performed"); 
                     break;
      }

   }
   return nbytes;
}


/********************************************************************** 
   Description : Open a TCP socket and bind on the port
   Params[In]  : None
   Return      : O1::SUCCESS - socket open and bind successful
                 O1::FAILURE - socket open and bind failed
**********************************************************************/
int TcpServer::makeSocket() 
{
   struct sockaddr_in name;
   /* Create the socket. */
   mSock = socket (PF_INET, SOCK_STREAM, 0);
   if (mSock < 0)
   {
      O1_LOG("\nO1 TcpServer : Socket error");
      return O1::FAILURE;
   }
   /* Give the socket a name. */
   bzero(&name, sizeof(name));
   name.sin_family = AF_INET;
   name.sin_port = htons (mPort);
   name.sin_addr.s_addr = htonl (INADDR_ANY);
   if (bind (mSock, (struct sockaddr *) &name, sizeof (name)) < 0)
   {
      close(mSock);
      O1_LOG("\nO1 TcpServer : Bind error");
      return O1::FAILURE;
   }
   return O1::SUCCESS;
}


/********************************************************************** 
   Description : Start TCP server in thread    
   Params[In]  : None
   Return      : true  - task launched in pthread successfully
                 false - task failed to launch
**********************************************************************/
bool TcpServer::start()
{
   return (pthread_create(&mThreadId, NULL, task, this) == 0);
}

/********************************************************************** 
   Description : A TCP server to handle multiple connection using
                 select multiplexing    
   Params[In]  : None
   Return      : true  - task launched in pthread successfully
                 false - task failed to launch
**********************************************************************/
bool TcpServer::run()
{

   fd_set active_fd_set, read_fd_set;
   int i;
   struct sockaddr_in clientName;
   socklen_t size;
   bool ret = true;;

   /* Create the socket and set it up to accept connections. */
   if( makeSocket() == O1::SUCCESS )
   {
      if (listen (mSock, 1) < 0)
      {
         O1_LOG("\nO1 TcpServer : Listen error");
         close(mSock);
         ret = false;
      }
      else
      {
         /* Initialize the set of active sockets. */
         FD_ZERO (&active_fd_set);
         FD_SET (mSock, &active_fd_set);

         while (1)
         {
            /* Block until input arrives on one or more active sockets. */
            read_fd_set = active_fd_set;
            if (select (FD_SETSIZE, &read_fd_set, NULL, NULL, NULL) < 0)
            {
               O1_LOG("\nO1 TcpServer : Select error");
               close(mSock);
               ret = false;
               break;
            }

            /* Service all the sockets with input pending. */
            for (i = 0; i < FD_SETSIZE; ++i)
            {
                if (FD_ISSET (i, &read_fd_set))
                {
                   if (i == mSock)
                   {
                      /* Connection request on original socket. */
                      int newFd;
                      bzero(&clientName, sizeof(clientName));
                      size = sizeof (clientName);
                      newFd = accept(mSock,(struct sockaddr *) &clientName,&size);
                      if (newFd < 0)
                      {
                         O1_LOG("\nO1 TcpServer : Accept error");
                         close(mSock);
                         ret = false;
                         break;
                      }
                      O1_LOG("\nO1 TcpServer : Connected from host %s, port %hd.\n",
      		          inet_ntoa (clientName.sin_addr),
      		          ntohs (clientName.sin_port));
                      FD_SET (newFd, &active_fd_set);
                   }      
                   else
                   {
                      /* Data arriving on an already-connected socket. */
                      if (readMessage(i) < 0)
                      {
                         close (i);
                         FD_CLR (i, &active_fd_set);
                      }
                   }
                }    
             }/* for loop ends */     
          } /* while(1) ends */
      } /* else ends */
   } /* outer if ends */
   else
   {
      ret = false;
   }
   return ret;
}


/********************************************************************** 
   Description : Static function for launching a TCP server instance 
                 in a thread    
   Params[In]  : TcpServer instance
   Return      : NULL
**********************************************************************/
void* TcpServer::task(void *args)
{
   TcpServer *tcpServer = (TcpServer*)args;
   tcpServer->run();
   return NULL;
}


/********************************************************************** 
   Description : Wait for the thread to complete in the parent process   
   Params[In]  : None
   Return      : true  - pthread join success
                 false - pthread join failed
**********************************************************************/
bool TcpServer::wait()
{
    return (pthread_join(mThreadId,NULL) == 0);
}



/**********************************************************************
         End of file
**********************************************************************/
