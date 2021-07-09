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
  
     File:     mac_msg_router.c
  
**********************************************************************/

/** @file mac_msg_router.c
@brief This file contains the implementation of callback functions 
registered with SSI during the LTE MAC Task initialization.
*/
/* header include files -- defines (.h) */
#include "common_def.h"
#include "rgu.h"           /* RGU defines */
#include "tfu.h"           /* RGU defines */
#include "lrg.h"           /* layer management defines for LTE-MAC */
#include "rgr.h"           /* layer management defines for LTE-MAC */
#include "crg.h"           /* CRG interface includes */
#include "rg_sch_inf.h"    /* SCH interface includes */
#include "rg_prg.h"        /* PRG interface includes */
#include "rg_env.h"        /* customisable defines and macros for LTE-MAC */
#include "rg.h"            /* defines and macros for MAC */
#include "rgr.h"            /* defines and macros for SCH */


/* header/extern include files (.x) */
#include "rgu.x"           /* RGU types */
#include "tfu.x"           /* RGU types */
#include "lrg.x"           /* layer management typedefs for MAC */
#include "rgr.x"           /* layer management typedefs for MAC */
#include "crg.x"           /* CRG interface typedes */
#include "rg_sch_inf.x"    /* SCH interface typedefs */
#include "rg_prg.x"        /*PRG interface includes*/
#include "du_app_mac_inf.h"
#include "rg.x"            /* typedefs for MAC */
#include "rlc_mac_inf.h"
#include "lwr_mac_upr_inf.h"

/**
 * @brief Task Activation callback function Entity SM. 
 *
 * @details
 *
 *     Function : rgHdlSMEvents
 *     
 *     Process Messages received from Entity SM
 *     
 *  @param[in]  Pst     *pst, Post structure of the primitive.     
 *  @param[in]  Buffer *mBuf, Packed primitive parameters in the buffer.
 *  @param[in]  Reason reason.
 *  @return  void
 **/
static inline void rgHdlSMEvents
(
Pst     *pst,                       /* post structure       */
Buffer  *mBuf                       /* message buffer       */
)
{
   switch(pst->event)
   {
#ifdef LCRGMILRG
      case EVTLRGCFGREQ:
         /* Process a config. request */
         cmUnpkLrgCfgReq(RgMiLrgCfgReq, pst, mBuf);
         break;
      case EVTMACSCHGENCFGREQ:
         /* Process a config. request */
         cmUnpkLrgSchCfgReq(MacSchGenCfgReq, pst, mBuf);
         break;
      case EVTLRGCNTRLREQ:
         /* Process a control request */
         cmUnpkLrgCntrlReq(RgMiLrgCntrlReq, pst, mBuf);
         break;
      case EVTLRGSSTAREQ:
         /* Process a status request  */
         cmUnpkLrgStaReq(RgMiLrgStaReq, pst, mBuf);
         break;
      case EVTLRGSTSREQ:
         /* Process a statistics request */
         cmUnpkLrgStsReq(RgMiLrgStsReq, pst, mBuf);
         break;
#endif /* LCRGMILRG */
      case EVENT_MAC_CELL_CONFIG_REQ:
         /* Process MAC cell config */
         unpackDuMacCellCfg(MacProcCellCfgReq, pst, mBuf);
         break;
      case EVENT_MAC_CELL_START:
	 /* Process MAC cell start request */
	 unpackMacCellStart(MacProcCellStart, pst, mBuf);
	 break;
      case EVENT_MAC_CELL_STOP:
	 /* Process MAC cell stop request */
	 unpackMacCellStop(MacProcCellStop, pst, mBuf);
	 break;
      case EVENT_MAC_DL_CCCH_IND:
	 /* Process DL CCCH Ind */
	 unpackMacDlCcchInd(MacProcDlCcchInd, pst, mBuf);
	 break;
      case EVENT_MAC_UE_CREATE_REQ:
	 /* Process Ue Create Request */
	 unpackMacUeCreateReq(MacProcUeCreateReq, pst, mBuf);
	 break;
      case EVENT_MAC_UE_RECONFIG_REQ:
	 /* Process Ue Reconfig Request */
	 unpackMacUeReconfigReq(MacProcUeReconfigReq, pst, mBuf);
	 break;
    case EVENT_MAC_UE_DELETE_REQ:
    {
       unpackMacUeDeleteReq(MacProcUeDeleteReq, pst, mBuf);
       break;
    }
    case EVENT_MAC_CELL_DELETE_REQ:
    {
       unpackMacCellDeleteReq(MacProcCellDeleteReq, pst, mBuf);
       break;
    }
    default:
         RG_FREE_MSG(mBuf);
         break;
   }
}


