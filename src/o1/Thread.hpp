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

/* This file contains class for launching and managing POSIX threads */

#ifndef __THREAD_HPP__
#define __THREAD_HPP__

#include <pthread.h>

class Thread
{
   private:
   pthread_t mThreadId;
   static void* task(void*);

   protected:
   virtual bool run() = 0;

   public:
   Thread();
   virtual ~Thread();
   bool start();
   bool stop();
   virtual void cleanUp()=0;
   bool setAffinity(int);
   bool printAffinity();
   bool join();
};

#endif

/**********************************************************************
End of file
**********************************************************************/