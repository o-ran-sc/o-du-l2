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
#include "CellOpStateCb.hpp"
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

int CellOpStateCb::oper_get_items(sysrepo::S_Session session, \
                                       const char *module_name, \
                                       const char *path, \
                                       const char *request_xpath, \
                                       uint32_t request_id, \
                                       libyang::S_Data_Node &parent, \
                                       void *private_data)
{
   O1_LOG("O1 CellOpStateCb : Callback called for path=%s on get request", path);
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

   //read the data from CellOpStateMap
#ifndef ODU_TEST_STUB
   std::map<uint16_t, CellOpStateStruct>::const_iterator it;
   for(it = mCellOpStateMap.begin(); it !=mCellOpStateMap.end(); it++)
   {
      O1_LOG("\nO1 CellOpStateCb : cellId = %d, opState=%d, cellState=%d\n", \
              it->first, it->second.opState, it->second.cellState);
      name.reset(new libyang::Data_Node(connection, mod, "name", \
                 to_string(it->first).c_str()));
      operational_state.reset(new libyang::Data_Node(connection, mod, \
                        "operational-state", \
                        enumToOperationalStateString(it->second.opState).c_str()));
      cell_state.reset(new libyang::Data_Node(connection, mod, "cell-state", \
                       enumToCellStateString(it->second.cellState).c_str()));
   }
#endif
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

void CellOpStateCb::print_change(sysrepo::S_Change change) {
   switch(change->oper()) {
      case SR_OP_CREATED:
         if (nullptr != change->new_val()) {
            O1_LOG("O1 CellOpStateCb : CREATED: %s", \
                    change->new_val()->to_string().c_str());
         }
         break;
      case SR_OP_DELETED:
         if (nullptr != change->old_val()) {
             O1_LOG("O1 CellOpStateCb : DELETED:  %s", \
                     change->old_val()->to_string().c_str());
         }
         break;
      case SR_OP_MODIFIED:
         if (nullptr != change->old_val() && nullptr != change->new_val()) {
            O1_LOG("O1 CellOpStateCb : MODIFIED: old value %s :new value %s", \
                    change->old_val()->to_string().c_str(), \
                    change->new_val()->to_string().c_str());
        }
        break;
     case SR_OP_MOVED:
        if (nullptr != change->old_val() && nullptr != change->new_val()) {
           O1_LOG("O1 CellOpStateCb : MOVED: %s :after %s ", \
                    change->new_val()->xpath(), \
                    change->old_val()->xpath());
        }
        else if (nullptr != change->new_val()) {
           O1_LOG("O1 CellOpStateCb : MOVED: %s : first\n", \
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
 *    Function : ev_to_str
 *
 *    Functionality:
 *      - convert event type to string
 *
 *
 * @params[in] sr_event_t event
 * @return event name in string form
 ******************************************************************/

/* Helper function for printing events. */
const char *CellOpStateCb::ev_to_str(sr_event_t ev) {
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

int CellOpStateCb::module_change(sysrepo::S_Session sess, \
                                    const char *module_name, \
                                    const char *xpath, \
                                    sr_event_t event, \
                                    uint32_t request_id, \
                                    void *private_data)
{
   char change_path[MAX_LEN];

   try {
      O1_LOG("O1 CellOpStateCb : Notification %s\n", ev_to_str(event));
      if (SR_EV_CHANGE == event)
      {
      snprintf(change_path, MAX_LEN, "/%s:*//.", module_name);
      auto it = sess->get_changes_iter(change_path);
      uint16_t cellId;
      while (auto change = sess->get_change_next(it)) {
         //print_change(change); //enable only for debugging
         if(nullptr != change->new_val())
         {
            O1_LOG("O1 CellOpStateCb : Parameter value has been \
changed val=%s\n", change->new_val()->val_to_string().c_str());
            std::map<uint16_t, CellOpStateStruct>::const_iterator it;
            for(it = mCellOpStateMap.begin(); it !=mCellOpStateMap.end(); it++)
            {
               stringstream xpath;
               xpath << CELL_STATE_MODULE_PATH << "/du-to-ru-connection[name='" \
               << it->first << "']/administrative-state";
               O1_LOG("O1 CellOpStateCb : created xpath = %s", \
                      xpath.str().c_str());
            
               if((change->new_val()->to_string().find(xpath.str().c_str()) != \
                   std::string::npos))
               {
                  print_change(change);
                  string val = change->new_val()->val_to_string();
                  AdminState newVal = adminStateToEnum(val);
#ifndef ODU_TEST_STUB
                  O1_LOG("O1 CellOpStateCb : Update admin state for \
cellId =%d with admin-state value=%s\n", it->first, val.c_str());
                 if(!updateAdminState(it->first, newVal)) {
                    O1_LOG("O1 CellOpStateCb : Could not change \
parameter value =%s\n", change->new_val()->val_to_string().c_str());
                    return SR_ERR_OK;
                 }
#endif
               }
               else
               {
                  O1_LOG("O1 CellOpStateCb : Non administrative-state \
change request recieved\n");
               }
            }
         }
     }
   }//if ev_to_str(event) check
   }
   catch( const std::exception& e ) {
      O1_LOG("exception : %s\n", e.what());
   }
   return SR_ERR_OK;
}

/*******************************************************************
 *
 * @brief convert admin state to enum
 *
 * @details
 *
 *    Function : adminStateToEnum
 *
 *    Functionality:
 *      - convert admin state to enum
 *
 *
 * @params[in] admin state in string form
 * @return admin state in enum form
 ******************************************************************/


AdminState CellOpStateCb::adminStateToEnum(string val)
{
   AdminState ret=LOCKED;
   if( val == "LOCKED")
      ret = LOCKED;
   else if(val == "UNLOCKED")
      ret = UNLOCKED;
   else if(val ==  "SHUTTING_DOWN")
      ret = SHUTTING_DOWN;
   else
      O1_LOG("O1 CellOpStateCb : %s admin state not handled\n", \
              val.c_str());

   return ret;
}

/*******************************************************************
 *
 * @brief convert enum of cell state to string
 *
 * @details
 *
 *    Function : enumToCellStateString
 *
 *    Functionality:
 *      - convert enum of cell state to string
 *
 *
 * @params[in] enum of cell state
 * @return cell state in string form
 ******************************************************************/


string CellOpStateCb::enumToCellStateString(CellState val)
{
   string ret = "IDLE";
   switch(val)
   {
      case INACTIVE:
         ret = "INACTIVE";
         break;
      case ACTIVE:
         ret = "ACTIVE";
         break;
      case IDLE:
         ret = "IDLE";
         break;
      default :
         O1_LOG("O1 CellOpStateCb : %d cell state not handled\n", val);

   }
   return ret;
}

/*******************************************************************
 *
 * @brief convert enum of operational state to string
 *
 * @details
 *
 *    Function : enumToOperationalStateString
 *
 *    Functionality:
 *      - convert enum of operational state to string
 *
 *
 * @params[in] enum of operational state
 * @return cell operational in string form
 ******************************************************************/

string CellOpStateCb::enumToOperationalStateString(OpState val)
{
   string ret = "DISABLED";
   switch(val)
   {
      case DISABLED:
         ret = "DISABLED";
         break;
      case ENABLED:
         ret = "ENABLED";
         break;
      default :
         O1_LOG("O1 CellOpStateCb : %d operational state not handled\n", val);

   }
   return ret;
}

/*******************************************************************
 *
 * @brief update cell and operational state in the map
 *
 * @details
 *
 *    Function : updateCellOpState
 *
 *    Functionality:
 *      - update cell and operational state in the map using cellId
 *        as key
 *
 *
 * @params[in] cellId, opState, cellState
 * @return true   - success
 *         false  - failure
 ******************************************************************/


bool CellOpStateCb::updateCellOpState(uint16_t cellId, \
                                    OpState opState, \
                                    CellState cellState)
{
   O1_LOG("\nO1 CellOpStateCb : Setting cellId = %d, opState=%d, \
cellState=%d\n", cellId, opState, cellState);
   CellOpStateStruct cellop ;
   cellop.cellId = cellId;
   cellop.opState = opState;
   cellop.cellState = cellState;
   mCellOpStateMap[cellId] = cellop;
   return true;   
}

/*******************************************************************
 *
 * @brief bring cell status according to the admin state
 *
 * @details
 *
 *    Function : updateAdminState
 *
 *    Functionality:
 *      - bring cell status according to the admin state
 *
 *
 * @params[in] cellId, Admin state
 * @return true   - success
 *         false  - failure
 ******************************************************************/
bool CellOpStateCb::updateAdminState(uint16_t cellId, AdminState newAdminState)
{
   if(newAdminState == UNLOCKED)
   {
      O1_LOG("\nCellOpStateCb : updateAdminState with UNLOCKED called" );
#ifndef ODU_TEST_STUB
      return bringCellUp(cellId);
#endif
   }
   else
   {
      O1_LOG("\nCellOpStateCb : updateAdminState with LOCKED called" );
#ifndef ODU_TEST_STUB
      return bringCellDown(cellId);
#endif
   }
   return true;
}

/**********************************************************************
         End of file
**********************************************************************/

