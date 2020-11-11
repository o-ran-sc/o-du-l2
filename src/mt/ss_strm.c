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

     Name:     System Services -- STREAMS

     Type:     C source file

     Desc:     Implementation of STREAMS messaging functions.

     File:     ss_strm.c

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
#include "ss_strm.h"       /* STREAMS */
#include "ss_msg.h"        /* messaging */
#include "ss_mem.h"        /* memory management interface */
#include "ss_gen.h"        /* general */
#include "cm_mem.h"        /* memory management */



/* header/extern include files (.x) */

#include "gen.x"           /* general layer */
#include "ssi.x"           /* system services */


#include "ss_dep.x"        /* implementation-specific */
#include "ss_queue.x"      /* queues */
#include "ss_task.x"       /* tasking */
#include "ss_timer.x"      /* timers */
#include "ss_strm.x"       /* STREAMS */
#include "ss_msg.x"        /* messaging */
#include "ss_mem.x"        /* memory management interface */
#include "ss_drvr.x"       /* driver tasks */
#ifdef SS_LOCKLESS_MEMORY
#include "cm_llist.x"
#include "cm_hash.x"
#include "cm_mem_wl.x"        /* common memory manager */
#else
#include "cm_mem.x"        /* common memory manager */
#endif /* SS_LOCKLESS_MEMORY */
#include "ss_gen.x"        /* general */



/* private variable declarations */

static struct
{
   Region mdRegion;     /* region for message and data blocks */
   Region datRegion;    /* region for data buffers */
} strmCfg;




/*
*     Interface functions (System Services--non-STREAMS interface)
*/
 

/*
*
*       Fun:   ssStrmCfg
*
*       Desc:  Configures the STREAMS system.
*
*       Ret:   ROK      - ok
*
*       Notes:
*
*       File:  ss_strm.c
*
*/
S16 ssStrmCfg
(
Region mdRegId,                 /* region for message and data blocks */
Region datRegId                 /* region for data buffers */
)
{

   strmCfg.mdRegion = mdRegId;
   strmCfg.datRegion = datRegId;


   return ROK;
}


/*
*     STREAMS interface functions
*
*       All these assume that ssStrmCfg() has been called first, with
*       valid parameters.
*/
 

/*
*
*       Fun:   ssAdjMsg
*
*       Desc:  Trim abs(len) bytes from a message. If len is less than
*              0, trim from the tail, otherwise from the head. Operates
*              only on blocks of the same type. Does not remove emptied
*              message blocks.
*
*
*       Ret:   1        - ok
*              0        - failure
*
*       Notes:
*
*       File:  ss_strm.c
*
*/
S32 ssAdjMsg
(
SsMblk *mp,                     /* message */
S32 len                         /* bytes to remove */
)
{
   S32 n;                       /* counter */
   S32 size;                    /* size of mblks of same type as head/tail */
   uint8_t type;                     /* message type */
   SsMblk *bp;                  /* mblk for iteration */
   SsMblk *first;               /* first mblk to be adjusted */

#if (ERRCLASS & ERRCLS_INT_PAR)
   if (mp == NULLP)
   {
      SSLOGERROR(ERRCLS_INT_PAR, ESS325, ERRZERO, "Null pointer");
      return (0);
   }
#endif


   if (len == 0)                /* nothing to do */
   {
      return (1);
   }


   if (len > 0)                 /* trim from the head */
   {
      /* figure out the size of all blocks of the same type as the head */
      bp = mp;
      size = 0;
      type = bp->b_datap->db_type;
      while (bp  &&  bp->b_datap->db_type == type)
      {
         n = bp->b_wptr - bp->b_rptr;
         if (n > 0)
         {
            size += n;
         }

         bp = bp->b_cont;
      }


      /* if we can't trim len bytes, fail */
      if (len > size)
      {
         return (0);
      }


      /* do the trimming */
      bp = mp;
      for (; ;)
      {
         n = bp->b_wptr - bp->b_rptr;

         if (n >= len)
         {
            bp->b_rptr += len;
            break;
         }
         else if (n > 0)
         {
            bp->b_rptr += len;
            len -= n;
         }

         bp = bp->b_cont;
      }
   }
   else                         /* trim from the tail */
   {
      /* figure out the size of all blocks of the same type as the tail */
      bp = mp;
      first = bp;
      size = 0;
      type = bp->b_datap->db_type;
      while (bp)
      {
         if (bp->b_datap->db_type == type)
         {
            n = bp->b_wptr - bp->b_rptr;
            if (n > 0)
            {
               size += n;
            }
         }
         else
         {
            type = bp->b_datap->db_type;
            first = bp;
            size = 0;
         }

         bp = bp->b_cont;
      }


      /* if we can't trim len bytes, fail */
      size += len;
      if (size < 0)
      {
         return (0);
      }


      /* do the trimming */
      while (first)
      {
         n = first->b_wptr - first->b_rptr;

         if (size <= 0)
         {
            first->b_rptr = first->b_wptr;
         }
         else if (n > 0)
         {
            if (n > size)
            {
               first->b_wptr = first->b_rptr + size;
            }

            size -= n;
         }

         first = first->b_cont;
      }
   }


   return (1);
} /* ssAdjMsg */


