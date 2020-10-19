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

#-------------------------------------------------------------#
#Makefile for product cm - script generated.
#Only the $(CCcmFLAGS) may be modified.
#-------------------------------------------------------------#
include ../common/rsys_fancy.mak
include ../common/env.mak
COLOR=$(COLOR_GREEN)
SRC_DIR=$(ROOT_DIR)/src/cm

EG_DIR=$(ROOT_DIR)/src/egtpu
SZ_DIR=$(ROOT_DIR)/src/s1ap
CZ_DIR=$(ROOT_DIR)/src/x2ap

# TODO - The dependency between layers and CM files should be removed #
I_OPTS+=-I$(EG_DIR) -I$(SZ_DIR) -I$(CZ_DIR)
I_OPTS+=-I$(ROOT_DIR)/src/du_app
I_OPTS+=-I$(ROOT_DIR)/src/codec_utils/common

#-------------------------------------------------------------#
# Define all the layers that are part of this binary so that  #
# their coupling requirements get defined from envopt.h.      #
#-------------------------------------------------------------#
MOD_FLAGS=-DNH -DWR -DNX -DSZ -DCZ -DSB -DHI -DEG -DSM -DNL -DSM -DRG -DKW -DYS
#This module does not support binary logging so disable it
TEXT_LOG=YES

#-------------------------------------------------------------#
# Source file list. This needs to be explicitly built as CM
# directory contains files for both CPUL and CPUH.
#-------------------------------------------------------------#
C_SRCS=$(wildcard $(SRC_DIR)/*.c)
C_SRCS:=$(filter-out %cm_ss.c, $(C_SRCS))
C_SRCS:=$(filter-out %egt_util.c, $(C_SRCS))
C_SRCS:=$(filter-out %lve.c, $(C_SRCS))
C_SRCS:=$(filter-out %cm_atm.c, $(C_SRCS))
C_SRCS:=$(filter-out %_ptsp.c, $(C_SRCS))
C_SRCS:=$(filter-out %_se_tst.c, $(C_SRCS))
ifeq ($(PLTFRM),XEON)
C_SRCS:=$(filter-out %mem_wl.c, $(C_SRCS))
else
C_SRCS:=$(filter-out %mem.c, $(C_SRCS))
endif

C_OBJS_WO_LOG=$(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(C_SRCS))
HDR_FILES+= $(wildcard $(SRC_DIR)/*.[hx])


lib: $(LIB_DIR)/libcm.a

include $(COM_BUILD_DIR)/compile.mak

#-------------------------------------------------------------#
#Linker macros             
#-------------------------------------------------------------#
$(LIB_DIR)/libcm.a:$(C_OBJS_WO_LOG)
	@echo -e "Creating Archive $(COLOR) $@ $(REVERT_COLOR)"
	$(Q)ar -cr $(LIB_DIR)/libcm.a $(C_OBJS)	
#-------------------------------------------------------------#
#Clean macros             
#-------------------------------------------------------------#
clean:
	@echo -e "$(COLOR_RED)Cleaning CPUH CM$(REVERT_COLOR)"
	@echo -e "$(COLOR_RED) $(REVERT_COLOR)"
	$(Q)\rm -f $(LIB_DIR)/libcm.a $(C_OBJS_WO_LOG)

#**********************************************************************
#         End of file
#**********************************************************************
