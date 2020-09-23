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

DuRlcUlUeReconfigReq packRlcUlUeReconfigReqOpts[] =
{
   packDuRlcUlUeReconfigReq,       /* Loose coupling */
   RlcUlProcUeReconfigReq,       /* TIght coupling */
   packDuRlcUlUeReconfigReq,       /* Light weight-loose coupling */
};

DuMacUeReconfigReq packMacUeReconfigReqOpts[] =
{
   packDuMacUeReconfigReq,       /* Loose coupling */
   MacProcUeReconfigReq,       /* TIght coupling */
   packDuMacUeReconfigReq,     /* Light weight-loose coupling */
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
   uint8_t lcId, bool deliveryStaReq, uint16_t rrcMsgLen, uint8_t *rrcMsg)
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
   for(lcIdx = 0; lcIdx <= MAX_NUM_LOGICAL_CHANNELS; lcIdx++)
   {
      if(ueCfg.rlcBearerCfgToAdd[lcIdx].lcId == lcId)
      {
         dlRrcMsgInfo->rbType = ueCfg.rlcBearerCfgToAdd[lcIdx].rbType;
         dlRrcMsgInfo->rbId   = ueCfg.rlcBearerCfgToAdd[lcIdx].rbId;
	 dlRrcMsgInfo->lcType = ueCfg.rlcBearerCfgToAdd[lcIdx].lcType;
         dlRrcMsgInfo->lcId   = ueCfg.rlcBearerCfgToAdd[lcIdx].lcId;
	 break;
      }
   }
   dlRrcMsgInfo->execDup = false;
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

   /* Filling DRBQOS */
   if(ueSetReqDb->drbQos)
   {
      if(!lcCfg->drbQos)
      {
         DU_ALLOC_SHRABL_BUF(lcCfg->drbQos, sizeof(DrbQosInfo));
         if(!lcCfg->drbQos)
         {
            DU_LOG("\nDU APP: Memory Alloc failed at drQos at getLcCfgToAddMod()");
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
            DU_LOG("\nDU APP: Memory Alloc failed at snnsai at getLcCfgToAddMod()");
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
uint8_t getLcCfgToAddMod(LcCfg *duLcCfg, LcCfg *macLcCfg, LcCfg *ueSetReqDb)
{
   uint8_t ret = ROK;
   ret = fillLcCfg(duLcCfg, ueSetReqDb);
   if(!ret)
   {
      /* Filling Mac Lc Cfg To Add/Mod only if the above is successfull */
      ret = fillLcCfg(macLcCfg, ueSetReqDb);
   }
   if(ret)
   {
      DU_LOG("\nDU APP: Failed at getLcCfgToAddMod()");
      return ret;
   }
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
   MacUeCfg *macUeCfg, DuUeCfg *duUeCfg, F1UeSetupReq *f1UeSetReqDb)
{
   uint8_t ret, lcIdx, numLcs;
   bool ueDbFound = false;
   memset(macUeCfg, 0, sizeof(MacUeCfg));

   ret =ROK;
   macUeCfg->cellId       = cellId;
   macUeCfg->ueIdx        = ueIdx;
   macUeCfg->crnti        = crnti;

   if(f1UeSetReqDb)
   {
      ueDbFound = true;
      numLcs = f1UeSetReqDb->numMacLcs;
      ret = procUeReCfgCellInfo(macUeCfg, f1UeSetReqDb->cellGrpCfg);
      if(!ret)
      {
         ret = getMaxAggrBitRate(macUeCfg->maxAggrBitRate, f1UeSetReqDb->maxAggrBitRate);
      }
   }
   else
   {
      numLcs = SRB_ID_1;
      procUeReCfgCellInfo(macUeCfg, NULL);
      macUeCfg->maxAggrBitRate = NULLP;
   }

   /* Filling LC Context */
   for(lcIdx = 0; (lcIdx < numLcs && ret ==ROK); lcIdx++)
   {  
      if(!ueDbFound)
      {
         fillMacSrbCfg(SRB_ID_1, &macUeCfg->lcCfgToAddList[lcIdx], NULL);
         memcpy(&duUeCfg->macLcCfg[lcIdx], &macUeCfg->lcCfgToAddList[lcIdx], sizeof(LcCfg));
         macUeCfg->numLcsToAdd++;
	 duUeCfg->numMacLcs++;
      }
      else
      {
         if(f1UeSetReqDb->macLcCfgToAddMod[lcIdx].lcId ==\
            duUeCfg->macLcCfg[lcIdx].lcId)
         {
            /* Fill MAC Ue Cfg To Mod Lc List */
	    ret = getLcCfgToAddMod(&duUeCfg->macLcCfg[lcIdx],\
	       &macUeCfg->lcCfgToModList[macUeCfg->numLcsToMod], &f1UeSetReqDb->macLcCfgToAddMod[lcIdx]);
	    if(!ret)
	    {
               macUeCfg->numLcsToMod++;
	    }
	    else
	       break;
         }
         else
         {
            /* Fill MAC Ue Cfg To Add Lc List */
            ret = getLcCfgToAddMod(&duUeCfg->macLcCfg[duUeCfg->numMacLcs], \
	       &macUeCfg->lcCfgToAddList[macUeCfg->numLcsToAdd], &f1UeSetReqDb->macLcCfgToAddMod[lcIdx]);
	    if(!ret)
	    {
	       duUeCfg->numMacLcs++;
               macUeCfg->numLcsToAdd++;
	    }
	    else
	       break;
         }
      }
   }/*End of FOR Loop*/

   /* FREE UE DB */
   if(ueDbFound)
   {
      if(f1UeSetReqDb->maxAggrBitRate)
      {
         DU_FREE(f1UeSetReqDb->maxAggrBitRate, sizeof(MaxAggrBitRate));
         f1UeSetReqDb->maxAggrBitRate = NULLP;
      }
      freeDrbList(lcIdx, f1UeSetReqDb);
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
void fillRlcUeCfg(uint16_t cellId, uint8_t ueIdx, \
   RlcUeCfg *rlcUeCfg, DuUeCfg *duUeCfg, F1UeSetupReq *f1UeSetReqDb)
{
   uint8_t lcIdx, numLcs;
   bool ueDbFound = false;

   memset(rlcUeCfg, 0, sizeof(RlcUeCfg));

   rlcUeCfg->cellId       = cellId;
   rlcUeCfg->ueIdx        = ueIdx;

   if(f1UeSetReqDb)
   {
      numLcs = f1UeSetReqDb->numRlcLcs;
      ueDbFound = true;
   }     
   else
   {
      numLcs = SRB_ID_1;
   }
   for(lcIdx = 0; lcIdx < numLcs; lcIdx++)
   {
      if(!ueDbFound)
      {
         procRlcLcCfgToAddMod(SRB_ID_1, SRB_ID_1, RB_TYPE_SRB, RLC_AM, NULL, &rlcUeCfg->rlcBearerCfgToAdd[lcIdx]);
         memcpy(&duUeCfg->rlcLcCfg[duUeCfg->numRlcLcs], &rlcUeCfg->rlcBearerCfgToAdd[lcIdx], sizeof(RlcBearerCfg));
         rlcUeCfg->numLcsToAdd++;
         duUeCfg->numRlcLcs++;
      }
      else
      {
         if(f1UeSetReqDb->rlcLcCfgToAddMod[lcIdx].lcId ==\
         duUeCfg->rlcLcCfg[lcIdx].lcId)
         {
            /* Fill Rlc Ue Cfg To Mod Lc List */
            memcpy(&rlcUeCfg->rlcBearerCfgToMod[rlcUeCfg->numLcsToMod], \
               &f1UeSetReqDb->rlcLcCfgToAddMod[lcIdx], sizeof(RlcBearerCfg));
            memcpy(&duUeCfg->rlcLcCfg[lcIdx], \
               &f1UeSetReqDb->rlcLcCfgToAddMod[lcIdx], sizeof(RlcBearerCfg));
            rlcUeCfg->numLcsToMod++;
         }
         else
         {
            /* Fill Rlc Ue Cfg To Add Lc List */
            memcpy(&rlcUeCfg->rlcBearerCfgToAdd[rlcUeCfg->numLcsToAdd], \
               &f1UeSetReqDb->rlcLcCfgToAddMod[lcIdx], sizeof(RlcBearerCfg));
            memcpy(&duUeCfg->rlcLcCfg[duUeCfg->numRlcLcs], \
               &f1UeSetReqDb->rlcLcCfgToAddMod[lcIdx], sizeof(RlcBearerCfg));
            rlcUeCfg->numLcsToAdd++;
            duUeCfg->numRlcLcs++;
         }
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

	 duCb.actvCellLst[cellIdx]->ueCb[ueIdx-1].crnti = ueCcchCtxt->crnti;
	 duCb.actvCellLst[cellIdx]->ueCb[ueIdx-1].gnbDuUeF1apId = ueCcchCtxt->gnbDuUeF1apId;
	 duCb.actvCellLst[cellIdx]->ueCb[ueIdx-1].gnbCuUeF1apId = gnbCuUeF1apId;
	 duCb.actvCellLst[cellIdx]->ueCb[ueIdx-1].ueState       = UE_ACTIVE;
	 duCb.actvCellLst[cellIdx]->ueCb[ueIdx-1].cellGrpCfg = NULLP;
	 /* Filling Du Ue LC Config*/
	 memset(&duCb.actvCellLst[cellIdx]->ueCb[ueIdx-1].duUeCfg, 0, sizeof(DuUeCfg));

	 /* Filling Mac Ue Config */ 
	 memset(&duCb.actvCellLst[cellIdx]->ueCb[ueIdx-1].macUeCfg, 0, sizeof(MacUeCfg));
         ret = duBuildAndSendUeCreateReqToMac(ueCcchCtxt->cellId, ueIdx, ueCcchCtxt->crnti,\
                &duCb.actvCellLst[cellIdx]->ueCb[ueIdx-1].macUeCfg,\
		&duCb.actvCellLst[cellIdx]->ueCb[ueIdx-1].duUeCfg);
         if(ret)
            DU_LOG("\nDU_APP: Failed to send UE create request to MAC");
         
	 /* Filling Rlc Ue Config */
         memset(&duCb.actvCellLst[cellIdx]->ueCb[ueIdx-1].rlcUeCfg, 0, sizeof(RlcUeCfg));
         ret = duBuildAndSendUeCreateReqToRlc(ueCcchCtxt->cellId, ueIdx, \
                &duCb.actvCellLst[cellIdx]->ueCb[ueIdx-1].rlcUeCfg,\
                &duCb.actvCellLst[cellIdx]->ueCb[ueIdx-1].duUeCfg);
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
   uint16_t crnti, MacUeCfg *duMacUeCfg, DuUeCfg *duUeCfg)
{
   uint8_t ret = ROK;
   Pst pst;
   MacUeCfg *macUeCfg = NULLP;

   ret = fillMacUeCfg(cellId, ueIdx, crnti, duMacUeCfg, duUeCfg, NULL);
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
   if((duCb.actvCellLst[cellIdx]->ueCb[ueIdx].\
         macUeCfgState == UE_RECFG_COMPLETED) &&
      (duCb.actvCellLst[cellIdx]->ueCb[ueIdx].\
         rlcUeCfgState == UE_RECFG_COMPLETED))
   {
      DU_LOG("\nDU_APP: Processing UE Context Setup Response\n");
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
   RlcUeCfg *duRlcUeCfg, DuUeCfg *duUeCfg)
{
   uint8_t ret = ROK;
   RlcUeCfg *rlcUeCfg = NULLP;
   Pst pst;
  
   fillRlcUeCfg(cellId, ueIdx, duRlcUeCfg, duUeCfg, NULL);
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
   RlcUeCfg *ueCfg = NULLP;

   FILL_PST_DUAPP_TO_RLC(pst, RLC_UL_INST, EVENT_RLC_UL_UE_RECONFIG_REQ);

   /* Copying ueCfg to a sharable buffer */
   DU_ALLOC_SHRABL_BUF(ueCfg, sizeof(RlcUeCfg));
   if(ueCfg)
   {
      memset(ueCfg, 0, sizeof(RlcUeCfg));
      memcpy(ueCfg, rlcUeCfg, sizeof(RlcUeCfg));

      /* Processing one Ue at a time to RLC */
      DU_LOG("\nDU_APP: Sending Reconfig Request to RLC UL");
      ret = (*packRlcUlUeReconfigReqOpts[pst.selector])(&pst, ueCfg);
      if(ret)
      {
         DU_LOG("\nDU_APP : Failure in sending Ue Reconfig Req to RLC");
         DU_FREE_SHRABL_BUF(DU_APP_MEM_REGION, DU_POOL, ueCfg, sizeof(RlcUeCfg));
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
   MacUeCfg *ueCfg = NULLP;
   
   /* Fill Pst */
   FILL_PST_DUAPP_TO_MAC(pst, EVENT_MAC_UE_RECONFIG_REQ);
   
   /* Copying ueCfg to a sharable buffer */
   DU_ALLOC_SHRABL_BUF(ueCfg, sizeof(MacUeCfg));
   if(ueCfg)
   {
      memset(ueCfg, 0, sizeof(MacUeCfg));
      memcpy(ueCfg, macUeCfg, sizeof(MacUeCfg));

      /* Processing one Ue at a time to MAC */
      DU_LOG("\nDU_APP: Sending Reconfig Request to MAC");
      ret = (*packMacUeReconfigReqOpts[pst.selector])(&pst, ueCfg);
      if(ret)
      {
	 DU_LOG("\nDU_APP : Failure in sending Ue ReConfig Req to MAC");
	 DU_FREE_SHRABL_BUF(DU_APP_MEM_REGION, DU_POOL, ueCfg, sizeof(MacUeCfg));
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
 * @brief Builds and Send Ue Reconfig Req to RLC
 *
 * @details
 *
 *    Function : duBuildAndSendUeReCfgReqToRLC
 *
 *    Functionality: Builds and Send Ue Reconfig Req to RLC
 *
 * @params[in] DuUeCfg *duUeCfg
 *             RlcUeCfg *rlcUeCfg
 *             F1UeSetupReq *f1UeSetReqDb
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t duBuildAndSendUeReCfgReqToRlc(uint8_t cellId, uint8_t crnti, DuUeCfg *duUeCfg, \
   RlcUeCfg *rlcUeCfg, F1UeSetupReq *f1UeSetReqDb)
{
   uint8_t ret = ROK, ueIdx = 0;
   memset(rlcUeCfg, 0, sizeof(RlcUeCfg));
   GET_UE_IDX(crnti, ueIdx);

   fillRlcUeCfg(cellId, ueIdx, rlcUeCfg, duUeCfg, f1UeSetReqDb);
   ret = sendUeReCfgReqToRlc(rlcUeCfg);
   if(ret)
      DU_LOG("\nDU APP: Failed at sendUeReCfgReqToRlc()");

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
 *             MacUeCfg    *macUeCfg
 *             F1UeSetupReq *f1UeSetReqDb
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t duBuildAndSendUeReCfgReqToMac(uint8_t cellId, uint8_t crnti, DuUeCfg *duUeCfg, \
   MacUeCfg *macUeCfg, F1UeSetupReq *f1UeSetReqDb)
{
   uint8_t ret, ueIdx = 0;
   memset(macUeCfg, 0, sizeof(MacUeCfg));

   ret = ROK;
   GET_UE_IDX(crnti, ueIdx);

   ret = fillMacUeCfg(cellId, ueIdx, crnti, macUeCfg, duUeCfg, f1UeSetReqDb);
   if(!ret)
   {
      ret = sendUeReCfgReqToMac(macUeCfg);
      if(ret)
         DU_LOG("\nDU APP: Failed at sendUeReCfgReqToMac()");
   }
   else
   {
      DU_LOG("\nDU_APP: Failed while filling Mac Ue Cfg in duBuildAndSendUeReCfgReqToMac()"); 
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

uint8_t duProcUeSetupRequest(F1UeSetupReq *ueSetupReqDb)
{
   uint8_t ret, ueIdx, cellId;
   DuUeCb     *ueCb = NULLP;

   ret = ROK;
   if(ueSetupReqDb)
   {
      cellId = duCb.actvCellLst[ueSetupReqDb->cellIdx]->cellId;
      for(ueIdx = 0; ueIdx < MAX_NUM_UE; ueIdx++)
      {
         if((duCb.actvCellLst[ueSetupReqDb->cellIdx]->ueCb[ueIdx].gnbCuUeF1apId == ueSetupReqDb->gnbCuUeF1apId) &&
          (duCb.actvCellLst[ueSetupReqDb->cellIdx]->ueCb[ueIdx].gnbDuUeF1apId == ueSetupReqDb->gnbDuUeF1apId &&
           duCb.actvCellLst[ueSetupReqDb->cellIdx]->ueCb[ueIdx].ueState == UE_ACTIVE))
         {
            ueCb = &duCb.actvCellLst[ueSetupReqDb->cellIdx]->ueCb[ueIdx];
            break;
         }
      }

      if(ueCb)
      {
         DU_LOG("\nDU_APP: Processing Ue Context Setup Request for cellId [%d]", cellId);
         /* Filling RLC Ue Reconfig */ 
         ret = duBuildAndSendUeReCfgReqToRlc(cellId, ueCb->crnti, &ueCb->duUeCfg, &ueCb->rlcUeCfg, ueSetupReqDb);
         if(ret)
         {
            DU_LOG("\nDU_APP: Failed at duBuildAndSendUeReCfgReqToRlc for cellId [%d]", cellId);
         }
         /* Filling MAC Ue Reconfig */
         ret = duBuildAndSendUeReCfgReqToMac(cellId, ueCb->crnti, &ueCb->duUeCfg, &ueCb->macUeCfg, ueSetupReqDb);
         if(ret)
         {
            DU_LOG("\nDU_APP: Failed at duBuildAndSendUeReCfgReqToMac for cellId [%d]", cellId);
         }
	 else
	 {
            /*Store Cell Grp Msg in Uecb*/
            /*Used to build Ue Setup Rsp*/
            if(ueSetupReqDb->cellGrpCfg)
	    {
               getCellGrpCfg(ueCb->cellGrpCfg, ueSetupReqDb->cellGrpCfg);
	    }

	 }
         /* Sending DL RRC Message to RLC */
         if(ueSetupReqDb->rrcMsg)
         {
            ret = duBuildAndSendDlRrcMsgToRlc(cellId, ueCb->rlcUeCfg, SRB_ID_1, \
               ueSetupReqDb->deliveryStaReq,  ueSetupReqDb->rrcMsgLen, ueSetupReqDb->rrcMsg);
            if(ret)
            {
               DU_LOG("\nDU APP : No UE found for CuUeF1apId[%d] and DuUeF1apId[%d]", \
                  ueSetupReqDb->gnbCuUeF1apId, ueSetupReqDb->gnbDuUeF1apId);
               DU_FREE_SHRABL_BUF(DU_APP_MEM_REGION, DU_POOL, ueSetupReqDb->rrcMsg, ueSetupReqDb->rrcMsgLen);
            }
         }
      }
      else
      {
         DU_LOG("\nDU APP : No UE found for CuUeF1apId[%d] and DuUeF1apId[%d]", \
          ueSetupReqDb->gnbCuUeF1apId, ueSetupReqDb->gnbDuUeF1apId);
         DU_FREE_SHRABL_BUF(DU_APP_MEM_REGION, DU_POOL, ueSetupReqDb->rrcMsg, ueSetupReqDb->rrcMsgLen);
         ret = RFAILED;
      }
   }
   return ret;
}

/**********************************************************************
  End of file
 ***********************************************************************/
