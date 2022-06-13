/*******************************************************************************
################################################################################
#   Copyright (c) [2022] [HCL Technologies Ltd.]                          #
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

/* This file contains macros and functions to support the preparation of pnf
   Registration VES Event*/

#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>
#include "JsonHelper.hpp"
#include "SliceMeasurementEventStdDef.hpp"
#include "CmInterface.h"

extern NRCellDU cellParams;

/* Default constructor*/
SliceMeasurementEventStdDef::SliceMeasurementEventStdDef()
               : PerfMeasurementEvent(VesEventType::PM_SLICE)
{

}

/* Default Destructor*/
SliceMeasurementEventStdDef::~SliceMeasurementEventStdDef()
{

}

/*******************************************************************
 *
 * @brief Returns ISO time String 
 *
 * @details
 *
 *    Function :getISOEventTime
 *
 *    Functionality:
 *      - Returns ISO time String
 *
 * @params[in] IN - void
 * @return value of string     - success
 *         empty string        - failure
 *
 * ****************************************************************/

std::string SliceMeasurementEventStdDef::getISOEventTime() {
   auto now = std::chrono::system_clock::now();
   auto itt = std::chrono::system_clock::to_time_t(now);
   std::ostringstream os;
   os << std::put_time(gmtime(&itt), "%FT%TZ");
   return os.str();
}

/*******************************************************************
*
* @brief prepare Stnd Slice Measurement path
*
* @details
*
* Function : getMeaspath
*
* Functionality:
* - prepare Stnd Slice Measurement path
*
* @params[in] IN - void
* @return true - path on success
*         false - failure
*
* ****************************************************************/
std::string SliceMeasurementEventStdDef::getMeasPath(string str, int sd, int sst)
{
   O1_LOG("\n O1 SliceMeasurementEventStdDef cellLocalId :%d",cellParams.cellLocalId);
   std::ostringstream os;
   os<<"/"<<YANG_NAME<<":network-function/distributed-unit-functions[id='"\ 
	   << ODU_HIGH <<"']/cell[id='"<<cellParams.cellLocalId <<\ 
	   "']/supported-measurements[performance-measurement-type='user-equipment-average-throughput-"\ 
	   <<str<<"']/supported-snssai-subcounter-instances[slice-differentiator='"\ 
	   <<sd<<"'][slice-service-type='"<<sst<<"']" ;
   return os.str();

}

/*******************************************************************
*
* @brief prepare Stnd Slice Measurement Fields
*
* @details
*
* Function : prepareEventFields
*
* Functionality:
* - prepare Stnd Slice Measurement Fields  in json format
*
* @params[in] IN - const pointer of Message type
* @return true - success
*         false - failure
*
* ****************************************************************/

bool SliceMeasurementEventStdDef::prepareEventFields(const Message* msg)
{

   const SliceMetrics* sliceMetrics = dynamic_cast<const SliceMetrics*> (msg);

   bool ret = true;

   cJSON* stndDefinedFields = this->mVesEventFields;
   if(JsonHelper::addNodeToObject(stndDefinedFields, "stndDefinedFieldsVersion",\
			   STND_DEFINED_FEILD_VERSION) == 0) 
   {
      ret = false;
   }
   else if(JsonHelper::addNodeToObject(stndDefinedFields, "schemaReference",\
			   SLICE_SCHEMA) == 0) 
   {
      ret = false;
   }

   cJSON* data = JsonHelper::createNode();
   if(data == 0)
   {
      O1_LOG("\nO1 SliceMeasurementEventStdDef : could not create data JSON object");
      return false;
   }
   else if(JsonHelper::addJsonNodeToObject(stndDefinedFields, "data", \
                                       data) == 0) {
      ret = false;
   }
   else if(JsonHelper::addNodeToObject(data, "id", SLICE_ID) == 0)
   {
      ret = false;
   }
   else if(JsonHelper::addNodeToObject(data, "start-time",\
			   getISOEventTime().c_str()) == 0)
   {
      ret = false;
   }
   else if(JsonHelper::addNodeToObject(data, "administrative-state",\
			   ADMINISTRATIVE_STATE) == 0)
   {
      ret = false;
   }
   else if(JsonHelper::addNodeToObject(data, "operational-state",\
			   OPERATIONAL_STATE) == 0)
   {
      ret = false;
   }
   else if(JsonHelper::addNodeToObject(data, "user-label", USER_LABEL) == 0)
   {
      ret = false;
   }
   else if(JsonHelper::addNodeToObject(data, "job-tag", "") == 0)
   {
      ret = false;
   }
   else if(JsonHelper::addNodeToObject(data, "granularity-period",\
			   GRANULARITY_PERIOD) == 0)
   {
      ret = false;
   }

   cJSON *measure= cJSON_CreateArray();
   if (measure == NULL)
   {
      ret = false;
   }
   else if(cJSON_AddItemToObject(data, "measurements", measure) == 0)
   {
      ret = false;
   }

   else if(ret)
   {
      const vector<SliceMetricRecord>& sliceList = sliceMetrics->getSliceMetrics();
      for (size_t i{0}; i < sliceList.size(); i++)
      {
         for(int j=0;j<MAX_THP_TYPE;j++)
         {
            cJSON* arr = JsonHelper::createNode();
            cJSON_AddItemToArray(measure,arr);

            string str;
            cJSON *value;
	    if(j==0)
	    {
               str = "downlink";
               value = cJSON_CreateNumber(sliceList[i].DRB_UEThpDl_SNSSAI);
            }
            else
	    {
               str = "uplink";
               value = cJSON_CreateNumber(sliceList[i].DRB_UEThpUl_SNSSAI);
            }

            if(JsonHelper::addNodeToObject(arr, MEAS_REF, getMeasPath(str,\
					    sliceList[i].networkSliceIdentifier.sd,\
					    sliceList[i].networkSliceIdentifier.sst).c_str()) == 0)
	    {
               ret = false;
            }

            cJSON_AddItemToObject(arr,"value", value);

            cJSON *unit = cJSON_CreateString(THROUGHPUT_UNIT);
            cJSON_AddItemToObject(arr,"unit", unit);
         }
      }
   }

   return ret;
}

/**********************************************************************
         End of file
**********************************************************************/
