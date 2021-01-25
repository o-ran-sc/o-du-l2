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
  
     Name:     system services interface
  
     Type:     C include file
  
     Desc:     Structures, variables and typedefs required by the
               System Services service user.
  
     File:     ssi.x
  
*********************************************************************21*/
  
#ifndef __SSIX__
#define __SSIX__

#ifdef __cplusplus
extern "C" {
#endif


   
/* typedefs */

typedef S16 Status;               /* status */

typedef uint32_t Ticks;                /* system clock ticks */

#ifdef LONG_MSG
typedef S32 MsgLen;               /* message length */
#else
typedef S16 MsgLen;               /* message length */
#endif

typedef S16 Order;                /* message or queue order */

#ifdef DOS
typedef uint16_t Size;                 /* size (for number of bytes per region or per s-pool) */
#else
typedef uint32_t Size;                 /* size (for number of bytes per region or per s-pool) */
typedef S32 PtrOff;               /* signed pointer offset */
#endif

typedef uint32_t QLen;                 /* queue length */

typedef QLen BufQLen;             /* buffer queue length */

typedef S16 RegSize;              /* region size (for number of regions per processor) */

typedef S16 DPoolSize;            /* dynamic pool size (for number of buffers per d-pool) */

typedef uint16_t Random;               /* random number */

typedef S16 Seq;                  /* sequence */
/* ssi_x_001.main_64 Additions */
typedef uint32_t CoreId;               /* core id */

/* Error */
typedef uint32_t ErrCls;               /* Error Class */

typedef uint32_t ErrCode;              /* Error Code  */

typedef uint32_t ErrVal;               /* Error Value */

typedef S16 VectNmb;              /* vector number */

typedef S16 Ttype;                /* task type */

typedef S8 Sema;                  /* semaphore */


/* post */


/*
   This section contains specific typedefs, structures and
   prototypes for the following operating systems:

   MOS      - MOSVER
   MOS, v2  - MSVER2
   psos     - PS
   vrtxsa   - VX
   vxworks  - VW
   sslib    - system service library for elvis
   other    - 
 
   The defines PORTVER, MOSVER and MSVER2 are mutually exclusive. The define PORTVER
   must be set in conjunction with the define PS, VX, VW or other.

*/


#ifdef MOSVER                     /* mos version */
/* mos typedefs, structures and prototypes */

/* forward definitions */
 
#ifndef CMFILE_REORG_1
typedef struct ss_buffer Buffer;  /* forward definition - buffer */
#endif
 
typedef struct ss_queue Queue;    /* forward definition - queue */

/* chain */
 
typedef struct chn                /* chain */
{
   Buffer *fwd;                   /* forward */
   Buffer *bck;                   /* backward */
} Chn;
 
typedef Chn Chain;                /* chain */
 
/* queue - typdef'd earlier */

struct ss_queue                   /* queue - typdef'd earlier */
{
   Chn chn;                       /* chain */
   QLen crntSize;                 /* current size */
   QLen maxSize;                  /* maximum size */
};
  
/* buffer - typedef'd earlier */
  
struct ss_buffer                  /* buffer - typdef'd earlier */
{
   Chn chn;                       /* chain */
   Mem mem;                       /* memory */
   S8 bufType;                    /* buffer type */
   union {
  
      struct
      {
         MsgLen strtIdx;          /* start index */
         MsgLen endIdx;           /* end index */
         MsgLen size;             /* size */
         Data data[DSIZE];        /* data */
      } dat;                      /* data buffer */
  
      struct
      {
         Queue bq;                /* buffer queue */
         MsgLen msgLen;           /* message length */
         S16 refCnt;              /* reference count */
         Pst pst;                 /* post (optional) */
         uint32_t wsUInt32[2];            /* workspace unsigned 32 bit values (optional) */
         uint16_t wsUInt16[1];            /* workspace unsigned 16 bit values (optional) */
         uint8_t  wsUInt8[2];             /* workspace unsigned 8 bit values (optional) */
      } msg;                      /* message buffer */
  
      struct
      {
         Buffer *entry[MAXEXTENT];/* entry */
      } ext;                      /* extension buffer */
 
      struct
      {
         Size size;               /* size */
      } stc;                      /* static buffer */
 
      struct
      {
         MsgLen strtIdx;          /* start index */
         MsgLen endIdx;           /* end index */
         MsgLen size;             /* size */
         Data *data;              /* data pointer */
      } dma;                      /* dma buffer */
 
   } t;
};

 S16 msOptInd;
 S8  *msOptArg;
 Txt **msArgv;
 S16 msArgc;

#else 
#ifdef MSVER2                     /* mos version 2 */

/* forward definitions */
#ifndef CMFILE_REORG_1
typedef struct ss_buffer Buffer;  /* forward definition - buffer */
#endif
 
typedef struct sBuf SBuf;         /* forward definition - static buffer */
 
typedef struct ss_queue Queue;    /* forward definition - queue */

/* buffer identifier */

/* dynamic chain (4b ALIGNED) */
 
typedef struct dChn               /* dynamic chain */
{
   Buffer *prev;                  /* previous */
   Buffer *crnt;                  /* current */
   Buffer *next;                  /* next */
} DChn;
 
/* static chain (4b ALIGNED)*/
 
typedef struct sChn               /* static chain */
{
   SBuf *fwd;                     /* forward */
} SChn;
 
/* buffer queue (4b ALIGNED) */
  
typedef struct bufQ               /* buffer queue */
{
   DChn dChn;                     /* dynamic chain */
   BufQLen crntSize;              /* current size */
} BufQ;
/* queue (4b ALIGNED) */
  
struct ss_queue                   /* queue - typdef'd earlier */
{
   DChn dChn;                     /* dynamic chain */
   QLen crntSize;                 /* current size */
};
  
/* 4Byte aligned */
typedef struct ss_dat             /* data buffer */
{
   MsgLen strtIdx;                /* start index (2b) */
   MsgLen endIdx;                 /* end index (2b) */
   MsgLen size;                   /* size (2b) */
   Pool pool;                     /* size (1b); Not used for MOS as of now */
   uint8_t spare;                      /* spare for alignment (1b) */
   Data data[DSIZE];              /* data (4b) */
} Dat;

/* 4Byte aligned */
typedef struct ss_msg             /* message buffer */
{
   Queue bufQ;                    /* buffer queue */
   Buffer *nxtDBuf;               /* next DBuf */
   MsgLen msgLen;                 /* message length */
   Pool pool;                     /* size (1b); Not used for MOS as of now */
   uint8_t spare;                      /* spare for alingment */
} Msg;
  
/* buffer - typedef'd earlier */
  
struct ss_buffer                  /* buffer - typedef'd earlier */
{
   DChn dChn;                     /* dynamic chain */
   S8 bufType;                    /* buffer type */
   uint8_t spare1;                     /* spare for alignment */
   uint16_t spare2;                    /* spare for alignment */
   union {
      Dat dat;                    /* data buffer */
      Msg msg;                    /* message buffer */
   } t;
};

/* static buffer - typedef'd earlier */
  
struct sBuf                       /* static buffer - typedef'd earlier */
{
   SChn sChn;                     /* static chain */
   Size size;                     /* size */
   S8 bufType;                    /* buffer type */
   uint8_t spare1;                     /* spare 1 */
   uint16_t spare2;                    /* spare 2 */
};

 S16 msOptInd;
 S8  *msOptArg;
 Txt **msArgv;
 S16 msArgc;

#else
#ifdef PS                         /* psos version */
/* psos typedefs, structures and prototypes */

/* forward definitions */
 
#ifndef CMFILE_REORG_1
typedef struct ss_buffer Buffer;  /* forward definition - buffer */
#endif
 
typedef struct ss_queue Queue;       /* forward definition - queue */

/* chain */
 
typedef struct chn                /* chain */
{
   Buffer *fwd;                   /* forward */
   Buffer *bck;                   /* backward */
} Chn;

/* queue - typdef'd earlier */

struct ss_queue                   /* queue - typdef'd earlier */
{
   Chn chn;                       /* chain */
   QLen crntSize;                 /* current size */
   QLen maxSize;                  /* maximum size */
};

/* buffer - typedef'd earlier */
  
struct ss_buffer                  /* buffer - typedef'd earlier */
{
   Chn chn;                       /* chain */
   Mem mem;                       /* memory */
   S8 bufType;                    /* buffer type */
   union {
  
      struct
      {
         MsgLen strtIdx;          /* start index */
         MsgLen endIdx;           /* end index */
         MsgLen size;             /* size */
         Data data[DSIZE];        /* data */
      } dat;                      /* data buffer */
  
      struct
      {
         Queue bq;                /* buffer queue */
         MsgLen msgLen;           /* message length */
         S16 refCnt;              /* reference count */
      } msg;                      /* message buffer */
  
      struct
      {
         Buffer *entry[MAXEXTENT];/* entry */
      } ext;                      /* extension buffer */
 
      struct
      {
         Size size;               /* size */
      } stc;                      /* static buffer */

      struct
      {
         MsgLen strtIdx;          /* start index */
         MsgLen endIdx;           /* end index */
         MsgLen size;             /* size */
         Data *data;              /* data pointer */
      } dma;                      /* dma buffer */
 
   } t;
};

#else
#ifdef VX                         /* vrtxsa version */
/* vrtxsa typedefs, structures and prototypes */

/* forward definitions */
 
#ifndef CMFILE_REORG_1
typedef struct ss_buffer Buffer;  /* forward definition - buffer */
#endif
 
typedef struct ss_queue Queue;       /* forward definition - queue */

/* buffer - typedef'd earlier */

struct ss_buffer                  /* buffer - typedef'd earlier */
{
   Buffer *fwd;                   /* forward chain pointer for queues */
   Buffer *bck;                   /* backward chain pointer for queues */
   uint16_t mPid;                      /* VRTXsa memory partition ID */
   S16 start;                     /* starting index of message in byte array */
   S16 end;                       /* ending index of message in byte array */
   uint16_t status;                    /* buffer status */
};

/* chain */
 
typedef struct chn                /* chain */
{
   Buffer *fwd;                   /* forward */
   Buffer *bck;                   /* backward */
} Chn;

/* queue - typdef'd earlier */
 
struct ss_queue                   /* queue - typdef'd earlier */
{
   Chn chn;                       /* chain */
   QLen crntSize;                 /* current size */
   QLen maxSize;                  /* maximum size */
};

#else
#ifdef VW                         /* vxworks version */
/* vxworks typedefs, structures and prototypes */

/* forward definitions */
 
#ifndef CMFILE_REORG_1
typedef struct ss_buffer Buffer;  /* forward definition - buffer */
#endif
 
typedef struct ss_queue Queue;    /* forward definition - queue */

typedef struct dChn               /* dynamic chain */
{
   Buffer *prev;                    /* previous */
   Buffer *crnt;                    /* current */
   Buffer *next;                    /* next */
} DChn;
 
/* static chain (4b ALIGNED)*/
 
/* buffer queue (4b ALIGNED) */
  
typedef struct bufQ               /* buffer queue */
{
   DChn dChn;                     /* dynamic chain */
   BufQLen crntSize;              /* current size */
} BufQ;
  
/* queue (4b ALIGNED) */
  
struct ss_queue                   /* queue - typdef'd earlier */
{
   DChn dChn;                     /* dynamic chain */
   QLen crntSize;                 /* current size */
};
  
typedef struct ss_dat             /* data buffer */
{
   MsgLen strtIdx;                /* start index */
   MsgLen endIdx;                 /* end index */
   MsgLen size;                   /* size */
   Pool pool;                     /* pool id */
   uint8_t spare;                      /* spare */
   Data *data;                    /* data */
} Dat;

typedef struct ss_msg             /* message buffer */
{
   Queue bufQ;                    /* buffer queue */
   Buffer *nxtDBuf;               /* next DBuf */
   MsgLen msgLen;                 /* message length */
   Pool pool;                     /* pool id */
   uint8_t spare;                      /* spare */
} Msg;
  
/* buffer - typedef'd earlier */
  
struct ss_buffer                  /* buffer - typedef'd earlier */
{
   DChn dChn;                     /* dynamic chain */
   S8 bufType;                    /* buffer type */
   uint8_t spare1;                     /* spare for alignment */
   uint16_t spare2;                    /* spare for alignment */
   union {
      Dat dat;                    /* data buffer */
      Msg msg;                    /* message buffer */
   } t;
};

#else
#ifdef SSLIB                      /* system service library */
/* forward definitions */
#ifndef CMFILE_REORG_1
typedef struct ss_buffer Buffer;  /* forward definition - buffer */
#endif
 
typedef struct sBuf SBuf;         /* forward definition - static buffer */
 
typedef struct ss_queue Queue;    /* forward definition - queue */


/* queue - typdef'd earlier */
 
struct ss_queue                   /* queue - typdef'd earlier */
{
   S16 dummy;                     /* dummy placeholder */
};

/* buffer - typedef'd earlier */
  
struct ss_buffer                  /* buffer - typedef'd earlier */
{
   S32 sIdx;                      /* start index */
   S32 len;                       /* buffer length */
   S8 dat[MAXBUFSIZ];             /* data */
};

#else                             /* portable/other version */
#ifdef SSRYLIB                      /* system service library */
/* forward definitions */
#ifndef CMFILE_REORG_1
typedef struct ss_buffer Buffer;  /* forward definition - buffer */
#endif
 
typedef struct sBuf SBuf;         /* forward definition - static buffer */
 
typedef struct ss_queue Queue;    /* forward definition - queue */

 
typedef S16 RyBufId;
/* dynamic chain (aligned) */
typedef struct ryDChn                 /* dynamic chain */
{
   RyBufId back;                      /* backward (2b) */
   RyBufId crnt;                      /* current  (2b) */
   RyBufId next;                      /* next     (2b) */
   RyBufId pad;                       /* pad      (2b) */
} RyDChn;
/* 
 * queue - typdef'd earlier 
 * This is exactly same as RyBufQ
 */
struct ss_queue                   /* queue - typdef'd earlier */
{
   RyDChn dChn;
   QLen crntSize;                 /* current size */
   uint16_t  ryChanId;                 /* relay channel Id */
};

/* buffer - typedef'd earlier */
  
struct ss_buffer                  /* buffer - typedef'd earlier */
{
   S32 sIdx;                      /* start index */
   S32 len;                       /* buffer length */
   S8 dat[MAXBUFSIZ];             /* data */
};

#else                             /* portable/other version */
#ifdef WINNT_IATM                 /* Windows NT Integrated ATM */

#ifndef CMFILE_REORG_1
#ifndef CFG_APP                   /* Don't include ndis.h for config app. */ 
#include "ndis.h"                 /* to support NDIS calls (listed above) */
#endif /* CFG_APP */

typedef struct _NDIS_PACKET Buffer; /* forward definition - buffer */

#endif /* CMFILE_REORG_1 */

typedef struct ss_queue Queue;      /* forward definition - queue */

/* Trillium Queue is built upon the NDIS library list functions.
 * No spin locks are applied for queues.
 */

struct ss_queue {
   LIST_ENTRY ListHead;           /* LIST_ENTRY to support NDIS list calls */
   QLen CurQLen;                  /* current queue length */
   QLen MaxQLen;                  /* maximum queue length */
};
#else  /* WINNT_IATM */
#ifdef MT                         /* multi-threaded version */
/* multi-threaded typedefs, structures and prototypes */

/* forward definitions */
 
#ifndef CMFILE_REORG_1
typedef struct ss_buffer Buffer;  /* forward definition - buffer */
#endif
 
typedef struct ss_queue Queue;    /* forward definition - queue */

typedef struct sBuf      SBuf;    /* forward definition - static buffer */

typedef struct dChn               /* dynamic chain */
{
   Buffer *prev;                    /* previous */
   Buffer *crnt;                    /* current */
   Buffer *next;                    /* next */
} DChn;

/* static chain (4b ALIGNED)*/
typedef struct sChn
{
   SBuf *fwd;                     /* forward */
} SChn;
 
/* static chain (4b ALIGNED)*/
 
/* buffer queue (4b ALIGNED) */
  
/* queue (4b ALIGNED) */
  
struct ss_queue                   /* queue - typdef'd earlier */
{
   DChn dChn;                     /* dynamic chain */
   QLen crntSize;                 /* current size */
};
  
typedef struct ss_dat             /* data buffer */
{
   MsgLen strtIdx;                /* start index */
   MsgLen endIdx;                 /* end index */
   MsgLen size;                   /* size */
   Pool pool;                     /* pool id */
   uint8_t spare;                      /* spare */
   Data *data;                    /* data */
} Dat;

typedef struct ss_msg             /* message buffer */
{
   Queue bufQ;                    /* buffer queue */
   Buffer *nxtDBuf;               /* next DBuf */
   MsgLen msgLen;                 /* message length */
   Pool pool;                     /* pool id */
   uint8_t spare;                      /* spare */
} Msg;
  
/* buffer - typedef'd earlier */
  
struct ss_buffer                  /* buffer - typedef'd earlier */
{
   DChn dChn;                     /* dynamic chain */
   S8 bufType;                    /* buffer type */
   uint8_t spare1;                     /* spare for alignment */
   uint16_t spare2;                    /* spare for alignment */
   union {
      Dat dat;                    /* data buffer */
      Msg msg;                    /* message buffer */
   } t;
};

/* static buffer - typedef'd earlier */
  
struct sBuf
{
   SChn sChn;                     /* static chain */
   Size size;                     /* size */
   S8 bufType;                    /* buffer type */
   uint8_t spare1;                     /* spare 1 */
   uint16_t spare2;                    /* spare 2 */
};

typedef S32 MtRwLockId;
typedef S32 MtSemaId;
typedef S32 MtCondId;
typedef S32 MtMtxId;
typedef S32 MtThrdId;
typedef S32 MtThrdFlags;
typedef S32 MtThrdPrior;

typedef Void *(MtThrd) ARGS((Void *)); 

#else  /* not MT */
#ifdef PORTVER
/* portable typedefs, structures and prototypes */

/* forward definitions */
 
#ifndef CMFILE_REORG_1
typedef struct ss_buffer Buffer;  /* forward definition - buffer */
#endif
 
typedef struct ss_queue Queue;       /* forward definition - queue */

/* queue - typdef'd earlier */
 
struct ss_queue                   /* queue - typdef'd earlier */
{
   S16 dummy;                     /* dummy placeholder */
};
 
/* buffer - typedef'd earlier */
  
struct ss_buffer                  /* buffer - typedef'd earlier */
{
   S16 dummy;                     /* dummy placeholder */
};

/* task related stuff */
/* data range modified */
#ifndef SS_MULTIPLE_PROCS
typedef uint8_t  SSTskId;             /* System Task Id */
#else /* SS_MULTIPLE_PROCS */
typedef uint16_t SSTskId;             /* System Task Id */
#endif /* SS_MULTIPLE_PROCS */

typedef S32 SSTskPrior;          /* System Task Priority */
#ifdef SS_AEHDPR_SUPPORT
typedef Void (*PISR) ARGS((PTR cxt)); /* pointer to ISR function handler */
typedef Void (*PDPR) ARGS((PTR cxt)); /* pointer to DPR function handler */
#endif /* SS_AEHDPR_SUPPORT */
#else  /* not PORTVER */
#ifdef SS                        /* Common System Services */

/* for SGetOpt() */
 S16 msOptInd;
 S8  *msOptArg;
 Txt **msArgv;
 S16 msArgc;

/* task related stuff */
/* data range modified */
#ifndef SS_MULTIPLE_PROCS
typedef uint8_t  SSTskId;             /* System Task Id */
#else /* SS_MULTIPLE_PROCS */
typedef uint16_t SSTskId;             /* System Task Id */
#endif /* SS_MULTIPLE_PROCS */

typedef S32 SSTskPrior;          /* System Task Priority */

#ifndef CMFILE_REORG_1

/* Buffer type is necessary */
typedef struct ssmsgb Buffer;

#endif /* CMFILE_REORG_1 */

/* message block */
struct ssmsgb
{
   S8                   refCnt;     /* Counter to Free Buff */
   struct ssmsgb        *b_next;     /* next message */
   struct ssmsgb        *b_prev;     /* previous message */
   struct ssmsgb        *b_cont;     /* next message block */
   uint8_t                   *b_rptr;     /* 1st unread data byte of buffer */
   uint8_t                   *b_wptr;     /* 1st unwritten data byte of buffer */
   struct ssdatab       *b_datap;    /* pointer to data block */
};

/* Queue data structure */
typedef struct ssQueue
{
   Buffer *head;                     /* head of queue */
   Buffer *tail;                     /* tail of queue */
   QLen   crntSize;                  /* current szie of queue */

} Queue;


/* Memory CTL operations structure */
typedef struct sMemCtl
{
   uint8_t op;                           /* operation */

   union
   {
      struct
      {
         Data *vaddr;               /* virtual address */
         Data **paddr;              /* physical address */
      } vtop;

      struct
      {
         Size size;                 /* size */
         Status *status;            /* status */
      } chkres;
   } u;

} SMemCtl;


/* memory management handler function types */
/* ssi_x_001.main_47 - addition for introducing additional parameter memType(static/dynamic) */
/* ssi_x_001.main_57 : Additions */
#ifdef SS_HISTOGRAM_SUPPORT 
#ifdef SSI_DEBUG_LEVEL1
typedef S16 (*SsAlloc) ARGS((Void *, Size *, uint32_t, Data **, uint32_t, uint32_t, uint8_t*, uint8_t, Bool));
#else
typedef S16 (*SsAlloc) ARGS((Void *, Size *, uint32_t, Data **, uint32_t, uint8_t*, uint8_t, Bool));
#endif /* SSI_DEBUG_LEVEL1 */
typedef S16 (*SsFree) ARGS((Void *, Data *, Size, uint32_t, uint8_t*, uint8_t, Bool));
typedef S16 (*SsCtl) ARGS((Void *, Event, SMemCtl *));
#elif defined(SS_LIGHT_MEM_LEAK_STS)
typedef S16 (*SsAlloc) ARGS((Void *, Size *, uint32_t , Data **, uint32_t, uint32_t , uint8_t *));
typedef S16 (*SsFree) ARGS((Void *, Data *, Size, uint32_t, uint8_t *));
typedef S16 (*SsCtl) ARGS((Void *, Event, SMemCtl *));
#else
#ifdef SSI_DEBUG_LEVEL1
typedef S16 (*SsAlloc) ARGS((Void *, Size *, uint32_t, Data **, uint32_t));
#else
#ifdef T2K_MEM_LEAK_DBG
typedef S16 (*SsAlloc) ARGS((Void *, Size *, uint32_t, Data **,char*, uint32_t));
#else
typedef S16 (*SsAlloc) ARGS((Void *, Size *, uint32_t, Data **));
#endif
#endif /* SSI_DEBUG_LEVEL1 */
#ifdef T2K_MEM_LEAK_DBG
typedef S16 (*SsFree) ARGS((Void *, Data *, Size,char*, uint32_t));
#else
typedef S16 (*SsFree) ARGS((Void *, Data *, Size));
#endif
typedef S16 (*SsCtl) ARGS((Void *, Event, SMemCtl *));
#endif /* SS_HISTOGRAM_SUPPORT */


/* region information structure -- passed to SRegRegion() */
typedef struct sRegInfo
{
   Void         *regCb;            /* region control block pointer */
   uint32_t          flags;             /* region characteristics flags  */
/* ssi_x_001.main_64 Additions */
#ifdef SS_CAVIUM
   cvmx_arena_list_t  regArena;    /* arena for the region */
#endif
   Data         *start;            /* start address of region */
   Size         size;              /* size of region */
   SsAlloc      alloc;             /* allocation function pointer */
   SsFree       free;              /* deallocation function pointer */
   SsCtl        ctl;               /* control request function pointer */

} SRegInfo;


#ifdef SS_OLD_THREAD

/* types for SGetThread(), SGetMutex, etc. */
typedef Void    *(SThrd) ARGS((Void *));

typedef S32     SThrdId;           /* thread Id */
typedef S32     SMtxId;            /* mutex Id */
typedef S32     SCondId;           /* condition Id */

#endif /* SS_OLD_THREAD */

/*ssi_x_001.main_63 ss_dep.x guarded under flag*/
#ifdef SS_CAVIUM
#include "jz_dep.x"
#else
#ifdef SS_4GMX_LCORE
#include "ss_dep.x"
#else
#include "ss_dep.x"
#endif /* SS_4GMX_LCORE */
#endif/*SS_CAVIUM*/

#endif /* SS */
#endif /* PORTVER */
#endif /* MT */
#endif /* WINNT_IATM */
#endif /* SSRYLIB */
#endif /* SSLIB */
#endif /* VW */
#endif /* VX */
#endif /* PS */
#endif /* MSVER2 */
#endif /* MOSVER */
/* ssi_x_001.main_57 : Additions */
#ifdef SS_HISTOGRAM_SUPPORT  
/*
typedef struct entInfo
{
   uint8_t                 entid;
}EntInfo;

typedef struct secondIndex
{
    EntInfo           info[26];
}SecondIndex;

typedef struct firstIndex
{
    SecondIndex       info[26];
}FirstIndex; */
#endif /* SS_HISTOGRAM_SUPPORT */


/* functions */

/* procId and control block added */ 
#ifndef SS_MULTIPLE_PROCS
typedef uint8_t  (*PAIFS16) ARGS((Ent ent,Inst inst,Region region,Reason reason ));
/* ssi_x_001.main_54 */
/* ssi_x_001.main_53 */
/* ssi_x_001.main_69: Removed the SS_MT_TMR guard from PAIFTMRS16
                      timer activation function typedef. */

typedef S16  (*PAIFTMRS16) ARGS((Ent ent, Inst inst));
#else
typedef uint8_t (*PAIFS16) ARGS((ProcId proc, 
                              Ent ent,
                              Inst inst,
                              Region region,
                              Reason reason,
                              Void **xxCb));

/* time activation function typedef */
typedef S16  (*PAIFTMRS16) ARGS((ProcId proc, 
                                 Ent ent,
                                 Inst inst));
#endif /* SS_MULTIPLE_PROCS */

#ifndef VX
typedef S16  (*PATFS16) ARGS((Prior prior,Route route,Ent ent,Inst inst,Buffer *mBuf));
#else
typedef void  (*PATFS16) ARGS((void *vPtr));
#endif
typedef S16  (*ActvTsk) ARGS((Pst *pst, Buffer *mBuf));
#ifdef ENB_RELAY
typedef struct uProc UProc;  /* forward definition - uproc */
#ifdef RYINT1
typedef void (*ISTsk) ARGS((Inst inst));
#else  /* for backward compatibility */
typedef void (*ISTsk) ARGS((UProc *uProc));
#endif /* RYINT1 */

struct uProc
{
   Inst inst;                         /* instance */
   ProcId low;                        /* procId low value */
   ProcId high;                       /* procId high value */
   ActvTsk actvTsk;                   /* activation function for drvr task */ 
   uint16_t isFlag;                        /* interrupt service flag */
   ISTsk isTsk;                       /* interrupt service task */
};
 

#endif /* ENB_RELAY */


/* functions */
S16 SChekMemUtilization ARGS((Region region,Bool *memAlarm));

Void prntMem ARGS((Data *strtAdr,S16 len));
Void prntMsg ARGS((Buffer *mBuf));
Void prntMsg1 ARGS((Buffer *mBuf,S16 src,S16 dst));
uint8_t tst ARGS((void ));
S16 rdConQ ARGS((Data data));

S16 SPkS8 ARGS((S8 val,Buffer *mBuf));
S16 oduUnpackUInt8 ARGS((uint8_t val,Buffer *mBuf));
S16 SPkS16 ARGS((S16 val,Buffer *mBuf));
S16 oduUnpackUInt16 ARGS((uint16_t val,Buffer *mBuf));
S16 SPkS32 ARGS((S32 val,Buffer *mBuf));
S16 oduUnpackUInt32 ARGS((uint32_t val,Buffer *mBuf));
/* ssi_x_001.main_49 : added packing of F32 and F64*/
#ifdef SS_FLOAT
S16 SPkF32 ARGS((F32 val,Buffer *mBuf));
S16 SPkF64 ARGS((F64 val,Buffer *mBuf));
#endif /* SS_FLOAT */
S16 SUnpkS8 ARGS((S8 *val,Buffer *mBuf));
S16 oduPackUInt8 ARGS((uint8_t *val,Buffer *mBuf));
S16 SUnpkS16 ARGS((S16 *val,Buffer *mBuf));
S16 oduPackUInt16 ARGS((uint16_t *val,Buffer *mBuf));
S16 SUnpkS32 ARGS((S32 *val,Buffer *mBuf));
S16 oduPackUInt32 ARGS((uint32_t *val,Buffer *mBuf));
/* ssi_x_001.main_49 : added unpacking of F32 and F64*/
#ifdef SS_FLOAT
S16 SUnpkF32 ARGS((F32 *val,Buffer *mBuf));
S16 SUnpkF64 ARGS((F64 *val,Buffer *mBuf));
#endif /* SS_FLOAT */
S16 SPrint ARGS((Txt *buf));
S16 SDisplay ARGS((S16 chan,Txt *buf));
S16 SPrntMsg ARGS((Buffer *mBuf,S16 src,S16 dst));
S16 SInitQueue ARGS((Queue *q));
#ifndef SS_ENABLE_MACROS
S16 SQueueFirst ARGS((Buffer *buf,Queue *q));
S16 SQueueLast ARGS((Buffer *buf,Queue *q));
S16 SDequeueFirst ARGS((Buffer * *bufPtr,Queue *q));
S16 SDequeueLast ARGS((Buffer * *bufPtr,Queue *q));
#endif /* SS_ENABLE_MACROS */
S16 SFlushQueue ARGS((Queue *q));
S16 SCatQueue ARGS((Queue *q1,Queue *q2,Order order));
S16 SFndLenQueue ARGS((Queue *q,QLen *lngPtr));
S16 SExamQueue ARGS((Buffer **bufPtr,Queue *q,QLen idx));
S16 SAddQueue ARGS((Buffer *buf,Queue *q,QLen idx));
S16 SRemQueue ARGS((Buffer **bufPtr,Queue *q,QLen idx));
#ifndef SS_ENABLE_MACROS
#ifdef T2K_MEM_LEAK_DBG
#define SGetDBuf(region,pool,buf) SGetDBufNew(region,pool,buf,__FILE__,__LINE__)
#define SPutDBuf(region,pool,buf) SPutDBufNew(region,pool,buf,file,line)
S16 SGetDBufNew ARGS((Region region,Pool pool,Buffer * *bufPtr,char*,uint32_t));
S16 SPutDBufNew ARGS((Region region,Pool pool,Buffer *buf,char*,uint32_t));
S16 SAttachPtrToBufNew ARGS((Region   region, Pool     pool, Data
         *ptr, MsgLen   totalLen, Buffer** mBuf, char* file, uint32_t line));
#define SAttachPtrToBuf(region,pool,ptr,totalLen,mBuf) SAttachPtrToBufNew(region,pool,ptr,totalLen,mBuf,__FILE__,__LINE__)
#define SPutZbcDBuf(region,buf) SPutZbcDBufNew(region,buf,__FILE__,__LINE__)
#elif SS_LIGHT_MEM_LEAK_STS
S16 SGetDBufNew ARGS((Region region,Pool pool,Buffer * *bufPtr,uint32_t line,uint8_t *fnName));
S16 SPutDBufNew ARGS((Region region,Pool pool,Buffer *buf, uint32_t line, uint8_t *fnName));
#else

S16 SGetDBuf ARGS((Region region,Pool pool,Buffer * *bufPtr));
S16 SPutDBuf ARGS((Region region,Pool pool,Buffer *buf));
#endif
#else /* SS_ENABLE_MACROS */
#ifdef T2K_MEM_LEAK_DBG
#define SGetDBuf(region,pool,buf) SGetDBufNew(region,pool,buf,__FILE__,__LINE__)
#define SPutDBuf(region,pool,buf) SPutDBufNew(region,pool,buf,__FILE__,__LINE__)
S16 SGetDBufNew ARGS((Region region,Pool pool,Buffer * *bufPtr,char*,uint32_t));
S16 SPutDBufNew ARGS((Region region,Pool pool,Buffer *buf,char*,uint32_t));
#elif SS_LIGHT_MEM_LEAK_STS
S16 SGetDBufNew ARGS((Region region,Pool pool,Buffer * *bufPtr,uint32_t line,uint8_t *fnName));
S16 SPutDBufNew ARGS((Region region,Pool pool,Buffer *buf, uint32_t line, uint8_t *fnName));
#else
S16 SGetDBuf ARGS((Region region,Pool pool,Buffer * *bufPtr));
S16 SPutDBuf ARGS((Region region,Pool pool,Buffer *buf));
#endif
#endif /* SS_ENABLE_MACROS */
S16 SPutMsgToRegion ARGS((Region region, Buffer *buf));
#ifdef SS_USE_ICC_MEMORY
Void *ssGetIccHdl ARGS((Region region));
#endif /* SS_USE_ICC_MEMORY */

/* ssi_x_001.main_57 : Additions */
/* ssi_x_001.main_59 : Added compile time flag */
#ifdef SS_LIGHT_MEM_LEAK_STS
S16 SGetMsgNew ARGS((Region region, Pool pool, Buffer * *mBufPtr, uint32_t line, uint8_t *fnName));
S16 SGetSBufNew ARGS((Region region,Pool pool, Data * *bufPtr, Size size, uint32_t line, uint8_t* fnName));
S16 SPutMsgNew ARGS((Buffer *mBuf, uint32_t line,  uint8_t *fnName));
S16 SPutSBufNew ARGS((Region region, Pool pool, Data *buf, Size size, uint32_t line, uint8_t *fnName));
#else /*SS_LIGHT_MEM_LEAK_STS */
#ifdef SS_HISTOGRAM_SUPPORT 
S16 SPutMsgNew ARGS((Buffer *mBuf, uint32_t line,  uint8_t *fileName));
S16 SGetMsgNew ARGS((Region region, Pool pool, Buffer * *mBufPtr, uint32_t line,  uint8_t *fileName));
S16 SGetSBufNew ARGS((Region region,Pool pool, Data * *bufPtr, Size size, uint32_t line, uint8_t *fileName));
S16 SPutSBufNew ARGS((Region region, Pool pool, Data *buf, Size size, uint32_t line, uint8_t *fileName));
#else
/*ssi_x_001.main_67 : RMIOS specific changes*/
#ifndef SS_RMIOS
#ifdef T2K_MEM_LEAK_DBG
#define SPutMsg(mBuf) SPutMsgNew(mBuf,__FILE__,__LINE__)
#define SGetMsg(region,pool,mBuf) SGetMsgNew(region,pool,mBuf,__FILE__,__LINE__)
S16 SPutMsgNew ARGS((Buffer *mBuf,char*, uint32_t));
S16 SGetMsgNew ARGS((Region region, Pool pool, Buffer * *mBufPtr, char*,uint32_t));
#else
S16 SPutMsg ARGS((Buffer *mBuf));
S16 SGetMsg ARGS((Region region, Pool pool, Buffer * *mBufPtr));
#endif
#else
#define SPutMsg(m) SPutMsgRmi(__FILE__, __LINE__, m)
#define SGetMsg(r, p, m)    SGetMsgRmi(__FILE__, __LINE__, r, p, m)
S16 SPutMsgRmi ARGS((char *file, int line, Buffer *mBuf));
S16 SGetMsgRmi ARGS((char *file, int line, Region region, Pool pool, Buffer * *mBufPtr));
#endif
#if (defined(SSI_STATIC_MEM_LEAK_DETECTION)|| defined(T2K_MEM_LEAK_DBG))
#define SGetSBuf(region,pool,bufPtr,size) SGetSBuf1(region,pool,bufPtr,size,__FILE__,__LINE__)
S16 SGetSBuf1 ARGS((Region region,Pool pool, Data * *bufPtr, Size size, char* file, uint32_t line));
void DumpStaticMemLeakFiles ARGS((void));
void DumpT2kMemLeakInfoToFile ARGS((void)); 
#define SPutSBuf(region,pool,buf,size) SPutSBuf1(region,pool,buf,size,__FILE__,__LINE__)
S16 SPutSBuf1 ARGS((Region region, Pool pool, Data *buf, Size size, char*, uint32_t));
#else
S16 SGetSBuf ARGS((Region region,Pool pool, Data * *bufPtr, Size size));
S16 SPutSBuf ARGS((Region region, Pool pool, Data *buf, Size size));
#endif
#endif /* SS_HISTOGRAM_SUPPORT */
#endif /*SS_LIGHT_MEM_LEAK_STS */
#ifdef INTEL_WLS_MEM
S16 SPutSBufWls(Region region, Pool pool, Data *ptr, Size size);
S16 SGetSBufWls(Region region, Pool pool, Data **ptr, Size size);
#endif
#ifdef INTEL_WLS
#ifdef T2K_MEM_LEAK_DBG
#define SGetSBufWls(region,pool,bufPtr,size) SGetSBufWls1(region,pool,bufPtr,size,__FILE__,__LINE__) 
#define SPutSBufWls(region,pool,bufPtr,size) SPutSBufWls1(region,pool,bufPtr,size,__FILE__,__LINE__) 
S16 SPutSBufWls1(Region region, Pool pool, Data *ptr, Size size,char* file, uint32_t line);
S16 SGetSBufWls1(Region region, Pool pool, Data **ptr, Size size,char* file, uint32_t line);  
#define SAttachWlsPtrToMBuf(region,pool,bufPtr,rPtr,size,pLen,mBuf) SAttachWlsPtrToMBuf1(region,pool,bufPtr,rPtr,size,pLen,mBuf,__FILE__,__LINE__)
S16 SAttachWlsPtrToMBuf1(Region region, Pool pool, Data *ptr, Data *readPtr, MsgLen totalLen, MsgLen ptrLen, Buffer** mBuf,char* file, uint32_t line);
#define SAttachPtrToMBuf(region,pool,bufPtr,size,pLen,mBuf) SAttachPtrToMBuf1(region,pool,bufPtr,size,pLen,mBuf,__FILE__,__LINE__)

#else
S16 SPutSBufWls(Region region, Pool pool, Data *ptr, Size size);
S16 SGetSBufWls(Region region, Pool pool, Data **ptr, Size size);   
S16 SAttachWlsPtrToMBuf(Region region, Pool pool, Data *ptr, Data *readPtr, MsgLen totalLen, MsgLen ptrLen, Buffer** mBuf);
#endif
#endif
S16 SGetStaticBuffer ARGS((Region region,Pool pool, Data * *bufPtr, Size size, uint8_t memType));
S16 SPutStaticBuffer ARGS((Region region, Pool pool, Data *buf, Size size, uint8_t memType));
/* ssi_x_001.main_65: Additions */
#ifdef SS_SEUM_CAVIUM
S16 ssInitRcvWork ARGS((void));
S16 SConvPtrPhy ARGS(( Buffer  **mBuf));
S16 SConvPhyPtr ARGS((Buffer  **workPtr));
S16 SCpyFpaMsg ARGS((Buffer *srcBuf, Region dstRegion,Pool dstPool, Buffer **dstBuf));
S16 SCpyMsgFpa ARGS(( Buffer *srcBuf, Buffer **dstBuf));
S16 SPutFpaMsg ARGS(( Buffer *fpaBuf));
#endif /* SS_SEUM_CAVIUM */

S16 SGetSMem ARGS((Region region,Size size,Pool *poolPtr));
S16 SPutSMem ARGS((Region region,Pool pool));
S16 SInitMsg ARGS((Buffer *mBuf));
S16 SAddPreMsg ARGS((Data data,Buffer *mBuf));
S16 SAddPstMsg ARGS((Data data,Buffer *mBuf));
/* ssi_x_001.main_70 - Added prototype for SAddPreMsgMultInOrder */
S16 SAddPreMsgMultInOrder ARGS((Data *src,MsgLen cnt,Buffer *mBuf));
S16 SRemPreMsg ARGS((Data *dataPtr,Buffer *mBuf));
S16 SRemPreMsgRegion ARGS((Region region, Data *dataPtr,Buffer *mBuf));
S16 SCatMsgRegion ARGS((Region region, Buffer *mBuf1,Buffer *mBuf2,Order order));
S16 SSegMsgRegion ARGS((Region region, Buffer *mBuf1,MsgLen idx,Buffer **mBuf2));
int SCreatePThread ARGS((pthread_t* tid, pthread_attr_t* attr, void *(*start_routine) (void *), void* arg));
S16 SRemPstMsg ARGS((Data *dataPtr,Buffer *mBuf));
#ifdef T2K_MEM_LEAK_DBG
#define SAddPreMsgMult(src, cnt, mBuf) SAddPreMsgMult1(src, cnt, mBuf, __FILE__,__LINE__)
#define SAddPstMsgMult(src, cnt, mBuf) SAddPstMsgMult1(src, cnt, mBuf, __FILE__,__LINE__)

S16 SAddPreMsgMult1 ARGS((Data *src,MsgLen cnt,Buffer *mBuf, char *file, uint32_t line));
S16 SAddPstMsgMult1 ARGS((Data *src,MsgLen cnt,Buffer *mBuf, char *file, uint32_t line));
#else
S16 SAddPreMsgMult ARGS((Data *src,MsgLen cnt,Buffer *mBuf));
S16 SAddPstMsgMult ARGS((Data *src,MsgLen cnt,Buffer *mBuf));
#endif
S16 SGetPstMsgMult ARGS((MsgLen cnt,Buffer *mBuf));
S16 SRemPreMsgMult ARGS((Data *dst,MsgLen cnt,Buffer *mBuf));
S16 SRemPstMsgMult ARGS((Data *dst,MsgLen cnt,Buffer *mBuf));
S16 SRepMsg ARGS((Data data,Buffer *mBuf,MsgLen idx));
S16 SExamMsg ARGS((Data *dataPtr,Buffer *mBuf,MsgLen idx));
/*ssi_x_001.main_60 */
S16 SGetDataFrmMsg ARGS ((Buffer *mBuf, Data *dataPtr, MsgLen idx, MsgLen dataLen));
S16 SFndLenMsg ARGS((Buffer *mBuf,MsgLen *lngPtr));
S16 SCatMsg ARGS((Buffer *mBuf1,Buffer *mBuf2,Order order));
#ifdef T2K_MEM_LEAK_DBG
#define SSegMsg(mBuf1, idx, mBuf2) SSegMsgNew(mBuf1, idx, mBuf2, __FILE__, __LINE__)
S16 SSegMsgNew ARGS((Buffer *mBuf1,MsgLen idx,Buffer **mBuf2,char*,uint32_t));
#else
S16 SSegMsg ARGS((Buffer *mBuf1,MsgLen idx,Buffer **mBuf2));
#endif
S16 SSwapMsg ARGS((Buffer *mBuf1, Buffer *mBuf2));
S16 SCpyMsgFix ARGS((Buffer *srcMbuf,MsgLen srcIdx,MsgLen cnt,
            Data *dstBuf,MsgLen *cCnt));
S16 SCpyFixMsg ARGS((Data *srcBuf,Buffer *dstMbuf,
            MsgLen dstIdx,MsgLen cnt,MsgLen *cCnt));
S16 SCompressMsg ARGS((Buffer *mBuf));
#ifdef T2K_MEM_LEAK_DBG
#define SAddMsgRef(mBuf,region,pool,dstBuf) SAddMsgRefNew(mBuf,region,pool,dstBuf,__FILE__,__LINE__)
#define SCpyMsgMsg(mBuf,region,pool, dstBuf) SCpyMsgMsgNew(mBuf,region,pool, dstBuf, __FILE__, __LINE__)
S16 SAddMsgRefNew ARGS((Buffer *mBuf, Region region, Pool pool, 
            Buffer **dstBuf,char*,uint32_t));
S16 SCpyMsgMsgNew ARGS((Buffer *mBuf, Region region, Pool pool, 
            Buffer **dstBuf,char* , uint32_t));
#else
S16 SCpyMsgMsg ARGS((Buffer *mBuf, Region region, Pool pool, 
            Buffer **dstBuf));
S16 SAddMsgRef ARGS((Buffer *mBuf, Region region, Pool pool, 
            Buffer **dstBuf));
S16 SIncMsgRef(Buffer *srcBuf, Region dstRegion, Pool dstPool, Buffer **dstBuf);

#ifdef SS_RBUF
/*  S16 SIncMsgRef ARGS((Buffer *srcBuf, Buffer **dstBuf)); */
Void SIncMsgLen ARGS((Buffer *mBuf));
#endif
#endif
S16 SChkRes ARGS((Region region,Pool pool,Status *status));
S16 SChkResUtl ARGS((Region region,uint8_t *wSum));
S16 SSetDateTime ARGS((DateTime *dt));
S16 SGetDateTime ARGS((DateTime *dt));
#ifdef L2_OPTMZ
Void SResetMBuf ARGS ((Buffer *mBuf));
#endif
/* ssi_x_001.main_57 : Additions */
/* ssi_x_001.main_58 : Additions */
/* ssi_x_001.main_60 : Modifications */
S16 SGetEpcTime ARGS((EpcTime *et));  
/* ssi_x_001.main_48: Added Timestamp changes */
S16 SGetTimeStamp ARGS(( S8    *ts));
S16 SGetSysTime ARGS((Ticks *sysTime));
S16 SGetRefTime ARGS((uint32_t refTime, uint32_t *sec, uint32_t *usec));
S16 SRandom ARGS((Random *value));
S16 SError ARGS((Seq seq,Reason reason));
Void SLogError ARGS((Ent ent, Inst inst, ProcId procId, Txt *file,
                       S32 line, ErrCls errCls, ErrCode errCode,
                       ErrVal errVal, Txt *errDesc));
/* ssi_x_001.main_49 : added prototype for SGetSystemTsk() */
uint32_t SGetSystemTsk ARGS ((Void));
/* changes to support multiple processors in single SSI */
/* multiple proc id changes: 
   these functions are not supported with multiple proc Ids */ 
#ifndef SS_MULTIPLE_PROCS
S16 SRegInit ARGS((Ent ent,Inst inst,PAIFS16 initFnct));
S16 SRegActvTsk ARGS((Ent ent,Inst inst,Ttype ttype,Prior prior,
                              ActvTsk actvTsk));
#endif /* SS_MULTIPLE_PROCS */

/* multiple proc id changes: procId added and time function type modified */ 
#ifndef SS_MULTIPLE_PROCS
S16 SRegCfgTmr ARGS((Ent ent,
                             Inst inst,
                             S16 period,
                             S16 units,
                             PFS16 tmrFnct));
#ifdef SS_MT_TMR
/* ssi_x_001.main_53 */
S16 SRegCfgTmrMt ARGS((Ent ent,
                               Inst inst,
                               S16 period,
                               S16 units,
                               PAIFTMRS16 tmrFnctMt));
#endif
S16 SDeregCfgTmr ARGS((Ent ent,
                               Inst inst,
                               S16 period,
                               S16 units,
                               PFS16 tmrFnct));
#ifdef SS_MT_TMR
/* ssi_x_001.main_53 */
S16 SDeregCfgTmrMt ARGS((Ent ent,
                                 Inst inst,
                                 S16 period,
                                 S16 units,
                                 PAIFTMRS16 tmrFnctMt));
#endif
#else
S16 SRegCfgTmr ARGS((ProcId proc, 
                             Ent ent,
                             Inst inst,
                             S16 period,
                             S16 units,
                             PAIFTMRS16 tmrFnct));
S16 SDeregCfgTmr ARGS((ProcId proc, 
                               Ent ent,
                               Inst inst,
                               S16 period,
                               S16 units,
                               PAIFTMRS16 tmrFnct));
#endif /* SS_MULTIPLE_PROCS */

S16 SPstTsk ARGS((Pst *pst, Buffer *mBuf));

#ifdef SS_ROUTE_MSG_CORE1
S16 SPstTskIcpu ARGS((Pst *pst, Buffer *mBuf));
#endif

#ifdef ENB_RELAY
S16 SRegDrvrTsk ARGS((Inst inst, ProcId low, ProcId high, 
                              ActvTsk actvTsk, ISTsk isTsk));
/*ssi_x_001.main_56*/
S16 SDeregDrvrTsk ARGS((Inst channel));
#endif /* ENB_RELAY */

#ifdef SS_RTR_SUPPORT
S16 SRegRtrTsk ARGS((Route *, Cntr, ActvTsk));
S16 SDeregRtrTsk ARGS((Route *, Cntr));
#endif /* SS_RTR_SUPPORT */

#ifdef SS_USE_ZBC_MEMORY
S16 SAttachPtrToBuf ARGS((
Region   region,
Pool     pool,
Data    *ptr,
MsgLen   totalLen,
Buffer** mBuf
));
#endif
/* multi-core support ssi_x_001.main_55 */
/*ssi_x_001.main_68 Added SS_AFFINITY_SUPPORT*/
#if defined(SS_MULTICORE_SUPPORT) || defined(SS_AFFINITY_SUPPORT)

typedef S8 SsAffinityMode;


typedef struct {
  uint32_t numCores;                   /* total number of cores available */
  uint32_t threadsPerCore;             /* total number of threads available per core */
  uint32_t threadRegister[SS_MAX_CORES]; /* available threads per core */
} SCpuInfo;

S16 SRegCpuInfo ARGS((SCpuInfo *cpuInfo));
S16 SSetAffinity ARGS((SSTskId *tskId, SsAffinityMode mode, uint32_t coreId, SSTskId *tskAssociatedTskId));
S16 SGetAffinity ARGS((SSTskId *tskId, uint32_t *coreId));

#endif /* SS_MULTICORE_SUPPORT || SS_AFFINITY_SUPPORT*/

#ifdef SS_AEHDPR_SUPPORT
S16 SSetAehDpr ARGS((ProcId procId, VectNmb vectNmb,PISR pisr,PTR ptr1,PDPR pdpr,PTR ptr2));
S16 SRemoveAehDpr ARGS((ProcId chipNum, VectNmb vectNmb));
#endif
S16 SAddDBufPst ARGS((Buffer *mBuf, Buffer *dBuf));
S16 SAddDBufPre ARGS((Buffer *mBuf, Buffer *dBuf));
S16 SRemDBufPst ARGS((Buffer *mBuf, Buffer **dBuf));
S16 SRemDBufPre ARGS((Buffer *mBuf, Buffer **dBuf));
S16 SGetDataRx ARGS((Buffer *dBuf, MsgLen pad, Data **dat, MsgLen *mLen));
S16 SGetDataTx ARGS((Buffer *dBuf, Data **dat, MsgLen *mLen));
S16 SUpdMsg ARGS((Buffer *mBuf, Buffer *dBuf, MsgLen mLen));
S16 SCacheFlush ARGS( (uint16_t cache_type, Data *addr, Size size));
S16 SCacheInvalidate ARGS( (uint16_t cache_type, Data *addr, Size size));
S16 SAlignDBufEven ARGS((Buffer *dBuf));
S16 SAlignDBuf ARGS((Buffer *dBuf, uint32_t align));
S16 SInitNxtDBuf ARGS((Buffer *mBuf));
S16 SGetNxtDBuf ARGS((Buffer *mBuf, Buffer **dBuf));
S16 SChkNxtDBuf ARGS((Buffer *mBuf));
S16 SSetIntPend ARGS((uint16_t id, Bool flag));
S16 SChkMsg ARGS((Buffer *mBuf));
S16 SDeregInitTskTmr ARGS((Ent ent,Inst inst));
S16 SExitTsk ARGS((void ));
S16 SExitInt ARGS((void ));
S16 SHoldInt ARGS((void ));
S16 SRelInt ARGS((void ));
S16 SEnbInt ARGS((void ));
S16 SDisInt ARGS((void ));
S16 SGetVect ARGS((VectNmb vectNmb,PIF *vectFnct));
S16 SPutVect ARGS((VectNmb vectNmb,PIF vectFnct));
#ifdef WINNT_KERN
S16 SPutIsrDpr ARGS((VectNmb vectNmb, Void *context, PIF isrFnct, 
                             PIF dprFnct));
S16 SSyncInt   ARGS((uint16_t adapterNmb, PFVOID syncFnct, 
                             Void *syncContext));
#endif
S16 SInitSema ARGS((Region region, Sema *sema));
S16 SRelSema ARGS((Region region, Sema sema));
S16 SGetSema ARGS((Region region, Sema sema));
S16 SActvInit ARGS((Ent ent,Inst inst,Region region,Reason reason));
S16 SActvTsk ARGS((Prior prior,Route route,Ent srcEnt,
            Inst srcInst,Buffer *mBuf));
S16 SActvTmr ARGS((void ));
S16 SGetOpt ARGS((int argc,char **argv,char *opts));
/* multiple proc id changes: 
   These functions not supported with multiple procIds */
#ifndef SS_MULTIPLE_PROCS
S16 SGetEntInst ARGS((Ent *ent, Inst *inst));
S16 SSetEntInst ARGS((Ent ent, Inst inst));
ProcId SFndProcId ARGS((void));
Void SSetProcId ARGS((ProcId pId));
#endif /* SS_MULTIPLE_PROCS */

S16 SGetDBufSiz ARGS((Region region, Pool pool, S16 *size));
S16 SGetStrtIdx ARGS((Region region, Pool pool,  S16 *idx));
S16 SGetEndIdx ARGS((Region region, Pool pool,  S16 *idx));
S16 SGetStrtPad ARGS((Region region, Pool pool,  S16 *pad));
/* multiple proc id changes: control block retrieval function */
#ifdef SS_MULTIPLE_PROCS
S16 SGetXxCb ARGS((ProcId proc, Ent ent, Inst inst, Void **xxCb));
#endif /* SS_MULTIPLE_PROCS */

#ifdef MT
/*
 * These routines are only available for multi-threaded
 * system service implementations
 */
S16 SGetMutex ARGS((MtMtxId *mId));
S16 SPutMutex ARGS((MtMtxId mId));
S16 SLockMutex ARGS((MtMtxId mId));
S16 SUnlockMutex ARGS((MtMtxId mId));
S16 SGetCond ARGS((MtCondId *cId));
S16 SPutCond ARGS((MtCondId cId));
S16 SCondWait ARGS((MtMtxId mId,MtCondId cId));
S16 SCondSignal ARGS((MtCondId cId));
S16 SCondBroadcast ARGS((MtCondId cId));
S16 SGetThread ARGS((MtThrd thrd, MtThrdFlags thr_flgs, 
                            Ptr arg, MtThrdId *thrdId));
S16 SPutThread ARGS((MtThrdId thrdId));
Void SThreadYield ARGS((void));
Void SThreadExit ARGS((Ptr *status));
Void SSetThrdPrior ARGS((MtThrdId tId, MtThrdPrior tPr));
Void SGetThrdPrior ARGS((MtThrdId tId, MtThrdPrior *tPr));
#endif /* MT */

Void SExit ARGS((Void));
#ifdef SS
/* multiple proc id changes: procId added */ 
#ifndef SS_MULTIPLE_PROCS

S16 SRegTTsk ARGS((Ent ent, 
                          Inst inst, 
                          Ttype type, 
                          Prior prior, 
                          PAIFS16 initTsk, 
                          ActvTsk actvTsk));
/* ssi_x_001.main_60 */
S16 SRegCbTsk ARGS((Ent ent, 
                          Inst inst, 
                          ActvTsk actvTsk));
S16 SDeregTTsk ARGS((Ent ent, Inst inst));

#else

S16 SRegTTsk ARGS((ProcId proc, 
                          Ent ent, 
                          Inst inst, 
                          Ttype type, 
                          Prior prior, 
                          PAIFS16 initTsk, 
                          ActvTsk actvTsk));
S16 SDeregTTsk ARGS((ProcId proc, Ent ent, Inst inst));

S16 SRegCbTsk ARGS((ProcId proc,
								  Ent ent, 
                          Inst inst, 
                          ActvTsk actvTsk));
#endif /* SS_MULTIPLE_PROCS */

S16 SCreateSTsk ARGS((SSTskPrior tskPrior, SSTskId *tskId));
S16 SDestroySTsk ARGS((SSTskId tskId));
/* multiple proc id changes: procId added */ 
#ifndef SS_MULTIPLE_PROCS

S16 SAttachTTsk ARGS((Ent ent, Inst inst, SSTskId tskId));
S16 SDetachTTsk ARGS((Ent ent, Inst inst));

#else

S16 SAttachTTsk ARGS((ProcId proc, Ent ent, Inst inst, SSTskId tskId));
S16 SDetachTTsk ARGS((ProcId proc, Ent ent, Inst inst));

#endif /* SS_MULTIPLE_PROCS */
S16 SRegRegion ARGS((Region region, SRegInfo *regInfo));
S16 SDeregRegion ARGS((Region region));
S16 SRegDynRegion ARGS((Region region, SRegInfo *regInfo)); 
/*ssi_x_001.main_62-prototype for SAlloc and SFree, gaurded under flag*/
#ifndef SS_FAP
 /* ssi_x_001.main_57 : Additions */
#ifdef SS_HISTOGRAM_SUPPORT 
S16 SAlloc ARGS((Region region, Size *size, uint32_t flags, Data **ptr,uint32_t line, uint8_t *fileName, uint8_t entId));
S16 SFree ARGS((Region region, Data *ptr, Size size, uint32_t line, uint8_t *fileName, uint8_t entId));
#else
#ifdef T2K_MEM_LEAK_DBG
#define SAlloc(region,size,flags,ptr) SAllocNew(region,size,flags,ptr,file,line)
#define SFree(region,ptr,size) SFreeNew(region,ptr,size,file,line)
S16 SAllocNew ARGS((Region region, Size *size, uint32_t flags, Data **ptr,char*,uint32_t));
S16 SFreeNew ARGS((Region region, Data *ptr, Size size,char*,uint32_t));
#elif defined(SS_LIGHT_MEM_LEAK_STS)
S16 SAlloc ARGS((Region region, Size *size, uint32_t flags, Data **ptr,uint32_t line, uint8_t *fnName));
S16 SFree ARGS((Region region, Data *ptr, Size size, uint32_t line, uint8_t *fnName));
#else
S16 SAlloc ARGS((Region region, Size *size, uint32_t flags, Data **ptr));
S16 SFree ARGS((Region region, Data *ptr, Size size));
#endif
#endif
S16 SGetBufRegionPool ARGS((Buffer *mBuf, Region *region, Pool *pool));
#endif /* SS_ENABLE_MACROS */

#ifdef SS_OLD_THREAD
S16 SGetMutex      ARGS((SMtxId *mId));
S16 SPutMutex      ARGS((SMtxId mId));
S16 SLockMutex     ARGS((SMtxId mId));
S16 SUnlockMutex   ARGS((SMtxId mId));
S16 SGetCond       ARGS((SCondId *cId));
S16 SPutCond       ARGS((SCondId cId));
S16 SCondWait      ARGS((SMtxId mId,SCondId cId));
S16 SCondSignal    ARGS((SCondId cId));
S16 SCondBroadcast ARGS((SCondId cId));
S16 SGetThread     ARGS((SThrd thrd, S32 thr_flgs, 
                                                 Ptr arg, SThrdId *thrdId));
S16 SPutThread     ARGS((SThrdId thrdId));
Void SThreadYield  ARGS((void));
Void SThreadExit   ARGS((Ptr status));
Void SSetThrdPrior ARGS((SThrdId tId, S32 tPr));
Void SGetThrdPrior ARGS((SThrdId tId, S32 *tPr));
Void SExit         ARGS((void));
#else /* SS_OLD_THREAD */
S16 SThreadYield  ARGS((void));
#endif /* SS_OLD_THREAD */

S16 SInitLock     ARGS((SLockId *lock,uint8_t type));
S16 SLock         ARGS((SLockId *lock));
S16 SUnlock       ARGS((SLockId *lock));
S16 SDestroyLock  ARGS((SLockId *lock));


S16 SInitSemaphore ARGS((SsSemaId *sem, uint8_t value));
S16 SWaitSemaphore ARGS((SsSemaId *sem));
S16 SPostSemaphore ARGS((SsSemaId *sem));
S16 SDestroySemaphore ARGS((SsSemaId *sem));

/* multiple proc id changes: 
   new function required to implement multiple procIds */
#ifdef SS_MULTIPLE_PROCS
S16 SAddProcIdLst ARGS((uint16_t numPIds, ProcId *pIdLst));
S16 SRemProcIdLst ARGS((uint16_t numPIds, ProcId *pIdLst));
S16 SGetProcIdLst ARGS((uint16_t *numPIds, ProcId *pIdLst));
#endif /* SS_MULTIPLE_PROCS */

#endif /* SS */

#ifdef SS_SEGV_SIG_HDLR
#ifndef SS_MULTIPLE_PROCS
S16 SRegIntrptHdlr ARGS((Ent ent, Inst inst, PFS16 intrptActvFn));
#else
S16 SRegIntrptHdlr ARGS((ProcId proc, Ent ent, Inst inst, PFS16 intrptActvFn));
#endif/*SS_MULTIPLE_PROCS*/
#endif /* SS_SEGV_SIG_HDLR */

#ifdef PORTVER
/* procId added */ 
#ifndef SS_MULTIPLE_PROCS
S16 SRegTTsk ARGS((Ent ent, 
                          Inst inst, 
                          Ttype type, 
                          Prior prior, 
                          PAIFS16 initTsk, 
                          ActvTsk actvTsk));
/* ssi_x_001.main_60 */
S16 SRegCbTsk ARGS((Ent ent, 
                          Inst inst, 
                          ActvTsk actvTsk));
S16 SDeregTTsk ARGS((Ent ent, Inst inst));
S16 SAttachTTsk ARGS((Ent ent, Inst inst, SSTskId tskId));
S16 SDetachTTsk ARGS((Ent ent, Inst inst));
#else
S16 SRegTTsk ARGS((ProcId proc, 
                          Ent ent, 
                          Inst inst, 
                          Ttype type, 
                          Prior prior, 
                          PAIFS16 initTsk, 
                          ActvTsk actvTsk));
S16 SRegCbTsk ARGS((ProcId proc,
								  Ent ent, 
                          Inst inst, 
                          ActvTsk actvTsk));
S16 SDeregTTsk ARGS((ProcId proc, Ent ent, Inst inst));
S16 SAttachTTsk ARGS((ProcId proc, Ent ent, Inst inst, SSTskId tskId));
S16 SDetachTTsk ARGS((ProcId proc, Ent ent, Inst inst));
#endif /* SS_MULTIPLE_PROCS */

S16 SCreateSTsk ARGS((SSTskPrior tskPrior, SSTskId *tskId));
S16 SDestroySTsk ARGS((SSTskId tskId));
#ifndef SS_ENABLE_MACROS
S16 SGetBufRegionPool ARGS((Buffer *mBuf, Region *region, Pool *pool));
#endif /* SS_ENABLE_MACROS */

S16 SInitLock     ARGS((SLockId *lock,uint8_t type));
S16 SLock         ARGS((SLockId *lock));
S16 SUnlock       ARGS((SLockId *lock));
S16 SDestroyLock  ARGS((SLockId *lock));

S16 SInitSemaphore ARGS((SsSemaId *sem, uint8_t value));
S16 SWaitSemaphore ARGS((SsSemaId *sem));
S16 SPostSemaphore ARGS((SsSemaId *sem));
S16 SDestroySemaphore ARGS((SsSemaId *sem));
/* functions required to implement multiple procIds */ 
#ifdef SS_MULTIPLE_PROCS
S16 SAddProcIdLst ARGS((uint16_t numPIds, ProcId *pIdLst));
S16 SRemProcIdLst ARGS((uint16_t numPIds, ProcId *pIdLst));
S16 SGetProcIdLst ARGS((uint16_t *numPIds, ProcId *pIdLst));
#endif /* SS_MULTIPLE_PROCS */

#endif /* PORTVER */

  
/* function prototypes */
 
/* stack manager initialization function */
/* multiple proc id changes: procId added */ 
#ifndef SS_MULTIPLE_PROCS
S16 smActvInit ARGS((Ent ent, Inst inst, Region region, Reason reason));
#else /* SS_MULTIPLE_PROCS */
S16 smActvInit ARGS((ProcId proc, 
                            Ent ent, 
                            Inst inst, 
                            Region region, 
                            Reason reason,
                            Void **xxCb));
#endif /* SS_MULTIPLE_PROCS */

/* stack manager external initialization function */
S16 smInitExt ARGS((void));
 
/* stack manager activation function */
S16 smActvTsk ARGS((Pst *pst, Buffer *mBuf));

/* ssi_x_001.main_54 */
#ifdef SS_SID_CHANGE
/* ssi_x_001.main_53 */
S16 SGetInDepSId ARGS((SystemId *s));
S16 SGetDepSId ARGS((SystemId *s));
#endif

/* Macros... */
 
#define SS_INITDCHN(chn) (chn)->next = NULLP; (chn)->prev = NULLP
#define SS_INITQUE(qu) \
   SS_INITDCHN(&(qu)->dChn); (qu)->crntSize = 0
#define SS_QLEN(bq)  ((bq)->crntSize)
#define SS_MLEN(mb) ((mb)->t.msg.msgLen)

/* ssi_x_001.main_50 : Added declarations for mutex related functions
 * to protect demand queue and memory allocation 
 */
/* ssi_x_001.main_51 : Added control flag as these are only used by windows */
#ifdef SS_WIN
S16 WTInitLock     ARGS((SLockId *lock,uint8_t type));
S16 WTLock         ARGS((SLockId *lock));
S16 WTUnlock       ARGS((SLockId *lock));
S16 WTDestroyLock  ARGS((SLockId *lock));
#endif /* End of SS_WIN */
 /* ssi_x_001.main_57 : Additions */
#ifdef SS_LOGGER_SUPPORT  
S16 SRegLogCfg ARGS(( uint8_t mode, S8 *path, uint32_t size, S8 *IPA, uint16_t port));
S16 SWrtLogBuf ARGS(( Txt *buf ));
/* ssi_x_001.main_60 */
S16 SDeregLogCfg ARGS((Void ));
#endif /* SS_LOGGER_SUPPORT */

#ifdef SS_HISTOGRAM_SUPPORT
S16 SRegForHstGrm ARGS((Ent ent));
S16 SHstGrmInfoShow ARGS((Ent *entId));
S16 SFillEntIds ARGS((Void));
S16 SGetEntInd ARGS((Ent *entId, uint8_t *fileName));
#endif /* SS_HISTOGRAM_SUPPORT */
/* ssi_x_001.main_68  Multiple declaration removed , one already in cm_task.x */
/* ssi_x_001.main_61: Lock support guraded under the flag */
#ifdef SS_LOCK_SUPPORT
S16 SLockNew ARGS((SLockInfo *LockId, uint8_t lockType));
S16 SInitLockNew ARGS((SLockInfo *LockId, uint8_t lockType));
S16 SUnlockNew ARGS((SLockInfo *LockId, uint8_t lockType));
S16 SDestroyLockNew ARGS((SLockInfo *LockId, uint8_t lockType));
#endif /* SS_LOCK_SUPPORT */
S8* SGetConfigPath ARGS((Void));

/* ssi_x_001.main_66 : Added new Buffer manegement APIs */
S16 SCpyPartMsg ARGS((Buffer *srcBuf, MsgLen idx, MsgLen cnt, Buffer *dstBuf));
S16 SRepPartMsg ARGS((Buffer *srcBuf, MsgLen idx, MsgLen cnt, Buffer *dstBuf));
S16 SMovPartMsg ARGS((Buffer *srcBuf, MsgLen idx, Buffer *dstBuf));
S16 SPkMsgMult ARGS((Data *src, MsgLen cnt, Buffer *mBuf));
S16 SGetReadPtr ARGS((Buffer *mBuf, uint8_t** data, MsgLen *len));

typedef enum
{
   SS_SHARABLE_MEMORY,
   SS_NON_SHARABLE_MEMORY
}ssMemoryType;
#ifdef TENB_T2K3K_SPECIFIC_CHANGES                              
S16 SReInitTmr ARGS((Void ));                            
#endif
/* ssi_x_001.main_69: Added MSPD debug macro */
#ifdef MSPD
uint32_t ysGT;
char ys_global_printbuf[256];
Void rbCallstackShow ARGS((Void));
Void rbCallstackShowForCore ARGS((uint32_t coreId));
uint32_t MacGetTick ARGS ((void));
#endif /* MSPD */
#ifdef LTE_L2_MEAS
uint64_t  glblTtiCnt;
#endif

S16 SStartTask ARGS((volatile uint32_t *startTime, uint32_t tarkId));
S16 SStopTask ARGS((volatile uint32_t startTime,uint32_t taskId));
#ifdef MSPD_MLOG_NEW
/* Removed for C++ Compilation
unsigned int MLogTask (unsigned int taskid, unsigned int resourceid , 
                       unsigned int ticksstart,unsigned int ticksstop);
--*/
#endif

#if defined(MSPD) && defined(MSPD_DBG_ENABLE)
#define MSPD_DBG_RLC(...) 
#ifdef TENB_T2K3K_SPECIFIC_CHANGES
#ifndef ALIGN_64BIT
#define MSPD_DBG(...) do { \
   char _local_printbuf[SS_MEMLOG_MAXSTRLEN]; \
   int _len1 = snprintf(_local_printbuf, SS_MEMLOG_MAXSTRLEN, "%s():%u @%lu ", __func__, __LINE__, ysGT); \
   int _len2 = snprintf(&_local_printbuf[_len1],SS_MEMLOG_MAXSTRLEN - _len1, __VA_ARGS__) + _len1; \
   ssMemlog(_local_printbuf, _len2); \
} while (0)
#else
#define MSPD_DBG(...) do { \
   char _local_printbuf[SS_MEMLOG_MAXSTRLEN]; \
   int _len1 = snprintf(_local_printbuf, SS_MEMLOG_MAXSTRLEN, "%s():%u @%u ", __func__, __LINE__, ysGT); \
   int _len2 = snprintf(&_local_printbuf[_len1],SS_MEMLOG_MAXSTRLEN - _len1, __VA_ARGS__) + _len1; \
   ssMemlog(_local_printbuf, _len2); \
} while (0)
#endif
#define MSPD_DBG_RAW(...) do { \
   char _local_printbuf[SS_MEMLOG_MAXSTRLEN]; \
   int _len = snprintf(_local_printbuf,SS_MEMLOG_MAXSTRLEN, __VA_ARGS__); \
   ssMemlog(_local_printbuf, _len); \
} while (0)
#ifndef ALIGN_64BIT
#define MSPD_LOG(...) do { \
   char _local_printbuf[SS_MEMLOG_MAXSTRLEN]; \
   int _len1 = snprintf(_local_printbuf, SS_MEMLOG_MAXSTRLEN,"%s():%u @%lu ", __func__, __LINE__, ysGT); \
   int _len2 = snprintf(&_local_printbuf[_len1],SS_MEMLOG_MAXSTRLEN - _len1, __VA_ARGS__) + _len1; \
   ssMemlog(_local_printbuf, _len2); \
} while (0)
#else
#define MSPD_LOG(...) do { \
   char _local_printbuf[SS_MEMLOG_MAXSTRLEN]; \
   int _len1 = snprintf(_local_printbuf, SS_MEMLOG_MAXSTRLEN,"%s():%u @%u ", __func__, __LINE__, ysGT); \
   int _len2 = snprintf(&_local_printbuf[_len1],SS_MEMLOG_MAXSTRLEN - _len1, __VA_ARGS__) + _len1; \
   ssMemlog(_local_printbuf, _len2); \
} while (0)

