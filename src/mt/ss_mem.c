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

     Type:     C source file

     Desc:     Implementation of the memory management interface.

     File:     ss_mem.c

*********************************************************************21*/


/* header include files (.h) */

#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */
  
#include "gen.h"           /* general layer */
#include "ssi.h"           /* system services */

#include "ss_err.h"        /* errors */
#include "ss_dep.h"        /* implementation-specific */
#include "ss_queue.h"      /* queues */
#include "ss_msg.h"        /* messaging */
#include "ss_mem.h"        /* memory management interface */
/*ss011.301 : RMI SSI specific includes*/
#ifdef SS_RMIOS
#include "uc_gen.h"        /* general */
#else
#include "ss_gen.h"        /* general */
#endif


#include "cm_mem.h"        /* common memory manager */
#include "cm_llist.h"
#include "cm_hash.h"

/* header/extern include files (.x) */

#include "gen.x"           /* general layer */
#include "ssi.x"           /* system services */

#include "ss_dep.x"        /* implementation-specific */
#include "ss_queue.x"      /* queues */
/*ss011.301 : RMI SSI specific includes*/
#ifdef SS_RMIOS
#include "uc_task.x"       /* tasking */
#include "uc_timer.x"      /* timers */
#else
#include "ss_task.x"       /* tasking */
#include "ss_timer.x"      /* timers */
#endif
#include "ss_strm.x"       /* STREAMS */
#include "ss_msg.x"        /* messaging */
#include "ss_mem.x"        /* memory management interface */
#include "ss_drvr.x"       /* driver tasks */
/*ss011.301 : RMI SSI specific includes*/
#ifdef SS_LOCKLESS_MEMORY
#include "cm_llist.x"
#include "cm_hash.x"
#include "cm_mem_wl.x"        /* common memory manager */
#else
#include "cm_mem.x"        /* common memory manager */
#endif /* SS_LOCKLESS_MEMORY */
#ifdef SS_RMIOS
#include "uc_gen.x"        /* general */
#else
#include "ss_gen.x"        /* general */
#endif
#include "cm_llist.x"
#include "cm_hash.x"



/*
*     Interface functions
*/
 

/*
*
*       Fun:   SRegDynRegion
*
*       Desc:  Registers a memory region.
*
*       Ret:   ROK      - ok
*              RFAILED  - failed, general (optional)
*
*       Notes:
*
*       File:  ss_mem.c
*
*/
S16 SRegDynRegion
(
Region region,                  /* region ID */
SRegInfo *regInfo               /* information about the region */
)
{
   S16 ret;

#if (ERRCLASS & ERRCLS_INT_PAR)
   /* validate region ID */
   if (region >= SS_MAX_REGS)
   {
      SSLOGERROR(ERRCLS_INT_PAR, ESS028, region, "Invalid region");
      return RFAILED;
   }

   /* validate region info pointer */
   if (regInfo == NULLP)
   {
      SSLOGERROR(ERRCLS_INT_PAR, ESS029, ERRZERO, "Null pointer");
      return RFAILED;
   }
#endif


   /* lock the region table */
   SS_ACQUIRE_ALL_SEMA(&osCp.regionTblSem, ret);
   if (ret != ROK)
   {

#if (ERRCLASS & ERRCLS_DEBUG)
      SSLOGERROR(ERRCLS_DEBUG, ESS030, (ErrVal) ret,
                  "Could not lock region table");
#endif

      return RFAILED;
   }


#if (ERRCLASS & ERRCLS_INT_PAR)
   /* verify that this region is not registered */
   if (osCp.dynRegionTbl[region].used == TRUE)
   {
      SS_RELEASE_ALL_SEMA(&osCp.regionTblSem);

      SSLOGERROR(ERRCLS_INT_PAR, ESS031, ERRZERO, "Region ID used");
      return RFAILED;
   }
#endif


   /* fill in the region information */
   osCp.dynRegionTbl[region].used = TRUE;
   osCp.dynRegionTbl[region].regCb = regInfo->regCb;
   osCp.dynRegionTbl[region].flags = regInfo->flags;
   osCp.dynRegionTbl[region].start = regInfo->start;
   osCp.dynRegionTbl[region].size = regInfo->size;
   osCp.dynRegionTbl[region].alloc = regInfo->alloc;
   osCp.dynRegionTbl[region].free = regInfo->free;
   osCp.dynRegionTbl[region].ctl = regInfo->ctl;

   osCp.numDynRegions++;


   /* unlock the region table */
   SS_RELEASE_ALL_SEMA(&osCp.regionTblSem);


   return ROK;
}




