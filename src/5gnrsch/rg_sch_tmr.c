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

/************************************************************************
 
     Name:     LTE-MAC layer
  
     Type:     C source file
  
     Desc:     C source code for Entry point fucntions
  
     File:     rg_sch_tmr.c 
  
**********************************************************************/

/** @file rg_sch_tmr.c 
@brief This module does processing related to timers for the scheduler.
*/
static const char* RLOG_MODULE_NAME="MAC";
static int RLOG_MODULE_ID=4096;
static int RLOG_FILE_ID=175;

/* header include files -- defines (.h) */
#include "common_def.h"
#include "tfu.h"           /* RGU defines */
#include "lrg.h"           /* layer management defines for LTE-MAC */
#include "rgr.h"           /* layer management defines for LTE-MAC */
#include "rg_env.h"            /* defines and macros for MAC */
#include "rg_sch_err.h"            /* defines and macros for MAC */
#include "rg_sch_inf.h"            /* defines and macros for MAC */
#include "rg_sch.h"            /* defines and macros for MAC */
#include "rl_interface.h"
#include "rl_common.h"


/* header/extern include files (.x) */
#include "tfu.x"           /* RGU types */
#include "lrg.x"           /* layer management typedefs for MAC */
#include "rgr.x"           /* layer management typedefs for MAC */
#include "rg_sch_inf.x"            /* typedefs for MAC */
#include "rg_sch.x"            /* typedefs for MAC */

#ifdef LTE_ADV
EXTERN Void rgSCHSCellActivation ARGS((
RgSchUeCellInfo  *sCell
));
#endif
 /** @brief This function is a utility function to start timers, it is a
  * generic function.
  *
  * @details
  *
  *     Function: rgSCHTmrStartTmr 
  *
  *         Processing steps:
  *         - Starts timer at scheduler
  *         
  *   @param[in]  RgSchCellCb    *cell
  *   @param[in]  Ptr            cb
  *   @param[in]  S16            tmrEvnt
  *   @param[in]  U32            tmrVal
  *   @return  Void
  */
#ifdef ANSI
Void rgSCHTmrStartTmr
(
RgSchCellCb *cell,
Ptr         cb,
S16         tmrEvnt,
U32         tmrVal
)
#else
Void rgSCHTmrStartTmr (cell, cb, tmrEvnt, tmrVal)
RgSchCellCb *cell;
Ptr         cb;
S16         tmrEvnt;
U32         tmrVal;
#endif
{
   CmTmrArg          arg;
   RgSchUeCb         *ue;
#ifdef LTE_ADV
   RgSchUeCellInfo *sCellCb = NULLP;
#endif

   TRC2(rgSCHTmrStartTmr);


#ifndef LTE_ADV
   ue = (RgSchUeCb*)cb;
#else
   if(tmrEvnt == RG_SCH_TMR_SCELL_DEACT)
   {
      sCellCb = (RgSchUeCellInfo *)cb;
   }
   else
   {
      ue = (RgSchUeCb*)cb;
   }
#endif

   switch (tmrEvnt)
   {
      case RG_SCH_TMR_ACKNACK_REP:
         arg.timers = &(ue->ackNakRepCb.ackNakRepTmr);
         RLOG_ARG0(L_ERROR,DBG_INSTID,cell->instIdx, 
            "Hit AckNackRep timer");
         break;
      case RG_SCH_TMR_MEASGAP:
         arg.timers = &(ue->measGapCb.measGapTmr);
         break;
      case RG_SCH_TMR_UL_ACKNACK:
         arg.timers = &(ue->ackNakRepCb.ackNakRepUlInactvTmr);
         break;
      case RG_SCH_TMR_DL_ACKNACK:
         arg.timers = &(ue->ackNakRepCb.ackNakRepDlInactvTmr);
         break;
      case RG_SCH_TMR_UL_MEASGAP:
         arg.timers = &(ue->measGapCb.measGapUlInactvTmr);
         break;
      case RG_SCH_TMR_DL_MEASGAP:
         arg.timers = &(ue->measGapCb.measGapDlInactvTmr);
         break;
      case RG_SCH_TMR_TA:
         arg.timers = &(ue->taTmr);
         break;
	/*MS_WORKAROUND for CR FIXME */
#ifndef RGR_V1
      case RG_SCH_TMR_BSR:
         {
            arg.timers = &(ue->bsrTmr);
            break;
         }
#else
      case RG_SCH_TMR_BSR:
         {
#ifdef NO_BSR_SR_5GTF
            RETVOID;
#endif
            arg.timers = &(ue->bsrTmr);
            break;
         }
#endif
      case RG_SCH_TMR_TXMODE_TRNSTN:
         {
            arg.timers = &(ue->txModeTransTmr);
            break;
         }
#ifdef LTE_ADV
      case RG_SCH_TMR_SCELL_DEACT:
         {
            arg.timers = &(sCellCb->deactTmr);
            break;
         }
      case RG_SCH_TMR_SCELL_ACT_DELAY:
         {
            sCellCb = (RgSchUeCellInfo *)cb;
            arg.timers = &(sCellCb->actDelayTmr);
            break;
         }
#endif
      default:
         RLOG_ARG0(L_ERROR,DBG_INSTID,cell->instIdx,  
            "rgSCHTmrStartTmr() Incorrect Timer event");
         RETVOID;
   }

   arg.tqCp = &(cell->tqCp);
   arg.tq   = cell->tq;
   arg.cb   = (PTR)cb;
   arg.evnt = tmrEvnt;
   arg.wait = tmrVal;
   arg.max  = 1;
   arg.tNum = NOTUSED;
   cmPlcCbTq(&arg);
   RETVOID;

} /* end of */ 

 /** @brief This function stops the timer.
  *
  * @details
  *
  *     Function: rgSCHTmrStopTmr 
  *
  *         Processing steps:
  *         - Stops timer at scheduler. 
  *
  *   @param[in]  RgSchCellCb    *cell
  *   @param[in]  S16            tmrEvnt
  *   @param[in]  Ptr            cb
  *   @return  Void
  */
