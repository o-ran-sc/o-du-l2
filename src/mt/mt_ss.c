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

     Name:     Multi-threaded System Services - Solaris

     Type:     C source file

     Desc:     C source code for the MTSS-Solaris implementation of
               System Services.

     File:     mt_ss.c

*********************************************************************21*/



/* POSIX includes */
#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE         199309L
#endif
/* mt003.301 moved env files to use the __USE_UNIX98 flag in sys includes */
#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */

#include <fcntl.h>
#include <pthread.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>
#include <sys/mman.h>
 /* mt003.301: included sys/time.h
  * for both solaris and linux
  * */
#include <sys/time.h>
 /* mt008.21: addition */
#include <errno.h>



/* header include files (.h) */


#include "gen.h"           /* general layer */
#include "ssi.h"           /* system services */

#include "cm5.h"           /* common timers */

#include "mt_ss.h"         /* MTSS specific */
#include "mt_err.h"        /* MTSS error defines */

#include "ss_queue.h"      /* queues */
#include "ss_task.h"       /* tasking */
#include "ss_msg.h"        /* messaging */
#include "ss_mem.h"        /* memory management interface */
#include "ss_gen.h"        /* general */
/* mt003.301 Additions - Task deregistration */
#include "ss_err.h"        /* error */
#include "cm_mem.h"        /* common memory manager */
#include "cm_lte.h"        /* common lte param */
/* mt001.301 : Additions */
#ifdef SS_THREAD_PROFILE
#include "ss_err.h"
#endif
#ifdef SS_LOCKLESS_MEMORY
#include "cm_llist.h"
#include "cm_hash.h"
#endif

/* multi-core support enhancement */
/*mt013.301 :Added SS_AFFINITY_SUPPORT  */
#if defined(SS_MULTICORE_SUPPORT) ||defined(SS_AFFINITY_SUPPORT)
#ifdef SS_LINUX
#include <sched.h>
#include <execinfo.h>
#else
#ifdef SUNOS
#include <sys/types.h>
#include <sys/processor.h>
#include <sys/procset.h>
#endif /* SUNOS */
#endif /* SS_LINUX */
#endif /* SS_MULTICORE_SUPPORT || SS_AFFINITY_SUPPORT */
/* mt001.301 : Additions */
#ifdef SS_WATCHDOG
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif /* SS_WATCHDOG */

/* header/extern include files (.x) */

#include "gen.x"           /* general layer */
#include "ssi.x"           /* system services */

#include "cm5.x"           /* common timers */

#include "mt_ss.x"         /* MTSS specific */
#ifdef SS_LOCKLESS_MEMORY
#include "mt_ss_wl.x"         /* MTSS specific */
#endif /* SS_LOCKLESS_MEMORY */

#include "ss_queue.x"      /* queues */
#include "ss_task.x"       /* tasking */
#include "ss_timer.x"      /* timers */
#include "ss_strm.x"       /* STREAMS */
#include "ss_msg.x"        /* messaging */
#include "ss_mem.x"        /* memory management interface */
#include "ss_drvr.x"       /* driver tasks */
#include "ss_gen.x"        /* general */
#ifdef SS_LOCKLESS_MEMORY
#include "cm_llist.x"
#include "cm_hash.x"
#include "cm_mem_wl.x"        /* common memory manager */
#else
#include "cm_mem.x"        /* common memory manager */
#endif /* SS_LOCKLESS_MEMORY */
#include "cm_lte.x"        /* common memory manager */
/* mt001.301 : Additions */
#ifdef SS_LOGGER_SUPPORT
#include "cm_lib.x"
#endif /*  SS_LOGGER_SUPPORT */

/*mt005.301: Cavium Changes */
#ifdef SS_SEUM_CAVIUM
/* cvmx includes files */
#include "cvmx-config.h"
#include "cvmx.h"
#include "cvmx-pow.h"
#include "cvmx-tim.h"
#include "cvmx-fpa.h"
#include "cvmx-helper-fpa.h"
#include "cvmx-malloc.h"
#endif /* SS_SEUM_CAVIUM */

#ifdef L2_L3_SPLIT
#include "mt_plat_t33.h"
#include "mt_plat_t33.x"
#include "sys/syscall.h"
#endif

#ifdef RGL_SPECIFIC_CHANGES
#include <wls_lib.h>
#include <hugetlbfs.h>
#endif

#if defined(SPLIT_RLC_DL_TASK) && defined(RLC_MAC_STA_RSP_RBUF)
EXTERN S16 rgBatchProc (Void);
#endif
#ifdef RLC_MAC_DAT_REQ_RBUF
EXTERN S16 rgDlDatReqBatchProc ARGS(( 
         Void));
#endif
#if defined(SPLIT_RLC_DL_TASK) && defined(RLC_MAC_STA_RSP_RBUF)
EXTERN S16 rgBatchProc ARGS((
    Void));
#endif  

#ifdef  TENB_T2K3K_SPECIFIC_CHANGES
/* general purpose debug zone  */
char            my_buffer2[4096 * 4] = { 0 };
char            my_buffer[4096] = { 0 };
int             my_buffer_idx = 0;



#define sigsegv_print(x, ...)    my_buffer_idx += sprintf(&my_buffer[my_buffer_idx], x "\n", ##__VA_ARGS__)

struct sigcontext my_uc_mcontext = { 0 };


#include <unistd.h>
#include <signal.h>
#include <ucontext.h>
#include <dlfcn.h>


#define SIGSEGV_STACK_GENERIC
#define REGFORMAT "%x\n"

#ifdef XEON_SPECIFIC_CHANGES
Void cmPrcTmr ARGS((CmTqCp* tqCp, CmTqType* tq, PFV func));
#endif

void            dump_external(void);

#ifdef ANSI
PRIVATE Void mtDelSigals
(
Void
)
#else
PRIVATE Void mtDelSignals()
#endif
{
   struct sigaction sa;

   memset(&sa, 0, sizeof(struct sigaction));
   sigemptyset(&sa.sa_mask);
   sa.sa_handler = SIG_DFL;
   sigaction(SIGSEGV, &sa, NULL);

   memset(&sa, 0, sizeof(struct sigaction));
   sigemptyset(&sa.sa_mask);
   sa.sa_handler = SIG_DFL;
   sigaction(SIGILL, &sa, NULL);

   RETVOID;
}
static void signal_segv(int signum, siginfo_t * info, void *ptr)
{
    static const char *si_codes[3] = { "", "SEGV_MAPERR", "SEGV_ACCERR" };
    int             sz;
    int             i;
    ucontext_t     *ucontext = (ucontext_t *) ptr;
#ifdef XEON_SPECIFIC_CHANGES
#else
    int            *p32 = (int *) 0x2fff0000;
#endif
    void           *buffer[100];
    char          **strings;

    printf("segv ooops @ %p\n", info->si_addr);
    my_buffer2[0] = 1;

    printf("Segmentation Fault!\n");
    printf("info.si_signo = %d\n", signum);
    printf("info.si_errno = %d\n", info->si_errno);
    printf("info.si_code  = %d (%s)\n", info->si_code, si_codes[info->si_code]);
    printf("info.si_addr  = %p\n", info->si_addr);

    memcpy(&my_uc_mcontext, &ucontext->uc_mcontext, sizeof(struct sigcontext));

    i = 0;
#ifndef RGL_SPECIFIC_CHANGES
    printf("reg[%02d]       = 0x" REGFORMAT, i++, (unsigned int)ucontext->uc_mcontext.arm_r0);
    printf("reg[%02d]       = 0x" REGFORMAT, i++, (unsigned int)ucontext->uc_mcontext.arm_r1);
    printf("reg[%02d]       = 0x" REGFORMAT, i++, (unsigned int)ucontext->uc_mcontext.arm_r2);
    printf("reg[%02d]       = 0x" REGFORMAT, i++, (unsigned int)ucontext->uc_mcontext.arm_r3);
    printf("reg[%02d]       = 0x" REGFORMAT, i++, (unsigned int)ucontext->uc_mcontext.arm_r4);
    printf("reg[%02d]       = 0x" REGFORMAT, i++, (unsigned int)ucontext->uc_mcontext.arm_r5);
    printf("reg[%02d]       = 0x" REGFORMAT, i++, (unsigned int)ucontext->uc_mcontext.arm_r6);
    printf("reg[%02d]       = 0x" REGFORMAT, i++, (unsigned int)ucontext->uc_mcontext.arm_r7);
    printf("reg[%02d]       = 0x" REGFORMAT, i++, (unsigned int)ucontext->uc_mcontext.arm_r8);
    printf("reg[%02d]       = 0x" REGFORMAT, i++, (unsigned int)ucontext->uc_mcontext.arm_r9);
    printf("reg[%02d]       = 0x" REGFORMAT, i++, (unsigned int)ucontext->uc_mcontext.arm_r10);
    printf("reg[%02d]       = 0x" REGFORMAT, i++, (unsigned int)ucontext->uc_mcontext.arm_fp);
    printf("reg[%02d]       = 0x" REGFORMAT, i++, (unsigned int)ucontext->uc_mcontext.arm_ip);
    printf("reg[sp]       = 0x" REGFORMAT, (unsigned int)ucontext->uc_mcontext.arm_sp);
    printf("reg[lr]       = 0x" REGFORMAT, (unsigned int)ucontext->uc_mcontext.arm_lr);
    printf("reg[pc]       = 0x" REGFORMAT, (unsigned int)ucontext->uc_mcontext.arm_pc);
    printf("reg[cpsr]     = 0x" REGFORMAT, (unsigned int)ucontext->uc_mcontext.arm_cpsr);
#endif

    printf("Stack trace (non-dedicated):\n");

    sz = backtrace(buffer, 50);
    strings = backtrace_symbols(buffer, sz);
    for (i = 0; i < sz; ++i)
        printf("%s\n", strings[i]);

    printf("End of stack trace.");

#ifdef XEON_SPECIFIC_CHANGES
#else
    p32[0] = 1;
#endif

    /* Lets first print our debug information */
    printf("Before dumping our Debug info\n");
    mtStopHndlr();
    printf("After dumping our Debug info\n");

    /* Disable the signal and make the enodeb to dump. This will make
     * eNB to generate the core with dumping the ccpu log
     */
    {
       char           *abc = NULL;
       mtDelSigals();
       *abc = 100;
    }
    /* End printing debug information */
    exit(1);
    /*while (1);*/
}
#endif
/*** TBD: IMPORTANT ***
 *** The following definition is temporary. This must be removed
 *** when all products have been updated with latest ssi.h file OR
 *** all ssi.h files have been updated to contain this definitions
 ***/
/* New error class for FTHA added */
#ifndef ERRCLS_FTHA
#define ERRCLS_FTHA    0x8
#endif /* ERRCLS_FTHA */

typedef struct _SPThreadCreateArg
{
   void *argument; /* argument that is to be passed to the actual pthread */
   void *(*start_routine) (void *); /* function from which pthread starts */   
}SPThreadCreateArg;

PUBLIC void *pthreadCreateHdlr(void*  arg);

#ifdef SS_LOCKLESS_MEMORY
PUBLIC Buffer *mtTskBuffer1;
PUBLIC Buffer *mtTskBuffer2;

EXTERN pthread_t tmpRegTidMap[20];
EXTERN U8 stopBtInfo;
EXTERN  S16 SGlobMemInfoShow(void);
#endif /* SS_LOCKLESS_MEMORY */

#ifdef L2_L3_SPLIT
EXTERN APP_CONTEXT AppContext;
EXTERN S32 clusterMode;
#endif

#ifdef TENB_T2K3K_SPECIFIC_CHANGES
EXTERN unsigned int tlPost(void *handle);
#endif

/* forward references */
/* mt003.301 Modifications - Moved to ss_gen.x */
#ifdef TENB_T2K3K_SPECIFIC_CHANGES
PUBLIC Void *mtTskHdlrT2kL2 ARGS((Void*));
PUBLIC  void mtSigSegvHndlr ARGS((void));
PUBLIC  void mtSigUsr2Hndlr ARGS((void));
#endif

PRIVATE S16 ssdSetPthreadAttr ARGS ((S32 tskPrior, pthread_attr_t *attr));
PRIVATE Void *mtTskHdlr ARGS((void *));
PRIVATE S16 mtTskHdlMsg ARGS((SsSTskEntry *sTsk));

PRIVATE Void *mtTmrHdlr ARGS((void *));
PRIVATE Void mtTimeout ARGS((PTR tCb, S16 evnt));

/*mt010.301 Fix for core when run with -o option and when killed with SIGINT*/
PRIVATE Void mtIntSigHndlr ARGS((int));
PRIVATE Void mtExitClnup ARGS((void));

#ifdef CONAVL
PRIVATE Void *mtConHdlr ARGS((void *));
#endif

#ifndef L2_L3_SPLIT
#ifdef SS_DRVR_SUPPORT
PRIVATE Void *mtIsTskHdlr ARGS((void *));
#endif
#endif

/* mt020.201 - Addition for no command line available */
#ifndef NOCMDLINE
PRIVATE Void mtGetOpts ARGS((void));
/* mt003.301 Additions - File Based task registration made
 * common for both MULTICORE and NON-MULTICORE
 */
PRIVATE Bool fileBasedMemCfg = FALSE;
#endif

/* mt033.201 - addition of local function to print the statistics such as
* (size vs. numAttempts) and (allocations vs. deallocations)
*/
#ifdef SSI_DEBUG_LEVEL1
PRIVATE S16 SPrintRegMemStats ARGS((Region region));
#endif /* SSI_DEBUG_LEVEL1 */

#ifdef SS_MULTICORE_SUPPORT
PRIVATE SsSTskEntry* ssdAddTmrSTsk(Void);
PRIVATE SsSTskEntry* ssdReAddTmrSTsk ARGS((U8 idx));
#ifndef SS_LOCKLESS_MEMORY
#ifndef RGL_SPECIFIC_CHANGES
PRIVATE S16 ssdInitMemInfo ARGS((void));
#endif
#endif
#endif

/* mt005.301: Cavium changes */
#ifdef SS_SEUM_CAVIUM
PRIVATE Void *workRcvTsk ARGS((void *));
#endif /* SS_SEUM_CAVIUM */

#ifdef SS_THR_REG_MAP
PUBLIC S32 ssCheckAndAddMemoryRegionMap ARGS((pthread_t  threadId,
                                               Region region));
PUBLIC S32 ssCheckAndDelMemoryRegionMap ARGS((pthread_t  threadId));
#endif /* SS_THR_REG_MAP */

/* type declarations */

#ifdef SS_DRVR_SUPPORT
typedef struct mtIsFlag
{
   U16 id;
   U8 action;

} MtIsFlag;
#endif



/* public variable declarations */

PUBLIC Cntr cfgNumRegs = SS_MAX_REGS;
/* Set memory configuration as false.
 * Set to true if memory configuration through file is successfull.
 */
PUBLIC Bool memConfigured = FALSE;
/* mt022.201 - Modification for shared memory relay region and memcal tool */
PUBLIC SsRegCfg cfgRegInfo[SS_MAX_REGS] =
{
   {
      SS_DFLT_REGION, SS_MAX_POOLS_PER_REG - 1,
      {
         { SS_POOL_DYNAMIC, MT_POOL_0_DSIZE },
         { SS_POOL_DYNAMIC, MT_POOL_1_DSIZE },
         { SS_POOL_DYNAMIC, MT_POOL_2_DSIZE },
         { SS_POOL_DYNAMIC, MT_POOL_3_DSIZE },
         { SS_POOL_STATIC, 0 }
      }
   }
#ifdef INTEL_WLS
   ,
   {
      SS_DFLT_REGION + 1, SS_MAX_POOLS_PER_REG - 1,
      {
         { SS_POOL_DYNAMIC, MT_POOL_0_DSIZE },
         { SS_POOL_DYNAMIC, MT_POOL_1_DSIZE },
         { SS_POOL_DYNAMIC, MT_POOL_2_DSIZE },
         { SS_POOL_DYNAMIC, MT_POOL_3_DSIZE },
         { SS_POOL_STATIC, 0 }
      }
   }
#endif /* INTEL_WLS */

#ifdef SS_LOCKLESS_MEMORY
   ,
   {
      SS_DFLT_REGION + 1, SS_MAX_POOLS_PER_REG - 1,
      {
         { SS_POOL_DYNAMIC, MT_POOL_0_DSIZE },
         { SS_POOL_DYNAMIC, MT_POOL_1_DSIZE },
         { SS_POOL_DYNAMIC, MT_POOL_2_DSIZE },
         { SS_POOL_DYNAMIC, MT_POOL_3_DSIZE },
         { SS_POOL_STATIC, 0 }
      }
   },
   {
      SS_DFLT_REGION + 2, SS_MAX_POOLS_PER_REG - 1,
      {
         { SS_POOL_DYNAMIC, MT_POOL_0_DSIZE },
         { SS_POOL_DYNAMIC, MT_POOL_1_DSIZE },
         { SS_POOL_DYNAMIC, MT_POOL_2_DSIZE },
         { SS_POOL_DYNAMIC, MT_POOL_3_DSIZE },
         { SS_POOL_STATIC, 0 }
      }
   },
    {
      SS_DFLT_REGION + 3, SS_MAX_POOLS_PER_REG - 1,
      {
         { SS_POOL_DYNAMIC, MT_POOL_0_DSIZE },
         { SS_POOL_DYNAMIC, MT_POOL_1_DSIZE },
         { SS_POOL_DYNAMIC, MT_POOL_2_DSIZE },
         { SS_POOL_DYNAMIC, MT_POOL_3_DSIZE },
         { SS_POOL_STATIC, 0 }
      }
   }, 
    {
      SS_DFLT_REGION + 4, SS_MAX_POOLS_PER_REG - 1,
      {
         { SS_POOL_DYNAMIC, MT_POOL_0_DSIZE },
         { SS_POOL_DYNAMIC, MT_POOL_1_DSIZE },
         { SS_POOL_DYNAMIC, MT_POOL_2_DSIZE },
         { SS_POOL_DYNAMIC, MT_POOL_3_DSIZE },
         { SS_POOL_STATIC, 0 }
      }
   } 
#endif /* SS_LOCKLESS_MEMORY */
};
/* mt003.301 Modifications - File Based task registration made
 * common for both MULTICORE and NON-MULTICORE
 */

#ifdef SS_LOCKLESS_MEMORY
PUBLIC MtDynMemCfg mtDynMemoCfg =
{
  SS_MAX_REGS,                                /* number of regions */
  {
     {
        SS_DFLT_REGION,                         /* region id */
        MT_MAX_BKTS,                            /* number of buckets */
        {
           /* block size, no. of blocks, Upper threshold, lower threshold */
           {SS_BLK_RELEASE_THRESHOLD, SS_BLK_ACQUIRE_THRESHOLD},
           {SS_BLK_RELEASE_THRESHOLD, SS_BLK_ACQUIRE_THRESHOLD},
           {SS_BLK_RELEASE_THRESHOLD, SS_BLK_ACQUIRE_THRESHOLD},
           {SS_BLK_RELEASE_THRESHOLD, SS_BLK_ACQUIRE_THRESHOLD} 
        }
     },
     {
        SS_DFLT_REGION + 1,                         /* region id */
        MT_MAX_BKTS,                            /* number of buckets */
        {
           /* block size, no. of blocks, Upper threshold, lower threshold */
           {SS_BLK_RELEASE_THRESHOLD, SS_BLK_ACQUIRE_THRESHOLD},
           {SS_BLK_RELEASE_THRESHOLD, SS_BLK_ACQUIRE_THRESHOLD},
           {SS_BLK_RELEASE_THRESHOLD, SS_BLK_ACQUIRE_THRESHOLD},
           {SS_BLK_RELEASE_THRESHOLD, SS_BLK_ACQUIRE_THRESHOLD} 
        }
     },
     {
        SS_DFLT_REGION + 2,                         /* region id */
        MT_MAX_BKTS,                            /* number of buckets */
        {
           /* block size, no. of blocks, Upper threshold, lower threshold */
           {SS_BLK_RELEASE_THRESHOLD, SS_BLK_ACQUIRE_THRESHOLD},
           {SS_BLK_RELEASE_THRESHOLD, SS_BLK_ACQUIRE_THRESHOLD},
           {SS_BLK_RELEASE_THRESHOLD, SS_BLK_ACQUIRE_THRESHOLD},
           {SS_BLK_RELEASE_THRESHOLD, SS_BLK_ACQUIRE_THRESHOLD} 
        }
     },
      {
        SS_DFLT_REGION + 3,                         /* region id */
        MT_MAX_BKTS,                            /* number of buckets */
        {
           /* block size, no. of blocks, Upper threshold, lower threshold */
           {SS_BLK_RELEASE_THRESHOLD, SS_BLK_ACQUIRE_THRESHOLD},
           {SS_BLK_RELEASE_THRESHOLD, SS_BLK_ACQUIRE_THRESHOLD},
           {SS_BLK_RELEASE_THRESHOLD, SS_BLK_ACQUIRE_THRESHOLD},
           {SS_BLK_RELEASE_THRESHOLD, SS_BLK_ACQUIRE_THRESHOLD} 
        }
     },
      {
        SS_DFLT_REGION + 4,                         /* region id */
        MT_MAX_BKTS,                            /* number of buckets */
        {
           /* block size, no. of blocks, Upper threshold, lower threshold */
           {SS_BLK_RELEASE_THRESHOLD, SS_BLK_ACQUIRE_THRESHOLD},
           {SS_BLK_RELEASE_THRESHOLD, SS_BLK_ACQUIRE_THRESHOLD},
           {SS_BLK_RELEASE_THRESHOLD, SS_BLK_ACQUIRE_THRESHOLD},
           {SS_BLK_RELEASE_THRESHOLD, SS_BLK_ACQUIRE_THRESHOLD} 
        }
     }
#if ((defined (SPLIT_RLC_DL_TASK)) && (!defined (L2_L3_SPLIT)))
     ,
     {
        SS_DFLT_REGION + 4,                         /* region id */
        MT_MAX_BKTS,                            /* number of buckets */
        {
           /* block size, no. of blocks, Upper threshold, lower threshold */
           {SS_BLK_RELEASE_THRESHOLD, SS_BLK_ACQUIRE_THRESHOLD},
           {SS_BLK_RELEASE_THRESHOLD, SS_BLK_ACQUIRE_THRESHOLD},
           {SS_BLK_RELEASE_THRESHOLD, SS_BLK_ACQUIRE_THRESHOLD},
           {SS_BLK_RELEASE_THRESHOLD, SS_BLK_ACQUIRE_THRESHOLD} 
        }
     }
#endif
  }

};

PUBLIC MtGlobMemCfg mtGlobMemoCfg =
{
   MT_MAX_BKTS,                            /* number of buckets */
   {
#if 1
      /* block size, no. of blocks, Upper threshold, lower threshold */
      {MT_BKT_0_DSIZE, (MT_BKT_0_NUMBLKS + MT_BKT_0_NUMBLKS), SS_DFLT_MEM_BLK_SET_SIZE},
      {MT_BKT_1_DSIZE, MT_BKT_1_NUMBLKS, SS_DFLT_MEM_BLK_SET_SIZE},
      {MT_BKT_2_DSIZE, MT_BKT_2_NUMBLKS, SS_DFLT_MEM_BLK_SET_SIZE},
      {MT_BKT_3_DSIZE, MT_BKT_3_NUMBLKS, SS_DFLT_MEM_BLK_SET_SIZE}
#else
      {1024, 12800 /* MT_BKT_0_NUMBLKS */, SS_DFLT_MEM_BLK_SET_SIZE},
      {1664, 12800 /* MT_BKT_1_NUMBLKS */, SS_DFLT_MEM_BLK_SET_SIZE},
      {4096, 3840 /* MT_BKT_2_NUMBLKS*/, SS_DFLT_MEM_BLK_SET_SIZE},
      {MT_BKT_3_DSIZE, 12800 /* MT_BKT_3_NUMBLKS */, SS_DFLT_MEM_BLK_SET_SIZE}
#endif
   }
};
#endif /* SS_LOCKLESS_MEMORY */

/* mt022.201 - Modification for memory calculator tool */
/* mt018.201 - added memory configuration matrix */
PUBLIC MtMemCfg mtMemoCfg =
{
#ifdef  RY_ENBS5SHM
  SS_MAX_REGS - 1,                            /* number of regions */
#else
#ifndef XEON_SPECIFIC_CHANGES  
  SS_MAX_REGS,                                /* number of regions */
#else
  2,
#endif  
#endif
  {
    {
      SS_DFLT_REGION,                         /* region id */
      MT_MAX_BKTS,                            /* number of buckets */
      MT_HEAP_SIZE,                           /* heap size */
      {
#ifndef XEON_SPECIFIC_CHANGES  
         {MT_BKT_0_DSIZE, MT_BKT_0_STATIC_NUMBLKS},   /* block size, no. of blocks */
         {MT_BKT_1_DSIZE, MT_BKT_1_STATIC_NUMBLKS},    /* block size, no. of blocks */
         {MT_BKT_2_DSIZE, MT_BKT_2_STATIC_NUMBLKS},   /* block size, no. of blocks */
         {MT_BKT_3_DSIZE, MT_BKT_3_STATIC_NUMBLKS}    /* block size, no. of blocks */
#else
         {256, 491520}, /* 60 pages of 2M*/
         {512, 12288},  /* 3 pages of 2M */
         {2048, 99328}, /* 97 Pages of 2M */
         {8192, 75008}, /* 293 Pages of 2M */
         {16384, 4096}  /* 32 pages of 2M */
#endif        
      }
    },
#ifdef INTEL_WLS
#ifndef SS_LOCKLESS_MEMORY    
    {
      SS_DFLT_REGION + 1,                         /* region id */
      MT_MAX_BKTS,                            /* number of buckets */
      /*MT_HEAP_SIZE 7194304 */ 10485760,                           /* heap size */
      {
        //{MT_BKT_0_DSIZE, MT_BKT_0_STATIC_NUMBLKS},   /* block size, no. of blocks */
        //{MT_BKT_1_DSIZE, MT_BKT_1_STATIC_NUMBLKS},    /* block size, no. of blocks */
        //{MT_BKT_2_DSIZE, MT_BKT_2_STATIC_NUMBLKS},   /* block size, no. of blocks */
        //{MT_BKT_3_DSIZE, MT_BKT_3_STATIC_NUMBLKS}    /* block size, no. of blocks */
        {128,   500000},
        {256,   500000},
        {2048,  200000},
        {4096,  40960},
        {18432, 10240}
      }
    },
#endif /* SS_LOCKLESS_MEMORY */
#endif /* INTEL_WLS */
#ifdef SS_LOCKLESS_MEMORY
    {
      SS_DFLT_REGION + 1,                         /* region id */
      MT_MAX_BKTS,                            /* number of buckets */
      MT_HEAP_SIZE,                           /* heap size */
      {
        {MT_BKT_0_DSIZE, MT_BKT_0_STATIC_NUMBLKS},   /* block size, no. of blocks */
        {MT_BKT_1_DSIZE, MT_BKT_1_STATIC_NUMBLKS},    /* block size, no. of blocks */
        {MT_BKT_2_DSIZE, MT_BKT_2_STATIC_NUMBLKS},   /* block size, no. of blocks */
        {MT_BKT_3_DSIZE, MT_BKT_3_STATIC_NUMBLKS}    /* block size, no. of blocks */
      }
    },
    {
      SS_DFLT_REGION + 2,                         /* region id */
      MT_MAX_BKTS,                            /* number of buckets */
      MT_HEAP_SIZE,                           /* heap size */
      {
        {MT_BKT_0_DSIZE, MT_BKT_0_STATIC_NUMBLKS},   /* block size, no. of blocks */
        {MT_BKT_1_DSIZE, MT_BKT_1_STATIC_NUMBLKS},    /* block size, no. of blocks */
        {MT_BKT_2_DSIZE, MT_BKT_2_STATIC_NUMBLKS},   /* block size, no. of blocks */
        {MT_BKT_3_DSIZE, MT_BKT_3_STATIC_NUMBLKS}    /* block size, no. of blocks */
      }
    },
     {
      SS_DFLT_REGION + 3,                         /* region id */
      MT_MAX_BKTS,                            /* number of buckets */
      MT_HEAP_SIZE,                           /* heap size */
      {
        {MT_BKT_0_DSIZE, MT_BKT_0_STATIC_NUMBLKS},   /* block size, no. of blocks */
        {MT_BKT_1_DSIZE, MT_BKT_1_STATIC_NUMBLKS},    /* block size, no. of blocks */
        {MT_BKT_2_DSIZE, MT_BKT_2_STATIC_NUMBLKS},   /* block size, no. of blocks */
        {MT_BKT_3_DSIZE, MT_BKT_3_STATIC_NUMBLKS}    /* block size, no. of blocks */
      }
    },
    {
       SS_DFLT_REGION + 4,                         /* region id */
       MT_MAX_BKTS,                            /* number of buckets */
       MT_HEAP_SIZE,                           /* heap size */
       {
          {MT_BKT_0_DSIZE, MT_BKT_0_STATIC_NUMBLKS},   /* block size, no. of blocks */
          {MT_BKT_1_DSIZE, MT_BKT_1_STATIC_NUMBLKS},    /* block size, no. of blocks */
          {MT_BKT_2_DSIZE, MT_BKT_2_STATIC_NUMBLKS},   /* block size, no. of blocks */
          {MT_BKT_3_DSIZE, MT_BKT_3_STATIC_NUMBLKS}    /* block size, no. of blocks */
       }
    }

#endif /* SS_LOCKLESS_MEMORY */
    STATIC_MEM_CFG
  }
};
/* mt003.301 Modifications - File Based task registration made
 * common for both MULTICORE and NON-MULTICORE
 * bucket info, as different regions may request for different no.
 * of blocks
 */
PUBLIC MtBktCfg mtBktInfo[MT_MAX_BKTS];
PUBLIC S16 msArgc;              /* argc */
PUBLIC Txt **msArgv;            /* argv */
PUBLIC S16 msOptInd;            /* SGetOpt vars */
PUBLIC S8 *msOptArg;            /* SGetOpt vars */


#ifdef INTEL_WLS
typedef struct _MtRegMemSz
{
   U32   reqdSz;
   U8    *startAddr;
}MtRegMemSz;

PRIVATE MtRegMemSz mtRegMemSz[MT_MAX_BKTS+1];
#endif


/* private variable declarations */
/* mt018.201 - change mtCMMRegCfg as array of pointers */
PRIVATE CmMmRegCfg *mtCMMRegCfg[SS_MAX_REGS];
PRIVATE CmMmRegCb *mtCMMRegCb[SS_MAX_REGS];
/* mt003.301 - Fixed compilation warnings */
/*mt004.301-addede new veriable for FAP*/
/*mt010.301 - removed veriable defined for FA*/


#ifdef INTEL_WLS

#ifdef NTL_LIB
void mtSetNtlHdl(unsigned int hdl)
{
    osCp.ntl.hdl =  hdl;
}

unsigned int mtGetNtlHdl()
{
    return(osCp.ntl.hdl);
}

#endif  /* NTL_LIB */

void * mtGetWlsHdl()
{
   RETVALUE(osCp.wls.intf);
}

#ifdef XEON_MULTIPLE_CELL_CHANGES
EXTERN S8 gWrWlsDeviceName[MAX_WLS_DEVICE_NAME_LEN];
EXTERN S16 smWrReadWlsConfigParams (Void);
#endif

PRIVATE int SOpenWlsIntf()
{
   void *hdl;
   #define WLS_DEVICE_NAME "/dev/wls"

#ifdef XEON_SPECIFIC_CHANGES
#ifdef XEON_MULTIPLE_CELL_CHANGES
   hdl = WLS_Open(gWrWlsDeviceName, 1);
#else
   hdl = WLS_Open(WLS_DEVICE_NAME, 1);
#endif
#else
   hdl = WLS_Open(WLS_DEVICE_NAME, 0);
#endif

   osCp.wls.intf = hdl;

   if(!osCp.wls.intf)
   {
     printf("Could not open WLS Interface \n");
     RETVALUE(0);
   }

   RETVALUE(1);
}
#endif

/* mt028.201 */
#ifndef API_MAIN

/*
*
*       Fun:   main
*
*       Desc:  This function is the entry point for the final binary. It
*              calls SInit() in the common code. It can be replaced by a
*              user function if required (SInit() must still be called).
*
*       Ret:   none on success
*              exit on failure
*
*       Notes:
*
*       File:  mt_ss.c
*
*/
#ifdef ANSI
PUBLIC int main
(
int argc,                       /* argument count */
char **argv                     /* argument vector */
)
#else
PUBLIC int main(argc, argv)
int argc;                       /* argument count */
char **argv;                    /* argument vector */
#endif
{
   TRC0(main);

#ifdef XEON_MULTIPLE_CELL_CHANGES
   /* Read the WLS parameters from the file and copy into global control block */
   if(smWrReadWlsConfigParams() != ROK)
   {
      fprintf(stderr, "Failed to read WLS params from file wr_cfg.txt");
      RETVALUE(RFAILED);
   } /* end of if statement */
#endif

#ifdef INTEL_WLS
   if(!SOpenWlsIntf())
    RETVALUE(0);
#endif /* INTEL_WLS */

   msArgc = argc;
   msArgv = argv;
   /* mt003.301 Modifications */
   if( ROK != SInit())
	{
		printf("\n SInit failed, SSI could not start \n");
		/* pthread_exit(NULLP);*/ /* Commented to Come out of Main thread*/

		RETVALUE(0);
	}
   /*mt010.301  cleanup part exposed to user*/
   SFini();
   RETVALUE(0);
}

#else

/*
*
*       Fun:   ssMain
*
*       Desc:  This function is the entry point for the final binary. It
*              calls SInit() in the common code. It can be replaced by a
*              user function if required (SInit() must still be called).
*
*       Ret:   none on success
*              exit on failure
*
*       Notes:
*
*       File:  mt_ss.c
*
*/
#ifdef ANSI
PUBLIC int ssMain
(
int argc,                       /* argument count */
char **argv                     /* argument vector */
)
#else
PUBLIC int ssMain(argc, argv)
int argc;                       /* argument count */
char **argv;                    /* argument vector */
#endif
{
   TRC0(ssMain);


   msArgc = argc;
   msArgv = argv;


   SInit();

   RETVALUE(0);
}

#endif
/*
*  initialization functions
*/

