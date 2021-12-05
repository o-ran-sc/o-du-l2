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

/* This file defines the SliceMetrics which holds the Network Slice Metrics */ 

#include "SliceMetrics.hpp"

/* Default Constructor */
SliceMetrics::SliceMetrics() {

}

/* Default Destructor */
SliceMetrics::~SliceMetrics() {

}

/*******************************************************************
 *
 * @brief Add the slice metric record to the list 
 *
 * @details
 *
 *    Function : addMetric
 *
 *    Functionality:
 *      - Adds slice metric record to the list
 *
 *
 * @params[in] SliceMetricRecord
 * @return void
 ******************************************************************/
void SliceMetrics::addMetric(SliceMetricRecord& sliceMetricRecord) {

   mSliceList.push_back(sliceMetricRecord);

}

/*******************************************************************
 *
 * @brief Returns the slice metric list
 *
 * @details
 *
 *    Function : getSliceMetrics
 *
 *    Functionality:
 *      - Returns the slice metric vector list 
 *
 *
 * @params[in] void
 * @return const reference to vector<SliceMetricRecord>
 ******************************************************************/
const vector<SliceMetricRecord>& SliceMetrics::getSliceMetrics() const {
   return mSliceList;
}

/**********************************************************************
         End of file
**********************************************************************/
