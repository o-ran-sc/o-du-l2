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
ifeq ($(COVERAGE),ENABLE)
CC          =gcc -m64 --coverage
else
CC          =gcc -m64
endif
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

PLTFRM_FLAGS= -UMSPD -DODU -DINTEL_FAPI -UODU_MEMORY_DEBUG_LOG -DDEBUG_ASN_PRINT -UDEBUG_PRINT -DERROR_PRINT -USTART_DL_UL_DATA -UNR_DRX -UCALL_FLOW_DEBUG_LOG -UODU_SLOT_IND_DEBUG_LOG -UKPI_CALCULATION

ifeq ($(MODE),TDD)
   PLTFRM_FLAGS += -DNR_TDD
endif
ifeq ($(CA_ENABLE),YES)
   PLTFRM_FLAGS += -DCA_ENABLE=YES
endif

ifeq ($(NODE),TEST_STUB)
	PLTFRM_FLAGS+=-DODU_TEST_STUB
endif
ifeq ($(PHY), INTEL_L1)
	PLTFRM_FLAGS+=-DSS_USE_WLS_MEM -DINTEL_WLS_MEM -DDEBUG_MODE -DINTEL_L1_V20_03_ONWARDS
ifeq ($(PHY_MODE),TIMER)
	PLTFRM_FLAGS+=-DINTEL_TIMER_MODE
endif
endif

ifeq ($(O1_ENABLE),YES)
	PLTFRM_FLAGS+=-DO1_ENABLE
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
ifdef XML_BASED_CONFIG
L_OPTS+=-lxml2
endif

ifeq ($(PHY), INTEL_L1)
	L_OPTS+=-L/root/Intel-L1-20.11.1/phy/wls_lib/ -lwls                         \
        -lhugetlbfs -lnuma -ldl -L/root/Intel-L1-20.11.1/dpdk-20.11.1/build/lib                        \
        -lrte_gso -lrte_acl -lrte_hash -lrte_bbdev -lrte_ip_frag -lrte_bitratestats -lrte_ipsec        \
        -lrte_bpf -lrte_jobstats -lrte_telemetry -lrte_kni -lrte_kvargs -lrte_latencystats -lrte_port  \
        -lrte_lpm -lrte_power -lrte_mbuf -lrte_rawdev -lrte_member -lrte_cfgfile -lrte_mempool         \
		  -lrte_cmdline -lrte_rcu -lrte_compressdev -lrte_reorder -lrte_cryptodev -lrte_rib              \
		  -lrte_distributor -lrte_meter  -lrte_ring -lrte_eal -lrte_metrics -lrte_sched -lrte_efd        \
		  -lrte_net -lrte_security -lrte_ethdev -lrte_pci -lrte_stack -lrte_eventdev -lrte_pdump         \
		  -lrte_table -lrte_fib -lrte_pipeline -lrte_timer -lrte_flow_classify -lrte_vhost               \
        -lrte_gro
endif

ifeq ($(O1_ENABLE),YES)
  L_OPTS+=-lsysrepo -lyang
  L_OPTS+=-lsysrepo-cpp -lyang-cpp
L_OPTS+= -lnetconf2 -lcjson -lcurl
  L_OPTS+=-lstdc++
endif

