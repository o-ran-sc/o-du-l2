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

#ifndef __CONFIG_LOADER_HPP__
#define __CONFIG_LOADER_HPP__

#include <string>
#include "VesConfigFile.hpp"
#include "NetconfConfigFile.hpp"
#include "VesUtils.hpp"
#include "Singleton.hpp"

using std::string;

class ConfigLoader : public Singleton<ConfigLoader> {
private: 
    ConfigLoader();
    ~ConfigLoader();

    VesConfigFile mOamConfig;
    VesConfigFile mSmoConfig;
    NetconfConfigFile mNetconfConfig;

public:
    friend Singleton<ConfigLoader>;

    const VesConfigFile& getOamConfigFile()const;
    const VesConfigFile& getSmoConfigFile()const;
    const NetconfConfigFile& getNetconfConfigFile()const;

    bool loadConfigurations();
};

#endif

/**********************************************************************
  End of file
 **********************************************************************/
