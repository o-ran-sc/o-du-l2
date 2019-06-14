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
  
        Name:     LTE RLC file for uplink and non real time tasks 
    
        Type:     C include file
  
        Desc:     This file contains helper macros for RLC uplink
                  and non real time tasks
 
        File:     kw_ul.h

*********************************************************************21*/
/** 
 * @file kw_ul.h
 * @brief RLC uplink helper macros
*/

#ifndef __KW_ULH__
#define __KW_ULH__

/**
 * @def KW_MEAS_IS_UL_IP_MEAS_ON_FOR_RB
 *
 *    This macro is used to check if UL IP throughput measurement is ON
 *    or off for the passed rb
 *
 *    Returns TRUE(non-zero) if measurement is ON else FALSE (zero)
 *
 * @param[in] _gCb     RLC UL Cb
 * @param[in] _rbCb    RLC uplink control block
 *
*/ 
#define KW_MEAS_IS_UL_IP_MEAS_ON_FOR_RB(_gCb, _rbCb)  \
     ((_rbCb->rlcId.rbType == CM_LTE_DRB) && \
     (_gCb->u.ulCb->kwL2Cb.measOn[_rbCb->qci] & LKW_L2MEAS_UL_IP) && \
     (_rbCb->rbL2Cb.measOn & LKW_L2MEAS_UL_IP))
#endif /* __KW_ULH__ */
  
/********************************************************************30**
  
         End of file
**********************************************************************/
