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
 
     Name:     Lockless Memory Manager 
 
     Type:     C include file
 
     Desc:     Data structure definitions that are used by 
               the Lockless Memory Manager.
 
     File:     cm_mem_wl.x
 
*********************************************************************21*/

#ifndef __CMMEMWLX_
#define __CMMEMWLX_

#ifdef __cplusplus
extern "C" {
#endif


#ifdef SS_LOCKLESS_MEMORY
/* typedefs */
typedef struct cmMmDynRegCb CmMmDynRegCb;
typedef struct cmMmDynBktCb CmMmDynBktCb;
typedef struct cmMmMapBkt  CmMmMapBkt;
typedef struct cmMmGlobRegCb CmMmGlobRegCb;
typedef struct cmMmGlobalBktCb CmMmGlobalBktCb;
typedef struct cmMmBlkSetElement CmMmBlkSetElement;
typedef Data CmMmEntry;
 
/* Size-To-Bucket map table structure */ 
struct cmMmMapBkt
{
   uint16_t   bktIdx;              /* The index to the memory bucket */

#if (ERRCLASS & ERRCLS_DEBUG)
   uint16_t   numReq;              /* Number of the allocation request */
   uint16_t   numFailure;          /* Number of allocation failure form the bucket */
#endif

};

/* The following structre is used as node to place the first address of the
 * bucket list in the linkedlist. During initialization, all the buckets are 
 * divided into limited number sets. The first pointer of the bucket is placed
 * in the following structure and node is placed in the linked list. Once the one set
 * get over, the next address in the BktCb is changed to new address */
struct cmMmBlkSetElement
{
   CmLList    memSetNode;     /* Linked list of the first pointer of the 
                                 bucket */
   uint32_t        numFreeBlks;    /* Number of Free Blocks avilable in the current
                                 set */
   CmMmEntry  *nextBktPtr;    /* Address of the first bucket which will be
                                 used for the allocation */
};

/* The following structure is used for the storing the buckets/pools
 * this will be present for each bucket which is configured and 
 * will contains the all the information about the bucket. This
 * will be used in global region  */
struct cmMmGlobalBktCb
{
   uint8_t           poolId;          /* Pool Id */
   Size         size;            /* Size of the pool */
   CmLListCp    listValidBktSet; /* LList Cp used to store the nodes of memory element
                                    node which holds valid pointers */
   CmLListCp    listFreeBktSet;  /* LList Cp used to store the nodes of memory element
                                    node which holds invalid information */
   CmMmEntry    *startAddr;      /* Starting address */
   CmMmEntry    *next;           /* Next address which will be allocated */
   uint32_t          numBlks;          /* Total number of buckets configured */
   uint32_t          bucketSetSize;
   uint32_t          bktFailCnt;      /* bucket alloc failure count */
   uint32_t          bktNoFitCnt;     /* number of blocks not fit */
   SLockId      bucketLock;      /* Lock used for the allocation/freeing of
                                    the set of buckets */
};

/* The following structure is used for the storing the buckets/pools
 * this will be present for each bucket which is configured and 
 * will contains the all the information about the bucket. This
 * is used in the system taks local region */
struct cmMmDynBktCb
{
   uint8_t                 poolId;                   /* Pool Id */
   Size               size;                     /* Size of the pool */
   CmLListCp          memBlkSetElem;            /* Memory set element likend list */
   CmMmBlkSetElement *crntMemBlkSetElem;        /* Pointer to the current memort block set */
   CmMmEntry         *startAddr;               /* Starting address */
   CmMmEntry         *next;                    /* Next address which will be allocated */
   uint32_t               numBktAlloc;              /* Total number of buckets allocted */
   uint32_t               numBkt;                   /* Total number of buckets configured */
   uint16_t               bucketSetSize;            /* Size of bucket sets */
   uint16_t               blkSetRelThreshold;       /* Upper threshold of bucket count upon
                                                  reaching will free the set to global region */
   uint16_t               blkSetAcquireThreshold;   /* Lower threshold of bucket count upon
                                                  reaching will request the set from
                                                  global region */
   uint32_t               bktFailCnt;                /* bucket alloc failure count */
   uint32_t               bktNoFitCnt;               /* number of blocks not fit */
};

/* The individual Task regionCb which will be associate with every system
 * task which will be created. This contains the mapping of the size to
 * to bucket index and the different bucket informations.*/
#ifdef SS_USE_ICC_MEMORY
struct cmMmDynRegCb
{
   Region          region;                   /* Region  Id of the memory */
   uint16_t             bktSize[CMM_MAX_BKT_ENT]; /* Bucket Size */
   void            *iccHdl;                  /* Handler used for ICC memory */
   SsAlloc         alloc;                    /* allocation function pointer */
   SsFree          free;                     /* deallocation function pointer */
   SsCtl           ctl;                      /* control request function pointer */
};
#else
struct cmMmDynRegCb
{
   Region          region;                  /* Region  Id of the memory */
   uint8_t              regType;                 /* Region Type, static or dynamic or global */
   uint8_t              numBktBlk;               /* Keeps the count of number of bucket set blocks */
   uint16_t             numBkts;                 /* Number of buckets in the Bucket Pool */
   Size            bktSize;                 /* Size of the memory used for the bucket pool */
   uint16_t             bktQnPwr;                /* Quantum size of the bucket pool */
   Size            bktMaxBlkSize;           /* Maximum size of block in the bucket pool */
   CmMmDynBktCb    bktTbl[CMM_MAX_BKT_ENT]; /* Pointer to the memory bkt tbl */
   SsAlloc         alloc;                   /* allocation function pointer */
   SsFree          free;                    /* deallocation function pointer */
   SsCtl           ctl;                     /* control request function pointer */
   CmMmMapBkt      mapTbl[CMM_MAX_MAP_ENT]; /* size-to-bucket map table */
};
#endif /* SS_USE_ICC_MEMORY */

/* The global region Cb which holds the entire buckets configured */
struct cmMmGlobRegCb
{
   Region           region;                  /* Region  Id of the memory */
   uint16_t              numBkts;                 /* Number of buckets in the Bucket Pool */
   CmMmGlobalBktCb  bktTbl[CMM_MAX_BKT_ENT]; /* Pointer to the memory bkt tbl */
};

/* SSI_OPT: Stop */

/* typedefs */
typedef struct cmMmRegCfg    CmMmRegCfg;
typedef struct cmMmBktCfg    CmMmBktCfg;

 
typedef struct   cmMmRegCb     CmMmRegCb;
typedef struct   cmMmHeapCb    CmMmHeapCb;
typedef struct   cmMmBkt       CmMmBkt;
typedef struct   cmHEntry      CmHEntry;
/* cm_mem_x_001.main_9 - addition of new data types for providing 
* header for memory blocks in buckets and a hash list to maintain
* size vs. numAttempts statistics 
*/
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
#ifdef SSI_DEBUG_LEVEL1
/* Memory Block Header */
struct cmMmBlkHdr
{
   /* trSignature shall always be maintained as the first element in this structure */
   uint8_t          trSignature[CMM_TRAMPLING_SIGNATURE_LEN];  /* signature to detect trampling */
   CmMmBlkHdr  *nextBlk;         /* pointer to the next memory block header */
   uint32_t         memFlags;         /* Free/Static/Dynamic */
   Size        requestedSize;    /* requested size for which this mem blk has been allocated */
};
#endif /* SSI_DEBUG_LEVEL1 */

/* Memory bucket structure */
struct cmMmBkt                /* Bucket Structure */
{
/* cm_mem_x_001.main_9 - addition of next block pointer */
#ifdef SSI_DEBUG_LEVEL1
   CmMmBlkHdr   *nextBlk;   /* pointer to the next memory block header */
#else
   CmMmEntry   *next;         /* Pointer to the next memory block */
#endif /* SSI_DEBUG_LEVEL1 */
   Size         size;         /* Size of the block */
   uint32_t          numBlks;      /* Total number of blocks in the bucket */
   uint32_t          maxAlloc;     /* Maximum number of blocks allocated till now */
   uint32_t          numAlloc;     /* Number of blocks allocated */
   SLockId      bktLock;      /* Lock to protect the bucket pool */ 
   uint32_t          bktFailCnt;   /* bucket alloc failure count */
   uint32_t          bktNoFitCnt;  /* number of blocks not fit */
/* cm_mem_x_001.main_9 - addition of statistics related data */
#ifdef SSI_DEBUG_LEVEL1
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
#ifdef SS_MULTICORE_SUPPORT /*cm_mem_x_001.main_10*/
   SsSTskEntry  *sTsk;
#endif /* SS_MULTICORE_SUPPORT */

};

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
   uint32_t                   memAddr;
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


/* functions prototypes */
 
S16 cmMmRegInit   ARGS((
                         Region region, 
                         CmMmRegCb *regCb, 
                         CmMmRegCfg *cfg));
S16 cmMmRegDeInit ARGS(( CmMmRegCb *regCb));
 /* cm_mem_x_001.main_11: Additions */
#ifdef SS_MEM_LEAK_STS
Void cmInitMemLeakMdl    ARGS((Void));
/* cm_mem_x_001.main_12 : Additions */
Void cmDeinitMemLeakMdl    ARGS((Void));
Void cmStorAllocBlk      ARGS((uint32_t addr, Size reqSz, Size allocSz,
                                      uint16_t bktIdx));
Void cmRlsAllocBlk       ARGS((uint32_t addr));
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


Void DumpLayersDebugInformation        ARGS((Void));
Void DumpSSIDemandQDebugInformation    ARGS((Void));
Void DumpPDCPDlDebugInformation        ARGS((Void));
Void DumpPDCPUlDebugInformation        ARGS((Void));
Void DumpRLCDlDebugInformation         ARGS((Void));
Void DumpRLCUlDebugInformation         ARGS((Void));
Void DumpRLCUlDebugInformation         ARGS((Void));
Void printMacCellInfo                  ARGS((Void));
Void printSchCellInfo                  ARGS((Void));

uint32_t isMemThreshReached    ARGS((Region region));

S16 cmMmStatRegInit       ARGS((Region region, CmMmRegCb *regCb, CmMmRegCfg *cfg));
S16 cmMmGlobRegInit       ARGS((CmMmGlobRegCb *regCb));
S16 cmMmDynRegInit        ARGS((CmMmDynRegCb  *regCb));
#ifdef USE_MALLOC
S16 ssGetDynMemBlkSet  ARGS((uint8_t bktIdx, CmMmBlkSetElement *dynMemSetElem));
S16 ssPutDynMemBlkSet  ARGS((uint8_t bktIdx, CmMmBlkSetElement *dynMemSetElem));
#else
S16 ssPutDynMemBlkSet ARGS((uint8_t bktIdx,CmMmBlkSetElement *dynMemSetElem,
                                   uint32_t doNotBlockForLock));
S16 ssGetDynMemBlkSet  ARGS((uint8_t bktIdx, CmMmBlkSetElement *dynMemSetElem,
                                   uint32_t doNotBlockForLock));
#endif /* USE_MALLOC */
#endif /* SS_LOCKLESS_MEMORY */
#ifdef __cplusplus
}
#endif

#ifdef T2K_MEM_LEAK_DBG
#endif
#endif

/**********************************************************************
         End of file
*********************************************************************/

