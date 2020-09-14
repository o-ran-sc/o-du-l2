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
/* This file contains UE management handling functionality for DU APP */
#include "common_def.h"
#include "lrg.h"
#include "lrg.x"
#include "ckw.h"
#include "ckw.x"
#include "kwu.h"
#include "kwu.x"
#include "lkw.h"
#include "lkw.x"
#include "legtp.h"
#include "du_app_mac_inf.h"
#include "du_app_rlc_inf.h"
#include "du_cfg.h"
#include "du_utils.h"
#include<ProtocolIE-Field.h>
#include "F1AP-PDU.h"
#include "du_mgr.h"
#include "du_f1ap_msg_hdl.h"
#include "du_ue_mgr.h"

#ifdef EGTP_TEST
U32 sduId = 0;
#endif

DuMacDlCcchInd packMacDlCcchIndOpts[] =
{
   packMacDlCcchInd,   /* Loose coupling */
   MacProcDlCcchInd,    /* TIght coupling */
   packMacDlCcchInd    /* Light weight-loose coupling */
};

DuMacUeCreateReq packMacUeCreateReqOpts[] =
{
   packDuMacUeCreateReq,       /* Loose coupling */
   MacProcUeCreateReq,          /* TIght coupling */
   packDuMacUeCreateReq,       /* Light weight-loose coupling */
};

DuRlcUlUeCreateReq packRlcUlUeCreateReqOpts[] =
{
   packDuRlcUlUeCreateReq,       /* Loose coupling */
   RlcUlProcUeCreateReq,          /* TIght coupling */
   packDuRlcUlUeCreateReq,       /* Light weight-loose coupling */
};

DuDlRrcMsgToRlcFunc duSendDlRrcMsgToRlcOpts[] =
{
   packDlRrcMsgToRlc,          /* Loose coupling */ 
   RlcProcDlRrcMsgTransfer,       /* Tight coupling */
   packDlRrcMsgToRlc           /* Light weight-loose coupling */
};

/*******************************************************************
 *
 * @brief Handles EGTP data from CU 
 *
 * @details
 *
 *    Function : duHdlEgtpData
 *
 *    Functionality: 
 *      Processes EGTP header and sends data to RLC
 *
 * @params[in]  Pointer to EGTP Message 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t duHdlEgtpDlData(EgtpMsg  *egtpMsg)
{

   /* TODO : Extract RbId/UeID/CellID/SduId from database
      using tunnel id in egtp header */

   DU_LOG("\nDU_APP : Processing DL data");
#ifdef EGTP_TEST
   Pst pst;
   KwuDatReqInfo datReqInfo;

   datReqInfo.rlcId.rbId = RB_ID;
   datReqInfo.rlcId.rbType = CM_LTE_DRB;
   datReqInfo.rlcId.ueId = UE_ID;
   datReqInfo.rlcId.cellId = NR_CELL_ID;

   datReqInfo.sduId = ++sduId;
   datReqInfo.lcType = CM_LTE_LCH_DTCH;

   /* Filling pst and Sending to RLC DL */
   pst.selector  = ODU_SELECTOR_LWLC;
   pst.srcEnt    = ENTDUAPP;
   pst.dstEnt    = ENTKW;
   pst.dstInst   = RLC_DL_INST;
   pst.dstProcId = DU_PROC;
   pst.srcProcId = DU_PROC;
   pst.region    = duCb.init.region;

   cmPkKwuDatReq(&pst, &datReqInfo, egtpMsg->msg);
#else
   //duBuildAndSendDlRrcMessageToRlc();
#endif
   return ROK;
}

/*******************************************************************
 *
 * @brief Handles UL data and send to CU
 *
 * @details
 *
 *    Function : duHdlRlcUlData
 *
 *    Functionality: 
 *     Processes UL Data from RLC and sends to CU
 * 
 *  @params[in]  Pointer to EGTP Message 
 *  @return ROK     - success
 *          RFAILED - failure
 * 
 *****************************************************************/

uint8_t duHdlRlcUlData(Pst *pst, KwuDatIndInfo* datInd, Buffer *mBuf)
{
   DU_LOG("\nDU_APP : Received UL Data at DU_APP");

   /* Send UL data to CU via EGTP */
   duSendEgtpDatInd(mBuf);
   ODU_PUT_MSG_BUF(mBuf);

   return ROK;
}

/******************************************************************
 *
 * @brief Builds and Sends DL CCCH Ind to MAC
 *
 * @details
 *
 *    Function : duBuildAndSendDlCcchInd 
 *
 *    Functionality: Builds and sends DL CCCH Ind Msg to MAC
 *
 * @params[in] dlCcchMsg - uint8_t*
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t duBuildAndSendDlCcchInd(uint16_t *cellId, uint16_t *crnti, \
      DlCcchMsgType msgType, uint8_t *dlCcchMsg, uint16_t dlCcchMsgSize)
{
   uint8_t ret                  = ROK;
   uint16_t idx2;
   DlCcchIndInfo *dlCcchIndInfo = NULLP;
   Pst pst;
   memset(&pst, 0, sizeof(Pst));

   DU_LOG("\nDU APP : Building and Sending DL CCCH Ind to MAC");

   DU_ALLOC_SHRABL_BUF(dlCcchIndInfo, sizeof(DlCcchIndInfo));

   if(!dlCcchIndInfo)
   {
      DU_LOG("\nDU APP : Memory alloc failed while building DL CCCH Ind");
      return RFAILED;
   }

   dlCcchIndInfo->cellId = *cellId;
   dlCcchIndInfo->crnti = *crnti;
   dlCcchIndInfo->msgType = msgType;
   dlCcchIndInfo->dlCcchMsgLen = dlCcchMsgSize;

   DU_ALLOC_SHRABL_BUF(dlCcchIndInfo->dlCcchMsg, dlCcchIndInfo->dlCcchMsgLen);
   if(!dlCcchIndInfo->dlCcchMsg)
   {
      DU_LOG("\nDU APP : Memory alloc failed while building DL CCCH Ind");
      DU_FREE_SHRABL_BUF(DU_APP_MEM_REGION, DU_POOL, dlCcchIndInfo, sizeof(DlCcchIndInfo));
      return RFAILED;
   }
   for(idx2 = 0; idx2 < dlCcchIndInfo->dlCcchMsgLen; idx2++)
   {
      dlCcchIndInfo->dlCcchMsg[idx2] = dlCcchMsg[idx2];
   }
   DU_FREE(dlCcchMsg, dlCcchMsgSize);

   /* Fill Pst */
   FILL_PST_DUAPP_TO_MAC(pst, EVENT_MAC_DL_CCCH_IND);
   ret = (*packMacDlCcchIndOpts[pst.selector])(&pst, dlCcchIndInfo);
   if(ret != ROK)
   {
      DU_LOG("\nDU_APP : Failure in sending DL CCCH to MAC");
      DU_FREE_SHRABL_BUF(DU_APP_MEM_REGION, DU_POOL, dlCcchIndInfo->dlCcchMsg,\
	    dlCcchIndInfo->dlCcchMsgLen);
      DU_FREE_SHRABL_BUF(DU_APP_MEM_REGION, DU_POOL, dlCcchIndInfo, \
	    sizeof(DlCcchIndInfo));
      ret = RFAILED; 
   }

   return ret;

}

