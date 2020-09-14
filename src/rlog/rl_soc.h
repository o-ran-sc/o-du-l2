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
     File:     rl_soc.c

*********************************************************************21*/
/**********************************************************************
 @ description: This source file has SOC specific functions required for 
 logging framework
************************************************************************/

#define L2LOG_BUFF_SIZE 10000
#define L2LOG_BUFF_BLOCK_SIZE L2LOG_BUFF_SIZE/4
#define RLOGTICKSCNTTOPRCL2LOGS 10
#define TENB_L2_PROC_ID 1
#define RLOG_LIMIT_L2_COUNT 100
#define RLOG_LIMIT_L3_COUNT 500

void rlInitL2SocSpecific(void);
void processL2LogBuff(void);
void rlGetL2LogBufPtr(void *mBuf,U32 *logLen,Data **logPtr);
void rlInvalidateL2LogsInCache(Data *ptr,U32   len);

extern void rlInitL2SocSpecific(void);
extern void processL2LogBuff(void);
extern void rlGetL2LogBufPtr(void *mBuf,U32 *logLen,Data **logPtr);

/**********************************************************************
         End of file
**********************************************************************/
