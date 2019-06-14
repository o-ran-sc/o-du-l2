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
#Makefile for product ib - script generated.
#Only the $(CCib) may be modified.
#-------------------------------------------------------------#

include ../common/rsys_fancy.mak

include ../common/env.mak

SEC_ENG=SWLIB

SRC_DIR=$(ROOT_DIR)/src/5gnrpdcp

#Prepare source file list
C_SRCS=$(wildcard $(SRC_DIR)/*.c)

#prepare the list of object files and RLOG related files
C_OBJS=$(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(C_SRCS))

#prepare the header file list for this layer
# prepare the list of common header files
HDR_FILES+=$(wildcard $(CM_DIR)/env*.[hx])
HDR_FILES+=$(wildcard $(CM_DIR)/gen*.[hx])
HDR_FILES+=$(wildcard $(CM_DIR)/ssi*.[hx])
HDR_FILES+=$(wildcard $(CM_DIR)/cm*.[hx])
HDR_FILES+=$(wildcard $(CM_DIR)/lpj.[hx])
HDR_FILES+=$(wildcard $(CM_DIR)/kwu.[hx])
HDR_FILES+=$(wildcard $(CM_DIR)/pju.[hx])
HDR_FILES+=$(wildcard $(CM_DIR)/cpj.[hx])

# Add the product specific header files
HDR_FILES+= $(wildcard $(SRC_DIR)/*.[hx])

#-------------------------------------------------------------#
#User macros (to be modified)
#-------------------------------------------------------------#
MOD_FLAGS= -UKW_PDCP -DKW -DPJ -DPTPJLIB 


lib:$(LIB_DIR)/libpj.a
include $(COM_BUILD_DIR)/compile.mak

#-------------------------------------------------------------#
#Linker macros             
#-------------------------------------------------------------#
$(LIB_DIR)/libpj.a:$(C_OBJS) $(C_WO_WERR_OBJS)
	@echo -e "Creating Archive $(COLOR) $@ $(REVERT_COLOR)"
	$(Q)ar -cr $(LIB_DIR)/libpj.a $(C_OBJS) $(C_WO_WERR_OBJS)	

#-------------------------------------------------------------#
#Clean macros             
#-------------------------------------------------------------#
clean:
	@echo -e "$(COLOR_RED)Cleaning PDCP$(REVERT_COLOR)"
	$(Q)\rm -f $(LIB_DIR)/libpj.a $(C_OBJS) $(C_WO_WERR_OBJS)


