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

     Name:     common functions - 5

     Type:     C source file

     Desc:     C source code for common timer routines;

     File:     cm_bdy5.c

*********************************************************************21*/


/* header include files -- defines (.h) */

#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */

#include "gen.h"           /* general */
#include "ssi.h"           /* system services */
#include "cm5.h"           /* common functions */

/* header/extern include files (.x) */

#include "gen.x"           /* general */
#include "ssi.x"           /* system services */
#include "cm5.x"           /* common functions */


/* local defines */

#define CM_RMV_TQCP(tqEnt, tgt)                            \
{                                                          \
   ((tgt)->prev) ? ((tgt)->prev->next = (tgt)->next):      \
                  ((tqEnt)->first = (tgt)->next);          \
   ((tgt)->next) ? ((tgt)->next->prev = (tgt)->prev):      \
                  ((tqEnt)->tail = (tgt)->prev);           \
   (tgt)->prev = NULLP;                                    \
   (tgt)->next = NULLP;                                    \
   (tgt)->ent2bUpd = FALSE;                                \
}
 
#define CM_PLC_TQCP(tqEnt, tgt)                            \
{                                                          \
   if ((tqEnt)->tail)                                      \
   {                                                       \
      (tqEnt)->tail->next = tgt;                           \
      (tgt)->prev = (tqEnt)->tail;                         \
      (tqEnt)->tail = tgt;                                 \
   }                                                       \
   else                                                    \
   {                                                       \
      (tqEnt)->first = (tqEnt)->tail = tgt;                \
   }                                                       \
}

/* local externs */

/* forward references */

/* functions in other modules */


/*
*
*       Fun:   cmPrcTmr
*
*       Desc:  Handle a timer entry
*
*       Ret:   RETVOID
*
*       Notes: 
*
*       File:  cm_bdy5.c
*
*/
 
#ifdef SS_FAP
#ifdef ANSI
PUBLIC Void cmPrcTmr
(
CmTqCp   *tqCp,          /* timing que control point */
CmTqType *tq,            /* timing queue */
PFV      func            /* function */
)
#else
PUBLIC Void cmPrcTmr(tqCp, tq, func)
CmTqCp   *tqCp;          /* connection control block */
CmTqType *tq;            /* message buffer */
PFV      func;           /* function */
#endif
{
/**/
   U32 expire;
   U32 entry;
   S16 event;
   PTR cb;
   CmTimer *tmp1;
   CmTimer **tmp2;
 
   TRC2(cmPrcTmr)

#ifdef CMDBG
{
   DateTime dt;
   Txt prntBuf[PRNTSZE];

   SGetDateTime(&dt);
   /* Here the year was being printed as 2 letter value and hence did
    * not represent accurate information.  It has been modified as
    * recommended to include the offset of year 1900 used in all Trillium
    * SSI implementations. Patch cmbdy5c_001.113
    */
   sprintf(prntBuf,"%s: date: %02d/%02d/%04d time: %02d:%02d:%02d\n",
           msArgv[0],dt.month,dt.day,(int)(dt.year + 1900), dt.hour,
           dt.min, dt.sec);
   SPrint(prntBuf);
}
#endif

   ++tqCp->nxtEnt;
   expire = tqCp->nxtEnt;
   /* cm_bdy5_c_002.113 - Modification for SRegCfgTmr support */
   entry = (U32) (expire % (U32)(tqCp->tmrLen));

   tmp2 = &tq[entry].first;
   while ((tmp1 = *tmp2) != NULLP)
   {
      if (tmp1->tqExpire == expire)
      {
         event = tmp1->tmrEvnt;
         cb = tmp1->cb;
         /* remove and reset this timer control block */
         (*tmp2) = tmp1->next;
         tmp1->tmrEvnt = TMR_NONE;
         tmp1->tqExpire = 0;
         tmp1->cb = NULLP;
         tmp1->next = NULLP;
         func(cb, event);
      }
      else
         tmp2 = &tmp1->next;
   }
   RETVOID;
} /* end of cmPrcTmr */

#else /* not defined SS_FAP */

