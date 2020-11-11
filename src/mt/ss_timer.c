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
 
     Name:     System Services -- Timing
 
     Type:     C source file
 
     Desc:     Source code for System Services related to timing.
 
     File:     ss_timer.c
 
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


/* Forward declarations */

/* ss041.103 Declaration for STmrRegHndlr */
/* ss015.301 Changed the timer activation function type as all timer activation
 * functions are enclosed in a union. 
 */
#ifndef SS_MULTIPLE_PROCS
static S16 STmrRegHndlr ARGS((
         Ent ent,
         Inst inst,
         S16 period,
         S16 units,
         SsTmrActvFn ssTmrActvFn
      ));
/* ss015.301: Removed the timer handler prototypes guarded under SS_MT_TMR and 
 * handled mtFlag in existing timer handlers. 
 */
#else /* SS_MULTIPLE_PROCS */
static S16 STmrRegHndlr ARGS((
         ProcId proc,
         Ent ent,
         Inst inst,
         S16 period,
         S16 units,
         SsTmrActvFn ssTmrActvFn
      ));
#endif /* SS_MULTIPLE_PROCS */

/* ss041.103 Declaration for STmrDeregHndlr */
#ifndef SS_MULTIPLE_PROCS
static S16 STmrDeregHndlr ARGS((
         Ent ent,
         Inst inst,
         S16 period,
         S16 units,
         SsTmrActvFn ssTmrActvFn
      ));
/* ss015.301 Removed the timer handler prototypes guarded under SS_MT_TMR and 
 * handled mtFlag in existing timer handlers.
 */
#else /* SS_MULTIPLE_PROCS */
static S16 STmrDeregHndlr ARGS((
         ProcId proc,
         Ent ent,
         Inst inst,
         S16 period,
         S16 units,
         SsTmrActvFn ssTmrActvFn
      ));
#endif /* SS_MULTIPLE_PROCS */



/* ss028.103 - Modification for SRegCfgTmr Support */
/*
*
*       Fun:   Register Configurable Timer Task - timer
*
*       Desc:  This function is used to register a timer function for the
*              layer. The system services will periodically invoke the
*              function passed to it. The timer function will be used by the
*              layer to manage the layers internal protocol timers.
*
*       Ret:   ROK      - ok
*              RFAILED  - failed, general (optional)
*              ROUTRES  - failed, out of resources (optional)
*
*       Notes:
*
*       File:  ss_timer.c
*
*/
/* ss029.103: addition: procId added and timer function type modified */ 
#ifndef SS_MULTIPLE_PROCS
S16 SRegCfgTmr
(
Ent ent,                    /* entity */
Inst inst,                  /* instance */
S16 period,                 /* period */
S16 units,                  /* period units */
PFS16 tmrFnct               /* timer function, typically SActvTmr */
)
{
   S16 ret;
   /* ss015.301 - Enclosed all timer activation functions in a union. */
   SsTmrActvFn ssTmrActvFn;
   /* ss041.103 - Moved handling to STmrRegHndlr */

   ssTmrActvFn.mtFlag = FALSE;
   ssTmrActvFn.actvFnc.tmrActvFn = tmrFnct;
   ret = STmrRegHndlr(ent, inst, period, units, ssTmrActvFn);

   return (ret);
}

#else /* SS_MULTIPLE_PROCS */

S16 SRegCfgTmr
(
ProcId proc,                /* processor */
Ent ent,                    /* entity */
Inst inst,                  /* instance */
S16 period,                 /* period */
S16 units,                  /* period units */
PAIFTMRS16 tmrFnct               /* timer function, typically SActvTmr */
)
{
   S16 ret;
   /* ss015.301 - Enclosed all timer activation functions in a union. */
   SsTmrActvFn ssTmrActvFn;
   /* ss041.103 - Moved handling to STmrRegHndlr */

   ssTmrActvFn.mtFlag = FALSE;
   ssTmrActvFn.actvFnc.tmrActvFn = tmrFnct;
   ret = STmrRegHndlr(proc, ent, inst, period, units, ssTmrActvFn);

   return (ret);
}

#endif /* SS_MULTIPLE_PROCS */


