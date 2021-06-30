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
# This script will add new netconf user
#!/bin/bash

adduser --system netconf && \
   echo "netconf:netconf!" | chpasswd

mkdir -p /home/netconf/.ssh && \
   ssh-keygen -A && \
   ssh-keygen -t dsa -P '' -f /home/netconf/.ssh/id_dsa && \
   cat /home/netconf/.ssh/id_dsa.pub > /home/netconf/.ssh/authorized_keys

################################################################################
#                              End of file                                     #
################################################################################