/*******************************************************************
 *
 * @brief Build and Send DL RRC Message transfer to RLC
 *
 * @details
 *
 *    Function : duBuildAndSendDlRrcMessageToRlc
 *
 *    Functionality:
 *      Build and Send DL RRC Message transfer to RLC
 *
 * @params[in] Cell ID
 *             UE Index
 *             Logical Channgel ID
 *             RRC Message
 *             RRC Message Length
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t duBuildAndSendDlRrcMessageToRlc(uint16_t cellId, RlcUeCfg ueCfg, \
   uint8_t lcId, uint16_t rrcMsgLen, uint8_t *rrcMsg)
{
   Pst  pst;
   uint8_t ret;
   uint8_t lcIdx;
   RlcDlRrcMsgInfo  *dlRrcMsgInfo;

   DU_ALLOC_SHRABL_BUF(dlRrcMsgInfo, sizeof(RlcDlRrcMsgInfo));
   if(!dlRrcMsgInfo)
   {
      DU_LOG("\nDU APP : Memory allocation failed for dlRrcMsgInfo in \
         duBuildAndSendDlRrcMessageToRlc");
      DU_FREE_SHRABL_BUF(DU_APP_MEM_REGION, DU_POOL, rrcMsg, rrcMsgLen);
      return RFAILED;
   }

   /* Filling up the RRC msg info */
   dlRrcMsgInfo->cellId = cellId;
   dlRrcMsgInfo->ueIdx = ueCfg.ueIdx;
   for(lcIdx = 0; lcIdx <= MAX_NUM_LOGICAL_CHANNELS; lcIdx++)
   {
      if(ueCfg.rlcBearerCfg[lcIdx].lcId == lcId)
      {
         dlRrcMsgInfo->rbId = ueCfg.rlcBearerCfg[lcIdx].rbId;
	 dlRrcMsgInfo->rbType = ueCfg.rlcBearerCfg[lcIdx].rbType;
         dlRrcMsgInfo->lcId = ueCfg.rlcBearerCfg[lcIdx].lcId;
	 dlRrcMsgInfo->lcType = ueCfg.rlcBearerCfg[lcIdx].lcType;
	 break;
      }
   }
   dlRrcMsgInfo->execDup = false;
   dlRrcMsgInfo->deliveryStaRpt = true;
   dlRrcMsgInfo->rrcMsg = rrcMsg;
   dlRrcMsgInfo->msgLen = rrcMsgLen;

   /* Filling post structure and sending msg */ 
   memset(&pst, 0, sizeof(Pst));
   FILL_PST_DUAPP_TO_RLC(pst, RLC_DL_INST, EVENT_DL_RRC_MSG_TRANS_TO_RLC);
   ret = (*duSendDlRrcMsgToRlcOpts[pst.selector])(&pst, dlRrcMsgInfo);
   if(ret != ROK)
   {
      DU_FREE_SHRABL_BUF(DU_APP_MEM_REGION, DU_POOL, rrcMsg, rrcMsgLen);
      DU_FREE_SHRABL_BUF(DU_APP_MEM_REGION, DU_POOL, dlRrcMsgInfo, sizeof(RlcDlRrcMsgInfo));
      return RFAILED;
   }

   return ROK;
} 

/*******************************************************************
 *
 * @brief Process UE context setup request from CU
 *
 * @details
 *
 *    Function : procUeCintextSetupReq
 *
 *    Functionality: Process UE context setup request from CU
 *
 * @params[in] F1AP message
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t procUeContextSetupReq(F1AP_PDU_t *f1apMsg)
{
   uint8_t    ret = ROK;
   uint8_t    ieIdx, byteIdx, ueIdx;
   uint8_t    *rrcMsg = NULLP;
   uint16_t   rrcMsgLen;
   uint16_t   cellId, cellIdx;
   uint32_t   gnbDuUeF1apId; /* GNB DU UE F1AP ID */
   uint32_t   gnbCuUeF1apId; /* GNB CU UE F1AP ID */
   DuUeCb     *ueCb = NULLP;
   UEContextSetupRequest_t   *ueSetReq = NULLP;
  
   ueSetReq = &f1apMsg->choice.initiatingMessage->value.choice.UEContextSetupRequest;

   /* TODO : fetch remaining values from f1ap msg */
   for(ieIdx=0; ieIdx < ueSetReq->protocolIEs.list.count; ieIdx++)
   {
      switch(ueSetReq->protocolIEs.list.array[ieIdx]->id)
      {
         case ProtocolIE_ID_id_gNB_CU_UE_F1AP_ID:
	    {
	       gnbCuUeF1apId = ueSetReq->protocolIEs.list.array[ieIdx]->value.choice.GNB_CU_UE_F1AP_ID;
	       break;
            }
	 case ProtocolIE_ID_id_gNB_DU_UE_F1AP_ID:
	    {
	       gnbDuUeF1apId = ueSetReq->protocolIEs.list.array[ieIdx]->value.choice.GNB_DU_UE_F1AP_ID;
	       break;
	    }
         case ProtocolIE_ID_id_ServCellIndex:
	    {
	       cellIdx = ueSetReq->protocolIEs.list.array[ieIdx]->value.choice.ServCellIndex;
	       break;
	    }
         case ProtocolIE_ID_id_RRCContainer: 
         {
            rrcMsgLen = ueSetReq->protocolIEs.list.array[ieIdx]->value.choice.RRCContainer.size;
            DU_ALLOC_SHRABL_BUF(rrcMsg, rrcMsgLen);
	    if(!rrcMsg)
	    {
	       DU_LOG("\nDU APP : Memory allocation failed for RRC Msg in procUeCtxtSetupReq");
	       return RFAILED;
	    }
	    memcpy(rrcMsg, ueSetReq->protocolIEs.list.array[ieIdx]->value.choice.RRCContainer.buf,\
	       rrcMsgLen);
	    break;
         }
	 
      }
   }

   cellId = duCb.actvCellLst[cellIdx]->cellId;
   for(ueIdx = 0; ueIdx < MAX_NUM_UE; ueIdx++)
   {
      if((duCb.actvCellLst[cellIdx]->ueCb[ueIdx].gnbCuUeF1apId == gnbCuUeF1apId) &&
         (duCb.actvCellLst[cellIdx]->ueCb[ueIdx].gnbDuUeF1apId == gnbDuUeF1apId &&
	 duCb.actvCellLst[cellIdx]->ueCb[ueIdx].ueState == UE_ACTIVE))
      {
         ueCb = &duCb.actvCellLst[cellIdx]->ueCb[ueIdx];
	 break;
      }
   }

   /* TODO :  send RB config to MAC/RLC */

   /* Sending DL RRC Message to RLC */
   if(ueIdx != MAX_NUM_UE)
   {
      ret = duBuildAndSendDlRrcMessageToRlc(cellId, ueCb->rlcUeCfg, SRB_ID_1, rrcMsgLen, rrcMsg); 
   }
   else
   {
      DU_LOG("\nDU APP : No UE found for CuUeF1apId[%d] and DuUeF1apId[%d]", \
         gnbCuUeF1apId, gnbDuUeF1apId);
      DU_FREE_SHRABL_BUF(DU_APP_MEM_REGION, DU_POOL, rrcMsg, rrcMsgLen);
      ret = RFAILED;
   }

   return ret;
}

