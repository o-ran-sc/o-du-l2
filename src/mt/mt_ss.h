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

     Name:     MTSS -- implementation specific definitions

     Type:     C include file

     Desc:     Various macro definitions demanded by systems services.
               The contents of these are for the MTSS implementation.

     File:     mt_ss.h

*********************************************************************21*/


#ifndef __MTSSH__
#define __MTSSH__


/* --- interface to SS --- */

/* general */
#define SS_PROC_ID                      PID_STK(1)

/* task related */
/* mt028.201: addition: multiple procs support related changes */
#ifdef SS_MULTIPLE_PROCS
#define SS_MAX_PROCS                    100 /* max entries for proc list */
#endif /* SS_MULTIPLE_PROCS */

#define SS_MAX_ENT                      255
#define SS_MAX_INST                     8

/* mt028.201: modification: multiple procs support related changes */
#ifndef SS_MULTIPLE_PROCS
#define SS_MAX_TTSKS                    100

#ifndef SS_MULTICORE_SUPPORT
#define SS_MAX_STSKS                    7
#else
/* mt001.301 : Additions */
#ifdef  SPLIT_RLC_DL_TASK
#ifdef L2_L3_SPLIT
#define SS_MAX_STSKS                    4 
#else
#define SS_MAX_STSKS                    5 
#endif
#else 
#ifndef INTEL_WLS_MEM
#define SS_MAX_STSKS                    8
#else
#define SS_MAX_STSKS                    7 
#endif
#endif 
#endif /* SS_MULTICORE_SUPPORT */

#else /* SS_MULTIPLE_PROCS */
#define SS_MAX_TTSKS                    1000 
#define SS_MAX_STSKS                    7
#endif /* SS_MULTIPLE_PROCS */

#ifdef SS_DRVR_SUPPORT
#define SS_MAX_DRVRTSKS                 70
#endif

#ifdef SS_RTR_SUPPORT
   /* mt0012.21 : Addition */
#define SS_MAX_RTRTSKS                  255
#endif


/* timer related */
#define SS_MAX_TMRS                     45

/* memory related */
/* mt022.201 - Modification for shared memory relay and memcal regions */
#ifdef SS_MULTICORE_SUPPORT
#define SS_MAX_REGS SS_MAX_STSKS
#else
#define SS_MAX_REGS  7
#endif

#ifdef CMM_MAX_BKT_ENT
#define SS_MAX_POOLS_PER_REG            CMM_MAX_BKT_ENT
#else
#define SS_MAX_POOLS_PER_REG            5
#endif
/* mt001.301 : Additions */
#ifdef SS_WATCHDOG
#define ENTDW                           0xcd
#define ENTHB                           0xce
#define INST0                           0x00
#define SS_TMR_HRTBT                    0x00
#define EVTSSHRTBTREQ                   0x00
#define EVTSSHRTBTRSP                   0x01
#define SS_LOOSE_COUPLING               0x00
#endif /* SS_WATCHDOG */

/* locks */
#define SS_STSKTBL_LOCK                 SS_LOCK_MUTEX
#define SS_STSKENTRY_LOCK               SS_LOCK_MUTEX
#define SS_TMRTBL_LOCK                  SS_LOCK_MUTEX
#define SS_DMNDQ_LOCK                   SS_LOCK_MUTEX
#define SS_DRVRENTRY_LOCK               SS_LOCK_MUTEX
#define SS_RTRENTRY_LOCK                SS_LOCK_MUTEX

#ifdef TENB_T2K3K_SPECIFIC_CHANGES
#define MIPS_FILE "/sys/devices/virtual/mmonitor/mmonitor/mips_stat"
#define GET_CPU_MAX(x, y) ((x) >= (y) ? (x) : (y))
#define MIPS_STRING_LEN    24
#endif


/* types needed by common SSI code */
#define SsSemaId                        sem_t
#define SLockId                         pthread_mutex_t


