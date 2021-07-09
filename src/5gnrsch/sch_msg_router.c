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
 
     Name:     NR-SCH layer
  
     Type:     C source file
  
     Desc:     C source code SSI Interface Implementation
  
     File:     sch_msg_router.c
  
**********************************************************************/

/** @file sch_msg_router.c
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

/*
* @brief
*
* Function:
*   name : callFlowSchActvTsk 
*
*  @b Description:
*  Function used to print values of src, dest, message
*  received at the layer
*
*  @param[in] pst   - Pst Structure
*
*  @return void
*/

void callFlowSchActvTsk(Pst *pst)    
{
   char sourceTask[50];
   char destTask[50]="ENTSCH";
   char message[100];

   switch(pst->srcEnt)
   {
      case ENTMAC: /* When MAC sends a msg to Scheduler instance */
         {
            strcpy(sourceTask,"ENTMAC");
            switch(pst->event)
            {
#ifdef LCSCH
               case EVTINFDEDBOUPDTREQ:
                  strcpy(message,"EVTINFDEDBOUPDTREQ");
                  break;
               case EVTINFCMNBOUPDTREQ:
                  strcpy(message,"EVTINFCMNBOUPDTREQ");
                  break;   
               case EVTINFSFRECPIND:
                  strcpy(message,"EVTINFSFRECPIND");
                  break;
                  /*Fix: start: Inform UE delete to scheduler*/
               case EVTINFUEDELIND:
                  strcpy(message,"EVTINFUEDELIND");
                  break;
                  /*Fix: end: Inform UE delete to scheduler*/
#ifdef LTE_L2_MEAS
               case EVTINFL2MEASCFM:
                  strcpy(message,"EVTINFL2MEASCFM");
                  break;
               case EVTINFL2MEASSTOPCFM:
                  strcpy(message,"EVTINFL2MEASSTOPCFM");
                  break;
#endif
#endif            
               case EVENT_SLOT_IND_TO_SCH:
                  strcpy(message,"EVENT_SLOT_IND_TO_SCH");
                  break;
               default:
                  strcpy(message,"Invalid Event");
                  break;
            }
            break;
         }
      default:
         strcpy(sourceTask,"Invalid Source Entity Id");
         break;
   }
   DU_LOG("\nCall Flow: %s -> %s : %s\n", sourceTask, destTask, message);
}

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
S16 schActvTsk
(
Pst     *pst,                       /* post structure       */
Buffer  *mBuf                       /* message buffer       */
)
{

#ifdef CALL_FLOW_DEBUG_LOG
   callFlowSchActvTsk(pst);
#endif

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