/**
 * @brief Task Activation callback function Entity NH. 
 *
 * @details
 *
 *     Function : rgHdlNHEvents
 *     
 *     Process Messages received from Entity NH
 *     
 *  @param[in]  Pst     *pst, Post structure of the primitive.     
 *  @param[in]  Buffer *mBuf, Packed primitive parameters in the buffer.
 *  @param[in]  Reason reason.
 *  @return  void
 **/
static inline void rgHdlNHEvents
(
Pst     *pst,                       /* post structure       */
Buffer  *mBuf                       /* message buffer       */
)
{
   switch(pst->event)
   {
#ifdef LCRGUICRG
      case EVTCRGBNDREQ:
         cmUnpkCrgBndReq(RgUiCrgBndReq, pst, mBuf);
         break;
      case EVTCRGUBNDREQ:
         cmUnpkCrgUbndReq(RgUiCrgUbndReq, pst, mBuf);
         break;
      case EVTCRGCFGREQ:
         cmUnpkCrgCfgReq(RgUiCrgCfgReq, pst, mBuf);
         break;
#endif            
      default:
         RG_FREE_MSG(mBuf);
         break;
   }
}

/**
 * @brief Task Activation callback function Entity KW. 
 *
 * @details
 *
 *     Function : rgHdlKWEvents
 *     
 *     Process Messages received from Entity KW
 *     
 *  @param[in]  Pst     *pst, Post structure of the primitive.     
 *  @param[in]  Buffer *mBuf, Packed primitive parameters in the buffer.
 *  @param[in]  Reason reason.
 *  @return  void
 **/
static inline void rgHdlKWEvents
(
Pst     *pst,                       /* post structure       */
Buffer  *mBuf                       /* message buffer       */
)
{
   switch(pst->event)
   {
#ifdef LCRGUIRGU
      case EVTRGUBNDREQ:
         cmUnpkRguBndReq(RgUiRguBndReq, pst, mBuf);
         break;
      case EVTRGUUBNDREQ:
         cmUnpkRguUbndReq(RgUiRguUbndReq, pst, mBuf);
         break;
      case EVENT_DL_DATA_TO_MAC:
         unpackRlcDlData(MacProcRlcDlData, pst, mBuf);
         break;
      case EVENT_BO_STATUS_TO_MAC:
         unpackRlcBoStatus(MacProcRlcBoStatus, pst, mBuf);
         break;
#ifdef LTE_L2_MEAS

      case EVTRGUL2MULTHRPMEASREQ:
         cmUnpkRguL2MUlThrpMeasReq(RgUiRguL2MUlThrpMeasReq, pst,mBuf);
         break;

#endif
#endif            
      default:
         RG_FREE_MSG(mBuf);
         break;
   }
}

/**
 * @brief Task Activation callback function Entity TF. 
 *
 * @details
 *
 *     Function : rgHdlTFEvents
 *     
 *     Process Messages received from Entity TF
 *     
 *  @param[in]  Pst     *pst, Post structure of the primitive.     
 *  @param[in]  Buffer *mBuf, Packed primitive parameters in the buffer.
 *  @param[in]  Reason reason.
 *  @return  void
 **/
static inline void rgHdlTFEvents
(
Pst     *pst,                       /* post structure       */
Buffer  *mBuf                       /* message buffer       */
)
{
   switch(pst->event)
   {
      case EVENT_SLOT_IND_TO_MAC:
	 unpackSlotInd(fapiMacSlotInd, pst, mBuf);
         break;
      case EVENT_STOP_IND_TO_MAC:
	 unpackStopInd(fapiMacStopInd, pst, mBuf);
	 break;
      case EVENT_RACH_IND_TO_MAC:
	 unpackRachInd(fapiMacRachInd, pst, mBuf);
	 break;
      case EVENT_CRC_IND_TO_MAC:
	 unpackCrcInd(fapiMacCrcInd, pst, mBuf);
	 break;
      case EVENT_RX_DATA_IND_TO_MAC:
	 unpackRxDataInd(fapiMacRxDataInd, pst, mBuf);
	 break;
      case EVENT_UCI_IND_TO_MAC:
	 unpackUciInd(FapiMacUciInd, pst, mBuf);
	 break;
      default:
         RG_FREE_MSG(mBuf);
         break;
   }
}


