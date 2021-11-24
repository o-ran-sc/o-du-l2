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

/* This file contains RRM Policy get and  update handler . It handles
   get and change callback for rrmpolicy yang module  */


#ifndef __RRM_POLICY_CB_HPP__
#define __RRM_POLICY_CB_HPP__

#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "sysrepo-cpp/Session.hpp"
#include "CmInterface.h"
#include "GlobalDefs.hpp"
#include "Singleton.hpp"
#include "NetconfUtils.hpp"

class RrmPolicyCb : public sysrepo::Callback
{
   public:
      int oper_get_items(sysrepo::S_Session session,\
                         const char *module_name,\
                         const char *path,\
                         const char *request_xpath,\
                         uint32_t request_id,\
                         libyang::S_Data_Node &parent,\
                         void *private_data); //override
      int module_change(sysrepo::S_Session sess, \
                         const char *module_name, \
                         const char *xpath, \
                         sr_event_t event, \
                         uint32_t request_id, \
                         void *private_data); //override

   private:
      bool updateParams(string &parent, string &leaf, string &val);
      RrmResourceType resourceTypeToEnum(string &val);
};

#endif

/**********************************************************************
         End of file
**********************************************************************/
