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
 
     Name:     System Services -- Queueing
 
     Type:     C source file
 
     Desc:     Source code for System Services queuing functions.
 
     File:     ss_queue.c
 
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
#include "ss_task.h"       /* tasking */
#include "ss_strm.h"       /* STREAMS */
#include "ss_msg.h"        /* messaging */
#include "ss_mem.h"        /* memory management interface */
#include "ss_gen.h"        /* general */
#include "cm_mem.h"        /* memory management */
/* ss040.103: addition */
#include <errno.h>

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


  
/*
*
*       Fun:   SInitQueue
*
*       Desc:  This function initializes a queue.
*
*       Ret:   ROK      - ok
*              RFAILED  - failed, general (optional)
*
*       Notes: no assumptions are made about the previous state
*              of the queue.
*
*              queue size is set to zero.
*
*       File:  ss_queue.c
*
*/
S16 SInitQueue
(
Queue *q               /* queue */
)
{

#if (ERRCLASS & ERRCLS_INT_PAR)
   /* check queue pointer */
   if (q == NULLP)
   {
      SSLOGERROR(ERRCLS_INT_PAR, ESS266, ERRZERO, "Null Ptr");
      return RFAILED;
   }
#endif
   q->head     = NULLP;
   q->tail     = NULLP;
   q->crntSize = 0;

   return ROK;

} /* end of SInitQueue */

  
/*
*
*       Fun:   SFlushQueue
*
*       Desc:  This function will release all of the data or message
*              buffers on the specified queue.
*
*       Ret:   ROK      - ok
*              RFAILED  - failed, general (optional)
*
*       Notes: if queue is empty: no action is taken.
*
*              if queue is not empty: all buffers in queue are returned
*              to memory. queue length is set to zero.
*
*              if dequeud buffer is a message buffer, all data buffers
*              associated with the message buffer are returned to memory
*
*       File:  ss_queue.c
*
*/
S16 SFlushQueue
(
Queue *q                    /* queue */
)
{
   Buffer *tBuf;
   Buffer *mBuf;
   SsMsgInfo *minfo;

#ifdef T2K_MEM_LEAK_DBG
   char* file = __FILE__;
   uint32_t line = __LINE__;
#endif


#if (ERRCLASS & ERRCLS_INT_PAR)
   /* check queue */
   if (q == NULLP)
   {
      SSLOGERROR(ERRCLS_INT_PAR, ESS267, ERRZERO, "Null Q Ptr");
      return RFAILED;
   }
#endif

   tBuf = q->head;
   while (tBuf != NULLP)
   {
      mBuf = tBuf->b_next;
      if (tBuf->b_datap->db_type == SS_M_PROTO)
         SPutMsg(tBuf);
      else
      {
         minfo = (SsMsgInfo *) tBuf->b_rptr;
         SPutDBuf(minfo->region, minfo->pool, tBuf);
      }
      tBuf = mBuf;
   }
   q->crntSize = 0;
   q->head     = NULLP;
   q->tail     = NULLP;

   return ROK;

} /* end of SFlushQueue */

  
/*
*
*       Fun:   SCatQueue
*
*       Desc:  This function will concatenate the two specified queues
*              into one queue.
*
*       Ret:   ROK     - ok
*              RFAILED - failed, general (optional)
*
*       Notes: if order equals Q1Q2: all buffers attached to queue 2 are
*              moved to the end of queue 1. queue 2 is set to empty.
*              queue 1 length is increased by length of queue 2. queue
*              2 length is set to zero. return is ok.
*
*              if order equals Q2Q1: all buffers attached to queue 2 are
*              moved to the front of queue 1. queue 2 is set to empty.
*              queue 1 length is increased by length of queue 2. queue
*              2 length is set to zero. return is ok.
*
*       File:  ss_queue.c
*
*/
S16 SCatQueue
(
Queue *q1,                  /* queue 1 */
Queue *q2,                  /* queue 2 */
Order order                 /* order */
)
{
 
#if (ERRCLASS & ERRCLS_INT_PAR)
   if (q1 == NULLP)
   {
      SSLOGERROR(ERRCLS_INT_PAR, ESS268, ERRZERO, "Null Q1 Ptr");
      return RFAILED;
   }
 
   if (q2 == NULLP)
   {
      SSLOGERROR(ERRCLS_INT_PAR, ESS269, ERRZERO, "Null Q2 Ptr");
      return RFAILED;
   }
   
   if ((order != Q1Q2) && (order != Q2Q1))
   {
      SSLOGERROR(ERRCLS_INT_PAR, ESS270, ERRZERO, "Invalid queue order");
      return RFAILED;
   }

   /* ss021.103 - Addition if Q1 is Q2 */
   if (q2 == q1)
   {
      SSLOGERROR(ERRCLS_INT_PAR, ESS271, ERRZERO, "Q1 == Q2");
      return RFAILED;
   }
   
#endif /* ERRCLASS */
   
   if (q1->crntSize == 0)
   {
      q1->head       = q2->head;
      q1->tail       = q2->tail;
      q1->crntSize   = q2->crntSize;

      q2->head       = NULLP;
      q2->tail       = NULLP;
      q2->crntSize   = 0;
      
      return ROK;
   }

   if (q2->crntSize == 0)
   {
      return ROK;
   }
   
   switch (order)
   {
      case Q1Q2:
      {
         q1->tail->b_next = q2->head;
         q2->head->b_prev = q1->tail;
         q1->tail         = q2->tail;

         break;
      }

      case Q2Q1:
      {
         q2->tail->b_next = q1->head;
         q1->head->b_prev = q2->tail;
         q1->head         = q2->head;

         break;
      }
      default:
         return RFAILED;
   }

   q1->crntSize  += q2->crntSize;

   q2->head       = NULLP;
   q2->tail       = NULLP;
   q2->crntSize   = 0;

   return ROK;

} /* end of SCatQueue */


  
/*
*
*       Fun:   SFndLenQueue
*
*       Desc:  This function determines the length of a queue.
*
*       Ret:   ROK      - ok
*              RFAILED  - failed
*
*       Notes: length of queue is determined, queue is unchanged
*              and length is returned via pointer to length.
*              
*       File:  ss_queue.c
*
*/
S16 SFndLenQueue
(
Queue *q,                   /* queue */
QLen  *lngPtr               /* pointer to length */
)
{

#if (ERRCLASS & ERRCLS_INT_PAR)
   /* check queue */
   if (q == NULLP)
   {
      SSLOGERROR(ERRCLS_INT_PAR, ESS272, ERRZERO, "Null Q Ptr");
      return RFAILED;
   }
   /* check length */
   if (lngPtr == NULLP)
   {
      SSLOGERROR(ERRCLS_INT_PAR, ESS273, ERRZERO, "Null Q Len Ptr");
      return RFAILED;
   }
#endif

   *lngPtr = q->crntSize;

   return ROK;

} /* end of SFndLenQueue */


