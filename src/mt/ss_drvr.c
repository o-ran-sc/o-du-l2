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
 
     Name:     System Services -- Driver
 
     Type:     C source file
 
     Desc:     Source code for those functions in System Services
               that exist for driver support.
 
     File:     ss_drvr.c
 
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


#ifdef SS_DRVR_SUPPORT



/*
*
*       Fun:   SRegDrvrTsk
*
*       Desc:  This function is used to register a driver task.
*
*       Ret:   ROK      - ok
*              RFAILED  - failed, general (optional)
*              ROUTRES  - failed, out of resources (optional)
*
*       Notes:
*
*       File:  ss_drvr.c
*
*/
S16 SRegDrvrTsk
(
Inst channel,                   /* channel instance */
ProcId low,                     /* processor ID -- low */
ProcId high,                    /* processor ID -- high */
ActvTsk actvTsk,                /* activation function */
ISTsk isTsk                     /* interrupt service function */
)
{
   S16 ret;
#if (ERRCLASS & ERRCLS_INT_PAR)

/* ss029.103: modification: multiple procId related changes */ 
#ifndef SS_MULTIPLE_PROCS
   ProcId thisProcId;
#endif

/* ss029.103: modification: multiple procId related changes */ 
#ifdef SS_MULTIPLE_PROCS
   uint16_t count;
   uint16_t i;
   ProcId procIdLst[SS_MAX_PROCS];
#endif /* SS_MULTIPLE_PROCS */

#endif


#if (ERRCLASS & ERRCLS_INT_PAR)

/* ss029.103: modification: multiple procId related changes */ 
#ifdef SS_MULTIPLE_PROCS
   if (SGetProcIdLst(&count, procIdLst) != ROK)
   {
      SSLOGERROR(ERRCLS_INT_PAR, ESS001, ERRZERO, "Null procId list");
      return RFAILED;
   }

   for (i = 0; i < count; i++)
   {
      if (procIdLst[i] >= low  &&  procIdLst[i] <= high)
      {
       SSLOGERROR(ERRCLS_INT_PAR, ESS002, ERRZERO, "Invalid procId range");
       return RFAILED;
      }
   }
#else /* SS_MULTIPLE_PROCS */
   thisProcId = SFndProcId();
   if (thisProcId >= low  &&  thisProcId <= high)
   {
      SSLOGERROR(ERRCLS_INT_PAR, ESS003, ERRZERO, "Invalid procId range");
      return RFAILED;
   }
#endif /* SS_MULTIPLE_PROCS */

   if (channel >= SS_MAX_DRVRTSKS)
   {
      SSLOGERROR(ERRCLS_INT_PAR, ESS004, channel, "Invalid channel");
      return RFAILED;
   }

   /* check activation functions */
   if (actvTsk == NULLP  ||  isTsk == NULLP)
   {
      SSLOGERROR(ERRCLS_INT_PAR, ESS005, ERRZERO, "Null pointer");
      return RFAILED;
   }

   /* check if already registered */
   if (osCp.drvrTskTbl[channel].used == TRUE)
   {
      SSLOGERROR(ERRCLS_INT_PAR, ESS006, ERRZERO,
                  "Driver task already registered");
      return RFAILED;
   }
#endif


#if (ERRCLASS & ERRCLS_DEBUG)
   /* check count of tasks */
   if (osCp.numDrvrTsks == SS_MAX_DRVRTSKS)
   {
      SSLOGERROR(ERRCLS_DEBUG, ESS007, ERRZERO, "Too many tasks");
      return (ROUTRES);
   }
#endif


   if (SInitLock(&osCp.drvrTskTbl[channel].lock, SS_DRVRENTRY_LOCK) != ROK)
   {

#if (ERRCLASS & ERRCLS_DEBUG)
      SSLOGERROR(ERRCLS_DEBUG, ESS008, ERRZERO, "Could not initialize lock");
#endif

      return RFAILED;
   }

   osCp.drvrTskTbl[channel].channel = channel;
   osCp.drvrTskTbl[channel].actvTsk = actvTsk;
   osCp.drvrTskTbl[channel].isTsk = isTsk;
   osCp.drvrTskTbl[channel].low = low;
   osCp.drvrTskTbl[channel].high = high;

   ret = ssdRegDrvrTsk(&osCp.drvrTskTbl[channel]);
   if (ret != ROK)
   {
      osCp.drvrTskTbl[channel].channel = 0;
      osCp.drvrTskTbl[channel].actvTsk = NULLP;
      osCp.drvrTskTbl[channel].isTsk = NULLP;
      osCp.drvrTskTbl[channel].low = 0;
      osCp.drvrTskTbl[channel].high = 0;
      SDestroyLock(&osCp.drvrTskTbl[channel].lock);
   }
   else
   {
      osCp.drvrTskTbl[channel].used = TRUE;
      osCp.numDrvrTsks++;
   }


   return (ret);
}

/*  ss001.301: Additions */
/*
*
*       Fun:   SDeregDrvrTsk
*
*       Desc:  This function is used to de-register a driver task.
*
*       Ret:   ROK      - ok
*              RFAILED  - failed, general (optional)
*              ROUTRES  - failed, out of resources (optional)
*
*       Notes:
*
*       File:  ss_drvr.c 
*
*/
S16 SDeregDrvrTsk
(
Inst channel                   /* channel instance */
)
{
	/* ss002.301 Modifications */

#if (ERRCLASS & ERRCLS_INT_PAR)
   if (channel >= SS_MAX_DRVRTSKS)
   {
      SSLOGERROR(ERRCLS_INT_PAR, ESSXXX, channel, "Invalid channel");
      return RFAILED;
   }

   /* check if already registered */
   if (osCp.drvrTskTbl[channel].used != TRUE)
   {
      SSLOGERROR(ERRCLS_INT_PAR, ESS477, ERRZERO,
                  "Driver task is not registered");
      return RFAILED;
   }
#endif


   if(SLock(&osCp.drvrTskTbl[channel].lock) != ROK)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
         SSLOGERROR(ERRCLS_DEBUG, ESS477, ERRZERO,
                     "Could not lock driver task lock");
#endif
      return RFAILED;
   }
   ssdDeregDrvrTsk(&osCp.drvrTskTbl[channel]);

   osCp.drvrTskTbl[channel].channel = 0;
   osCp.drvrTskTbl[channel].actvTsk = NULLP;
   osCp.drvrTskTbl[channel].isTsk = NULLP;
   osCp.drvrTskTbl[channel].low = 0;
   osCp.drvrTskTbl[channel].high = 0;

   osCp.drvrTskTbl[channel].used = FALSE;
   osCp.numDrvrTsks--;
   if(SUnlock(&osCp.drvrTskTbl[channel].lock) != ROK)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
         SSLOGERROR(ERRCLS_DEBUG, ESS477, ERRZERO,
                     "Could not unlock driver task lock");
#endif
      return RFAILED;
   }
   SDestroyLock(&osCp.drvrTskTbl[channel].lock);
   /* ss002.301 Modifications */
   return ROK;
}
#endif /* SS_DRVR_SUPPORT */
/**********************************************************************
         End of file
**********************************************************************/
