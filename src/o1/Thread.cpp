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

#include "Thread.hpp"
#include "GlobalDefs.hpp"
#include <stdio.h>
#include <unistd.h>

/*******************************************************************
 *
 * @brief Constructor
 *
 * @details
 *
 *    Function : Thread
 *
 *    Functionality:
 *      - Constructor intialization
 *
 * @params[in] None
 * @return None
 ******************************************************************/

Thread::Thread(): mThreadId(0)
{

}

/*******************************************************************
 *
 * @brief Destructor
 *
 * @details
 *
 *    Function : ~Thread
 *
 *    Functionality:
 *      - Destructor
 *
 * @params[in] None
 * @return None
 ******************************************************************/

Thread::~Thread() 
{
   
}

/*******************************************************************
 *
 * @brief Static function registered to run as pthread

 *
 * @details
 *
 *    Function : task
 *
 *    Functionality:
 *      - Static function registered to run as pthread. It calls
 *        the run function that is implemented by the derived
 *        class instance.
 *
 * @params[in] void pointer to the base class instance that 
               implements this Thread class
 * @return void
 ******************************************************************/

void* Thread::task(void* args)
{
   Thread* thisPtr = static_cast<Thread*>(args);
   thisPtr->run();
}


/*******************************************************************
 *
 * @brief Creates a thread
 *
 * @details
 *
 *    Function : start
 *
 *    Functionality:
 *      - Starts a pthread registering the "task" function for
 *        performing the thread task.
 *
 * @params[in] void
 * @return true  : success
 *         false : failure
 ******************************************************************/

bool Thread::start()
{
   return (pthread_create(&mThreadId, NULL, task, this) == 0);
}


/*******************************************************************
 *
 * @brief Stops the thread
 *
 * @details
 *
 *    Function : stop
 *
 *    Functionality:
 *      - Stops the thread. Performs an clean ups prior to
 *        stopping the thread
 *
 *
 * @params[in] void
 * @return true  : success
 *         false : failure
 ******************************************************************/
bool Thread::stop()
{
   cleanUp();
   return (pthread_cancel(mThreadId) == 0);
}

/*******************************************************************
 *
 * @brief  Wait for the thread to complete 
 *
 * @details
 *
 *    Function : join
 *
 *    Functionality:
 *      - Waits for the thread to complete in the calling process/
 *        thread
 *
 * @params[in] void
 * @return true  : success
 *         false : failure
 ******************************************************************/
bool Thread::join()
{
   return (pthread_join(mThreadId,NULL) == 0);
}


/*******************************************************************
 *
 * @brief  Set the affinity of the thread
 *
 * @details
 *
 *    Function : setAffinity
 *
 *    Functionality:
 *      - Pins the thread to cpu core(s)
 *
 * @params[in] CPU cores
 * @return true  : success
 *         false : failure
 ******************************************************************/
bool Thread::setAffinity(int coreNum)
{
   int ret;
   cpu_set_t cpuset;
   long nCores = sysconf(_SC_NPROCESSORS_ONLN);

   CPU_ZERO(&cpuset);
   CPU_SET(coreNum%nCores, &cpuset);
   ret = pthread_setaffinity_np(mThreadId, sizeof(cpu_set_t), &cpuset);
   if (ret != 0)
   {
       return false;
   }
   return true;  
}

/*******************************************************************
 *
 * @brief  Print the affinity of the thread
 *
 * @details
 *
 *    Function : printAffinity
 *
 *    Functionality:
 *      - Prints the cpu core(s) the thread is pinned to
 *
 * @params[in] void
 * @return true  : success
 *         false : failure
 ******************************************************************/
bool Thread::printAffinity()
{
   int ret;
   long nCores = sysconf(_SC_NPROCESSORS_ONLN);

   cpu_set_t cpuset;
   CPU_ZERO(&cpuset);

   ret = pthread_getaffinity_np(mThreadId, sizeof(cpu_set_t), &cpuset);
   if (ret != 0)
       return false;

   for (int i = 0; i < nCores; i++)
       if (CPU_ISSET(i, &cpuset))
           O1_LOG("CPU %d ", i);
   return true;
}

/**********************************************************************
End of file
**********************************************************************/