/******************************************************************
 *
 * @brief Processes DL RRC Message Transfer  sent by CU
 *
 * @details
 *
 *    Function : procDlRrcMsgTrans 
 *
 *    Functionality: Processes DL RRC Message Transfer sent by CU
 *
 * @params[in] F1AP_PDU_t ASN decoded F1AP message
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t procDlRrcMsgTrans(F1AP_PDU_t *f1apMsg)
{
   DLRRCMessageTransfer_t *dlRrcMsg = NULLP;
   uint8_t                *dlCcchMsg = NULLP;
   uint8_t                idx, ret, srbId;
   uint16_t               idx2, crnti, cellId, dlCcchMsgSize;
   uint32_t               gnbCuUeF1apId, gnbDuUeF1apId;


   DU_LOG("\nDU_APP : DL RRC message transfer Recevied");
   dlRrcMsg = &f1apMsg->choice.initiatingMessage->value.choice.DLRRCMessageTransfer;

   ret = ROK;

   for(idx=0; idx<dlRrcMsg->protocolIEs.list.count; idx++)
   {
      switch(dlRrcMsg->protocolIEs.list.array[idx]->id)
      {
	 case ProtocolIE_ID_id_gNB_CU_UE_F1AP_ID:
	    {
	       gnbCuUeF1apId = dlRrcMsg->protocolIEs.list.array[idx]->value.choice.GNB_CU_UE_F1AP_ID;
	       break;
	    }
	 case ProtocolIE_ID_id_gNB_DU_UE_F1AP_ID:
	    {
	       gnbDuUeF1apId = dlRrcMsg->protocolIEs.list.array[idx]->value.choice.GNB_DU_UE_F1AP_ID;
	       break;
	    }
	 case ProtocolIE_ID_id_SRBID:
	    {
	       srbId = dlRrcMsg->protocolIEs.list.array[idx]->value.choice.SRBID;
	       break;
	    }
	 case ProtocolIE_ID_id_ExecuteDuplication:
	    break;

	 case ProtocolIE_ID_id_RRCContainer:
	    {
	       if(dlRrcMsg->protocolIEs.list.array[idx]->value.choice.RRCContainer.size > 0)
	       {
		  dlCcchMsgSize = dlRrcMsg->protocolIEs.list.array[idx]->value.choice.RRCContainer.size;
		  DU_ALLOC(dlCcchMsg, dlCcchMsgSize);
		  for(idx2 = 0; idx2 < dlCcchMsgSize; idx2++)
		  {
		     dlCcchMsg[idx2] = \
		        dlRrcMsg->protocolIEs.list.array[idx]->value.choice.RRCContainer.buf[idx2];
		  }
	       }
	       else
	       {
		  DU_LOG("\nDU_APP : RRC Container Size is invalid:%ld",\
			dlRrcMsg->protocolIEs.list.array[idx]->value.choice.RRCContainer.size);
	       }
	       break;
	    }

	 default:
	    DU_LOG("\nDU_APP : Invalid IE received in DL RRC Msg Transfer:%ld",
		  dlRrcMsg->protocolIEs.list.array[idx]->id);
      }
   }

   for(idx=0; idx<duCb.numUe; idx++)
   {
      if(gnbDuUeF1apId == duCb.ueCcchCtxt[idx].gnbDuUeF1apId)
      {
	 crnti  = duCb.ueCcchCtxt[idx].crnti;
	 cellId = duCb.ueCcchCtxt[idx].cellId;
	 break;
      }
   }
   if(srbId == SRB_ID_1) //RRC connection setup
   {
      ret = duBuildAndSendDlCcchInd(&cellId, &crnti, RRC_SETUP, dlCcchMsg, dlCcchMsgSize);
      if(ret)
      {
	 DU_LOG("\nDU_APP: Falied at duBuildAndSendDlCcchInd()");
      }
      else
      {
	 if(duCb.actvCellLst[cellId-1]->numActvUes < MAX_NUM_UE)
	 {
	    ret = duCreateUeCb(&duCb.ueCcchCtxt[idx], gnbCuUeF1apId);
	    if(ret)
	    {
	       DU_LOG("\nDU_APP: Failed at duCreateUeCb for cellId [%d]", duCb.ueCcchCtxt[idx].cellId);
	       ret = RFAILED;
	    }
	 }
	 else
	 {
	    DU_LOG("\nDU_APP: Max Active UEs has reached");
	    ret = RFAILED;
	 }
      }
   }            
   return ret;
}

/******************************************************************
 *
 * @brief Generates GNB DU Ue F1AP ID
 *
 * @details
 *
 *    Function : genGnbDuUeF1apId
 *
 *    Functionality: Generates GNB DU Ue F1AP ID
 *
 * @params[in] void
 * @return gnbDuF1apId
 *
 * ****************************************************************/
uint32_t genGnbDuUeF1apId()
{
   static uint32_t gnbDuUeF1apId = 0;

   return ++gnbDuUeF1apId;
}

/******************************************************************
 *
 * @brief Processes UL CCCH Ind recvd from MAC
 *
 * @details
 *
 *    Function : duProcUlCcchInd
 *
 *    Functionality: Processes UL CCCH Ind recvd from MAC
 *
 * @params[in] UlCcchIndInfo *ulCcchIndInfo
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t duProcUlCcchInd(UlCcchIndInfo *ulCcchIndInfo)
{

   uint8_t ret = ROK;
   uint32_t gnbDuUeF1apId = 0;

   gnbDuUeF1apId = genGnbDuUeF1apId();

   /* Store Ue mapping */
   duCb.ueCcchCtxt[duCb.numUe].gnbDuUeF1apId = gnbDuUeF1apId;
   duCb.ueCcchCtxt[duCb.numUe].crnti         = ulCcchIndInfo->crnti;
   duCb.ueCcchCtxt[duCb.numUe].cellId        = ulCcchIndInfo->cellId;

   duCb.numUe++;

   ret = (BuildAndSendInitialRrcMsgTransfer(gnbDuUeF1apId, ulCcchIndInfo->crnti, ulCcchIndInfo->ulCcchMsgLen,
	    ulCcchIndInfo->ulCcchMsg));
   if(ret != ROK)
   {
      DU_LOG("\nDU_APP : BuildAndSendInitialRrcMsgTransfer failed");
   }

   DU_FREE_SHRABL_BUF(MAC_MEM_REGION, MAC_POOL, ulCcchIndInfo->ulCcchMsg, ulCcchIndInfo->ulCcchMsgLen);
   DU_FREE_SHRABL_BUF(MAC_MEM_REGION, MAC_POOL, ulCcchIndInfo, sizeof(UlCcchIndInfo));

   return ret;

}