/**
 * @brief Task Activation callback function Entity RG SCH. 
 *
 * @details
 *
 *     Function : rgHdlRGEvents
 *     
 *     Process Messages received from Entity RG SCH
 *     
 *  @param[in]  Pst     *pst, Post structure of the primitive.     
 *  @param[in]  Buffer *mBuf, Packed primitive parameters in the buffer.
 *  @param[in]  Reason reason.
 *  @return  void
 **/
static inline void rgHdlRGEvents
(
Pst     *pst,                       /* post structure       */
Buffer  *mBuf                       /* message buffer       */
)
{
   switch(pst->event)
   {
#ifdef LCRG
      case EVTINFCELLREGREQ:
         cmUnpkSchMacCellRegReq(RgSchMacCellRegReq, pst, mBuf);
         break;
      case EVTINFSFALLOCREQ:
         cmUnpkSchMacSfAllocReq(RgSchMacSfAllocReq, pst, mBuf);
         break;
      case EVTINFRLSHQREQ:
         cmUnpkSchMacRlsHqReq(RgSchMacRlsHqReq, pst, mBuf);
         break;
      case EVTINFHQENTRESET:
         cmUnpkSchMacRstHqEntReq(RgSchMacRstHqEntReq, pst, mBuf);
         break;
      case EVTINFRLSRNTIREQ:
         cmUnpkSchMacRlsRntiReq(RgSchMacRlsRntiReq, pst, mBuf);
         break;
#ifdef LTEMAC_SPS
      case EVTINFSPSLCREG:
         cmUnpkSchMacSpsLcRegReq(RgSchMacSpsLcRegReq, pst, mBuf);
         break;
      case EVTINFSPSLCDEREG:
         cmUnpkSchMacSpsLcDeregReq(RgSchMacSpsLcDeregReq, pst, mBuf);
         break;
      case EVTINFSPSRESET:
         cmUnpkSchMacUlSpsResetReq(RgSchMacUlSpsResetReq, pst, mBuf);
         break;
#endif /* LTEMAC_SPS */
#ifdef LTE_L2_MEAS
      case EVTINFL2MEASREQ:
         cmUnpkSchMacL2MeasReq(RgSchMacL2MeasReq, pst, mBuf);
         break;
      case EVTINFL2MEASSENDREQ :
         cmUnpkSchMacL2MeasSendReq(RgSchMacL2MeasSendReq, pst , mBuf);
         break;
      case EVTINFL2MEASSTOPREQ:
         cmUnpkSchMacL2MeasStopReq(RgSchMacL2MeasStopReq, pst , mBuf);
         break;
#endif/* LTE_L2_MEAS */
      case EVTINFLCGREG:
         cmUnpkSchMacLcgRegReq(RgSchMacLcgRegReq, pst, mBuf);
         break;
#endif            
#if defined(LTE_ADV) && defined(LCPRG)
      case EVTPRGUESCELLCFGREQ:
      {
         cmUnpkPrgPMacSMacUeSCellCfgReq(RgPrgPMacSMacUeSCellCfgReq, pst, mBuf);
      }
      break;
      case EVTPRGUESCELLCFGCFM:
      case EVTPRGUESCELLLCHMODCFM:
      case EVTPRGUESCELLLCHDELCFMDEL:
      case EVTPRGUESCELLLCHADDCFM:
      {
         cmUnpkPrgSMacPMacCfgCfm(RgPrgSMacPMacCfgCfm, pst, mBuf);
      }
      break;
      case EVTPRGUESCELLDELREQ:
      {
         cmUnpkPrgPMacSMacUeSCellDelReq(RgPrgPMacSMacUeSCellDelReq, pst, mBuf);
      }
      break;
      case EVTPRGUESCELLLCHMODREQ:
      {
         cmUnpkPrgPMacSMacUeSCellLchModReq(RgPrgPMacSMacUeSCellLchModReq, pst,
                                           mBuf);
      }
      break;
      case EVTPRGUESCELLLCHDELREQ:
      {
         cmUnpkPrgPMacSMacUeSCellLchDelReq(RgPrgPMacSMacUeSCellLchDelReq, pst,
                                           mBuf);
      }
      break;
      case EVTPRGUESCELLLCHADDREQ:
      {
         cmUnpkPrgPMacSMacUeSCellLchAddReq(RgPrgPMacSMacUeSCellLchAddReq, pst,
                                           mBuf);
      }
      break;

#endif
      default:
      {
         RG_FREE_MSG(mBuf);
         break;
      }

   }
}

