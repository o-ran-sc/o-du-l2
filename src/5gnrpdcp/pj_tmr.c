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
  
        Name:    PDCP - TMR module file
    
        Type:    C source file
  
        Desc:    Source code for timer functions such as, 

                 - pjStartTmr
                 - pjStopTmr
                 - pjTmrExpiry
                 - pjHdlDiscTmrExp
                 - pjHdlInitObdTmrExp 
                 - pjHdlUeDelWaitTmrExp
                  
        File:    pj_tmr.c
  
*********************************************************************21*/
static const char* RLOG_MODULE_NAME="PDCP";
static int RLOG_MODULE_ID=1024;
static int RLOG_FILE_ID=223;


/* header (.h) include files */
#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */

#include "gen.h"           /* general */
#include "ssi.h"           /* system services */
#include "cm5.h"           /* common timer defines */
#include "cm_tkns.h"       /* common tokens defines */
#include "cm_mblk.h"       /* common memory allocation library defines */
#include "cm_llist.h"      /* common link list  defines  */
#include "cm_hash.h"       /* common hash list  defines */
#include "cm_lte.h"        /* common LTE defines */
#include "cpj.h"           /* RRC layer */
#include "pju.h"           /* PDCP service user */
#include "kwu.h"
#include "lpj.h"           /* LPJ defines */
#include "pj_env.h"        /* RLC environment options */
#include "pj.h"            /* RLC defines */
#include "pj_err.h"        /* Error defines */
#include "pj_udx.h"

/* extern (.x) include files */
#include "gen.x"           /* general */
#include "ssi.x"           /* system services */
#include "cm5.x"           /* common timer library */
#include "cm_tkns.x"       /* common tokens */
#include "cm_mblk.x"       /* common memory allocation */
#include "cm_llist.x"      /* common link list */
#include "cm_hash.x"       /* common hash list */
#include "cm_lte.x"        /* common LTE includes */
#include "cm_lib.x"        /* common memory allocation library */
#include "cpj.x"           /* RRC layer */
#include "kwu.x"
#include "pju.x"           /* PDCP service user */
#include "lpj.x"           /* LPJ */
#include "pj.x"
#include "pj_udx.h"
#include "pj_udx.x"
#include "pj_ul.x"
#include "pj_dl.x"

/* local defines */

/* local externs */

/* forward references */
PRIVATE Void pjBndTmrExpiry ARGS ((S16 tmrEvnt, PTR cb));
/* public variable declarations */

/* This structure holds all the global structs we need. */

/* private variable declarations */

/* private function declarations */
/** @file gp_pj_tmr.c
@brief PDCP Timer Module
**/

