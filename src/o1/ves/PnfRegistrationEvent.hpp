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

/* This file contains macros and functions to support the preparation of pnf
   Registration VES Event*/


#ifndef __PNF_REGISTRATION_EVENT_HPP__
#define __PNF_REGISTRATION_EVENT_HPP__

#include <iostream>
#include <string>
#include <cjson/cJSON.h>
#include <stdlib.h>
#include <stdio.h>
#include "VesUtils.hpp"
#include "VesEvent.hpp"
#include "Message.hpp"

#define MAX_TIME_STR 11

using namespace std;

class PnfRegistrationEvent : public VesEvent
{

   public:
      /* Default constructor/Destructor */
      PnfRegistrationEvent();
      ~PnfRegistrationEvent();

   protected:
      bool prepareEventFields(const Message* msg = NULL);

   private:
      bool prepareAdditionalFields(cJSON *addFields);
      string getCurrentDate();
      string getNetconfMacAddr();
      string getNetconfV4ServerIP();
      string getNetconfV6ServerIP();
      string getNetconfPort();
      string getUsername();
      string getPassword();
      string getSerialNumber();
      string getUnitFamily();
      void getNetconfConfig();

      //member variables
      string mNetconfMacAddr;
      string mNetconfIpv4;
      string mNetconfIpv6;
      string mNetconfPort;
      string mNetconfUsername;
      string mNetconfPassword;
};

#endif
/**********************************************************************
  End of file
 **********************************************************************/