/*
*
*       Fun:   Initialize OS control point
*
*       Desc:  This function initializes MTSS-specific information
*              in the OS control point.
*
*       Ret:   ROK      - ok
*
*       Notes:
*
*       File:  mt_ss.c
*
*/
#ifdef ANSI
PUBLIC S16 ssdInitGen
(
void
)
#else
PUBLIC S16 ssdInitGen()
#endif
{
   struct sigaction act;
   sigset_t set;
#ifdef TENB_T2K3K_SPECIFIC_CHANGES
   struct sigaction sa;
#endif

   TRC0(ssdInitGen);

  /*mt014.301 : 4GMX release related changes*/
#ifdef SS_4GMX_UCORE
   uarmInit();
#endif
/* mt005.301 : Cavium changes */
#ifdef SS_SEUM_CAVIUM
	/* set group mask for the core */
	cvmx_pow_set_group_mask(cvmx_get_core_num(), SS_CVMX_GRP_MASK);
#endif /* SS_SEUM_CAVIUM */

   osCp.dep.sysTicks = 0;

   /* mt020.201 - Addition for no command line available */
#ifndef NOCMDLINE
   /* parse command line */
   mtGetOpts();
	/* mt003.301 Additions */
	if(fileBasedMemCfg == TRUE && memConfigured == FALSE)
	{
		printf("\n File Based Memory configuration failed \n");
		RETVALUE(RFAILED);
	}
#endif

#ifndef RGL_SPECIFIC_CHANGES /* ANOOP :: This ssdInitMemInfo() was present in 2.1 */
#ifndef SS_LOCKLESS_MEMORY
#ifdef SS_MULTICORE_SUPPORT
   if(memConfigured == FALSE)
      ssdInitMemInfo();
#endif
#endif
#endif

   /* initialize the started semaphore */
   if (sem_init(&osCp.dep.ssStarted, 0, 0) != 0)
   {
      RETVALUE(RFAILED);
   }

   /* mt028.201 added compile time flag to allow not to mask signals */
#ifndef UNMASK_SIG
   /* mask all signals in the main thread */
   sigfillset(&set);
   sigdelset(&set, SIGINT);
#ifdef TENB_T2K3K_SPECIFIC_CHANGES
   sigdelset(&set, SIGSEGV);
   sigdelset(&set, SIGUSR2);
   sigdelset(&set, SIGILL);
#ifdef XEON_SPECIFIC_CHANGES
   sigdelset(&set, SIGABRT);
   sigdelset(&set, SIGTERM);
   sigdelset(&set, SIGHUP);
#endif   
#endif
   pthread_sigmask(SIG_SETMASK, &set, NULLP);
#endif /* UNMASK_SIG */

   /* install a SIGINT handler to shutdown */
   /*mt010.301 Fix for core when run with -o option and when killed with SIGINT*/

   /*Initialize SIGSEGV Signal */
#ifdef TENB_T2K3K_SPECIFIC_CHANGES
#if 1
    memset(&sa, 0, sizeof(struct sigaction));
    sigemptyset(&sa.sa_mask);
    sa.sa_sigaction = signal_segv;
    sa.sa_flags = SA_SIGINFO;
#ifndef XEON_SPECIFIC_CHANGES
    sigaction(SIGSEGV, &sa, NULL);

    memset(&sa, 0, sizeof(struct sigaction));
    sigemptyset(&sa.sa_mask);
    sa.sa_sigaction = signal_segv;
    sa.sa_flags = SA_SIGINFO;

    sigaction(SIGILL, &sa, NULL);
#else
    if(sigaction(SIGILL, &sa, NULL) != 0)
    {
       printf("Failed to process sigaction for the SIGILL\n");
       RETVALUE(RFAILED);
    }
    if(sigaction(SIGSEGV, &sa, NULL) != 0)
    {
       printf("Failed to process sigaction for the SIGSEGV\n");
       RETVALUE(RFAILED);
    }
    if(sigaction(SIGABRT, &sa, NULL) != 0)
    {
       printf("Failed to process sigaction for the SIGABRT\n");
       RETVALUE(RFAILED);
    }
    if(sigaction(SIGTERM, &sa, NULL) != 0)
    {
       printf("Failed to process sigaction for the SIGTERM\n");
       RETVALUE(RFAILED);
    }
    if(sigaction(SIGHUP, &sa, NULL) != 0)
    {
       printf("Failed to process sigaction for the SIGHUP\n");
       RETVALUE(RFAILED);
    }
#endif    
#else
   signal (SIGSEGV, mtSigSegvHndlr);
   signal (SIGKILL, mtSigSegvHndlr);
   signal (SIGUSR2, mtSigUsr2Hndlr);
#endif   
#endif

#ifdef MLOG_XEON
   signal (SIGINT, mtStopHndlr);
#else

   act.sa_handler = mtIntSigHndlr;
   sigfillset(&act.sa_mask);
   act.sa_flags = 0;
   if (sigaction(SIGINT, &act, NULLP) != 0)
   {
      RETVALUE(RFAILED);
   }
#endif

   /* mt040.201 initialise random seed */
   osCp.dep.randSeed = time(NULLP);

   RETVALUE(ROK);
}


/*
*
*       Fun:   De-initialize OS control point
*
*       Desc:  This function reverses the initialization in ssdInitGen().
*
*       Ret:   ROK      - ok
*
*       Notes:
*
*       File:  mt_ss.c
*
*/
#ifdef ANSI
PUBLIC Void ssdDeinitGen
(
void
)
#else
PUBLIC Void ssdDeinitGen()
#endif
{
   TRC0(ssdDeinitGen);


   sem_destroy(&osCp.dep.ssStarted);


   RETVOID;
}
#ifdef SS_LOCKLESS_MEMORY
#ifdef USE_MALLOC
/*
*
*       Fun:   ssPutDynMemBlkSet
*
*       Desc:  Returns the set of dynamic Blocks into the global region
*
*
*       Ret:   ROK     - successful, 
*              RFAILED - unsuccessful.
*
*       Notes: 
*
*       File:  cm_mem.c
*
*/
#ifdef ANSI
PUBLIC S16 ssPutDynMemBlkSet
(
U8                    bktIdx,        /* Index to bucket list */
CmMmBlkSetElement    *dynMemSetElem  /* Memory set element which is needs to be 
                                        added to global region */
)
#else
PUBLIC S16 ssPutDynMemBlkSet(bktIdx, dynMemSetElem)
U8                    bktIdx;        /* Index to bucket list */
CmMmBlkSetElement    *dynMemSetElem; /* Memory set element which is needs to be 
                                        added to global region */
#endif
{
   CmMmGlobRegCb        *globReg;
   CmMmGlobalBktCb      *bktCb;
   Data                 *blkPtr;
   U8                    blkCnt;

   globReg = osCp.globRegCb;

#if (ERRCLASS & ERRCLS_INT_PAR)
   if(bktIdx >= globReg->numBkts)
   {
      RETVALUE(RFAILED);
   }
#endif /* ERRCLASS & ERRCLS_INT_PAR */

   bktCb = &(globReg->bktTbl[bktIdx]);

   for(blkCnt = 0; blkCnt < bktCb->bucketSetSize; blkCnt++)
   {
      blkPtr = dynMemSetElem->nextBktPtr;
      dynMemSetElem->nextBktPtr = *((CmMmEntry **)blkPtr);
      free((Void *)blkPtr);
   }

   dynMemSetElem->nextBktPtr = NULLP;
   dynMemSetElem->numFreeBlks = 0;

   RETVALUE(ROK);
}

/*
*
*       Fun:   ssGetDynMemBlkSet
*
*       Desc:  Gets the set of dynamic memory blocks from the global region
*
*
*       Ret:   ROK     - successful, 
*              RFAILED - unsuccessful.
*
*       Notes: 
*
*       File:  cm_mem.c
*
*/
#ifdef ANSI
PUBLIC S16 ssGetDynMemBlkSet
(
U8                     bktIdx,        /* Index to bucket list */
CmMmBlkSetElement     *dynMemSetElem  /* Memory set element which is updated 
                                      with new set values */
)
#else
PUBLIC S16 ssGetDynMemBlkSet(bktIdx, dynMemSetElem)
U8                     bktIdx;        /* Index to bucket list */
CmMmBlkSetElement     *dynMemSetElem; /* Memory set element which is updated 
                                      with new set values */
#endif
{

   CmMmGlobRegCb        *globReg;
   CmMmGlobalBktCb      *bktCb;
   Data                **basePtr;
   Data                 *blkPtr;
   U8                    blkCnt;

   globReg = osCp.globRegCb;

#if (ERRCLASS & ERRCLS_INT_PAR)
   if(bktIdx >= globReg->numBkts)
   {
      RETVALUE(RFAILED);
   }
#endif /* ERRCLASS & ERRCLS_INT_PAR */

   bktCb   = &(globReg->bktTbl[bktIdx]);
   basePtr = &(dynMemSetElem->nextBktPtr);

   for(blkCnt = 0; blkCnt < bktCb->bucketSetSize; blkCnt++)
   {
      blkPtr = (Data *)malloc(bktCb->size);
      *basePtr = blkPtr;
      basePtr = (CmMmEntry **)blkPtr;
   }

   dynMemSetElem->numFreeBlks = bktCb->bucketSetSize;

   RETVALUE(ROK);

} /* ssGetDynMemBlkSet */

#else
/*
*
*       Fun:   ssPutDynMemBlkSet
*
*       Desc:  Returns the set of dynamic Blocks into the global region
*
*
*       Ret:   ROK     - successful, 
*              RFAILED - unsuccessful.
*
*       Notes: 
*
*       File:  cm_mem.c
*
*/
#ifdef ANSI
PUBLIC S16 ssPutDynMemBlkSet
(
U8                    bktIdx,               /* Index to bucket list */
CmMmBlkSetElement    *dynMemSetElem,        /* Memory set element which is needs to be 
                                               added to global region */
U32                    doNotBlockForLock    /* Boolean whether to block for lock or not */
)
#else
PUBLIC S16 ssPutDynMemBlkSet(bktIdx, dynMemSetElem)
U8                    bktIdx;               /* Index to bucket list */
CmMmBlkSetElement    *dynMemSetElem;        /* Memory set element which is needs to be 
                                               added to global region */
U32                    doNotBlockForLock;   /* Boolean whether to block for lock or not */
#endif
{
   CmMmGlobRegCb       *globReg;
   CmMmGlobalBktCb     *bktCb;
   CmLList             *lstNode;
   CmMmBlkSetElement   *globMemNode;
   S16                  lockRet = 0;

   TRC1(ssPutDynMemBlkSet);

   globReg = osCp.globRegCb;

#if (ERRCLASS & ERRCLS_INT_PAR)
   if(bktIdx >= globReg->numBkts)
   {
      RETVALUE(RFAILED);
   }
#endif /* ERRCLASS & ERRCLS_INT_PAR */

   bktCb = &(globReg->bktTbl[bktIdx]);

   /* Lock the global region first. If the doNotBlockForLock is non-zero, the 
      try lock is used as it is not required to block as it will be taken
      in the next go else it will be blocked for lock as we have to get the
      blocks must */
   {
      SLock(&(bktCb->bucketLock));
   }

   if(lockRet == 0)
   {

      /* Get a free node from the free node linked list */
      lstNode = cmLListFirst(&(bktCb->listFreeBktSet));
      if(lstNode == NULLP)
      {
         SUnlock(&(bktCb->bucketLock));
         RETVALUE(RFAILED);
      }

      cmLListDelFrm(&(bktCb->listFreeBktSet), lstNode);

      /* Copy the content of the received element information on to free node 
       * and add it to valid linked list */
      globMemNode = (CmMmBlkSetElement *)lstNode->node;
      globMemNode->numFreeBlks = dynMemSetElem->numFreeBlks;
      globMemNode->nextBktPtr = dynMemSetElem->nextBktPtr;
      dynMemSetElem->numFreeBlks = 0;
      dynMemSetElem->nextBktPtr = NULLP;

      cmLListAdd2Tail(&(bktCb->listValidBktSet), &(globMemNode->memSetNode));

      SUnlock(&(bktCb->bucketLock));
   }

   RETVALUE(ROK);
}

/*
*
*       Fun:   ssGetDynMemBlkSet
*
*       Desc:  Gets the set of dynamic memory blocks from the global region
*
*
*       Ret:   ROK     - successful, 
*              RFAILED - unsuccessful.
*
*       Notes: The parameter doNotBlockForLock specifies whether to block for lock
*              or not 
*
*       File:  cm_mem.c
*
*/
#ifdef ANSI
PUBLIC S16 ssGetDynMemBlkSet
(
U8                     bktIdx,              /* Index to bucket list */
CmMmBlkSetElement     *dynMemSetElem,       /* Memory set element which is updated 
                                               with new set values */
U32                    doNotBlockForLock    /* Boolean whether to block for lock or not */
)
#else
PUBLIC S16 ssGetDynMemBlkSet(bktIdx, dynMemSetElem)
U8                     bktIdx;              /* Index to bucket list */
CmMmBlkSetElement     *dynMemSetElem;       /* Memory set element which is updated 
                                               with new set values */
U32                    doNotBlockForLock;   /* Boolean whether to block for lock or not */
#endif
{
   CmMmGlobRegCb        *globReg;
   CmMmGlobalBktCb      *bktCb;
   CmLList              *lstNode;
   CmMmBlkSetElement    *globMemNode;
   S16                   lockRet = 0;

   TRC1(ssGetDynMemBlkSet);

   globReg = osCp.globRegCb;

#if (ERRCLASS & ERRCLS_INT_PAR)
   if(bktIdx >= globReg->numBkts)
   {
      RETVALUE(RFAILED);
   }
#endif /* ERRCLASS & ERRCLS_INT_PAR */

   bktCb = &(globReg->bktTbl[bktIdx]);

   /* Lock the global region first. If the doNotBlockForLock is non-zero, the 
      try lock is used as it is not required to block as it will be taken
      in the next go else it will be blocked for lock as we have to get the
      blocks must */
   {
      SLock(&(bktCb->bucketLock));
   }

   if(lockRet == 0)
   {
      lstNode = cmLListFirst(&(bktCb->listValidBktSet));

      if(lstNode == NULLP)
      {
         SUnlock(&(bktCb->bucketLock));
         RETVALUE(RFAILED);
      }

      /* Delete the node from the valid linked list and copy the values of the
       * elements of structrues into pointer */
      cmLListDelFrm(&(bktCb->listValidBktSet), lstNode);
      globMemNode = (CmMmBlkSetElement *)lstNode->node;
      dynMemSetElem->numFreeBlks = globMemNode->numFreeBlks;
      dynMemSetElem->nextBktPtr = globMemNode->nextBktPtr;

      /* Add this node to the free node linked list */
      cmLListAdd2Tail(&(bktCb->listFreeBktSet), lstNode);

      SUnlock(&(bktCb->bucketLock));
   }

   RETVALUE(ROK);
} /* ssGetDynMemBlkSet */


#define NUM_CALLS_TO_CHECK_MEM_DYN_AGAIN 100
U32 gDynMemAlrm[4];
PRIVATE U32 memoryCheckCounter;

#ifdef ANSI
PUBLIC U32 isMemThreshReached(
Region reg
)
#else
PUBLIC U32 isMemThreshReached(reg)
Region reg;
#endif
{
   CmMmGlobRegCb        *globReg;
   CmMmGlobalBktCb      *bktCb;
   U8 bktIdx= reg;
 TRC3(isMemThreshReached)

   globReg = osCp.globRegCb;

#if (ERRCLASS & ERRCLS_INT_PAR)
   if(bktIdx >= globReg->numBkts)
   {
      RETVALUE(RFAILED);
   }
#endif /* ERRCLASS & ERRCLS_INT_PAR */

   bktCb   = &(globReg->bktTbl[bktIdx]);

   if(gDynMemAlrm[bktIdx])
   {
   //        printf ("under memory   bktCb->listValidBktSet.count %d bktIdx %d\n",bktCb->listValidBktSet.count ,bktIdx);
	   SLock(&(bktCb->bucketLock));
	   if(bktCb->listValidBktSet.count > 25)
            {
	   gDynMemAlrm[bktIdx] = FALSE;
     //      printf ("recoverd bktCb->listValidBktSet.count %d bktIdx %d\n",bktCb->listValidBktSet.count ,bktIdx);
             }
	   SUnlock(&(bktCb->bucketLock));
	   RETVALUE(RFAILED);
   }
   else
   {

	   if(memoryCheckCounter++ >= NUM_CALLS_TO_CHECK_MEM_DYN_AGAIN)
	   {
       //    printf ("CHECK  bktCb->listValidBktSet.count %d bktIdx %d\n",bktCb->listValidBktSet.count ,bktIdx);
		   SLock(&(bktCb->bucketLock));
		   if(bktCb->listValidBktSet.count < 15 )
			   gDynMemAlrm[bktIdx] = TRUE;
		   memoryCheckCounter = 0;
		   SUnlock(&(bktCb->bucketLock));
	   }
   }
   RETVALUE(ROK);
}

#endif /* USE_MALLOC */
#endif /* SS_LOCKLESS_MEMORY */

#ifdef SS_USE_ICC_MEMORY
/*
*
*       Fun:   Initialize region/pool tables
*
*       Desc:  This function initializes MTSS-specific information
*              in the region/pool tables and configures the common
*              memory manager for use.
*
*       Ret:   ROK      - ok
*
*       Notes:
*
*       File:  mt_ss.c
*
*/
#ifdef ANSI
PUBLIC Void * ssGetIccHdl  
(
Region region
)
#else
PUBLIC Void * ssGetIccHdl()
Region region;
#endif
{
   CmMmDynRegCb *dynRegCb;

   /* Klock work fix ccpu00148484 */
   if(!(region < SS_MAX_REGS))
   {
      RETVALUE(NULLP);
   }
   
   dynRegCb = (CmMmDynRegCb *)osCp.dynRegionTbl[region].regCb; 

   RETVALUE(dynRegCb->iccHdl);
}
#endif /* SS_USE_ICC_MEMORY */

#ifdef T2K_MEM_LEAK_DBG
extern RegionMemLeakInfo regMemLeakInfo;
#endif /* T2K_MEM_LEAK_DBG */

#ifdef INTEL_WLS

PUBLIC S16 SPartitionWlsMemory()
{
   U32    i;
#ifndef ALIGN_64BIT
   U64    reqdSz;
   U64   pageSize[1], hugePageSize;
#else
   long int reqdSz;
   long int pageSize[1], hugePageSize;
#endif
   U32 numHugePg;
   #define DIV_ROUND_OFFSET(X,Y) ( X/Y + ((X%Y)?1:0) )

   U8   *regMemStrtAddr = (U8 *)osCp.wls.allocAddr;

   gethugepagesizes(pageSize,1);
   hugePageSize = pageSize[0];
   for (i = 0; i < 1; i++)
   {
      mtRegMemSz[i].startAddr = regMemStrtAddr;
      CM_LOG_DEBUG(CM_LOG_ID_MT, "Global Region-->Bkt[%d] Addr:%p\n", i, mtRegMemSz[i].startAddr);

      numHugePg = DIV_ROUND_OFFSET(mtRegMemSz[i].reqdSz, hugePageSize);
      reqdSz    = numHugePg * hugePageSize;
      regMemStrtAddr += reqdSz;
#ifdef T2K_MEM_LEAK_DBG
      /* Since wls is region 0 */
      regMemLeakInfo.regStartAddr[i] = (U64)mtRegMemSz[i].startAddr;
      regMemLeakInfo.numActvRegions++;
#endif /* T2K_MEM_LEAK_DBG */
   }
   //Store last region addr for validation
   mtRegMemSz[i].startAddr = regMemStrtAddr;
   RETVALUE(ROK);
}

#ifdef SS_MEM_WL_DEBUG
PUBLIC Void SChkAddrValid(int type, int region, PTR ptr)
{
   char *tryPtr = NULL;
   if(type == 0) //Global
   {
      if(ptr < mtRegMemSz[0].startAddr || ptr >=
              (mtRegMemSz[mtGlobMemoCfg.numBkts].startAddr + mtGlobMemoCfg.heapSize))
      {
         printf("****INVALID PTR in Global Region: ptr:%p start:%p end:%p***\n", ptr, mtRegMemSz[0].startAddr, mtRegMemSz[mtGlobMemoCfg.numBkts].startAddr);
         *tryPtr = 0;
      }
   }
   else
   {
      if(ptr > mtRegMemSz[0].startAddr && ptr <= mtRegMemSz[mtGlobMemoCfg.numBkts].startAddr )
      {
         printf("****INVALID PTR in Static Region: ptr:%p start:%p end:%p***\n", ptr, mtRegMemSz[0].startAddr, mtRegMemSz[mtGlobMemoCfg.numBkts].startAddr);
         *tryPtr = 0;
      }

   }
}
#endif /* SS_MEM_WL_DEBUG */

PUBLIC S16 SPartitionStaticMemory(U8  *startAddr)
{
   int    i;
   U32    reqdSz;

   U8   *regMemStrtAddr = (U8 *)startAddr;


   //for (i = 0; i < mtMemoCfg.numRegions; i++)
   for (i = 1; i < mtMemoCfg.numRegions; i++)
   {
      mtRegMemSz[i].startAddr = regMemStrtAddr;
      reqdSz    = /* regMemStrtAddr + */mtRegMemSz[i].reqdSz;
      regMemStrtAddr += reqdSz;
#ifdef T2K_MEM_LEAK_DBG
      {  /* Since region 1 onwards are used for non wls */
         regMemLeakInfo.regStartAddr[i] = (U64)mtRegMemSz[i].startAddr;
         regMemLeakInfo.numActvRegions++;
      }
#endif /* T2K_MEM_LEAK_DBG */
   }
   RETVALUE(ROK);
}
PUBLIC S16 SAllocateWlsMem()
{

   U32            reqdMemSz;
   U32            i, j;
   MtRegCfg       *region;

   reqdMemSz = 0;
   //memset(&mtRegMemSz[0], sizeof(mtRegMemSz), 0);
   memset(&mtRegMemSz[0], 0, sizeof(mtRegMemSz));

   for (i = 0; i < 1; i++)
   {
      /* allocate space for the region */
      region = &mtMemoCfg.region[i];
      reqdMemSz += region->heapsize;
      mtRegMemSz[i].reqdSz  += region->heapsize;

      for (j = 0; j < region->numBkts; j++)
      {
         reqdMemSz += region->bkt[j].blkSize * region->bkt[j].numBlks;
         mtRegMemSz[i].reqdSz += region->bkt[j].blkSize * region->bkt[j].numBlks;
      }
   }
   osCp.wls.allocAddr = WLS_Alloc(osCp.wls.intf, (512 *1024 * 1024));
   //osCp.wls.allocAddr = WLS_Alloc(osCp.wls.intf, (reqdMemSz + (1024 * 1024 * 2 * 2)));
#ifndef ALIGN_64BIT
   printf("\n ************* \n WLS memory: %llx, %ld\n ****** \n", osCp.wls.allocAddr, reqdMemSz); 
#else
   printf("\n ************* \n WLS memory: %lx, %d\n ****** \n", (PTR)osCp.wls.allocAddr, reqdMemSz); 
#endif
   SPartitionWlsMemory();
   RETVALUE(ROK);
}
PUBLIC S16 SAllocateStaticMem()
{

   U32            reqdMemSz;
   int            i, j;
   MtRegCfg       *region;
   U8             *startAddr;

   reqdMemSz = 0;
   //memset(&mtRegMemSz[0], sizeof(mtRegMemSz), 0);

   //for (i = 0; i < mtMemoCfg.numRegions; i++)
   for (i = 1; i <  mtMemoCfg.numRegions; i++)
   {
      /* allocate space for the region */
      region = &mtMemoCfg.region[i];
      reqdMemSz += region->heapsize;
      mtRegMemSz[i].reqdSz  += region->heapsize;

      for (j = 0; j < region->numBkts; j++)
      {
         reqdMemSz += region->bkt[j].blkSize * region->bkt[j].numBlks;
         mtRegMemSz[i].reqdSz += region->bkt[j].blkSize * region->bkt[j].numBlks;
      }
   }

   startAddr = malloc(reqdMemSz + (1024 * 10));
#ifndef ALIGN_64BIT
   printf("\n ************* \n Static memory: %llx, %ld\n ****** \n", startAddr, reqdMemSz); 
#else
   printf("\n ************* \n Static memory: %lx, %d\n ****** \n", (PTR)startAddr, reqdMemSz); 
#endif
   SPartitionStaticMemory(startAddr);
   RETVALUE(ROK);
}
#endif /* INTEL_WLS */



/*
*
*       Fun:   Initialize region/pool tables
*
*       Desc:  This function initializes MTSS-specific information
*              in the region/pool tables and configures the common
*              memory manager for use.
*
*       Ret:   ROK      - ok
*
*       Notes:
*
*       File:  mt_ss.c
*
*/
#ifdef ANSI
PUBLIC S16 ssdInitMem
(
void
)
#else
PUBLIC S16 ssdInitMem()
#endif
{
   /* mt018.201 - added local variable */
   U8              i;
   U16             j;
   U8              k;
   MtRegCfg       *region;
   Txt             errMsg[256] = {'\0'};
#ifdef SS_LOCKLESS_MEMORY
   CmMmDynRegCb   *dynRegCb;
#ifdef SS_USE_ICC_MEMORY
#else
   CmMmGlobRegCb  *globReg;
   Size            memSize;
#endif
#endif /* SS_LOCKLESS_MEMORY */
   
   TRC0(ssdInitMem);

   /* Use the default SSI memory manager if the ICC memory manager is not 
    * avilable. If ICC memory manager is avilable, it will be used for
    * all sharable memory allocation and de-allocation */
#ifdef SS_LOCKLESS_MEMORY
#ifdef SS_USE_ICC_MEMORY
#ifndef YS_PHY_3_8_2
#endif
   for (i = 0; i < mtDynMemoCfg.numRegions; i++)
   {
      dynRegCb = (CmMmDynRegCb *)calloc(1, sizeof(CmMmDynRegCb));
      if(dynRegCb == NULLP)
      {
         RETVALUE(RFAILED);
      }
      for(k = 0; k < mtDynMemoCfg.region[i].numBkts; k++)
      {
         dynRegCb->bktSize[k] = mtGlobMemoCfg.bkt[k].blkSize;
      }
      dynRegCb->region = i;
      cmMmDynRegInit(dynRegCb);
      printf("iccHdl = %lx\n", (PTR)dynRegCb->iccHdl);
      sleep(1);
   }
 /*   ysIccHdl = dynRegCb->iccHdl; */

#else
   /* Initialize the global region first */
   osCp.globRegCb = calloc(1, sizeof(CmMmGlobRegCb));

   if(osCp.globRegCb == NULLP)
   {
      RETVALUE(RFAILED);
   }

   globReg = (CmMmGlobRegCb *)osCp.globRegCb;

   for(i = 0; i < mtGlobMemoCfg.numBkts; i++)
   {
      memSize = (mtGlobMemoCfg.bkt[i].blkSize * mtGlobMemoCfg.bkt[i].numBlks);
#ifndef INTEL_WLS      
      globReg->bktTbl[i].startAddr = (Data *)calloc(memSize, sizeof(Data));
#else
      globReg->bktTbl[i].startAddr = (Data *)mtRegMemSz[i].startAddr;
#endif
      if(globReg->bktTbl[i].startAddr == NULLP)
      {
         RETVALUE(RFAILED);
      }
      globReg->bktTbl[i].poolId = i;
      globReg->bktTbl[i].size = mtGlobMemoCfg.bkt[i].blkSize;
      globReg->bktTbl[i].numBlks = mtGlobMemoCfg.bkt[i].numBlks;
      globReg->bktTbl[i].bucketSetSize = mtGlobMemoCfg.bkt[i].bucketSetSize;
   }

   globReg->numBkts = mtGlobMemoCfg.numBkts;
   cmMmGlobRegInit(globReg);

   /* Initialize the dynamic task regions and sanity check for the theshold 
    * values */
   for (i = 0; i < mtDynMemoCfg.numRegions; i++)
   {
      dynRegCb = (CmMmDynRegCb *)calloc(1, sizeof(CmMmDynRegCb));
      if(dynRegCb == NULLP)
      {
         RETVALUE(RFAILED);
      }
      for(k = 0; k < mtDynMemoCfg.region[i].numBkts; k++)
      {
         if((mtDynMemoCfg.region[i].bkt[k].blkSetRelThreshold < 
                       mtDynMemoCfg.region[i].bkt[k].blkSetAcquireThreshold) ||
             (mtDynMemoCfg.region[i].bkt[k].blkSetAcquireThreshold == 0) ||
             (mtDynMemoCfg.region[i].bkt[k].blkSetRelThreshold == 0))
         {
#ifdef XEON_SPECIFIC_CHANGES
            free(dynRegCb);
#endif            
            RETVALUE(RFAILED);
         }
         dynRegCb->bktTbl[k].size = mtGlobMemoCfg.bkt[k].blkSize;
         dynRegCb->bktTbl[k].blkSetRelThreshold = mtDynMemoCfg.region[i].bkt[k].blkSetRelThreshold;
         dynRegCb->bktTbl[k].blkSetAcquireThreshold = mtDynMemoCfg.region[i].bkt[k].blkSetAcquireThreshold;
         dynRegCb->bktTbl[k].bucketSetSize = mtGlobMemoCfg.bkt[k].bucketSetSize;
         if(dynRegCb->bktMaxBlkSize < dynRegCb->bktTbl[k].size)
         {
            dynRegCb->bktMaxBlkSize = dynRegCb->bktTbl[k].size;
         }
      }
      dynRegCb->region = i;
      dynRegCb->numBkts = mtDynMemoCfg.region[i].numBkts;
      cmMmDynRegInit(dynRegCb);
   }
#endif /* SS_USE_ICC_MEMORY */
#endif /* SS_LOCKLESS_MEMORY */

#ifdef T2K_MEM_LEAK_DBG
    U8 reg; 
    /* Initailize mem leak tool memorys for debguing */
    regMemLeakInfo.numActvRegions=0;
    for(reg=0; reg <SS_MAX_REGS; reg++)
    {   
       regMemLeakInfo.gMemLeakInfo[reg] =  malloc(sizeof(T2kMeamLeakInfo)*T2K_MEM_LEAK_INFO_TABLE_SIZE);
       memset(regMemLeakInfo.gMemLeakInfo[reg],0x0,
             sizeof(T2kMeamLeakInfo)*T2K_MEM_LEAK_INFO_TABLE_SIZE);
       regMemLeakInfo.regStartAddr[reg] = 0;


       regMemLeakInfo.regStartAddr[reg] = 0;
       if (pthread_mutex_init(&(regMemLeakInfo.memLock[reg]), NULL) != 0)
       {
          printf("\n mutex init failed\n");
          RETVALUE(RFAILED);
       }
    }
#endif
#ifdef INTEL_WLS
   /* Now allocate WLS memory */
   SAllocateWlsMem();
   SAllocateStaticMem();
#endif
   /* mt018.201 - CMM Initialization */
   for (i = 0; i < mtMemoCfg.numRegions; i++)
   {
      /* allocate space for the region control block */
      mtCMMRegCb[i] = (CmMmRegCb *)calloc(1, sizeof(CmMmRegCb));
#ifdef TENB_RTLIN_CHANGES
      mlock(mtCMMRegCb[i], sizeof(CmMmRegCb));
#endif
      if (mtCMMRegCb[i] == NULLP)
      {
			sprintf(errMsg,"\n ssdInitMem(): Could not allocated memory \
								for the Region:%d control block\n",i);
			SPrint(errMsg);
         for (k = 0; k < i; k++)
         {
            cmMmRegDeInit(mtCMMRegCb[k]);
            free(mtCMMRegCfg[k]->vAddr);
            free(mtCMMRegCb[k]);
	    free(mtCMMRegCfg[k]);
         }
         RETVALUE(RFAILED);
      }

      mtCMMRegCfg[i] = (CmMmRegCfg *)calloc(1, sizeof(CmMmRegCfg));
#ifdef TENB_RTLIN_CHANGES
      mlock(mtCMMRegCfg[i], sizeof(CmMmRegCfg));
#endif
      if (mtCMMRegCfg[i] == NULLP)
		{
		  for (k = 0; k < i; k++)
		  {
			 cmMmRegDeInit(mtCMMRegCb[k]);
			 free(mtCMMRegCfg[k]->vAddr);
			 free(mtCMMRegCb[k]);
			 free(mtCMMRegCfg[k]);
		  }
		  free(mtCMMRegCb[i]);
		  RETVALUE(RFAILED);
		}


      /* allocate space for the region */
      region = &mtMemoCfg.region[i];
      mtCMMRegCfg[i]->size = region->heapsize;
      for (j = 0; j < region->numBkts; j++)
      {
/* mt033.201 - addition for including the header size while computing the total size */
#ifdef SSI_DEBUG_LEVEL1
         mtCMMRegCfg[i]->size += (region->bkt[j].blkSize + sizeof(CmMmBlkHdr)) *\
                                 (region->bkt[j].numBlks);
#else
         mtCMMRegCfg[i]->size += region->bkt[j].blkSize * region->bkt[j].numBlks;
#endif /* SSI_DEBUG_LEVEL1 */
      }
#ifdef INTEL_WLS
      mtCMMRegCfg[i]->vAddr = (Data *)mtRegMemSz[i].startAddr;
#else
      mtCMMRegCfg[i]->vAddr = (Data *)calloc(mtCMMRegCfg[i]->size,
                                             sizeof(Data));
#endif
#ifdef XEON_SPECIFIC_CHANGES
      CM_LOG_DEBUG(CM_LOG_ID_MT, "Static Region-->Bkt[%d] Addr:[%p] RegionId=[%d] Size=[%d] \n", 
                                 i, mtCMMRegCfg[i]->vAddr, region->regionId, mtCMMRegCfg[i]->size);
#endif      
#ifdef TENB_RTLIN_CHANGES
      mlock(mtCMMRegCfg[i]->vAddr, mtCMMRegCfg[i]->size*sizeof(Data));
#endif

      if (mtCMMRegCfg[i]->vAddr == NULLP)
      {
			sprintf(errMsg,"\n ssdInitMem(): Could not allocate memory \
								for the Region:%d \n",i);
		   SPrint(errMsg);
         for (k = 0; k < i; k++)
         {
            cmMmRegDeInit(mtCMMRegCb[k]);
            free(mtCMMRegCfg[k]->vAddr);
            free(mtCMMRegCb[k]);
            free(mtCMMRegCfg[k]);
         }
         free(mtCMMRegCb[i]);
         free(mtCMMRegCfg[i]);
         RETVALUE(RFAILED);
      }


      /* set up the CMM configuration structure */
      mtCMMRegCfg[i]->lType = SS_LOCK_MUTEX;
      mtCMMRegCfg[i]->chFlag = 0;
      mtCMMRegCfg[i]->bktQnSize = MT_BKTQNSIZE;
      mtCMMRegCfg[i]->numBkts = region->numBkts;

      for (j = 0; j < region->numBkts; j++)
      {
         mtCMMRegCfg[i]->bktCfg[j].size    = region->bkt[j].blkSize;
         mtCMMRegCfg[i]->bktCfg[j].numBlks = region->bkt[j].numBlks;
      }

      /* initialize the CMM */
#ifdef SS_LOCKLESS_MEMORY
      if (cmMmStatRegInit(region->regionId, mtCMMRegCb[i], mtCMMRegCfg[i]) != ROK)
#else
      if (cmMmRegInit(region->regionId, mtCMMRegCb[i], mtCMMRegCfg[i]) != ROK)
#endif /* SS_LOCKLESS_MEMORY */
      {
         for (k = 0; k < i; k++)
         {
            cmMmRegDeInit(mtCMMRegCb[k]);
            free(mtCMMRegCfg[k]->vAddr);
            free(mtCMMRegCb[k]);
            free(mtCMMRegCfg[k]);
         }
         free(mtCMMRegCfg[i]->vAddr);
         free(mtCMMRegCb[i]);
         free(mtCMMRegCfg[i]);
         RETVALUE(RFAILED);
      }


      /* initialize the STREAMS module */
      /* mt019.201: STREAMS module will only apply to DFLT_REGION */
      if (region->regionId == 0)
      {
         if (ssStrmCfg(region->regionId, region->regionId) != ROK)
         {
            for (k = 0; k < i; k++)
            {
               cmMmRegDeInit(mtCMMRegCb[k]);
               free(mtCMMRegCfg[k]->vAddr);
               free(mtCMMRegCb[k]);
               free(mtCMMRegCfg[k]);
            }
            cmMmRegDeInit(mtCMMRegCb[i]);
            free(mtCMMRegCfg[i]->vAddr);
            free(mtCMMRegCb[i]);
            free(mtCMMRegCfg[i]);
            RETVALUE(RFAILED);
         }
      }
   }
/* mt001.301 : Additions */
#ifdef SS_MEM_LEAK_STS
   cmInitMemLeakMdl();
#endif /* SS_MEM_LEAK_STS */


   RETVALUE(ROK);
}


