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

#include "common_def.h"
#include "lrg.h"
#include "tfu.h"
#include "du_log.h"
#include "rgr.h"
#include "rg_sch_inf.h"
#include "rg_sch.h"
#include "mac_sch_interface.h"

#include "tfu.x"           /* TFU types */
#include "lrg.x"           /* layer management typedefs for MAC */
#include "rgr.x"           /* layer management typedefs for MAC */
#include "rg_sch_inf.x"         /* typedefs for Scheduler */
#include "sch.h"
#include "sch_tmr.h"

/**
 * @brief Task Initiation function. 
 *
 * @details
 *
 *     Function : schActvInit
 *     
 *     This function is supplied as one of parameters during MAC's 
 *     task registration. MAC will invoke this function once, after
 *     it creates and attaches this TAPA Task to a system task.
 *     
 *  @param[in]  Ent Entity, the entity ID of this task.     
 *  @param[in]  Inst Inst, the instance ID of this task.
 *  @param[in]  Region Region, the region ID registered for memory 
 *              usage of this task.
 *  @param[in]  Reason Reason.
 *  @return  int
 *      -# ROK
 **/
uint8_t schActvInit(Ent entity, Inst instId, Region region, Reason reason)
{
   Inst inst = (instId  - SCH_INST_START);

   /* Initialize the MAC TskInit structure to zero */
   memset ((uint8_t *)&schCb[inst], 0, sizeof(schCb));

   /* Initialize the MAC TskInit with received values */
   schCb[inst].schInit.ent = entity;
   schCb[inst].schInit.inst = inst;
   schCb[inst].schInit.region = region;
   schCb[inst].schInit.pool = 0;
   schCb[inst].schInit.reason = reason;
   schCb[inst].schInit.cfgDone = FALSE;
   schCb[inst].schInit.acnt = FALSE;
   schCb[inst].schInit.usta = FALSE;
   schCb[inst].schInit.trc = FALSE;
   schCb[inst].schInit.procId = ODU_GET_PROCID();

   return ROK;
} /* schActvInit */

/**
 * @brief Task Activation callback function.
 *
 * @details
 *
 *     Function : schActvTsk
 *
 *     Primitives invoked by SCH's users/providers through
 *     a loosely coupled interface arrive here by means of
 *     SSI's message handling. This API is registered with
 *     SSI during the Task Registration of SCH.
 *
 *  @param[in]  Pst     *pst, Post structure of the primitive.
 *  @param[in]  Buffer *mBuf, Packed primitive parameters in the buffer.
 *  @param[in]  Reason reason.
 *  @return  uint8_t
 *      -# ROK
 **/
uint8_t schActvTsk(Pst *pst, Buffer *mBuf)
{
   SchMessageRouter(pst, (void *)mBuf);

   ODU_EXIT_TASK();
   return ROK;
}

#ifdef CALL_FLOW_DEBUG_LOG
/**
* @brief Function prints src, dest, msg infor about all the msgs that received.
*
* @details
*
*     Function : callFlowSchMsgRouter 
*
*     Function prints src, dest, msg infor about all the msgs that received
*
*  @param[in]  Pst     *pst, Post structure of the primitive.
*
*  @return  void
**/

