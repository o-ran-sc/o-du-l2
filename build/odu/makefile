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

PLTFRM_FLAGS= -UMSPD -DODU #-DEGTP_TEST #-WLS_MEM

ifeq ($(MODE),TDD)
   PLTFRM_FLAGS += -DMODE=TDD
endif
ifeq ($(CA_ENABLE),YES)
   PLTFRM_FLAGS += -DCA_ENABLE=YES
endif

ifeq ($(NODE),TEST_STUB)
	PLTFRM_FLAGS+=-DODU_TEST_STUB
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
#L_OPTS+=-lnsl -lrt -lm -lpthread -lsctp
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
		@echo -e "$(RULE)odu       - Builds all components of ODU$(NORM)"
		@echo -e "$(RULE)cu_stub   - Builds all CU Stub$(NORM)"
		@echo -e "$(RULE)ric_stub   - Builds all RIC_Stub$(NORM)"
		@echo -e "$(RULE)clean_odu - clean up ODU$(NORM)"
		@echo -e "$(RULE)clean_cu  - clean up CU Stub$(NORM)"
		@echo -e "$(RULE)clean_ric  - clean up RIC Stub$(NORM)"
		@echo -e "$(RULE)clean_all - cleanup everything$(NORM)"
		@echo -e "$(OPTS)  options: $(NORM)"
		@echo -e "$(OPTS)    MACHINE=BIT64/BIT32  - Default is BIT32$(NORM)"
		@echo -e "$(OPTS)    NODE=TEST_STUB         - Mandatory option for cu_stub/ric_stub$(NORM)"
		@echo -e "$(OPTS)    MODE=TDD             - If not specified, MODE=FDD$(NORM)"
		@echo -e "******************************************************************"

prepare_dirs:
	$(Q)echo -e "Preparing directories for build..."
	$(Q)mkdir -p $(BUILD_DIR)/obj/odu
	$(Q)mkdir -p $(LIB_ROOT)/odu
	$(Q)mkdir -p $(ROOT_DIR)/libs/
	$(Q)mkdir -p $(BIN_DIR)/odu
	$(Q)mkdir -p $(ROOT_DIR)/bin
	$(Q)echo -e "Directories are successfully prepared"

prepare_cu_dirs:
	 $(Q)echo -e "Preparing directories for build..."
	 $(Q)mkdir -p $(BUILD_DIR)/obj/cu_stub
	 $(Q)mkdir -p $(LIB_ROOT)/cu_stub
	 $(Q)mkdir -p $(BIN_DIR)/cu_stub
	 $(Q)echo -e "Directories are successfully prepared"

prepare_ric_dirs:
	 $(Q)echo -e "Preparing directories for build..."
	 $(Q)mkdir -p $(BUILD_DIR)/obj/ric_stub
	 $(Q)mkdir -p $(LIB_ROOT)/ric_stub
	 $(Q)mkdir -p $(BIN_DIR)/ric_stub
	 $(Q)echo -e "Directories are successfully prepared"