/*
*
*       Fun:   SRegRegion
*
*       Desc:  Registers a memory region.
*
*       Ret:   ROK      - ok
*              RFAILED  - failed, general (optional)
*
*       Notes:
*
*       File:  ss_mem.c
*
*/
S16 SRegRegion
(
Region region,                  /* region ID */
SRegInfo *regInfo               /* information about the region */
)
{
   S16 ret;

#if (ERRCLASS & ERRCLS_INT_PAR)
   /* validate region ID */
   if (region >= SS_MAX_REGS)
   {
      SSLOGERROR(ERRCLS_INT_PAR, ESS028, region, "Invalid region");
      return RFAILED;
   }

   /* validate region info pointer */
   if (regInfo == NULLP)
   {
      SSLOGERROR(ERRCLS_INT_PAR, ESS029, ERRZERO, "Null pointer");
      return RFAILED;
   }
#endif


   /* lock the region table */
   SS_ACQUIRE_ALL_SEMA(&osCp.regionTblSem, ret);
   if (ret != ROK)
   {

#if (ERRCLASS & ERRCLS_DEBUG)
      SSLOGERROR(ERRCLS_DEBUG, ESS030, (ErrVal) ret,
                  "Could not lock region table");
#endif

      return RFAILED;
   }


#if (ERRCLASS & ERRCLS_INT_PAR)
   /* verify that this region is not registered */
   if (osCp.regionTbl[region].used == TRUE)
   {
      SS_RELEASE_ALL_SEMA(&osCp.regionTblSem);

      SSLOGERROR(ERRCLS_INT_PAR, ESS031, ERRZERO, "Region ID used");
      return RFAILED;
   }
#endif


   /* fill in the region information */
   osCp.regionTbl[region].used = TRUE;
   osCp.regionTbl[region].regCb = regInfo->regCb;
   osCp.regionTbl[region].flags = regInfo->flags;
   osCp.regionTbl[region].start = regInfo->start;
   osCp.regionTbl[region].size = regInfo->size;
   osCp.regionTbl[region].alloc = regInfo->alloc;
   osCp.regionTbl[region].free = regInfo->free;
   osCp.regionTbl[region].ctl = regInfo->ctl;

   osCp.numRegions++;


   /* unlock the region table */
   SS_RELEASE_ALL_SEMA(&osCp.regionTblSem);


   return ROK;
}


/*
*
*       Fun:   SDeregRegion
*
*       Desc:  Deregisters a memory region.
*
*       Ret:   ROK      - ok
*              RFAILED  - failed, general (optional)
*
*       Notes:
*
*       File:  ss_mem.c
*
*/
S16 SDeregRegion
(
Region region                   /* region ID */
)
{
   S16 ret;

#if (ERRCLASS & ERRCLS_INT_PAR)
   /* validate region ID */
   if (region >= SS_MAX_REGS)
   {
      SSLOGERROR(ERRCLS_INT_PAR, ESS032, region, "Invalid region");
      return RFAILED;
   }
#endif


   /* lock the region table */
   SS_ACQUIRE_ALL_SEMA(&osCp.regionTblSem, ret);
   if (ret != ROK)
   {

#if (ERRCLASS & ERRCLS_DEBUG)
      SSLOGERROR(ERRCLS_DEBUG, ESS033, (ErrVal) ret,
                  "Could not lock region table");
#endif

      return RFAILED;
   }


#if (ERRCLASS & ERRCLS_INT_PAR)
   /* check if this region is registered */
   if (osCp.regionTbl[region].used == FALSE)
   {
      SS_RELEASE_ALL_SEMA(&osCp.regionTblSem);
      SSLOGERROR(ERRCLS_INT_PAR, ESS034, region, "Region not registered");
      return RFAILED;
   }
#endif


   /* empty the information about the region */
   osCp.regionTbl[region].used = FALSE;
   osCp.regionTbl[region].start = NULLP;
   osCp.regionTbl[region].size = 0;
   osCp.regionTbl[region].regCb = NULLP;
   osCp.regionTbl[region].flags = 0;
   osCp.regionTbl[region].alloc = NULLP;
   osCp.regionTbl[region].free = NULLP;
   osCp.regionTbl[region].ctl = NULLP;

   osCp.numRegions--;


   /* unlock the region table */
   SS_RELEASE_ALL_SEMA(&osCp.regionTblSem);


   return ROK;
}


