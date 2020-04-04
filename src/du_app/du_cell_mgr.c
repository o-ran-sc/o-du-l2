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

/* This file contains message handling functionality for DU APP */
#include "du_cell_mgr.h"

extern DuCfgParams duCfgParam;

extern S16 cmPkRgrCfgReq(Pst* pst, RgrCfgTransId transId, \
      RgrCfgReqInfo *cfgReqInfo);

extern S16 duBuildAndSendMacCellCfg();

/*******************************************************************
 *
 * @brief Processes cells to be activated
 *
 * @details
 *
 *    Function : procCellsToBeActivated
 *
 *    Functionality:
 *      - Processes cells to be activated list received in F1SetupRsp
 *
 * @params[in] void
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
S16 procCellsToBeActivated(Cells_to_be_Activated_List_t cellsToActivate)
{
   U16 idx = 0;
   S16 ret = ROK;

   for(idx=0; idx<cellsToActivate.list.count; idx++)
   {
      U16 nci = 0;
      U16 pci = 0;
      DuCellCb *cellCb = NULLP;

      Cells_to_be_Activated_List_Item_t cell = cellsToActivate.list.array[idx]->\
          value.choice.Cells_to_be_Activated_List_Item;

      bitStringToInt(&cell.nRCGI.nRCellIdentity, &nci);
      if(nci <= 0 || nci > DU_MAX_CELLS)
      {
         DU_LOG("\nDU APP : Invalid NCI %d", nci);
         return RFAILED;
      }

      if(cell.nRPCI)
      {
         pci = *cell.nRPCI;
      }

      cellCb = duCb.cfgCellLst[nci-1];

      if(!cellCb)
      {
         DU_LOG("\nDU APP : No Cell found for NCI %d", nci);
         return RFAILED;
      }
      cellCb->cellStatus = ACTIVATION_IN_PROGRESS; 
      cellCb->cellInfo.nrPci = pci;

      /* Now remove this cell from configured list and move to active list */
      duCb.cfgCellLst[nci-1] = NULLP;
      duCb.actvCellLst[nci-1] = cellCb;

      /* Build and send Mac Cell Cfg for the number of active cells */
      ret = duBuildAndSendMacCellCfg();
      if(ret != ROK)
      {
         DU_LOG("\nDU APP : macCellCfg build and send failed");
         return RFAILED;
      }
   }

   return ret;
}

/******************************************************************
*
* @brief Processes F1 Setup Response sent by CU
*
* @details
*
*    Function : procF1SetupRsp
*
*    Functionality: Processes F1 Setup Response sent by CU
*
* @params[in] F1AP_PDU_t ASN decoded F1AP message
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/
S16 procF1SetupRsp(F1AP_PDU_t *f1apMsg)
{
   S16 ret = ROK;

   F1SetupResponse_t *f1SetRspMsg;
   F1SetupRsp    f1SetRspDb;
   GNB_CU_Name_t *cuName;
   RRC_Version_t *rrc_Ver;
   U16 idx;

   DU_LOG("\nDU_APP : F1 Setup Response received"); 
 	printf("\nDU_APP : F1 Setup Response received");
   duCb.f1Status = TRUE; //Set F1 status as true
   f1SetRspMsg = &f1apMsg->choice.successfulOutcome->value.choice.F1SetupResponse;

   for(idx=0; idx<f1SetRspMsg->protocolIEs.list.count; idx++)
   {
//      F1SetupResponseIEs_t f1RspIe = f1SetRspMsg->protocolIEs.list.array[idx];
      switch(f1SetRspMsg->protocolIEs.list.array[idx]->id)
      {
         case ProtocolIE_ID_id_Cells_to_be_Activated_List:
         {
            procCellsToBeActivated(f1SetRspMsg->protocolIEs.list.array[idx]->\
                  value.choice.Cells_to_be_Activated_List);
            break;
         }
         //TODO: where to store and how to use below variables?can they be skipped
         case ProtocolIE_ID_id_TransactionID:
         {
            f1SetRspDb.transId = f1SetRspMsg->protocolIEs.list.array[idx]->\
                                 value.choice.TransactionID;
            break;
         }
         case ProtocolIE_ID_id_gNB_CU_Name:
         {
            cuName = &f1SetRspMsg->protocolIEs.list.array[idx]->\
                     value.choice.GNB_CU_Name;
            strcpy(f1SetRspDb.cuName, (const char*)cuName->buf);
            break;
         }
         case ProtocolIE_ID_id_GNB_CU_RRC_Version:
         {
            rrc_Ver = &f1SetRspMsg->protocolIEs.list.array[idx]->\
                      value.choice.RRC_Version;
            strcpy(f1SetRspDb.rrcVersion.rrcVer,
                  (const char*)rrc_Ver->latest_RRC_Version.buf);
            break;
         }
         default:
            DU_LOG("\nDU_APP : Invalid IE received in F1SetupRsp:%ld",
                  f1SetRspMsg->protocolIEs.list.array[idx]->id);
      }
   }
 
   /* TODO :Check the deallocation */
