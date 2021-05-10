/*******************************************************************************
################################################################################
#   Copyright (c) [2021] [HCL Technologies Ltd.]                               #
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

/* This file contains macros and functions to support the preparation of pnf Registration VES Event*/


#ifndef __PNF_REGISTRATION_HPP__
#define __PNF_REGISTRATION_HPP__

#include <iostream>
#include <string>
#include <cjson/cJSON.h>
#include <stdlib.h>
#include <stdio.h>
#include "VesUtils.hpp"
#include "VesEvent.hpp"

#define MAX_TIME_STR 11

using namespace std;

class PnfRegistration : public VesEvent
{

   public:
      PnfRegistration();
      ~PnfRegistration();
      bool prepareEventFields();

   private:
      bool prepareAdditionalFields(cJSON *addFields);
      bool getCurrentDate(char * str);
      string getNetconfV4ServerIP();
      string getNetconfV6ServerIP();
      string getNetconfPort();
      string getUsername();
      string getPassword();
      bool readConfigFile();
      //member variables   
      string mNetconfIp;
      string mNetconfPort;
      string mUsername;
      string mPassword;
};

#endif
/**********************************************************************
  End of file
 **********************************************************************/