/**
* @brief Function prints src, dest, msg infor about all the msgs that received.
*
* @details
*
*     Function : callFlowRgActvTsk 
*
*     Function prints src, dest, msg infor about all the msgs that received
*
*  @param[in]  Pst     *pst, Post structure of the primitive.
*
*  @return  void
**/

void callFlowRgActvTsk(Pst *pst)
{
   char sourceTask[50];
   char destTask[50]="ENTMAC";
   char message[100];

   switch(pst->srcEnt)
   {
      case ENTDUAPP:
         {
            strcpy(sourceTask,"ENTDUAPP");
            switch(pst->event)
            {
#ifdef LCRGMILRG
               case EVTLRGCFGREQ:
                  strcpy(message,"EVTLRGCFGREQ");
                  break;
               case EVTMACSCHGENCFGREQ:
                  strcpy(message,"EVTMACSCHGENCFGREQ");
                  break;
               case EVTLRGCNTRLREQ:
                  strcpy(message,"EVTLRGCNTRLREQ");
                  break;
               case EVTLRGSSTAREQ:
                  strcpy(message,"EVTLRGSSTAREQ");
                  break;
               case EVTLRGSTSREQ:
                  strcpy(message,"EVTLRGSTSREQ");
                  break;
#endif /* LCRGMILRG */
               case EVENT_MAC_CELL_CONFIG_REQ:
                  strcpy(message,"EVENT_MAC_CELL_CONFIG_REQ");
                  break;
               case EVENT_MAC_CELL_START:
                  strcpy(message,"EVENT_MAC_CELL_START");
                  break;
               case EVENT_MAC_CELL_STOP:
                  strcpy(message,"EVENT_MAC_CELL_STOP");
                  break;
               case EVENT_MAC_DL_CCCH_IND:
                  strcpy(message,"EVENT_MAC_DL_CCCH_IND");
                  break;
               case EVENT_MAC_UE_CREATE_REQ:
                  strcpy(message,"EVENT_MAC_UE_CREATE_REQ");
                  break;
               case EVENT_MAC_UE_RECONFIG_REQ:
                  strcpy(message,"EVENT_MAC_UE_RECONFIG_REQ");
                  break;
               case EVENT_MAC_UE_DELETE_REQ:
                  strcpy(message,"EVENT_MAC_UE_DELETE_REQ");
                  break;
               case EVENT_MAC_CELL_DELETE_REQ:
                  strcpy(message,"EVENT_MAC_CELL_DELETE_REQ");
                  break;
               default:
                  strcpy(message,"Invalid Event");
                  break;
            }
            break;
         }
      case ENTRLC:
         { 
            strcpy(sourceTask,"ENTRLC");
            switch(pst->event)
            {
#ifdef LCRGUIRGU
               case EVTRGUBNDREQ:
                  strcpy(message,"EVTRGUBNDREQ");
                  break;
               case EVTRGUUBNDREQ:
                  strcpy(message,"EVTRGUUBNDREQ");
                  break;
               case EVENT_DL_DATA_TO_MAC:
                  strcpy(message,"EVENT_DL_DATA_TO_MAC");
                  break;
               case EVENT_BO_STATUS_TO_MAC:
                  strcpy(message,"EVENT_BO_STATUS_TO_MAC");
                  break;
#endif
               default:
                  strcpy(message,"Invalid Event");
                  break;
            }

            break;
         }
      case ENTLWRMAC:
         {
            strcpy(sourceTask,"ENTLWRMAC");
            switch(pst->event)
            {
               case EVENT_SLOT_IND_TO_MAC:
                  strcpy(message,"EVENT_SLOT_IND_TO_MAC");
                  break;
               case EVENT_STOP_IND_TO_MAC:
                  strcpy(message,"EVENT_STOP_IND_TO_MAC");
                  break;
               case EVENT_RACH_IND_TO_MAC:
                  strcpy(message,"EVENT_RACH_IND_TO_MAC");
                  break;
               case EVENT_CRC_IND_TO_MAC:
                  strcpy(message,"EVENT_CRC_IND_TO_MAC");
                  break;
               case EVENT_RX_DATA_IND_TO_MAC:
                  strcpy(message,"EVENT_RX_DATA_IND_TO_MAC");
                  break;
               case EVENT_UCI_IND_TO_MAC:
                  strcpy(message,"EVENT_UCI_IND_TO_MAC");
                  break;
               default:
                  strcpy(message,"Invalid Event");
                  break;
            }

            break;
         }
      case ENTMAC: /* When scheduler instance sends msg to MAC */
         {
            strcpy(sourceTask,"ENTMAC");
            switch(pst->event)
            {
#ifdef LCRG
               case EVTINFCELLREGREQ:
                  strcpy(message,"EVTINFCELLREGREQ");
                  break;
               case EVTINFSFALLOCREQ:
                  strcpy(message,"EVTINFSFALLOCREQ");
                  break;
               case EVTINFRLSHQREQ:
                  strcpy(message,"EVTINFRLSHQREQ");
                  break;
               case EVTINFHQENTRESET:
                  strcpy(message,"EVTINFHQENTRESET");
                  break;
               case EVTINFRLSRNTIREQ:
                  strcpy(message,"EVTINFRLSRNTIREQ");
                  break;
#ifdef LTEMAC_SPS
               case EVTINFSPSLCREG:
                  strcpy(message,"EVTINFSPSLCREG");
                  break;
               case EVTINFSPSLCDEREG:
                  strcpy(message,"EVTINFSPSLCDEREG");
                  break;
               case EVTINFSPSRESET:
                  strcpy(message,"EVTINFSPSRESET");
                  break;
#endif /* LTEMAC_SPS */
               case EVTINFLCGREG:
                  strcpy(message,"EVTINFLCGREG");
                  break;
#endif            
#if defined(LTE_ADV) && defined(LCPRG)
               case EVTPRGUESCELLCFGREQ:
                  strcpy(message,"EVTPRGUESCELLCFGREQ");
                  break;
               case EVTPRGUESCELLCFGCFM:
                  strcpy(message,"EVTPRGUESCELLCFGCFM");
                  break;
               case EVTPRGUESCELLLCHMODCFM:
                  strcpy(message,"EVTPRGUESCELLLCHMODCFM");
                  break;
               case EVTPRGUESCELLLCHDELCFMDEL:
                  strcpy(message,"EVTPRGUESCELLLCHDELCFMDEL");
                  break;
               case EVTPRGUESCELLLCHADDCFM:
                  strcpy(message,"EVTPRGUESCELLLCHADDCFM");
                  break;
               case EVTPRGUESCELLDELREQ:
                  strcpy(message,"EVTPRGUESCELLDELREQ");
                  break;
               case EVTPRGUESCELLLCHMODREQ:
                  strcpy(message,"EVTPRGUESCELLLCHMODREQ");
                  break;
               case EVTPRGUESCELLLCHDELREQ:
                  strcpy(message,"EVTPRGUESCELLLCHDELREQ");
                  break;
               case EVTPRGUESCELLLCHADDREQ:
                  strcpy(message,"EVTPRGUESCELLLCHADDREQ");
                  break;

#endif
               default:
                  strcpy(message,"Invalid Event");
                  break;
            }
            break;
         }

      default:
         {
            strcpy(sourceTask,"Invalid Source Entity Id");
            break;
         }
   }
   DU_LOG("\nCall Flow: %s -> %s : %s\n", sourceTask, destTask, message);
}


