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

#ifndef __NETCONF_CONFIG_FILE_HPP__
#define __NETCONF_CONFIG_FILE_HPP__

#include "ConfigFile.hpp"
#include <string>


using std::string;

class NetconfConfigFile : public ConfigFile {
public:  
    //constructor & Destructor
    NetconfConfigFile(string filepath);
    ~NetconfConfigFile();

    //member functions
    bool loadConfigFile() override;


    //Getters
    string getNetconfMacAddr() const;
    string getNetconfIpv4() const;
    string getNetconfIpv6() const;
    string getNetconfPort() const;
    string getNetconfUsername() const;
    string getNetconfPassword() const;
    
private: 
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
