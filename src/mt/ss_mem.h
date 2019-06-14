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
 
     Name:     System Services -- Memory management interface
 
     Type:     C include file
 
     Desc:     Various macro definitions required for the Memory
               management interface.
 
     File:     ss_mem.h
 
*********************************************************************21*/


#ifndef __SSMEMH__
#define __SSMEMH__

#ifdef __cplusplus
extern "C" {
#endif



/* region flags */
#define SS_OUTBOARD_FLAG        16

/* ss036.103 - addition of a macro. This implies that we
* can hold the memory statistics of upto SS_MAX_BKT_PER_DBGTBL buckets
*/
#define SS_MAX_BKT_PER_DBGTBL   16

/* ss036.103 - addition of macros for memory statistics related information */
#ifdef SSI_DEBUG_LEVEL1
#define SS_DYNAMIC_MEM_FLAG     0x04 /* same as CMM_DYNAMIC_MEM_FLAG */
#define SS_STATIC_MEM_FLAG      0x08 /* same as CMM_STATIC_MEM_FLAG */
#define SS_MEM_BLK_SIZE_PROFILE    0  /* to print size vs. numRequests */
#define SS_MEM_BKT_ALLOC_PROFILE   1  /* to print the static/dynamic mem used */
#endif /* SSI_DEBUG_LEVEL1 */

/* ss001.301: FAP related changes in SSI */
#ifdef SS_FAP
#define SAlloc(_region, _size,_flags, _ptr) \
           ((osCp.regionTbl[(_region)].alloc)((osCp.regionTbl[(_region)].regCb), (_size), (_flags),(_ptr)))
#define SFree(_region, _ptr, _size) \
           (osCp.regionTbl[(_region)].free)(\
           (osCp.regionTbl[(_region)].regCb), ( _ptr), (_size))

#endif

#ifdef __cplusplus
}
#endif

#endif  /*  __SSMEMH__  */


  
/********************************************************************30**

         End of file
**********************************************************************/
