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
/* This is a singleton class to load and store all configuration parameters */

#include "ConfigLoader.hpp"

/* Constructor */
ConfigLoader::ConfigLoader() 
    : mNetconfConfig(NETCONF_CONFIG), 
    mOamConfig(OAM_VES_CONFIG),
    mSmoConfig(SMO_VES_CONFIG)
{
};
/* Default Destructor */
ConfigLoader::~ConfigLoader()
{
}
 
/*******************************************************************
 *
 * @brief Load all configuration files 
 *
 * @details
 *
 *    Function : loadConfigurations
 *
 *    Functionality:
 *      - Loads all the configuration files
 *
 *
 * @params[in] void
 * @return true  - success
 *         false - failure
 ******************************************************************/
bool ConfigLoader::loadConfigurations() {
    return mOamConfig.loadConfigFile() 
            &&
            mSmoConfig.loadConfigFile()
            &&
            mNetconfConfig.loadConfigFile();
}

/*******************************************************************
 *
 * @brief Get OAM Ves collector configuration 
 *
 * @details
 *
 *    Function : getOamConfigFile
 *
 *    Functionality:
 *      -  Returns the OAM config object
 *
 *
 * @params[in] void
 * @return const reference to VesConfigFile
 *         
 ******************************************************************/
const VesConfigFile& ConfigLoader::getOamConfigFile() const{
    return mOamConfig;
}

/*******************************************************************
 *
 * @brief Get SMO Ves collector configuration
 *
 * @details
 *
 *    Function : getsmoConfigFile
 *
 *    Functionality:
 *      -  Returns the Smo config object
 *
 *
 * @params[in] void
 * @return const reference to VesConfigFile
 *         
 ******************************************************************/
const VesConfigFile& ConfigLoader::getSmoConfigFile() const{
    return mSmoConfig;
}

/*******************************************************************
 *
 * @brief Get Netconf server configuration
 *
 * @details
 *
 *    Function : getNetconfConfigFile
 *
 *    Functionality:
 *      -  Returns the Netconf config object
 *
 *
 * @params[in] void
 * @return const reference to NetconfConfigFile
 *         
 ******************************************************************/
const NetconfConfigFile& ConfigLoader::getNetconfConfigFile() const{
    return mNetconfConfig;
}

/**********************************************************************
  End of file
 **********************************************************************/
