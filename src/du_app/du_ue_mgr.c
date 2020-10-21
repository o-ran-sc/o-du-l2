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
   packDuMacUeCreateReq       /* Light weight-loose coupling */
};

DuRlcUlUeCreateReq packRlcUlUeCreateReqOpts[] =
{
   packDuRlcUlUeCreateReq,       /* Loose coupling */
   RlcUlProcUeCreateReq,          /* TIght coupling */
   packDuRlcUlUeCreateReq       /* Light weight-loose coupling */
};

DuDlRrcMsgToRlcFunc duSendDlRrcMsgToRlcOpts[] =
{
   packDlRrcMsgToRlc,          /* Loose coupling */ 
   RlcProcDlRrcMsgTransfer,       /* Tight coupling */
   packDlRrcMsgToRlc           /* Light weight-loose coupling */
};

DuRlcUlUeReconfigReq packRlcUlUeReconfigReqOpts[] =
{
   packDuRlcUlUeReconfigReq,       /* Loose coupling */
   RlcUlProcUeReconfigReq,       /* TIght coupling */
   packDuRlcUlUeReconfigReq       /* Light weight-loose coupling */
};

DuMacUeReconfigReq packMacUeReconfigReqOpts[] =
{
   packDuMacUeReconfigReq,       /* Loose coupling */
   MacProcUeReconfigReq,       /* TIght coupling */
   packDuMacUeReconfigReq     /* Light weight-loose coupling */
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
   pst.dstEnt    = ENTRLC;
   pst.dstInst   = RLC_DL_INST;
   pst.dstProcId = DU_PROC;
   pst.srcProcId = DU_PROC;
   pst.region    = duCb.init.region;

   cmPkKwuDatReq(&pst, &datReqInfo, egtpMsg->msg);
#else
   //duBuildAndSendDlRrcMsgToRlc();
#endif
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
      DlCcchMsgType msgType, uint16_t dlCcchMsgSize, uint8_t *dlCcchMsg)
{
   uint8_t ret                  = ROK;
   uint16_t idx2;
   DlCcchIndInfo *dlCcchIndInfo = NULLP;
   Pst pst;

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
 *    Function : duBuildAndSendDlRrcMsgToRlc
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
uint8_t duBuildAndSendDlRrcMsgToRlc(uint16_t cellId, RlcUeCfg ueCfg, \
   uint8_t lcId, bool execDup, bool deliveryStaReq, uint16_t rrcMsgLen, uint8_t *rrcMsg)
{
   Pst  pst;
   uint8_t ret;
   uint8_t lcIdx;
   RlcDlRrcMsgInfo  *dlRrcMsgInfo;

   DU_ALLOC_SHRABL_BUF(dlRrcMsgInfo, sizeof(RlcDlRrcMsgInfo));
   if(!dlRrcMsgInfo)
   {
      DU_LOG("\nDU APP : Memory allocation failed for dlRrcMsgInfo in \
         duBuildAndSendDlRrcMsgToRlc");
      DU_FREE_SHRABL_BUF(DU_APP_MEM_REGION, DU_POOL, rrcMsg, rrcMsgLen);
      return RFAILED;
   }

   /* Filling up the RRC msg info */
   dlRrcMsgInfo->cellId = cellId;
   dlRrcMsgInfo->ueIdx = ueCfg.ueIdx;
   for(lcIdx = 0; lcIdx <= MAX_NUM_LC; lcIdx++)
   {
      if(ueCfg.rlcLcCfg[lcIdx].lcId == lcId)
      {
         dlRrcMsgInfo->rbType = ueCfg.rlcLcCfg[lcIdx].rbType;
         dlRrcMsgInfo->rbId = ueCfg.rlcLcCfg[lcIdx].rbId;
	 dlRrcMsgInfo->lcType = ueCfg.rlcLcCfg[lcIdx].lcType;
         dlRrcMsgInfo->lcId = ueCfg.rlcLcCfg[lcIdx].lcId;
	 break;
      }
   }
   dlRrcMsgInfo->execDup = execDup;
   dlRrcMsgInfo->deliveryStaRpt = deliveryStaReq;
   dlRrcMsgInfo->rrcMsg = rrcMsg;
   dlRrcMsgInfo->msgLen = rrcMsgLen;

   /* Filling post structure and sending msg */ 
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
   uint8_t                *rrcMsgPdu = NULLP;
   uint8_t                ieIdx, ueIdx, cellIdx;
   uint8_t                ret, srbId;
   uint16_t               byteIdx, crnti, cellId, rrcMsgSize;
   uint32_t               gnbCuUeF1apId, gnbDuUeF1apId;
   bool                   execDup = false;
   bool                   deliveryStaRpt = false;
   bool                   ueFound = false;
   bool                   ueCcchCtxtFound = false; 

   DU_LOG("\nDU_APP : DL RRC message transfer Recevied");
   dlRrcMsg = &f1apMsg->choice.initiatingMessage->value.choice.DLRRCMessageTransfer;

   ret = ROK;

   for(ieIdx=0; ieIdx<dlRrcMsg->protocolIEs.list.count; ieIdx++)
   {
      switch(dlRrcMsg->protocolIEs.list.array[ieIdx]->id)
      {
	 case ProtocolIE_ID_id_gNB_CU_UE_F1AP_ID:
	    {
	       gnbCuUeF1apId = dlRrcMsg->protocolIEs.list.array[ieIdx]->value.choice.GNB_CU_UE_F1AP_ID;
	       break;
	    }
	 case ProtocolIE_ID_id_gNB_DU_UE_F1AP_ID:
	    {
	       gnbDuUeF1apId = dlRrcMsg->protocolIEs.list.array[ieIdx]->value.choice.GNB_DU_UE_F1AP_ID;
	       break;
	    }
	 case ProtocolIE_ID_id_SRBID:
	    {
	       srbId = dlRrcMsg->protocolIEs.list.array[ieIdx]->value.choice.SRBID;
	       break;
	    }
	 case ProtocolIE_ID_id_ExecuteDuplication:
	    {
	       execDup = true;
	       break;
	    }
	 case ProtocolIE_ID_id_RRCContainer:
	    {
	       if(dlRrcMsg->protocolIEs.list.array[ieIdx]->value.choice.RRCContainer.size > 0)
	       {
		  rrcMsgSize = dlRrcMsg->protocolIEs.list.array[ieIdx]->value.choice.RRCContainer.size;
		  DU_ALLOC(rrcMsgPdu, rrcMsgSize);
		  if(!rrcMsgPdu)
		  {
		      DU_LOG("\nDU_APP : Memory allocation failed in procDlRrcMsgTrans"); 
		      return RFAILED;
		  }
		  for(byteIdx = 0; byteIdx < rrcMsgSize; byteIdx++)
		  {
		     rrcMsgPdu[byteIdx] = \
		        dlRrcMsg->protocolIEs.list.array[ieIdx]->value.choice.RRCContainer.buf[byteIdx];
		  }
	       }
	       else
	       {
		  DU_LOG("\nDU_APP : RRC Container Size is invalid:%ld",\
			dlRrcMsg->protocolIEs.list.array[ieIdx]->value.choice.RRCContainer.size);
                  return RFAILED;
	       }
	       break;
	    }
         case ProtocolIE_ID_id_RRCDeliveryStatusRequest:
	    {
	       deliveryStaRpt = true;
	       break;
	    }
	 default:
	    DU_LOG("\nDU_APP : Invalid IE received in DL RRC Msg Transfer:%ld",
		  dlRrcMsg->protocolIEs.list.array[ieIdx]->id);
      }
   }

   if(srbId == SRB1_LCID) //RRC connection setup
   {
      for(ueIdx=0; ueIdx < duCb.numUe; ueIdx++)
      {
         if(gnbDuUeF1apId == duCb.ueCcchCtxt[ueIdx].gnbDuUeF1apId)
         {
	    ueCcchCtxtFound = true;
	    crnti  = duCb.ueCcchCtxt[ueIdx].crnti;
	    cellId = duCb.ueCcchCtxt[ueIdx].cellId;
	    break;
         }
      }
   }
   if(ueCcchCtxtFound)
   {
      ret = duBuildAndSendDlCcchInd(&cellId, &crnti, RRC_SETUP, rrcMsgSize, rrcMsgPdu);
      if(ret)
      {
	 DU_LOG("\nDU_APP: Falied at duBuildAndSendDlCcchInd()");
      }
      else
      {
	 if(duCb.actvCellLst[cellId-1]->numActvUes < MAX_NUM_UE)
	 {
	    ret = duCreateUeCb(&duCb.ueCcchCtxt[ueIdx], gnbCuUeF1apId);
	    if(ret)
	    {
	       DU_LOG("\nDU_APP: Failed at duCreateUeCb for cellId [%d]", \
		     duCb.ueCcchCtxt[ueIdx].cellId);
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
   else
   {
      for(cellIdx = 0; cellIdx < MAX_NUM_CELL; cellIdx++)
      {
	 for(ueIdx = 0 ; ueIdx < MAX_NUM_UE; ueIdx++)
	 {
	    if((gnbCuUeF1apId == duCb.actvCellLst[cellIdx]->ueCb[ueIdx].gnbCuUeF1apId)
		  && (gnbDuUeF1apId == duCb.actvCellLst[cellIdx]->ueCb[ueIdx].gnbDuUeF1apId))
	    {
	       ueFound = true;
	       ret = duBuildAndSendDlRrcMsgToRlc(duCb.actvCellLst[cellIdx]->cellId, \
		     duCb.actvCellLst[cellIdx]->ueCb[ueIdx].rlcUeCfg, srbId, \
		     execDup, deliveryStaRpt,  rrcMsgSize, rrcMsgPdu);
	       break; 
	    }
	 }
	 if(ueFound)
	    break;
      }
      if(!ueFound)
	 ret = RFAILED;
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
 * @brief Fills Default UL LC Cfg
 *
 * @details
 *
 *    Function : fillDefUlLcCfg
 *
 *    Functionality: Fills Default UL LC Cfg
 *
 *
 *****************************************************************/
void fillDefUlLcCfg(UlLcCfg *ulLcCfg)
{
   ulLcCfg->priority = LC_PRIORITY_1;
   ulLcCfg->lcGroup =  0;
   ulLcCfg->schReqId = 0;
   ulLcCfg->pbr = PBR_KBPS_INFINITY;
   ulLcCfg->bsd = BSD_MS_1000;
}

/******************************************************************
 *
 * @brief Fills Initial DL Bandwidth Part
 *
 * @details
 *
 *    Function : fillDefInitDlBwp
 *
 *    Functionality: Fills Initial DL Bandwidth Part
 *
 *
 *****************************************************************/
void fillDefInitDlBwp(InitialDlBwp *initDlBwp)
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
	    freqDomRscAllocType0(((coreset1StartPrb)/6), (coreset1NumPrb/6), freqDomainResource);
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
	 initDlBwp->pdschCfg.bundlingInfo.StaticBundling.size = 0;
      }
   }

}

/******************************************************************
 *
 * @brief Fills Initial UL Bandwidth Part
 *
 * @details
 *
 *    Function : fillDefInitUlBwp
 *
 *    Functionality: Fills Initial UL Bandwidth Part
 *
 *
 *****************************************************************/
void fillDefInitUlBwp(InitialUlBwp *initUlBwp)
{
   uint8_t idx;
   if(initUlBwp)
   {
      initUlBwp->pucchPresent = FALSE;

      /*Filling PUSCH Config */
      initUlBwp->puschPresent = TRUE;
      if(initUlBwp->puschPresent)
      {
         initUlBwp->puschCfg.dataScramblingId = SCRAMBLING_ID;
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
 *    Function : fillDefSpCellGrpInfo
 *
 *    Functionality: Fills Sp Cell Group Info
 *
 *
 *****************************************************************/
void fillDefSpCellGrpInfo(SpCellCfg *spCell)
{
   if(spCell)
   {
      spCell->servCellIdx = SERV_CELL_IDX;
      /* Filling Initial Dl Bwp */
      fillDefInitDlBwp(&spCell->servCellCfg.initDlBwp);

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
      fillDefInitUlBwp(&spCell->servCellCfg.initUlBwp);
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
 *    Function : fillDefPhyCellGrpInfo
 *
 *    Functionality: Fills Physical Cell Group Info
 *
 *
 *****************************************************************/
void fillDefPhyCellGrpInfo(PhyCellGrpCfg *cellGrp)
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
 *    Function : fillDefMacCellGrpInfo
 *
 *    Functionality: Fills Mac Cell Group Info
 *
 *
 *****************************************************************/
void fillDefMacCellGrpInfo(MacCellGrpCfg *cellGrp)
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
      cellGrp->bsrTmrCfg.periodicTimer = BSR_PERIODIC_TIMER_SF_10;
      cellGrp->bsrTmrCfg.retxTimer = BSR_RETX_TIMER_SF_320;
      cellGrp->bsrTmrCfg.srDelayTimer = BSR_SR_DELAY_TMR_2560;

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



void fillMacSrb1LcCfg(LcCfg *macLcCfg)
{
   macLcCfg->lcId   = SRB1_LCID;
   macLcCfg->configType = CONFIG_ADD_TYPE;
   macLcCfg->drbQos = NULLP;
   macLcCfg->snssai = NULLP;
   macLcCfg->ulLcCfgPres = true;
   fillDefUlLcCfg(&macLcCfg->ulLcCfg);
}

/******************************************************************
 *
 * @brief Function to fill the Lc cfg from ueSetupReqDb
 *
 * @details
 *
 *    Function : fillLcCfg
 *
 *    Functionality: Function to fill the Lc cfg from ueSetupReqDb
 *
 *
 *****************************************************************/

uint8_t fillLcCfg(LcCfg *lcCfg, LcCfg *ueSetReqDb)
{
   uint8_t ret = ROK; 
   lcCfg->lcId = ueSetReqDb->lcId;
   lcCfg->configType = ueSetReqDb->configType;
   /* Filling DRBQOS */
   if(ueSetReqDb->drbQos)
   {
      if(!lcCfg->drbQos)
      {
         DU_ALLOC_SHRABL_BUF(lcCfg->drbQos, sizeof(DrbQosInfo));
         if(!lcCfg->drbQos)
         {
            DU_LOG("\nDU APP: Memory Alloc failed at drQos at fillLcCfg()");
            ret = RFAILED;
         }
      }
      if(!ret)
      {
         memcpy(lcCfg->drbQos, ueSetReqDb->drbQos, sizeof(DrbQosInfo));
      }
   }
   else
   {
      lcCfg->drbQos = NULLP;
   }

   /* Filling SNSSAI */
   if(ueSetReqDb->snssai && ret == ROK)
   {
      if(!lcCfg->snssai)
      {
         DU_ALLOC_SHRABL_BUF(lcCfg->snssai, sizeof(Snssai));
         if(!lcCfg->snssai)
         {
            DU_LOG("\nDU APP: Memory Alloc failed at snnsai at fillLcCfg()");
            ret = RFAILED;
         }
      }
      if(!ret)
      {
         memcpy(lcCfg->snssai, ueSetReqDb->snssai, sizeof(Snssai));
      }
   }
   else
   {
      lcCfg->snssai = NULLP;
      if(lcCfg->drbQos)
      {
         DU_FREE_SHRABL_BUF(DU_APP_MEM_REGION, DU_POOL, lcCfg->drbQos, sizeof(DrbQosInfo));
         lcCfg->drbQos = NULLP;
      }
   }
   lcCfg->ulLcCfgPres = ueSetReqDb->ulLcCfgPres;
   memcpy(&lcCfg->ulLcCfg, &ueSetReqDb->ulLcCfg, sizeof(UlLcCfg));
   memcpy(&lcCfg->dlLcCfg, &ueSetReqDb->dlLcCfg, sizeof(DlLcCfg));
   return ret;
}

/******************************************************************
 *
 * @brief Function to copy the Lc cfg from ueSetupReqDb
 *
 * @details
 *
 *    Function : getLcCfgToAddMod
 *
 *    Functionality: Function to copy the Lc cfg from ueSetupReqDb
 *
 *
 *****************************************************************/
uint8_t getLcCfgToAddMod(LcCfg *macLcCfg, LcCfg *ueSetReqDb)
{
   uint8_t ret = ROK;
   ret = fillLcCfg(macLcCfg, ueSetReqDb);
   if(ret)
   {
      DU_LOG("\nDU APP: Failed at getLcCfgToAddMod()");
   }
   return ret;
}

/******************************************************************
 *
 * @brief Function to copy the Bit rate from ueSetupReqDb
 *
 * @details
 *
 *    Function : getMaxAggrBitRate
 *
 *    Functionality: Function to copy bit Rate from ueSetupReqDb
 *
 *
 *****************************************************************/

uint8_t getMaxAggrBitRate(MaxAggrBitRate *macBitRate, MaxAggrBitRate *ueDbBitRate)
{
   uint8_t ret = ROK;

   if(ueDbBitRate)
   {
      if(!macBitRate)
      {
         DU_ALLOC_SHRABL_BUF(macBitRate, sizeof(MaxAggrBitRate));
         if(!macBitRate)
         {
            DU_LOG("\nDUAPP: Memory Alloc Failed at getMaxAggrBitRate()");
            ret = RFAILED;
         }
      }
      if(!ret)
      {
         memcpy(macBitRate, ueDbBitRate, sizeof(MaxAggrBitRate));
      }
   }
   else
   {
      macBitRate = NULLP;
   }
   return ret;
}

/******************************************************************
 *
 * @brief Builds and Send UE ReConfig Request to MAC
 *
 * @details
 *
 *    Function : sendUeReCfgReqToMac
 *
 *    Functionality: Builds and Send UE ReConfig Request to MAC
 *
 * @Params[in]  MacUeCfg *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t sendUeReCfgReqToMac(MacUeCfg *macUeCfg)
{
   uint8_t ret = ROK;
   Pst pst;
   
   /* Fill Pst */
   FILL_PST_DUAPP_TO_MAC(pst, EVENT_MAC_UE_RECONFIG_REQ);
   
   if(macUeCfg)
   {
      /* Processing one Ue at a time to MAC */
      DU_LOG("\nDU_APP: Sending Reconfig Request to MAC");
      ret = (*packMacUeReconfigReqOpts[pst.selector])(&pst, macUeCfg);
      if(ret)
      {
	 DU_LOG("\nDU_APP : Failure in sending Ue ReConfig Req to MAC");
	 DU_FREE_SHRABL_BUF(DU_APP_MEM_REGION, DU_POOL, macUeCfg, sizeof(MacUeCfg));
	 ret = RFAILED;
      }
   }
   else
   {
      DU_LOG("\n DU_APP: Memory alloc failed at sendUeReCfgReqToMac()");
      ret = RFAILED;
   }
   return ret;
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
uint8_t fillMacUeCfg(uint16_t cellId, uint8_t ueIdx, uint16_t crnti, \
   UeContextSetupDb *ueCfgDb, MacUeCfg *macUeCfg)
{
   uint8_t ret, dbIdx, lcIdx, cellIdx;
   bool lcIdFound = false;
   MacUeCfg *duMacDb = NULLP;

   ret =ROK;

   if(!ueCfgDb)
   {
      macUeCfg->cellId       = cellId;
      macUeCfg->ueIdx        = ueIdx;
      macUeCfg->crnti        = crnti;
      fillDefMacCellGrpInfo(&macUeCfg->macCellGrpCfg);
      fillDefPhyCellGrpInfo(&macUeCfg->phyCellGrpCfg);
      fillDefSpCellGrpInfo(&macUeCfg->spCellCfg);
      macUeCfg->maxAggrBitRate = NULLP;
      fillMacSrb1LcCfg(&macUeCfg->lcCfgList[0]);
      macUeCfg->numLcs++;
   }
   else
   {
      /* Fetching MacDb from DuUeCb */
      GET_CELL_IDX(cellId, cellIdx);
      duMacDb = &duCb.actvCellLst[cellIdx]->ueCb[ueIdx-1].macUeCfg;
      /* Fetching MaUeCfg List for ADD/MOD/DEL */
      macUeCfg->cellId       = cellId;
      macUeCfg->ueIdx        = ueIdx;
      macUeCfg->crnti        = crnti;
      ret = procUeReCfgCellInfo(macUeCfg, ueCfgDb->cellGrpCfg);
      if(!ret)
      {
	 ret = getMaxAggrBitRate(macUeCfg->maxAggrBitRate, ueCfgDb->maxAggrBitRate);
      }

      /* Filling LC Context */
      for(dbIdx = 0; (dbIdx < ueCfgDb->duUeCfg.numMacLcs && ret == ROK); dbIdx++)
      {
	 if(!ueCfgDb->duUeCfg.macLcCfg[dbIdx].ulLcCfgPres)
	 {
	    /* Filling default UL LC config in MAC if not present */
	    ueCfgDb->duUeCfg.macLcCfg[dbIdx].ulLcCfgPres = true;
	    fillDefUlLcCfg(&ueCfgDb->duUeCfg.macLcCfg[dbIdx].ulLcCfg);
	 }
	 for(lcIdx = 0; lcIdx < duMacDb->numLcs; lcIdx++)
	 {
	    if(ueCfgDb->duUeCfg.macLcCfg[dbIdx].lcId == duMacDb->lcCfgList[lcIdx].lcId)
	    {
	       lcIdFound = true;
	       if((ueCfgDb->duUeCfg.macLcCfg[dbIdx].configType == CONFIG_UNKNOWN) ||
		     (ueCfgDb->duUeCfg.macLcCfg[dbIdx].configType == CONFIG_MOD_TYPE))
	       {
		  ueCfgDb->duUeCfg.macLcCfg[dbIdx].configType = CONFIG_MOD_TYPE;
		  ret = getLcCfgToAddMod(&macUeCfg->lcCfgList[dbIdx], &ueCfgDb->duUeCfg.macLcCfg[dbIdx]);
	       }
	    }
	    else
	       lcIdFound = false;
	 }
	 if(!lcIdFound)
	 {
	    /* ADD/DEL CONFIG */
	    ret = getLcCfgToAddMod(&macUeCfg->lcCfgList[dbIdx], &ueCfgDb->duUeCfg.macLcCfg[dbIdx]);
	 }
	 if(!ret)
	 {
	    macUeCfg->numLcs++;
	 }
	 else
	 {
	    DU_LOG("\n DU_APP: Failed at fillMacUeCfg()"); 
	    break;
	 }
      }/*End of Outer FOR loop */
   }
   return ret;
}

void fillRlcSrb1LcCfg(RlcBearerCfg *rlcLcCfg)
{
   rlcLcCfg->rbId   = SRB1_LCID;
   rlcLcCfg->rbType = RB_TYPE_SRB;
   rlcLcCfg->lcId   = SRB1_LCID;
   rlcLcCfg->lcType = LCH_DCCH;
   rlcLcCfg->rlcMode = RLC_AM;
   rlcLcCfg->rlcModeCfgPres = true;
   rlcLcCfg->configType = CONFIG_ADD_TYPE;
   buildRlcModeDefaultCfg(rlcLcCfg->rlcMode, rlcLcCfg);
}

/*******************************************************************
 *
 * @brief Processes UE ReConfig Req to RLC UL
 *
 * @details
 *
 *    Function : sendUeReCfgReqToRlc
 *
 *    Functionality: 
 *     Processes UE Reconfig Req to RLC UL
 * 
 *  @params[in]     Pointer to RlcUeCfg
 *  @return ROK     - success
 *          RFAILED - failure
 * 
 *****************************************************************/

uint8_t sendUeReCfgReqToRlc(RlcUeCfg *rlcUeCfg)
{
   uint8_t ret;
   Pst pst;
   
   FILL_PST_DUAPP_TO_RLC(pst, RLC_UL_INST, EVENT_RLC_UL_UE_RECONFIG_REQ);
   if(rlcUeCfg)
   {
      /* Processing one Ue at a time to RLC */
      DU_LOG("\nDU_APP: Sending Reconfig Request to RLC UL");
      ret = (*packRlcUlUeReconfigReqOpts[pst.selector])(&pst, rlcUeCfg);
      if(ret)
      {
         DU_LOG("\nDU_APP : Failure in sending Ue Reconfig Req to RLC");
         DU_FREE_SHRABL_BUF(DU_APP_MEM_REGION, DU_POOL, rlcUeCfg, sizeof(RlcUeCfg));
         ret = RFAILED;
      }
   }
   else
   {
      DU_LOG("\n DU_APP: Received RlcUeCfg is NULL at sendUeReCfgReqToRlc()");
      ret = RFAILED;
   }
   return ret;
}

/******************************************************************
 *
 * @brief Fills RlcBearerCfg structure
 *
 * @details
 *
 *    Function : fillRlcUeCfg
 *
 *    Functionality: Fills Rlc Bearer Cfg
 *
 *
 *****************************************************************/
uint8_t fillRlcUeCfg(uint16_t cellId, uint8_t ueIdx,\
   DuUeCfg *ueCfgDb, RlcUeCfg *rlcUeCfg)
{
   uint8_t ret, dbIdx, lcIdx, cellIdx;
   bool lcIdFound = false;
   RlcUeCfg *duRlcDb = NULLP;

   ret = ROK;
   if(!ueCfgDb)
   {
      /* Initial RB being Added */ 
      rlcUeCfg->cellId       = cellId;
      rlcUeCfg->ueIdx        = ueIdx;
      fillRlcSrb1LcCfg(&rlcUeCfg->rlcLcCfg[0]);
      rlcUeCfg->numLcs++;
   }
   else
   {
      /* Fetch RlcDb from DuUeCb */ 
      GET_CELL_IDX(cellId, cellIdx);
      duRlcDb = &duCb.actvCellLst[cellIdx]->ueCb[ueIdx-1].rlcUeCfg;
      /*Filling RlcUeCfg */
      rlcUeCfg->cellId       = cellId;
      rlcUeCfg->ueIdx        = ueIdx;
      for(dbIdx = 0; dbIdx < ueCfgDb->numRlcLcs; dbIdx++)
      {
	 if(!ueCfgDb->rlcLcCfg[dbIdx].rlcModeCfgPres)
	 {
	    /* Filling default config for RLC Mode */
	    ueCfgDb->rlcLcCfg[dbIdx].rlcModeCfgPres = true;
	    buildRlcModeDefaultCfg(ueCfgDb->rlcLcCfg[dbIdx].rlcMode, &ueCfgDb->rlcLcCfg[dbIdx]);
	 }
	 /* Fill Rlc Ue Cfg List for ADD/MOD/DEL */
	 for(lcIdx = 0; lcIdx < duRlcDb->numLcs; lcIdx++)
	 { 
	    if(ueCfgDb->rlcLcCfg[dbIdx].lcId == duRlcDb->rlcLcCfg[lcIdx].lcId)
	    {
	       lcIdFound = true;
	       if((ueCfgDb->rlcLcCfg[dbIdx].configType == CONFIG_UNKNOWN)||
		     (ueCfgDb->rlcLcCfg[dbIdx].configType == CONFIG_MOD_TYPE))
	       {
		  /* MOD */ 
		  ueCfgDb->rlcLcCfg[dbIdx].configType = CONFIG_MOD_TYPE; /* update Db for MOD type */
		  memcpy(&rlcUeCfg->rlcLcCfg[dbIdx], &ueCfgDb->rlcLcCfg[dbIdx], sizeof(RlcBearerCfg));
	       }
	    }
	    else
	       lcIdFound = false;
	 }
	 if(!lcIdFound)
	 {
	    /* ADD/ DEL Config Type */
	    memcpy(&rlcUeCfg->rlcLcCfg[dbIdx], &ueCfgDb->rlcLcCfg[dbIdx], sizeof(RlcBearerCfg));
	 }
	 rlcUeCfg->numLcs++;
      }
   }
   return ret;
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

	 duCb.actvCellLst[cellIdx]->ueCb[ueIdx-1].crnti = ueCcchCtxt->crnti;
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
   Pst pst;
   MacUeCfg *macUeCfg = NULLP;

   ret = fillMacUeCfg(cellId, ueIdx, crnti, NULL, duMacUeCfg);
   if(ret)
   {
      DU_LOG("\nDU APP: Failed at fillMacUeCfg()");
      return RFAILED;
   }
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

/******************************************************************
*
* @brief Function to Delete the Lc cfg from UE APP DB
*
* @details
*
*  Function : delLcCfg
*
*  Functionality: Function to fill the Lc cfg from delLcCfg
*
*
 *****************************************************************/

void  delLcCfg(LcCfg *lcCfg)
{
    /* Deleting DRBQOS */
   if(!lcCfg->drbQos)
   {
      DU_FREE_SHRABL_BUF(DU_APP_MEM_REGION, DU_POOL, lcCfg->drbQos, sizeof(DrbQosInfo));
      lcCfg->drbQos = NULLP;
   }
   /* Deleting SNSSAI */
   if(!lcCfg->snssai)
   {
      DU_FREE_SHRABL_BUF(DU_APP_MEM_REGION, DU_POOL, lcCfg->snssai, sizeof(Snssai));
      lcCfg->snssai = NULLP;
   }
   memset(lcCfg, 0, sizeof(LcCfg));
}

/*******************************************************************
 *
 * @brief To update DuUeCb Mac Cfg
 *
 * @details
 *
 *    Function : duUpdateMacCfg
 *    Functionality:  update DuUeCb MAC Cfg
 *
 * @params[in] DuUeCb Pointer
 *             UeContextSetupDb pointer 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t duUpdateMacCfg(MacUeCfg *macUeCfg, UeContextSetupDb *f1UeDb) 
{
   uint8_t ret, lcIdx, dbIdx, numLcs, lcDelIdx;
   ret = ROK;

   /*Filling Cell Group Cfg*/
   ret =  procUeReCfgCellInfo(macUeCfg, f1UeDb->cellGrpCfg);
   if(!ret)
   {
      ret = getMaxAggrBitRate(macUeCfg->maxAggrBitRate, f1UeDb->maxAggrBitRate);
   }
   /* Filling LC Context */
   for(dbIdx = 0; (dbIdx < f1UeDb->duUeCfg.numMacLcs && ret == ROK); dbIdx++)
   {
      numLcs = macUeCfg->numLcs;
      for(lcIdx = 0; lcIdx < numLcs; lcIdx++)
      {
	 if(f1UeDb->duUeCfg.macLcCfg[dbIdx].lcId ==  macUeCfg->lcCfgList[lcIdx].lcId)
	 {
	    if(f1UeDb->duUeCfg.macLcCfg[dbIdx].configType == CONFIG_MOD_TYPE)
	    {
	       ret = getLcCfgToAddMod(&macUeCfg->lcCfgList[lcIdx],&f1UeDb->duUeCfg.macLcCfg[dbIdx]);
	    }
	    else if(f1UeDb->duUeCfg.macLcCfg[dbIdx].configType == CONFIG_DEL_TYPE)
	    {
	       /* Free memory at matched  lcIdx index */
	       delLcCfg(&macUeCfg->lcCfgList[lcIdx]);
	       macUeCfg->numLcs--;
	       for(lcDelIdx = lcIdx; lcDelIdx < macUeCfg->numLcs; lcDelIdx++)
	       {
	          /* moving all elements one index ahead */
		  ret = getLcCfgToAddMod(&macUeCfg->lcCfgList[lcDelIdx], &macUeCfg->lcCfgList[lcDelIdx+1]);
	          delLcCfg(&macUeCfg->lcCfgList[lcDelIdx+1]);
		  if(ret)
		  {
                     DU_LOG("\nDU_APP: Failed at duUpdateMacCfg()");
		     break;
		  }
	       }
	    }
	 }
      } 
      if(f1UeDb->duUeCfg.macLcCfg[dbIdx].configType == CONFIG_ADD_TYPE)
      {
	 ret = getLcCfgToAddMod(&macUeCfg->lcCfgList[numLcs], &f1UeDb->duUeCfg.macLcCfg[dbIdx]);
	 if(ret)
	 {
	    DU_LOG("\nDU_APP: Failed at duUpdateMacCfg()");
	    break;
	 }
	 macUeCfg->numLcs++;
      }
		
   }
   return ret;
}

/*******************************************************************
 *
 * @brief To update DuUeCb Rlc Lc Cfg
 *
 * @details
 *
 *    Function : duUpdateRlcLcCfg
 *    Functionality:  update DuUeCb Rlc Lc Cfg
 *
 * @params[in] DuUeCb Pointer
 *             UeContextSetupDb pointer 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t duUpdateRlcLcCfg(RlcUeCfg *rlcUeCfg, UeContextSetupDb *f1UeDb)
{
   uint8_t lcIdx, dbIdx, numLcs, lcDelIdx;
   //bool lcIdFound = false;

   for(dbIdx = 0; dbIdx < f1UeDb->duUeCfg.numRlcLcs; dbIdx++)
   {
      numLcs = rlcUeCfg->numLcs;
      for(lcIdx = 0; lcIdx < numLcs; lcIdx++)
      {
	 if(f1UeDb->duUeCfg.rlcLcCfg[dbIdx].lcId == rlcUeCfg->rlcLcCfg[lcIdx].lcId)
	 {
	    if(f1UeDb->duUeCfg.rlcLcCfg[dbIdx].configType == CONFIG_MOD_TYPE)
	    {
	       memcpy(&rlcUeCfg->rlcLcCfg[lcIdx],\
		     &f1UeDb->duUeCfg.rlcLcCfg[dbIdx], sizeof(RlcBearerCfg));
	    }
	    else if(f1UeDb->duUeCfg.rlcLcCfg[dbIdx].configType == CONFIG_DEL_TYPE)
	    {
	       rlcUeCfg->numLcs--;
	       for(lcDelIdx = lcIdx; lcDelIdx < rlcUeCfg->numLcs; lcDelIdx++)
	       {
		  memcpy(&rlcUeCfg->rlcLcCfg[lcDelIdx],\
			&rlcUeCfg->rlcLcCfg[lcDelIdx+1], sizeof(RlcBearerCfg));
	          memset(&rlcUeCfg->rlcLcCfg[lcDelIdx+1], 0, sizeof(RlcBearerCfg));
	       }
	    }
	 }
      }
      if(f1UeDb->duUeCfg.rlcLcCfg[dbIdx].configType == CONFIG_ADD_TYPE)
      {
	 memcpy(&rlcUeCfg->rlcLcCfg[numLcs],\
	       &f1UeDb->duUeCfg.rlcLcCfg[dbIdx], sizeof(RlcBearerCfg));
	 rlcUeCfg->numLcs++;
      }
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief To update DuUeCb Mac and Rlc Ue Cfg
 *
 * @details
 *
 *    Function : duUpdateDuUeCbCfg
 *    Functionality:  update DuUeCb Mac and Rlc Ue Cfg
 *
 * @params[in] DuUeCb Pointer
 *             UeContextSetupDb pointer 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t duUpdateDuUeCbCfg(uint8_t cellIdx, uint8_t ueIdx, DuUeCb *ueCb, UeContextSetupDb *f1UeDb)
{
   uint8_t ret = ROK, cellId, crnti;

   cellId = cellIdx++;
   GET_CRNTI(crnti, ueIdx);

   /*Filling RLC Ue Cfg */
   ueCb->rlcUeCfg.cellId = cellId;
   ueCb->rlcUeCfg.ueIdx = ueIdx;
   ret = duUpdateRlcLcCfg(&ueCb->rlcUeCfg, f1UeDb);
   if(!ret)
      DU_LOG("\nDU APP: Failed while updating RLC LC Config at duUpdateDuUeCbCfg()");

   /*Filling MAC Ue Cfg */
   ueCb->macUeCfg.cellId = cellId;
   ueCb->macUeCfg.ueIdx = ueIdx;
   ueCb->macUeCfg.crnti = crnti;
   ret = duUpdateMacCfg(&ueCb->macUeCfg, f1UeDb);
   if(!ret)
      DU_LOG("\nDU APP: Failed while updating MAC LC Config at duUpdateDuUeCbCfg()");

   return ret;
}

/*******************************************************************
 *
 * @brief Process UeContextSetupRsp to F1AP
 *
 * @details
 *
 *    Function : duProcUeContextSetupRsp
 *
 *    Functionality: Process UeContextSetupRsp to F1AP
 *
 * @params[in] ueIdx, cellIdx 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t duProcUeContextSetupRsp(uint8_t ueIdx, uint8_t cellIdx)
{
   uint8_t ret = ROK;

   if((duCb.actvCellLst[cellIdx]->ueCb[ueIdx].\
         macUeCfgState == UE_RECFG_COMPLETED) &&
      (duCb.actvCellLst[cellIdx]->ueCb[ueIdx].\
         rlcUeCfgState == UE_RECFG_COMPLETED))
   {
      DU_LOG("\nDU_APP: Processing UE Context Setup Response\n");
      ret = duUpdateDuUeCbCfg(cellIdx, ueIdx, &duCb.actvCellLst[cellIdx]->ueCb[ueIdx],\
         &duCb.actvCellLst[cellIdx]->ueCb[ueIdx].f1UeDb);
      if(!ret)
         BuildAndSendUeContextSetupRsp(ueIdx, cellIdx);
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Handle UE config response from MAC
 *
 * @details
 *
 *    Function : DuProcMacUeCfgRsp
 *
 *    Functionality: Handle UE Config response from MAC
 *
 * @params[in] Pointer to MacUeCfgRsp and Pst 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t DuProcMacUeCfgRsp(Pst *pst, MacUeCfgRsp *cfgRsp)
{
   uint8_t ret = ROK;
   uint8_t ueIdx, cellIdx;

   if(cfgRsp)
   {
      if(cfgRsp->result == MAC_DU_APP_RSP_OK)
      {
         cellIdx = (cfgRsp->cellId -1);
         ueIdx = (cfgRsp->ueIdx -1);

         if(pst->event == EVENT_MAC_UE_CREATE_RSP)
	 {
            DU_LOG("\nDU APP : MAC UE Create Response : SUCCESS [UE IDX : %d]", cfgRsp->ueIdx);
            duCb.actvCellLst[cellIdx]->ueCb[ueIdx].macUeCfgState = UE_CREATE_COMPLETED;
	 }
         else if(pst->event == EVENT_MAC_UE_RECONFIG_RSP)
         {
            DU_LOG("\nDU APP : MAC UE Reconfig Response : SUCCESS [UE IDX : %d]", cfgRsp->ueIdx);
            duCb.actvCellLst[cellIdx]->ueCb[ueIdx].macUeCfgState = UE_RECFG_COMPLETED;
            duProcUeContextSetupRsp(ueIdx, cellIdx);
         }
      }
      else
      {
         DU_LOG("\nDU APP : MAC UE CFG Response for EVENT[%d]: FAILURE [UE IDX : %d]", pst->event, cfgRsp->ueIdx);
	 if(pst->event == EVENT_MAC_UE_RECONFIG_RSP)
	 {
	    //TODO: Send the failure case in Ue Context Setup Response
	 }
         ret = RFAILED;
      }
   }
   else
   {
      DU_LOG("\nDU APP : Received MAC Ue Config Response is NULL at DuProcMacUeCfgRsp()");
      ret = RFAILED;
   }
   return ret;
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

uint8_t duBuildAndSendUeCreateReqToRlc(uint16_t cellId, uint8_t ueIdx,\
   RlcUeCfg *duRlcUeCfg)
{
   uint8_t ret = ROK;
   RlcUeCfg *rlcUeCfg = NULLP;
   Pst pst;
  
   fillRlcUeCfg(cellId, ueIdx, NULL, duRlcUeCfg);
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

uint8_t DuProcRlcUlUeCfgRsp(Pst *pst, RlcUeCfgRsp *cfgRsp)
{
   uint8_t ret = ROK;
   uint8_t ueIdx, cellIdx;

   if(cfgRsp)
   {
      if(cfgRsp->result == RLC_DU_APP_RSP_OK)
      {
         cellIdx = (cfgRsp->cellId -1);
         ueIdx   = (cfgRsp->ueIdx -1);

         if(pst->event == EVENT_RLC_UL_UE_CREATE_RSP)
	 {
	    DU_LOG("\nDU_APP: RLC UE Create Response : SUCCESS [UE IDX:%d]", cfgRsp->ueIdx);
	    duCb.actvCellLst[cellIdx]->ueCb[ueIdx].rlcUeCfgState = UE_CREATE_COMPLETED;
	 }
	 else if(pst->event == EVENT_RLC_UL_UE_RECONFIG_RSP)
	 {
            DU_LOG("\nDU_APP: RLC UE Reconfig Response : SUCCESS [UE IDX:%d]", cfgRsp->ueIdx);
	    duCb.actvCellLst[cellIdx]->ueCb[ueIdx].rlcUeCfgState = UE_RECFG_COMPLETED;
	    duProcUeContextSetupRsp(ueIdx, cellIdx);
	 }
      }
      else
      {
         DU_LOG("\nDU_APP: RLC UE CFG Response for EVENT[%d] : FAILED [UE IDX : %d, REASON :%d]",\
	    pst->event, cfgRsp->ueIdx, cfgRsp->reason);
	 if((pst->event == EVENT_RLC_UL_UE_RECONFIG_RSP))
	 {
            //TODO: update failure case in ue Context setup Response
	 }
	 ret = RFAILED;
      }
      DU_FREE_SHRABL_BUF(DU_APP_MEM_REGION, DU_POOL, cfgRsp, sizeof(RlcUeCfgRsp));
   }
   else
   {
      DU_LOG("\nDU_APP: Received RLC Ue Config Response is NULL");
      ret = RFAILED;
   }
   return ret;
}




/*******************************************************************
 *
 * @brief Builds and Send Ue Reconfig Req to RLC
 *
 * @details
 *
 *    Function : duBuildAndSendUeReCfgReqToRLC
 *
 *    Functionality: Builds and Send Ue Reconfig Req to RLC
 *
 * @params[in] cellId, crnti
 *             DuUeCfg *ueCfgDb
 *             RlcUeCfg *rlcUeCfg
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t duBuildAndSendUeReCfgReqToRlc(uint8_t cellId, uint8_t crnti, DuUeCfg *ueCfgDb)
{
   uint8_t ret = ROK, ueIdx = 0;
   RlcUeCfg *rlcUeCfg = NULLP;

   GET_UE_IDX(crnti, ueIdx);
   DU_ALLOC_SHRABL_BUF(rlcUeCfg, sizeof(RlcUeCfg));
   if(rlcUeCfg)
   {
      memset(rlcUeCfg, 0, sizeof(RlcUeCfg));
      ret = fillRlcUeCfg(cellId, ueIdx, ueCfgDb, rlcUeCfg);
      if(ret)
         DU_LOG("\nDU APP: Failed at duBuildAndSendUeReCfgReqToRlc()");
      else
         ret = sendUeReCfgReqToRlc(rlcUeCfg);
   }
   else
   {
      DU_LOG("\nDU APP: Memory Alloc failed at duBuildAndSendUeReCfgReqToRlc()");
      ret = RFAILED;
   }
   return ret;
}

/*******************************************************************
 *
 * @brief Builds and Send Ue Reconfig Req to MAC
 *
 * @details
 *
 *    Function : duBuildAndSendUeReCfgReqToMac
 *
 *    Functionality: Builds and Send Ue Reconfig Req to MAC
 *
 * @params[in] CellGroupConfigRrc_t *macCellGrpCfg
 *             DuUeCfg *ueCfgDb
 *             MacUeCfg    *macUeCfg
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t duBuildAndSendUeReCfgReqToMac(uint8_t cellId, uint8_t crnti, UeContextSetupDb *ueCfgDb)
{
   uint8_t ret, ueIdx = 0;
   MacUeCfg *macUeCfg = NULLP;

   ret = ROK;
   GET_UE_IDX(crnti, ueIdx);

   DU_ALLOC_SHRABL_BUF(macUeCfg, sizeof(MacUeCfg));
   if(macUeCfg)
   {
      memset(macUeCfg, 0, sizeof(MacUeCfg));
      ret = fillMacUeCfg(cellId, ueIdx, crnti, ueCfgDb, macUeCfg);
      if(!ret)
      {
         ret = sendUeReCfgReqToMac(macUeCfg);
      }
   }
   else
   {
      DU_LOG("\nDU_APP: Failed at duBuildAndSendUeReCfgReqToMac()");
      ret = RFAILED;
   }
   return ret;
}

/*******************************************************************
 *
 * @brief Process UE context setup request from CU
 *
 * @details
 *
 *    Function : duProcUeSetupRequest
 *
 *    Functionality: Process UE context setup request from CU
 *
 * @params[in] F1AP message
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t duProcUeSetupRequest(UeContextSetupDb *ueSetupDb)
{
   uint8_t ret, ueIdx, cellId;
   DuUeCb     *ueCb = NULLP;
   
   ret = ROK;
   if(ueSetupDb)
   {
      cellId = duCb.actvCellLst[ueSetupDb->cellIdx]->cellId;
      for(ueIdx = 0; ueIdx < MAX_NUM_UE; ueIdx++)
      {
         if((duCb.actvCellLst[ueSetupDb->cellIdx]->ueCb[ueIdx].gnbCuUeF1apId == ueSetupDb->gnbCuUeF1apId) &&
          (duCb.actvCellLst[ueSetupDb->cellIdx]->ueCb[ueIdx].gnbDuUeF1apId == ueSetupDb->gnbDuUeF1apId &&
           duCb.actvCellLst[ueSetupDb->cellIdx]->ueCb[ueIdx].ueState == UE_ACTIVE))
         {
            ueCb = &duCb.actvCellLst[ueSetupDb->cellIdx]->ueCb[ueIdx];
            break;
         }
      }

      if(ueCb)
      {
         /* Sending DL RRC Message to RLC */
         if(ueSetupDb->rrcMsg)
         {
            ret = duBuildAndSendDlRrcMsgToRlc(cellId, ueCb->rlcUeCfg, SRB1_LCID, false, \
               ueSetupDb->deliveryStaReq,  ueSetupDb->rrcMsgLen, ueSetupDb->rrcMsg);
            if(ret)
            {
               DU_LOG("\nDU APP : No UE found for CuUeF1apId[%d] and DuUeF1apId[%d]", \
                  ueSetupDb->gnbCuUeF1apId, ueSetupDb->gnbDuUeF1apId);
               DU_FREE_SHRABL_BUF(DU_APP_MEM_REGION, DU_POOL, ueSetupDb->rrcMsg, ueSetupDb->rrcMsgLen);
            }
         }
	 if(!ret)
	 {
            DU_LOG("\nDU_APP: Processing Ue Context Setup Request for cellId [%d]", cellId);
            /* Filling RLC Ue Reconfig */ 
            ret = duBuildAndSendUeReCfgReqToRlc(cellId, ueCb->crnti, &ueSetupDb->duUeCfg);
	    if(ret)
            {
               DU_LOG("\nDU_APP: Failed at duBuildAndSendUeReCfgReqToRlc for cellId [%d]", cellId);
            }
            /* Filling MAC Ue Reconfig */
            ret = duBuildAndSendUeReCfgReqToMac(cellId, ueCb->crnti, ueSetupDb);
            if(ret)
            {
               DU_LOG("\nDU_APP: Failed at duBuildAndSendUeReCfgReqToMac for cellId [%d]", cellId);
            }
	 }
      }
      else
      {
         DU_LOG("\nDU APP : No UE found for CuUeF1apId[%d] and DuUeF1apId[%d]", \
          ueSetupDb->gnbCuUeF1apId, ueSetupDb->gnbDuUeF1apId);
         DU_FREE_SHRABL_BUF(DU_APP_MEM_REGION, DU_POOL, ueSetupDb->rrcMsg, ueSetupDb->rrcMsgLen);
         ret = RFAILED;
	 //TODO: To send the failure cause in UeContextSetupRsp
      }
   }
   //TODO: To send the failure cause in UeContextSetupRsp
   return ret;
}

/**********************************************************************
  End of file
 ***********************************************************************/
