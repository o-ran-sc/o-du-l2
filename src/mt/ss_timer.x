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
 
     Type:     C include file
 
     Desc:     Data structure definitions required for timing.
 
     File:     ss_timer.x
 
*********************************************************************21*/


#ifndef __SSTIMERX__
#define __SSTIMERX__

#ifdef __cplusplus
extern "C" {
#endif



/* timer ID and type */
typedef SsIdx           SsTmrId;
/* ss015.301 Enclosed all timer activation functions in a union. */
typedef union {
#ifdef SS_MULTIPLE_PROCS
    PAIFTMRS16        tmrActvFn;
#else
    PAIFTMRS16        tmrActvFnMt;
    PFS16             tmrActvFn;
#endif
}pTmrActvFn;


typedef struct ssTmrActvFn
{
    Bool                 mtFlag;                 /* TRUE if tmrActvFnMt to be used */
    pTmrActvFn           actvFnc;
}SsTmrActvFn;



/* individual entry in the timer table */
typedef struct ssTmrEntry
{
   SsdTmrEntry  dep;                    /* implementation specific */


   Bool         used;                   /* entry is used? */
   SsTmrId      tmrId;                  /* timer ID */
/* ss029.103: addition: procId added */ 
#ifdef SS_MULTIPLE_PROCS
   ProcId       ownerProc;              /* owner task processor ID */
#endif /* SS_MULTIPLE_PROCS */
   Ent          ownerEnt;               /* owner task entity ID */
   Inst         ownerInst;              /* owner task instance ID */
   /* ss028.103 - Modification for SRegCfgTmr support */
   uint32_t     interval;               /* timer interval */
   /* ss029.103: modification: timer function type modified */ 
   /* ss015.301 Enclosed all timer activation functions in a union. */
   SsTmrActvFn  ssTmrActvFn;

   Buffer       *mBuf;                  /* timer message buffer */


   SsIdx        nxt;                    /* table implementation */

} SsTmrEntry;



#ifdef __cplusplus
}
#endif

#endif  /*  __SSTIMERX__  */


  
/********************************************************************30**

         End of file
**********************************************************************/
