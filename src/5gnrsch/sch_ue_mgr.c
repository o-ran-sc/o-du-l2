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
/* This file contains UE management handling functionality for SCH */

#include "common_def.h"
#include "tfu.h"
#include "lrg.h"

#include "tfu.x"
#include "lrg.x"
#include "du_log.h"
#include "du_app_mac_inf.h"
#include "mac_sch_interface.h"
#include "sch.h"
#include "sch_utils.h"

/* local defines */
SchUeCfgRspFunc SchUeCfgRspOpts[] =
{
   packSchUeCfgRsp,      /* LC */
   MacProcSchUeCfgRsp,   /* TC */
   packSchUeCfgRsp       /* LWLC */
};


/*******************************************************************
 *
 * @brief Fill and send UE cfg response to MAC
 *
 * @details
 *
 *    Function : SchSendUeCfgRspToMac
 *
 *    Functionality: Fill and send UE cfg response to MAC
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
void SchSendUeCfgRspToMac(SchUeCfg *ueCfg, Inst inst,\
      SchMacRsp result, SchUeCfgRsp *cfgRsp)
{
   Pst rspPst;

   DU_LOG("\nSCH: Sending UE Create response to MAC");

   cfgRsp->cellId = ueCfg->cellId;
   cfgRsp->crnti = ueCfg->crnti;
   GET_UE_IDX(ueCfg->crnti, cfgRsp->ueIdx);
   cfgRsp->rsp = result;   

   /* Filling response post */
   memset(&rspPst, 0, sizeof(Pst));
   FILL_PST_SCH_TO_MAC(rspPst, inst);
   rspPst.event = EVENT_UE_CREATE_RSP_TO_MAC;

   SchUeCfgRspOpts[rspPst.selector](&rspPst, cfgRsp);
}

/*******************************************************************
 *
 * @brief Hanles Ue create request from MAC
 *
 * @details
 *
 *    Function : macSchUeCreateReq
 *
 *    Functionality: Hanles Ue create request from MAC
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t macSchUeCreateReq(Pst *pst, SchUeCfg *ueCfg)
{
   uint8_t      idx;
   uint16_t     ueIdx;
   SchCellCb    *cellCb;
   SchUeCb      *ueCb;
   SchUeCfgRsp  cfgRsp;
   Inst         inst = pst->dstInst - 1;

   DU_LOG("\nSCH : UE Create Request for CRNTI[%d]", ueCfg->crnti);

   memset(&cfgRsp, 0, sizeof(SchUeCfgRsp));

   if(!ueCfg)
   {
      DU_LOG("\nSCH : UE create request failed");
      return RFAILED;
   }

   /* Search of cell cb */
   for(idx = 0; idx < MAX_NUM_CELL; idx++)
   {
      cellCb = schCb[inst].cells[idx];
      if(cellCb->cellId == ueCfg->cellId)
	 break;
   }
   if(idx == MAX_NUM_CELL)
   {
      DU_LOG("\nSCH : Ue create request failed. Invalid cell id %d", ueCfg->cellId);
      SchSendUeCfgRspToMac(ueCfg, inst, RSP_NOK, &cfgRsp);
      return ROK;
   }

   /* Check if max number of UE configured */
   if(cellCb->numActvUe > MAX_NUM_UE)
   {
      DU_LOG("SCH : Max number of UE [%d] already configured", MAX_NUM_UE);
      SchSendUeCfgRspToMac(ueCfg, inst, RSP_NOK, &cfgRsp);
      return ROK;
   }

   /* Search if UE already configured */
   GET_UE_IDX(ueCfg->crnti, ueIdx);
   ueCb = &cellCb->ueCb[ueIdx];
   if(ueCb)
   {
      if((ueCb->crnti == ueCfg->crnti) && (ueCb->state == SCH_UE_STATE_ACTIVE))
      {
	 DU_LOG("\n SCH : CRNTI %d already configured ", ueCfg->crnti);
	 SchSendUeCfgRspToMac(ueCfg, inst, RSP_OK, &cfgRsp);
	 return ROK;
      }
   }

   /* Fill received Ue Configuration in UeCb */
   memset(ueCb, 0, sizeof(SchUeCb));

   GET_UE_IDX(ueCfg->crnti, ueCb->ueIdx);
   ueCb->crnti = ueCfg->crnti;
   memcpy(&ueCb->ueCfg, ueCfg, sizeof(SchUeCfg));
   ueCb->state = SCH_UE_STATE_ACTIVE;
   cellCb->numActvUe++;

   SchSendUeCfgRspToMac(ueCfg, inst, RSP_OK, &cfgRsp);
   return ROK;
}

/**********************************************************************
  End of file
 **********************************************************************/