#ifndef SS_MULTIPLE_PROCS
/* ss041.103 - New function for MT timer reg */
#ifdef SS_MT_TMR
/*
*
*       Fun:   Register Configurable Timer Task - timer
*
*       Desc:  This function is used to register a timer function for the
*              layer. The system services will periodically invoke the
*              function passed to it. The timer function will be used by the
*              layer to manage the layers internal protocol timers.
*
*       Ret:   ROK      - ok
*              RFAILED  - failed, general (optional)
*              ROUTRES  - failed, out of resources (optional)
*
*       Notes: By invoking this function, Ent and Inst are passed in the
*              timer activation function.
*
*       File:  ss_timer.c
*
*/
S16 SRegCfgTmrMt
(
Ent ent,                    /* entity */
Inst inst,                  /* instance */
S16 period,                 /* period */
S16 units,                  /* period units */
PAIFTMRS16 tmrFnctMt        /* timer function, typically SActvTmr */
)
{
   S16 ret;
   /* ss015.301 - Enclosed all timer activation functions in a union. */
   SsTmrActvFn ssTmrActvFn;

   ssTmrActvFn.actvFnc.tmrActvFnMt = tmrFnctMt;
   ssTmrActvFn.mtFlag = TRUE;
   ret = STmrRegHndlr(ent, inst, period, units, ssTmrActvFn);

   return (ret);
}
#endif /* SS_MT_TMR */
#endif /* not SS_MULTIPLE_PROCS */

/* ss041.103 - Function for timer registration handling */
/*
*
*       Fun:   Timer registration handler function
*
*       Desc:  This function is called by the timer registration interface
*              functions, and does the actual handling for timer
*              registration.
*
*       Ret:   ROK      - ok
*              RFAILED  - failed, general (optional)
*              ROUTRES  - failed, out of resources (optional)
*
*       Notes:
*
*       File:  ss_timer.c
*
*/

/* ss015.301 Changed the timer activation function type as all timer activation
 * functions are enclosed in a union.
 */

#ifndef SS_MULTIPLE_PROCS

static S16 STmrRegHndlr
(
Ent ent,                    /* entity */
Inst inst,                  /* instance */
S16 period,                 /* period */
S16 units,                  /* period units */
SsTmrActvFn ssTmrActvFn               /* timer function, typically SActvTmr */
)
/* ss015.301: Removed the timer handler prototypes guarded
 * under SS_MT_TMR and handled mtFlag in existing timer handlers.
 */

#else /* SS_MULTIPLE_PROCS */

