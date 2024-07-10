/*******************************************************************************
################################################################################
#   Copyright (c) [2017-2019] [Radisys]                                        #
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

#include <stdio.h>
#include <time.h> 
#include <sys/time.h> 

char* getFormattedTime(void) 
{
   struct timeval tp;
   gettimeofday(&tp, 0);

   time_t rawtime;
   struct tm* timeinfo;

   rawtime = tp.tv_sec;
   timeinfo = localtime(&rawtime);


   static char _retval[30];
   snprintf(_retval, sizeof(_retval), "[%02d:%02d:%02d:%03d]",  timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, tp.tv_usec/1000);

   return _retval;
}
