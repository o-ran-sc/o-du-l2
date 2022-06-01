#!/bin/bash
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

#initialize globle variables
CURRENT_DIR=$PWD
ROOT_DIR=$CURRENT_DIR/../../
CONFIG_PATH=$ROOT_DIR/bin/odu/config/
YANG_PATH_3GPP="https://forge.3gpp.org/rep/sa5/MnS/-/raw/0032d2835382c47bc402c077b40ef71d77c9a3c9/yang-models"

#list of 3gpp yang models
declare -a YANG_MODEL_3GPP=( "_3gpp-common-yang-types.yang"
                        "_3gpp-common-top.yang"
                        "_3gpp-common-measurements.yang"
                        "_3gpp-common-trace.yang"
                        "_3gpp-common-managed-function.yang"
                        "_3gpp-common-subscription-control.yang"
                        "_3gpp-common-fm.yang"
                        "_3gpp-common-managed-element.yang"
                        "_3gpp-5g-common-yang-types.yang"
                        "_3gpp-nr-nrm-rrmpolicy.yang"
                        "_3gpp-nr-nrm-gnbdufunction.yang"
                        "_3gpp-nr-nrm-nrcelldu.yang")


#list of ORAN yang models
declare -a YANG_MODEL_ORAN=( "o-ran-sc-odu-alarm-v1.yang"
                        "o-ran-sc-du-hello-world.yang"
			"o-ran-sc-odu-interface-v1.yang")


#list of config files
declare -a CONFIGURATION_ARRAY=( "startup_config.xml"
                                 "nacm_config.xml"
                                 "netconf_server_ipv6.xml")

#list of modules corresponding to the above config files
declare -a MODULE_ARRAY=( "o-ran-sc-odu-interface-v1"
                          "ietf-netconf-acm"
                          "ietf-netconf-server")


#check 3GPP yang module if not available then  downalod and update
check3gppYang()
{
   #check 3GPP yang modules available
   for yang in "${YANG_MODEL_3GPP[@]}"
   do

      if [ ! -f "$ROOT_DIR/build/yang/$yang" ]
      then
            wget $YANG_PATH_3GPP/$yang -P $ROOT_DIR/build/yang
      fi

   done
}

#3GPP yang modules having some warning so fix it.
updateYang()
{
   sed -i -e "s/'\.\/tj/'\.\.\/tj/g" $ROOT_DIR/build/yang/_3gpp-common-trace.yang
   sed -i -e "s/\ \.\/tj/\ \.\.\/tj/g" $ROOT_DIR/build/yang/_3gpp-common-trace.yang
   sed -i -e 's/"IMMEDIATE_MDT"/"IMMEDIATE_MDT_ONLY"/g' $ROOT_DIR/build/yang/_3gpp-common-trace.yang
   sed -i -e 's/"TRACE"/"TRACE_ONLY"/g' $ROOT_DIR/build/yang/_3gpp-common-trace.yang

   echo "updateYang done"
}


#initialize the variable
init()
{
   if [ -d "$ROOT_DIR/bin/odu/config" ]
   then
      CONFIG_PATH=$ROOT_DIR/bin/odu/config/
      echo "CONFIG_PATH = $CONFIG_PATH"
   else
      CONFIG_PATH=$ROOT_DIR/build/config/
      echo "CONFIG_PATH = $CONFIG_PATH"
   fi
      echo "CONFIG_PATH = $CONFIG_PATH"

}

#install 3GPP and ORAN yang modules
installYang()
{
   echo "### install yang modules ###"
   #install 3GPP yang modules
   for yang in "${YANG_MODEL_3GPP[@]}"
   do
      sysrepoctl -i      $ROOT_DIR/build/yang/$yang
   done

   #install ORAN yang modules
   for yang in "${YANG_MODEL_ORAN[@]}"
   do
      sysrepoctl -i      $ROOT_DIR/build/yang/$yang
   done
}

#load initial configuration
loadConfig()
{
   echo "### load initial configuration ###"

   for ((i=0;i<${#CONFIGURATION_ARRAY[@]};i++))
   do
      sysrepocfg --import=$CONFIG_PATH/${CONFIGURATION_ARRAY[$i]} -v 3 --datastore running --module  ${MODULE_ARRAY[$i]}
   done

   echo "### load initial configuration done ###"
}

init
check3gppYang
updateYang
installYang
loadConfig

################################################################################
#                              End of file                                     #
################################################################################