static S16 STmrRegHndlr
(
ProcId proc,                /* processor */
Ent ent,                    /* entity */
Inst inst,                  /* instance */
S16 period,                 /* period */
S16 units,                  /* period units */
SsTmrActvFn ssTmrActvFn     /* timer function */
)
#endif /* SS_MULTIPLE_PROCS */
{
#if (ERRCLASS & ERRCLS_INT_PAR)
   uint8_t i;
#endif
   S16 ret;
   SsTmrEntry *tmr;
#ifdef SS_MULTIPLE_PROCS
#if (ERRCLASS & ERRCLS_INT_PAR)
   uint16_t procIdIdx;
#endif /* ERRCLASS & ERRCLS_INT_PAR */
#endif /* SS_MULTPLE_PROCS */

#if (ERRCLASS & ERRCLS_INT_PAR)

#ifdef SS_MULTIPLE_PROCS
   if ((proc == SS_INV_PROCID) || (ent >= SS_MAX_ENT) ||  (inst >= SS_MAX_INST))
   {
      SSLOGERROR(ERRCLS_INT_PAR, ESS528, ERRZERO, "Invalid processor/entity/instance");
      return RFAILED;
   }
#else /* SS_MULTIPLE_PROCS */
   if (ent >= SS_MAX_ENT ||  inst >= SS_MAX_INST)
   {
      SSLOGERROR(ERRCLS_INT_PAR, ESS529, ERRZERO, "Invalid entity/instance");
      return RFAILED;
   }
#endif /* SS_MULTIPLE_PROCS */

   /* check period */
   if (period <= 0)
   {
      SSLOGERROR(ERRCLS_INT_PAR, ESS530, ERRZERO, "Invalid period");
      return RFAILED;
   }

   /* check period units*/
   if ((units < 1) || (units > SS_TICKS_SEC))
   {
      SSLOGERROR(ERRCLS_INT_PAR, ESS531, ERRZERO, "Invalid period units");
      return RFAILED;
   }

   if(ssTmrActvFn.mtFlag == TRUE) 
   { 
   /* ss015.301 Enclosed all timer activation functions in a union. */
#ifndef SS_MULTIPLE_PROCS
#ifdef SS_MT_TMR
      if (ssTmrActvFn.actvFnc.tmrActvFnMt == NULLP)  
      {
         SSLOGERROR(ERRCLS_INT_PAR, ESS532, ERRZERO, "Null pointer");
         return RFAILED;
      }
#else
      return RFAILED;
#endif
#else
      return RFAILED;
#endif
   }
   else
   { 
      if (ssTmrActvFn.actvFnc.tmrActvFn == NULLP)  
      {
          SSLOGERROR(ERRCLS_INT_PAR, ESS532, ERRZERO, "Null pointer");
          return RFAILED;
      }
   } 

#ifdef SS_MULTIPLE_PROCS
   /* check if the procId has been registered with SSI */
   procIdIdx = SGetProcIdIdx(proc);

   if (procIdIdx == SS_INV_PROCID_IDX)
   {
      SSLOGERROR(ERRCLS_INT_PAR, ESS533, ERRZERO,
                     "Could not find proc id index");
      return RFAILED;
   }
#endif /* SS_MULTIPLE_PROCS */


   /* check task ID */
   /* lock TAPA task table */
   SS_ACQUIRE_SEMA(&osCp.tTskTblSem, ret);
   if (ret != ROK)
   {
      SSLOGERROR(ERRCLS_DEBUG, ESS534, ERRZERO,
                     "Could not lock TAPA task table");
      return RFAILED;
   }

#ifdef SS_MULTIPLE_PROCS
   if (osCp.tTskIds[procIdIdx][ent][inst] == SS_TSKNC)
#else /* SS_MULTIPLE_PROCS */
   if (osCp.tTskIds[ent][inst] == SS_TSKNC)
#endif /* SS_MULTIPLE_PROCS */
   {
      if ( SS_RELEASE_SEMA(&osCp.tTskTblSem) != ROK)
      {
#if (ERRCLASS & ERRCLS_DEBUG)
      SSLOGERROR(ERRCLS_DEBUG, ESS535, ERRZERO,
                     "Could not release the semaphore");
      return RFAILED;
#endif
      }
      SSLOGERROR(ERRCLS_INT_PAR, ESS536, ERRZERO, "Unknown task");
      return RFAILED;
   }
   if ( SS_RELEASE_SEMA(&osCp.tTskTblSem) != ROK)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      SSLOGERROR(ERRCLS_DEBUG, ESS537, ERRZERO,
                     "Could not release the semaphore");
   return RFAILED;
#endif
   }

#endif

   /* lock the timer table */
   ret = SLock(&osCp.tmrTblLock);
   if (ret != ROK)
   {

#if (ERRCLASS & ERRCLS_DEBUG)
      SSLOGERROR(ERRCLS_DEBUG, ESS538, (ErrVal) ret,
                     "Could not lock timer table");
#endif
      return (ret);
   }


   /* check if we've got room for another timer */
   if (osCp.numTmrs == SS_MAX_TMRS)
   {
      if ( SUnlock(&osCp.tmrTblLock) != ROK)
      {
#if (ERRCLASS & ERRCLS_DEBUG)
         SSLOGERROR(ERRCLS_DEBUG, ESS539, ERRZERO,
                     "Could not unlock the semaphore");
         return RFAILED;
#endif
      }

#if (ERRCLASS & ERRCLS_DEBUG)
      SSLOGERROR(ERRCLS_DEBUG, ESS540, ERRZERO, "Too many timers");
#endif

      return (ROUTRES);
   }

