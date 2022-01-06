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

/* This file contains Base call functions to support send VES Event*/


#ifndef __VES_EVENT_HPP__
#define __VES_EVENT_HPP__

#include <iostream>
#include <string>
#include <cjson/cJSON.h>
#include <stdlib.h>
#include <stdio.h>
#include "VesUtils.hpp"
#include "VesCommonHeader.hpp"
#include "HttpClient.hpp"
#include "Message.hpp"
#include "ConfigLoader.hpp"

using namespace std;

class VesEvent{

   public:
      VesEvent();
      VesEvent(VesEventType);
      virtual ~VesEvent();
      void init();
      bool prepare(const Message* msg = NULL);
      bool send();

   protected:
      virtual bool prepareEventFields(const Message* msg = NULL) = 0;
      virtual void getConfig();
      virtual void createUrl();
      VesEventType mVesEventType;
      cJSON* mVesEventFields;
  	   string mVesServerIp;
      string mVesServerPort;
      string mVesServerUsername;
      string mVesServerPassword;
      string mVesUrl;
	  
   private:
      string getEventFieldName();
      char * mSendData;
      HttpClient *mHttpClient;
      
};

#endif
/**********************************************************************
  End of file
 **********************************************************************/
