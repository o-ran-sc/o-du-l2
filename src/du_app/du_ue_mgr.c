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
#include "du_ue_mgr.h"

#ifdef EGTP_TEST
U32 sduId = 0;
#endif

DuMacDlCcchInd packMacDlCcchIndOpts[] =
{
   packMacDlCcchInd,   /* Loose coupling */
   MacHdlDlCcchInd,    /* TIght coupling */
   packMacDlCcchInd    /* Light weight-loose coupling */
};

DuMacUeCreateReq packMacUeCreateReqOpts[] =
{
   packDuMacUeCreateReq,       /* Loose coupling */
   MacHdlUeCreateReq,          /* TIght coupling */
   packDuMacUeCreateReq,       /* Light weight-loose coupling */
};

/******************************************************************
 *
 * @brief Send UE configuration to RLC
 *
 * @details
 *
 *    Function : duSendUeCreateReqToRlc
 *
 *    Functionality: Send UeCreateReqToRlc
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
S16 duSendUeCreateReqToRlc()
{
   U8  idx;
   Pst pst;
   CkwCfgInfo *ueCfg;
   
   DU_SET_ZERO(&ueCfg, sizeof(ueCfg));
   DU_SET_ZERO(&pst, sizeof(Pst));

   DU_ALLOC(ueCfg, sizeof(CkwCfgInfo));

#ifdef EGTP_TEST
   ueCfg->ueId = UE_ID;
#endif
   ueCfg->cellId = NR_CELL_ID;
   ueCfg->numEnt = 1;
   
   for(idx = 0; idx < ueCfg->numEnt; idx++)
   {
#ifdef EGTP_TEST
      ueCfg->entCfg[idx].rbId           = RB_ID;
      ueCfg->entCfg[idx].rbType         = CM_LTE_DRB;
      ueCfg->entCfg[idx].lCh[0].lChId   = LC_ID;
      ueCfg->entCfg[idx].lCh[0].type    = CM_LTE_LCH_DTCH;
#endif
      ueCfg->entCfg[idx].entMode        = CM_LTE_MODE_UM;
      ueCfg->entCfg[idx].dir            = CKW_CFG_DIR_BOTH;
      switch(ueCfg->entCfg[idx].entMode)
      {
         case CM_LTE_MODE_TM:
         {
            break;
         }

         case CM_LTE_MODE_UM:
         {
            ueCfg->entCfg[idx].m.umInfo.dl.snLen = 1;      /* For 12 bit SN */
            ueCfg->entCfg[idx].m.umInfo.ul.snLen = 1;      /* For 12 bit SN */
            ueCfg->entCfg[idx].m.umInfo.ul.reOrdTmr = 10;  /* in msec */
            break;
         }

         case CM_LTE_MODE_AM:
         {
            break;
         }
         
         default:
            break;
      } /* End of switch(entMode) */
   } /* End of entity configuration for loop */

   /* Fill Pst */
   pst.selector  = DU_SELECTOR_LWLC;
   pst.srcEnt    = ENTDUAPP;
   pst.dstEnt    = ENTKW;
   pst.dstInst   = RLC_UL_INST;
   pst.dstProcId = DU_PROC;
   pst.srcProcId = DU_PROC;
   pst.region    = duCb.init.region;

   /* Sending to RLC */
   packUeCreateReq(&pst, ueCfg);

   RETVALUE(ROK); 
} /* End of duSendUeCreateReqToRlc */

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
S16 duHdlEgtpDlData(EgtpMsg  *egtpMsg)
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
   pst.selector  = DU_SELECTOR_LWLC;
   pst.srcEnt    = ENTDUAPP;
   pst.dstEnt    = ENTKW;
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