#if (ERRCLASS & ERRCLS_INT_PAR)
   /* check through the timer table for a matching timer entry */
   for (i = 0; i < SS_MAX_TMRS; i++)
   {   
      if ((osCp.tmrTbl[i].ownerEnt == ent)
#ifdef SS_MULTIPLE_PROCS
            &&  (osCp.tmrTbl[i].ownerProc == proc)
#endif /* SS_MULTIPLE_PROCS */
            && (osCp.tmrTbl[i].ownerInst == inst)
            /* ss041.103 - Added mtFlag check */
            /* ss015.301 Enclosed all timer activation functions in a union. */
#ifndef SS_MULTIPLE_PROCS
#ifdef SS_MT_TMR
            && (osCp.tmrTbl[i].ssTmrActvFn.mtFlag == ssTmrActvFn.mtFlag)
            && ((ssTmrActvFn.mtFlag == FALSE
                  && osCp.tmrTbl[i].ssTmrActvFn.actvFnc.tmrActvFn == ssTmrActvFn.actvFnc.tmrActvFn)  
               || osCp.tmrTbl[i].ssTmrActvFn.actvFnc.tmrActvFnMt == ssTmrActvFn.actvFnc.tmrActvFnMt)
#else
            && (osCp.tmrTbl[i].ssTmrActvFn.actvFnc.tmrActvFn == ssTmrActvFn.actvFnc.tmrActvFn)
#endif
#else
            && (osCp.tmrTbl[i].ssTmrActvFn.actvFnc.tmrActvFn == ssTmrActvFn.actvFnc.tmrActvFn)
#endif
            && (osCp.tmrTbl[i].interval == (uint32_t) ((period * SS_TICKS_SEC) / units)))
      {
         /* is this timer in use ? (unexpired) */
         if (osCp.tmrTbl[i].used == TRUE)
         {  
            /* to prevent the same timer to be registered with SS  UNLOCK and */
            /* then RFAIL */
            if (SUnlock(&osCp.tmrTblLock) != ROK)
            {
#if (ERRCLASS & ERRCLS_DEBUG)
   SSLOGERROR(ERRCLS_DEBUG, ESS541, ERRZERO, "Could not unlock the semaphore");
   return RFAILED;
#endif
            }
            return RFAILED;
         }
      }
   }
#endif

   /* fill in the information we have into the timer entry */
   tmr = &osCp.tmrTbl[osCp.nxtTmrEntry];
   tmr->tmrId     = osCp.nxtTmrEntry;
#ifdef SS_MULTIPLE_PROCS
   tmr->ownerProc = proc;
#endif /* SS_MULTIPLE_PROCS */
   tmr->ownerEnt  = ent;
   tmr->ownerInst = inst;
   tmr->interval  = (uint32_t) ((period * SS_TICKS_SEC) / units);
   tmr->ssTmrActvFn.mtFlag = ssTmrActvFn.mtFlag;

   /* ss041.103 */
   /* ss015.301 Enclosed all timer activation functions in a union. */
#ifndef SS_MULTIPLE_PROCS
#ifdef SS_MT_TMR
   if (ssTmrActvFn.mtFlag == TRUE)
   {
      tmr->ssTmrActvFn.actvFnc.tmrActvFnMt = ssTmrActvFn.actvFnc.tmrActvFnMt;  
   }
   else
#endif
#endif
   {
      tmr->ssTmrActvFn.actvFnc.tmrActvFn = ssTmrActvFn.actvFnc.tmrActvFn;
   }


   /* ask the implementation to start the timer */
   ret = ssdRegTmr(tmr);
   if (ret != ROK)
   {
      tmr->tmrId = 0;
#ifdef SS_MULTIPLE_PROCS
      tmr->ownerProc = PROCNC;
#endif /* SS_MULTIPLE_PROCS */
      tmr->ownerEnt = ENTNC;
      tmr->ownerInst = INSTNC;
      tmr->interval = 0;
      /* ss015.301 Enclosed all timer activation functions in a union. */
      tmr->ssTmrActvFn.mtFlag = FALSE;
      tmr->ssTmrActvFn.actvFnc.tmrActvFn = NULLP;
#ifndef SS_MULTIPLE_PROCS
#ifdef SS_MT_TMR
      tmr->ssTmrActvFn.actvFnc.tmrActvFnMt = NULLP;
#endif
#endif
   }
   else
   {
      tmr->used = TRUE;
      osCp.nxtTmrEntry = tmr->nxt;
      osCp.numTmrs++;
   }


   /* unlock the timer table */
   if ( SUnlock(&osCp.tmrTblLock) != ROK)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      SSLOGERROR(ERRCLS_DEBUG, ESS542, ERRZERO,
                     "Could not unlock the semaphore");
      return RFAILED;
#endif
   }

   return (ret);
}



