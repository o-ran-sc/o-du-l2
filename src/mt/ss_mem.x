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
 
     Desc:     Data structure definitions required for the memory
               management interface.
 
     File:     ss_mem.x

*********************************************************************21*/


#ifndef __SSMEMX__
#define __SSMEMX__

#ifdef __cplusplus
extern "C" {
#endif


/* region entry structure */
typedef struct ssRegionEntry
{
   SsdRegionEntry dep;                          /* implementation specific */


   Bool         used;                           /* entry used? */

   Data         *start;                         /* start address */
   Size         size;                           /* size */

   Void         *regCb;                         /* control block */

   uint32_t          flags;                          /* flags */

   SsPoolEntry  poolTbl[SS_MAX_POOLS_PER_REG];  /* pool table */
   SsCntr       numPools;                       /* count of pools */

   SsAlloc      alloc;                          /* allocator function */
   SsFree       free;                           /* de-allocator function */
   SsCtl        ctl;                            /* ctl function */
	/* ss006.301 */
	SLockId      mBufRefLock;                    /* per region lock used for 
	                                               data block sharing */
} SsRegionEntry;

/* ss036.103 - addition of data type to return the memory statistics data
* this is used in SGetRegInfo 
*/
typedef struct ssMemBktDbgInfo
{
   Size         size;         /* Size of the block */
   uint32_t          numBlks;      /* Total number of blocks in the bucket */
   uint32_t          numAlloc;     /* Number of blocks allocated */
}SsMemBktDbgInfo;

typedef struct ssMemDbgInfo
{
   Region       region;        /* Region  Id of the memory */
   uint16_t          numBkts;       /* Number of buckets in bktDbgTbl */
   SsMemBktDbgInfo      bktDbgTbl[SS_MAX_BKT_PER_DBGTBL];
   Size         heapSize;      /* Size of the heap pool */
   Size         heapAlloc;          /* Total allocated memory */
   uint32_t          availmem;
#if (ERRCLASS & ERRCLS_DEBUG)
   uint16_t       numFragBlk;       /* Number of fragmented block */
#endif /* ERRCLASS & ERRCLS_DEBUG */
}SsMemDbgInfo;

/* ss036.103 - Addition of  prototypes for memory statistics */
S16 SRegInfoShow  ARGS((Region region, uint32_t *availmem));
S16 SGetRegInfo ARGS((Region region, SsMemDbgInfo *dbgInfo));
#ifdef XEON_SPECIFIC_CHANGES
S16 SRegReachedMemThreshold ARGS((Region region, uint8_t maxBkt));
#endif
#ifdef SSI_DEBUG_LEVEL1
S16 SPrintRegMemStatusInfo ARGS((Region region, uint8_t typeFlag));
Void SRegMemErrHdlr ARGS((Region region, Data *ptr, S16 errCode));
S16 SPrintRegMemProfile ARGS((Region region));
#endif /* SSI_DEBUG_LEVEL1 */

/* ss001.301: additions */
#ifdef SS_HISTOGRAM_SUPPORT 
S16 SGetTapaTskEntIds ARGS((Ent *ent));
S16 SGetHstGrmInfo ARGS((Ent *entId, Bool *hstReg));
#endif /* SS_HISTOGRAM_SUPPORT */

#ifdef __cplusplus
}
#endif

#endif  /*  __SSMEMX__  */


  
/********************************************************************30**

         End of file
**********************************************************************/
