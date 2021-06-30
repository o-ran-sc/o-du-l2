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

/* This file contains Cell state update handler for CLA use case. It handles
   get and change callback for o-ran-sc-du-hello-world yang module  */

#include <sstream>
#include "NrCellCb.hpp"
#define MAX_LEN 100

using namespace std;

/*******************************************************************
 *
 * @brief override oper_get_items to handle callback
 *
 * @details
 *
 *    Function : oper_get_items
 *
 *    Functionality:
 *      - override oper_get_items to handle callback of get of
 *        o-ran-sc-du-hello-world yang module
 *
 *
 * @params[in] sysrepo::S_Session session, const char *module_name,
 *             const char *path, const char *request_xpath,
 *             uint32_t request_id, libyang::S_Data_Node &parent,
 *             void *private_data
 * @return SR_ERR_OK  - success
 *         SR_ERR_OK  - failure
 ******************************************************************/

int NrCellCb::oper_get_items(sysrepo::S_Session session, \
                                       const char *module_name, \
                                       const char *path, \
                                       const char *request_xpath, \
                                       uint32_t request_id, \
                                       libyang::S_Data_Node &parent, \
                                       void *private_data)
{
   O1_LOG("\nO1 NrCellCb : Callback called for path=%s on get request", path);
   libyang::S_Context ctx = session->get_context();
   libyang::S_Module mod = ctx->get_module(module_name);

   //first create root of the tree then add nodes and leaves and fill data
   parent.reset(new libyang::Data_Node(ctx, CELL_STATE_MODULE_PATH, \
                nullptr, LYD_ANYDATA_CONSTSTRING, 0));
   libyang::S_Data_Node connection(new libyang::Data_Node(parent, mod, \
                                   "du-to-ru-connection"));

   libyang::S_Data_Node name;
   libyang::S_Data_Node operational_state;
   libyang::S_Data_Node cell_state;
   libyang::S_Data_Node administrative_state;

   NrCellList & cellList  = NrCellList::instance();
   NrCellInfo cellInfo;
   const NrCellList::CellOpStateMap & cellOpStateMap = cellList.getCellOpStateList();
   //read the data from CellOpStateMap
   std::map<uint16_t, NrCellInfo>::const_iterator it;
   for(it = cellOpStateMap.begin(); it !=cellOpStateMap.end(); it++)
   {
      O1_LOG("\nO1 NrCellCb : cellId = %d, opState=%d, cellState=%d", \
              it->first, (int) it->second.getOpState(), (int) it->second.getCellState());
      name.reset(new libyang::Data_Node(connection, mod, "name", \
                 to_string(it->first).c_str()));
      operational_state.reset(new libyang::Data_Node(connection, mod, \
                        "operational-state", \
                        cellInfo.enumToOperationalStateString(it->second.getOpState()).c_str()));
      cell_state.reset(new libyang::Data_Node(connection, mod, "cell-state", \
                       cellInfo.enumToCellStateString(it->second.getCellState()).c_str()));
   }
   return SR_ERR_OK;
}

/*******************************************************************
 *
 * @brief print changes of given operation
 *
 * @details
 *
 *    Function : oper_get_items
 *
 *    Functionality:
 *      -  print changes of given operation, old and new value
 *
 *
 * @params[in] sysrepo::S_Change change
 * @return void
 ******************************************************************/

