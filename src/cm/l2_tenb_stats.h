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
 
    File:  l2_tenb_stats.h
 
**********************************************************************/
 
/** @file pj_tenb_stats.h
*/
 
#ifdef TENB_STATS 
#ifndef __TENBSTATSH__
#define __TENBSTATSH__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
/*MCELL changes*/
#ifdef LTE_PAL_ENB
#define L2_STATS_MAX_CELLS 1
#else
#define L2_STATS_MAX_CELLS 1
#endif
#ifndef XEON_SPECIFIC_CHANGES   
#define L2_STATS_MAX_UES   10
#define L2_STATS_MAX_RNTIS 500
#else
#define L2_STATS_MAX_UES   TENB_MAX_UE_SUPPORTED /* ANOOP changed */
#define L2_STATS_MAX_RNTIS 61 + TENB_MAX_UE_SUPPORTED
#endif   

#endif
#endif /* TENB_STATS */
/**********************************************************************
         End of file
**********************************************************************/