/*
*
*       Fun:   ssAllocB
*
*       Desc:  Returns a pointer to a message block of type M_DATA
*              in which the data buffer is of at least the specified
*              size.
*
*
*       Ret:   non-NULL - success
*              NULL     - failure
*
*       Notes: The parameter pri is unused.
*
*       File:  ss_strm.c
*
*/
SsMblk *ssAllocB
(
S32 size,                       /* required size */
uint32_t pri                         /* message priority */
)
{
   SsMblk *bp;                  /* mblk for iteration */
   Data *dat;                   /* pointer to data buffer */
   Size m;                      /* temporary */
   Size n;                      /* temporary */
   S16 r;                       /* return value */


#ifdef T2K_MEM_LEAK_DBG
   char* file = __FILE__;
   uint32_t line = __LINE__;
#endif

   UNUSED(pri);


   /* allocate a single block for the mblock and the dblock */
   m = (sizeof(SsMblk) + sizeof(SsDblk));
/* ss001.301: additions */
#ifdef SS_HISTOGRAM_SUPPORT
   r = SAlloc(strmCfg.mdRegion, &m, 0, (Data **)&bp, __LINE__, (uint8_t*)__FILE__, ENTNC);
#else
   r = SAlloc(strmCfg.mdRegion, &m, 0, (Data **)&bp);
#endif /* SS_HISTOGRAM_SUPPORT */
   if (r != ROK)
   {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SSLOGERROR(ERRCLS_ADD_RES, ESS326, (ErrVal) r, "SAlloc() failed");
#endif

      return (NULLP);
   }


   /* allocate space for the data block */
   if (size > 0)
   {
      n = size;
/* ss001.301: additions */
#ifdef SS_HISTOGRAM_SUPPORT
      r = SAlloc(strmCfg.datRegion, &n, 0, &dat, __LINE__, (uint8_t*)__FILE__, ENTNC);
#else
      r = SAlloc(strmCfg.datRegion, &n, 0, &dat);
#endif /* SS_HISTOGRAM_SUPPORT */
      if (r != ROK)
      {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SSLOGERROR(ERRCLS_ADD_RES, ESS327, (ErrVal) r, "SAlloc() failed");
#endif

/* ss001.301: additions */
#ifdef SS_HISTOGRAM_SUPPORT
         SFree(strmCfg.mdRegion, (Data *)bp, m, __LINE__, (uint8_t*)__FILE__, ENTNC);
#else
         SFree(strmCfg.mdRegion, (Data *)bp, m);
#endif /* SS_HISTOGRAM_SUPPORT */
         return (NULLP);
      }
   }
   /* we _can_ allocate a message with an empty data block */
   else
   {
      dat = NULLP;
   }

   /* generic set-up-message function */
   SS_STRM_INITB(bp, (SsDblk *)(((uint8_t *)bp) + sizeof(SsMblk)), dat, size, NULLP);


   return (bp);
} /* ssAllocB */