#endif
#ifndef ALIGN_64BIT
#define MSPD_ERR(...) do { \
   char _local_printbuf[SS_MEMLOG_MAXSTRLEN]; \
   int _len1 = snprintf(_local_printbuf, SS_MEMLOG_MAXSTRLEN, "ERROR: %s():%u @%lu ", __func__, __LINE__, ysGT); \
   int _len2 = snprintf(&_local_printbuf[_len1],SS_MEMLOG_MAXSTRLEN - _len1, __VA_ARGS__) + _len1; \
   ssMemlog(_local_printbuf, _len2); \
} while (0)
#else
#define MSPD_ERR(...) do { \
   char _local_printbuf[SS_MEMLOG_MAXSTRLEN]; \
   int _len1 = snprintf(_local_printbuf, SS_MEMLOG_MAXSTRLEN, "ERROR: %s():%u @%u ", __func__, __LINE__, ysGT); \
   int _len2 = snprintf(&_local_printbuf[_len1],SS_MEMLOG_MAXSTRLEN - _len1, __VA_ARGS__) + _len1; \
   ssMemlog(_local_printbuf, _len2); \
} while (0)

#endif

Void ssMlogInit(Void);
Void ssMlogIncrCounter(Void);
Void ssMemlogInit(Void);
Void ssMemlog(char *, uint32_t size);
Void ssMemlogWrite(Void);

