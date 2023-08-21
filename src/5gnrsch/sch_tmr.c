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

#include <sys/time.h>

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
      case EVENT_DL_TOTAL_PRB_USAGE_TMR:
      {
         if(((TotalPrbUsage *)cb)->periodTimer.tmrEvnt == EVENT_DL_TOTAL_PRB_USAGE_TMR)
         {
             DU_LOG("\nDEBUG  -->  SCH : schChkTmr: Timer Evnt [%d] already running", tmrEvnt);
             return TRUE;
         }
         break;
      }

      case EVENT_UL_TOTAL_PRB_USAGE_TMR:
      {
         if(((TotalPrbUsage *)cb)->periodTimer.tmrEvnt == EVENT_UL_TOTAL_PRB_USAGE_TMR)
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
void schStartTmr(SchCb *gCb, PTR cb, int16_t tmrEvnt, uint8_t timerValue)
{
   TotalPrbUsage *dlTotalPrbUsage;
   TotalPrbUsage *ulTotalPrbUsage;
   CmTmrArg arg;
   arg.wait = 0;

   DU_LOG("\nINFO   -->  SCH : Starting Timer Event [%d] with Wait Time [%d] ms", \
      tmrEvnt, timerValue);
   
   switch (tmrEvnt)
   {
      case EVENT_DL_TOTAL_PRB_USAGE_TMR:
      {
         dlTotalPrbUsage = ((TotalPrbUsage *)cb);
         TMR_CALCUATE_WAIT(arg.wait, timerValue, gCb->schTimersInfo.tmrRes);

         arg.timers = &dlTotalPrbUsage->periodTimer;
         arg.max = MAX_TOTAL_PRB_USAGE_TMR;
         break;
      }

      case EVENT_UL_TOTAL_PRB_USAGE_TMR:
      {
         ulTotalPrbUsage = ((TotalPrbUsage *)cb);
         TMR_CALCUATE_WAIT(arg.wait, timerValue, gCb->schTimersInfo.tmrRes);

         arg.timers = &ulTotalPrbUsage->periodTimer;
         arg.max = MAX_TOTAL_PRB_USAGE_TMR;
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

      struct timeval tp;
      gettimeofday(&tp, 0);
      time_t curtime = tp.tv_sec;
      struct tm *t = localtime(&curtime);

      DU_LOG("\nHLAL Starting timer : %02d:%02d:%02d:%03d\n", t->tm_hour, t->tm_min, t->tm_sec, tp.tv_usec/1000);
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

   DU_LOG("\nINFO   -->  SCH : Stopping Timer Event [%d]", tmrType);

   switch (tmrType)
   {
      case EVENT_DL_TOTAL_PRB_USAGE_TMR:
         {
            arg.timers  = &((TotalPrbUsage *)cb)->periodTimer;
            arg.max = MAX_TOTAL_PRB_USAGE_TMR;
            break;
         }
         case EVENT_UL_TOTAL_PRB_USAGE_TMR:
         {
            arg.timers  = &((TotalPrbUsage *)cb)->periodTimer;
            arg.max = MAX_TOTAL_PRB_USAGE_TMR;
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
 * @brief Handler to process Timer expiry of DL Total PRB Usage calculation 
 *
 * @param[in] cb        Control block depending on the type of the timer event.
 * @param[in] tmrEvnt   Timer event to be started
 *
 * @return  Bool indicating whether the timer is running or not
 *      -# ROK
 *      -# RFAILED
*/
uint8_t SchProcDlTotalPrbUsageTmrExp(TotalPrbUsage *dlTotalPrbUsage)
{
   uint8_t percentageOfTotalPrbUsed = 0;

   if(dlTotalPrbUsage->totalPrbAvailForTx)
      percentageOfTotalPrbUsed = ((dlTotalPrbUsage->numPrbUsedForTx * 100) / dlTotalPrbUsage->totalPrbAvailForTx);
   //SchSendStatsIndToMac(dlTotalPrbUsage->schInst, SCH_DL_TOTAL_PRB_USAGE, percentageOfTotalPrbUsed);
   
   DU_LOG("\nHLAL DL Total PRB Usage = [%d]%", percentageOfTotalPrbUsed);

   /* Restart Timer */
   dlTotalPrbUsage->numPrbUsedForTx = 0;
   dlTotalPrbUsage->totalPrbAvailForTx = 0;
   schStartTmr(&schCb[dlTotalPrbUsage->schInst], (PTR)(dlTotalPrbUsage), EVENT_DL_TOTAL_PRB_USAGE_TMR, \
      dlTotalPrbUsage->periodicity);

   return ROK;
}

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
uint8_t SchProcUlTotalPrbUsageTmrExp(TotalPrbUsage *ulTotalPrbUsage)
{
   uint8_t percentageOfTotalPrbUsed = 0;

   if(ulTotalPrbUsage->totalPrbAvailForTx)
      percentageOfTotalPrbUsed = ((ulTotalPrbUsage->numPrbUsedForTx * 100) / ulTotalPrbUsage->totalPrbAvailForTx);
   //SchSendStatsIndToMac(ulTotalPrbUsage->schInst, SCH_UL_TOTAL_PRB_USAGE, percentageOfTotalPrbUsed);

   DU_LOG("\nHLAL UL Total PRB Usage = [%d]%", percentageOfTotalPrbUsed);
   /* Restart Timer */
   ulTotalPrbUsage->numPrbUsedForTx = 0;
   ulTotalPrbUsage->totalPrbAvailForTx = 0;
   schStartTmr(&schCb[ulTotalPrbUsage->schInst], (PTR)(ulTotalPrbUsage), EVENT_UL_TOTAL_PRB_USAGE_TMR, \
      ulTotalPrbUsage->periodicity);

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
   DU_LOG("\nINFO   -->  SCH : Timer Event [%d] expired", tmrEvnt);

   struct timeval tp;
   gettimeofday(&tp, 0);
   time_t curtime = tp.tv_sec;
   struct tm *t = localtime(&curtime);

   DU_LOG("\nHLAL Expired timer : %02d:%02d:%02d:%03d\n", t->tm_hour, t->tm_min, t->tm_sec, tp.tv_usec/1000);

   switch (tmrEvnt)
   {
      case EVENT_DL_TOTAL_PRB_USAGE_TMR:
         {
            SchProcDlTotalPrbUsageTmrExp((TotalPrbUsage*)cb);
            break;
         }
      case EVENT_UL_TOTAL_PRB_USAGE_TMR:
         {
            SchProcUlTotalPrbUsageTmrExp((TotalPrbUsage*)cb);
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