/*
*
*       Fun:   ssCopyB
*
*       Desc:  Copies the contents of the specified message block
*              into a newly-allocated message block of at least
*              the same size. Calls ssAllocB().
*
*       Ret:   non-NULL - ok
*              NULL     - failure
*
*       Notes:
*
*       File:  ss_strm.c
*
*/
SsMblk *ssCopyB
(
SsMblk *mp                      /* message block */
)
{
   SsMblk *bp;                  /* mblk for iteration */
   uint8_t *ptr;                     /* pointer to data */
   uint32_t size;                    /* size of data content */


#if (ERRCLASS & ERRCLS_INT_PAR)
   if (mp == NULLP)
   {
      SSLOGERROR(ERRCLS_INT_PAR, ESS328, ERRZERO, "Null pointer");
      return (NULLP);
   }
#endif


   /* allocate another message */
   bp = ssAllocB((mp->b_datap->db_lim - mp->b_datap->db_base), 0);
   if (bp == NULLP)
   {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SSLOGERROR(ERRCLS_ADD_RES, ESS329, ERRZERO, "ssAllocB() failed");
#endif

      return (NULLP);
   }


   /* copy the contents, if any */
   size = (mp->b_wptr - mp->b_rptr);
   if (size > 0)
   {
      ptr = mp->b_rptr;
      while (ptr != mp->b_wptr)
      {
         *bp->b_wptr++ = *ptr++;
      }
   }


   return (bp);
} /* ssCopyB */


/*
*
*       Fun:   ssCopyMsg
*
*       Desc:  Uses ssCopyB to copy the message blocks contained in
*              the specified message to newly allocated blocks and
*              links those blocks into a new message.
*
*       Ret:   non-NULL - ok
*              NULL     - failure
*
*       Notes:
*
*       File:  ss_strm.c
*
*/
SsMblk *ssCopyMsg
(
SsMblk *mp                      /* message block */
)
{
   SsMblk *first;               /* first mblk in message */
   SsMblk *bp;                  /* mblk for iteration */

#if (ERRCLASS & ERRCLS_INT_PAR)
   if (mp == NULLP)
   {
      SSLOGERROR(ERRCLS_INT_PAR, ESS330, ERRZERO, "Null pointer");
      return (NULLP);
   }
#endif


   /* copy the first mblock */
   first = bp = ssCopyB(mp);
   if (bp == NULLP)
   {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SSLOGERROR(ERRCLS_ADD_RES, ESS331, ERRZERO, "ssCopyB() failed");
#endif

      return (NULLP);
   }


   /* copy all the rest, linking the new ones at the same time */
   while (mp->b_cont)
   {
      mp = mp->b_cont;

      bp->b_cont = ssCopyB(mp);

      /* if any copy fails, free whatever exists of the new message */
      if (bp->b_cont == NULLP)
      {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SSLOGERROR(ERRCLS_ADD_RES, ESS332, ERRZERO, "ssCopyB() failed");
#endif

         ssFreeMsg(first);
         return (NULLP);
      }

      bp = bp->b_cont;
   }


   return (first);
} /* ssCopyMsg */


/*
*
*       Fun:   ssDupB
*
*       Desc:  Duplicates the specified message block, copying it
*              into a newly-allocated message block. Increments
*              the reference count of the data block that is pointed
*              at by the original message block descriptor.
*
*       Ret:   non-NULL - ok
*              NULL     - failure
*
*       Notes:
*
*       File:  ss_strm.c
*
*/
SsMblk *ssDupB
(
SsMblk *mp                      /* message block */
)
{
   SsMblk *bp;                  /* mblk for iteration */
   /* ss028.103 - Addition of lock for mBuf reference count */

#if (ERRCLASS & ERRCLS_INT_PAR)
   if (mp == NULLP)
   {
      SSLOGERROR(ERRCLS_INT_PAR, ESS333, ERRZERO, "Null pointer");
      return (NULLP);
   }
#endif


   /* allocate a message with no data buffer */
   bp = ssAllocB(0, 0);
   if (bp == NULLP)
   {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SSLOGERROR(ERRCLS_ADD_RES, ESS334, ERRZERO, "ssAllocB() failed");
#endif

      return (NULLP);
   }


   /*  make the new message block identical to the one to be dup'ed.
    *  notice that an mblk/dblk pair is allocated but only the mblk
    *  is used,, this is for optimum performance in the average case.
    */



 /* ss002.13: addition */


 /* ss003.13: addition */
   SMemCpy( (Void *)bp, (Void *)mp, (size_t)sizeof(SsMblk));


   /* ss028.103 - Addition of lock for mBuf reference count */
	/* ss006.301 - use the mBufRefLock for the DFLT_REGION */
#ifndef SS_DBUF_REFLOCK_DISABLE
   if(SLock(&mp->b_datap->dBufLock) != ROK)
   {
      SSLOGERROR(ERRCLS_DEBUG, ESS335, ERRZERO,
                     "Could not lock the mBuf Ref Lock");
      return (NULLP);
   }
#endif

   /* increment the reference count of the dblock */
   mp->b_datap->db_ref++;
   mp->b_datap->shared = TRUE;

	/* ss006.301 - use the mBufRefLock for the DFLT_REGION */
#ifndef SS_DBUF_REFLOCK_DISABLE
   if ( SUnlock(&mp->b_datap->dBufLock) != ROK)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      SSLOGERROR(ERRCLS_DEBUG, ESS336, ERRZERO,
                      "Could not give the Semaphore");
      return (NULLP);
#endif
   }