/*
*
*       Fun:   SAlloc
*
*       Desc:  Allocates a block of memory of at least the specified
*              size.
*
*       Ret:   ROK      - ok
*              RFAILED  - failed, general (optional)
*
*       Notes:
*
*       File:  ss_mem.c
*
*/
/*ss003.301-gaurded under flag SS_FAP*/
#ifndef SS_FAP
/* ss001.301: Additions */
#ifdef SS_HISTOGRAM_SUPPORT 
S16 SAlloc
(
Region region,                  /* region ID */
Size *size,                     /* size of block required/allocated */
uint32_t flags,                      /* allocation flags */
Data **ptr,                      /* filled with pointer to block */
uint32_t   line,
uint8_t    *fileName,
uint8_t    entId
)
#else
#ifdef T2K_MEM_LEAK_DBG
S16 SAllocNew
(
Region region,                  /* region ID */
Size *size,                     /* size of block required/allocated */
uint32_t flags,                      /* allocation flags */
Data **ptr,                      /* filled with pointer to block */
char* file,
uint32_t line
)
#else
S16 SAlloc
(
Region region,                  /* region ID */
Size *size,                     /* size of block required/allocated */
uint32_t flags,                      /* allocation flags */
Data **ptr                      /* filled with pointer to block */
)
#endif
#endif
{
   S16 ret;
/* ss001.301: Additions */
#ifdef SS_HISTOGRAM_SUPPORT 
   Bool hstReg = FALSE;
#endif /* SS_HISTOGRAM_SUPPORT */




#if (ERRCLASS & ERRCLS_INT_PAR)
   /* validate region ID */
   if (region >= SS_MAX_REGS)
   {
      SSLOGERROR(ERRCLS_INT_PAR, ESS035, region, "Invalid region");
      return RFAILED;
   }
#endif
#ifndef XEON_SPECIFIC_CHANGES
   /* Get the region which is assgined with this thread. The region was 
    * stored in the osCp and is taken with respect to thread ID. This macro
    * is used only when SS_LOCKLESS_MEMORY flag is used and it overwrites 
    * the region passed.
    */
   region = SS_GET_THREAD_MEM_REGION();
#endif
/* ss037.103 Removed the semaphore acquisition for performance enhancement */
#ifndef SS_PERF
   /* acquire one semaphore, to protect against deregistration */
   SS_ACQUIRE_SEMA(&osCp.regionTblSem, ret);
   if (ret != ROK)
   {

#if (ERRCLASS & ERRCLS_DEBUG)
      SSLOGERROR(ERRCLS_DEBUG, ESS036, (ErrVal) ret,
                  "Could not lock region table");
#endif

      return RFAILED;
   }

#endif
#if (ERRCLASS & ERRCLS_INT_PAR)
   /* verify that this region is present */
   if (osCp.regionTbl[region].used == FALSE)
   {
/* ss037.103 Removed the semaphore operation for performance enhancement */
#ifndef SS_PERF
/* ss006.13: addition */
      if( SS_RELEASE_SEMA(&osCp.regionTblSem) != ROK)
      {
#if (ERRCLASS & ERRCLS_DEBUG)
      SSLOGERROR(ERRCLS_DEBUG, ESS037, ERRZERO, 
                  "Could not release the semaphore");
      return RFAILED;
#endif
      }
#endif
      SSLOGERROR(ERRCLS_INT_PAR, ESS038, region, "Region not registered");
      return RFAILED;
   }
#endif
/* ss001.301: Additions */
#ifdef SS_HISTOGRAM_SUPPORT 
   SGetHstGrmInfo(&entId, &hstReg);
   /* call the memory manager, to allocate this memory */
   /* ss036.103 - addition for passing memType as dynamic */
#ifdef SSI_DEBUG_LEVEL1
   ret = (osCp.regionTbl[region].alloc)
               (osCp.regionTbl[region].regCb, size, flags, ptr, 
                SS_DYNAMIC_MEM_FLAG, line, fileName, entId, hstReg);
#else
   ret = (osCp.regionTbl[region].alloc)
               (osCp.regionTbl[region].regCb, size, flags, ptr, line, 
                fileName, entId, hstReg);
#endif /* SSI_DEBUG_LEVEL1 */

#else

   /* call the memory manager, to allocate this memory */
/* ss036.103 - addition for passing memType as dynamic */
#ifdef SSI_DEBUG_LEVEL1
   ret = (osCp.regionTbl[region].alloc)
               (osCp.regionTbl[region].regCb, size, flags, ptr, SS_DYNAMIC_MEM_FLAG);
#else
#ifdef SS_LOCKLESS_MEMORY
#ifdef T2K_MEM_LEAK_DBG
   ret = (osCp.dynRegionTbl[region].alloc)
               (osCp.dynRegionTbl[region].regCb, size, flags, ptr,file,line);
#else
   ret = (osCp.dynRegionTbl[region].alloc)
               (osCp.dynRegionTbl[region].regCb, size, flags, ptr);
#endif
#else
#ifdef T2K_MEM_LEAK_DBG
   ret = (osCp.regionTbl[region].alloc)
               (osCp.regionTbl[region].regCb, size, flags, ptr,file,line);
#else
   ret = (osCp.regionTbl[region].alloc)
               (osCp.regionTbl[region].regCb, size, flags, ptr);
#endif
#endif /* SS_LOCKLESS_MEMORY */
#endif /* SSI_DEBUG_LEVEL1 */

#endif /* SS_HISTOGRAM_SUPPORT */

/* ss037.103 Removed the semaphore operation for performance enhancement */

#ifndef SS_PERF
   /* release the semaphore we took */
/* ss006.13: addition */
   if( SS_RELEASE_SEMA(&osCp.regionTblSem) != ROK)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
     SSLOGERROR(ERRCLS_DEBUG, ESS039, ERRZERO, 
                  "Could not release the semaphore");
     return RFAILED;
#endif
   }