/******************************************************************
 *
 * @brief Fills Initial DL Bandwidth Part
 *
 * @details
 *
 *    Function : fillInitDlBwp
 *
 *    Functionality: Fills Initial DL Bandwidth Part
 *
 *
 *****************************************************************/
void fillInitDlBwp(InitialDlBwp *initDlBwp)
{
   uint8_t idx = 0;
   uint8_t freqDomainResource[FREQ_DOM_RSRC_SIZE] = {0};
   uint8_t coreset0EndPrb, coreset1StartPrb, coreset1NumPrb;


   if(initDlBwp)
   {
      /* Filling PDCCH Config */
      initDlBwp->pdcchPresent = TRUE;
      if(initDlBwp->pdcchPresent)
      {
	 initDlBwp->pdcchCfg.numCRsetToAddMod = PDCCH_CTRL_RSRC_SET_ONE_ID;
	 memset(initDlBwp->pdcchCfg.cRSetToAddModList, 0, MAX_NUM_CRSET);
	 if(initDlBwp->pdcchCfg.numCRsetToAddMod <= MAX_NUM_CRSET)
	 {
	    initDlBwp->pdcchCfg.cRSetToAddModList[idx].cRSetId = \
	       PDCCH_CTRL_RSRC_SET_ONE_ID;
	    memset(initDlBwp->pdcchCfg.cRSetToAddModList[idx].freqDomainRsrc, 0,\
	       FREQ_DOM_RSRC_SIZE); 
	    coreset0EndPrb = CORESET0_END_PRB;
	    coreset1StartPrb = coreset0EndPrb +6;
	    coreset1NumPrb = CORESET1_NUM_PRB;
	    /* calculate the PRBs */
	    schAllocFreqDomRscType0(((coreset1StartPrb)/6), (coreset1NumPrb/6), freqDomainResource);
	    memcpy(initDlBwp->pdcchCfg.cRSetToAddModList[idx].freqDomainRsrc, freqDomainResource,
	       FREQ_DOM_RSRC_SIZE);

	    initDlBwp->pdcchCfg.cRSetToAddModList[idx].duration = \
	       PDCCH_CTRL_RSRC_SET_ONE_DURATION;
	    initDlBwp->pdcchCfg.cRSetToAddModList[idx].cceRegMappingType = \
	       CCE_REG_MAPPINGTYPE_PR_NONINTERLEAVED;
	    initDlBwp->pdcchCfg.cRSetToAddModList[idx].precoderGranularity = \
	       ALL_CONTIGUOUS_RBS;
	    initDlBwp->pdcchCfg.cRSetToAddModList[idx].dmrsScramblingId = \
	       SCRAMBLING_ID;
	 }
	 initDlBwp->pdcchCfg.numCRsetToRel = 0;
	 /* Filling Serach Space */
	 initDlBwp->pdcchCfg.numSearchSpcToAddMod = PDCCH_CTRL_RSRC_SET_ONE_ID;
	 memset(initDlBwp->pdcchCfg.searchSpcToAddModList, 0, MAX_NUM_CRSET);
	 if(initDlBwp->pdcchCfg.numSearchSpcToAddMod <= MAX_NUM_CRSET)
	 {
	    initDlBwp->pdcchCfg.searchSpcToAddModList[idx].searchSpaceId =\
	       PDCCH_SRCH_SPC_TWO_ID;
	    initDlBwp->pdcchCfg.searchSpcToAddModList[idx].cRSetId = \
	       PDCCH_CTRL_RSRC_SET_ONE_ID;
	    initDlBwp->pdcchCfg.searchSpcToAddModList[idx].\
	       mSlotPeriodicityAndOffset = SLOTPERIODICITYANDOFFSET_PR_SL1;
	    memset(initDlBwp->pdcchCfg.searchSpcToAddModList[idx].mSymbolsWithinSlot, 0,\
	       MONITORING_SYMB_WITHIN_SLOT_SIZE);
	    initDlBwp->pdcchCfg.searchSpcToAddModList[idx].mSymbolsWithinSlot[idx] =\
	       PDCCH_SYMBOL_WITHIN_SLOT;
	    initDlBwp->pdcchCfg.searchSpcToAddModList[idx].numCandidatesAggLevel1 =\
	       AGGREGATIONLEVEL_N8; 
	    initDlBwp->pdcchCfg.searchSpcToAddModList[idx].numCandidatesAggLevel2 =\
	       AGGREGATIONLEVEL_N8; 
	    initDlBwp->pdcchCfg.searchSpcToAddModList[idx].numCandidatesAggLevel4 =\
	       AGGREGATIONLEVEL_N4; 
	    initDlBwp->pdcchCfg.searchSpcToAddModList[idx].numCandidatesAggLevel8 =\
	       AGGREGATIONLEVEL_N2; 
	    initDlBwp->pdcchCfg.searchSpcToAddModList[idx].numCandidatesAggLevel16 =\
	       AGGREGATIONLEVEL_N1;
	    initDlBwp->pdcchCfg.searchSpcToAddModList[idx].searchSpaceType = \
	       SEARCHSPACETYPE_PR_UE_SPECIFIC;
	    initDlBwp->pdcchCfg.searchSpcToAddModList[idx].ueSpecificDciFormat =\
	       PDCCH_SRCH_SPC_TWO_UE_SPEC_DCI_FORMAT;

	    initDlBwp->pdcchCfg.numSearchSpcToRel = 0;

	 }
      }
      /* Filling PDSCH Config */
      initDlBwp->pdschPresent = TRUE;
      if(initDlBwp->pdschPresent)
      {
	 initDlBwp->pdschCfg.dmrsDlCfgForPdschMapTypeA.addPos = ADDITIONALPOSITION_POS0;
	 initDlBwp->pdschCfg.resourceAllocType = RESOURCEALLOCATION_TYPE1;
	 initDlBwp->pdschCfg.numTimeDomRsrcAlloc = 1;
	 initDlBwp->pdschCfg.timeDomRsrcAllociList[idx].mappingType = \
	    MAPPING_TYPEA;
	 initDlBwp->pdschCfg.timeDomRsrcAllociList[idx].startSymbol = PDSCH_START_SYMBOL; 
	 initDlBwp->pdschCfg.timeDomRsrcAllociList[idx].symbolLength = PDSCH_LENGTH_SYMBOL;
	 initDlBwp->pdschCfg.timeDomRsrcAllociList[idx].startSymbolAndLength = \
	    calcSliv(PDSCH_START_SYMBOL, PDSCH_LENGTH_SYMBOL);
	 initDlBwp->pdschCfg.rbgSize = RBG_SIZE_CONFIG1;
	 initDlBwp->pdschCfg.numCodeWordsSchByDci = CODEWORDS_SCHED_BY_DCI_N1;
	 initDlBwp->pdschCfg.bundlingType = TYPE_STATIC_BUNDLING;
      }
   }

}