PUBLIC S16 duHdlRlcUlData(Pst *pst, KwuDatIndInfo* datInd, Buffer *mBuf)
{
   DU_LOG("\nDU_APP : Received UL Data at DU_APP");
 
   /* Send UL data to CU via EGTP */
   duSendEgtpDatInd(mBuf);
   SPutMsg(mBuf);

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
uint8_t duBuildAndSendDlCcchInd(uint8_t *dlCcchMsg, uint16_t crnti, uint16_t cellId, DlCcchMsgType msgType)
{
   Pst pst;
	uint8_t ret                  = ROK;
   DlCcchIndInfo *dlCcchIndInfo = NULLP;

   DU_LOG("\nDU APP : Building and Sending DL CCCH Ind to MAC");

	DU_ALLOC_SHRABL_BUF(dlCcchIndInfo, sizeof(DlCcchIndInfo));

   if(!dlCcchIndInfo)
   {
		DU_LOG("\nDU APP : Memory alloc failed while building DL CCCH Ind");
		return RFAILED;
	}

	dlCcchIndInfo->cellId = cellId;
	dlCcchIndInfo->crnti = crnti;
	dlCcchIndInfo->msgType = msgType;
	DU_ALLOC_SHRABL_BUF(dlCcchIndInfo->dlCcchMsg, strlen((const char*)dlCcchMsg));
	if(!dlCcchIndInfo->dlCcchMsg)
   {
		DU_LOG("\nDU APP : Memory alloc failed while building DL CCCH Ind");
		DU_FREE_SHRABL_BUF(DU_APP_MEM_REGION, DU_POOL, dlCcchIndInfo, sizeof(DlCcchIndInfo));
		return RFAILED;
	}
	memcpy(dlCcchIndInfo->dlCcchMsg, dlCcchMsg, strlen((const char*)dlCcchMsg));
	DU_FREE(dlCcchMsg, strlen((const char*)dlCcchMsg));

	/* Fill Pst */
	pst.selector  = DU_MAC_LWLC;
	pst.srcEnt    = ENTDUAPP;
	pst.dstEnt    = ENTRG;
	pst.dstInst   = 0;
	pst.srcInst   = 0;
	pst.dstProcId = DU_PROC;
	pst.srcProcId = DU_PROC;
	pst.region    = DU_APP_MEM_REGION;
	pst.pool      = DU_POOL;
	pst.event     = EVENT_MAC_DL_CCCH_IND;

   ret = (*packMacDlCcchIndOpts[pst.selector])(&pst, dlCcchIndInfo);
	if(ret != ROK)
	{
      DU_LOG("\nDU_APP : Failure in sending DL CCCH to MAC");
		DU_FREE_SHRABL_BUF(DU_APP_MEM_REGION, DU_POOL, dlCcchIndInfo->dlCcchMsg, strlen((const char*)dlCcchMsg));
		DU_FREE_SHRABL_BUF(DU_APP_MEM_REGION, DU_POOL, dlCcchIndInfo, sizeof(DlCcchIndInfo));
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
	uint8_t                srbId, idx, ret;
	uint16_t               crnti, cellId;
	int                    ueIdx = -1;
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
			case ProtocolIE_ID_id_RRCContainer:
				{
				   DU_ALLOC(dlCcchMsg, dlRrcMsg->protocolIEs.list.array[idx]->value.choice.RRCContainer.size);
					memcpy(dlCcchMsg,
							dlRrcMsg->protocolIEs.list.array[idx]->value.choice.RRCContainer.buf,
							dlRrcMsg->protocolIEs.list.array[idx]->value.choice.RRCContainer.size);
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
		   duCb.ueCcchCtxt[idx].gnbCuUeF1apId = gnbCuUeF1apId;
		   crnti  = duCb.ueCcchCtxt[idx].crnti;
			cellId = duCb.ueCcchCtxt[idx].cellId;
			break;
		}
	}
	if(srbId == SRB_ID_1) //RRC connection setup
	{
		ret = duBuildAndSendDlCcchInd(dlCcchMsg, crnti, cellId, RRC_SETUP);
      if(ret)
      {
         DU_LOG("\nDU_APP: Falied at duBuildAndSendDlCcchInd()");
      }
      else
      {
		   if(duCb.actvCellLst[idx]->numActvUes < DU_MAX_UE)
			{
            ret = duCreateUeCb(&duCb.ueCcchCtxt[idx], &ueIdx);
				if(ret)
				{
               DU_LOG("\nDU_APP: Failed at duCreateUeCb for cellId [%d]", duCb.ueCcchCtxt[idx].cellId);
				}
				else
				{
               DU_LOG("\nDU_APP: Sending Ue create Req to MAC");
			      duBuildAndSendUeCreateReqToMac(&duCb.actvCellLst[idx]->ueCb[ueIdx]);
			      /* TODO:  Send Ue create Req To RLC */
				}
			}
			else
			{
            DU_LOG("\nDU_APP: Max Active UEs has reached");
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

   ret = (BuildAndSendInitialRrcMsgTransfer(gnbDuUeF1apId, ulCcchIndInfo->crnti,
				ulCcchIndInfo->ulCcchMsg));
	if(ret != ROK)
	{
      DU_LOG("\nDU_APP : BuildAndSendInitialRrcMsgTransfer failed");
	}

   DU_FREE_SHRABL_BUF(MAC_MEM_REGION, RG_POOL, ulCcchIndInfo->ulCcchMsg, strlen((const char*)ulCcchIndInfo->ulCcchMsg));
   DU_FREE_SHRABL_BUF(MAC_MEM_REGION, RG_POOL, ulCcchIndInfo, sizeof(UlCcchIndInfo));

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
            initDlBwp->pdcchCfg.cRSetToAddModList[idx].freqDomainRsrc[idx] =\
             PDCCH_FREQ_DOM_RSRC;
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
         initDlBwp->pdschCfg.timeDomRsrcAllociList[idx].startSymbolAndLength = \
          PDSCH_START_SYMBOL_LEN;
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
            initUlBwp->puschCfg.timeDomRsrcAllocList[idx].startSymbolAndLength =\
              PUSCH_START_SYMBOL_LEN;
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
      lcCfgInfo->dlLcCfg.lcp = SRB_ID_1;
      
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
void fillMacUeCfg(MacUeCfg *ueCfg)
{
   uint8_t idx;

   /* Filling MacCellGroup Config */ 
   fillMacCellGrpInfo(&ueCfg->macCellGrpCfg);
   /* Filling PhyCellGroup Config */ 
   fillPhyCellGrpInfo(&ueCfg->phyCellGrpCfg);
   /* Filling SpCellGroup Config */ 
   fillSpCellGrpInfo(&ueCfg->spCellCfg);
   /* Filling AMBR for UL and DL */ 
	ueCfg->maxAggrBitRate = NULLP;
   /* Filling LC Context */
   ueCfg->numLcs = 1;
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
 * @brief Fills Ue Index for DuUeCb
 *
 * @details
 *
 *    Function : fillUeIdx
 *
 *    Functionality: Fills Ue Index for DuUeCb
 *
 * @params[in] ueIdx to be filled
 *             pointer to ueBitMap
 * @return ROK     - success
 *         RFAILED - failure
 *
 *****************************************************************/
uint8_t fillUeIdx(int *ueIdx, uint8_t *ueBitMap)
{
   uint8_t ueBitPos = 0;
   uint8_t ueBitMask = 1; 
   
   do{
   	 if((*ueBitMap & ueBitMask) == 0)
   	 {
   	   *ueIdx = ueBitPos;
   	   *ueBitMap |= ueBitMask; 
   	   break;
       }
   	 else
   	 {
   	    ueBitPos++; 
   	    ueBitMask <<= 1;
   	 }
     }while(ueBitPos < DU_MAX_UE);

  return *ueIdx;
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
uint8_t duCreateUeCb(UeCcchCtxt *ueCcchCtxt, int *ueIdx)
{
	uint8_t cellIdx = 0;
	uint8_t ret     = ROK;
	DuCellCb duCellCb;

	memset(&duCellCb, 0, sizeof(DuCellCb));

   for(cellIdx = 0; cellIdx < DU_MAX_CELLS; cellIdx++)
	{
	   if(ueCcchCtxt->cellId == duCb.actvCellLst[cellIdx]->cellId)
		{
		   duCellCb = *(duCb.actvCellLst[cellIdx]);
			*ueIdx = fillUeIdx(&duCellCb.ueCb[cellIdx].ueIdx , &duCellCb.ueBitMap);
			if(*ueIdx >= 0 && *ueIdx < DU_MAX_UE)
			{
            DU_LOG("\nDU_APP: Filling UeCb for ueIdx [%d]", *ueIdx);

            duCellCb.ueCb[*ueIdx].ueState       = ACTIVE;
            duCellCb.ueCb[*ueIdx].crnti         = ueCcchCtxt->crnti;
            duCellCb.ueCb[*ueIdx].cellIdx       = ueCcchCtxt->cellId;
		      duCellCb.ueCb[*ueIdx].gnbDuUeF1apId = ueCcchCtxt->gnbDuUeF1apId;
		      duCellCb.ueCb[*ueIdx].gnbCuUeF1apId = ueCcchCtxt->gnbCuUeF1apId;
            /* Filling Mac Ue Config */ 
            memset(&duCellCb.ueCb[*ueIdx].macUeCfg, 0, sizeof(MacUeCfg));
            fillMacUeCfg(&duCellCb.ueCb[*ueIdx].macUeCfg);
		      duCellCb.numActvUes++;
				memset(ueCcchCtxt, 0, sizeof(UeCcchCtxt));
			}
			else
			{
			   DU_LOG("\nDU_APP: Failed while assigning Ue Idx[%d]", *ueIdx);
				ret = RFAILED;
			}
		}
		else
		{
		   DU_LOG("\nDU_APP: CellId not found in Active Cell List[%d]", ueCcchCtxt->cellId);
			ret = RFAILED;
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
 * @Params[in]  DuUeCb UeIdx Pointer
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t duBuildAndSendUeCreateReqToMac(DuUeCb *duUeCb)
{
   uint8_t ret = ROK;
   MacUeCfg *macUeCfg = NULLP;
   Pst pst;

   memset(&pst, 0, sizeof(MacUeCfg));

   /* Fill Pst */
   pst.selector  = DU_MAC_LWLC;
   pst.srcEnt    = ENTDUAPP;
   pst.srcInst   = DU_INST;
   pst.dstEnt    = ENTRG;
   pst.dstInst   = 0;
   pst.dstProcId = DU_PROC;
   pst.srcProcId = DU_PROC;
   pst.region    = DU_APP_MEM_REGION;
   pst.pool      = DU_POOL;
   pst.event     = EVENT_MAC_UE_CREATE_REQ;

   /* Copying ueCb to a sharable buffer */
   DU_ALLOC_SHRABL_BUF(macUeCfg, sizeof(MacUeCfg));
	memset(macUeCfg, 0, sizeof(MacUeCfg));
	if(macUeCfg)
	{
      memcpy(macUeCfg, &duUeCb->macUeCfg, sizeof(MacUeCfg));
      /* Processing one Ue at a time to MAC */
      ret = packDuMacUeCreateReq(&pst, macUeCfg);
		if(ret)
		{
         DU_LOG("\nDU_APP : Failure in sending Ue Create Req to MAC");
		   DU_FREE_SHRABL_BUF(DU_APP_MEM_REGION, DU_POOL, macUeCfg, sizeof(MacUeCfg));
			ret = RFAILED;
         return ret;
		}
	}
	else
	{
	   DU_LOG("\n DU_APP: Memory alloc failed at \
	     duBuildAndSendUeCreateReq()");
		ret = RFAILED;
	   return ret;
	}
}

/**********************************************************************
         End of file
***********************************************************************/