#ifdef ANSI
PUBLIC Void cmPrcTmr
(
CmTqCp   *tqCp,          /* timing que control point */
CmTqType *tq,            /* timing queue */
PFV      func            /* function */
)
#else
PUBLIC Void cmPrcTmr(tqCp, tq, func)
CmTqCp   *tqCp;          /* connection control block */
CmTqType *tq;            /* message buffer */
PFV      func;           /* function */
#endif
{
/**/
   U32 expire;
   U32 entry, entry1;
   S16 event;
   CmTqType *tqEnt, *tqEnt1; 
   PTR cb;
   CmTimer *tmp1;
   VOLATILE U32     startTime = 0;
   
   TRC2(cmPrcTmr)
 
   /*starting Task*/
   SStartTask(&startTime, PID_CM_PRC_TMR);

#ifdef CMDBG
{
   DateTime dt;
   Txt prntBuf[PRNTSZE];
 
   SGetDateTime(&dt);
   /* Here the year was being printed as 2 letter value and hence did
    * not represent accurate information.  It has been modified as
    * recommended to include the offset of year 1900 used in all Trillium
    * SSI implementations.
    */
   sprintf(prntBuf,"%s: date: %02d/%02d/%04d time: %02d:%02d:%02d\n",
           msArgv[0],dt.month,dt.day,(int)(dt.year + 1900), dt.hour, 
           dt.min, dt.sec);
   SPrint(prntBuf);
}
#endif
 
   ++tqCp->nxtEnt;
   expire = tqCp->nxtEnt;
	tqCp->tmrLen = 1;
   entry = (U32) (expire % (U32)(tqCp->tmrLen));
  
   tqCp->tmp = (tqEnt = &tq[entry])->first;
   while ((tmp1 = tqCp->tmp) != NULLP)
   {
      tqCp->tmp = tmp1->next;
      /* SRIKY - Temp Fix to overcome timer not firing. Need to take */
	  /* care of wrap around case                                                    */
      if ((tmp1->tqExpire <= expire) || (tmp1->ent2bUpd))
      {
         event = tmp1->tmrEvnt;
         cb = tmp1->cb;
 
         if (!(tmp1->ent2bUpd))
         {
            CM_RMV_TQCP(tqEnt, tmp1);
            tmp1->tmrEvnt = TMR_NONE; 
            tmp1->tqExpire = 0;     
            tmp1->cb = NULLP;      
 
            func(cb, event);
         }
         else
         {
            entry1 = (U32) (tmp1->tqExpire % (U32)(tqCp->tmrLen));
            tqEnt1 = &tq[entry1];
            CM_RMV_TQCP(tqEnt, tmp1);
            tmp1->entIdx = entry1; 
            CM_PLC_TQCP(tqEnt1, tmp1);
         }
      }
   }

   /*stoping Task*/
   SStopTask(startTime, PID_CM_PRC_TMR);
   RETVOID;
} /* end of cmPrcTmr */
#endif /* SS_FAP */



/*
*
*       Fun:   cmInitTimers
*
*       Desc:  initialize timers
*
*       Ret:   RETVOID
*
*       Notes: Connection Oriented Control
*
*       File:  cm_bdy5.c
*
*/
#ifdef ANSI
PUBLIC Void cmInitTimers
(
CmTimer *timers,     /* timer list */
U8 max               /* maximum tmrs */
)
#else
PUBLIC Void cmInitTimers(timers, max)
CmTimer *timers;     /* timer list */
U8 max;              /* maximum tmrs */
#endif
{
   CmTimer *tPtr;
   REG1 U8 i;

   TRC2(cmInitTimers)

   for (i = 0, tPtr = timers; i < max; i++, tPtr++)
   {
      tPtr->tmrEvnt = TMR_NONE;
      tPtr->tqExpire = 0;
      tPtr->cb = 0;
      tPtr->next = (struct cmTimer *)NULLP;
      tPtr->prev = (struct cmTimer *)NULLP;
      tPtr->ent2bUpd = FALSE;
   }
   RETVOID;
} /* end of cmInitTimers */

/*
*
*       Fun:    cmPlcCbTq
*
*       Desc:   Places Control Block on Timing Queue
*
*       Ret:    RETVOID
*
*       Notes:  None
*
*       File:   cm_bdy5.c
*
*/
  
