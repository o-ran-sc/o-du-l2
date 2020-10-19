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
#
include ../common/rsys_fancy.mak
COLOR=$(COLOR_GREEN)

#-------------------------------------------------------------#
#Makefile for RL
#-------------------------------------------------------------#
MOD_FLAGS=-UUSE_RLOG_DATA_TYPES 

SRC_DIR=$(ROOT_DIR)/src/rlog
CM_DIR =$(ROOT_DIR)/src/cm

# Product sources-------------------------------------------------------
C_SRCS=$(SRC_DIR)/rl_rlog.c $(SRC_DIR)/rl_common.c $(SRC_DIR)/rl_platform.c $(SRC_DIR)/rl_soc.c
rlPOSTPROCSRCS=$(SRC_DIR)/rl_common.c $(SRC_DIR)/rl_postproc.cpp

# Product includes -------------------------------------------------------
HDR_FILES=$(SRC_DIR)/rl_common.h  $(SRC_DIR)/rl_rlog.h  $(SRC_DIR)/rl_interface.h \
		  $(SRC_DIR)/rl_platform.h
HDR_FILES+=$(wildcard $(CM_DIR)/*.)

# Object files ------------------------------------
C_OBJS_WO_LOG=$(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(C_SRCS))

#-------------------------------------------------------------#
#Makefile for RL
#-------------------------------------------------------------#
rlPOSTPROCOPTS=-DSS_LINUX -DUSE_RLOG_DATA_TYPES

lib:$(LIB_DIR)/librl.a

include $(COM_BUILD_DIR)/compile.mak

#-------------------------------------------------------------#
#Linker macros             
#-------------------------------------------------------------#
$(LIB_DIR)/librl.a:$(C_OBJS_WO_LOG)
	@echo -e "Creating Archive $(COLOR) $@ $(REVERT_COLOR)"
	$(Q)$(AR) -cr $(OBJ_DIR)/librl.a $(C_OBJS)	
	@echo -e "$(COLOR)Building the binary logging post processor$(REVERT_COLOR)"
	$(Q)g++ $(rlPOSTPROCOPTS) $(rlPOSTPROCSRCS) -o	$(OBJ_DIR)/rlogapp

#-------------------------------------------------------------#
#Clean macros             
#-------------------------------------------------------------#
clean:
	@echo -e "$(COLOR_RED)Clearing binary logging$(REVERT_COLOR)"
	$(Q)rm -f $(LIB_DIR)/librl.a $(C_OBJS_WO_LOG)

#**********************************************************************
#         End of file
#**********************************************************************
