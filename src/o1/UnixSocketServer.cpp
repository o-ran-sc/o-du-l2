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

/* This file contains UnixSocketServer class that listens for Netconf Alarm 
   messages on a Unix socket from ODU. It calls the AlarmManager functions
   for raising or clearing the alarms based on the actions received 
*/

#include "UnixSocketServer.hpp"
#include "Alarm.hpp"
#include "AlarmManager.hpp"
#include "CmInterface.h"
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
#include <sys/un.h>
#include "InitConfig.hpp"


using std::map;
using std::pair;

/*******************************************************************
 *
 * @brief Constructor
 *
 * @details
 *
 *    Function : UnixSocketServer
 *
 *    Functionality:
 *      - Constructor intialization
 *
 * @params[in] socket path
 * @return None
 ******************************************************************/
UnixSocketServer::UnixSocketServer(const string& sockPath)
                                   : mSockPath(sockPath),
                                     mIsRunning(false)
{

}  
  
/*******************************************************************
 *
 * @brief Destructor
 *
 * @details
 *
 *    Function : ~UnixSocketServer
 *
 *    Functionality:
 *      - Destructor
 *
 * @params[in] None
 * @return None
 ******************************************************************/
UnixSocketServer::~UnixSocketServer()
{

}


/*******************************************************************
 *
 * @brief Read the data from the connected client application
 *
 * @details
 *
 *    Function : readMessage
 *
 *    Functionality:
 *      - Reads the data from the connected client application
 *
 * @params[in] File descriptor
 * @return No. of bytes read
 *
 ******************************************************************/
int UnixSocketServer::readMessage(int fd)
{
   AlarmRecord *alrmRec = NULL;
   char recvBuf[BUFLEN];
   Alarm alrm;
   bzero(&recvBuf,sizeof(recvBuf));
   
   int nbytes = read (fd, &recvBuf, sizeof(recvBuf));
   
   if (nbytes > 0)
   {
      MsgHeader *msgHdr = (MsgHeader*)recvBuf;

      O1_LOG("\nO1 UnixSocketServer :\nMsgType %d",msgHdr->msgType);
      
      if ( msgHdr->msgType == ALARM ){
         uint16_t alrmId;
         alrmRec = (AlarmRecord*) recvBuf;
         O1_LOG("\nO1 UnixSocketServer :\n"
                   "Action %d\n"
                   "Alarm ID %s\n" 
                   "Severity %d\n" 
                   "Additional Text %s\n"
                   "Specific Problem %s\n"
                   "Additional Info %s\n"
                   "Alarm Raise Time %s\n",
                   alrmRec->msgHeader.action,
                   alrmRec->alarmId,
                   alrmRec->perceivedSeverity,
                   alrmRec->additionalText,
                   alrmRec->specificProblem,
                   alrmRec->additionalInfo,
                   alrmRec->alarmRaiseTime
                );
      
         /*Fill the alarm structure */
         sscanf(alrmRec->alarmId,"%hu",&alrmId);
         alrm.setAlarmId(alrmId);
         alrm.setPerceivedSeverity(alrmRec->perceivedSeverity);
         alrm.setAdditionalText(alrmRec->additionalText);
         alrm.setEventType(alrmRec->eventType);
         alrm.setSpecificProblem(alrmRec->specificProblem);
         alrm.setAdditionalInfo(alrmRec->additionalInfo);
      }

      switch(msgHdr->action)
      {
         case RAISE_ALARM: 

                     if(AlarmManager::instance().raiseAlarm(alrm))
                     {
                        O1_LOG("\nO1 UnixSocketServer : "
                               "Alarm raised for alarm Id %s",
                                alrmRec->alarmId);
                     }

                     else
                     {
                        O1_LOG("\nO1 UnixSocketServer : "
                               "Error in raising alarm for alrm Id %s",
                                alrmRec->alarmId);
                     }
                     break;  
                     
         case CLEAR_ALARM: 
                     if(AlarmManager::instance().clearAlarm(alrm))
                     {
                        O1_LOG("\nO1 UnixSocketServer : "
                               "Alarm cleared for alarm Id %s",
                                alrmRec->alarmId);

                     }
                     else
                     {
                        O1_LOG("\nO1 UnixSocketServer : "
                               "Error in clearing alarm for alarm Id %s",
                                alrmRec->alarmId);
                     }
                     break;
         default:    
                     O1_LOG("\nO1 UnixSocketServer : No action performed"); 
                     break;
      }

   }
   return nbytes;
}


