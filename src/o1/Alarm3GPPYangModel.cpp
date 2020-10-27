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

#include "Alarm3GPPYangModel.hpp"

using namespace std;


/********************************************************************** 
   Description : It is a callback function, called on get request of
                 alarm-list (overridden function of sysrepo::Callback )
   Params[In]  : (sysrepo::S_Session, module_name, path,
                 request_xpath, request_id, &parent, private_data)
   Return      : SR_ERR_OK - success
**********************************************************************/

int Alarm3GPPYangModel::oper_get_items(sysrepo::S_Session session,
                                       const char         *module_name,
                                       const char         *path,
                                       const char         *request_xpath,
                                       uint32_t           request_id,
                                       libyang::S_Data_Node &parent,
                                       void *private_data) 
{
    O1_LOG("\n\n ========== CALLBACK CALLED TO PROVIDE \" %s DATA ==========\n", path); 

    libyang::S_Context ctx = session->get_context();
    libyang::S_Module mod = ctx->get_module(module_name);

    parent.reset(new libyang::Data_Node(ctx, ALARM_MODULE_PATH_3GPP, nullptr, LYD_ANYDATA_CONSTSTRING, 0));
    //read the data from map
    //tree of alarmRecord  : AlarmList/AlarmListGrp/alarmRecordList/AlarmRecordGrp

    libyang::S_Data_Node alarms(new libyang::Data_Node(parent, mod, "AlarmList"));
    libyang::S_Data_Node alarm(new libyang::Data_Node(alarms, mod, "AlarmListGrp"));
    libyang::S_Data_Node ifc3(new libyang::Data_Node(parent, mod, "alarmRecordList"));
    libyang::S_Data_Node ifc4(new libyang::Data_Node(ifc3, mod, "AlarmRecordGrp"));
    

    libyang::S_Data_Node id;
    libyang::S_Data_Node text;
    libyang::S_Data_Node severity;
    libyang::S_Data_Node status;
    libyang::S_Data_Node add_info;

    //read the data from map
    map<uint16_t,Alarm>::const_iterator it;

    AlarmManager& alrmMgr = AlarmManager::instance();
    const map<uint16_t,Alarm>& alrmList = alrmMgr.getAlarmList();

    for(it = alrmList.begin(); it !=alrmList.end(); it++)
    {
        O1_LOG("\nAlarm ID %hu", it->second.getAlarmId());
        alarm.reset(new libyang::Data_Node(alarms, mod, "alarm"));
        id.reset(new libyang::Data_Node(alarm, mod, "alarm-id", to_string(it->second.getAlarmId()).c_str()));
        text.reset(new libyang::Data_Node(alarm, mod, "alarm-text", it->second.getAdditionalText().c_str()));
        severity.reset(new libyang::Data_Node(alarm, mod, "severity", to_string(it->second.getEventType()).c_str()));
        status.reset(new libyang::Data_Node(alarm, mod, "status", it->second.getSpecificProblem().c_str()));
        add_info.reset(new libyang::Data_Node(alarm, mod, "additional-info", it->second.getAdditionalInfo().c_str()));
    }
    return SR_ERR_OK;
}

/**********************************************************************
         End of file
**********************************************************************/
