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
uint8_t duBuildAndSendDlCcchInd(uint8_t *dlCcchMsg, uint16_t dlCcchMsgSize,\
   uint16_t crnti, uint16_t cellId, DlCcchMsgType msgType)
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

	dlCcchIndInfo->cellId = cellId;
	dlCcchIndInfo->crnti = crnti;
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
					UNUSED(gnbCuUeF1apId); //This is currently not used
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
				      DU_LOG("\nDU_APP : RRC Container Size is invalid:%d",\
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
		}
	}
	if(srbId == SRB_ID_1) //RRC connection setup
	{
		ret = duBuildAndSendDlCcchInd(dlCcchMsg, dlCcchMsgSize, crnti, cellId, RRC_SETUP);
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


/**********************************************************************
         End of file
***********************************************************************/