#endif
/* ss036.103 - Addition to handle the memory trampling return value
* This in turn can call SRegMemErrHdlr 
*/
#ifdef SSI_DEBUG_LEVEL1
    if (ret == RTRAMPLINGNOK)
    {
        SRegMemErrHdlr( region,  *ptr,  ret);
    }
#endif /* SSI_DEBUG_LEVEL1 */

   return (ret);
}


/*
*
*       Fun:   SFree
*
*       Desc:  Frees a block of memory previously allocated by SAlloc().
*
*       Ret:   ROK      - ok
*              RFAILED  - failed, general (optional)
*
*       Notes:
*
*       File:  ss_mem.c
*
*/
/* ss001.301: Additions */
#ifdef SS_HISTOGRAM_SUPPORT 
S16 SFree
(
Region region,                  /* region ID */
Data *ptr,                      /* pointer to the allocated block */
Size size,                      /* size of block */
uint32_t  line,
uint8_t   *fileName,
uint8_t   entId
)
#else
#ifdef T2K_MEM_LEAK_DBG
S16 SFreeNew
(
Region region,                  /* region ID */
Data *ptr,                      /* pointer to the allocated block */
Size size,                       /* size of block */
char* file,
uint32_t line
)
#else
S16 SFree
(
Region region,                  /* region ID */
Data *ptr,                      /* pointer to the allocated block */
Size size                       /* size of block */
)
#endif
#endif
{
   S16 ret;

/* ss001.301: Additions */
#ifdef SS_HISTOGRAM_SUPPORT 
   Bool hstReg = FALSE;
#endif /* SS_HISTOGRAM_SUPPORT */



   /* Get the region which is assgined with this thread. The region was 
    * stored in the osCp and is taken with respect to thread ID. This macro
    * is used only when SS_LOCKLESS_MEMORY flag is used and it overwrites 
    * the region passed.
    */
#ifdef XEON_SPECIFIC_CHANGES
   region = 0;
#else   
   region = SS_GET_THREAD_MEM_REGION();
#endif   

#if (ERRCLASS & ERRCLS_INT_PAR)
   /* validate region ID */
   if (region >= SS_MAX_REGS)
   {
      SSLOGERROR(ERRCLS_INT_PAR, ESS040, region, "Invalid region");
      return RFAILED;
   }

/* ss021.103 - Addition to validate size and ptr */
   if (size <= NULLD)
   {
      SSLOGERROR(ERRCLS_INT_PAR, ESS041, region, "Invalid size");
      return RFAILED;
   }

   /* validate ptr */
   if (ptr == (Data *)NULLP)
   {
      SSLOGERROR(ERRCLS_INT_PAR, ESS042, region, "Invalid ptr");
      return RFAILED;
   }

#endif
/* ss037.103 Removed the semaphore operation for performance enhancement */

#ifndef SS_PERF
   /* acquire one semaphore, to protect against deregistration */
   SS_ACQUIRE_SEMA(&osCp.regionTblSem, ret);
   if (ret != ROK)
   {

#if (ERRCLASS & ERRCLS_DEBUG)
      SSLOGERROR(ERRCLS_DEBUG, ESS043, (ErrVal) ret,
                  "Could not lock region table");
#endif

      return RFAILED;
   }
#endif
#if (ERRCLASS & ERRCLS_INT_PAR)
   /* verify that this region is around */
   if (osCp.regionTbl[region].used == FALSE)
   {
#ifndef SS_PERF
/* ss006.13: addition */
      if( SS_RELEASE_SEMA(&osCp.regionTblSem) != ROK)
      {
#if (ERRCLASS & ERRCLS_DEBUG)
         SSLOGERROR(ERRCLS_DEBUG, ESS044, ERRZERO, 
                  "Could not release the semaphore");
         return RFAILED;
#endif
       }
#endif
      SSLOGERROR(ERRCLS_INT_PAR, ESS045, region, "Region not registered");
      return RFAILED;
   }
#endif

/* ss001.301: Additions */
#ifdef SS_HISTOGRAM_SUPPORT 
   SGetHstGrmInfo(&entId, &hstReg);

   /* call the memory manager to free this memory */
   ret = (osCp.regionTbl[region].free)
           (osCp.regionTbl[region].regCb, ptr, size, line, fileName, 
            entId, hstReg);

#else

   /* call the memory manager to free this memory */
#ifdef SS_LOCKLESS_MEMORY
#ifdef T2K_MEM_LEAK_DBG
   ret = (osCp.dynRegionTbl[region].free)(osCp.dynRegionTbl[region].regCb, ptr, size,file,line);
#else
   ret = (osCp.dynRegionTbl[region].free)(osCp.dynRegionTbl[region].regCb, ptr, size);
#endif
#else
#ifdef T2K_MEM_LEAK_DBG
   ret = (osCp.regionTbl[region].free)(osCp.regionTbl[region].regCb, ptr, size,file,line);
#else
   ret = (osCp.regionTbl[region].free)(osCp.regionTbl[region].regCb, ptr, size);
#endif
#endif /* SS_LOCKLESS_MEMORY */

#endif /* SS_HISTOGRAM_SUPPORT */

/* ss037.103 Removed the semaphore operation for performance enhancement */

#ifndef SS_PERF
   /* release the semaphore we took */
/* ss006.13: addition */
   if( SS_RELEASE_SEMA(&osCp.regionTblSem) != ROK)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      SSLOGERROR(ERRCLS_DEBUG, ESS046, ERRZERO, 
                  "Could not release the semaphore");
      return RFAILED;
#endif
    }
