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
 
     Desc:     Data structure definitions that are used by 
               the Common Memory Manager.
 
     File:     cm_mem.x
 
     Sid:      cm_mem.x@@/main/13 - Fri Feb 26 13:16:42 2010
 
     Prg:      rm
 
*********************************************************************21*/

#ifndef __CMMEMX_
#define __CMMEMX_

#ifdef __cplusplus
extern "C" {
#endif
/*cm_mem_x_001.main_12: As these required for the following only */
#if (defined(SS_HISTOGRAM_SUPPORT) || defined(SS_MEM_LEAK_STS))
#include "cm_hash.h" /* cm_mem_x_001.main_11: Additions */
#include "cm_hash.x"
#endif /* SS_HISTOGRAM_SUPPORT */



/* typedefs */
typedef struct cmMmRegCfg    CmMmRegCfg;
typedef struct cmMmBktCfg    CmMmBktCfg;

 
typedef struct   cmMmRegCb     CmMmRegCb;
typedef struct   cmMmMapBkt    CmMmMapBkt;
typedef struct   cmMmHeapCb    CmMmHeapCb;
typedef struct   cmMmBkt       CmMmBkt;
typedef struct   cmHEntry      CmHEntry;
/* cm_mem_x_001.main_9 - addition of new data types for providing 
* header for memory blocks in buckets and a hash list to maintain
* size vs. numAttempts statistics 
*/
#if (defined(SSI_DEBUG_LEVEL1) || defined(BRDCM_SSI_MEM_LEAK_DEBUG_LEVEL1) || defined (SS_LIGHT_MEM_LEAK_STS))
typedef struct cmMmBlkHdr      CmMmBlkHdr;
typedef struct cmMmHashListEnt   CmMmHashListEnt;
typedef struct cmMmHashListCp   CmMmHashListCp;
#endif /* SSI_DEBUG_LEVEL1 */
typedef Data     CmMmEntry;
 /* cm_mem_x_001.main_11: Additions */
#ifdef SS_HISTOGRAM_SUPPORT 
typedef struct cmHstGrmHashListCp   CmHstGrmHashListCp;
typedef struct cmHstGrmHashListEnt  CmHstGrmHashListEnt;
typedef struct cmMemEntries         CmMemEntries;
#endif /* SS_HISTOGRAM_SUPPORT */
 
/* forward definitions */


/************************************************************************
      Common Memory Manager Configuration Data Structures
************************************************************************/

/* Bucket configuration structure. */
struct cmMmBktCfg
{
   Size  size;              /* Size of the memory block */
   uint32_t   numBlks;           /* Number of the block in the bucket */
};

/* Memory Region configuration structure. */ 
struct cmMmRegCfg
{
   Size      size;          /* Size of the memory */
   Data     *vAddr;         /* Start address of the memory */
   uint8_t        lType;         /* Lock Type to be used */
 
   uint8_t        chFlag;        /* Flag defines the memory region characteristics */
   Data     *pAddr;         /* Physical address of the memory block: Valid 
                               if CMM_REG_PHY_VALID bit of chFlag is set */
   Size      bktQnSize;     /* Quatum size of the memory block */
   uint16_t       numBkts;       /* Number of buckets in the Bucket Pool */
 
   CmMmBktCfg  bktCfg[CMM_MAX_BKT_ENT];  /* Bucket configuration structure */
};
 
/************************************************************************
      Common Memory Manager Implementation Data Structures
************************************************************************/
/* cm_mem_x_001.main_11: Additions */
#ifdef SS_HISTOGRAM_SUPPORT 
struct cmMemEntries /* Entry information */
{
   uint32_t              line; /* Line Number where memory allocation primitive used */
   uint32_t              allocBytes; /* Granted Byte */
   uint32_t              allocOverFlow; /* If the value of allocBytes overflows then we use this variable */
   uint32_t              freedBytes; /* Freed Bytes */
   uint32_t              freeOverFlow; /* If the value of freeByte overflows then we use this value */
   uint32_t              wastedBytes; /* Difference between Granted and Requested bytes */
   uint32_t              bucketAllocReq; /* No of times this Bucket used */
   uint32_t              bucketFreeReq; /* No of times this Bucket freed */
   uint32_t              entId; /* Tapa task used this bucket */
   uint32_t              key; /* combination of file name and line */
   uint8_t               fileName[CMM_HIST_MAX_FILENAME]; /* File Name where memory allocation primitive used */
};

struct cmHstGrmHashListEnt    /* hash list entry */
{
   CmMemEntries      entries[CMM_HIST_MAX_MEM_ENTRY_PER_BIN]; /* Number of entires in each bin */
   uint32_t               numOfEntries; /*Number of entries in hash list */
};

struct cmHstGrmHashListCp /* Hash List control pointer */
{
   CmHstGrmHashListEnt   hashList[CMM_HIST_MAX_MEM_BIN]; /* Number of bins in hash list */
   uint16_t                   totalNumEntries; /* Total number of bins */
};

#endif /* SS_HISTOGRAM_SUPPORT */

/* cm_mem_x_001.main_9 - addition of memory header for blocks in buckets */
#if (defined(SSI_DEBUG_LEVEL1) || defined(BRDCM_SSI_MEM_LEAK_DEBUG_LEVEL1) || defined (SS_LIGHT_MEM_LEAK_STS))
/* Memory Block Header */
struct cmMmBlkHdr
{
   /* trSignature shall always be maintained as the first element in this structure */
   uint8_t          trSignature[CMM_TRAMPLING_SIGNATURE_LEN];  /* signature to detect trampling */
   CmMmBlkHdr  *nextBlk;         /* pointer to the next memory block header */
   uint32_t         memFlags;         /* Free/Static/Dynamic */
   Size        requestedSize;    /* requested size for which this mem blk has been allocated */
#ifdef SS_LIGHT_MEM_LEAK_STS
   uint32_t         timeStamp;        /* Time stamp */
   uint32_t         lineNo;
   uint32_t         allocQueueIndx;
   uint8_t          *currFuncName;
#endif
#ifdef BRDCM_SSI_MEM_LEAK_DEBUG_LEVEL2
   CmHashListEnt         ent;    /* hash list entry  */
   Size                  reqSz;  /* Requested size */
   Size                  allocSz;/* Allocated size */
   uint8_t                    bktIdx; /* Bucket ID */
   Void                  *backTrace[BRDCM_MEM_LEAK_BTRACE]; /* Back trace */
#endif
};
#ifdef BRDCM_SSI_MEM_LEAK_DEBUG_LEVEL1
typedef struct cmMmBlkTail
{
   uint8_t     trSignature[CMM_TRAMPLING_SIGNATURE_LEN];  /* signature to detect trampling */
}CmMmBlkTail;
typedef struct memLkInfo
{
   Bool        memLkMdlInit;
   FILE        *fileLkLog;
}MemLkInfo;
#endif
#endif /* SSI_DEBUG_LEVEL1 */

/* Memory bucket structure */
struct cmMmBkt                /* Bucket Structure */
{
/* cm_mem_x_001.main_9 - addition of next block pointer */
#if (defined(SSI_DEBUG_LEVEL1) || defined(BRDCM_SSI_MEM_LEAK_DEBUG_LEVEL1) || defined (SS_LIGHT_MEM_LEAK_STS))
   CmMmBlkHdr   *nextBlk;   /* pointer to the next memory block header */
#if (defined(BRDCM_SSI_MEM_LEAK_DEBUG_LEVEL1) || defined (SS_LIGHT_MEM_LEAK_STS))
   CmMmBlkHdr   *lastBlk;      /* Pointer to the last memory block header */
#endif
#else
   CmMmEntry   *next;         /* Pointer to the next memory block */
#ifdef SSI_MEM_CORR_PREVENTION
   CmMmEntry   *last;         /* Pointer to the last memory block */
#endif
#ifdef MS_MBUF_CORRUPTION /*PST Should not take code Under this flag*/
   CmMmEntry   *last;         /* Pointer to the last memory block */
#endif
#endif /* SSI_DEBUG_LEVEL1 */
   Size         size;         /* Size of the block */
   uint32_t          numBlks;      /* Total number of blocks in the bucket */
   uint32_t          maxAlloc;     /* Maximum number of blocks allocated till now */
   uint32_t          numAlloc;     /* Number of blocks allocated */
   SLockId      bktLock;      /* Lock to protect the bucket pool */ 
   uint32_t          bktFailCnt;   /* bucket alloc failure count */
   uint32_t          bktNoFitCnt;  /* number of blocks not fit */
/* cm_mem_x_001.main_9 - addition of statistics related data */
#if (defined(SSI_DEBUG_LEVEL1) || defined(BRDCM_SSI_MEM_LEAK_DEBUG_LEVEL1))
   Data         *bktStartPtr;   /* maintains the start address of the memory block in the bucket */
   StsCntr      numAllocAttempts;   /* total number of allocation attempts */
   StsCntr      numDeallocAttempts;   /* total number of de-allocation attempts */
   Size         staticMemUsed;   /* amount of memory used for static allocations */
   Size         dynamicMemUsed;   /* amount of memory used for dynamic allocations */
   uint8_t           trampleCount;   /* incremented everytime a trampling is detected in any block of the bucket */
#endif /*SSI_DEBUG_LEVEL1*/
/* cm_mem_x_001.main_11: Additions */
#ifdef SS_HISTOGRAM_SUPPORT 
  CmHstGrmHashListCp hstGrmHashListCp;
#endif /* SS_HISTOGRAM_SUPPORT */
};


/* Size-To-Bucket map table structure */ 
struct cmMmMapBkt
{
   uint16_t   bktIdx;              /* The index to the memory bucket */

#if (ERRCLASS & ERRCLS_DEBUG)
   uint16_t   numReq;              /* Number of the allocation request */
   uint16_t   numFailure;          /* Number of allocation failure form the bucket */
#endif

};

/* Heap entry structure linked in the heap control block */ 
struct cmHEntry
{
/* cm_mem_x_001.main_9 - addition to provide trampling and double-free detections support */
#ifdef SSI_DEBUG_LEVEL1
  /* trSignature shall always be maintianed as the first element in this structure */
  uint8_t         trSignature[CMM_TRAMPLING_SIGNATURE_LEN];  /* signature to detect trampling */
  uint32_t        memFlags;          /* flag to maintain the memory status */
  Size       requestedSize;     /* size of the heap entry block */
#endif /* SSI_DEBUG_LEVEL1 */
  CmHEntry  *next;             /* Pointer to the next entry block */
  Size       size;             /* size of the heap entry block */
};

/* Heap control block */
struct cmMmHeapCb
{
   Data     *vStart;           /* Memory start address */
   Data     *vEnd;             /* Memory end address */
   CmHEntry *next;             /* Next heap block entry */
   Size      avlSize;          /* Total available memory */
   Size      minSize;          /* Minimum size that can be allocated */
   SLockId   heapLock;         /* Lock to protect the heap pool */
#if (ERRCLASS & ERRCLS_DEBUG)
   uint16_t       numFragBlk;       /* Number of fragmented block */
   uint16_t       numReq;           /* Number of allocation request */
   uint16_t       numFailure;       /* Number of allocation failure */
#endif
   uint32_t       heapAllocCnt;     /* Number of blocks from heap */
/* cm_mem_x_001.main_9 - addition of statistics related data */
#ifdef SSI_DEBUG_LEVEL1
   Size     staticHeapMemUsed; /* amount of heap memory used for static allocations */
   Size     dynamicHeapMemUsed; /* amount of heap memory used for dynamic allocations */
   Size     nextOffset; /* Offset to the next pointer in structure CmHEntry */
                        /* Update this in cmMmHeapInit if cmHEntry is changed */
   StsCntr      numAllocAttempts;   /* total number of allocation attempts in heap */
   StsCntr      numDeallocAttempts;   /* total number of de-allocation attempts in heap */
   uint8_t	    trampleCount;	/* incremented everytime a trampling is detected in any block of the heap */
#endif /* SSI_DEBUG_LEVEL1 */
 /* cm_mem_x_001.main_11: Additions */
#ifdef SS_HISTOGRAM_SUPPORT
   /* Hash list to support Heap Block histogram */
   CmHstGrmHashListCp heapHstGrmHashListCp;
#endif /* SS_HISTOGRAM_SUPPORT */

};

/* cm_mem_x_001.main_9 - addition of hash list for maintaining size vs. numAttempts statistics */
#ifdef SSI_DEBUG_LEVEL1
struct cmMmHashListEnt    /* hash list entry */
{
   uint32_t  size;             /* pointer to key */
   uint32_t  numAttempts;      /* number of attempts */
};

struct cmMmHashListCp
{
   CmMmHashListEnt  *hashList;
   uint16_t  numOfbins;
   uint16_t  numOfEntries;
};
#endif /* SSI_DEBUG_LEVEL1 */

/* Memory region control block */ 
struct cmMmRegCb
{
   Region       region;        /* Region  Id of the memory */
   SRegInfo     regInfo;       /* Region information block */

   uint8_t           chFlag;        /* Flag defines the region characteristics */
   Data        *pAddr;         /* Physical address of the memory block.
                                  Valid if CMM_REG_PHY_VALID bit is set */
 
   Size         bktSize;       /* Size of the memory used for the bucket pool */
   uint16_t          bktQnPwr;      /* Quantum size of the bucket pool */
   Size         bktMaxBlkSize; /* Maximum size of block in the bucket pool */
   uint16_t          numBkts;       /* Number of buckets in the Bucket Pool */

   CmMmMapBkt   mapTbl[CMM_MAX_MAP_ENT]; /* size-to-bucket map table */
   CmMmBkt      bktTbl[CMM_MAX_BKT_ENT]; /* Pointer to the memory bkt tbl */
 
   Bool         heapFlag;      /* Set to true if the heap pool is configured */
   Size         heapSize;      /* Size of the heap pool */
   CmMmHeapCb   heapCb;        /* Heap pool control block */
/* cm_mem_x_001.main_9 - addition of hash list control point in region control block */
#ifdef SSI_DEBUG_LEVEL1
   CmMmHashListCp hashListCp;  /* hast list to maintain the size statistics */
#endif /* SSI_DEBUG_LEVEL1 */
#ifdef BRDCM_SSI_MEM_LEAK_DEBUG_LEVEL2
   CmHashListCp brdcmSsiLstCp;  /* hast list to maintain the SSI memory information for Broadcom */
#endif
#ifdef SS_MULTICORE_SUPPORT /*cm_mem_x_001.main_10*/
   SsSTskEntry  *sTsk;
#endif /* SS_MULTICORE_SUPPORT */

};

#ifdef SS_LIGHT_MEM_LEAK_STS
typedef struct allocInfo
{
   Void   *memAddr;
   uint8_t     used;
}AllocInfo;

typedef struct memLeakCb
{
  SLockId    memLock;
  uint8_t         isStarted;
  AllocInfo  allocInfo[CM_MAX_ALLOC_ENTERIES];
  uint32_t        queue[CM_MAX_ALLOC_ENTERIES];
  uint32_t        head;
  uint32_t        tail;
}MemLeakCb;

Void cmInitMemLeak     ARGS((Void));
Void cmDeinitMemLeak   ARGS((Void));
Void cmStartStopLeakLog   ARGS((Void));
uint32_t  cmStorAllocBlk       ARGS((Void *addr));
Void cmRlsAllocBlk        ARGS((uint32_t indx));
Void cmPrintLeakLog       ARGS((Void));
#endif /* SS_LIGHT_MEM_LEAK_STS */

 /* cm_mem_x_001.main_11: Additions */
#ifdef SS_MEM_LEAK_STS
typedef struct _memHash
{
   SLockId         memLck;
   Bool            used;
   CmHashListCp    memHashCp;
}MemHash;

typedef struct _memAllocInfo
{
   CmHashListEnt         ent;
#ifdef BIT_64
   uint64_t                   memAddr;
#else
   uint32_t                   memAddr;
#endif
   Size                  reqSz;
   Size                  allocSz;
   uint16_t                   bktIdx;
   uint8_t                    moduleId;
   uint8_t                    bTrcSz;
   PTR                   backTrace;
}MemAllocInfo; 

typedef struct _memLkCb
{
   Bool        memLkMdlInit;
   FILE        *fileLkLog;
   MemHash     memUsrMdl[CM_MEM_USR_MDL][CM_MAX_HASH_PER_TSK];
}MemLkCb;

typedef struct _memUsrMdlStr
{
   Txt   *fPStr;
   Txt   *mdlStr; 
}MemUsrMdlStr;

#ifdef SS_MEM_LEAK_SOL
typedef struct backtrace
{
  Void   **bt_buffer;
  S32    bt_maxcount;
  S32    bt_actcount;
} Backtrace_t;
#endif /* SS_MEM_LEAK_SOL */

#endif /* SS_MEM_LEAK_STS */

#ifdef T2K_MEM_LEAK_DBG
#define T2K_MEM_LEAK_INFO_TABLE_SIZE 8388606
   /* osCp.wls.allocAddr is the starting address allocated by WLS,
          * whenever that changes pleasse change here */
uint64_t  t2k_MEM_LEAK_START_ADDR;


typedef struct _t2kMemLeakInfo
{
   uint64_t address; /* 0 means it is free */
   uint32_t size;    /* size of the block */
   uint32_t age; /* in ticks, when it was allocated */
   uint32_t lineNo;  /* lineNo in the file from where allocation was done */
   char* fileName; /* from where allocation was done */
   uint32_t prevRemLineNo;  /* lineNo in the file from where allocation was done */
   char* prevRemFileName; /* from where allocation was done */
   uint32_t lastDelLineNum;  /* lineNo in the file from where allocation was done */
   char* lastDelFileName; /* from where allocation was done */
}T2kMeamLeakInfo;

typedef struct _regMemLeakInfo
{
   uint32_t             numActvRegions;
   T2kMeamLeakInfo *gMemLeakInfo[SS_MAX_REGS]; /* Allocate using calloc(system Call ) size T2K_MEM_LEAK_INFO_TABLE_SIZE*/
   uint64_t             regStartAddr[SS_MAX_REGS];  /* start Address of Region  */
   pthread_mutex_t memLock[SS_MAX_REGS];  /*  Region Lock */
}RegionMemLeakInfo;

#endif /*T2K_MEM_LEAK_DBG */


/* functions prototypes */
 
S16 cmMmRegInit   ARGS((
                         Region region, 
                         CmMmRegCb *regCb, 
                         CmMmRegCfg *cfg));
S16 cmMmRegDeInit ARGS(( CmMmRegCb *regCb));
 /* cm_mem_x_001.main_11: Additions */
#ifdef BRDCM_SSI_MEM_LEAK_DEBUG_LEVEL2
Void cmRlsAllocBlk       ARGS((CmMmBlkHdr *ptrHdr, CmMmRegCb *regCb));
Void cmStorAllocBlk      ARGS((CmMmBlkHdr  *ptrHdr, Size reqSz, Size allocSz,
                                      uint16_t bktIdx, CmMmRegCb *regCb));
#endif
#ifdef SS_MEM_LEAK_STS
Void cmInitMemLeakMdl    ARGS((Void));
/* cm_mem_x_001.main_12 : Additions */
Void cmDeinitMemLeakMdl    ARGS((Void));
#ifdef BIT_64
Void cmStorAllocBlk      ARGS((uint64_t addr, Size reqSz, Size allocSz,
                                      uint16_t bktIdx));
Void cmRlsAllocBlk       ARGS((uint64_t addr));
#else
Void cmStorAllocBlk      ARGS((uint32_t addr, Size reqSz, Size allocSz,
                                      uint16_t bktIdx));
Void cmRlsAllocBlk       ARGS((uint32_t addr));
#endif
uint8_t   cmMemGetModuleId    ARGS((S8 **funNm, S32 traceSize));
S16  cmMemGetStrMtchIdx  ARGS((uint8_t strtIdx, uint8_t endIdx,
                                      S8 *str, S8 **strLst));
Void cmMemOpenMemLkFile  ARGS((S8 *arg));
Void SLogLkInfo          ARGS((Void));
Void SFlushLkInfo        ARGS((Void));

#ifdef SS_MEM_LEAK_SOL
S32 cmAddrToSymStr       ARGS((Void *pc, S8 *buffer, S32 size));
S32 cmLeakCallBack       ARGS((uintptr_t pc, S32 signo, Void *arg));
S32 backtrace            ARGS((Void **buffer, S32 count));
#endif /* SS_MEM_LEAK_SOL */

#endif /* SS_MEM_LEAK_STS */
/* cm_mem_x_001.main_9 - addition of an API prototype for sanity check */
#ifdef SSI_DEBUG_LEVEL1
S16 cmMmRegIsBlkSane ARGS((CmMmBlkHdr *blkPtr));
#endif /* SSI_DEBUG_LEVEL1 */

#ifdef BRDCM_SSI_MEM_LEAK_DEBUG_LEVEL1
S16 cmMmRegIsBlkSane ARGS((CmMmBlkHdr *blkPtr, Size size));
#endif
#ifdef SS_HISTOGRAM_SUPPORT
#ifdef SSI_DEBUG_LEVEL1
S16  cmAllocNL ARGS((Void *regionCb, Size *size, uint32_t flags, Data **ptr, uint32_t  memType, uint32_t line, uint8_t *fileName, uint8_t entId, Bool hstReg));
#else
S16  cmAllocNL ARGS((Void *regionCb, Size *size, uint32_t flags, Data **ptr, uint32_t line, uint8_t *fileName, uint8_t entId, Bool hstReg));
#endif
#elif defined(SS_LIGHT_MEM_LEAK_STS)
S16  cmAllocNL ARGS((Void   *regionCb,Size   *size,uint32_t     flags,Data  **ptr,uint32_t     memType,uint32_t     lineNo,uint8_t     *funcName));
#else
#if (defined(SSI_DEBUG_LEVEL1) || defined(BRDCM_SSI_MEM_LEAK_DEBUG_LEVEL1))
S16  cmAllocNL ARGS((Void *regionCb, Size *size, uint32_t flags, Data **ptr, uint32_t  memType));
#else
S16  cmAllocNL ARGS((Void *regionCb, Size *size, uint32_t flags, Data **ptr));
#endif
#endif

#ifdef SS_HISTOGRAM_SUPPORT
S16  cmFreeNL ARGS((Void *regionCb, Data *ptr, Size size, uint32_t line, uint8_t *fileName, uint8_t entId, Bool hstReg));
#elif defined(SS_LIGHT_MEM_LEAK_STS)
S16  cmFreeNL ARGS((Void   *regionCb,Data   *ptr, Size    size,uint32_t     lineNo,uint8_t      *funcName));
#else
S16  cmFreeNL ARGS((Void *regionCb, Data *ptr, Size size));
#endif

#ifdef __cplusplus
}
#endif

#endif

/**********************************************************************
         End of file
**********************************************************************/
