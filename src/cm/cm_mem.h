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
 
     Name:     Common Memory Manager 
 
     Type:     C include file
 
     Desc:     Defines that are required by the Common Memory Manager.
 
     File:     cm_mem.h
 
*********************************************************************21*/

#ifndef __CMMEMH_
#define __CMMEMH_

/* cm_mem_h_001.103 - Addition */
#ifdef __cplusplus
extern "C" {
#endif


#ifdef XEON_MULTIPLE_CELL_CHANGES
#define MAX_WLS_DEVICE_NAME_LEN 16
#define DEFAULT_WLS_DEVICE_NAME "/dev/wls"
#endif

#define CMM_MINBUFSIZE   (PTRALIGN(sizeof(CmHEntry)))
#define CMM_DATALIGN(s, msz)  (((Size)(s) % msz) ? ((Size)(s) + ((msz - (Size)(s) % msz))): (Size)(s)) 

#define CMM_BUFSIZE(s, msz)   ((Size)(s) < (Size) msz ? \
                                      (Size) msz : \
                                      (Size) CMM_DATALIGN(s, msz))

/* defines */
/* cm_mem_h_001.103 - Addition for MEMCAL */
#ifndef CMM_MAX_BKT_ENT
#define  CMM_MAX_BKT_ENT    30  
#endif
#ifndef CMM_MAX_MAP_ENT
/* cm_mem_h_001.main_10: Modifications*/
#define  CMM_MAX_MAP_ENT    512
#endif

/* cm_mem_h_001.main_6 - addition for SSI enhancements */
#if (defined(SSI_DEBUG_LEVEL1) || defined(BRDCM_SSI_MEM_LEAK_DEBUG_LEVEL1) || defined (SS_LIGHT_MEM_LEAK_STS))
/* 
* Memory block header signature length
* Customer might choose to modify this to set his desired trampling signature length
* Customer should choose a value between 1 byte to 4 bytes 
*/
#define CMM_TRAMPLING_SIGNATURE_LEN   4

/* 
* Maximum number of trampling allowed
* Customer might choose to modify this, however too big a value is not suggestible
*/
#define CMM_TRAMPLING_THRESHOLD   3

/* 
* Hash list size to mantain the statistics of size requested & the number of attempts for allocation
* Customer might choose to modify this to accomodate statistics for more sizes
* preferably choose a prime number for better performance
*/
#define CMM_STAT_HASH_TBL_LEN   211 /* prime number */

/* values for memFlags in CmMmBlkHdr */
#define CMM_FREE_FLAG         0x01
#define CMM_DYNAMIC_MEM_FLAG  0x04
#define CMM_STATIC_MEM_FLAG   0x08

/* 
* MACROs for setting/resetting above flags
* FREE state related flags 
*/
#define CMM_SET_FREE_FLAG(x) ((x) |= CMM_FREE_FLAG)
#define CMM_RESET_FREE_FLAG(x) ((x) &= (~(CMM_FREE_FLAG)))
#define CMM_IS_FREE(x) ((x) & CMM_FREE_FLAG)

/* DYNAMIC state related flags */
#define CMM_SET_DYNAMIC_FLAG(x) ((x) |= CMM_DYNAMIC_MEM_FLAG)
#define CMM_RESET_DYNAMIC_FLAG(x) ((x) &= (~(CMM_DYNAMIC_MEM_FLAG)))
#define CMM_IS_DYNAMIC(x) ((x) & CMM_DYNAMIC_MEM_FLAG)

/* STATIC state related flags */
#define CMM_SET_STATIC_FLAG(x) ((x) |= CMM_STATIC_MEM_FLAG)
#define CMM_RESET_STATIC_FLAG(x) ((x) &= (~(CMM_STATIC_MEM_FLAG)))
#define CMM_IS_STATIC(x) ((x) & CMM_STATIC_MEM_FLAG)
#endif /* SSI_DEBUG_LEVEL1 */

/* Valid Physical Bit */
#define  CMM_REG_PHY_VALID  0x01 
#define  CMM_REG_OUTBOARD   0x02 
/* cm_mem_h_001.main_7 : Additions */ 
#if (defined(SS_MEM_LEAK_STS) || defined(BRDCM_SSI_MEM_LEAK_DEBUG_LEVEL1))
#ifndef CM_MEM_USR_MDL
#define CM_MEM_USR_MDL 10
#ifndef CM_MAX_STACK_TRACE
#define CM_MAX_STACK_TRACE 20
#endif /* if CM_MAX_STACK_TRACE is not defined in Makefile */
#endif
#define MEMRAW2STR(x,y)   {#x, #y}
#define CM_MAX_HASH_PER_TSK 4
#ifdef BRDCM_SSI_MEM_LEAK_DEBUG_LEVEL2
#define BRDCM_MEM_LEAK_BTRACE  6 /* Back trace */
#endif
#endif /* SS_MEM_LEAK_STS */ 

#ifdef SS_HISTOGRAM_SUPPORT
#define CMM_HIST_MAX_FILENAME 100
#define CMM_HIST_MAX_MEM_ENTRY_PER_BIN 25
#define CMM_HIST_MAX_MEM_BIN 25
#endif /* SS_HISTOGRAM_SUPPORT */

#ifdef SS_LOCKLESS_MEMORY
#define CMM_MAX_NUMBER_BKT_NODE   2
#endif

#ifdef SS_LIGHT_MEM_LEAK_STS
#define CM_MAX_ALLOC_ENTERIES 100000
#endif
/* cm_mem_h_001.103 */
#ifdef __cplusplus
}
#endif

#endif /* __CMMEMH_ */

/**********************************************************************
         End of file
 **********************************************************************/
 
