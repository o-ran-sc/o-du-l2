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

/* This file contains implementation of RRM Policy get and update handler along
 * with some supporting funtion. It handles get and change callback for
 * rrmpolicy yang module  */

#include <sstream>
#include "RrmPolicyCb.hpp"

RrmPolicyList rrmPolicy[MAX_POLICY];

uint8_t policyNum = 0;
uint8_t policyIndex = 0;
uint8_t policyMemberListNum =0;

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

int RrmPolicyCb::oper_get_items(sysrepo::S_Session session, \
                                       const char *module_name, \
                                       const char *path, \
                                       const char *request_xpath, \
                                       uint32_t request_id, \
                                       libyang::S_Data_Node &parent, \
                                       void *private_data)
{
   O1_LOG("\nO1 RrmPolicyCb : Callback called for path=%s on get request", \
	     path);
   return SR_ERR_OK;
}

/*******************************************************************
 *
 * @brief convert string resouce type to enum
 *
 * @details
 *
 *    Function : resourceTypeToEnum
 *
 *    Functionality:
 *      - convert string resouce type to enum
 *
 *
 * @params[in] string resouceType
 * @return Enum resouceType
 ******************************************************************/
RrmResourceType RrmPolicyCb::resourceTypeToEnum(string &val)
{
   RrmResourceType ret;

   if(val == "PRB")
   {
      ret = PRB;
   }
   else if(val == "PRB_UL")
   {
      ret = PRB_UL;
   }
   else if(val == "PRB_DL")
   {
      ret = PRB_DL;
   }
   else if(val == "RRC")
   {
      ret = RRC;
   }
   else if(val == "DRB")
   {
      ret = DRB;
   }
   else
   {
      O1_LOG("\nO1 RrmPolicyCb::updateParams non handeled value of "
	     "ResourceType = %s", val.c_str());
   }
   O1_LOG("\nO1 rrmPolicy resourceTypeToEnum : val [%d]", ret);
   return ret;
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
 * @params[in] string leafNode, string val
 * @return void
 ******************************************************************/

bool RrmPolicyCb::updateParams(string &parent, string &leafNode, string &val)
{

   O1_LOG("\nO1 RrmPolicyCb::updateParams parameter name=%s val = %s", \
           leafNode.c_str(), val.c_str());
   bool ret = true;

   if(val != "")
   {
      if (leafNode == "id")
      {
	 policyIndex++;
	 policyNum = policyIndex -1 ;
         memset(rrmPolicy[policyNum].id, '\0', ID_MAX_LEN);
         strncpy(rrmPolicy[policyNum].id, val.c_str(), ID_MAX_LEN-1);
	 O1_LOG("\nO1 RrmPolicyCb::updateParams id = %s", rrmPolicy[policyNum].id);
	 rrmPolicy[policyNum].rRMMemberNum = 0;
      }
      else if(leafNode ==  "resourceType")
      {
         rrmPolicy[policyNum].resourceType = resourceTypeToEnum(val);
         O1_LOG("\nO1 RrmPolicyCb::updateParams  ResourceType = %d", \
                  rrmPolicy[policyNum].resourceType);
      }
     else if(leafNode == "rRMPolicyMaxRatio")
      {
         rrmPolicy[policyNum].rRMPolicyMaxRatio = atoi(val.c_str());
	 O1_LOG("\nO1 RrmPolicyCb::updateParams rRMPolicyMaxRatio = %d", \
                   rrmPolicy[policyNum].rRMPolicyMaxRatio);
      }
      else if(leafNode == "rRMPolicyMinRatio")
      {
         rrmPolicy[policyNum].rRMPolicyMinRatio = atoi(val.c_str());
	 O1_LOG("\nO1 RrmPolicyCb::updateParams rRMPolicyMinRatio = %d", \
                   rrmPolicy[policyNum].rRMPolicyMinRatio);
      }
      else if(leafNode == "rRMPolicyDedicatedRatio")
      {
         rrmPolicy[policyNum].rRMPolicyDedicatedRatio = atoi(val.c_str());
	 O1_LOG("\nO1 RrmPolicyCb::updateParams rRMPolicyDedicatedRatio = %d", \
                   rrmPolicy[policyNum].rRMPolicyDedicatedRatio);
      }
      else if((leafNode == "mcc") || (leafNode == "mnc")
	     || leafNode == "sst" || leafNode == "sd")
      {
	 int memberNum = policyMemberListNum/MAX_POLICY_LIST;
         rrmPolicy[policyNum].rRMMemberNum = memberNum+1;
         if(leafNode == "mcc")
         {
            memset(rrmPolicy[policyNum].rRMPolicyMemberList[memberNum].mcc, \
	           '\0', MCC_LEN);
            rrmPolicy[policyNum].rRMPolicyMemberList[memberNum].mcc[0] = std::stoi(val.substr(0,1).c_str(),0,10);
            rrmPolicy[policyNum].rRMPolicyMemberList[memberNum].mcc[1] = std::stoi(val.substr(1,1).c_str(),0,10);
            rrmPolicy[policyNum].rRMPolicyMemberList[memberNum].mcc[2] = std::stoi(val.substr(2,1).c_str(),0,10);
            O1_LOG("\nO1 RrmPolicyCb::updateParams rrmPolicy[%d]."
	           "rRMPolicyMemberList[%d].mcc[2] = %d", policyNum, memberNum, \
		   rrmPolicy[policyNum].rRMPolicyMemberList[memberNum].mcc[2]);
         }
	 else if(leafNode == "mnc")
         {
	    memset(rrmPolicy[policyNum].rRMPolicyMemberList[memberNum].mnc, \
	           '\0', MNC_LEN);
            rrmPolicy[policyNum].rRMPolicyMemberList[memberNum].mnc[0] = std::stoi(val.substr(0,1).c_str(),0,10);
            rrmPolicy[policyNum].rRMPolicyMemberList[memberNum].mnc[1] = std::stoi(val.substr(1,1).c_str(),0,10);
            if(strlen(val.c_str())>2)
            {
               rrmPolicy[policyNum].rRMPolicyMemberList[memberNum].mnc[2] = std::stoi(val.substr(2,1).c_str(),0,10);
            }
           O1_LOG("\nO1 RrmPolicyCb::updateParams rrmPolicy[%d]."
                     "rRMPolicyMemberList[%d].mnc[1] = %d", policyNum, memberNum, \
		      rrmPolicy[policyNum].rRMPolicyMemberList[memberNum].mnc[1]);
         }
         else if(leafNode == "sst")
         {
            rrmPolicy[policyNum].rRMPolicyMemberList[memberNum].sst = \
		    atoi(val.c_str());
	    O1_LOG("\nO1 RrmPolicyCb::updateParams sst val = %d",
	              rrmPolicy[policyNum].rRMPolicyMemberList[memberNum].sst);
         }
         else if(leafNode == "sd")
         {
            memset(rrmPolicy[policyNum].rRMPolicyMemberList[memberNum].sd, \
	           '\0', SD_LEN);

            rrmPolicy[policyNum].rRMPolicyMemberList[memberNum].sd[0] = \
                    std::stoi(val.substr(0,2).c_str(),0,16);
           rrmPolicy[policyNum].rRMPolicyMemberList[memberNum].sd[1] = \
                    std::stoi(val.substr(2,2).c_str(),0,16);
            rrmPolicy[policyNum].rRMPolicyMemberList[memberNum].sd[2] = \
                    std::stoi(val.substr(4,2).c_str(),0,16);
            O1_LOG("\nO1 RrmPolicyCb::updateParams rrmPolicy[%d]."
		   "rRMPolicyMemberList[%d].sd[2] = %c", policyNum, memberNum, \
		   rrmPolicy[policyNum].rRMPolicyMemberList[memberNum].sd[SD_LEN-1]);
         }
         policyMemberListNum++;
      }
      else
      {
         O1_LOG("\nO1 RrmPolicyCb::updateParams some parameter not handled name=%s "
		"val = %s", leafNode.c_str(), val.c_str());
	 ret = false;
      }
   }
   else
   {
      O1_LOG("\nO1 RrmPolicyCb::updateParams without value node name=%s val = %s",
	        leafNode.c_str(), val.c_str());
   }
   return ret;
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

int RrmPolicyCb::module_change(sysrepo::S_Session sess, \
                                    const char *module_name, \
                                    const char *xpath, \
                                    sr_event_t event, \
                                    uint32_t request_id, \
                                    void *private_data)
{
   char change_path[MAX_LEN];
   policyNum = 0;
   policyIndex = 0;
   policyMemberListNum =0;

   try {
      O1_LOG("\nO1 RrmPolicyCb : Notification %s", NetconfUtils::evToStr(event));
      if (SR_EV_CHANGE == event)
      {
         snprintf(change_path, MAX_LEN, "/%s:*//.", module_name);
         auto it = sess->get_changes_iter(change_path);
         while (auto change = sess->get_change_next(it)) {
            //NetconfUtils::printChange(change); //enable only for debugging
            if(nullptr != change->new_val())
            {
               O1_LOG("\n\nO1 RrmPolicyCb : Parameter value has been \
changed val=%s", change->new_val()->val_to_string().c_str());
	       string val = change->new_val()->val_to_string();
               string parent, leafNode;
               NetconfUtils::getLeafInfo(change->new_val()->to_string(), parent, leafNode);
               O1_LOG("\nO1 RrmPolicy : LeafNode name = [%s]", leafNode.c_str());
               if(!updateParams(parent, leafNode, val)){
                  O1_LOG("\nO1 RrmPolicy : LeafNode  [%s] not handled", leafNode.c_str());
		  //later error handling can be done
	       }
            }
         }
#ifndef ODU_TEST_STUB
	 if(ROK != setRrmPolicy(rrmPolicy, policyIndex))
	 {
            O1_LOG("\nO1 RrmPolicy : updateRrmPolicy failed");
	    return SR_ERR_INTERNAL;
	    //later error handling can be done
	 }
#endif
      }
   }
   catch( const std::exception& e ) {
      O1_LOG("\nO1 RrmPolicyCb exception : %s\n", e.what());
      return SR_ERR_INTERNAL;
   }
   return SR_ERR_OK;
}

/**********************************************************************
         End of file
**********************************************************************/
