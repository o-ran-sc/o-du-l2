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
#include "sch_tmr.h"

/**
 * @brief Handler to check if the timer is running
 *
 * @param[in] cb        Control block depending on the type of the timer event.
 * @param[in] tmrEvnt   Timer event to be started
 *
 * @return  Bool indicating whether the timer is running or not
 *      -# ROK
 *      -# RFAILED
*/
bool schChkTmr(PTR cb, int16_t tmrEvnt)
{
   switch (tmrEvnt)
   {
      case EVENT_STATISTICS_TMR:
         {
            if(((SchStatsGrp *)cb)->periodTimer.tmrEvnt == EVENT_STATISTICS_TMR)
            {
               DU_LOG("\nDEBUG  -->  SCH : schChkTmr: Timer Evnt [%d] already running", tmrEvnt);
               return TRUE;
            }
            break;
         }
      default:
         {
            DU_LOG("\nERROR  -->  SCH : schChkTmr: Invalid tmr Evnt [%d]", tmrEvnt);
         }
   }
   return FALSE;
}

/**
 * @brief Handler to start timer
 *
 * @param[in] cb        Control block depending on the type of the timer event.
 * @param[in] tmrEvnt   Timer event to be started
 *
 * @return  Void
*/
void schStartTmr(SchCb *gCb, PTR cb, int16_t tmrEvnt, uint16_t timerValue)
{
   SchStatsGrp *statsGrp = NULLP;
   CmTmrArg arg;

   arg.wait = 0;

#ifdef DEBUG_PRINT
   DU_LOG("\nDEBUG   -->  SCH : Starting Timer Event [%d] with Wait Time [%d] ms", \
      tmrEvnt, timerValue);
#endif      
   
   switch (tmrEvnt)
   {
      case EVENT_STATISTICS_TMR:
      {
         statsGrp = ((SchStatsGrp *)cb);
         TMR_CALCUATE_WAIT(arg.wait, timerValue, gCb->schTimersInfo.tmrRes);

         arg.timers = &statsGrp->periodTimer;
         arg.max = MAX_NUM_TMR_PER_STATS_GRP;
         break;
      }

      default:
      {
         DU_LOG("\nERROR  -->  SCH : schStartTmr: Invalid tmr Evnt [%d]", tmrEvnt);
      }
   }

   if(arg.wait != 0)
   {
      arg.tqCp   = &(gCb->schTimersInfo.tmrTqCp);
      arg.tq     = gCb->schTimersInfo.tmrTq;
      arg.cb     = cb;
      arg.evnt   = tmrEvnt;
      arg.tNum   = 0;

      cmPlcCbTq(&arg);
   }

   return;
}

/**
 * @brief Handler to stop a timer
 *
 * @param[in] cb        Control block depending on the type of the timer event.
 * @param[in] tmrType   Timer event to be started
 *
 * @return  Void
*/
void schStopTmr(SchCb *gCb, PTR cb, uint8_t tmrType)
{
   CmTmrArg   arg;

   arg.timers = NULLP;

#ifdef DEBUG_PRINT
   DU_LOG("\nDEBUG   -->  SCH : Stopping Timer Event [%d]", tmrType);
#endif   

   switch (tmrType)
   {
      case EVENT_STATISTICS_TMR:
         {
            arg.timers  = &((SchStatsGrp *)cb)->periodTimer;
            arg.max = MAX_NUM_TMR_PER_STATS_GRP;
            break;
         }

      default:
      {
         DU_LOG("\nERROR  -->  SCH : schStopTmr: Invalid tmr Evnt[%d]", tmrType);
         break;
      }
   }

   if (tmrType != TMR0)
   {
      arg.tqCp   = &gCb->schTimersInfo.tmrTqCp;
      arg.tq     = gCb->schTimersInfo.tmrTq;
      arg.cb     = cb;
      arg.evnt   = tmrType;
      arg.wait   = 0;
      arg.tNum   = 0;
      cmRmvCbTq(&arg);
   }

   return;
}

/**
 * @brief Handler for Statistics group timer expiry
 *
 * @details
 *
 *     Function : SchProcStatisticsGrpTmrExp
 *
 *     This function calculates and sends statistics of
 *     the stats-group for which timer expired.
 *     Once Statistics Indication is sent, timer for this
 *     group is restarted.
 *
 *  @param[in]  Statistics group control block
 *  @return  uint8_t
 *      -# ROK
 **/
uint8_t SchProcStatisticsGrpTmrExp(SchStatsGrp *cb)
{
   if(schCalcAndSendGrpStats(cb) != ROK)
   {
      DU_LOG("\nERROR  -->  SCH : SchProcStatisticsGrpTmrExp: Fails to send group statistics");
   }
   schStartTmr(&schCb[cb->schInst], (PTR)(cb), EVENT_STATISTICS_TMR, cb->periodicity);
   return ROK;
}

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
 *  @param[in]  uint8_t   tmrEvnt, the Timer Event
 *  @return  uint8_t
 *      -# ROK
 **/
uint8_t schTmrExpiry(PTR cb, uint8_t tmrEvnt)
{
#ifdef DEBUG_PRINT
   DU_LOG("\nDEBUG   -->  SCH : Timer Expired. Event [%d]", tmrEvnt);
#endif

   switch (tmrEvnt)
   {
      case EVENT_STATISTICS_TMR:
         {
#ifdef DEBUG_PRINT
            DU_LOG("\nDEBUG   -->  SCH : Statistics Timer Expired for Subscription Id [%ld] GroupId [%d]", \
                  ((SchStatsGrp*)cb)->subscriptionId, ((SchStatsGrp*)cb)->groupId);
#endif
            SchProcStatisticsGrpTmrExp((SchStatsGrp*)cb);
            break;
         }

      default:
         {
            DU_LOG("\nERROR  -->  DU : duStartTmr: Invalid tmr Evnt [%d]", tmrEvnt);
            break;
         }
   }
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
   cmPrcTmr(&schCb[schInst].schTimersInfo.tmrTqCp, schCb[schInst].schTimersInfo.tmrTq, (PFV) schTmrExpiry);

   return ROK;

} /* end of schActvTmr */

/**********************************************************************

         End of file
**********************************************************************/

