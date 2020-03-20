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
#include "du_cfg.h"
#include "PLMN-Identity.h"
#include "RICeventTriggerDefinition.h"

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
   U16 idx;
   S16 ret;

   for(idx=0; idx<cellsToActivate.list.count; idx++)
   {
      U16 nci;
      U16 pci;
      DuCellCb *cellCb = NULLP;

      Cells_to_be_Activated_List_Item_t cell = cellsToActivate.list.array[idx]->\
          value.choice.Cells_to_be_Activated_List_Item;

      bitStringToInt(&cell.nRCGI.nRCellIdentity, &nci);
      if(cell.nRPCI)
      {
         pci = *cell.nRPCI;
      }
      if(ROK != (cmHashListFind(&(duCb.cellLst), (U8*) &nci, sizeof(nci),
                  0, (PTR*)cellCb)))
      {
         return RFAILED;
      }
      if(!cellCb)
      {
			DU_LOG("\nDU_APP : HashList Find failed for nci [%d]", nci);
         return RFAILED;
      }
      cellCb->cellStatus = ACTIVATION_IN_PROGRESS; 
      cellCb->cellInfo.nrPci = pci;

      /* Now remove this cell from configured list and move to active list */
      ret = cmHashListDelete(&(duCb.actvCellLst), (PTR)(cellCb)); 
      if(ret != ROK)
      {
         DU_LOG("\nDU_APP : HashListInsert into ActvCellLst failed for [%d]", nci);
      }
      ret = cmHashListInsert(&(duCb.actvCellLst), (PTR)(cellCb), 
            (U8 *)&(nci), (U16) sizeof(nci));

      if(ret != ROK)
      {
         DU_LOG("\nDU_APP : HashListInsert into ActvCellLst failed for [%d]", nci);
         break;
      }
      else
      {
         DU_LOG("\nDU_APP : HashListInsert into ActvCellLst successful for [%d]", nci);
      }
   }

   /* Start sending scheduler config */
   if(ret == ROK)
   {
      //TODO: uncomment later duSendSchGnbCfg(); 
   }

   return ROK;
}

/******************************************************************
*
* @brief Processes E2 Setup Response sent by RIC
*
* @details
*
*    Function : procE2SetupRsp
*
*    Functionality: Processes E2 Setup Response sent by RIC
*
* @params[in] E2AP_PDU_t ASN decoded E2AP message
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/
S16 procE2SetupRsp(E2AP_PDU_t *e2apMsg)
{
   E2setupResponse_t *e2SetRspMsg;
   E2apMsgDb e2SetupRspDb;
   U8 idx; 

   DU_LOG("\nE2AP : E2 Setup Response received"); 
   duCb.e2Status = TRUE; //Set E2 status as true
   e2SetRspMsg = &e2apMsg->choice.successfulOutcome->value.choice.E2setupResponse;

   for(idx=0; idx<e2SetRspMsg->protocolIEs.list.count; idx++)
   {
      switch(e2SetRspMsg->protocolIEs.list.array[idx]->id)
      {
         case ProtocolIE_IDE2_id_GlobalRIC_ID:
         {
            /* To store the Ric Id Params */
            U32 recvBufLen;             
            memset(&e2SetupRspDb.plmn, 0, sizeof(PLMN_IdentityE2_t));

            recvBufLen = sizeof(e2SetRspMsg->protocolIEs.list.array[idx]->value.choice.GlobalRIC_ID.pLMN_Identity);

            bitStringToInt(&e2SetRspMsg->protocolIEs.list.array[idx]->value.choice.GlobalRIC_ID.ric_ID, &e2SetupRspDb.ricId);
            
            aper_decode(0, &asn_DEF_PLMN_IdentityE2, (void **)&e2SetupRspDb.plmn, &e2SetRspMsg->protocolIEs.list.array[idx]->value.choice.GlobalRIC_ID.pLMN_Identity, recvBufLen, 0, 0);
            //xer_fprint(stdout, &asn_DEF_PLMN_IdentityE2, &e2SetupRspDb.plmn);

            break;
         }
         default:
            DU_LOG("\nE2AP : Invalid IE received in E2SetupRsp:%ld",
                  e2SetRspMsg->protocolIEs.list.array[idx]->id);
            break;
      }
   }
   RETVALUE(ROK);
}