#endif

   return (bp);
} /* ssDupB */


/*
*
*       Fun:   ssDupMsg
*
*       Desc:  Calls ssDupB to duplicate the specified message by
*              copying all message block descriptors and then linking
*              the new message blocks together to form a new message.
*
*       Ret:   non-NULL - ok
*              NULL     - failure
*
*       Notes:
*
*       File:  ss_strm.c
*
*/
SsMblk *ssDupMsg
(
SsMblk *mp                      /* message block */
)
{
   SsMblk *first;               /* first mblk in message */
   SsMblk *bp;                  /* mblk for iteration */

   /* dup the first mblock */
   first = bp = ssDupB(mp);
   if (bp == NULLP)
   {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SSLOGERROR(ERRCLS_ADD_RES, ESS337, ERRZERO, "ssDupB() failed");
#endif

      return (NULLP);
   }


   /* dup all the rest, linking the new ones at the same time */
   while (mp->b_cont)
   {
      mp = mp->b_cont;

      bp->b_cont = ssDupB(mp);

      /* if any dup fails, free whatever exists of the new message */
      if (bp->b_cont == NULLP)
      {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SSLOGERROR(ERRCLS_ADD_RES, ESS338, ERRZERO, "ssDupB() failed");
#endif

         ssFreeMsg(first);
         return (NULLP);
      }

      bp = bp->b_cont;
   }


   return (first);
} /* ssDupMsg */


/*
*
*       Fun:   ssESBAlloc
*
*       Desc:  Allocates message and data blocks that point directly
*              at a client-supplied buffer.
*
*       Ret:   non-NULL - ok
*              NULL     - failure
*
*       Notes:
*
*       File:  ss_strm.c
*
*/
SsMblk *ssESBAlloc
(
uint8_t *base,                       /* client supplied data buffer */
S32 size,                       /* size of data buffer */
S32 pri,                        /* message priority */
SsFrtn *fr_rtn                  /* free routine */
)
{
   SsMblk *bp;                  /* mblk for iteration */
   Size m;                      /* mblk + dblk */
   S16 r;                       /* return value */

   UNUSED(pri);


#if (ERRCLASS & ERRCLS_INT_PAR)
   if (base == NULLP  ||  fr_rtn == NULLP)
   {
      SSLOGERROR(ERRCLS_INT_PAR, ESS339, ERRZERO, "Null pointer");
      return (NULLP);
   }
#endif


   /* allocate space for an mblock and a dblock */
   m = (sizeof(SsMblk) + sizeof(SsDblk));
/* ss001.301: additions */
#ifdef SS_HISTOGRAM_SUPPORT
   r = SAlloc(strmCfg.mdRegion, &m, 0, (Data **)&bp, __LINE__, (uint8_t*)__FILE__, ENTNC);
#else
#ifdef T2K_MEM_LEAK_DBG
   char* file = __FILE__;
   uint32_t line = __LINE__;
#endif
   r = SAlloc(strmCfg.mdRegion, &m, 0, (Data **)&bp);
#endif /* SS_HISTOGRAM_SUPPORT */
   if (r != ROK)
   {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SSLOGERROR(ERRCLS_ADD_RES, ESS340, (ErrVal) r, "SAlloc() failed");
#endif

      return (NULLP);
   }


   /* use the generic set-up-message function to initialize everything */
   SS_STRM_INITB(bp, (SsDblk *)(((uint8_t *)bp) + sizeof(SsMblk)),
                  base, size, fr_rtn);


   return (bp);
} /* ssESBAlloc */


