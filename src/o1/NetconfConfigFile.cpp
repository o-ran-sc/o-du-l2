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
/* This class defines functions to read Netconf Config file */

#include "NetconfConfigFile.hpp"
#include "JsonHelper.hpp"
#include "GlobalDefs.hpp"


// Constructor
NetconfConfigFile::NetconfConfigFile(string filepath)
    :ConfigFile(filepath) 
{
}
// Default Destructor
NetconfConfigFile::~NetconfConfigFile()
{
}

//Getters
string NetconfConfigFile::getNetconfMacAddr() const
{
    return mNetconfMacAddr;
}
string NetconfConfigFile::getNetconfIpv4() const
{
    return mNetconfIpv4;
}
string NetconfConfigFile::getNetconfIpv6() const
{
    return mNetconfIpv6;
}
string NetconfConfigFile::getNetconfPort() const
{
    return mNetconfPort;
}
string NetconfConfigFile::getNetconfUsername() const
{
    return mNetconfUsername;
}
string NetconfConfigFile::getNetconfPassword() const
{
    return mNetconfPassword;
}

/*******************************************************************
*
* @brief load Netconf config file
*
* @details
*
* Function : loadConfigFile
*
* Functionality:
* - read Netconf Config file and store its contents
*
* @params[in] IN - NULL
* @return true - success
*         false - failure
*
* ****************************************************************/

bool NetconfConfigFile::loadConfigFile()
{
   cJSON *json = JsonHelper::read(mFilepath.c_str());
   if(json == NULL) {
       O1_LOG("\nO1 PnfRegistrationEvent : Config file reading error is  :%s", JsonHelper::getError());
    return false;
    }
    else {
       cJSON *rootNode = NULL;
       rootNode = JsonHelper::getNode(json, "NetconfServer");
       if(rootNode) {
          O1_LOG("\nO1 PnfRegistrationEvent : Reading NetconfServer config file");
          mNetconfMacAddr = JsonHelper::getValue(rootNode, "MacAddress");
          mNetconfIpv4 = JsonHelper::getValue(rootNode, "NetconfServerIpv4");
          mNetconfIpv6 = JsonHelper::getValue(rootNode, "NetconfServerIpv6");
          mNetconfPort = JsonHelper::getValue(rootNode, "NetconfPort");
          mNetconfUsername = JsonHelper::getValue(rootNode, "NetconfUsername");
          mNetconfPassword = JsonHelper::getValue(rootNode, "NetconfPassword");
      }
      else {
         O1_LOG("\nO1 PnfRegistrationEvent : smoConfig Object is not available in config file");
         return false;
      }
   }
   JsonHelper::deleteNode(json);
   return true;
}

/**********************************************************************
  End of file
 **********************************************************************/