/*
*
*       Fun:   SDeregCfgTmr  
*
*       Desc:  This function is used to deregister a timer function.
*
*       Ret:   ROK      - ok
*              RFAILED  - failed, general (optional)
*
*       Notes:
*
*       File:  ss_timer.c
*
*/
/* ss029.103: addition: procId added and timer function type modified */ 
#ifndef SS_MULTIPLE_PROCS

S16 SDeregCfgTmr
(
Ent ent,                    /* entity */
Inst inst,                  /* instance */
S16 period,                 /* period */
S16 units,                  /* period units */
PFS16 tmrFnct               /* timer function */
)
{
   S16 ret;
   /* ss015.301 Enclosed all timer activation functions in a union. */
   SsTmrActvFn ssTmrActvFn;
   /* ss041.103 - Moved handling to STmrDeregHndlr */

   ssTmrActvFn.mtFlag = FALSE;
   ssTmrActvFn.actvFnc.tmrActvFn = tmrFnct;  
   ret = STmrDeregHndlr(ent, inst, period, units, ssTmrActvFn);

   return (ret);
}

#else /* SS_MULTIPLE_PROCS */

S16 SDeregCfgTmr
(
ProcId proc,                /* processor */
Ent ent,                    /* entity */
Inst inst,                  /* instance */
S16 period,                 /* period */
S16 units,                  /* period units */
PAIFTMRS16 tmrFnct               /* timer function */
)
{
   S16 ret;
   /* ss015.301 Enclosed all timer activation functions in a union. */ 
   SsTmrActvFn ssTmrActvFn;

   /* ss041.103 - Moved handling to STmrDeregHndlr */

   ssTmrActvFn.mtFlag = FALSE;
   ssTmrActvFn.actvFnc.tmrActvFn = tmrFnct;  
   ret = STmrDeregHndlr(proc, ent, inst, period, units, ssTmrActvFn);

   return (ret);
}

#endif /* SS_MULTIPLE_PROCS */


/* ss041.103 - Addition to support MT timer dereg */
#ifndef SS_MULTIPLE_PROCS
#ifdef SS_MT_TMR
/*
*
*       Fun:   SDeregCfgTmrMt
*
*       Desc:  This function is used to deregister an MT timer function.
*
*       Ret:   ROK      - ok
*              RFAILED  - failed, general (optional)
*
*       Notes:
*
*       File:  ss_timer.c
*
*/
S16 SDeregCfgTmrMt
(
Ent ent,                    /* entity */
Inst inst,                  /* instance */
S16 period,                 /* period */
S16 units,                  /* period units */
PAIFTMRS16 tmrFnctMt               /* timer function */
)
{
   S16 ret;
   /* ss015.301 Enclosed all timer activation functions in a union. */
   SsTmrActvFn ssTmrActvFn;

   ssTmrActvFn.actvFnc.tmrActvFnMt = tmrFnctMt;
   ssTmrActvFn.mtFlag = TRUE;

   ret = STmrDeregHndlr(ent, inst, period, units, ssTmrActvFn);

   return (ret);
}
#endif /* SS_MT_TMR */
#endif /* not SS_MULTIPLE_PROCS */


/* ss041.103 - Addition of STmrDeregHndlr */
/*
*
*       Fun:   STmrDeregHndlr  
*
*       Desc:  This function is used to deregister a timer function.
*
*       Ret:   ROK      - ok
*              RFAILED  - failed, general (optional)
*
*       Notes:
*
*       File:  ss_timer.c
*
*/
/* ss029.103: addition: procId added and timer function type modified */ 

/* ss015.301 Changed the timer activation function type as all timer activation
 * functions are enclosed in a union.
 */
#ifndef SS_MULTIPLE_PROCS
static S16 STmrDeregHndlr
(
Ent ent,                    /* entity */
Inst inst,                  /* instance */
S16 period,                 /* period */
S16 units,                  /* period units */
SsTmrActvFn ssTmrActvFn           /* timer function */
)
/* ss015.301: Removed the timer handler prototypes guarded under SS_MT_TMR
 * and handled mtFlag in existing timer handlers.
 */

#else /* SS_MULTIPLE_PROCS */

static S16 STmrDeregHndlr
(
ProcId proc,                /* processor */
Ent ent,                    /* entity */
Inst inst,                  /* instance */
S16 period,                 /* period */
S16 units,                  /* period units */
SsTmrActvFn ssTmrActvFn          /* timer function */
)

