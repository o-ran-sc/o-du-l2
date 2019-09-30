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
 
     Desc:     Various macro definitions required for STREAMS.
 
     File:     ss_strm.h
 
*********************************************************************21*/


#ifndef __SSSTRMH__
#define __SSSTRMH__

#ifdef __cplusplus
extern "C" {
#endif



/* STREAMS message types */
#define SS_M_DATA       0
#define SS_M_PROTO      1
#define SS_M_BREAK      2
#define SS_M_CTL        3
#define SS_M_DELAY      4
#define SS_M_IOCTL      5
#define SS_M_PASSFP     6
#define SS_M_RSE        7
#define SS_M_SETOPTS    8
#define SS_M_SIG        9


/* message initialization macro */
/* ss008.301 */
#ifdef SS_DBLK_FREE_RTN
#define SS_STRM_INITB(mp, dp, dat, size, frtn) \
   (mp)->b_next = NULLP; \
   (mp)->b_prev = NULLP; \
   (mp)->b_cont = NULLP; \
   (mp)->b_rptr = (dat); \
   (mp)->b_wptr = (dat); \
   (mp)->b_datap = (dp); \
   (dp)->db_frtnp = (frtn); \
   (dp)->db_base = (dat); \
   (dp)->db_lim = (dat) == NULLP ? (dat) : ((dat) + (size)); \
   (dp)->db_ref = 1; \
   (dp)->db_type = SS_M_DATA; \
 	(dp)->shared = FALSE;
  
#else
#define SS_STRM_INITB(mp, dp, dat, size, frtn) \
   (mp)->b_next = NULLP; \
   (mp)->b_prev = NULLP; \
   (mp)->b_cont = NULLP; \
   (mp)->b_rptr = (dat); \
   (mp)->b_wptr = (dat); \
   (mp)->b_datap = (dp); \
   (dp)->db_base = (dat); \
   (dp)->db_lim = (dat) == NULLP ? (dat) : ((dat) + (size)); \
   (dp)->db_ref = 1; \
   (dp)->db_type = SS_M_DATA; \
	(dp)->shared = FALSE;
#endif /* SS_DBLK_FREE_RTN */


#ifdef __cplusplus
}
#endif

#endif  /*  __SSSTRMH__  */

/**********************************************************************
         End of file
 **********************************************************************/
