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
#include "OCTET_STRING.h"
#include "BIT_STRING.h"
#include "odu_common_codec.h"
/*Header files required for UE context Setup */
#include "CellGroupConfigRrc.h"
#include "ProtocolExtensionField.h"
#include "MAC-CellGroupConfig.h"
#include "SchedulingRequestConfig.h"
#include "SchedulingRequestToAddMod.h"
#include "TAG.h"
#include "TAG-Config.h"
#include "PhysicalCellGroupConfig.h"
#include "SpCellConfig.h"
#include "ServingCellConfig.h"
#include "BWP-DownlinkDedicated.h"
#include "PDCCH-Config.h"
#include "PDSCH-Config.h"
#include "ControlResourceSet.h"
#include "SearchSpace.h"
#include "DMRS-DownlinkConfig.h"
#include "PDSCH-TimeDomainResourceAllocation.h"
#include "PDSCH-TimeDomainResourceAllocationList.h"
#include "PDSCH-CodeBlockGroupTransmission.h"
#include "PDSCH-ServingCellConfig.h"
#include "PUSCH-Config.h"
#include "BWP-UplinkDedicated.h"
#include "PUSCH-ServingCellConfig.h"
#include "DMRS-UplinkConfig.h"
#include "UplinkConfig.h"
#include "PUSCH-TimeDomainResourceAllocation.h"
#include "PUSCH-TimeDomainResourceAllocationList.h"
#include "DRB-Information.h"
#include "GBR-QoSFlowInformation.h"
#include "QoSFlowLevelQoSParameters.h"

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


DuRlcUlUeReconfigReq packRlcUlUeReconfigReqOpts[] =
{
   packDuRlcUlUeCreateReq,       /* Loose coupling */
   RlcUlProcUeReconfigReq,       /* TIght coupling */
   packDuRlcUlUeCreateReq,       /* Light weight-loose coupling */
};