/*
*
*       Fun:   SExamQueue
*
*       Desc:  This function examines the queue at the desired index.
*
*       Ret:   ROK      - ok
*              ROKDNA   - ok, data not available
*              RFAILED  - failed 
*
*       Notes: index is 0 based and indicates location in queue.
*
*              if queue is empty: pointer to buffer is set to null and
*              return is ok, data not available. queue length is unchanged.
*
*              if queue is not empty: pointer to buffer is set to indexed
*              buffer in queue. return is ok. queue length is unchanged.
*
*       File:  ss_queue.c
*
*/
S16 SExamQueue
(
Buffer **bufPtr,            /* pointer to buffer */
Queue  *q,                  /* queue */
QLen   idx                  /* index */
)
{
   Buffer *tmpBuf;
   QLen   i;

 
#if (ERRCLASS & ERRCLS_INT_PAR)
   /* check buffer pointer */
   if (bufPtr == NULLP)
   {
      SSLOGERROR(ERRCLS_INT_PAR, ESS274, ERRZERO, "Null Buf Ptr");
      return RFAILED;
   }
 
   /* check index */
   if ((S32)idx < 0)
   {
      SSLOGERROR(ERRCLS_INT_PAR, ESS275, ERRZERO, "-ve index ");
      return RFAILED;
   }
 
   /* check queue */
   if (q == NULLP)
   {
      SSLOGERROR(ERRCLS_INT_PAR, ESS276, ERRZERO, "Null Q Ptr");
      return RFAILED;
   }
#endif /* ERRCLASS */
 
   if (idx >= q->crntSize)
   {
      *bufPtr = NULLP;
      return (ROKDNA);
   }

   if (idx == 0)
   {
      *bufPtr = q->head;
   }
   else  if (idx == q->crntSize -1)
   {
      *bufPtr = q->tail;
   }
   else 
   {
      tmpBuf = q->head;
      for (i = 0; i < idx; i++)
      {
         tmpBuf = tmpBuf->b_next;
      }
      *bufPtr = tmpBuf;
   }

   return ROK;

} /* end of SExamQueue */


