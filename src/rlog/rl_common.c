/*******************************************************************************
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
*******************************************************************************/

/********************************************************************20**

     Name:     Radisys Logging Framework
     Type:     C source file
     Desc:     This file contains logging framework implementation.
     File:     rl_common.c

*********************************************************************21*/
/****************************************************************************
@ description: This is common source file for postprocessor and log library.
*****************************************************************************/

#include <stdio.h>
#include "rl_common.h"

/* String representation of Log Levels */
const char* g_logStr[L_MAX_LOG_LEVEL] =
{
   "ALWAYS",
   "FATAL",
   "ERROR",
   "WARNING",
   "EVENT",
   "INFO",
   "DEBUG"
};

const char* g_splStr[DBG_MAX_IDs] =
{
   "CELLID",
   "PEERID",
   "ENBID",
   "MME",
   "CRNTI", 
   "UEIDX",
   "UEID", 
   "RBID", 
   "LCID", 
   "LCGID", 
   "TRNSID"
};

void hextostr(char* p, PSTR h, int hexlen)
{
   int i;
   for(i=0; i<hexlen; i++, p+=3, h++)
      sprintf(p, "%02x ", *h);
}

/**********************************************************************
  End of file
 **********************************************************************/
