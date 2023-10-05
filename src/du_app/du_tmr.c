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
#include "du_tmr.h"
#include "du_app_rlc_inf.h"
#include "du_app_mac_inf.h"
#include "du_cfg.h"
#include "du_e2ap_mgr.h"
#include "du_mgr.h"
#include "du_e2ap_msg_hdl.h"

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
      case EVENT_E2_SETUP_TMR:
      {
         if(((CmTimer *)cb)->tmrEvnt == EVENT_E2_SETUP_TMR)
         {
             DU_LOG("\nERROR  -->  DU_APP : duChkTmr: Invalid tmr Evnt [%d]", tmrEvnt);
             return TRUE;
         }
         break;
      }
      case EVENT_RIC_SERVICE_UPDATE_TMR:
      {
         if(((RicServiceUpdateTimer*)cb)->timer.tmrEvnt == EVENT_RIC_SERVICE_UPDATE_TMR)
         {
            DU_LOG("\nERROR  -->  DU_APP : duChkTmr: Timer already running for event [%d]", tmrEvnt);
            return TRUE;
         }
         break;
      }      
      case EVENT_E2_NODE_CONFIG_UPDATE_TMR:
      {
         if(((E2NodeConfigUpdateTimer *)cb)->timer.tmrEvnt == EVENT_E2_NODE_CONFIG_UPDATE_TMR)
         {
             DU_LOG("\nERROR  -->  DU_APP : duChkTmr: Invalid tmr Evnt [%d]", tmrEvnt);
             return TRUE;
         }
         break;
      }
      case EVENT_RIC_SUBSCRIPTION_REPORTING_TMR:
      {
         if(((RicSubscription *)cb)->ricSubsReportTimer.tmrEvnt == EVENT_RIC_SUBSCRIPTION_REPORTING_TMR)
         {
             DU_LOG("\nERROR  -->  DU_APP : duChkTmr: Timer already running for event [%d]", tmrEvnt);
             return TRUE;
         }
         break;
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

void duStartTmr(PTR cb, int16_t tmrEvnt, uint32_t timerValue)
{
   CmTmrArg arg;
   arg.wait = 0;
   
   switch (tmrEvnt)
   {
      case EVENT_E2_SETUP_TMR:
      {
         CmTimer *e2SetupTimer = NULLP;
         e2SetupTimer = ((CmTimer *)cb);
         TMR_CALCUATE_WAIT(arg.wait, timerValue, duCb.duTimersInfo.tmrRes);

         arg.timers = e2SetupTimer;
         arg.max = MAX_E2_SETUP_TMR;
         break;
      }
      case EVENT_RIC_SERVICE_UPDATE_TMR:
      {
         RicServiceUpdateTimer *ricServiceUpdateTimer = NULLP;
         ricServiceUpdateTimer= ((RicServiceUpdateTimer*)cb);
         TMR_CALCUATE_WAIT(arg.wait, timerValue, duCb.duTimersInfo.tmrRes);

         arg.timers = &ricServiceUpdateTimer->timer;
         arg.max = MAX_RIC_SERVICE_UPDATE_TMR;
         break;
      }
      case EVENT_E2_NODE_CONFIG_UPDATE_TMR:
      {
         E2NodeConfigUpdateTimer *cfgUpdateTimer;
         cfgUpdateTimer = ((E2NodeConfigUpdateTimer*)cb);
         TMR_CALCUATE_WAIT(arg.wait, timerValue, duCb.duTimersInfo.tmrRes);

         arg.timers = &cfgUpdateTimer->timer;
         arg.max = MAX_E2_NODE_CONFIG_UPDATE_TMR;
         break;
      }
      case EVENT_RIC_SUBSCRIPTION_REPORTING_TMR:
      {
         RicSubscription *ricSubscription = NULLP;
         ricSubscription = ((RicSubscription*)cb);
         TMR_CALCUATE_WAIT(arg.wait, timerValue, duCb.duTimersInfo.tmrRes);

         arg.timers = &ricSubscription->ricSubsReportTimer;
         arg.max = MAX_RIC_SUBSCRIPTION_REPORTING_TMR;
         break;
      }
      default:
      {
         DU_LOG("\nERROR  -->  DU : duStartTmr: Invalid tmr Evnt [%d]", tmrEvnt);
         return;
      }
   }

   if(arg.wait != 0)
   {
      arg.tqCp   = &(duCb.duTimersInfo.tmrTqCp);
      arg.tq     = duCb.duTimersInfo.tmrTq;
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

void duTmrExpiry(PTR cb,int16_t tmrEvnt)
{
   switch (tmrEvnt)
   {
      case EVENT_E2_SETUP_TMR:
      {
         BuildAndSendE2SetupReq();
         break;
      }
      case EVENT_RIC_SERVICE_UPDATE_TMR:
      {
         RicServiceUpdateTimer *ricServiceUpdateTimer;
         
         ricServiceUpdateTimer= ((RicServiceUpdateTimer*)cb);
         BuildAndSendRicServiceUpdate(ricServiceUpdateTimer->ricService);
         break;
      }
      case EVENT_E2_NODE_CONFIG_UPDATE_TMR:
      {
         E2NodeConfigUpdateTimer *cfgUpdateTimer;
         
         cfgUpdateTimer = ((E2NodeConfigUpdateTimer*)cb);
         BuildAndSendE2NodeConfigUpdate(&cfgUpdateTimer->configList);
         break;
      }
      case EVENT_RIC_SUBSCRIPTION_REPORTING_TMR:
      {
         RicSubscription *ricSubscription = NULLP;

         ricSubscription = ((RicSubscription *)cb);
         E2apHdlRicSubsReportTmrExp(ricSubscription);
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
 * @brief Handler to stop timer
 *
 * @param[in] cb        Control block depending on the type of the timer event.
 * @param[in] tmrEvnt   Timer event to be stopped
 *
 * @return  Void
*/

void duStopTmr(PTR cb, uint8_t tmrType)
{
   CmTmrArg   arg;
   arg.timers = NULLP;

   switch (tmrType)
   {
      case EVENT_E2_SETUP_TMR:
      {
         CmTimer *e2SetupTimer = NULLP;
         
         e2SetupTimer = ((CmTimer *)cb);
         arg.timers = e2SetupTimer;
         arg.max = MAX_E2_SETUP_TMR;
         break;
      }
      case EVENT_RIC_SERVICE_UPDATE_TMR:
      {
         RicServiceUpdateTimer *ricServiceUpdateTimer = NULLP;
         
         ricServiceUpdateTimer= ((RicServiceUpdateTimer*)cb);
         arg.timers = &ricServiceUpdateTimer->timer;
         arg.max = MAX_RIC_SERVICE_UPDATE_TMR;
         break;
      }
      case EVENT_E2_NODE_CONFIG_UPDATE_TMR:
      {
         E2NodeConfigUpdateTimer *cfgUpdateTimer;
         
         cfgUpdateTimer = ((E2NodeConfigUpdateTimer*)cb);
         arg.timers = &cfgUpdateTimer->timer;
         arg.max = MAX_E2_NODE_CONFIG_UPDATE_TMR;
         break;
      }
      case EVENT_RIC_SUBSCRIPTION_REPORTING_TMR:
      {
         RicSubscription *ricSubscription = NULLP;
         
         ricSubscription = ((RicSubscription*)cb);
         arg.timers = &ricSubscription->ricSubsReportTimer;
         arg.max = MAX_RIC_SUBSCRIPTION_REPORTING_TMR;
         break;
      }
      default:
      {
         DU_LOG("\nERROR  -->  RLC : rlcStopTmr: Invalid tmr Evnt[%d]", tmrType);
         break;
      }
   }

   if (tmrType != TMR0)
   {
      arg.tqCp   = &(duCb.duTimersInfo.tmrTqCp);
      arg.tq     = duCb.duTimersInfo.tmrTq;
      arg.cb     = cb;
      arg.evnt   = tmrType;
      arg.wait   = 0;
      arg.tNum   = 0;
      cmRmvCbTq(&arg);
   }

   return;
}

/**
 * @brief DU instance timer call back function registered with system services.
 *
 * @details
 *
 *     Function :  duActvTmr
 *
 *     This function is invoked for every timer activation
 *     period expiry. Note that SS_MT_TMR flag needs to be enabled for this
 *     as isntId is needed.As part of SRegTmr call for du instance
 *     SS_MT_TMR flag needs to be enabled and duActvTmr needs to be given as
 *     callback function
 *
 *  @return  short int
 *      -# ROK
 **/

short int duActvTmr(Ent ent,Inst inst)
{
   /* Check if any timer in the du instance has expired */
   cmPrcTmr(&(duCb.duTimersInfo.tmrTqCp), duCb.duTimersInfo.tmrTq, (PFV) duTmrExpiry);

   return ROK;

} /* end of duActvTmr */

/**********************************************************************

         End of file
**********************************************************************/

