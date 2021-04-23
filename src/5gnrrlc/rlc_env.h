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
  
        Name:     NR RLC file        
    
        Type:     C include file
  
        Desc:     This file contains the constants required for NR RLC.
                  The defines in this file need to be changed by the customer 
                  to reflect the software architecture needed to run under the 
                  target system hardware architecture.

        File:     rlc_env.h
  
*********************************************************************21*/
/** @file rlc_env.h
@brief RLC Hash definitions
*/

#ifndef __RLC_ENV_H__
#define __RLC_ENV_H__

#define MAX_RLC_INSTANCES            2
#define RLC_MAX_LI                       KWU_MAX_STA_IND_SDU

#define RLC_MAX_DL_LI                   28 
#define RLC_MAX_NEW_DL_PDU              16
/* kw003.201 Adding new environment variables */
/* This parameter is utilized when KW_BG_DL_PROC enbled. This parameter */
/* defines how many PDUs may be released in one go without affecting    */
/* TTI response time                                                    */
/* kw004.201 modifed the value from 20 to 15 */
#define RLC_AM_MAX_PDUS_RLS              1
/* This parameter governs maximum number of PDUs to processed at a      */
/* given point when KW_BG_UL_PROC is enabled.                           */
/* kw004.201 modifed the value from 20 to 5 */
#define RLC_AM_MAX_UL_PDUS               1 

#define RLC_AM_UM_MAX_UL_SDUS            1
#define RLC_AM_UM_MAX_DL_SDUS            300

#ifdef SPLIT_RLC_DL_TASK
#define RLC_MAX_TO_BE_FREED              60
#else
#define RLC_MAX_TO_BE_FREED              10
#endif

/* Number of packets queued in SDU Q after which overload 
 * START is signalled to PDCP for an UM bearer */
#define RLC_UM_RB_OVERLOAD_HIGH_THRES   512
/* Number of packets queued in SDU Q after which overload 
 * STOP is signalled to PDCP for an UM bearer */
#define RLC_UM_RB_OVERLOAD_LOW_THRES  300 

/* Value of this macro can be in the range of 1 to RGU_MAX_PDU */
#define RLC_MAX_PDU                      16

#ifdef LTE_L2_MEAS
/* This value is decided on the assumption that there will be maximum 3 active DRB at a time */
#define RLC_MAX_ACTV_DRB                2
/* Assumption is that all UEs wont have all RBs with max SDU: if greater, will be ignored */
#define RLC_L2MEAS_SDUIDX               ((RLC_MAX_DL_LI/4)*RLC_MAX_ACTV_DRB)
/* Number of out standing SDUS in one RbCb : This would be sufficient but anything more will be discarded for counters */
#define RLC_L2MEAS_MAX_OUTSTNGSDU       31
#endif


#endif /* __RLC_ENV_H__ */
  
/********************************************************************30**
  
         End of file
**********************************************************************/