/* calls needed by common SSI code */
#ifndef TENB_RTLIN_CHANGES
#define SInitLock(l, t)                 pthread_mutex_init(l, NULL)
#endif
/*uint32_t gt[128]; */
/*#define SLock(l)                        (((gt[0x000000FF
 * &((uint32_t)pthread_self())]=MacGetTick())&&pthread_mutex_lock(l)&&MLogTask(30340, RESOURCE_LINL2, gt[0x000000FF
 * &((uint32_t)pthread_self())], MacGetTick()))?0:0)*/
#define SLock(l)                        pthread_mutex_lock(l)
#define SUnlock(l)                      pthread_mutex_unlock(l)
#define SDestroyLock(l)                 pthread_mutex_destroy(l)

#define ssInitSema(s, c)                sem_init(s, 0, c)
#define ssWaitSema(s)                   sem_wait(s)
#define ssPostSema(s)                   sem_post(s)
#define ssDestroySema(s)                sem_destroy(s)

#define SS_CHECK_CUR_STSK(t)            (pthread_equal(pthread_self(), \
                                          (t)->dep.tId))
 /* mt013.21: Addition */
#define SInitSemaphore(s, c)            sem_init(s, 0, c)
#define SWaitSemaphore(s)               sem_wait(s)
#define SPostSemaphore(s)               sem_post(s)
#define SDestroySemaphore(s)            sem_destroy(s)

#define ssdPstTsk(p, m, t)

/* added SExit() for exiting process : mt017.21 */
/* mt001.301 : Additions */
#ifndef SS_LOGGER_SUPPORT
#define SExit()                         exit(0)
#else
#define SExit()  \
{ \
SCleanUp(); \
exit(0); \
}
#endif /* SS_LOGGER_SUPPORT  */

  /* mt007.21 addition */
/* calls needed by Message Functions */
#define SMemCpy(d,s,c)	memcpy(d,s,c)
#define SMemSet(s,c,n)  memset(s,c,n)
/* --- internal to MTSS-Solaris --- */


/*mt041.201 Value of MT_TICK_CNT changed*/
/*mt004.301- defining the MT_TICK_CNT in Micro seconds (usecs) */
/* mt010.301  Removed #ifdef SS_FAP portion and enabled oroginal code */
#define MT_TICK_CNT             (((uint32_t)0x0F4240)/SS_TICKS_SEC)

#define MT_MAX_TICK_CNT_VAL     35
#define MT_MIN_TICK_CNT_VAL     1


/* interrupt service flags */
#define MT_IS_SET               0
#define MT_IS_UNSET             1
#define MT_IS_RESET             2

/******************************************************************
 mt018.201 - Memory Configuration.

Memory block sizes and counts for memory manager configuration
There is no restriction in the size of each block for the bucket.
However, it is recommended that the bucket size should be word aligned.
The CMM (Common Memory Manager) also create a look up table which map
the size to the bucket index.
The number of entry in the lookup table  (CMM_MAX_MAP_ENT, defined in
cm_mem.h) = ((maximum bucket size)/(bucket quantum size)) + 1.
The CMM_MAX_MAP_ENT should be changed depending on the bucket sizes
that are configured below.
*******************************************************************/

/* Bucket 0 configuration */
/* mt032.201 changed  MT_BKT_0_DSIZE from 120 to 128 for 64k compilation */
#define MT_BKT_0_DSIZE          256
/*mt004.301-changed for FAP*/
#ifndef SS_FAP
#ifdef XEON_SPECIFIC_CHANGES
#define MT_BKT_0_NUMBLKS        5248 /* 10500 Modified from 3500 to 10500 */
#else
#ifdef SS_USE_WLS_MEM
#define MT_BKT_0_NUMBLKS        200704 /* 10500 Modified from 3500 to 10500 */
#else
#define MT_BKT_0_NUMBLKS        10000 /* 10500 Modified from 3500 to 10500 */
#endif
#endif
#else
#define MT_BKT_0_NUMBLKS        10000
#endif