/*
*
*       Fun:   De-initialize region/pool tables
*
*       Desc:  This function reverses the initialization in ssdInitMem().
*
*       Ret:   ROK      - ok
*
*       Notes:
*
*       File:  mt_ss.c
*
*/
#ifdef ANSI
PUBLIC Void ssdDeinitMem
(
void
)
#else
PUBLIC Void ssdDeinitMem()
#endif
{
   /* mt018.201 - added local variables */
   U8     i;

   TRC0(ssdDeinitMem);
	/* mt008.301 Additions */
#ifdef SS_MEM_LEAK_STS
	cmDeinitMemLeakMdl();
#endif /* SS_MEM_LEAK_STS */

   for (i = 0; i < mtMemoCfg.numRegions; i++)
   {
      cmMmRegDeInit(mtCMMRegCb[i]);
      free(mtCMMRegCfg[i]->vAddr);
      free(mtCMMRegCb[i]);
      free(mtCMMRegCfg[i]);
   }

   RETVOID;
}


/*
*
*       Fun:   Initialize task table
*
*       Desc:  This function initializes MTSS-specific information
*              in the task table.
*
*       Ret:   ROK      - ok
*
*       Notes:
*
*       File:  mt_ss.c
*
*/
#ifdef ANSI
PUBLIC S16 ssdInitTsk
(
void
)
#else
PUBLIC S16 ssdInitTsk()
#endif
{
/* mt001.301 : Additions */
/*mt013.301 :Added SS_AFFINITY_SUPPORT  */
#if defined(SS_MULTICORE_SUPPORT) ||defined(SS_AFFINITY_SUPPORT)
   U32 tskInd = 0;
#endif /* SS_MULTICORE_SUPPORT || SS_AFFINITY_SUPPORT */

   TRC0(ssdInitTsk);


/*mt013.301 :Added SS_AFFINITY_SUPPORT  */
#if defined(SS_MULTICORE_SUPPORT) || defined(SS_AFFINITY_SUPPORT)
   /* initialize system task information */
   for (tskInd = 0;  tskInd < SS_MAX_STSKS;  tskInd++)
   {
      osCp.sTskTbl[tskInd].dep.lwpId = 0;
   }
#endif /* SS_MULTICORE_SUPPORT || SS_AFFINITY_SUPPORT */
   RETVALUE(ROK);
}


/*
*
*       Fun:   Deinitialize task table
*
*       Desc:  This function reverses the initialization perfomed in
*              ssdInitTsk().
*
*       Ret:   ROK      - ok
*
*       Notes:
*
*       File:  mt_ss.c
*
*/
#ifdef ANSI
PUBLIC Void ssdDeinitTsk
(
void
)
#else
PUBLIC Void ssdDeinitTsk()
#endif
{
   TRC0(ssdDeinitTsk);

   RETVOID;
}


#ifdef SS_DRVR_SUPPORT
/*
*
*       Fun:   Initialize driver task table
*
*       Desc:  This function initializes MTSS-specific information
*              in the driver task table.
*
*       Ret:   ROK      - ok
*
*       Notes:
*
*       File:  mt_ss.c
*
*/
#ifdef ANSI
PUBLIC S16 ssdInitDrvr
(
void
)
#else
PUBLIC S16 ssdInitDrvr()
#endif
{
   S16 i;
#ifndef L2_L3_SPLIT
   pthread_attr_t attr;
#endif

   TRC0(ssdInitDrvr);


   /* initialize the dependent portion of the driver task entries */
   for (i = 0;  i < SS_MAX_DRVRTSKS;  i++)
   {
      osCp.drvrTskTbl[i].dep.flag = FALSE;
   }


   /* create pipe for communication between SSetIntPend() and
    *  the isTskHdlr thread.
    */
   if (pipe(osCp.dep.isFildes) != 0)
   {
      RETVALUE(RFAILED);
   }

#ifndef L2_L3_SPLIT
   /* create the isTskHdlr thread */
   pthread_attr_init(&attr);
   /* mt021.201 - Addition to set stack size */
   pthread_attr_setstacksize(&attr, (size_t)MT_ISTASK_STACK);
   pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
   pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
   if ((pthread_create(&osCp.dep.isTskHdlrTID, &attr, mtIsTskHdlr, NULLP)) != 0)
   {
      /* mt020.201 - Addition for destroying thread attribute object attr */
      pthread_attr_destroy(&attr);

      RETVALUE(RFAILED);
   }
#endif   

  /*mt014.301 : 4GMX release related changes*/
#ifdef SS_4GMX_UCORE
   uarmDrvrInit();
#endif

#ifdef L2_L3_SPLIT
   drvrTskInit();
#endif
#ifndef L2_L3_SPLIT
   /* mt020.201 - Addition for destroying thread attribute object attr */
   pthread_attr_destroy(&attr);
#endif

   RETVALUE(ROK);
}


/*
*
*       Fun:   Deinitialize driver information
*
*       Desc:  This function reverses the initialization performed in
*              ssdInitDrvr().
*
*       Ret:   ROK      - ok
*
*       Notes:
*
*       File:  mt_ss.c
*
*/
#ifdef ANSI
PUBLIC Void ssdDeinitDrvr
(
void
)
#else
PUBLIC Void ssdDeinitDrvr()
#endif
{
   TRC0(ssdDeinitDrvr);
  /* mt008.301: Terminate the Driver Task on exit */
  while(pthread_cancel(osCp.dep.isTskHdlrTID));

#ifdef L2_L3_SPLIT
  TL_Close(AppContext.hUAII);
  if (clusterMode == RADIO_CLUSTER_MODE)
  {
      TL_Close(AppContext.hUAII_second);
  }
#endif

   RETVOID;
}
#endif /* SS_DRVR_SUPPORT */


/*
*
*       Fun:   Initialize timer table
*
*       Desc:  This function initializes MTSS-specific information
*              in the timer table.
*
*       Ret:   ROK      - ok
*
*       Notes:
*
*       File:  mt_ss.c
*
*/
#ifdef ANSI
PUBLIC S16 ssdInitTmr
(
void
)
#else
PUBLIC S16 ssdInitTmr()
#endif
{
   pthread_attr_t attr;
   struct sched_param param_sched;
  /* mt010.21: addition */
   S32 i;
#ifdef SS_MULTICORE_SUPPORT
   SsSTskEntry     *sTsk;
#endif /* SS_MULTICORE_SUPPORT */
#ifdef SS_THR_REG_MAP
   U32 threadCreated = FALSE;
#endif /* SS_THR_REG_MAP */

   TRC0(ssdInitTmr);


   osCp.dep.tmrTqCp.tmrLen = SS_MAX_TMRS;
  /* mt010.21: addition */
   osCp.dep.tmrTqCp.nxtEnt = 0;
   for (i=0; i< SS_MAX_TMRS; i++)
   {
      osCp.dep.tmrTq[i].first = (CmTimer *)NULLP;
   }

#ifdef SS_MULTICORE_SUPPORT
   sTsk = ssdAddTmrSTsk();
   if(!sTsk)
   {
      RETVALUE(RFAILED);
   }
#endif /* SS_MULTICORE_SUPPORT */
   /* create the timer handler thread */
   pthread_attr_init(&attr);
   /* mt021.201 - Addition to set stack size */
   pthread_attr_setstacksize(&attr, (size_t)MT_TMRTASK_STACK);
   pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
   pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
   pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
   param_sched.sched_priority = sched_get_priority_max(SCHED_FIFO);
   pthread_attr_setschedparam(&attr, &param_sched);


#ifdef SS_THR_REG_MAP
   /* When the thread is created, we check for the memory mapping table if
    * threadId can be placed in thread memory map table. If it is not able to place
    * threadId is stored in tmporary array. Once thread is created successful,
    * thread_cancel is sent for each thread which are created before. All the 
    * threads are made to wait on sema which is cancel point for thread.
    */
   while(threadCreated == FALSE)
   {
#endif
      if ((pthread_create(&osCp.dep.tmrHdlrTID, &attr, mtTmrHdlr, NULLP)) != 0)
      {
         /* mt020.201 - Addition for destroying thread attribute object attr */
         pthread_attr_destroy(&attr);

         RETVALUE(RFAILED);
      }

#ifdef SS_THR_REG_MAP
      threadCreated = ssCheckAndAddMemoryRegionMap(osCp.dep.tmrHdlrTID, 
                                                   sTsk->region);
   }
#endif /* SS_THR_REG_MAP */
#ifdef SS_MEM_WL_DEBUG
   tmpRegTidMap[sTsk->region] = osCp.dep.tmrHdlrTID;
#endif

   /* mt020.201 - Addition for destroying thread attribute object attr */
   pthread_attr_destroy(&attr);


   RETVALUE(ROK);
}


/*
*
*       Fun:   Deinitialize timer table
*
*       Desc:  This function reverses the initialization performed in
*              ssdInitTmr().
*
*       Ret:   ROK      - ok
*
*       Notes:
*
*       File:  mt_ss.c
*
*/
#ifdef ANSI
PUBLIC Void ssdDeinitTmr
(
void
)
#else
PUBLIC Void ssdDeinitTmr()
#endif
{
#ifdef SS_MULTICORE_SUPPORT
   SsSTskEntry *sTsk;
   S16         ret;
#endif /* SS_MULTICORE_SUPPORT */

   TRC0(ssdDeinitTmr);

#ifdef SS_MULTICORE_SUPPORT
   ret = SLock(&osCp.sTskTblLock);
   if (ret != ROK)
   {

#if (ERRCLASS & ERRCLS_DEBUG)
      MTLOGERROR(ERRCLS_DEBUG, EMT008, (ErrVal) ret,
                 "Could not lock system task table");
#endif
      RETVOID;
   }
   sTsk = &osCp.sTskTbl[0]; /* first entry is timer entry always */
   /* clean up the system task entry */
   sTsk->used = FALSE;
   sTsk->tskPrior = 0;
   sTsk->numTTsks = 0;
   SDestroyLock(&sTsk->lock);
   ssDestroyDmndQ(&sTsk->dQ);


   /* make this entry available in the system task table */
   sTsk->nxt = osCp.nxtSTskEntry;
   osCp.nxtSTskEntry = 0;

   osCp.numSTsks--;

   /* unlock the system task table */
   SUnlock(&osCp.sTskTblLock);

#endif /* SS_MULTICORE_SUPPORT */
  /* mt008.301: Terminate the timer thread on exit */
  while(pthread_cancel(osCp.dep.tmrHdlrTID));
  RETVOID;
}



/*
*
*       Fun:   ssdInitLog
*
*       Desc:  Pre-tst() initialization.
*
*       Ret:   ROK      - ok
*
*       Notes:
*
*       File:  mt_ss.c
*
*/
#ifdef ANSI
PUBLIC S16 ssdInitLog
(
void
)
#else
PUBLIC S16 ssdInitLog()
#endif
{
/* mt027.201 - Modification to fix warnings with no STDIN and STDOUT */
#ifdef CONAVL
#ifndef CONRD
   S32 flags;
   pthread_attr_t attr;
#ifndef CONSTDIO
   struct termios tio;
#endif /* CONSTDIO */
   S16 fd;
#endif /* CONRD */
#endif /* CONAVL */

/* mt008.301: ssdInitFinal changed to ssdInitLog */
   TRC0(ssdInitLog);


#ifdef CONAVL

   osCp.dep.conInFp = (FILE *) stdin;
   osCp.dep.conOutFp = (FILE *) stdout;
/* added compile time flag CONRD: mt017.21 */
#ifndef CONRD
#ifndef CONSTDIO

   /* disable canonical input processing */
   fd = fileno(osCp.dep.conInFp);
   if ((tcgetattr(fd, &tio)) != 0)
   {
      printf("Error: disable canonical input processing\n");
      RETVALUE(RFAILED);
   }

   tio.c_lflag &= ~ICANON;
   tio.c_cc[VMIN] = 1;   /* wait for a minimum of 1 character input */
   tio.c_cc[VTIME] = 0;
   if ((tcsetattr(fd, TCSANOW, &tio)) != 0)
   {
      printf("Error: while tcsetattr() processing\n");
      RETVALUE(RFAILED);
   }

#endif /* CONSTDIO */


   /* set up the input fd to block when no data is available */
   fd = fileno(osCp.dep.conInFp);
   flags = fcntl(fd, F_GETFL, &flags);
   flags &= ~O_NONBLOCK;
   if (fcntl(fd, F_SETFL, flags) == -1)
   {
      printf("Error: while fcntl processing\n");
      RETVALUE(RFAILED);
   }


   /* create the console handler thread */
   pthread_attr_init(&attr);
   /* mt021.201 - Addition to set stack size */
   pthread_attr_setstacksize(&attr, (size_t)MT_CONSOLE_STACK);
   pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
   pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);


   if((SCreatePThread(&osCp.dep.conHdlrTID, &attr, mtConHdlr, NULLP)) != 0)
   {
      /* mt020.201 - Addition for destroying thread attribute object attr */
      pthread_attr_destroy(&attr);

      printf("Error: Logging Thread creation failed \n");
      RETVALUE(RFAILED);
   }

   /* mt020.201 - Addition for destroying thread attribute object attr */
   pthread_attr_destroy(&attr);

#endif /* CONRD */
#endif /* CONAVL */


   RETVALUE(ROK);
}


/*
*
*       Fun:   ssdDeinitLog
*
*       Desc:  This function reverses the initialization performed in
*              ssdInitLog().
*
*       Ret:   ROK      - ok
*
*       Notes:
*
*       File:  mt_ss.c
*
*/
/* mt008.301: ssdDeinitFinal changed to ssdDeinitLog */
#ifdef ANSI
PUBLIC Void ssdDeinitLog
(
void
)
#else
PUBLIC Void ssdDeinitLog()
#endif
{
/* mt008.301: ssdDeinitFinal changed to ssdDeinitLog */
   TRC0(ssdDeinitLog);
#ifdef CONAVL
#ifndef CONRD
  /* mt008.301: Terminate the console reader on exit */
  while(pthread_cancel(osCp.dep.conHdlrTID));
#endif /* CONRD */
#endif /* CONVAL */

   RETVOID;
}
/* mt001.301 : Additions */
#ifdef SS_WATCHDOG


#ifdef ANSI
PUBLIC S16 ssdInitWatchDog
(
U16 port
)
#else
PUBLIC S16 ssdInitWatchDog(port)
U16 port;
#endif
{
   U8 idx;
   Txt prntBuf[PRNTSZE];
   Pst     pst;
   Buffer *mBuf;
#ifdef SS_WATCHDOG_IPV6
   struct sockaddr_in6 tmpaddr;
#else
   struct sockaddr_in tmpaddr;
#endif /* SS_WATCHDOG_IPV6 */
#ifdef SS_MULTIPLE_PROCS
   ProcId procId = SS_WD_WDPROC;
   if (SAddProcIdLst(1, &procId) != ROK)
   {
      RETVALUE(RFAILED);
   }
#endif /* SS_MULTIPLE_PROCS */

   TRC0(ssdInitWatchDog);

   SInitLock(&osCp.wdCp.wdLock, SS_LOCK_MUTEX);

   /* Create a watch dog system task */
   SCreateSTsk(0, &(osCp.wdCp.watchDgTskId));

   /* Create a watch dog reveiver system task */
   SCreateSTsk(0, &(osCp.wdCp.watchDgRcvrTskId));

   /* Register and attach watch dog TAPA task */
#ifdef SS_MULTIPLE_PROCS
   SRegTTsk (procId, ENTDW, INST0, TTNORM, PRIOR0, NULLP, watchDgActvTsk);
   SAttachTTsk (procId, ENTDW, INST0, osCp.wdCp.watchDgTskId);
#else
   SRegTTsk ( ENTDW, INST0, TTNORM, PRIOR0, NULLP, watchDgActvTsk);
   SAttachTTsk ( ENTDW, INST0, osCp.wdCp.watchDgTskId);
#endif /* SS_MULTIPLE_PROCS */
   /* Register and attach watch dog receiver TAPA task */
#ifdef SS_MULTIPLE_PROCS
   SRegTTsk (procId, ENTHB, INST0, TTNORM, PRIOR0, NULLP, watchDgRcvrActvTsk);
   SAttachTTsk (procId, ENTHB, INST0, osCp.wdCp.watchDgRcvrTskId);
#else
   SRegTTsk ( ENTHB, INST0, TTNORM, PRIOR0, NULLP, watchDgRcvrActvTsk);
   SAttachTTsk ( ENTHB, INST0, osCp.wdCp.watchDgRcvrTskId);
#endif /* SS_MULTIPLE_PROCS */

#ifndef SS_MULTIPLE_PROCS
   osCp.wdCp.watchDgPst.srcProcId = SFndProcId();
   osCp.wdCp.watchDgPst.dstProcId = SFndProcId();
#else
   osCp.wdCp.watchDgPst.srcProcId = procId;
   osCp.wdCp.watchDgPst.dstProcId = procId;
#endif /* SS_MULTIPLE_PROCS */

   /* Initialise the pst structure */
   ssdInitWatchDgPst(&(osCp.wdCp.watchDgPst));
   /* Initialize the watch dog timer resolution default is 1 sec */

   cmInitTimers(osCp.wdCp.watchDgTmr, (U8)1);
   osCp.wdCp.watchDgTqCp.nxtEnt = 0;
   osCp.wdCp.watchDgTqCp.tmrLen = 1;
   for(idx = 0; idx < 1; idx++)
   {
      osCp.wdCp.watchDgTs[idx].first = NULLP;
      osCp.wdCp.watchDgTs[idx].tail = NULLP;
   }
#ifdef SS_MULTIPLE_PROCS
   SRegCfgTmr(procId,ENTDW, INST0, 10, SS_100MS, ssdWatchDgActvTmr);
#else
   SRegCfgTmr(ENTDW, INST0, 10, SS_100MS, ssdWatchDgActvTmr);
#endif /* SS_MULTIPLE_PROCS */

   /* Create the watch dog receiver socket */
   osCp.wdCp.globWd.sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
   if(osCp.wdCp.globWd.sock == -1)
   {
      sprintf(prntBuf,"ssdInitWatchDog: socket failed errno [%d]\n", errno);
      SPrint(prntBuf);
   }

#ifdef SS_WATCHDOG_IPV6
   tmpaddr.sin6_len = sizeof(tmpadDr);
   tmpaddr.sin6_family = AF_INET6;
   tmpaddr.sin6_addr = in6addr_any;
   tmpaddr.sin6_port = htons(port);
#else
   tmpaddr.sin_family = AF_INET;
   tmpaddr.sin_addr.s_addr = htonl(INADDR_ANY);
   tmpaddr.sin_port = htons(port);
#endif /* SS_WATCHDOG_IPV6 */

   if(bind(osCp.wdCp.globWd.sock, (struct sockaddr *)&tmpaddr, sizeof(struct sockaddr)) != 0
)
   {
      sprintf(prntBuf,"ssdInitWatchDog: bind failed errno [%d]\n", errno);
      SPrint(prntBuf);
   }

   if (SGetMsg(SS_DFLT_REGION, SS_DFLT_POOL, &mBuf) != ROK)
   {
      RETVALUE(RFAILED);
   }
#ifndef SS_MULTIPLE_PROCS
   pst.srcProcId = SFndProcId();
   pst.dstProcId = SFndProcId();
#else
   pst.srcProcId = procId;
   pst.dstProcId = procId;
#endif /* SS_MULTIPLE_PROCS */
   pst.event     = EVTSSHRTBTREQ;
   ssdInitWatchDgPst(&pst);
   SPstTsk(&pst, mBuf);

   RETVALUE(ROK);
}

#ifdef ANSI
PUBLIC S16 ssdInitWatchDgPst
(
Pst *pst
)
#else
PUBLIC S16 ssdInitWatchDgPst(pst)
Pst *pst;
#endif
{
   TRC1(ssInitWatchDgPst);

   pst->selector  = SS_LOOSE_COUPLING;

   pst->region    = DFLT_REGION;             /* region */
   pst->pool      = DFLT_POOL;                 /* pool */

   pst->prior     = PRIOR0;                   /* priority */
   pst->route     = RTESPEC;                  /* route */

   pst->dstEnt    = ENTHB;                   /* destination entity */
   pst->dstInst   = 0;
   pst->srcEnt    = ENTDW;                   /* source entity */
   pst->srcInst   = 0;

   RETVALUE(ROK);
}

#ifdef SS_MULTIPLE_PROCS
#ifdef ANSI
PUBLIC S16 ssdWatchDgActvTmr
(
ProcId proc,
Ent ent,
Inst inst
)
#else
PUBLIC S16 ssdWatchDgActvTmr(proc, ent, inst)
#endif
#else
#ifdef ANSI
PUBLIC S16 ssdWatchDgActvTmr
(
Void
)
#else
PUBLIC S16 ssdWatchDgActvTmr()
#endif
#endif /* SS_MULTIPLE_PROCS */
{
   TRC3(ssWatchDgActvTmr);

   cmPrcTmr(&osCp.wdCp.watchDgTqCp, osCp.wdCp.watchDgTs, (PFV)ssdWatchDgTmrEvt);

   RETVALUE(ROK);
}

#ifdef ANSI
PUBLIC Void ssdWatchDgTmrEvt
(
PTR       cb,        /* control block */
S16       event      /* timer number */
)
#else
PUBLIC Void ssdWatchDgTmrEvt(cb, event)
PTR       cb;        /* control block */
S16       event;     /* timer number */
#endif
{
/* mt003.301 Fixed warings */
#ifdef DEBUGP
   DateTime dt;
#endif /* DEBUGP */
   Txt prntBuf[PRNTSZE];
   Bool restartTmr;
   int i;

   TRC2(ssWatchDgTmrEvt);

   switch(event)
   {
      case SS_TMR_HRTBT:
#ifdef DEBUGP
        SPrint("Timer Heartbeat Request Expired");
        SGetDateTime(&dt);
        sprintf(prntBuf," Time: %02d:%02d:%02d\n",dt.hour,dt.min, dt.sec);
        SPrint(prntBuf);
#endif
        restartTmr=TRUE;

        SLock(&osCp.wdCp.wdLock);
        for(i=0; i < osCp.wdCp.globWd.numNodes; i++)
        {
           if(osCp.wdCp.globWd.wdsta[i].status == 0)
           {
              sprintf(prntBuf, "Node [ %s ] Down. Calling user callback\n", inet_ntoa(osCp.wdCp.globWd.wdsta[i].addr));
              SPrint(prntBuf);
              if(osCp.wdCp.globWd.callback != 0)
              {
                 osCp.wdCp.globWd.callback(osCp.wdCp.globWd.data);
              }
           }
        }
        SUnlock(&osCp.wdCp.wdLock);

		  if(!osCp.wdCp.globWd.watchdogStop)
		  {
           ssdStartWatchDgTmr(NULLP, SS_TMR_HRTBT, osCp.wdCp.globWd.timeout);
           ssdSndHrtBtMsg(restartTmr, SS_WD_HB_REQ);
		  }
        break;

      default:
         break;
   }

}

#ifdef ANSI
PUBLIC Void ssdStartWatchDgTmr
(
void             *cb,
S16              event,
U16              wait
)
#else
PUBLIC Void ssdStartWatchDgTmr(cb, event, wait)
void             *cb;
S16              event;
U16              wait;
#endif
{
   CmTmrArg    arg;
   int i;
#ifdef DEBUGP
   DateTime dt;
   Txt prntBuf[PRNTSZE];
#endif


   TRC2(ssStartWatchDgTmr)
	/* mt003.301 Modifications */
#ifdef DEBUGP
   SGetDateTime(&dt);
   sprintf(prntBuf," Time: %02d:%02d:%02d\n",dt.hour,dt.min, dt.sec);
   if(event == SS_TMR_HRTBT)
   {
      SPrint("\nSTART SS_TMR_HRTBT");
      SPrint(prntBuf);
   }
#endif

   /* Re-init ACKs */

   SLock(&osCp.wdCp.wdLock);
   for(i=0; i < osCp.wdCp.globWd.numNodes; i++)
   {
      osCp.wdCp.globWd.wdsta[i].status = 0;
   }
   SUnlock(&osCp.wdCp.wdLock);

   arg.tq     = osCp.wdCp.watchDgTs;
   arg.tqCp   = &osCp.wdCp.watchDgTqCp;
   arg.timers = osCp.wdCp.watchDgTmr;
   arg.cb     = (PTR)NULLP;
   arg.evnt   = event;
   arg.wait   = osCp.wdCp.globWd.timeout = wait;
   arg.tNum   = NOTUSED;
   arg.max    = 1;
   cmPlcCbTq(&arg);

   RETVOID;
}

#ifdef ANSI
PUBLIC Void ssdStopWatchDgTmr
(
void             *cb,
S16              event
)
#else
PUBLIC Void ssdStopWatchDgTmr(cb, event)
void             *cb;
S16              event;
#endif
{
   CmTmrArg    arg;
#ifdef DEBUGP
   DateTime dt;
   Txt prntBuf[PRNTSZE];
   int i;
#endif

   TRC2(ssStopWatchDgTmr)
	/* mt003.301 Modifications */
#ifdef DEBUGP
   SGetDateTime(&dt);
   sprintf(prntBuf," Time: %02d:%02d:%02d\n",dt.hour,dt.min, dt.sec);
   if(event == SS_TMR_HRTBT)
   {
      SPrint("STOP SS_TMR_HRTBT");
      SPrint(prntBuf);
   }
   /* Re-init ACKs */
   SLock(&osCp.wdCp.wdLock);
   for(i=0; i < osCp.wdCp.globWd.numNodes; i++)
   {
      osCp.wdCp.globWd.wdsta[i].status = 0;
   }
   SUnlock(&osCp.wdCp.wdLock);

#endif
   arg.tq     = osCp.wdCp.watchDgTs;
   arg.tqCp   = &osCp.wdCp.watchDgTqCp;
   arg.timers = osCp.wdCp.watchDgTmr;
   arg.cb     = (PTR)NULLP;
   arg.evnt   = event;
   arg.wait   = NOTUSED;
   arg.tNum   = NOTUSED;
   arg.max    = 1;
   cmRmvCbTq(&arg);

   RETVOID;
}

#ifdef ANSI
PUBLIC S16 ssdSndHrtBtMsg
(
Bool             restart,
U32              type
)
#else
PUBLIC S16 ssdSndHrtBtMsg(restart, type)
Bool             restart;
U32              type;
#endif
{
   S16     ret = ROK;
#ifdef DEBUGP
   DateTime dt;
   Txt prntBuf[PRNTSZE];
#endif
   struct sockaddr_in tmpaddr;
   char hbMsg[SS_WD_HB_MSG_SIZE];
   int             n;
   int             err;

   TRC2(ssdSndHrtBtReq)

#ifdef DEBUGP
   SGetDateTime(&dt);
   sprintf(prntBuf,"TX HEARTBEAT REQ Time: %02d:%02d:%02d\n", dt.hour, dt.min, dt.sec);
   SPrint(prntBuf);
#endif

   /* Pack the message */
   strcpy(hbMsg, "<HB>REQ</HB>");

   /* Send the heartbeat messages to all the configured nodes */
   SLock(&osCp.wdCp.wdLock);
   for (n=0; n < osCp.wdCp.globWd.numNodes; n++)
   {
      if(osCp.wdCp.globWd.wdsta[n].addr.s_addr == 0)
      {
         continue;
      }

      /* Identify the destination node */
#ifdef SS_WATCHDOG_IPV6
      tmpaddr.sin6_len = sizeof(tmpaddr);
      tmpaddr.sin6_family = AF_INET6;
      tmpaddr.sin6_addr = osCp.wdCp.globWd.wdsta[n].addr;
      tmpaddr.sin_port = osCp.wdCp.globWd.wdsta[n].port;
#else
      tmpaddr.sin_family = AF_INET;
      tmpaddr.sin_addr.s_addr = osCp.wdCp.globWd.wdsta[n].addr.s_addr;
      tmpaddr.sin_port = osCp.wdCp.globWd.wdsta[n].port;
#endif /* SS_WATCHDOG_IPV6 */

      err = sendto(osCp.wdCp.globWd.sock, hbMsg, strlen(hbMsg), 0, (struct sockaddr *)&tmpaddr, sizeof(struct sockaddr));
      if(err == -1)
      {
#ifdef DEBUGP
      sprintf(prntBuf,"ssdSndHrtBtMsg: HB to node [%s:%d] failed status[%d]\n",
                      inet_ntoa(tmpaddr.sin_addr), tmpaddr.sin_port, errno);
      SPrint(prntBuf);
#endif /* DEBUGP */
      }
      else
      {
#ifdef DEBUGP
      sprintf(prntBuf,"ssdSndHrtBtMsg: HB to node [%s:%d] sent[%d]\n", inet_ntoa(tmpaddr.sin_addr), tmpaddr.sin_port, err);
      SPrint(prntBuf);
#endif /* DEBUGP */
      }
   }
   SUnlock(&osCp.wdCp.wdLock);

   RETVALUE(ret);
}

#endif /* SS_WATCHDOG */



/* mt022.201 - Modification to fix problem when NOCMDLINE is defined */
#ifndef NOCMDLINE
/*
*
*       Fun:   mtGetOpts
*
*       Desc:  This function gets command line options.
*
*       Ret:
*
*       Notes:
*
*       File:  mt_ss.c
*
*/
#ifdef ANSI
PRIVATE Void mtGetOpts
(
void
)
#else
PRIVATE Void mtGetOpts()
#endif
{
   S32 argc;
   S8 **argv;
   S16 ret;
   /* mt028.201 */
#ifndef NOFILESYS
   FILE         *memOpt;             /* memory options file pointer */
   Txt pBuf[128];
   U8 i;
/* mt007.301 : Fix related to file based mem config on 64 bit machine */
   PTR numReg;
   PTR numBkts;
   PTR bktSz = NULLP;
   PTR heapSz=0;
   PTR bktNum;
   /*KWORK_FIX: Initializing the variable for avoidning corruption */
   PTR bktIdx = 0;
   /*mt010.301 Fix for reading the variables on 64 bit/32bit platforms correctly */
   PTR bktUpdtCnt = 0;
   PTR regUpdtCnt = 0;
   PTR regId;
   PTR numPools;
   PTR poolIdx;
#ifdef SS_LOCKLESS_MEMORY
   PTR bktSetSize;
   PTR bktRelThr;
   PTR bktAqurThr;
#endif
   Txt line[256];
   Bool error = FALSE;
   Cntr   idx=0;
#endif

   TRC0(mtGetOpts);


   msOptInd = 1;

#ifndef NOFILESYS
   osCp.dep.fileOutFp = (FILE *)NULLP;

   /* initialize memOpt */
   memOpt = (FILE *) NULLP;
#endif

   argc = msArgc;
   argv = msArgv;

   /* mt028.201 */
   while ((ret = SGetOpt(argc, argv, "o:f:s:m:c:")) != EOF)
   {
      switch (ret)
      {
#ifndef NOFILESYS
/* mt001.301 : Additions */
#ifdef SS_MEM_LEAK_STS
         case 'm':
            cmMemOpenMemLkFile(msOptArg);
            break;
#endif
         case 'o':
            osCp.dep.fileOutFp = fopen(msOptArg, "w");
            break;
         case 'f':
				fileBasedMemCfg = TRUE;
            memOpt = fopen(msOptArg, "r");

            /* if file does not exist or could not be opened then use the
             * default memory configuration as defined in mt_ss.h
             */
            if (memOpt == (FILE *) NULLP)
            {
               sprintf(pBuf, "\nMTSS: Memory configuration file: %s could not\
									be opened, using default mem configuration\n", msOptArg);
               SPrint(pBuf);
               break;
            }
            i = 0;
            while (fgets((Txt *)line, 256, memOpt) != NULLP)
            {
               if(line[0] == '#' || line[0] < '0' || line[0] > '9') /* Comment line or non numeric character, so skip it and read next line */
                   continue;
               if(error == TRUE)
                   break;
               switch  (i)
               {
                  case 0:  /*** INPUT: Number of regions ***/
                     sscanf(line, "%ld", (long *) &numReg);
                     mtMemoCfg.numRegions = numReg;
                     if(mtMemoCfg.numRegions > SS_MAX_REGS)
                     {
								printf("\n No. of regions are > SS_MAX_REGS:%d \n",SS_MAX_REGS);
                        error = TRUE;
                        break;
                     }
                     i++;
                     break;
                  case 1: /*** INPUT: Number of buckets and number of Pools ***/
							sscanf(line, "%ld %ld", (long *) &numBkts, (long *) &numPools);
							if(numBkts > MT_MAX_BKTS)
							{
							  printf("\n No. of buckets are > MT_MAX_BKTS :%d \n",MT_MAX_BKTS);
							  error = TRUE;
							  break;
							}
							if(numPools > SS_MAX_POOLS_PER_REG)
							{
							  printf("\n No. of pools are > SS_MAX_POOLS_PER_REG:%d \n",SS_MAX_POOLS_PER_REG);
							  error = TRUE;
							  break;
							}
							/*
							 * Delay updation from local variable to global
							 * structure of number of regions and heap data to
							 * counter error conditions present above.
							 */
							for(idx = 0; idx < cfgNumRegs; idx++)
							{
							  mtMemoCfg.region[idx].numBkts = numBkts;
							  cfgRegInfo[idx].region = idx;
							  cfgRegInfo[idx].numPools = numPools;
							  /*
								* Initialize the pool info as static type with size zero
								*/
							  for(poolIdx = 0; poolIdx < numPools; poolIdx++)
							  {
                          cfgRegInfo[idx].pools[poolIdx].type = SS_POOL_STATIC;
                          cfgRegInfo[idx].pools[poolIdx].size = 0;
							  }
							}
							i++;
                     break;
                  case 2: /*** INPUT: Bucket Id and size of the bucket ***/
							if(bktUpdtCnt < numBkts) /* more set of bucket can be added */
							{
								sscanf(line, "%ld %ld",(long *)&bktIdx, (long *) &bktSz);
							}
							if(bktIdx >= numBkts)
							{
							  printf("\n Invalid Bucket Id, may be >= the No. of buckets:%ld\n",numBkts);
							  error = TRUE;
							  break;

							}
							mtBktInfo[bktIdx].blkSize  = bktSz;
							bktUpdtCnt++;
							if(bktUpdtCnt == numBkts)
							{
							  i++; /*done reading bkt info, start reading individual region info*/
							  bktUpdtCnt = 0;
							}
                     break;
                     case 3: /*** INPUT: Region Id (ranges from 0 to numRegions-1) **/
                     sscanf(line,"%ld",(long *) &regId);
                     if(regId >= mtMemoCfg.numRegions)
                     {
                       printf("\n Invalid Region Id, may be >= the No. of regions:%d\n",mtMemoCfg.numRegions);
#ifndef XEON_SPECIFIC_CHANGES                       
                       error = TRUE;
#endif                       
                       break;
                     }
                     mtMemoCfg.region[regId].regionId = regId;
                     i++;
                     break;
                     case 4: /*** INPUT: BktId (ranges from 0 to numBkts-1), No. of blks ***/
                     if(bktUpdtCnt < numBkts)
                     {
                       sscanf(line, "%ld %ld",(long *)&bktIdx, (long *)&bktNum);
                       if(bktIdx >= numBkts)
                       {
                         printf("\n Invalid Bucket Id, may be >= the No. of buckets:%ld\n",numBkts);
                         error = TRUE;
                         break;

                       }
                       if(bktIdx < MT_MAX_BKTS)
                       {
                         mtMemoCfg.region[regId].bkt[bktIdx].blkSize = mtBktInfo[bktIdx].blkSize;
                         mtMemoCfg.region[regId].bkt[bktIdx].numBlks = bktNum;
                         cfgRegInfo[regId].pools[bktIdx].type = SS_POOL_DYNAMIC;
                         cfgRegInfo[regId].pools[bktIdx].size = mtBktInfo[bktIdx].blkSize - (sizeof(SsMblk)+sizeof(SsDblk));
                       }
                       bktUpdtCnt++;
                       if(bktUpdtCnt == numBkts)
                       {
                          i++;
                        bktUpdtCnt = 0;
                       }
                     }
                     break;
                     case 5: /* INPUT: Heapsize ***/
                     sscanf(line, "%ld", (long *) &heapSz);
                     mtMemoCfg.region[regId].heapsize = heapSz;
                     regUpdtCnt++;
                     if(regUpdtCnt != mtMemoCfg.numRegions)
                     {
                        i = 3;   
                     }
                     else
                     {
                        i++;
                     }
                     break;
#ifdef SS_LOCKLESS_MEMORY
                     case 6:
                     sscanf(line, "%ld", (long *) &numBkts);
                     mtGlobMemoCfg.numBkts = numBkts;
#ifndef XEON_SPECIFIC_CHANGES                     
                     mtDynMemoCfg.numRegions = mtMemoCfg.numRegions;
#endif                     

#ifdef XEON_SPECIFIC_CHANGES
                     CM_LOG_DEBUG(CM_LOG_ID_MT, "numRegions = %d numBkts = %d\n",
                                    mtDynMemoCfg.numRegions, mtGlobMemoCfg.numBkts);
                     for(idx = 0; idx < mtDynMemoCfg.numRegions; idx++)
#else                     
                     for(idx = 0; idx < mtMemoCfg.numRegions; idx++)
#endif                        
                     {
                        mtDynMemoCfg.region[idx].regionId = idx;
                        mtDynMemoCfg.region[idx].numBkts = numBkts;
                     }

                     bktUpdtCnt = 0;
                     i++;
                     break;

                     case 7:
                     if(bktUpdtCnt < numBkts)
                     {
                        sscanf(line, "%ld %ld %ld %ld %ld %ld", (long *) &bktIdx,
                                      (long *) &bktSz, (long *) &bktNum,
                                      (long *) &bktSetSize, (long *) &bktRelThr, 
                                      (long *) &bktAqurThr);
                        /* Klock work fix ccpu00148484 */
                        if(bktIdx < SS_MAX_POOLS_PER_REG)
                        {
                           mtGlobMemoCfg.bkt[bktIdx].blkSize = bktSz;
                           mtGlobMemoCfg.bkt[bktIdx].numBlks = bktNum;
                           mtGlobMemoCfg.bkt[bktIdx].bucketSetSize = bktSetSize;
#ifdef XEON_SPECIFIC_CHANGES
                           CM_LOG_DEBUG(CM_LOG_ID_MT, "Pool [%d] blkSize %d numBlks %d bucketSetSize %d\n",
                                 bktUpdtCnt, mtGlobMemoCfg.bkt[bktIdx].blkSize,
                                 mtGlobMemoCfg.bkt[bktIdx].numBlks, mtGlobMemoCfg.bkt[bktIdx].bucketSetSize); 

                        if(bktIdx >= SS_MAX_POOLS_PER_REG)
                        {
                           printf("\nNo. of Buckets/pools are > SS_MAX_POOLS_PER_REG:%d\n",SS_MAX_POOLS_PER_REG);
                           error = TRUE;
                           break;
                        }

#endif                           
                        for(idx = 0; idx < mtMemoCfg.numRegions; idx++)
                        {
                           mtDynMemoCfg.region[idx].bkt[bktIdx].blkSetRelThreshold = bktRelThr;
                           mtDynMemoCfg.region[idx].bkt[bktIdx].blkSetAcquireThreshold = bktAqurThr;
#ifdef XEON_SPECIFIC_CHANGES
                           CM_LOG_DEBUG(CM_LOG_ID_MT, "Pool [%d] blkSetRelThreshold %d blkSetAcquireThreshold %d\n",
                                 bktUpdtCnt, mtDynMemoCfg.region[idx].bkt[bktIdx].blkSetRelThreshold,
                                 mtDynMemoCfg.region[idx].bkt[bktIdx].blkSetAcquireThreshold);
#endif                           
                        } 
                        }
                        bktUpdtCnt++;
                     }
#ifdef XEON_SPECIFIC_CHANGES
                     if(bktUpdtCnt == numBkts)
                     {   
                        i=8;
                     }
                     break;

                     case 8: /* INPUT: Global Heapsize ***/
                     sscanf(line, "%ld", (long *) &heapSz);
                     mtGlobMemoCfg.heapSize = heapSz;
                     CM_LOG_DEBUG(CM_LOG_ID_MT, "Global Heap size = %d\n", mtGlobMemoCfg.heapSize);
#endif                           
                     break;
#endif
               }
            }
            if(error == TRUE)
            {
               memConfigured = FALSE;
            }
            else
            {
               memConfigured = TRUE;
            }
            fclose (memOpt);
            break;
#endif


         case 's':
/* mt028.201: modification: multiple procs support related changes */
#ifndef SS_MULTIPLE_PROCS

#ifdef ENB_RELAY
            osCp.procId = PID_STK((ProcId) strtol(msOptArg, NULLP, 0));
#else
            osCp.procId = (ProcId) strtol(msOptArg, NULLP, 0);
#endif

#else /* SS_MULTIPLE_PROCS */
        {
           ProcId procId;
#ifdef ENB_RELAY
            procId = PID_STK((ProcId) strtol(msOptArg, NULLP, 0));
#else
            procId = (ProcId) strtol(msOptArg, NULLP, 0);
#endif
           SAddProcIdLst(1, &procId);
        }

#endif /* SS_MULTIPLE_PROCS */
            break;

         case 'c':
            osCp.configFilePath = msOptArg;
            break;

         case '?':
            /* fall through */


         default:
            break;
      }
   }

   msOptInd = 1;


   RETVOID;
}
#endif