du:
		$(Q)$(MAKE) -f $(COM_BUILD_DIR)/asn_common.mak OBJ_DIR=$(OBJ_ROOT)/odu LIB_DIR=$(LIB_ROOT)/odu LOG_DIR=$(LOG_ROOT)/odu CC='$(CC1)'
		$(Q)$(MAKE) -f $(COM_BUILD_DIR)/asn_f1ap.mak OBJ_DIR=$(OBJ_ROOT)/odu LIB_DIR=$(LIB_ROOT)/odu LOG_DIR=$(LOG_ROOT)/odu CC='$(CC1)'
		$(Q)$(MAKE) -f $(COM_BUILD_DIR)/asn_e2ap.mak OBJ_DIR=$(OBJ_ROOT)/odu LIB_DIR=$(LIB_ROOT)/odu LOG_DIR=$(LOG_ROOT)/odu CC='$(CC1)'
		$(Q)$(MAKE) -f $(COM_BUILD_DIR)/asn_sysinfo.mak OBJ_DIR=$(OBJ_ROOT)/odu LIB_DIR=$(LIB_ROOT)/odu LOG_DIR=$(LOG_ROOT)/odu CC='$(CC1)'
		$(Q)$(MAKE) -f $(COM_BUILD_DIR)/du_app.mak OBJ_DIR=$(OBJ_ROOT)/odu LIB_DIR=$(LIB_ROOT)/odu LOG_DIR=$(LOG_ROOT)/odu CC='$(CC1)'
		#$(Q)$(MAKE) -f $(COM_BUILD_DIR)/f1ap.mak OBJ_DIR=$(OBJ_ROOT)/odu LIB_DIR=$(LIB_ROOT)/odu LOG_DIR=$(LOG_ROOT)/odu CC='$(CC1)'
		$(Q)$(MAKE) -f $(COM_BUILD_DIR)/kw.mak OBJ_DIR=$(OBJ_ROOT)/odu LIB_DIR=$(LIB_ROOT)/odu LOG_DIR=$(LOG_ROOT)/odu CC='$(CC1)'
		$(Q)$(MAKE) -f $(COM_BUILD_DIR)/cm.mak OBJ_DIR=$(OBJ_ROOT)/odu LIB_DIR=$(LIB_ROOT)/odu LOG_DIR=$(LOG_ROOT)/odu CC='$(CC1)'
		$(Q)$(MAKE) -f $(COM_BUILD_DIR)/mac.mak OBJ_DIR=$(OBJ_ROOT)/odu LIB_DIR=$(LIB_ROOT)/odu LOG_DIR=$(LOG_ROOT)/odu CC='$(CC1)'
		$(Q)$(MAKE) -f $(COM_BUILD_DIR)/sch.mak OBJ_DIR=$(OBJ_ROOT)/odu LIB_DIR=$(LIB_ROOT)/odu LOG_DIR=$(LOG_ROOT)/odu CC='$(CC1)'
		$(Q)$(MAKE) -f $(COM_BUILD_DIR)/mt.mak OBJ_DIR=$(OBJ_ROOT)/odu LIB_DIR=$(LIB_ROOT)/odu LOG_DIR=$(LOG_ROOT)/odu CC='$(CC1)'
		$(Q)$(MAKE) -f $(COM_BUILD_DIR)/rl.mak OBJ_DIR=$(OBJ_ROOT)/odu LIB_DIR=$(LIB_ROOT)/odu LOG_DIR=$(LOG_ROOT)/odu CC='$(CC1)'
		$(Q)$(MAKE) -f $(COM_BUILD_DIR)/phy_stub.mak OBJ_DIR=$(OBJ_ROOT)/odu LIB_DIR=$(LIB_ROOT)/odu LOG_DIR=$(LOG_ROOT)/odu CC='$(CC1)'

