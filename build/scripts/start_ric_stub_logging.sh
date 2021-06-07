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
# This script is used to log RIC stub
#!/bin/sh

ROOT_DIR=$PWD
d=`date +%Y_%m_%d_%I_%M_%S`

touch  $ROOT_DIR/"log_ric_stub_$d.txt"
chmod -c 777 $ROOT_DIR/"log_ric_stub_$d.txt"
touch /etc/rsyslog.d/rsyslog_loginauth.conf
cp /dev/null /etc/rsyslog.d/rsyslog_loginauth.conf
echo "if \$programname == \"RIC_STUB\" then" >> \
/etc/rsyslog.d/rsyslog_loginauth.conf
echo "$ROOT_DIR/../ric_stub/log_ric_stub_$d.txt" >> \
/etc/rsyslog.d/rsyslog_loginauth.conf
systemctl restart rsyslog


#**********************************************************************
#	  End of file
#**********************************************************************
