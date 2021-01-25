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
 
     Type:     C include file
 
     Desc:     Data structure definitions for STREAMS messages.
 
     File:     ss_strm.x
 
*********************************************************************21*/


#ifndef __SSSTRMX__
#define __SSSTRMX__

#ifdef __cplusplus
extern "C" {
#endif


/* message block is defined in ssi.x */
typedef struct ssmsgb    SsMblk;



/* data block */
struct ssdatab
{
   uint8_t                   db_ref;                 /* reference count */
   uint8_t                   db_type;                /* type of data block */
	/* ss006.301 */
   Bool                 shared;                 /* is this data block shared ? */
#ifndef  SS_DBUF_REFLOCK_DISABLE
   SLockId              dBufLock;              /* Lock per Dbuf */
#endif
   uint8_t                   *db_base;               /* base of data buffer */
   uint8_t                   *db_lim;                /* limit of data buffer */
	/* ss008.301 - deprecating for normal OS, useful for Cavium/RMI OS*/
#ifdef SS_DBLK_FREE_RTN
   struct ssfree_rtn    *db_frtnp;             /* free routine */
#endif /* SS_DBLK_FREE_RTN */
};

typedef struct ssdatab   SsDblk;



/* to free memory used with esballoc() */
struct ssfree_rtn
{
   void                 (*free_func)(char *);   /* free function */
   char                 *free_arg;              /* argument to function */
};

typedef struct ssfree_rtn SsFrtn;



/* STREAMS functions prototypes */
S16      ssStrmCfg ARGS((Region, Region));
S32      ssAdjMsg ARGS((SsMblk *, S32));
SsMblk   *ssAllocB ARGS((S32, uint32_t));
SsMblk   *ssCopyB ARGS((SsMblk *));
SsMblk   *ssCopyMsg ARGS((SsMblk *));
SsMblk   *ssDupB ARGS((SsMblk *));
SsMblk   *ssDupMsg ARGS((SsMblk *));
SsMblk   *ssESBAlloc ARGS((uint8_t *, S32, S32, SsFrtn *));
void     ssFreeB ARGS((SsMblk *));
void     ssFreeMsg ARGS((SsMblk *));
void     ssLinkB ARGS((SsMblk *, SsMblk *));
S32      ssMsgDSize ARGS((SsMblk *));
S32      ssPullupMsg ARGS((SsMblk *, S32));
SsMblk   *ssRmvB ARGS((SsMblk *, SsMblk *));
S32      ssTestB ARGS((S32, uint32_t));
SsMblk   *ssUnlinkB ARGS((SsMblk *));


#ifdef __cplusplus
}
#endif

#endif  /*  __SSSTRMX__  */

/**********************************************************************
         End of file
**********************************************************************/
