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
#include "lkw.h"
#include "lkw.x"
#include "lrg.h"
#include "lrg.x"
#include "du_app_rlc_inf.h"
#include "du_app_mac_inf.h"
#include "du_cfg.h"
#include "du_e2ap_mgr.h"
#include "du_mgr.h"
#include "du_tmr.h"
/**
 * @def DU_TMR_CALCUATE_WAIT
 *
 *    This macro calculates and assigns wait time based on the value of the
 *    timer and the timer resolution. Timer value of 0 signifies that the
 *    timer is not configured
 *
 * @param[out] _wait   Time for which to arm the timer changed to proper
 *                     value according to the resolution
 * @param[in] _tmrVal   Value of the timer
 * @param[in] _timerRes   Resolution of the timer
 *
*/
#define DU_TMR_CALCUATE_WAIT(_wait, _tmrVal, _timerRes)       \
{                                                             \
   (_wait) = ((_tmrVal) * SS_TICKS_SEC)/((_timerRes) * 1000); \
   if((0 != (_tmrVal)) && (0 == (_wait)))                     \
   {                                                          \
      (_wait) = 1;                                            \
   }                                                          \
}
/**
 * @brief Handler to check if the timer is running
 *
 * @param[in] cb        Control block depending on the type of the timer event.
 *                      It can be uplink/downlink rbCb or rgu sap control block
 * @param[in] tmrEvnt   Timer event to be started
 *
 * @return  Bool indicating whether the timer is running or not
 *      -# ROK
 *      -# RFAILED
*/
bool duChkTmr(PTR cb, int16_t tmrEvnt)
{
   switch (tmrEvnt)
   {
      case EVENT_E2_SETUP_PROCEDURE:
      {
         if(((E2apDb *)cb)->e2SetupTimer.tmrEvnt == EVENT_E2_SETUP_PROCEDURE)
         {
             DU_LOG("\nERROR  -->  DU_APP : duChkTmr: Invalid tmr Evnt [%d]", tmrEvnt);
             return TRUE;
         }
      }
      
      default:
      {
         DU_LOG("\nERROR  -->  DU_APP : duChkTmr: Invalid tmr Evnt [%d]", tmrEvnt);
      }
   }

   return FALSE;
}

/**
 * @brief Handler to start timer
 *
 * @param[in] cb        Control block depending on the type of the timer event.
 *                      It can be uplink/downlink rbCb or rgu sap control block
 * @param[in] tmrEvnt   Timer event to be started
 *
 * @return  Void
*/
void duStartTmr(PTR cb, int16_t tmrEvnt)
{
   E2apDb *e2apDb;
   CmTmrArg arg;
   arg.wait = 0;
   
   switch (tmrEvnt)
   {
      case EVENT_E2_SETUP_PROCEDURE:
      {
         e2apDb = ((E2apDb *)cb);
         DU_TMR_CALCUATE_WAIT(arg.wait, e2apDb->e2SetupTimerInterval, e2apDb->tmrRes);

         arg.timers = &e2apDb->e2SetupTimer;
         arg.max = MAX_E2_SETUP_TMR;
         break;
      }
      default:
      {
         DU_LOG("\nERROR  -->  DU : duStartTmr: Invalid tmr Evnt [%d]", tmrEvnt);
      }
   }

   if(arg.wait != 0)
   {
      arg.tqCp   = &e2apDb->tmrTqCp;
      arg.tq     = e2apDb->tmrTq;
      arg.cb     = cb;
      arg.evnt   = tmrEvnt;
      arg.tNum   = 0;

      cmPlcCbTq(&arg);
   }
   return;
}

/**
 * @brief Handler to invoke events on expiry of timer.
 *
 * @details
 *    This function is used to handle expiry of timer,it invokes relevant
 *    functions.
 *
 * @param[in] cb        Control block depending on the type of the timer event.
 *                      It can be uplink/downlink rbCb or rgu sap control block
 * @param[in] tmrEvnt   Timer event to be started
 *
 * @return  Void
*/
Void duTmrExpiry(PTR cb,S16 tmrEvnt)
{
   switch (tmrEvnt)
   {
      case EVENT_E2_SETUP_PROCEDURE:
      {
         BuildAndSendE2SetupReq();
         break;
      }
      default:
      {
         DU_LOG("\nERROR  -->  DU : duStartTmr: Invalid tmr Evnt [%d]", tmrEvnt);
         break;
      }
   }

   return;
}



/**
 * @brief Scheduler instance timer call back function registered with system services.
 *
 * @details
 *
 *     Function :  duActvTmr
 *
 *     This function is invoked for every timer activation
 *     period expiry. Note that SS_MT_TMR flag needs to be enabled for this
 *     as isntId is needed.As part of SRegTmr call for dueduler instance
 *     SS_MT_TMR flag needs to be enabled and duActvTmr needs to be given as
 *     callback function
 *
 *  @return  short int
 *      -# ROK
 **/
short int duActvTmr(Ent ent,Inst inst)
{
   /* Check if any timer in the dueduler instance has expired */
   cmPrcTmr(&duCb.e2apDb.tmrTqCp, duCb.e2apDb.tmrTq, (PFV) duTmrExpiry);

   return ROK;

} /* end of duActvTmr */

/**********************************************************************

         End of file
**********************************************************************/