#if 0
   SPutSBuf(DU_APP_MEM_REGION, DU_POOL,(Data *)&(f1SetupRsp->protocolIEs.list.array),\
         (Size)elementCnt * sizeof(F1SetupResponseIEs_t *));
   SPutSBuf(DU_APP_MEM_REGION, DU_POOL,(Data *)&(f1apMsg->choice.successfulOutcome),\
         (Size)sizeof(SuccessfulOutcome_t));
   SPutSBuf(DU_APP_MEM_REGION, DU_POOL,(Data *)&f1apMsg,(Size)sizeof(F1AP_PDU_t));
#endif
 
   return ret;
}

/******************************************************************
*
* @brief Send gNB cfg to scheduler via MAC
*
* @details
*
*    Function : duSendSchGnbCfg
*
*    Functionality: Send gNB cfg to scheduler via MAC
*
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/
S16 duSendSchGnbCfg()
{

   RgrCfgReqInfo  *cfgReq = NULLP;
   MacSchedGnbCfg *cfg = NULLP;
   U32 transId = 1;

   DU_ALLOC(cfgReq, sizeof(RgrCfgReqInfo));
   if( cfgReq == NULLP)
   {
      DU_LOG("\nDU_APP : Mem allocation failed in duSendSchGnbCfg");
      return RFAILED;
   }

   cfgReq->action = SCH_CONFIG;
   cfgReq->u.cfgInfo.cfgType = MAC_GNB_CFG;
   cfg = &(cfgReq->u.cfgInfo.u.schedGnbCfg);
   cfg->numTxAntPorts = duCfgParam.schedCfg.numTxAntPorts;
   cfg->ulSchdType = duCfgParam.schedCfg.ulSchdType;
   cfg->dlSchdType = duCfgParam.schedCfg.dlSchdType;
   cfg->numCells = duCfgParam.schedCfg.numCells;
   cfg->maxUlUePerTti = duCfgParam.schedCfg.maxUlUePerTti;
   cfg->maxDlUePerTti = duCfgParam.schedCfg.maxDlUePerTti;

   if(ROK != duSendSchGnbCfgToMac(cfgReq, transId))
   {
      return RFAILED;
   }

   return ROK;
}

/******************************************************************
*
* @brief Send gNB cfg to scheduler via MAC
*
* @details
*
*    Function : duSendSchGnbCfgToMac 
*
*    Functionality: Send gNB cfg to scheduler via MAC
*
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/
S16 duSendSchGnbCfgToMac(RgrCfgReqInfo *cfgReq, U32 trans_id)
{
   RgrCfgTransId transId;
   Pst pst;

   DU_SET_ZERO(&pst, sizeof(Pst));
   DU_SET_ZERO(&transId, sizeof(RgrCfgTransId));

   transId.trans[0] = MAC_GNB_CFG;
   transId.trans[1] = cfgReq->action;
   transId.trans[7] = trans_id & 0x000000ff; trans_id >>= 8;
   transId.trans[6] = trans_id & 0x000000ff; trans_id >>= 8;
   transId.trans[5] = trans_id & 0x000000ff; trans_id >>= 8;
   transId.trans[4] = trans_id & 0x000000ff; trans_id >>= 8;

   pst.selector  = DU_SELECTOR_LC;
   pst.srcEnt    = ENTDUAPP;
   pst.dstEnt    = ENTRG;
   pst.dstInst   = (Inst)0;
   pst.dstProcId = DU_PROC;
   pst.srcProcId = DU_PROC;
   pst.region    = duCb.init.region;
   pst.event     = (Event) EVTMACSCHCFGREQ;

   cmPkRgrCfgReq(&pst, transId, cfgReq);

   return ROK;
}

/**********************************************************************
  End of file
 **********************************************************************/
