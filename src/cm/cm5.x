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
  
     Type:     C include file
  
     Desc:     Structures, variables and typedefs required for
               common timer routines
  
     File:     cm5.x
  
*********************************************************************21*/

#ifndef __CM5X__
#define __CM5X__

#ifdef __cplusplus
extern "C" {
#endif


/* typedefs */

typedef struct cmTimer         /* timer structure */
{
   S16 tmrEvnt;                /* Timer Event */
   U32 tqExpire;               /* Timing Queue Expiry */
   PTR cb;                     /* Pointer to start of control block list */
   struct cmTimer *next;       /* Next item in the timing queue */
   struct cmTimer *prev;       /* Previous item in the timing queue */
   U8     ent2bUpd;            /* TRUE, if the timer block is not in the
				  proper bin */
   U16   entIdx;                /* Current entry index */
} CmTimer;

typedef struct cmTqCp          /*  timing queue control point */
{
   U32 nxtEnt;                 /* next Entry */
   U32 tmrLen;                 /* length of timer (MODULO is -1) */
   CmTimer *tmp;               /* Temporary  variable used in cmPrcTmr */
} CmTqCp;

typedef struct cmTqType
{
   CmTimer *first;             /* First Entry */
   CmTimer *tail;              /* Last Entry */
} CmTqType;

typedef struct cmTmrArg
{
   CmTqCp     *tqCp;           /* timing queue control point */
   CmTqType   *tq;             /* timing queue point */
   CmTimer    *timers;         /* timers array */
   PTR        cb;              /* control block pointer */
   S16        evnt;            /* event */
   U32        wait;            /* wait */
   U8         tNum;            /* timer number */
   U8         max;             /* max timers */
} CmTmrArg;


/* trillium supplied functions */

typedef Void (*PFV) ARGS((PTR, S16));

extern Void cmInitTimers ARGS((CmTimer* timers, U8 max));
extern Void cmPlcCbTq ARGS((CmTmrArg* arg));
extern Void cmRmvCbTq ARGS((CmTmrArg* arg));
extern Void cmPrcTmr ARGS((CmTqCp* tqCp, CmTqType* tq, PFV func));
extern Void cmRstCbTq ARGS((CmTmrArg* arg));

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __CM3X__ */


/********************************************************************30**

  End of file
 **********************************************************************/
