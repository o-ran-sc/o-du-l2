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
 
     Name:     System Services -- Queueing
 
     Type:     C include file
 
     Desc:     System Services queuing functions.
 
     File:     ss_queue.x
  
*********************************************************************21*/


#ifndef __SSQUEUEX__
#define __SSQUEUEX__

#ifdef __cplusplus
extern "C" {
#endif


/* demand queue structure */
typedef struct ssDmndQ
{
   SsSemaId   dmndQSema;                        /* lock for queue access */
   SLockId    dmndQLock[SS_MAX_NUM_DQ];    /* lock for each byte in 
                                                 * bitmask */
   Queue      queue[SS_MAX_NUM_DQ];             /* the queues */
#ifndef TENB_RTLIN_CHANGES
   uint8_t         bitMask[SS_DQ_BIT_MASK_LEN];      /* bit mask */
#endif
} SsDmndQ;



/* functions */
S16  ssInitDmndQ       ARGS((SsDmndQ *queue));
S16  ssDestroyDmndQ    ARGS((SsDmndQ *queue));
S16  ssDmndQPut        ARGS((SsDmndQ *queue, Buffer *mBuf, \
                                              Prior prior, Order order));
S16  ssDmndQGet        ARGS((SsDmndQ *queue, Buffer **mBuf, \
                                                           Order order));
S16  ssDmndQWait     ARGS((SsDmndQ *queue));

S16  ssFndLenDmndQ     ARGS((SsDmndQ *queue, Prior prior, QLen *len));


#ifdef __cplusplus
}
#endif

#endif /* __SSQUEUEX__ */


/********************************************************************30**
  
         End of file
**********************************************************************/
