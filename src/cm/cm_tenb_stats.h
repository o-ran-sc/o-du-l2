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

/**********************************************************************
 
    Name:  
 
    Type:  
 
    Desc:  
 
    File:  cm_tenb_stats.h
 
**********************************************************************/
 
/** @file cm_tenb_stats.h
*/
 
#ifdef TENB_STATS
#ifndef __TENBSTATSINFH__
#define __TENBSTATSINFH__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define TENBSTATSINIT       0x7F

#ifdef BRDCM
#define CL_DSTENT_SELECTOR 0
#define STATS_SRCPROCID 1 
#define STATS_DSTPROCID 1 
#define STATS_DSTENT    ENTPJ
#define STATS_DSTINST   0        /* PDCP UL */
#else /* BRDCM */
#ifdef MSPD
#define CL_DSTENT_SELECTOR 0
#ifdef L2_L3_SPLIT
#define STATS_SRCPROCID 1
#define STATS_DSTPROCID 1 
#define STATS_DSTENT    ENTRLC
#else
#define STATS_SRCPROCID 100
#define STATS_DSTPROCID 100 
#define STATS_DSTENT    ENTRLC
#endif /* L2_L3_SPLIT */

#define STATS_DSTINST   0        /* PDCP UL */
#else /* MSPD */
#define CL_DSTENT_SELECTOR 0
#define STATS_SRCPROCID 0
#define STATS_DSTPROCID 0 
#define STATS_DSTENT    ENTPJ
#define STATS_DSTINST   0        /* PDCP UL */
#endif /* MSPD */
#endif

#define STATS_SRCENT    ENTLWRMAC

#define EVTTENBL2CELLSTATS  0xF1
#define EVTTENBL2UESTATS    0xF2

#endif
#endif /* TENB_STATS */
/**********************************************************************
         End of file
**********************************************************************/
