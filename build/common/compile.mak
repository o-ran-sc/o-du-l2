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

#-DSSINT2 -DSS_TICKS_SEC=100 
# MTSS defines
SS_FLAGS=-DSS -DSS_MT -DSUNOS -DSS_LINUX -UCONRD -UNOFILESYS -USS_DEBG_MEM_CORRUP_LEVEL1 \
    -DSS_TICKS_SEC=100 -USSI_DEBUG_LEVEL1 -USS_LICENSE_CHECK -DANSI -D_GNU_SOURCE\
    -D_REENTRANT -D__EXTENSIONS__ -DDEBUGNOEXIT 

# Flags to be used if text logging is enabled
TEXT_LOGGING = -DRLOG_ENABLE_TEXT_LOGGING -DRLOG_DEBUG_MODE

ifeq ($(INSTRUMENTATION), YES)
INSTRUMENTATION_FLAGS = -DSS_MEM_CORRUPTION_DETECTION 
endif

# Flags pertaining to the functionality of TeNB are included here
TENB_FLAGS=-DLTERRC_REL9 -DEGTP_U_REL_9 -UMAC_SCH_STATS -USCH_STATS \
    -DTENB_TTI_PERF -DAES -DLTE_RRC_DISSECTOR -DRSYS_WIRESHARK

#ifneq ($(PLTFRM), PAL)
#TENB_FLAGS += -DIPV6_SUPPORTED
#endif

ifeq ($(RADIO_CLUSTER), YES)
TENB_FLAGS += -DRADIO_CLUSTER
endif
# Flags that define the broad level functioning of the binary
ENV_FLAGS=-DNO_ERRCLS -DNOERRCHK -DDEBUGP 

RVW_FLAGS=-DCM_PASN_DBG -DLTEMAC_MIMO -DWR_UL_PWR \
     -DREVIEW -DNL

# Flags that can be removed when everything works. These flags are not included
# in the compilation at the moment.
DEL_FLAGS=-DSS_WL_REGION=1 -DVE_PERF_MEAS -UVE_SUPPORT_RLC_UM_MODE -ULTE_LNX_AFFINITY \
     -DKW_BG_DL_PROC -DKW_BG_UL_PROC -DWR_SB_SCTP -DPHY_ERROR_LOGGING -DWR_DETECT_RLF \
     -USS_MEM_CORRUPTION_DETECTION -DL2_LOGGING_ENABLED -DLTEMAC_DRX -UUSE_PURE \
     -DLTE_MULTIUE -DCMKV2 -DYS_ENB_CFG -DTA_NEW -DTENB_DISABLE_DL_ZBC

ALL_FLAGS=$(SS_FLAGS) $(CMN_FLAGS) $(ENV_FLAGS) $(RVW_FLAGS) $(TENB_FLAGS) $(LNXENV)

# compiler options:
#C_OPTS+=-g -O3 -pipe -pedantic -Wall -Werror -Wno-comment -Wshadow 
C_OPTS+=-g -pipe -Wall -Wno-comment -Wshadow \
    -Wcast-qual -fno-strict-aliasing -fsigned-char --std=c99

COPTS_WO_WERROR=-g -O3 -pipe -pedantic -Wall -Wno-comment -Wshadow \
    -Wcast-qual -fno-strict-aliasing -fsigned-char --std=c99

xCPP_OPTS+=-g -pipe -pedantic -Wall -Werror -Wno-comment -Wshadow \
    -Wcast-qual -fno-strict-aliasing -fsigned-char -lstdc++ 

# Include path settings
I_OPTS+=-I$(SRC_DIR) -I$(ROOT_DIR)/src/cm -I$(ROOT_DIR)/src/mt -I$(ROOT_DIR)/src/wls_lib
I_OPTS+=$(PLTFRM_INCLUDES)
.SUFFIXES:.c .o

$(C_OBJS):$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HDR_FILES)
	@echo -e "Compiling $(COLOR_RED) $< $(REVERT_COLOR)"
	$(Q)$(CC) -c -o $@ $(C_OPTS) $(I_OPTS) $(ALL_FLAGS) $(TEXT_LOGGING) $(MOD_FLAGS) \
    $(PLTFRM_FLAGS) $<

$(C_WO_WERR_OBJS):$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HDR_FILES)
	@echo -e "Compiling $(COLOR_RED) $< $(REVERT_COLOR)"
	$(Q)$(CC) -c -o $@ $(COPTS_WO_WERROR) $(I_OPTS) $(ALL_FLAGS) $(TEXT_LOGGING) $(MOD_FLAGS) \
    $(PLTFRM_FLAGS) $<

$(CPP_OBJS):$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(HDR_FILES)
	@echo -e "Compiling $(COLOR_RED) $< $(REVERT_COLOR)"
	$(Q)$(CC) -c -o $@ $(CPP_OPTS) $(I_OPTS) $(ALL_FLAGS) $(TEXT_LOGGING) $(MOD_FLAGS) \
    $(PLTFRM_FLAGS) $<

$(C_OBJS_WO_LOG):$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HDR_FILES)
	@echo -e "Compiling $(COLOR_RED) $< $(REVERT_COLOR)"
	$(Q)$(CC) -c -o $@ $(C_OPTS) $(I_OPTS) $(ALL_FLAGS) $(TEXT_LOGGING) $(MOD_FLAGS) \
    $(PLTFRM_FLAGS) $<

$(CPP_OBJS_WO_LOG):$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(HDR_FILES)
	@echo -e "Compiling $(COLOR_RED) $< $(REVERT_COLOR)"
	$(Q)$(CC) -c -o $@ $(CPP_OPTS) $(I_OPTS) $(ALL_FLAGS) $(TEXT_LOGGING) $(MOD_FLAGS) \
    $(PLTFRM_FLAGS) $<

#**********************************************************************
#         End of file
#**********************************************************************
