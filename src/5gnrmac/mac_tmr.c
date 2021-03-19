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

#include "common_def.h"
#include "lrg.h"           /* Layer manager interface includes*/
#include "lrg.x"           /* layer management typedefs for MAC */
#include "du_app_mac_inf.h"
#include "mac_sch_interface.h"
#include "lwr_mac_upr_inf.h"
#include "mac.h"

/**
 * @brief Timer Expiry handler.
 *
 * @details
 *
 *     Function : macTmrExpiry
 *
 *     This is a callback function used as an input parameter to cmPrcTmr()
 *     to check expiry of any timer. In this function, the only concern is
 *     about tmrEvnt=Bind timer.
 *
 *  @param[in]  PTR   cb,  Entry for which Timer expired
 *  @param[in]  S16   tmrEvnt, the Timer Event
 *  @return  uint8_t
 *      -# ROK
 **/
uint8_t macTmrExpiry(PTR cb, uint8_t tmrEvnt)
{
   /* TODO : Handling of any timer event expiry */
   return ROK;
}

/**
 * @brief MAC timer call back function registered with System services
 *
 * @details
 *
 *     Function :  macActvTmr
 *
 *     This function is invoked for every timer activation
 *     period expiry.
 *
 *  @return  S16
 *      -# ROK
 **/
short int macActvTmr(Ent ent,Inst inst)
{
   /* Check if any MAC timer has expired */
   cmPrcTmr(&macCb.tmrTqCp, macCb.tmrTq, (PFV) macTmrExpiry);

   return ROK;

} /* end of rgActvTmr */

/**********************************************************************

  End of file
**********************************************************************/
