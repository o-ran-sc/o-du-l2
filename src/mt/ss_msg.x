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
 
     Name:     System Services -- Message manipulation functions
 
     Type:     C include file
 
     Desc:     Data structure definitions for message related functions.
 
     File:     ss_msg.x
 
*********************************************************************21*/

#ifndef __SSMSGX__
#define __SSMSGX__

#ifdef __cplusplus
extern "C" {
#endif


/* pool type -- dynamic or static */
typedef uint8_t              SsPoolType;



/* dynamic pool */
typedef struct ssDPoolEntry
{
   SsdDPoolEntry        dep;                    /* implementation specific */
   Size                 size;                   /* to use for SAlloc() */
} SsDPoolEntry;


/* static pool */
typedef struct ssSPoolEntry
{
   SsdSPoolEntry        dep;                    /* implementation specific */
} SsSPoolEntry;


/* generic pool (includes dynamic and static) */
typedef struct ssPoolEntry
{
   SsdPoolEntry         dep;                    /* implementation specific */
   SsPoolType           type;                   /* dynamic or static */
   union 
   {
      SsSPoolEntry      spool;                  /* static pool */
      SsDPoolEntry      dpool;                  /* dynamic pool */
   } u;
} SsPoolEntry;


/* event information--what kind of message is this */
typedef struct ssEventInfo
{
   uint8_t event;                                    /* event type */
   union
   {
      struct
      {
         SsIdx tmrIdx;                          /* timer index */
         Bool  inUse;                           /* message in use? */
         Bool  dynBuf;                          /* dynamic buffer? */
      } tmr;                                    /* for timer events */
   } u;
} SsEventInfo;


/* Information stored in the control portion of a message.
 * The SGETBUFREGION, SGETBUFPOOL, SGetBufRegionPool macros
 * depend on the order of the first two elements of this
 * structure. Make sure the macros are consistent with the
 * definition here.
 */
typedef struct ssMsgInfo
{
   Region       region;                         /* region id of the msg chain */
   Pool         pool;                           /* pool id of the msg chain */
   MsgLen       len;                            /* num of bytes in the msg */
   Buffer       *endptr;                        /* ptr to last mblk in chain */
   Buffer       *next;                          /* for SInitNxtDBuf() */
   Pst          pst;                            /* post for this message */
   SsEventInfo  eventInfo;                      /* event information */
} SsMsgInfo;

#ifdef __cplusplus
}
#endif

#endif /* __SSMSGX__ */

 

/********************************************************************30**
 
         End of file
**********************************************************************/