#ifdef ANSI
PUBLIC Void cmPlcCbTq
(
CmTmrArg *arg
)
#else
PUBLIC Void cmPlcCbTq(arg)
CmTmrArg *arg;
#endif
{
/*added FAP modifications*/
#ifdef SS_FAP
   REG1 U8 tmrNum;
   /* cm_bdy5_c_001.main_20 - Modification for SRegCfgTmr support */
   U32 ent;
   U32 expire;
   CmTimer **tmp;

   TRC2(cmPlcCbTq)

   expire = (arg->tqCp->nxtEnt + arg->wait);
   /* cm_bdy5_c_002.113 - Modification for SRegCfgTmr support */
   ent = (U32)(expire % (U32)(arg->tqCp->tmrLen));

   for (tmrNum = 0; tmrNum < arg->max; tmrNum++)
   {
      if (arg->timers[tmrNum].tmrEvnt == TMR_NONE)
      {
         arg->timers[tmrNum].tmrEvnt = arg->evnt;
         arg->timers[tmrNum].tqExpire = expire;
         arg->timers[tmrNum].cb = arg->cb;
         arg->timers[tmrNum].next = NULLP;

         tmp = &(arg->tq[ent].first);
         while (*tmp)
            tmp = &((*tmp)->next);
         *tmp = &arg->timers[tmrNum];

         RETVOID;
      }
   }
   RETVOID;
#else
   REG1 U8 tmrNum;
   U32 ent;
   CmTqType *tq;
   CmTimer  *target;
   U32 expire;
   TRC2(cmPlcCbTq)
 
   expire = (arg->tqCp->nxtEnt + arg->wait);
   ent = (U32)(expire % (U32)(arg->tqCp->tmrLen));
 
   for (tmrNum = 0; tmrNum < arg->max; tmrNum++)
   {
      if (arg->timers[tmrNum].tmrEvnt == TMR_NONE)
      {
         target = &arg->timers[tmrNum];
         tq = &arg->tq[ent]; 
 
         target->tmrEvnt = arg->evnt;
         target->tqExpire = expire;
         target->cb = arg->cb;
         target->ent2bUpd = FALSE;
         target->entIdx   = ent;

         /* Place the timer block in the timer list */
         CM_PLC_TQCP(tq, target); 
 
         RETVOID;
      }
   }
   RETVOID;
#endif
} /* end of cmPlcCbTq */
 
/*
*
*       Fun:    cmRstCbTq
*
*       Desc:   Places Control Block on Timing Queue
*
*       Ret:    RETVOID
*
*       Notes:  None
*
*       File:   cm_bdy5.c
*
*/
  
#ifdef ANSI
PUBLIC Void cmRstCbTq
(
CmTmrArg *arg
)
#else
PUBLIC Void cmRstCbTq(arg)
CmTmrArg *arg;
#endif
{
   TRC2(cmRstCbTq)
 
   arg->timers[arg->tNum].tqExpire = arg->tqCp->nxtEnt + arg->wait;
   arg->timers[arg->tNum].ent2bUpd = TRUE; 
 
   RETVOID;
} /* end of cmRstCbTq */

/*
*
*       Fun:    cmRmvCbTq
*
*       Desc:   Removes control block from Timing Queue
*
*       Ret:    RETVOID
*
*       Notes:  None
*
*       File:   cm_bdy5.c
*
*/
  
#ifdef ANSI
PUBLIC Void cmRmvCbTq
(
CmTmrArg *arg
)
#else
PUBLIC Void cmRmvCbTq(arg)
CmTmrArg *arg;
#endif
{
/*Added FAP modifications*/
#ifdef SS_FAP
/* cm_bdy5_c_002.113 - Modification for SRegCfgTmr support */
   U32 ent;
   CmTimer *target;
   CmTimer *tmp1;
   CmTimer **tmp2;

   TRC2(cmRmvCbTq)

   target = &arg->timers[arg->tNum];
   if (target->tmrEvnt != TMR_NONE)
   {
      /* cm_bdy5_c_002.113 - Modification for SRegCfgTmr support */
      ent = (U32) (target->tqExpire % (U32)(arg->tqCp->tmrLen));
      tmp2 = &arg->tq[ent].first;

      while ((tmp1 = *tmp2) != NULLP)
      {
         if (tmp1 == target)
         {
            /* find the timer control block to be removed */
            (*tmp2) = tmp1->next;
            tmp1->tmrEvnt = TMR_NONE;
            tmp1->tqExpire = 0;
            tmp1->cb = NULLP;
            tmp1->next = NULLP;
            break;
         }
         else
            /* find the next timer control block */
            tmp2 = &tmp1->next;
      }
   }
   RETVOID;
#else
   U32 ent;
   CmTimer  *target;
   CmTqType *tq;
   
 
   TRC2(cmRmvCbTq)
 
   target = &arg->timers[arg->tNum];
   if (target->tmrEvnt != TMR_NONE)
   {
      ent = (U32) (target->entIdx);
      tq = &arg->tq[ent];
 
      /* 
       * Update the timer pointer in the for correct processing in
       * CmPrcTmr.
       */
      if (target == arg->tqCp->tmp)
         arg->tqCp->tmp = target->next;
 
      /* Remove the entru from the list */
      CM_RMV_TQCP( tq , target);
      target->tmrEvnt = TMR_NONE;
      target->tqExpire = 0;
      target->cb = NULLP;
 
   }
   RETVOID;
#endif
} /* end of cmRmvCbTq */
 
/**********************************************************************
         End of file
**********************************************************************/
