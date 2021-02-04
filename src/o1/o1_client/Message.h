/*******************************************************************************
################################################################################
#   Copyright (c) [2020] [HCL Technologies Ltd.]                               #
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
*******************************************************************************/

/* This file contains definitions of common message structures */

#ifndef __MESSAGE_H__
#define __MESSAGE_H__

#include <string.h>

typedef enum
{
   RAISE_ALARM,
   CLEAR_ALARM,
   GET_STARTUP_CONFIG
}MsgAction;

typedef enum
{
   ALARM,
   CONFIGURATION
}MsgType;

typedef struct
{
   MsgType msgType;
   MsgAction action;
}MsgHeader;

#endif

/**********************************************************************
         End of file
**********************************************************************/