DuMacUeReconfigReq packMacUeReconfigReqOpts[] =
{
   packDuMacUeCreateReq,       /* Loose coupling */
   MacProcUeReconfigReq,       /* TIght coupling */
   packDuMacUeCreateReq,       /* Light weight-loose coupling */
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

   Pst pst;
   KwuDatReqInfo datReqInfo;

#ifdef EGTP_TEST
   datReqInfo.rlcId.rbId = RB_ID;
   datReqInfo.rlcId.rbType = CM_LTE_DRB;
   datReqInfo.rlcId.ueId = UE_ID;
   datReqInfo.rlcId.cellId = NR_CELL_ID;

   datReqInfo.sduId = ++sduId;
   datReqInfo.lcType = CM_LTE_LCH_DTCH;
#endif
   /* Filling pst and Sending to RLC DL */
   pst.selector  = ODU_SELECTOR_LWLC;
   pst.srcEnt    = ENTDUAPP;
   pst.dstEnt    = ENTRLC;
   pst.dstInst   = RLC_DL_INST;
   pst.dstProcId = DU_PROC;
   pst.srcProcId = DU_PROC;
   pst.region    = duCb.init.region;

   cmPkKwuDatReq(&pst, &datReqInfo, egtpMsg->msg);
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
   ODU_PUT_MSG(mBuf);

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
 *    Function : fillLcCfgToAddList
 *
 *    Functionality: Fills Logical channel Config List
 *
 *
 *****************************************************************/
void fillLcCfgToAddList(LcCfg *lcCfgInfo)
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
   ueCfg->numLcsToAdd = SRB_ID_1;
   ueCfg->numLcsToMod = 0;
   ueCfg->numLcsToDel = 0;
   if(ueCfg->numLcsToAdd < MAX_NUM_LOGICAL_CHANNELS)
   {
      for(idx = 0; idx < ueCfg->numLcsToAdd; idx++)
      {   
	 fillLcCfgToAddList(&ueCfg->lcCfgToAddList[idx]);
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
   ueCfg->numLcsToAdd  = SRB_ID_1; 
   ueCfg->numLcsToMod  = 0;
   memset(&ueCfg->rlcBearerCfgToMod, 0, sizeof(RlcBearerCfg));
   ueCfg->numLcsToDel  = 0; 
   memset(&ueCfg->rlcBearerCfgToDel, 0, sizeof(RlcBearerCfg));
   
   for(idx = 0; idx < ueCfg->numLcsToAdd; idx++)
   {
      ueCfg->rlcBearerCfgToAdd[idx].rbId         = SRB_ID_1;
      ueCfg->rlcBearerCfgToAdd[idx].rbType       = RB_TYPE_SRB;
      ueCfg->rlcBearerCfgToAdd[idx].lcId         = SRB_ID_1;
      ueCfg->rlcBearerCfgToAdd[idx].lcType       = LCH_DCCH;
      ueCfg->rlcBearerCfgToAdd[idx].rlcMode      = RLC_AM;
      switch(ueCfg->rlcBearerCfgToAdd[idx].rlcMode)
      {
         case RLC_AM:
	    memset(&ueCfg->rlcBearerCfgToAdd[idx].u.amCfg, 0, sizeof(AmBearerCfg));
            fillAmInfo(&ueCfg->rlcBearerCfgToAdd[idx].u.amCfg);
            break;
         case RLC_UM_BI_DIRECTIONAL:
	    memset(&ueCfg->rlcBearerCfgToAdd[idx].u.umBiDirCfg, 0, sizeof(UmBiDirBearerCfg));
            fillUmBiDirInfo(&ueCfg->rlcBearerCfgToAdd[idx].u.umBiDirCfg);
            break;
         case RLC_UM_UNI_DIRECTIONAL_UL:
	    memset(&ueCfg->rlcBearerCfgToAdd[idx].u.umUniDirUlCfg, 0, sizeof(UmUniDirUlBearerCfg));
            fillUmUniDirUlInfo(&ueCfg->rlcBearerCfgToAdd[idx].u.umUniDirUlCfg);
            break;
         case RLC_UM_UNI_DIRECTIONAL_DL:
	    memset(&ueCfg->rlcBearerCfgToAdd[idx].u.umUniDirDlCfg, 0, sizeof(UmUniDirDlBearerCfg));
            fillUmUniDirDlInfo(&ueCfg->rlcBearerCfgToAdd[idx].u.umUniDirDlCfg);
            break;
         default :
            DU_LOG("\nDU_APP: Rlc Mode invalid %d", ueCfg->rlcBearerCfgToAdd[idx].rlcMode);
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
 * @Params[in]  cellId,
 *              ueIdx
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t sendUeReCfgReqToMac(MacUeCfg *macUeCfg)
{
   uint8_t ret = ROK;
   Pst pst;
   memset(&pst, 0, sizeof(Pst));
   
   /* Fill Pst */
   FILL_PST_DUAPP_TO_MAC(pst, EVENT_MAC_UE_RECONFIG_REQ);

   if(macUeCfg)
   {
      /* Processing one Ue at a time to MAC */
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
   uint8_t ret = ROK;
   Pst pst;
   memset(&pst, 0, sizeof(Pst));
  
   FILL_PST_DUAPP_TO_RLC(pst, RLC_UL_INST, EVENT_RLC_UL_UE_RECONFIG_REQ);

   if(rlcUeCfg)
   {
      /* Processing one Ue at a time to RLC */
      DU_LOG("\nDU_APP: Sending UE Reconfig Request to RLC UL");
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

/*******************************************************************
 *
 * @brief Fills Reconfig SchReqReConfig
 *
 * @details
 *
 *    Function : fillSchReqReConfig
 *
 *    Functionality: Fills Reconfig SchReqReConfig
 *
 * @params[in] SchedulingRequestConfig_t *cuSchedReq
 *             SchedReqCfg*  duSchedReq
 * @return void
 *
 * ****************************************************************/
void fillSchReqReConfig(SchedulingRequestConfig_t *cuSchedReq, SchedReqCfg *duSchedReq)
{
   uint8_t schReqIdx = 0;
   struct SchedulingRequestConfig__schedulingRequestToAddModList *schReqListToAdd = NULLP;
   struct SchedulingRequestConfig__schedulingRequestToReleaseList *schReqListToRel = NULLP;

   if(cuSchedReq->schedulingRequestToAddModList)
   {
      schReqListToAdd = cuSchedReq->schedulingRequestToAddModList;
      if(schReqListToAdd->list.count)
      {
	 duSchedReq->addModListCount = schReqListToAdd->list.count;
	 for(schReqIdx = 0; schReqIdx < schReqListToAdd->list.count; schReqIdx++)
	 {
	    duSchedReq->addModList[schReqIdx].schedReqId = \
	       schReqListToAdd->list.array[schReqIdx]->schedulingRequestId;
	    duSchedReq->addModList[schReqIdx].srProhibitTmr = \
	       *(schReqListToAdd->list.array[schReqIdx]->sr_ProhibitTimer);
	    duSchedReq->addModList[schReqIdx].srTransMax    =\
	       schReqListToAdd->list.array[schReqIdx]->sr_TransMax;
	 }
      }
   }
   /* Scheduling Req To release */
   if(cuSchedReq->schedulingRequestToReleaseList)
   {
      schReqListToRel = cuSchedReq->schedulingRequestToReleaseList;
      if(schReqListToRel->list.count)
      {
	 duSchedReq->relListCount = schReqListToRel->list.count;
	 for(schReqIdx = 0; schReqIdx < schReqListToRel->list.count; schReqIdx++)
	 {
	    duSchedReq->relList[schReqIdx] = \
	       *schReqListToRel->list.array[schReqIdx];
	 }
      }
   }
}

/*******************************************************************
 *
 * @brief Fills TagReconfig
 *
 * @details
 *
 *    Function : fillTagReconfig
 *
 *    Functionality: Fills fillTagReconfig
 *
 * @params[in] TAG_Config_t *cuTagCfg
 *             TagCfg *duTagCfg
 * @return void
 *
 * ****************************************************************/

void fillTagReconfig(TAG_Config_t *cuTagCfg, TagCfg *duTagCfg)
{
  uint8_t tagIdx = 0;
  struct TAG_Config__tag_ToAddModList  *tagListToAddMod = NULLP;
  struct TAG_Config__tag_ToReleaseList *tagListToRel = NULLP;

  /* Tag config to AddMod */
  if(cuTagCfg->tag_ToAddModList)
  {
     tagListToAddMod = cuTagCfg->tag_ToAddModList; 
     if(tagListToAddMod->list.count)
     {
        duTagCfg->addModListCount = tagListToAddMod->list.count;
        for(tagIdx = 0; tagIdx < tagListToAddMod->list.count; tagIdx++)
	{
           duTagCfg->addModList[tagIdx].tagId =\
              tagListToAddMod->list.array[tagIdx]->tag_Id;     
           duTagCfg->addModList[tagIdx].timeAlignTimer = \

              tagListToAddMod->list.array[tagIdx]->timeAlignmentTimer;
	}
     }
  }
  /* Tag config to release */
  if(cuTagCfg->tag_ToReleaseList)
  {
     tagListToRel = cuTagCfg->tag_ToReleaseList;
     if(tagListToRel->list.count)
     {
        duTagCfg->relListCount = tagListToRel->list.count;
        for(tagIdx = 0; tagIdx < tagListToRel->list.count; tagIdx++)
	{
           duTagCfg->relList[tagIdx] = *tagListToRel->list.array[tagIdx];     
	}
     }
  }
}

/*******************************************************************
 *
 * @brief Fills PdcchCfg received by CU
 *
 * @details
 *
 *    Function : fillPdcchCfg
 *
 *    Functionality: Fills PdcchCfg received  by CU
 *
 * @params[in] PDCCH_Config_t *cuPdcchCfg,
 *             PdcchConfig *duPdcchCfg
 * @return void
 *
 * ****************************************************************/

void fillPdcchCfg(PDCCH_Config_t *cuPdcchCfg, PdcchConfig *duPdcchCfg)
{
   uint8_t cRsetIdx = 0;
   uint8_t srchSpcIdx = 0;

   struct PDCCH_Config__controlResourceSetToAddModList *cRsetToAddModList = NULLP;
   struct PDCCH_Config__controlResourceSetToReleaseList *cRsetToRelList = NULLP;
   struct PDCCH_Config__searchSpacesToAddModList *srchSpcToAddModList = NULLP;
   struct PDCCH_Config__searchSpacesToReleaseList *srchSpcToRelList = NULLP;


   /* Control Resource Set To Add/Mod List */
   if(cuPdcchCfg->controlResourceSetToAddModList)
   {
      cRsetToAddModList = cuPdcchCfg->controlResourceSetToAddModList;
      if(cRsetToAddModList->list.count)
      {
         duPdcchCfg->numCRsetToAddMod = cRsetToAddModList->list.count;
	 for(cRsetIdx = 0; cRsetIdx < cRsetToAddModList->list.count; cRsetIdx++)
	 {
	    duPdcchCfg->cRSetToAddModList[cRsetIdx].cRSetId = \
	      cRsetToAddModList->list.array[cRsetIdx]->controlResourceSetId;
	    bitStringToInt(&cRsetToAddModList->list.array[cRsetIdx]->frequencyDomainResources,\
	       duPdcchCfg->cRSetToAddModList[cRsetIdx].freqDomainRsrc, UINT8);
            duPdcchCfg->cRSetToAddModList[cRsetIdx].duration = \
	      cRsetToAddModList->list.array[cRsetIdx]->duration;

	    duPdcchCfg->cRSetToAddModList[cRsetIdx].cceRegMappingType = \
	      cRsetToAddModList->list.array[cRsetIdx]->cce_REG_MappingType.present;   
            if(duPdcchCfg->cRSetToAddModList[cRsetIdx].cceRegMappingType == CCE_REG_MAPPINGTYPE_PR_INTERLEAVED)
	    {
	       //TODO: handle the case for Interleaved
            }
            duPdcchCfg->cRSetToAddModList[cRsetIdx].precoderGranularity = \
	      cRsetToAddModList->list.array[cRsetIdx]->precoderGranularity;
	    if(cRsetToAddModList->list.array[cRsetIdx]->pdcch_DMRS_ScramblingID)
	       duPdcchCfg->cRSetToAddModList[cRsetIdx].dmrsScramblingId= \
	     *(cRsetToAddModList->list.array[cRsetIdx]->pdcch_DMRS_ScramblingID); 
         }
      }

   }
   /* Control Resource Set To Release List */
   if(cuPdcchCfg->controlResourceSetToReleaseList)
   {
      cRsetToRelList = cuPdcchCfg->controlResourceSetToReleaseList;
      if(cRsetToRelList->list.count)
      {
         duPdcchCfg->numCRsetToRel = cRsetToRelList->list.count;
	 for(cRsetIdx = 0; cRsetIdx < cRsetToRelList->list.count; cRsetIdx++)
	 {
            duPdcchCfg->cRSetToRelList[cRsetIdx] = *(cRsetToRelList->list.array[cRsetIdx]);
	 }
      }
   }

   /* Search space To Add/Mod List */
   if(cuPdcchCfg->searchSpacesToAddModList)
   {
      srchSpcToAddModList = cuPdcchCfg->searchSpacesToAddModList;
      if(srchSpcToAddModList->list.count)
      {
         duPdcchCfg->numSearchSpcToAddMod = srchSpcToAddModList->list.count;
	 for(srchSpcIdx = 0; srchSpcIdx < srchSpcToAddModList->list.count; srchSpcIdx++)
	 {
            duPdcchCfg->searchSpcToAddModList[srchSpcIdx].searchSpaceId =\
	       srchSpcToAddModList->list.array[srchSpcIdx]->searchSpaceId;
            duPdcchCfg->searchSpcToAddModList[srchSpcIdx].cRSetId =\
	       *(srchSpcToAddModList->list.array[srchSpcIdx]->controlResourceSetId);
	    if(srchSpcToAddModList->list.array[srchSpcIdx]->monitoringSlotPeriodicityAndOffset)
	    {
               duPdcchCfg->searchSpcToAddModList[srchSpcIdx].mSlotPeriodicityAndOffset =\
	          srchSpcToAddModList->list.array[srchSpcIdx]->monitoringSlotPeriodicityAndOffset->present;
            }
            if(srchSpcToAddModList->list.array[srchSpcIdx]->monitoringSymbolsWithinSlot)
            {
	       bitStringToInt(srchSpcToAddModList->list.array[srchSpcIdx]->monitoringSymbolsWithinSlot,\
	          duPdcchCfg->searchSpcToAddModList[srchSpcIdx].mSymbolsWithinSlot, UINT8);
            }
	    if(srchSpcToAddModList->list.array[srchSpcIdx]->nrofCandidates)
            {
	      duPdcchCfg->searchSpcToAddModList[srchSpcIdx].numCandidatesAggLevel1 = \
	          srchSpcToAddModList->list.array[srchSpcIdx]->nrofCandidates->aggregationLevel1;
              duPdcchCfg->searchSpcToAddModList[srchSpcIdx].numCandidatesAggLevel2 = \
	          srchSpcToAddModList->list.array[srchSpcIdx]->nrofCandidates->aggregationLevel2;
              duPdcchCfg->searchSpcToAddModList[srchSpcIdx].numCandidatesAggLevel4 = \
              	  srchSpcToAddModList->list.array[srchSpcIdx]->nrofCandidates->aggregationLevel4;
              
              duPdcchCfg->searchSpcToAddModList[srchSpcIdx].numCandidatesAggLevel8 = \
              	  srchSpcToAddModList->list.array[srchSpcIdx]->nrofCandidates->aggregationLevel8;
              
              duPdcchCfg->searchSpcToAddModList[srchSpcIdx].numCandidatesAggLevel16 = \
	          srchSpcToAddModList->list.array[srchSpcIdx]->nrofCandidates->aggregationLevel16;
	    }
            if(srchSpcToAddModList->list.array[srchSpcIdx]->searchSpaceType)
	    {
	       duPdcchCfg->searchSpcToAddModList[srchSpcIdx].searchSpaceType =\
	          srchSpcToAddModList->list.array[srchSpcIdx]->searchSpaceType->present;
	       if(duPdcchCfg->searchSpcToAddModList[srchSpcIdx].searchSpaceType == SEARCHSPACETYPE_PR_UE_SPECIFIC)
	       {
		  duPdcchCfg->searchSpcToAddModList[srchSpcIdx].ueSpecificDciFormat =\
		     srchSpcToAddModList->list.array[srchSpcIdx]->searchSpaceType->choice.ue_Specific->dci_Formats;
	       }
         
	    }
	 }
      }
   }
   /* Search space To Rel List */
   if(cuPdcchCfg->searchSpacesToReleaseList)
   {
      srchSpcToRelList = cuPdcchCfg->searchSpacesToReleaseList;
      if(srchSpcToRelList->list.count)
      {
         duPdcchCfg->numSearchSpcToRel = srchSpcToRelList->list.count;
	 for(srchSpcIdx = 0; srchSpcIdx < srchSpcToRelList->list.count; srchSpcIdx++)
	 {
            duPdcchCfg->searchSpcToRelList[srchSpcIdx] =\
	       *(srchSpcToRelList->list.array[srchSpcIdx]);
	 }
      }
   }
}

/*******************************************************************
 *
 * @brief Fills PdschCfg received by CU
 *
 * @details
 *
 *    Function : fillPdschCfg
 *
 *    Functionality: Fills PdschCfg received  by CU
 *
 * @params[in] PDSCH_Config_t *cuPdschCfg,
 *             PdschConfig *duPdschCfg
 * @return void
 *
 * ****************************************************************/

void fillPdschCfg(PDSCH_Config_t *cuPdschCfg, PdschConfig *duPdschCfg)
{
   uint8_t timeDomIdx;
   struct PDSCH_Config__pdsch_TimeDomainAllocationList *timeDomAlloc = NULLP;

   if(cuPdschCfg->dmrs_DownlinkForPDSCH_MappingTypeA)
   {
      if(cuPdschCfg->dmrs_DownlinkForPDSCH_MappingTypeA->present == \
         PDSCH_Config__dmrs_DownlinkForPDSCH_MappingTypeA_PR_setup)
      {
         if(cuPdschCfg->dmrs_DownlinkForPDSCH_MappingTypeA->choice.setup)
	 {
            duPdschCfg->dmrsDlCfgForPdschMapTypeA.addPos = \
	       *(cuPdschCfg->dmrs_DownlinkForPDSCH_MappingTypeA->choice.setup->dmrs_AdditionalPosition);
	 }
      }
   }
   duPdschCfg->resourceAllocType = cuPdschCfg->resourceAllocation;
   if(cuPdschCfg->pdsch_TimeDomainAllocationList)
   {
      timeDomAlloc = cuPdschCfg->pdsch_TimeDomainAllocationList;
      if(timeDomAlloc->present ==\
         PDSCH_Config__pdsch_TimeDomainAllocationList_PR_setup)
      {
         if(timeDomAlloc->choice.setup)
	 {
	    duPdschCfg->numTimeDomRsrcAlloc  = timeDomAlloc->choice.setup->list.count;
            for(timeDomIdx = 0; timeDomIdx < timeDomAlloc->choice.setup->list.count; timeDomIdx++)
            {
	       duPdschCfg->timeDomRsrcAllociList[timeDomIdx].mappingType = \
	          timeDomAlloc->choice.setup->list.array[timeDomIdx]->mappingType;
	       //duPdschCfg->timeDomRsrcAllociList[timeDomIdx].startSymbol = \
	          
	       //duPdschCfg->timeDomRsrcAllociList[timeDomIdx].symbolLength;
	       duPdschCfg->timeDomRsrcAllociList[timeDomIdx].startSymbolAndLength = \
	          timeDomAlloc->choice.setup->list.array[timeDomIdx]->startSymbolAndLength;
	    }
	 }
      }
   }
   duPdschCfg->rbgSize = cuPdschCfg->rbg_Size; 
   if(cuPdschCfg->maxNrofCodeWordsScheduledByDCI)
      duPdschCfg->numCodeWordsSchByDci = *(cuPdschCfg->maxNrofCodeWordsScheduledByDCI);
   if(cuPdschCfg->prb_BundlingType.present == PDSCH_Config__prb_BundlingType_PR_staticBundling)
   {
      duPdschCfg->bundlingType = cuPdschCfg->prb_BundlingType.present;
      if(cuPdschCfg->prb_BundlingType.choice.staticBundling)
      {
         if(cuPdschCfg->prb_BundlingType.choice.staticBundling->bundleSize)
	 {
            duPdschCfg->bundlingInfo.StaticBundling.size = \
	       *(cuPdschCfg->prb_BundlingType.choice.staticBundling->bundleSize);
	 }
      }
   }
   else if(cuPdschCfg->prb_BundlingType.present == PDSCH_Config__prb_BundlingType_PR_dynamicBundling)
   {
      duPdschCfg->bundlingType = cuPdschCfg->prb_BundlingType.present;
   }

}

/*******************************************************************
 *
 * @brief Fills PdschServingCellCfg received by CU
 *
 * @details
 *
 *    Function : fillPdschServingCellCfg
 *
 *    Functionality: Fills PdschCfg received  by CU
 *
 * @params[in] PDSCH_ServingCellConfig_t *cuPdschSrvCellCfg,
 *             PdschServCellCfg *duUePdschSrvCellCfg
 * @return void
 *
 * ****************************************************************/

void fillPdschServingCellCfg(PDSCH_ServingCellConfig_t *cuPdschSrvCellCfg, PdschServCellCfg *duUePdschSrvCellCfg)
{
   if(cuPdschSrvCellCfg->codeBlockGroupTransmission)
   {
      if(cuPdschSrvCellCfg->codeBlockGroupTransmission->choice.setup)
      {
         if(duUePdschSrvCellCfg->maxCodeBlkGrpPerTb)
         {
            *(duUePdschSrvCellCfg->maxCodeBlkGrpPerTb)  = \
	       cuPdschSrvCellCfg->codeBlockGroupTransmission->choice.setup->maxCodeBlockGroupsPerTransportBlock;
	 }
	 else
	 {
            DU_ALLOC_SHRABL_BUF(duUePdschSrvCellCfg->maxCodeBlkGrpPerTb, sizeof(uint8_t));
            if(duUePdschSrvCellCfg->maxCodeBlkGrpPerTb)
	    {
               *(duUePdschSrvCellCfg->maxCodeBlkGrpPerTb)  = \
	          cuPdschSrvCellCfg->codeBlockGroupTransmission->choice.setup->maxCodeBlockGroupsPerTransportBlock;
	    }
	    else
	    {
	       DU_LOG("\nDUAPP: Memory allocation failed for maxCodeBlkGrpPerTb at fillPdschServingCellCfg()");
	    }
	 }
         if(duUePdschSrvCellCfg->codeBlkGrpFlushInd)
         {
            *(duUePdschSrvCellCfg->codeBlkGrpFlushInd)  = \
	       cuPdschSrvCellCfg->codeBlockGroupTransmission->choice.setup->codeBlockGroupFlushIndicator;
	 }
	 else
	 {
            DU_ALLOC_SHRABL_BUF(duUePdschSrvCellCfg->maxCodeBlkGrpPerTb, sizeof(bool));
            if(duUePdschSrvCellCfg->codeBlkGrpFlushInd)
	    {
               *(duUePdschSrvCellCfg->codeBlkGrpFlushInd)  = \
	          cuPdschSrvCellCfg->codeBlockGroupTransmission->choice.setup->codeBlockGroupFlushIndicator;
	    }
	    else
	    {
	       DU_LOG("\nDUAPP: Memory allocation failed for codeBlkGrpFlushInd at fillPdschServingCellCfg()");
	    }
	 }
      }
   }
   if(cuPdschSrvCellCfg->nrofHARQ_ProcessesForPDSCH)
   {
      duUePdschSrvCellCfg->numHarqProcForPdsch = *(cuPdschSrvCellCfg->nrofHARQ_ProcessesForPDSCH); 
   }
   if(cuPdschSrvCellCfg->ext1)
   {
      if(cuPdschSrvCellCfg->ext1->maxMIMO_Layers)
      {
        if(duUePdschSrvCellCfg->maxMimoLayers)
	{
           *(duUePdschSrvCellCfg->maxMimoLayers)  = *(cuPdschSrvCellCfg->ext1->maxMIMO_Layers);
	}
	else
	{
           DU_ALLOC_SHRABL_BUF(duUePdschSrvCellCfg->maxMimoLayers, sizeof(uint8_t));
           if(duUePdschSrvCellCfg->maxMimoLayers)
	   {
              *(duUePdschSrvCellCfg->maxMimoLayers)  = *(cuPdschSrvCellCfg->ext1->maxMIMO_Layers);
	   }
	   else
	   {
	      DU_LOG("\nDUAPP: Memory allocation failed for maxMimoLayers at fillPdschServingCellCfg()");
	   }
	}
      }
   }
   if(cuPdschSrvCellCfg->xOverhead)
   {
      if(duUePdschSrvCellCfg->xOverhead)
      {
         *(duUePdschSrvCellCfg->xOverhead)  = *(cuPdschSrvCellCfg->xOverhead);
      }
      else
      {
         DU_ALLOC_SHRABL_BUF(duUePdschSrvCellCfg->xOverhead, sizeof(uint8_t));
         if(duUePdschSrvCellCfg->xOverhead)
         {
            *(duUePdschSrvCellCfg->xOverhead)  = *(cuPdschSrvCellCfg->xOverhead);
         }
         else
         {
            DU_LOG("\nDUAPP: Memory allocation failed for xOverhead at fillPdschServingCellCfg()");
         }
      }
   }
}

/*******************************************************************
 *
 * @brief Fills PuschCfg received by CU
 *
 * @details
 *
 *    Function : fillPuschCfg
 *
 *    Functionality: Fills PuschCfg received  by CU
 *
 * @params[in] BWP_UplinkDedicated__pusch_Config *cuPuschCfg,
 *             PuschCfg *duPuschCfg
 * @return void
 *
 * ****************************************************************/

void fillPuschCfg(struct BWP_UplinkDedicated__pusch_Config *cuPuschCfg, PuschCfg *duPuschCfg)
{
   uint8_t timeDomIdx = 0;
   DMRS_UplinkConfig_t *dmrsUlCfg = NULLP;
   struct PUSCH_Config__pusch_TimeDomainAllocationList *timeDomAllocList = NULLP;

   if(cuPuschCfg->present == BWP_UplinkDedicated__pusch_Config_PR_setup)
   {
      if(cuPuschCfg->choice.setup)
      {
         if(cuPuschCfg->choice.setup->dataScramblingIdentityPUSCH)
	 {
             duPuschCfg->dataScramblingId = \
	        *(cuPuschCfg->choice.setup->dataScramblingIdentityPUSCH);
	 }
	 if(cuPuschCfg->choice.setup->dmrs_UplinkForPUSCH_MappingTypeA)
	 {
	    if(cuPuschCfg->choice.setup->dmrs_UplinkForPUSCH_MappingTypeA->present == PUSCH_Config__dmrs_UplinkForPUSCH_MappingTypeA_PR_setup)
	    {
	       if(cuPuschCfg->choice.setup->dmrs_UplinkForPUSCH_MappingTypeA->choice.setup)
	       {
	          dmrsUlCfg = (cuPuschCfg->choice.setup->dmrs_UplinkForPUSCH_MappingTypeA->choice.setup);
	          if(dmrsUlCfg->dmrs_AdditionalPosition)
	          {
                     duPuschCfg->dmrsUlCfgForPuschMapTypeA.addPos =\
	                *(dmrsUlCfg->dmrs_AdditionalPosition);
	          }
                  if(dmrsUlCfg->transformPrecodingDisabled)
	          {
	             if(dmrsUlCfg->transformPrecodingDisabled->scramblingID0)
	             {
	                duPuschCfg->dmrsUlCfgForPuschMapTypeA.transPrecodDisabled.scramblingId0 = \
	                   *(dmrsUlCfg->transformPrecodingDisabled->scramblingID0);
                     }
	          }
	       }
	    }
	 }
	 /*Res Alloc Type for UL */
	 if(cuPuschCfg->choice.setup->resourceAllocation)
	 {
            duPuschCfg->resourceAllocType = \
	       cuPuschCfg->choice.setup->resourceAllocation;
	 }
	 if(cuPuschCfg->choice.setup->pusch_TimeDomainAllocationList)
	 {
	    timeDomAllocList = cuPuschCfg->choice.setup->pusch_TimeDomainAllocationList;
	    if(timeDomAllocList->present == PUSCH_Config__pusch_TimeDomainAllocationList_PR_setup)
	    {
               if(timeDomAllocList->choice.setup)
	       {
	          duPuschCfg->numTimeDomRsrcAlloc = timeDomAllocList->choice.setup->list.count;
	          for(timeDomIdx = 0; timeDomIdx <timeDomAllocList->choice.setup->list.count; timeDomIdx++)
		  {
		     duPuschCfg->timeDomRsrcAllocList[timeDomIdx].k2 = \
		        *(timeDomAllocList->choice.setup->list.array[timeDomIdx]->k2);
                     duPuschCfg->timeDomRsrcAllocList[timeDomIdx].mappingType = \
                     		        timeDomAllocList->choice.setup->list.array[timeDomIdx]->mappingType;
                     duPuschCfg->timeDomRsrcAllocList[timeDomIdx].startSymbolAndLength = \
		        timeDomAllocList->choice.setup->list.array[timeDomIdx]->startSymbolAndLength;
                    //duPuschCfg->timeDomRsrcAllocList[timeDomIdx].startSymbol = \
		        timeDomAllocList->choice.setup->list.array[timeDomIdx]->startSymbol;
                    //duPuschCfg->timeDomRsrcAllocList[timeDomIdx].symbolLen = \
		        timeDomAllocList->choice.setup->list.array[timeDomIdx]->symbolLen;
                  }
	       }
	    }
	 }
	 if(cuPuschCfg->choice.setup->transformPrecoder)
            duPuschCfg->transformPrecoder = *(cuPuschCfg->choice.setup->transformPrecoder);
      }
   }
}

/*******************************************************************
 *
 * @brief Fills ServingCellReconfig received by CU
 *
 * @details
 *
 *    Function : fillServingCellReconfig
 *
 *    Functionality: Fills ServingCellReconfig received  by CU
 *
 * @params[in] ServingCellConfig_t *cuSrvCellCfg
 *             ServCellCfgInfo *duSrvCellCfg
 * @return void
 *
 * ****************************************************************/
void fillServingCellReconfig(ServingCellConfig_t *cuSrvCellCfg, ServCellCfgInfo *duSrvCellCfg)
{
   BWP_DownlinkDedicated_t *dlBwp = NULLP;
   BWP_UplinkDedicated_t   *ulBwp = NULLP;

   if(cuSrvCellCfg->initialDownlinkBWP)
   {
      dlBwp = ((BWP_DownlinkDedicated_t *)(cuSrvCellCfg->initialDownlinkBWP));
      if(dlBwp->pdcch_Config)
      {
         if(dlBwp->pdcch_Config->choice.setup)
	 {
	    fillPdcchCfg(dlBwp->pdcch_Config->choice.setup, &duSrvCellCfg->initDlBwp.pdcchCfg);
	 }
      }
      if(dlBwp->pdsch_Config)
      {
         if(dlBwp->pdsch_Config->choice.setup)
	 {
	    fillPdschCfg(dlBwp->pdsch_Config->choice.setup, &duSrvCellCfg->initDlBwp.pdschCfg);
	 }
      }
   }
   if(cuSrvCellCfg->firstActiveDownlinkBWP_Id)
      duSrvCellCfg->firstActvDlBwpId = *(cuSrvCellCfg->firstActiveDownlinkBWP_Id);
   if(cuSrvCellCfg->defaultDownlinkBWP_Id)
      duSrvCellCfg->defaultDlBwpId = *(cuSrvCellCfg->defaultDownlinkBWP_Id);
   if(cuSrvCellCfg->bwp_InactivityTimer)
   {
      if(duSrvCellCfg->bwpInactivityTmr)
      {
         memcpy(duSrvCellCfg->bwpInactivityTmr, cuSrvCellCfg->bwp_InactivityTimer, sizeof(uint8_t));
      }
      else
      {
         duSrvCellCfg->bwpInactivityTmr = NULLP;
         DU_ALLOC_SHRABL_BUF(duSrvCellCfg->bwpInactivityTmr, sizeof(uint8_t));
	 if(duSrvCellCfg->bwpInactivityTmr)
	 {
            memcpy(duSrvCellCfg->bwpInactivityTmr, cuSrvCellCfg->bwp_InactivityTimer, sizeof(uint8_t));
            
	 }
      }
   }
   if(cuSrvCellCfg->pdsch_ServingCellConfig)
   {
      if(cuSrvCellCfg->pdsch_ServingCellConfig->choice.setup)
      {
         fillPdschServingCellCfg(cuSrvCellCfg->pdsch_ServingCellConfig->choice.setup, &duSrvCellCfg->pdschServCellCfg);
      }
   }
   if(cuSrvCellCfg->uplinkConfig)
   {
     if(cuSrvCellCfg->uplinkConfig->initialUplinkBWP)
     {
        ulBwp = ((BWP_UplinkDedicated_t *)(cuSrvCellCfg->uplinkConfig->initialUplinkBWP));
	if(ulBwp->pusch_Config)
	{
	   duSrvCellCfg->initUlBwp.puschPresent = true;
           fillPuschCfg(ulBwp->pusch_Config, &duSrvCellCfg->initUlBwp.puschCfg);
	}
	if(ulBwp->pucch_Config)
	{
	   duSrvCellCfg->initUlBwp.pucchPresent = true;
           //fillPucchCfg(ulBwp->pucch_Config, &duSrvCellCfg->initUlBwp.pucchCfg); 
	}
     }
     if(cuSrvCellCfg->uplinkConfig->firstActiveUplinkBWP_Id)
        duSrvCellCfg->firstActvUlBwpId = *(cuSrvCellCfg->uplinkConfig->firstActiveUplinkBWP_Id);

   }
}

/*******************************************************************
 *
 * @brief Fills Reconfig Cell group Info received by CU
 *
 * @details
 *
 *    Function : fillUeReCfgCellGrpInfo
 *
 *    Functionality: Fills Reconfig Cell group Info received by CU
 *
 * @params[in] CellGroupConfigRrc_t *macCellGrpCfg
 *             MacUeCfg*  duUeCfg
 * @return void
 *
 * ****************************************************************/
void fillUeReCfgCellGrpInfo(CellGroupConfigRrc_t *cellGrp, MacUeCfg *duUeCfg)
{
   MAC_CellGroupConfig_t     *macCellGroup = NULLP;
   PhysicalCellGroupConfig_t *phyCellGrpCfg = NULLP;
   SpCellConfig_t            *spcellCfg = NULLP;
   ServingCellConfig_t       *servCellCfg = NULLP;

   if(cellGrp)
   {
      /* Fill MacCell Group Reconfig  */
      if(cellGrp->mac_CellGroupConfig)
      {
         macCellGroup = ((MAC_CellGroupConfig_t *)(cellGrp->mac_CellGroupConfig));
         if(macCellGroup->schedulingRequestConfig)
	 {
            fillSchReqReConfig(macCellGroup->schedulingRequestConfig, &duUeCfg->macCellGrpCfg.schReqCfg);
	 }
         if(macCellGroup->tag_Config)
	 {
            fillTagReconfig(macCellGroup->tag_Config, &duUeCfg->macCellGrpCfg.tagCfg);
	 }
      }
      /* Fill Physical Cell Group Reconfig */
      if(cellGrp->physicalCellGroupConfig)
      {
         phyCellGrpCfg = ((PhysicalCellGroupConfig_t *)(cellGrp->physicalCellGroupConfig));
         if(phyCellGrpCfg->p_NR_FR1)
	 {
	    if(*(phyCellGrpCfg->p_NR_FR1) != duUeCfg->phyCellGrpCfg.pNrFr1)
               duUeCfg->phyCellGrpCfg.pNrFr1 = *(phyCellGrpCfg->p_NR_FR1);
	 }
	 if(phyCellGrpCfg->pdsch_HARQ_ACK_Codebook != duUeCfg->phyCellGrpCfg.pdschHarqAckCodebook)
            duUeCfg->phyCellGrpCfg.pdschHarqAckCodebook = phyCellGrpCfg->pdsch_HARQ_ACK_Codebook;
      }
      /* Fill SpCell Reconfig */
      if(cellGrp->spCellConfig)
      {
         spcellCfg = ((SpCellConfig_t *)(cellGrp->spCellConfig));  
         if(spcellCfg->servCellIndex)
	 {
	    if(*(spcellCfg->servCellIndex) != duUeCfg->spCellCfg.servCellIdx)
            {
               duUeCfg->spCellCfg.servCellIdx = *(spcellCfg->servCellIndex);
	    }
	 }
         /* Fill Serving cell Reconfig info */
	 if(cellGrp->spCellConfig->spCellConfigDedicated)
	 {
	    servCellCfg = ((ServingCellConfig_t *)(cellGrp->spCellConfig->spCellConfigDedicated));
            fillServingCellReconfig(servCellCfg, &duUeCfg->spCellCfg.servCellCfg);
	 }
      }
   }
}

/*******************************************************************
 *
 * @brief Fills UeReCfg Srb received by CU
 *
 * @details
 *
 *    Function : fillMacLcReCfgSrbInfo
 *
 *    Functionality: Fills UeReCfg Srb received  by CU
 *
 * @params[in] SRBs_ToBeSetup_Item_t *cuSrbItem,
 *             LcCfg *duUeLcCfg
 * @return void
 *
 * ****************************************************************/

void fillMacLcReCfgSrbInfo(LcCfg *duUeLcCfg, SRBs_ToBeSetup_Item_t *cuSrbItem)
{
   duUeLcCfg->lcId = cuSrbItem->sRBID;
   duUeLcCfg->drbQos = NULLP;
   duUeLcCfg->snssai = NULLP;
   duUeLcCfg->ulLcCfg = NULLP;
   duUeLcCfg->dlLcCfg.lcp = LC_PRIORITY_1;
}

/*******************************************************************
 *
 * @brief Fills DrbQos Info received by CU
 *
 * @details
 *
 *    Function : fillDrbQosInfo
 *
 *    Functionality: Fills DrbQos Info received  by CU
 *
 * @params[in] DrbQosInfo *duDrbQos, 
 *             QoSFlowLevelQoSParameters_t *cuDrbInfo
 * @return void
 *
 * ****************************************************************/

void fillDrbQosInfo(DrbQosInfo *duDrbQos, QoSFlowLevelQoSParameters_t *cuDrbInfo)
{
   duDrbQos->fiveQiType = cuDrbInfo->qoS_Characteristics.present;
   duDrbQos->u.nonDyn5Qi.fiveQi     =\
         cuDrbInfo->qoS_Characteristics.choice.non_Dynamic_5QI->fiveQI;
   if(cuDrbInfo->qoS_Characteristics.choice.non_Dynamic_5QI->averagingWindow)
   {
      duDrbQos->u.nonDyn5Qi.avgWindow = \
        *(cuDrbInfo->qoS_Characteristics.choice.non_Dynamic_5QI->averagingWindow);
   }
   duDrbQos->u.nonDyn5Qi.maxDataBurstVol = \
      *(cuDrbInfo->qoS_Characteristics.choice.non_Dynamic_5QI->maxDataBurstVolume);
   if(cuDrbInfo->qoS_Characteristics.choice.non_Dynamic_5QI->qoSPriorityLevel)
   {
      duDrbQos->u.nonDyn5Qi.priorLevel = \
         *(cuDrbInfo->qoS_Characteristics.choice.non_Dynamic_5QI->qoSPriorityLevel);
   }
   duDrbQos->ngRanRetPri.priorityLevel = \
      cuDrbInfo->nGRANallocationRetentionPriority.priorityLevel; 
   duDrbQos->ngRanRetPri.preEmptionCap = \
      cuDrbInfo->nGRANallocationRetentionPriority.pre_emptionCapability;
   duDrbQos->ngRanRetPri.preEmptionVul = \
      cuDrbInfo->nGRANallocationRetentionPriority.pre_emptionVulnerability;
   if(cuDrbInfo->gBR_QoS_Flow_Information)
   {
      memcpy(&duDrbQos->grbQosInfo.maxFlowBitRateDl, \
         cuDrbInfo->gBR_QoS_Flow_Information->maxFlowBitRateDownlink.buf, \
         cuDrbInfo->gBR_QoS_Flow_Information->maxFlowBitRateDownlink.size);
      memcpy(&duDrbQos->grbQosInfo.maxFlowBitRateUl, \
         cuDrbInfo->gBR_QoS_Flow_Information->maxFlowBitRateUplink.buf, \
         cuDrbInfo->gBR_QoS_Flow_Information->maxFlowBitRateUplink.size);
      memcpy(&duDrbQos->grbQosInfo.guarFlowBitRateDl,\
         cuDrbInfo->gBR_QoS_Flow_Information->guaranteedFlowBitRateDownlink.buf, \
         cuDrbInfo->gBR_QoS_Flow_Information->guaranteedFlowBitRateDownlink.size);
      memcpy(&duDrbQos->grbQosInfo.guarFlowBitRateUl,\
         cuDrbInfo->gBR_QoS_Flow_Information->guaranteedFlowBitRateUplink.buf, \
         cuDrbInfo->gBR_QoS_Flow_Information->guaranteedFlowBitRateUplink.size);
   }
   duDrbQos->pduSessionId = 0;
   duDrbQos->ulPduSessAggMaxBitRate = 0;
}

/*******************************************************************
 *
 * @brief Fills Ue ReCfg Drb Info received by CU
 *
 * @details
 *
 *    Function : fillMacLcReCfgDrbInfo
 *
 *    Functionality: Fills Ue ReCfg Drb Info received  by CU
 *
 * @params[in] LcCfg *duLcCfg,
 *             DRBs_ToBeSetup_Item_t *cuDrbItem
 * @return ROK/RFAILED
 *
 * ****************************************************************/

uint8_t fillMacLcReCfgDrbInfo(LcCfg *duLcCfg, DRBs_ToBeSetup_Item_t *cuDrbItem)
{
   DRB_Information_t *drbInfo = NULLP;

   duLcCfg->lcId = getMacDrbLcId();
   if(cuDrbItem->qoSInformation.present == QoSInformation_PR_eUTRANQoS)
   {
      //TODO : To be handled for eUTRANQoS

   }
   if(cuDrbItem->qoSInformation.present == QoSInformation_PR_choice_extension)
   {
      if(cuDrbItem->qoSInformation.choice.choice_extension)
      {
         if(cuDrbItem->qoSInformation.choice.choice_extension->value.present ==
	 QoSInformation_ExtIEs__value_PR_DRB_Information)
	 {
            drbInfo = &cuDrbItem->qoSInformation.choice.choice_extension->value.choice.DRB_Information;
            
	    if(!duLcCfg->drbQos)
	    {
               DU_ALLOC_SHRABL_BUF(duLcCfg->drbQos, sizeof(DrbQosInfo));
	       if(!duLcCfg->drbQos)
               {
                  DU_LOG("\n DUAPP:Memory failed at allocating DrbQos at fillMacLcReCfgDrbInfo()");
		  return RFAILED;
	       }
               
	    }
	    if(drbInfo->dRB_QoS.qoS_Characteristics.present == QoS_Characteristics_PR_non_Dynamic_5QI)
	    {
	       fillDrbQosInfo(duLcCfg->drbQos, &drbInfo->dRB_QoS);
	    }
	    if(!duLcCfg->snssai)
	    {
               DU_ALLOC_SHRABL_BUF(duLcCfg->snssai, sizeof(Snssai));
               if(!duLcCfg->snssai)
	       {
                  DU_LOG("\n DUAPP:Memory failed at allocating SNSSAI at fillMacLcReCfgDrbInfo()");
		  return RFAILED;
	       }
	    }
	    memcpy(&duLcCfg->snssai->sst, drbInfo->sNSSAI.sST.buf, \
	       drbInfo->sNSSAI.sST.size);
            if(drbInfo->sNSSAI.sD)
	    {
	       memcpy(duLcCfg->snssai->sd, drbInfo->sNSSAI.sD->buf, \
	          drbInfo->sNSSAI.sD->size);
	    }
         }/*End of DRB Info*/
      }
      
   }/*End of Qos Choice Exten */
   if(cuDrbItem->uLConfiguration)
   {
      if(cuDrbItem->uLConfiguration->uLUEConfiguration != ULUEConfiguration_no_data)
      {
         if(!duLcCfg->ulLcCfg)
         {
	    DU_ALLOC_SHRABL_BUF(duLcCfg->ulLcCfg, sizeof(UlLcCfg));
            if(duLcCfg->ulLcCfg)
            {
               DU_LOG("\nDUAPP: Memory allocation failed for UL LcCfg at fillMacLcReCfgDrbInfo");
	       return RFAILED;
            }
         }
         //TODO: dependency on ProtocolExtensionContainer in ASN, fillUeUlLcCfg();
      }
   }
   duLcCfg->dlLcCfg.lcp = LC_PRIORITY_1;
   return ROK;
}

/*******************************************************************
 *
 * @brief Fills Ue ReCfg Lc List received by CU
 *
 * @details
 *
 *    Function : fillUeReCfgLcList
 *
 *    Functionality: Fills Ue ReCfg Lc List received  by CU
 *
 * @params[in] DRBs_ToBeSetup_List_t *cuDrbList, 
 *             SRBs_ToBeSetup_List_t *cuSrbList,
 *             MacUeCfg *duUeCfg, MacUeCfg *macUeCfg
 * @return void
 *
 * ****************************************************************/

void fillUeReCfgLcList(DRBs_ToBeSetup_List_t *cuDrbList, SRBs_ToBeSetup_List_t *cuSrbList, \
   MacUeCfg *duUeCfg, MacUeCfg *macUeCfg)
{
   uint8_t drbIdx = 0;
   uint8_t srbIdx = 0;
   SRBs_ToBeSetup_Item_t *srbItem = NULLP;
   DRBs_ToBeSetup_Item_t *drbItem = NULLP;

   macUeCfg->numLcsToAdd = 0;
   memset(macUeCfg->lcCfgToAddList, 0, sizeof(LcCfg));

   if(cuDrbList)
   {
      for(drbIdx = 0; drbIdx < cuDrbList->list.count; drbIdx++)
      {
         drbItem = &cuDrbList->list.array[drbIdx]->value.choice.DRBs_ToBeSetup_Item;
         if(duUeCfg->numLcsToAdd >= 0)
	 {
	    /* update existing Ue Cfg Info */
            fillMacLcReCfgDrbInfo(&duUeCfg->lcCfgToAddList[duUeCfg->numLcsToAdd], drbItem);
	    /*update new ue cfg to be sent to MAC */
	    memcpy(&macUeCfg->lcCfgToAddList[drbIdx], &duUeCfg->lcCfgToAddList[duUeCfg->numLcsToAdd],\
	       sizeof(LcCfg));
	    duUeCfg->numLcsToAdd++;
	    duUeCfg->numLcsToAdd++;

	 }
      }
   }
   if(cuSrbList)
   {
      for(srbIdx = 0; srbIdx < cuSrbList->list.count; srbIdx++)
      {
         srbItem = &cuSrbList->list.array[srbIdx]->value.choice.SRBs_ToBeSetup_Item;
	 if(duUeCfg->numLcsToAdd >= 0)
	 {
	    /* update existing Ue Cfg Info */
            fillMacLcReCfgSrbInfo(&duUeCfg->lcCfgToAddList[duUeCfg->numLcsToAdd], srbItem);
	    /*update new ue cfg to be sent to MAC */
	    if(macUeCfg->numLcsToAdd > 0)
	    {
	       memcpy(&macUeCfg->lcCfgToAddList[macUeCfg->numLcsToAdd], &duUeCfg->lcCfgToAddList[duUeCfg->numLcsToAdd],\
	          sizeof(LcCfg));
	    }
	    else
	    {
               memcpy(&macUeCfg->lcCfgToAddList[srbIdx], &duUeCfg->lcCfgToAddList[duUeCfg->numLcsToAdd],\
	          sizeof(LcCfg));
	    }
	    duUeCfg->numLcsToAdd++;
	    macUeCfg->numLcsToAdd++;
	 }
      }
   }
}

/*******************************************************************
 *
 * @brief Fills Ue ReCfg Srb Lc List received by CU
 *
 * @details
 *
 *    Function : fillRlcSrbLcList
 *
 *    Functionality: Fills Ue ReCfg Lc List received  by CU
 *
 * @params[in] RlcBearerCfg *duRlcLcCfg, 
 *             SRBs_ToBeSetup_Item_t *cuSrbItem
 * @return void
 *
 * ****************************************************************/

void fillRlcSrbLcList(RlcBearerCfg *duRlcLcCfg, SRBs_ToBeSetup_Item_t *cuSrbItem)
{
   duRlcLcCfg->rbId    = cuSrbItem->sRBID;
   duRlcLcCfg->rbType  = RB_TYPE_SRB;
   duRlcLcCfg->lcId    = cuSrbItem->sRBID;
   duRlcLcCfg->lcType  = LCH_DCCH;
   duRlcLcCfg->rlcMode = RLC_AM;
   memset(&duRlcLcCfg->u.amCfg, 0, sizeof(AmBearerCfg));
   fillAmInfo(&duRlcLcCfg->u.amCfg);
}

/*******************************************************************
 *
 * @brief Fills Ue ReCfg Drb Lc List received by CU
 *
 * @details
 *
 *    Function : fillRlcDrbLcList
 *
 *    Functionality: Fills Ue Drb ReCfg Lc List received  by CU
 *
 * @params[in] RlcBearerCfg *duRlcDrbCfg, 
 *             DRBs_ToBeSetup_Item_t *cuDrbItem
 * @return void
 *
 * ****************************************************************/

void fillRlcDrbLcList(RlcBearerCfg *duRlcDrbCfg, DRBs_ToBeSetup_Item_t *cuDrbItem)
{
   duRlcDrbCfg->rbId    = cuDrbItem->dRBID;
   duRlcDrbCfg->rbType  = RB_TYPE_DRB;
   duRlcDrbCfg->lcId    = getRlcDrbLcId();
   duRlcDrbCfg->lcType  = LCH_DTCH;
   duRlcDrbCfg->rlcMode = cuDrbItem->rLCMode;
   switch(duRlcDrbCfg->rlcMode)
   {
      case RLC_AM:
         memset(&duRlcDrbCfg->u.amCfg, 0, sizeof(AmBearerCfg));
         fillAmInfo(&duRlcDrbCfg->u.amCfg);
	 break;
      case RLC_UM_BI_DIRECTIONAL:
	 memset(&duRlcDrbCfg->u.umBiDirCfg, 0, sizeof(UmBiDirBearerCfg));
	 fillUmBiDirInfo(&duRlcDrbCfg->u.umBiDirCfg);
	 break;
      case RLC_UM_UNI_DIRECTIONAL_UL:
	 memset(&duRlcDrbCfg->u.umUniDirUlCfg, 0, sizeof(UmUniDirUlBearerCfg));
	 fillUmUniDirUlInfo(&duRlcDrbCfg->u.umUniDirUlCfg);
	 break;
      case RLC_UM_UNI_DIRECTIONAL_DL:
	 memset(&duRlcDrbCfg->u.umUniDirDlCfg, 0, sizeof(UmUniDirDlBearerCfg));
	 fillUmUniDirDlInfo(&duRlcDrbCfg->u.umUniDirDlCfg);
	 break;
      default :
	 DU_LOG("\nDU_APP: Rlc Mode invalid %d at fillRlcDrbLcList()", duRlcDrbCfg->rlcMode);
	 break;
   }
}

/*******************************************************************
 *
 * @brief Fills Ue Rlc ReCfg Rb List received by CU
 *
 * @details
 *
 *    Function : fillRlcReCfgRbList
 *
 *    Functionality: Fills Ue Rlc ReCfg Rb List received  by CU
 *
 * @params[in] DRBs_ToBeSetup_List_t *cuDrbList,
 *             SRBs_ToBeSetup_List_t *cuSrbList,
 *             RlcUeCfg *duRlcUeCfg
 * @return ROK/RFAILED
 *
 * ****************************************************************/

uint8_t fillRlcReCfgRbList(DRBs_ToBeSetup_List_t *cuDrbList, SRBs_ToBeSetup_List_t *cuSrbList, RlcUeCfg *duRlcUeCfg)
{
   uint8_t srbIdx , drbIdx;
   uint8_t lcCount = 0;
   DRBs_ToBeSetup_Item_t *drbItem = NULLP;
   SRBs_ToBeSetup_Item_t *srbItem = NULLP;

   if(cuSrbList)
   {
      for(srbIdx = 0; srbIdx < cuSrbList->list.count; srbIdx++)
      {
         srbItem = &cuSrbList->list.array[srbIdx]->value.choice.SRBs_ToBeSetup_Item;
	 if(duRlcUeCfg->numLcsToAdd >= 0)
	 {
            fillRlcSrbLcList(&duRlcUeCfg->rlcBearerCfgToAdd[duRlcUeCfg->numLcsToAdd], srbItem);
	    duRlcUeCfg->numLcsToAdd++;
	    lcCount++;
	 }
      }

   }
   if(cuDrbList)
   {
      for(drbIdx = 0; drbIdx < cuDrbList->list.count; drbIdx++)
      {
         drbItem = &cuDrbList->list.array[drbIdx]->value.choice.DRBs_ToBeSetup_Item;
	 if(duRlcUeCfg->numLcsToAdd >= 0)
	 {
            fillRlcDrbLcList(&duRlcUeCfg->rlcBearerCfgToAdd[duRlcUeCfg->numLcsToAdd], drbItem);
            duRlcUeCfg->numLcsToAdd++;
	    lcCount++;
	 }
      }
   }
   return lcCount;
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
 * @params[in] RlcUeCfg *duUeCfg
 *             DRBs_ToBeSetup_List_t *cuDrbList
 *             SRBs_ToBeSetup_List_t *cuSrbList
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t duBuildAndSendUeReCfgReqToRlc(DRBs_ToBeSetup_List_t *cuDrbList, SRBs_ToBeSetup_List_t *cuSrbList, RlcUeCfg *duUeCfg)
{
   uint8_t idx, lcStartIdx;
   uint8_t ret = ROK, newLcsToAdd = 0;
   RlcUeCfg *rlcUeCfg = NULLP;

   DU_ALLOC_SHRABL_BUF(rlcUeCfg, sizeof(RlcUeCfg));
   if(!rlcUeCfg)
   {
      DU_LOG("\n DU_APP: Memory alloc failed at duBuildAndSendUeReCfgReqToRlc()");
      return RFAILED;
   }
   else
   {
      memset(rlcUeCfg, 0, sizeof(RlcUeCfg));
      memcpy(rlcUeCfg, duUeCfg, sizeof(RlcUeCfg));
   }

   newLcsToAdd = fillRlcReCfgRbList(cuDrbList, cuSrbList, duUeCfg);
   
   if(newLcsToAdd > 0)
   {
      lcStartIdx = duUeCfg->numLcsToAdd - newLcsToAdd;
      rlcUeCfg->numLcsToAdd = newLcsToAdd;

      for(idx = 0; idx < rlcUeCfg->numLcsToAdd; idx++,lcStartIdx++)
      {
         memcpy(&rlcUeCfg->rlcBearerCfgToAdd[idx], \
	   &duUeCfg->rlcBearerCfgToAdd[lcStartIdx], sizeof(RlcBearerCfg));
      }
      ret = sendUeReCfgReqToRlc(rlcUeCfg); 
      
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
 *             MacUeCfg*  duUeCfg
 *             DRBs_ToBeSetup_List_t *cuDrbList
 *             SRBs_ToBeSetup_List_t *cuSrbList
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t duBuildAndSendUeReCfgReqToMac(CellGroupConfigRrc_t *macCellGrpCfg, MacUeCfg *duUeCfg,\
   DRBs_ToBeSetup_List_t *cuDrbList, SRBs_ToBeSetup_List_t *cuSrbList)
{
   uint8_t ret =ROK;
   MacUeCfg *macUeCfg = NULLP;

   DU_ALLOC_SHRABL_BUF(macUeCfg, sizeof(MacUeCfg));
   if(!macUeCfg)
   {
      DU_LOG("\n DU_APP: Memory alloc failed at duBuildAndSendUeReCfgReqToMac()");
      return RFAILED;
   }
   else
   {
      memset(macUeCfg, 0, sizeof(MacUeCfg));
      fillUeReCfgCellGrpInfo(macCellGrpCfg, duUeCfg); //updating duUeCfg
      memcpy(macUeCfg, duUeCfg, sizeof(MacUeCfg));
      fillUeReCfgLcList(cuDrbList, cuSrbList, duUeCfg, macUeCfg);
      ret = sendUeReCfgReqToMac(macUeCfg);
   }
   return ret;
}

/*******************************************************************
 *
 * @brief Function to decode cu to Du Info
 *
 * @details
 *
 *    Function : decodeCuToDuInfo
 *
 *    Functionality: Function to decode cu to Du Info
 *
 * @params[in] CUtoDURRCInformation_t *cuToDuInfo
 *
 * @return CellGroupConfigRrc_t *
 *
 * ****************************************************************/

CellGroupConfigRrc_t *decodeCuToDuInfo(CUtoDURRCInformation_t *cuToDuInfo)
{
   uint8_t idx2;
   uint16_t id;
   uint16_t recvBufLen;
   CellGroupConfigRrc_t *cellGrpCfg = NULLP;
   CUtoDURRCInformation_ExtIEs_t *extIeInfo = NULLP;
   asn_dec_rval_t rval; /* Decoder return value */
   memset(&rval, 0, sizeof(asn_dec_rval_t));

   if(cuToDuInfo->iE_Extensions)
   {
      for(idx2 = 0; idx2 < cuToDuInfo->iE_Extensions->list.count; idx2++)
      {
         extIeInfo = ((CUtoDURRCInformation_ExtIEs_t *)(cuToDuInfo->iE_Extensions->list.array[idx2]));
	 id = extIeInfo->id;
         switch(id)
         {
            case ProtocolIE_ID_id_CellGroupConfig:
            {
	       recvBufLen = extIeInfo->extensionValue.choice.CellGroupConfig.size;
	       /* decoding the CellGroup Buf received */
	       DU_ALLOC(cellGrpCfg, sizeof(CellGroupConfigRrc_t));
	       if(cellGrpCfg)
	       {
                  memset(cellGrpCfg, 0, sizeof(CellGroupConfigRrc_t));
                  rval = aper_decode(0, &asn_DEF_CellGroupConfigRrc, (void **)&cellGrpCfg,
	             extIeInfo->extensionValue.choice.CellGroupConfig.buf, recvBufLen, 0, 0);
                  if(rval.code == RC_FAIL || rval.code == RC_WMORE)
                  {
                     DU_LOG("\nF1AP : ASN decode failed at decodeCellGrpCfg()");
                     return NULLP;
                  }
                  xer_fprint(stdout, &asn_DEF_CellGroupConfigRrc, cellGrpCfg);
	       }
	       break;
            }
            default:
               DU_LOG("\nF1AP : Invalid IE received CUtoDURRCInformation:%d at decodeCellGrpCfg()", id);
      	       break;
         }
      }
   }
   return cellGrpCfg;
}

/******************************************************************
 *
 * @brief Processes UE Context Setup Request sent by CU
 *
 * @details
 *
 *    Function : procUecontextSetupReq
 *
 *    Functionality: Processes UE Context Setup Request sent by CU
 *
 * @params[in] F1AP_PDU_t ASN decoded F1AP message
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t procUeContextSetupReq(F1AP_PDU_t *f1apMsg)
{
   uint8_t                 idx, ueIdx, ret;
   uint8_t                 servCellIdx = 0;
   uint32_t                gnbCuUeF1apId, gnbDuUeF1apId;
   UEContextSetupRequest_t *ueCntxtMsg = NULLP;
   SRBs_ToBeSetup_List_t   *cuSrbList    = NULLP;
   DRBs_ToBeSetup_List_t   *cuDrbList    = NULLP;
   CUtoDURRCInformation_t  *cuToDuInfo = NULLP;
   CellGroupConfigRrc_t    *cellGrpCfg = NULLP;
   DuUeCb  *ueCb = NULLP;

   DU_LOG("\nDU_APP : UE Context Setup Request Message");
   ueCntxtMsg = &f1apMsg->choice.initiatingMessage->value.choice.UEContextSetupRequest;

   ret = ROK;

   for(idx=0; idx<ueCntxtMsg->protocolIEs.list.count; idx++)
   {
      switch(ueCntxtMsg->protocolIEs.list.array[idx]->id)
      {
	 case ProtocolIE_ID_id_gNB_CU_UE_F1AP_ID:
	    {
	       gnbCuUeF1apId = ueCntxtMsg->protocolIEs.list.array[idx]->value.choice.GNB_CU_UE_F1AP_ID;
	       break;
	    }
	 case ProtocolIE_ID_id_gNB_DU_UE_F1AP_ID:
	    {
	       gnbDuUeF1apId = ueCntxtMsg->protocolIEs.list.array[idx]->value.choice.GNB_DU_UE_F1AP_ID;
	       break;
	    }
	 case ProtocolIE_ID_id_SpCell_ID:
	    {
               /* TBD in case of handover */
	       break;
	    }
         case ProtocolIE_ID_id_ServCellIndex:
	    {
               servCellIdx = ueCntxtMsg->protocolIEs.list.array[idx]->value.choice.ServCellIndex;
               break;
	    }
         case ProtocolIE_ID_id_SpCellULConfigured:
	    /* Indicates whether the gNB-CU requests the gNB-DU to configure the uplink as no UL, 
	    UL, SUL or UL+SUL for the indicated cell for the UE */
	    break;
         case ProtocolIE_ID_id_CUtoDURRCInformation:
	    {
	       cuToDuInfo = &(ueCntxtMsg->protocolIEs.list.array[idx]->value.choice.CUtoDURRCInformation);
               cellGrpCfg = decodeCuToDuInfo(cuToDuInfo); 
               break;
            } 
         case ProtocolIE_ID_id_SCell_ToBeSetup_List:
	    break; 
         case ProtocolIE_ID_id_SRBs_ToBeSetup_List:
	    {
	       cuSrbList = &ueCntxtMsg->protocolIEs.list.array[idx]->value.choice.SRBs_ToBeSetup_List;
	       
	       break;
	    }
         case ProtocolIE_ID_id_DRBs_ToBeSetup_List:
	    cuDrbList = &ueCntxtMsg->protocolIEs.list.array[idx]->value.choice.DRBs_ToBeSetup_List;
            break;

         case ProtocolIE_ID_id_RRCContainer:
	    /*TODO: DL RRC Msg Transfer */
	    //ueCntxtMsg->protocolIEs.list.array[idx]->value.choice.RRCContainer;
            break;

         default:
	    DU_LOG("\nDU_APP : Invalid IE received in Ue Context Setup Request:%ld",
		  ueCntxtMsg->protocolIEs.list.array[idx]->id);
            break;
      }
   }

   for(ueIdx = 0; ueIdx < MAX_NUM_UE; ueIdx++)
   {
      if((duCb.actvCellLst[servCellIdx]->ueCb[ueIdx].gnbDuUeF1apId == gnbDuUeF1apId) &&
         (duCb.actvCellLst[servCellIdx]->ueCb[ueIdx].gnbCuUeF1apId == gnbCuUeF1apId) &&
         (duCb.actvCellLst[servCellIdx]->ueCb[ueIdx].ueState == UE_ACTIVE))
      {
         DU_LOG("\nDU_APP: Received Reconfig for cellId [%d] and ueId [%d]",\
            duCb.actvCellLst[servCellIdx]->cellId, duCb.actvCellLst[servCellIdx]->ueCb[ueIdx].macUeCfg.ueIdx);
	 ueCb = &duCb.actvCellLst[servCellIdx]->ueCb[ueIdx];
         break;
      }
   }
   if(ueIdx != MAX_NUM_UE)
   {
      /* Filling RLC Ue Reconfig */ 
      ret = duBuildAndSendUeReCfgReqToRlc(cuDrbList, cuSrbList, &ueCb->rlcUeCfg);
      if(ret)
      {
         DU_LOG("\nDU_APP: Failed at duBuildAndSendUeReCfgReqToRlc for cellId [%d]", \
            duCb.actvCellLst[servCellIdx]->cellId);
         ret = RFAILED;
      }
      /* Filling MAC Ue Reconfig */
      if(cellGrpCfg)
      {
         ret = duBuildAndSendUeReCfgReqToMac(cellGrpCfg, &ueCb->macUeCfg, cuDrbList, cuSrbList);
         if(ret)
         {
            DU_LOG("\nDU_APP: Failed at duBuildAndSendUeReCfgReqToMac for cellId [%d]", \
               duCb.actvCellLst[servCellIdx]->cellId);
            ret = RFAILED;
	 }
	 
      }
   }
   return ret;
}

/*******************************************************************
 *
 * @brief Processes UE Reconfig Rsp received from RLC UL
 *
 * @details
 *
 *    Function : DuProcRlcUlUeReconfigRsp
 *
 *    Functionality: 
 *     Processes UE Reconfig Rsp received from RLC UL
 * 
 *  @params[in]  Post structure
 *               Pointer to RlcCfgCfm
 *  @return ROK     - success
 *          RFAILED - failure
 * 
 *****************************************************************/

uint8_t DuProcRlcUlUeReconfigRsp(Pst *pst, RlcUeCfgRsp *cfgRsp)
{
   uint8_t ret = ROK;

   if(cfgRsp)
   {
      if(cfgRsp->result == RLC_DU_APP_RSP_OK)
      {
         DU_LOG("\nDU_APP: RLC UE Reconfig Response : SUCCESS [UE IDX:%d]", cfgRsp->ueIdx);
      }
      else
      {
         DU_LOG("\nDU_APP: RLC UE Response Response : FAILED [UE IDX:%d, REASON :%d]",\
	    cfgRsp->ueIdx, cfgRsp->reason);
	 ret = RFAILED;
      }
      DU_FREE_SHRABL_BUF(DU_APP_MEM_REGION, DU_POOL, cfgRsp, sizeof(RlcUeCfgRsp));
   }
   else
   {
      DU_LOG("\nDU_APP: Received RLC Ue Reconfig Response is NULL");
      ret = RFAILED;
   }
   return ret;
}

/*******************************************************************
 *
 * @brief Handle UE Reconfig response from MAC
 *
 * @details
 *
 *    Function : DuHandleMacUeReconfigRsp
 *
 *    Functionality: Handle UE reconfig response from MAC
 *
 * @params[in] Pointer to MacUeCfgRsp and Pst 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t DuHandleMacUeReconfigRsp(Pst *pst, MacUeCfgRsp *cfgRsp)
{
   if(cfgRsp->result == MAC_DU_APP_RSP_OK)
   {
      DU_LOG("\nDU APP : MAC UE Reconfig Response : SUCCESS [UE IDX : %d]", cfgRsp->ueIdx);
   }
   else
   {
      DU_LOG("\nDU APP : MAC UE Reconfig Response : FAILURE [UE IDX : %d]", cfgRsp->ueIdx);
   }
   return ROK;
}

/**********************************************************************
  End of file
 ***********************************************************************/
