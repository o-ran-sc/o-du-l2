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

#ifndef __VES_CONFIG_FILE_HPP__
#define __VES_CONFIG_FILE_HPP__

#include <string>
#include "ConfigFile.hpp"


using std::string;

class VesConfigFile : public ConfigFile {
public:
    //Constructor and Destructor
    VesConfigFile(string filepath);
    ~VesConfigFile();

    //Getters
    string getVesServerIp()const;
    string getVesServerPort()const;
    string getVesServerUsername()const;
    string getVesServerPassword()const;

    //member functions
    bool loadConfigFile()override;
    
private: 
    //member variables
    string mVesServerIp;
    string mVesServerPort;
    string mVesServerUsername;
    string mVesServerPassword;
};
#endif

/**********************************************************************
  End of file
 **********************************************************************/
