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

##-------------------------------------------------------------#
#Makefile for product RG - script generated.
#-------------------------------------------------------------#
include ../common/rsys_fancy.mak
include ../common/env.mak
COLOR=$(COLOR_GREEN)

SRC_DIR=$(ROOT_DIR)/src/5gnrsch

# prepare the list of common header files
HDR_FILES+=$(wildcard $(CM_DIR)/env*.[hx])
HDR_FILES+=$(wildcard $(CM_DIR)/gen*.[hx])
HDR_FILES+=$(wildcard $(CM_DIR)/ssi*.[hx])
HDR_FILES+=$(wildcard $(CM_DIR)/cm*.[hx])
HDR_FILES+=$(wildcard $(CM_DIR)/crg.[hx])
HDR_FILES+=$(wildcard $(CM_DIR)/lrg*.[hx])
HDR_FILES+=$(wildcard $(CM_DIR)/rgr.[hx])
HDR_FILES+=$(wildcard $(CM_DIR)/rgm.[hx])

# Add the product specific header files
HDR_FILES+= $(wildcard $(SRC_DIR)/*.[hx])

#prepare the list of source files
C_SRCS=$(wildcard $(SRC_DIR)/*.c)
#C_SRCS:=$(filter-out $(SRC_DIR)/rg_sch%, $(C_SRCS))

#prepare the list of object files and RLOG related files
C_OBJS=$(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(C_SRCS))

#-------------------------------------------------------------#
#User macros (to be modified)
#-------------------------------------------------------------#
# Including RG_PHASE2_SCHED for supporting more than one schedulers 
# supported by mac
# TODO: make it define for LTEMAC_MIMO and remove it from envopt.h
MOD_FLAGS=-DRGM_LC -DRGM_LWLC -USM -URG_DEBUG -DxRG_PHASE2_SCHED -DxRGR_V1 \
    -DRG_UL_DELTA=2 -ULTEMAC_DLUE_TMGOPTMZ -UTENB_SPLIT_ARCH -DRG -ULTEMAC_MIMO

lib:$(LIB_DIR)/librg.a
include $(COM_BUILD_DIR)/compile.mak

I_OPTS+=-I$(ROOT_DIR)/src/5gnrmac

#-------------------------------------------------------------#
#Linker macros             
#-------------------------------------------------------------#
$(LIB_DIR)/librg.a:$(C_OBJS)
	@echo -e "Creating Archive $(COLOR) $@ $(REVERT_COLOR)"
	$(Q)ar -cr $(LIB_DIR)/librg.a $(C_OBJS)	

#-------------------------------------------------------------#
#Clean macros             
#-------------------------------------------------------------#
clean:
	@echo -e "$(COLOR_RED)Cleaing MAC$(REVERT_COLOR)"
	$(Q)\rm -f $(LIB_DIR)/librg.a $(C_OBJS)

#**********************************************************************
#         End of file
#**********************************************************************