/******************************************************************
 *
 * @brief Fills Initial UL Bandwidth Part
 *
 * @details
 *
 *    Function : fillInitUlBwp
 *
 *    Functionality: Fills Initial UL Bandwidth Part
 *
 *
 *****************************************************************/
void fillInitUlBwp(InitialUlBwp *initUlBwp)
{
   uint8_t idx;
   if(initUlBwp)
   {
      initUlBwp->pucchPresent = FALSE;

      /*Filling PUSCH Config */
      initUlBwp->puschPresent = TRUE;
      if(initUlBwp->puschPresent)
      {
	 initUlBwp->puschCfg.dmrsUlCfgForPuschMapTypeA.addPos = ADDITIONALPOSITION_POS0; 
	 initUlBwp->puschCfg.dmrsUlCfgForPuschMapTypeA.transPrecodDisabled. \
	    scramblingId0 = SCRAMBLING_ID; 
	 initUlBwp->puschCfg.resourceAllocType = RESOURCEALLOCATION_TYPE1;
	 initUlBwp->puschCfg.numTimeDomRsrcAlloc = 1;
	 idx = 0;
	 if(initUlBwp->puschCfg.numTimeDomRsrcAlloc <= MAX_NUM_UL_ALLOC)
	 {
	    initUlBwp->puschCfg.timeDomRsrcAllocList[idx].k2 = PUSCH_K2;
	    initUlBwp->puschCfg.timeDomRsrcAllocList[idx].mappingType =\
	       MAPPING_TYPEA;
	    initUlBwp->puschCfg.timeDomRsrcAllocList[idx].startSymbol = PUSCH_START_SYMBOL;
	    initUlBwp->puschCfg.timeDomRsrcAllocList[idx].symbolLength = PUSCH_LENGTH_SYMBOL;
	    initUlBwp->puschCfg.timeDomRsrcAllocList[idx].startSymbolAndLength =\
	       calcSliv(PUSCH_START_SYMBOL, PUSCH_LENGTH_SYMBOL);
	 }
	 initUlBwp->puschCfg.transformPrecoder = TRANSFORM_PRECODER_DISABLED;
      }
   }
   else
   {
      DU_LOG("\n DUAPP: Memory is NULL of InitalUlBwp");
   }

}
/******************************************************************
 *
 * @brief Fills SpCell Group Info
 *
 * @details
 *
 *    Function : fillSpCellGrpInfo
 *
 *    Functionality: Fills Sp Cell Group Info
 *
 *
 *****************************************************************/
void fillSpCellGrpInfo(SpCellCfg *spCell)
{
   if(spCell)
   {
      spCell->servCellIdx = SERV_CELL_IDX;
      /* Filling Initial Dl Bwp */
      fillInitDlBwp(&spCell->servCellCfg.initDlBwp);

      spCell->servCellCfg.numDlBwpToAdd    = 0; 
      spCell->servCellCfg.firstActvDlBwpId = ACTIVE_DL_BWP_ID;
      spCell->servCellCfg.defaultDlBwpId   = ACTIVE_DL_BWP_ID;
      spCell->servCellCfg.bwpInactivityTmr = NULLP;
      spCell->servCellCfg.pdschServCellCfg.maxMimoLayers = NULLP;
      spCell->servCellCfg.pdschServCellCfg.maxCodeBlkGrpPerTb = NULLP;
      spCell->servCellCfg.pdschServCellCfg.codeBlkGrpFlushInd = NULLP;
      spCell->servCellCfg.pdschServCellCfg.xOverhead = NULLP;
      spCell->servCellCfg.pdschServCellCfg.numHarqProcForPdsch =\
         NUM_HARQ_PROC_FOR_PDSCH_N_16;
      /* Filling Initial UL Bwp*/
      fillInitUlBwp(&spCell->servCellCfg.initUlBwp);
      spCell->servCellCfg.numUlBwpToAdd     = 0; 
      spCell->servCellCfg.firstActvUlBwpId  = ACTIVE_DL_BWP_ID; 
   }
   else
   {
      DU_LOG("\n DU_APP: Memory is NULL for SpCellGrp");
   }
}

/******************************************************************
 *
 * @brief Fills Physical Cell Group Info
 *
 * @details
 *
 *    Function : fillPhyCellGrpInfo
 *
 *    Functionality: Fills Physical Cell Group Info
 *
 *
 *****************************************************************/
void fillPhyCellGrpInfo(PhyCellGrpCfg *cellGrp)
{
   if(cellGrp)
   {
      cellGrp->pdschHarqAckCodebook = PDSCH_HARQ_ACK_CODEBOOK_DYNAMIC;
      cellGrp->pNrFr1 = P_NR_FR1;
   }
   else
   {
      DU_LOG("\nDUAPP: Memory is NULL for Physical Cell Group");
   }
}

/******************************************************************
 *
 * @brief Fills Mac Cell Group Info
 *
 * @details
 *
 *    Function : fillMacCellGrpInfo
 *
 *    Functionality: Fills Mac Cell Group Info
 *
 *
 *****************************************************************/
