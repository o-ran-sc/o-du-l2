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
  
     File:     rg_ex_ms.c 
  
**********************************************************************/

/** @file rg_ex_ms.c
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
#ifdef ANSI
PRIVATE INLINE void rgHdlSMEvents
(
Pst     *pst,                       /* post structure       */
Buffer  *mBuf                       /* message buffer       */
)
#else
PRIVATE INLINE void rgHdlSMEvents(pst, mBuf)
Pst     *pst;                       /* post structure       */
Buffer  *mBuf;                      /* message buffer       */
#endif
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
      case EVENT_MAC_CELL_START_REQ:
	 /* Process MAC cell start request */
	 unpackMacCellStartReq(MacProcCellStartReq, pst, mBuf);
	 break;
      case EVENT_MAC_CELL_STOP_REQ:
	 /* Process MAC cell stop request */
	 unpackMacCellStopReq(MacProcCellStopReq, pst, mBuf);
	 break;
      case EVENT_MAC_DL_CCCH_IND:
	 /* Process DL CCCH Ind */
	 unpackMacDlCcchInd(MacProcDlCcchInd, pst, mBuf);
	 break;
      case EVENT_MAC_UE_CONFIG_REQ:
	 /* Process Ue Create Request */
	 unpackMacUeCreateReq(MacProcUeCreateReq, pst, mBuf);
	 break;
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
#ifdef ANSI
PRIVATE INLINE void rgHdlNHEvents
(
Pst     *pst,                       /* post structure       */
Buffer  *mBuf                       /* message buffer       */
)
#else
PRIVATE INLINE void rgHdlNHEvents(pst, mBuf)
Pst     *pst;                       /* post structure       */
Buffer  *mBuf;                      /* message buffer       */
#endif
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
#ifdef ANSI
PRIVATE INLINE void rgHdlKWEvents
(
Pst     *pst,                       /* post structure       */
Buffer  *mBuf                       /* message buffer       */
)
#else
PRIVATE INLINE void rgHdlKWEvents(pst, mBuf)
Pst     *pst;                       /* post structure       */
Buffer  *mBuf;                      /* message buffer       */
#endif
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
#ifdef ANSI
PRIVATE INLINE void rgHdlTFEvents
(
Pst     *pst,                       /* post structure       */
Buffer  *mBuf                       /* message buffer       */
)
#else
PRIVATE INLINE void rgHdlTFEvents(pst, mBuf)
Pst     *pst;                       /* post structure       */
Buffer  *mBuf;                      /* message buffer       */
#endif
{
   switch(pst->event)
   {
#if  (defined(LCRGLITFU) || defined(LWLCRGLITFU))
      case EVTTFUBNDCFM:
         cmUnpkTfuBndCfm(RgLiTfuBndCfm, pst, mBuf);
         break;
      case EVTTFUDATIND:
         cmUnpkTfuDatInd(RgLiTfuDatInd, pst, mBuf);
         break;
#if defined(TENB_T2K3K_SPECIFIC_CHANGES) && defined(LTE_TDD)
      case EVTTFUNONRTIND:
         cmUnpkTfuNonRtInd(RgLiTfuNonRtInd, pst, mBuf);
         break;
#endif
#endif            
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
#ifdef ANSI
PRIVATE INLINE void rgHdlRGEvents
(
Pst     *pst,                       /* post structure       */
Buffer  *mBuf                       /* message buffer       */
)
#else
PRIVATE INLINE void rgHdlRGEvents(pst, mBuf)
Pst     *pst;                       /* post structure       */
Buffer  *mBuf;                      /* message buffer       */
#endif
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
#ifdef ANSI
S16 rgActvTsk
(
Pst     *pst,                       /* post structure       */
Buffer  *mBuf                       /* message buffer       */
)
#else
S16 rgActvTsk(pst, mBuf)
Pst     *pst;                       /* post structure       */
Buffer  *mBuf;                      /* message buffer       */
#endif
{
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
