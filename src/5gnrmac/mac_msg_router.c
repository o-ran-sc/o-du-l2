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
#include "mac_sch_interface.h"
#include "mac.h"
#include "mac_ue_mgr.h"
#include "mac_harq_dl.h"

/**
 * @brief Task Activation callback function Entity DU APP. 
 *
 * @details
 *
 *     Function : MacHdlDuappEvents
 *     
 *     Process Messages received from Entity DU APP
 *     
 *  @param[in]  Pst     *pst, Post structure of the primitive.     
 *  @param[in]  Buffer *mBuf, Packed primitive parameters in the buffer.
 *  @param[in]  Reason reason.
 *  @return  void
 **/
void MacHdlDuappEvents(Pst *pst, Buffer *mBuf)
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
      case EVTLRGSTSREQ:
         /* Process a statistics request */
         cmUnpkLrgStsReq(RgMiLrgStsReq, pst, mBuf);
         break;
#endif /* LCRGMILRG */
      case EVENT_MAC_CELL_CONFIG_REQ:
         {
            /* Process MAC cell config */
            unpackDuMacCellCfg(MacProcCellCfgReq, pst, mBuf);
            break;
         }
      case EVENT_MAC_CELL_START:
         {
            /* Process MAC cell start request */
            unpackMacCellStart(MacProcCellStart, pst, mBuf);
            break;
         }
      case EVENT_MAC_CELL_STOP:
         {
            /* Process MAC cell stop request */
            unpackMacCellStop(MacProcCellStop, pst, mBuf);
            break;
         }
      case EVENT_MAC_DL_CCCH_IND:
         {
            /* Process DL CCCH Ind */
            unpackMacDlCcchInd(MacProcDlCcchInd, pst, mBuf);
            break;
         }
      case EVENT_MAC_UE_CREATE_REQ:
         {
            /* Process Ue Create Request */
            unpackMacUeCreateReq(MacProcUeCreateReq, pst, mBuf);
            break;
         }
      case EVENT_MAC_UE_RECONFIG_REQ:
         {
            /* Process Ue Reconfig Request */
            unpackMacUeReconfigReq(MacProcUeReconfigReq, pst, mBuf);
            break;
         }
      case EVENT_MAC_UE_DELETE_REQ:
         {
            /* Process UE Delete Request */  
            unpackMacUeDeleteReq(MacProcUeDeleteReq, pst, mBuf);
            break;
         }
      case EVENT_MAC_CELL_DELETE_REQ:
         {
            /* Process Cell Delete Request */
            unpackMacCellDeleteReq(MacProcCellDeleteReq, pst, mBuf);
            break;
         }
      case EVENT_MAC_SLICE_CFG_REQ:
         {
            /* Process Slice Configuration Request */
            unpackMacSliceCfgReq(MacProcSliceCfgReq, pst, mBuf);
            break;
         }
      case EVENT_MAC_SLICE_RECFG_REQ:
         {
            /* Process Slice Reconfiguration Request */
            unpackMacSliceRecfgReq(MacProcSliceRecfgReq, pst, mBuf);
            break;
         }
      case EVENT_MAC_RACH_RESOURCE_REQ:
         {
            /* Process Rach Resource Request */
            unpackMacRachRsrcReq(MacProcRachRsrcReq, pst, mBuf);
            break;
         }
      case EVENT_MAC_RACH_RESOURCE_REL:
         {
            /* Process Rach Resource Release */
            unpackMacRachRsrcRel(MacProcRachRsrcRel, pst, mBuf);
            break;
         }
      case EVENT_MAC_DL_PCCH_IND:
         {
            /* Process Pcch indication */
            unpackMacDlPcchInd(MacProcDlPcchInd, pst, mBuf);
            break;
         }
      case EVENT_MAC_DL_BROADCAST_REQ:
         {
            /* Process Dl Broadcast Request */  
            unpackMacDlBroadcastReq(MacProcDlBroadcastReq, pst, mBuf);
            break;
         }

      case EVENT_MAC_UE_RESET_REQ:
         {
            /* Process UE Reset Request */  
            unpackMacUeResetReq(MacProcUeResetReq, pst, mBuf);
            break;
         }

      case EVENT_MAC_STATISTICS_REQ:
         {
            /* Process Statistics Request */
            unpackMacStatsReq(MacProcStatsReq, pst, mBuf);
            break;
         }
      
      case EVENT_MAC_STATS_DELETE_REQ:
         {
            /* Process Statistics Delete Request */
            unpackMacStatsDeleteReq(MacProcStatsDeleteReq, pst, mBuf);
            break;
         }
      case EVENT_MAC_STATISTICS_MODIFY_REQ:
         {
            /* Process Statistics modification Request */
            unpackMacStatsModificationReq(MacProcStatsModificationReq, pst, mBuf);
            break;
         }


      default:
         RG_FREE_MSG(mBuf);
         break;
   }
}

