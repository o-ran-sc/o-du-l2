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

# Master makefile for O1 module

# Identify the location our software which is used by rest
# of the build scripts

include ../common/rsys_fancy.mak
include ../common/env.mak

ROOT_DIR=$(patsubst %/build/o1,%,$(BUILD_DIR))
RULE=$(COLOR_RED)
OPTS=$(COLOR_GREEN)
NORM=$(REVERT_COLOR)

# For improved speed
CPUINFO=`cat /proc/cpuinfo | grep processor| wc -l`
ifeq ($(CPUINFO), 1)
   NUM_PARALLEL=1
else
      NUM_PARALLEL=$(shell echo $(CPUINFO) - 1 |bc)
endif

ifeq ($(FAST), 1)
      NUM_PARALLEL=$(shell echo $(CPUINFO) + 2 |bc)
endif

PARALLEL_COMP=-j $(NUM_PARALLEL)

# Different options to O1 build
# # Other apsects of tool chain set here
# # These should be made available appropriately to the builds
ifeq ($(MACHINE),BIT64)
CC          =gcc -m64
CCPP        =g++ -m64
else
CC          =gcc -m32
CCPP        =g++ -m32
endif

CC1= $(CC)
CCPP1= $(CCPP)


# The include options get merged with the options needed by
# # the called makefiles and hence we export these to make them
# # available to them.
BUILD=i686-linux

# The called makefiles depend on these macros and these need to be exported
export PLTFRM
export PLTFRM_FLAGS
export BUILD
export I_OPTS

# Add to the linker options the platform specific components
#L_OPTS+=-lnsl -lrt -lm -lpthread -lsctp
L_OPTS=-lsysrepo -lyang
L_OPTS+= -lsysrepo-cpp -lyang-cpp
L_OPTS+= -lm -lpthread

# Export some of the flags expected from the command line.
# # These macros are made available for the makefiles called from this makefile
export MACHINE
export OAM_ENABLE

help:
		@echo -e "******************************************************************"
		@echo -e "BUILD COMMAND            DESCRIPTION                              "
		@echo -e "------------------------------------------------------------------"
		@echo -e "$(RULE)o1        - Builds all components of O1$(NORM)"
		@echo -e "$(RULE)clean_o1  - clean up O1$(NORM)"
		@echo -e "$(RULE)clean_all - cleanup O1 and all directories$(NORM)"
		@echo -e "$(OPTS)  options: $(NORM)"
		@echo -e "$(OPTS)    MACHINE=BIT64/BIT32  - Default is BIT32$(NORM)"
		@echo -e "******************************************************************"

prepare_dirs:
	$(Q)echo -e "Preparing directories for build..."
	$(Q)mkdir -p $(BUILD_DIR)/obj/o1
	$(Q)mkdir -p $(LIB_ROOT)/o1
	$(Q)mkdir -p $(BIN_DIR)/o1
	$(Q)echo -e "Directories are successfully prepared"

o1_mod:
		$(Q)$(MAKE) -f $(COM_BUILD_DIR)/o1.mak OBJ_DIR=$(OBJ_ROOT)/o1 LIB_DIR=$(LIB_ROOT)/o1 LOG_DIR=$(LOG_ROOT)/o1 CC='$(CCPP1)'

link_o1: o1_mod
		$(Q)$(CCPP1) -g -o $(OBJ_ROOT)/o1/o1 -Wl,-R../lib/:. $(OBJ_ROOT)/o1/*.o\
			$(L_OPTS) -L$(LIB_ROOT)/o1

clean_o1: 
		$(Q)$(MAKE) -f $(COM_BUILD_DIR)/o1.mak clean OBJ_DIR=$(OBJ_ROOT)/o1 LIB_DIR=$(LIB_ROOT)/o1 LOG_DIR=$(LOG_ROOT)/o1 CC='$(CC1)'
		$(Q)rm -rf $(OBJ_ROOT)/o1/*
		$(Q)rm -rf $(LIB_ROOT)/o1/*
		$(Q)rm -rf $(BIN_DIR)/o1/*
	        $(Q)echo -e "***** O1 CLEAN COMPLETE *****"

clean_all: clean_o1
			  $(Q)rm -rf $(OBJ_ROOT)
			  $(Q)rm -rf $(LIB_ROOT)
			  $(Q)rm -rf $(LOG_ROOT)
			  $(Q)rm -rf $(BIN_DIR)

copy_build: link_o1
			$(Q)cp -f ./obj/o1/o1 ./bin/o1
		        $(Q)echo -e "***** O1 BUILD COMPLETE *****"

o1: prepare_dirs copy_build

#######################################################################
#         End of file                                                 #
#######################################################################
