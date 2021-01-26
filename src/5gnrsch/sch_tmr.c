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
#include "lrg.h"
#include "lrg.x"
#include "mac_sch_interface.h"
#include "sch.h"

/**
 * @brief Timer Expiry handler.
 *
 * @details
 *
 *     Function : schTmrExpiry
 *
 *     This is a callback function used as an input parameter to cmPrcTmr()
 *     to check expiry of any timer. In this function, we are only concerned
 *     about tmrEvnt=Bind timer.
 *
 *  @param[in]  PTR   cb,  Entry for which Timer expired
 *  @param[in]  S16   tmrEvnt, the Timer Event
 *  @return  uint8_t
 *      -# ROK
 **/
uint8_t schTmrExpiry(PTR cb, uint8_t tmrEvnt)
{
   /* TODO : Handling of any timer event expiry */
   return ROK;
}

/**
 * @brief Scheduler instance timer call back function registered with system services.
 *
 * @details
 *
 *     Function :  schActvTmr
 *
 *     This function is invoked for every timer activation
 *     period expiry. Note that SS_MT_TMR flag needs to be enabled for this
 *     as isntId is needed.As part of SRegTmr call for scheduler instance
 *     SS_MT_TMR flag needs to be enabled and schActvTmr needs to be given as
 *     callback function
 *
 *  @return  short int
 *      -# ROK
 **/
short int schActvTmr(Ent ent,Inst inst)
{
   Inst schInst = (inst  - SCH_INST_START);

   /* Check if any timer in the scheduler instance has expired */
   cmPrcTmr(&schCb[schInst].tmrTqCp,
            schCb[schInst].tmrTq, (PFV) schTmrExpiry);

   return ROK;

} /* end of schActvTmr */

/**********************************************************************

         End of file
**********************************************************************/