void callFlowSchMsgRouter(Pst *pst)
{
   char sourceTask[50];
   char destTask[50]="ENTSCH";
   char message[100];

   strcpy(sourceTask,"ENTMAC");
   switch(pst->event)
   {
      case EVENT_SCH_GEN_CFG:
         {
            strcpy(message,"EVENT_SCH_GEN_CFG");
            break;
         }   
      case EVENT_SLICE_CFG_REQ_TO_SCH:
         {      
            strcpy(message,"EVENT_SLICE_CFG_REQ_TO_SCH");
            break;
         }      
      case EVENT_SLICE_RECFG_REQ_TO_SCH:
         {      
            strcpy(message,"EVENT_SLICE_RECFG_REQ_TO_SCH");
            break;
         }   
      case EVENT_SCH_CELL_CFG:
         {
            strcpy(message,"EVENT_SCH_CELL_CFG");
            break;
         }   
      case EVENT_SLOT_IND_TO_SCH:
         {
            strcpy(message,"EVENT_SLOT_IND_TO_SCH");
            break;
         }   
      case EVENT_ADD_UE_CONFIG_REQ_TO_SCH:
         {   
            strcpy(message,"EVENT_ADD_UE_CONFIG_REQ_TO_SCH");
            break;
         }   
      case EVENT_MODIFY_UE_CONFIG_REQ_TO_SCH:
         {   
            strcpy(message,"EVENT_MODIFY_UE_CONFIG_REQ_TO_SCH");
            break;
         }
      case EVENT_UL_CQI_TO_SCH:
         {
            strcpy(message,"EVENT_UL_CQI_TO_SCH");
            break;
         }
      case EVENT_DL_CQI_TO_SCH:
         {
            strcpy(message,"EVENT_DL_CQI_TO_SCH");
            break;
         }
      case EVENT_PHR_IND_TO_SCH
         {
            strcpy(message,"EVENT_PHR_IND_TO_SCH");
         }
      case EVENT_RACH_IND_TO_SCH:
         {
            strcpy(message,"EVENT_RACH_IND_TO_SCH");
            break;
         }
      case EVENT_CRC_IND_TO_SCH:
         {
            strcpy(message,"EVENT_CRC_IND_TO_SCH");
            break;
         }
      case EVENT_DL_RLC_BO_INFO_TO_SCH:
         {
            strcpy(message,"EVENT_DL_RLC_BO_INFO_TO_SCH");
            break;
         }
      case EVENT_SHORT_BSR:
         {
            strcpy(message,"EVENT_SHORT_BSR");
            break;
         }
      case EVENT_LONG_BSR:
         {
            strcpy(message,"EVENT_LONG_BSR");
            break;
         }
      case EVENT_UCI_IND_TO_SCH:
         {
            strcpy(message,"EVENT_UCI_IND_TO_SCH");
            break;
         }
      case EVENT_UE_DELETE_REQ_TO_SCH:
         {
            strcpy(message,"EVENT_UE_DELETE_REQ_TO_SCH");
            break;
         }
      case EVENT_CELL_DELETE_REQ_TO_SCH:
         {
            strcpy(message,"EVENT_CELL_DELETE_REQ_TO_SCH");
            break;
         }
      case EVENT_RACH_RESOURCE_REQUEST_TO_SCH:
         {
            strcpy(message,"EVENT_RACH_RESOURCE_REQUEST_TO_SCH");
            break;
         }
      case EVENT_RACH_RESOURCE_RELEASE_TO_SCH:
         {
            strcpy(message,"EVENT_RACH_RESOURCE_RELEASE_TO_SCH");
            break;
         }
      case EVENT_PAGING_IND_TO_SCH:
         {
            strcpy(message,"EVENT_PAGING_IND_TO_SCH");
            break;
         }
      case EVENT_DL_HARQ_IND_TO_SCH:
         {
            strcpy(message,"EVENT_DL_HARQ_IND_TO_SCH");
            break;
         }
      case EVENT_STATISTICS_REQ_TO_SCH:
         {
            strcpy(message,"EVENT_STATISTICS_REQ_TO_SCH");
            break;
         }
      case EVENT_STATISTICS_DELETE_REQ_TO_SCH:
         {
            strcpy(message,"EVENT_STATISTICS_DELETE_REQ_TO_SCH");
            break;
         }
      case EVENT_STATISTICS_MODIFY_REQ_TO_SCH:
         {
            strcpy(message,"EVENT_STATISTICS_MODIFY_REQ_TO_SCH");
            break;
         }
      default:
         strcpy(message,"Invalid Event");
         break;
   }
   DU_LOG("\nCall Flow: %s -> %s : %s\n", sourceTask, destTask, message);
}
#endif

