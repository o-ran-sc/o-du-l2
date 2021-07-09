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
/* File : du_mgr_msg_router.c */
/* This file contains message handling functionality for DU APP */
#include "common_def.h"
#include "lrg.h"
#include "legtp.h"
#include "lsctp.h"
#include "legtp.h"
#include "lkw.h"
#include "kwu.h"
#include "lrg.x"
#include "lkw.x"
#include "kwu.x"
#include "du_app_mac_inf.h"
#include "du_app_rlc_inf.h"
#include "du_cfg.h"
#include "du_mgr.h"
#include "E2AP-PDU.h"
#include "du_sctp.h"
#include "F1AP-PDU.h"
#include "du_f1ap_msg_hdl.h"
#include "du_e2ap_msg_hdl.h"
#include "du_app_mac_inf.h"
#include "du_ue_mgr.h"
#include "du_utils.h"

uint8_t unpackRlcConfigCfm(RlcConfigCfm func,Pst *pst, Buffer *mBuf);
uint8_t cmUnpkLkwCntrlCfm(LkwCntrlCfm func,Pst *pst, Buffer *mBuf);
uint8_t cmUnpkLrgCfgCfm(LrgCfgCfm func,Pst *pst, Buffer *mBuf);
uint8_t cmUnpkKwuDatInd(KwuDatInd func,Pst *pst, Buffer *mBuf);
uint8_t cmUnpkLrgSchCfgCfm(LrgSchCfgCfm func,Pst *pst,Buffer *mBuf);
/**************************************************************************
 * @brief Task Initiation callback function. 
 *
 * @details
 *
 *     Function : duActvInit 
 *    
 *     Functionality:
 *             This function is supplied as one of parameters during DU APP's 
 *             task registration. SSI will invoke this function once, after
 *             it creates and attaches this TAPA Task to a system task.
 *     
 * @param[in]  Ent entity, the entity ID of this task.     
 * @param[in]  Inst inst, the instance ID of this task.
 * @param[in]  Region region, the region ID registered for memory 
 *              usage of this task.
 * @param[in]  Reason reason.
 * @return ROK     - success
 *         RFAILED - failure
 ***************************************************************************/
uint8_t duActvInit(Ent entity, Inst inst, Region region, Reason reason)
{
   uint8_t id;

   memset(&duCb, 0, sizeof(DuCb));

   duCb.init.procId  = ODU_GET_PROCID();
   duCb.init.ent     = entity;
   duCb.init.inst    = inst;
   duCb.init.region  = region;
   duCb.init.reason  = reason;
   duCb.init.cfgDone = FALSE;
   duCb.init.pool    = DU_POOL;
   duCb.init.acnt    = FALSE;
   duCb.init.trc     = FALSE;
   duCb.init.usta    = TRUE;
   duCb.mem.region   = DFLT_REGION;
   duCb.mem.pool     = DU_POOL;

   duCb.f1Status     = FALSE;
   duCb.e2Status     = FALSE;

   for(id = 0; id < MAX_NUM_CELL; id ++)
   {
      duCb.cfgCellLst[id] = NULL;
      duCb.actvCellLst[id] = NULL;
   }
   duCb.numUe = 0;
   memset(duCb.ueCcchCtxt, 0, MAX_NUM_UE * sizeof(UeCcchCtxt));
   duCb.numCfgCells  = 0;
   duCb.numActvCells = 0;
   cmLListInit(&duCb.reservedF1apPduList);
   ODU_SET_PROC_ID(DU_PROC);

   return ROK;

}

/**************************************************************************
* @brief Function prints the src dest and msg reached to egtp.
*
* @details
*
*      Function : callFlowduActvTsk 
*
*      Functionality:
*           Function prints the src dest and msg reached to egtp.
*
* @param[in]  Pst     *pst, Post structure of the primitive.
*
* @return void
*
+***************************************************************************/