void fillMacCellGrpInfo(MacCellGrpCfg *cellGrp)
{
   uint8_t idx;

   if(cellGrp)
   {
      /* Filling Scheduling Request Config */
      cellGrp->schReqCfg.addModListCount = 1;
      if(cellGrp->schReqCfg.addModListCount <= MAX_NUM_SR_CFG_PER_CELL_GRP)
      {
	 for(idx = 0; idx < cellGrp->schReqCfg.addModListCount; idx++)
	 {
	    cellGrp->schReqCfg.addModList[idx].schedReqId    = SCH_REQ_ID;
	    cellGrp->schReqCfg.addModList[idx].srProhibitTmr = SR_PROHIBIT_MS_32;
	    cellGrp->schReqCfg.addModList[idx].srTransMax    = SR_TRANS_MAX_N_16;
	 }
      }
      cellGrp->schReqCfg.relListCount = 0;

      /* Filling Tag config */
      cellGrp->tagCfg.addModListCount = 1;
      if(cellGrp->tagCfg.addModListCount <= MAC_NUM_TAGS)
      {
	 for(idx = 0; idx < cellGrp->tagCfg.addModListCount; idx++)
	 {
	    cellGrp->tagCfg.addModList[idx].tagId = TAG_ID;
	    cellGrp->tagCfg.addModList[idx].timeAlignTimer = TIME_ALIGNMENT_TIMER_INFINITY;
	 }
      }
      cellGrp->tagCfg.relListCount = 0;

      /* Filling BSR config */
      cellGrp->bsrTmrCfg.periodicTimer = PERIODIC_BSR_TMR;
      cellGrp->bsrTmrCfg.retxTimer = RETX_BSR_TMR;
      cellGrp->bsrTmrCfg.srDelayTimer = SR_DELAY_TMR;

      /* Filling PHR config */
      cellGrp->phrCfgSetupPres = true;
      cellGrp->phrCfg.periodicTimer = PHR_PERIODIC_TIMER_INFINITY;
      cellGrp->phrCfg.prohibitTimer = PHR_PROHIBIT_TIMER_SF_0;
      cellGrp->phrCfg.txPowerFactor = PHR_TX_PWR_FACTOR_CHANGE_INFINITY;
      cellGrp->phrCfg.multiplePHR   = false;
      cellGrp->phrCfg.dummy         = false;
      cellGrp->phrCfg.phrType2OtherCell = false;
      cellGrp->phrCfg.phrOtherCG = PHR_MODE_OTHER_CG_REAL;  

   }
   else
   {
      DU_LOG("\nDUAPP: Memory is NULL for Master Cell Group");
   }
}

/******************************************************************
 *
 * @brief Fills Logical Channel Config List
 *
 * @details
 *
 *    Function : fillLcCfgList
 *
 *    Functionality: Fills Logical channel Config List
 *
 *
 *****************************************************************/
void fillLcCfgList(LcCfg *lcCfgInfo)
{
   if(lcCfgInfo)
   {
      lcCfgInfo->lcId = SRB_ID_1;
      lcCfgInfo->drbQos = NULLP;
      lcCfgInfo->snssai = NULLP;
      lcCfgInfo->ulLcCfg = NULLP;
      lcCfgInfo->dlLcCfg.lcp = LC_PRIORITY_1;

#if 0
      /* TODO: To be filled on receving UE CONTEXT SETUP from CU */
      /* Filling Qos characteristics */
      lcCfgInfo->drbQos.fiveQiType = QoS_Characteristics_PR_non_Dynamic_5QI;
      lcCfgInfo->drbQos.u.nonDyn5Qi.fiveQi = 0;
      lcCfgInfo->drbQos.u.nonDyn5Qi.priorLevel = 0;
      lcCfgInfo->drbQos.u.nonDyn5Qi.avgWindow = 0;
      lcCfgInfo->drbQos.u.nonDyn5Qi.maxDataBurstVol = 0;

      /* Filling NgRAN */
      lcCfgInfo->drbQos.ngRanRetPri.priorityLevel = PriorityLevel_highest;
      lcCfgInfo->drbQos.ngRanRetPri.preEmptionCap = \
						    Pre_emptionCapability_may_trigger_pre_emption;
      lcCfgInfo->drbQos.ngRanRetPri.preEmptionVul = \
						    Pre_emptionVulnerability_not_pre_emptable;

      /* Filling Grb Qos */
      lcCfgInfo->drbQos.grbQosInfo.maxFlowBitRateDl  = 0;
      lcCfgInfo->drbQos.grbQosInfo.maxFlowBitRateUl  = 0;
      lcCfgInfo->drbQos.grbQosInfo.guarFlowBitRateDl = 0;
      lcCfgInfo->drbQos.grbQosInfo.guarFlowBitRateUl = 0;

      /* Filling S-NSSAI */
      /* TODO :To be filled when UE Context Setup Request is sent from CU */
      /* Filling UL Logical Channel Config */
      lcCfgInfo->ulLcCfg.priority = 0;
      lcCfgInfo->ulLcCfg.lcGroup  = 0;
      lcCfgInfo->ulLcCfg.schReqId = 0;
      lcCfgInfo->ulLcCfg.pbr = 0;
      lcCfgInfo->ulLcCfg.bsd = 0;

      /* Filling DL Logical Channel Config */
      lcCfgInfo->dlLcCfg.lcp = 0;
#endif
   }
   else
   {
      DU_LOG("\n Memory is null for LcCfgList");
   }
}

/******************************************************************
 *
 * @brief Fills MacUeCfg structure
 *
 * @details
 *
 *    Function : fillMacUeCfg
 *
 *    Functionality: Fills MacUeCfg
 *
 *
 *****************************************************************/
void fillMacUeCfg(uint16_t cellId, uint8_t ueIdx,\
  uint16_t crnti, MacUeCfg *ueCfg)
{
   uint8_t idx;
   ueCfg->cellId       = cellId;
   ueCfg->ueIdx        = ueIdx;
   ueCfg->crnti        = crnti;
   /* Filling MacCellGroup Config */ 
   fillMacCellGrpInfo(&ueCfg->macCellGrpCfg);
   /* Filling PhyCellGroup Config */ 
   fillPhyCellGrpInfo(&ueCfg->phyCellGrpCfg);
   /* Filling SpCellGroup Config */ 
   fillSpCellGrpInfo(&ueCfg->spCellCfg);
   /* Filling AMBR for UL and DL */ 
   ueCfg->maxAggrBitRate = NULLP;
   /* Filling LC Context */
   ueCfg->numLcs = SRB_ID_1;
   if(ueCfg->numLcs < MAX_NUM_LOGICAL_CHANNELS)
   {
      for(idx = 0; idx < ueCfg->numLcs; idx++)
      {   
	 fillLcCfgList(&ueCfg->lcCfgList[idx]);
      }
   }

}

/******************************************************************
 *
 * @brief Fills Rlc AM Information
 *
 * @details
 *
 *    Function : fillAmInfo
 *
 *    Functionality: Fills Rlc AM Information
 *
 *
 *****************************************************************/
void fillAmInfo(AmBearerCfg *amCfg)
{
   /* DL AM */
   amCfg->dlAmCfg.snLenDl     = AM_SIZE_12;
   amCfg->dlAmCfg.pollRetxTmr = POLL_RETX_TMR_45MS;
   amCfg->dlAmCfg.pollPdu     = POLL_PDU_TMR_INFINITY;
   amCfg->dlAmCfg.pollByte    = POLL_BYTES_INFINITY;
   amCfg->dlAmCfg.maxRetxTh   = RETX_TH_8;   
 
   /* UL AM */
   amCfg->ulAmCfg.snLenUl     = AM_SIZE_12;
   amCfg->ulAmCfg.reAssemTmr  = RE_ASM_40MS; 
   amCfg->ulAmCfg.statProhTmr = PROH_35MS;

}