/*
*
*       Fun:   SAddQueue
*
*       Desc:  This function inserts a bufer into the queue before 
*              the desired index.
*
*       Ret:   ROK     - ok
*              RFAILED - failed
*              ROKDNA  - failed - specified index not available
*
*       Notes: index is 0 based and indicates location in queue.
*
*              if queue is empty: buffer is placed in the queue.
*              queue length is incremented.
*
*              if queue is not empty: if index is less than the queue length, 
*              buffer is inserted before the desired index;
*              otherwise, buffer is placed behind all other buffers in queue.
*              queue length is incremented.
*
*       File:  ss_queue.c
*
*/
S16 SAddQueue
(
Buffer *mBuf,                /* buffer */
Queue  *q,                   /* queue */
QLen   idx                   /* index */
)
{
   Buffer *tBuf;
   QLen   i;

 
#if (ERRCLASS & ERRCLS_INT_PAR)
   /* check queue */
   if (q == NULLP)
   {
      SSLOGERROR(ERRCLS_INT_PAR, ESS277, ERRZERO, "Null Q Ptr");
      return RFAILED;
   }
 
   if (mBuf == NULLP)
   {
      SSLOGERROR(ERRCLS_INT_PAR, ESS278, ERRZERO, "Null Buf Ptr");
      return RFAILED;
   }
 
   if ((S32)idx < 0)
   {
      SSLOGERROR(ERRCLS_INT_PAR, ESS279, ERRZERO, "-ve index");
      return RFAILED;
   }
   /* ss021.103 - Addition to check buffer type and if duplicate message */
   if (mBuf->b_datap->db_type != SS_M_PROTO)
   {
      SSLOGERROR(ERRCLS_INT_PAR, ESS280, ERRZERO, 
                 "Incorrect buffer type");
      return RFAILED;
   }
   tBuf = q->head;
   while (tBuf != (Buffer *)NULLP)
   {
      if (tBuf == mBuf)
      {
         SSLOGERROR(ERRCLS_INT_PAR, ESS281, ERRZERO, "Duplicate queued mBuf");
         return RFAILED;
      }
      tBuf = tBuf->b_next;
   }
#endif /* ERRCLASS */

   if (idx > q->crntSize)
   {
      SSLOGERROR(ERRCLS_DEBUG, ESS282, ERRZERO, "Invalid index");
      return (ROKDNA);
   }
   else if (q->crntSize == 0)
   {
      q->head = mBuf;
      q->tail = mBuf;

      mBuf->b_next = NULLP;
      mBuf->b_prev = NULLP;
   }
   else if (idx == 0)
   {
      mBuf->b_next     = q->head;
      mBuf->b_prev     = NULLP;
      q->head->b_prev  = mBuf;
      q->head          = mBuf;
   }
   else if (idx == q->crntSize)
   {
      mBuf->b_prev    = q->tail;
      mBuf->b_next    = NULLP;
      q->tail->b_next = mBuf;
      q->tail         = mBuf;
   }
   else
   {
      tBuf = q->head;
      for (i = 0; i < idx; i++)
      {
         tBuf = tBuf->b_next;
      }
    
      tBuf->b_prev->b_next = mBuf;
      mBuf->b_prev         = tBuf->b_prev;
      mBuf->b_next         = tBuf;
      tBuf->b_prev         = mBuf;
   }
   q->crntSize++;
   return ROK;

} /* end of SAddQueue */


/*
*
*       Fun:   SRemQueue
*
*       Desc:  This function removes a buffer from the queue at 
*              the desired index.
*
*       Ret:   ROK      - ok
*              ROKDNA   - ok, data not available
*              RFAILED  - failed
*
*       Notes: index is 0 based and indicates location in queue.
*
*              if queue is empty: pointer to buffer is set to null and
*              return is ok, data not available. queue length is unchanged.
*
*              if queue is not empty: pointer to buffer is set to indexed
*              buffer in queue. return is ok. queue length is decremented.
*
*       File:  ss_queue.c
*
*/
S16 SRemQueue
(
Buffer **bufPtr,            /* pointer to buffer */
Queue  *q,                  /* queue */
QLen   idx                  /* index */
)
{
   Buffer *tBuf;
   QLen   i;

 
#if (ERRCLASS & ERRCLS_INT_PAR)
   /* check buffer pointer */
   if (bufPtr == NULLP)
   {
      SSLOGERROR(ERRCLS_INT_PAR, ESS283, ERRZERO, "Null Buf Ptr");
      return RFAILED;
   }
 
   /* check queue */
   if (q == NULLP)
   {
      SSLOGERROR(ERRCLS_INT_PAR, ESS284, ERRZERO, "Null Q Ptr");
      return RFAILED;
   }
 
   if ((S32)idx < 0)
   {
      SSLOGERROR(ERRCLS_INT_PAR, ESS285, ERRZERO, "-ve Index");
      return RFAILED;
   }      
#endif /* ERRCLASS */   
 
   if (idx >= q->crntSize)
   {
      *bufPtr = NULLP;
      return (ROKDNA);
   }
   if (idx == 0)
   {
      *bufPtr = q->head;
      if (q->crntSize == 1)
      {
         q->head = NULLP;
         q->tail = NULLP;
      }
      else
      {
         q->head         = q->head->b_next;
         q->head->b_prev = NULLP;
      }
   }
   else if (idx == q->crntSize -1)
   {
      *bufPtr = q->tail;
      q->tail = q->tail->b_prev;
      q->tail->b_next = NULLP;
   }
   else 
   {
      tBuf = q->head;

      for (i = 0; i < idx; i++)
      {
         tBuf = tBuf->b_next;
      }
      *bufPtr = tBuf;
      
      tBuf->b_prev->b_next = tBuf->b_next;
      tBuf->b_next->b_prev = tBuf->b_prev;
   }
   q->crntSize--;

   return ROK;

} /* end of SRemQueue */


#ifndef SS_ENABLE_MACROS