uint8_t SchMessageRouter(Pst *pst, void *msg)
{
#ifdef CALL_FLOW_DEBUG_LOG
   callFlowSchMsgRouter(pst);
#endif

   switch(pst->event)
   {
      case EVENT_SCH_GEN_CFG:
      {
            SchProcGenCfgReq(pst, (RgMngmt *)msg);
         break;
      }
      case EVENT_SLICE_CFG_REQ_TO_SCH:
      {    
         SchProcSliceCfgReq(pst, (SchSliceCfgReq *)msg);
         break;
      }    
      case EVENT_SLICE_RECFG_REQ_TO_SCH:
      {    
         SchProcSliceRecfgReq(pst, (SchSliceRecfgReq *)msg);
         break;
      }
      case EVENT_SCH_CELL_CFG:
      {
         SchProcCellCfgReq(pst, (SchCellCfg *)msg);
         break;
      }
      case EVENT_SLOT_IND_TO_SCH:
      {
         SchProcSlotInd(pst, (SlotTimingInfo *)msg);
         break;
      }
      case EVENT_ADD_UE_CONFIG_REQ_TO_SCH:
      {
         SchAddUeConfigReq(pst, (SchUeCfgReq *)msg);
         break;
      }
      case EVENT_MODIFY_UE_CONFIG_REQ_TO_SCH:
      {
         SchModUeConfigReq(pst, (SchUeRecfgReq *)msg);
         break;
      }
      case EVENT_UL_CQI_TO_SCH:
      {
         SchProcUlCqiInd(pst, (SchUlCqiInd *)msg);
         break;
      }
      case EVENT_DL_CQI_TO_SCH:
      {
         SchProcDlCqiInd(pst, (SchDlCqiInd *)msg);
         break;
      }
      case EVENT_PHR_IND_TO_SCH:
      {
         SchProcPhrInd(pst, (SchPwrHeadroomInd *)msg);
         break;
      }
      case EVENT_RACH_IND_TO_SCH:
      {
         SchProcRachInd(pst, (RachIndInfo *)msg);
         break;
      }
      case EVENT_CRC_IND_TO_SCH:
      {
         SchProcCrcInd(pst, (CrcIndInfo *)msg);
         break;
      }
      case EVENT_DL_RLC_BO_INFO_TO_SCH:
      {
         SchProcDlRlcBoInfo(pst, (DlRlcBoInfo *)msg);
         break;
      }
      case EVENT_SHORT_BSR:
      case EVENT_LONG_BSR:
      {
         SchProcBsr(pst, (UlBufferStatusRptInd *)msg);
         break;
      }
      case EVENT_UCI_IND_TO_SCH:
      {
         SchProcSrUciInd(pst, (SrUciIndInfo *)msg);
         break;
      }
      case EVENT_UE_DELETE_REQ_TO_SCH:
      {
         SchProcUeDeleteReq(pst, (SchUeDelete *)msg);
         break;
      }
      case EVENT_CELL_DELETE_REQ_TO_SCH:
      {
         SchProcCellDeleteReq(pst, (SchCellDeleteReq *)msg);
         break;
      }
      case EVENT_RACH_RESOURCE_REQUEST_TO_SCH:
      {
         SchProcRachRsrcReq(pst, (SchRachRsrcReq *)msg);
         break;
      }
      case EVENT_RACH_RESOURCE_RELEASE_TO_SCH:
      {
         SchProcRachRsrcRel(pst, (SchRachRsrcRel *)msg);
         break;
      }
      case EVENT_PAGING_IND_TO_SCH:
      {
         SchProcPagingInd(pst, (SchPageInd *)msg);
         break;
      }
      case EVENT_DL_HARQ_IND_TO_SCH:
      {
         SchProcDlHarqInd(pst, (DlHarqInd *)msg);
         break;
      }
      case EVENT_STATISTICS_REQ_TO_SCH:
      {
         SchProcStatsReq(pst, (SchStatsReq *)msg);
         break;
      }
      case EVENT_STATISTICS_DELETE_REQ_TO_SCH:
      {
         SchProcStatsDeleteReq(pst, (SchStatsDeleteReq *)msg);
         break;
      }
      case EVENT_STATISTICS_MODIFY_REQ_TO_SCH:
      {
         SchProcStatsModificationReq(pst, (SchStatsModificationReq *)msg);
         break;
      }
      default:
      {
         DU_LOG("\nERROR  -->  SCH : SchMessageRouter(): Invalid event [%d] received", pst->event);
         return RFAILED;
      }
   }
   return ROK;
}


/**********************************************************************
 
         End of file
**********************************************************************/
