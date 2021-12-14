
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

/* This file contains NrCellDu get and update handler . It handles
   get and change callback for NrCellDu yang module  */

#include <sstream>
#include "NrCellDuCb.hpp"
#include "NrCellInfo.hpp"
#include "NrCellList.hpp"

#define XPATH_MAX_LEN 256
#define MAX_PLMN_MEMBER 4

using namespace std;

//Global variable
NRCellDU cellParams;
ManagedElement managedElement;
int plmnListNum = 0;

/*******************************************************************
 *
 * @brief override oper_get_items to handle callback
 *
 * @details
 *
 *    Function : oper_get_items
 *
 *    Functionality:
 *      - override oper_get_items to handle callback
 *
 *
 * @params[in] sysrepo::S_Session session, const char *module_name,
 *             const char *path, const char *request_xpath,
 *             uint32_t request_id, libyang::S_Data_Node &parent,
 *             void *private_data
 * @return SR_ERR_OK/SR_ERR_NOK
 ******************************************************************/

int NrCellDuCb::oper_get_items(sysrepo::S_Session session, \
                                       const char *module_name, \
                                       const char *path, \
                                       const char *request_xpath, \
                                       uint32_t request_id, \
                                       libyang::S_Data_Node &parent, \
                                       void *private_data)
{
   O1_LOG("\nO1 NrCellCb : Callback called for path=%s on get request", path);
   libyang::S_Context ctx = session->get_context();
   libyang::S_Module modMe = ctx->get_module(module_name);

   //first create root of the tree then add nodes and leaves and fill data
   parent.reset(new libyang::Data_Node(ctx, MANAGED_ELEMENT_MODULE_PATH, \
                nullptr, LYD_ANYDATA_CONSTSTRING, 0));
   libyang::S_Data_Node id(new libyang::Data_Node(parent, modMe, "id", \
                  managedElement.meId));
   libyang::S_Module modGnbdu = ctx->get_module(GNB_DU_FUNTION_MODULE_NAME);
   libyang::S_Data_Node gnbdu(new libyang::Data_Node(parent, modGnbdu, \
                                  "GNBDUFunction"));
   libyang::S_Data_Node id1 (new libyang::Data_Node(gnbdu, modGnbdu, "id", \
                  managedElement.gnbId));
   libyang::S_Module mod = ctx->get_module(NR_CELL_DU_MODULE_NAME);
   libyang::S_Data_Node nrCellDu(new libyang::Data_Node(gnbdu, mod, \
                                  "NRCellDU"));
   id.reset(new libyang::Data_Node(nrCellDu, mod, "id", \
                  managedElement.nrCellDuId));
   libyang::S_Data_Node attributes(new libyang::Data_Node(nrCellDu, mod, \
                                  "attributes"));

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
      operational_state.reset(new libyang::Data_Node(attributes, mod, \
                        "operationalState", \
                        cellInfo.enumToOperationalStateString(it->second.getOpState()).c_str()));
      cell_state.reset(new libyang::Data_Node(attributes, mod, "cellState", \
                       cellInfo.enumToCellStateString(it->second.getCellState()).c_str()));
   }
   return SR_ERR_OK;
}

/*******************************************************************
 *
 * @brief convert administrative state string to enum
 *
 * @details
 *
 *    Function : administrativeStateToEnum
 *
 *    Functionality:
 *      - convert administrative state string to enum
 *
 *
 * @params[in] string AdminState
 * @return Enum AdminState
 ******************************************************************/

AdminState NrCellDuCb::administrativeStateToEnum(string &val)
{
   AdminState state;
   if (val=="LOCKED") {
      state = LOCKED;
   }
   else if(val=="UNLOCKED") {
      state = UNLOCKED;
   }
   else if(val=="SHUTTING_DOWN") {
      state = SHUTTING_DOWN;
   }
   return state;
}

/*******************************************************************
 *
 * @brief fill the parameters value into the structure instance
 *
 * @details
 *
 *    Function : updateParams
 *
 *    Functionality:
 *      - fill the parameters value into the structure instance
 *
 *
 * @params[in] string leaf, string val
 * @return void
 ******************************************************************/