/*
*
*       Fun:   SGetOpt
*
*       Desc:  Get options from command line
*
*       Ret:   option  - success
*              '?'     - fail
*              EOF     - end of options
*
*       Notes: Handles command lines like the following
*
*              if opts = "abc:d"
*                 then command line should look like this...
*                    -a foo -b foo1 -c -d foo
*
*              code usage:
*
*              while ((ret = SGetOpt(msArgc, msArgv, "ls")) != EOF )
*              {
*                 switch(ret)
*                 {
*                    case 'l':
*                       nloops = atoi(msArgv[msOptInd]);
*                       break;
*                    case 's':
*                       state1 = atoi(msArgv[msOptInd]);
*                    case '?':
*                    default:
*                       break;
*                 }
*              }
*
*       File:  mt_ss.c
*
*/
#ifdef ANSI
PUBLIC S16 SGetOpt
(
int argc,                   /* argument count */
char **argv,                /* argument value */
char *opts                  /* options */
)
#else
PUBLIC S16 SGetOpt(argc, argv, opts)
int argc;                   /* argument count */
char **argv;                /* argument value */
char *opts;                 /* options */
#endif
{
   /* mt020.201 - Removed for no command line */
#ifndef NOCMDLINE
   S16 sp;
   S16 c;
   S8 *cp;
#endif

   TRC1(SGetOpt);

   /* mt020.201 - Addition for no command line */
#ifdef NOCMDLINE
   UNUSED(argc);
   UNUSED(argv);
   UNUSED(opts);

   RETVALUE(EOF);
#else

   sp = 1;
   if (sp == 1)
   {
       /*mt013.301 : Changes as per coding standards*/
      if (msOptInd >= (S16) argc  ||  argv[msOptInd][0] == '\0')
      {
          RETVALUE(EOF);
      }
      else
      {
         if (!strcmp(argv[msOptInd], "--"))
         {
            msOptInd++;
            RETVALUE(EOF);
         }
         else if (argv[msOptInd][0] != '-')
         {
            msOptInd++;
            RETVALUE('?');
         }
      }
   }

   c = argv[msOptInd][sp];
   if (c == ':'  ||  (cp = (S8 *) strchr(opts, c)) == (S8 *) NULLP)
   {
      if (argv[msOptInd][++sp] == '\0')
      {
         msOptInd++;
         sp = 1;
      }

      RETVALUE('?');
   }

   if (*++cp == ':')
   {
      if (argv[msOptInd][sp+1] != '\0') msOptArg = &argv[msOptInd++][sp+1];
      else
      {
         if (++msOptInd >= (S16) argc)
         {
            sp = 1;
            RETVALUE('?');
         }
         else msOptArg = argv[msOptInd++];

         sp = 1;
      }
   }
   else
   {
      if (argv[msOptInd][++sp] == '\0')
      {
         sp = 1;
         msOptInd++;
      }

      msOptArg = NULLP;
   }


   RETVALUE(c);

#endif /* NOCMDLINE */
}


/*
*
*       Fun:   ssdStart
*
*       Desc:  This function starts system services execution; the
*              permanent tasks are started and the system enters a
*              ready state.
*
*       Ret:   Void
*
*       Notes:
*
*       File:  mt_ss.c
*
*/
#ifdef ANSI
PUBLIC Void ssdStart
(
void
)
#else
PUBLIC Void ssdStart()
#endif
{
   S16 i;


   TRC0(ssdStart);


   /* mt025.201 - Modification for adding lock to timer handler */
   for (i = 0;  i <= SS_MAX_STSKS + 5;  i++)
   {
      sem_post(&osCp.dep.ssStarted);
   }


   RETVOID;
}


/*
*     indirect interface functions to system services service user
*/


/*
*
*       Fun:   ssdAttachTTsk
*
*       Desc:  This function sends the initial tick message to a TAPA
*              task if the task is a permanent task.
*
*       Ret:   ROK      - ok
*
*       Notes:
*
*       File:  mt_ss.c
*
*/
#ifdef ANSI
PUBLIC S16 ssdAttachTTsk
(
SsTTskEntry *tTsk           /* pointer to TAPA task entry */
)
#else
PUBLIC S16 ssdAttachTTsk(tTsk)
SsTTskEntry *tTsk;          /* pointer to TAPA task entry */
#endif
{
   Buffer *mBuf;
   SsMsgInfo *mInfo;
   S16 ret;


   TRC0(ssdAttachTTsk);


   if (tTsk->tskType == SS_TSK_PERMANENT)
   {
      /* Send a permanent tick message to this task, to start
       * execution.
       */
      ret = SGetMsg(SS_DFLT_REGION, SS_DFLT_POOL, &mBuf);
      if (ret != ROK)
      {
#if (ERRCLASS & ERRCLS_DEBUG)
         MTLOGERROR(ERRCLS_DEBUG, EMT001, ret, "SGetMsg() failed");
#endif
         RETVALUE(RFAILED);
      }

      mInfo = (SsMsgInfo *)mBuf->b_rptr;
      mInfo->eventInfo.event = SS_EVNT_PERMTICK;

      /* set up post structure */
/* mt028.201: modification: multiple procs support related changes */
#ifndef SS_MULTIPLE_PROCS
      mInfo->pst.dstProcId = SFndProcId();
      mInfo->pst.srcProcId = SFndProcId();
#else /* SS_MULTIPLE_PROCS */
      mInfo->pst.dstProcId = tTsk->proc;
      mInfo->pst.srcProcId = tTsk->proc;
#endif /* SS_MULTIPLE_PROCS */
      mInfo->pst.selector  = SEL_LC_NEW;
      mInfo->pst.region    = DFLT_REGION;
      mInfo->pst.pool      = DFLT_POOL;
      mInfo->pst.prior     = PRIOR3;
      mInfo->pst.route     = RTESPEC;
      mInfo->pst.event     = 0;
      mInfo->pst.dstEnt    = tTsk->ent;
      mInfo->pst.dstInst   = tTsk->inst;
      mInfo->pst.srcEnt    = tTsk->ent;
      mInfo->pst.srcInst   = tTsk->inst;

      ret = ssDmndQPutLast(&tTsk->sTsk->dQ, mBuf,
                           (tTsk->tskPrior * SS_MAX_MSG_PRI) + PRIOR3);

      if (ret != ROK)
      {
         SPutMsg(mBuf);

#if (ERRCLASS & ERRCLS_DEBUG)
         MTLOGERROR(ERRCLS_DEBUG, EMT002, ret,
                        "Could not write to demand queue");
#endif
         RETVALUE(RFAILED);
      }
   }


   RETVALUE(ROK);
}


/*
*
*       Fun:   ssdDetachTTsk
*
*       Desc:  Does nothing.
*
*       Ret:   ROK      - ok
*
*       Notes:
*
*       File:  mt_ss.c
*
*/
#ifdef ANSI
PUBLIC S16 ssdDetachTTsk
(
SsTTskEntry *tTsk           /* pointer to TAPA task entry */
)
#else
PUBLIC S16 ssdDetachTTsk(tTsk)
SsTTskEntry *tTsk;          /* pointer to TAPA task entry */
#endif
{
   TRC0(ssdDetachTTsk);


   RETVALUE(ROK);
}


/*
*
*       Fun:   ssdCreateSTsk
*
*       Desc:  This function creates a system task. A thread is started
*              on the system task handler function defined later.
*
*       Ret:   ROK      - ok
*
*       Notes:
*
*       File:  mt_ss.c
*
*/
#ifdef ANSI
PUBLIC S16 ssdCreateSTsk
(
SsSTskEntry *sTsk           /* pointer to system task entry */
)
#else
PUBLIC S16 ssdCreateSTsk(sTsk)
SsSTskEntry *sTsk;          /* pointer to system task entry */
#endif
{
   pthread_attr_t attr;
   /* struct sched_param param_sched;*/

#ifdef SS_THR_REG_MAP
   U32 threadCreated = FALSE;
#endif

   TRC0(ssdCreateSTsk);


#ifdef SS_SINGLE_THREADED
/* mt001.301 : Additions */
#ifndef SS_WATCHDOG
#ifdef SS_MULTICORE_SUPPORT
   if (osCp.numSTsks > 1)
#else
   if (osCp.numSTsks > 0)
#endif /* SS_MULTICORE_SUPPORT */
#else
#ifdef SS_MULTICORE_SUPPORT
   if (osCp.numSTsks > 3)
#else
   if (osCp.numSTsks > 2)
#endif /* SS_MULTICORE_SUPPORT */
#endif /* SS_WATCHDOG */
   {
      RETVALUE(ROK);
   }
#endif


   /* set the current executing entity and instance IDs to
    * 'not configured'. create the lock to access them.
    */
   sTsk->dep.ent = ENTNC;
   sTsk->dep.inst = INSTNC;


   /* create the thread */
   pthread_attr_init(&attr);
   ssdSetPthreadAttr(sTsk->tskPrior, &attr);

   printf("Creating thread here %s %d\n", __FILE__, __LINE__);
#ifdef TENB_T2K3K_SPECIFIC_CHANGES
   if (sTsk->tskPrior == 0)
   {
      printf("Creating RT thread #######################\n");
#ifdef SS_THR_REG_MAP
      /* When the thread is created, we check for the memory mapping table if
       * threadId can be placed in thread memory map table. If it is not able to place
       * threadId is stored in tmporary array. Once thread is created successful,
       * thread_cancel is sent for each thread which are created before. All the 
       * threads are made to wait on sema which is cancel point for thread.
       */
      while(threadCreated == FALSE)
      {
#endif
         if ((pthread_create(&sTsk->dep.tId, &attr, mtTskHdlrT2kL2, (Ptr)sTsk)) != 0)
         {

            pthread_attr_destroy(&attr);

#if (ERRCLASS & ERRCLS_DEBUG)
            MTLOGERROR(ERRCLS_DEBUG, EMT004, ERRZERO, "Could not create thread");
#endif

            RETVALUE(RFAILED);
         }
#ifdef SS_THR_REG_MAP
         threadCreated = ssCheckAndAddMemoryRegionMap(sTsk->dep.tId, 
                                                      sTsk->region);
      }
#endif
   }
   else
#endif
   {
#ifdef SS_THR_REG_MAP
      /* When the thread is created, we check for the memory mapping table if
       * threadId can be placed in thread memory map table. If it is not able to place
       * threadId is stored in tmporary array. Once thread is created successful,
       * thread_cancel is sent for each thread which are created before. All the 
       * threads are made to wait on sema which is cancel point for thread.
       */
      while(threadCreated == FALSE)
      {
#endif
         if ((pthread_create(&sTsk->dep.tId, &attr, mtTskHdlr, (Ptr)sTsk)) != 0)
         {

            /* mt020.201 - Addition for destroying thread attribute object attr */
            pthread_attr_destroy(&attr);

#if (ERRCLASS & ERRCLS_DEBUG)
            MTLOGERROR(ERRCLS_DEBUG, EMT004, ERRZERO, "Could not create thread");
#endif

            RETVALUE(RFAILED);
         }
#ifdef SS_THR_REG_MAP
         threadCreated = ssCheckAndAddMemoryRegionMap(sTsk->dep.tId, 
                                                      sTsk->region);
      }
#endif
   }


/*mt013.301 :Added SS_AFFINITY_SUPPORT  */
#if defined(SS_MULTICORE_SUPPORT) ||defined(SS_AFFINITY_SUPPORT)
   {
     static U32 stLwpId = 3;
     sTsk->dep.lwpId = ++stLwpId;
   }
#endif /* SS_MULTICORE_SUPPORT || SS_AFFINITY_SUPPORT */

   /* mt020.201 - Addition for destroying thread attribute object attr */
   pthread_attr_destroy(&attr);

   RETVALUE(ROK);
}


#ifdef ANSI
PUBLIC int SCreatePThread
(
pthread_t* tid,
pthread_attr_t* attr,
void *(*start_routine) (void *),
void* arg
)
#else
PUBLIC int SCreatePThread(tid, attr, start_routine, arg)
pthread_t* tid;
pthread_attr_t* attr;
void *(*start_routine) (void *);
void* arg;
#endif
{
   int retVal = 0;
#ifdef SS_THR_REG_MAP
   U32 threadCreated = FALSE;
#endif

   SPThreadCreateArg* threadArg = (SPThreadCreateArg*)malloc(sizeof(SPThreadCreateArg));
   /* Klock work fix ccpu00148484 */
   if(threadArg == NULLP)
   {
      RETVALUE(RFAILED);
   }
   threadArg->argument = arg;
   threadArg->start_routine = start_routine;
   
   TRC0(SCreatePThread);

   printf("Creating thread here %s %d\n", __FILE__, __LINE__);
   {
#ifdef SS_THR_REG_MAP
      /* When the thread is created, we check for the memory mapping table if
       * threadId can be placed in thread memory map table. If it is not able to place
       * threadId is stored in tmporary array. Once thread is created successful,
       * thread_cancel is sent for each thread which are created before. All the 
       * threads are made to wait on sema which is cancel point for thread.
       */
      while(threadCreated == FALSE)
      {
#endif
         /*pthreadCreateHdlr */
         if (((retVal = pthread_create(tid, attr, pthreadCreateHdlr, threadArg))) != 0)
         {

            RETVALUE(retVal);
         }
#ifdef SS_THR_REG_MAP
         threadCreated = ssCheckAndAddMemoryRegionMap(*tid, SS_MAX_REGS - 1);
      }
#endif
   }

   RETVALUE(retVal);
}


/*
*
*       Fun:   Set Pthread Attributes
*
*       Desc:  This function is used to set various explicit
*              pthread attributes like, priority scheduling,etc
*
*       Ret:   ROK      - ok
*
*       Notes:
*
*       File:  mt_ss.c
*
*/

#ifdef ANSI
PRIVATE S16 ssdSetPthreadAttr
(
S32              tskPrior,
pthread_attr_t  *attr
)
#else
PRIVATE S16 ssdSetPthreadAttr(sTsk, attr)
S32               tskPrior,
pthread_attr_t   *attr
#endif
{
   struct sched_param    param;

   TRC0 (ssdSetPthreadAttr)

   SMemSet(&param, 0, sizeof(param));

#ifndef TENB_T2K3K_SPECIFIC_CHANGES
   param.sched_priority = 100 - 1 - tskPrior;
#else
   param.sched_priority = 100 - 10 - tskPrior;
#endif

#if 1/* Nawas:: Overriding DL RLC prority to one higher than iccserv */
   /* TODO:: This can be avoided by reducing the priority
    * of iccserv thread in l1_master.sh*/
#ifdef L2_L3_SPLIT
         if (clusterMode == RADIO_CLUSTER_MODE)
         {
            if(tskPrior == PRIOR1)
            {/* DL RLC */
               param.sched_priority = 91;
            }
         }

#endif
#endif

   printf("Set priority %u\n", param.sched_priority);

   /* Set Scheduler to explicit, without this non of the below
      pthread attr works */
#ifdef  TENB_RTLIN_CHANGES
   pthread_attr_setinheritsched(attr, PTHREAD_EXPLICIT_SCHED);
#endif

   pthread_attr_setstacksize(attr, (size_t)MT_TASK_STACK);
   pthread_attr_setscope(attr, PTHREAD_SCOPE_SYSTEM);
   pthread_attr_setdetachstate(attr, PTHREAD_CREATE_DETACHED);
#ifdef TENB_RTLIN_CHANGES
   pthread_attr_setschedpolicy(attr, SCHED_FIFO);
#endif
   pthread_attr_setschedparam(attr, &param);

   RETVALUE (ROK);

} /* ssdSetPthreadAttr */

/************* multi-core support **************/
/*mt013.301 :Added SS_AFFINITY_SUPPORT  */
#if defined(SS_MULTICORE_SUPPORT) ||defined(SS_AFFINITY_SUPPORT)

/*
*
*       Fun:   Get the current core/cpu affinity for a thread/lwp
*
*       Desc:  This function is used to get the current processor/core
*              affinity for a a system task (thread/lwp). It sets the
*              affinity based on the mode supplied by the caller.
*
*       Ret:   ROK      - ok
*              RFAILED  - failed, general (optional)
*
*       Notes:
*
*       File:  ss_task.c
*
*/
#ifdef ANSI
PUBLIC S16 ssdGetAffinity
(
SSTskId *tskId,                  /* filled in with system task ID */
U32 *coreId                      /* the core/processor id to which the affinity is set */
)
#else
PUBLIC S16 ssdGetAffinity(tskId, coreId)
SSTskId *tskId;                 /* filled in with system task ID */
U32 *coreId;                    /* the core/processor id to which the affinity is set */
#endif
{

   U32 tskInd;

#ifdef SS_LINUX

   pthread_t tId =0;
   cpu_set_t cpuSet;
   U32 cpuInd = 0;
   /*mt013.301 :Fix for TRACE5 feature crash due to missing TRC MACRO*/
#else
#ifdef SUNOS
   U32 lwpId = *tskId;
#endif /*SUNOS*/
#endif /*SS_LINUX*/
   TRC0(ssdGetAffinity);
#ifdef SS_LINUX
   for (tskInd = 0;  tskInd < SS_MAX_STSKS;  tskInd++)
   {
      if (osCp.sTskTbl[tskInd].tskId == *tskId)
      {
         tId = osCp.sTskTbl[tskInd].dep.tId;
         break;
      } /* end if */
   } /* end for */

   /* if tskId is not found in the tskTbl */
   if (tskInd == SS_MAX_STSKS)
   {
       MTLOGERROR(ERRCLS_DEBUG, EMT036, ERRZERO, "Invalid system task Id\n");
       RETVALUE(RFAILED);
   }


   /* initialize the cpu mask */
   CPU_ZERO( &cpuSet);

   /* set thread affinity for linux */
    if (pthread_getaffinity_np(tId, sizeof(cpuSet), &cpuSet) < 0)
    {
#if (ERRCLASS & ERRCLS_DEBUG)
        MTLOGERROR(ERRCLS_DEBUG, EMT037, ERRZERO, "Could not get thread affinity\n");
#endif
       RETVALUE(RFAILED);
    } /* end if pthread_setaffinity fails */

   for (cpuInd = 0; cpuInd <CPU_SETSIZE; cpuInd++)
   {
       if (CPU_ISSET (cpuInd, & cpuSet))
       {
           *coreId = cpuInd;
           break;
       } /* end if */
   } /* end for */

#else
#ifdef SUNOS
   for (tskInd = 0;  tskInd < SS_MAX_STSKS;  tskInd++)
   {
      if (osCp.sTskTbl[tskInd].tskId == *tskId)
      {
         lwpId = osCp.sTskTbl[tskInd].dep.lwpId;
         break;
      } /* end if */
   } /* end for */

   /* if tskId is not found in the tskTbl */
   if (tskInd == SS_MAX_STSKS)
   {
       MTLOGERROR(ERRCLS_DEBUG, EMT036, ERRZERO, "Invalid system task Id\n");
       RETVALUE(RFAILED);
   }

   /* set thread affinity for Solaris */
   if (processor_bind(P_LWPID, lwpId, PBIND_QUERY, (processorid_t*)coreId) < 0)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      MTLOGERROR(ERRCLS_DEBUG, EMT037, ERRZERO, "Could not get thread affinity\n");
#endif
      RETVALUE(RFAILED);
   } /* end if processor_bind fails */

#endif /* SUNOS */
#endif /* SS_LINUX */

   RETVALUE(ROK);

} /* ssdGetAffinity */


/*
*
*       Fun:   Set the core/cpu affinity for a thread/lwp
*
*       Desc:  This function is used to set processor/core affinity for a
*              a system task (thread/lwp). It sets the affinity based on the
*              mode supplied by the caller.
*
*       Ret:   ROK      - ok
*              RFAILED  - failed, general (optional)
*
*       Notes:
*
*       File:  ss_task.c
*
*/
#ifdef ANSI
PUBLIC S16 ssdSetAffinity
(
SSTskId *tskId,                  /* filled in with system task ID */
U32 coreId                       /* the core/processor id to which the affinity has to be set */
)
#else
PUBLIC S16 ssdSetAffinity(tskId, coreId)
SSTskId *tskId;                 /* filled in with system task ID */
U32 coreId;                     /* the core/processor id to which the affinity has to be set */
#endif
{

   U32 tskInd = 0;
#ifdef SS_LINUX

   pthread_t tId = 0;
   cpu_set_t cpuSet;
   /*mt013.301 :Fix for TRACE5 feature crash due to missing TRC MACRO*/
#else
#ifdef SUNOS
   U32 lwpId = *tskId;
#endif /*SUNOS*/
#endif /*SS_LINUX*/

   TRC0(ssdSetAffinity)

#ifdef SS_LINUX
   for (tskInd = 0;  tskInd < SS_MAX_STSKS;  tskInd++)
   {
   /* Here tskId can not be used as index as the task may be terminated if
      there is a TERM even for that tsk, thus breaking the task Id numbering
      sequence  */
      if (osCp.sTskTbl[tskInd].tskId == *tskId)
      {
         tId = osCp.sTskTbl[tskInd].dep.tId;
         break;
      } /* end if */
   } /* end for */

   /* if tskId is not found in the tskTbl */
   if (tskInd == SS_MAX_STSKS)
   {
       MTLOGERROR(ERRCLS_DEBUG, EMT036, ERRZERO, "Invalid system task Id\n");
       RETVALUE(RFAILED);
   }

   /* initialize the cpu mask */
   CPU_ZERO( &cpuSet);

   /* set the cpu mask */
   CPU_SET(coreId, &cpuSet);

   /* set thread affinity for linux */
   if (pthread_setaffinity_np(tId, sizeof(cpuSet), &cpuSet) < 0)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      MTLOGERROR(ERRCLS_DEBUG, EMT038, ERRZERO, "Could not set thread affinity\n");
#endif
      RETVALUE(RFAILED);
   } /* end if pthread_setaffinity fails */

#else
#ifdef SUNOS
   for (tskInd = 0;  tskInd < SS_MAX_STSKS;  tskInd++)
   {
      /* comment: modify to use tskId as lwpId to avoid the loop and the new lwpId variable  in dep */
      if (osCp.sTskTbl[tskInd].tskId == *tskId)
      {
         lwpId = osCp.sTskTbl[tskInd].dep.lwpId;
         break;
      } /* end if */
   } /* end for */

   /* if tskId is not found in the tskTbl */
   if (tskInd == SS_MAX_STSKS)
   {
      MTLOGERROR(ERRCLS_DEBUG, EMT036, ERRZERO, "Invalid system task Id\n");
      RETVALUE(RFAILED);
   }

   /* set thread affinity for Solaris */
   if (processor_bind(P_LWPID, lwpId, coreId, NULL) < 0)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      MTLOGERROR(ERRCLS_DEBUG, EMT038, ERRZERO, "Could not set thread affinity\n");
#endif
      RETVALUE(RFAILED);
   } /* end if processor_bind fails */

#endif /* SUNOS */
#endif /* SS_LINUX */
   RETVALUE(ROK);
} /* ssdSetAffinity */

#endif /* SS_MULTICORE_SUPPORT || SS_AFFINITY_SUPPORT */
/************ end multi-core support *************/


/*
*
*       Fun:   ssdDestroySTsk
*
*       Desc:  This function destroys a system task. A terminate
*              event message is sent to the thread function.
*
*       Ret:   ROK      - ok
*
*       Notes:
*
*       File:  mt_ss.c
*
*/
#ifdef ANSI
PUBLIC S16 ssdDestroySTsk
(
SsSTskEntry *sTsk           /* pointer to system task entry */
)
#else
PUBLIC S16 ssdDestroySTsk(sTsk)
SsSTskEntry *sTsk;          /* pointer to system task entry */
#endif
{
   Buffer *mBuf;
   SsMsgInfo *mInfo;


   TRC0(ssdDestroySTsk);


   /* we send a message to this system task to tell it to die */
   if (SGetMsg(SS_DFLT_REGION, SS_DFLT_POOL, &mBuf) != ROK)
   {

#if (ERRCLASS & ERRCLASS_DEBUG)
      MTLOGERROR(ERRCLS_DEBUG, EMT005, ERRZERO, "Could not get a message");
#endif

      RETVALUE(RFAILED);
   }

   mInfo = (SsMsgInfo *)mBuf->b_rptr;
   mInfo->eventInfo.event = SS_EVNT_TERM;

   if (ssDmndQPutLast(&sTsk->dQ, mBuf, 0) != ROK)
   {
      SPutMsg(mBuf);

#if (ERRCLASS & ERRCLASS_DEBUG)
      MTLOGERROR(ERRCLS_DEBUG, EMT006, ERRZERO,
                     "Could not write to demand queue");
#endif

      RETVALUE(RFAILED);
   }


   RETVALUE(ROK);
}

/* mt023.201 - Added SThreadYield function to yield CPU
*
*       Fun:   SThreadYield
*
*       Desc:  This function defers thread execution to any other ready
*              thread.
*
*       Ret:   ROK      - ok
*              RFAILED  - failure
*
*       Notes:
*
*       File:  mt_ss.c
*
*/
#ifdef ANSI
PUBLIC S16 SThreadYield
(
void
)
#else
PUBLIC S16 SThreadYield()
#endif
{

   TRC0(SThreadYield);

/* mt024.201 - seperated Linux and other UNIX implementations
 */
#ifdef SS_LINUX
   {
      struct timeval tw;

      /* Set sleep value to 0 to yield CPU */
      tw.tv_sec=0;
      tw.tv_usec=0;

      RETVALUE(select(0,0,0,0,&tw) == 0 ? ROK : RFAILED);
   }
#else /* other UNICes */

   RETVALUE(sleep(0) == 0 ? ROK : RFAILED);

#endif /* SS_LINUX */

}


/*
*
*       Fun:   Register timer
*
*       Desc:  This function is used to register a timer
*              function for the service user. System services
*              will invoke the timer activation function
*              passed to it at the specified intervals.
*
*       Ret:   ROK      - ok
*
*       Notes: Timing is handled by the common timers. The
*              ticks are handled by a thread that uses
*              nanosleep() and thus timing precision will not
*              be very accurate.
*
*       File:  mt_ss.c
*
*/
#ifdef ANSI
PUBLIC S16 ssdRegTmr
(
SsTmrEntry *tmr             /* pointer to timer entry */
)
#else
PUBLIC S16 ssdRegTmr(tmr)
SsTmrEntry *tmr;            /* pointer to timer entry */
#endif
{
   CmTmrArg arg;


   TRC0(ssdRegTmr);


   /* initialize common timers */
   cmInitTimers(tmr->dep.timers, TMR_DEF_MAX);


   /* start the timer */
   arg.tq = osCp.dep.tmrTq;
   arg.tqCp = &osCp.dep.tmrTqCp;
   arg.timers = tmr->dep.timers;
   arg.cb = (PTR) tmr;
   arg.evnt = TMR_DEF;
   arg.wait = 0;
   arg.tNum = NOTUSED;
   arg.max = TMR_DEF_MAX;
   arg.wait = tmr->interval;
   cmPlcCbTq(&arg);


   RETVALUE(ROK);
}


/*
*
*       Fun:   Deregister timer
*
*       Desc:  This function is used to deregister a timer function.
*
*       Ret:   ROK      - ok
*
*       Notes:
*
*       File:  mt_ss.c
*
*/
#ifdef ANSI
PUBLIC S16 ssdDeregTmr
(
SsTmrEntry *tmr             /* pointer to timer entry */
)
#else
PUBLIC S16 ssdDeregTmr(tmr)
SsTmrEntry *tmr;            /* pointer to timer entry */
#endif
{
   CmTmrArg arg;


   TRC0(ssdDeregTmr);


   /* stop the timer */
   arg.tq = osCp.dep.tmrTq;
   arg.tqCp = &osCp.dep.tmrTqCp;
   arg.timers = tmr->dep.timers;
   arg.cb = (PTR) tmr;
   arg.evnt = TMR_DEF;
   arg.wait = 0;
   arg.tNum = NOTUSED;
   arg.max = TMR_DEF_MAX;
   arg.wait = tmr->interval;
   cmRmvCbTq(&arg);


   RETVALUE(ROK);
}


/*
*
*       Fun:   Critical error
*
*       Desc:  This function is called when a critical error occurs.
*
*       Ret:   ROK      - ok
*
*       Notes:
*
*       File:  mt_ss.c
*
*/
#ifdef ANSI
PUBLIC S16 ssdError
(
Seq seq,                    /* sequence number */
Reason reason               /* reset reason */
)
#else
PUBLIC S16 ssdError(seq, reason)
Seq seq;                    /* sequence number */
Reason reason;              /* reset reason */
#endif
{
   S16 i;
   pthread_t tId;
   Txt errBuf[256];


   TRC0(ssdError);


   /* get calling task ID */
   tId = pthread_self();


   /* set up the message to display */
   sprintf(errBuf, "\n\nFATAL ERROR - taskid = %x, errno = %d,"
            "reason = %d\n\n", (U8)tId, seq, reason);
   SPrint(errBuf);


   /* delete all system tasks */
   for (i = 0;  i < SS_MAX_STSKS;  i++)
   {
      if (osCp.sTskTbl[i].used
            &&  !pthread_equal(osCp.sTskTbl[i].dep.tId, tId))
      {
         pthread_kill(osCp.sTskTbl[i].dep.tId, SIGKILL);
      }
   }


   /* delete self */
   pthread_exit(NULLP);


   /* won't reach here */
   RETVALUE(ROK);
}


/*
*
*       Fun:   Log error
*
*       Desc:  This function is called to log an error.
*
*       Ret:   ROK      - ok
*
*       Notes:
*
*       File:  mt_ss.c
*
*/
#ifdef ANSI
PUBLIC Void ssdLogError
(
Ent ent,                    /* Calling layer's entity id */
Inst inst,                  /* Calling layer's instance id */
ProcId procId,              /* Calling layer's processor id */
Txt *file,                  /* file name where error occured */
S32 line,                   /* line in file where error occured */
ErrCls errCls,              /* error class */
ErrCode errCode,            /* layer unique error code */
ErrVal errVal,              /* error value */
Txt *errDesc                /* description of error */
)
#else
PUBLIC Void ssdLogError(ent, inst, procId, file, line,
                        errCls, errCode, errVal, errDesc)