/*
*
*       Fun:   SQueueFirst
*
*       Desc:  This function queues a data or message buffer to the
*              front of the specified queue.
*
*       Ret:   ROK     - ok
*              RFAILED - failed, general (optional)
*
*       Notes: if queue is empty: buffer is placed in the queue. queue
*              length is incremented.
*              
*              if queue is not empty: buffer is placed in front of all
*              other buffers in queue. queue length is incremented.
*
*       File:  ss_queue.c
*
*/
inline S16 SQueueFirst
(
Buffer *buf,                /* buffer */
Queue *q                    /* queue */
)
{

   return (SAddQueue(buf, q, 0));
} /* end of SQueueFirst */

  
/*
*
*       Fun:   SDequeueFirst
*
*       Desc:  This function dequeues a data or message buffer from
*              the front of the specified queue.
*
*       Ret:   ROK      - ok
*              ROKDNA   - ok, data not available
*              RFAILED  - failed, general (optional)
*
*       Notes: if queue is empty: pointer to buffer is set to null and
*              return is ok, data not available. queue length is unchanged.
*              
*              if queue is not empty: pointer to buffer is set to first
*              buffer in queue, first buffer in queue is removed and
*              return is ok. queue length is decremented.
*
*       File:  ss_queue.c
*
*/
inline S16 SDequeueFirst
(
Buffer **bufPtr,            /* pointer to buffer */
Queue *q                    /* queue */
)
{

   return (SRemQueue(bufPtr, q, 0));
} /* end of SDequeueFirst */

  
/*
*
*       Fun:   SQueueLast
*
*       Desc:  This function queues a data or message buffer to the
*              back of the specified queue.
*
*       Ret:   ROK      - ok
*              RFAILED  - failed, general (optional)
*
*       Notes: if queue is empty: buffer is placed in the queue.
*              queue length is incremented.
*              
*              if queue is not empty: buffer is placed behind all
*              other buffers in queue. queue length is incremented.
*
*       File:  ss_queue.c
*
*/
S16 SQueueLast
(
Buffer *buf,                /* buffer */
Queue *q                    /* queue */
)
{

#if (ERRCLASS & ERRCLS_INT_PAR)
   /* check queue */
   if (q == NULLP)
   {
      SSLOGERROR(ERRCLS_INT_PAR, ESS286, ERRZERO, "Null Q Ptr");
      return RFAILED;
   }
   /* check queue */
   if (buf == NULLP)
   {
      SSLOGERROR(ERRCLS_INT_PAR, ESS287, ERRZERO, "Null Buf Ptr");
      return RFAILED;
   }
#endif
   return (SAddQueue(buf, (q), ((q)->crntSize)));
}


  
/*
*
*       Fun:   SDequeueLast
*
*       Desc:  This function dequeues a data or message buffer from the
*              back of the specified queue.
*
*       Ret:   ROK     - ok
*              ROKDNA  - ok, data not available
*              RFAILED - failed, general (optional)
*
*       Notes: if queue is empty: pointer to buffer is set to null and
*              return is ok, data not available. queue length is unchanged.
*              
*              if queue is not empty: pointer to buffer is set to last
*              buffer in queue, last buffer in queue is removed and
*              return is ok. queue length is decremented.
*
*       File:  ss_queue.c
*
*/
S16 SDequeueLast
(
Buffer **bufPtr,            /* pointer to buffer */
Queue *q                    /* queue */
)
{
   S16   ret;


#if (ERRCLASS & ERRCLS_INT_PAR)
   /* check buffer pointer */
   if (!bufPtr)
   {
      SSLOGERROR(ERRCLS_INT_PAR, ESS288, ERRZERO, "Null Buf Ptr");
      return RFAILED;
   }
   /* check queue */
   if (!q)
   {
      SSLOGERROR(ERRCLS_INT_PAR, ESS289, ERRZERO, "Null Q Ptr");
      return RFAILED;
   }
#endif
   if(q->crntSize > 0)
      ret = SRemQueue(bufPtr, q, q->crntSize-1);
   else
      ret = SRemQueue(bufPtr, q, q->crntSize);

   return (ret);
}

#endif /* SS_ENABLE_MACROS */


