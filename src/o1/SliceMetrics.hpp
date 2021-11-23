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

#ifndef __SLICE_METRICS_HPP__
#define __SLICE_METRICS_HPP__

#include <string>
#include <vector>
#include "Message.hpp"
#include "PmInterface.h"

using namespace std;

class SliceMetrics : public Message {
   
public:
    SliceMetrics();
    ~SliceMetrics();

    const vector<SliceMetricRecord>& getSliceMetrics() const;
    void addMetric(SliceMetricRecord& sliceMetricRecord);

private: 
    vector<SliceMetricRecord> mSliceList;

};

#endif

/**********************************************************************
End of file
**********************************************************************/