/*
*
*       Fun:   ssFreeB
*
*       Desc:  Deallocates the specified message block descriptor
*              and frees the corresponding data block if the
*              reference count is equal to 1. If not, the reference
*              count is decremented.
*
*       Ret:   Void
*
*       Notes:
*
*       File:  ss_strm.c
*
*/
Void ssFreeB
(
SsMblk *mp                      /* message block */
)
{
   SsMblk *bp;                  /* mblk for iteration */
   Size size;                   /* mblk + dblk */
   /* ss028.103 - Addition of lock for mBuf reference count */

#ifdef T2K_MEM_LEAK_DBG
   char* file = __FILE__;
   uint32_t line = __LINE__;
#endif

#if (ERRCLASS & ERRCLS_INT_PAR)
   if (mp == NULLP)
   {
      SSLOGERROR(ERRCLS_INT_PAR, ESS341, ERRZERO, "Null pointer");
      return;
   }
#endif


   size = sizeof(SsMblk) + sizeof(SsDblk);


   /*  this mblock has already been freed, and still exists only
    *  because there is a reference to its dblock. it will be
    *  freed when the referring mblock is freed.
    */

   if (mp->b_datap == NULLP)
   {
      return;
   }


   /* ?? this should never happen ?? */
   if (mp->b_datap->db_ref == 0)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      SSLOGERROR(ERRCLS_DEBUG, ESS342, ERRZERO,
                  "Zero reference count in ssFreeB; logic failure");
#endif

/* ss001.301: additions */
#ifdef SS_HISTOGRAM_SUPPORT
      SFree(strmCfg.mdRegion, (Data *)mp, size, __LINE__, (uint8_t*)__FILE__, ENTNC);
#else
#ifdef T2K_MEM_LEAK_DBG
      SFree(strmCfg.mdRegion, (Data *)mp, size);
#else
      SFree(strmCfg.mdRegion, (Data *)mp, size);
#endif
#endif /* SS_HISTOGRAM_SUPPORT */
      return;
   }

   /* ss028.103 - Addition of lock for mBuf reference count */
	/* ss006.301 - use the mBufRefLock for the DFLT_REGION */
#ifndef SS_DBUF_REFLOCK_DISABLE
   if(SLock(&mp->b_datap->dBufLock) != ROK)
   {
      SSLOGERROR(ERRCLS_DEBUG, ESS343, ERRZERO,
                     "Could not lock the mBuf Ref Lock");
      return;
   }
#endif

   /* decrement reference count of the dblock */
   mp->b_datap->db_ref--;

	/* ss006.301 - use the mBufRefLock for the DFLT_REGION */
#ifndef SS_DBUF_REFLOCK_DISABLE
   if ( SUnlock(&mp->b_datap->dBufLock) != ROK)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      SSLOGERROR(ERRCLS_DEBUG, ESS344, ERRZERO,
                      "Could not give the Semaphore");
      return;
#endif
   }
