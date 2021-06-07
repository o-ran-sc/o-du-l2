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
# This script is used to build and execute ODU-High and test stubs
#!/bin/bash

CURRENT_DIR=$PWD
ROOT_DIR=$CURRENT_DIR/../../

echo ""
cd $ROOT_DIR/build/odu
make clean_all
echo "***** Building O-DU Binary *****"
make odu MACHINE=BIT64 MODE=FDD
echo ""
echo "***** Building CU Stub Binary *****"
make cu_stub NODE=TEST_STUB MACHINE=BIT64 MODE=FDD
echo ""
echo "***** Building RIC Stub Binary *****"
make ric_stub NODE=TEST_STUB MACHINE=BIT64 MODE=FDD

echo ""
echo "***** Assigning IP addresses *****"
INTERFACE=$(ip route | grep default | sed -e "s/^.*dev.//" -e "s/.proto.*//")
INTERFACE="$(echo -e "${INTERFACE}" | tr -d '[:space:]')"
ifconfig $INTERFACE:ODU "192.168.130.81"
ifconfig $INTERFACE:CU_STUB "192.168.130.82"
ifconfig $INTERFACE:RIC_STUB "192.168.130.80"

xterm -hold -e "cd $ROOT_DIR/bin/cu_stub; chmod 777 *; ./start_cu_stub_logging.sh && ./cu_stub" &
sleep 2
xterm -hold -e "cd $ROOT_DIR/bin/ric_stub; chmod 777 *; ./start_ric_stub_logging.sh && ./ric_stub" &
sleep 2
xterm -hold -e "cd $ROOT_DIR/bin/odu; chmod 777 *; ./start_du_logging.sh && ./odu" &

################################################################################
#                              End of file                                     #
################################################################################                                                                              
