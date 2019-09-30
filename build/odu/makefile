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

# Master makefile

# Identify the location our software which is used by rest
# of the build scripts

include ../common/rsys_fancy.mak
include ../common/env.mak

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

# Different options to CPUH and CPUL builds
# # Other apsects of tool chain set here
# # These should be made available appropriately to the builds
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
	$(Q)mkdir -p $(BUILD_DIR)/obj/odu
	$(Q)mkdir -p $(LOG_ROOT)/odu
	$(Q)mkdir -p $(LIB_ROOT)/odu
	$(Q)echo -e "Directories are successfully prepared"

PLTFRM_FLAGS= -UMSPD -DODU

ifeq ($(MODE),TDD)
   PLTFRM_FLAGS += -DMODE=TDD
endif
ifeq ($(CA_ENABLE),YES)
   PLTFRM_FLAGS += -DCA_ENABLE=YES
endif

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
L_OPTS+=-lnsl -lrt -lm -lpthread -lsctp

# Export some of the flags expected from the command line.
# # These macros are made available for the makefiles called from this makefile
export BOARD
export MACHINE
export MODE
export OAM_ENABLE
export CNM_ENABLE
export TEXT_LOG
export CA_ENABLE

help:
		@echo -e "******************************************************************"
		@echo -e "BUILD COMMAND            DESCRIPTION                              "
		@echo -e "------------------------------------------------------------------"
		@echo -e "$(RULE)odu - Builds all components of ODU$(NORM)"
		@echo -e "$(OPTS)  options: $(NORM)"
		@echo -e "$(OPTS)    MACHINE=BIT64/BIT32  - Default is BIT32$(NORM)"
		@echo -e "$(OPTS)    TEXT_LOG=YES   - With text logging instead of binary$(NORM)"
		@echo -e "$(OPTS)    MODE=TDD       - If not specified, MODE=FDD$(NORM)"
		@echo -e "$(RULE)clean_all         tdd cleanup everything$(NORM)"
		@echo -e "******************************************************************"

du:
		$(Q)$(MAKE) -f $(COM_BUILD_DIR)/du_app.mak OBJ_DIR=$(OBJ_ROOT)/odu LIB_DIR=$(LIB_ROOT)/odu LOG_DIR=$(LOG_ROOT)/odu CC='$(CC1)'
		$(Q)$(MAKE) -f $(COM_BUILD_DIR)/f1ap.mak OBJ_DIR=$(OBJ_ROOT)/odu LIB_DIR=$(LIB_ROOT)/odu LOG_DIR=$(LOG_ROOT)/odu CC='$(CC1)'
		$(Q)$(MAKE) -f $(COM_BUILD_DIR)/asn.mak OBJ_DIR=$(OBJ_ROOT)/odu LIB_DIR=$(LIB_ROOT)/odu LOG_DIR=$(LOG_ROOT)/odu CC='$(CC1)'
		$(Q)$(MAKE) -f $(COM_BUILD_DIR)/kw.mak OBJ_DIR=$(OBJ_ROOT)/odu LIB_DIR=$(LIB_ROOT)/odu LOG_DIR=$(LOG_ROOT)/odu CC='$(CC1)'
		$(Q)$(MAKE) -f $(COM_BUILD_DIR)/cm.mak OBJ_DIR=$(OBJ_ROOT)/odu LIB_DIR=$(LIB_ROOT)/odu LOG_DIR=$(LOG_ROOT)/odu CC='$(CC1)'
		$(Q)$(MAKE) -f $(COM_BUILD_DIR)/rg.mak OBJ_DIR=$(OBJ_ROOT)/odu LIB_DIR=$(LIB_ROOT)/odu LOG_DIR=$(LOG_ROOT)/odu CC='$(CC1)'
		$(Q)$(MAKE) -f $(COM_BUILD_DIR)/mt.mak OBJ_DIR=$(OBJ_ROOT)/odu LIB_DIR=$(LIB_ROOT)/odu LOG_DIR=$(LOG_ROOT)/odu CC='$(CC1)'
		$(Q)$(MAKE) -f $(COM_BUILD_DIR)/rl.mak OBJ_DIR=$(OBJ_ROOT)/odu LIB_DIR=$(LIB_ROOT)/odu LOG_DIR=$(LOG_ROOT)/odu CC='$(CC1)'
		$(Q)$(MAKE) -f $(COM_BUILD_DIR)/phy_stub.mak OBJ_DIR=$(OBJ_ROOT)/odu LIB_DIR=$(LIB_ROOT)/odu LOG_DIR=$(LOG_ROOT)/odu CC='$(CC1)'