#ifdef ANSI
Void rgSCHTmrStopTmr
(
RgSchCellCb   *cell,
S16           tmrEvnt,
Ptr           cb
)
#else
Void rgSCHTmrStopTmr (cell, tmrEvnt, cb)
RgSchCellCb   *cell;
S16           tmrEvnt;
Ptr           cb;
#endif
{
   CmTmrArg          arg;
   RgSchUeCb         *ue;
#ifdef LTE_ADV
   RgSchUeCellInfo *sCellCb = NULLP;
#endif

   TRC2(rgSCHTmrStopTmr);


#ifndef LTE_ADV
   ue = (RgSchUeCb*)cb;
#else
   if(tmrEvnt == RG_SCH_TMR_SCELL_DEACT)
   {
      sCellCb = (RgSchUeCellInfo *)cb;
   }
   else
   {
      ue = (RgSchUeCb*)cb;
   }
#endif

   switch (tmrEvnt)
   {
      case RG_SCH_TMR_ACKNACK_REP:
         arg.timers = &(ue->ackNakRepCb.ackNakRepTmr);
         break;
      case RG_SCH_TMR_MEASGAP:
         arg.timers = &(ue->measGapCb.measGapTmr);
         break;
      case RG_SCH_TMR_UL_ACKNACK:
         arg.timers = &(ue->ackNakRepCb.ackNakRepUlInactvTmr);
         break;
      case RG_SCH_TMR_DL_ACKNACK:
         arg.timers = &(ue->ackNakRepCb.ackNakRepDlInactvTmr);
         break;
      case RG_SCH_TMR_UL_MEASGAP:
         arg.timers = &(ue->measGapCb.measGapUlInactvTmr);
         break;
      case RG_SCH_TMR_DL_MEASGAP:
         arg.timers = &(ue->measGapCb.measGapDlInactvTmr);
         break;
      case RG_SCH_TMR_TA:
         arg.timers = &(ue->taTmr);
         break;
	 	/*MS_WORKAROUND for CR FIXME */
#ifndef RGR_V1
      case RG_SCH_TMR_BSR:

         {
            arg.timers = &(ue->bsrTmr);
            break;
         }
#else
      case RG_SCH_TMR_BSR:
         {
#ifdef NO_BSR_SR_5GTF
            RETVOID;
#endif
            arg.timers = &(ue->bsrTmr);
            break;
         }

#endif
      case RG_SCH_TMR_TXMODE_TRNSTN:
         {
            arg.timers = &(ue->txModeTransTmr);
            break;
         }
#ifdef LTE_ADV
      case RG_SCH_TMR_SCELL_DEACT:
         {
            arg.timers = &(sCellCb->deactTmr);
            break;
         }
      case RG_SCH_TMR_SCELL_ACT_DELAY:
         {
            sCellCb = (RgSchUeCellInfo *)cb;
            arg.timers = &(sCellCb->actDelayTmr);
            break;
         }
#endif

      default:
         RLOG_ARG0(L_ERROR,DBG_INSTID,cell->instIdx,  
            "rgSCHTmrStopTmr() Incorrect Timer event");
         RETVOID;
   }

   arg.tqCp = &(cell->tqCp);
   arg.tq   = cell->tq;
   arg.cb   = (PTR)cb;
   arg.evnt = tmrEvnt;
   arg.wait = NOTUSED;
   arg.max  = 0;
   arg.tNum = NOTUSED;
   cmRmvCbTq(&arg);
   RETVOID;
} /* end of */ 

 /** @brief This function handles timer expiry.
  *
  * @details
  *
  *     Function: rgSCHTmrProcTmr
  *
  *         Processing steps:
  *         - Handles processing on timer expiry at scheduler.
  *
  *   @param[in]  Ptr            cb
  *   @param[in]  S16            tmrEvnt
  *   @return  Void
  */
