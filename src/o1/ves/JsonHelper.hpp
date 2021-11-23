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

/* This file contains functions to support Json related operation (read/write)*/

#ifndef __JSON_HELPER_HPP__
#define __JSON_HELPER_HPP__

#include <iostream>
#include <stdlib.h>
#include <assert.h>
#include <cjson/cJSON.h>
#include <string>

class JsonHelper
{

   public:
      /* Default constructor/Destructor */
      JsonHelper(){}
      ~JsonHelper(){}
      static cJSON * createNode();
      static cJSON* addNodeToObject(cJSON * parent, const char * nodeName, \
                                  const char* value);
      static cJSON* addNodeToObject(cJSON * parent, \
                                  const char * nodeName, double value);

      static void deleteNode(cJSON * node);
      static cJSON_bool addJsonNodeToObject(cJSON * parent, \
                             const char * nodeName, cJSON * node);
      static cJSON* read(const char * fileName);
      static std::string getValue(cJSON *json, const char *node);
      static cJSON * getNode(cJSON *json, const char *node);
      static char *printUnformatted(cJSON * node);
      static char *print(cJSON * node);
      static const char *getError();
      static cJSON* createArray();
      static cJSON_bool addJsonNodeToArray(cJSON * array, cJSON* node);


};

#endif
/**********************************************************************
  End of file
 **********************************************************************/
