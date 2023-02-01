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

uint8_t SchMessageRouter(Pst *pst, void *msg)
{
   switch(pst->event)
   {
      case EVENT_SCH_GEN_CFG:
      {
         SchProcGenCfgReq(pst, (RgMngmt *)msg);
         break;
      }
      case EVENT_SLICE_CFG_REQ_TO_SCH:
      {    
         MacSchSliceCfgReq(pst, (SchSliceCfgReq *)msg);
         break;
      }    
      case EVENT_SLICE_RECFG_REQ_TO_SCH:
      {    
         MacSchSliceRecfgReq(pst, (SchSliceRecfgReq *)msg);
         break;
      }
      case EVENT_SCH_CELL_CFG:
      {
         SchHdlCellCfgReq(pst, (SchCellCfg *)msg);
         break;
      }
      case EVENT_SLOT_IND_TO_SCH:
      {
         MacSchSlotInd(pst, (SlotTimingInfo *)msg);
         break;
      }
      case EVENT_ADD_UE_CONFIG_REQ_TO_SCH:
      {
         MacSchAddUeConfigReq(pst, (SchUeCfgReq *)msg);
         break;
      }
      case EVENT_MODIFY_UE_CONFIG_REQ_TO_SCH:
      {
         MacSchModUeConfigReq(pst, (SchUeRecfgReq *)msg);
         break;
      }
      case EVENT_RACH_IND_TO_SCH:
      {
         MacSchRachInd(pst, (RachIndInfo *)msg);
         break;
      }
      case EVENT_CRC_IND_TO_SCH:
      {
         MacSchCrcInd(pst, (CrcIndInfo *)msg);
         break;
      }
      case EVENT_DL_RLC_BO_INFO_TO_SCH:
      {
         MacSchDlRlcBoInfo(pst, (DlRlcBoInfo *)msg);
         break;
      }
      case EVENT_SHORT_BSR:
      case EVENT_LONG_BSR:
      {
         MacSchBsr(pst, (UlBufferStatusRptInd *)msg);
         break;
      }
      case EVENT_UCI_IND_TO_SCH:
      {
         MacSchSrUciInd(pst, (SrUciIndInfo *)msg);
         break;
      }
      case EVENT_UE_DELETE_REQ_TO_SCH:
      {
         MacSchUeDeleteReq(pst, (SchUeDelete *)msg);
         break;
      }
      case EVENT_CELL_DELETE_REQ_TO_SCH:
      {
         MacSchCellDeleteReq(pst, (SchCellDeleteReq *)msg);
         break;
      }
      case EVENT_RACH_RESOURCE_REQUEST_TO_SCH:
      {
         MacSchRachRsrcReq(pst, (SchRachRsrcReq *)msg);
         break;
      }
      case EVENT_RACH_RESOURCE_RELEASE_TO_SCH:
      {
         MacSchRachRsrcRel(pst, (SchRachRsrcRel *)msg);
         break;
      }
      case EVENT_PAGING_IND_TO_SCH:
      {
         MacSchPagingInd(pst, (SchPageInd *)msg);
         break;
      }
      case EVENT_DL_HARQ_IND_TO_SCH:
      {
         MacSchDlHarqInd(pst, (DlHarqInd *)msg);
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