clean: 
		$(Q)$(MAKE) -f $(COM_BUILD_DIR)/du_app.mak clean OBJ_DIR=$(OBJ_ROOT)/odu LIB_DIR=$(LIB_ROOT)/odu LOG_DIR=$(LOG_ROOT)/odu CC='$(CC1)'
		$(Q)$(MAKE) -f $(COM_BUILD_DIR)/f1ap.mak clean OBJ_DIR=$(OBJ_ROOT)/odu LIB_DIR=$(LIB_ROOT)/odu LOG_DIR=$(LOG_ROOT)/odu CC='$(CC1)'
		$(Q)$(MAKE) -f $(COM_BUILD_DIR)/asn.mak clean OBJ_DIR=$(OBJ_ROOT)/odu LIB_DIR=$(LIB_ROOT)/odu LOG_DIR=$(LOG_ROOT)/odu CC='$(CC1)'
		$(Q)$(MAKE) -f $(COM_BUILD_DIR)/kw.mak clean OBJ_DIR=$(OBJ_ROOT)/odu LIB_DIR=$(LIB_ROOT)/odu LOG_DIR=$(LOG_ROOT)/odu CC='$(CC1)'
		$(Q)$(MAKE) -f $(COM_BUILD_DIR)/cm.mak clean OBJ_DIR=$(OBJ_ROOT)/odu LIB_DIR=$(LIB_ROOT)/odu LOG_DIR=$(LOG_ROOT)/odu CC='$(CC1)'
		$(Q)$(MAKE) -f $(COM_BUILD_DIR)/rg.mak clean OBJ_DIR=$(OBJ_ROOT)/odu LIB_DIR=$(LIB_ROOT)/odu LOG_DIR=$(LOG_ROOT)/odu CC='$(CC1)'
		$(Q)$(MAKE) -f $(COM_BUILD_DIR)/mt.mak clean OBJ_DIR=$(OBJ_ROOT)/odu LIB_DIR=$(LIB_ROOT)/odu LOG_DIR=$(LOG_ROOT)/odu CC='$(CC1)'
		$(Q)$(MAKE) -f $(COM_BUILD_DIR)/rl.mak clean OBJ_DIR=$(OBJ_ROOT)/odu LIB_DIR=$(LIB_ROOT)/odu LOG_DIR=$(LOG_ROOT)/odu CC='$(CC1)'
		$(Q)$(MAKE) -f $(COM_BUILD_DIR)/phy_stub.mak clean OBJ_DIR=$(OBJ_ROOT)/odu LIB_DIR=$(LIB_ROOT)/odu LOG_DIR=$(LOG_ROOT)/odu CC='$(CC1)'

clean_all: clean
			  $(Q)rm -rf $(OBJ_ROOT)
			  $(Q)rm -rf $(LIB_ROOT)
			  $(Q)rm -rf $(LOG_ROOT)
			  $(Q)rm -rf bin
link_all: du
		   $(Q)$(CC1) -g -o $(OBJ_ROOT)/odu/odu -Wl,-R../lib/:. $(OBJ_ROOT)/odu/*.o\
			$(L_OPTS) -L$(LIB_ROOT)/odu -L$(ROOT_DIR)/libs/odu 

odu: prepare_dirs copy_build

copy_build: link_all
		   $(Q)mkdir -p bin  bin/config bin/odu bin/scripts
ifneq ($(TEXT_LOG), YES)
			$(Q)cp -f ./obj/odu/rlogapp bin/odu
			$(Q)cat $(LOG_ROOT)/odu/*.db > $(LOG_ROOT)/odu/rlog.logdb
			$(Q)cp -f $(LOG_ROOT)/odu/rlog.logdb odu/bin/
			$(Q)echo -e "Completed generation of log database"
endif
			$(Q)cp -f ./obj/odu/odu ./bin/odu
#			$(Q)cp -f ../config/wr_cfg_fdd.txt ./odu/bin/wr_cfg.txt
#			$(Q)cp -f ../config/ys_cfg.txt ./odu/bin/ys_cfg.txt
			$(Q)cp -f ../config/ssi_mem ./bin/config
			$(Q)cp -rf ./bin $(ROOT_DIR)/bin/
			$(Q)mkdir -p $(ROOT_DIR)/libs/
			$(Q)cp -f ./lib/odu/*.a $(ROOT_DIR)/libs/
		   $(Q)echo -e "***** BUILD COMPLETE *****"