#endif

   /*  if we have no more references to this dblock, we can free
    *  the data buffer and the header.
    */

   if (mp->b_datap->db_ref == 0)
   {
      /*  if there is a free routine, we call it for the data buffer;
       *  otherwise, we've allocated the data buffer and so we free it.
       */
#ifndef SS_DBUF_REFLOCK_DISABLE
       SDestroyLock(&mp->b_datap->dBufLock);
#endif
/* ss008.301 */
#ifdef SS_DBLK_FREE_RTN 
      if (mp->b_datap->db_frtnp)
      {
         (*(mp->b_datap->db_frtnp->free_func))(mp->b_datap->db_frtnp->free_arg);
      }
      else
#endif      
      if (mp->b_datap->db_base != NULLP)
      {
/* ss001.301: additions */
#ifdef SS_HISTOGRAM_SUPPORT
         SFree(strmCfg.datRegion, mp->b_datap->db_base,
                  (Size)(mp->b_datap->db_lim - mp->b_datap->db_base), 
						__LINE__, (uint8_t*)__FILE__, ENTNC);
#else
         SFree(strmCfg.datRegion, mp->b_datap->db_base,
                  (Size)(mp->b_datap->db_lim - mp->b_datap->db_base));
#endif /* SS_HISTOGRAM_SUPPORT */
      }


      /*  if the dblock is in this header, we free this header
       *  and we're done. if the dblock is in another header,
       *  we have to free that header too.
       */

      if (mp->b_datap != ((SsDblk *)(mp + sizeof(SsMblk))))
      {
         bp = (SsMblk *)(mp->b_datap - sizeof (SsMblk));
/* ss001.301: additions */
#ifdef SS_HISTOGRAM_SUPPORT
         SFree(strmCfg.mdRegion, (Data *)bp, size, __LINE__, (uint8_t*)__FILE__, ENTNC);
#else
         SFree(strmCfg.mdRegion, (Data *)bp, size);
#endif /* SS_HISTOGRAM_SUPPORT */
      }

/* ss001.301: additions */
#ifdef SS_HISTOGRAM_SUPPORT
      SFree(strmCfg.mdRegion, (Data *)mp, size, __LINE__, (uint8_t*)__FILE__, ENTNC);
#else
      SFree(strmCfg.mdRegion, (Data *)mp, size);
#endif /* SS_HISTOGRAM_SUPPORT */
   }
   else
   {
      /*  reference count is non-zero; if it is this header's
       *  dblock, we don't free the header, we just mark the
       *  dblock pointer empty so we know about it. if it is
       *  another header's dblock, we can free this header.
       */

      if (mp->b_datap == (SsDblk *)(mp + sizeof(SsMblk)))
      {
         mp->b_datap = NULLP;
      }
      else
      {
/* ss001.301: additions */
#ifdef SS_HISTOGRAM_SUPPORT
         SFree(strmCfg.mdRegion, (Data *)mp, size, __LINE__, (uint8_t*)__FILE__, ENTNC);
#else
         SFree(strmCfg.mdRegion, (Data *)mp, size);
#endif /* SS_HISTOGRAM_SUPPORT */
      }
   }

   return;
} /* ssFreeB */


/*
*
*       Fun:   ssFreeMsg
*
*       Desc:  Calls ssFreeB to free all message blocks and their
*              corresponding data blocks for the specified
*              message.
*
*       Ret:   Void
*
*       Notes:
*
*       File:  ss_strm.c
*
*/
Void ssFreeMsg
(
SsMblk *mp                      /* message block */
)
{
   SsMblk *bp;                  /* mblk for iteration */

   /* free all the message blocks in the message */
   while (mp)
   {
      bp = mp;
      mp = mp->b_cont;
      ssFreeB(bp);
   }

   return;
} /* ssFreeMsg */


/*
*
*       Fun:   ssLinkB
*
*       Desc:  Puts the second specified message at the tail of
*              the first specified message.
*
*       Ret:   Void
*
*       Notes:
*
*       File:  ss_strm.c
*
*/
Void ssLinkB
(
SsMblk *mp,                     /* first message block */
SsMblk *bp                      /* second message block */
)
{

#if (ERRCLASS & ERRCLS_INT_PAR)
   if (mp == NULLP  ||  bp == NULLP)
   {
      SSLOGERROR(ERRCLS_INT_PAR, ESS345, ERRZERO, "Null pointer");
      return;
   }
#endif


   /* run down to the end of the message */
   while (mp->b_cont)
   {
      mp = mp->b_cont;
   }


   /* link in the passed mblock */
   mp->b_cont = bp;

   return;
} /* ssLinkB */


/*
*
*       Fun:   ssMsgDSize
*
*       Desc:  Returns the number of bytes of data in the
*              specified message.
*
*       Ret:   S32      - number of bytes
*
*       Notes:
*
*       File:  ss_strm.c
*
*/
S32 ssMsgDSize
(
SsMblk *mp                      /* message block */
)
{
   S32 n;                       /* temporary */
   S32 size;                    /* size of data */

   /* for all blocks that are of type data, count the bytes */
   size = 0;
   while (mp)
   {
      if (mp->b_datap->db_type == SS_M_DATA)
      {
         n = mp->b_wptr - mp->b_rptr;
         if (n > 0)
         {
            size += n;
         }
      }

      mp = mp->b_cont;
   }


   return (size);
} /* ssMsgDSize */