#endif /* TENB_T2K3K_SPECIFIC_CHANGES */
#endif

#ifdef __cplusplus
}
#endif

Void SIncrementTtiCount(Void);
Ticks SGetTtiCount(Void);
#ifdef TENB_T2K3K_SPECIFIC_CHANGES
Void mtStopHndlr(Void);
#endif

#ifdef TENB_RTLIN_CHANGES
Void ysPrntBkTrace(Void);
Void ssMlogInit(Void);
Void ssMlogIncrCounter(Void);
#endif
#ifdef SS_THR_REG_MAP
Void ssRegMainThread(Void);
#endif

#ifdef T2K_MEM_LEAK_DBG
#define ssGetDBufOfSize(region,size,dBuf) ssGetDBufOfSizeNew(region,size,dBuf,__FILE__,__LINE__)
S16 ssGetDBufOfSizeNew ARGS((Region region, Size size, Buffer **dBuf,char*,uint32_t));
#else
S16 ssGetDBufOfSize(Region region,Size size,Buffer **dBuf);
#endif

/* ssi_x_001.main_69: Added MSPD debug macro */
#ifdef MSPD
void SEND_DBG_MSG(uint8_t *str, ...);
#endif

#endif /* __SSIX__ */


/********************************************************************30**
  
         End of file
**********************************************************************/
