################################################################################
#   Copyright (c) [2020-2021] [HCL Technologies Ltd]                           #
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

#!/bin/bash

# Pre-requisite script for installing Netconf Libraries
# and compiling the O1 module

#variable declaration
CURRENT_PATH=`pwd`
HOME="$CURRENT_PATH/../.."
MAKE_PATH="$HOME/build/o1"
NETCONF_PATH="$HOME/build/o1/netconf"
SYSREPOCTL_PATH="$NETCONF_PATH/sysrepo/build/sysrepoctl"
YANG_PATH="$HOME/build/o1/yang"

INSTALL="netconf"
CLEANUP="cleanup"
SUDO="sudo"

#logging functions
log_error(){
   echo ""
   echo -e "\e[1;31m *****  $1  ***** \e[0m"
   echo ""
}
log_warning() {
   echo ""
   echo -e "\e[1;35m *****  $1  ***** \e[0m"
   echo ""
}
log_success() {   
   echo ""
   echo -e "\e[1;32m *****  $1  ***** \e[0m"
   echo ""
}
log()         {
   echo -e "$1 "
}



#functions definitions
#TBD: install only mandatory packages

#install pre-requisite packages
prerequisite_o1() {
        $SUDO apt-get update
        $SUDO apt-get install -y libpcap-dev
        $SUDO apt-get install libpcap-dev gcc-multilib g++-multilib libsctp-dev
}

prerequisite_netconf() {
      $SUDO apt-get update

      $SUDO apt-get install -y jq \
      cmake \
      build-essential \
      supervisor \
      libpcre3-dev \
      pkg-config \
      libavl-dev \
      libev-dev \
      libprotobuf-c-dev \
      protobuf-c-compiler \
      libssh-dev \
      libssl-dev \
      swig \
      iputils-ping \
      python-dev
}

#check return value
check_ret() {

   if [ $2 -eq 0 ]; then
      log_success " $1 INSTALLATION DONE"
   else
      log_error " $1 INSTALLATION FAILED "
      exit
   fi
}


#install netconf  libraries
install_netconf_lib() {

   #with sudo we can not create new user so we need to create it manually using 
   #root credentials.

      #$SUDO adduser --system netconf && \
      #      echo "netconf:netconf" | chpasswd

      #$SUDO mkdir -p /home/netconf/.ssh && \
      #      ssh-keygen -A && \
      #      ssh-keygen -t dsa -P '' -f /home/netconf/.ssh/id_dsa && \
      #      cat /home/netconf/.ssh/id_dsa.pub > /home/netconf/.ssh/authorized_keys

   if [[ "$CLEANUP" == "cleanup" ]]; then
      rm -rf $NETCONF_PATH
      log_warning "DELETED $NETCONF_PATH"
   fi

   mkdir -p $NETCONF_PATH

   #1. libssh
   cd $NETCONF_PATH && \
      git clone http://git.libssh.org/projects/libssh.git && \
      cd libssh; mkdir build; cd build && \
      cmake .. && \
      make && \
      $SUDO make install

   check_ret "LIBSSH" "$?"

   # libyang
   cd $NETCONF_PATH && \
      git clone https://github.com/CESNET/libyang.git && \
      cd libyang && mkdir build && cd build && \
      cmake -DGEN_LANGUAGE_BINDINGS=ON -DGEN_PYTHON_BINDINGS=OFF \
            -DCMAKE_BUILD_TYPE:String="Debug" -DENABLE_BUILD_TESTS=OFF .. && \
      make -j2 && \
      $SUDO make install && \
      $SUDO ldconfig

   check_ret "LIBYANG" "$?"

   # sysrepo
   cd $NETCONF_PATH && \
      git clone https://github.com/sysrepo/sysrepo.git && \
      cd sysrepo && sed -i -e 's/2000/30000/g;s/5000/30000/g' src/common.h.in && \
      mkdir build && cd build && \
      cmake -DGEN_LANGUAGE_BINDINGS=ON -DGEN_PYTHON_BINDINGS=OFF \
            -DCMAKE_BUILD_TYPE:String="Debug" -DENABLE_TESTS=OFF \
	    -DREPOSITORY_LOC:PATH=/etc/sysrepo .. && \
      make -j2 && \
      $SUDO make install && $SUDO make sr_clean && \
      $SUDO ldconfig

   check_ret "SYSREPO" "$?"

   # libnetconf2
   cd $NETCONF_PATH && \
      git clone https://github.com/CESNET/libnetconf2.git && \
      cd libnetconf2 && mkdir build && cd build && \
      cmake -DCMAKE_BUILD_TYPE:String="Debug" -DENABLE_BUILD_TESTS=OFF .. && \
      make -j2 && \
      $SUDO make install && \
      $SUDO ldconfig

   check_ret "LIBNETCONF2" "$?"

   # netopeer2
   cd $NETCONF_PATH && \
      git clone https://github.com/CESNET/Netopeer2.git && \
      cd Netopeer2 && mkdir build && cd build && \
      cmake -DCMAKE_BUILD_TYPE:String="Debug" -DNP2SRV_DATA_CHANGE_TIMEOUT=30000 \
            -DNP2SRV_DATA_CHANGE_WAIT=OFF .. && \
      make -j2 && \
      $SUDO make install -d
   check_ret "NETOPEER2" "$?"

}

