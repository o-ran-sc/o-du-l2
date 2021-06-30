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
# This script is used to install yang module and load initial configuration
#!/bin/bash


CURRENT_DIR=$PWD
ROOT_DIR=$CURRENT_DIR/../../

#load yand models
echo "### loading yang model ###"
sysrepoctl -i $ROOT_DIR/build/yang/o-ran-sc-odu-alarm-v1.yang
sysrepoctl -i $ROOT_DIR/build/yang/o-ran-sc-du-hello-world.yang
sysrepoctl -i $ROOT_DIR/build/yang/o-ran-sc-odu-interface-v1.yang
echo "### loading yang model Done###"

#load initial configuration
echo "### loading initial configuration ###"
sysrepocfg --import=$ROOT_DIR/bin/odu/config/startup_config.xml -v 3 --datastore running --module  o-ran-sc-odu-interface-v1
sysrepocfg --import=$ROOT_DIR/bin/odu/config/nacm_config.xml -v 3 --datastore running --module  ietf-netconf-acm
sysrepocfg --import=$ROOT_DIR/bin/odu/config/netconf_server_ipv6.xml -v 3 --datastore running --module  ietf-netconf-server
echo "### loading initial configuration done ###"

################################################################################
#                              End of file                                     #
################################################################################