Ent ent;                    /* Calling layer's entity id */
Inst inst;                  /* Calling layer's instance id */
ProcId procId;              /* Calling layer's processor id */
Txt *file;                  /* file name where error occured */
S32 line;                   /* line in file where error occured */
ErrCls errCls;              /* error class */
ErrCode errCode;            /* layer unique error code */
ErrVal errVal;              /* error value */
Txt *errDesc;               /* description of error */
#endif
{
#ifndef DEBUGNOEXIT
   S16 i;
   pthread_t tId;
#endif
   Txt *errClsMsg;
   Txt errBuf[512];


   TRC0(ssdLogError);


   /* get calling task ID */
#ifndef DEBUGNOEXIT
   tId = pthread_self();
#endif


   switch(errCls)
   {
      case ERRCLS_ADD_RES:
         errClsMsg = "ERRCLS_ADD_RES";
         break;

      case ERRCLS_INT_PAR:
         errClsMsg = "ERRCLS_INT_PAR";
         break;

      case ERRCLS_DEBUG:
         errClsMsg = "ERRCLS_DEBUG";
         break;

/* mt028.201 : Addition - ERRCLS_FTHA changes */
      case ERRCLS_FTHA:
         errClsMsg = "ERRCLS_FTHA";
         break;

      default:
         errClsMsg = "INVALID ERROR CLASS!";
         break;
   }


/*mt009.301 Fixed 64BIT compilation warnings*/
#ifdef ALIGN_64BIT
   sprintf(errBuf,
             "\nmtss(posix): sw error:  ent: %03d  inst: %03d  proc id: %03d \n"
             "file: %s line: %03d errcode: %05d errcls: %s\n"
             "errval: %05d  errdesc: %s\n",
           ent, inst, procId, file, line, errCode, errClsMsg, errVal, errDesc);
#else
   sprintf(errBuf,
             "\nmtss(posix): sw error:  ent: %03d  inst: %03d  proc id: %03d \n"
             "file: %s line: %03ld errcode: %05ld errcls: %s\n"
             "errval: %05ld  errdesc: %s\n",
           ent, inst, procId, file, line, errCode, errClsMsg, errVal, errDesc);
#endif
   SDisplay(0, errBuf);
/* mt001.301 : Additions */
#ifdef SS_LOGGER_SUPPORT
   SWrtLogBuf(errBuf);
#endif /* SS_LOGGER_SUPPORT  */


#ifndef DEBUGNOEXIT
   /* debug errors halt the system */
   if (errCls == ERRCLS_DEBUG)
   {
/* mt001.301 : Additions */
#ifdef SS_LOGGER_SUPPORT
     SCleanUp();
#endif /* SS_LOGGER_SUPPORT  */
      /* delete all system tasks */
      for (i = 0;  i < SS_MAX_STSKS;  i++)
      {
         if (osCp.sTskTbl[i].used
               &&  !pthread_equal(osCp.sTskTbl[i].dep.tId, tId))
         {
            pthread_kill(osCp.sTskTbl[i].dep.tId, SIGKILL);
         }
      }


      /* delete self */
      pthread_exit(NULLP);
   }
#endif


   RETVOID;
}

#ifdef ENB_RELAY

/*
*
*       Fun:   Register driver task
*
*       Desc:  This function is called to register the handlers for a
*              driver task.
*
*       Ret:   ROK      - ok
*
*       Notes:
*
*       File:  mt_ss.c
*
*/
#ifdef ANSI
PUBLIC S16 ssdRegDrvrTsk
(
SsDrvrTskEntry *drvrTsk         /* driver task entry */
)
#else
PUBLIC S16 ssdRegDrvrTsk(drvrTsk)
SsDrvrTskEntry *drvrTsk;        /* driver task entry */
#endif
{
   TRC0(ssdRegDrvrTsk);


   RETVALUE(ROK);
}
/* mt001.30 : Additions */
/*
*
*       Fun:   Deregister driver task
*
*       Desc:  This function is called to deregister the handlers for a
*              driver task.
*
*       Ret:   ROK      - ok
*
*       Notes:
*
*       File:  mt_ss.c
*
*/
#ifdef ANSI
PUBLIC S16 ssdDeregDrvrTsk
(
SsDrvrTskEntry *drvrTsk         /* driver task entry */
)
#else
PUBLIC S16 ssdDeregDrvrTsk(drvrTsk)
SsDrvrTskEntry *drvrTsk;        /* driver task entry */
#endif
{
   TRC0(ssdDeregDrvrTsk);


   RETVALUE(ROK);
}
#endif




/*
 * mt003.301 Additions - SDeRegTTsk fix
 */
#ifdef SS_MULTIPLE_PROCS
#ifdef ANSI
PUBLIC S16 ssdProcTTskTerm
(
ProcId procIdx,
SsTTskEntry *tTsk,
SsIdx idx
)
#else
PUBLIC S16 ssdProcTTskTerm(procIdx, tTsk, idx)
ProcId procIdx;
SsTTskEntry *tTsk;
SsIdx idx;
#endif
#else /*SS_MULTIPLE_PROCS*/
#ifdef ANSI
PUBLIC S16 ssdProcTTskTerm
(
SsTTskEntry *tTsk,
SsIdx idx
)
#else
PUBLIC S16 ssdProcTTskTerm(tTsk, idx)
SsTTskEntry *tTsk;
SsIdx idx;
#endif
#endif /*SS_MULTIPLE_PROCS*/
{
#ifdef SS_MULTIPLE_PROCS
   ProcId proc;
#endif
   Ent ent;
   Inst inst;
   SsSTskEntry *sTsk;
   S16 n;
	S16  ret;

   TRC0(ssdProcTTskTerm);


   ent = tTsk->ent;
   inst = tTsk->inst;
    /* We check the sTsk element; if it is not NULLP, the
     *  task is attached. So we have to detach it before
     *  deregistering the task.
     */
   ret = SLock(&osCp.sTskTblLock);
   if (ret != ROK)
   {
       MTLOGERROR(ERRCLS_DEBUG, EMTXXX, ERRZERO, "Could not lock system task table");
       RETVALUE(RFAILED);
   }
	SS_ACQUIRE_ALL_SEMA(&osCp.tTskTblSem, ret);
   if (ret != ROK)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      MTLOGERROR(ERRCLS_DEBUG, EMTXXX, ERRZERO, "Could not lock TAPA task table");
#endif
      if ( SUnlock(&osCp.sTskTblLock) != ROK)
      {
#if (ERRCLASS & ERRCLS_DEBUG)
       	MTLOGERROR(ERRCLS_DEBUG, EMTXXX, ERRZERO, "Could not Unlock system task table");
          RETVALUE(RFAILED);
#endif
      }

      RETVALUE(RFAILED);
   }

#ifdef SS_MULTIPLE_PROCS
   proc = tTsk->proc;
   if (tTsk->initTsk != NULLP)
   {
#ifndef USE_MEMCAL
      (Void)(*(tTsk->initTsk))(proc, ent, inst,
                              DFLT_REGION,
                              NRM_TERM,
                              &(osCp.tTskTbl[idx].xxCb));
#else
      (Void)(*(tTsk->initTsk))(proc, ent, inst,
                              SS_STATIC_REGION,
                              NRM_TERM,
                              &(osCp.tTskTbl[idx].xxCb));
#endif /* USE_MEMCAL */
   }
#endif /* SS_MULTIPLE_PROCS */

   if (tTsk->sTsk != NULLP)
   {
      sTsk = tTsk->sTsk;

      sTsk->dep.ent = ent;
      sTsk->dep.inst = inst;

      for (n = 0;  n < SS_MAX_TTSKS;  n++)
      {
         if (sTsk->tTsks[n] == idx)
         {
            sTsk->tTsks[n] = SS_INVALID_IDX;
            sTsk->numTTsks--;
            break;
         }
      }

       /* call the implementation to detach the task */
       ssdDetachTTsk(tTsk);
       /* 100178 */
       sTsk->dep.ent = ENTNC;
       sTsk->dep.inst = INSTNC;
   }

    /* Now we empty the entry for this task and update the table
     *  information
     */
#ifdef SS_MULTIPLE_PROCS
    osCp.tTskIds[procIdx][ent][inst] = SS_TSKNC;
#else /* SS_MULTIPLE_PROCS */
    osCp.tTskIds[ent][inst] = SS_TSKNC;
#endif /* SS_MULTIPLE_PROCS */

    tTsk->used    = FALSE;
#ifdef SS_MULTIPLE_PROCS
    tTsk->proc    = PROCNC;
#endif /* SS_MULTIPLE_PROCS */
    tTsk->ent     = ENTNC;
    tTsk->inst    = INSTNC;
    tTsk->tskType = TTUND;
    tTsk->initTsk = NULLP;
    tTsk->actvTsk = NULLP;
    tTsk->sTsk    = NULLP;

    tTsk->nxt = osCp.nxtTTskEntry;
    osCp.nxtTTskEntry = idx;
    osCp.numTTsks--;

#ifdef SS_MULTIPLE_PROCS
    /* mark the control block for this task as invalid */
    osCp.tTskTbl[idx].xxCb = NULLP;
#endif

   SS_RELEASE_ALL_SEMA(&osCp.tTskTblSem);
   if ( SUnlock(&osCp.sTskTblLock) != ROK)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      MTLOGERROR(ERRCLS_DEBUG, EMTXXX, ERRZERO, "Could not Unlock system task table");
#endif
       RETVALUE(RFAILED);
   }
	RETVALUE(ROK);
}

//#ifndef SPLIT_RLC_DL_TASK
#ifdef TENB_T2K3K_SPECIFIC_CHANGES
#if defined (L2_L3_SPLIT) && defined(SPLIT_RLC_DL_TASK) 
EXTERN Void ysMtTskHdlr(Void);
EXTERN Void ysMtPollPhyMsg(U8 region);
EXTERN Void ysMtRcvPhyMsg(Void);
#ifdef ANSI
PUBLIC Void *mtTskHdlrT2kL2
(
Ptr tskPtr                      /* pointer to task entry */
)
#else
PUBLIC Void *mtTskHdlrT2kL2(tskPtr)
Ptr tskPtr;                     /* pointer to task entry */
#endif
{
   S16 ret;


   /* wait for SS to come up */
   /* It is required to block on this semaphore before starting actual processing of 
     the thread becasue the creator of this thread might want to cance it without
     doing any processing. When this semaphore is released, means the creator gives
     the go ahead for actual processing and we should never come back to this point */
   while ((ret = sem_wait(&osCp.dep.ssStarted) != ROK) && (errno == EINTR))
      continue;

#ifdef YS_PHY_3_8_2
   tlSetReady(NULL);
#endif

   while(1)
   {
      ysMtPollPhyMsg(0); /* blocks, waiting for messages for L2
                      * (processes L1 msgs) */
  }

  RETVALUE(NULLP);
}
#else
EXTERN Void ysMtTskHdlr(Void);
EXTERN Void YsPhyRecvMsg();
#ifdef ANSI
PUBLIC Void *mtTskHdlrT2kL2
(
Ptr tskPtr                      /* pointer to task entry */
)
#else
PUBLIC Void *mtTskHdlrT2kL2(tskPtr)
Ptr tskPtr;                     /* pointer to task entry */
#endif
{
   S16 ret;
   SsSTskEntry *sTsk;

   /* get out the system task entry from the parameter */
   sTsk = (SsSTskEntry *) tskPtr;

   /* wait for SS to come up */
   /* It is required to block on this semaphore before starting actual processing of 
     the thread becasue the creator of this thread might want to cance it without
     doing any processing. When this semaphore is released, means the creator gives
     the go ahead for actual processing and we should never come back to this point */
   while ((ret = sem_wait(&osCp.dep.ssStarted) != ROK) && (errno == EINTR))
      continue;

#ifndef RGL_SPECIFIC_CHANGES
#ifdef YS_PHY_3_8_2
   tlSetReady(NULL);
#endif
#endif

   while(1)
   {
#ifdef V5GTF_SPECIFIC_CHANGES
      YsPhyRecvMsg();      
#else      
      ysMtTskHdlr(); /* blocks, waiting for messages for L2
                      * (processes L1 msgs) */
#endif
      /* get a message from the demand queue */
      /* RT Processing */
#ifdef RLC_MAC_DAT_REQ_RBUF
      rgDlDatReqBatchProc();
#endif

      ret = mtTskHdlMsg(sTsk); 
      if (ret != ROK)
      {
         /* exit the for loop here */
         break;
      }
#if defined(SPLIT_RLC_DL_TASK) && defined(RLC_MAC_STA_RSP_RBUF)
       rgBatchProc();
#endif  
   }

   RETVALUE(NULLP);
}
#endif /* TENB_T2K3K_SPECIFIC_CHANGES */
#endif

#ifdef ANSI
PUBLIC void *pthreadCreateHdlr
(
void * arg
)
#else
PUBLIC void *pthreadCreateHdlr(pthreadCreateArg)
void *arg;
#endif
{
   S16 ret;
   SPThreadCreateArg*  pthreadCreateArg = (SPThreadCreateArg*)arg;
   /* mt038.201 changed how sem_wait is called  */
   while ((ret = sem_wait(&osCp.dep.ssStarted) != ROK) && (errno == EINTR))
      continue;

   pthreadCreateArg->start_routine(pthreadCreateArg->argument);
   RETVALUE(ROK);
}

/*
*
*       Fun:   Task handler
*
*       Desc:  This is the system task handler function. It blocks on
*              the system task's demand queue. On receiving a message,
*              it identifies the target TAPA task, verifies that the
*              TAPA task belongs to this system task and if so, calls
*              the activation function of that TAPA task with the
*              received message. The task activation function or the
*              timer activation function may be called.
*
*       Ret:   (thread function)
*
*       Notes:
*
*       File:  mt_ss.c
*
*/
#ifdef ANSI
PUBLIC Void *mtTskHdlr
(
Ptr tskPtr                      /* pointer to task entry */
)
#else
PUBLIC Void *mtTskHdlr(tskPtr)
Ptr tskPtr;                     /* pointer to task entry */
#endif
{
   S16 ret;
   SsSTskEntry *sTsk;

   /* get out the system task entry from the parameter */
   sTsk = (SsSTskEntry *) tskPtr;


   /* wait for SS to come up */

   /* mt038.201 changed how sem_wait is called  */
   while ((ret = sem_wait(&osCp.dep.ssStarted) != ROK) && (errno == EINTR))
      continue;
#ifdef XEON_SPECIFIC_CHANGES
   printf("\n**********MT Task Handler********\n");
#endif   
   while (1)
   {
      /* Wait for a message from the demand queue */
#ifdef SS_CDMNDQ_SUPPORT
      ret = ssCDmndQWait(&sTsk->dQ);
#else
      ret = ssDmndQWait(&sTsk->dQ);
#endif
      if (ret != ROK)
         continue;

      ret = mtTskHdlMsg(sTsk); 
      if (ret != ROK)
      {
         break;
      }
   }

   RETVALUE(NULLP);
}


/*
*
*       Fun:   Task handler
*
*       Desc:  This is the system task handler function. It blocks on
*              the system task's demand queue. On receiving a message,
*              it identifies the target TAPA task, verifies that the
*              TAPA task belongs to this system task and if so, calls
*              the activation function of that TAPA task with the
*              received message. The task activation function or the
*              timer activation function may be called.
*
*       Ret:   (thread function)
*
*       Notes:
*
*       File:  mt_ss.c
*
*/
#ifdef ANSI
PUBLIC S16 mtTskHdlMsg
(
SsSTskEntry *sTsk
)
#else
PUBLIC S16 mtTskHdlMsg(sTsk)
SsSTskEntry *sTsk
#endif
{
   S16 i;
   S16 ret;
   SsIdx idx;
   SsTTskEntry *tTsk;
   Buffer *mBuf;
#ifdef SS_PERF
   Buffer *mBuf2;
#endif
   SsMsgInfo *mInfo;
   Pst nPst;
/* mt028.201: modification: multiple procs support related changes */
#ifndef SS_MULTIPLE_PROCS
#ifdef SS_MT_TMR
   PAIFTMRS16 tmrActvFnMt = NULLP;
#endif
   /* mt015.301 Initialized the timer activation functions with NULLP */
   PFS16 tmrActvFn = NULLP;
#else
   PAIFTMRS16 tmrActvFn;
   U16 procIdIdx;
#endif /* SS_MULTIPLE_PROCS */
	/* mt003.301 Modifications */
#ifdef SS_THREAD_PROFILE
  EpcTime et1,et2;
#endif /* SS_THREAD_PROFILE */


   ret = ssDmndQGet(&sTsk->dQ, &mBuf, SS_DQ_FIRST);
   if (ret != ROK)
   {
      /* nothing to receive */
      RETVALUE(ROK);
   }

   /* if we can't lock this system task entry, return the message */
   ret = SLock(&sTsk->lock);
   if (ret != ROK)
   {

#if (ERRCLASS & ERRCLS_DEBUG)
      MTLOGERROR(ERRCLS_DEBUG, EMT007, (ErrVal) ret,
                     "Could not lock system task entry");
#endif
      SPutMsg(mBuf);
      RETVALUE(ROK);
   }

/* mt034.201 */
#ifdef SS_PERF
   do 
   {
      mBuf2 = mBuf->b_next;
#endif
      /* find out what kind of message this is */
      mInfo = (SsMsgInfo *)mBuf->b_rptr;
#ifdef SS_MEM_WL_DEBUG
      mtTskBuffer1 = mBuf2;
      if(mBuf2)
      mtTskBuffer2 = mBuf2->b_next;

      if(mInfo == 0x5050505)
      {
         stopBtInfo = TRUE;
         SGlobMemInfoShow();
         cmAnalyseBtInfo((PTR) mBuf,4);
         SGlobMemInfoShow();
         printf("\n In trouble .... \n");
      }
      else if (mInfo == 0x2020202)
      {
         stopBtInfo = TRUE;
         cmAnalyseBtInfo((PTR) mBuf,1);
         printf("\n In trouble .... \n");
      }
#endif /* SS_MEM_WL_DEBUG */
      switch (mInfo->eventInfo.event)
      {
         /* this is a termination event, we die */
         case SS_EVNT_TERM:
            /* release the message */
            SPutMsg(mBuf);

            /* Unlock the system task entry and lock the system
             *  task table to clean our entry up.
             */
            SUnlock(&sTsk->lock);

            ret = SLock(&osCp.sTskTblLock);
            if (ret != ROK)
            {

#if (ERRCLASS & ERRCLS_DEBUG)
               MTLOGERROR(ERRCLS_DEBUG, EMT008, (ErrVal) ret,
                           "Could not lock system task table");
#endif
               /* what to do here? */
               RETVALUE(ROK);
            }

            /* clean up the system task entry */
            sTsk->used = FALSE;
            sTsk->tskPrior = 0;
				/* mt003.301 Modifications - SDeRegTTsk */
				/* sTsk->numTTsks = 0; */
            SDestroyLock(&sTsk->lock);
            ssDestroyDmndQ(&sTsk->dQ);

            /* lock for current executing TAPA task ID */

            /* make this entry available in the system task table */
            sTsk->nxt = osCp.nxtSTskEntry;
            for (i = 0;  i < SS_MAX_STSKS;  i++)
            {
               if (sTsk == &osCp.sTskTbl[i])
               {
                  osCp.nxtSTskEntry = i;
                  break;
               }
            }

            osCp.numSTsks--;

            /* unlock the system task table */
            SUnlock(&osCp.sTskTblLock);

            RETVALUE(RFAILED);


         /* this is a data message or a permanent task keep-alive message */
         case SS_EVNT_DATA:
         case SS_EVNT_PERMTICK:
            /* message to a task. find the destination task */
/* mt028.201: modification: multiple procs support related changes */
#ifdef SS_MULTIPLE_PROCS
            procIdIdx = SGetProcIdIdx(mInfo->pst.dstProcId);

            if (procIdIdx == SS_INV_PROCID_IDX)
            {
               SPutMsg(mBuf);
               break;
            }

            idx = osCp.tTskIds[procIdIdx][mInfo->pst.dstEnt][mInfo->pst.dstInst];
#else /* SS_MULTIPLE_PROCS */
            idx = osCp.tTskIds[mInfo->pst.dstEnt][mInfo->pst.dstInst];
#endif /* SS_MULTIPLE_PROCS */

            /* verify that it hasn't been deregistered */
            if (idx == SS_TSKNC)
            {
               SPutMsg(mBuf);
               break;
            }

            /* verify that this system task is still running it */
            tTsk = &osCp.tTskTbl[idx];
            if (tTsk->sTsk != sTsk)
            {
               SPutMsg(mBuf);
               break;
            }

               /* set the current executing TAPA task ID */
               sTsk->dep.ent = mInfo->pst.dstEnt;
               sTsk->dep.inst = mInfo->pst.dstInst;

            /* copy the Pst structure into a local duplicate */
            for (i = 0;  i < (S16) sizeof(Pst);  i++)
               *(((U8 *)(&nPst)) + i) = *(((U8 *)&mInfo->pst) + i);

            /* Give the message to the task activation function. If
             *  its a normal data message, we pass it, if this is a
             *  keep-alive message for a permanent task then we pass
             *  NULLP in place of the message to the task activation
             *  function.
             */
            if (mInfo->eventInfo.event == SS_EVNT_DATA)
            {
#ifndef RGL_SPECIFIC_CHANGES
#ifdef SS_TSKLOG_ENABLE
              U32 t = MacGetTick();
#endif
#endif
				  /* mt003.301 Modifications */
#if SS_THREAD_PROFILE
                tTsk->curEvent = nPst.event;
                SGetEpcTime(&et1);
#endif /* SS_THREAD_PROFILE */
               tTsk->actvTsk(&nPst, mBuf);
#ifndef RGL_SPECIFIC_CHANGES
#ifdef SS_TSKLOG_ENABLE
               SStopTask(t,PID_SSI_TSK);
#endif
#endif
#if SS_THREAD_PROFILE
                SGetEpcTime(&et2);
                tTsk->curEvtTime = (U32)(et2 - et1);
                tTsk->totTime += (U64)tTsk->curEvtTime;
#endif /* SS_THREAD_PROFILE */
            }
            else
            {
#if (ERRCLASS & ERRCLS_DEBUG)
               /* this message should only come to a permanent task */
               if (tTsk->tskType != SS_TSK_PERMANENT)
               {
                  MTLOGERROR(ERRCLS_DEBUG, EMT009, ERRZERO, "Logic failure");
                  break;
               }
#endif
               tTsk->actvTsk(&nPst, NULLP);

               /* We need to re-send this message back to ourselves so
                *  the permanent task continues to run.
                */
               /* Check if this task got deregistered or detached
                *  by the activation function; if so, there's nothing
                *  more to do here, otherwise go ahead.
                */
               ret = ROK;
               if (tTsk->used == TRUE  &&  tTsk->sTsk != NULLP)
               {
                  ret = ssDmndQPutLast(&tTsk->sTsk->dQ, mBuf,
                              ((tTsk->tskPrior) * SS_MAX_MSG_PRI) +
                              mInfo->pst.prior);
               }

               /* failure here is a real problem */
               if (ret != ROK)
               {
#if (ERRCLASS & ERRCLS_DEBUG)
                  MTLOGERROR(ERRCLS_DEBUG, EMT010, ERRZERO,
                              "Could not write to demand queue");
#endif
                  SPutMsg(mBuf);
               }
            }

            /* unset the current executing TAPA task ID */
               sTsk->dep.ent = ENTNC;
               sTsk->dep.inst = INSTNC;
            break;


         case SS_EVNT_TIMER:
            /* timer event. find the timer entry */
            idx = mInfo->eventInfo.u.tmr.tmrIdx;

            /* lock the timer table, coz we're going to peek in it */
            ret = SLock(&osCp.tmrTblLock);
            if (ret != ROK)
            {

#if (ERRCLASS & ERRCLS_DEBUG)
               MTLOGERROR(ERRCLS_DEBUG, EMT011, (ErrVal) ret,
                              "Could not lock timer table");
#endif
               SPutMsg(mBuf);
               break;
            }

            /* Verify that this timer entry is still around and that it
             *  belongs to our task.
             */
            if (osCp.tmrTbl[idx].used == FALSE
/* mt028.201: modification: multiple procs support related changes */
#ifdef SS_MULTIPLE_PROCS
                  ||  osCp.tmrTbl[idx].ownerProc != mInfo->pst.dstProcId
#endif /* SS_MULTIPLE_PROCS */
                  ||  osCp.tmrTbl[idx].ownerEnt != mInfo->pst.dstEnt
                  ||  osCp.tmrTbl[idx].ownerInst != mInfo->pst.dstInst)
            {
               SUnlock(&osCp.tmrTblLock);
               SPutMsg(mBuf);
               break;
            }

 /* mt005.21: addition */
            /* set the current executing TAPA task ID */
               sTsk->dep.ent = mInfo->pst.dstEnt;
               sTsk->dep.inst = mInfo->pst.dstInst;

#ifndef SS_MULTIPLE_PROCS
#ifdef SS_MT_TMR
            /*mt006.301 Adding Initializing the tmrActvFnMt*/
            tmrActvFnMt = NULLP;
            if (osCp.tmrTbl[idx].ssTmrActvFn.mtFlag == TRUE)
            {
               tmrActvFnMt = osCp.tmrTbl[idx].ssTmrActvFn.actvFnc.tmrActvFnMt;
            }
            else
#endif
#endif
            {
               tmrActvFn = osCp.tmrTbl[idx].ssTmrActvFn.actvFnc.tmrActvFn;
            }

            /* unlock the timer table */
            SUnlock(&osCp.tmrTblLock);

            /* activate the timer function */
/* mt028.201: modification: multiple procs support related changes */
#ifndef SS_MULTIPLE_PROCS
#ifdef SS_MT_TMR
            if (tmrActvFnMt)
            {
               tmrActvFnMt(osCp.tmrTbl[idx].ownerEnt,
                           osCp.tmrTbl[idx].ownerInst);
            }
            else
#endif
            {
               tmrActvFn();
            }
#else
            tmrActvFn(osCp.tmrTbl[idx].ownerProc, osCp.tmrTbl[idx].ownerEnt,
                        osCp.tmrTbl[idx].ownerInst);
#endif /* SS_MULTIPLE_PROCS */

 /*mt005.21: addition */
            /* unset the current executing TAPA task ID */
               sTsk->dep.ent = ENTNC;
               sTsk->dep.inst = INSTNC;


            /* return the message buffer */
            SPutMsg(mBuf);
            break;
				/*
				 * mt003.301 - SDeRegTTsk fix
				 */
         case SS_EVNT_TTSK_TERM:
#ifdef SS_MULTIPLE_PROCS
            procIdIdx = SGetProcIdIdx(mInfo->pst.dstProcId);

            if (procIdIdx == SS_INV_PROCID_IDX)
            {
               SPutMsg(mBuf);
               break;
            }

            idx = osCp.tTskIds[procIdIdx][mInfo->pst.dstEnt][mInfo->pst.dstInst];
#else /* SS_MULTIPLE_PROCS */
            idx = osCp.tTskIds[mInfo->pst.dstEnt][mInfo->pst.dstInst];
#endif /* SS_MULTIPLE_PROCS */

            /* verify that it hasn't been deregistered */
            if (idx == SS_TSKNC)
            {
               SPutMsg(mBuf);
               break;
            }

            /* verify that this system task is still running it */
            tTsk = &osCp.tTskTbl[idx];
            if (tTsk->sTsk != sTsk)
            {
               SPutMsg(mBuf);
               break;
            }
#ifdef SS_MULTIPLE_PROCS
            ssdProcTTskTerm(procIdIdx, tTsk, idx);
#else
            ssdProcTTskTerm(tTsk, idx);
#endif
            SPutMsg(mBuf);
            break;

         default:
#if (ERRCLASS & ERRCLS_DEBUG)
            MTLOGERROR(ERRCLS_DEBUG, EMT012, (ErrVal) ret,
                        "Illegal event");
#endif
            break;
      }
#ifdef SS_PERF
         mBuf = mBuf2;
   } while (mBuf != NULLP);
#endif

   /* unlock the system task entry */
   SUnlock(&sTsk->lock);

#ifndef SS_PERF
   /* yield for other threads */
   /* mt024.201 - changed to use SSI SThreadYield instead of sleep */
   SThreadYield();
#endif

   RETVALUE(ROK);
}

Bool g_usettitmr;
/*
*       Fun:   mtTmrHdlrPublic
*/
#ifdef ANSI
PUBLIC Void mtTmrHdlrPublic
(
)
#else
PUBLIC Void mtTmrHdlrPublic()
#endif
{
   if (SLock(&osCp.tmrTblLock) != ROK)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      MTLOGERROR(ERRCLS_DEBUG, EMT016, ERRZERO, "Could not lock timer table");
#endif
      return;
   }
   cmPrcTmr(&osCp.dep.tmrTqCp, osCp.dep.tmrTq, mtTimeout);
   /* unlock the timer table */
   SUnlock(&osCp.tmrTblLock);
}


/*
*
*       Fun:   mtTmrHdlr
*
*       Desc:  The timer handler thread function. Counts time
*              and invokes the common timer function on each
*              tick.
*
*       Ret:   (thread function)
*
*       Notes:
*
*       File:  mt_ss.c
*
*/
/*mt041.201 Modified SSI tick handling in mtTmrHdlr() */
#ifdef ANSI
PRIVATE Void *mtTmrHdlr
(
void *parm                        /* unused */
)
#else
   /* mt009.21: addition */
PRIVATE Void *mtTmrHdlr(parm)
void *parm;                       /* unused */
#endif
{
/*mt004.301-addede new region*/
/* mt010.301 Removed SS_FAP portion and
 * enabled oroginal code in function mtTmrHdlr */

   struct timespec ts;
   U32 time_int;
   U32 i, cnt, oldTicks, newTicks;
   struct timeval tv1,tv2;
   /* mt038.201 added return */
   S16 ret;
   /* mt039.201 changes for nanosleep */
   struct timespec tsN;
   PRIVATE U32 err_in_usec;

   /*mt013.301 : doesn't need TRC macro ,as this will never return*/


   UNUSED(parm);

   /* mt027.201 - Modification for SRegCfgTmr support */
   /* check SS_TICKS_SEC */
   if (SS_1MS < SS_TICKS_SEC)
   {
      MTLOGERROR(ERRCLS_DEBUG, EMT013, ERRZERO, "Minimum SSI ticks is 1ms");
   }

   /* mt025.201 - Addition to stop timer handler till task registration is done */
   /* wait for SS to come up */
   /* mt038.201 changed how sem_wait is called  */
   while ((ret = sem_wait(&osCp.dep.ssStarted) != ROK) && (errno == EINTR))
      continue;

   /* mt027.201 - Modification for SRegCfgTmr support */
   /* set up parameter to nanosleep() for SS_TICKS_SEC */
   ts.tv_sec = 0;
   ts.tv_nsec = (MT_TICK_CNT * 1000);
   /* mt039.201 changes for nanosleep */
   tsN.tv_sec = 0;
   tsN.tv_nsec = 0;

   err_in_usec =  0;

   if (gettimeofday(&tv1, NULL) == -1)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
            MTLOGERROR(ERRCLS_DEBUG, EMT014, (ErrVal) errno,
                        "Error in clock_gettime");
#endif
   }

   /* infinite loop */
   for (; ;)
   {
#if 1
     if (g_usettitmr)
     {
#ifndef STUB_TTI_HANDLING_5GTF        
        printf("Returning from mtTmrHdlr()\n");
        return NULL;
#endif        
     }
#endif
     /* mt039.201 changes for nanosleep */
      /* sleep for MT_TICK_CNT milli seconds */
      ts.tv_nsec = (MT_TICK_CNT - err_in_usec) * 1000;
      while ((ret = nanosleep (&ts, &tsN) != ROK) && (errno == EINTR))
      {
         ts.tv_nsec = tsN.tv_nsec;
         tsN.tv_nsec = 0;
         continue;
      }

      if (gettimeofday(&tv2,NULL) == -1)
      {
#if (ERRCLASS & ERRCLS_DEBUG)
            MTLOGERROR(ERRCLS_DEBUG, EMT015, (ErrVal) errno,
                        "Error in clock_gettime");
#endif
      }

     /*mt013.301 : changed check while calculating timer to fix
      * diffrence between MTSS time and real unix time
      */
     if ((tv2.tv_sec == tv1.tv_sec)&&(tv2.tv_usec > tv1.tv_usec))
     {
        time_int = (tv2.tv_usec - tv1.tv_usec);
     }
     else if (tv2.tv_sec > tv1.tv_sec)
     {
        time_int = ((tv2.tv_sec - tv1.tv_sec)*1000000) + (tv2.tv_usec - tv1.tv_usec);
     }
     else /*  ts2 < ts1, this will not happen in normal scenario */
     {
        /* to make sure cnt = 1  */
        err_in_usec = 0;
        time_int = MT_TICK_CNT;
     }

     oldTicks = osCp.dep.sysTicks;
     osCp.dep.sysTicks += (time_int/(MT_TICK_CNT - err_in_usec));
     err_in_usec = (time_int % (MT_TICK_CNT - err_in_usec));
     newTicks = osCp.dep.sysTicks;
     tv1.tv_usec = tv2.tv_usec;
     tv1.tv_sec = tv2.tv_sec;

     cnt = newTicks - oldTicks;

     while(err_in_usec >= MT_TICK_CNT)
     {
        cnt++;
        err_in_usec -= MT_TICK_CNT;
     }
     if( cnt >= MT_MAX_TICK_CNT_VAL)
        cnt = MT_MIN_TICK_CNT_VAL;
     /* call the common timer tick handler */
	  for (i = 0; i < cnt; i++)
	  {
	  	 /* mt008.301: cmPrcTmr is guarded with a lock */
		 /* lock the timer table */
		 if (SLock(&osCp.tmrTblLock) != ROK)
		 {
#if (ERRCLASS & ERRCLS_DEBUG)
			MTLOGERROR(ERRCLS_DEBUG, EMT016, ERRZERO, "Could not lock timer table");
#endif
			continue;
		 }
		 cmPrcTmr(&osCp.dep.tmrTqCp, osCp.dep.tmrTq, mtTimeout);
		 /* unlock the timer table */
		 SUnlock(&osCp.tmrTblLock);
	  }
   }

   /* mt009.21: addition */
   RETVALUE( (Void *) NULLP);
   /* will not reach here */
}