void NrCellDuCb::updateParams(string &parent, string &leaf, string &val)
{
   int memberNum = plmnListNum/MAX_PLMN_MEMBER;
   if(parent.find("attribute") != string::npos)
   {
      if(leaf == "cellLocalId"){
         cellParams.cellLocalId = atoi(val.c_str());
         O1_LOG("\nO1 NrCellDuCb : val = %d", cellParams.cellLocalId);
      }
      else if(leaf == "administrativeState"){
         cellParams.administrativeState = administrativeStateToEnum(val);
         O1_LOG("\nO1 NrCellDuCb : val = %d", cellParams.administrativeState);
      }
     else if(leaf == "nRPCI"){
         cellParams.nRPCI = atoi(val.c_str());
         O1_LOG("\nO1 NrCellDuCb : val = %d", cellParams.nRPCI);
      }
      else if(leaf == "nRTAC"){
         cellParams.nRTAC = atoi(val.c_str());
         O1_LOG("\nO1 NrCellDuCb : val = %d", cellParams.nRTAC);
      }
      else if(leaf == "arfcnDL"){
         cellParams.arfcnDL = atoi(val.c_str());
         O1_LOG("\nO1 NrCellDuCb : val = %d", cellParams.arfcnDL);
      }
      else if(leaf == "arfcnUL"){
         cellParams.arfcnUL = atoi(val.c_str());
         O1_LOG("\nO1 NrCellDuCb : val = %d", cellParams.arfcnUL);
      }
      else if(leaf == "arfcnSUL"){
         cellParams.arfcnSUL = atoi(val.c_str());
         O1_LOG("\nO1 NrCellDuCb : val = %d", cellParams.arfcnSUL);
      }
      else if(leaf == "ssbFrequency"){
         cellParams.ssbFrequency = atoi(val.c_str());
         O1_LOG("\nO1 NrCellDuCb : val = %d", cellParams.ssbFrequency);
      }
      else if(leaf == "ssbPeriodicity"){
         cellParams.ssbPeriodicity = atoi(val.c_str());
         O1_LOG("\nO1 NrCellDuCb : val = %d", cellParams.ssbPeriodicity);
      }
      else if(leaf == "ssbSubCarrierSpacing"){
         cellParams.ssbSubCarrierSpacing = atoi(val.c_str());
         O1_LOG("\nO1 NrCellDuCb : val = %d", cellParams.ssbSubCarrierSpacing);
      }
      else if(leaf == "ssbOffset"){
         cellParams.ssbOffset = atoi(val.c_str());
         O1_LOG("\nO1 NrCellDuCb : val = %d", cellParams.ssbOffset);
      }
      else if(leaf == "ssbDuration"){
         cellParams.ssbDuration = atoi(val.c_str());
	 O1_LOG("\nO1 NrCellDuCb : val = %d", cellParams.ssbDuration);
      }
      else if(leaf == "bSChannelBwUL"){
         cellParams.bSChannelBwUL = atoi(val.c_str());
         O1_LOG("\nO1 NrCellDuCb : val = %d", cellParams.bSChannelBwUL);
      }
      else if(leaf == "bSChannelBwDL"){
         cellParams.bSChannelBwDL = atoi(val.c_str());
         O1_LOG("\nO1 NrCellDuCb : val = %d", cellParams.bSChannelBwDL);
      }
      else if(leaf == "bSChannelBwSUL"){
         cellParams.bSChannelBwSUL = atoi(val.c_str());
         O1_LOG("\nO1 NrCellDuCb : val = %d", cellParams.bSChannelBwSUL);
      }
   }
   else if(parent.find("pLMNInfoList") != string::npos && memberNum < MAX_SUPPORTED_PLMN)
   {
         if(leaf == "mcc")
         {
            memset(cellParams.plmnList[memberNum].mcc, \
	           '\0', MCC_LEN);
            cellParams.plmnList[memberNum].mcc[0] = std::stoi(val.substr(0,1).c_str(),0,10);
            cellParams.plmnList[memberNum].mcc[1] = std::stoi(val.substr(1,1).c_str(),0,10);
            cellParams.plmnList[memberNum].mcc[2] = std::stoi(val.substr(2,1).c_str(),0,10);
            O1_LOG("\nO1 NrCellDuCb : cellParams.plmnList[%d].mcc[2]  = %d",
	           memberNum, cellParams.plmnList[memberNum].mcc[2]);
         }
	 else if(leaf == "mnc")
         {
	    memset(cellParams.plmnList[memberNum].mnc, \
	           '\0', MNC_LEN);
            cellParams.plmnList[memberNum].mnc[0] = std::stoi(val.substr(0,1).c_str(),0,10);
            cellParams.plmnList[memberNum].mnc[1] = std::stoi(val.substr(1,1).c_str(),0,10);
            if(strlen(val.c_str())>2)
            {
               cellParams.plmnList[memberNum].mnc[2] = std::stoi(val.substr(2,1).c_str(),0,10);
            }
            O1_LOG("\nO1 NrCellDuCb : cellParams.plmnList[%d].mnc[1]  = %d",
	           memberNum, cellParams.plmnList[memberNum].mnc[1]);
         }
         else if(leaf == "sst")
         {
            cellParams.plmnList[memberNum].sst = \
		    atoi(val.c_str());
         O1_LOG("\nO1 NrCellDuCb : cellParams.plmnList[%d].sst  = %d", \
			 memberNum, cellParams.plmnList[memberNum].sst);
         }
         else if(leaf == "sd")
          {
            memset(cellParams.plmnList[memberNum].sd, \
	           '\0', SD_LEN);

            cellParams.plmnList[memberNum].sd[0] = \
                    std::stoi(val.substr(0,2).c_str(),0,16);
            cellParams.plmnList[memberNum].sd[1] = \
                    std::stoi(val.substr(2,2).c_str(),0,16);
            cellParams.plmnList[memberNum].sd[2] = \
                    std::stoi(val.substr(4,2).c_str(),0,16);
            O1_LOG("\nO1 NrCellDuCb : cellParams.plmnList[%d].sd[2]  = %d",
                    memberNum, cellParams.plmnList[memberNum].sd[SD_LEN-1]);
         }
         plmnListNum++;
      }
    else if(parent.find("ManagedElement") != string::npos)
    {
         if(leaf == "id")
         {
            strncpy(managedElement.meId, val.c_str(), strlen(val.c_str()));
            O1_LOG("\nO1 NrCellDuCb : ManagedElement id  = %s", managedElement.meId);
         }
    }
    else if(parent.find("GNBDUFunction") != string::npos)
    {
         if(leaf == "id")
         {
            strncpy(managedElement.gnbId, val.c_str(), strlen(val.c_str()));
            O1_LOG("\nO1 NrCellDuCb : GNBDUFuntion id  = %s", managedElement.gnbId);
         }
    }
    else if(parent.find("NRCellDu") != string::npos)
    {
         if(leaf == "id")
         {
            strncpy(managedElement.nrCellDuId, val.c_str(), strlen(val.c_str()));
            O1_LOG("\nO1 NrCellDuCb : NRCELLDU id  = %s", managedElement.nrCellDuId);
         }
    }
}