/*
*
*       Fun:   ssPullupMsg
*
*       Desc:  Concatenates and aligns the first 'len' bytes
*              of the specified message into a single, contiguous
*              message block.
*
*       Ret:   1        - success
*              0        - failure
*
*       Notes:
*
*       File:  ss_strm.c
*
*/
S32 ssPullupMsg
(
SsMblk *mp,                     /* message block */
S32 len                         /* number of bytes to align */
)
{
   SsMblk *bp;                  /* mblk for iteration */
   SsMblk *newbp;               /* the new mblk */
   SsMblk *prev;                /* mblk of same type */
   uint8_t *base;                    /* for swapping data buffers */
   uint8_t *lim;                     /* for swapping data buffers */
   uint8_t *rptr;                    /* for swapping data buffers */
   uint8_t *wptr;                    /* for swapping data buffers */
	/* ss008.301 */
#ifdef SS_DBLK_FREE_RTN
   SsFrtn *frtn;                /* for swapping data buffers */
#endif   
   S32 mLen;                    /* number of bytes in all blocks of same type */
   S32 m;                       /* temporary */
   S32 n;                       /* temporary */
   uint8_t type;                     /* message type */

#if (ERRCLASS & ERRCLS_INT_PAR)
   if (mp == NULLP)
   {
      SSLOGERROR(ERRCLS_INT_PAR, ESS346, ERRZERO, "Null pointer");
      return (0);
   }
   
   if (len < -1)
   {
      SSLOGERROR(ERRCLS_INT_PAR, ESS347, len, "Invalid length");
      return (0);
   }
#endif


   /* count the number of bytes in all blocks of the same type */
   bp = mp;
   mLen = 0;
   type = bp->b_datap->db_type;
   while (bp  &&  bp->b_datap->db_type == type)
   {
      n = bp->b_wptr - bp->b_rptr;
      if (n > 0)
      {
         mLen += n;
      }

      bp = bp->b_cont;
   }

   /* if -1 has been passed, we want to pull up all bytes */
   if (len == -1)
   {
      len = mLen;
   }

   if (len == 0)
   {
      return (1);
   }


   /* do we have enough bytes to pull up? */
   if (len < 0  ||  len > mLen)
   {
      return (0);
   }


   /* allocate a new message of the required size */
   newbp = ssAllocB(len, 0);
   if (newbp == NULLP)
   {
      return (0);
   }

   newbp->b_datap->db_type = mp->b_datap->db_type;


   /*  now we copy all the data in the blocks of the same type into
    *  this new message
    */

   prev = NULLP;
   bp = mp;
   while (bp != NULLP)
   {
      /* if this buffer is of a different type, we just skip over it */
      if (bp->b_datap->db_type != newbp->b_datap->db_type)
      {
         prev = bp;
         bp = bp->b_cont;
         continue;
      }


      /* we have to copy this buffer */
      n = bp->b_wptr - bp->b_rptr;
      if (n > 0)
      {
         n = m = MIN(n, len);
         while (m > 0)
         {
            *newbp->b_wptr++ = *bp->b_rptr++;
            m--;
         }

         len -= n;
      }


      /*  the buffer is copied, now--can we free it? if this the
       *  first buffer, we don't free it. also, if this buffer has
       *  something in it, we don't free it. otherwise, we link
       *  around this buffer, from the previous one to the next,
       *  and free this buffer.
       */

      if (prev != NULLP  &&  bp->b_rptr == bp->b_wptr)
      {
         prev->b_cont = bp->b_cont;
         bp->b_cont = NULLP;
         ssFreeB(bp);
         bp = prev->b_cont;
      }
      else
      {
         prev = bp;
      }
   }


   /*  now we have to make the passed mblock point at the new data
    *  buffer we've just filled. then, we have to free the new
    *  mblock we created. also, we have to free the data buffer of
    *  the old mblock, we left it in there. we do this by basically
    *  swapping the data buffers of the passed mblock and the new
    *  mblock, and then freeing the new mblock.
    */

   rptr = mp->b_rptr;
   wptr = mp->b_wptr;
   base = mp->b_datap->db_base;
   lim = mp->b_datap->db_lim;
/* ss008.301 */
#ifdef SS_DBLK_FREE_RTN
   frtn = mp->b_datap->db_frtnp;
#endif   

   mp->b_rptr = newbp->b_rptr;
   mp->b_wptr = newbp->b_wptr;
   mp->b_datap->db_base = newbp->b_datap->db_base;
   mp->b_datap->db_lim = newbp->b_datap->db_lim;
/* ss008.301 */
#ifdef SS_DBLK_FREE_RTN
   mp->b_datap->db_frtnp = NULLP;
#endif  

   newbp->b_rptr = rptr;
   newbp->b_wptr = wptr;
   newbp->b_datap->db_base = base;
   newbp->b_datap->db_lim = lim;
/* ss008.301 */
#ifdef SS_DBLK_FREE_RTN
   newbp->b_datap->db_frtnp = frtn;
#endif   

   ssFreeB(newbp);


   return (1);
} /* ssPullupMsg */


