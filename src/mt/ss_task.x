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
 
     Name:     System Services -- Task Management
 
     Type:     C include file
 
     Desc:     Data structure definitions required for the task mgmt.
 
     File:     ss_task.x
 
*********************************************************************21*/


#ifndef __SSTASKX__
#define __SSTASKX__

#ifdef __cplusplus
extern "C" {
#endif


/* miscellaneous types */
/* ss029.103: modification: 
   data type changed to allow more number of TAPA tasks */ 
#ifndef SS_MULTIPLE_PROCS
typedef uint8_t                      SsCntr;
typedef uint8_t                      SsIdx;
#else /* SS_MULTIPLE_PROCS */
typedef uint16_t                      SsCntr;
typedef uint16_t                      SsIdx;
#endif /* SS_MULTIPLE_PROCS */


/* forward declaration */
typedef struct ssSTskEntry      SsSTskEntry;


/* individual entry in the table of TAPA tasks */
typedef struct ssTTskEntry
{
   SsdTTskEntry dep;                    /* implementation specific */


   Bool         used;                   /* entry is used? */
/* ss029.103: addition: procId added */ 
#ifdef SS_MULTIPLE_PROCS
   ProcId       proc;                   /* task processor ID */
#endif /* SS_MULTIPLE_PROCS */
   Ent          ent;                    /* task entity ID */
   Inst         inst;                   /* task instance ID */
   Ttype        tskType;                /* normal/permanent/driver */
   Prior        tskPrior;               /* priority of task */
   PAIFS16      initTsk;                /* initialization function */
   ActvTsk      actvTsk;                /* activation function */

   SsSTskEntry  *sTsk;                  /* system task */


   SsIdx        nxt;                    /* table implementation */
/* ss029.103: addition: TAPA task control block (user level) added */ 
#ifdef SS_MULTIPLE_PROCS
   Void         *xxCb;                  /* protocol control block */
#endif /* SS_MULTIPLE_PROCS */

/* ss001.301: additions */
#ifdef SS_HISTOGRAM_SUPPORT
   Bool         hstReg;
#endif /* SS_HISTOGRAM_SUPPORT */

/* ss001.301: additions */
/* ss002.301: Modifications */
#ifdef SS_THREAD_PROFILE
   Bool        updated;
   Event       curEvent;
   uint32_t    curEvtTime;
   uint64_t    totTime;
#endif /* SS_THREAD_PROFILE */
	/* ss02.301 */
   ActvTsk     cbTsk; /* call back function pointer */

} SsTTskEntry;



/* individual entry in the table of system tasks */
struct ssSTskEntry
{
   SsdSTskEntry dep;                    /* implementation specific */
   Bool         used;                   /* entry is used or not */
   Bool         termPend;               /* termination pending */
   SSTskId      tskId;                  /* system task ID */
   SSTskPrior   tskPrior;               /* system task priority */
   SsDmndQ      dQ;                     /* demand queue */


   /* TAPA task information: the TAPA tasks being handled by
    *  this system task.
    */
   SsIdx        tTsks[SS_MAX_TTSKS];    /* indices into TAPA task table */
   SsCntr       numTTsks;               /* count of TAPA tasks */
   SLockId      lock;                   /* lock for table access */


   SsIdx        nxt;                    /* table implementation */
#ifdef SS_MULTICORE_SUPPORT
   Region       region;
#endif

};


#ifdef __cplusplus
}
#endif

#endif  /*  __SSTASKX__  */


  
/********************************************************************30**

         End of file
**********************************************************************/
