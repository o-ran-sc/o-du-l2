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

/* This file contains SessionHandler class header*/

#ifndef __SESSION_MANAGER_HPP__
#define __SESSION_MANAGER_HPP__

#include "sysrepo-cpp/Session.hpp"
#include "Alarm3GPPYangModel.hpp"
#include "AlarmOranYangModel.hpp"


class SessionHandler
{
   public:
      SessionHandler();
      ~SessionHandler();
      
      bool init();


   private:
      sysrepo::S_Connection mConn;
      sysrepo::S_Session mSess;
      sysrepo::S_Subscribe mSub;
      
      /*creation of Connection, Session, Subscribe obj functions*/
      bool subscribeModule(sysrepo::S_Subscribe sub);
      sysrepo::S_Connection createConnection();
      sysrepo::S_Session createSession(sysrepo::S_Connection conn);
      sysrepo::S_Subscribe createSubscribe(sysrepo::S_Session sess);
};

#endif

/**********************************************************************
         End of file
**********************************************************************/