#endif /* SS_MULTIPLE_PROCS */
{
   S16 ret;
   S16 idx;
   SsTmrEntry *tmr;
#ifdef SS_MULTIPLE_PROCS
#if (ERRCLASS & ERRCLS_INT_PAR)
   uint16_t procIdIdx;
#endif /* ERRCLASS & ERRCLS_INT_PAR */
#endif /* SS_MULTIPLE_PROCS */


#if (ERRCLASS & ERRCLS_INT_PAR)

#ifdef SS_MULTIPLE_PROCS
   if ((proc == SS_INV_PROCID) || (ent >= SS_MAX_ENT) ||  (inst >= SS_MAX_INST))
   {
      SSLOGERROR(ERRCLS_INT_PAR, ESS543, ERRZERO, "Invalid processor/entity/instance");
      return RFAILED;
   }

#else /* SS_MULTIPLE_PROCS */
   if (ent >= SS_MAX_ENT ||  inst >= SS_MAX_INST)
   {
      SSLOGERROR(ERRCLS_INT_PAR, ESS544, ERRZERO, "Invalid entity/instance");
      return RFAILED;
   }
#endif /* SS_MULTIPLE_PROCS */
   /* check period */
   if (period <= 0)
   {
      SSLOGERROR(ERRCLS_INT_PAR, ESS545, ERRZERO, "Invalid period");
      return RFAILED;
   }

   /* check period units */
   if ((units < 1) || (units > SS_TICKS_SEC))
   {
      SSLOGERROR(ERRCLS_INT_PAR, ESS546, ERRZERO, "Invalid period units");
      return RFAILED;
   }

   /* check timer function */
   if(ssTmrActvFn.mtFlag == TRUE)
   {
#ifndef SS_MULTIPLE_PROCS
#ifdef SS_MT_TMR
   if (ssTmrActvFn.actvFnc.tmrActvFnMt == NULLP)  
   {
      SSLOGERROR(ERRCLS_INT_PAR, ESS547, ERRZERO, "Null pointer");
      return RFAILED;
   }
#else
    return RFAILED;
#endif
#else
    return RFAILED;
#endif
 
   }
   else
   {
   if (ssTmrActvFn.actvFnc.tmrActvFn == NULLP)  
   {
      SSLOGERROR(ERRCLS_INT_PAR, ESS547, ERRZERO, "Null pointer");
      return RFAILED;
   }
  }

#ifdef SS_MULTIPLE_PROCS
   /* check if the procId has been registered with SSI */
   procIdIdx = SGetProcIdIdx(proc);

   if (procIdIdx == SS_INV_PROCID_IDX)
   {
      SSLOGERROR(ERRCLS_INT_PAR, ESS548, ERRZERO,
                     "Could not find proc id index");
      return RFAILED;
   }
#endif /* SS_MULTIPLE_PROCS */

   /* check task ID */
   /* lock TAPA task table */
   SS_ACQUIRE_SEMA(&osCp.tTskTblSem, ret);
   if (ret != ROK)
   {
      SSLOGERROR(ERRCLS_DEBUG, ESS549, ERRZERO,
                     "Could not lock TAPA task table");
      return RFAILED;
   }

#ifdef SS_MULTIPLE_PROCS
   if (osCp.tTskIds[procIdIdx][ent][inst] == SS_TSKNC)
#else /* SS_MULTIPLE_PROCS */
   if (osCp.tTskIds[ent][inst] == SS_TSKNC)
#endif /* SS_MULTIPLE_PROCS */
   {
      if ( SS_RELEASE_SEMA(&osCp.tTskTblSem) != ROK)
      {
#if (ERRCLASS & ERRCLS_DEBUG)
         SSLOGERROR(ERRCLS_DEBUG, ESS550, ERRZERO,
                     "Could not release the semaphore");
         return RFAILED;
#endif
      }
      SSLOGERROR(ERRCLS_INT_PAR, ESS551, ERRZERO, "Unknown task");
      return RFAILED;
   }
   if ( SS_RELEASE_SEMA(&osCp.tTskTblSem) != ROK)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      SSLOGERROR(ERRCLS_DEBUG, ESS552, ERRZERO,
                     "Could not release the semaphore");
      return RFAILED;
#endif
    }

