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

/* This file contains functions to support Json related operation (read/write)*/

#include <bits/stdc++.h>
#include "JsonHelper.hpp"
#include "VesUtils.hpp"

using namespace std;

/* Default constructor */
JsonHelper::JsonHelper()
{

}

/* Default Destructor */
JsonHelper::~JsonHelper()
{

}


/*******************************************************************
 *
 * @brief wrapper of cJSON_CreateObject
 *
 * @details
 *
 *    Function : createNode
 *
 *    Functionality:
 *      - wrapper of cJSON_CreateObject
 *
 * @params[in] void
 * @return pointer to cJSON object     - success
 *         NULL                      - failure
 *
 * ****************************************************************/

cJSON * JsonHelper::createNode()
{
   return  cJSON_CreateObject();
}

/*******************************************************************
 *
 * @brief wrapper of cJSON_AddStringToObject
 *
 * @details
 *
 *    Function : addNodeToObject
 *
 *    Functionality:
 *      - wrapper of cJSON_AddStringToObject
 *
 * @params[in] void
 * @return pointer to cJSON object     - success
 *         NULL                      - failure
 *
 * ****************************************************************/

cJSON* JsonHelper::addNodeToObject(cJSON * parent, \
                                          const char * nodeName, \
                                          const char* value)
{
   return cJSON_AddStringToObject(parent, nodeName, value);
}

/*******************************************************************
 *
 * @brief wrapper of cJSON_AddNumberToObject
 *
 * @details
 *
 *    Function : addNodeToObject
 *
 *    Functionality:
 *      - wrapper of cJSON_AddNumberToObject
 *
 * @params[in] void
 * @return pointer to cJSON object     - success
 *         NULL                      - failure
 *
 * ****************************************************************/

cJSON* JsonHelper::addNodeToObject(cJSON * parent, \
                                          const char * nodeName, double value)
{
   return cJSON_AddNumberToObject(parent, nodeName, (double) value);
}

/*******************************************************************
 *
 * @brief get value of the provided node
 *
 * @details
 *
 *    Function : getObject
 *
 *    Functionality:
 *      - fetch the value of the provided node
 *
 * @params[in] void
 * @return pointer to string value         - success
 *         NULL                            - failure
 *
 * ****************************************************************/

char* JsonHelper::getObject(cJSON *json, const char *node)
{
   cJSON *object;
   char * value = NULL;
   object = cJSON_GetObjectItem(json, node);
   if(object) 
   {
      value = object->valuestring;
      O1_LOG("O1 VES : [ %s] : [%s]\n",node, value );
   }
   else
      O1_LOG("O1 VES : node [ %s] not found\n",node);
   return value;
}

/*******************************************************************
 *
 * @brief get value of the provided node
 *
 * @details
 *
 *    Function : getObject
 *
 *    Functionality:
 *      - fetch the value of the provided node
 *
 * @params[in] void
 * @return pointer to string value         - success
 *         NULL                            - failure
 *
 * ****************************************************************/

cJSON* JsonHelper::readConfig(const char * fileName)
{
   std::fstream fs(fileName, std::ios::in | std::ios::binary);

   if (!fs) {
       O1_LOG("O1 VES : json can NOT open file %s\n", fileName);
        return NULL;
   }

   std::stringstream iss;

   iss << fs.rdbuf();
   
    cJSON *json = cJSON_Parse(iss.str().c_str());
    return json;
}

/**********************************************************************
  End of file
 **********************************************************************/
