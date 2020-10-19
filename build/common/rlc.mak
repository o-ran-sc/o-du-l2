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
##Makefile for product ib - script generated.
#Only the $(CCib) may be modified.
#-------------------------------------------------------------#
include ../common/rsys_fancy.mak
include ../common/env.mak

SRC_DIR=$(ROOT_DIR)/src/5gnrrlc

#Prepare source file list
C_SRCS=$(wildcard $(SRC_DIR)/*.c)

#prepare the list of object files and RLOG related files
C_OBJS=$(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(C_SRCS))

#-------------------------------------------------------------#
#User macros (to be modified)
#-------------------------------------------------------------#
MOD_FLAGS=-DRG -DYS -DKW -DLCRGU -DLCKWULUDX -DLCUDX -DLWLCKWULUDX -UPJ

#-------------------------------------------------------------#
#Linker macros             
#-------------------------------------------------------------#

lib: $(LIB_DIR)/libkw.a
include $(COM_BUILD_DIR)/compile.mak

$(LIB_DIR)/libkw.a:$(C_OBJS)
	@echo -e "Creating Archive $(COLOR) $@ $(REVERT_COLOR)"
	$(Q)ar -cr $(LIB_DIR)/libkw.a $(C_OBJS)	

#-------------------------------------------------------------#
#Clean macros             
#-------------------------------------------------------------#
clean:
	@echo -e "$(COLOR_RED)Cleaning RLC$(REVERT_COLOR)"
	$(Q)\rm -f $(LIB_DIR)/libkw.a $(C_OBJS)

#**********************************************************************
#         End of file
#**********************************************************************
