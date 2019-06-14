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
 
     Name:     System Services -- Message manipulation functions
 
     Type:     C source file
 
     Desc:     Macro definitions for message related functions.
 
     File:     ss_msg.h
 
*********************************************************************21*/

#ifndef __SSMSGH__
#define __SSMSGH__

#ifdef __cplusplus
extern "C" {
#endif


/* default SS region ID */
#define SS_DFLT_REGION                  0
#define SS_DFLT_POOL                    0

#ifdef SS_MULTICORE_SUPPORT
#define SS_REGION_0                     0
#define SS_REGION_1                     1
#define SS_REGION_2                     2
#define SS_REGION_3                     3
#define SS_REGION_4                     4
#define SS_REGION_5                     5
#endif /* SS_MULTICORE_SUPPORT */

/* pool types */
#define SS_POOL_UND                     0
#define SS_POOL_DYNAMIC                 1
#define SS_POOL_STATIC                  2

/* Bucket threshold default values */
#define SS_BLK_RELEASE_THRESHOLD        110
#define SS_BLK_ACQUIRE_THRESHOLD        20
#define SS_DFLT_MEM_BLK_SET_SIZE        128
#define SS_MEM_TYPE_SSI_ZBC 0x11
#ifdef XEON_SPECIFIC_CHANGES
#define SS_MEM_TYPE_DPDK_ZBC 0x12
#endif   

/* utility macros */
#define FIND_OFFSET(current, idx)   \
   { \
      MsgLen bufSiz; \
      while (idx) \
      { \
         bufSiz = (current->b_wptr - current->b_rptr); \
         if (bufSiz > idx) \
            break; \
         idx -= bufSiz; \
         current = current->b_cont; \
      } \
   }
 
#define FIND_OFFSET_AND_PREV(previous, current, idx)   \
   { \
      MsgLen bufSiz; \
      while (idx) \
      { \
         bufSiz = (current->b_wptr - current->b_rptr); \
         if (bufSiz > idx) \
            break; \
         idx -= bufSiz; \
         previous = current; \
         current = current->b_cont; \
      } \
   }

#ifdef __cplusplus
}
#endif

#endif /* __SSMSGH__ */

 

/********************************************************************30**
 
         End of file
**********************************************************************/
