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


#ifndef __SLICE_MEASUREMENT_EVENT_STD_DEF_HPP__
#define __SLICE_MEASUREMENT_EVENT_STD_DEF_HPP__

#include <iostream>
#include <string>
#include "VesUtils.hpp"
#include "JsonHelper.hpp"
#include "PerfMeasurementEvent.hpp"
#include <cjson/cJSON.h>
#include "SliceMetrics.hpp"
#include "PmInterface.h"

using namespace std;

//Macros
#define SLICE_SCHEMA "https://gerrit.o-ran-sc.org/r/gitweb?p=scp/oam/modeling.git;a=blob_plain;f=data-model/oas3/experimental/o-ran-sc-du-hello-world-pm-streaming-oas3.yaml"
#define SLICE_ID "pm1_1638984365"
#define ADMINISTRATIVE_STATE "unlocked"
#define OPERATIONAL_STATE "enabled"
#define USER_LABEL "pm-1"
#define GRANULARITY_PERIOD 5.0
#define RANDOM_NUM 12345
#define THROUGHPUT_UNIT "kbit/s"
#define MEAS_REF "measurement-type-instance-reference"
#define YANG_NAME "o-ran-sc-du-hello-world"
#define MAX_THP_TYPE 2

class SliceMeasurementEventStdDef : public PerfMeasurementEvent
{

   public:
      /* Default constructor/Destructor */
      SliceMeasurementEventStdDef();
      ~SliceMeasurementEventStdDef();

      std::string getISOEventTime();

   protected:
      bool prepareEventFields(const Message* msg = NULL);

   private:
      string getMeasPath(string str, int sd, int sst);

};

#endif
/**********************************************************************
  End of file
 **********************************************************************/