#endif

   /* lock the timer table */
   ret = SLock(&osCp.tmrTblLock);
   if (ret != ROK)
   {

#if (ERRCLASS & ERRCLS_DEBUG)
      SSLOGERROR(ERRCLS_DEBUG, ESS553, (ErrVal) ret,
                     "Could not lock timer table");
#endif
      return (ret);
   }


   /* Note: Right now, we're using ent, inst and tmrActvFn to locate
    *       the timer,, this will change to using tmrId, some day.
    */

   /* locate the timer to delete in the timer table */
   for (idx = 0;  idx < SS_MAX_TMRS;  idx++)
   {
      /* ss021.103 - Modification find timer by ent, inst, func, and period */
      /* ss029.103: addition: multiple procId related changes */ 
      if (osCp.tmrTbl[idx].ownerEnt == ent
#ifdef SS_MULTIPLE_PROCS
            &&  osCp.tmrTbl[idx].ownerProc == proc 
#endif /* SS_MULTIPLE_PROCS */
            &&  osCp.tmrTbl[idx].ownerInst == inst
#ifndef SS_MULTIPLE_PROCS
#ifdef SS_MT_TMR
           /*
	    *  ss015.301 - Modifed as timer activation functions
	    *  enclosed in a union. 
	    */
            &&  osCp.tmrTbl[idx].ssTmrActvFn.mtFlag == ssTmrActvFn.mtFlag
            && ((ssTmrActvFn.mtFlag == FALSE
                  &&  osCp.tmrTbl[idx].ssTmrActvFn.actvFnc.tmrActvFn == ssTmrActvFn.actvFnc.tmrActvFn)
               || osCp.tmrTbl[idx].ssTmrActvFn.actvFnc.tmrActvFnMt == ssTmrActvFn.actvFnc.tmrActvFnMt)
#else
            &&  osCp.tmrTbl[idx].ssTmrActvFn.actvFnc.tmrActvFn == ssTmrActvFn.actvFnc.tmrActvFn
#endif
#else
            &&  osCp.tmrTbl[idx].ssTmrActvFn.actvFnc.tmrActvFn == ssTmrActvFn.actvFnc.tmrActvFn
#endif
            &&  osCp.tmrTbl[idx].interval == (uint32_t) ((period * SS_TICKS_SEC) / units))
      {
         break;
      }
   }

   if (idx == SS_MAX_TMRS)
   {
      if ( SUnlock(&osCp.tmrTblLock) != ROK)
      {
#if (ERRCLASS & ERRCLS_DEBUG)
         SSLOGERROR(ERRCLS_DEBUG, ESS554, ERRZERO,
                     "Could not unlock the semaphore");
         return RFAILED;
#endif
      }

#if (ERRCLASS & ERRCLS_DEBUG)
      SSLOGERROR(ERRCLS_DEBUG, ESS555, ERRZERO, "Could not locate timer");
#endif

      return RFAILED;
   }


   /* ask the implementation to shut down this timer */
   tmr = &osCp.tmrTbl[idx];
   ret = ssdDeregTmr(tmr);
   if (ret == ROK)
   {
      tmr->used = FALSE;
      tmr->tmrId = 0;
#ifdef SS_MULTIPLE_PROCS
      tmr->ownerProc = PROCNC;
#endif /* SS_MULTIPLE_PROCS */
      tmr->ownerEnt = ENTNC;
      tmr->ownerInst = INSTNC;
      tmr->interval = 0;
      /*
       *  ss015.301: Modifed as timer activation functions enclosed in a union. 
       */
      tmr->ssTmrActvFn.mtFlag = FALSE;
      tmr->ssTmrActvFn.actvFnc.tmrActvFn = NULLP;
#ifndef SS_MULTIPLE_PROCS
#ifdef SS_MT_TMR
      tmr->ssTmrActvFn.actvFnc.tmrActvFnMt = NULLP;
#endif
#endif

      tmr->nxt = osCp.nxtTmrEntry;
      osCp.nxtTmrEntry = (SsIdx)idx;
      osCp.numTmrs--;
   }


   /* unlock the timer table */
   if ( SUnlock(&osCp.tmrTblLock) != ROK)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      SSLOGERROR(ERRCLS_DEBUG, ESS556, ERRZERO,
                     "Could not unlock the semaphore");
      return RFAILED;
#endif
   }


   return (ret);
}

/**********************************************************************
         End of file
**********************************************************************/
