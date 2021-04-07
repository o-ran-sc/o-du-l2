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

/* This file contains the O1App class which is responsible for running/starting
   all the O1 modules in a thread. It inherits the Thread class and Singleton
   class.
*/

#ifndef __O1_APP_HPP__
#define __O1_APP_HPP__

#include "Singleton.hpp"
#include "Thread.hpp"
#include "UnixSocketServer.hpp"


class O1App : public Singleton<O1App>, public Thread 
{
   friend Singleton<O1App>;
   
   private:
   bool mStartupStatus;
   UnixSocketServer mUxSocketServer;

   protected:
   bool run();

   public:
   O1App();
   ~O1App();
   bool getStartupStatus()const;
   void cleanUp(void);
};


#endif