link_du: du
		   $(Q)$(CC1) -g -o $(OBJ_ROOT)/odu/odu -Wl,-R../lib/:. $(OBJ_ROOT)/odu/*.o\
			$(L_OPTS) -L$(LIB_ROOT)/odu -L$(ROOT_DIR)/libs/odu 

clean_odu: 
		$(Q)$(MAKE) -f $(COM_BUILD_DIR)/du_app.mak clean OBJ_DIR=$(OBJ_ROOT)/odu LIB_DIR=$(LIB_ROOT)/odu LOG_DIR=$(LOG_ROOT)/odu CC='$(CC1)'
		$(Q)$(MAKE) -f $(COM_BUILD_DIR)/asn_common.mak clean OBJ_DIR=$(OBJ_ROOT)/odu LIB_DIR=$(LIB_ROOT)/odu LOG_DIR=$(LOG_ROOT)/odu CC='$(CC1)'
		$(Q)$(MAKE) -f $(COM_BUILD_DIR)/asn_f1ap.mak clean OBJ_DIR=$(OBJ_ROOT)/odu LIB_DIR=$(LIB_ROOT)/odu LOG_DIR=$(LOG_ROOT)/odu CC='$(CC1)'
		$(Q)$(MAKE) -f $(COM_BUILD_DIR)/asn_e2ap.mak clean OBJ_DIR=$(OBJ_ROOT)/odu LIB_DIR=$(LIB_ROOT)/odu LOG_DIR=$(LOG_ROOT)/odu CC='$(CC1)'
		$(Q)$(MAKE) -f $(COM_BUILD_DIR)/asn_sysinfo.mak clean OBJ_DIR=$(OBJ_ROOT)/odu LIB_DIR=$(LIB_ROOT)/odu LOG_DIR=$(LOG_ROOT)/odu CC='$(CC1)'
		$(Q)$(MAKE) -f $(COM_BUILD_DIR)/kw.mak clean OBJ_DIR=$(OBJ_ROOT)/odu LIB_DIR=$(LIB_ROOT)/odu LOG_DIR=$(LOG_ROOT)/odu CC='$(CC1)'
		$(Q)$(MAKE) -f $(COM_BUILD_DIR)/cm.mak clean OBJ_DIR=$(OBJ_ROOT)/odu LIB_DIR=$(LIB_ROOT)/odu LOG_DIR=$(LOG_ROOT)/odu CC='$(CC1)'
		$(Q)$(MAKE) -f $(COM_BUILD_DIR)/mac.mak clean OBJ_DIR=$(OBJ_ROOT)/odu LIB_DIR=$(LIB_ROOT)/odu LOG_DIR=$(LOG_ROOT)/odu CC='$(CC1)'
		$(Q)$(MAKE) -f $(COM_BUILD_DIR)/sch.mak clean OBJ_DIR=$(OBJ_ROOT)/odu LIB_DIR=$(LIB_ROOT)/odu LOG_DIR=$(LOG_ROOT)/odu CC='$(CC1)'
		$(Q)$(MAKE) -f $(COM_BUILD_DIR)/mt.mak clean OBJ_DIR=$(OBJ_ROOT)/odu LIB_DIR=$(LIB_ROOT)/odu LOG_DIR=$(LOG_ROOT)/odu CC='$(CC1)'
		$(Q)$(MAKE) -f $(COM_BUILD_DIR)/rl.mak clean OBJ_DIR=$(OBJ_ROOT)/odu LIB_DIR=$(LIB_ROOT)/odu LOG_DIR=$(LOG_ROOT)/odu CC='$(CC1)'
		$(Q)$(MAKE) -f $(COM_BUILD_DIR)/phy_stub.mak clean OBJ_DIR=$(OBJ_ROOT)/odu LIB_DIR=$(LIB_ROOT)/odu LOG_DIR=$(LOG_ROOT)/odu CC='$(CC1)'
		$(Q)rm -rf $(OBJ_ROOT)/odu/*
		$(Q)rm -rf $(LIB_ROOT)/odu/*
		$(Q)rm -rf $(BIN_DIR)/odu/*
	   $(Q)rm -rf $(ROOT_DIR)/bin/odu
	   $(Q)echo -e "***** ODU CLEAN COMPLETE *****"

clean_all: clean_odu clean_cu clean_ric
			  $(Q)rm -rf $(OBJ_ROOT)
			  $(Q)rm -rf $(LIB_ROOT)
			  $(Q)rm -rf $(LOG_ROOT)
			  $(Q)rm -rf $(BIN_DIR)
			  $(Q)rm -rf $(ROOT_DIR)/bin
			  $(Q)rm -rf $(ROOT_DIR)/libs

cu: 
	 $(Q)$(MAKE) -f $(COM_BUILD_DIR)/cu_stub.mak OBJ_DIR=$(OBJ_ROOT)/cu_stub LIB_DIR=$(LIB_ROOT)/cu_stub LOG_DIR=$(LOG_ROOT)/cu_stub CC='$(CC1)'
	 $(Q)$(MAKE) -f $(COM_BUILD_DIR)/asn_common.mak OBJ_DIR=$(OBJ_ROOT)/cu_stub LIB_DIR=$(LIB_ROOT)/cu_stub LOG_DIR=$(LOG_ROOT)/cu_stub CC='$(CC1)'
	 $(Q)$(MAKE) -f $(COM_BUILD_DIR)/asn_f1ap.mak OBJ_DIR=$(OBJ_ROOT)/cu_stub LIB_DIR=$(LIB_ROOT)/cu_stub LOG_DIR=$(LOG_ROOT)/cu_stub CC='$(CC1)'
	 $(Q)$(MAKE) -f $(COM_BUILD_DIR)/asn_sysinfo.mak OBJ_DIR=$(OBJ_ROOT)/cu_stub LIB_DIR=$(LIB_ROOT)/cu_stub LOG_DIR=$(LOG_ROOT)/cu_stub CC='$(CC1)'
	 $(Q)$(MAKE) -f $(COM_BUILD_DIR)/cm.mak OBJ_DIR=$(OBJ_ROOT)/cu_stub LIB_DIR=$(LIB_ROOT)/cu_stub LOG_DIR=$(LOG_ROOT)/cu_stub CC='$(CC1)'
	 $(Q)$(MAKE) -f $(COM_BUILD_DIR)/mt.mak OBJ_DIR=$(OBJ_ROOT)/cu_stub LIB_DIR=$(LIB_ROOT)/cu_stub LOG_DIR=$(LOG_ROOT)/cu_stub CC='$(CC1)'

clean_cu:
	 $(Q)$(MAKE) -f $(COM_BUILD_DIR)/cu_stub.mak clean OBJ_DIR=$(OBJ_ROOT)/cu_stub LIB_DIR=$(LIB_ROOT)/cu_stub LOG_DIR=$(LOG_ROOT)/cu_stub CC='$(CC1)'
	 $(Q)$(MAKE) -f $(COM_BUILD_DIR)/asn_common.mak clean OBJ_DIR=$(OBJ_ROOT)/cu_stub LIB_DIR=$(LIB_ROOT)/cu_stub LOG_DIR=$(LOG_ROOT)/cu_stub CC='$(CC1)'
	 $(Q)$(MAKE) -f $(COM_BUILD_DIR)/asn_f1ap.mak clean OBJ_DIR=$(OBJ_ROOT)/cu_stub LIB_DIR=$(LIB_ROOT)/cu_stub LOG_DIR=$(LOG_ROOT)/cu_stub CC='$(CC1)'
	 $(Q)$(MAKE) -f $(COM_BUILD_DIR)/asn_sysinfo.mak clean OBJ_DIR=$(OBJ_ROOT)/cu_stub LIB_DIR=$(LIB_ROOT)/cu_stub LOG_DIR=$(LOG_ROOT)/cu_stub CC='$(CC1)'
	 $(Q)$(MAKE) -f $(COM_BUILD_DIR)/cm.mak clean OBJ_DIR=$(OBJ_ROOT)/cu_stub LIB_DIR=$(LIB_ROOT)/cu_stub LOG_DIR=$(LOG_ROOT)/cu_stub CC='$(CC1)'
	 $(Q)$(MAKE) -f $(COM_BUILD_DIR)/mt.mak clean OBJ_DIR=$(OBJ_ROOT)/cu_stub LIB_DIR=$(LIB_ROOT)/cu_stub LOG_DIR=$(LOG_ROOT)/cu_stub CC='$(CC1)'
	 $(Q)rm -rf $(OBJ_ROOT)/cu_stub/*
	 $(Q)rm -rf $(LIB_ROOT)/cu_stub/*
	 $(Q)rm -rf $(BIN_DIR)/cu_stub/*
	 $(Q)rm -rf $(ROOT_DIR)/bin/cu_stub
	 $(Q)echo -e "***** CU STUB CLEAN COMPLETE *****"

link_cu: 
		$(Q)$(CC1) -g -o $(OBJ_ROOT)/cu_stub/cu_stub -Wl,-R../lib/:. $(OBJ_ROOT)/cu_stub/*.o\
		$(L_OPTS) -L$(LIB_ROOT)/cu_stub -L$(ROOT_DIR)/libs/cu_stub
		$(Q)cp -f ./obj/cu_stub/cu_stub ./bin/cu_stub
		$(Q)cp -rf ./bin/cu_stub $(ROOT_DIR)/bin/
		$(Q)cp -f ../scripts/cu_script.sh $(ROOT_DIR)/bin/cu_stub
		$(Q)echo -e "***** CU STUB BUILD COMPLETE *****"

ric: 
	 $(Q)$(MAKE) -f $(COM_BUILD_DIR)/ric_stub.mak OBJ_DIR=$(OBJ_ROOT)/ric_stub LIB_DIR=$(LIB_ROOT)/ric_stub LOG_DIR=$(LOG_ROOT)/ric_stub CC='$(CC1)'
	 $(Q)$(MAKE) -f $(COM_BUILD_DIR)/asn_common.mak OBJ_DIR=$(OBJ_ROOT)/ric_stub LIB_DIR=$(LIB_ROOT)/ric_stub LOG_DIR=$(LOG_ROOT)/ric_stub CC='$(CC1)'
	 $(Q)$(MAKE) -f $(COM_BUILD_DIR)/asn_e2ap.mak OBJ_DIR=$(OBJ_ROOT)/ric_stub LIB_DIR=$(LIB_ROOT)/ric_stub LOG_DIR=$(LOG_ROOT)/ric_stub CC='$(CC1)'
	 $(Q)$(MAKE) -f $(COM_BUILD_DIR)/cm.mak OBJ_DIR=$(OBJ_ROOT)/ric_stub LIB_DIR=$(LIB_ROOT)/ric_stub LOG_DIR=$(LOG_ROOT)/ric_stub CC='$(CC1)'
	 $(Q)$(MAKE) -f $(COM_BUILD_DIR)/mt.mak OBJ_DIR=$(OBJ_ROOT)/ric_stub LIB_DIR=$(LIB_ROOT)/ric_stub LOG_DIR=$(LOG_ROOT)/ric_stub CC='$(CC1)'

clean_ric:
	 $(Q)$(MAKE) -f $(COM_BUILD_DIR)/ric_stub.mak clean OBJ_DIR=$(OBJ_ROOT)/ric_stub LIB_DIR=$(LIB_ROOT)/ric_stub LOG_DIR=$(LOG_ROOT)/ric_stub CC='$(CC1)'
	 $(Q)$(MAKE) -f $(COM_BUILD_DIR)/asn_common.mak clean OBJ_DIR=$(OBJ_ROOT)/ric_stub LIB_DIR=$(LIB_ROOT)/ric_stub LOG_DIR=$(LOG_ROOT)/ric_stub CC='$(CC1)'
	 $(Q)$(MAKE) -f $(COM_BUILD_DIR)/asn_e2ap.mak clean OBJ_DIR=$(OBJ_ROOT)/ric_stub LIB_DIR=$(LIB_ROOT)/ric_stub LOG_DIR=$(LOG_ROOT)/ric_stub CC='$(CC1)'
	 $(Q)$(MAKE) -f $(COM_BUILD_DIR)/asn_sysinfo.mak clean OBJ_DIR=$(OBJ_ROOT)/ric_stub LIB_DIR=$(LIB_ROOT)/ric_stub LOG_DIR=$(LOG_ROOT)/ric_stub CC='$(CC1)'
	 $(Q)$(MAKE) -f $(COM_BUILD_DIR)/cm.mak clean OBJ_DIR=$(OBJ_ROOT)/ric_stub LIB_DIR=$(LIB_ROOT)/ric_stub LOG_DIR=$(LOG_ROOT)/ric_stub CC='$(CC1)'
	 $(Q)$(MAKE) -f $(COM_BUILD_DIR)/mt.mak clean OBJ_DIR=$(OBJ_ROOT)/ric_stub LIB_DIR=$(LIB_ROOT)/ric_stub LOG_DIR=$(LOG_ROOT)/ric_stub CC='$(CC1)'
	 $(Q)rm -rf $(OBJ_ROOT)/ric_stub/*
	 $(Q)rm -rf $(LIB_ROOT)/ric_stub/*
	 $(Q)rm -rf $(BIN_DIR)/ric_stub/*
	 $(Q)rm -rf $(ROOT_DIR)/bin/ric_stub
	 $(Q)echo -e "***** RIC STUB CLEAN COMPLETE *****"


link_ric: 
		$(Q)$(CC1) -g -o $(OBJ_ROOT)/ric_stub/ric_stub -Wl,-R../lib/:. $(OBJ_ROOT)/ric_stub/*.o\
		$(L_OPTS) -L$(LIB_ROOT)/ric_stub -L$(ROOT_DIR)/libs/ric_stub
		$(Q)cp -f ./obj/ric_stub/ric_stub ./bin/ric_stub
		$(Q)cp -rf ./bin/ric_stub $(ROOT_DIR)/bin/
		$(Q)echo -e "***** RIC STUB BUILD COMPLETE *****"


copy_build: link_du
			$(Q)cp -f ./obj/odu/odu ./bin/odu
			$(Q)cp -rf ./bin/odu $(ROOT_DIR)/bin/
			$(Q)cp -f ../scripts/odu_script.sh $(ROOT_DIR)/bin/odu
			$(Q)cp -f ./lib/odu/*.a $(ROOT_DIR)/libs/
		   $(Q)echo -e "***** BUILD COMPLETE *****"

odu: prepare_dirs copy_build

cu_stub: prepare_cu_dirs cu link_cu
ric_stub: prepare_ric_dirs ric link_ric

