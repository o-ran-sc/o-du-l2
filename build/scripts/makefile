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

#********************************************************************20**
#
#       Name:   gNodeB Sample Application
#
#       Type:   make file
#
#       Desc:   Compile, assemble and link product software for
#               various configurations. Further comments are
#               embedded in the file. 
#
#               This file supports a variety of environments and
#               build targets. The default build target will be the
#               portable target for the Linu 2.4.x with GNU C(gcc)
#
#       Env:    Linux 2.4.x with GNU C (gcc)
#
#               BUILD TARGETS:
#
#               clean            remove all object files
#   
#       File:  makefile
#
#********************************************************************21*/
#=======================================================================

# Identify the location our software which is used by rest
# of the build scripts
include ../common/rsys_fancy.mak
include ../common/env.mak

RULE=$(COLOR_RED)
OPTS=$(COLOR_GREEN)
NORM=$(REVERT_COLOR)

ROOT_DIR=$(ROOT_DIR_SCRIPTS)
export ROOT_DIR

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
#PARALLEL_COMP=-j 1

# Different options to CPUH and CPUL builds
# Other apsects of tool chain set here
# These should be made available appropriately to the builds

ifeq ($(MACHINE),BIT64)
CC          =gcc -m64
CC_STRIP    =strip --strip-all
CC_LINKER   =gcc -m64
CCPP        =g++ -m64
CCPP_LINKER =g++ -m64
else
CC          =gcc -m32
CC_STRIP    =strip --strip-all
CC_LINKER   =gcc -m32
CCPP        =g++ -m32
CCPP_LINKER =g++ -m32
endif

CC1= $(CC)
CCPP1= $(CCPP)

ifeq ($(MODE),TDD)
CNM_ENABLE = YES
else
CNM_ENABLE=NO
endif

#-----------------------------------------------------------------------
# macro for output file name and makefile name
#
prepare_dirs:
	$(Q)echo -e "Preparing directories for build..."
	$(Q)mkdir -p $(BUILD_DIR)/obj
	$(Q)mkdir -p $(LOG_ROOT)
	$(Q)mkdir -p $(LIB_ROOT)
	$(Q)echo -e "Directories are successfully prepared"


ifeq ($(CNM_ENABLE),YES) 
CNM_OPT = -DxENABLE_CNM -DREM_ENABLE -DCNM_DEBUG
else
CNM_OPT = -UENABLE_CNM -UREM_ENABLE
endif

   PLTFRM_FLAGS= -UMSPD $(CNM_OPT)

ifeq ($(MODE),TDD) 
   PLTFRM_FLAGS += -DMODE=TDD
endif
CA_ENABLE=YES
ifeq ($(CA_ENABLE),YES)
   PLTFRM_FLAGS += -DCA_ENABLE=YES
endif

# The include options get merged with the options needed by
# the called makefiles and hence we export these to make them
# available to them.
PLTFRM=PAL
BUILD=i686-linux


# The called makefiles depend on these macros and these need to be exported
export PLTFRM
export PLTFRM_FLAGS
export BUILD
export I_OPTS

# Add to the linker options the platform specific components
L_OPTS+=-lnsl -lrt -lm -lpthread 


# Export some of the flags expected from the command line.
# These macros are made available for the makefiles called from this makefile
export MACHINE
export MODE
export CNM_ENABLE
export CA_ENABLE

###################################################################################################
#	TeNB Compilation Options	- Help Menu
###################################################################################################
help:
	@echo -e "******************************************************************"
	@echo -e "BUILD COMMAND            DESCRIPTION                              "
	@echo -e "------------------------------------------------------------------"
	@echo -e "$(RULE)tenb - Builds all components of TeNB$(NORM)"
	@echo -e "$(OPTS)  options: $(NORM)"
	@echo -e "$(OPTS)    MACHINE=BIT64/BIT32  - Default is BIT32$(NORM)"
	@echo -e "$(OPTS)    MODE=TDD       - If not specified, MODE=FDD$(NORM)"
	@echo -e "$(RULE)clean_all         tdd cleanup everything$(NORM)"
	@echo -e "******************************************************************"


enb: 
	$(Q)$(MAKE) -f $(COM_BUILD_DIR)/kw.mak OBJ_DIR=$(OBJ_ROOT) LIB_DIR=$(LIB_ROOT) LOG_DIR=$(LOG_ROOT) CC='$(CC1)'
	$(Q)$(MAKE) -f $(COM_BUILD_DIR)/pj.mak OBJ_DIR=$(OBJ_ROOT) LIB_DIR=$(LIB_ROOT) LOG_DIR=$(LOG_ROOT) CC='$(CC1)'
	$(Q)$(MAKE) -f $(COM_BUILD_DIR)/rg.mak OBJ_DIR=$(OBJ_ROOT) LIB_DIR=$(LIB_ROOT) LOG_DIR=$(LOG_ROOT) CC='$(CC1)'

clean_all:
	$(Q)$(MAKE) -f $(COM_BUILD_DIR)/rg.mak clean OBJ_DIR=$(OBJ_ROOT) LIB_DIR=$(LIB_ROOT) LOG_DIR=$(LOG_ROOT) CC='$(CC2)'
	$(Q)$(MAKE) -f $(COM_BUILD_DIR)/kw.mak clean OBJ_DIR=$(OBJ_ROOT) LIB_DIR=$(LIB_ROOT) LOG_DIR=$(LOG_ROOT) CC='$(CC2)'
	$(Q)$(MAKE) -f $(COM_BUILD_DIR)/pj.mak clean OBJ_DIR=$(OBJ_ROOT) LIB_DIR=$(LIB_ROOT) LOG_DIR=$(LOG_ROOT) CC='$(CC2)'

	
###################################################################################################
#	TeNB Compilation
###################################################################################################

tenb: prepare_dirs copy_build

copy_build: enb
	$(Q)mkdir -p $(ROOT_DIR)/libs/
	$(Q)cp -f ./lib/*.a $(ROOT_DIR)/libs/

###################################################################################################
#													END OF MAKEFILE
###################################################################################################