void NrCellCb::printChange(sysrepo::S_Change change) {
   switch(change->oper()) {
      case SR_OP_CREATED:
         if (nullptr != change->new_val()) {
            O1_LOG("\nO1 NrCellCb : CREATED: %s", \
                    change->new_val()->to_string().c_str());
         }
         break;
      case SR_OP_DELETED:
         if (nullptr != change->old_val()) {
             O1_LOG("\nO1 NrCellCb : DELETED:  %s", \
                     change->old_val()->to_string().c_str());
         }
         break;
      case SR_OP_MODIFIED:
         if (nullptr != change->old_val() && nullptr != change->new_val()) {
            O1_LOG("\nO1 NrCellCb : MODIFIED: old value %s :new value %s", \
                    change->old_val()->to_string().c_str(), \
                    change->new_val()->to_string().c_str());
        }
        break;
     case SR_OP_MOVED:
        if (nullptr != change->old_val() && nullptr != change->new_val()) {
           O1_LOG("\nO1 NrCellCb : MOVED: %s :after %s ", \
                    change->new_val()->xpath(), \
                    change->old_val()->xpath());
        }
        else if (nullptr != change->new_val()) {
           O1_LOG("\nO1 NrCellCb : MOVED: %s : first", \
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
const char *NrCellCb::evToStr(sr_event_t ev) {
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
 * @brief override module_change to handle callback
 *
 * @details
 *
 *    Function : module_change
 *
 *    Functionality:
 *      - override module_change to handle callback of modification of
 *        o-ran-sc-du-hello-world yang module
 *
 *
 * @params[in] sysrepo::S_Session session, const char *module_name,
 *             const char *xpath, sr_event_t event, uint32_t request_id,
 *             void *private_data
 * @return SR_ERR_OK  - success
 *         SR_ERR_OK  - failure
 ******************************************************************/

int NrCellCb::module_change(sysrepo::S_Session sess, \
                                    const char *module_name, \
                                    const char *xpath, \
                                    sr_event_t event, \
                                    uint32_t request_id, \
                                    void *private_data)
{
   char change_path[MAX_LEN];

   try {
      O1_LOG("\nO1 NrCellCb : Notification %s", evToStr(event));
      if (SR_EV_CHANGE == event)
      {
         NrCellList & cellList  = NrCellList::instance();
         NrCellInfo cellInfo;
         const NrCellList::CellOpStateMap & cellOpStateMap = cellList.getCellOpStateList();
         snprintf(change_path, MAX_LEN, "/%s:*//.", module_name);
         auto it = sess->get_changes_iter(change_path);
         uint16_t cellId;
         while (auto change = sess->get_change_next(it)) {
         //printChange(change); //enable only for debugging
         if(nullptr != change->new_val())
         {
            O1_LOG("\nO1 NrCellCb : Parameter value has been \
changed val=%s", change->new_val()->val_to_string().c_str());
            std::map<uint16_t, NrCellInfo>::const_iterator it;
            for(it = cellOpStateMap.begin(); it !=cellOpStateMap.end(); it++)
            {
               stringstream xpath;
               xpath << CELL_STATE_MODULE_PATH << "/du-to-ru-connection[name='" \
               << it->first << "']/administrative-state";
               O1_LOG("\nO1 NrCellCb : created xpath = %s", \
                      xpath.str().c_str());

               if((change->new_val()->to_string().find(xpath.str().c_str()) != \
                   std::string::npos))
               {
                  printChange(change);
                  string val = change->new_val()->val_to_string();
                  AdminState newVal = cellInfo.adminStateToEnum(val);
                  O1_LOG("\nO1 NrCellCb : Update admin state \
cellId =%d with admin-state value=%s", it->first, val.c_str());
                 if(!setAdminState(it->first, newVal)) {
                    O1_LOG("\nO1 NrCellCb : Could not change \
parameter value =%s", change->new_val()->val_to_string().c_str());
                    return SR_ERR_INTERNAL;
                 }
               }
            }
         }
     }
   }//if evToStr(event) check
   }
   catch( const std::exception& e ) {
      O1_LOG("\nO1 NrCellCb exception : %s\n", e.what());
   }
   return SR_ERR_OK;
}

/*******************************************************************
 *
 * @brief bring cell status according to the admin state
 *
 * @details
 *
 *    Function : setAdminState
 *
 *    Functionality:
 *      - bring cell status according to the admin state
 *
 *
 * @params[in] cellId, Admin state
 * @return true   - success
 *         false  - failure
 ******************************************************************/
bool NrCellCb::setAdminState(uint16_t cellId, AdminState newAdminState)
{
   if(newAdminState == UNLOCKED)
   {
      O1_LOG("\nNrCellList : set Admin State UNLOCKED" );
#ifndef ODU_TEST_STUB
      return bringCellUp(cellId);
#endif
   }
   else
   {
      O1_LOG("\nNrCellList : set Admin State LOCKED" );
#ifndef ODU_TEST_STUB
      return bringCellDown(cellId);
#endif
   }
   return true;
}


/**********************************************************************
         End of file
**********************************************************************/
