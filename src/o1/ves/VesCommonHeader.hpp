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

/* This file contains functions to support the preparation of VES common header
   parameters*/

#ifndef __VES_COMMON_HEADER_HPP__
#define __VES_COMMON_HEADER_HPP__


#include <iostream>
#include <string>
#include <cstdint>
#include <cjson/cJSON.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
#include "VesUtils.hpp"

using namespace std;

class VesCommonHeader{

   public:
      /* Default constructor/Destructor*/
      VesCommonHeader(){}
      ~VesCommonHeader(){}

      bool prepare(cJSON *node, VesEventType type);

   private:
      uint16_t getSequenceNo();
      uint16_t nextSequenceNo();
      string getEventTypeToStr();
      string getEventType();
      string getEventId();
      string getPriority();
      string getEventName();
      string getReportingEntityName();
      string getSourceName();
      string getNamingCode();
      uint64_t getEpochTime();
      time_t getCurrentTime();
	   string formatTime(time_t);
      uint64_t mLastEpochTime;

      VesEventType mEventType;
};

#endif
/**********************************************************************
  End of file
 **********************************************************************/