#o1 compilation
compile_o1() {
   cd $HOME
   clean_o1=""
   if [[ "$CLEANUP" == "cleanup" ]]; then
      clean_o1="clean_o1"
   fi

   cd $MAKE_PATH 
   make $clean_o1 o1 MACHINE=BIT64

   cd $HOME
}

#pre-requisite packages installation
prerequisite() {

   if [[ "$INSTALL" == "o1" ]] || [[ "$INSTALL" == "all" ]]; then
   {
      prerequisite_o1
      log_success " O1 PREREQUISITE INSTALLATION DONE"
   }
   elif [[ "$INSTALL" == "netconf" ]] || [[ "$INSTALL" == "all" ]]; then
   {
      prerequisite_netconf
      log_success " NETCONF PREREQUISITE INSTALLATION DONE"
   }
   else
   {
      log_warning " NO INSTALLATION DEFINED install=$INSTALL "
   }
   fi
}

#install yang module

install_yang_module() {
   $SYSREPOCTL_PATH -i "$YANG_PATH/o-ran-sc-du-alarm-v1.yang"
}



#wait for ack of user
acknowledge() {
   echo "$1"
   read -n 1 -p "Press any key to continue, or CTRL+C to abort" mainmenuinout
   echo
   }

#show help for user to provide valid input
show_help(){
   echo "use following options "
   echo "-i  options : <netconf>, <o1>, <all>"
   echo "-c  options : <cleanup>, <no>"
   exit
}

main() {

   unset INSTALL CLEANUP

   while getopts "h?i:c:" opt; do
      case "$opt" in
         h|\?)
            show_help
            exit 0
            ;;
         i)  INSTALL=$OPTARG
            if [[ "$INSTALL" == "o1" ]] || [[ "$INSTALL" == "netconf" ]] || \
               [[ "$INSTALL" == "yang" ]] || [[ "$INSTALL" == "all" ]] ; then
               log "INSTALL =$INSTALL"
            else
               log_error "install parameter error"
               log_error " Please check -i param value which is=$INSTALL"
               show_help
            fi
            ;;
         c)  CLEANUP=$OPTARG
            if [[ "$CLEANUP" == "cleanup" ]] || [[ "$CLEANUP" == "no" ]] ; then
               log "CLEANUP =$CLEANUP"
            else
               log_error "cleanup parameter error"
               log_error " Please check -c param value which is=$CLEANUP"
               show_help
            fi

            ;;
      esac
   done

   shift $((OPTIND-1))

   [ "${1:-}" = "--" ] && shift

   log_success "MAIN: PARSING OF ARGUMENT DONE"

}

#start execution / function calls
if [[ "$#" -le 1 ]] ; then
   log_error " NUMBER OF PARAMETER : $# "
   show_help
fi

main $@

prerequisite

if [[ "$INSTALL" == "netconf" ]] || [[ "$INSTALL" == "all" ]] ; then
{
   log " NETCONF LIBS INSTALLATION PROCESSING"
   install_netconf_lib
   log_success " NETCONF LIBS INSTALLATION DONE"
}
fi

if [[ "$INSTALL" == "o1" ]] || [[ "$INSTALL" == "all" ]] ; then
{
   log " O1 COMPILATION PROCESSING "
   compile_o1
   log_success " O1 COMPILATION DONE "
}
fi

#yang installation yet not enabled
if [[ "$INSTALL" == "yang" ]] ; then
{
   log " YANG INSTALLATION PROCESSING "
   install_yang_module
   log_success " YANG INSTALLATION DONE "
}
fi


log_success " SCRIPT COMPLETED"
exit

#**********************************************************************
#    End of file
#**********************************************************************