/*
*
*       Fun:   ssInitDmndQ
*
*       Desc:  This function initializes a Demand Queue
*
*       Ret:   ROK      - ok
*              RFAILED  - failed
*
*       Notes: 
*
*       File:  ss_queue.c
*
*/
S16 ssInitDmndQ
(
SsDmndQ *dQueue                 /* Demand Queue */
)
{
   uint8_t  i;
   S16 ret;


#if (ERRCLASS & ERRCLS_INT_PAR)
   if (dQueue == NULLP)
   {
      SSLOGERROR(ERRCLS_INT_PAR, ESS290, ERRZERO, "NULL DQ Pointer");
      return RFAILED;
   }
#endif

   for (i = 0; i < SS_MAX_NUM_DQ; i++)
   {
      dQueue->queue[i].head     = NULLP;
      dQueue->queue[i].tail     = NULLP;
      dQueue->queue[i].crntSize = 0;
   }

#ifndef TENB_RTLIN_CHANGES
   for (i = 0; i < SS_DQ_BIT_MASK_LEN; i++)
#else
   for (i = 0; i < SS_MAX_NUM_DQ; i++)
#endif
   {
#ifndef TENB_RTLIN_CHANGES
      dQueue->bitMask[i] =  0;
#endif
      /* ss039.103 : Replaced SInitLock with WTInitLock */
#ifdef SS_WIN
      ret = WTInitLock(&dQueue->dmndQLock[i], SS_DMNDQ_LOCK);
#else
      ret = SInitLock(&dQueue->dmndQLock[i], SS_DMNDQ_LOCK);
#endif
      if (ret != ROK)
      {
#if (ERRCLASS & ERRCLS_DEBUG)
         SSLOGERROR(ERRCLS_DEBUG, ESS291, (ErrVal)ret,
                                   "Failed to initialize lock");
#endif
         return RFAILED;
      }
   }

   /* initialize the semaphore */
   ret = ssInitSema(&dQueue->dmndQSema, 0);
   if (ret != ROK)
   {
#ifndef TENB_RTLIN_CHANGES
      for (i = 0; i < SS_DQ_BIT_MASK_LEN; i++)
#else
      for (i = 0; i < SS_MAX_NUM_DQ; i++)
#endif
      {
         /* ss039.103 : Replaced SDestroyLock with WTDestroyLock */
#ifdef SS_WIN
         WTDestroyLock(&dQueue->dmndQLock[i]);
#else
         SDestroyLock(&dQueue->dmndQLock[i]);
#endif
      }
#if (ERRCLASS & ERRCLS_DEBUG)
      SSLOGERROR(ERRCLS_DEBUG, ESS292, (ErrVal)ret, 
                                   "Failed to init semaphore");
#endif
      return RFAILED;
   }
   return  (ROK);

} /* End of ssInitDmndQ */


/*
*
*       Fun:   ssDestroyDmndQ
*
*       Desc:  This function destroys a Demand Queue by releasing all the
*              queued messages and detroying all the associated locks
*
*       Ret:   ROK      - ok
*              RFAILED  - failed, general (optional)
*
*       Notes: 
*
*       File:  ss_queue.c
*
*/
S16 ssDestroyDmndQ
(
SsDmndQ *dQueue                        /* demand Queue */
)
{
   uint8_t     i;
   Buffer *tBuf;
   S16    ret;


#if (ERRCLASS & ERRCLS_INT_PAR)
   if (dQueue == NULLP)
   {
      SSLOGERROR(ERRCLS_INT_PAR, ESS293, ERRZERO, "NULL DQ Pointer");
      return RFAILED;
   }
#endif

#ifndef TENB_RTLIN_CHANGES
      for (i = 0; i < SS_DQ_BIT_MASK_LEN; i++)
#else
      for (i = 0; i < SS_MAX_NUM_DQ; i++)
#endif
   {
      /* ss039.103 : Replaced SDestroyLock with WTDestroyLock */
#ifdef SS_WIN
      ret = WTDestroyLock(&dQueue->dmndQLock[i]);
#else
      ret = SDestroyLock(&dQueue->dmndQLock[i]);
#endif
      if (ret != ROK)
      {
#if (ERRCLASS & ERRCLS_DEBUG)
         SSLOGERROR(ERRCLS_DEBUG, ESS294, (ErrVal)ret, "Failed to destroy lock");
#endif
         return RFAILED;
      }
   }
   for (i = 0; i < SS_MAX_NUM_DQ; i++)
   {
      while (dQueue->queue[i].head != NULLP)
      {
         tBuf = dQueue->queue[i].head;
         dQueue->queue[i].head = dQueue->queue[i].head->b_next;
         SPutMsg(tBuf);
      }
   }

/* ss06.13:addition */
   if( ssDestroySema(&dQueue->dmndQSema) != ROK)
   {
      SSLOGERROR(ERRCLS_DEBUG, ESS295, ERRZERO,
                         "Could not delete the Semaphore");
      return RFAILED;

   }
   return  (ROK);

} /* end of ssDestroyDmndQ */