/**
 * @brief Task Activation callback function Entity RLC. 
 *
 * @details
 *
 *     Function : MacHdlRlcEvents
 *     
 *     Process Messages received from Entity RLC
 *     
 *  @param[in]  Pst     *pst, Post structure of the primitive.     
 *  @param[in]  Buffer *mBuf, Packed primitive parameters in the buffer.
 *  @param[in]  Reason reason.
 *  @return  void
 **/
void MacHdlRlcEvents(Pst *pst, Buffer *mBuf)
{
   switch(pst->event)
   {
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
      default:
         RG_FREE_MSG(mBuf);
         break;
   }
}

/**
 * @brief Task Activation callback function Entity Lower MAC 
 *
 * @details
 *
 *     Function : MacHdlLwrMacEvents
 *     
 *     Process Messages received from Entity Lower MAC
 *     
 *  @param[in]  Pst     *pst, Post structure of the primitive.     
 *  @param[in]  Buffer *mBuf, Packed primitive parameters in the buffer.
 *  @param[in]  Reason reason.
 *  @return  void
 **/
void MacHdlLwrMacEvents(Pst *pst, Buffer *mBuf)
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

#ifdef CALL_FLOW_DEBUG_LOG
/**
* @brief Function prints src, dest, msg infor about all the msgs that received.
*
* @details
*
*     Function : callFlowMacActvTsk 
*
*     Function prints src, dest, msg infor about all the msgs that received
*
*  @param[in]  Pst     *pst, Post structure of the primitive.
*
*  @return  void
**/

