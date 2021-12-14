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

/* This file contains SliceMeasurementEvent class for  preparing the
   Slice Measurement VES Event*/

#include "SliceMeasurementEvent.hpp"
#include "JsonHelper.hpp"
#include "PmInterface.h"

#define MEASUREMENT_INTERVAL 60
#define MEASUREMENT_FIELDS_VERSION "4.0"

/* Constructor*/
SliceMeasurementEvent::SliceMeasurementEvent() 
                        : PerfMeasurementEvent(VesEventType::PM_SLICE)
{
}
/* Default destructor*/
SliceMeasurementEvent::~SliceMeasurementEvent()
{
}

/*******************************************************************
*
* @brief prepare Slice Measurement Fields
*
* @details
*
* Function : prepareEventFields
*
* Functionality:
* - prepare Slice Measurement Fields  in json format
*
* @params[in] IN - const pointer of Message type
* @return true - success
*         false - failure
*
* ****************************************************************/

bool SliceMeasurementEvent::prepareEventFields(const Message* msg)
{

    const SliceMetrics* sliceMetrics = dynamic_cast<const SliceMetrics*> (msg);
    
    bool ret = true;
    cJSON *measurementFields = this->mVesEventFields;

    if(JsonHelper::addNodeToObject(measurementFields, \
                                    "measurementFieldsVersion", \
                                    MEASUREMENT_FIELDS_VERSION) == 0)
    {
        ret = false;
    }

    cJSON *networkSliceArray = JsonHelper::createArray();

    if(networkSliceArray == 0)
    {
        ret = false;
    }

    else if (JsonHelper::addJsonNodeToObject(measurementFields, \
                                            "networkSliceArray", \
                                            networkSliceArray) == 0)
    {
        ret = false;
    }
    else if(ret)
    {
        const vector<SliceMetricRecord>& sliceList = sliceMetrics->getSliceMetrics();
        for (size_t i{0}; i < sliceList.size(); i++)            
        {
            cJSON *Slice = JsonHelper::createNode();
            char networkSliceId[7] = {0};
            sprintf(networkSliceId,"%06X", sliceList[i].networkSliceIdentifier.sd);
            if(Slice == 0)
            {
                ret = false;
            }
            else if (JsonHelper::addJsonNodeToArray(networkSliceArray, Slice) == 0)
            {
                ret = false;
            }

            else if (JsonHelper::addNodeToObject(Slice, \
                                                "DRB.UEThpDl.SNSSAI", \
                                                 sliceList[i].DRB_UEThpDl_SNSSAI) == 0)
            {
                ret = false;
            }

            else if (JsonHelper::addNodeToObject(Slice, \
                                                "DRB.UEThpUl.SNSSAI", \
                                                sliceList[i].DRB_UEThpUl_SNSSAI) == 0)
            {
                ret = false;
            }
            else if (JsonHelper::addNodeToObject(Slice, \
                                                "networkSliceIdentifier", \
                                                 networkSliceId) == 0)
            {
                ret = false;
            }
        }
    }

    if(JsonHelper::addNodeToObject(measurementFields, \
                                   "measurementInterval", \
                                    MEASUREMENT_INTERVAL) == 0)
    {
        ret = false;
    }

    return ret;
}

/**********************************************************************
         End of file
**********************************************************************/
