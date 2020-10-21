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
  
     Desc:     C source code SSI Interface Implementation
  
     File:     rg_sch_ex_ms.c 
  
**********************************************************************/

/** @file rg_ex_ms.c
@brief This file contains the implementation of callback functions 
registered with SSI during the LTE MAC Task initialization.
*/
/* header include files -- defines (.h) */
#include "common_def.h"
#include "tfu.h"           /* RGU defines */
#include "lrg.h"           /* layer management defines for LTE-MAC */
#include "rgr.h"           /* layer management defines for LTE-MAC */
#include "rgm.h"           /* layer management defines for LTE-MAC */
#include "rg_env.h"        /* customisable defines and macros for LTE-MAC */
#include "rg_sch_err.h"        /* defines and macros for Scheduler */
#include "rg_sch_inf.h"        /* defines and macros for Scheduler */
#include "rg_sch.h"        /* defines and macros for Scheduler */


/* header/extern include files (.x) */
#include "tfu.x"           /* RGU types */
#include "lrg.x"           /* layer management typedefs for MAC */
#include "rgr.x"           /* layer management typedefs for MAC */
#include "rgm.x"           /* layer management typedefs for MAC */
#include "rg_sch_inf.x"        /* typedefs for Scheduler */
#include "rg_sch.x"        /* typedefs for Scheduler */
#include "mac_sch_interface.h"



/**
 * @brief Task Activation callback function. 
 *
 * @details
 *
 *     Function : schActvTsk
 *     
 *     Primitives invoked by MAC's users/providers through
 *     a loosely coupled interface arrive here by means of 
 *     SSI's message handling. This API is registered with
 *     SSI during the Task Registration of MAC.
 *     
 *  @param[in]  Pst     *pst, post structure of the Primitive.     
 *  @param[in]  Buffer *mBuf, Packed primitive parameters in the buffer.
 *  @param[in]  Reason reason.
 *  @return  S16
 *      -# ROK
 **/