void callFlowMacActvTsk(Pst *pst)
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
               case EVENT_MAC_SLICE_CFG_REQ:
                  strcpy(message,"EVENT_MAC_SLICE_CFG_REQ");
                  break;
               case EVENT_MAC_SLICE_RECFG_REQ:
                  strcpy(message,"EVENT_MAC_SLICE_RECFG_REQ");
                  break;
               case EVENT_MAC_STATISTICS_REQ:
                  strcpy(message,"EVENT_MAC_STATISTICS_REQ");
                  break;
               case EVENT_MAC_STATS_DELETE_REQ:
                  strcpy(message,"EVENT_MAC_STATS_DELETE_REQ");
                  break;
               case EVENT_MAC_STATISTICS_MODIFY_REQ:
                  strcpy(message,"EVENT_MAC_STATISTICS_MODIFY_REQ");
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
      case ENTMAC:
         {
            if(pst->srcInst == 1)
            {
               strcpy(sourceTask,"ENTSCH");
               switch(pst->event)
               {
                  case EVENT_SLICE_CFG_RSP_TO_MAC:
                     {
                        strcpy(message,"EVENT_SLICE_CFG_RSP_TO_MAC");
                        break;
                     }
                  case EVENT_SLICE_RECFG_RSP_TO_MAC:
                     {
                        strcpy(message,"EVENT_SLICE_RECFG_RSP_TO_MAC");
                        break;
                     }
                  case EVENT_SCH_CELL_CFG_CFM:
                     {
                        strcpy(message,"EVENT_SCH_CELL_CFG_CFM");
                        break;
                     }
                  case EVENT_UE_CONFIG_RSP_TO_MAC:
                     {
                        strcpy(message,"EVENT_UE_CONFIG_RSP_TO_MAC");
                        break;
                     }
                  case EVENT_UE_RECONFIG_RSP_TO_MAC:
                     {
                        strcpy(message,"EVENT_UE_RECONFIG_RSP_TO_MAC");
                        break;
                     }
                  case EVENT_DL_SCH_INFO:
                     {
                        strcpy(message,"EVENT_DL_SCH_INFO");
                        break;
                     }
                  case EVENT_UL_SCH_INFO:
                     {
                        strcpy(message,"EVENT_UL_SCH_INFO");
                        break;
                     }
                  case EVENT_RACH_RESOURCE_RESPONSE_TO_MAC:
                     {
                        strcpy(message,"EVENT_RACH_RESOURCE_RESPONSE_TO_MAC");
                        break;
                     }
                  case EVENT_DL_PAGING_ALLOC:
                     {
                        strcpy(message,"EVENT_DL_PAGING_ALLOC");
                        break;
                     }
                  case EVENT_UE_DELETE_RSP_TO_MAC:
                     {
                        strcpy(message,"EVENT_UE_DELETE_RSP_TO_MAC");
                        break;
                     }
                  case EVENT_CELL_DELETE_RSP_TO_MAC:
                     {
                        strcpy(message,"EVENT_CELL_DELETE_RSP_TO_MAC");
                        break;
                     }
                  case EVENT_DL_REL_HQ_PROC:
                     {
                        strcpy(message,"EVENT_DL_REL_HQ_PROC");
                        break;
                     }
                  case EVENT_STATISTICS_RSP_TO_MAC:
                     {
                        strcpy(message,"EVENT_STATISTICS_RSP_TO_MAC");
                        break;
                     }
                  case EVENT_STATISTICS_IND_TO_MAC:
                     {
                        strcpy(message,"EVENT_STATISTICS_IND_TO_MAC");
                        break;
                     }
                  case EVENT_STATISTICS_DELETE_RSP_TO_MAC:
                     {
                        strcpy(message,"EVENT_STATISTICS_DELETE_RSP_TO_MAC");
                        break;
                     }
                  case EVENT_STATISTICS_MODIFY_RSP_TO_MAC:
                     {
                        strcpy(message,"EVENT_STATISTICS_MODIFY_RSP_TO_MAC");
                        break;
                     }
                  default:
                     strcpy(message,"Invalid Event");
                     break;
               }
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
#endif


/**
 * @brief Task Activation callback function. 
 *
 * @details
 *
 *     Function : macActvTsk
 *     
 *     Primitives invoked by MAC's users/providers through
 *     a loosely coupled interface arrive here by means of 
 *     SSI's message handling. This API is registered with
 *     SSI during the Task Registration of MAC.
 *     
 *  @param[in]  Pst     *pst, Post structure of the primitive.     
 *  @param[in]  Buffer *mBuf, Packed primitive parameters in the buffer.
 *  @param[in]  Reason reason.
 *  @return  uint8_t
 *      -# ROK
 **/
uint8_t macActvTsk(Pst *pst, Buffer *mBuf)
{

#ifdef CALL_FLOW_DEBUG_LOG
   callFlowMacActvTsk(pst);
#endif

   switch(pst->srcEnt)
   {
      case ENTDUAPP:
         MacHdlDuappEvents(pst, mBuf);
         break;
      case ENTRLC:
         MacHdlRlcEvents(pst, mBuf);
         break;
      case ENTLWRMAC:
         MacHdlLwrMacEvents(pst, mBuf);
         break;
      default:
         RG_FREE_MSG(mBuf);
         break;
   }
   ODU_EXIT_TASK();
   return ROK;
}/* end of MacActvTsk */

/**
 * @brief Callback function for events from entity SCH
 *
 * @details
 *
 *     Function :MacMessageRouter
 *     
 *     Process Messages received from Entity SCH
 *     
 *  @param[in]  Pst     *pst, Post structure of the primitive.     
 *  @param[in]  Buffer *mBuf, Packed primitive parameters in the buffer.
 *  @param[in]  Reason reason.
 *  @return  void
 **/
uint8_t MacMessageRouter(Pst *pst, void *msg)
{
#ifdef CALL_FLOW_DEBUG_LOG
   callFlowMacActvTsk(pst);
#endif

   switch(pst->event)
   {
      case EVENT_SLICE_CFG_RSP_TO_MAC:
         {
            MacProcSchSliceCfgRsp(pst, (SchSliceCfgRsp *)msg);
            break;
         }
      case EVENT_SLICE_RECFG_RSP_TO_MAC:
         {
            MacProcSchSliceRecfgRsp(pst, (SchSliceRecfgRsp *)msg);
            break;
         }
      case EVENT_SCH_CELL_CFG_CFM:
         {
            MacProcSchCellCfgCfm(pst, (SchCellCfgCfm *)msg);
            break;
         }
      case EVENT_UE_CONFIG_RSP_TO_MAC:
         {
            MacProcSchUeCfgRsp(pst, (SchUeCfgRsp *)msg);
            break;
         }
      case EVENT_UE_RECONFIG_RSP_TO_MAC:
         {
            MacProcSchUeRecfgRsp(pst, (SchUeRecfgRsp *)msg);
            break;
         }
      case EVENT_DL_SCH_INFO:
         {
            MacProcDlAlloc(pst, (DlSchedInfo *)msg);
            break;
         }
      case EVENT_UL_SCH_INFO:
         {
            MacProcUlSchInfo(pst, (UlSchedInfo *)msg);
            break;
         }
      case EVENT_RACH_RESOURCE_RESPONSE_TO_MAC:
         {
            MacProcSchRachRsrcRsp(pst, (SchRachRsrcRsp *)msg);
            break;
         }
      case EVENT_DL_PAGING_ALLOC:
         {
            MacProcDlPageAlloc(pst, (DlPageAlloc *)msg);
            break;
         }
      case EVENT_UE_DELETE_RSP_TO_MAC:
         {
            MacProcSchUeDeleteRsp(pst, (SchUeDeleteRsp *)msg);
            break;
         }
      case EVENT_CELL_DELETE_RSP_TO_MAC:
         {
            MacProcSchCellDeleteRsp(pst, (SchCellDeleteRsp *)msg);
            break;
         }
      case EVENT_DL_REL_HQ_PROC: 
         {
            MacSchReleaseDlHarqProc(pst, (SchRlsHqInfo *)msg);
            break;
         }
      case EVENT_STATISTICS_RSP_TO_MAC:
         {
            MacProcSchStatsRsp(pst, (SchStatsRsp *)msg);
            break;
         }
      case EVENT_STATISTICS_IND_TO_MAC:
         {
            MacProcSchStatsInd(pst, (SchStatsInd *)msg);
            break;
         }
      case EVENT_STATISTICS_DELETE_RSP_TO_MAC:
         {
            MacProcSchStatsDeleteRsp(pst, (SchStatsDeleteRsp *)msg);
            break;
         }
      case EVENT_STATISTICS_MODIFY_RSP_TO_MAC:
         {
            MacProcSchStatsModificationRsp(pst, (SchStatsModificationRsp *)msg);
            break;
         }
      default:
         {
            return RFAILED;
         }
   }
   return ROK;
}


/**********************************************************************
 
         End of file
**********************************************************************/