void callFlowduActvTsk(Pst *pst)
{
   char sourceTask[50];
   char destTask[50]="ENTDUAPP";
   char message[100];
   
   switch(pst->srcEnt)
   {
      case ENTDUAPP:
         {
            strcpy(sourceTask,"ENTDUAPP");
            switch(pst->event)
            {
               case EVTCFG:
                  {
                     strcpy(message,"EVTCFG");
                     break;
                  }
               default:
                  {
                     strcpy(message,"Invalid Event");
                     break;
                  }
            }

            break;
         }
      case ENTRLC:
         {
            strcpy(sourceTask,"ENTRLC");
            switch(pst->event)
            {
               case LKW_EVT_CFG_CFM:
                  {
                     strcpy(message,"LKW_EVT_CFG_CFM");
                     break;
                  }
               case LKW_EVT_CNTRL_CFM:
                  {
                     strcpy(message,"LKW_EVT_CNTRL_CFM");
                     break;
                  }
               case LKW_EVT_STA_IND:
                  {
                     strcpy(message,"LKW_EVT_STA_IND");
                     break;
                  }
               case EVENT_RLC_UE_CREATE_RSP:
                  {
                     strcpy(message,"EVENT_RLC_UE_CREATE_RSP");
                     break;
                  }
               case EVENT_RLC_UE_RECONFIG_RSP:
                  {
                     strcpy(message,"EVENT_RLC_UE_RECONFIG_RSP");
                     break;
                  }
               case EVENT_RLC_UE_DELETE_RSP:
                  {
                     strcpy(message,"EVENT_RLC_UE_DELETE_RSP");
                     break;
                  }
               case EVENT_UL_RRC_MSG_TRANS_TO_DU:
                  {
                     strcpy(message,"EVENT_UL_RRC_MSG_TRANS_TO_DU");
                     break;
                  }
               case EVENT_RRC_DELIVERY_MSG_TRANS_TO_DU:
                  {
                     strcpy(message,"EVENT_RRC_DELIVERY_MSG_TRANS_TO_DU");
                     break;
                  }
               case EVENT_DL_RRC_MSG_RSP_TO_DU:
                  {
                     strcpy(message,"EVENT_DL_RRC_MSG_RSP_TO_DU");
                     break;
                  }
               case EVENT_UL_USER_DATA_TRANS_TO_DU:
                  {
                     strcpy(message,"EVENT_UL_USER_DATA_TRANS_TO_DU");
                     break;
                  }
               default:
                  {
                     strcpy(message,"Invalid Event");
                  }
            }
            break;
         }
      case ENTMAC:
         {
            strcpy(sourceTask,"ENTMAC");
            switch(pst->event)
            {
               case EVTCFG:
                  {
                     strcpy(message,"EVTCFG");
                     break;
                  }
               case EVTLRGCFGCFM:
                  {
                     strcpy(message,"EVTLRGCFGCFM");
                     break;
                  }
               case EVTLRGCNTRLCFM:
                  {
                     strcpy(message,"EVTLRGCNTRLCFM");
                     break;
                  }
               case EVTMACSCHGENCFGCFM:
                  {
                     strcpy(message,"EVTMACSCHGENCFGCFM");
                     break;
                  }
               case EVENT_MAC_CELL_CONFIG_CFM:
                  {
                     strcpy(message,"EVENT_MAC_CELL_CONFIG_CFM");
                     break;
                  }
               case EVENT_MAC_CELL_UP_IND:
                  {
                     strcpy(message,"EVENT_MAC_CELL_UP_IND");
                     break;
                  }
               case EVENT_MAC_STOP_IND:
                  {
                     strcpy(message,"EVENT_MAC_STOP_IND");
                     break;
                  }
               case EVENT_MAC_UL_CCCH_IND:
                  {
                     strcpy(message,"EVENT_MAC_UL_CCCH_IND");
                     break;
                  }
               case EVENT_MAC_UE_CREATE_RSP:
                  {
                     strcpy(message,"EVENT_MAC_UE_CREATE_RSP");
                     break;
                  }
               case EVENT_MAC_UE_RECONFIG_RSP:
                  {
                     strcpy(message,"EVENT_MAC_UE_RECONFIG_RSP");
                     break;
                  }
               case EVENT_MAC_UE_DELETE_RSP:
                  {
                     strcpy(message,"EVENT_MAC_UE_DELETE_RSP");
                     break;
                  }
               case EVENT_MAC_CELL_DELETE_RSP:
                  {
                     strcpy(message,"EVENT_MAC_CELL_DELETE_RSP");
                     break;
                  }
               default:
                  {
                     strcpy(message,"Invalid Event");
                     break;
                  }
            }

            break;
         }
      case ENTSCTP:
         {
            strcpy(sourceTask,"ENTSCTP");
            switch(pst->event)
            {
               case EVENT_CU_DATA:
                  {
                     strcpy(message,"EVENT_CU_DATA");
                     break;
                  }
               case EVENT_SCTP_NTFY:
                  {
                     strcpy(message,"EVENT_SCTP_NTFY");
                     break;
                  }
               case EVENT_RIC_DATA:
                  {
                     strcpy(message,"EVENT_RIC_DATA");
                     break;
                  }
               default:
                  {
                     strcpy(message,"Invalid Event");
                     break;
                  }

            }
            break;
         }
      case ENTEGTP:
         {
            strcpy(sourceTask,"ENTEGTP");
            switch(pst->event)
            {
               case EVTCFGCFM:
                  {
                     strcpy(message,"EVTCFGCFM");
                     break;
                  }
               case EVTSRVOPENCFM:
                  {
                     strcpy(message,"EVTSRVOPENCFM");
                     break;
                  }
               case EVTTNLMGMTCFM:
                  {
                     strcpy(message,"EVTTNLMGMTCFM");
                     break;
                  }
               default:
                  {
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
/**************************************************************************
 * @brief Task Activation callback function. 
 *
 * @details
 *
 *      Function : duActvTsk 
 * 
 *      Functionality:
 *           Primitives invoked by DU APP's users/providers through
 *           a loosely coupled interface arrive here by means of 
 *           SSI's message handling. This API is registered with
 *           SSI during the Task Registration of DU APP.
 *     
 * @param[in]  Pst     *pst, Post structure of the primitive.     
 * @param[in]  Buffer *mBuf, Packed primitive parameters in the
 *  buffer.
 * @return ROK     - success
 *         RFAILED - failure
 *
 ***************************************************************************/
uint8_t duActvTsk(Pst *pst, Buffer *mBuf)
{
   uint8_t ret = ROK;
   
#ifdef CALL_FLOW_DEBUG_LOG   
   callFlowduActvTsk(pst);
#endif

   switch(pst->srcEnt)
   {
      case ENTDUAPP:
         {
            switch(pst->event)
            {
               case EVTCFG:
                  {
                     DU_LOG("\n****** Received initial configs at DU APP ******\n");
                     duProcCfgComplete();
                     ODU_PUT_MSG_BUF(mBuf);
                     break;
                  }
               default:
                  {
                     DU_LOG("\nERROR  -->  DU_APP : Invalid event received at duActvTsk from ENTDUAPP");
                     ODU_PUT_MSG_BUF(mBuf);
                     ret = RFAILED;
                  }
            }

            break;
         }
      case ENTRLC:
         {
            switch(pst->event)
            {
               case LKW_EVT_CFG_CFM:
                  {
                     ret = unpackRlcConfigCfm(DuHdlRlcCfgComplete, pst, mBuf);
                     break;
                  }
               case LKW_EVT_CNTRL_CFM:
                  {
                     ret = cmUnpkLkwCntrlCfm(duHdlRlcCntrlCfgComplete, pst, mBuf);
                     break;
                  }
               case LKW_EVT_STA_IND:
                  {
                     break;
                  }
               case EVENT_RLC_UE_CREATE_RSP:
                  {
                     ret = unpackRlcUeCfgRsp(DuProcRlcUeCfgRsp, pst, mBuf);
                     break;
                  }
               case EVENT_RLC_UE_RECONFIG_RSP:
                  {
                     ret = unpackRlcUeCfgRsp(DuProcRlcUeCfgRsp, pst, mBuf);
                     break;
                  }
               case EVENT_RLC_UE_DELETE_RSP:
                  {
                     ret = unpackRlcUeDeleteRsp(DuProcRlcUeDeleteRsp, pst, mBuf);
                     break;
                  }
               case EVENT_UL_RRC_MSG_TRANS_TO_DU:
                  {
                     ret = unpackRlcUlRrcMsgToDu(DuProcRlcUlRrcMsgTrans, pst, mBuf);
                     break;
                  }
               case EVENT_RRC_DELIVERY_MSG_TRANS_TO_DU:
                  {
                     ret = unpackRrcDeliveryReportToDu(DuProcRlcRrcDeliveryReport, pst, mBuf);
                     break;
                  }
               case EVENT_DL_RRC_MSG_RSP_TO_DU:
                  {
                     ret = unpackRlcDlRrcMsgRspToDu(DuProcRlcDlRrcMsgRsp, pst, mBuf);
                     break;
                  }
               case EVENT_UL_USER_DATA_TRANS_TO_DU:
                  {
                     ret = unpackRlcUlUserDataToDu(DuProcRlcUlUserDataTrans, pst, mBuf);
                     break;
                  }
               default:
                  {
                     DU_LOG("\nERROR  -->  DU_APP : Invalid event %d received at duActvTsk from ENTRLC", \
                           pst->event);
                     ODU_PUT_MSG_BUF(mBuf);
                     ret = RFAILED;
                  }
            }
            break;
         }
      case ENTMAC:
         {
            switch(pst->event)
            {
               //Config complete
               case EVTCFG:
                  {
                     ODU_PUT_MSG_BUF(mBuf);
                     break;
                  }
               case EVTLRGCFGCFM:
                  {
                     ret = cmUnpkLrgCfgCfm(duHdlMacCfgComplete, pst, mBuf);
                     break;
                  }
               case EVTLRGCNTRLCFM:
                  {
                     break;
                  }
               case EVTMACSCHGENCFGCFM:
                  {
                     ret = cmUnpkLrgSchCfgCfm(duHdlSchCfgComplete, pst, mBuf);
                     break;
                  }
               case EVENT_MAC_CELL_CONFIG_CFM:
                  {
                     ret = unpackMacCellCfgCfm(duHandleMacCellCfgCfm, pst, mBuf);
                     break;
                  }
               case EVENT_MAC_CELL_UP_IND:
                  {
                     ret = unpackMacCellUpInd(duHandleCellUpInd, pst, mBuf);
                     break;
                  }
               case EVENT_MAC_STOP_IND:
                  {
                     ret = unpackMacStopInd(duHandleStopInd, pst, mBuf);
                     break;
                  }
               case EVENT_MAC_UL_CCCH_IND:
                  {
                     ret = unpackMacUlCcchInd(duHandleUlCcchInd, pst, mBuf);
                     break;
                  }
               case EVENT_MAC_UE_CREATE_RSP:
                  {
                     ret = unpackDuMacUeCfgRsp(DuProcMacUeCfgRsp, pst, mBuf); 
                     break;
                  }
               case EVENT_MAC_UE_RECONFIG_RSP:
                  {
                     ret = unpackDuMacUeCfgRsp(DuProcMacUeCfgRsp, pst, mBuf); 
                     break;
                  }
               case EVENT_MAC_UE_DELETE_RSP:
                  {
                     ret = unpackDuMacUeDeleteRsp(DuProcMacUeDeleteRsp, pst, mBuf);
                     break;
                  }
               case EVENT_MAC_CELL_DELETE_RSP:
                  {
                     ret = unpackDuMacCellDeleteRsp(DuProcMacCellDeleteRsp, pst, mBuf);
                     break;
                  }
               default:
                  {
                     DU_LOG("\nERROR  -->  DU_APP : Invalid event received at duActvTsk from ENTMAC");
                     ODU_PUT_MSG_BUF(mBuf);
                     ret = RFAILED;
                  }
            }

            break;
         }
      case ENTSCTP:
         {
            switch(pst->event)
            {
               case EVENT_CU_DATA:
                  {
                     F1APMsgHdlr(mBuf);
                     break;
                  }
               case EVENT_SCTP_NTFY:
                  {
                     ret = cmUnpkSctpNtfy(duSctpNtfyHdl, pst, mBuf);
                     break;
                  }
               case EVENT_RIC_DATA:
                  {
                     E2APMsgHdlr(mBuf);
                     break;
                  }
               default:
                  {
                     DU_LOG("\nERROR  -->  DU_APP : Invalid event received at duActvTsk from ENTSCTP");
                     ret = RFAILED;
                  }

            }
            ODU_PUT_MSG_BUF(mBuf);
            break;
         }
      case ENTEGTP:
         {
            switch(pst->event)
            {
               case EVTCFGCFM:
                  {
                     unpackEgtpCfgCfm(duHdlEgtpCfgComplete, mBuf);
                     break;
                  }
               case EVTSRVOPENCFM:
                  {
                     unpackEgtpSrvOpenCfm(duHdlEgtpSrvOpenComplete, mBuf);
                     break;
                  }
               case EVTTNLMGMTCFM:
                  {
                     unpackEgtpTnlMgmtCfm(duHdlEgtpTnlMgmtCfm, mBuf);
                     break;
                  }
               default:
                  {
                     DU_LOG("\nERROR  -->  DU_APP : Invalid event[%d] received at duActvTsk from ENTEGTP", pst->event);
                     ret = RFAILED;
                  }
            }
            ODU_PUT_MSG_BUF(mBuf);
            break;
         }
      default:
         {
            DU_LOG("\nERROR  -->  DU_APP : DU APP can not process message from Entity %d", pst->srcEnt);
            ODU_PUT_MSG_BUF(mBuf);
            ret = RFAILED;
         }

   }
   ODU_EXIT_TASK();
   return ret;
}

/**********************************************************************
  End of file
 **********************************************************************/
