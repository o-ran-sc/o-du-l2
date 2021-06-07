################################################################################
#   Copyright (c) [2017-2019] [Radisys]                                        #
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
# This script is used to log ODU-High
#!/bin/sh

ROOT_DIR=$PWD
d=`date +%Y_%m_%d_%I_%M_%S`

touch  $ROOT_DIR/../odu/"log_odu_$d.txt"
chmod -c 777 $ROOT_DIR/../odu/"log_odu_$d.txt"
echo "if \$programname == \"ODU\" then" >> \
/etc/rsyslog.d/rsyslog_loginauth.conf
echo "$ROOT_DIR/../odu/log_odu_$d.txt" >> \
/etc/rsyslog.d/rsyslog_loginauth.conf
echo $ROOT_DIR
systemctl restart rsyslog


#**********************************************************************
#    End of file
#**********************************************************************
