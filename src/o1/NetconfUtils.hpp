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

/* This file contains generic netconf utility function which can be used by any
 * class*/


#ifndef __NETCONF_UTILS_HPP__
#define __NETCONF_UTILS_HPP__

#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "GlobalDefs.hpp"
#include "sysrepo-cpp/Session.hpp"

class NetconfUtils
{
   public:
      static void printChange(sysrepo::S_Change change);
      static const char *evToStr(sr_event_t ev);
      static void getLeafInfo(string xpath,string &parent, string &leaf );
};

#endif

/**********************************************************************
         End of file
**********************************************************************/
