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

# This is makefile for ASN

include ../common/rsys_fancy.mak
include ../common/env.mak
COLOR=$(COLOR_RED)

SRC_DIR=$(ROOT_DIR)/src/codec_utils/F1AP
C_SRCS=$(wildcard $(SRC_DIR)/*.c)
C_OBJS=$(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(C_SRCS))

lib: $(LIB_DIR)/libasn_f1ap.a
include $(COM_BUILD_DIR)/compile.mak

I_OPTS+=-I$(SRC_DIR)/../common
#-------------------------------------------------------------#
#Linker macros
#-------------------------------------------------------------#
$(LIB_DIR)/libasn_f1ap.a:$(C_OBJS) 
		  @echo -e "Creating Archive $(COLOR) $@ $(REVERT_COLOR)"
		  $(Q)ar -cr $(LIB_DIR)/libasn_f1ap.a $(C_OBJS)


#-------------------------------------------------------------#
#Clean macros
#-------------------------------------------------------------#
clean:
		  @echo -e "$(COLOR_RED)Cleaning ASN$(REVERT_COLOR)"
		  @echo $(SRC_DIR) $(CM_DIR)
		  $(Q)\rm -f $(LIB_DIR)/libasn_f1ap.a $(C_OBJS) 

#**********************************************************************
#         End of file
#**********************************************************************
