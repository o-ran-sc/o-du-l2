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

# This is makefile for O1 module

include ../common/rsys_fancy.mak
include ../common/env.mak
COLOR=$(COLOR_RED)

SRC_DIR=$(ROOT_DIR)/src/o1/ves
CPP_SRCS=$(wildcard $(SRC_DIR)/*.cpp)
CPP_OBJS=$(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(CPP_SRCS))

PLTFRM_FLAGS=
lib: $(LIB_DIR)/libo1.a
include $(COM_BUILD_DIR)/compile.mak

L_OPTS=-lsysrepo -lyang
L_OPTS+= -lsysrepo-cpp -lyang-cpp
L_OPTS+= -lm -lpthread
L_OPTS+= -lnetconf2 -lcjson -lcurl
I_OPTS=-I$(ROOT_DIR)/src/o1/ves
I_OPTS+=-I$(ROOT_DIR)/src/o1

#-------------------------------------------------------------#
#Linker macros
#-------------------------------------------------------------#

$(LIB_DIR)/libo1.a:$(CPP_OBJS)
		  @echo -e "Creating Archive $(COLOR) $@ $(REVERT_COLOR)"
		  $(Q)ar -cr $(LIB_DIR)/libves.a $(CPP_OBJS)

#-------------------------------------------------------------#
#Clean macros
#-------------------------------------------------------------#
clean:
		  @echo -e "$(COLOR_RED)Cleaning O1$(REVERT_COLOR)"
		  $(Q)\rm -f $(LIB_DIR)/libves.a $(CPP_OBJS)

#**********************************************************************
#         End of file
#**********************************************************************