/*
*
*       Fun:   ssDmndQPut
*
*       Desc:  This function adds a message to the head or tail of the 
*              priority queue specified. The priority specified is the 
*              destination Q index i.e 
*              ((dst_Tsk_pri * SS_MAX_MSG_PRI) + msg_pri)
*
*       Ret:   ROK      - ok
*              RFAILED  - failed
*
*       Notes: 
*
*       File:  ss_queue.c
*
*/
S16 ssDmndQPut
(
SsDmndQ *dQueue,                       /* demand Queue */
Buffer  *mBuf,                         /* message buffer */
Prior   priority,                      /* priority */
Order   order                          /* position */
)
{
#ifndef TENB_RTLIN_CHANGES
   uint8_t     maskIndex;                   /* mask Index */
   uint8_t     bitPosition;                 /* bit position in index */
#else
   uint8_t    qIndex;
#endif
   Queue *queue;                       /* queue in demand queue */
   S16    ret;                         /* return value */
#ifdef SS_PERF
   int    value;
   uint32_t    size;
#endif
#ifdef MSPD_MLOG_NEW 
   uint32_t    t = MacGetTick();
#endif 


#if (ERRCLASS & ERRCLS_INT_PAR)
   if (dQueue == NULLP)
   {
      SSLOGERROR(ERRCLS_INT_PAR, ESS296, ERRZERO, "NULL DQ Pointer");
      return RFAILED;
   }

   if (mBuf == NULLP)
   {
      SSLOGERROR(ERRCLS_INT_PAR, ESS297, ERRZERO, "NULL mBuf Pointer");
      return RFAILED;
   }

   if ((priority == PRIORNC) || (priority > SS_MAX_DQ_PRIOR))
   {
      SSLOGERROR(ERRCLS_INT_PAR, ESS298, ERRZERO, "invalid priority ");
      return RFAILED;
   }

   if ((order != SS_DQ_FIRST) && (order != SS_DQ_LAST))
   {
      SSLOGERROR(ERRCLS_INT_PAR, ESS299, ERRZERO, "invalid order ");
      return RFAILED;
   }
#endif
   
#ifndef TENB_RTLIN_CHANGES
   maskIndex   = priority >> 3;
   bitPosition = 7 - (priority % 8);
   queue       = &dQueue->queue[priority];
#else
   qIndex      = priority;
   queue       = &dQueue->queue[qIndex];
#endif

   /* ss039.103 : Replaced SLock with WTLock */
#ifdef SS_WIN
#ifndef TENB_RTLIN_CHANGES
   ret = WTLock(&dQueue->dmndQLock[maskIndex]);
#else
   ret = WTLock(&dQueue->dmndQLock[qIndex]);
#endif
#else
#ifndef TENB_RTLIN_CHANGES
   ret = SLock(&dQueue->dmndQLock[maskIndex]);
#else
   ret = SLock(&dQueue->dmndQLock[qIndex]);
#endif
#endif
   if (ret != ROK)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      SSLOGERROR(ERRCLS_DEBUG, ESS300, (ErrVal)ret, "Failed to get lock");
#endif
      return  (RFAILED);
   }

   if (queue->crntSize == 0)
   {
      queue->head   = mBuf;
      queue->tail   = mBuf;
      mBuf->b_next  = NULLP;
      mBuf->b_prev  = NULLP;

#ifndef TENB_RTLIN_CHANGES
      /* Set the corresponding bit in bit mask */
      dQueue->bitMask[maskIndex] |= (1 << bitPosition);
#endif
   }
   else
   {
      if (order == SS_DQ_LAST)
      {
         mBuf->b_prev        = queue->tail;
         mBuf->b_next        = NULLP;
         queue->tail->b_next = mBuf;
         queue->tail         = mBuf;
      }
      else
      {
         mBuf->b_next        = queue->head;
         mBuf->b_prev        = NULLP;
         queue->head->b_prev = mBuf;
         queue->head         = mBuf;
      }
   }
   queue->crntSize++;
#ifdef SS_PERF
   size = queue->crntSize;
#endif

   /* ss039.103 : Replaced SUnlock with WTUnlock */
#ifdef SS_WIN
#ifndef TENB_RTLIN_CHANGES
   ret = WTUnlock(&dQueue->dmndQLock[maskIndex]);
#else
   ret = WTUnlock(&dQueue->dmndQLock[qIndex]);
#endif
#else
#ifndef TENB_RTLIN_CHANGES
   ret = SUnlock(&dQueue->dmndQLock[maskIndex]);
#else
   ret = SUnlock(&dQueue->dmndQLock[qIndex]);
#endif
#endif
   if (ret != ROK)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      SSLOGERROR(ERRCLS_DEBUG, ESS301, (ErrVal)ret, "Failed to release lock");
#endif
      /* ss035.103 */
      if (order == SS_DQ_LAST)
      {
         SDequeueLast(&mBuf, queue);
      }
      else
      {
         SDequeueFirst(&mBuf, queue);
      }
      return  (RFAILED);
   }

   /* increment the counting semaphore */
   /* ss006.13: addition */

/* ss037.103 for Performance enhancement : this is to ensure that semaphore is posted every time the first message is posted to the queue so that permanent tick is picked */
#ifdef SS_PERF
  if (size > 1)
  {
     sem_getvalue(&dQueue->dmndQSema, &value);
     if (value > 0)
           return ROK;
  }
#endif
   if (ssPostSema(&dQueue->dmndQSema) != ROK)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
       SSLOGERROR(ERRCLS_DEBUG, ESS302, ERRZERO,
                         "Could not unlock the Semaphore");
       /* ss035.103 */
       if (order == SS_DQ_LAST)
       {
          SDequeueLast(&mBuf, queue);
       }
       else
       {
          SDequeueFirst(&mBuf, queue);
       } 
       return RFAILED;
#endif
   }
   return ROK;

} /* End of ssDmndQPut */