/**
 * @def PJ_TMR_CALCUATE_WAIT
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
#define PJ_TMR_CALCUATE_WAIT(_wait, _tmrVal, _timerRes)       \
{                                                             \
      (_wait) = ((_tmrVal) * SS_TICKS_SEC)/((_timerRes) * 1000); \
      if((0 != (_tmrVal)) && (0 == (_wait)))                     \
      {                                                          \
               (_wait) = 1;                                            \
            }                                                          \
}

/**
 *
 * @brief Handler to start timer
 *       
 *
 * @b Description
 *        This function is invoked by DLM modules to start the discard timer for SDUs.
 *
 *
 *  @param[in] cb      Transmission Buffer control block 
 *  @param[in] tmrEvnt Timer event to be started
 *
 * 
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
*/
#ifdef ANSI
PUBLIC S16 pjStartTmr
(
PjCb      *gCb,
PTR       cb,           /* Parent control block */
S16       tmrEvnt       /* Timer event */
)
#else
PUBLIC S16 pjStartTmr (gCb, cb, tmrEvnt) 
PjCb      *gCb;
PTR       cb;           /* Parent control block */
S16       tmrEvnt;      /* Timer event */
#endif
{
#if (defined(PJ_SEC_ASYNC) || defined(PJ_CMP_ASYNC))
   PjDlRbCb   *dlRbCb;
   PjTxEnt  *txEnt;
#endif
   U16      wait;
   CmTimer  *tmr;
   U8       maxTmrs;
   CmTmrArg arg;
   /* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS_COMMENT
   PjL2MeasEvtCb *measEvtCb;
#endif

#ifdef LTE_L2_MEAS
    PjL2Cb *pjL2Cb = NULLP;
    PjCb   *tPjMeasCb = NULLP;
#endif
   PjUlRbCb   *ulRbCb;

   TRC2(pjStartTmr)

   RLOG1(L_DEBUG, "pjStartTmr(cb, tmrEvnt (%d)) ", tmrEvnt);

#if (defined(PJ_SEC_ASYNC) || defined(PJ_CMP_ASYNC))
   ulRbCb  = NULLP; 
   dlRbCb  = NULLP;
   txEnt   = NULLP;


#endif
   wait    = 0;
   tmr     = NULLP;
   maxTmrs = 0;

   switch (tmrEvnt)
   {
      case PJ_EVT_WAIT_KWU_BNDCFM:
      {
         PjKwuSapCb *kwuSap = (PjKwuSapCb *)cb;
         wait   = (U16)((kwuSap->bndTmrInt * SS_TICKS_SEC)/(gCb->pjGenCfg.timeRes * 1000));  /*KW_FIX*/
         if( (0 != kwuSap->bndTmrInt) && (0 == wait) )
         {
            wait = 1;
         }
         tmr    = &kwuSap->bndTmr;
         maxTmrs = PJ_MAX_KWUSAP_TMR;
         break;
      }
      case PJ_EVT_WAIT_UDX_BNDCFM:
      {
         PjUdxUlSapCb *udxSap = (PjUdxUlSapCb *)cb;
         wait   = (U16)((udxSap->bndTmrInt * SS_TICKS_SEC)/(gCb->pjGenCfg.timeRes * 1000));  /*KW_FIX*/
         if( (0 != udxSap->bndTmrInt) && (0 == wait) )
         {
            wait = 1;
         }
         tmr    = &udxSap->bndTmr;
         maxTmrs = PJ_MAX_UDXSAP_TMR;
         break;
      }    
#if (defined(PJ_SEC_ASYNC) || defined(PJ_CMP_ASYNC))
      case PJ_EVT_DL_OBD_TMR:
      {
         dlRbCb = (PjDlRbCb *)cb;
         tmr=&(dlRbCb->dlCb.obdTmr);
         maxTmrs = PJ_MAX_OBD_TMR;
         txEnt = pjDbmGetTxEnt(gCb,&(dlRbCb->dlCb.txBuf), dlRbCb->dlCb.nxtToSub);
         if ( txEnt != NULLP )
         {
            wait = gCb->pjGenCfg.obdTmrInt;
            dlRbCb->dlCb.obdCount = txEnt->count;
         }

         break;
      }
      case PJ_EVT_UL_OBD_TMR:
      {
         ulRbCb = (PjUlRbCb *)cb;
         tmr=&(ulRbCb->ulCb.obdTmr);
         maxTmrs = PJ_MAX_OBD_TMR;
         PJ_ULM_GET_SUBCNT(ulRbCb,(ulRbCb->ulCb.obdPdu),gCb);
         if(pjDbmGetRxEnt(gCb,&(ulRbCb->ulCb.recBuf), ulRbCb->ulCb.obdPdu) != NULLP )
         {
            wait = gCb->pjGenCfg.obdTmrInt;
         }

         break;
      } 
      case PJ_EVT_OBD_WAIT_TMR:
      {
         if (gCb->pjGenCfg.mode == LPJ_MODE_PDCP_DL)
         {
            PjDlUeCb  *dlUeCb = (PjDlUeCb *)cb;
            tmr=&(dlUeCb->libInfo.obdTmr);
         }
         else
         {
            PjUlUeCb  *ulUeCb = (PjUlUeCb *)cb;
            tmr=&(ulUeCb->libInfo.obdTmr);
         }
         maxTmrs = PJ_MAX_OBD_TMR;
         wait = gCb->pjGenCfg.obdWtTmrInt;
         break;
      }