# Export some of the flags expected from the command line.
# # These macros are made available for the makefiles called from this makefile
export BOARD
export MACHINE
export MODE
export OAM_ENABLE
export CNM_ENABLE
export TEXT_LOG
export CA_ENABLE
export O1_ENABLE

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
		@echo -e "$(OPTS)options: $(NORM)"
		@echo -e "$(OPTS)    MACHINE=BIT64/BIT32  - Default is BIT32$(NORM)"
		@echo -e "$(OPTS)    NODE=TEST_STUB       - Mandatory option for cu_stub/ric_stub$(NORM)"
		@echo -e "$(OPTS)    MODE=TDD             - If not specified, MODE=FDD$(NORM)"
		@echo -e "$(OPTS)    PHY=INTEL_L1         - If not specified, Phy stub is used$(NORM)"
		@echo -e "$(OPTS)    PHY_MODE=TIMER       - Testing mode for INTEL_L1"
		@echo -e "$(OPTS)    COVERAGE=ENABLE      - To record the code coverage stats. Disabled by default"
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
		$(Q)$(MAKE) -j -f $(COM_BUILD_DIR)/asn_common.mak OBJ_DIR=$(OBJ_ROOT)/odu LIB_DIR=$(LIB_ROOT)/odu LOG_DIR=$(LOG_ROOT)/odu CC='$(CC1)'
		$(Q)$(MAKE) -j -f $(COM_BUILD_DIR)/asn_f1ap.mak OBJ_DIR=$(OBJ_ROOT)/odu LIB_DIR=$(LIB_ROOT)/odu LOG_DIR=$(LOG_ROOT)/odu CC='$(CC1)'
		$(Q)$(MAKE) -j -f $(COM_BUILD_DIR)/asn_e2ap.mak OBJ_DIR=$(OBJ_ROOT)/odu LIB_DIR=$(LIB_ROOT)/odu LOG_DIR=$(LOG_ROOT)/odu CC='$(CC1)'
		$(Q)$(MAKE) -j -f $(COM_BUILD_DIR)/asn_e2ap_kpm.mak OBJ_DIR=$(OBJ_ROOT)/odu LIB_DIR=$(LIB_ROOT)/odu LOG_DIR=$(LOG_ROOT)/odu CC='$(CC1)'
		$(Q)$(MAKE) -j -f $(COM_BUILD_DIR)/asn_rrc.mak OBJ_DIR=$(OBJ_ROOT)/odu LIB_DIR=$(LIB_ROOT)/odu LOG_DIR=$(LOG_ROOT)/odu CC='$(CC1)'
		$(Q)$(MAKE) -j -f $(COM_BUILD_DIR)/du_app.mak OBJ_DIR=$(OBJ_ROOT)/odu LIB_DIR=$(LIB_ROOT)/odu LOG_DIR=$(LOG_ROOT)/odu CC='$(CC1)'
		$(Q)$(MAKE) -j -f $(COM_BUILD_DIR)/rlc.mak OBJ_DIR=$(OBJ_ROOT)/odu LIB_DIR=$(LIB_ROOT)/odu LOG_DIR=$(LOG_ROOT)/odu CC='$(CC1)'
		$(Q)$(MAKE) -j -f $(COM_BUILD_DIR)/cm.mak OBJ_DIR=$(OBJ_ROOT)/odu LIB_DIR=$(LIB_ROOT)/odu LOG_DIR=$(LOG_ROOT)/odu CC='$(CC1)'
		$(Q)$(MAKE) -j -f $(COM_BUILD_DIR)/mac.mak OBJ_DIR=$(OBJ_ROOT)/odu LIB_DIR=$(LIB_ROOT)/odu LOG_DIR=$(LOG_ROOT)/odu CC='$(CC1)'
		$(Q)$(MAKE) -j -f $(COM_BUILD_DIR)/sch.mak OBJ_DIR=$(OBJ_ROOT)/odu LIB_DIR=$(LIB_ROOT)/odu LOG_DIR=$(LOG_ROOT)/odu CC='$(CC1)'
		$(Q)$(MAKE) -j -f $(COM_BUILD_DIR)/mt.mak OBJ_DIR=$(OBJ_ROOT)/odu LIB_DIR=$(LIB_ROOT)/odu LOG_DIR=$(LOG_ROOT)/odu CC='$(CC1)'
		
ifneq ($(PHY), INTEL_L1)
		$(Q)$(MAKE) -j -f $(COM_BUILD_DIR)/phy_stub.mak OBJ_DIR=$(OBJ_ROOT)/odu LIB_DIR=$(LIB_ROOT)/odu LOG_DIR=$(LOG_ROOT)/odu CC='$(CC1)'
endif
ifeq ($(O1_ENABLE),YES)
		$(Q)$(MAKE) -j -f $(COM_BUILD_DIR)/o1.mak OBJ_DIR=$(OBJ_ROOT)/odu LIB_DIR=$(LIB_ROOT)/odu LOG_DIR=$(LOG_ROOT)/odu CC='$(CCPP1)'
		$(Q)$(MAKE) -j -f $(COM_BUILD_DIR)/ves.mak OBJ_DIR=$(OBJ_ROOT)/odu LIB_DIR=$(LIB_ROOT)/odu LOG_DIR=$(LOG_ROOT)/odu CC='$(CCPP1)'
