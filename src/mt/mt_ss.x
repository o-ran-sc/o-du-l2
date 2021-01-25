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

     Desc:     Data structure definitions demanded by systems services.
               The contents of these are for the MTSS implementation.

     File:     mt_ss.x

*********************************************************************21*/


#ifndef __MTSSX__
#define __MTSSX__

/* mt001.301: Addition */
#ifdef SS_LOGGER_SUPPORT
/* #define __USE_POSIX */
#include <limits.h>
#include <netinet/in.h>
#endif /* SS_LOGGER_SUPPORT  */

/* TAPA task entry doesn't need anything extra for MTSS */
typedef S8              SsdTTskEntry;


/* System task entry */
typedef struct ssdSTskEntry
{
   pthread_t            tId;
   Ent                  ent;
   Inst                 inst;
   SLockId              lock;

#ifdef SS_MULTICORE_SUPPORT
   uint32_t                  lwpId;
#endif /* SS_MULTICORE_SUPPORT */
} SsdSTskEntry;


#ifdef SS_DRVR_SUPPORT
/* Driver task entry */
typedef struct ssdDrvrTskEntry
{
   Bool flag;

} SsdDrvrTskEntry;
#endif


/* timer entry--MTSS uses common timers */
typedef struct ssdTmrEntry
{
   CmTimer              timers[TMR_DEF_MAX];

} SsdTmrEntry;


/* dynamic pool doesn't need anything extra for MTSS */
typedef S8              SsdDPoolEntry;


/* static pool -- information for the memory management scheme */
typedef S8              SsdSPoolEntry;


/* generic pool doesn't need anything extra for MTSS */
typedef S8              SsdPoolEntry;


/* region doesn't need anything extra for MTSS */
typedef S8              SsdRegionEntry;


/* system services control point--tick count, timer implementation,
 *   console implementation, IS task implementation
 */
typedef struct ssdOs
{
   unsigned     randSeed;               /* random number generator seed */

   Ticks        sysTicks;               /* elapsed system ticks */

   pthread_t    tmrHdlrTID;             /* timer handler thread */
   CmTqCp       tmrTqCp;                /* common timer control point */
   CmTqType     tmrTq[SS_MAX_TMRS];     /* common timer queue */

   sem_t        ssStarted;              /* posted when SS completes init */
#ifdef CONAVL
   FILE         *conInFp;               /* console input file pointer */
   FILE         *conOutFp;              /* console output file pointer */
   pthread_t    conHdlrTID;             /* console handler thread ID */
#endif

#ifndef NOFILESYS
   FILE         *fileOutFp;             /* output file pointer */
#endif

#ifdef SS_DRVR_SUPPORT
   pthread_t    isTskHdlrTID;           /* IS task handler thread ID */
   int          isFildes[2];            /* pipe for SSetIntPend to isTskHdlr */
#endif
   Bool         sigEvnt;                /*mt010.301 Flag to check interupt signal(SIGINT)*/

} SsdOs;

/* mt018.201 - added for memory configuration */
typedef struct mtBktCfg
{
   Size         blkSize;                /* bucket quantum size */
   uint32_t          numBlks;                /* the total blocks in the bucket */
} MtBktCfg;

typedef struct mtRegCfg
{
   Region       regionId;
   uint16_t          numBkts;
   Size         heapsize;
   MtBktCfg     bkt[SS_MAX_POOLS_PER_REG];
} MtRegCfg;

typedef struct mtMemCfg
{
   uint8_t           numRegions;
   MtRegCfg     region[SS_MAX_REGS];
} MtMemCfg;


/* mt003.301 Readwrite lock additions */
#ifdef SS_LOCK_SUPPORT
typedef struct sLockInfo
{
  union
  {
#ifdef SS_RDWR_LOCK_SUPPORT
   pthread_rwlock_t rdWrLockId;
#endif /* SS_RDWR_LOCK_SUPPORT */
#ifdef SS_REC_LOCK_SUPPORT
	pthread_mutex_t  recurLock;
#endif /* SS_REC_LOCK_SUPPORT */
  }l;
}SLockInfo;
#endif /* SS_LOCK_SUPPORT */

uint32_t gt[128];
#endif  /*  __MTSSX__  */



/********************************************************************30**

         End of file
**********************************************************************/