/**********************************************************************

/**
 * @brief Task Activation callback function. 
 *
 * @details
 *
 *     Function : rgActvTsk
 *     
 *     Primitives invoked by MAC's users/providers through
 *     a loosely coupled interface arrive here by means of 
 *     SSI's message handling. This API is registered with
 *     SSI during the Task Registration of MAC.
 *     
 *  @param[in]  Pst     *pst, Post structure of the primitive.     
 *  @param[in]  Buffer *mBuf, Packed primitive parameters in the buffer.
 *  @param[in]  Reason reason.
 *  @return  S16
 *      -# ROK
 **/
S16 rgActvTsk
(
Pst     *pst,                       /* post structure       */
Buffer  *mBuf                       /* message buffer       */
)
{

#ifdef CALL_FLOW_DEBUG_LOG
   callFlowRgActvTsk(pst);
#endif

   switch(pst->srcEnt)
   {
      /* The originator of this message is the stack manager,
       * unpack and go to the respective primitive processing function */
      case ENTDUAPP:
          rgHdlSMEvents(pst, mBuf);
           break;
      case ENTNH:
          rgHdlNHEvents(pst, mBuf);
          break;
      case ENTRLC:
          rgHdlKWEvents(pst, mBuf);
          break;
      case ENTLWRMAC:
          rgHdlTFEvents(pst, mBuf);
          break;
      case ENTMAC: /* When scheduler instance sends msg to MAC */
          rgHdlRGEvents(pst, mBuf);
          break;
       default:
          RG_FREE_MSG(mBuf);
          break;
   }
   ODU_EXIT_TASK();
   return ROK;
}/* end of rgActvTsk */


/**********************************************************************
 
         End of file
**********************************************************************/