/*
*
*       Fun:   ssDmndQWait
*
*       Desc:  This function removes a message from head or tail of the 
*              highest non-empty priority queue message. 
*
*       Ret:   ROK      - ok
*              RFAILED  - failed
*              ROKDNA   - ok, no data available in queue
*
*       Notes:  This is a blocking call
*
*       File:  ss_queue.c
*
*/
S16 ssDmndQWait
(
SsDmndQ *dQueue                          /* demand queue */
)
{
   S16   ret;


#if (ERRCLASS & ERRCLS_INT_PAR)
   if (dQueue == NULLP)
   {
      SSLOGERROR(ERRCLS_INT_PAR, ESS303, ERRZERO, "NULL DQ Pointer");
      return RFAILED;
   }

#endif

   /* ss040.103 updating for possible non-fatal retur from sem_wait */
   while ((ret = ssWaitSema(&dQueue->dmndQSema) != ROK) && (errno == EINTR))
      continue;
   if (ret != ROK)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      SSLOGERROR(ERRCLS_DEBUG, ESS306, (ErrVal)ret, "Failed to get semaphore");
#endif
      return  (RFAILED);
   }

   return  (ROK);
} /* End of ssDmndQWait */



/*
*
*       Fun:   ssDmndQGet
*
*       Desc:  This function removes a message from head or tail of the 
*              highest non-empty priority queue message. 
*
*       Ret:   ROK      - ok
*              RFAILED  - failed
*              ROKDNA   - ok, no data available in queue
*
*       Notes:  This is a blocking call
*
*       File:  ss_queue.c
*
*/
S16 ssDmndQGet
(
SsDmndQ *dQueue,                          /* demand queue */
Buffer  **mBuf,                           /* message buffer */
Order   order                             /* position */ 
)
{
   Queue *queue;
   S16   ret;
   S16   i;
#ifndef TENB_RTLIN_CHANGES
   uint8_t    bitPosition;
   uint8_t    qIndex;
#endif


#if (ERRCLASS & ERRCLS_INT_PAR)
   if (mBuf == NULLP)
   {
      SSLOGERROR(ERRCLS_INT_PAR, ESS304, ERRZERO, "NULL mBuf Pointer");
      return RFAILED;
   }

   if ((order != SS_DQ_FIRST) && (order != SS_DQ_LAST))
   {
      SSLOGERROR(ERRCLS_INT_PAR, ESS305, ERRZERO, "invalid order ");
      return RFAILED;
   }
#endif

#ifndef TENB_RTLIN_CHANGES
   for (i = 0; i < SS_DQ_BIT_MASK_LEN; i++)
   {
      /* ss039.103 : Replaced SLock with WTLock */
#ifdef SS_WIN
      ret = WTLock(&dQueue->dmndQLock[i]);
#else
      ret = SLock(&dQueue->dmndQLock[i]);
#endif
      if (ret != ROK)
      {
#if (ERRCLASS & ERRCLS_DEBUG)
         SSLOGERROR(ERRCLS_DEBUG, ESS307, (ErrVal)ret, "Failed to get lock");
#endif
         return  (RFAILED);
      }

      bitPosition = osCp.dmndQLookupTbl[dQueue->bitMask[i]];
      if (bitPosition != 255)
         break;

      /* ss039.103 : Replaced SUnlock with WTUnlock */
#ifdef SS_WIN
      ret = WTUnlock(&dQueue->dmndQLock[i]);
#else
      ret = SUnlock(&dQueue->dmndQLock[i]);
#endif
      if (ret != ROK)
      {
#if (ERRCLASS & ERRCLS_DEBUG)
         SSLOGERROR(ERRCLS_DEBUG, ESS308, ret, "Failed to release lock");
#endif
         return  (RFAILED);
      }
   }
   if (i >= SS_DQ_BIT_MASK_LEN)
   {
      /* Demand Queue is empty */
      *mBuf = NULLP;
      return  (ROKDNA);
   }
   
   qIndex = (i * 8) +  (7 - bitPosition);
   queue = &dQueue->queue[qIndex];

#else
   for (i = 0; i < SS_MAX_NUM_DQ; i++)
   {
      queue = &dQueue->queue[i];
      if (queue->crntSize)
         break;
   }

   if (i >= SS_MAX_NUM_DQ)
   {
      /* Demand Queue is empty */
      *mBuf = NULLP;
      return  (ROKDNA);
   }

   /* ss039.103 : Replaced SLock with WTLock */
#ifdef SS_WIN
   ret = WTLock(&dQueue->dmndQLock[i]);
#else
   ret = SLock(&dQueue->dmndQLock[i]);
#endif
   if (ret != ROK)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      SSLOGERROR(ERRCLS_DEBUG, ESS307, (ErrVal)ret, "Failed to get lock");
#endif
      return  (RFAILED);
   }

#endif
/* ss037.103 For performance enhancement replace the check sequence with simple 
setting the crntSize to 0 and removing the message */
#ifndef SS_PERF
   if (queue->crntSize == 1)
   {
      *mBuf = queue->head;
      queue->head = NULLP;
      queue->tail = NULLP;

      /* Reset the corresponding bit in bit mask */
      dQueue->bitMask[i] &= (~( 1 << (bitPosition)));
   }
   else
   {
      if (order == SS_DQ_FIRST)
      {
         *mBuf = queue->head;
         queue->head = queue->head->b_next;
         queue->head->b_prev = NULLP;
      }
      else
      {
         *mBuf = queue->tail;
         queue->tail = queue->tail->b_prev;
         queue->tail->b_next = NULLP;
      }
   }
   queue->crntSize--;
#else
   queue->crntSize = 0;
   *mBuf = queue->head;
   queue->head = NULLP;
   queue->tail = NULLP;
       