#ifdef ANSI
Void rgSCHTmrProcTmr 
(
Ptr         cb,
S16         tmrEvnt
)
#else
Void rgSCHTmrProcTmr (cb, tmrEvnt)
Ptr         cb;
S16         tmrEvnt;
#endif
{
   RgSchUeCb      *ue = NULLP;
#ifdef LTE_ADV
   RgSchUeCellInfo *sCellCb = NULLP;
#endif

   TRC2(rgSCHTmrProcTmr);

#ifndef LTE_ADV
   ue = (RgSchUeCb*)cb;
#else
   if(tmrEvnt == RG_SCH_TMR_SCELL_DEACT)
   {
      sCellCb = (RgSchUeCellInfo *)cb;
   }
   else
   {
      ue = (RgSchUeCb*)cb;
   }
#endif


   switch (tmrEvnt)
   {
      case RG_SCH_TMR_ACKNACK_REP:
         rgSCHAckNakRepTmrExpry (ue);
         break;
      case RG_SCH_TMR_MEASGAP:
         rgSCHMeasGapANRepTmrExpry (ue);
         break;
      case RG_SCH_TMR_UL_MEASGAP:
      case RG_SCH_TMR_UL_ACKNACK:
         rgSCHMeasGapANRepUlInactvTmrExpry (ue, (U8)tmrEvnt);
         break;
      case RG_SCH_TMR_DL_ACKNACK:
      case RG_SCH_TMR_DL_MEASGAP:
         rgSCHMeasGapANRepDlInactvTmrExpry (ue, (U8)tmrEvnt);
         break;
      case RG_SCH_TMR_TA:
#ifdef EMTC_ENABLE
         /*TODO Needto handle TA Timer expiry for EMTC UE*/
         if(TRUE == ue->isEmtcUe)
         {
            RLOG0(L_INFO,"TA Timer Expiry is not handled for EMTC UE\n");
            break;
         }
#endif
         rgSCHDhmProcTAExp (ue);
         break;
	 	/*MS_WORKAROUND for CR FIXME */
#ifndef RGR_V1
      case RG_SCH_TMR_BSR:
         {
            rgSCHCmnBsrTmrExpry(ue);
         }
         break;
#else
      case RG_SCH_TMR_BSR:
         {
            rgSCHCmnBsrTmrExpry(ue);
         }
         break;

#endif
      case RG_SCH_TMR_TXMODE_TRNSTN:
         {
            ue->txModeTransCmplt = TRUE;
            break;
         }
#ifdef LTE_ADV
      case RG_SCH_TMR_SCELL_DEACT:
         {
            rgSCHSCellDeactTmrExpry(sCellCb);
            break;
         }
      case RG_SCH_TMR_SCELL_ACT_DELAY:
         {
            sCellCb = (RgSchUeCellInfo *)cb;
            rgSCHSCellActivation(sCellCb);
            break;
         }
#endif
      default:
        if(ue)
        {
           RLOG_ARG0(L_ERROR,DBG_INSTID,ue->cell->instIdx,  
                 "rgSCHTmrProcTmr() Incorrect Timer event");
        }
         RETVOID;
   }
   RETVOID;
} /* end of */ 


/**********************************************************************
 
         End of file
**********************************************************************/
