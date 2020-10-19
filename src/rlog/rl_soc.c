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
#include "envopt.h"
#include "envdep.h"

#include "rl_interface.h"
#include "rl_rlog.h"
#include "rl_platform.h"

#include "gen.h"
#include "ssi.h"
#include "ss_msg.h"
#include "ss_task.h"
#include "gen.x"
#include "ssi.x"
#include "ss_queue.h"
#include "ss_queue.x"
#include "ss_task.x"
#include "ss_msg.x"
#include "cm_inet.h"
#include "cm_inet.x"

#include "rl_soc.h"

//////////////////////////////////////////////////////////////////////////
//  @Function    : rlInitL2Log 
//  @Description : This will be trigigered from cl init function to
//                 allocate buffer from shared memory and to intialize
//                 the buffer
//  @in          : void
//  @out         : void
//////////////////////////////////////////////////////////////////////////

void rlInitL2SocSpecific(void)
{
#ifndef RLOG_ENABLE_TEXT_LOGGING

   if (SGetStaticBuffer(0,  /* region - value does not matter in this case  */
                        0,  /* pool   - value does not matter in this case  */  
                        &g_l2rlogBuf, 
                        L2LOG_BUFF_BLOCK_SIZE, /* Buffer size to be allocated */
                        0) != ROK)      /* Memory type is SS_SHARABLE_MEMORY  */
   {
      printf("Memory allocation failed for g_l2rlogBuf!!!\n");
      /* kill binary */
   }
   memset(g_l2rlogBuf, 0, L2LOG_BUFF_SIZE); 

   g_l2LogBufBasePtr = g_l2rlogBuf;
   g_l2LogBufStartPtr     = g_l2rlogBuf + sizeof(g_l2LogBufLen);

#endif
   return;
}

//////////////////////////////////////////////////////////////////////////
//  @Function    : processL2LogBuff 
//  @Description : This function will get triggered after every 10 tti 
//                 and this will reset log buffers for L2 and also  sends log
//                 buffer to L3 which was collected
//  @in          : void
//  @out         : void
//////////////////////////////////////////////////////////////////////////

void processL2LogBuff(void)
{
#ifndef RLOG_ENABLE_TEXT_LOGGING
   if(g_l2LogBufLen > 0)
   {
      Data *l2rlogBuf_post;

      /* Log buffer position in flat buffer. If position is last wrap to first buffer */
      if (g_l2logBuffPos++ >=4)
      {
         g_l2logBuffPos = 0;
      }

      g_writeCirBuf = 1; /* This will avoid contention while switching log buffers */

      l2rlogBuf_post = g_l2LogBufBasePtr;   /* copy logBufferPointer for later use */
      /* Set L2 Log Buffer length in first 4 bytes of flat buffer */
      *((uint32_t*) g_l2LogBufBasePtr) = g_l2LogBufLen; /* Set L2 Log Buffer length in 
                                               first 4 bytes of flat buffer */

      /* Re-setting pointer so that L2 will use this to write logs */
      g_l2LogBufBasePtr = g_l2rlogBuf + (g_l2logBuffPos * L2LOG_BUFF_BLOCK_SIZE) ; 
      g_l2LogBufStartPtr     = g_l2LogBufBasePtr + sizeof(g_l2LogBufLen); 

      g_l2LogBufLen = 0;

      g_writeCirBuf = 0; /* This will avoid contention while switching log buffers */

   }
#endif   
   return; 
}

//////////////////////////////////////////////////////////////////////////
//  @Function    : rlGetL2LogBufPtr 
//  @Description : This function reads LogBufPointer and Length of log Buffer 
//                 and stores pointer and length variable so that these 
//                 values can be used while reading L2 Log Buffer 
//  @in          : mBuf     - Log Buffer
//  @out         : logLen   - Length of log Buffer
//                 logPtr   - Address of log Buffer
//////////////////////////////////////////////////////////////////////////

void rlGetL2LogBufPtr
(
   void *mBuf,
   uint32_t *logLen,
   Data **logPtr
)
{
   /* Get Buffer pointer and length */
   *logPtr = ((Data *)mBuf) + sizeof(logLen);
   *logLen = *((uint32_t *) mBuf);
}

//////////////////////////////////////////////////////////////////////////
//  @Function    : rlInvalidateL2LogsInCache 
//  @Description : Dummy function which is not used for this SOC. 
//                 
//  @in          : ptr     - starting address to be invalidated.
//                 len     - Length to be invalidated.
//  @out         : void
//////////////////////////////////////////////////////////////////////////

void rlInvalidateL2LogsInCache
(
   Data *ptr,
   uint32_t   len
)
{
   return;
}

/**********************************************************************
         End of file
***********************************************************************/
