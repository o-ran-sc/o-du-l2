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

/* This class is the netconf manager class. It creates instance of the 
   SessionHandler that provides connections/sessions related funtionality
   and signal handling. 
*/

#ifndef __NETCONF_MANAGER_HPP__
#define __NETCONF_MANAGER_HPP__

#include "SessionHandler.hpp"
#include "Singleton.hpp"

class NetconfManager : public Singleton<NetconfManager>
{

   friend Singleton<NetconfManager>;

   public:
      /*initialize variables*/
      bool init();

      /*handler for netopeer server*/
      bool startNetopeerServer(void);
      static bool stopNetopeerServer(void);
      static void sigintHandler(int signum);

      /*getter setter for  SessionHandler obj*/
      bool setSessionHandler (SessionHandler  sessMgr);
      SessionHandler  getSessionHandler(void);


   protected:
      NetconfManager();
      ~NetconfManager();

   private:
      SessionHandler *mSessHndl;
};


#endif

/**********************************************************************
         End of file
**********************************************************************/