endif

link_du: du
		   $(Q)$(CC1) -g -o $(OBJ_ROOT)/odu/odu -Wl,-R../lib/:. $(OBJ_ROOT)/odu/*.o\
			$(L_OPTS) -L$(LIB_ROOT)/odu -L$(ROOT_DIR)/libs/odu 

clean_odu: 
		$(Q)$(MAKE) -j -f $(COM_BUILD_DIR)/du_app.mak clean OBJ_DIR=$(OBJ_ROOT)/odu LIB_DIR=$(LIB_ROOT)/odu LOG_DIR=$(LOG_ROOT)/odu CC='$(CC1)'
		$(Q)$(MAKE) -j -f $(COM_BUILD_DIR)/asn_common.mak clean OBJ_DIR=$(OBJ_ROOT)/odu LIB_DIR=$(LIB_ROOT)/odu LOG_DIR=$(LOG_ROOT)/odu CC='$(CC1)'
		$(Q)$(MAKE) -j -f $(COM_BUILD_DIR)/asn_f1ap.mak clean OBJ_DIR=$(OBJ_ROOT)/odu LIB_DIR=$(LIB_ROOT)/odu LOG_DIR=$(LOG_ROOT)/odu CC='$(CC1)'
		$(Q)$(MAKE) -j -f $(COM_BUILD_DIR)/asn_e2ap.mak clean OBJ_DIR=$(OBJ_ROOT)/odu LIB_DIR=$(LIB_ROOT)/odu LOG_DIR=$(LOG_ROOT)/odu CC='$(CC1)'
		$(Q)$(MAKE) -j -f $(COM_BUILD_DIR)/asn_e2ap_kpm.mak clean OBJ_DIR=$(OBJ_ROOT)/odu LIB_DIR=$(LIB_ROOT)/odu LOG_DIR=$(LOG_ROOT)/odu CC='$(CC1)'
		$(Q)$(MAKE) -j -f $(COM_BUILD_DIR)/asn_rrc.mak clean OBJ_DIR=$(OBJ_ROOT)/odu LIB_DIR=$(LIB_ROOT)/odu LOG_DIR=$(LOG_ROOT)/odu CC='$(CC1)'
		$(Q)$(MAKE) -j -f $(COM_BUILD_DIR)/rlc.mak clean OBJ_DIR=$(OBJ_ROOT)/odu LIB_DIR=$(LIB_ROOT)/odu LOG_DIR=$(LOG_ROOT)/odu CC='$(CC1)'
		$(Q)$(MAKE) -j -f $(COM_BUILD_DIR)/cm.mak clean OBJ_DIR=$(OBJ_ROOT)/odu LIB_DIR=$(LIB_ROOT)/odu LOG_DIR=$(LOG_ROOT)/odu CC='$(CC1)'
		$(Q)$(MAKE) -j -f $(COM_BUILD_DIR)/mac.mak clean OBJ_DIR=$(OBJ_ROOT)/odu LIB_DIR=$(LIB_ROOT)/odu LOG_DIR=$(LOG_ROOT)/odu CC='$(CC1)'
		$(Q)$(MAKE) -j -f $(COM_BUILD_DIR)/sch.mak clean OBJ_DIR=$(OBJ_ROOT)/odu LIB_DIR=$(LIB_ROOT)/odu LOG_DIR=$(LOG_ROOT)/odu CC='$(CC1)'
		$(Q)$(MAKE) -j -f $(COM_BUILD_DIR)/mt.mak clean OBJ_DIR=$(OBJ_ROOT)/odu LIB_DIR=$(LIB_ROOT)/odu LOG_DIR=$(LOG_ROOT)/odu CC='$(CC1)'
		
ifneq ($(PHY), INTEL_L1)
		$(Q)$(MAKE) -j -f $(COM_BUILD_DIR)/phy_stub.mak clean OBJ_DIR=$(OBJ_ROOT)/odu LIB_DIR=$(LIB_ROOT)/odu LOG_DIR=$(LOG_ROOT)/odu CC='$(CC1)'
endif
ifeq ($(O1_ENABLE),YES)
		$(Q)$(MAKE) -j -f $(COM_BUILD_DIR)/o1.mak clean OBJ_DIR=$(OBJ_ROOT)/odu LIB_DIR=$(LIB_ROOT)/odu LOG_DIR=$(LOG_ROOT)/odu CC='$(CCPP1)'
		$(Q)$(MAKE) -j -f $(COM_BUILD_DIR)/ves.mak clean OBJ_DIR=$(OBJ_ROOT)/odu LIB_DIR=$(LIB_ROOT)/odu LOG_DIR=$(LOG_ROOT)/odu CC='$(CCPP1)'
endif
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
	 $(Q)$(MAKE) -j -f $(COM_BUILD_DIR)/cu_stub.mak OBJ_DIR=$(OBJ_ROOT)/cu_stub LIB_DIR=$(LIB_ROOT)/cu_stub LOG_DIR=$(LOG_ROOT)/cu_stub CC='$(CC1)'
	 $(Q)$(MAKE) -j -f $(COM_BUILD_DIR)/asn_common.mak OBJ_DIR=$(OBJ_ROOT)/cu_stub LIB_DIR=$(LIB_ROOT)/cu_stub LOG_DIR=$(LOG_ROOT)/cu_stub CC='$(CC1)'
	 $(Q)$(MAKE) -j -f $(COM_BUILD_DIR)/asn_f1ap.mak OBJ_DIR=$(OBJ_ROOT)/cu_stub LIB_DIR=$(LIB_ROOT)/cu_stub LOG_DIR=$(LOG_ROOT)/cu_stub CC='$(CC1)'
	 $(Q)$(MAKE) -j -f $(COM_BUILD_DIR)/asn_rrc.mak OBJ_DIR=$(OBJ_ROOT)/cu_stub LIB_DIR=$(LIB_ROOT)/cu_stub LOG_DIR=$(LOG_ROOT)/cu_stub CC='$(CC1)'
	 $(Q)$(MAKE) -j -f $(COM_BUILD_DIR)/cm.mak OBJ_DIR=$(OBJ_ROOT)/cu_stub LIB_DIR=$(LIB_ROOT)/cu_stub LOG_DIR=$(LOG_ROOT)/cu_stub CC='$(CC1)'
	 $(Q)$(MAKE) -j -f $(COM_BUILD_DIR)/mt.mak OBJ_DIR=$(OBJ_ROOT)/cu_stub LIB_DIR=$(LIB_ROOT)/cu_stub LOG_DIR=$(LOG_ROOT)/cu_stub CC='$(CC1)'
ifeq ($(O1_ENABLE),YES)
		$(Q)$(MAKE) -j -f $(COM_BUILD_DIR)/o1.mak OBJ_DIR=$(OBJ_ROOT)/cu_stub LIB_DIR=$(LIB_ROOT)/cu_stub LOG_DIR=$(LOG_ROOT)/cu_stub CC='$(CCPP1)'
		$(Q)$(MAKE) -j -f $(COM_BUILD_DIR)/ves.mak OBJ_DIR=$(OBJ_ROOT)/cu_stub LIB_DIR=$(LIB_ROOT)/cu_stub LOG_DIR=$(LOG_ROOT)/cu_stub CC='$(CCPP1)'
endif

clean_cu:
	 $(Q)$(MAKE) -j -f $(COM_BUILD_DIR)/cu_stub.mak clean OBJ_DIR=$(OBJ_ROOT)/cu_stub LIB_DIR=$(LIB_ROOT)/cu_stub LOG_DIR=$(LOG_ROOT)/cu_stub CC='$(CC1)'
	 $(Q)$(MAKE) -j -f $(COM_BUILD_DIR)/asn_common.mak clean OBJ_DIR=$(OBJ_ROOT)/cu_stub LIB_DIR=$(LIB_ROOT)/cu_stub LOG_DIR=$(LOG_ROOT)/cu_stub CC='$(CC1)'
	 $(Q)$(MAKE) -j -f $(COM_BUILD_DIR)/asn_f1ap.mak clean OBJ_DIR=$(OBJ_ROOT)/cu_stub LIB_DIR=$(LIB_ROOT)/cu_stub LOG_DIR=$(LOG_ROOT)/cu_stub CC='$(CC1)'
	 $(Q)$(MAKE) -j -f $(COM_BUILD_DIR)/asn_rrc.mak clean OBJ_DIR=$(OBJ_ROOT)/cu_stub LIB_DIR=$(LIB_ROOT)/cu_stub LOG_DIR=$(LOG_ROOT)/cu_stub CC='$(CC1)'
	 $(Q)$(MAKE) -j -f $(COM_BUILD_DIR)/cm.mak clean OBJ_DIR=$(OBJ_ROOT)/cu_stub LIB_DIR=$(LIB_ROOT)/cu_stub LOG_DIR=$(LOG_ROOT)/cu_stub CC='$(CC1)'
	 $(Q)$(MAKE) -j -f $(COM_BUILD_DIR)/mt.mak clean OBJ_DIR=$(OBJ_ROOT)/cu_stub LIB_DIR=$(LIB_ROOT)/cu_stub LOG_DIR=$(LOG_ROOT)/cu_stub CC='$(CC1)'
ifeq ($(O1_ENABLE),YES)
		$(Q)$(MAKE) -j -f $(COM_BUILD_DIR)/o1.mak clean OBJ_DIR=$(OBJ_ROOT)/cu_stub LIB_DIR=$(LIB_ROOT)/cu_stub LOG_DIR=$(LOG_ROOT)/cu_stub CC='$(CCPP1)'
		$(Q)$(MAKE) -j -f $(COM_BUILD_DIR)/ves.mak clean OBJ_DIR=$(OBJ_ROOT)/cu_stub LIB_DIR=$(LIB_ROOT)/cu_stub LOG_DIR=$(LOG_ROOT)/cu_stub CC='$(CCPP1)'
endif
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
		$(Q)cp -f ../scripts/start_cu_stub_logging.sh $(ROOT_DIR)/bin/cu_stub
		$(Q)echo -e "***** CU STUB BUILD COMPLETE *****"

ric: 
	 $(Q)$(MAKE) -j -f $(COM_BUILD_DIR)/ric_stub.mak OBJ_DIR=$(OBJ_ROOT)/ric_stub LIB_DIR=$(LIB_ROOT)/ric_stub LOG_DIR=$(LOG_ROOT)/ric_stub CC='$(CC1)'
	 $(Q)$(MAKE) -j -f $(COM_BUILD_DIR)/asn_common.mak OBJ_DIR=$(OBJ_ROOT)/ric_stub LIB_DIR=$(LIB_ROOT)/ric_stub LOG_DIR=$(LOG_ROOT)/ric_stub CC='$(CC1)'
	 $(Q)$(MAKE) -j -f $(COM_BUILD_DIR)/asn_e2ap.mak OBJ_DIR=$(OBJ_ROOT)/ric_stub LIB_DIR=$(LIB_ROOT)/ric_stub LOG_DIR=$(LOG_ROOT)/ric_stub CC='$(CC1)'
	 $(Q)$(MAKE) -j -f $(COM_BUILD_DIR)/asn_e2ap_kpm.mak OBJ_DIR=$(OBJ_ROOT)/ric_stub LIB_DIR=$(LIB_ROOT)/ric_stub LOG_DIR=$(LOG_ROOT)/ric_stub CC='$(CC1)'
	 $(Q)$(MAKE) -j -f $(COM_BUILD_DIR)/cm.mak OBJ_DIR=$(OBJ_ROOT)/ric_stub LIB_DIR=$(LIB_ROOT)/ric_stub LOG_DIR=$(LOG_ROOT)/ric_stub CC='$(CC1)'
	 $(Q)$(MAKE) -j -f $(COM_BUILD_DIR)/mt.mak OBJ_DIR=$(OBJ_ROOT)/ric_stub LIB_DIR=$(LIB_ROOT)/ric_stub LOG_DIR=$(LOG_ROOT)/ric_stub CC='$(CC1)'
ifeq ($(O1_ENABLE),YES)
		$(Q)$(MAKE) -j -f $(COM_BUILD_DIR)/o1.mak OBJ_DIR=$(OBJ_ROOT)/ric_stub LIB_DIR=$(LIB_ROOT)/ric_stub LOG_DIR=$(LOG_ROOT)/ric_stub CC='$(CCPP1)'
		$(Q)$(MAKE) -j -f $(COM_BUILD_DIR)/ves.mak OBJ_DIR=$(OBJ_ROOT)/ric_stub LIB_DIR=$(LIB_ROOT)/ric_stub LOG_DIR=$(LOG_ROOT)/ric_stub CC='$(CCPP1)'
endif

clean_ric:
	 $(Q)$(MAKE) -j -f $(COM_BUILD_DIR)/ric_stub.mak clean OBJ_DIR=$(OBJ_ROOT)/ric_stub LIB_DIR=$(LIB_ROOT)/ric_stub LOG_DIR=$(LOG_ROOT)/ric_stub CC='$(CC1)'
	 $(Q)$(MAKE) -j -f $(COM_BUILD_DIR)/asn_common.mak clean OBJ_DIR=$(OBJ_ROOT)/ric_stub LIB_DIR=$(LIB_ROOT)/ric_stub LOG_DIR=$(LOG_ROOT)/ric_stub CC='$(CC1)'
	 $(Q)$(MAKE) -j -f $(COM_BUILD_DIR)/asn_e2ap.mak clean OBJ_DIR=$(OBJ_ROOT)/ric_stub LIB_DIR=$(LIB_ROOT)/ric_stub LOG_DIR=$(LOG_ROOT)/ric_stub CC='$(CC1)'
	 $(Q)$(MAKE) -j -f $(COM_BUILD_DIR)/asn_e2ap_kpm.mak clean OBJ_DIR=$(OBJ_ROOT)/ric_stub LIB_DIR=$(LIB_ROOT)/ric_stub LOG_DIR=$(LOG_ROOT)/ric_stub CC='$(CC1)'
	 $(Q)$(MAKE) -j -f $(COM_BUILD_DIR)/asn_rrc.mak clean OBJ_DIR=$(OBJ_ROOT)/ric_stub LIB_DIR=$(LIB_ROOT)/ric_stub LOG_DIR=$(LOG_ROOT)/ric_stub CC='$(CC1)'
	 $(Q)$(MAKE) -j -f $(COM_BUILD_DIR)/cm.mak clean OBJ_DIR=$(OBJ_ROOT)/ric_stub LIB_DIR=$(LIB_ROOT)/ric_stub LOG_DIR=$(LOG_ROOT)/ric_stub CC='$(CC1)'
	 $(Q)$(MAKE) -j -f $(COM_BUILD_DIR)/mt.mak clean OBJ_DIR=$(OBJ_ROOT)/ric_stub LIB_DIR=$(LIB_ROOT)/ric_stub LOG_DIR=$(LOG_ROOT)/ric_stub CC='$(CC1)'
ifeq ($(O1_ENABLE),YES)
		$(Q)$(MAKE) -j -f $(COM_BUILD_DIR)/o1.mak clean OBJ_DIR=$(OBJ_ROOT)/ric_stub LIB_DIR=$(LIB_ROOT)/ric_stub LOG_DIR=$(LOG_ROOT)/ric_stub CC='$(CCPP1)'
		$(Q)$(MAKE) -j -f $(COM_BUILD_DIR)/ves.mak clean OBJ_DIR=$(OBJ_ROOT)/ric_stub LIB_DIR=$(LIB_ROOT)/ric_stub LOG_DIR=$(LOG_ROOT)/ric_stub CC='$(CCPP1)'
endif
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
		$(Q)cp -f ../scripts/start_ric_stub_logging.sh $(ROOT_DIR)/bin/ric_stub
		$(Q)echo -e "***** RIC STUB BUILD COMPLETE *****"


copy_build: link_du
			$(Q)cp -f ./obj/odu/odu ./bin/odu
			$(Q)cp -rf ./bin/odu $(ROOT_DIR)/bin/
			$(Q)cp -f ../scripts/start_du_logging.sh $(ROOT_DIR)/bin/odu
			$(Q)cp -rf ../config/ $(ROOT_DIR)/bin/odu
			$(Q)cp -f ./lib/odu/*.a $(ROOT_DIR)/libs/
		   $(Q)echo -e "***** BUILD COMPLETE *****"

odu: prepare_dirs copy_build
cu_stub: prepare_cu_dirs cu link_cu
ric_stub: prepare_ric_dirs ric link_ric

#**********************************************************************
#         End of file
#**********************************************************************