/*******************************************************************
 *
 * @brief Open a Unix socket and bind on the port
 *
 * @details
 *
 *    Function : makeSocket
 *
 *    Functionality:
 *      -  Opens a Unix socket and bind on the port
 *
 * @params[in] void
 * @return O1:SUCCESS - success
 *         O1:FAILURE - failure
 ******************************************************************/

int UnixSocketServer::makeSocket() 
{
   struct sockaddr_un name;
   /* Create the socket. */
   mSock = socket (AF_UNIX, SOCK_STREAM, 0);
   if (mSock < 0)
   {
      O1_LOG("\nO1 UnixSocketServer : Socket error");
      return O1::FAILURE;
   }
   /* Give the socket a name. */
   bzero(&name, sizeof(name));
   name.sun_family = AF_UNIX;

   /* Remove the socket file if it already exists */ 
   if ( unlink(mSockPath.c_str()) == 0)
   {
      O1_LOG("\nO1 UnixSocketServer : "
             "Removing the existing socket path %s",
              mSockPath.c_str());
   }        
   strcpy(name.sun_path, mSockPath.c_str());
   if (bind (mSock, (struct sockaddr *) &name, sizeof (name)) < 0)
   {
      close(mSock);
      O1_LOG("\nO1 UnixSocketServer : Bind error");
      return O1::FAILURE;
   }
   return O1::SUCCESS;
}


/*******************************************************************
 *
 * @brief A Unix server to handle multiple connection
 *
 * @details
 *
 *    Function : run
 *
 *    Functionality:
 *      -  A Unix server to handle multiple connection 
 *         Uses select multiplexing
 *
 * @params[in] void
 * @return true  - success
 *         false - failure
 ******************************************************************/
bool UnixSocketServer::run()
{

   fd_set active_fd_set, read_fd_set;
   int i;
   struct sockaddr_un clientName;
   socklen_t size;
   mIsRunning = true;

   /* Create the socket and set it up to accept connections. */
   if( makeSocket() == O1::SUCCESS )
   {
      if (listen (mSock, 1) < 0)
      {
         O1_LOG("\nO1 UnixSocketServer : Listen error");
         close(mSock);
         mIsRunning = false;
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
               O1_LOG("\nO1 UnixSocketServer : Select error");
               close(mSock);
               mIsRunning = false;
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
                         O1_LOG("\nO1 UnixSocketServer : Accept error");
                         close(mSock);
                         mIsRunning = false;
                         break;
                      }
                      O1_LOG("\nO1 UnixSocketServer : Connected from client\n");
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
      mIsRunning = false;
   }
   return mIsRunning;
}


/*******************************************************************
 *
 * @brief Clean up open socket
 *
 * @details
 *
 *    Function : cleanUp
 *
 *    Functionality:
 *      -  Performs any clean ups before stopping the thread
 *
 * @params[in] void
 * @return void
 ******************************************************************/
void UnixSocketServer::cleanUp(void)
{
   close(mSock);
   O1_LOG("\nO1 UnixSocketServer : Cleaning up Closing socket \n");
}

/*******************************************************************
 *
 * @brief Check if the server is running
 *
 * @details
 *
 *    Function : isRunning
 *
 *    Functionality:
 *      -  Returns the running status of the server
 *
 * @params[in] void
 * @return true : running
 *         false: not running
 ******************************************************************/
bool UnixSocketServer::isRunning() const
{
   return mIsRunning;
}

/**********************************************************************
         End of file
**********************************************************************/