/******************************************************************
*
* @brief Processes RIC Subscription Req sent by RIC
*
* @details
*
*    Function : procRicSubsReq
*
*    Functionality: Processes E2 Setup Response sent by CU
*
* @params[in] E2AP_PDU_t ASN decoded E2AP message
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/

S16 procRicSubsReq(E2AP_PDU_t *e2apMsg)
{
   S16 ret = ROK;
   U8 idx; 
   U8 ied; 
   RICsubscriptionRequest_t *ricSubsReq;
   RICaction_ToBeSetup_ItemIEs_t *actionItem;
   E2apMsgDb ricReqDb;
  
   DU_LOG("\nE2AP : Ric Subscription request received"); 
   ricSubsReq = &e2apMsg->choice.initiatingMessage->value.choice.RICsubscriptionRequest;

   for(idx=0; idx<ricSubsReq->protocolIEs.list.count; idx++)
   {
      switch(ricSubsReq->protocolIEs.list.array[idx]->id)
      {
         case ProtocolIE_IDE2_id_RICrequestID:
         {
            ricReqDb.ricReqId = ricSubsReq->protocolIEs.list.array[idx]->value.choice.RICrequestID.ricRequestorID;
            ricReqDb.ricInstanceId = ricSubsReq->protocolIEs.list.array[idx]->value.choice.RICrequestID.ricInstanceID;
            break;
         }
         case ProtocolIE_IDE2_id_RANfunctionID:
         {
            ricReqDb.ranFuncId = ricSubsReq->protocolIEs.list.array[idx]->value.choice.RANfunctionID; 
            break;
         }
         case ProtocolIE_IDE2_id_RICsubscriptionDetails:
         {
            U32 recvBufLen;             
            memset(&ricReqDb.ricEventTrigger, 0, sizeof(RICeventTriggerDefinition_t));

            recvBufLen = sizeof(ricSubsReq->protocolIEs.list.array[idx]->value.choice.RICsubscriptionDetails.ricEventTriggerDefinition);

            aper_decode(0, &asn_DEF_RICeventTriggerDefinition, (void **)&ricReqDb.ricEventTrigger, &(ricSubsReq->protocolIEs.list.array[idx]->value.choice.RICsubscriptionDetails.ricEventTriggerDefinition), recvBufLen, 0, 0);
            //xer_fprint(stdout, &asn_DEF_RICeventTriggerDefinition, &ricReqDb.ricEventTrigger);

            actionItem = *ricSubsReq->protocolIEs.list.array[idx]->value.choice.RICsubscriptionDetails.ricAction_ToBeSetup_List.list.array;
            
            for(ied = 0; ied < ricSubsReq->protocolIEs.list.array[idx]->value.choice.\
                                RICsubscriptionDetails.ricAction_ToBeSetup_List.list.count; ied++)
            {
               switch(actionItem->id)
               {
                  case ProtocolIE_IDE2_id_RICaction_ToBeSetup_Item:
                  {
                     ricReqDb.ricActionId = actionItem->value.choice.RICaction_ToBeSetup_Item.ricActionID;
                     ricReqDb.ricActionType = actionItem->value.choice.RICaction_ToBeSetup_Item.ricActionType;
                     break;
                  }
                  default:
                     DU_LOG("\nE2AP : Invalid IE received in RicSetupLst:%ld",actionItem->id);
                  break;
               }
            }
 
            break;
         }

         default:
            DU_LOG("\nE2AP : Invalid IE received in Ric SubsReq:%ld",
                  ricSubsReq->protocolIEs.list.array[idx]->id);
            break;
      }
   }
   ret = BuildAndSendRicSubscriptionRsp();

   RETVALUE(ret);
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
 
   /* Build and send Mac Cell Cfg Paramaters */
   //ret = duBuildAndSendMacCellCfg();
   ret = BuildAndSendDUConfigUpdate();

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
