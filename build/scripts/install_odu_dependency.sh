################################################################################
#   Copyright (c) [2020] [Radisys]                                             #
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
# This script is used to setup environment for execution of ODU-High
#!/bin/bash

echo "***** Installation Start *****"

if [ -f /etc/os-release ]; then
   . /etc/os-release
   OS=$ID
fi

echo "===> Installing GCC"
if [ $OS == "ubuntu" ] ; then
   sudo apt-get install -y build-essential
else
   sudo yum groups mark install -y “Development Tools”
fi
echo""

echo "===> Installing LKSCTP"
if [ $OS == "ubuntu" ] ; then
   sudo apt-get install -y libsctp-dev
else
   sudo yum install -y lksctp-tools-devel
fi
echo ""

echo "===> Installing PCAP"
if [ $OS == "ubuntu" ] ; then
   sudo apt-get install -y libpcap-dev
else
   sudo yum install -y libpcap-devel
fi
echo ""

echo "===> Installing XTERM"
if [ $OS == "ubuntu" ] ; then
   sudo apt-get install -y xterm
else
   sudo yum install -y xterm
fi
echo ""

echo "***** Installation Complete *****"

################################################################################
#                              End of file                                     #
################################################################################
