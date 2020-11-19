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

/* This file contains Alarm Yang model data filling and sending related methods */

#ifndef __ALARM_ORAN_YANG_MODEL_HPP__
#define __ALARM_ORAN_YANG_MODEL_HPP__
#include <string.h>
#include <stdlib.h>
#include "sysrepo-cpp/Session.hpp"
#include "AlarmManager.hpp"
#include "GlobalDefs.hpp"


class AlarmOranYangModel : public sysrepo::Callback {
   public:
      int oper_get_items(sysrepo::S_Session session,\
                         const char *module_name,\
                         const char *path,\
                         const char *request_xpath,\
                         uint32_t request_id,\
                         libyang::S_Data_Node &parent,\
                         void *private_data);

};

#endif

/**********************************************************************
         End of file
**********************************************************************/