/*
*
*       Fun:   mtTimeout
*
*       Desc:  Process timer event. Called from the common timer
*              code when a timeout occurs.
*
*       Ret:   Void
*
*       Notes:
*
*       File:  mt_ss.c
*
*/
#ifdef ANSI
PUBLIC Void mtTimeout
(
PTR tCb,                        /* control block */
S16 evnt                        /* event */
)
#else
PUBLIC Void mtTimeout(tCb, evnt)
PTR tCb;                        /* control block */
S16 evnt;                       /* event */
#endif
{
   Buffer *mBuf;
   SsMsgInfo *mInfo;
   CmTmrArg arg;
   SsTmrEntry *tEnt;
   SsTTskEntry *tTsk;
   SsIdx idx;
#ifndef TENB_RTLIN_CHANGES
   S16 ret;
#endif
/* mt028.201: modification: multiple procs support related changes */
#ifdef SS_MULTIPLE_PROCS
   U16 procIdIdx;
#endif /* SS_MULTIPLE_PROCS */
#ifdef RGL_SPECIFIC_CHANGES
#ifdef MSPD_MLOG_NEW
   U32 t = GetTIMETICK();
#endif
#endif

   TRC0(mtTimeout);


   /* get the timer entry */
   tEnt = (SsTmrEntry *) tCb;


   /* if the timer was deleted, this will be NULL, so drop it */
   if (tEnt == NULL)
   {
      RETVOID;
   }

/* mt008.301 Deletion: tmrTbl Lock is moved to mtTmrHdlr */


   /* Hmmmm, the timer might have been deleted while we've been
    *  working at getting here, so we just skip this.
    */
   if (tEnt->used == FALSE)
   {
      RETVOID;
   }


   /* Set up and send a timer message to the destination tasks'
    * demand queue.
    */
#ifndef SS_MULTICORE_SUPPORT
   if (SGetMsg(SS_DFLT_REGION, SS_DFLT_POOL, &mBuf) != ROK)
#else
#ifdef RGL_SPECIFIC_CHANGES
   if (SGetMsg((SS_DFLT_REGION), SS_DFLT_POOL, &mBuf) != ROK)
#else
   if (SGetMsg((osCp.sTskTbl[0].region), SS_DFLT_POOL, &mBuf) != ROK)
#endif
#endif
   {

#if (ERRCLASS & ERRCLS_DEBUG)
      MTLOGERROR(ERRCLS_DEBUG, EMT017, ERRZERO, "Could not get message");
#endif

      RETVOID;
   }

   mInfo = (SsMsgInfo *)mBuf->b_rptr;
   mInfo->eventInfo.event = SS_EVNT_TIMER;
   mInfo->eventInfo.u.tmr.tmrIdx = tEnt->tmrId;

   mInfo->pst.dstEnt = tEnt->ownerEnt;
   mInfo->pst.dstInst = tEnt->ownerInst;
   mInfo->pst.srcEnt = tEnt->ownerEnt;
   mInfo->pst.srcInst = tEnt->ownerInst;
/* mt028.201: modification: multiple procs support related changes */
#ifndef SS_MULTIPLE_PROCS
   mInfo->pst.dstProcId = SFndProcId();
   mInfo->pst.srcProcId = SFndProcId();
#else /* SS_MULTIPLE_PROCS */
   mInfo->pst.dstProcId = tEnt->ownerProc;
   mInfo->pst.srcProcId = tEnt->ownerProc;
#endif /* SS_MULTIPLE_PROCS */
   mInfo->pst.selector = SEL_LC_NEW;
#ifndef SS_MULTICORE_SUPPORT
   mInfo->pst.region = DFLT_REGION;
#else
#endif
   mInfo->pst.pool = DFLT_POOL;
   mInfo->pst.prior = PRIOR0;
   mInfo->pst.route = RTESPEC;
   mInfo->pst.event = 0;


#ifndef TENB_RTLIN_CHANGES
   /* get a semaphore for the TAPA task table */
   SS_ACQUIRE_SEMA(&osCp.tTskTblSem, ret);
   if (ret != ROK)
   {
      SPutMsg(mBuf);

#if (ERRCLASS & ERRCLS_DEBUG)
      MTLOGERROR(ERRCLS_DEBUG, EMT018, ret, "Could not lock TAPA task table");
#endif

      RETVOID;
   }
#endif


   /* find the owner TAPA task */
/* mt028.201: modification: multiple procs support related changes */
#ifdef SS_MULTIPLE_PROCS
   procIdIdx = SGetProcIdIdx(tEnt->ownerProc);
   idx = osCp.tTskIds[procIdIdx][tEnt->ownerEnt][tEnt->ownerInst];
#else /* SS_MULTIPLE_PROCS */
   idx = osCp.tTskIds[tEnt->ownerEnt][tEnt->ownerInst];
#endif /* SS_MULTIPLE_PROCS */
   if (idx == SS_TSKNC)
   {
#ifndef TENB_RTLIN_CHANGES
      SS_RELEASE_SEMA(&osCp.tTskTblSem);
#endif
      SPutMsg(mBuf);
      RETVOID;
   }


   /* ensure that the TAPA task is hale and hearty */
   tTsk = &osCp.tTskTbl[idx];
   if (!tTsk->used)
   {
#ifndef TENB_RTLIN_CHANGES
      SS_RELEASE_SEMA(&osCp.tTskTblSem);
#endif
      SPutMsg(mBuf);
      RETVOID;
   }
   /* Klock work fix ccpu00148484 */
   /* write the timer message to the queue of the destination task */
	/* mt008.301 : check sTsk before putting into it's DQ */
   if (tTsk->sTsk == NULLP)
   {
#ifndef TENB_RTLIN_CHANGES
      SS_RELEASE_SEMA(&osCp.tTskTblSem);
#endif
      SPutMsg(mBuf);

#if (ERRCLASS & ERRCLS_DEBUG)
      MTLOGERROR(ERRCLS_DEBUG, EMT019, ERRZERO,
                        "Could not write to demand queue");
#endif

      RETVOID;
   }
#ifdef SS_LOCKLESS_MEMORY
   mInfo->pst.region = tTsk->sTsk->region;
   mInfo->region = tTsk->sTsk->region;
#endif /* SS_LOCKLESS_MEMORY */
   if (ssDmndQPutLast(&tTsk->sTsk->dQ, mBuf,
               (tTsk->tskPrior * SS_MAX_MSG_PRI) + PRIOR0) != ROK)
   {
#ifndef TENB_RTLIN_CHANGES
      SS_RELEASE_SEMA(&osCp.tTskTblSem);
#endif
      SPutMsg(mBuf);

#if (ERRCLASS & ERRCLS_DEBUG)
      MTLOGERROR(ERRCLS_DEBUG, EMT019, ERRZERO,
                        "Could not write to demand queue");
#endif

      RETVOID;
   }
/* Fix for ccpu00130657 */
#ifdef TENB_T2K3K_SPECIFIC_CHANGES
   if (tTsk->sTsk->tskPrior == PRIOR0)
   {
#ifdef INTEL_WLS
        WLS_WakeUp(mtGetWlsHdl());
#else
      tlPost(NULLP);
#endif
   }
#endif

   /* release the semaphore for the TAPA task table */
#ifndef TENB_RTLIN_CHANGES
   SS_RELEASE_SEMA(&osCp.tTskTblSem);
#endif


   /* restart the timer */
   arg.tq = osCp.dep.tmrTq;
   arg.tqCp = &osCp.dep.tmrTqCp;
   arg.timers = tEnt->dep.timers;
   arg.cb = (PTR) tEnt;
   arg.evnt = TMR_DEF;
   arg.wait = 0;
   arg.tNum = NOTUSED;
   arg.max = TMR_DEF_MAX;
   arg.wait = tEnt->interval;
   cmPlcCbTq(&arg);
#ifdef RGL_SPECIFIC_CHANGES
#ifdef MSPD_MLOG_NEW
   MLogTask(131313, RESOURCE_LARM, t, GetTIMETICK());
#endif
#endif
   RETVOID;
}


#ifdef CONAVL
/*
*
*       Fun:   mtConHdlr
*
*       Desc:  This thread reads the console and hands over any
*              data read to a user function.
*
*       Ret:   (thread function)
*
*       Notes:
*
*       File:  mt_ss.c
*
*/
#ifdef ANSI
PRIVATE Void *mtConHdlr
(
Ptr parm                        /* unused */
)
#else
  /* mt009.21: addition */
PRIVATE Void *mtConHdlr(parm)
Ptr parm;                       /* unused */
#endif
{
   int fd;
   Data data;


   /*mt013.301 : doesn't need TRC macro ,as this will never return*/


   UNUSED(parm);


   /* check if we have a console input file handle */
   if (osCp.dep.conInFp == NULLP)
   {
      /* die */
      RETVALUE(NULLP);
   }

   fd = fileno(osCp.dep.conInFp);

   /* infinite loop */
   for (; ;)
   {
      if ((read(fd, &data, 1)) != 1)
      {
         continue;
      }


      /* call rdConQ, defined by the system service user */
      //rdConQ(data);
   }


   /* not reached */
}
#endif /* CONAVL */

#ifndef L2_L3_SPLIT
#ifdef SS_DRVR_SUPPORT
/*
*
*       Fun:   Interrupt service task handler
*
*       Desc:  This is the interrupt service task handler. It blocks
*              on a pipe from which it reads an isFlag structure. The
*              structure indicates which interrupt service task is to
*              be executed. The thread identifies the task, calls the
*              isTsk function and sends itself a message to repeat
*              this operation until it receives a message to cease.
*
*       Ret:   ROK      - ok
*
*       Notes:
*
*       File:  mt_ss.c
*
*/
#ifdef ANSI
  /* mt009.21: addition */
PRIVATE Void *mtIsTskHdlr
(
Ptr tskPtr                      /* pointer to task entry */
)
#else
  /* mt009.21: addition */
PRIVATE Void *mtIsTskHdlr(tskPtr)
Ptr tskPtr;                     /* pointer to task entry */
#endif
{
#if (ERRCLASS & ERRCLS_DEBUG)
   int ret;
#endif
   MtIsFlag isFlag;


   TRC0(mtIsTskHdlr);


   for (; ;)
   {
      if (read(osCp.dep.isFildes[0], &isFlag, sizeof(isFlag)) != sizeof(isFlag))
      {
         continue;
      }

      switch (isFlag.action)
      {
         case MT_IS_SET:
            osCp.drvrTskTbl[isFlag.id].dep.flag = TRUE;

            /* call the interrupt service task activation function */
            osCp.drvrTskTbl[isFlag.id].isTsk(isFlag.id);

            /* send self a message to keep doing this */
            isFlag.action = MT_IS_RESET;

#if (ERRCLASS & ERRCLS_DEBUG)
            ret = write(osCp.dep.isFildes[1], &isFlag, sizeof(isFlag));
            if (ret != sizeof(isFlag))
            {
               MTLOGERROR(ERRCLS_DEBUG, EMT020, ERRZERO,
                              "write() to pipe failed");
            }
#else
            write(osCp.dep.isFildes[1], &isFlag, sizeof(isFlag));
#endif

            break;


         case MT_IS_UNSET:
            osCp.drvrTskTbl[isFlag.id].dep.flag = FALSE;
            break;


         case MT_IS_RESET:
            if (osCp.drvrTskTbl[isFlag.id].dep.flag)
            {
               /* call the interrupt service task activation function */
               osCp.drvrTskTbl[isFlag.id].isTsk(isFlag.id);

#if (ERRCLASS & ERRCLS_DEBUG)
               /* send self a message to do this again */
               ret = write(osCp.dep.isFildes[1], &isFlag, sizeof(isFlag));

               if (ret != sizeof(isFlag))
               {
                  MTLOGERROR(ERRCLS_DEBUG, EMT021, ERRZERO,
                                 "write() to pipe failed");
               }
#else
               write(osCp.dep.isFildes[1], &isFlag, sizeof(isFlag));
#endif

            }
            break;


         default:
            /* where did THIS come from?? */
            break;
      }
   }
  /* mt009.21: addition */
  RETVALUE( (Void *) NULLP);

   /* not reached */
}
#endif /* SS_DRVR_SUPPORT */
#endif /* L2_L3_SPLIT */

/*mt010.301 Fix for core when run with -o option and when killed with SIGINT*/

/*
*
*       Fun:   mtIntSigHndlr
*
*       Desc:  Exit function, shuts down.
*
*       Ret:   Void
*
*       Notes:
*
*       File:  mt_ss.c
*
*/
#ifdef ANSI
PUBLIC Void mtIntSigHndlr
(
int arg
)
#else
PUBLIC Void mtIntSigHndlr(arg)
int arg;
#endif
{

   TRC0(mtIntSigHndlr);

   osCp.dep.sigEvnt=TRUE;

#ifdef MSPD
#ifdef TENB_RTLIN_CHANGES
   mtStopHndlr();
#endif
#endif

   RETVOID;
}

/*mt010.301 Fix for core when run with -o option and when killed with SIGINT*/
/*
*
*       Fun:   mtExitClnup
*
*       Desc:   function, shuts down.
*
*       Ret:   Void
*
*       Notes:
*
*       File:  mt_ss.c
*
*/
#ifdef ANSI
PUBLIC Void mtExitClnup
(
void
)
#else
PUBLIC Void mtExitClnup()
#endif
{
   Ticks ticks;
   S8 buf[128];


   TRC0(mtExitClnup);

   SGetSysTime(&ticks);
#ifdef ALIGN_64BIT
   sprintf(buf, "\n\nmtss(posix) ends\nticks: %u\n", ticks);
#else
   sprintf(buf, "\n\nmtss(posix) ends\nticks: %lu\n", ticks);
#endif
#ifdef SS_HISTOGRAM_SUPPORT
   SDisplay(0, buf);
#endif

   osCp.dep.sigEvnt=FALSE;
#ifndef NOFILESYS
   if (osCp.dep.fileOutFp)
   {
      fclose(osCp.dep.fileOutFp);
   }
#endif

   exit(0);
}

Ticks TtiCount = 0;
Void SIncrementTtiCount(Void)
{
   TtiCount++;
}

Ticks SGetTtiCount(Void)
{
   return TtiCount;
}

/*
*
*       Fun:   SDisplay
*
*       Desc:  This function displays a string to a given output
*              channel.
*
*       Ret:   ROK      - ok
*
*       Notes: Buffer should be null terminated.
*
*              channel 0 is reserved for backwards compatibility
*              with SPrint
*
*       File:  mt_ss.c
*
*/
#ifdef ANSI
PUBLIC S16 SDisplay
(
S16 chan,                   /* channel */
Txt *buf                    /* buffer */
)
#else
PUBLIC S16 SDisplay(chan, buf)
S16 chan;                   /* channel */
Txt *buf;                   /* buffer */
#endif
{
   TRC1(SDisplay);

/* mt020.201 - Fixed typo */
#if (ERRCLASS & ERRCLS_INT_PAR)
   if (buf == NULLP)
   {
      MTLOGERROR(ERRCLS_INT_PAR, EMT022, ERRZERO, "Null pointer");
      RETVALUE(RFAILED);
   }
#endif

#ifndef XEON_SPECIFIC_CHANGES
#ifdef TENB_T2K3K_SPECIFIC_CHANGES
   ssMemlog(buf, strlen(buf));
   RETVALUE(ROK);
#endif
#endif

 /* mt012.301 :FIX for LOG RELATED ISSUE  */
#ifdef CONAVL
   if(chan==1)
   {
       printf("%s",buf);
   }
   else
   {
    if (osCp.dep.conOutFp) fwrite(buf, strlen(buf), 1, osCp.dep.conOutFp);
   }
#endif


#ifndef NOFILESYS
   if (osCp.dep.fileOutFp)
      fwrite(buf, strlen(buf), 1, osCp.dep.fileOutFp);
   /*mt031.201 added under compile time flag FLUSHBUFF a call to fflush() */
#ifdef FLUSHBUFF

   fflush(osCp.dep.fileOutFp);

#endif
#endif

   RETVALUE(ROK);
}

/*mt010.301 */
/*
*
*       Fun:   SFini
*
*       Desc:  function, shuts down.
*
*       Ret:   Void
*
*       Notes:
*
*       File:  mt_ss.c
*
*/
#ifdef ANSI
PUBLIC S16 SFini
(
void
)
#else
PUBLIC S16 SFini()
#endif
{
   TRC1(SFini);

   /* mt030.201 added under compilet time flag SS_LINUX and SLES9_PLUS
      a loop to overcome the child processes being killed upon exiting the
      thread */
#ifdef SS_LINUX  /* this should have already been defined */
   /* mt010.301 removed flag SLES9_PLUS */
   /* wait forever for children */
   for (;;)
   {
      pause();
      if(osCp.dep.sigEvnt==TRUE)
      {
         mtExitClnup();
      }
   }

#endif
   pthread_exit(NULLP);
   RETVALUE(0);
}

/*
*
*       Fun:   Set date and time
*
*       Desc:  This function is used to set the calendar
*              date and time.
*
*       Ret:   ROK      - ok
*
*       Notes: Unimplemented
*
*       File:  mt_ss.c
*
*/
#ifdef ANSI
PUBLIC S16 SSetDateTime
(
REG1 DateTime *dt           /* date and time */
)
#else
PUBLIC S16 SSetDateTime(dt)
REG1 DateTime *dt;          /* date and time */
#endif
{
   TRC1(SSetDateTime);


   UNUSED(dt);


   RETVALUE(ROK);
}


/*
*
*       Fun:   Get date and time
*
*       Desc:  This function is used to determine the calendar
*              date and time. This information may be used for
*              some management functions.
*
*       Ret:   ROK      - ok
*              RFAILED  - error
*
*       Notes:
*
*       File:  mt_ss.c
*
*/
#ifdef ANSI
PUBLIC S16 SGetDateTime
(
REG1 DateTime *dt           /* date and time */
)
#else
PUBLIC S16 SGetDateTime(dt)
REG1 DateTime *dt;          /* date and time */
#endif
{
   /*-- mt035.201 : SSI enhancements for micro second in datetime struct --*/
   /* time_t tt; --*/
#ifndef SS_LINUX
   struct timespec ptime;
#else
   struct timeval ptime;
#endif

   struct tm tme;


   TRC1(SGetDateTime);


#if (ERRCLASS & ERRCLS_INT_PAR)
   if (dt == NULLP)
   {
      MTLOGERROR(ERRCLS_INT_PAR, EMT023, ERRZERO, "Null pointer");
      RETVALUE(RFAILED);
   }
#endif


/*-- mt035.201 --*/
   /*--
   time(&tt);
   localtime_r(&tt, &tme);
   --*/
#ifndef SS_LINUX
  clock_gettime(CLOCK_REALTIME, &ptime);
#else
  gettimeofday(&ptime, NULL);
#endif
   localtime_r(&ptime.tv_sec, &tme);

   dt->month = (U8) tme.tm_mon + 1;
   dt->day = (U8) tme.tm_mday;
   dt->year = (U8) tme.tm_year;
   dt->hour = (U8) tme.tm_hour;
   dt->min = (U8) tme.tm_min;
   dt->sec = (U8) tme.tm_sec;
   dt->tenths = 0;

#ifdef SS_DATETIME_USEC
#ifndef SS_LINUX
   dt->usec = ptime.tv_nsec / 1000;
#else
   dt->usec = ptime.tv_usec;
#endif
#endif /*-- SS_DATETIME_USEC --*/

   RETVALUE(ROK);
}

/*
* Get time from epoch in milliseconds
*
*       Fun:   Get time from epoch in milliseconds
*
*       Desc:  This function is used to get the time from epoch in milli seconds.
*              This information may be used for calculating a layer's activation function
*              execution time used for thread profiling.
*
*       Ret:   ROK      - ok
*              RFAILED  - error
*
*       Notes:
*
*       File:  mt_ss.c
*/
/* mt003.301 Modifications */
#ifdef ANSI
PUBLIC S16 SGetEpcTime
(
EpcTime *et           /* date and time */
)
#else
PUBLIC S16 SGetEpcTime(et)
EpcTime *et;          /* date and time */
#endif
{
/* mt003.301 Modifications */
PRIVATE U64 now;
		  U64  to_sec  = 1000000;
		  U64  to_nsec = 1000;
#ifndef SS_LINUX
   struct timespec ptime;
#else
   struct timeval ptime;
#endif

   TRC1(SEpcTime);


#if (ERRCLASS & ERRCLS_INT_PAR)
   if (et == NULLP)
   {
      RETVALUE(RFAILED);
   }
#endif


#ifndef SS_LINUX
  clock_gettime(CLOCK_REALTIME, &ptime);
#else
  gettimeofday(&ptime, NULL);
#endif /* SS_LINUX */

    now = (ptime.tv_sec * to_sec);

#ifndef SS_LINUX
   now += (ptime.tv_nsec / to_nsec);
#else /* SS_LINUX */
    now += (ptime.tv_usec);

#endif /* SS_LINUX */
    now = (now / to_nsec);

   *et = now;

   RETVALUE(ROK);
}



/*
*
*       Fun:   Get system time
*
*       Desc:  This function is used to determine the system time.
*
*       Ret:   ROK      - ok
*
*       Notes: osCp.dep.sysTicks is updated by the timer thread.
*
*       File:  mt_ss.c
*
*/
#ifdef ANSI
PUBLIC S16 SGetSysTime
(
Ticks *sysTime              /* system time */
)
#else
PUBLIC S16 SGetSysTime(sysTime)
Ticks *sysTime;             /* system time */
#endif
{
   TRC1(SGetSysTime);


#if (ERRCLASS & ERRCLS_INT_PAR)
   if (sysTime == NULLP)
   {
      MTLOGERROR(ERRCLS_INT_PAR, EMT024, ERRZERO, "Null pointer");
      RETVALUE(RFAILED);
   }
#endif


   *sysTime = osCp.dep.sysTicks;


   RETVALUE(ROK);
}

/* mt021.201 - Addition of SGetRefTime function */
/*
*
*       Fun:   Get referenced time
*
*       Desc:  This function is used to determine the time in seconds
*              and microseconds from a reference time.  The reference
*              time is expressed in seconds from UTC EPOC, January 1,
*              1970.
*
*       Ret:   ROK      - ok
*              RFAILED  - fail
*
*       Notes: Macros are defined for reference times:
*                 SS_REFTIME_01_01_1970
*                 SS_REFTIME_01_01_2002
*
*       File:  mt_ss.c
*
*/
#ifdef ANSI
PUBLIC S16 SGetRefTime
(
U32 refTime,             /* reference time */
U32 *sec,
U32 *usec
)
#else
PUBLIC S16 SGetRefTime(refTime, sec, usec)
U32 refTime;             /* reference time */
U32 *sec;
U32 *usec;
#endif
{

#ifndef SS_LINUX
   struct timespec ptime;
#else
   struct timeval ptime;
#endif

   TRC1(SGetSysTime);

#ifndef SS_LINUX
  clock_gettime(CLOCK_REALTIME, &ptime);
#else
  gettimeofday(&ptime, NULL);
#endif

#if (ERRCLASS & ERRCLS_INT_PAR)
   if (sec == NULLP || usec == NULLP)
   {
      MTLOGERROR(ERRCLS_INT_PAR, EMT025, ERRZERO, "Null pointer");
      RETVALUE(RFAILED);
   }
   /* mt022.201 - Modification to fix compile warning */
   if (refTime > (U32)(ptime.tv_sec))
   {
      MTLOGERROR(ERRCLS_INT_PAR, EMT026, ERRZERO, "Reference time exceeds present time");
      RETVALUE(RFAILED);
   }
#endif

   *sec = ptime.tv_sec - refTime;
#ifndef SS_LINUX
   *usec = ptime.tv_nsec / 1000;
#else
   *usec = ptime.tv_usec;
#endif

  RETVALUE(ROK);

}


/*
*
*       Fun:   Get Random Number
*
*       Desc:  Invoked by layer when a pseudorandom number is required.
*
*       Ret:   ROK      - ok
*
*       Notes: Suggested approach uses shuffled Linear Congruential
*              Operators as described in Byte magazine October
*              1984; "Generating and Testing Pseudorandom Numbers"
*
*       File:  mt_ss.c
*
*/
#ifdef ANSI
PUBLIC S16 SRandom
(
Random *value               /* random number */
)
#else
PUBLIC S16 SRandom(value)
Random *value;              /* random number */
#endif
{
   TRC1(SRandom);


#if (ERRCLASS & ERRCLS_INT_PAR)
   if (value == NULLP)
   {
 /* mt011.21: addition */
      MTLOGERROR(ERRCLS_INT_PAR, EMT028, (ErrVal)0 , "Null pointer");
      RETVALUE(RFAILED);
   }
#endif


   *value = (Random) rand_r(&osCp.dep.randSeed);


   RETVALUE(ROK);
}


/*
*
*       Fun:   Exit Task
*
*       Desc:  This function exits from a task.
*
*       Ret:   ROK      - ok
*
*       Notes: Currently does nothing.
*
*       File:  mt_ss.c
*
*/
#ifdef ANSI
PUBLIC S16 SExitTsk
(
void
)
#else
PUBLIC S16 SExitTsk()
#endif
{
   TRC1(SExitTsk);


   RETVALUE(ROK);
}


/*
*
*       Fun:   Exit Interrupt
*
*       Desc:  This function exits from an interrupt.
*
*       Ret:   ROK      - ok
*
*       Notes: Currently does nothing.
*
*       File:  mt_ss.c
*
*/
#ifdef ANSI
PUBLIC S16 SExitInt
(
void
)
#else
PUBLIC S16 SExitInt()
#endif
{
   TRC1(SExitInt);


   RETVALUE(ROK);
}


/*
*
*       Fun:   Hold Interrupt
*
*       Desc:  This function prohibits interrupts from being enabled until
*              release interrupt. This function should be called when
*              interrupts are disabled and prior to any call to system
*              services either by entry to an interrupt service routine or
*              by explicit call to disable interrupt.
*
*       Ret:   ROK      - ok
*
*       Notes: Currently does nothing
*
*       File:  mt_ss.c
*
*/
#ifdef ANSI
PUBLIC S16 SHoldInt
(
void
)
#else
PUBLIC S16 SHoldInt()
#endif
{
   TRC1(SHoldInt);


   RETVALUE(ROK);
}


/*
*
*       Fun:   Release Interrupt
*
*       Desc:  This function allows interrupts to be enabled.
*
*       Ret:   ROK      - ok
*
*       Notes: Currently does nothing.
*
*       File:  mt_ss.c
*
*/
#ifdef ANSI
PUBLIC S16 SRelInt
(
void
)
#else
PUBLIC S16 SRelInt()
#endif
{
   TRC1(SRelInt);


   RETVALUE(ROK);
}


/*
*
*       Fun:   SEnbInt
*
*       Desc:  Enable interrupts
*
*       Ret:   ROK on success
*              RFAILED on error
*
*       Notes: Currently does nothing.
*
*       File:  mt_ss.c
*
*/
#ifdef ANSI
PUBLIC INLINE S16 SEnbInt
(
void
)
#else
PUBLIC INLINE S16 SEnbInt()
#endif
{
   TRC1(SEnbInt);


   RETVALUE(ROK);
}


/*
*
*       Fun:   SDisInt
*
*       Desc:  Disable interrupts
*
*       Ret:   ROK on success
*              RFAILED on error
*
*       Notes: Currently does nothing.
*
*       File:  mt_ss.c
*
*/
#ifdef ANSI
PUBLIC INLINE S16 SDisInt
(
void
)
#else
PUBLIC INLINE S16 SDisInt()
#endif
{
   TRC1(SDisInt);


   RETVALUE(ROK);
}


/*
*
*       Fun:   Get Vector
*
*       Desc:  This function gets the function address stored at the
*              specified interrupt vector.
*
*       Ret:   ROK      - ok
*
*       Notes: Currently does nothing.
*
*       File:  mt_ss.c
*
*/
#ifdef ANSI
PUBLIC S16 SGetVect
(
VectNmb vectNmb,                /* vector number */
PIF *vectFnct                   /* vector function */
)
#else
PUBLIC S16 SGetVect(vectNmb, vectFnct)
VectNmb vectNmb;                /* vector number */
PIF *vectFnct;                  /* vector function */
#endif
{
   TRC1(SGetVect);


   UNUSED(vectNmb);
   UNUSED(vectFnct);


   RETVALUE(ROK);
}


/*
*
*       Fun:   Put Vector
*
*       Desc:  This function installs the specified function at the
*              specified interrupt vector.
*
*       Ret:   ROK      - ok
*
*       Notes: Currently does nothing.
*
*       File:  mt_ss.c
*
*/
#ifdef ANSI
PUBLIC S16 SPutVect
(
VectNmb vectNmb,                /* vector number */
PIF vectFnct                    /* vector function */
)
#else
PUBLIC S16 SPutVect(vectNmb, vectFnct)
VectNmb vectNmb;                /* vector number */
PIF vectFnct;                   /* vector function */
#endif
{
   TRC1(SPutVect);


   UNUSED(vectNmb);
   UNUSED(vectFnct);


   RETVALUE(ROK);
}

/* mt028.201: modification: multiple procs support related changes */
#ifndef SS_MULTIPLE_PROCS

/*
*
*       Fun:   SGetEntInst
*
*       Desc:  This function gets the current entity and instance.
*
*       Ret:   ROK      - ok
*              RFAILED  - failed, general (optional)
*
*       Notes: This function may be called by the OS or Layer 1
*              hardware drivers.
*
*       File:  mt_ss.c
*
*/
#ifdef ANSI
PUBLIC S16 SGetEntInst
(
Ent *ent,                       /* entity */
Inst *inst                      /* instance */
)
#else
PUBLIC S16 SGetEntInst(ent, inst)
Ent *ent;                       /* entity */
Inst *inst;                     /* instance */
#endif
{
   S16 i;
   S16 ret;
   pthread_t tId;
   SsSTskEntry *sTsk;


   TRC1(SGetEntInst);


#if (ERRCLASS & ERRCLS_INT_PAR)
   /* check pointers */
   if (ent == NULLP  ||  inst == NULLP)
   {
      MTLOGERROR(ERRCLS_INT_PAR, EMT029, ERRZERO, "Null pointer");
      RETVALUE(RFAILED);
   }
#endif


   /* get the thread id */
   tId = pthread_self();


   /* find the system task in whose context we're running */
   sTsk = NULLP;
   ret = SLock(&osCp.sTskTblLock);
   if (ret != ROK)
   {
      RETVALUE(RFAILED);
   }
   for (i = 0;  i < SS_MAX_STSKS;  i++)
   {
      if (pthread_equal(osCp.sTskTbl[i].dep.tId, tId))
      {
         sTsk = &osCp.sTskTbl[i];
         break;
      }
   }
   if (sTsk != NULLP)
   {
         *ent = sTsk->dep.ent;
         *inst = sTsk->dep.inst;
   }
   SUnlock(&osCp.sTskTblLock);


   RETVALUE(ret == ROK ? ROK : RFAILED);
}


/*
*
*       Fun:   SSetEntInst
*
*       Desc:  This function sets the current entity and instance.
*
*       Ret:   ROK      - ok
*
*       Notes:
*
*       File:  mt_ss.c
*
*/
#ifdef ANSI
PUBLIC S16 SSetEntInst
(
Ent ent,                    /* entity */
Inst inst                   /* instance */
)
#else
PUBLIC S16 SSetEntInst(ent, inst)
Ent ent;                    /* entity */
Inst inst;                  /* instance */
#endif
{
   S16 i;
   S16 ret;
   pthread_t tId;
   SsSTskEntry *sTsk;


   TRC1(SSetEntInst);


#if (ERRCLASS & ERRCLS_INT_PAR)
   /* check entity and instance IDs */
   if (ent >= ENTNC  ||  inst >= INSTNC)
   {
      MTLOGERROR(ERRCLS_INT_PAR, EMT030, ERRZERO, "Invalid entity/instance");
      RETVALUE(RFAILED);
   }
#endif


   /* get the thread id */
   tId = pthread_self();


   /* find the system task in whose context we're running */
   sTsk = NULLP;
   ret = SLock(&osCp.sTskTblLock);
   if (ret != ROK)
   {
      RETVALUE(RFAILED);
   }
   for (i = 0;  i < SS_MAX_STSKS;  i++)
   {
      if (pthread_equal(osCp.sTskTbl[i].dep.tId, tId))
      {
         sTsk = &osCp.sTskTbl[i];
         break;
      }
   }
   if (sTsk != NULLP)
   {
         sTsk->dep.ent = ent;
         sTsk->dep.inst = inst;
   }
   SUnlock(&osCp.sTskTblLock);


   RETVALUE(ret == ROK ? ROK : RFAILED);
}

#endif /* SS_MULTIPLE_PROCS */

#ifdef SS_DRVR_SUPPORT

/*
*
*       Fun:   SSetIntPend
*
*       Desc:  Set interrupt pending flag
*
*       Ret:   ROK on success
*              RFAILED on error
*
*       Notes:
*
*       File:  mt_ss.c
*
*/
#ifdef ANSI
PUBLIC INLINE S16 SSetIntPend
(
U16 id,                         /* driver task identifier */
Bool flag                       /* flag */
)
#else
PUBLIC INLINE S16 SSetIntPend(id, flag)
U16 id;                         /* driver task identifier */
Bool flag;                      /* flag */
#endif
{
   MtIsFlag isFlag;


   TRC1(SSetIntPend);


#if (ERRCLASS & ERRCLS_INT_PAR)
   if (id >= SS_MAX_DRVRTSKS  ||  osCp.drvrTskTbl[id].used == FALSE)
   {
      MTLOGERROR(ERRCLS_INT_PAR, EMT031, id, "Invalid instance");
      RETVALUE(RFAILED);
   }
#endif


   isFlag.id = id;
   isFlag.action = (flag ? MT_IS_SET : MT_IS_UNSET);

   if (write(osCp.dep.isFildes[1], &isFlag, sizeof(isFlag)) != sizeof(isFlag))
   {
      RETVALUE(RFAILED);
   }


   RETVALUE(ROK);
}
#endif  /* SS_DRVR_SUPPORT */


#ifdef SS_LOCKLESS_MEMORY
/*
*
*       Fun:   SGlobMemInfoShow
*
*       Desc:  This function displays the memory usage information
*              for the destined region. It will show the usage of
*              each configured bucket and the heap for the specified region.
*
*       Ret:   ROK		OK
*              RFAILED		Region not registered
*
*       File:  mt_ss.c
*
*/
#ifdef ANSI
PUBLIC S16 SGlobMemInfoShow
(
Void
)
#else
PUBLIC S16 SGlobMemInfoShow()
#endif
{
   U16   idx;
   Txt   prntBuf[100];
   CmMmGlobRegCb   *globReg;
   
   TRC1(SGlobMemInfoShow);

   globReg = osCp.globRegCb;

   sprintf(prntBuf, "--------------------------------------------------------------\n");
   SDisplay(0, prntBuf);
   sprintf(prntBuf, "Global Region Bucket Information\n");
   SDisplay(0, prntBuf);
   sprintf(prntBuf, "====================================================\n");
   SDisplay(0, prntBuf);
   sprintf(prntBuf, "Bucket Id  Set Size  Free Sets  Allocated\n");
   SDisplay(0, prntBuf);
   sprintf(prntBuf, "====================================================\n");
   SDisplay(0, prntBuf);
   

   for (idx = 0; idx < globReg->numBkts; idx++)
   {
#ifdef XEON_SPECIFIC_CHANGES
      sprintf(prntBuf, "%2u  %12lu  %12lu  %8lu %9lu\n",
              idx, globReg->bktTbl[idx].size, globReg->bktTbl[idx].bucketSetSize, globReg->bktTbl[idx].listValidBktSet.count, globReg->bktTbl[idx].listFreeBktSet.count);
#else      
#ifndef ALIGN_64BIT
      sprintf(prntBuf, "%2u  %12lu  %8lu %9lu\n", 
              idx, globReg->bktTbl[idx].bucketSetSize, globReg->bktTbl[idx].listValidBktSet.count, globReg->bktTbl[idx].listFreeBktSet.count);
#else
      sprintf(prntBuf, "%2u  %12u  %8u %9u\n", 
              idx, globReg->bktTbl[idx].bucketSetSize, globReg->bktTbl[idx].listValidBktSet.count, globReg->bktTbl[idx].listFreeBktSet.count);
#endif
#endif      
      SDisplay(0, prntBuf);
   }
   sprintf(prntBuf, "--------------------------------------------------------------\n");
   SDisplay(0, prntBuf);

   RETVALUE(ROK);
}   

#endif /* SS_LOCKLESS_MEMORY */