#endif
      /* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS_COMMENT
      case PJ_EVT_L2_TMR:
      {
         measEvtCb = (PjL2MeasEvtCb *)cb;
         tmr=&(measEvtCb->l2Tmr);
         maxTmrs = PJ_L2_MAX_TIMERS;
         wait = (measEvtCb->l2TmrCfg.val * SS_TICKS_SEC)/(pjCb.genCfg.timeRes * 1000);
         if((measEvtCb->l2TmrCfg.val > 0) && wait == 0)
         {
            wait = 1;
         }
         break;
      }
#endif
#ifdef LTE_L2_MEAS
      case PJ_EVT_L2_MEAS_TMR:
      {
         tPjMeasCb = (PjCb      *)cb;
         pjL2Cb = &tPjMeasCb->u.ulCb->pjL2Cb;
         tmr=&(pjL2Cb->measTmr);
         maxTmrs = PJ_L2_MAX_TIMERS;
         wait = (pjL2Cb->measTmrCfg.val * SS_TICKS_SEC)/(gCb->pjGenCfg.timeRes * 1000);
         wait = 500;
         if((pjL2Cb->measTmrCfg.val > 0) && wait == 0)
         {
            wait = 1;
         }
         break;
      }
#endif
     case PJ_EVT_UL_REORD_TMR:
      {
         ulRbCb = (PjUlRbCb *)cb;
         tmr=&(ulRbCb->ulCb.tReordTmr);
         maxTmrs = PJ_MAX_UL_REORD_TMRS;
         PJ_TMR_CALCUATE_WAIT(wait, ulRbCb->reOrdrTmrVal, gCb->pjGenCfg.timeRes);
         break;
      }
      default:
      {
         RLOG0(L_ERROR, "Invalid Start tmr Evnt");
         RETVALUE(RFAILED);
      }
   } 

   if(wait != 0)
   {
      arg.tqCp   = &gCb->pjTqCp;
      arg.tq     = gCb->pjTq;
      arg.cb     = cb;
      arg.timers = tmr;
      arg.evnt   = tmrEvnt;
      arg.wait   = wait;
      arg.tNum   = 0;
      arg.max    = maxTmrs;

      cmPlcCbTq(&arg);
   }

   RETVALUE(ROK);
}

/**
 *
 * @brief Handler to stop timer
 *       
 *
 * @b Description
 *        This function is used to stop protocol timer, based on the timer event.
 *
 *
 *  @param[in] cb      Transmission Buffer control block 
 *  @param[in] tmrEvnt Timer event to be started 
 *
 * 
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
*/
#ifdef ANSI
PUBLIC S16 pjStopTmr
(
PjCb   *gCb,
PTR    cb,              /* Parent control block */
U8     tmrEvnt          /* Timer */
)
#else
PUBLIC S16 pjStopTmr (gCb, cb, tmrEvnt) 
PjCb   *gCb;
PTR    cb;              /* Parent control block */
U8     tmrEvnt;         /* Timer */
#endif
{
   CmTmrArg     arg;
   CmTimer      *tmr;
#if (defined(PJ_SEC_ASYNC) || defined(PJ_CMP_ASYNC))
   PjDlRbCb     *pjDlRbCb;
#endif
   PjKwuSapCb   *kwuSap;
   PjUdxUlSapCb   *udxSap;
#if (defined(PJ_SEC_ASYNC) || defined(PJ_CMP_ASYNC))
   PjDlUeCb       *dlUeCb;
   PjUlUeCb       *ulUeCb;
#endif
   /* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
   PjL2Cb *pjL2Cb = NULLP;
   PjCb   *tPjMeasCb = NULLP;
#endif
   U8           maxTimers;
   PjUlRbCb     *pjUlRbCb;

   TRC2(pjStopTmr)
   
   RLOG1(L_DEBUG, "pjStopTmr(cb, tmrEvnt(%d)) ", tmrEvnt);
   
   tmr = NULLP;
   maxTimers = 0;
#if (defined(PJ_SEC_ASYNC) || defined(PJ_CMP_ASYNC))
   pjUlRbCb = NULLP;
   pjDlRbCb    = NULLP;
   /* ueCb   = NULLP; */
#endif

   switch (tmrEvnt)
   {
      case PJ_EVT_WAIT_KWU_BNDCFM:
      {
         kwuSap = (PjKwuSapCb *)cb;
         tmr = &kwuSap->bndTmr;
         arg.max = PJ_MAX_KWUSAP_TMR;
         break;
      }
      case PJ_EVT_WAIT_UDX_BNDCFM:
      {
         udxSap = (PjUdxUlSapCb *)cb;
         tmr = &udxSap->bndTmr;
         arg.max = PJ_MAX_UDXSAP_TMR;
         break;
      }
#if (defined(PJ_SEC_ASYNC) || defined(PJ_CMP_ASYNC))
      case PJ_EVT_DL_OBD_TMR:
      {
         pjDlRbCb = (PjDlRbCb *)cb;
         tmr = &(pjDlRbCb->dlCb.obdTmr);
         maxTimers = PJ_MAX_OBD_TMR;
         break;
      }
      case PJ_EVT_UL_OBD_TMR:
      {
         pjUlRbCb = (PjUlRbCb *)cb;
         tmr = &(pjUlRbCb->ulCb.obdTmr);
         maxTimers = PJ_MAX_OBD_TMR;
         break;
      }
      case PJ_EVT_OBD_WAIT_TMR:
      {
         /*if (gCb->pjGenCfg.mode == LPJ_PDCP_MODE_DL)*/
         if (gCb->pjGenCfg.mode == LPJ_MODE_PDCP_DL)
         {
            dlUeCb = (PjDlUeCb *)cb;
            tmr  = &(dlUeCb->libInfo.obdTmr);
         }
         else
         {
            ulUeCb = (PjUlUeCb *)cb;
            tmr  = &(ulUeCb->libInfo.obdTmr);
         }
         maxTimers = PJ_MAX_OBD_TMR;
         break;
      }

#endif
     /* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS_COMMENT
      case PJ_EVT_L2_TMR:
      {
         measEvtCb = (PjL2MeasEvtCb *)cb;
         tmr  = &(measEvtCb->l2Tmr);
         maxTimers = PJ_L2_MAX_TIMERS;
         break;
      }
#endif
#ifdef LTE_L2_MEAS
      case PJ_EVT_L2_MEAS_TMR:
      {
         tPjMeasCb = (PjCb      *)cb;
         pjL2Cb = &tPjMeasCb->u.ulCb->pjL2Cb;
         tmr  = &(pjL2Cb->measTmr);
         maxTimers = PJ_L2_MAX_TIMERS;
         break;
      }
#endif
     case PJ_EVT_UL_REORD_TMR:
      {
         pjUlRbCb = (PjUlRbCb *)cb;
         tmr=&(pjUlRbCb->ulCb.tReordTmr);
         maxTimers = PJ_MAX_UL_REORD_TMRS;
         break;
      }
      default:
      {
         RLOG0(L_ERROR, "Invalid Stop tmr Evnt");
         RETVALUE(RFAILED);
      }
   }
   
   
   if (tmrEvnt != TMR0)
   {
      arg.tqCp    = &gCb->pjTqCp;
      arg.tq      = gCb->pjTq;
      arg.cb      = (PTR)cb;
      arg.timers  = tmr;
      arg.evnt    = tmrEvnt;
      arg.wait    = 0;
      arg.max     = maxTimers;
      arg.tNum    = 0;
      cmRmvCbTq(&arg);

   }
   
   RETVALUE(ROK);
}

/**
 *
 * @brief Handler to invoke events on expiry of timer.
 *       
 *
 * @b Description
 *        This function is used to handle expiry of timer,it invokes relevant functions.
 *
 *
 *  @param[in] cb        Transmission Buffer control block 
 *  @param[in] tmrType   Type of the timer to be stopped
 *
 * 
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
*/
#ifdef ANSI
PUBLIC S16 pjTmrExpiry
(
PTR       cb,           /* Parent control block */
S16       tmrEvnt       /* Timer event */
)
#else
PUBLIC S16 pjTmrExpiry (cb, tmrEvnt)
PTR       cb;           /* Parent control block */
S16       tmrEvnt;      /* Timer event */
#endif
{
#if (defined(PJ_SEC_ASYNC) || defined(PJ_CMP_ASYNC))
   PjDlRbCb      *dlRbCb        = NULLP;
   PjDlUeCb      *dlUeCb          = NULLP;
   PjUlUeCb      *ulUeCb          = NULLP;
#endif 
   PjUlRbCb      *ulRbCb        = NULLP; 
#ifdef LTE_L2_MEAS
   PjL2Cb *pjL2Cb = NULLP;
   PjCb   *tPjMeasCb = NULLP;
#endif
   /* pj005.201 added support for L2 Measurement */
   TRC2(pjTmrExpiry)

  switch (tmrEvnt)
   {
      case PJ_EVT_WAIT_KWU_BNDCFM:
      {
         pjBndTmrExpiry(tmrEvnt,cb);
         /* kw005.201 added missing break statement */
         break;
      }
      case PJ_EVT_WAIT_UDX_BNDCFM:
      {
         pjBndTmrExpiry(tmrEvnt,cb);
         /* kw005.201 added missing break statement */
         break;
      }
#if (defined(PJ_SEC_ASYNC) || defined(PJ_CMP_ASYNC))
      case PJ_EVT_DL_OBD_TMR:
      {
         PjCb *tPjCb;
         dlRbCb = (PjDlRbCb *)cb;

         if (dlRbCb->ueCb->inst >= PJ_MAX_PDCP_INSTANCES)
         {
            RETVALUE(RFAILED);
         }
         tPjCb = PJ_GET_PJCB(dlRbCb->ueCb->inst);

         pjDlmObdTmrExp(tPjCb,dlRbCb);
         break;
      }
      case PJ_EVT_UL_OBD_TMR:
      {
         PjCb *tPjCb;
         ulRbCb = (PjUlRbCb *)cb;

         if (ulRbCb->ueCb->inst >= PJ_MAX_PDCP_INSTANCES)
         {
            RETVALUE(RFAILED);
         }
         tPjCb = PJ_GET_PJCB(ulRbCb->ueCb->inst);
          pjUlmHdlObdTmrExpiry(tPjCb,ulRbCb);
         break;
      }
      case PJ_EVT_OBD_WAIT_TMR:
      {
         PjCb *tPjCb;
         tPjCb = (PjCb*)cb;
         if (tPjCb->pjGenCfg.mode == LPJ_MODE_PDCP_UL)
         {
            ulUeCb = (PjUlUeCb *)cb;
            if (ulUeCb->inst >= PJ_MAX_PDCP_INSTANCES)
            {
                RETVALUE(RFAILED);
            }
            tPjCb = PJ_GET_PJCB(ulUeCb->inst);
            pjUtlUlHdlInitObdTmrExp(tPjCb,ulUeCb);
            
         }
         else
         {
             dlUeCb = (PjDlUeCb *)cb;
             if (dlUeCb->inst >= PJ_MAX_PDCP_INSTANCES)
             {
                 RETVALUE(RFAILED);
             }
             tPjCb = PJ_GET_PJCB(dlUeCb->inst);
             pjUtlDlHdlInitObdTmrExp(tPjCb,dlUeCb);
         }
         break;
      }

#endif
#ifdef LTE_L2_MEAS
      case PJ_EVT_L2_MEAS_TMR:
      {
         tPjMeasCb = (PjCb      *)cb;
         pjL2Cb = &tPjMeasCb->u.ulCb->pjL2Cb;
         pjUtlMemCpuHdlTmrExp((PjCb *)cb,pjL2Cb);
      }
      break;
#endif
     case PJ_EVT_UL_REORD_TMR:
      {
         PjCb *tPjCb;
         ulRbCb = (PjUlRbCb *)cb;
         if (ulRbCb->ueCb->inst >= PJ_MAX_PDCP_INSTANCES)
         {
            RETVALUE(RFAILED);
         }
         tPjCb = PJ_GET_PJCB(ulRbCb->ueCb->inst);
         pjUlmHdlReordTmrExpiry(tPjCb, ulRbCb);
         break;
      }
      /* pj005.201 added support for L2 Measurement */
      default:
      {
         RETVALUE(RFAILED);
      }
   }

   RETVALUE(ROK);
}

/**
 *
 * @brief Handler to check if the timer is running
 *       
 *
 * @b Description
 *        This function is used to check if the timer is running at the given
 *        moment.
 *
 *
 *  @param[in] cb      Transmission Buffer control block 
 *  @param[in] tmrType   Type of the timer to be stopped 
 *
 * 
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 */
#ifdef ANSI
PUBLIC Bool pjChkTmr
(
PjCb      *gCb,
PTR       cb,           /* Parent control block */
S16       tmrEvnt       /* Timer event */
)
#else
PUBLIC Bool pjChkTmr(gCb, cb, tmrEvnt) 
PjCb      *gCb;
PTR       cb;           /* Parent control block */
S16       tmrEvnt;      /* Timer event */
#endif
{
#if (defined(PJ_SEC_ASYNC) || defined(PJ_CMP_ASYNC))
   PjDlUeCb  *dlUeCb         = NULLP;
   PjUlUeCb  *ulUeCb         = NULLP;
#endif
#ifdef LTE_L2_MEAS
   PjL2Cb *pjL2Cb = NULLP;
   PjCb   *tPjMeasCb = NULLP;
#endif
   Bool tmrRunning = FALSE;
   PjUlRbCb  *ulRbCb         = NULLP;

   TRC2(pjChkTmr)

   RLOG1(L_UNUSED, "pjChkTmr(cb, tmrEvnt(%d)) ", tmrEvnt);

   switch (tmrEvnt)
   {
#if (defined(PJ_SEC_ASYNC) || defined(PJ_CMP_ASYNC))
      case PJ_EVT_DL_OBD_TMR:
      {
         dlRbCb = (PjDlRbCb *)cb;
         if (dlRbCb->dlCb.obdTmr.tmrEvnt == PJ_EVT_DL_OBD_TMR)
         {
            tmrRunning = TRUE;
         }
         break;
      }
      case PJ_EVT_UL_OBD_TMR:
      {
          ulRbCb = (PjUlRbCb *)cb;
          if (ulRbCb->ulCb.obdTmr.tmrEvnt == PJ_EVT_UL_OBD_TMR)
          {
              tmrRunning = TRUE;
          }
          break;
      }
      case PJ_EVT_OBD_WAIT_TMR:
      {
          if(gCb->pjGenCfg.mode == LPJ_MODE_PDCP_UL)
          {
              ulUeCb = (PjUlUeCb *)cb;
              if (ulUeCb->inst >= PJ_MAX_PDCP_INSTANCES)
              {
                  RETVALUE(RFAILED);
              }
              if (ulUeCb->libInfo.obdTmr.tmrEvnt == PJ_EVT_OBD_WAIT_TMR)
              {
                  tmrRunning = TRUE;
              }
          }
          else
          {
              dlUeCb = (PjDlUeCb *)cb;
              if (dlUeCb->inst >= PJ_MAX_PDCP_INSTANCES)
              {
                  RETVALUE(RFAILED);
              }
              if (dlUeCb->libInfo.obdTmr.tmrEvnt == PJ_EVT_OBD_WAIT_TMR)
              {
                  tmrRunning =  TRUE;
              }
          }
          break;
      }
#endif
#ifdef LTE_L2_MEAS
      case PJ_EVT_L2_MEAS_TMR:
      {
         tPjMeasCb = (PjCb      *)cb;
         pjL2Cb = &tPjMeasCb->u.ulCb->pjL2Cb;
         if (pjL2Cb->measTmr.tmrEvnt == PJ_EVT_L2_MEAS_TMR)
         {
             tmrRunning = TRUE;
         }
         break;
      }
#endif
      case PJ_EVT_UL_REORD_TMR:
      {
         ulRbCb = (PjUlRbCb *)cb;
         if(PJ_EVT_UL_REORD_TMR == ulRbCb->ulCb.tReordTmr.tmrEvnt)
         {
             tmrRunning = TRUE;
         }
         break;
      }
      default:
      {

          RLOG0(L_ERROR, "Invalid Chk tmr Evnt");
          RETVALUE(RFAILED);
      }
   } 

   RETVALUE(tmrRunning);
}
/**
 *
 * @brief Private handler to invoke an event for bind timer expiry
 *       
 *
 * @b Description
 *        This function processes the RLC bind timer expiry. If the number of retry is
 *        less than the maximum retry counter, bind request is sent again, else an 
 *        alarm is raised to the layer manager.
 *
 *
 *  @param[in] cb      RB control block 
 *
 * 
 *   @return Void
 *      -# RETVOID
*/
#ifdef ANSI
PRIVATE Void pjBndTmrExpiry
(
S16       tmrEvnt,
PTR       cb            /* Parent control block */
)
#else
PRIVATE Void pjBndTmrExpiry(tmrEvnt, cb) 
S16       tmrEvnt;
PTR       cb;           /* Parent control block */
#endif
{
   PjCb          *tPjCb;
   PjUdxUlSapCb  *udxSap;
   PjKwuSapCb *kwuSapCb; 

   TRC2(pjBndTmrExpiry)

   if (tmrEvnt == PJ_EVT_WAIT_KWU_BNDCFM)
   {
      kwuSapCb = (PjKwuSapCb *) cb;

      tPjCb = PJ_GET_PJCB(kwuSapCb->pst.srcInst);
      if (kwuSapCb->state == PJ_SAP_BINDING)
      {
         if (kwuSapCb->retryCnt < PJ_MAX_SAP_BND_RETRY)
         {
            /* Send bind request */
            kwuSapCb->retryCnt++;

            /* start timer to wait for bind confirm */
            pjStartTmr(tPjCb, (PTR)kwuSapCb, PJ_EVT_WAIT_KWU_BNDCFM);

            PjLiKwuBndReq (&kwuSapCb->pst, kwuSapCb->suId, kwuSapCb->spId);
         }
         else
         {
            kwuSapCb->retryCnt = 0;
            kwuSapCb->state = PJ_SAP_CFG;

            /* Send alarm to the layer manager */
#ifdef LTE_L2_MEAS
            pjLmmSendAlarm(tPjCb,LCM_CATEGORY_INTERFACE, LCM_EVENT_BND_FAIL,
                           LCM_CAUSE_TMR_EXPIRED, 0, 0, 0);
#else
            pjLmmSendAlarm(tPjCb,LCM_CATEGORY_INTERFACE, LCM_EVENT_BND_FAIL,
                           LCM_CAUSE_TMR_EXPIRED, 0, 0);
#endif
         }
      }
   }
   else
   {
      udxSap = (PjUdxUlSapCb *)cb;

      tPjCb  = PJ_GET_PJCB (udxSap->pst.srcInst);

      if (udxSap->state == PJ_SAP_BINDING)
      {
         if (udxSap->retryCnt < PJ_MAX_SAP_BND_RETRY)
         {
            /* Send bind request */
            udxSap->retryCnt++;

            /* start timer to wait for bind confirm */
            pjStartTmr(tPjCb, (PTR)udxSap, PJ_EVT_WAIT_UDX_BNDCFM);

            PjUlUdxBndReq (&udxSap->pst, udxSap->suId, udxSap->spId);
         }
         else
         {
            udxSap->retryCnt = 0;
            udxSap->state = PJ_SAP_CFG;

            /* Send alarm to the layer manager */
#ifdef LTE_L2_MEAS
            pjLmmSendAlarm(tPjCb,LCM_CATEGORY_INTERFACE, LCM_EVENT_BND_FAIL,
                           LCM_CAUSE_TMR_EXPIRED, 0, 0, 0);
#else
            pjLmmSendAlarm(tPjCb,LCM_CATEGORY_INTERFACE, LCM_EVENT_BND_FAIL,
                           LCM_CAUSE_TMR_EXPIRED, 0, 0);
#endif
         }
      }

   }

   RETVOID;
}


  
/********************************************************************30**
  
         End of file
**********************************************************************/
