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

#include "tfu.x"           /* TFU types */
#include "lrg.x"           /* layer management typedefs for MAC */
#include "rgr.x"           /* layer management typedefs for MAC */
#include "rg_sch_inf.x"         /* typedefs for Scheduler */
#include "mac_sch_interface.h"
#include "sch.h"
#include "sch_utils.h"

uint8_t SchMessageRouter(Pst *pst, uint16_t cellId, void *msg)
{
   Inst  inst;
   uint16_t cellIdx = 0;
   SchCellCb *cellCb = NULLP;

   /* Fetch Cell CB */
   if(cellId != 0)
   {
      inst = pst->dstInst - SCH_INST_START;
      for(cellIdx = 0; cellIdx < MAX_NUM_CELL; cellIdx++)
      {
         if((schCb[inst].cells[cellIdx]) && (schCb[inst].cells[cellIdx]->cellId == cellId))
         {
            cellCb = schCb[inst].cells[cellIdx];
            break;
         }
      }

      if(cellCb == NULLP)
      {
         DU_LOG("\nERROR  -->  SCH : SchMessageRouter(): CellId [%d] not found", cellId);
         return RFAILED;
      }
   }

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
         SchHdlCellCfgReq(pst, (SchCellCfg *)msg);
         break;
      }
      case EVENT_SLOT_IND_TO_SCH:
      {
         cellCb->api->SchSlotInd(pst, (SlotTimingInfo *)msg);
         break;
      }
      case EVENT_ADD_UE_CONFIG_REQ_TO_SCH:
      {
         cellCb->api->SchAddUeConfigReq(pst, (SchUeCfgReq *)msg);
         break;
      }
      case EVENT_MODIFY_UE_CONFIG_REQ_TO_SCH:
      {
         cellCb->api->SchModUeConfigReq(pst, (SchUeRecfgReq *)msg);
         break;
      }
      case EVENT_RACH_IND_TO_SCH:
      {
         cellCb->api->SchRachInd(pst, (RachIndInfo *)msg);
         break;
      }
      case EVENT_CRC_IND_TO_SCH:
      {
         cellCb->api->SchCrcInd(pst, (CrcIndInfo *)msg);
         break;
      }
      case EVENT_DL_RLC_BO_INFO_TO_SCH:
      {
         cellCb->api->SchDlRlcBoInfo(pst, (DlRlcBoInfo *)msg);
         break;
      }
      case EVENT_SHORT_BSR:
      case EVENT_LONG_BSR:
      {
         cellCb->api->SchBsr(pst, (UlBufferStatusRptInd *)msg);
         break;
      }
      case EVENT_UCI_IND_TO_SCH:
      {
         cellCb->api->SchSrUciInd(pst, (SrUciIndInfo *)msg);
         break;
      }
      case EVENT_UE_DELETE_REQ_TO_SCH:
      {
         cellCb->api->SchUeDeleteReq(pst, (SchUeDelete *)msg);
         break;
      }
      case EVENT_CELL_DELETE_REQ_TO_SCH:
      {
         cellCb->api->SchCellDeleteReq(pst, (SchCellDeleteReq *)msg);
         break;
      }
      case EVENT_RACH_RESOURCE_REQUEST_TO_SCH:
      {
         cellCb->api->SchRachRsrcReq(pst, (SchRachRsrcReq *)msg);
         break;
      }
      case EVENT_RACH_RESOURCE_RELEASE_TO_SCH:
      {
         cellCb->api->SchRachRsrcRel(pst, (SchRachRsrcRel *)msg);
         break;
      }
      case EVENT_PAGING_IND_TO_SCH:
      {
         cellCb->api->SchPagingInd(pst, (SchPageInd *)msg);
         break;
      }
      case EVENT_DL_HARQ_IND_TO_SCH:
      {
         cellCb->api->SchDlHarqInd(pst, (DlHarqInd *)msg);
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
