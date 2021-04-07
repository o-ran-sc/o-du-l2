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

/* This class is the netconf manager class. It will handle netopeer-server start and stop along with session handler init. Additionally it will provide the
   signal handling. */

#include "NetconfManager.hpp"

/* Default constructor */
NetconfManager::NetconfManager()
{
}


/* Destructor */
NetconfManager::~NetconfManager()
{
   if( NULL != mSessHndl)
   {
      delete mSessHndl;
   }  
}

/********************************************************************** 
   Description : This function will start the netopeer2-server and 
                 redirect logs to the /etc/netopeer2-server.log file
   Params[In]  : void
   Return      : true  - started successful
                 false - start failed
**********************************************************************/

bool NetconfManager::startNetopeerServer ()
{
    int status = system("netopeer2-server -d -v3 > /etc/netopeer2-server.log 2>&1 &");
    if (status < 0)
    {
       O1_LOG("\nO1 NetconfManager : Error during netopeer server start status : %s",\
               strerror(errno));
    }
    else
    {
       if (WIFEXITED(status))
          O1_LOG("\nO1 NetconfManager : netopeer server started normally with status : %d",\
                  WEXITSTATUS(status));
       else
          O1_LOG("\nO1 NetconfManager : netopeer server started abnormally with status : %d",\
                  WEXITSTATUS(status));
    }
    return (status == 0);
}


/********************************************************************** 
   Description : This function will stop the netopeer2-server 
   Params[In]  : void
   Return      : true  - started successful
                 false - start failed
**********************************************************************/
bool NetconfManager::stopNetopeerServer(void)
{
   int status = system("kill -9 `pidof netopeer2-server`");
   if (status < 0)
   {
      O1_LOG("\nO1 NetconfManager : Error during Netopeer server stopped status : %s\n",\
              strerror(errno));
   }
   else
   {
      if (WIFEXITED(status))
      {
         O1_LOG("\nO1 NetconfManager : Netopeer server stopped normally with status : %d\n",\
                 WEXITSTATUS(status));
      }
      else
      {
         O1_LOG("\nO1  NetconfManager : Netopeer server stopped abnormally with status : %d\n",\
                 WEXITSTATUS(status));
      }
   }
   return (status == 0);
}


/********************************************************************** 
   Description : catch and handle the SIGINT signal 
   Params[In]  : signum
   Return      : void 
**********************************************************************/
void NetconfManager::sigintHandler(int signum)
{
   if (true != NetconfManager::stopNetopeerServer())
   {
      O1_LOG("\nO1 NetconfManager : Error stopping Netopeer server");
   }
}


/********************************************************************** 
   Description : This function will start the netopeer2-server and 
                 session handler  
   Params[In]  : void
   Return      : true  - started successful
                 false - start failed
**********************************************************************/
bool NetconfManager::init(void)
{
  /* if(startNetopeerServer())
   {
      O1_LOG("\nO1 NetconfManager : netopeer server started");
   }
   */
   try
   {
      mSessHndl = new SessionHandler;
      
      O1_LOG("\nO1 NetconfManager : SessionHandler created ");

      if( !mSessHndl->init())
      {
         O1_LOG("\nO1 NetconfManager : SessionHandler init failed \n");         
         return false;
      }
      return true;
   }
   catch( const std::exception& e ) 
   {
      O1_LOG("\nO1 O1_main : Exception : %s", e.what());
      return false;
   }
}


/**********************************************************************
         End of file
**********************************************************************/
