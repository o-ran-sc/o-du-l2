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
# This is the main script that in turn triggers sub-scripts to install, compile 
# and execute ODU-High modules
#!/bin/bash

# Installing dependencies for ODU-High
./install_odu_dependency.sh

# Compiling and executing ODU-High binaries
./start_gnb.sh

################################################################################
#                              End of file                                     #
################################################################################                                                                              