/* Bucket 1 configuration */
/*mt004.301-changed for FAP*/
#ifndef SS_FAP
#ifdef XEON_SPECIFIC_CHANGES
#define MT_BKT_1_DSIZE          1024  /* Modified from 256 to 4096 */
#else
#define MT_BKT_1_DSIZE          1280  /* Modified from 256 to 4096 */
#endif
#ifdef SS_USE_WLS_MEM
#define MT_BKT_1_NUMBLKS        310720 /* 1000*/
#else
#define MT_BKT_1_NUMBLKS        10496 /* 1000*/
#endif
#else
/*mt010.301*/
#define MT_BKT_1_DSIZE          256
#define MT_BKT_1_NUMBLKS        2000
#endif

/* Bucket 2 configuration */
/*mt010.301*/
#ifdef SS_FAP
#define MT_BKT_2_DSIZE      1376      /* Fill in this value as required */
#define MT_BKT_2_NUMBLKS    4000        /* Fill in this value as required */
#else
#ifdef XEON_SPECIFIC_CHANGES
#define MT_BKT_2_DSIZE   1768 /* 1664 2048 */
#define MT_BKT_2_NUMBLKS 5248 /* 10496 */
#else
#define MT_BKT_2_DSIZE   2048 /* 1664 2048 */
#define MT_BKT_2_NUMBLKS 2000 /* 10496 */
#endif
#endif


/* Bucket 3 configuration */
/*mt010.301*/
#ifdef SS_FAP
#define MT_BKT_3_DSIZE     2592      /* Fill in this value as required */
#define MT_BKT_3_NUMBLKS   1500     /* Fill in this value as required */
#else
#ifdef XEON_SPECIFIC_CHANGES
#define MT_BKT_3_DSIZE     4224      /* Fill in this value as required */
#define MT_BKT_3_NUMBLKS   5248 /*10496 */       /* Fill in this value as required */
#else
#define MT_BKT_3_DSIZE     12200      /* Fill in this value as required */
#define MT_BKT_3_NUMBLKS   1000 /*10496 */       /* Fill in this value as required */
#endif
#endif

#define MT_BKT_4_DSIZE   65000
#define MT_BKT_4_NUMBLKS 1000

/* For Non-Sharable regions/static regions */
#ifdef XEON_SPECIFIC_CHANGES
#define MT_BKT_0_STATIC_NUMBLKS   500000     /* Fill in this value as required */
#define MT_BKT_1_STATIC_NUMBLKS   500000     /* Fill in this value as required */
#define MT_BKT_2_STATIC_NUMBLKS   200000     /* Fill in this value as required */
#define MT_BKT_3_STATIC_NUMBLKS   40960     /* Fill in this value as required */
#define MT_BKT_4_STATIC_NUMBLKS   4096     /* Fill in this value as required */
#else
#define MT_BKT_0_STATIC_NUMBLKS   75000     /* Fill in this value as required */
#define MT_BKT_1_STATIC_NUMBLKS   15000     /* Fill in this value as required */
#define MT_BKT_2_STATIC_NUMBLKS   500     /* Fill in this value as required */
#define MT_BKT_3_STATIC_NUMBLKS   1600     /* Fill in this value as required */
#define MT_BKT_4_STATIC_NUMBLKS   1000     /* Fill in this value as required */
#endif
/*mt010.301*/
#ifdef RGL_SPECIFIC_CHANGES
#define MT_MAX_BKTS             5
#else
#define MT_MAX_BKTS             5
#endif

/* mt029.201 corrected typos */
/* memory pool data size definitions for pool-to-size mapping table */
#define MT_POOL_4_DSIZE        (MT_BKT_4_DSIZE-(sizeof(SsMblk)+sizeof(SsDblk)))
#define MT_POOL_3_DSIZE        (MT_BKT_3_DSIZE-(sizeof(SsMblk)+sizeof(SsDblk)))
#define MT_POOL_2_DSIZE        (MT_BKT_2_DSIZE-(sizeof(SsMblk)+sizeof(SsDblk)))
#define MT_POOL_1_DSIZE        (MT_BKT_1_DSIZE-(sizeof(SsMblk)+sizeof(SsDblk)))
#define MT_POOL_0_DSIZE        (MT_BKT_0_DSIZE-(sizeof(SsMblk)+sizeof(SsDblk)))



