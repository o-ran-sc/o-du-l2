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


#ifndef __CELL_OP_STATE_CB_HPP__
#define __CELL_OP_STATE_CB_HPP__

#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "sysrepo-cpp/Session.hpp"
#include "ConfigInterface.h"
#include "GlobalDefs.hpp"
#include "Singleton.hpp"

typedef enum {
   LOCKED,
   UNLOCKED,
   SHUTTING_DOWN
}AdminState;

typedef struct CellOpStateStruct{
   uint16_t cellId;
   OpState opState;
   CellState cellState;
   } cellOpStateStruct;

class CellOpStateCb : public Singleton<CellOpStateCb>, \
                              public sysrepo::Callback {
   public:
      friend Singleton<CellOpStateCb>;
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
      bool updateCellOpState(uint16_t cellId, OpState opState, \
                             CellState cellState);
   private:
      bool updateAdminState(uint16_t cellId, AdminState newAdminState);
      AdminState adminStateToEnum(string val);
      string enumToOperationalStateString(OpState val);
      string enumToCellStateString(CellState val);
      static void print_change(sysrepo::S_Change change);
      const char *ev_to_str(sr_event_t ev);
      typedef std::map<uint16_t, CellOpStateStruct> CellStateMap;
      CellStateMap mCellOpStateMap;


};

#endif

/**********************************************************************
         End of file
**********************************************************************/
