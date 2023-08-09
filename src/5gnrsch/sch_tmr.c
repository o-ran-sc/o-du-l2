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
#include "sch_tmr.h"
#include "sch.h"

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
void schStartTmr(Inst inst, PTR cb, int16_t tmrEvnt, uint8_t timerValue)
{
   TotalPrbUsage *dlTotalPrbUsage;
   TotalPrbUsage *ulTotalPrbUsage;
   CmTmrArg arg;
   arg.wait = 0;
   
   switch (tmrEvnt)
   {
      case EVENT_DL_TOTAL_PRB_USAGE_TMR:
      {
         dlTotalPrbUsage = ((TotalPrbUsage *)cb);
         TMR_CALCUATE_WAIT(arg.wait, timerValue, schCb[inst].schTimersInfo.tmrRes);

         arg.timers = &dlTotalPrbUsage->periodTimer;
         arg.max = MAX_TOTAL_PRB_USAGE_TMR;
         break;
      }

      case EVENT_UL_TOTAL_PRB_USAGE_TMR:
      {
         ulTotalPrbUsage = ((TotalPrbUsage *)cb);
         TMR_CALCUATE_WAIT(arg.wait, timerValue, schCb[inst].schTimersInfo.tmrRes);

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
      arg.tqCp   = &(schCb[inst].schTimersInfo.tmrTqCp);
      arg.tq     = schCb[inst].schTimersInfo.tmrTq;
      arg.cb     = cb;
      arg.evnt   = tmrEvnt;
      arg.tNum   = 0;

      cmPlcCbTq(&arg);
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
   uint8_t percentageOfTotalPrbUsed;

   percentageOfTotalPrbUsed = ((dlTotalPrbUsage->numPrbUsedForTx * 100) / dlTotalPrbUsage->totalPrbAvailForTx);
   //SchBuildAndSendStatsInd(SCH_DL_TOTAL_PRB_USAGE, (void *)&percentageOfTotalPrbUsed);

   memset(dlTotalPrbUsage, 0, sizeof(TotalPrbUsage));
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
   uint8_t percentageOfTotalPrbUsed;

   percentageOfTotalPrbUsed = ((ulTotalPrbUsage->numPrbUsedForTx * 100) / ulTotalPrbUsage->totalPrbAvailForTx);
   //SchBuildAndSendStatsInd(SCH_UL_TOTAL_PRB_USAGE, (void *)&percentageOfTotalPrbUsed);

   memset(ulTotalPrbUsage, 0, sizeof(TotalPrbUsage));
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

