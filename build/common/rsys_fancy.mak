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

#********************************************************************20**
#
#       Name:   RSYS FANCY MAKE OUTOUT
#
#       Type:   Makefile
#
#       Desc:   To control fancy features in the build process
#
#
#       File:   rsys_fancy.mak
#
#********************************************************************21*/

#-----------------------------------------------------------------------
# FANCY STUFF... you can play around with this if you want to
COLOR_RED=\e[31m\e[1m
COLOR_GREEN=\e[32m\e[1m
COLOR_YELLOW=\e[33m\e[1m
COLOR_BLUE=\e[34m\e[1m
COLOR_MAG=\e[35m\e[1m
COLOR_CYAN=\e[36m\e[1m
REVERT_COLOR=\e[m

#-----------------------------------------------------------------------
# DO NOT EDIT THIS SECTION
ifeq ($(VERBOSE),YES)
export QUIET=
MK_QUIET=
else
export QUIET=@ 
MK_QUIET=-s # Make works in quite mode
endif

#**********************************************************************
#         End of file
#**********************************************************************