/******************************************************************
 *
 * @brief Fills RLC UM Bi Directional Information
 *
 * @details
 *
 *    Function : fillUmBiDirInfo
 *
 *    Functionality: Fills RLC UM Bi Directional Information
 *
 *
 *****************************************************************/
void fillUmBiDirInfo(UmBiDirBearerCfg *umBiDirCfg)
{
   /* UL UM BI DIR INFO */
   umBiDirCfg->ulUmCfg.snLenUlUm = UM_SIZE_12;
   umBiDirCfg->ulUmCfg.reAssemTmr = RE_ASM_40MS;

   /* DL UM BI DIR INFO */
   umBiDirCfg->dlUmCfg.snLenDlUm  = UM_SIZE_12;

}

/******************************************************************
 *
 * @brief Fills RLC UM Uni Directional UL Information
 *
 * @details
 *
 *    Function : fillUmUniDirUlInfo
 *
 *    Functionality: Fills RLC UM Uni Directional Info
 *
 *
 *****************************************************************/
void fillUmUniDirUlInfo(UmUniDirUlBearerCfg *umUniDirUlCfg)
{
   umUniDirUlCfg->ulUmCfg.snLenUlUm = UM_SIZE_12;
   umUniDirUlCfg->ulUmCfg.reAssemTmr = RE_ASM_40MS;
}

/******************************************************************
 *
 * @brief Fills RLC UM Uni Directional DL Information
 *
 * @details
 *
 *    Function : fillUmUniDirDlInfo
 *
 *    Functionality: Fills RLC UM Uni Directional DL Info
 *
 *
 *****************************************************************/
void fillUmUniDirDlInfo(UmUniDirDlBearerCfg *umUniDirDlCfg)
{
   umUniDirDlCfg->dlUmCfg.snLenDlUm  = UM_SIZE_12;
}

/******************************************************************
 *
 * @brief Fills RlcBearerCfg structure
 *
 * @details
 *
 *    Function : fillRlcBearerCfg
 *
 *    Functionality: Fills Rlc Bearer Cfg
 *
 *
 *****************************************************************/
void fillRlcBearerCfg(uint16_t cellId, uint8_t ueIdx, RlcUeCfg *ueCfg)
{
   uint8_t idx;
   ueCfg->cellId       = cellId;
   ueCfg->ueIdx        = ueIdx;
   ueCfg->numLcs       = SRB_ID_1; 

   for(idx = 0; idx < ueCfg->numLcs; idx++)
   {
      ueCfg->rlcBearerCfg[idx].rbId         = RB_ID_SRB;
      ueCfg->rlcBearerCfg[idx].rbType       = RB_TYPE_SRB;
      ueCfg->rlcBearerCfg[idx].lcId         = SRB_ID_1;
      ueCfg->rlcBearerCfg[idx].lcType       = LCH_DCCH;
      ueCfg->rlcBearerCfg[idx].rlcMode      = RLC_AM;
      switch(ueCfg->rlcBearerCfg[idx].rlcMode)
      {
         case RLC_AM:
	    memset(&ueCfg->rlcBearerCfg[idx].u.amCfg, 0, sizeof(AmBearerCfg));
            fillAmInfo(&ueCfg->rlcBearerCfg[idx].u.amCfg);
            break;
         case RLC_UM_BI_DIRECTIONAL:
	    memset(&ueCfg->rlcBearerCfg[idx].u.umBiDirCfg, 0, sizeof(UmBiDirBearerCfg));
            fillUmBiDirInfo(&ueCfg->rlcBearerCfg[idx].u.umBiDirCfg);
            break;
         case RLC_UM_UNI_DIRECTIONAL_UL:
	    memset(&ueCfg->rlcBearerCfg[idx].u.umUniDirUlCfg, 0, sizeof(UmUniDirUlBearerCfg));
            fillUmUniDirUlInfo(&ueCfg->rlcBearerCfg[idx].u.umUniDirUlCfg);
            break;
         case RLC_UM_UNI_DIRECTIONAL_DL:
	    memset(&ueCfg->rlcBearerCfg[idx].u.umUniDirDlCfg, 0, sizeof(UmUniDirDlBearerCfg));
            fillUmUniDirDlInfo(&ueCfg->rlcBearerCfg[idx].u.umUniDirDlCfg);
            break;
         default :
            DU_LOG("\nDU_APP: Rlc Mode invalid %d", ueCfg->rlcBearerCfg[idx].rlcMode);
            break;
      }
   }
}

/******************************************************************
 *
 * @brief creates UE context
 *
 * @details
 *
 *    Function : duCreateUeCb
 *
 *    Functionality: Creates UE Conetxt
 *
 * @params[in] UeCcchCtxt Pointer
 *             UeIdx Pointer
 *
 * @return ROK     - success
 *         RFAILED - failure
 * ****************************************************************/
uint8_t duCreateUeCb(UeCcchCtxt *ueCcchCtxt, uint32_t gnbCuUeF1apId)
{
   uint8_t cellIdx = 0;
   uint8_t ret     = ROK;
   uint8_t ueIdx;

   for(cellIdx = 0; cellIdx < MAX_NUM_CELL; cellIdx++)
   {
      if(ueCcchCtxt->cellId == duCb.actvCellLst[cellIdx]->cellId)
      {
	 GET_UE_IDX(ueCcchCtxt->crnti, ueIdx);
	 DU_LOG("\nDU_APP: Filling UeCb for ueIdx [%d]", ueIdx);

	 duCb.actvCellLst[cellIdx]->ueCb[ueIdx-1].gnbDuUeF1apId = ueCcchCtxt->gnbDuUeF1apId;
	 duCb.actvCellLst[cellIdx]->ueCb[ueIdx-1].gnbCuUeF1apId = gnbCuUeF1apId;
	 duCb.actvCellLst[cellIdx]->ueCb[ueIdx-1].ueState       = UE_ACTIVE;

	 /* Filling Mac Ue Config */ 
	 memset(&duCb.actvCellLst[cellIdx]->ueCb[ueIdx-1].macUeCfg, 0, sizeof(MacUeCfg));
         ret = duBuildAndSendUeCreateReqToMac(ueCcchCtxt->cellId, ueIdx, ueCcchCtxt->crnti,\
                &duCb.actvCellLst[cellIdx]->ueCb[ueIdx-1].macUeCfg);
         if(ret)
            DU_LOG("\nDU_APP: Failed to send UE create request to MAC");
         
	 /* Filling Rlc Ue Config */
         memset(&duCb.actvCellLst[cellIdx]->ueCb[ueIdx-1].rlcUeCfg, 0, sizeof(RlcUeCfg));
         ret = duBuildAndSendUeCreateReqToRlc(ueCcchCtxt->cellId, ueIdx, \
                &duCb.actvCellLst[cellIdx]->ueCb[ueIdx-1].rlcUeCfg);
         if(ret)
            DU_LOG("\nDU_APP: Failed to send UE create request to RLC");

	 duCb.actvCellLst[cellIdx]->numActvUes++;
	 memset(ueCcchCtxt, 0, sizeof(UeCcchCtxt));
      }
   }
   return ret;
}

