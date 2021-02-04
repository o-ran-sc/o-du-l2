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
#include "Config.h"
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
#include "InitConfig.hpp"

using std::map;
using std::pair;

/* Destructor */
TcpServer::~TcpServer()
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
int TcpServer::readMessage(int fd)
{
   AlarmRecord *alrmRec = NULL;
   char recvBuf[BUFLEN];
   Alarm alrm;
   bzero(&recvBuf,sizeof(recvBuf));
   
   int nbytes = read (fd, &recvBuf, sizeof(recvBuf));
   
   if (nbytes > 0)
   {
      MsgHeader *msgHdr = (MsgHeader*)recvBuf;

      O1_LOG("\nO1 TcpServer :\nBuf size %ld", sizeof(recvBuf));
      O1_LOG("\nO1 TcpServer :\nMsgType %d",msgHdr->msgType);
      O1_LOG("\nO1 TcpServer :\nAction %d",msgHdr->action);
      
      if ( msgHdr->msgType == ALARM ){
         uint16_t alrmId;
         alrmRec = (AlarmRecord*) recvBuf;
         O1_LOG("\nO1 TcpServer :\nAction %d\nalarm ID %s\n%d\n%s\n%d\n%s\n%s\nbytes %d",
                     alrmRec->msgHeader.action,
                     alrmRec->alarmId,
                     alrmRec->perceivedSeverity,
                     alrmRec->additionalText,
                     alrmRec->eventType,
                     alrmRec->specificProblem,
                     alrmRec->additionalInfo,
                     nbytes
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
                        O1_LOG("\nO1 TcpServer : Alarm raised for alarm Id %s", alrmRec->alarmId);
                     }
                     else
                     {
                        O1_LOG("\nO1 TcpServer : Error in raising alarm for alrm Id %s", alrmRec->alarmId);
                     }
                     break;  
         case CLEAR_ALARM: 
                     if(AlarmManager::instance().clearAlarm(alrm))
                     {
                        O1_LOG("\nO1 TcpServer : Alarm cleared for alarm Id %s", alrmRec->alarmId);
                     }
                     else
                     {
                        O1_LOG("\nO1 TcpServer : Error in clearing alarm for alarm Id %s", alrmRec->alarmId);
                     }
                     break;
         case GET_STARTUP_CONFIG:
                     {
                        StartupConfig cfg;
			               InitConfig::instance().getCurrInterfaceConfig(cfg);
                        O1_LOG("\nO1 TcpServer : cfg.DU_IPV4_Addr [%s]", cfg.DU_IPV4_Addr);
                        O1_LOG("\nO1 TcpServer : cfg.DU_Port [%d]", cfg.DU_Port);
                        O1_LOG("\nO1 TcpServer : cfg.CU_IPV4_Addr [%s]", cfg.CU_IPV4_Addr);
                        O1_LOG("\nO1 TcpServer : cfg.CU_Port [%d]", cfg.CU_Port);
                        O1_LOG("\nO1 TcpServer : cfg.RIC_IPV4_Addr [%s]", cfg.RIC_IPV4_Addr);
                        O1_LOG("\nO1 TcpServer : cfg.RIC_Port [%d]", cfg.RIC_Port);
                        if (write (fd, &cfg, sizeof(cfg)) < 0)
                        {
                           O1_LOG("\nO1 TcpServer : Error sending startup configuration \n");
                        }
                        break; 
                     }
         default:    
                     O1_LOG("\nO1 TcpServer : No action performed"); 
                     break;
      }

   }
   return nbytes;
}


/*******************************************************************
 *
 * @brief Open a TCP socket and bind on the port
 *
 * @details
 *
 *    Function : makeSocket
 *
 *    Functionality:
 *      -  Opens a TCP socket and bind on the port
 *
 * @params[in] void
 * @return O1:SUCCESS - success
 *         O1:FAILURE - failure
 ******************************************************************/
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


/*******************************************************************
 *
 * @brief Start TCP server in thread
 *
 * @details
 *
 *    Function : start
 *
 *    Functionality:
 *      -  Start TCP server in thread
 *
 * @params[in] void
 * @return true  - success
 *         false - failure
 ******************************************************************/
bool TcpServer::start()
{
   return (pthread_create(&mThreadId, NULL, task, this) == 0);
}

/*******************************************************************
 *
 * @brief A TCP server to handle multiple connection
 *
 * @details
 *
 *    Function : run
 *
 *    Functionality:
 *      -  A TCP server to handle multiple connection 
 *         Uses select multiplexing
 *
 * @params[in] void
 * @return true  - success
 *         false - failure
 ******************************************************************/
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


/*******************************************************************
 *
 * @brief Static function for launching a TCP server instance
 *        in a thread
 *
 * @details
 *
 *    Function : task
 *
 *    Functionality:
 *      - Static function for launching a TCP server instance
 *        in a thread
 *
 * @params[in] TcpServer instance
 * @return void
 ******************************************************************/
void* TcpServer::task(void *args)
{
   TcpServer *tcpServer = (TcpServer*)args;
   tcpServer->run();
   return NULL;
}


/*******************************************************************
 *
 * @brief Wait for the thread to complete in the parent process
 *
 * @details
 *
 *    Function : wait
 *
 *    Functionality:
 *      - Waits for the thread to complete in the parent process
 *
 * @params[in] void
 * @return true   : success
 *          false : failure
 ******************************************************************/
bool TcpServer::wait()
{
    return (pthread_join(mThreadId,NULL) == 0);
}



/**********************************************************************
         End of file
**********************************************************************/