/*
Bool IsMemoryThresholdHit(Region reg, Pool pool)
{
  if((mtCMMRegCb[reg]->bktTbl[pool].numAlloc * 100 )/mtCMMRegCb[reg]->bktTbl[pool].numBlks > 70)
  {
     MSPD_DBG("Threshold reached reg(%d) pool(%d) numAllc(%d) numBlks(%d)\n",
               reg,
               pool,
               mtCMMRegCb[reg]->bktTbl[pool].numAlloc,
               mtCMMRegCb[reg]->bktTbl[pool].numBlks);
     RETVALUE(TRUE);
  }
  RETVALUE(FALSE);
}
*/

/* mt022.201 - Addition of SRegInfoShow function */
/*
*
*       Fun:   SRegInfoShow
*
*       Desc:  This function displays the memory usage information
*              for the destined region. It will show the usage of
*              each configured bucket and the heap for the specified region.
*
*       Ret:   ROK		OK
*              RFAILED		Region not registered
*
*       Notes: A Sample Output from the function
*       Bucket Memory: region 1
*       ====================================================
*       Bucket  Number of Blks configured  Size  Allocated
*       ====================================================
*       0                     1             16         1
*       1                     1             32         0
*       2                     1             80         0
*       3                     1            256         0
*       4                     1            320         0
*
*       ---------------
*       Heap Memory: region 1
*       Heap Size: 0
*       Heap Allocated: 0
*       Heap Segmented blocks: 0
*
*
*       File:  mt_ss.c
*
*/
#ifdef ANSI
PUBLIC S16 SRegInfoShow
(
Region region,
U32 *availmem
)
#else
PUBLIC S16 SRegInfoShow(region, availmem)
Region region;
U32 *availmem;
#endif
{
   U16   idx;
   Txt   prntBuf[100];

   TRC1(SRegInfoShow);

#if (ERRCLASS & ERRCLS_INT_PAR)
   if (region > (SS_MAX_REGS-1) )
   {
      MTLOGERROR(ERRCLS_INT_PAR, EMT032, ERRZERO, "Invalid Region");
      RETVALUE(RFAILED);
   }
#endif

   *availmem = 0;

#ifndef TENB_T2K3K_SPECIFIC_CHANGES
   sprintf(prntBuf, "\n\nBucket Memory: region %d\n", region);
   SDisplay(0, prntBuf);
   sprintf(prntBuf, "====================================================\n");
   SDisplay(0, prntBuf);
   sprintf(prntBuf, "Bucket  Number of Blks configured  Size  Allocated\n");
   SDisplay(0, prntBuf);
   sprintf(prntBuf, "====================================================\n");
   SDisplay(0, prntBuf);
#endif


   for (idx = 0; idx < mtCMMRegCb[region]->numBkts; idx++)
   {
#ifdef TENB_T2K3K_SPECIFIC_CHANGES
#ifdef ALIGN_64BIT
      sprintf((char *)prntBuf, "%2u              %8u          %5u  %8u  %8u\n",
            idx, mtCMMRegCb[region]->bktTbl[idx].numBlks,
            mtCMMRegCb[region]->bktTbl[idx].size,
            mtCMMRegCb[region]->bktTbl[idx].numAlloc,
            mtCMMRegCb[region]->bktTbl[idx].maxAlloc);
#else
      sprintf((char *)prntBuf, "%2u              %8lu          %5lu  %8lu  %8lu\n",
            idx, mtCMMRegCb[region]->bktTbl[idx].numBlks,
            mtCMMRegCb[region]->bktTbl[idx].size,
            mtCMMRegCb[region]->bktTbl[idx].numAlloc,
            mtCMMRegCb[region]->bktTbl[idx].maxAlloc);
#endif
#else
/*mt009.301 Fixed 64BIT compilation warnings*/
#ifdef ALIGN_64BIT
      sprintf(prntBuf, "%2u              %8u          %5u  %8u\n",
              idx, mtCMMRegCb[region]->bktTbl[idx].numBlks,
              mtCMMRegCb[region]->bktTbl[idx].size,
              mtCMMRegCb[region]->bktTbl[idx].numAlloc);
#else
      sprintf(prntBuf, "%2u              %8lu          %5lu  %8lu\n",
              idx, mtCMMRegCb[region]->bktTbl[idx].numBlks,
              mtCMMRegCb[region]->bktTbl[idx].size,
              mtCMMRegCb[region]->bktTbl[idx].numAlloc);
#endif
#endif /* not TENB_RTLIN_CHANGES */
      SDisplay(0, prntBuf);
      *availmem = *availmem + (mtCMMRegCb[region]->bktTbl[idx].size * \
          (mtCMMRegCb[region]->bktTbl[idx].numBlks -  \
	   mtCMMRegCb[region]->bktTbl[idx].numAlloc));
   }
   sprintf(prntBuf, "\n---------------\n");
   SDisplay(0, prntBuf);
   sprintf(prntBuf, "Heap Memory: region %d\n", region);
   SDisplay(0, prntBuf);
/*mt009.301 Fixed 64BIT compilation warnings*/
#ifdef ALIGN_64BIT
   sprintf(prntBuf, "Heap Size: %u\n", mtCMMRegCb[region]->heapSize);
#else
   sprintf(prntBuf, "Heap Size: %lu\n", mtCMMRegCb[region]->heapSize);
#endif
   SDisplay(0, prntBuf);
/*mt009.301 Fixed 64BIT compilation warnings*/
#ifdef ALIGN_64BIT
   sprintf(prntBuf, "Heap Allocated: %u\n",
           (mtCMMRegCb[region]->heapSize - mtCMMRegCb[region]->heapCb.avlSize));
#else
   sprintf(prntBuf, "Heap Allocated: %lu\n",
           (mtCMMRegCb[region]->heapSize - mtCMMRegCb[region]->heapCb.avlSize));
#endif
   SDisplay(0, prntBuf);
   *availmem = *availmem + mtCMMRegCb[region]->heapCb.avlSize;
#if (ERRCLASS & ERRCLS_DEBUG)
   sprintf(prntBuf, "Heap Segmented blocks: %d\n",
                    mtCMMRegCb[region]->heapCb.numFragBlk);
   SDisplay(0, prntBuf);
#endif

   RETVALUE(ROK);
}
#ifdef XEON_SPECIFIC_CHANGES
#define  SSI_MAX_BKT_THRESHOLD 6
#define  SSI_MAX_REG_THRESHOLD 2
U32 SMemMaxThreshold[SSI_MAX_REG_THRESHOLD][SSI_MAX_BKT_THRESHOLD] = {{0}};
U32 SMemMidThreshold[SSI_MAX_REG_THRESHOLD][SSI_MAX_BKT_THRESHOLD] = {{0}};
U32 SMemLowThreshold[SSI_MAX_REG_THRESHOLD][SSI_MAX_BKT_THRESHOLD] = {{0}};

#ifdef ANSI
PRIVATE Void SInitMemThreshold
(
Region region,
U8     maxBkt
)
#else
PRIVATE Void SInitMemThreshold(region, maxBkt)
Region region;
U8     maxBkt;
#endif
{
   U8   idx = 0;
   for (idx = 0; (idx < maxBkt && idx < mtCMMRegCb[region]->numBkts); idx++)
   {
      SMemMaxThreshold[region][idx] = (mtCMMRegCb[region]->bktTbl[idx].numBlks*95)/100;
      SMemMidThreshold[region][idx] = (mtCMMRegCb[region]->bktTbl[idx].numBlks*85)/100;
      SMemLowThreshold[region][idx] = (mtCMMRegCb[region]->bktTbl[idx].numBlks*80)/100;
      printf("REGION:%d, BKT:%d max:%d mid:%d low:%d\n", region, idx, SMemMaxThreshold[region][idx], SMemMidThreshold[region][idx], SMemLowThreshold[region][idx]);
   }
}

#ifdef ANSI
PUBLIC S16 SRegReachedMemThreshold
(
Region region,
U8     maxBkt
)
#else
PUBLIC S16 SRegReachedMemThreshold(region, maxBkt)
Region region;
U8     maxBkt;
#endif
{
   U8           idx       = 0;
   U8           memStatus = 3;
   PRIVATE U8   initFlag  = 1;
   if(initFlag)
   {
      initFlag = 0;
      SInitMemThreshold(region, maxBkt);
   }

   for (idx = 0; (idx < maxBkt && idx < mtCMMRegCb[region]->numBkts); idx++)
   {
      if(mtCMMRegCb[region]->bktTbl[idx].numAlloc >= SMemMaxThreshold[region][idx])
      {
         memStatus = 0;
         break;
      }
      else if((mtCMMRegCb[region]->bktTbl[idx].numAlloc >= SMemMidThreshold[region][idx]) && (memStatus >1))
      {
         memStatus = 1;
      }
      else if((mtCMMRegCb[region]->bktTbl[idx].numAlloc >= SMemLowThreshold[region][idx]) && (memStatus >2))
      {
         memStatus = 2;
      }
   }
   RETVALUE(memStatus);
}
#endif
/* mt033.201 - addition of API to return the memory statistical data */
/*
*
*       Fun:   SGetRegInfo
*
*       Desc:  This function returns the memory usage information
*              for the destined region. It will return the usage of
*              each configured bucket and the heap for the specified region.
*
*       Ret:   ROK   OK
*              RFAILED   Region not registered
*
*       Notes:
*
*       File:  mt_ss.c
*
*/
#ifdef ANSI
PUBLIC S16 SGetRegInfo
(
Region region,
SsMemDbgInfo *dbgInfo
)
#else
PUBLIC S16 SGetRegInfo(region, dbgInfo)
Region region;
SsMemDbgInfo *dbgInfo;
#endif
{
   U32 idx;

   TRC1(SGetRegInfo);

#if (ERRCLASS & ERRCLS_INT_PAR)
   if (region >= mtMemoCfg.numRegions )
   {
      MTLOGERROR(ERRCLS_INT_PAR, EMT033, ERRZERO, "Invalid Region");
      RETVALUE(RFAILED);
   }
#endif

   dbgInfo->availmem = 0;

   if (mtCMMRegCb[region]->numBkts > SS_MAX_BKT_PER_DBGTBL)
      dbgInfo->numBkts = SS_MAX_BKT_PER_DBGTBL;
   else
      dbgInfo->numBkts = mtCMMRegCb[region]->numBkts;

   for (idx = 0; (idx < mtCMMRegCb[region]->numBkts) && (idx < SS_MAX_BKT_PER_DBGTBL); idx++)
   {
      dbgInfo->bktDbgTbl[idx].numBlks = mtCMMRegCb[region]->bktTbl[idx].numBlks;
      dbgInfo->bktDbgTbl[idx].size = mtCMMRegCb[region]->bktTbl[idx].size;
      dbgInfo->bktDbgTbl[idx].numAlloc = mtCMMRegCb[region]->bktTbl[idx].numAlloc;

      dbgInfo->availmem += (mtCMMRegCb[region]->bktTbl[idx].size * \
                           (mtCMMRegCb[region]->bktTbl[idx].numBlks -  \
                            mtCMMRegCb[region]->bktTbl[idx].numAlloc));
   }

   dbgInfo->region = region;

   dbgInfo->heapSize = mtCMMRegCb[region]->heapSize;

   dbgInfo->heapAlloc = (mtCMMRegCb[region]->heapSize - \
                         mtCMMRegCb[region]->heapCb.avlSize);

   dbgInfo->availmem += mtCMMRegCb[region]->heapCb.avlSize;

#if (ERRCLASS & ERRCLS_DEBUG)
   dbgInfo->numFragBlk = mtCMMRegCb[region]->heapCb.numFragBlk;
#endif

   RETVALUE(ROK);
}

#ifdef ANSI
PUBLIC S16 SGetRegPoolInfo
(
U8 *numRegion,
U8 *numPool
)
#else
PUBLIC S16 SGetRegPoolInfo(numRegion, numPool)
U8 *numRegion;
U8 *numPool;
#endif
{
   /* Send number of Region available */
   *numRegion = mtMemoCfg.numRegions;
   /* Send number of Pools available */
   *numPool = cfgRegInfo[0].numPools;

   RETVALUE(ROK);
}

/* mt033.201 - addition of APIs to print the memory statistical data
 * as defined by SSI enhancements
 */
#ifdef SSI_DEBUG_LEVEL1
/*
 *
 *       Fun:   SPrintRegMemStatusInfo
 *
 *       Desc:  This function displays the memory usage information
 *              for the destined region. It will show the total memory
 *              used for static and dynamic memory if typeFlag is
 *              SS_MEM_BKT_ALLOC_PROFILE. It will show the number of
*              memory block allocated for a particular size if typeFlag
*              is SS_MEM_BLK_SIZE_PROFILE from the hash list by
*              calling SRegPrintMemStats.
*
*       Ret:   ROK
*
*       Notes:
*
*       File:  mt_ss.c
*
*/
#ifdef ANSI
PUBLIC S16 SPrintRegMemStatusInfo
(
Region region,
U8 typeFlag
)
#else
PUBLIC S16 SPrintRegMemStatusInfo(region, typeFlag)
Region region;
U8 typeFlag;
#endif
{
   Txt prntBuf[150];
   U32 idx;
   U32 statMemSize;
   U32 dynMemSize;

   TRC1(SPrintRegMemStatusInfo);

#if (ERRCLASS & ERRCLS_INT_PAR)
   if (region >= mtMemoCfg.numRegions )
   {
      MTLOGERROR(ERRCLS_INT_PAR, EMT034, ERRZERO, "Invalid Region");
      RETVALUE(RFAILED);
   }
#endif

   /* initialize the counters*/
   statMemSize = 0;
   dynMemSize = 0;

   if (typeFlag == SS_MEM_BKT_ALLOC_PROFILE)
   {
      /* total static and dynamic memory allocated from all the buckets in region requested */
      sprintf(prntBuf, "\nAllocated Memory profile of Buckets from region: %d \n", region);
      SDisplay(0, prntBuf);
      sprintf(prntBuf, "===========================================\n");
      SDisplay(0, prntBuf);
      sprintf(prntBuf, "Bucket        Static Memory      Dynamic Memory\n");
      SDisplay(0, prntBuf);
      sprintf(prntBuf, "===========================================\n");
      SDisplay(0, prntBuf);
      for (idx = 0; idx < mtCMMRegCb[region]->numBkts; idx++)
      {
/*mt009.301 Fixed 64BIT compilation warnings*/
#ifdef ALIGN_64BIT
         sprintf(prntBuf, "%2u           %8u           %8u\n", idx,
                     mtCMMRegCb[region]->bktTbl[idx].staticMemUsed,
                     mtCMMRegCb[region]->bktTbl[idx].dynamicMemUsed);
#else
         sprintf(prntBuf, "%2lu           %8lu           %8lu\n", idx,
                     mtCMMRegCb[region]->bktTbl[idx].staticMemUsed,
                     mtCMMRegCb[region]->bktTbl[idx].dynamicMemUsed);
#endif
         SDisplay(0, prntBuf);
         /* update the total count */
         statMemSize += mtCMMRegCb[region]->bktTbl[idx].staticMemUsed;
         dynMemSize += mtCMMRegCb[region]->bktTbl[idx].dynamicMemUsed;
      }
      /* from buckets */
/*mt009.301 Fixed 64BIT compilation warnings*/
#ifdef ALIGN_64BIT
      sprintf(prntBuf, "Total Static Memory allocated from buckets: %u\n", statMemSize);
      SDisplay(0, prntBuf);
      sprintf(prntBuf, "Total Dynamic Memory allocated from buckets: %u\n", dynMemSize);
#else
      sprintf(prntBuf, "Total Static Memory allocated from buckets: %lu\n", statMemSize);
      SDisplay(0, prntBuf);
      /*mt010.301 fix for compilation error*/
      sprintf(prntBuf, "Total Dynamic Memory allocated from buckets: %lu\n", dynMemSize);
#endif
      SDisplay(0, prntBuf);
      /* from heap */
      sprintf(prntBuf, "\n\nAllocated Memory profile from Heap of region: %d \n", region);
      SDisplay(0, prntBuf);
/*mt009.301 Fixed 64BIT compilation warnings*/
#ifdef ALIGN_64BIT
      sprintf(prntBuf, "STATIC MEMORY: %u	DYNAMIC MEMORY:%u \n",
         mtCMMRegCb[region]->heapCb.staticHeapMemUsed, mtCMMRegCb[region]->heapCb.dynamicHeapMemUsed);
#else
      sprintf(prntBuf, "STATIC MEMORY: %lu	DYNAMIC MEMORY:%lu \n",
         mtCMMRegCb[region]->heapCb.staticHeapMemUsed, mtCMMRegCb[region]->heapCb.dynamicHeapMemUsed);
#endif
      SDisplay(0, prntBuf);
   }
   else if (typeFlag == SS_MEM_BLK_SIZE_PROFILE)
   {
      /* Bucket Memory allocation Statistics */
      RETVALUE(SPrintRegMemStats(region));
   }
   else
   {
      /* error case */
      sprintf(prntBuf, "\n Invalid choice \n");
      SDisplay(0, prntBuf);
   }

   RETVALUE(ROK);
}

/*
*
*       Fun:   SPrintRegMemStats
*
*       Desc:  This function displays the memory usage information for
*              the destined region. It will show the number of memory
*              block allocated for a particular size from the hash list.
*
*       Ret:   ROK
*
*       Notes:
*
*       File:  mt_ss.c
*
*/
#ifdef ANSI
PRIVATE S16 SPrintRegMemStats
(
Region region
)
#else
PRIVATE S16 SPrintRegMemStats(region)
Region region;
#endif
{
   CmMmHashListCp *hashListCp;
   Txt prntBuf[150];
   U32 idx;
   U32 cntEnt;

   TRC1(SPrintRegMemStats);

   hashListCp = &mtCMMRegCb[region]->hashListCp;

   sprintf(prntBuf, "\n\nSize Vs. NumAttempts and Alloc/Dealloc profile of region %d\n", region);
   SDisplay(0, prntBuf);
   sprintf(prntBuf, "Maximum Entries: %u    Current Entries: %u\n",
                     hashListCp->numOfbins, hashListCp->numOfEntries);
   SDisplay(0, prntBuf);
   sprintf(prntBuf, "===================================\n");
   SDisplay(0, prntBuf);
   sprintf(prntBuf, "Block Size      Total number of requests\n");
   SDisplay(0, prntBuf);
   sprintf(prntBuf, "===================================\n");
   SDisplay(0, prntBuf);

   for (idx = 0, cntEnt=0; (cntEnt < hashListCp->numOfEntries) &&
               (idx < CMM_STAT_HASH_TBL_LEN); idx++)
   {
      if (hashListCp->hashList[idx].numAttempts)
      {
         cntEnt++;
/*mt009.301 Fixed 64BIT compilation warnings*/
#ifdef ALIGN_64BIT
         sprintf(prntBuf, "%8u           %8u\n", hashListCp->hashList[idx].size,
                     hashListCp->hashList[idx].numAttempts);
#else
         sprintf(prntBuf, "%8lu           %8lu\n", hashListCp->hashList[idx].size,
                     hashListCp->hashList[idx].numAttempts);
#endif
         SDisplay(0, prntBuf);
      }
   }

   sprintf(prntBuf, "\nAllocation/De-allocation profile in Buckets\n");
   SDisplay(0, prntBuf);
   sprintf(prntBuf, "=================================================\n");
   SDisplay(0, prntBuf);
   sprintf(prntBuf, "Bucket    Num of Alloc Attempts    Num of De-alloc Attempts\n");
   SDisplay(0, prntBuf);
   sprintf(prntBuf, "=================================================\n");
   SDisplay(0, prntBuf);

   /* Print the statistics of total number of alloc/de-alloc attempts in each bucket of this region */
   for (idx = 0; idx < mtCMMRegCb[region]->numBkts; idx++)
   {
/*mt009.301 Fixed 64BIT compilation warnings*/
#ifdef ALIGN_64BIT
      sprintf(prntBuf, "%4u        %8u             %8u\n", idx,
                           mtCMMRegCb[region]->bktTbl[idx].numAllocAttempts,
                           mtCMMRegCb[region]->bktTbl[idx].numDeallocAttempts);
#else
      sprintf(prntBuf, "%4lu        %8lu             %8lu\n", idx,
                           mtCMMRegCb[region]->bktTbl[idx].numAllocAttempts,
                           mtCMMRegCb[region]->bktTbl[idx].numDeallocAttempts);
#endif
      SDisplay(0, prntBuf);
   }
   sprintf(prntBuf, "\nAllocation/De-allocation profile in Heap\n");
   SDisplay(0, prntBuf);
/*mt009.301 Fixed 64BIT compilation warnings*/
#ifdef ALIGN_64BIT
   sprintf(prntBuf, "Num of Alloc Attempts: %u      Num of De-alloc Attempts: %u\n",
                           mtCMMRegCb[region]->heapCb.numAllocAttempts,
                           mtCMMRegCb[region]->heapCb.numDeallocAttempts);
#else
   sprintf(prntBuf, "Num of Alloc Attempts: %lu      Num of De-alloc Attempts: %lu\n",
                           mtCMMRegCb[region]->heapCb.numAllocAttempts,
                           mtCMMRegCb[region]->heapCb.numDeallocAttempts);
#endif
   SDisplay(0, prntBuf);
   sprintf(prntBuf, "\n");
   SDisplay(0, prntBuf);

   RETVALUE(ROK);
}

/*
*
*       Fun:   SRegMemErrHdlr
*
*       Desc:  This function handles the errors returned from the memory
*              related functions. Customers are suggested to modify this
*              API according to their specific requirement.
*
*       Ret:   ROK   OK
*
*       Notes:
*
*       File:  mt_ss.c
*
*/
#ifdef ANSI
PUBLIC Void SRegMemErrHdlr
(
Region region,
Data *ptr,
S16 errCode
)
#else
PUBLIC Void SRegMemErrHdlr(region, ptr, errCode)
Region region;
Data *ptr;
S16 errCode;
#endif
{
   Txt prntBuf[150];

   TRC1(SRegMemErrHdlr);

   if (errCode == RDBLFREE)
   {
      sprintf(prntBuf, "\nDouble free attempted at location:%8p  in region:%d\n", ptr, region);
      SDisplay(0, prntBuf);
   }
   else if (errCode == RTRAMPLINGNOK)
   {
      sprintf(prntBuf, "\nMemory Trampling crossed Threshold in region:%d\n", region);
      SDisplay(0, prntBuf);
   }

   RETVOID;
}

/*
*
*       Fun:   SPrintRegMemProfile
*
*       Desc:  This function displays the memory profile information
*              for the destined region. This function prints for:
*              1) each memory bucket-Block address, size, size for which it is allocated, free/allocated, static/dynamic
*              2) heap - memory block address, size, requested size, free/allocated, static/dynamic
*
*       Ret:   ROK   OK
*
*       Notes:
*
*       File:  mt_ss.c
*
*/
#ifdef ANSI
PUBLIC S16 SPrintRegMemProfile
(
Region region
)
#else
PUBLIC S16 SPrintRegMemProfile(region)
Region region;
#endif
{
   CmMmHeapCb *heapCb;
   CmMmRegCb *regCb;
   CmMmBlkHdr *curBktBlk;
   CmHEntry *curHBlk;
   Size offsetToNxtBlk;
   Size hdrSize;
   Txt prntBuf[250];
   U32 idx;
   U32 blkCnt;


   TRC1(SPrintRegMemProfile);

#if (ERRCLASS & ERRCLS_INT_PAR)
   if (region >= mtMemoCfg.numRegions )
   {
      MTLOGERROR(ERRCLS_INT_PAR, EMT035, ERRZERO, "Invalid Region");
      RETVALUE(RFAILED);
   }
#endif

   regCb = mtCMMRegCb[region];

   /* memory profile */
   sprintf(prntBuf, "\n\nFull Memory Profile of region %d\n", region);
   SDisplay(0, prntBuf);

   /* bucket profile */
   sprintf(prntBuf, "\nBucket Profile\n");
   SDisplay(0, prntBuf);

   for (idx = 0; idx < regCb->numBkts; idx++)
   {

/*mt009.301 Fixed 64BIT compilation warnings*/
#ifdef ALIGN_64BIT
      sprintf(prntBuf, "\nBucket number:%4u  of Size:%u  Num of Blocks: %u\n",
                        idx, regCb->bktTbl[idx].size, regCb->bktTbl[idx].numBlks);
#else
      sprintf(prntBuf, "\nBucket number:%4lu  of Size:%lu  Num of Blocks: %lu\n",
                        idx, regCb->bktTbl[idx].size, regCb->bktTbl[idx].numBlks);
#endif
      SDisplay(0, prntBuf);

      sprintf(prntBuf, "==========================================================================\n");
      SDisplay(0, prntBuf);
      sprintf(prntBuf, " Block    Location    Free/Allocated  Static/dynamic  Size requested\n");
      SDisplay(0, prntBuf);
      sprintf(prntBuf, "==========================================================================\n");
      SDisplay(0, prntBuf);

      offsetToNxtBlk = regCb->bktTbl[idx].size + sizeof(CmMmBlkHdr);

      for (blkCnt=0, curBktBlk = (CmMmBlkHdr *)(regCb->bktTbl[idx].bktStartPtr);
            ((curBktBlk) && (blkCnt < regCb->bktTbl[idx].numBlks));
            curBktBlk = (CmMmBlkHdr *)((Data *)curBktBlk + offsetToNxtBlk), blkCnt++)
      {
/*mt009.301 Fixed 64BIT compilation warnings*/
#ifdef ALIGN_64BIT
         sprintf(prntBuf, "%6u   %8p", blkCnt, (void *)curBktBlk);
#else
         sprintf(prntBuf, "%6lu   %8p", blkCnt, (void *)curBktBlk);
#endif
         SDisplay(0, prntBuf);
         /* check if it is a sane block, elxe jump to next block */
         if (cmMmRegIsBlkSane(curBktBlk) != ROK)
         {
            sprintf(prntBuf, "     Trampled                         \n");
            SDisplay(0, prntBuf);

            continue;
         }

         if (CMM_IS_STATIC(curBktBlk->memFlags))
         {
/*mt009.301 Fixed 64BIT compilation warnings*/
#ifdef ALIGN_64BIT
            sprintf(prntBuf, "     Allocated     Static      %8u\n", curBktBlk->requestedSize);
#else
            sprintf(prntBuf, "     Allocated     Static      %8lu\n", curBktBlk->requestedSize);
#endif
            SDisplay(0, prntBuf);
         }
         else if (CMM_IS_DYNAMIC(curBktBlk->memFlags))
         {
/*mt009.301 Fixed 64BIT compilation warnings*/
#ifdef ALIGN_64BIT
            sprintf(prntBuf, "     Allocated       Dynamic      %8u\n", curBktBlk->requestedSize);
#else
            sprintf(prntBuf, "     Allocated       Dynamic      %8lu\n", curBktBlk->requestedSize);
#endif
            SDisplay(0, prntBuf);
         }
         else if (CMM_IS_FREE(curBktBlk->memFlags))
         {
/*mt009.301 Fixed 64BIT compilation warnings*/
#ifdef ALIGN_64BIT
            sprintf(prntBuf, "     Free                        %8u\n", curBktBlk->requestedSize);
#else
            sprintf(prntBuf, "     Free                        %8lu\n", curBktBlk->requestedSize);
#endif
            SDisplay(0, prntBuf);
         }
         else
         {
            sprintf(prntBuf, "     Trampled                         \n");
            SDisplay(0, prntBuf);
         }
      }
   }

   /* heap profile */
   sprintf(prntBuf, "\nHeap Profile\n");
   SDisplay(0, prntBuf);

   /* point to heapCb */
   heapCb = &(regCb->heapCb);

   sprintf(prntBuf, "\nHeap Start: %8p          Heap End: %8p\n", heapCb->vStart, heapCb->vEnd);
   SDisplay(0, prntBuf);
   sprintf(prntBuf, "==========================================================================\n");
   SDisplay(0, prntBuf);
   sprintf(prntBuf, " Block     Location      Size    Free/Allocated   Static/dynamic  Size requested\n");
   SDisplay(0, prntBuf);
   sprintf(prntBuf, "==========================================================================\n");
   SDisplay(0, prntBuf);

   /* traverse the entire heap to output the heap profile */
   hdrSize = sizeof(CmHEntry);
   for (blkCnt=0, curHBlk = (CmHEntry *)heapCb->vStart;
            ((curHBlk) && (curHBlk < (CmHEntry *)heapCb->vEnd)); blkCnt++)
   {
/*mt009.301 Fixed 64BIT compilation warnings*/
#ifdef ALIGN_64BIT
      sprintf(prntBuf, "%6u   %8p", blkCnt, (void *)curHBlk);
#else
      sprintf(prntBuf, "%6lu   %8p", blkCnt, (void *)curHBlk);
#endif
      SDisplay(0, prntBuf);

      /* check if it is a sane block, elxe jump to next block */
      if (cmMmRegIsBlkSane((CmMmBlkHdr *)curHBlk) != ROK)
      {
         sprintf(prntBuf, "                Trampled                         \n");
         SDisplay(0, prntBuf);

         sprintf(prntBuf, "Trampled block encountered: Stopping heap profile\n");
         SDisplay(0, prntBuf);

         /*
         * To go to next block in the heap we do not have any offset value
         * other than curHBlk->size. As the block is already trampled
         * we cannot rely on this size. So it is better to stop here unless there
         * exists any other mechanism(?) to know the offset to next block.
         */
         RETVALUE(ROK);
      }

/*mt009.301 Fixed 64BIT compilation warnings*/
#ifdef ALIGN_64BIT
      sprintf(prntBuf, "   %8u", curHBlk->size);
#else
      sprintf(prntBuf, "   %8lu", curHBlk->size);
#endif
      SDisplay(0, prntBuf);

      if (CMM_IS_STATIC(curHBlk->memFlags))
      {
/*mt009.301 Fixed 64BIT compilation warnings*/
#ifdef ALIGN_64BIT
         sprintf(prntBuf, "     Allocated       Static       %8u\n", curHBlk->requestedSize);
#else
         sprintf(prntBuf, "     Allocated       Static       %8lu\n", curHBlk->requestedSize);
#endif
         SDisplay(0, prntBuf);
      }
      else if (CMM_IS_DYNAMIC(curHBlk->memFlags))
      {
/*mt009.301 Fixed 64BIT compilation warnings*/
#ifdef ALIGN_64BIT
         sprintf(prntBuf, "     Allocated       Dynamic      %8u\n", curHBlk->requestedSize);
#else
         sprintf(prntBuf, "     Allocated       Dynamic      %8lu\n", curHBlk->requestedSize);
#endif
         SDisplay(0, prntBuf);
      }
      else if (CMM_IS_FREE(curHBlk->memFlags))
      {
/*mt009.301 Fixed 64BIT compilation warnings*/
#ifdef ALIGN_64BIT
         sprintf(prntBuf, "     Free                      %8u\n", curHBlk->requestedSize);
#else
         sprintf(prntBuf, "     Free                      %8lu\n", curHBlk->requestedSize);
#endif
         SDisplay(0, prntBuf);
      }
      else
      {
         sprintf(prntBuf, "     Trampled                         \n");
         SDisplay(0, prntBuf);
      }
      /* goto next block in the heap */
      curHBlk = (CmHEntry *)((Data *)curHBlk + hdrSize + curHBlk->size);

   }

   RETVALUE(ROK);
}
#endif /* SSI_DEBUG_LEVEL1 */

/*-- mt035.201 : Added new API for timestamp --*/
/*--
*
*       Fun:   Get TimeStamp
*
*       Desc:  This function is used to Get TimeStamp in micro seconds
*
*       Ret:   ROK      - ok
*              RFAILED  - error
*
*       Notes:
*
*       File:  mt_ss.c
*
--*/
#ifdef ANSI
PUBLIC S16 SGetTimeStamp
(
S8    *ts
)
#else
PUBLIC S16 SGetTimeStamp(ts)
S8    *ts;
#endif
{

#ifndef SS_LINUX
   struct timespec ptime;
#else
   struct timeval ptime;
#endif

   struct tm* ptm;
   S8 time_string[40];
   S32 microseconds;

   TRC1(SGetTimeStamp);

#ifndef SS_LINUX
  clock_gettime(CLOCK_REALTIME, &ptime);
#else
  gettimeofday(&ptime, NULL);
#endif

   /* Obtain the time of day, and convert it to a tm struct. --*/
   ptm = localtime (&ptime.tv_sec);
   /* Klock work fix ccpu00148484 */
   if(ptm != NULLP)
   {
   /* Format the date and time, down to a single second. --*/
   strftime (time_string, sizeof (time_string), "%a %b %d %Y %H:%M:%S", ptm);
   }

   /* Compute microseconds. --*/
#ifndef SS_LINUX
   microseconds = ptime.tv_nsec / 1000;
#else
   microseconds = ptime.tv_usec;
#endif

   /* Print the formatted time, in seconds, followed by a decimal point
      and the microseconds. --*/
/*mt009.301 Fixed 64BIT compilation warnings*/
#ifdef ALIGN_64BIT
   sprintf(ts, "%s.%03d", time_string, microseconds);
#else
   sprintf(ts, "%s.%03ld", time_string, microseconds);
#endif

   RETVALUE(ROK);

}
/*-- mt037.201 : Added new API for SGetSystemTsk --*/
/*
*
*       Fun:   Get SGetSystemTsk
*
*       Desc:  This function is used to Get sytem task id
*
*       Ret:   task id
*
*       Notes:
*
*       File:  mt_ss.c
*
--*/
#ifdef ANSI
PUBLIC U32 SGetSystemTsk
(
Void
)
#else
PUBLIC U32 SGetSystemTsk()
#endif
{
   TRC1(SGetSystemTskS);

   RETVALUE(pthread_self());

} /* end of SGetSystemTsk */