/* mt026.201 - Modification to increase default heap size */
/* memory size used for heap by the memory manager (2MB) */
/* mt001.301 : Additions */
/*mt004.301- changed heap size for FAP */
#ifdef SS_FAP
#define MT_HEAP_SIZE            5242880U
#else
#ifdef XEON_SPECIFIC_CHANGES
#define MT_HEAP_SIZE            6194304U
#else
#define MT_HEAP_SIZE            1004800U /*PAL FIX: 4MB to 6MB flexran */
#endif
#endif

#ifndef USE_MEMCAL
#define STATIC_MEM_CFG
#endif

/* mt022.201 - definition of MT_BKTQNSIZE */
/*mt010.301*/
#define MT_BKTQNSIZE 128


/* mt021.201 - Addition for setting stack size for threads */
/* Configuration for stack size (in bytes) of spawned threads
 * Size of zero gives default of 1 MB or 2 MB for 32 bit or 64 bit
 * compilers, respectively */
#define MT_TASK_STACK		NULLD	/* stack size for task handler */
#define MT_ISTASK_STACK		NULLD	/* stack size for IS task handler */
#define MT_TMRTASK_STACK	NULLD	/* stack size for timer handler */
#define MT_CONSOLE_STACK	NULLD	/* stack size for console handler */

/* mt001.301 : Additions Logger support */
#ifdef SS_LOGGER_SUPPORT
#ifndef SS_MAX_LOGBUF_SIZE
#define SS_MAX_LOGBUF_SIZE 8192
#endif
#ifndef SS_MAX_PATH
#define SS_MAX_PATH 1024
#endif
#endif /* SS_LOGGER_SUPPORT */
/* mt005.301: Cavium changes: */
#ifdef SS_SEUM_CAVIUM
/* Cavium related hashdefines */
#define SS_CVMX_GRP_MASK   (1 << osCp.procId)

#define SS_CVMX_NUM_PKT_BUFFERS   0
#define SS_CVMX_NUM_WQE_BUFFERS   100
#define SS_CVMX_NUM_PKO_BUFFERS   0
#define SS_CVMX_NUM_TIM_BUFFERS   1000
#define SS_CVMX_NUM_DFA_BUFFERS   0

/* fpa pool 0 configuration */
#define SS_CVMX_POOL_0_NUMBLKS        3000

/* fpa pool 1 configuration */
#define SS_CVMX_POOL_1_NUMBLKS        100

/* fpa pool 2 configuration */
#define SS_CVMX_POOL_2_NUMBLKS        10        /* Fill in this value as required */

/* fpa pool 3 configuration */
#define SS_CVMX_POOL_3_NUMBLKS        2500      /* Fill in this value as required */

#define SS_CVMX_TICK_TIME   1000  /* the value is in micorsec */
#define SS_CVMX_MAX_TICKS   1000
#define SS_CVMX_TICKS       500

#define SS_CVMX_TMR_TAG     1000
#define SS_CVMX_MBUF_TAG    1001
#endif /* SS_SEUM_CAVIUM */

/* mt011.301: Pool definition for both 64 bit and 32 bit Cavium */
#ifndef SS_CVMX_WQE_POOL
#ifdef BIT_64
#define SS_CVMX_WQE_POOL CVMX_FPA_WQE_POOL
#else
#define SS_CVMX_WQE_POOL SS_CVMX_POOL_0
#endif
#endif

#define WLS_MEM_SIZE  0x7ec00000 /* Total size of WLS memory configured */

#endif  /*  __MTSSH__  */



/********************************************************************30**

         End of file
**********************************************************************/