#endif

   /* ss039.103 : Replaced SUnlock with WTUnlock */
#ifdef SS_WIN
   ret = WTUnlock(&dQueue->dmndQLock[i]); 
#else
   ret = SUnlock(&dQueue->dmndQLock[i]); 
#endif
   if (ret != ROK)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      SSLOGERROR(ERRCLS_DEBUG, ESS309, (ErrVal)ret, "Failed to release lock");
#endif
      return  (RFAILED);
   }

   return  (ROK);

} /* End of ssDmndQGet */


/*
*
*       Fun:   ssFndLenDmndQ 
*
*       Desc:  This function returns the number of messages in a queue
*              If priority is specified, length of the associated Q is 
*              returned otherwise total length of all Qs is returned.
*
*       Ret:   ROK      - ok
*              RFAILED  - failed
*
*       Notes: 
*
*       File:  ss_queue.c
*
*/
S16 ssFndLenDmndQ
(
SsDmndQ *dQueue,                               /* demand queue */
Prior   priority,                              /* priority */
QLen    *len                                   /* queue length */
)
{
   
   S16  ret;                                   /* return value */
   uint8_t   i;


#if (ERRCLASS & ERRCLS_INT_PAR)
   if ((dQueue == NULLP) || (len == NULLP))
   {
      SSLOGERROR(ERRCLS_INT_PAR, ESS310, ERRZERO, "NULL Pointer");
      return RFAILED;
   }
#endif

   *len = 0;
   if (priority != PRIORNC)
   {
      i = priority >> 3; 
      /* ss039.103 : Replaced SLock with WTLock */
#ifdef SS_WIN
      ret = WTLock(&dQueue->dmndQLock[i]);
#else
      ret = SLock(&dQueue->dmndQLock[i]);
#endif
      if (ret != ROK)
      {
#if (ERRCLASS & ERRCLS_DEBUG)
         SSLOGERROR(ERRCLS_DEBUG, ESS311, (ErrVal)ret, "Failed to get lock");
#endif
         return  (RFAILED);
      }

      *len = dQueue->queue[priority].crntSize;

      /* ss039.103 : Replaced SUnlock with WTUnlock */
#ifdef SS_WIN
      ret = WTUnlock(&dQueue->dmndQLock[i]);
#else
      ret = SUnlock(&dQueue->dmndQLock[i]);
#endif
      if (ret != ROK)
      {
#if (ERRCLASS & ERRCLS_DEBUG)
         SSLOGERROR(ERRCLS_DEBUG, ESS312, (ErrVal)ret,  \
                                         "Failed to release lock");
#endif
         return  (RFAILED);
      }
   }
   else
   {
#ifndef TENB_RTLIN_CHANGES
      for (i = 0; i < SS_DQ_BIT_MASK_LEN; i++)
#else
      for (i = 0; i < SS_MAX_NUM_DQ; i++)
#endif
      {
         /* ss039.103 : Replaced SLock with WTLock */
#ifdef SS_WIN
         ret = WTLock(&dQueue->dmndQLock[i]);
#else
         ret = SLock(&dQueue->dmndQLock[i]);
#endif
         if (ret != ROK)
         {
#if (ERRCLASS & ERRCLS_DEBUG)
            SSLOGERROR(ERRCLS_DEBUG, ESS313, (ErrVal)ret, "Failed to get lock");
#endif
            /* Release all the locks aquired */
            while (i > 0)
            {

/* ss006.13: addition */
                /* ss039.103 : Replaced SUnlock with WTUnlock */
#ifdef SS_WIN
                if( WTUnlock(&dQueue->dmndQLock[i-1]) != ROK)
#else
                if( SUnlock(&dQueue->dmndQLock[i-1]) != ROK)
#endif
                {
#if (ERRCLASS & ERRCLS_DEBUG)
                   SSLOGERROR(ERRCLS_DEBUG, ESS314, ERRZERO,
                         "Could not give the Semaphore");
                   return RFAILED;
#endif
                }

               i--;
            }

            return  (RFAILED);
         }
      }

      for (i = 0; i  < SS_MAX_NUM_DQ; i++)
         *len  += dQueue->queue[i].crntSize;

#ifndef TENB_RTLIN_CHANGES
      for (i = 0; i < SS_DQ_BIT_MASK_LEN; i++)
#else
      for ( i = 0; i < SS_MAX_NUM_DQ; i++)
#endif
      {
         /* ss039.103 : Replaced SUnlock with WTUnlock */ 
#ifdef SS_WIN
         ret = WTUnlock(&dQueue->dmndQLock[i]);
#else
         ret = SUnlock(&dQueue->dmndQLock[i]);
#endif
         if (ret != ROK)
         {
#if (ERRCLASS & ERRCLS_DEBUG)
            SSLOGERROR(ERRCLS_DEBUG, ESS315, (ErrVal)ret, "Failed to get lock");
#endif
            return  (RFAILED);
         }
      }
   }
   return ROK;

} /* End of ssFndLenDmndQ */

/**********************************************************************
         End of file
**********************************************************************/