/******************************************************************
 *
 * @brief Builds and Send UE Create Request to MAC
 *
 * @details
 *
 *    Function : duBuildAndSendUeCreateReqToMac
 *
 *    Functionality: Builds and Send UE Create Request to MAC
 *
 * @Params[in]  cellId,
 *              ueIdx
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t duBuildAndSendUeCreateReqToMac(uint16_t cellId, uint8_t ueIdx,\
   uint16_t crnti, MacUeCfg *duMacUeCfg)
{
   uint8_t ret = ROK;
   MacUeCfg *macUeCfg = NULLP;
   Pst pst;
   memset(&pst, 0, sizeof(Pst));

   fillMacUeCfg(cellId, ueIdx, crnti, duMacUeCfg);

   /* Fill Pst */
   FILL_PST_DUAPP_TO_MAC(pst, EVENT_MAC_UE_CREATE_REQ);

   /* Copying ueCb to a sharable buffer */
   DU_ALLOC_SHRABL_BUF(macUeCfg, sizeof(MacUeCfg));
   if(macUeCfg)
   {
      memset(macUeCfg, 0, sizeof(MacUeCfg));
      memcpy(macUeCfg, &duCb.actvCellLst[cellId - 1]->ueCb[ueIdx -1].macUeCfg, sizeof(MacUeCfg));
      DU_LOG("\nDU_APP: Sending UE create request to MAC");

      /* Processing one Ue at a time to MAC */
      ret = (*packMacUeCreateReqOpts[pst.selector])(&pst, macUeCfg);
      if(ret)
      {
	 DU_LOG("\nDU_APP : Failure in sending Ue Create Req to MAC");
	 DU_FREE_SHRABL_BUF(DU_APP_MEM_REGION, DU_POOL, macUeCfg, sizeof(MacUeCfg));
	 ret = RFAILED;
      }
   }
   else
   {
      DU_LOG("\n DU_APP: Memory alloc failed at duBuildAndSendUeCreateReqToMac()");
      ret = RFAILED;
   }
   return ret;
}

/*******************************************************************
 *
 * @brief Handle UE create response from MAC
 *
 * @details
 *
 *    Function : DuHandleMacUeCreateRsp
 *
 *    Functionality: Handle UE create response from MAC
 *
 * @params[in] Pointer to MacUeCfgRsp and Pst 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t DuHandleMacUeCreateRsp(Pst *pst, MacUeCfgRsp *cfgRsp)
{
   if(cfgRsp->result == MAC_DU_APP_RSP_OK)
   {
      DU_LOG("\nDU APP : MAC UE Create Response : SUCCESS [UE IDX : %d]", cfgRsp->ueIdx);
   }
   else
   {
      DU_LOG("\nDU APP : MAC UE Create Response : FAILURE [UE IDX : %d]", cfgRsp->ueIdx);
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Processes UE create Req to RLC UL
 *
 * @details
 *
 *    Function : duBuildAndSendUeCreateReqToRlc
 *
 *    Functionality: 
 *     Processes UE create Req to RLC UL
 * 
 *  @params[in]  cellId,
 *               ueIdx,
 *               Pointer to RlcUeCfg
 *  @return ROK     - success
 *          RFAILED - failure
 * 
 *****************************************************************/

uint8_t duBuildAndSendUeCreateReqToRlc(uint16_t cellId, uint8_t ueIdx, RlcUeCfg *duRlcUeCfg)
{
   uint8_t ret = ROK;
   RlcUeCfg *rlcUeCfg = NULLP;
   Pst pst;
   memset(&pst, 0, sizeof(Pst));
  
   fillRlcBearerCfg(cellId, ueIdx, duRlcUeCfg);
   FILL_PST_DUAPP_TO_RLC(pst, RLC_UL_INST, EVENT_RLC_UL_UE_CREATE_REQ);

   /* Copying ueCfg to a sharable buffer */
   DU_ALLOC_SHRABL_BUF(rlcUeCfg, sizeof(RlcUeCfg));
   if(rlcUeCfg)
   {
      memset(rlcUeCfg, 0, sizeof(RlcUeCfg));
      memcpy(rlcUeCfg, duRlcUeCfg, sizeof(RlcUeCfg));
      /* Processing one Ue at a time to RLC */
      DU_LOG("\nDU_APP: Sending UE create request to RLC UL");
      ret = (*packRlcUlUeCreateReqOpts[pst.selector])(&pst, rlcUeCfg);
      if(ret)
      {
         DU_LOG("\nDU_APP : Failure in sending Ue Create Req to RLC");
         DU_FREE_SHRABL_BUF(DU_APP_MEM_REGION, DU_POOL, rlcUeCfg, sizeof(RlcUeCfg));
         ret = RFAILED;
      }
   }
   else
   {
      DU_LOG("\n DU_APP: Memory alloc failed at duBuildAndSendUeCreateReqToRlc()");
      ret = RFAILED;
   }
   return ret;
}



/*******************************************************************
 *
 * @brief Processes UE create Rsp received from RLC UL
 *
 * @details
 *
 *    Function : DuProcRlcUlUeCfgRsp
 *
 *    Functionality: 
 *     Processes UE create Rsp received from RLC UL
 * 
 *  @params[in]  Post structure
 *               Pointer to RlcCfgCfm
 *  @return ROK     - success
 *          RFAILED - failure
 * 
 *****************************************************************/

uint8_t DuProcRlcUlUeCreateRsp(Pst *pst, RlcUeCfgRsp *cfgRsp)
{
   uint8_t ret = ROK;

   if(cfgRsp)
   {
      if(cfgRsp->result == RLC_DU_APP_RSP_OK)
      {
         DU_LOG("\nDU_APP: RLC UE Create Response : SUCCESS [UE IDX:%d]", cfgRsp->ueIdx);
      }
      else
      {
         DU_LOG("\nDU_APP: RLC UE Create Response : FAILED [UE IDX:%d, REASON :%d]",\
	    cfgRsp->ueIdx, cfgRsp->reason);
	 ret = RFAILED;
      }
      DU_FREE_SHRABL_BUF(DU_APP_MEM_REGION, DU_POOL, cfgRsp, sizeof(RlcUeCfgRsp));
   }
   else
   {
      DU_LOG("\nDU_APP: Received RLC Ue Create Response is NULL");
      ret = RFAILED;
   }
   return ret;
}
/**********************************************************************
  End of file
 ***********************************************************************/