/*******************************************************************
 *
 * @brief set cell parameters
 *
 * @details
 *
 *    Function : configureCell
 *
 *    Functionality:
 *      - pass the cell Parameters to ODU high stack
 *
 *
 * @params[in] void
 * @return success/failure
 ******************************************************************/

bool NrCellDuCb::configureCell()
{
   O1_LOG("\nO1 rannetworkCb : configcell");
#ifndef ODU_TEST_STUB
   if( setCellParam() != ROK)
   {
      O1_LOG("\nO1 rannetworkCb : fail to set cell configuration in DU");
      return false;
   }
   return true;
#else
   return true;
#endif
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
 * @return SR_ERR_OK        - success
 *         SR_ERR_INTERNAL  - failure
 ******************************************************************/

int NrCellDuCb::module_change(sysrepo::S_Session sess, \
                                    const char *module_name, \
                                    const char *xpath, \
                                    sr_event_t event, \
                                    uint32_t request_id, \
                                    void *private_data)
{
   O1_LOG("\nO1 NrCellDuCb : Notification edit sucessful");

   char change_path[XPATH_MAX_LEN];
   plmnListNum = 0;

   try {
      O1_LOG("\nO1 NrCellDuCbCb : Notification %s", NetconfUtils::evToStr(event));
      if (SR_EV_CHANGE == event)
      {
         snprintf(change_path, XPATH_MAX_LEN, "/%s:*//.", module_name);
         auto it = sess->get_changes_iter(change_path);
         while (auto change = sess->get_change_next(it)) {
         //NetconfUtils::printChange(change); //enable only for debugging

	 if(nullptr != change->new_val())
         {
               //O1_LOG("\nO1 NrCellDuCb : Parameter value has been \
changed val=%s", change->new_val()->val_to_string().c_str());
               string val = change->new_val()->val_to_string();
               string parent, leaf;

	       NetconfUtils::getLeafInfo(change->new_val()->to_string(), \
			                 parent, leaf);
               O1_LOG("\nO1 NrCellDuCb : parent = [%s], leaf = [%s]", \
			 parent.c_str(), leaf.c_str());
               updateParams(parent, leaf ,val);
            }
         }
         if(!configureCell())
	 {
            O1_LOG("\nO1 NrCellDuCb : configcell failed");
	    return SR_ERR_INTERNAL;
	 }

      }
   }
   catch( const std::exception& e ) {
      O1_LOG("\nO1 rannetworkCb exception : %s\n", e.what());
      return SR_ERR_INTERNAL;
   }

   return SR_ERR_OK;
}

/**********************************************************************
         End of file
**********************************************************************/