/*
*
*       Fun:   ssRmvB
*
*       Desc:  Removes the specified message block from the
*              specified message and restores the linkage of
*              the remaining blocks in the message.
*
*       Ret:   non-NULL - pointer to the head of the new message
*              -1       - specified message block not found in message
*              NULL     - failure
*
*       Notes: The block removed is not freed.
*
*       File:  ss_strm.c
*
*/
SsMblk *ssRmvB
(
SsMblk *mp,                     /* message */
SsMblk *bp                      /* message block */
)
{
   SsMblk *rp;                  /* mblk that will be returned */

#if (ERRCLASS & ERRCLS_INT_PAR)
   if (mp == NULLP  ||  bp == NULLP)
   {
      SSLOGERROR(ERRCLS_INT_PAR, ESS348, ERRZERO, "Null pointer");
      return (NULLP);
   }
#endif


   /* we will return rp */
   rp = mp;


   /* if the block to be removed is the first one, its easy */
   if (mp == bp)
   {
      rp = mp->b_cont;
   }
   /* otherwise we run through the message, hunting */
   else
   {
      for (; ;)
      {
         if (mp == NULLP)
         {
            return ((SsMblk *)-1);
         }
         else if (mp->b_cont == bp)
         {
            mp->b_cont = bp->b_cont;
            break;
         }
         else
         {
            mp = mp->b_cont;
         }
      }
   }


   /* we've removed bp from the message, clear its next pointer */
   bp->b_cont = NULLP;


   return (rp);
} /* ssRmvB */


/*
*
*       Fun:   ssTestB
*
*       Desc:  Checks for the availability of a message buffer
*              of the specified size without actually retrieving
*              the buffer.
*
*       Ret:   1        - buffer is available
*              0        - not available
*
*       Notes: Is currently a hack that allocates a message and
*              frees it.
*
*       File:  ss_strm.c
*
*/
S32 ssTestB
(
S32 size,                       /* size required */
uint32_t pri                         /* priority of the message buffer */
)
{
   SsMblk *bp;                  /* mblk for iteration */

   bp = ssAllocB(size, pri);
   if (bp)
   {
      ssFreeB(bp);
      return (1);
   }


   return (0);
} /* ssTestB */


/*
*
*       Fun:   ssUnlinkB
*
*       Desc:  Removes the first message block pointed at
*              by the specified message and returns a pointer
*              to the head of the resultant message.
*
*       Ret:   non-NULL - head of the new message
*              NULL     - failure
*
*       Notes:
*
*       File:  ss_strm.c
*
*/
SsMblk *ssUnlinkB
(
SsMblk *mp                      /* message */
)
{
   SsMblk *bp;                  /* mblk for iteration */

#if (ERRCLASS & ERRCLS_INT_PAR)
   if (mp == NULLP)
   {
      SSLOGERROR(ERRCLS_INT_PAR, ESS349, ERRZERO, "Null pointer");
      return (NULLP);
   }
#endif


   /* does this need a comment? ;) */
   bp = mp->b_cont;
   mp->b_cont = NULLP;


   return (bp);
} /* ssUnlinkB */

/**********************************************************************
         End of file
 **********************************************************************/