#ifdef ANSI
S16 schActvTsk
(
Pst     *pst,                       /* post structure       */
Buffer  *mBuf                       /* message buffer       */
)
#else
S16 schActvTsk(pst, mBuf)
Pst     *pst;                       /* post structure       */
Buffer  *mBuf;                      /* message buffer       */
#endif
{
   switch(pst->srcEnt)
   {
      /* The originator of this message is the stack manager,
       * unpack and go to the respective primitive processing function */
      case ENTSM:
         switch(pst->event)
         {
#ifdef LCRGMILRG
            case EVTMACSCHGENCFGREQ:
               /* Process a config. request */
               cmUnpkLrgSchCfgReq(SchProcGenCfgReq, pst, mBuf);
               break;
            case EVTLRGSCHCNTRLREQ:
               /* Process a control request */
               cmUnpkLrgSchCntrlReq(RgMiLrgSchCntrlReq, pst, mBuf);
               break;
            case EVTLRGSCHSTAIND:
               /* Process a control request */
               cmUnpkLrgSchStaInd(RgMiLrgSchStaInd, pst, mBuf);
               break;
#ifdef LTE_L2_MEAS
            case EVTLRGSCHL2MEASREQ:
               /* Process L2 Measurement request */
               cmUnpkLrgSchL2MeasReq(RgMiLrgSchL2MeasReq, pst, mBuf);
               break;
            case EVTLRGSCHL2MEASSTOPREQ:
               /* Process L2 Measurement Stop request */
               cmUnpkLrgSchL2MeasStopReq(RgMiLrgSchL2MeasStopReq, pst, mBuf);
               break;
            case EVTLRGSCHL2MEASSENDREQ:
               /* Process L2 Measurement Send  request */
               cmUnpkLrgSchL2MeasSendReq(RgMiLrgSchL2MeasSendReq, pst, mBuf);
               break;
#endif
#endif /* LCRGMILRG */
            default:
               RGSCH_FREE_MSG(mBuf);
               break;
         }
         break;
     case ENTNX:
         switch(pst->event)
         {
#ifdef LCRGUIRGR
            case EVTRGRBNDREQ:
               cmUnpkRgrBndReq(RgUiRgrBndReq, pst, mBuf);
               break;
            case EVTRGRUBNDREQ:
               cmUnpkRgrUbndReq(RgUiRgrUbndReq, pst, mBuf);
               break;
#ifdef RGR_SI_SCH
            case EVTRGRSICFGREQ:
               cmUnpkRgrSiCfgReq(RgUiRgrSiCfgReq, pst, mBuf);
               break;
            case EVTRGRWARNINGSICFGREQ:
               cmUnpkRgrWarningSiCfgReq(RgUiRgrWarningSiCfgReq, pst, mBuf);
               break;

            case EVTRGRWARNINGSISTOPREQ:
               cmUnpkRgrWarningSiStopReq(RgUiRgrWarningSiStopReq, pst, mBuf);
               break;
#endif/*RGR_SI_SCH */
               /* LTE_ADV_FLAG_REMOVED_START */
            case EVTRGRLOADINFREQ:
               cmUnpkRgrLoadInfReq(RgUiRgrLoadInfReq, pst, mBuf);
               break;
               /* LTE_ADV_FLAG_REMOVED_END */
#endif            
            default:
               RGSCH_FREE_MSG(mBuf);
               break;
         }
         break;
      case ENTLWRMAC:
         switch(pst->event)
         {
/*#ifdef LCRGLITFU L2Split */
#if (defined(LCRGLITFU) || defined(LWLCRGLITFU)) 
            case EVTTFUSCHBNDCFM:
               cmUnpkTfuBndCfm(RgLiTfuSchBndCfm, pst, mBuf);
               break;
            case EVTTFURAREQIND:
               cmUnpkTfuRaReqInd(RgLiTfuRaReqInd, pst, mBuf);
               break;
            case EVTTFUULCQIIND:
               cmUnpkTfuUlCqiInd(RgLiTfuUlCqiInd, pst, mBuf);
               break;
            case EVTTFUHQIND:
               cmUnpkTfuHqInd(RgLiTfuHqInd, pst, mBuf);
               break;
            case EVTTFUSRIND:
               cmUnpkTfuSrInd(RgLiTfuSrInd, pst, mBuf);
               break;
            case EVTTFUDLCQIIND:
               cmUnpkTfuDlCqiInd(RgLiTfuDlCqiInd, pst, mBuf);
               break;
            case EVTTFUCRCIND:
               /*cmUnpkTfuCrcIndInfo(RgLiTfuCrcInd, pst, mBuf); */
               cmUnpkTfuCrcInd(RgLiTfuCrcInd, pst, mBuf);
               break;
            case EVTTFUTIMINGADVIND:
               cmUnpkTfuTimingAdvInd(RgLiTfuTimingAdvInd, pst, mBuf);
               break;
            case EVTTFUPUCCHDELPWR:
               cmUnpkTfuPucchDeltaPwr(RgLiTfuPucchDeltaPwrInd, pst, mBuf);
               break;
            case EVTTFUDOAIND:
               cmUnpkTfuDoaInd(RgLiTfuDoaInd, pst, mBuf);
               break;
#ifdef TFU_UPGRADE
            case EVTTFURAWCQIIND:
               cmUnpkTfuRawCqiInd(RgLiTfuRawCqiInd, pst, mBuf);
               break;
            case EVTTFUSRSIND:
               cmUnpkTfuSrsInd(RgLiTfuSrsInd, pst, mBuf);
               break;
#endif 
               /*LAA: Error Indication on SCell*/
            case EVTTFUERRIND:
               cmUnpkTfuErrInd(RgLiTfuErrInd, pst, mBuf);
               break;
#endif            
            default:
               RGSCH_FREE_MSG(mBuf);
               break;
         }
         break;
      case ENTMAC: /* When MAC sends a msg to Scheduler instance */
         switch(pst->event)
         {
#ifdef LCSCH
            case EVTINFDEDBOUPDTREQ:
               cmUnpkMacSchDedBoUpdtReq(RgMacSchDedBoUpdtReq, pst, mBuf);
               break;
            case EVTINFCMNBOUPDTREQ:
               cmUnpkMacSchCmnBoUpdtReq(RgMacSchCmnBoUpdtReq, pst, mBuf);
               break;   
            case EVTINFSFRECPIND:
               cmUnpkMacSchSfRecpInd(RgMacSchSfRecpInd, pst, mBuf);
               break;
               /*Fix: start: Inform UE delete to scheduler*/
            case EVTINFUEDELIND:
               cmUnpkMacSchUeDelInd(RgMacSchUeDelInd, pst, mBuf);
               break;
            /*Fix: end: Inform UE delete to scheduler*/
#ifdef LTE_L2_MEAS
            case EVTINFL2MEASCFM:
               cmUnpkMacSchL2MeasCfm(RgMacSchL2MeasCfm, pst, mBuf);
               break;
            case EVTINFL2MEASSTOPCFM:
               cmUnpkMacSchL2MeasCfm(RgMacSchL2MeasStopCfm, pst, mBuf);
               break;
#endif
#endif            
            case EVENT_SLOT_IND_TO_SCH:
               unpackMacSchSlotInd(MacSchSlotInd, pst, mBuf);
               break;
            default:
               RGSCH_FREE_MSG(mBuf);
               break;
         }
         break;
      case ENTRM: /* When RRM sends msg to scheduler */
         switch(pst->event)
         {
            case EVTRGMBNDREQ:
               cmUnpkRgmBndReq(RgUiRgmBndReq, pst, mBuf); 
               break;
            case EVTRGMUBNDREQ:
               cmUnpkRgmUbndReq(RgUiRgmUbndReq, pst, mBuf); 
               break;
            case EVTRGMCFGPRBRPRT:
               cmUnpkRgmCfgPrbRprt(RgUiRgmCfgPrbRprt, pst, mBuf);
               break;
            default:
               RGSCH_FREE_MSG(mBuf);
               break;
         }
         break;
      default:
          RGSCH_FREE_MSG(mBuf);
          break;
   }
   ODU_EXIT_TASK();
   return ROK;
}/* end of schActvTsk */


/**********************************************************************
 
         End of file
**********************************************************************/