#endif
/* ss036.103 - changes to handle double free and trmapling return values
* This in turn may invoke SRegMemErrHdlr 
*/
#ifdef SSI_DEBUG_LEVEL1
    /* handle the double free error here by calling the OS specific error handling function */
    if ((ret == RDBLFREE) || (ret == RTRAMPLINGNOK))
    {
        SRegMemErrHdlr( region,  ptr,  ret);
    }
#endif /* SSI_DEBUG_LEVEL1 */

   return (ret);
}
#endif /* SS_FAP */

#ifndef SS_FAP
/* ss001.301: Additions */
#ifdef SS_HISTOGRAM_SUPPORT 

/*
*
*       Fun:  SHstGrmInfoShow 
*
*       Desc:  This function displays the memory usage information
*              per Tapa task, which are registerd during initialization.
*
*
*       Ret:   
*              
*
*       Notes: A Sample Output from the function 
*       Memory Histogram for the Tapa task = (Entity Id)117
*       ------Region Id = 0-------
*       -----------Bucket Id = 0-----------
*
*      File Name|   Line  | Bucket Alloc | Bytes Allocated | Bytes Wasted | Buckets Freed|Bytes Freed|
*      lhi.c    |    413  |            4 |             512 |          224 |            0 |         0 |
*      hi_acc1.c|    333  |            0 |               0 |            0 |            2 |       256 |
*      hi_acc1.c|    209  |           13 |            1664 |          728 |            0 |         0 |
*
*       File:  mt_ss.c
*
*/
S16 SHstGrmInfoShow 
(
Ent      *entId
)
{
    /*ss012.301 : Increased Buffer size to fix segmentation fault*/
    Txt   					prntBuf[511];  /* Buffer to print on the console */
	uint16_t   					ret = ROK;     /* return value */
	Ent   					tapaTsk[SS_MAX_TTSKS]; /* List of tapa task */
	uint32_t   					tskCnt = 0;   /* Tapa task Count */
	uint32_t   					regCnt = 0;   /* Region count */
	uint32_t   					bktCnt = 0;   /* Bucket count in each region */
	CmHstGrmHashListCp 	*hashListCp = NULLP; /* Hash List ponter of bucket */
	uint32_t                  binCnt = 0;   
	uint32_t                  entCnt = 0;
	CmMemEntries        *entry = NULLP;
	uint32_t                  blkSize = 0;
   

   memset(tapaTsk, ENTNC, sizeof(tapaTsk));

   if(entId == NULLP)
	{
	    /* If user is not asking for specific entity id then,
		  * Take Tapa task entity id from the osCp structure and 
		  * print the histogram for all tapa task */
   	ret = SGetTapaTskEntIds(tapaTsk);
	}/* End of if */
	else
	{
	   /* If user asked for specific tapa task then print histogram 
		 * for those tapa task */
		for(tskCnt = 0; entId[tskCnt]; tskCnt++) 
		{ 
			tapaTsk[tskCnt] = entId[tskCnt];
		}/* end of for */
	}
   for (tskCnt = 0; tapaTsk[tskCnt] != ENTNC; tskCnt++)
	{
   	sprintf(prntBuf, "\n\nMemory Histogram for the Tapa task = (Entity Id)%d\n", tapaTsk[tskCnt]);
   	SPrint(prntBuf);
	 	for (regCnt = 0; regCnt < osCp.numRegions; regCnt++)
		{
		   
			CmMmRegCb* regCb = (CmMmRegCb*)osCp.regionTbl[regCnt].regCb;

   		sprintf(prntBuf, "------Region Id = %d-------\n", regCb->region);
   		SPrint(prntBuf);
         for (bktCnt = 0; bktCnt < regCb->numBkts; bktCnt++)
			{
						 /* ss002.301 - Compilation fixes */
            /*ss013.301 Fix for compilation warnings  32/64 bit */                
#ifdef ALIGN_64BIT
   			sprintf(prntBuf, "-----------Bucket Id = %u-----------\n\n", bktCnt);
#else
   			sprintf(prntBuf, "-----------Bucket Id = %lu-----------\n\n", bktCnt);
#endif
   			SPrint(prntBuf);
   			sprintf(prntBuf, "File Name         |   Line  | Bucket Alloc | Bytes Allocated | Bytes Wasted | Buckets Freed|Bytes Freed|\n");
   			SPrint(prntBuf);
			   hashListCp = &(regCb->bktTbl[bktCnt].hstGrmHashListCp);
			   blkSize = regCb->bktTbl[bktCnt].size;
			   for(binCnt = 0; binCnt < CMM_HIST_MAX_MEM_BIN; binCnt++)
				{
					for (entCnt = 0; entCnt < hashListCp->hashList[binCnt].numOfEntries; entCnt++)
					{
					    entry = &(hashListCp->hashList[binCnt].entries[entCnt]);   
						 if(entry->entId == tapaTsk[tskCnt])
						 {
						 /* ss002.301 - Compilation fixes */
                         /*ss013.301 Fix for compilation warnings  32/64 bit */                
#ifdef ALIGN_64BIT
						 	sprintf(prntBuf, 
							   "%-18s|%7u  |%13u |%16u |%13u |%13u |%10u |\n", 
                        entry->fileName, entry->line, entry->bucketAllocReq, 
								(blkSize*entry->bucketAllocReq), entry->wastedBytes, 
								entry->bucketFreeReq, (blkSize*entry->bucketFreeReq));
#else
						 	sprintf(prntBuf, 
							   "%-18s|%7lu  |%13lu |%16lu |%13lu |%13lu |%10lu |\n", 
                        entry->fileName, entry->line, entry->bucketAllocReq, 
								(blkSize*entry->bucketAllocReq), entry->wastedBytes, 
								entry->bucketFreeReq, (blkSize*entry->bucketFreeReq));
#endif
                     SPrint(prntBuf);
						 }/* End of If */
					}/* end of for. Entry count */
				} /* End of for. Hash bin count */ 
			 }

   			sprintf(prntBuf, "\n\n----------- Heap blocks -----------\n\n");
   			SPrint(prntBuf);
   			sprintf(prntBuf, "File Name         |   Line  | Heap Allocs  | Bytes Allocated | Bytes Wasted |  Heap Frees  |Bytes Freed|\n");
   			SPrint(prntBuf);
			   hashListCp = &(regCb->heapCb.heapHstGrmHashListCp);
			   for(binCnt = 0; binCnt < CMM_HIST_MAX_MEM_BIN; binCnt++)
				{
					for (entCnt = 0; entCnt < hashListCp->hashList[binCnt].numOfEntries; entCnt++)
					{
					    entry = &(hashListCp->hashList[binCnt].entries[entCnt]);   
						 if(entry->entId == tapaTsk[tskCnt])
						 {
						 /* ss002.301 - Compilation fixes */
                         /*ss013.301 Fix for compilation warnings  32/64 bit */                
#ifdef ALIGN_64BIT
						 	sprintf(prntBuf, 
							    "%-18s|%7u  |%13u |%16u |%13u |%13u |%10u |\n", 
                     entry->fileName, entry->line, entry->bucketAllocReq, 
							entry->allocBytes, entry->wastedBytes, 
							entry->bucketFreeReq, entry->freedBytes);
#else
						 	sprintf(prntBuf, 
							    "%-18s|%7lu  |%13lu |%16lu |%13lu |%13lu |%10lu |\n", 
                     entry->fileName, entry->line, entry->bucketAllocReq, 
							entry->allocBytes, entry->wastedBytes, 
							entry->bucketFreeReq, entry->freedBytes);
#endif
                     SPrint(prntBuf);
						 }/* End of If */
					}/* end of for. Entry count */
				} /* End of for. Hash bin count */ 
		 }/* End of for. Region Count */
	} /* End of for (Tapa task)*/


   /* Print the memory information used at common, sample and ssi files. 
	 we considerd common Sample file as invalid entity id. */
  	sprintf(prntBuf, "\n\nMemory Histogram for the Tapa task = (Entity Id = ENTNC)%d\n", ENTNC);
  	SPrint(prntBuf);
 	for (regCnt = 0; regCnt < osCp.numRegions; regCnt++)
	{
		CmMmRegCb* regCb = (CmMmRegCb*)osCp.regionTbl[regCnt].regCb;

  		sprintf(prntBuf, "------Region Id = %d-------\n", regCb->region);
  		SPrint(prntBuf);
      for (bktCnt = 0; bktCnt < regCb->numBkts; bktCnt++)
		{
						 /* ss002.301 - Compilation fixes */
            /*ss013.301 Fix for compilation warnings  32/64 bit */                
#ifdef ALIGN_64BIT
  			sprintf(prntBuf, "-----------Bucket Id = %u-----------\n\n", bktCnt);
#else
  			sprintf(prntBuf, "-----------Bucket Id = %lu-----------\n\n", bktCnt);
#endif
  			SPrint(prntBuf);
  			sprintf(prntBuf, "File Name         |   Line  | Bucket Alloc | Bytes Allocated | Bytes Wasted | Buckets Freed|Bytes Freed|\n");
  			SPrint(prntBuf);
		   hashListCp = &(regCb->bktTbl[bktCnt].hstGrmHashListCp);
		   blkSize = regCb->bktTbl[bktCnt].size;
		   for(binCnt = 0; binCnt < CMM_HIST_MAX_MEM_BIN; binCnt++)
			{
				for (entCnt = 0; entCnt < hashListCp->hashList[binCnt].numOfEntries; entCnt++)
				{
				    entry = &(hashListCp->hashList[binCnt].entries[entCnt]);   
					 if(entry->entId == tapaTsk[tskCnt])
					 {
						 /* ss002.301 - Compilation fixes */
            /*ss013.301 Fix for compilation warnings  32/64 bit */                
#ifdef ALIGN_64BIT
					 	sprintf(prntBuf, "%-18s|%7u  |%13u |%16u |%13u |%13u |%10u |\n", 
                  entry->fileName, entry->line, entry->bucketAllocReq, (blkSize*entry->bucketAllocReq),
						entry->wastedBytes, entry->bucketFreeReq, (blkSize*entry->bucketFreeReq));
#else
					 	sprintf(prntBuf, "%-18s|%7lu  |%13lu |%16lu |%13lu |%13lu |%10lu |\n", 
                  entry->fileName, entry->line, entry->bucketAllocReq, (blkSize*entry->bucketAllocReq),
						entry->wastedBytes, entry->bucketFreeReq, (blkSize*entry->bucketFreeReq));
#endif
                  SPrint(prntBuf);
					 }/* End of If */
				}/* end of for. Entry count */
			} /* End of for. Hash bin count */ 

		 }/* End of for. Bucket Count */

   		sprintf(prntBuf, "\n\n----------- Heap blocks -----------\n\n");
   		SPrint(prntBuf);
   		sprintf(prntBuf, "File Name         |   Line  | Heap Allocs  | Bytes Allocated | Bytes Wasted |  Heap Frees  |Bytes Freed|\n");
   		SPrint(prntBuf);
			hashListCp = &(regCb->heapCb.heapHstGrmHashListCp);
			for(binCnt = 0; binCnt < CMM_HIST_MAX_MEM_BIN; binCnt++)
			{
					for (entCnt = 0; entCnt < hashListCp->hashList[binCnt].numOfEntries; entCnt++)
					{
					    entry = &(hashListCp->hashList[binCnt].entries[entCnt]);   
						 if(entry->entId == tapaTsk[tskCnt])
						 {
						 /* ss002.301 - Compilation fixes */
                         /*ss013.301 Fix for compilation warnings  32/64 bit */                
#ifdef ALIGN_64BIT
						 	sprintf(prntBuf, 
							    "%-18s|%7u  |%13u |%16u |%13u |%13u |%10u |\n", 
                     entry->fileName, entry->line, entry->bucketAllocReq, 
							entry->allocBytes, entry->wastedBytes, 
							entry->bucketFreeReq, entry->freedBytes);
#else
						 	sprintf(prntBuf, 
							    "%-18s|%7lu  |%13lu |%16lu |%13lu |%13lu |%10lu |\n", 
                     entry->fileName, entry->line, entry->bucketAllocReq, 
							entry->allocBytes, entry->wastedBytes, 
							entry->bucketFreeReq, entry->freedBytes);
#endif
                     SPrint(prntBuf);
						 }/* End of If */
					}/* end of for. Entry count */
			} /* End of for. Hash bin count */ 

	 }/* End of for. Region Count */

   return ROK;
}   

#endif /* SS_HISTOGRAM_SUPPORT */
#endif /* SS_FAP */

/**********************************************************************
         End of file
 **********************************************************************/
