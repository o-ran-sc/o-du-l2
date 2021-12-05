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

#include <bits/stdc++.h>
#include "JsonHelper.hpp"
#include "VesUtils.hpp"

using namespace std;

/*******************************************************************
 *
 * @brief wraps cJSON_CreateObject cJSON library function
 *
 * @details
 *
 *    Function : createNode
 *
 *    Functionality:
 *      - wraps cJSON_CreateObject cJSON library function
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
 * @brief wraps cJSON_AddStringToObject cJSON library function
 *
 * @details
 *
 *    Function : addNodeToObject
 *
 *    Functionality:
 *      - wraps cJSON_AddStringToObject cJSON library function
 *
 * @params[in] cJSON * parent, const char * nodeName, const char* value
 * @return pointer to cJSON object     - success
 *         NULL                        - failure
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
 * @brief wraps cJSON_AddNumberToObject cJSON library function
 *
 * @details
 *
 *    Function : addNodeToObject
 *
 *    Functionality:
 *      - wraps cJSON_AddNumberToObject cJSON library function
 *
 * @params[in] cJSON * parent, const char * nodeName, double value
 * @return pointer to cJSON object     - success
 *         NULL                        - failure
 *
 * ****************************************************************/

cJSON* JsonHelper::addNodeToObject(cJSON * parent, \
                                          const char * nodeName, double value)
{
   return cJSON_AddNumberToObject(parent, nodeName, (double) value);
}


/*******************************************************************
 *
 * @brief wraps cJSON_AddItemToObject cJSON library function
 *
 * @details
 *
 *    Function : addJsonNodeToObject
 *
 *    Functionality:
 *      - wraps cJSON_AddItemToObject cJSON library function
 *
 * @params[in] cJSON * parent, const char * nodeName, cJSON * node
 * @return cJSON_bool non zero  - success
 *         cJSON_bool 0         - failure
 *
 * ****************************************************************/

cJSON_bool JsonHelper::addJsonNodeToObject(cJSON * parent, \
                                   const char * nodeName, cJSON * node)
{
   return cJSON_AddItemToObject(parent, nodeName, node);
}

/*******************************************************************
 *
 * @brief wraps cJSON_Delete cJSON library function
 *
 * @details
 *
 *    Function : deleteNode
 *
 *    Functionality:
 *      - wrapper of cJSON_AddNumberToObject
 *
 * @params[in] cJSON * node
 * @return void
 *
 * ****************************************************************/


void JsonHelper::deleteNode(cJSON * node)
{
   cJSON_Delete(node);
}

/*******************************************************************
 *
 * @brief wraps cJSON_PrintUnformatted cJSON library function
 *
 * @details
 *
 *    Function : addNodeToObject
 *
 *    Functionality:
 *      - wraps cJSON_PrintUnformatted cJSON library function
 *
 * @params[in] cJSON * node
 * @return char pointer to string  - success
 *         nullptr                 - failure
 *
 * ****************************************************************/

char *JsonHelper::printUnformatted(cJSON * node)
{
   return cJSON_PrintUnformatted(node);
}

/*******************************************************************
 *
 * @brief wraps cJSON_PrintUnformatted cJSON library function
 *
 * @details
 *
 *    Function : addNodeToObject
 *
 *    Functionality:
 *      - wraps cJSON_PrintUnformatted cJSON library function
 *
 * @params[in] cJSON * node
 * @return char pointer to json message  - success
 *         nullptr                       - failure
 *
 * ****************************************************************/

char *JsonHelper::print(cJSON * node)
{
   return cJSON_Print(node);
}

/*******************************************************************
 *
 * @brief wraps cJSON_PrintUnformatted cJSON library function
 *
 * @details
 *
 *    Function : addNodeToObject
 *
 *    Functionality:
 *      - wraps cJSON_PrintUnformatted cJSON library function
 *
 * @params[in] void
 * @return char pointer to string  - success
 *         nullptr                 - failure
 *
 * ****************************************************************/

const char *JsonHelper::getError()
{
   return cJSON_GetErrorPtr();
}


/*******************************************************************
 *
 * @brief get value of the provided node
 *
 * @details
 *
 *    Function : getValue
 *
 *    Functionality:
 *      - fetch the value of the provided node
 *
 * @params[in] void
 * @return pointer to string value         - success
 *         NULL                            - failure
 *
 * ****************************************************************/

string JsonHelper::getValue(cJSON *json, const char *node)
{
   cJSON *object;
   string value = "";
   object = cJSON_GetObjectItem(json, node);
   if(object)
   {
      value = object->valuestring;
      O1_LOG("O1 VES : [ %s] : [%s]\n",node, value.c_str() );
   }
   else
      O1_LOG("O1 VES : node [ %s] not found\n",node);
   return value;
}

/*******************************************************************
 *
 * @brief wraps cJSON_GetObjectItem cJSON library function
 *
 * @details
 *
 *    Function : addNodeToObject
 *
 *    Functionality:
 *      - wraps cJSON_GetObjectItem cJSON library function
 *
 * @params[in] cJSON *json, const char *node
 * @return pointer to cJSON node   - success
 *         nullptr                 - failure
 *
 * ****************************************************************/

cJSON * JsonHelper::getNode(cJSON *json, const char *node)
{
   return cJSON_GetObjectItem(json, node);
}


/*******************************************************************
 *
 * @brief reads json file
 *
 * @details
 *
 *    Function : read
 *
 *    Functionality:
 *      - opens and reads json file and returns point to that file.
 *
 * @params[in] const char * fileName
 * @return point to json file root node    - success
 *         NULL                            - failure
 *
 * ****************************************************************/

cJSON* JsonHelper::read(const char * fileName)
{
   std::fstream fs(fileName, std::ios::in | std::ios::binary);

   if (!fs) {
      O1_LOG("\nO1 JsonHelper : Cannot open file %s", fileName);
      return NULL;
   }

   std::stringstream iss;

   iss << fs.rdbuf();

   cJSON *json = cJSON_Parse(iss.str().c_str());
   return json;
}

cJSON* JsonHelper::createArray() 
{
   return cJSON_CreateArray();
}

cJSON_bool JsonHelper::addJsonNodeToArray(cJSON * array, cJSON* node)
{
   return cJSON_AddItemToArray(array, node);
}



/**********************************************************************
  End of file
 **********************************************************************/
