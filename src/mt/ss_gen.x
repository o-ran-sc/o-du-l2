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
 
     Name:     System Services -- general
 
     Type:     C include file
 
     Desc:     Data structure definitions that are used by all
               implementations of system services.
 
     File:     ss_gen.x
 
*********************************************************************21*/


#ifndef __SSGENX__
#define __SSGENX__

#ifdef __cplusplus
extern "C" {
#endif

struct cmMmGlobRegCb;
/* ss029.103: addition: multiple procIds related changes */ 
#ifdef SS_MULTIPLE_PROCS
typedef struct {
   uint16_t free;
   ProcId procId[SS_MAX_PROCS];
} ProcIdLst;
#endif /* SS_MULTIPLE_PROCS */

/* multi-core enhancements */
/*ss013.301 : changes for SS_AFFINITY_SUPPORT*/
#if defined(SS_MULTICORE_SUPPORT) || defined(SS_AFFINITY_SUPPORT)

typedef struct {
  SCpuInfo cpuInfo; /* the info about the cores/threads per core */


  /* the currently used core */
  uint32_t currentCore;
  
  /*COMMENT: add the thread id for use on hyperthreading machines */
  struct {
    uint32_t thrs;                     /* available no. of threads per core */
    S8 exclusive;                 /* exclusive flag */
    SSTskId tskPerCoreLst[SS_MAX_THREADS_PER_CORE]; /* System tasks running on this core */
  } coreInfo[SS_MAX_CORES];
} SMultiCoreInfo;
#endif /* SS_MULTICORE_SUPPORT || SS_AFFINITY_SUPPORT */

/* ss001.301: additions */
/* ss002.301: Modifications */
#ifdef SS_THREAD_PROFILE
#ifdef SS_MULTIPLE_PROCS
S16 SGetThrdProf(SSTskId *sTskId,ProcId procId,Ent ent,Inst inst,Event *curEvent,uint32_t *curEvtTime,uint64_t *totTime);
#else
S16 SGetThrdProf(SSTskId *sTskId,Ent ent,Inst inst,Event *curEvent,uint32_t *curEvtTime,uint64_t *totTime);
#endif /* SS_MULTIPLE_PROCS */
#endif /* SS_THERAD_PROFILE */

#ifdef SS_WATCHDOG

typedef void (*WdUserCallback)(void *);

typedef struct {
#ifdef SS_WATCHDOG_IPV6
   struct in6_addr  addr;
#else
   struct in_addr addr;       /* configured node addr */
#endif /* SS_WATCHDOG_IPV6 */
   uint16_t port;       /* configured watchdog port */
   uint8_t status;      /* HB ACK status */
} watchDogStatus;

typedef struct ssWd {
   uint32_t            timeout;                 /* configured HB timer */
   uint8_t             watchdogStop;            /* watchdog stop flag */
   int            numNodes;                /* configured nodes */
#ifdef SS_WIN
   unsigned int   sock;                    /* HB socket descriptor */
#else
   int            sock;
#endif /* SS_WIN */

   watchDogStatus wdsta[SS_MAX_WD_NODES];  /* node config or status */
   WdUserCallback callback;                /* user callback */
   void           *data;                   /* user callback data */
} SsWd;

typedef struct ssWdCp{
   SSTskId               watchDgTskId;
   SSTskId               watchDgRcvrTskId;
   Pst                   watchDgPst;
   CmTqCp                watchDgTqCp;  /* timing queue control point : WatchDog */
   CmTqType              watchDgTs[2]; /*timing queue */ 
   CmTimer               watchDgTmr[2];         
   ProcId                watchDgprocId;
   SsWd                  globWd;
   SLockId               wdLock;
} SsWdCp;
S16 SInitWatchdog(uint16_t port);
S16 SRegCfgWd(uint32_t numNodes, uint8_t *addr[], uint16_t port[], uint32_t timeout, WdUserCallback callback, void *data);
S16 SDeregCfgWd(void);
S16 SStartHrtBt(uint8_t timeInterval);
S16 SStopHrtBt(void);
#endif /* SS_WATCHDOG */

#ifdef SS_LOGGER_SUPPORT
/* Logger Info */
typedef struct sloggerInfo
{
     Bool            started;      /* flag to indicate logger status */

     Bool            configured;   /* flag to indicate whether logger is configured */

     Bool            opt;               /* write to file/socket based on the flags provided*/      
   
     FILE*           filep;
     S8              filePath[SS_MAX_PATH];
   
     S32             socketdes;
     struct sockaddr_in remoteAddr;
   
     uint16_t             curNumFlush;
     uint16_t             maxNumFlush;
     
     S8              buffer[SS_MAX_LOGBUF_SIZE];
     uint32_t             maxBufSiz;        /*The size of this is determined by the 
                                            system on which its running.*/
     uint32_t             curBufSiz;
     SLockId         bufLock;            /* lock for global buffer access */
} SLoggerInfo;
#endif /*  SS_LOGGER_SUPPORT  */

#if defined (INTEL_WLS) || defined (SS_USE_WLS_MEM)
typedef struct _MtWls
{
  Void   *intf;
  Void   *allocAddr;
}SsMtWls;
#endif   /* INTEL_WLS */


#ifdef NTL_LIB
typedef struct _MtNtl
{
   uint32_t        hdl;
}SsMtNtl;
#endif   /* NTL_LIB */



/* SS control point */
typedef struct ssos
{
   
   SsdOs                dep;                    /* implementation specific */

/* ss029.103: modification: 
   with multiple procId support, SSI shall keep list of registered procIds  */
#ifndef SS_MULTIPLE_PROCS
   ProcId               procId;                 /* processor ID */

    /* TAPA task info */
   SsIdx                tTskIds[SS_MAX_ENT][SS_MAX_INST];
#else
   ProcIdLst            procLst;                /* processor ID list */
   SsIdx                tTskIds[SS_MAX_PROCS][SS_MAX_ENT][SS_MAX_INST];
   /* TAPA task info */
#endif /* SS_MULTIPLE_PROCS */
                                               /* index table */
   SsTTskEntry          tTskTbl[SS_MAX_TTSKS];  /* task table */
   SsCntr               numTTsks;               /* count of tasks */
   SsIdx                nxtTTskEntry;           /* next available slot */

   SsSemaId             tTskTblSem;             /* lock for table access */


   /* system task info */
   SsSTskEntry          sTskTbl[SS_MAX_STSKS];  /* task table */

   /* Thread to region mapping. The array is indexed with reminder
      of thread ID after deviding it with SS_MAX_THREAD_REGION_MAP 
    */
#ifdef SS_THR_REG_MAP
   Region               threadMemoryRegionMap[SS_MAX_THREAD_REGION_MAP];
#endif
   SsCntr               numSTsks;               /* count of tasks */
   SsIdx                nxtSTskEntry;           /* next available slot */

   SLockId              sTskTblLock;            /* lock for table access */


   uint8_t                   dmndQLookupTbl[256];    /* demand queue lookup table */


#ifdef SS_DRVR_SUPPORT

   /* driver task info */
   SsDrvrTskEntry       drvrTskTbl[SS_MAX_DRVRTSKS];
                                                /* task table */
   SsCntr               numDrvrTsks;            /* count of tasks */

#endif  /* SS_DRVR_SUPPORT */


#ifdef SS_RTR_SUPPORT

   /* router task info */
   ActvTsk              rtrTskTbl[SS_MAX_RTRTSKS];
   SLockId              rtrTskLocks[SS_MAX_RTRTSKS];

#endif  /* SS_RTR_SUPPORT */


   /* timer info */
   SsTmrEntry           tmrTbl[SS_MAX_TMRS];        /* timer table */
   SsCntr               numTmrs;                    /* count of timers */
   SsIdx                nxtTmrEntry;                /* next available slot */

   SLockId              tmrTblLock;                 /* lock for table access */

   /* Pointer to global region */
   struct cmMmGlobRegCb *globRegCb;                 /* Global Region Cb */

   SsCntr               numRegions;                 /* count of regions */
   SsRegionEntry        regionTbl[SS_MAX_REGS];     /* region table */
   SsCntr               numDynRegions;              /* Number of dynamic regions */
   SsRegionEntry        dynRegionTbl[SS_MAX_REGS];  /* Dynamic Region Table */

   SsSemaId             regionTblSem;               /* lock for table access */

   /* ss028.103 - Addition of lock for mBuf reference count */
	/* ss007.301 - moved the lock to RegionTbl */
   /* SLockId              mBufRefLock; */            /* lock for mBuf ref access */

/*ss013.301 : changes for SS_AFFINITY_SUPPORT*/
#if defined(SS_MULTICORE_SUPPORT) || defined(SS_AFFINITY_SUPPORT)
   /* the Information about the CPU */
   SMultiCoreInfo mCInfo;

   SLockId mCILock;                             /* Lock for mCInfo access */

#endif /* SS_MULTICORE_SUPPORT || SS_AFFINITY_SUPPORT */

/* ss001.301: additions */
#ifdef SS_WATCHDOG 
   SsWdCp                wdCp;
#endif /* SS_WATCHDOG */

#ifdef SS_HISTOGRAM_SUPPORT
   uint8_t                    entId[26][26];
#endif /* SS_HISTOGRAM_SUPPORT */

#ifdef SS_LOGGER_SUPPORT
 SLoggerInfo logger;
#endif /*  SS_LOGGER_SUPPORT  */
 S8          *configFilePath;
#if defined (INTEL_WLS) || defined (SS_USE_WLS_MEM)
   SsMtWls   wls;
#endif

#ifdef NTL_LIB
   SsMtNtl   ntl;
#endif

} SsOs;



/* configuration data structures */

/* pool configuration */
typedef struct ssPoolCfg
{
   SsPoolType   type;                   /* dynamic or static */
   Size         size;                   /* size to use (for dynamic pools) */

} SsPoolCfg;


/* region content--pools in a region */
typedef struct ssRegCfg
{
   Region       region;                      /* region ID */
   SsCntr       numPools;                    /* count of pools */
   SsPoolCfg    pools[SS_MAX_POOLS_PER_REG]; /* pools configuration */

} SsRegCfg;



/* external variable declaration */
/*ss014.301  osCp as volatile for SSI-4GMX*/
#ifdef  SS_4GMX_LCORE
volatile SsOs osCp;
#else
SsOs osCp;
#endif


/* functions */
S16 SInit ARGS((void));
/*ss009.301*/
S16 SFini ARGS((void));
/* ss034.103 */
S16 SDeInit ARGS((void));

/* implementation-specific functions */
S16 ssdInitGen ARGS((void));
S16 ssdInitMem ARGS((void));
S16 ssdInitTsk ARGS((void));
S16 ssdInitDrvr ARGS((void));
S16 ssdInitTmr ARGS((void));
S16 ssdReInitTmr ARGS((void));
/* ss005.301: ssdInitFinal changed to ssdInitLog */
S16 ssdInitLog ARGS((void));

Void ssdDeinitGen ARGS((void));
Void ssdDeinitMem ARGS((void));
Void ssdDeinitTsk ARGS((void));
Void ssdDeinitDrvr ARGS((void));
Void ssdDeinitTmr ARGS((void));
/* ss005.301: ssdDeinitFinal changed to ssdDeinitLog */
Void ssdDeinitLog ARGS((void));

Void ssdStart ARGS((void));

S16 ssdAttachTTsk ARGS((SsTTskEntry *));
S16 ssdDetachTTsk ARGS((SsTTskEntry *));
S16 ssdCreateSTsk ARGS((SsSTskEntry *));
S16 ssdDestroySTsk ARGS((SsSTskEntry *));
S16 ssdPstTsk ARGS((Pst *, Buffer *, SsTTskEntry *));
S16 ssdRegTmr ARGS((SsTmrEntry *));
S16 ssdDeregTmr ARGS((SsTmrEntry *));
S16 ssdError ARGS((Seq, Reason));
Void ssdLogError ARGS((Ent, Inst, ProcId, Txt *, S32, \
                              ErrCls, ErrCode, ErrVal, Txt *));

Void mtTmrHdlrPublic ARGS ((void)); 

/*
 * SDeRegTTsk patch
 */
/* for TTask Dereg */
#ifdef SS_MULTIPLE_PROCS
S16 ssdProcTTskTerm ARGS((ProcId proc, SsTTskEntry *tTsk, SsIdx idx));
#else
S16 ssdProcTTskTerm ARGS((SsTTskEntry *tTsk, SsIdx idx));
#endif /* SS_MULTIPLE_PROCS */

#ifdef SS_DRVR_SUPPORT
S16 ssdRegDrvrTsk ARGS((SsDrvrTskEntry *));
/*ss001.301: Additions */
S16 ssdDeregDrvrTsk ARGS((SsDrvrTskEntry *));
#endif

/* ss029.103: addition: support function to implement multiple procIds */ 
#ifdef SS_MULTIPLE_PROCS
uint16_t SGetProcIdIdx ARGS((ProcId proc));
#endif /* SS_MULTIPLE_PROCS */

/* multi-core support */
/*ss013.301 : changes for SS_AFFINITY_SUPPORT*/
#if defined(SS_MULTICORE_SUPPORT) || defined(SS_AFFINITY_SUPPORT)
 
S16 ssdSetAffinity ARGS((SSTskId *tskId, uint32_t coreId));
S16 ssdGetAffinity ARGS((SSTskId *tskId, uint32_t *coreId));
#endif /* SS_MULTICORE_SUPPORT || SS_AFFINITY_SUPPORT */

/* ss001.301: additions */
#ifdef SS_LOGGER_SUPPORT 
S16 ssdInitLogger ARGS((void));
S16 SFlushBufToLog ARGS (( S8 *logBuf));
/* ss02.301: additions */
S16 SCleanUp ARGS ((Void ));
Void SStartLogger ARGS ((Void ));
Void SStopLogger ARGS ((Void ));
#endif /* SS_LOGGER_SUPPORT  */
/* ss02.301: additions */
#ifdef SS_WATCHDOG 
#ifdef SS_MULTIPLE_PROCS
S16 ssdWatchDgActvTmr ARGS(( ProcId proc, Ent ent, Inst instVoid));
#else
S16 ssdWatchDgActvTmr ARGS(( Void ));
#endif /* SS_MULTIPLE_PROCS */
Void ssdWatchDgTmrEvt ARGS(( PTR cb, S16 event ));
S16 watchDgActvTsk ARGS(( Pst *pst, Buffer *mBuf));
S16 watchDgRcvrActvTsk ARGS(( Pst *pst, Buffer *mBuf ));
S16 ssdSndHrtBtMsg ARGS(( Bool  restart, uint32_t type));
Void ssdStartWatchDgTmr ARGS(( void *cb, S16 event, uint16_t wait));
Void ssdStopWatchDgTmr ARGS(( void *cb, S16 event));
S16 ssdInitWatchDgPst ARGS((Pst *pst)); 	
S16 ssdInitWatchDog ARGS((uint16_t port));
#endif

#ifdef SS_FBSED_TSK_REG
S16 SRegTskInfo ARGS((uint8_t *cfgFile));  
#endif
/* ss002.301 Readwrite lock additions */
#ifdef SS_LOCK_SUPPORT
S16 ssdLockNew ARGS((SLockInfo *LockId, uint8_t locktype));
S16 ssdInitLockNew ARGS((SLockInfo *LockId, uint8_t lockType));
S16 ssdUnlockNew ARGS((SLockInfo *LockId, uint8_t lockType));
S16 ssdDestroyLockNew ARGS((SLockInfo *LockId, uint8_t lockType));
#endif /* SS_LOCK_SUPPORT */

#ifdef SSI_STATIC_MEM_LEAK_DETECTION
/* For Static memory leak detection */
#define MAX_MEM_ALLOCATIONS 100000

typedef struct _listInfo
{
   uint32_t nextIdx;
}ListInfo;

typedef struct _eachAllocInfo
{
   ListInfo listInfo;
   /* other info should come here */
   char *file;
   uint32_t lineNo;
   uint32_t age;
   void *ptr;
   uint32_t size;
   /* end of other info */
}EachAllocInfo;

typedef struct _staticMemAllocationInfo
{
   uint32_t nextFreeIdx;
   EachAllocInfo allocations[MAX_MEM_ALLOCATIONS];
}StaticMemAllocInfo;

StaticMemAllocInfo SMemLeakInfo[4];
FILE* StaticMemLeakFileArr[4];
/* End Static Memory leak detection */
#endif

#ifdef __cplusplus
}
#endif

void DumpSSIDemandQDebugInformation ARGS((void));
void mtSigSegvHndlr ARGS((void));
void mtSigUsr2Hndlr ARGS((void));
#endif  /*  __SSGENX__  */



/********************************************************************30**
 
         End of file
**********************************************************************/
