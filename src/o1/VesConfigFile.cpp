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
/* This class defines functions to read Ves Config files */

#include "VesConfigFile.hpp"
#include "JsonHelper.hpp"
#include "GlobalDefs.hpp"


/* Constructor */
VesConfigFile::VesConfigFile(string filepath)
    :ConfigFile(filepath) 
{
}
/* Default Destructor */
VesConfigFile::~VesConfigFile()
{
}

//Getters
string VesConfigFile::getVesServerIp() const
{
    return mVesServerIp;
}
string VesConfigFile::getVesServerPort() const
{
    return mVesServerPort;
}
string VesConfigFile::getVesServerUsername() const
{
    return mVesServerUsername;
}
string VesConfigFile::getVesServerPassword() const
{
    return mVesServerPassword;
}

/*******************************************************************
*
* @brief load Ves config file
*
* @details
*
* Function : loadConfigFile
*
* Functionality:
* - read Ves Config file and store its contents
*
* @params[in] IN - NULL
* @return true - success
*         false - failure
*
* ****************************************************************/

bool VesConfigFile::loadConfigFile()
{
    cJSON *json = JsonHelper::read(mFilepath.c_str());
    if (json == NULL)
    {
        O1_LOG("\nO1 VesEvent : Error reading config file  :%s", JsonHelper::getError());
        return false;
    }
    else
    {
        cJSON *rootNode = NULL;
        rootNode = JsonHelper::getNode(json, "vesConfig");
        if (rootNode)
        {
            O1_LOG("\nO1 VesEvent : Reading Config.json file\n");
            mVesServerIp = JsonHelper::getValue(rootNode, "vesV4IpAddress");
            mVesServerPort = JsonHelper::getValue(rootNode, "vesPort");
            mVesServerUsername = JsonHelper::getValue(rootNode, "username");
            mVesServerPassword = JsonHelper::getValue(rootNode, "password");
        }
        else
        {
            O1_LOG("\nO1 VesEvent : Config Object is not available in config file");
            return false;
        }
    }
    JsonHelper::deleteNode(json);
    return true;
}

/**********************************************************************
  End of file
 **********************************************************************/
