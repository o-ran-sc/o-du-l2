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

/* This file contains defination of generic netconf utility function which can
 *  be used by any class*/

#include <sstream>
#include "NetconfUtils.hpp"

/*******************************************************************
 *
 * @brief print changes of given operation
 *
 * @details
 *
 *    Function : printChange
 *
 *    Functionality:
 *      -  print changes of given operation, old and new value
 *
 *
 * @params[in] sysrepo::S_Change change
 * @return void
 ******************************************************************/

void NetconfUtils::printChange(sysrepo::S_Change change) {
   switch(change->oper()) {
      case SR_OP_CREATED:
         if (nullptr != change->new_val()) {
            O1_LOG("\nO1 NetconfUtils : CREATED: %s", \
                    change->new_val()->to_string().c_str());
         }
         break;
      case SR_OP_DELETED:
         if (nullptr != change->old_val()) {
             O1_LOG("\nO1 NetconfUtils : DELETED:  %s", \
                     change->old_val()->to_string().c_str());
         }
         break;
      case SR_OP_MODIFIED:
         if (nullptr != change->old_val() && nullptr != change->new_val()) {
            O1_LOG("\nO1 NetconfUtils : MODIFIED: old value %s :new value %s", \
                    change->old_val()->to_string().c_str(), \
                    change->new_val()->to_string().c_str());
        }
        break;
     case SR_OP_MOVED:
        if (nullptr != change->old_val() && nullptr != change->new_val()) {
           O1_LOG("\nO1 NetconfUtils : MOVED: %s :after %s ", \
                    change->new_val()->xpath(), \
                    change->old_val()->xpath());
        }
        else if (nullptr != change->new_val()) {
           O1_LOG("\nO1 NetconfUtils : MOVED: %s : first", \
                   change->new_val()->xpath());
        }
        break;
    }
}

/*******************************************************************
 *
 * @brief convert event type to string
 *
 * @details
 *
 *    Function : evToStr
 *
 *    Functionality:
 *      - convert event type to string
 *
 *
 * @params[in] sr_event_t event
 * @return event name in string form
 ******************************************************************/

/* Helper function for printing events. */
const char *NetconfUtils::evToStr(sr_event_t ev) {
   switch (ev) {
      case SR_EV_CHANGE:
         return "change";
      case SR_EV_DONE:
         return "done";
      case SR_EV_ABORT:
      default:
         return "abort";
   }
}

/*******************************************************************
 *
 * @brief get the leaf name from xpath
 *
 * @details
 *
 *    Function : getLeafInfo
 *
 *    Functionality:
 *      - extract the leaf name from xpath
 *
 *
 * @params[in] xpath, output string
 * @return void
 ******************************************************************/

void NetconfUtils::getLeafInfo(string xpath,string &parent, string &leaf ){
   std::stringstream ssLeaf(xpath);
   std::stringstream ssParent(xpath);
   char ch='/';
   getline(ssLeaf, leaf, ch);
   while(getline(ssLeaf, leaf, ch)){
       getline(ssParent, parent, ch);
   } //get leaf name after the last '/'
   std::stringstream ss(leaf);
   ch=' ';
   getline(ss, leaf, ch); //remove extra space
   //O1_LOG("\nO1 NetconfUtils : parent = [%s]", parent.c_str());
   //O1_LOG("\nO1 NetconfUtils : leaf = [%s]", leaf.c_str());

}

/**********************************************************************
         End of file
**********************************************************************/