#ifdef SS_MULTICORE_SUPPORT
/*
*
*       Fun:   Add Timer thread into system task table
*
*       Desc:  This function is used to add the system task
*              associated with Timer thread.
*
*       Ret:   None
*
*       Notes:
*
*       File:  mt_ss.c
*
--*/
#ifdef ANSI
PRIVATE SsSTskEntry* ssdAddTmrSTsk(Void)
#else
PRIVATE SsSTskEntry* ssdAddTmrSTsk()
#endif
{
   SsSTskEntry *sTsk;
   S16 ret;

   TRC1(ssdAddTmrSTsk);
   sTsk = NULLP;
   /* lock the system task table */
   ret = SLock(&osCp.sTskTblLock);
   if (ret != ROK)
   {

#if (ERRCLASS & ERRCLS_DEBUG)
      MTLOGERROR(ERRCLS_DEBUG, EMT039, (ErrVal) ret,
                     "Could not lock system task table");
#endif

      RETVALUE(sTsk);
   }

   /* check count of system tasks */
   if (osCp.numSTsks == SS_MAX_STSKS)
   {

      if ( SUnlock(&osCp.sTskTblLock) != ROK)
      {
#if (ERRCLASS & ERRCLS_DEBUG)
           MTLOGERROR(ERRCLS_DEBUG, EMT040, ERRZERO,
                       "Could not give the Semaphore");
           RETVALUE(sTsk);
#endif
      }

#if (ERRCLASS & ERRCLS_ADD_RES)
      MTLOGERROR(ERRCLS_ADD_RES, EMT041, ERRZERO, "Too many system tasks");
#endif

      RETVALUE(sTsk);
   }


   /* initialize the system task entry with the information we have */
   sTsk = &osCp.sTskTbl[osCp.nxtSTskEntry];

   /* store the system task priority */
   sTsk->tskPrior = SS_NORM_TSK_PRI;

   /* initialize the demand queue */
   if (ssInitDmndQ(&sTsk->dQ) != ROK)
   {

      if ( SUnlock(&osCp.sTskTblLock) != ROK)
      {
#if (ERRCLASS & ERRCLS_DEBUG)
           MTLOGERROR(ERRCLS_DEBUG, EMT042, ERRZERO,
                       "Could not give the Semaphore");
           RETVALUE(NULLP);
#endif
      }

#if (ERRCLASS & ERRCLS_DEBUG)
      MTLOGERROR(ERRCLS_DEBUG, EMT043, (ErrVal) ret,
                  "Could not initialize demand queue");
#endif

      RETVALUE(NULLP);
   }

   /* initialize the system task entry lock */
   if (SInitLock(&sTsk->lock, SS_STSKENTRY_LOCK) != ROK)
   {
      ssDestroyDmndQ(&sTsk->dQ);

      if ( SUnlock(&osCp.sTskTblLock) != ROK)
      {
#if (ERRCLASS & ERRCLS_DEBUG)
           MTLOGERROR(ERRCLS_DEBUG, EMT044, ERRZERO,
                       "Could not give the Semaphore");
           RETVALUE(NULLP);
#endif
      }

#if (ERRCLASS & ERRCLS_DEBUG)
      MTLOGERROR(ERRCLS_DEBUG, EMT045, (ErrVal) ret,
                  "Could not initialize system task entry lock");
#endif

      RETVALUE(NULLP);
   }


   /* success, update the table */
   sTsk->tskId       = osCp.nxtSTskEntry;
   sTsk->used        = TRUE;
   sTsk->termPend    = FALSE;
   osCp.nxtSTskEntry = sTsk->nxt;
   osCp.numSTsks++;

   /* unlock the system task table */

      if ( SUnlock(&osCp.sTskTblLock) != ROK)
      {
#if (ERRCLASS & ERRCLS_DEBUG)
           MTLOGERROR(ERRCLS_DEBUG, EMT046, ERRZERO,
                       "Could not give the Semaphore");
           RETVALUE(NULLP);
#endif
      }

   RETVALUE(sTsk);
}
#endif /* SS_MULTICORE_SUPPORT */
/* mt003.301 Readwrite lock and recursive mutex additions */
#ifdef SS_LOCK_SUPPORT
/*
*
*       Fun:   ssdInitLockNew
*
*       Desc:  This function is used to initialise lock/mutex
*
*       Ret:   ROK   OK
*
*       Notes:
*
*       File:  mt_ss.c
*
*/
#ifdef ANSI
PUBLIC S16 ssdInitLockNew
(
SLockInfo *lockId,
U8        lockType
)
#else
PUBLIC S16 ssdInitLockNew(lockId, lockType)
SLockInfo *lockId;
U8        lockType;
#endif
{

#ifdef SS_REC_LOCK_SUPPORT
	pthread_mutexattr_t attr;
#endif /* SS_REC_LOCK_SUPPORT */
   Txt prntBuf[PRNTSZE];
   S16    retVal = ROK;

   TRC1(ssdInitLockNew);

   switch(lockType)
   {
#ifdef SS_RDWR_LOCK_SUPPORT
      case SRDWRLOCK :
      {
         if((retVal = pthread_rwlock_init((&(lockId->l.rdWrLockId)), NULLP)) != ROK)
         {
            sprintf(prntBuf, "\n\n ssdInitLockNew(): Initialization of read write lock failed,Error# retVal %d\n", retVal);
            SDisplay(0, prntBuf);
            RETVALUE(RFAILED);
         }
         break;
      }
#endif /* SS_RDWR_LOCK_SUPPORT */
#ifdef SS_REC_LOCK_SUPPORT
	case SMUTEXRECUR:
		{
		  retVal = pthread_mutexattr_init(&attr);

		  if(retVal != 0)
		  {
			 sprintf(prntBuf,"\n ssdInitLockNew(): mutexattr init failed,Error# %d \n",retVal);
			 SPrint(prntBuf);
			 RETVALUE(RFAILED);
		  }
#ifdef SS_LINUX
		  retVal = pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE_NP);
#else
		  retVal = pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
#endif
		  if(retVal != 0)
		  {
			 sprintf(prntBuf,"\n ssdInitLockNew(): mutexattr settype failed,Error# %d \n",retVal);
			 pthread_mutexattr_destroy(&attr);
			 SPrint(prntBuf);
			 RETVALUE(RFAILED);
		  }
		  retVal = pthread_mutex_init((pthread_mutex_t *)&(lockId->l.recurLock), &attr);
		  if(retVal != 0)
		  {
			 sprintf(prntBuf,"\n ssdInitLockNew(): mutex init failed,Error# %d \n",retVal);
			 pthread_mutexattr_destroy(&attr);
			 SPrint(prntBuf);
			 RETVALUE(RFAILED);
		  }
		  break;
		}
#endif /* SS_REC_LOCK_SUPPORT */
      default :
      {
         sprintf(prntBuf, "\n\n ssdInitLockNew(): Invalid lock type %d\n", lockType);
         SDisplay(0, prntBuf);
         RETVALUE(RFAILED);
      }
   }
   RETVALUE(ROK);
}
/*
*
*       Fun:   ssdLockNew
*
*       Desc:  This function is used to aquire the read write lock
*
*       Ret:   ROK   OK
*
*       Notes:
*
*       File:  mt_ss.c
*
*/
#ifdef ANSI
PUBLIC S16 ssdLockNew
(
SLockInfo *lockId,
U8         lockType
)
#else
PUBLIC S16 ssdLockNew(lockId, lockType)
SLockInfo *lockId;
U8         lockType;
#endif
{

   Txt prntBuf[PRNTSZE];
   S16    retVal = ROK;

   TRC1(ssdLockNew);

   switch(lockType)
   {
#ifdef SS_RDWR_LOCK_SUPPORT
      case SRDLOCK :
      {
         if((retVal = pthread_rwlock_rdlock(&(lockId->l.rdWrLockId))) != ROK)
         {
           sprintf(prntBuf, "\n\n ssdLockNew(): Failed to aquire the read lock,Error# %d\n", retVal);
           SDisplay(0, prntBuf);
           RETVALUE(RFAILED);
         }
         break;
      }
      case SWRLOCK :
      {
         if((retVal = pthread_rwlock_wrlock(&(lockId->l.rdWrLockId))) != ROK)
         {
           sprintf(prntBuf, "\n\n ssdLockNew(): Failed to aquire the write lock,Error# %d\n", retVal);
           SDisplay(0, prntBuf);
           RETVALUE(RFAILED);
         }
         break;
      }
      case STRYRDLOCK :
      {
         if((retVal = pthread_rwlock_tryrdlock(&(lockId->l.rdWrLockId))) != ROK)
         {
           sprintf(prntBuf, "\n\n ssdLockNew(): Failed to aquire the read lock,Error# %d\n", retVal);
           SDisplay(0, prntBuf);
           RETVALUE(RFAILED);
         }
         break;
      }
      case STRYWRLOCK:
      {
         if((retVal = pthread_rwlock_trywrlock(&(lockId->l.rdWrLockId))) != ROK)
         {
           sprintf(prntBuf, "\n\n ssdLockNew(): Failed to aquire the read lock,Error# %d\n", retVal);
           SDisplay(0, prntBuf);
           RETVALUE(RFAILED);
         }
         break;
      }
#endif /* SS_RDWR_LOCK_SUPPORT */
#ifdef SS_REC_LOCK_SUPPORT
		case SMUTEXRECUR:
		{
		   if((retVal = pthread_mutex_lock(&(lockId->l.recurLock)) != ROK))
			{
				sprintf(prntBuf, "\n\n ssdLockNew(): Failed to aquire the recursive mutex,Error# %d\n", retVal);
        		SDisplay(0, prntBuf);
        		RETVALUE(RFAILED);
		   }
		  break;
		}
#endif /* SS_REC_LOCK_SUPPORT */
      default :
      {
         sprintf(prntBuf, "\n\n ssdLockNew(): Invalid lock type %d\n", lockType);
         SDisplay(0, prntBuf);
         RETVALUE(RFAILED);
      }
   }

   RETVALUE(ROK);
}


/*
*
*       Fun:   ssdUnlockNew
*
*       Desc:  This function is used to Unlock the read write lock
*
*       Ret:   ROK   OK
*
*       Notes:
*
*       File:  mt_ss.c
*
*/
#ifdef ANSI
PUBLIC S16 ssdUnlockNew
(
SLockInfo *lockId,
U8        lockType
)
#else
PUBLIC S16 ssdUnlockNew(lockId, lockType)
SLockInfo *lockId;
U8        lockType;
#endif
{

   Txt prntBuf[PRNTSZE];
   S16    retVal = ROK;

   TRC1(ssdUnlockNew);

   switch(lockType)
   {
#ifdef SS_RDWR_LOCK_SUPPORT
      case SRDWRLOCK :
      {
         if((retVal = pthread_rwlock_unlock(&(lockId->l.rdWrLockId))) != ROK)
         {
            sprintf(prntBuf, "\n\n ssdUnLockNew(): Failed to unlock the lock,Error# %d\n", retVal);
            SDisplay(0, prntBuf);
            RETVALUE(RFAILED);
         }
         break;
      }
#endif /* SS_RDWR_LOCK_SUPPORT */
#ifdef SS_REC_LOCK_SUPPORT
	   case SMUTEXRECUR:
		{
		   if((retVal = pthread_mutex_unlock(&(lockId->l.recurLock)) != ROK))
			{
				sprintf(prntBuf, "\n\n ssdUnLockNew(): Failed to aquire the recursive mutex,Error# %d\n", retVal);
        		SDisplay(0, prntBuf);
        		RETVALUE(RFAILED);
		   }
		  break;
		}
#endif /* SS_REC_LOCK_SUPPORT */
      default :
      {
         sprintf(prntBuf, "\n\n ssdUnlockNew(): Invalid lock type %d\n", lockType);
         SDisplay(0, prntBuf);
         RETVALUE(RFAILED);
      }
   }
   RETVALUE(ROK);
}

/*
*
*       Fun:   ssdDestroyLockNew
*
*       Desc:  This function is used to destroy the read write lock
*
*       Ret:   ROK   OK
*
*       Notes:
*
*       File:  mt_ss.c
*
*/
#ifdef ANSI
PUBLIC S16 ssdDestroyLockNew
(
SLockInfo *lockId,
U8        lockType
)
#else
PUBLIC S16 ssdDestroyLockNew(lockId, lockType)
SLockInfo *lockId;
U8        lockType;
#endif
{
   Txt prntBuf[PRNTSZE];
   S16    retVal = ROK;

   TRC1(ssdDestroyLockNew);

   switch(lockType)
   {
#ifdef SS_RDWR_LOCK_SUPPORT
      case SRDWRLOCK :
      {
         if((retVal = pthread_rwlock_destroy(&(lockId->l.rdWrLockId))) != ROK)
         {
            sprintf(prntBuf, "\n\n ssdDestroyLockNew(): Failed to destroy the lock,Error# %d\n", retVal);
            SDisplay(0, prntBuf);
            RETVALUE(RFAILED);
         }
         break;
      }
#endif /* SS_RDWR_LOCK_SUPPORT */
#ifdef SS_REC_LOCK_SUPPORT
	   case SMUTEXRECUR:
		{
		   if((retVal = pthread_mutex_destroy(&(lockId->l.recurLock)) != ROK))
			{
            sprintf(prntBuf, "\n\n ssdDestroyLockNew(): Failed to destroy the mutex,Error# %d\n", retVal);
        		SDisplay(0, prntBuf);
        		RETVALUE(RFAILED);
		   }
		  break;
		}
#endif /* SS_REC_LOCK_SUPPORT */
      default :
      {
         sprintf(prntBuf, "\n\n ssdDestroyLockNew(): Invalid lock type %d\n", lockType);
         SDisplay(0, prntBuf);
         RETVALUE(RFAILED);
      }
   }
   RETVALUE(ROK);
}
#endif /* SS_LOCK_SUPPORT */

/* mt005.301 : Cavium Changes */
#ifdef SS_SEUM_CAVIUM

/*
 *
 *        Fun:   ssInitRcvWork
 *
 *       Desc:  This is the initializtion function of receive
 *              work thread.
 *
 *       Ret:   ROK      - ok
 *              RFAILED  - failed, general (optional)
 *
 *       Notes: Function to initialize the work queue packet
 *              receiving thread. This creates the new thread to
 *              receive the work and sets the affinity.
 *
 *       File:
 *
 **/
#ifdef ANSI
PUBLIC S16 ssInitRcvWork
(
 void
 )
#else
PUBLIC S16 ssInitRcvWork()
#endif
{
  pthread_attr_t attr;
  pthread_t      thread;

  TRC1(ssInitRcvWork);

  /* set the required attributes */
  pthread_attr_init(&attr);
  pthread_attr_setstacksize(&attr, (size_t)MT_ISTASK_STACK);
  pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

  /* Create a new thread to receive the work queue messages */
  if ((pthread_create(&thread, &attr, workRcvTsk, NULLP)) != 0)
  {
	 pthread_attr_destroy(&attr);

	 RETVALUE(RFAILED);
  }

  pthread_attr_destroy(&attr);

  RETVALUE(ROK);

}/* ssInitRcvWork */


/*
 *
 *       Fun:   workRcvTsk
 *
 *       Desc:  This is the handler function of receive
 *              work thread.
 *
 *       Ret:   ROK      - ok
 *              RFAILED  - failed, general (optional)
 *
 *       Notes:The handler function of the work queue receiver task.
 *             This will be waiting for the work and after receiving
 *             it, work will converted and posted to that entityt
 *
 *       File:
 *
 **/

#ifdef ANSI
PRIVATE void *workRcvTsk
(
 Ptr ptr
 )
#else
PRIVATE void *workRcvTsk (ptr)
  Ptr ptr;
#endif
{

  cvmx_wqe_t *workPtr;
  Buffer     *mBuf, *rcvdBuf;
  SsMsgInfo  *minfoPtr;
  S16         ret;
  struct timespec ts;
  Pst         pst;

  TRC1(workRcvTsk);


  for (;;)
  {
	 /* get the work if its avilable */
	 workPtr = cvmx_pow_work_request_sync(CVMX_POW_NO_WAIT);

	 if ( workPtr == NULLP )
	 {
		/* If there is no work then sleep for 10 usec */
		ts.tv_sec = 0;
		ts.tv_nsec = 500000;

		nanosleep(&ts, NULLP);
		continue;
	 }

	 switch(workPtr->tag)
	 {
		/* Switch over according to the tag value */
		case SS_CVMX_MBUF_TAG:

		  rcvdBuf = (Buffer*)workPtr->packet_ptr.ptr;

		  /* Convert the physical address to Pointers */
		  ret = SConvPhyPtr(&rcvdBuf);
		  if (ret != ROK)
		  {
           /* mt011.301: Cavium 32 bit changes */
			 cvmx_fpa_free(workPtr, SS_CVMX_WQE_POOL, 0);
			 continue;
		  }

		  /* Copy the buffer to this region */
		  ret = SCpyFpaMsg(rcvdBuf, SS_DFLT_REGION, SS_DFLT_POOL, &mBuf);
		  if( ret != ROK )
		  {
           /* mt011.301: Cavium 32 bit changes */
			 cvmx_fpa_free(workPtr, SS_CVMX_WQE_POOL, 0);
			 continue;
		  }

        /* mt011.301: Cavium 32 bit changes */
		  cvmx_fpa_free(workPtr, SS_CVMX_WQE_POOL, 0);

		  minfoPtr = (SsMsgInfo*)mBuf->b_rptr;

		  /* Get the post strucutre and Post the message */
		  if ( minfoPtr != NULLP)
		  {
			 SMemCpy( &pst, &minfoPtr->pst, sizeof(Pst));

			 (Void)SPstTsk(&pst, mBuf);
		  }
		  /* Free the buffer allocated if it cannot be sent */
		  else
		  {
			 SPutMsg(mBuf);
		  }
		  break;

		default:
		  {
			 /* Invalid tag value, drop the work */
           /* mt011.301: Cavium 32 bit changes */
			 cvmx_fpa_free(workPtr, SS_CVMX_WQE_POOL, 0);
			 continue;
		  }
		  break;
	 }

  }
} /* workRcvTsk */

#endif /* SS_SEUM_CAVIUM */

#ifdef TENB_RTLIN_CHANGES
PUBLIC S16 SInitLock(SLockId *l, U8 t)
{
   S16 r = 0;
   pthread_mutexattr_t prior;
   pthread_mutexattr_init(&prior);
#ifndef RGL_SPECIFIC_CHANGES
   pthread_mutexattr_setprotocol(&prior, PTHREAD_PRIO_INHERIT);
#endif
   r = pthread_mutex_init(l, &prior);
   pthread_mutexattr_destroy(&prior);
   return r;
}
#endif
#ifdef SS_THR_REG_MAP
/*
 *
 *       Fun:   ssRegMainThread
 *
 *       Desc:  This function is used to add the memory region
 *              mapping for the main thread.
 *
 *       Ret:   VOID (Always successful)
 *
 *       Notes:
 *
 *       File: mt_ss.c
 *
 */

PUBLIC Void ssRegMainThread(Void)
{

   if(SS_INVALID_THREAD_REG_MAP != SS_GET_THREAD_MEM_REGION())
   {
      printf("not able to get different Id for main thread\n");
      exit(1);
   }
   /* Here the default region is added as we dont have any region associated with
    * Main thread. The thread should not perform any allocation except 
    * the initial configuratin 
    */
#ifdef XEON_SPECIFIC_CHANGES
   SS_GET_THREAD_MEM_REGION() = mtMemoCfg.numRegions;
#else   
   SS_GET_THREAD_MEM_REGION() = 
#endif      
                                                                  DFLT_REGION;
}

/*
 *
 *       Fun:   ssCheckAndAddMemoryRegionMap
 *
 *       Desc:  This function is used to add the memory region
 *              mapping for the provided sTsk associated thread.
 *              If the threadId can be placed in the thread memory 
 *              region mapping table and returns success if it is able
 *              to place. If not, it keeps the thread ID in the static
 *              local array and increments the count. Once thread Id 
 *              is successfully placed in the thread memory region mapping 
 *              table, pthread_cancel is sent for all the previous threads
 *              which are failed to place in table.
 *
 *       Ret:   TRUE - Thread ID successfully placed in thread memory region
 *                     mapping table
 *              FALSE - If thread Id is not placed in thread memory region
 *                     mapping table
 *
 *       Notes:mapping tablemapping tablng tablee
 *
 *       File: mt_ss.c
 *
 */
PUBLIC S32 ssCheckAndAddMemoryRegionMap
(
pthread_t    threadId,    /* Thread Id of system task */
Region       region       /* Region associated with thread */
)
{
   PRIVATE U32       createdThreads;
   PRIVATE pthread_t createdThreadIds[SS_MAX_THREAD_CREATE_RETRY];
   U32               indx;

   TRC1(ssCheckAndAddMemoryRegionMap);

   /* Here  0xFF is considered as invalid region and if the mapping table
    * contains 0xFF, that mapping entry is free
    */
   if(SS_INVALID_THREAD_REG_MAP != 
            osCp.threadMemoryRegionMap[((threadId >> SS_MEM_THREAD_ID_SHIFT) % SS_MAX_THREAD_REGION_MAP)])
   {
      /* Klock work fix ccpu00148484 */
      if(!(createdThreads < SS_MAX_THREAD_CREATE_RETRY))
      {
         printf("failed in index = %ld\n", ((threadId >> SS_MEM_THREAD_ID_SHIFT) % SS_MAX_THREAD_REGION_MAP));
         printf("Not able to get the different thread ID, exiting\n");
         exit(1);
      }
      createdThreadIds[createdThreads++] = threadId;
      RETVALUE(FALSE);
   }
   /* If we found free mapping table entry, place the region and send pthread_cancel
    * for all the thread Ids which are created before this 
    */
   osCp.threadMemoryRegionMap[((threadId >> SS_MEM_THREAD_ID_SHIFT) % SS_MAX_THREAD_REGION_MAP)] = region;
#ifdef XEON_SPECIFIC_CHANGES
   printf("ThreadId %ld, Thread Idx %d, Region %d\n", threadId,
          ((threadId >> SS_MEM_THREAD_ID_SHIFT) % 
          SS_MAX_THREAD_REGION_MAP), region);
#endif   
   for(indx = 0; indx < createdThreads; indx++)
   {
#ifdef XEON_SPECIFIC_CHANGES
      printf("Sending pthred Cancel to thread Id %d \n",createdThreadIds[indx]);
#endif      
      pthread_cancel(createdThreadIds[indx]);
   }
   createdThreads = 0;

   RETVALUE(TRUE);

} /* ssCheckAndAddMemoryRegionMap */

/*
 *
 *       Fun:   ssCheckAndDelMemoryRegionMap
 *
 *       Desc:  This function is used to add the memory region
 *              mapping for the provided sTsk associated thread.
 *              If the threadId can be placed in the thread memory 
 *              region mapping table and returns success if it is able
 *              to place. If not, it keeps the thread ID in the static
 *              local array and increments the count. Once thread Id 
 *              is successfully placed in the thread memory region mapping 
 *              table, pthread_cancel is sent for all the previous threads
 *              which are failed to place in table.
 *
 *       Ret:   TRUE - Thread ID successfully placed in thread memory region
 *                     mapping table
 *              FALSE - If thread Id is not placed in thread memory region
 *                     mapping table
 *
 *       Notes:mapping tablemapping tablng tablee
 *
 *       File: mt_ss.c
 *
 */
PUBLIC S32 ssCheckAndDelMemoryRegionMap
(
pthread_t    threadId    /* Thread Id of system task */
)
{

   TRC1(ssCheckAndDelMemoryRegionMap);

   /* Raghu To-Do Check with team, is it necessary to acquire lock
    * as del and add may go parallel */
   /* Here  0xFF is considered as invalid region and if the mapping table
    * contains 0xFF, that mapping entry is free
    */
   if(SS_INVALID_THREAD_REG_MAP ==
            osCp.threadMemoryRegionMap[((threadId >> SS_MEM_THREAD_ID_SHIFT) % SS_MAX_THREAD_REGION_MAP)])
   {
#ifndef ALIGN_64BIT
      printf("Invalid Thread ID (%ld)\n", (U32)threadId);
#else
      printf("Invalid Thread ID (%d)\n", (U32)threadId);
#endif
      RETVALUE(RFAILED);
   }
   /* If we found free mapping table entry, place the region and send pthread_cancel
    * for all the thread Ids which are created before this 
    */
   osCp.threadMemoryRegionMap[((threadId >> SS_MEM_THREAD_ID_SHIFT) % SS_MAX_THREAD_REGION_MAP)] = SS_INVALID_THREAD_REG_MAP;

   RETVALUE(ROK);

} /* ssCheckAndAddMemoryRegionMap */


#endif
#ifdef SS_TSKLOG_ENABLE
/*
*
*       Fun:   SStartTask
*
*       Desc:  This function will return current time through input parameter.
*
*       Ret:   ROK      - ok
*              RFAILED  - failed, general (optional)
*
*
*       File:  pt_ss.c
*
*/
#ifdef ANSI
PUBLIC S16 SStartTask
(
VOLATILE U32      *startTime,
U32       taskId
)
#else
PUBLIC S16 SStartTask(startTime, taskId)
VOLATILE U32      *startTime;
U32       taskId;
#endif
{
#ifdef MSPD_MLOG_NEW
   *startTime = GetTIMETICK();
#endif
   RETVALUE(ROK);
}

/*
*
*       Fun:   SStopTask
*
*       Desc:  This function will return current time through input parameter.
*              and take the difference of start time provided as input parameter
*              and current time.
*
*       Ret:   ROK      - ok
*              RFAILED  - failed, general (optional)
*
*
*       File:  pt_ss.c
*
*/
#ifdef ANSI
PUBLIC S16 SStopTask
(
VOLATILE U32       startTime,
U32       taskId
)
#else
PUBLIC S16 SStopTask(startTime, taskId)
VOLATILE U32       startTime;
U32       taskId;
#endif
{
   /*U32      stopTime;*/
   switch(taskId)
   {
      case PID_MAC_HARQ_IND:
      case PID_SCH_TTI_IND:               
      case PID_SSI_TSK:               
      case PID_MAC_DAT_IND:
      case PID_MAC_SF_ALLOC_REQ:
      case PID_MAC_STA_RSP:
      case PID_MAC_DL_SCHD:
      case PID_MAC_DL_CQI_IND:
      case PID_MAC_UL_CQI_IND:
      case PID_MAC_UL_SCHD:
      case PID_MAC_TTI_IND:
      case PID_CL_RCV_PHY_MSG:
      case PID_CL_HARQ_STA_IND:
      case PID_MAC_AM_HARQ_RLS:
      case PID_CL_DL_BATCH_PROC:
      case PID_CL_DLM_PRC_TTI_IND:
      case PID_CRC_IND_REAL:
      case PID_CRC_IND_DUMMY:
      case PID_TTI_LATENCY:
      case PID_RECPREQ_PROC:
#ifdef CA_PHY
#ifndef LTE_TDD               
         MLogTask(0, taskId, RESOURCE_LARM, startTime, GetTIMETICK());
#else
         MLogTask(taskId, RESOURCE_LARM, startTime, GetTIMETICK());
#endif
#else
         MLogTask(taskId, RESOURCE_LARM, startTime, GetTIMETICK());
#endif
         break;
   }
   RETVALUE(ROK);
}
#else
#ifdef ANSI
PUBLIC S16 SStartTask
(
VOLATILE U32      * startTime,
U32       taskId
)
#else
PUBLIC S16 SStartTask(startTime, taskId)
VOLATILE U32      * startTime;
U32       taskId;
#endif
{
   *startTime = 0;
   RETVALUE(ROK);
}

#ifdef ANSI
PUBLIC S16 SStopTask
(
VOLATILE U32       startTime,
U32       taskId
)
#else
PUBLIC S16 SStopTask(startTime, taskId)
VOLATILE U32       startTime;
U32       taskId;
#endif
{
   RETVALUE(ROK);
}

#endif /*#ifdef SS_TSKLOG_ENABLE */
#ifdef TENB_T2K3K_SPECIFIC_CHANGES
/** @details
* This primitive is used to calculate the CPU Utilization per Core
* for Intel T2200
*
* @param [in]   Void
*
* @return  Void - function is always success
*/
#ifdef ANSI
PUBLIC Void UpdateSocCpuInfo
(
CmCpuStatsInfo *cpuInfo, 
U8    idx
)
#else
PUBLIC Void UpdateSocCpuInfo(*cpuInfo, idx)
CmCpuStatsInfo *cpuInfo;
U8       idx;
#endif
{
   FILE       *mipsFd;
   S8          mipsStr[MIPS_STRING_LEN];
   S8         *strPart;
   U32         l2FreeCpu;
   U32         l2CpuUsed;
   U32         l3FreeCpu;
   U32         l3CpuUsed;

   /* Open the file which holds the MIPS available value */
   mipsFd = fopen(MIPS_FILE, "r");

   if(mipsFd == NULLP)
   {
      RETVOID;
   }

   /* Get the free mips available value from the file */
   if(NULLP  == fgets(mipsStr, 24, mipsFd))
   {
      printf("fgets to get the free mips available failed\n");
      fclose(mipsFd);
      RETVOID;
   }

   strtok(mipsStr, " ");

   strPart = strtok(NULLP, " ");

   if(idx == CM_L2_CPU_UTIL)
   {  
      if(strPart != NULLP)
      {
         l2FreeCpu = atoi(strPart);   
         l2CpuUsed = 100 - l2FreeCpu;
         cpuInfo->cpuUtil[0].totCpuUtil += l2CpuUsed;
         cpuInfo->cpuUtil[0].maxCpuUtil = GET_CPU_MAX((cpuInfo->cpuUtil[0].maxCpuUtil), l2CpuUsed);;
         cpuInfo->cpuUtil[0].numSamples++;
      }
   }
   if(idx == CM_L3_CPU_UTIL)
   {  
      strPart = strtok(NULLP, " ");
      if(strPart != NULLP)
      {
         l3FreeCpu = atoi(strPart);   
         l3CpuUsed = 100 - l3FreeCpu;
         cpuInfo->cpuUtil[0].totCpuUtil += l3CpuUsed;
         cpuInfo->cpuUtil[0].maxCpuUtil = GET_CPU_MAX((cpuInfo->cpuUtil[0].maxCpuUtil), l3CpuUsed);;
         cpuInfo->cpuUtil[0].numSamples++;
      }
   }
   if(idx == CM_L2_CPU_UTIL)
   {   
      cpuInfo->numCores = CM_NUM_L2_CORES ; 
   }
   else if(idx == CM_L3_CPU_UTIL)
   {   
      cpuInfo->numCores = CM_NUM_L3_CORES ; 
   }
   fclose(mipsFd);

   RETVOID;
}
#endif /* TENB_T2K3K_SPECIFIC_CHANGES */
#ifdef SS_MULTICORE_SUPPORT
/*
*
*       Fun:   Add Timer thread into system task table
*
*       Desc:  This function is used to add the system task
*              associated with Timer thread.
*
*       Ret:   None
*
*       Notes:
*
*       File:  mt_ss.c
*
--*/
#ifdef ANSI
PRIVATE SsSTskEntry* ssdReAddTmrSTsk(
U8 idx
)
#else
PRIVATE SsSTskEntry* ssdReAddTmrSTsk(idx)
U8 idx;
#endif
{
   SsSTskEntry *sTsk;
   S16 ret;

   TRC1(ssdReAddTmrSTsk);
   sTsk = NULLP;
   /* lock the system task table */
   ret = SLock(&osCp.sTskTblLock);
   if (ret != ROK)
   {

#if (ERRCLASS & ERRCLS_DEBUG)
      MTLOGERROR(ERRCLS_DEBUG, EMT039, (ErrVal) ret,
                     "Could not lock system task table");
#endif

      RETVALUE(sTsk);
   }

   /* initialize the system task entry with the information we have */
   sTsk = &osCp.sTskTbl[idx];

   sTsk->used = FALSE;
   sTsk->tskPrior = 0;
   sTsk->numTTsks = 0;
   SDestroyLock(&sTsk->lock);
   ssDestroyDmndQ(&sTsk->dQ);


   /* store the system task priority */
   sTsk->tskPrior = SS_NORM_TSK_PRI;

   /* initialize the demand queue */
   if (ssInitDmndQ(&sTsk->dQ) != ROK)
   {

      if ( SUnlock(&osCp.sTskTblLock) != ROK)
      {
#if (ERRCLASS & ERRCLS_DEBUG)
           MTLOGERROR(ERRCLS_DEBUG, EMT042, ERRZERO,
                       "Could not give the Semaphore");
           RETVALUE(NULLP);
#endif
      }

#if (ERRCLASS & ERRCLS_DEBUG)
      MTLOGERROR(ERRCLS_DEBUG, EMT043, (ErrVal) ret,
                  "Could not initialize demand queue");
#endif

      RETVALUE(NULLP);
   }

   /* initialize the system task entry lock */
   if (SInitLock(&sTsk->lock, SS_STSKENTRY_LOCK) != ROK)
   {
      ssDestroyDmndQ(&sTsk->dQ);

      if ( SUnlock(&osCp.sTskTblLock) != ROK)
      {
#if (ERRCLASS & ERRCLS_DEBUG)
           MTLOGERROR(ERRCLS_DEBUG, EMT044, ERRZERO,
                       "Could not give the Semaphore");
           RETVALUE(NULLP);
#endif
      }

#if (ERRCLASS & ERRCLS_DEBUG)
      MTLOGERROR(ERRCLS_DEBUG, EMT045, (ErrVal) ret,
                  "Could not initialize system task entry lock");
#endif

      RETVALUE(NULLP);
   }


   /* success, update the table */
   sTsk->tskId       = idx + 1;
   sTsk->used        = TRUE;
   sTsk->termPend    = FALSE;

   /* unlock the system task table */

      if ( SUnlock(&osCp.sTskTblLock) != ROK)
      {
#if (ERRCLASS & ERRCLS_DEBUG)
           MTLOGERROR(ERRCLS_DEBUG, EMT046, ERRZERO,
                       "Could not give the Semaphore");
           RETVALUE(NULLP);
#endif
      }

   RETVALUE(sTsk);
}
#endif /* SS_MULTICORE_SUPPORT */


/*
*
*       Fun:   Initialize timer table
*
*       Desc:  This function initializes MTSS-specific information
*              in the timer table.
*
*       Ret:   ROK      - ok
*
*       Notes:
*
*       File:  mt_ss.c
*
*/
#ifdef ANSI
PUBLIC S16 ssdReInitTmr
(
void
)
#else
PUBLIC S16 ssdReInitTmr()
#endif
{
   pthread_attr_t attr;
   struct sched_param param_sched;
#ifndef XEON_SPECIFIC_CHANGES
   U8 ret = ROK;
#endif
#ifdef SS_MULTICORE_SUPPORT
   SsSTskEntry     *sTsk;
#endif /* SS_MULTICORE_SUPPORT */
#ifdef SS_THR_REG_MAP
   U32 threadCreated = FALSE;
#endif /* SS_THR_REG_MAP */

   TRC0(ssdReInitTmr);

#ifndef XEON_SPECIFIC_CHANGES
   ret = ssCheckAndDelMemoryRegionMap(osCp.dep.tmrHdlrTID);
   if(ret != ROK)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
       MTLOGERROR(ERRCLS_DEBUG, EMT046, ERRZERO,
                       "Could not give the Semaphore");
#endif
       RETVALUE(RFAILED);
   }
#endif   

   osCp.dep.tmrTqCp.tmrLen = SS_MAX_TMRS;
  /* mt010.21: addition */

#ifdef SS_MULTICORE_SUPPORT
   sTsk = ssdReAddTmrSTsk(0);
   if(!sTsk)
   {
      RETVALUE(RFAILED);
   }
#endif /* SS_MULTICORE_SUPPORT */
   /* create the timer handler thread */

   pthread_attr_init(&attr);
   /* mt021.201 - Addition to set stack size */
   pthread_attr_setstacksize(&attr, (size_t)MT_TMRTASK_STACK);
   pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
   pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
   pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
   param_sched.sched_priority = sched_get_priority_max(SCHED_FIFO);
   pthread_attr_setschedparam(&attr, &param_sched);


#ifdef SS_THR_REG_MAP
   /* When the thread is created, we check for the memory mapping table if
    * threadId can be placed in thread memory map table. If it is not able to place
    * threadId is stored in tmporary array. Once thread is created successful,
    * thread_cancel is sent for each thread which are created before. All the 
    * threads are made to wait on sema which is cancel point for thread.
    */
   while(threadCreated == FALSE)
   {
#endif
      if ((pthread_create(&osCp.dep.tmrHdlrTID, &attr, mtTmrHdlr, NULLP)) != 0)
      {
         /* mt020.201 - Addition for destroying thread attribute object attr */
         pthread_attr_destroy(&attr);

         RETVALUE(RFAILED);
      }

#ifdef SS_THR_REG_MAP
      threadCreated = ssCheckAndAddMemoryRegionMap(osCp.dep.tmrHdlrTID, 
                                                   sTsk->region);
   }
#endif /* SS_THR_REG_MAP */
#ifdef SS_MEM_WL_DEBUG
   tmpRegTidMap[sTsk->region] = osCp.dep.tmrHdlrTID;
#endif

   /* mt020.201 - Addition for destroying thread attribute object attr */
   pthread_attr_destroy(&attr);
   sem_post(&osCp.dep.ssStarted);
   RETVALUE(ROK);
}

/**********************************************************************
         End of file
**********************************************************************/
