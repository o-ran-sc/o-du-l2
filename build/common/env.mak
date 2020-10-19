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

Q=
S=
# Check for verbose build
ifneq ($(V),1)
    Q:=@
    S:=-s
endif
export Q
export S

# With the assumption that the make command is run from the directory
# where the makefile is located, PWD fetches the build directory and
# all other directories are generated using the build directory.
BUILD_DIR=$(PWD)
ROOT_DIR_SCRIPTS =$(patsubst %/build/scripts,%,$(BUILD_DIR))
COM_BUILD_DIR=$(BUILD_DIR)/../common
ROOT_DIR=$(patsubst %/build/odu,%,$(BUILD_DIR))
export BUILD_DIR
export COM_BUILD_DIR
export ROOT_DIR

# Path where the .o files or the directories with .o files are located
OBJ_ROOT =$(BUILD_DIR)/obj
export OBJ_ROOT
# Path where the binary logging related files are generated
LOG_ROOT  =$(BUILD_DIR)/logdb
export LOG_ROOT
# Path where the libraries are created
LIB_ROOT =$(BUILD_DIR)/lib
export LIB_ROOT
# Path for the common files that are used across multiple layers
CM_DIR   =$(ROOT_DIR)/src/cm
export CM_DIR
# Path where the final binaries are created
BIN_DIR  =$(BUILD_DIR)/bin
export BIN_DIR
# Path where the libraries are packaged and these are used when not built from source
EXT_LIB_DIR  =$(ROOT_DIR)/lib
export EXT_LIB_DIR

CMENBE2EOPTS += $(RLOG_OPTS_TEXT)

L_OPTS = -lrt -lm -lpthread -lpcap -lstdc++  -L $(LIB_ROOT)

#**********************************************************************
#         End of file
#**********************************************************************
