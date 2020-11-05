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
##-------------------------------------------------------------#
#Makefile for product mt - script generated.
#Only the $(CCmtFLAGS) may be modified.
#-------------------------------------------------------------#
include ../common/rsys_fancy.mak
include ../common/env.mak

#-------------------------------------------------------------#
#User macros (to be modified)
#-------------------------------------------------------------#

CCmtFLAGS=-DSS_MT_TMR -USS_LOGGER_SUPPORT -DEGTP_U#<---Insert mt specific defines here

#-------------------------------------------------------------#
#File/Obj macros             
#-------------------------------------------------------------#
SRC_DIR=$(ROOT_DIR)/src/mt

C_SRCS=$(wildcard $(SRC_DIR)/*.c)
C_SRCS:=$(filter-out %ss_acc.c,$(C_SRCS))
C_SRCS:=$(filter-out %mt_tst.c,$(C_SRCS))
C_SRCS:=$(filter-out %sol.c, $(C_SRCS))
C_SRCS:=$(filter-out %diag.c, $(C_SRCS))
C_SRCS:=$(filter-out %mtsp.c, $(C_SRCS))
C_SRCS:=$(filter-out %tskent.c, $(C_SRCS))

ifeq ($(PLTFRM),PAL)
C_SRCS:=$(filter-out %4gmx.c, $(C_SRCS))
endif
ifneq ($(BOARD), T33H)
C_SRCS:=$(filter-out %t33.c, $(C_SRCS))
endif

C_OBJS_WO_LOG=$(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(C_SRCS))
I_OPTS+=-isystem $(BSPPATH)/usr/include/brcm
I_OPTS+=-I$(SRC_DIR)/security
I-OPTS+=-I$(ROOT_DIR)/src/wls_lib
I_OPTS+=-I$(ROOT_DIR)/src/dpdk_lib

HDR_FILES+=$(wildcard $(CM_DIR)/env*.[hx])
HDR_FILES+=$(wildcard $(CM_DIR)/gen*.[hx])
HDR_FILES+=$(wildcard $(CM_DIR)/ssi*.[hx])
HDR_FILES+=$(wildcard $(CM_DIR)/cm*.[hx])
HDR_FILES+=$(wildcard $(SRC_DIR)/*.[hx])
HDR_FILES+=$(wildcard $(SRC_DIR)/*.[hx])

lib:$(LIB_DIR)/libmt.a

include $(COM_BUILD_DIR)/compile.mak

#-------------------------------------------------------------#
#Linker macros             
#-------------------------------------------------------------#
$(LIB_DIR)/libmt.a:$(C_OBJS_WO_LOG)
	$(Q)ar -cr $(LIB_DIR)/libmt.a $(C_OBJS_WO_LOG)	

#-------------------------------------------------------------#
#Clean macros             
#-------------------------------------------------------------#
clean:
	@echo -e "$(COLOR_RED)Cleaning SSI from $(OBJ_DIR)$(REVERT_COLOR)"
	$(Q)\rm -f $(LIB_DIR)/libmt.a $(C_OBJS_WO_LOG)

#**********************************************************************
#         End of file
#**********************************************************************
