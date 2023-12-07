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
#include "du_tmr.h"
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
#include "du_e2ap_mgr.h"
#include "du_cfg.h"
#include "du_mgr.h"
#include "du_utils.h"
#include "du_f1ap_msg_hdl.h"
#include "du_ue_mgr.h"

#ifdef O1_ENABLE
#include "AlarmInterface.h"
#include "CmInterface.h"
#endif

DuRlcUeReestablishReq packRlcUeReestablishReqOpts[] =
{
   packDuRlcUeReestablishReq,       /* Loose coupling */
   RlcProcUeReestablishReq,         /* TIght coupling */
   packDuRlcUeReestablishReq        /* Light weight-loose coupling */
};

DuMacDlCcchInd packMacDlCcchIndOpts[] =
{
   packMacDlCcchInd,           /* Loose coupling */
   MacProcDlCcchInd,           /* TIght coupling */
   packMacDlCcchInd            /* Light weight-loose coupling */
};

DuMacUeCreateReq packMacUeCreateReqOpts[] =
{
   packDuMacUeCreateReq,       /* Loose coupling */
   MacProcUeCreateReq,         /* TIght coupling */
   packDuMacUeCreateReq        /* Light weight-loose coupling */
};

DuRlcUeCreateReq packRlcUeCreateReqOpts[] =
{
   packDuRlcUeCreateReq,       /* Loose coupling */
   RlcProcUeCreateReq,         /* TIght coupling */
   packDuRlcUeCreateReq        /* Light weight-loose coupling */
};

DuDlRrcMsgToRlcFunc duSendDlRrcMsgToRlcOpts[] =
{
   packDlRrcMsgToRlc,          /* Loose coupling */ 
   RlcProcDlRrcMsgTransfer,    /* Tight coupling */
   packDlRrcMsgToRlc           /* Light weight-loose coupling */
};

DuRlcUeReconfigReq packRlcUeReconfigReqOpts[] =
{
   packDuRlcUeReconfigReq,     /* Loose coupling */
   RlcProcUeReconfigReq,       /* TIght coupling */
   packDuRlcUeReconfigReq      /* Light weight-loose coupling */
};

DuMacUeReconfigReq packMacUeReconfigReqOpts[] =
{
   packDuMacUeReconfigReq,     /* Loose coupling */
   MacProcUeReconfigReq,       /* TIght coupling */
   packDuMacUeReconfigReq      /* Light weight-loose coupling */
};

DuMacRachRsrcReq packMacRachRsrcReqOpts[] = 
{
   packDuMacRachRsrcReq,      /* Loose coupling */
   MacProcRachRsrcReq,        /* Tight coupling */
   packDuMacRachRsrcReq       /* Light weight-loose coupling */
};

DuRlcDlUserDataToRlcFunc duSendRlcDlUserDataToRlcOpts[] =
{
   packRlcDlUserDataToRlc,     /* Loose coupling */ 
   RlcProcDlUserDataTransfer,  /* Tight coupling */
   packRlcDlUserDataToRlc      /* Light weight-loose coupling */
};

DuMacRachRsrcRel packMacRachRsrcRelOpts[] = 
{
   packDuMacRachRsrcRel,      /* Loose coupling */
   MacProcRachRsrcRel,        /* Tight coupling */
   packDuMacRachRsrcRel       /* Light weight-loose coupling */
};

DuMacUeDeleteReq packMacUeDeleteReqOpts[] =
{
   packDuMacUeDeleteReq,       /* Loose coupling */
   MacProcUeDeleteReq,         /* TIght coupling */
   packDuMacUeDeleteReq        /* Light weight-loose coupling */
};

DuRlcUeDeleteReq packRlcUeDeleteReqOpts[] =
{
   packDuRlcUeDeleteReq,       /* Loose coupling */
   RlcProcUeDeleteReq,         /* TIght coupling */
   packDuRlcUeDeleteReq        /* Light weight-loose coupling */
};

DuMacUeResetReq packMacUeResetReqOpts[] =
{
   packDuMacUeResetReq,       /* Loose coupling */
   MacProcUeResetReq,         /* TIght coupling */
   packDuMacUeResetReq        /* Light weight-loose coupling */
};

/*******************************************************************
 *
 * @brief Processes UE's max retransmission information received from RLC 
 *
 * @details
 *
 *    Function : DuProcRlcMaxRetransInd
 *
 *    Functionality:
 *     Processes max retransmission reached information received from RLC 
 *
 *  @params[in]  Post structure
 *               Pointer to RlcMaxRetransInfo
 *  @return ROK     - success
 *          RFAILED - failure
 *
 * *****************************************************************/

uint8_t DuProcRlcMaxRetransInd(Pst *pst, RlcMaxRetransInfo *maxRetransInfo)
{
   uint8_t  ueId = 0, ret = RFAILED;
   uint16_t cellIdx = 0,crnti=0;

   if(maxRetransInfo)
   {
      GET_CELL_IDX(maxRetransInfo->cellId, cellIdx);

      if(duCb.actvCellLst[cellIdx]!=NULLP)
      {
         ueId = maxRetransInfo->ueId;
         GET_CRNTI(crnti, ueId);
         if(duCb.actvCellLst[cellIdx]->ueCb[ueId-1].crnti ==  crnti)
         {
            /*TODO: complete the processing of max retransmission */
            ret = ROK;
         }
         else
            DU_LOG("\nERROR  -->  DU APP : DuProcRlcMaxRetransInd(): CRNTI [%d] not found", crnti);
      }
      else
         DU_LOG("\nERROR  -->  DU APP : DuProcRlcMaxRetransInd(): Cell Id[%d] is not found", maxRetransInfo->cellId);
      
      DU_FREE_SHRABL_BUF(pst->region, pst->pool, maxRetransInfo, sizeof(RlcMaxRetransInfo));

   }
   return ret;
}

/******************************************************************
 *
 * @brief Function to return Drb LcId
 *
 * @details
 *
 *    Function : getDrbLcId
 *
 *    Functionality: Function to return Drb LcId
 *
 * @params[in] drbBitMap
 *
 * Returns: lcId - SUCCESS
 *         RFAILED - FAILURE
 *****************************************************************/

uint8_t getDrbLcId(uint32_t *drbBitMap)
{
   uint8_t bitMask = 1, bitPos = 0;
   bitPos = MIN_DRB_LCID;

   while(bitPos <= MAX_DRB_LCID)
   {
      if((*drbBitMap & (bitMask << bitPos)) == 0)
      {
         *drbBitMap = ((bitMask << bitPos)| *drbBitMap);
         return bitPos;
      }
      else
      {
         bitPos++;
      }
   }
   DU_LOG("\nERROR   -->  DU_APP: Max LC Reached in getDrbLcId()");
   return RFAILED;
}

/*******************************************************************
 *
 * @brief Function to fillDlUserDataInfo
 *
 * @details
 *
 *    Function : fillDlUserDataInfo
 *
 *    Functionality:
 *      Function to fillDlUserDataInfo
 *
 * @params[in] teId,
 *             dlDataMsgInfo
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t fillDlUserDataInfo(uint32_t teId, RlcDlUserDataInfo *dlDataMsgInfo)
{
   uint8_t teIdx = 0;

   /*Traversing the duCb to find the CellId, UeId, LCID corresponding to TEID*/
   for(teIdx = 0; teIdx < duCb.numTeId; teIdx++)
   {
      if(duCb.upTnlCfg[teIdx] && (duCb.upTnlCfg[teIdx]->tnlCfg1 != NULLP))
      {
         if(duCb.upTnlCfg[teIdx]->tnlCfg1->teId == teId)
         {
            dlDataMsgInfo->cellId = duCb.upTnlCfg[teIdx]->cellId;
            dlDataMsgInfo->ueId = duCb.upTnlCfg[teIdx]->ueId;
            dlDataMsgInfo->rbId = duCb.upTnlCfg[teIdx]->drbId;
            return ROK;
         }
      }
   }
   return RFAILED;
}

 /*******************************************************************
 *
 * @brief Build and Send DL Data Message transfer to RLC
 *
 * @details
 *
 *    Function : duBuildAndSendDlUserDataToRlc
 *
 *    Functionality:
 *      Build and Send DL Data Message transfer to RLC
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

uint8_t duBuildAndSendDlUserDataToRlc(uint16_t msgLen, EgtpMsg *egtpMsg)
{
   uint8_t ret = RFAILED;
   Pst     pst;
   RlcDlUserDataInfo *dlDataMsgInfo = NULLP;

   DU_ALLOC_SHRABL_BUF(dlDataMsgInfo, sizeof(RlcDlUserDataInfo));
   if(!dlDataMsgInfo)
   {
      DU_LOG("\nERROR  -->  DU_APP : Memory allocation failed for dlDataMsgInfo in duHdlEgtpDlData()");
      return RFAILED;
   }
   memset(dlDataMsgInfo, 0, sizeof(RlcDlUserDataInfo));
   dlDataMsgInfo->dlMsg = egtpMsg->msg;
   dlDataMsgInfo->msgLen = msgLen;

   /* Filling DL DATA Msg Info */
   if(fillDlUserDataInfo(egtpMsg->msgHdr.teId, dlDataMsgInfo) == ROK)
   {
      /* Filling post structure and sending msg */ 
      FILL_PST_DUAPP_TO_RLC(pst, RLC_DL_INST, EVENT_DL_USER_DATA_TRANS_TO_RLC);
      DU_LOG("\nDEBUG  -->  DU_APP : Sending User Data Msg to RLC [TEID, nPDU]:[%d, %d]\n",\
                       egtpMsg->msgHdr.teId, egtpMsg->msgHdr.nPdu.val);
      ret = (*duSendRlcDlUserDataToRlcOpts[pst.selector])(&pst, dlDataMsgInfo);
   }
   if(ret != ROK)
   {
      DU_LOG("\nERROR  -->  DU_APP : Failed to send User Data to RLC in duHdlEgtpDlData()");
      ODU_PUT_MSG_BUF(dlDataMsgInfo->dlMsg);
      DU_FREE_SHRABL_BUF(DU_APP_MEM_REGION, DU_POOL, dlDataMsgInfo, sizeof(RlcDlUserDataInfo));
   }
   return ret;
}

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
   uint16_t msgLen = 0;

#ifdef CALL_FLOW_DEBUG_LOG
    DU_LOG("\nCall Flow: ENTEGTP -> ENTDUAPP : EVENT_HDL_RECV_DL_DATA\n");
#endif

   DU_LOG("\nDEBUG  -->  DU_APP : Processing DL data in duHdlEgtpDlData()");
   
   if(!egtpMsg->msg)
   {
      DU_LOG("\nERROR  -->  DU_APP : Recevied Dl Data is NULLP in duHdlEgtpDlData()");
      return RFAILED;
   }
   ODU_GET_MSG_LEN(egtpMsg->msg, (MsgLen *)&msgLen);
   if(duBuildAndSendDlUserDataToRlc(msgLen, egtpMsg) != ROK)
   {
      DU_LOG("\nERROR  -->  DU_APP : Failed to build DL USer Data in duHdlEgtpDlData()");
      return RFAILED;
   }
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
   uint8_t  ret                  = ROK;
   uint16_t idx2;
   DlCcchIndInfo *dlCcchIndInfo = NULLP;
   Pst pst;

   DU_LOG("\nDEBUG   -->  DU APP : Building and Sending DL CCCH Ind to MAC");

   DU_ALLOC_SHRABL_BUF(dlCcchIndInfo, sizeof(DlCcchIndInfo));

   if(!dlCcchIndInfo)
   {
      DU_LOG("\nERROR  -->  DU APP : Memory alloc failed while building DL CCCH Ind");
      return RFAILED;
   }

   dlCcchIndInfo->cellId = *cellId;
   dlCcchIndInfo->crnti = *crnti;
   dlCcchIndInfo->msgType = msgType;
   dlCcchIndInfo->dlCcchMsgLen = dlCcchMsgSize;

   DU_ALLOC_SHRABL_BUF(dlCcchIndInfo->dlCcchMsg, dlCcchIndInfo->dlCcchMsgLen);
   if(!dlCcchIndInfo->dlCcchMsg)
   {
      DU_LOG("\nERROR  -->  DU APP : Memory alloc failed while building DL CCCH Ind");
      DU_FREE_SHRABL_BUF(DU_APP_MEM_REGION, DU_POOL, dlCcchIndInfo, sizeof(DlCcchIndInfo));
      return RFAILED;
   }
   for(idx2 = 0; idx2 < dlCcchIndInfo->dlCcchMsgLen; idx2++)
   {
      dlCcchIndInfo->dlCcchMsg[idx2] = dlCcchMsg[idx2];
   }
   DU_FREE_SHRABL_BUF(DU_APP_MEM_REGION, DU_POOL, dlCcchMsg, dlCcchMsgSize);

   /* Fill Pst */
   FILL_PST_DUAPP_TO_MAC(pst, EVENT_MAC_DL_CCCH_IND);
   ret = (*packMacDlCcchIndOpts[pst.selector])(&pst, dlCcchIndInfo);
   if(ret != ROK)
   {
      DU_LOG("\nERROR  -->  DU_APP : Failure in sending DL CCCH to MAC");
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
uint8_t duBuildAndSendDlRrcMsgToRlc(uint16_t cellId, DuRlcUeCfg ueCfg, F1DlRrcMsg *f1DlRrcMsg)
{
   Pst     pst;
   uint8_t ret;
   uint8_t lcIdx;
   RlcDlRrcMsgInfo  *dlRrcMsgInfo = NULLP;

   if(!f1DlRrcMsg)
   {
      DU_LOG("\nERROR  -->  DU APP : Received Dl RRC Msg is NULL at duBuildAndSendDlRrcMsgToRlc()");
      return RFAILED;
   }

   /*As per Spec ORAN WG8 AAD, lcId for DL RRC range from 1...3*/
   if((f1DlRrcMsg->srbId < SRB1_LCID) || (f1DlRrcMsg->srbId > SRB3_LCID))
   {
      DU_LOG("\nERROR  -->  DU APP : Received SRBID for this Dl RRC Msg is not valid");
      return RFAILED;
   }

   DU_ALLOC_SHRABL_BUF(dlRrcMsgInfo, sizeof(RlcDlRrcMsgInfo));
  
   if(!dlRrcMsgInfo)
   {
      DU_LOG("\nERROR  -->  DU APP : Memory allocation failed for dlRrcMsgInfo in \
            duBuildAndSendDlRrcMsgToRlc");
      DU_FREE_SHRABL_BUF(DU_APP_MEM_REGION, DU_POOL, f1DlRrcMsg->rrcMsgPdu, f1DlRrcMsg->rrcMsgSize);
      return RFAILED;
   }

   /* Filling up the RRC msg info */
   dlRrcMsgInfo->cellId = cellId;
   dlRrcMsgInfo->ueId = ueCfg.ueId;
   for(lcIdx = 0; lcIdx <= MAX_NUM_LC; lcIdx++)
   {
      if(ueCfg.rlcLcCfg[lcIdx].rlcBearerCfg.lcId == f1DlRrcMsg->srbId)
      {
         dlRrcMsgInfo->lcId   = f1DlRrcMsg->srbId;
         break;
      }
   }
   if(lcIdx == (MAX_NUM_LC + 1))
   {
      DU_LOG("\nERROR  -->  DU APP : (duBuildAndSendDlRrcMsgToRlc) SRB for this DL_RRC msg is not configured.");
      DU_FREE_SHRABL_BUF(DU_APP_MEM_REGION, DU_POOL, f1DlRrcMsg->rrcMsgPdu, f1DlRrcMsg->rrcMsgSize);
      return RFAILED;
   }

   dlRrcMsgInfo->execDup = f1DlRrcMsg->execDup;
   dlRrcMsgInfo->deliveryStaRpt = f1DlRrcMsg->deliveryStatRpt;
   dlRrcMsgInfo->msgLen = f1DlRrcMsg->rrcMsgSize;
   dlRrcMsgInfo->rrcMsg = f1DlRrcMsg->rrcMsgPdu;

   /* Filling post structure and sending msg */ 
   FILL_PST_DUAPP_TO_RLC(pst, RLC_DL_INST, EVENT_DL_RRC_MSG_TRANS_TO_RLC);
   DU_LOG("\nDEBUG  -->  DU_APP: Sending Dl RRC Msg to RLC \n");
   ret = (*duSendDlRrcMsgToRlcOpts[pst.selector])(&pst, dlRrcMsgInfo);
   if(ret != ROK)
   {
      DU_FREE_SHRABL_BUF(DU_APP_MEM_REGION, DU_POOL, f1DlRrcMsg->rrcMsgPdu, f1DlRrcMsg->rrcMsgSize);
      DU_FREE_SHRABL_BUF(DU_APP_MEM_REGION, DU_POOL, dlRrcMsgInfo, sizeof(RlcDlRrcMsgInfo));
      return RFAILED;
   }

   return ROK;
} 



/******************************************************************
 *
 * @brief Process DL RRC Msg recevied from F1AP
 *
 * @details
 *
 *    Function : duProcDlRrcMsg
 *
 *    Functionality: Process DL RRC Msg recevied from F1AP
 *
 * @params[in] dlCcchMsg - uint8_t*
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t duProcDlRrcMsg(F1DlRrcMsg *dlRrcMsg)
{
   uint8_t ueIdx, ret;
   uint16_t crnti, cellId, cellIdx;
   bool ueCcchCtxtFound = false;
   bool ueFound = false;

   ret = ROK;

   if(dlRrcMsg->srbId == SRB0_LCID) //RRC connection setup
   {
      for(ueIdx=0; ueIdx<duCb.numUe; ueIdx++)
      {
         if(dlRrcMsg->gnbDuUeF1apId == duCb.ueCcchCtxt[ueIdx].gnbDuUeF1apId)
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
      ret = duBuildAndSendDlCcchInd(&cellId, &crnti, RRC_SETUP, dlRrcMsg->rrcMsgSize, dlRrcMsg->rrcMsgPdu);
      if(ret == RFAILED)
      {
         DU_LOG("\nERROR  -->  DU APP : Failed to build DlCcch Ind at procDlRrcMsgTrans()");
      }
      else
      {
         if(duCb.actvCellLst[cellId-1] == NULLP)
         {
            DU_LOG("\nERROR  -->  DU APP : cellId [%d] does not exist", cellId);
            ret = RFAILED;
         }
         if(duCb.actvCellLst[cellId-1]->numActvUes < MAX_NUM_UE)
         {
            ret = duCreateUeCb(&duCb.ueCcchCtxt[ueIdx], dlRrcMsg->gnbCuUeF1apId);
            if(ret == RFAILED)
            {
               DU_LOG("\nERROR  -->  DU APP : Failed to createUeCb for cellId [%d] at procDlRrcMsgTrans()", \
                     duCb.ueCcchCtxt[ueIdx].cellId);
            }
         }
         else
         {
            DU_LOG("\nERROR   -->  DU_APP: Max Active UEs has reached at procDlRrcMsgTrans()");
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
            if(duCb.actvCellLst[cellIdx] && (dlRrcMsg->gnbCuUeF1apId == duCb.actvCellLst[cellIdx]->ueCb[ueIdx].gnbCuUeF1apId)
                  && (dlRrcMsg->gnbDuUeF1apId == duCb.actvCellLst[cellIdx]->ueCb[ueIdx].gnbDuUeF1apId))
            {
               ueFound = true;
               ret = duBuildAndSendDlRrcMsgToRlc(duCb.actvCellLst[cellIdx]->cellId, \
                     duCb.actvCellLst[cellIdx]->ueCb[ueIdx].duRlcUeCfg, dlRrcMsg);
               if(ret == RFAILED)
               {
                  DU_LOG("\nERROR   -->  DU_APP: duBuildAndSendDlRrcMsgToRlc() Failed for UE ID:%d", dlRrcMsg->gnbDuUeF1apId);
                  return RFAILED;
               }
               break; 
            }
         }
         if(ueFound)
            break;
      }
      if(!ueFound)
      {
         DU_LOG("\nERROR   -->  DU_APP: UE ID [%d] not found", dlRrcMsg->gnbDuUeF1apId);
         ret = RFAILED;
      }
   }
   return ret;
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
   int32_t gnbDuUeF1apId = 0;

   if(ulCcchIndInfo->crnti)
   {
      GET_UE_ID(ulCcchIndInfo->crnti, gnbDuUeF1apId);
   }
   else
   {
      DU_LOG("\nERROR  -->  DU_APP : Received invalid CRNTI [%d] ", ulCcchIndInfo->crnti);
      return RFAILED;
   }
   
   /* Store Ue mapping */
   duCb.ueCcchCtxt[duCb.numUe].gnbDuUeF1apId = (uint32_t)gnbDuUeF1apId;
   duCb.ueCcchCtxt[duCb.numUe].crnti         = ulCcchIndInfo->crnti;
   duCb.ueCcchCtxt[duCb.numUe].cellId        = ulCcchIndInfo->cellId;

   duCb.numUe++;

   ret = (BuildAndSendInitialRrcMsgTransfer(gnbDuUeF1apId, ulCcchIndInfo->crnti, ulCcchIndInfo->ulCcchMsgLen,
	    ulCcchIndInfo->ulCcchMsg));
   if(ret != ROK)
   {
      DU_LOG("\nERROR  -->  DU_APP : BuildAndSendInitialRrcMsgTransfer failed");
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
 *    Function : fillDefaultUlLcCfg
 *
 *    Functionality: Fills Default UL LC Cfg
 *
 * @params[in]  UlLcCfg *ulLcCfg 
 * @return void
 *****************************************************************/
void fillDefaultUlLcCfg(UlLcCfg *ulLcCfg)
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
 *    Function : fillDefaultInitDlBwp
 *
 *    Functionality: Fills Initial DL Bandwidth Part
 *
 * @params[in]  InitialDlBwp *initDlBwp
 * @return ROK - success
 *         RFAILED - failure
 *
 *****************************************************************/
uint8_t fillDefaultInitDlBwp(InitialDlBwp *initDlBwp)
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
            initDlBwp->pdcchCfg.cRSetToAddModList[idx].cRSetId = PDCCH_CTRL_RSRC_SET_ONE_ID;
            memset(initDlBwp->pdcchCfg.cRSetToAddModList[idx].freqDomainRsrc, 0, FREQ_DOM_RSRC_SIZE); 
            coreset0EndPrb = CORESET0_END_PRB;
            coreset1StartPrb = coreset0EndPrb +6;
            coreset1NumPrb = CORESET1_NUM_PRB;
            /* calculate the PRBs */
            fillCoresetFeqDomAllocMap(((coreset1StartPrb)/6), (coreset1NumPrb/6), freqDomainResource);
            memcpy(initDlBwp->pdcchCfg.cRSetToAddModList[idx].freqDomainRsrc, freqDomainResource, FREQ_DOM_RSRC_SIZE);

            initDlBwp->pdcchCfg.cRSetToAddModList[idx].duration = PDCCH_CTRL_RSRC_SET_ONE_DURATION;
            initDlBwp->pdcchCfg.cRSetToAddModList[idx].cceRegMappingType = CCE_REG_MAPPINGTYPE_PR_NONINTERLEAVED;
            initDlBwp->pdcchCfg.cRSetToAddModList[idx].precoderGranularity = ALL_CONTIGUOUS_RBS;
            initDlBwp->pdcchCfg.cRSetToAddModList[idx].dmrsScramblingId = SCRAMBLING_ID;
         }
         initDlBwp->pdcchCfg.numCRsetToRel = 0;

         /* Filling Serach Space */
         initDlBwp->pdcchCfg.numSearchSpcToAddMod = PDCCH_CTRL_RSRC_SET_ONE_ID;
         memset(initDlBwp->pdcchCfg.searchSpcToAddModList, 0, MAX_NUM_CRSET);
         if(initDlBwp->pdcchCfg.numSearchSpcToAddMod <= MAX_NUM_CRSET)
         {
            initDlBwp->pdcchCfg.searchSpcToAddModList[idx].searchSpaceId = PDCCH_SRCH_SPC_TWO_ID;
            initDlBwp->pdcchCfg.searchSpcToAddModList[idx].cRSetId = PDCCH_CTRL_RSRC_SET_ONE_ID;
            initDlBwp->pdcchCfg.searchSpcToAddModList[idx].mSlotPeriodicityAndOffset.mSlotPeriodicity\
                                                                   = SLOTPERIODICITY_PR_SL1;
            initDlBwp->pdcchCfg.searchSpcToAddModList[idx].mSlotPeriodicityAndOffset.mSlotOffset = 0;
            memset(initDlBwp->pdcchCfg.searchSpcToAddModList[idx].mSymbolsWithinSlot, 0, MONITORING_SYMB_WITHIN_SLOT_SIZE);
            initDlBwp->pdcchCfg.searchSpcToAddModList[idx].mSymbolsWithinSlot[idx] = PDCCH_SYMBOL_WITHIN_SLOT;
            initDlBwp->pdcchCfg.searchSpcToAddModList[idx].numCandidatesAggLevel1 = AGGREGATIONLEVEL_N8; 
            initDlBwp->pdcchCfg.searchSpcToAddModList[idx].numCandidatesAggLevel2 = AGGREGATIONLEVEL_N8; 
            initDlBwp->pdcchCfg.searchSpcToAddModList[idx].numCandidatesAggLevel4 = AGGREGATIONLEVEL_N4; 
            initDlBwp->pdcchCfg.searchSpcToAddModList[idx].numCandidatesAggLevel8 = AGGREGATIONLEVEL_N2; 
            initDlBwp->pdcchCfg.searchSpcToAddModList[idx].numCandidatesAggLevel16 = AGGREGATIONLEVEL_N1;
            initDlBwp->pdcchCfg.searchSpcToAddModList[idx].searchSpaceType = SEARCHSPACETYPE_PR_UE_SPECIFIC;
            initDlBwp->pdcchCfg.searchSpcToAddModList[idx].ueSpecificDciFormat = PDCCH_SRCH_SPC_TWO_UE_SPEC_DCI_FORMAT;
         }
         initDlBwp->pdcchCfg.numSearchSpcToRel = 0;
      }

      /* Filling PDSCH Config */
      initDlBwp->pdschPresent = TRUE;
      if(initDlBwp->pdschPresent)
      {
         initDlBwp->pdschCfg.dmrsDlCfgForPdschMapTypeA.addPos = ADDITIONALPOSITION_POS0;
         initDlBwp->pdschCfg.resourceAllocType = RESOURCEALLOCATION_TYPE1;

         initDlBwp->pdschCfg.numTimeDomRsrcAlloc = NUM_TIME_DOM_RSRC_ALLOC;

         idx = 0; 
         initDlBwp->pdschCfg.timeDomRsrcAllociList[idx].mappingType = MAPPING_TYPEA;
         initDlBwp->pdschCfg.timeDomRsrcAllociList[idx].startSymbol = PDSCH_START_SYMBOL; 
         initDlBwp->pdschCfg.timeDomRsrcAllociList[idx].symbolLength = PDSCH_LENGTH_SYMBOL;
         initDlBwp->pdschCfg.timeDomRsrcAllociList[idx].startSymbolAndLength = calcSliv(PDSCH_START_SYMBOL, PDSCH_LENGTH_SYMBOL);

         idx++;
         DU_ALLOC_SHRABL_BUF(initDlBwp->pdschCfg.timeDomRsrcAllociList[idx].k0, sizeof(uint8_t));
         if(initDlBwp->pdschCfg.timeDomRsrcAllociList[idx].k0 == NULLP)
         {
            DU_LOG("\nERROR  -->  DUAPP : Failed to allocate memory to K0 in fillDefaultInitDlBwp");
            return RFAILED;
         }
         if(initDlBwp->pdschCfg.timeDomRsrcAllociList[idx].k0)
         {
            *(initDlBwp->pdschCfg.timeDomRsrcAllociList[idx].k0) = 1;
         }
         initDlBwp->pdschCfg.timeDomRsrcAllociList[idx].mappingType = MAPPING_TYPEA;
         initDlBwp->pdschCfg.timeDomRsrcAllociList[idx].startSymbol = PDSCH_START_SYMBOL; 
         initDlBwp->pdschCfg.timeDomRsrcAllociList[idx].symbolLength = PDSCH_LENGTH_SYMBOL;
         initDlBwp->pdschCfg.timeDomRsrcAllociList[idx].startSymbolAndLength = calcSliv(PDSCH_START_SYMBOL, PDSCH_LENGTH_SYMBOL);

         initDlBwp->pdschCfg.rbgSize = RBG_SIZE_CONFIG1;
         initDlBwp->pdschCfg.numCodeWordsSchByDci = CODEWORDS_SCHED_BY_DCI_N1;
         initDlBwp->pdschCfg.bundlingType = TYPE_STATIC_BUNDLING;
         initDlBwp->pdschCfg.bundlingInfo.StaticBundling.size = 0;
      }
   }

   return ROK;
}

/******************************************************************
 *
 * @brief Fills Initial UL Bandwidth Part
 *
 * @details
 *
 *    Function : fillDefaultInitUlBwp
 *
 *    Functionality: Fills Initial UL Bandwidth Part
 *
 * @params[in]  InitialUlBwp *initUlBwp
 * @return void
 *
 *****************************************************************/
void fillDefaultInitUlBwp(InitialUlBwp *initUlBwp)
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
         initUlBwp->puschCfg.numTimeDomRsrcAlloc = 2;
         idx = 0;
         if(initUlBwp->puschCfg.numTimeDomRsrcAlloc <= MAX_NUM_UL_ALLOC)
         {
            initUlBwp->puschCfg.timeDomRsrcAllocList[idx].k2 = PUSCH_K2_CFG1;
            initUlBwp->puschCfg.timeDomRsrcAllocList[idx].mappingType =\
                                                                       MAPPING_TYPEA;
            initUlBwp->puschCfg.timeDomRsrcAllocList[idx].startSymbol = PUSCH_START_SYMBOL;
            initUlBwp->puschCfg.timeDomRsrcAllocList[idx].symbolLength = PUSCH_LENGTH_SYMBOL;
            initUlBwp->puschCfg.timeDomRsrcAllocList[idx].startSymbolAndLength =\
                                                                                calcSliv(PUSCH_START_SYMBOL, PUSCH_LENGTH_SYMBOL);

            idx++;
            initUlBwp->puschCfg.timeDomRsrcAllocList[idx].k2 = PUSCH_K2_CFG2;
            initUlBwp->puschCfg.timeDomRsrcAllocList[idx].mappingType = MAPPING_TYPEA;
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
      DU_LOG("\nERROR  -->  DU APP : Memory is NULL of InitalUlBwp");
   }

}
/******************************************************************
 *
 * @brief Fills SpCell Group Info
 *
 * @details
 *
 *    Function : fillDefaultSpCellGrpInfo
 *
 *    Functionality: Fills Sp Cell Group Info
 *
 * @params[in]  DuMacUeCfg *macUeCfg
 * @return ROK - Success
 *         RFAILED - Failure
 *
 *****************************************************************/
uint8_t fillDefaultSpCellGrpInfo(DuMacUeCfg *macUeCfg)
{
   SpCellRecfg *spCell = NULL;

   if(macUeCfg)
      spCell = &macUeCfg->spCellCfg;

   if(spCell)
   {
      macUeCfg->spCellCfgPres = true;

      spCell->servCellIdx = SERV_CELL_IDX;
      /* Filling Initial Dl Bwp */
      if((fillDefaultInitDlBwp(&spCell->servCellCfg.initDlBwp)) != ROK)
      {
         DU_LOG("\nERROR  -->  DUAPP : Failed in fillDefaultInitDlBwp");
         return RFAILED;
      }

      spCell->servCellCfg.numDlBwpToAddOrMod    = 0; 
      spCell->servCellCfg.numDlBwpToRel    = 0; 
      spCell->servCellCfg.firstActvDlBwpId = ACTIVE_DL_BWP_ID;
      spCell->servCellCfg.defaultDlBwpId   = ACTIVE_DL_BWP_ID;
      spCell->servCellCfg.bwpInactivityTmr = NULLP;
      spCell->servCellCfg.pdschServCellCfg.maxMimoLayers = NULLP;
      spCell->servCellCfg.pdschServCellCfg.maxCodeBlkGrpPerTb = NULLP;
      spCell->servCellCfg.pdschServCellCfg.codeBlkGrpFlushInd = NULLP;
      spCell->servCellCfg.pdschServCellCfg.xOverhead = NULLP;
      spCell->servCellCfg.pdschServCellCfg.numHarqProcForPdsch = NUM_HARQ_PROC_FOR_PDSCH_N_16;

      /* Filling Initial UL Bwp*/
      fillDefaultInitUlBwp(&spCell->servCellCfg.initUlBwp);
      spCell->servCellCfg.numUlBwpToAddOrMod  = 0; 
      spCell->servCellCfg.numUlBwpToRel       = 0; 
      spCell->servCellCfg.firstActvUlBwpId    = ACTIVE_DL_BWP_ID; 
   }
   else
   {
      DU_LOG("\nERROR  -->  DU APP : Memory is NULL for SpCellGrp");
      return RFAILED;
   }
   return ROK;
}

/******************************************************************
 *
 * @brief Fills Physical Cell Group Info
 *
 * @details
 *
 *    Function : fillDefaultPhyCellGrpInfo
 *
 *    Functionality: Fills Physical Cell Group Info
 *
 * @params[in]  DuMacUeCfg *macUeCfg
 * @return void
 *
 *****************************************************************/
void fillDefaultPhyCellGrpInfo(DuMacUeCfg *macUeCfg)
{
   PhyCellGrpCfg *cellGrp = NULL;

   if(macUeCfg)
      cellGrp = &macUeCfg->phyCellGrpCfg;

   if(cellGrp)
   {
      macUeCfg->phyCellGrpCfgPres = true;
      cellGrp->pdschHarqAckCodebook = PDSCH_HARQ_ACK_CODEBOOK_DYNAMIC;
      cellGrp->pNrFr1 = P_NR_FR1;
   }
   else
   {
      DU_LOG("\nERROR  -->  DU APP : Memory is NULL for Physical Cell Group");
   }
}

/******************************************************************
 *
 * @brief Fills Mac Cell Group Info
 *
 * @details
 *
 *    Function : fillDefaultMacCellGrpInfo
 *
 *    Functionality: Fills Mac Cell Group Info
 *
 * @params[in]  DuMacUeCfg *macUeCfg
 * @return void
 *
 *****************************************************************/
void fillDefaultMacCellGrpInfo(DuMacUeCfg *macUeCfg)
{
   uint8_t idx;
   MacCellGrpCfg *cellGrp = NULL;
   
   if(macUeCfg)
      cellGrp = &macUeCfg->macCellGrpCfg;

   if(cellGrp)
   {
      macUeCfg->macCellGrpCfgPres = true;

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

      /* Filling Drx Config */
#ifdef NR_DRX
      cellGrp->drxCfg.drxOnDurationTimer.onDurationTimerValInMs = DRX_ONDURATION_TIMER_VALUE_PRESENT_IN_MS;
      if(!cellGrp->drxCfg.drxOnDurationTimer.onDurationTimerValInMs)
         cellGrp->drxCfg.drxOnDurationTimer.onDurationtimerValue.subMilliSeconds = \
                                                                                   DRX_ONDURATION_TIMER_VALUE_IN_SUBMS;
      else
         cellGrp->drxCfg.drxOnDurationTimer.onDurationtimerValue.milliSeconds = \
                                                                                DRX_ONDURATION_TIMER_VALUE_IN_MS;
      cellGrp->drxCfg.drxInactivityTimer = DRX_INACTIVITY_TIMER;
      cellGrp->drxCfg.drxHarqRttTimerDl = DRX_HARQ_RTT_TIMER_DL;
      cellGrp->drxCfg.drxHarqRttTimerUl = DRX_HARQ_RTT_TIMER_UL;
      cellGrp->drxCfg.drxRetransmissionTimerDl = DRX_RETRANSMISSION_TIMER_DL;
      cellGrp->drxCfg.drxRetransmissionTimerUl = DRX_RETRANSMISSION_TIMER_UL;
      cellGrp->drxCfg.drxLongCycleStartOffset.drxLongCycleStartOffsetVal = DRX_LONG_CYCLE_START_OFFSET_VAL;
      cellGrp->drxCfg.drxLongCycleStartOffset.drxLongCycleStartOffsetChoice = DRX_LONG_CYCLE_START_OFFSET_CHOICE;
      cellGrp->drxCfg.shortDrxPres = DRX_SHORT_CYCLE_PRESENT;
      if(cellGrp->drxCfg.shortDrxPres)
      {
         cellGrp->drxCfg.shortDrx.drxShortCycle = DRX_SHORT_CYCLE;
         cellGrp->drxCfg.shortDrx.drxShortCycleTimer = DRX_SHORT_CYCLE_TIMER;
      }
      cellGrp->drxCfg.drxSlotOffset = DRX_SLOT_OFFSET;
#endif

   }
   else
   {
      DU_LOG("\nERROR  -->  DU APP : Memory is NULL for Master Cell Group");
   }
}

/*******************************************************************
 *
 * @brief Fills default modulation info for a UE
 *
 * @details
 *
 *    Function : fillDefaultModulation
 *
 *    Functionality: Fills default modulation info for a UE
 *
 * @params[in] Pointer to MAC UE configuration
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
void fillDefaultModulation(DuMacUeCfg *ueCfg)
{
   ueCfg->dlModInfo.modOrder = MOD_ORDER_QPSK;
   ueCfg->dlModInfo.mcsIndex = DEFAULT_MCS;
   ueCfg->dlModInfo.mcsTable = MCS_TABLE_QAM64; /* Spec 38.214-Table 5.1.3.1-1 */

   ueCfg->ulModInfo.modOrder = MOD_ORDER_QPSK;
   ueCfg->ulModInfo.mcsIndex = DEFAULT_MCS;
   ueCfg->ulModInfo.mcsTable = MCS_TABLE_QAM64; /* Spec 38.214-Table 5.1.3.1-1 */
}

/******************************************************************
 *
 * @brief Function to fill Mac Lc Cfg for SRB1
 *
 * @details
 *
 *    Function : fillMacSrb1LcCfg
 *
 *    Functionality: Function to fill Mac Lc cfg for SRB1
 *
 * @params[in]  LcCfg *lcCfg, LcCfg *ueSetReqDb
 * @return void
 *****************************************************************/

void fillMacSrb1LcCfg(DuLcCfg *macLcCfg)
{
   macLcCfg->lcConfig.lcId   = SRB1_LCID;
   macLcCfg->configType = CONFIG_ADD;
   macLcCfg->lcConfig.drbQos = NULLP;
   macLcCfg->lcConfig.snssai = NULLP;
   macLcCfg->lcConfig.ulLcCfgPres = true;
   fillDefaultUlLcCfg(&macLcCfg->lcConfig.ulLcCfg);
}

/******************************************************************
 *
 * @brief Function to fill the Lc cfg from ueSetupReqDb
 *
 * @details
 *
 *    Function : fillMacLcCfgToAddMod
 *
 *    Functionality: Function to fill the Lc cfg from ueSetupReqDb
 *
 * @params[in]  LcCfg *lcCfg, LcCfg *ueSetReqDb
 * @return ROK/RFAILED
 *
 *****************************************************************/

uint8_t fillMacLcCfgToAddMod(DuLcCfg *macLcCfgToSend, DuLcCfg *ueLcCfgDb, DuLcCfg *oldLcCfg, Bool toUpdate)
{
   if(!toUpdate)
   {
      if(macLcCfgToSend)
      {
         macLcCfgToSend->lcConfig.lcId = ueLcCfgDb->lcConfig.lcId;
         macLcCfgToSend->configType = ueLcCfgDb->configType;

         if(ueLcCfgDb->lcConfig.drbQos)
            macLcCfgToSend->lcConfig.drbQos = ueLcCfgDb->lcConfig.drbQos;
         else if(oldLcCfg)
            macLcCfgToSend->lcConfig.drbQos = oldLcCfg->lcConfig.drbQos;
         else
            macLcCfgToSend->lcConfig.drbQos = NULL;

         if(ueLcCfgDb->lcConfig.snssai)
            macLcCfgToSend->lcConfig.snssai = ueLcCfgDb->lcConfig.snssai;
         else if(oldLcCfg)
            macLcCfgToSend->lcConfig.snssai = oldLcCfg->lcConfig.snssai;
         else
            macLcCfgToSend->lcConfig.snssai = NULL;

         macLcCfgToSend->lcConfig.ulLcCfgPres = ueLcCfgDb->lcConfig.ulLcCfgPres;
         memcpy(&macLcCfgToSend->lcConfig.ulLcCfg, &ueLcCfgDb->lcConfig.ulLcCfg, sizeof(UlLcCfg));
         memcpy(&macLcCfgToSend->lcConfig.dlLcCfg, &ueLcCfgDb->lcConfig.dlLcCfg, sizeof(DlLcCfg));
      }
   }
   else
   {
      oldLcCfg->lcConfig.lcId = ueLcCfgDb->lcConfig.lcId;
      oldLcCfg->configType = ueLcCfgDb->configType;

      if(ueLcCfgDb->lcConfig.drbQos)
      {
         if(oldLcCfg->lcConfig.drbQos)
            DU_FREE_SHRABL_BUF(DU_APP_MEM_REGION, DU_POOL, oldLcCfg->lcConfig.drbQos, sizeof(DrbQosInfo));

         DU_ALLOC_SHRABL_BUF(oldLcCfg->lcConfig.drbQos, sizeof(DrbQosInfo));
         if(oldLcCfg->lcConfig.drbQos == NULL)
         {
            DU_LOG("\nERROR  -->  DU APP : Memory Alloc Failed at fillMacLcCfgToAddMod()");
            return RFAILED;
         }
         memcpy(oldLcCfg->lcConfig.drbQos, ueLcCfgDb->lcConfig.drbQos, sizeof(DrbQosInfo));
      }

      if(ueLcCfgDb->lcConfig.snssai)
      {
         if(oldLcCfg->lcConfig.snssai)
            DU_FREE_SHRABL_BUF(DU_APP_MEM_REGION, DU_POOL, oldLcCfg->lcConfig.snssai, sizeof(Snssai));

         DU_ALLOC_SHRABL_BUF(oldLcCfg->lcConfig.snssai, sizeof(Snssai));
         if(oldLcCfg->lcConfig.snssai == NULL)
         {
            DU_LOG("\nERROR  -->  DU APP : Memory Alloc Failed at fillMacLcCfgToAddMod()");
            DU_FREE_SHRABL_BUF(DU_APP_MEM_REGION, DU_POOL, oldLcCfg->lcConfig.drbQos, sizeof(DrbQosInfo));
            return RFAILED;
         }
         memcpy(oldLcCfg->lcConfig.snssai, ueLcCfgDb->lcConfig.snssai, sizeof(Snssai));
      }

      oldLcCfg->lcConfig.ulLcCfgPres = ueLcCfgDb->lcConfig.ulLcCfgPres;
      memcpy(&oldLcCfg->lcConfig.ulLcCfg, &ueLcCfgDb->lcConfig.ulLcCfg, sizeof(UlLcCfg));
      memcpy(&oldLcCfg->lcConfig.dlLcCfg, &ueLcCfgDb->lcConfig.dlLcCfg, sizeof(DlLcCfg));
   }
   return ROK;
}

/******************************************************************
 *
 * @brief Function to copy the Bit rate from ueSetupReqDb
 *
 * @details
 *
 *    Function : fillAmbr
 *
 *    Functionality: Function to copy bit Rate from ueSetupReqDb
 *
 * @params[in]  AmbrCfg **macAmbr, AmbrCfg  *ueDbAmbr
 * @return ROK/RFAILED
 *
 *****************************************************************/

uint8_t fillAmbr(AmbrCfg **macAmbrCfgToSend, AmbrCfg *ueDbAmbr, AmbrCfg **oldMacAmbrCfg, Bool toUpdate)
{
   if(!toUpdate)
   {
      if(ueDbAmbr)
      {
         *macAmbrCfgToSend = ueDbAmbr;
      }
      else if(oldMacAmbrCfg)
         *macAmbrCfgToSend = *oldMacAmbrCfg;       
   }
   else
   {
      if(ueDbAmbr)
      {
         if(oldMacAmbrCfg)
         {
            if(*oldMacAmbrCfg)
            {
               DU_FREE_SHRABL_BUF(DU_APP_MEM_REGION, DU_POOL, *oldMacAmbrCfg, sizeof(AmbrCfg));
            }
            DU_ALLOC_SHRABL_BUF(*oldMacAmbrCfg, sizeof(AmbrCfg));
            if(*oldMacAmbrCfg == NULLP)
            {
               DU_LOG("\nERROR  -->  DU APP : Memory Alloc Failed at fillAmbr()");
               return RFAILED;
            }
            memset(*oldMacAmbrCfg, 0, sizeof(AmbrCfg));
            (*oldMacAmbrCfg)->ulBr = ueDbAmbr->ulBr;
         }
      }
   }

   return ROK;
}

/******************************************************************
 *
 * @brief Builds and Send UE ReConfig Request to MAC
 *
 * @details
 *
 *    Function : sendUeRecfgReqToMac
 *
 *    Functionality: Builds and Send UE ReConfig Request to MAC
 *
 * @Params[in]  MacUeRecfg pointer
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t sendUeRecfgReqToMac(MacUeRecfg *macUeRecfg)
{
   uint8_t ret = ROK;
   Pst pst;

   /* Fill Pst */
   FILL_PST_DUAPP_TO_MAC(pst, EVENT_MAC_UE_RECONFIG_REQ);

   if(macUeRecfg)
   {
      /* Processing one Ue at a time to MAC */
      DU_LOG("\nDEBUG   -->  DU_APP: Sending Ue Reconfig Request to MAC");
      ret = (*packMacUeReconfigReqOpts[pst.selector])(&pst, macUeRecfg);
      if(ret == RFAILED)
      {
         DU_LOG("\nERROR  -->  DU APP : Failed to send Reconfig Request to MAC at sendUeRecfgReqToMac()");
         DU_FREE_SHRABL_BUF(DU_APP_MEM_REGION, DU_POOL, macUeRecfg, sizeof(MacUeRecfg));
      }
   }
   else
   {
      DU_LOG("\nERROR  -->  DU_APP: Received macUeRecfg is NULLP at sendUeRecfgReqToMac()");
      ret = RFAILED;
   }
   return ret;
}



/******************************************************************
 *
 * @brief update DuMacUeCfg structure
 *
 * @details
 *
 *    Function : updateDuMacUeCfg
 *
 *    Functionality: Fills DuMacUeCfg
 *
 * @params[in]  cellId, ueId, crnti, 
 *              DuUeCfg  pointer,
 *              DuMacUeCfg pointer
 * @return ROK/RFAILED
 *
 *****************************************************************/
uint8_t updateDuMacUeCfg(uint16_t cellId, uint8_t gnbDuUef1apId, uint16_t crnti, DuUeCfg *ueCfgDb, DuMacUeCfg *duMacUeCfg)
{
   uint8_t ret = ROK, dbIdx = 0, lcIdx = 0, cellIdx = 0;
   bool lcIdFound = false;
   DuMacUeCfg *duMacDb = NULLP;

   duMacUeCfg->cellId = cellId;
   duMacUeCfg->ueId = gnbDuUef1apId;
   duMacUeCfg->crnti = crnti;

   if(!ueCfgDb)
   {
      fillDefaultMacCellGrpInfo(duMacUeCfg);
      fillDefaultPhyCellGrpInfo(duMacUeCfg);

      if((fillDefaultSpCellGrpInfo(duMacUeCfg)) != ROK)
      {
         DU_LOG("\nERROR  --> DUAPP : Failed in fillDefaultSpCellGrpInfo");
         return RFAILED;
      }

      duMacUeCfg->ambrCfg = NULLP;
      fillDefaultModulation(duMacUeCfg);
      fillMacSrb1LcCfg(&duMacUeCfg->lcCfgList[0]);
      duMacUeCfg->numLcs++;
   }
   else
   {
      if(ueCfgDb->dataTransmissionAction == STOP_TRANSMISSION)
      {
         duMacUeCfg->transmissionAction = ueCfgDb->dataTransmissionAction; 
         return ROK;
      }

      GET_CELL_IDX(cellId, cellIdx);
      if(duCb.actvCellLst[cellIdx])
         duMacDb = &duCb.actvCellLst[cellIdx]->ueCb[duMacUeCfg->ueId-1].duMacUeCfg;
      else
      {
         DU_LOG("\nERROR  -->  DU APP : Cell Id [%d] does not exist", cellId);
         return RFAILED;
      }
      duMacDb->macUeCfgState = UE_CFG_INPROGRESS;
      
      if(ueCfgDb->cellGrpCfg)
      {
         ret = procUeRecfgCellInfo(duMacUeCfg, duMacDb, ueCfgDb->cellGrpCfg);
         if(ret == ROK)
         {
            if(duMacUeCfg->spCellCfgPres == true)
            {
               if(duMacUeCfg->spCellCfg.servCellCfg.initDlBwp.pdschPresent)
               {
                  fillStartSymbolAndLen(duMacUeCfg->spCellCfg.servCellCfg.initDlBwp.pdschCfg.numTimeDomRsrcAlloc,\
                        &duMacUeCfg->spCellCfg.servCellCfg.initDlBwp.pdschCfg, NULL);
               }
               if(duMacUeCfg->spCellCfg.servCellCfg.initUlBwp.puschPresent)
               {
                  fillStartSymbolAndLen(duMacUeCfg->spCellCfg.servCellCfg.initUlBwp.puschCfg.numTimeDomRsrcAlloc,\
                        NULL, &duMacUeCfg->spCellCfg.servCellCfg.initUlBwp.puschCfg);
               }
            }

            if(duMacDb)
               ret = fillAmbr(&duMacUeCfg->ambrCfg, ueCfgDb->ambrCfg , &duMacDb->ambrCfg, FALSE);
            else
               ret = fillAmbr(&duMacUeCfg->ambrCfg, ueCfgDb->ambrCfg , NULL, FALSE);

            duFillModulationDetails(duMacUeCfg, duMacDb, ueCfgDb->ueNrCapability);
         }
      }
      else
      {
         fillDefaultMacCellGrpInfo(duMacUeCfg);
         fillDefaultPhyCellGrpInfo(duMacUeCfg);
         fillDefaultSpCellGrpInfo(duMacUeCfg);
         fillDefaultModulation(duMacUeCfg);
         duMacUeCfg->ambrCfg = NULLP;
      }

#ifdef NR_DRX
      
      duMacUeCfg->drxConfigIndicatorRelease = ueCfgDb->drxConfigIndicatorRelease;

      if(ueCfgDb->drxCyclePres)
      {
         duMacUeCfg->macCellGrpCfg.drxCfg.drxLongCycleStartOffset.drxLongCycleStartOffsetChoice = ueCfgDb->drxCycle.drxLongCycleLength;
         if(ueCfgDb->drxCycle.shortDrxCyclePres)
         {
            duMacUeCfg->macCellGrpCfg.drxCfg.shortDrxPres = true;
            duMacUeCfg->macCellGrpCfg.drxCfg.shortDrx.drxShortCycle = ueCfgDb->drxCycle.shortDrxCycle.drxShortCycle;
            duMacUeCfg->macCellGrpCfg.drxCfg.shortDrx.drxShortCycleTimer = ueCfgDb->drxCycle.shortDrxCycle.drxShortCycleTimer;
         }
         else
         {
            duMacUeCfg->macCellGrpCfg.drxCfg.shortDrxPres = false;
         }
      }
#endif

      /* Filling LC Context */
      for(dbIdx = 0; (dbIdx < ueCfgDb->numMacLcs && ret == ROK); dbIdx++)
      {
         if(!ueCfgDb->macLcCfg[dbIdx].lcConfig.ulLcCfgPres)
         {
            /* Filling default UL LC config in MAC if not present */
            ueCfgDb->macLcCfg[dbIdx].lcConfig.ulLcCfgPres = true;
            fillDefaultUlLcCfg(&ueCfgDb->macLcCfg[dbIdx].lcConfig.ulLcCfg);
         }

         if(duMacDb)
         {
            for(lcIdx = 0; lcIdx < duMacDb->numLcs; lcIdx++)
            {
               if(ueCfgDb->macLcCfg[dbIdx].lcConfig.lcId == duMacDb->lcCfgList[lcIdx].lcConfig.lcId)
               {
                  lcIdFound = true;
                  if((ueCfgDb->macLcCfg[dbIdx].configType == CONFIG_UNKNOWN) ||
                        (ueCfgDb->macLcCfg[dbIdx].configType == CONFIG_MOD))
                  {
                     ueCfgDb->macLcCfg[dbIdx].configType = CONFIG_MOD;
                     ret = fillMacLcCfgToAddMod(&duMacUeCfg->lcCfgList[dbIdx], &ueCfgDb->macLcCfg[dbIdx],\
                           &duMacDb->lcCfgList[lcIdx], FALSE);
                  }
               }
               else
                  lcIdFound = false;
            }
         }

         if(!lcIdFound)
         {
            /* ADD/DEL CONFIG */
            ret = fillMacLcCfgToAddMod(&duMacUeCfg->lcCfgList[dbIdx], &ueCfgDb->macLcCfg[dbIdx], NULL, FALSE);
         }
         if(ret == ROK)
         {
            duMacUeCfg->numLcs++;
         }
         else
         {
            DU_LOG("\nERROR  -->  DU APP : Failed to add Lc at Idx %d in updateDuMacUeCfg()", dbIdx); 
            break;
         }
      }/*End of Outer FOR loop */
      memcpy(&ueCfgDb->copyOfmacUeCfg, duMacUeCfg, sizeof(DuMacUeCfg));
   }
   return ret;
}

/******************************************************************
 *
 * @brief Fills Rlc AM Information
 *
 * @details
 *
 *    Function : fillDefaultAmInfo
 *
 *    Functionality: Fills Rlc AM Information
 *
 * @params[in]  AmBearerCfg *amCfg
 * @return void
 *
 *****************************************************************/
void fillDefaultAmInfo(AmBearerCfg *amCfg)
{
   /* DL AM */
   amCfg->dlAmCfg.snLenDl     = AM_SIZE_12;
   amCfg->dlAmCfg.pollRetxTmr = T_POLL_RETRANSMIT_VAL;
   amCfg->dlAmCfg.pollPdu     = POLL_PDU_VAL;
   amCfg->dlAmCfg.pollByte    = POLL_BYTE_VAL;
   amCfg->dlAmCfg.maxRetxTh   = MAX_RETX_THRESHOLD_VAL;   
 
   /* UL AM */
   amCfg->ulAmCfg.snLenUl     = AM_SIZE_12;
   amCfg->ulAmCfg.reAssemTmr  = T_REASSEMBLY_VAL; 
   amCfg->ulAmCfg.statProhTmr = T_STATUS_PROHIBHIT_VAL;
}

/******************************************************************
 *
 * @brief Fills RLC UM Bi Directional Information
 *
 * @details
 *
 *    Function : fillDefaultUmBiInfo
 *
 *    Functionality: Fills RLC UM Bi Directional Information
 *
 * @params[in]  UmBiDirBearerCfg *umBiDirCfg
 * @return void
 *
 *****************************************************************/
void fillDefaultUmBiInfo(UmBiDirBearerCfg *umBiDirCfg)
{
   /* UL UM BI DIR INFO */
   umBiDirCfg->ulUmCfg.snLenUlUm = UM_SIZE_12;
   umBiDirCfg->ulUmCfg.reAssemTmr = T_REASSEMBLY_VAL;

   /* DL UM BI DIR INFO */
   umBiDirCfg->dlUmCfg.snLenDlUm  = UM_SIZE_12;
}

/******************************************************************
 *
 * @brief Fills RLC UM Uni Directional UL Information
 *
 * @details
 *
 *    Function : fillDefaultUmUlInfo
 *
 *    Functionality: Fills RLC UM Uni Directional Info
 *
 * @params[in]  UmUniDirUlBearerCfg *UmUlCfg
 * @return void
 *
 *****************************************************************/
void fillDefaultUmUlInfo(UmUniDirUlBearerCfg *UmUlCfg)
{
   UmUlCfg->ulUmCfg.snLenUlUm = UM_SIZE_12;
   UmUlCfg->ulUmCfg.reAssemTmr = T_REASSEMBLY_VAL;
}

/******************************************************************
 *
 * @brief Fills RLC UM Uni Directional DL Information
 *
 * @details
 *
 *    Function : fillDefaultUmDlInfo
 *
 *    Functionality: Fills RLC UM Uni Directional DL Info
 *
 * @params[in]  UmUniDirDlBearerCfg *UmDlCfg
 * @return void
 *
 *****************************************************************/
void fillDefaultUmDlInfo(UmUniDirDlBearerCfg *UmDlCfg)
{
   UmDlCfg->dlUmCfg.snLenDlUm  = UM_SIZE_12;
}

/******************************************************************
 *
 * @brief Builds Rlc Mode Default Configuration
 *
 * @details
 *
 *    Function : fillDefaultRlcModeCfg
 *
 *    Functionality: Builds Rlc Mode Default Configuration
 *
 * @params[in]  rlcMode, RlcBearerCfg *lcCfg
 * @return ROK/RFAILED
 *
 *****************************************************************/

uint8_t fillDefaultRlcModeCfg(uint8_t rlcMode, RlcBearerCfg *lcCfg)
{
   if(lcCfg)
   {
      switch(rlcMode)
      {
         case RLC_AM :
            {
               if(!lcCfg->u.amCfg)
               {
                  DU_ALLOC_SHRABL_BUF(lcCfg->u.amCfg, sizeof(AmBearerCfg));
                  if(lcCfg->u.amCfg)
                     fillDefaultAmInfo(lcCfg->u.amCfg);
                  else
                  {
                     DU_LOG("\n ERROR  -->  DU APP : Memory Alloc failed at AmCfg at fillDefaultRlcModeCfg()");
                     return RFAILED;
                  }
               }
               break;
            }
         case RLC_UM_BI_DIRECTIONAL :
            {
               if(!lcCfg->u.umBiDirCfg)
               {
                  DU_ALLOC_SHRABL_BUF(lcCfg->u.umBiDirCfg, sizeof(UmBiDirBearerCfg));
                  if(lcCfg->u.umBiDirCfg)
                     fillDefaultUmBiInfo(lcCfg->u.umBiDirCfg);
                  else
                  {
                     DU_LOG("\n ERROR  -->  DU APP : Memory Alloc failed at UmBiDirCfg at fillDefaultRlcModeCfg()");
                     return RFAILED;
                  }
               }
               break;
            }
         case RLC_UM_UNI_DIRECTIONAL_UL :
            {
               if(!lcCfg->u.umUniDirUlCfg)
               {
                  DU_ALLOC_SHRABL_BUF(lcCfg->u.umUniDirUlCfg, sizeof(UmUniDirUlBearerCfg));
                  if(lcCfg->u.umUniDirUlCfg)
                     fillDefaultUmUlInfo(lcCfg->u.umUniDirUlCfg);
                  else
                  {
                     DU_LOG("\n ERROR  -->  DU APP : Memory Alloc failed at UmUniDirUlCfg at fillDefaultRlcModeCfg()");
                     return RFAILED;
                  }
               }
               break;
            }
         case RLC_UM_UNI_DIRECTIONAL_DL :
            {
               if(!lcCfg->u.umUniDirDlCfg)
               {
                  DU_ALLOC_SHRABL_BUF(lcCfg->u.umUniDirDlCfg, sizeof(UmUniDirDlBearerCfg));
                  if(lcCfg->u.umUniDirDlCfg)
                     fillDefaultUmDlInfo(lcCfg->u.umUniDirDlCfg);
                  else
                  {
                     DU_LOG("\n ERROR  -->  DU APP : Memory Alloc failed at UmUniDirDlCfg at fillDefaultRlcModeCfg()");
                     return RFAILED;
                  }
               }
               break;
            }
         default:
            DU_LOG("\nERROR  -->  DUAPP: Invalid rlcMode %d at extractRlcCfgToAddMod()", rlcMode);
            return RFAILED;
      }

   }
   else
   {
      DU_LOG("\nERROR  -->  DUAPP: Received LC Config is NULL");
      return RFAILED;
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Function to fill Rlc Lc Cfg for SRB1
 *
 * @details
 *
 *    Function : fillRlcSrb1LcCfg
 *
 *    Functionality: 
 *     Function to fill Rlc Lc Cfg for SRB1
 * 
 *  @params[in]     Pointer to RlcBearerCfg
 *  @return ROK/RFAILED
 * 
 *****************************************************************/

uint8_t fillRlcSrb1LcCfg(DuRlcBearerCfg *rlcLcCfg)
{
   uint8_t ret = ROK;

   rlcLcCfg->rlcBearerCfg.rbId   = SRB1_LCID;
   rlcLcCfg->rlcBearerCfg.rbType = RB_TYPE_SRB;
   rlcLcCfg->rlcBearerCfg.lcId   = SRB1_LCID;
   rlcLcCfg->rlcBearerCfg.lcType = LCH_DCCH;
   rlcLcCfg->rlcBearerCfg.rlcMode = RLC_AM;
   rlcLcCfg->configType = CONFIG_ADD;
   ret = fillDefaultRlcModeCfg(rlcLcCfg->rlcBearerCfg.rlcMode, &rlcLcCfg->rlcBearerCfg);
   rlcLcCfg->rlcBearerCfg.isLcAddModRspSent = true;
   return ret;
}

/*******************************************************************
 *
 * @brief Processes UE ReConfig Req to RLC UL
 *
 * @details
 *
 *    Function : sendUeRecfgReqToRlc
 *
 *    Functionality: 
 *     Processes UE Reconfig Req to RLC UL
 * 
 *  @params[in]     Pointer to RlcUeRecfg
 *  @return ROK     - success
 *          RFAILED - failure
 * 
 *****************************************************************/

uint8_t sendUeRecfgReqToRlc(RlcUeRecfg *rlcUeRecfg)
{
   uint8_t ret;
   Pst pst;
   
   FILL_PST_DUAPP_TO_RLC(pst, RLC_UL_INST, EVENT_RLC_UE_RECONFIG_REQ);
   if(rlcUeRecfg)
   {
      /* Processing one Ue at a time to RLC */
      DU_LOG("\nDEBUG   -->  DU_APP: Sending Ue Reconfig Request to RLC UL");
      ret = (*packRlcUeReconfigReqOpts[pst.selector])(&pst, rlcUeRecfg);
      if(ret == RFAILED)
      {
         DU_LOG("\nERROR  -->  DU_APP : Failed to send Ue Reconfig Req to RLC at sendUeRecfgReqToRlc()");
         DU_FREE_SHRABL_BUF(DU_APP_MEM_REGION, DU_POOL, rlcUeRecfg, sizeof(RlcUeRecfg));
      }
   }
   else
   {
      DU_LOG("\nERROR  -->   DU_APP: Received RlcUeRecfg is NULL at sendUeRecfgReqToRlc()");
      ret = RFAILED;
   }
   return ret;
}

/******************************************************************
 *
 * @brief Fills Snssai information
 *
 * @details
 *
 *    Function : fillSnssaiInfo
 *
 *    Functionality: Fills Snssai information
 *
 *  @params[in]    LcCfg *snssaiTobeSend, LcCfg *snssaiDb, LcCfg *oldSnssai,
 *  Bool toUpdateg
 *  @return ROK     - success
 *          RFAILED - failure
 * 
 *****************************************************************/
uint8_t fillSnssaiInfo(Snssai *snssaiTobeSend, Snssai *snssaiDb, Snssai **oldSnssai, Bool toUpdate)
{
   if(!toUpdate)
   {
      if(snssaiDb)
         snssaiTobeSend = snssaiDb;
      else if(oldSnssai)
         snssaiTobeSend = *oldSnssai;
      else
         snssaiTobeSend = NULL;
   }
   else
   {
      if(snssaiDb)
      {
         if(*oldSnssai)
            DU_FREE_SHRABL_BUF(DU_APP_MEM_REGION, DU_POOL, *oldSnssai, sizeof(Snssai));

         DU_ALLOC_SHRABL_BUF(*oldSnssai, sizeof(Snssai));
         if(*oldSnssai == NULL)
         {
            DU_LOG("\nERROR  -->  DU APP : Memory Alloc Failed at fillSnssaiInfo()");
            return RFAILED;
         }
         memcpy(*oldSnssai, snssaiDb, sizeof(Snssai));
      }
   }
   return ROK;
}
/******************************************************************
 *
 * @brief Fills RlcBearerCfg structure
 *
 * @details
 *
 *    Function : updateRlcUeCfg
 *
 *    Functionality: Fills Rlc Bearer Cfg
 *
 *
 *****************************************************************/
uint8_t updateRlcUeCfg(uint16_t cellId, uint8_t duUeF1apId, DuUeCfg *ueCfgDb, DuRlcUeCfg *rlcUeCfg)
{
   uint8_t ret, dbIdx, lcIdx, cellIdx;
   bool lcIdFound = false;
   DuRlcUeCfg *duRlcDb = NULLP;

   ret = ROK;
   rlcUeCfg->cellId       = cellId;
   rlcUeCfg->ueId         = duUeF1apId;

   if(!ueCfgDb)
   {
      /* Initial RB being Added */ 
      ret = fillRlcSrb1LcCfg(&rlcUeCfg->rlcLcCfg[0]);
      if(ret == ROK)
         rlcUeCfg->numLcs++;
      else
         memset(rlcUeCfg, 0, sizeof(DuRlcUeCfg));
   }
   else
   {
      /* Fetch RlcDb from DuUeCb */ 
      GET_CELL_IDX(cellId, cellIdx);
      duRlcDb = &duCb.actvCellLst[cellIdx]->ueCb[rlcUeCfg->ueId-1].duRlcUeCfg;
      duRlcDb->rlcUeCfgState = UE_CFG_INPROGRESS;

      for(dbIdx = 0; (dbIdx < ueCfgDb->numRlcLcs && ret == ROK); dbIdx++)
      {
         ret = fillDefaultRlcModeCfg(ueCfgDb->rlcLcCfg[dbIdx].rlcBearerCfg.rlcMode, &ueCfgDb->rlcLcCfg[dbIdx].rlcBearerCfg);
         if(ret == RFAILED)
         {
            DU_LOG("\n ERROR  -->  DU APP : Failed to fill Rlc Mode at fillRlcUeCfg()");
            memset(rlcUeCfg, 0, sizeof(DuRlcUeCfg));
            return ret;
         }

         if(duRlcDb)
         {
            /* Fill Rlc Ue Cfg List for ADD/MOD/DEL */
            for(lcIdx = 0; lcIdx < duRlcDb->numLcs; lcIdx++)
            { 
               if(ueCfgDb->rlcLcCfg[dbIdx].rlcBearerCfg.lcId == duRlcDb->rlcLcCfg[lcIdx].rlcBearerCfg.lcId)
               {
                  lcIdFound = true;
                  if((ueCfgDb->rlcLcCfg[dbIdx].configType == CONFIG_UNKNOWN)||
                        (ueCfgDb->rlcLcCfg[dbIdx].configType == CONFIG_MOD))
                  {
                     /* MOD */ 
                     ueCfgDb->rlcLcCfg[dbIdx].configType = CONFIG_MOD; /* update Db for MOD type */
                     memcpy(&rlcUeCfg->rlcLcCfg[dbIdx], &ueCfgDb->rlcLcCfg[dbIdx], sizeof(DuRlcBearerCfg));
                     fillSnssaiInfo(rlcUeCfg->rlcLcCfg[dbIdx].rlcBearerCfg.snssai, ueCfgDb->rlcLcCfg[dbIdx].rlcBearerCfg.snssai,\
                           &duRlcDb->rlcLcCfg[lcIdx].rlcBearerCfg.snssai,false);
                  }
               }
               else
                  lcIdFound = false;
            }
         }

         if(!lcIdFound)
         {
            /* ADD/ DEL Config Type */
            memcpy(&rlcUeCfg->rlcLcCfg[dbIdx], &ueCfgDb->rlcLcCfg[dbIdx], sizeof(DuRlcBearerCfg));
            fillSnssaiInfo(rlcUeCfg->rlcLcCfg[dbIdx].rlcBearerCfg.snssai, ueCfgDb->rlcLcCfg[dbIdx].rlcBearerCfg.snssai, NULL, false);
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
   uint8_t ueId = 0, ueIdx = 0;

   for(cellIdx = 0; cellIdx < MAX_NUM_CELL; cellIdx++)
   {
      if(duCb.actvCellLst[cellIdx] && (ueCcchCtxt->cellId == duCb.actvCellLst[cellIdx]->cellId))
      {
         GET_UE_ID(ueCcchCtxt->crnti, ueId);
         DU_LOG("\nDEBUG   -->  DU_APP: Filling UeCb for ueId [%d]", ueId);

         ueIdx = ueId-1;
         duCb.actvCellLst[cellIdx]->ueCb[ueIdx].f1UeDb        = NULLP;
         duCb.actvCellLst[cellIdx]->ueCb[ueIdx].crnti         = ueCcchCtxt->crnti;
         duCb.actvCellLst[cellIdx]->ueCb[ueIdx].gnbDuUeF1apId = ueCcchCtxt->gnbDuUeF1apId;
         duCb.actvCellLst[cellIdx]->ueCb[ueIdx].gnbCuUeF1apId = gnbCuUeF1apId;
         duCb.actvCellLst[cellIdx]->ueCb[ueIdx].drbBitMap     = NULLP;
         duCb.actvCellLst[cellIdx]->ueCb[ueIdx].ueState       = UE_ACTIVE;

         /* Filling Mac Ue Config */ 
         memset(&duCb.actvCellLst[cellIdx]->ueCb[ueIdx].duMacUeCfg, 0, sizeof(DuMacUeCfg));
         ret = duBuildAndSendUeCreateReqToMac(ueCcchCtxt->cellId, ueCcchCtxt->gnbDuUeF1apId, ueCcchCtxt->crnti, NULL, 
               &duCb.actvCellLst[cellIdx]->ueCb[ueIdx].duMacUeCfg);
         if(ret == RFAILED)
            DU_LOG("\nERROR  -->  DU APP : Failed to send UE create request to MAC");

         /* Filling Rlc Ue Config */
         memset(&duCb.actvCellLst[cellIdx]->ueCb[ueIdx].duRlcUeCfg, 0, sizeof(DuRlcUeCfg));
         ret = duBuildAndSendUeCreateReqToRlc(ueCcchCtxt->cellId, ueCcchCtxt->gnbDuUeF1apId, NULL,
               &duCb.actvCellLst[cellIdx]->ueCb[ueIdx].duRlcUeCfg);
         if(ret == RFAILED)
            DU_LOG("\nERROR  -->  DU APP : Failed to send UE create request to RLC");

         duCb.actvCellLst[cellIdx]->numActvUes++;
         memset(ueCcchCtxt, 0, sizeof(UeCcchCtxt));
         duCb.numUe--;
      }
   }
   return ret;
}

/**********************************************************************************
 *
 * @brief Fills Ue Cfg from DU DB to MacUeCreateReq
 *
 * @details
 *
 *    Function : fillMacUeCfg
 *
 *    Functionality: fills ue Cfg to be sent to MAC
 *
 * @params[in] DuMacUeCfg Pointer
 *             MacUeCreateReq Pointer
 *
 * @return ROK     - success
 *         RFAILED - failure
 * *******************************************************************************/
void fillMacUeCfg(DuMacUeCfg *duMacUeCfg, MacUeCreateReq *macUeCfg)
{
   uint8_t lcIdx = 0;

   macUeCfg->cellId = duMacUeCfg->cellId;
   macUeCfg->ueId = duMacUeCfg->ueId;
   macUeCfg->crnti = duMacUeCfg->crnti;
   macUeCfg->macCellGrpCfgPres = duMacUeCfg->macCellGrpCfgPres;
   if(macUeCfg->macCellGrpCfgPres)
   {
      memcpy(&macUeCfg->macCellGrpCfg, &duMacUeCfg->macCellGrpCfg, sizeof(MacCellGrpCfg));
   }
   macUeCfg->phyCellGrpCfgPres = duMacUeCfg->phyCellGrpCfgPres;
   if(macUeCfg->phyCellGrpCfgPres)
   {
      memcpy(&macUeCfg->phyCellGrpCfg, &duMacUeCfg->phyCellGrpCfg, sizeof(PhyCellGrpCfg));
   }
   macUeCfg->spCellCfgPres = duMacUeCfg->spCellCfgPres;
   if(macUeCfg->spCellCfgPres)
   {
      macUeCfg->spCellCfg.servCellIdx = duMacUeCfg->spCellCfg.servCellIdx;
      macUeCfg->spCellCfg.servCellCfg.initDlBwp = duMacUeCfg->spCellCfg.servCellCfg.initDlBwp;
      macUeCfg->spCellCfg.servCellCfg.numDlBwpToAdd = duMacUeCfg->spCellCfg.servCellCfg.numDlBwpToAddOrMod;
      if(macUeCfg->spCellCfg.servCellCfg.numDlBwpToAdd > 0)
      {
         memcpy(macUeCfg->spCellCfg.servCellCfg.dlBwpToAddList, duMacUeCfg->spCellCfg.servCellCfg.dlBwpToAddOrModList, \
                   (sizeof(DlBwpInfo) * MAX_NUM_BWP));
      }
      macUeCfg->spCellCfg.servCellCfg.firstActvDlBwpId = duMacUeCfg->spCellCfg.servCellCfg.firstActvDlBwpId;
      macUeCfg->spCellCfg.servCellCfg.defaultDlBwpId = duMacUeCfg->spCellCfg.servCellCfg.defaultDlBwpId;
      macUeCfg->spCellCfg.servCellCfg.bwpInactivityTmr = duMacUeCfg->spCellCfg.servCellCfg.bwpInactivityTmr;
      memcpy(&macUeCfg->spCellCfg.servCellCfg.pdschServCellCfg, &duMacUeCfg->spCellCfg.servCellCfg.pdschServCellCfg, sizeof(PdschServCellCfg));
      memcpy(&macUeCfg->spCellCfg.servCellCfg.initUlBwp, &duMacUeCfg->spCellCfg.servCellCfg.initUlBwp, sizeof(InitialUlBwp));
      macUeCfg->spCellCfg.servCellCfg.numUlBwpToAdd = duMacUeCfg->spCellCfg.servCellCfg.numUlBwpToAddOrMod;
      if(macUeCfg->spCellCfg.servCellCfg.numUlBwpToAdd > 0)
      {
         memcpy(macUeCfg->spCellCfg.servCellCfg.ulBwpToAddList, duMacUeCfg->spCellCfg.servCellCfg.ulBwpToAddOrModList,\
                 (sizeof(UlBwpInfo) * MAX_NUM_BWP));
      }
      macUeCfg->spCellCfg.servCellCfg.firstActvUlBwpId =  duMacUeCfg->spCellCfg.servCellCfg.firstActvUlBwpId;
   }
   if(duMacUeCfg->ambrCfg != NULLP)
   {
      memcpy(macUeCfg->ambrCfg, duMacUeCfg->ambrCfg, sizeof(AmbrCfg));
   }
   memcpy(&macUeCfg->dlModInfo, &duMacUeCfg->dlModInfo, sizeof(ModulationInfo));
   memcpy(&macUeCfg->ulModInfo, &duMacUeCfg->ulModInfo, sizeof(ModulationInfo));
   if(duMacUeCfg->numLcs > 0)
   {
      macUeCfg->numLcs = 0;
      for(lcIdx = 0; lcIdx < duMacUeCfg->numLcs; lcIdx++)
      {
         if(duMacUeCfg->lcCfgList[lcIdx].configType == CONFIG_ADD)
         {
            //VS: To thoroughly check
            memcpy(&macUeCfg->lcCfgList[macUeCfg->numLcs], &duMacUeCfg->lcCfgList[lcIdx].lcConfig, sizeof(LcCfg));
            macUeCfg->numLcs++;
         }
      }
   }
}

/**********************************************************************************
 *
 * @brief Fills Ue Cfg from DU DB to RLCUeCfg
 *
 * @details
 *
 *    Function : fillRlcUeCfg
 *
 *    Functionality: fills ue Cfg to be sent to RLC
 *
 * @params[in] DuRlcUeCfg Pointer
 *             RlcUeCreate Pointer
 *
 * @return ROK     - success
 *         RFAILED - failure
 * *******************************************************************************/
void fillRlcUeCfg(DuRlcUeCfg *duRlcUeCfg, RlcUeCreate *rlcUeCfg)
{
   uint8_t lcIdx = 0;

   rlcUeCfg->cellId = duRlcUeCfg->cellId;
   rlcUeCfg->ueId   = duRlcUeCfg->ueId;
   
   rlcUeCfg->numLcsToAdd = 0;
   for(lcIdx = 0; lcIdx < duRlcUeCfg->numLcs; lcIdx++)
   {
      if(duRlcUeCfg->rlcLcCfg[lcIdx].configType == CONFIG_ADD)
      {
         memcpy(&rlcUeCfg->rlcLcCfgAdd[rlcUeCfg->numLcsToAdd], &duRlcUeCfg->rlcLcCfg[lcIdx].rlcBearerCfg, sizeof(RlcBearerCfg));
         rlcUeCfg->numLcsToAdd++;
      }
   }
   return;
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
 *              ueId,
 *              crnti,
 *              UE config extracted from F1AP msg
 *              MAC UE config struct to be filled
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t duBuildAndSendUeCreateReqToMac(uint16_t cellId, uint8_t gnbDuUeF1apId, uint16_t crnti, DuUeCfg *ueCfgDb, DuMacUeCfg *duMacUeCfg)
{
   uint8_t  ret = ROK;
   MacUeCreateReq *macUeCfg = NULLP;
   Pst       pst;
   memset(&pst, 0, sizeof(Pst));


   ret = updateDuMacUeCfg(cellId, gnbDuUeF1apId, crnti, ueCfgDb, duMacUeCfg);
   if(ret == RFAILED)
   {
      DU_LOG("\nERROR  -->  DU APP : Failed to fill MacUeCfg at duBuildAndSendUeCreateReqToMac()");
      return RFAILED;
   }

   /* Fill Pst */
   FILL_PST_DUAPP_TO_MAC(pst, EVENT_MAC_UE_CREATE_REQ);

   /* Copying ueCb to a sharable buffer */
   DU_ALLOC_SHRABL_BUF(macUeCfg, sizeof(MacUeCreateReq));
   if(macUeCfg)
   {
      memset(macUeCfg, 0, sizeof(MacUeCreateReq));
      fillMacUeCfg(duMacUeCfg, macUeCfg); 
      DU_LOG("\nDEBUG   -->  DU_APP: Sending UE create request to MAC");

      /* Processing one Ue at a time to MAC */
      ret = (*packMacUeCreateReqOpts[pst.selector])(&pst, macUeCfg);
      if(ret == RFAILED)
      {
         DU_LOG("\nERROR  -->  DU_APP : Failure in sending Ue Create Req to MAC at duBuildAndSendUeCreateReqToMac()");
         DU_FREE_SHRABL_BUF(DU_APP_MEM_REGION, DU_POOL, macUeCfg, sizeof(MacUeCreateReq));
      }
   }
   else
   {
      DU_LOG("\n ERROR  -->  DU APP : Memory alloc failed at duBuildAndSendUeCreateReqToMac()");
      ret = RFAILED;
   }
   return ret;
}

/*******************************************************************
 *
 * @brief Build and send RACH Resource request to MAC
 *
 * @details
 *
 *    Function : duBuildAndSendRachRsrcReqToMac
 *    Functionality:
 *        Build and send RACH Resource request to MAC
 *
 * @params[in] Cell Id
 *             UE Id
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t duBuildAndSendRachRsrcReqToMac(uint16_t cellId, uint16_t ueId)
{
   uint16_t cellIdx = 0, ssbIdx = 0;
   Pst pst;
   MacRachRsrcReq *rachRsrcReq = NULLP;

   GET_CELL_IDX(cellId, cellIdx);
   if(duCb.actvCellLst[cellIdx] == NULLP)
   {
      DU_LOG("\nERROR  -->  DU APP : Cell Id [%d] not found in duBuildAndSendRachRsrcReqToMac()", cellId);
      return RFAILED;
   }

   DU_ALLOC_SHRABL_BUF(rachRsrcReq, sizeof(MacRachRsrcReq));
   if(!rachRsrcReq)
   {
      DU_LOG("\nERROR  -->  DU APP : Failed to allocate memory for RACH Resource Request in \
            duBuildAndSendRachRsrcReqToMac()");
      return RFAILED;
   }

   rachRsrcReq->cellId = cellId;
   rachRsrcReq->ueId = ueId;
   rachRsrcReq->numSsb = duCfgParam.macCellCfg.prachCfg.ssbPerRach;
   for(ssbIdx = 0; ssbIdx < rachRsrcReq->numSsb; ssbIdx++)
   {
      rachRsrcReq->ssbIdx[ssbIdx] = ssbIdx;
   }

   /* Fill Pst */
   FILL_PST_DUAPP_TO_MAC(pst, EVENT_MAC_RACH_RESOURCE_REQ);
   
   if(((*packMacRachRsrcReqOpts[pst.selector])(&pst, rachRsrcReq)) != ROK)
   {
      DU_LOG("\nERROR  -->  DU_APP : Failure in sending RACH Resource Request to MAC at \
            duBuildAndSendRachRsrcReqToMac()");
      DU_FREE_SHRABL_BUF(DU_APP_MEM_REGION, DU_POOL, rachRsrcReq, sizeof(MacRachRsrcReq));
      return RFAILED;
   }

   return ROK;
}

/*******************************************************************
 *
 * @brief Process RACH resource response from MAC
 *
 * @details
 *
 *    Function : DuProcMacRachRsrcRsp
 *    Functionality:
 *        Process RACH resource response from MAC
 *
 * @params[in] Post structure
 *             RACH resource response
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t DuProcMacRachRsrcRsp(Pst *pst, MacRachRsrcRsp *rachRsrcRsp)
{
   uint8_t  ret = RFAILED;
   uint16_t cellIdx = 0;
   DuCellCb *cellCb = NULLP;
   DuUeCb   *ueCb = NULLP;

   DU_LOG("\nINFO  -->  DU APP : Received RACH Resource Response from MAC. Cell ID [%d] UE ID [%d]",
         rachRsrcRsp->cellId, rachRsrcRsp->ueId);

   if(rachRsrcRsp->result == MAC_DU_APP_RSP_OK)
   {
      DU_LOG("\nINFO : DU APP : RACH Resource Response from MAC : Result [SUCCESS]");

      /* Fetch Cell Cb */
      GET_CELL_IDX(rachRsrcRsp->cellId, cellIdx);
      if(duCb.actvCellLst[cellIdx] && (duCb.actvCellLst[cellIdx]->cellId == rachRsrcRsp->cellId))
      {
         cellCb = duCb.actvCellLst[cellIdx];

         /* Fetch UE CB */
         if(cellCb->ueCb[rachRsrcRsp->ueId-1].gnbDuUeF1apId == rachRsrcRsp->ueId)
         {
            ueCb = &cellCb->ueCb[rachRsrcRsp->ueId-1];

            /* Store CRNTI alloted to this UE by MAC */
            ueCb->crnti = rachRsrcRsp->newCrnti;

            /* Store the assigned CF-RA resources */
            memcpy(&ueCb->cfraResource, &rachRsrcRsp->cfraResource, sizeof(MacCfraResource));

            /* RACH resources allocated to UE is sent to CU in UE Context Setup Response
             * along with the result of UE Context setup requested by CU */
            if((ret = BuildAndSendUeCtxtRsp(rachRsrcRsp->cellId, rachRsrcRsp->ueId)) != ROK)
               DU_LOG("\nERROR  ->  DU APP : Failure in BuildAndSendUeCtxtRsp()");
         }
         else
            DU_LOG("\nERROR  -->  DU APP : UE ID [%d] not found in DuProcMacRachRsrcRsp", rachRsrcRsp->ueId);
      }
      else
         DU_LOG("\nERROR  -->  DU APP : Cell ID [%d] not found in DuProcMacRachRsrcRsp", rachRsrcRsp->cellId);
   }
   else
      DU_LOG("\nINFO : DU APP : RACH Resource Response from MAC : Result [FAILURE]");

   DU_FREE_SHRABL_BUF(DU_APP_MEM_REGION, DU_POOL, rachRsrcRsp, sizeof(MacRachRsrcRsp));
   return ret;
}

/*******************************************************************
 *
 * @brief  fill k0 value in duCb
 *
 * @details
 *
 *    Function : fillK0Values
 *    Functionality:  update k0 value in duCb 
 *
 * @params[in] PdschConfig *cuPdschCfg, PdschConfig *storePdschCfg 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t fillK0Values(PdschConfig *cuPdschCfg, PdschConfig *storePdschCfg)
{
   uint8_t numTimeDomRsrcAlloc, rsrcIdx;

   if(cuPdschCfg)
   {
      if(storePdschCfg->numTimeDomRsrcAlloc)
      {
         numTimeDomRsrcAlloc = cuPdschCfg->numTimeDomRsrcAlloc;
         for(rsrcIdx =0 ; rsrcIdx<numTimeDomRsrcAlloc; rsrcIdx++)
         {
            if(cuPdschCfg->timeDomRsrcAllociList[rsrcIdx].k0)
            {
               if(storePdschCfg->timeDomRsrcAllociList[rsrcIdx].k0)
               {
                  *(storePdschCfg->timeDomRsrcAllociList[rsrcIdx].k0) = *(cuPdschCfg->timeDomRsrcAllociList[rsrcIdx].k0);
                  if(storePdschCfg->timeDomRsrcAllociList[rsrcIdx].k0 != cuPdschCfg->timeDomRsrcAllociList[rsrcIdx].k0)
                     DU_FREE_SHRABL_BUF(DU_APP_MEM_REGION, DU_POOL, cuPdschCfg->timeDomRsrcAllociList[rsrcIdx].k0, sizeof(uint8_t));
               }
               else
               {
                  (storePdschCfg->timeDomRsrcAllociList[rsrcIdx].k0) = (cuPdschCfg->timeDomRsrcAllociList[rsrcIdx].k0);
               }
            }
         }
      }
   }
   return ROK;
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
 *             F1UeContextSetupDb pointer 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t duUpdateMacCfg(DuMacUeCfg *macUeCfg, F1UeContextSetupDb *f1UeDb) 
{
   uint8_t ret, lcIdx, dbIdx, numLcs, lcDelIdx, cellIdx;
   DuMacUeCfg *oldMacUeCfg;
   ret = ROK;

   GET_CELL_IDX(macUeCfg->cellId, cellIdx);
   if(duCb.actvCellLst[cellIdx] == NULLP)
   {
      DU_LOG("\nERROR  --> DU APP: CellId[%d] not found", macUeCfg->cellId);
      return RFAILED;
   }
   oldMacUeCfg = &duCb.actvCellLst[cellIdx]->ueCb[macUeCfg->ueId-1].duMacUeCfg;

   /*Filling Cell Group Cfg*/
   ret =  procUeRecfgCellInfo(macUeCfg, &f1UeDb->duUeCfg.copyOfmacUeCfg, f1UeDb->duUeCfg.cellGrpCfg);
#ifdef NR_DRX
   memcpy(&macUeCfg->macCellGrpCfg.drxCfg, &f1UeDb->duUeCfg.copyOfmacUeCfg.macCellGrpCfg.drxCfg, sizeof(DrxCfg));
#endif
   if(ret == ROK)
   {
      if(macUeCfg->spCellCfg.servCellCfg.initDlBwp.pdschPresent)
      {
         /* update k0 values */
         fillK0Values(&f1UeDb->duUeCfg.copyOfmacUeCfg.spCellCfg.servCellCfg.initDlBwp.pdschCfg, &macUeCfg->spCellCfg.servCellCfg.initDlBwp.pdschCfg); 
         fillStartSymbolAndLen(macUeCfg->spCellCfg.servCellCfg.initDlBwp.pdschCfg.numTimeDomRsrcAlloc,\
               &macUeCfg->spCellCfg.servCellCfg.initDlBwp.pdschCfg, NULL);
      }
      if(macUeCfg->spCellCfg.servCellCfg.initUlBwp.puschPresent)
      {
         fillStartSymbolAndLen(macUeCfg->spCellCfg.servCellCfg.initUlBwp.puschCfg.numTimeDomRsrcAlloc,\
               NULL, &macUeCfg->spCellCfg.servCellCfg.initUlBwp.puschCfg);
      }
      ret = fillAmbr(NULL, f1UeDb->duUeCfg.ambrCfg, &oldMacUeCfg->ambrCfg, true);
      duFillModulationDetails(macUeCfg, oldMacUeCfg, f1UeDb->duUeCfg.ueNrCapability);
   }

   /* Filling LC Context */
   for(dbIdx = 0; (dbIdx < f1UeDb->duUeCfg.numMacLcs && ret == ROK); dbIdx++)
   {
      numLcs = macUeCfg->numLcs;
      for(lcIdx = 0; lcIdx < numLcs; lcIdx++)
      {
         if(f1UeDb->duUeCfg.macLcCfg[dbIdx].lcConfig.lcId ==  macUeCfg->lcCfgList[lcIdx].lcConfig.lcId)
         {
            if(f1UeDb->duUeCfg.macLcCfg[dbIdx].configType == CONFIG_MOD)
            {
               ret = fillMacLcCfgToAddMod(NULL, &f1UeDb->duUeCfg.macLcCfg[dbIdx], &macUeCfg->lcCfgList[lcIdx], true);
            }
            else if(f1UeDb->duUeCfg.macLcCfg[dbIdx].configType == CONFIG_DEL)
            {
               /* Free memory at matched  lcIdx index */
               freeMacLcCfg(&macUeCfg->lcCfgList[lcIdx].lcConfig);
               macUeCfg->numLcs--;
               for(lcDelIdx = lcIdx; lcDelIdx < macUeCfg->numLcs; lcDelIdx++)
               {
                  /* moving all elements one index ahead */
                  ret = fillMacLcCfgToAddMod(NULL,  &macUeCfg->lcCfgList[lcDelIdx+1], &macUeCfg->lcCfgList[lcDelIdx], true);
                  freeMacLcCfg(&macUeCfg->lcCfgList[lcDelIdx+1].lcConfig);
                  if(ret == RFAILED)
                  {
                     DU_LOG("\nERROR  -->  DU APP : Failed to delete LC at Idx %d in duUpdateMacCfg()", lcDelIdx);
                     break;
                  }
               }
            }
         }
      } 
      if(f1UeDb->duUeCfg.macLcCfg[dbIdx].configType == CONFIG_ADD)
      {
         ret = fillMacLcCfgToAddMod(NULL, &f1UeDb->duUeCfg.macLcCfg[dbIdx], &macUeCfg->lcCfgList[numLcs], true);
         if(ret == RFAILED)
         {
            DU_LOG("\nERROR  -->  DU APP : Failed to add LC at Idx %d in duUpdateMacCfg()", numLcs);
            break;
         }
         macUeCfg->numLcs++;
      }

   }
   return ret;
}

/******************************************************************
 *
 * @brief Function to fill the RLC Lc cfg from ueSetupReqDb
 *
 * @details
 *
 *    Function : fillRlcCfgToAddMod
 *
 *    Functionality: Function to fill the RLC Lc cfg from ueSetupReqDb
 *
 *
 *****************************************************************/

uint8_t fillRlcCfgToAddMod(DuRlcBearerCfg *lcCfg, DuRlcBearerCfg *f1UeDbLcCfg)
{
   lcCfg->configType = f1UeDbLcCfg->configType;
   lcCfg->rlcBearerCfg.rbId       = f1UeDbLcCfg->rlcBearerCfg.rbId;
   lcCfg->rlcBearerCfg.rbType     = f1UeDbLcCfg->rlcBearerCfg.rbType;
   lcCfg->rlcBearerCfg.lcId       = f1UeDbLcCfg->rlcBearerCfg.lcId;
   lcCfg->rlcBearerCfg.lcType     = f1UeDbLcCfg->rlcBearerCfg.lcType;
   lcCfg->rlcBearerCfg.rlcMode    = f1UeDbLcCfg->rlcBearerCfg.rlcMode;
   
   switch(lcCfg->rlcBearerCfg.rlcMode)
   {
      case RLC_AM :
         {
            if(!lcCfg->rlcBearerCfg.u.amCfg)
            {
               DU_ALLOC_SHRABL_BUF(lcCfg->rlcBearerCfg.u.amCfg, sizeof(AmBearerCfg));
               if(!lcCfg->rlcBearerCfg.u.amCfg)
                  return RFAILED;
            }
            /* DL AM */
            lcCfg->rlcBearerCfg.u.amCfg->dlAmCfg.snLenDl     = f1UeDbLcCfg->rlcBearerCfg.u.amCfg->dlAmCfg.snLenDl;    
            lcCfg->rlcBearerCfg.u.amCfg->dlAmCfg.pollRetxTmr = f1UeDbLcCfg->rlcBearerCfg.u.amCfg->dlAmCfg.pollRetxTmr;
            lcCfg->rlcBearerCfg.u.amCfg->dlAmCfg.pollPdu     = f1UeDbLcCfg->rlcBearerCfg.u.amCfg->dlAmCfg.pollPdu;
            lcCfg->rlcBearerCfg.u.amCfg->dlAmCfg.pollByte    = f1UeDbLcCfg->rlcBearerCfg.u.amCfg->dlAmCfg.pollByte;   
            lcCfg->rlcBearerCfg.u.amCfg->dlAmCfg.maxRetxTh   = f1UeDbLcCfg->rlcBearerCfg.u.amCfg->dlAmCfg.maxRetxTh;   

            /* UL AM */
            lcCfg->rlcBearerCfg.u.amCfg->ulAmCfg.snLenUl     = f1UeDbLcCfg->rlcBearerCfg.u.amCfg->ulAmCfg.snLenUl;
            lcCfg->rlcBearerCfg.u.amCfg->ulAmCfg.reAssemTmr  = f1UeDbLcCfg->rlcBearerCfg.u.amCfg->ulAmCfg.reAssemTmr; 
            lcCfg->rlcBearerCfg.u.amCfg->ulAmCfg.statProhTmr = f1UeDbLcCfg->rlcBearerCfg.u.amCfg->ulAmCfg.statProhTmr;
            break;
         }
      case RLC_UM_BI_DIRECTIONAL :
         {
            if(!lcCfg->rlcBearerCfg.u.umBiDirCfg)
            {
               DU_ALLOC_SHRABL_BUF(lcCfg->rlcBearerCfg.u.umBiDirCfg, sizeof(UmBiDirBearerCfg));
               if(!lcCfg->rlcBearerCfg.u.umBiDirCfg)
                  return RFAILED;
            }
            /* UL UM BI DIR INFO */
            lcCfg->rlcBearerCfg.u.umBiDirCfg->ulUmCfg.snLenUlUm  = f1UeDbLcCfg->rlcBearerCfg.u.umBiDirCfg->ulUmCfg.snLenUlUm;  
            lcCfg->rlcBearerCfg.u.umBiDirCfg->ulUmCfg.reAssemTmr = f1UeDbLcCfg->rlcBearerCfg.u.umBiDirCfg->ulUmCfg.reAssemTmr;
            /* DL UM BI DIR INFO */
            lcCfg->rlcBearerCfg.u.umBiDirCfg->dlUmCfg.snLenDlUm  = f1UeDbLcCfg->rlcBearerCfg.u.umBiDirCfg->dlUmCfg.snLenDlUm;
            break;
         }
      case RLC_UM_UNI_DIRECTIONAL_UL :
         {
            if(!lcCfg->rlcBearerCfg.u.umUniDirUlCfg)
            {
               DU_ALLOC_SHRABL_BUF(lcCfg->rlcBearerCfg.u.umUniDirUlCfg, sizeof(UmUniDirUlBearerCfg));
               if(!lcCfg->rlcBearerCfg.u.umUniDirUlCfg)
                  return RFAILED;
            }
            lcCfg->rlcBearerCfg.u.umUniDirUlCfg->ulUmCfg.snLenUlUm  = f1UeDbLcCfg->rlcBearerCfg.u.umUniDirUlCfg->ulUmCfg.snLenUlUm;  
            lcCfg->rlcBearerCfg.u.umUniDirUlCfg->ulUmCfg.reAssemTmr = f1UeDbLcCfg->rlcBearerCfg.u.umUniDirUlCfg->ulUmCfg.reAssemTmr;
            break;

         }
      case RLC_UM_UNI_DIRECTIONAL_DL :
         {
            if(!lcCfg->rlcBearerCfg.u.umUniDirDlCfg)
            {
               DU_ALLOC_SHRABL_BUF(lcCfg->rlcBearerCfg.u.umUniDirDlCfg, sizeof(UmUniDirDlBearerCfg));
               if(!lcCfg->rlcBearerCfg.u.umUniDirDlCfg)
                  return RFAILED;
            }
            lcCfg->rlcBearerCfg.u.umUniDirDlCfg->dlUmCfg.snLenDlUm  = f1UeDbLcCfg->rlcBearerCfg.u.umUniDirDlCfg->dlUmCfg.snLenDlUm;
            break;
         }
      default:
         DU_LOG("\nERROR  -->  DU_APP: Invalid Rlc Mode %d at fillRlcCfgToAddMod()", lcCfg->rlcBearerCfg.rlcMode);
         return RFAILED;
   }
   return ROK;
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
 *             F1UeContextSetupDb pointer 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t duUpdateRlcLcCfg(DuRlcUeCfg *rlcUeCfg, F1UeContextSetupDb *f1UeDb)
{
   uint8_t ret, lcIdx, dbIdx, numLcs, lcDelIdx;

   ret = ROK;
   for(dbIdx = 0; (dbIdx < f1UeDb->duUeCfg.numRlcLcs && ret ==ROK); dbIdx++)
   {
      numLcs = rlcUeCfg->numLcs;
      for(lcIdx = 0; lcIdx < numLcs; lcIdx++)
      {
         if(f1UeDb->duUeCfg.rlcLcCfg[dbIdx].rlcBearerCfg.lcId == rlcUeCfg->rlcLcCfg[lcIdx].rlcBearerCfg.lcId)
         {
            if(f1UeDb->duUeCfg.rlcLcCfg[dbIdx].configType == CONFIG_MOD)
            {
               ret = fillRlcCfgToAddMod(&rlcUeCfg->rlcLcCfg[lcIdx], &f1UeDb->duUeCfg.rlcLcCfg[dbIdx]);
               if(ret == RFAILED)
               {
                  DU_LOG("\nERROR  -->  DU APP : Failed to modify LC at Idx %d in duUpdateRlcCfg()", lcDelIdx);
                  break;
               }
               fillSnssaiInfo(NULL, f1UeDb->duUeCfg.rlcLcCfg[dbIdx].rlcBearerCfg.snssai,\
                                  &rlcUeCfg->rlcLcCfg[lcIdx].rlcBearerCfg.snssai, true);
            }
            else if(f1UeDb->duUeCfg.rlcLcCfg[dbIdx].configType == CONFIG_DEL)
            {
               /* Free memory at matched lcIdx index */
               freeRlcLcCfg(&rlcUeCfg->rlcLcCfg[lcIdx].rlcBearerCfg);
               rlcUeCfg->numLcs--;
               for(lcDelIdx = lcIdx; lcDelIdx < rlcUeCfg->numLcs; lcDelIdx++)
               {
                  ret = fillRlcCfgToAddMod(&rlcUeCfg->rlcLcCfg[lcDelIdx], &rlcUeCfg->rlcLcCfg[lcDelIdx+1]);
                  fillSnssaiInfo(NULL, rlcUeCfg->rlcLcCfg[lcDelIdx+1].rlcBearerCfg.snssai,\
                                   &rlcUeCfg->rlcLcCfg[lcDelIdx].rlcBearerCfg.snssai, true);
                  freeRlcLcCfg(&rlcUeCfg->rlcLcCfg[lcDelIdx+1].rlcBearerCfg);
                  if(ret == RFAILED)
                  {
                     DU_LOG("\nERROR  -->  DU APP : Failed to delete LC at Idx %d in duUpdateRlcCfg()", lcDelIdx);
                     break;
                  }
               }
            }
         }
      }
      if(f1UeDb->duUeCfg.rlcLcCfg[dbIdx].configType == CONFIG_ADD)
      {
         ret = fillRlcCfgToAddMod(&rlcUeCfg->rlcLcCfg[lcIdx], &f1UeDb->duUeCfg.rlcLcCfg[dbIdx]);
         if(ret == ROK)
         {    
            fillSnssaiInfo(NULL, f1UeDb->duUeCfg.rlcLcCfg[dbIdx].rlcBearerCfg.snssai,\
                              &rlcUeCfg->rlcLcCfg[rlcUeCfg->numLcs].rlcBearerCfg.snssai, true);
            rlcUeCfg->numLcs++;
         }
      }
   }
   return ret;
}

/*******************************************************************
 *
 * @brief Function to fill Tunnel Config to Add/Mod
 * 
 *
 * @details
 *
 *    Function : fillTnlCfgToAddMod
 *
 *    Functionality: Function to fill tunnel Config to Add/Mod
 *
 * @params[in] Pointer to tnlCfgDb,
 *             pointer to f1TnlCfg
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t fillTnlCfgToAddMod(UpTnlCfg **ueCbTnlCfg, UpTnlCfg *f1TnlCfg)
{
   if(*ueCbTnlCfg)
   {
      DU_FREE((*ueCbTnlCfg)->tnlCfg1, sizeof(GtpTnlCfg));
      DU_FREE(*ueCbTnlCfg, sizeof(UpTnlCfg));
   }

   if(*ueCbTnlCfg == NULLP)
   {
      /* copying to DuCb Tnl Cfg */
      DU_ALLOC(*ueCbTnlCfg, sizeof(UpTnlCfg));
      if(*ueCbTnlCfg == NULLP)
      {
         DU_LOG("\nERROR  -->  DU_APP : fillTnlCfgToAddMod: Memory Alloc failed for drbId[%d]", f1TnlCfg->drbId);
         return RFAILED;
      }
   }
   memset(*ueCbTnlCfg, 0, sizeof(UpTnlCfg));
   (*ueCbTnlCfg)->configType = f1TnlCfg->configType;
   (*ueCbTnlCfg)->cellId    = f1TnlCfg->cellId;
   (*ueCbTnlCfg)->ueId      = f1TnlCfg->ueId;
   (*ueCbTnlCfg)->drbId     = f1TnlCfg->drbId;
   if(f1TnlCfg->tnlCfg1)
   {
      if((*ueCbTnlCfg)->tnlCfg1 == NULLP)
      {
         DU_ALLOC((*ueCbTnlCfg)->tnlCfg1, sizeof(GtpTnlCfg));
         if((*ueCbTnlCfg)->tnlCfg1 == NULLP)
         {
            DU_LOG("\nERROR  -->  DU_APP : fillTnlCfgToAddMod: Memory Alloc failed for tnlCfg1 for drbId[%d]", f1TnlCfg->drbId);
            return RFAILED;
         }
      }
      memset((*ueCbTnlCfg)->tnlCfg1, 0, sizeof(GtpTnlCfg));
      (*ueCbTnlCfg)->tnlCfg1->teId = f1TnlCfg->tnlCfg1->teId;
      (*ueCbTnlCfg)->tnlCfg1->ulTnlAddress = f1TnlCfg->tnlCfg1->ulTnlAddress;
      (*ueCbTnlCfg)->tnlCfg1->dlTnlAddress = f1TnlCfg->tnlCfg1->dlTnlAddress;
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Processing the tunnel Request to EGTP
 *        
 * @details
 *
 *    Function : duProcEgtpTunnelCfg
 *
 *    Functionality: Processing the tunnel Request to EGTP
 *
 * @params[in] UptnlCfg *duTnlCfg, UpTnlCfg *f1TnlCfg 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t duProcEgtpTunnelCfg(uint8_t ueCbIdx, UpTnlCfg *duTnlCfg, UpTnlCfg *f1TnlCfg)
{
   uint8_t ret = RFAILED, delIdx;

   if(f1TnlCfg->tnlCfg1 == NULLP)
   {
      DU_LOG("\nERROR  -->  DU_APP : Tunnel config not found");
      return ret;
   }

   if(f1TnlCfg->configType == CONFIG_ADD)
   {
      if(duSendEgtpTnlMgmtReq(EGTP_TNL_MGMT_ADD, NULLP, f1TnlCfg->tnlCfg1) == ROK)
      {
         if(fillTnlCfgToAddMod(&duCb.upTnlCfg[duCb.numTeId], f1TnlCfg) == ROK)
         {
            duCb.numTeId++;
            ret = ROK;
         }
      }      
   }
   else if(f1TnlCfg->configType == CONFIG_MOD)
   {
      if(duSendEgtpTnlMgmtReq(EGTP_TNL_MGMT_MOD, duTnlCfg->tnlCfg1->teId, f1TnlCfg->tnlCfg1) == ROK)
      {
         if(fillTnlCfgToAddMod(&duTnlCfg, f1TnlCfg) == ROK)
         {
            ret = ROK;
         }
      }   
   }
   else if(f1TnlCfg->configType == CONFIG_DEL)
   {
      if(duSendEgtpTnlMgmtReq(EGTP_TNL_MGMT_DEL, duTnlCfg->tnlCfg1->teId, f1TnlCfg->tnlCfg1) == ROK)
      {	   
         /* Free memory at drbIdx */
         duCb.numTeId--;
         for(delIdx = ueCbIdx; delIdx < duCb.numTeId; delIdx++)
         {
            /* moving all elements one index ahead */
            ret = fillTnlCfgToAddMod(&duCb.upTnlCfg[delIdx], duCb.upTnlCfg[delIdx+1]);
            if(ret != ROK)
            {
               return ret;
            }
         }
         if(duCb.upTnlCfg[delIdx])
         {
            DU_FREE(duCb.upTnlCfg[delIdx]->tnlCfg1, sizeof(GtpTnlCfg));
            DU_FREE(duCb.upTnlCfg[delIdx], sizeof(UpTnlCfg));
         }
      }   
   }
   return ret;
}

/***********************************************************************
 *
 * @brief Function to fill Tunnel Config
 *        and sends tunnel Req to EGTP
 * 
 *
 * @details
 *
 *    Function : duUpdateTunnelCfgDb
 *
 *    Functionality: Function to fill tunnel Config
 *                   and sends tunnel Cfg Req to EGTP
 *
 * @params[in] ueId, cellId, DuUeCfg 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t duUpdateTunnelCfgDb(uint8_t ueId, uint8_t cellId, DuUeCfg *duUeCfg)
{
   uint8_t ret = ROK, drbIdx, teIdx;
   bool drbFound = false;

   /*If Add/Mod tunnels request for that DRB is successful in EGTP */
   /*then update drbId and tunnel Info in duCb */
   for(drbIdx=0; drbIdx < duUeCfg->numDrb; drbIdx++)
   {
      duUeCfg->upTnlInfo[drbIdx].cellId = cellId;
      duUeCfg->upTnlInfo[drbIdx].ueId = ueId;
      for(teIdx = 0; teIdx < duCb.numTeId; teIdx++)
      {
         if((duCb.upTnlCfg[teIdx]->ueId == duUeCfg->upTnlInfo[drbIdx].ueId) && \
            (duCb.upTnlCfg[teIdx]->drbId == duUeCfg->upTnlInfo[drbIdx].drbId))
         {
            drbFound = true; /* existing DRB */
            if(duProcEgtpTunnelCfg(teIdx, duCb.upTnlCfg[teIdx], &duUeCfg->upTnlInfo[drbIdx]) != ROK)
            {
               DU_LOG("\nERROR  -> DU_APP : duUpdateTunnelCfgDb: Failed to modify tunnel req for Drb id[%d]",
                     duUeCfg->upTnlInfo[drbIdx].drbId);
               ret = RFAILED;
            }
            break;
         }
         else
            drbFound = false;
      }

      if(!drbFound && ret == ROK)/* new DRB to Add */
      {
         if(duProcEgtpTunnelCfg(NULLP, NULLP, &duUeCfg->upTnlInfo[drbIdx]) != ROK)
         {
            DU_LOG("\nERROR  -> DU_APP : duUpdateTunnelCfgDb: Failed to add tunnel req for Drb id[%d]",
                  duUeCfg->upTnlInfo[drbIdx].drbId);
            ret = RFAILED;
            break;
         }
      }
   }
   return ret;
}

/*******************************************************************
 *
 * @brief @brief To update DuUeCb Mac and Rlc Ue Cfg
 * 
 *
 * @details
 *
 *    Function : duUpdateDuUeCbCfg
 *
 *    Functionality: update DuUeCb Mac and Rlc Ue Cfg
 *
 * @params[in] ueId, cellIdx 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t duUpdateDuUeCbCfg(uint8_t ueId, uint8_t cellId)
{
   uint8_t ret = ROK;
   uint16_t cellIdx = 0, crnti=0;
   DuUeCb *ueCb = NULLP;

   GET_CELL_IDX(cellId, cellIdx);
   ueCb = &duCb.actvCellLst[cellIdx]->ueCb[ueId-1];

   /*Filling RLC Ue Cfg */
   ueCb->duRlcUeCfg.cellId = cellId;
   ueCb->duRlcUeCfg.ueId   = ueId;
   ret = duUpdateRlcLcCfg(&ueCb->duRlcUeCfg, ueCb->f1UeDb);
   if(ret == ROK)
   {
      /*Filling MAC Ue Cfg */
      GET_CRNTI(crnti, ueId);
      ueCb->duMacUeCfg.cellId = cellId;
      ueCb->duMacUeCfg.ueId  = ueId;
      ueCb->duMacUeCfg.crnti  = crnti;
      ret = duUpdateMacCfg(&ueCb->duMacUeCfg, ueCb->f1UeDb);
      if(ret == RFAILED)
         DU_LOG("\nERROR  -->  DU APP : Failed while updating MAC LC Config at duUpdateDuUeCbCfg()");
      else
      {
         if(duUpdateTunnelCfgDb(ueId, cellId, &ueCb->f1UeDb->duUeCfg) != ROK)
         {
            DU_LOG("\nERROR  -->  DU_APP : Failed to establish tunnel in duUpdateDuUeCbCfg()");
            return RFAILED;
         }
      }
   }
   else
      DU_LOG("\nERROR  -->  DU APP : Failed while updating RLC LC Config at duUpdateDuUeCbCfg()");
   return ret;
}

/*******************************************************************
 *
 * @brief Handle UE create response from MAC
 *
 * @details
 *
 *    Function : DuProcMacUeCreateRsp
 *
 *    Functionality: Handle UE Create response from MAC
 *
 * @params[in] Pointer to MacUeCreateRsp and Pst 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t DuProcMacUeCreateRsp(Pst *pst, MacUeCreateRsp *cfgRsp)
{
   uint8_t ret = ROK;
   uint16_t cellIdx;

   if(cfgRsp)
   {
      GET_CELL_IDX(cfgRsp->cellId, cellIdx);
      if(cfgRsp->result == MAC_DU_APP_RSP_OK)
      {
         if(pst->event == EVENT_MAC_UE_CREATE_RSP)
         {
            DU_LOG("\nINFO   -->  DU APP : MAC UE Create Response : SUCCESS [DU UE F1AP ID : %d]", cfgRsp->ueId);

            if(duCb.actvCellLst[cellIdx] && (duCb.actvCellLst[cellIdx]->ueCb[cfgRsp->ueId -1].gnbDuUeF1apId == cfgRsp->ueId))
            {
               duCb.actvCellLst[cellIdx]->ueCb[cfgRsp->ueId -1].duMacUeCfg.macUeCfgState = UE_CREATE_COMPLETE;

              if((duCb.actvCellLst[cfgRsp->cellId -1]->ueCb[cfgRsp->ueId -1].ueState == UE_HANDIN_IN_PROGRESS) && 
                    (duCb.actvCellLst[cfgRsp->cellId -1]->ueCb[cfgRsp->ueId -1].duMacUeCfg.macUeCfgState == UE_CREATE_COMPLETE) &&
                     (duCb.actvCellLst[cfgRsp->cellId -1]->ueCb[cfgRsp->ueId -1].duRlcUeCfg.rlcUeCfgState == UE_CREATE_COMPLETE))
               {
                  if((ret = duUpdateDuUeCbCfg(cfgRsp->ueId, cfgRsp->cellId)) == ROK)
                  {
                     /* If UE is in handover, RACH resource needs to be requested
                      * from MAC for CFRA */
                     if((duBuildAndSendRachRsrcReqToMac(cfgRsp->cellId, cfgRsp->ueId)) != ROK)
                     {
                        DU_LOG("\nERROR  --> DU APP : Failed to send RACH Resource Request to MAC");
                        DU_FREE_SHRABL_BUF(DU_APP_MEM_REGION, DU_POOL, cfgRsp, sizeof(MacUeCreateRsp));
                        return RFAILED;
                     }
                  }
                  else
                  {
                     DU_LOG("\nERROR  ->  DU APP : Failure in updating DU UE CB");
                     DU_FREE_SHRABL_BUF(DU_APP_MEM_REGION, DU_POOL, cfgRsp, sizeof(MacUeCreateRsp));
                     return RFAILED;
                  }
               }
            }
         }
      }
      else
      {
         DU_LOG("\nERROR  -->  DU APP : MAC UE CFG Response for EVENT[%d]: FAILURE [DU UE F1AP ID : %d]", pst->event, cfgRsp->ueId);
         ret = RFAILED;
      }
      DU_FREE_SHRABL_BUF(DU_APP_MEM_REGION, DU_POOL, cfgRsp, sizeof(MacUeCreateRsp));
   }
   else
   {
      DU_LOG("\nERROR  -->  DU APP : Received MAC Ue Config Response is NULL at DuProcMacUeCreateRsp()");
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
 *    Function : DuProcMacUeRecfgRsp
 *
 *    Functionality: Handle UE ReConfig response from MAC
 *
 * @params[in] Pointer to MacUeRecfgRsp and Pst 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t DuProcMacUeRecfgRsp(Pst *pst, MacUeRecfgRsp *reCfgRsp)
{
   uint8_t ret = ROK;
   uint16_t cellIdx;

   if(reCfgRsp)
   {
      GET_CELL_IDX(reCfgRsp->cellId, cellIdx);
      if(reCfgRsp->result == MAC_DU_APP_RSP_OK)
      {
         if(pst->event == EVENT_MAC_UE_RECONFIG_RSP)
         {
            DU_LOG("\nINFO   -->  DU APP : MAC UE Reconfig Response : SUCCESS [DU UE F1AP ID : %d]", reCfgRsp->ueId);
            if(duCb.actvCellLst[cellIdx] && 
                  (duCb.actvCellLst[cellIdx]->ueCb[reCfgRsp->ueId -1].gnbDuUeF1apId == reCfgRsp->ueId))
            {
               duCb.actvCellLst[cellIdx]->ueCb[reCfgRsp->ueId -1].duMacUeCfg.macUeCfgState = UE_RECFG_COMPLETE;
               if((duCb.actvCellLst[cellIdx]->ueCb[reCfgRsp->ueId -1].duMacUeCfg.macUeCfgState == UE_RECFG_COMPLETE) &&
                     (duCb.actvCellLst[reCfgRsp->cellId -1]->ueCb[reCfgRsp->ueId -1].duRlcUeCfg.rlcUeCfgState == UE_RECFG_COMPLETE))
               {
                  if((ret = duUpdateDuUeCbCfg(reCfgRsp->ueId, reCfgRsp->cellId)) == ROK)
                  {  
                     if((BuildAndSendUeCtxtRsp(reCfgRsp->cellId, reCfgRsp->ueId)) != ROK)
                     {
                        DU_LOG("\nERROR  ->  DU APP : Failure in BuildAndSendUeCtxtRsp()");
                        DU_FREE_SHRABL_BUF(DU_APP_MEM_REGION, DU_POOL, reCfgRsp, sizeof(MacUeRecfgRsp));
                        return RFAILED;
                     }
                  }
                  else
                  {
                     DU_LOG("\nERROR  ->  DU APP : Failure in updating DU UE CB");
                     DU_FREE_SHRABL_BUF(DU_APP_MEM_REGION, DU_POOL, reCfgRsp, sizeof(MacUeRecfgRsp));
                     return RFAILED;
                  }
               }
            }
         }
      }
      else
      {
         DU_LOG("\nERROR  -->  DU APP : MAC UE RECFG Response for EVENT[%d]: FAILURE [DU UE F1AP ID : %d]", pst->event, reCfgRsp->ueId);
         if(pst->event == EVENT_MAC_UE_RECONFIG_RSP)
         {
            //TODO: Send the failure case in Ue Context Setup Response
         }
         ret = RFAILED;
      }
      DU_FREE_SHRABL_BUF(DU_APP_MEM_REGION, DU_POOL, reCfgRsp, sizeof(MacUeRecfgRsp));
   }
   else
   {
      DU_LOG("\nERROR  -->  DU APP : Received MAC Ue ReConfig Response is NULL at DuProcMacUeRecfgRsp()");
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
 *               ueId,
 *               Pointer to RlcUeCreate
 *  @return ROK     - success
 *          RFAILED - failure
 * 
 *****************************************************************/

uint8_t duBuildAndSendUeCreateReqToRlc(uint16_t cellId, uint8_t gnbDuUeF1apId, DuUeCfg *ueCfgDb, DuRlcUeCfg *duRlcUeCfg)
{
   uint8_t  ret = ROK;
   RlcUeCreate *rlcUeCfg = NULLP;
   Pst       pst;
  
   ret = updateRlcUeCfg(cellId, gnbDuUeF1apId, ueCfgDb, duRlcUeCfg);
   if(ret == RFAILED)
   {
      DU_LOG("\nERROR  -->  DU APP : Failed to fill Rlc Ue Cfg at duBuildAndSendUeCreateReqToRlc()");
      return ret;
   }

   FILL_PST_DUAPP_TO_RLC(pst, RLC_UL_INST, EVENT_RLC_UE_CREATE_REQ);
   /* Copying ueCfg to a sharable buffer */
   DU_ALLOC_SHRABL_BUF(rlcUeCfg, sizeof(RlcUeCreate));
   if(rlcUeCfg)
   {
      memset(rlcUeCfg, 0, sizeof(RlcUeCreate));
      fillRlcUeCfg(duRlcUeCfg, rlcUeCfg);

      /* Processing one Ue at a time to RLC */
      DU_LOG("\nDEBUG   -->  DU_APP: Sending UE create request to RLC UL");
      ret = (*packRlcUeCreateReqOpts[pst.selector])(&pst, rlcUeCfg);
      if(ret == RFAILED)
      {
         DU_LOG("\nERROR  -->  DU_APP : Failure in sending Ue Create Req to RLC");
         DU_FREE_SHRABL_BUF(DU_APP_MEM_REGION, DU_POOL, rlcUeCfg, sizeof(RlcUeCreate));
         ret = RFAILED;
      }
   }
   else
   {
      DU_LOG("\n ERROR  -->  DU APP : Memory alloc failed at duBuildAndSendUeCreateReqToRlc()");
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
 
 *    Function : DuProcRlcUeCreateRsp
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
uint8_t DuProcRlcUeCreateRsp(Pst *pst, RlcUeCreateRsp *cfgRsp)
{
   uint8_t ret = ROK;

   if(cfgRsp)
   {
      if(cfgRsp->result == RLC_DU_APP_RSP_OK)
      {
         if(pst->event == EVENT_RLC_UE_CREATE_RSP)
         {
            DU_LOG("\nINFO   -->  DU_APP: RLC UE Create Response : SUCCESS [UE IDX:%d]", cfgRsp->ueId);
            duCb.actvCellLst[cfgRsp->cellId -1]->ueCb[cfgRsp->ueId -1].duRlcUeCfg.rlcUeCfgState = UE_CREATE_COMPLETE;

            if((duCb.actvCellLst[cfgRsp->cellId -1]->ueCb[cfgRsp->ueId -1].ueState == UE_HANDIN_IN_PROGRESS) &&
                  (duCb.actvCellLst[cfgRsp->cellId -1]->ueCb[cfgRsp->ueId -1].duMacUeCfg.macUeCfgState == UE_CREATE_COMPLETE) &&
                  (duCb.actvCellLst[cfgRsp->cellId -1]->ueCb[cfgRsp->ueId -1].duRlcUeCfg.rlcUeCfgState == UE_CREATE_COMPLETE))
            {
               if((ret = duUpdateDuUeCbCfg(cfgRsp->ueId, cfgRsp->cellId)) == ROK)
               {
                  /* If UE is in handover, RACH resource needs to be requested
                   * from MAC for CFRA */
                  if((duBuildAndSendRachRsrcReqToMac(cfgRsp->cellId, cfgRsp->ueId)) != ROK)
                  {
                     DU_LOG("\nERROR  --> DU APP : Failed to send RACH Resource Request to MAC");
                     DU_FREE_SHRABL_BUF(DU_APP_MEM_REGION, DU_POOL, cfgRsp, sizeof(RlcUeCreateRsp));
                     return RFAILED;
                  }
               }
               else
               {
                  DU_LOG("\nERROR  -->  DU APP : Failure in updating DU UE CB");
                  DU_FREE_SHRABL_BUF(DU_APP_MEM_REGION, DU_POOL, cfgRsp, sizeof(RlcUeCreateRsp));
                  return RFAILED;
               }
            }
         }
      }
      else
      {
         DU_LOG("\nERROR  -->  DU_APP: RLC UE CREATE Response for EVENT[%d] : FAILED [UE IDX : %d, REASON :%d]",\
               pst->event, cfgRsp->ueId, cfgRsp->reason);
         ret = RFAILED;
      }
      DU_FREE_SHRABL_BUF(DU_APP_MEM_REGION, DU_POOL, cfgRsp, sizeof(RlcUeCreateRsp));
   }
   else
   {
      DU_LOG("\nERROR  -->  DU_APP: Received RLC Ue Create Response is NULL at DuProcRlcUeCreateRsp()");
      ret = RFAILED;
   }
   return ret;
}

/*******************************************************************
 *
 * @brief Processes UE reconfig Rsp received from RLC UL
 *
 * @details
 *
 
 *    Function : DuProcRlcUeReconfigRsp
 *
 *    Functionality: 
 *     Processes UE reconfig Rsp received from RLC UL
 * 
 *  @params[in]  Post structure
 *               Pointer to RlcCfgCfm
 *  @return ROK     - success
 *          RFAILED - failure
 * 
 *****************************************************************/
uint8_t DuProcRlcUeReconfigRsp(Pst *pst, RlcUeReconfigRsp *cfgRsp)
{
   uint8_t ret = ROK;

   if(cfgRsp)
   {
      if(cfgRsp->result == RLC_DU_APP_RSP_OK)
      {
         if(pst->event == EVENT_RLC_UE_RECONFIG_RSP)
         {
            DU_LOG("\nINFO   -->  DU_APP: RLC UE Reconfig Response : SUCCESS [UE IDX:%d]", cfgRsp->ueId);

            duCb.actvCellLst[cfgRsp->cellId -1]->ueCb[cfgRsp->ueId -1].duRlcUeCfg.rlcUeCfgState = UE_RECFG_COMPLETE;
            if((duCb.actvCellLst[cfgRsp->cellId -1]->ueCb[cfgRsp->ueId -1].duMacUeCfg.macUeCfgState == UE_RECFG_COMPLETE) &&
                  (duCb.actvCellLst[cfgRsp->cellId -1]->ueCb[cfgRsp->ueId -1].duRlcUeCfg.rlcUeCfgState == UE_RECFG_COMPLETE))
            {
               if((ret = duUpdateDuUeCbCfg(cfgRsp->ueId, cfgRsp->cellId)) == ROK)
               {
                  if((BuildAndSendUeCtxtRsp(cfgRsp->cellId, cfgRsp->ueId)) != ROK)
                  {
                     DU_LOG("\nERROR  -->  DU APP : Failure in BuildAndSendUeCtxtRsp");
                     DU_FREE_SHRABL_BUF(DU_APP_MEM_REGION, DU_POOL, cfgRsp, sizeof(RlcUeReconfigRsp));
                     return RFAILED;
                  }
               }
               else
               {
                  DU_LOG("\nERROR  -->  DU APP : Failure in updating DU UE CB");
                  DU_FREE_SHRABL_BUF(DU_APP_MEM_REGION, DU_POOL, cfgRsp, sizeof(RlcUeReconfigRsp));
                  return RFAILED;
               }
            }
         }
      }
      else
      {
         DU_LOG("\nERROR  -->  DU_APP: RLC UE RE-CFG Response for EVENT[%d] : FAILED [UE IDX : %d, REASON :%d]",\
               pst->event, cfgRsp->ueId, cfgRsp->reason);
         if((pst->event == EVENT_RLC_UE_RECONFIG_RSP))
         {
            //TODO: update failure case in ue Context setup Response
         }
         ret = RFAILED;
      }
      DU_FREE_SHRABL_BUF(DU_APP_MEM_REGION, DU_POOL, cfgRsp, sizeof(RlcUeReconfigRsp));
   }
   else
   {
      DU_LOG("\nERROR  -->  DU_APP: Received RLC Ue ReConfig Response is NULL at DuProcRlcUeReconfigRsp()");
      ret = RFAILED;
   }
   return ret;
}
/**********************************************************************************
 *
 * @brief Fills Ue ReCfg from DU DB to RlcUeRecfg
 *
 * @details
 *
 *    Function : fillRlcUeRecfg
 *
 *    Functionality: fills ue ReCfg to be sent to RLC
 *
 * @params[in] DuRlcUeCfg Pointer
 *             RlcUeRecfg Pointer
 *
 * @return void 
 * *******************************************************************************/
void fillRlcUeRecfg(DuRlcUeCfg *duRlcUeCfg, RlcUeRecfg *rlcUeRecfg)
{
   uint8_t lcIdx = 0;

   rlcUeRecfg->cellId = duRlcUeCfg->cellId;
   rlcUeRecfg->ueId   = duRlcUeCfg->ueId;


   if(duRlcUeCfg->numLcs > 0)
   {
      rlcUeRecfg->numLcsToAdd = 0;
      rlcUeRecfg->numLcsToMod = 0;
      rlcUeRecfg->numLcsToRel = 0;

      for(lcIdx = 0; lcIdx < duRlcUeCfg->numLcs; lcIdx++)
      {
         if(duRlcUeCfg->rlcLcCfg[lcIdx].configType == CONFIG_ADD)
         {
            memcpy(&rlcUeRecfg->rlcLcCfgAdd[rlcUeRecfg->numLcsToAdd], &duRlcUeCfg->rlcLcCfg[lcIdx].rlcBearerCfg, sizeof(RlcBearerCfg));
            rlcUeRecfg->numLcsToAdd++;
         }
         if(duRlcUeCfg->rlcLcCfg[lcIdx].configType == CONFIG_MOD)
         {
            memcpy(&rlcUeRecfg->rlcLcCfgMod[rlcUeRecfg->numLcsToMod], &duRlcUeCfg->rlcLcCfg[lcIdx].rlcBearerCfg, sizeof(RlcBearerCfg));
            rlcUeRecfg->numLcsToMod++;
         }
         if(duRlcUeCfg->rlcLcCfg[lcIdx].configType == CONFIG_DEL)
         {
            memcpy(&rlcUeRecfg->rlcLcCfgRel[rlcUeRecfg->numLcsToRel], &duRlcUeCfg->rlcLcCfg[lcIdx].rlcBearerCfg, sizeof(RlcBearerCfg));
            rlcUeRecfg->numLcsToRel++;
         }
      }
   }
   return;
}

/*******************************************************************
 *
 * @brief Builds and Send Ue Reconfig Req to RLC
 *
 * @details
 *
 *    Function : duBuildAndSendUeRecfgReqToRLC
 *
 *    Functionality: Builds and Send Ue Reconfig Req to RLC
 *
 * @params[in] uint16_t cellId,
 *             uint6_t crnti #AS per 38.473 V15.3.0, Section 9.3.1.32 crnti
 *             value range is b/w 0..65535#  
 *             DuUeCfg *ueCfgDb
 *             RlcUeCreate *rlcUeCfg
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t duBuildAndSendUeRecfgReqToRlc(uint16_t cellId, uint8_t gnbDuUeF1apId, uint16_t crnti, DuUeCfg *ueCfgDb)
{
   uint8_t ret = ROK;
   DuRlcUeCfg *duRlcUeCfg = NULLP;
   RlcUeRecfg *rlcUeRecfg = NULLP;

   DU_ALLOC(duRlcUeCfg, sizeof(DuRlcUeCfg));
   DU_ALLOC_SHRABL_BUF(rlcUeRecfg, sizeof(RlcUeRecfg));
   if(duRlcUeCfg && rlcUeRecfg)
   {
      memset(duRlcUeCfg, 0, sizeof(DuRlcUeCfg));
      memset(rlcUeRecfg, 0, sizeof(RlcUeRecfg));

      ret = updateRlcUeCfg(cellId, gnbDuUeF1apId, ueCfgDb, duRlcUeCfg);

      if(ret == RFAILED)
         DU_LOG("\nERROR  -->  DU APP : Failed at duBuildAndSendUeRecfgReqToRlc()");
      else
      {
         fillRlcUeRecfg(duRlcUeCfg, rlcUeRecfg);
         ret = sendUeRecfgReqToRlc(rlcUeRecfg);
      }
   }
   else
   {
      DU_LOG("\nERROR  -->  DU APP : Memory Alloc failed at duBuildAndSendUeRecfgReqToRlc()");
      ret = RFAILED;
   }
   DU_FREE(duRlcUeCfg, sizeof(DuRlcUeCfg));
   return ret;
}

/**********************************************************************************
 *
 * @brief Fills Ue ReCfg from DU DB to MacUeRecfg
 *
 * @details
 *
 *    Function : fillMacUeRecfg
 *
 *    Functionality: fills ue ReCfg to be sent to MAC
 *
 * @params[in] DuMacUeCfg Pointer
 *             MacUeRecfg Pointer
 *
 * @return void 
 * *******************************************************************************/
void fillMacUeRecfg(DuMacUeCfg *duMacUeCfg, MacUeRecfg *macUeRecfg)
{
   uint8_t lcIdx = 0;

   macUeRecfg->cellId = duMacUeCfg->cellId;
   macUeRecfg->ueId = duMacUeCfg->ueId;
   macUeRecfg->crnti = duMacUeCfg->crnti;
   macUeRecfg->macCellGrpRecfgPres = duMacUeCfg->macCellGrpCfgPres;
   if(macUeRecfg->macCellGrpRecfgPres)
   {
      memcpy(&macUeRecfg->macCellGrpRecfg, &duMacUeCfg->macCellGrpCfg, sizeof(MacCellGrpCfg));
   }
   macUeRecfg->phyCellGrpRecfgPres = duMacUeCfg->phyCellGrpCfgPres;
   if(macUeRecfg->phyCellGrpRecfgPres)
   {
      memcpy(&macUeRecfg->phyCellGrpRecfg, &duMacUeCfg->phyCellGrpCfg, sizeof(PhyCellGrpCfg));
   }
   macUeRecfg->spCellRecfgPres = duMacUeCfg->spCellCfgPres;
   if(macUeRecfg->spCellRecfgPres)
   {
      memcpy(&macUeRecfg->spCellRecfg, &duMacUeCfg->spCellCfg, sizeof(SpCellRecfg));
   }
   if(duMacUeCfg->ambrCfg != NULLP)
   {
      DU_ALLOC_SHRABL_BUF(macUeRecfg->ambrRecfg, sizeof(AmbrCfg));
      memcpy(macUeRecfg->ambrRecfg, duMacUeCfg->ambrCfg, sizeof(AmbrCfg));
   }
   memcpy(&macUeRecfg->dlModInfo, &duMacUeCfg->dlModInfo, sizeof(ModulationInfo));
   memcpy(&macUeRecfg->ulModInfo, &duMacUeCfg->ulModInfo, sizeof(ModulationInfo));
   if(duMacUeCfg->numLcs > 0)
   {
      macUeRecfg->numLcsToAdd = 0;
      macUeRecfg->numLcsToDel = 0;
      macUeRecfg->numLcsToMod = 0;

      for(lcIdx = 0; lcIdx < duMacUeCfg->numLcs; lcIdx++)
      {
         if(duMacUeCfg->lcCfgList[lcIdx].configType == CONFIG_ADD)
         {
            /*VS: To thoroughly check*/
            memcpy(&macUeRecfg->lcCfgAdd[macUeRecfg->numLcsToAdd], &duMacUeCfg->lcCfgList[lcIdx].lcConfig, sizeof(LcCfg));
            macUeRecfg->numLcsToAdd++;
         }
         if(duMacUeCfg->lcCfgList[lcIdx].configType == CONFIG_DEL)
         {
            /*VS: To thoroughly check*/
            macUeRecfg->lcIdToDel[macUeRecfg->numLcsToDel] = duMacUeCfg->lcCfgList[lcIdx].lcConfig.lcId;
            macUeRecfg->numLcsToDel++;
         }
         if(duMacUeCfg->lcCfgList[lcIdx].configType == CONFIG_MOD)
         {
            /*VS: To thoroughly check*/
            memcpy(&macUeRecfg->lcCfgMod[macUeRecfg->numLcsToMod], &duMacUeCfg->lcCfgList[lcIdx].lcConfig, sizeof(LcCfg));
            macUeRecfg->numLcsToMod++;
         }
      }
   }
   macUeRecfg->transmissionAction = duMacUeCfg->transmissionAction;
#ifdef NR_DRX
   macUeRecfg->drxConfigIndicatorRelease = duMacUeCfg->drxConfigIndicatorRelease;
#endif
}

/*******************************************************************
 *
 * @brief Builds and Send Ue Reconfig Req to MAC
 *
 * @details
 *
 *    Function : duBuildAndSendUeRecfgReqToMac
 *
 *    Functionality: Builds and Send Ue Reconfig Req to MAC
 *
 * @params[in] uint16_t cellId,
 *             uint6_t crnti #AS per 38.473 V15.3.0, Section 9.3.1.32 crnti
 *             value range is b/w 0..65535#  
 *             CellGroupConfigRrc_t *macCellGrpCfg
 *             DuUeCfg *ueCfgDb
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t duBuildAndSendUeRecfgReqToMac(uint16_t cellId, uint8_t duUeF1apId, uint16_t crnti, DuUeCfg *ueCfgDb)
{
   uint8_t ret = ROK;
   DuMacUeCfg *duMacUeCfg = NULLP;
   MacUeRecfg *macUeRecfg = NULLP;

   DU_ALLOC(duMacUeCfg, sizeof(DuMacUeCfg));
   DU_ALLOC_SHRABL_BUF(macUeRecfg, sizeof(MacUeRecfg));
   if(macUeRecfg && duMacUeCfg)
   {
      memset(duMacUeCfg, 0, sizeof(DuMacUeCfg));
      memset(macUeRecfg, 0, sizeof(MacUeRecfg));

      ret = updateDuMacUeCfg(cellId, duUeF1apId, crnti, ueCfgDb, duMacUeCfg);
      if(ret == RFAILED)
         DU_LOG("\nERROR  -->  DU APP : Failed to fill Mac Ue Cfg at duBuildAndSendUeRecfgReqToMac()");
      else
      {
         fillMacUeRecfg(duMacUeCfg, macUeRecfg);
         ret = sendUeRecfgReqToMac(macUeRecfg);
      }
   }
   else
   {
      DU_LOG("\nERROR  -->  DU APP : Memory alloc failed for macUeCfg at duBuildAndSendUeRecfgReqToMac()");
      ret = RFAILED;
   }
   DU_FREE(duMacUeCfg, sizeof(DuMacUeCfg));

   return ret;
}

/*******************************************************************
 *
 * @brief Build and Send Ue context setup request
 *
 * @details
 *
 
 *    Function : duBuildAndSendUeContextSetupReq
 *
 *    Functionality: 
 *     Build and Send Ue context setup request
 * 
 *  @params[in]  cellId, crnti, DuUeCfg pointer
 *  @return ROK     - success
 *          RFAILED - failure
 * 
 *****************************************************************/

uint8_t duBuildAndSendUeContextSetupReq(uint16_t cellId, DuUeCb *ueCb)
{
   uint8_t ret = ROK;
   uint16_t crnti; 
   DuUeCfg *duUeCfg = NULLP;

   DU_LOG("\nDEBUG   -->  DU_APP: Processing Ue Context Setup Request for cellId [%d]", cellId);

   if(!ueCb)
   {
      DU_LOG("\nERROR  -->  DU APP : UE Cb is NULL");
      return RFAILED;
   }

   crnti = ueCb->crnti;
   duUeCfg = &ueCb->f1UeDb->duUeCfg;

   /* If UE is being handed-in to this DU, UE context setup request will create
    * new UE context at MAC/SCH and RLC.
    * If UE is in active state, UE contex setup request will lead to
    * reconfiguration of UE at MAC/SCH and RLC
    */
   if(ueCb->ueState == UE_HANDIN_IN_PROGRESS)
   {
      /* Filling MAC UE Config */
      memset(&ueCb->duMacUeCfg, 0, sizeof(DuMacUeCfg));

      /* Since UE attach has not yet happened, crnti is unknow. Hence passing 0 */
      ret = duBuildAndSendUeCreateReqToMac(cellId, ueCb->gnbDuUeF1apId, 0, duUeCfg, &ueCb->duMacUeCfg);
      if(ret == RFAILED)
         DU_LOG("\nERROR  -->  DU APP : Failed to send UE create request to MAC");

      ret = duBuildAndSendUeCreateReqToRlc(cellId, ueCb->gnbDuUeF1apId, duUeCfg, &ueCb->duRlcUeCfg);
      if(ret == RFAILED)
         DU_LOG("\nERROR  --> DU APP : Failed to send UE create request to RLC");

   }
   else
   {
      /* Filling RLC UE Reconfig */ 
      ret = duBuildAndSendUeRecfgReqToRlc(cellId, ueCb->gnbDuUeF1apId, crnti, duUeCfg);
      if(ret == RFAILED)
         DU_LOG("\nERROR  -->  DU APP : Failed to build ctxt setup req for RLC at duBuildAndSendUeContextSetupReq()");

      /* Filling MAC UE Reconfig */
      ret = duBuildAndSendUeRecfgReqToMac(cellId, ueCb->gnbDuUeF1apId, crnti, duUeCfg);
      if(ret == RFAILED)
         DU_LOG("\nERROR  -->  DU APP : Failed at build ctxt setup req for MAC at duBuildAndSendUeContextSetupReq()");
   }

   return ret;
}

/*******************************************************************
 *
 * @brief Processes DL Rsp received from RLC DL
 *
 * @details
 *
 
 *    Function : DuProcRlcDlRrcMsgRsp
 *
 *    Functionality: 
 *     Processes UE Rsp received from RLC DL
 * 
 *  @params[in]  Post structure
 *               Pointer to RlcCfgCfm
 *  @return ROK     - success
 *          RFAILED - failure
 * 
 *****************************************************************/
uint8_t DuProcRlcDlRrcMsgRsp(Pst *pst, RlcDlRrcMsgRsp *dlRrcMsg)
{
   uint8_t ret = ROK, ueId = 0;
   uint16_t cellId, crnti;
   DuUeCb *ueCb = NULLP;
   DlMsgState state;

   state = dlRrcMsg->state;
   cellId = dlRrcMsg->cellId;
   crnti = dlRrcMsg->crnti;
   DU_FREE_SHRABL_BUF(DU_APP_MEM_REGION, DU_POOL, dlRrcMsg, sizeof(RlcDlRrcMsgRsp));

   if(state == TRANSMISSION_COMPLETE)
   {
      GET_UE_ID(crnti, ueId);
      ueCb = &duCb.actvCellLst[cellId -1]->ueCb[ueId -1];

      if(ueCb->f1UeDb && ueCb->f1UeDb->dlRrcMsgPres)
      {
         if(ueCb->f1UeDb->actionType == UE_CTXT_SETUP)
         {
            ret = duBuildAndSendUeContextSetupReq(cellId, ueCb);
            if(ret == RFAILED)
               DU_LOG("\nERROR  -->  DU APP : Failed to process UE Context Setup Request in DuProcRlcDlRrcMsgRsp()");
         }
         
         if(ueCb->f1UeDb->actionType == UE_CTXT_MOD)
         {
            ret = duBuildAndSendUeContextModReq(cellId, ueCb->gnbDuUeF1apId, crnti, &ueCb->f1UeDb->duUeCfg);
            if(ret == RFAILED)
               DU_LOG("\nERROR  -->  DU APP : Failed to process UE Context Mod Request in DuProcRlcDlRrcMsgRsp()");
         }

         if(ueCb->f1UeDb->actionType == UE_CTXT_RELEASE && ueCb->ueState == UE_ACTIVE)
         {
            ret = duBuildAndSendUeDeleteReq(cellId, crnti);
            if(ret == RFAILED)
            {
               DU_LOG("\nERROR  -->  DU APP : Failed to process UE Context Release Request in DuProcRlcDlRrcMsgRsp()");
            }
         }
      }
   }
   else
      DU_LOG("\nERROR  -->  DU APP : Failed to transmit DL RRC Msg");

   return ret;
}
/*******************************************************************
 *
 * @brief Process UE context setup request from CU
 *
 * @details
 *
 *    Function : duProcUeContextSetupRequest 
 *
 *    Functionality: Process UE context setup request from CU
 *
 * @params[in] F1AP message
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t duProcUeContextSetupRequest(DuUeCb *ueCb)
{
   uint8_t ret, cellId;

   ret = ROK;
   if(ueCb)
   {
      cellId = duCb.actvCellLst[ueCb->f1UeDb->cellIdx]->cellId;

      /* Send DL RRC msg for security Mode */
      if(ueCb->f1UeDb->dlRrcMsg)
      {
         if(ueCb->f1UeDb->dlRrcMsg->rrcMsgPdu)
         {
            /* Sending DL RRC Message to RLC */
            ret = duBuildAndSendDlRrcMsgToRlc(cellId, ueCb->duRlcUeCfg, ueCb->f1UeDb->dlRrcMsg);
            if(ret == RFAILED)
            {
               DU_LOG("\nERROR  -->  DU APP : Failed to send DL RRC msg in duProcUeContextSetupRequest()");
               DU_FREE_SHRABL_BUF(DU_APP_MEM_REGION, DU_POOL, ueCb->f1UeDb->dlRrcMsg->rrcMsgPdu,\
                     ueCb->f1UeDb->dlRrcMsg->rrcMsgSize);
               DU_FREE_SHRABL_BUF(DU_APP_MEM_REGION, DU_POOL, ueCb->f1UeDb->dlRrcMsg, sizeof(F1DlRrcMsg));
            }
         }
      }
      else if(ueCb->f1UeDb->actionType == UE_CTXT_SETUP)
      {
         ret = duBuildAndSendUeContextSetupReq(cellId, ueCb);
         if(ret == RFAILED)
         {
            DU_LOG("\nERROR  -->  DU APP : Failed to build ue context setup Req in duProcUeContextSetupRequest()");
         }
      }
   }
   else
   {
      //TODO: To send the failure cause in UeContextSetupRsp
      DU_LOG("ERROR  -->  DU APP : Failed to process UE CNTXT SETUP REQ at duProcUeContextSetupRequest()");
   }
   return ret;
}

/*******************************************************************
 *
 * @brief Build and Send Ue context mod request
 *
 * @details
 *
 *
 *    Function : duBuildAndSendUeContextModReq
 *
 *    Functionality: 
 *     Build and Send Ue context mod request
 * 
 *  @params[in]  cellId, crnti, DuUeCfg pointer
 *  @return ROK     - success
 *          RFAILED - failure
 * 
 *****************************************************************/

uint8_t duBuildAndSendUeContextModReq(uint16_t cellId, uint8_t gnbDuUeF1apId, uint16_t crnti, DuUeCfg *duUeCfg)
{
   uint8_t ret = ROK;

   DU_LOG("\nDEBUG   -->  DU_APP: Processing Ue Context Mod Request for cellId [%d]", cellId);
   /* Filling RLC Ue Reconfig */ 
   ret = duBuildAndSendUeRecfgReqToRlc(cellId, gnbDuUeF1apId, crnti, duUeCfg);
   if(ret == RFAILED)
      DU_LOG("\nERROR  -->  DU APP : Failed to build ctxt setup req for RLC at duBuildAndSendUeContextModReq()");
   
   /* Filling MAC Ue Reconfig */
   ret = duBuildAndSendUeRecfgReqToMac(cellId, gnbDuUeF1apId, crnti, duUeCfg);
   if(ret == RFAILED)
      DU_LOG("\nERROR  -->  DU APP : Failed at build ctxt setup req for MAC at duBuildAndSendUeContextModReq()");

   return ret;
}
/*******************************************************************
 *
 * @brief Processing Ue context mod request
 *
 * @details
 *
 *
 *    Function : duProcUeContextModReq 
 *
 *    Functionality:
 *        Processing  Ue context mod request
 *
 *  @params[in] DuUeCb *ueCb 
 *  @return ROK     - success
 *          RFAILED - failure
 *
 *****************************************************************/

uint8_t duProcUeContextModReq(DuUeCb *ueCb)
{
   uint8_t ret, cellId;

   ret = ROK;
   if(ueCb)
   {
      cellId = duCb.actvCellLst[ueCb->f1UeDb->cellIdx]->cellId;
      /* Send DL RRC msg for security Mode */
      if(ueCb->f1UeDb->dlRrcMsg)
      {
         if(ueCb->f1UeDb->dlRrcMsg->rrcMsgPdu)
         {
            /* Sending DL RRC Message to RLC */
            ret = duBuildAndSendDlRrcMsgToRlc(cellId, ueCb->duRlcUeCfg, ueCb->f1UeDb->dlRrcMsg);
            if(ret == RFAILED)
            {
               DU_LOG("\nERROR  -->  DU APP : Failed to send DL RRC msg in duProcUeContextModReq()");
               DU_FREE_SHRABL_BUF(DU_APP_MEM_REGION, DU_POOL, ueCb->f1UeDb->dlRrcMsg->rrcMsgPdu,\
                     ueCb->f1UeDb->dlRrcMsg->rrcMsgSize);
               DU_FREE_SHRABL_BUF(DU_APP_MEM_REGION, DU_POOL, ueCb->f1UeDb->dlRrcMsg, sizeof(F1DlRrcMsg));
            }
         }
      }
      else if(ueCb->f1UeDb->actionType == UE_CTXT_MOD)
      {
         ret = duBuildAndSendUeContextModReq(cellId, ueCb->gnbDuUeF1apId, ueCb->crnti, &ueCb->f1UeDb->duUeCfg);
         if(ret == RFAILED)
         {
            DU_LOG("\nERROR  -->  DU APP : Failed to build ue context setup Req in duProcUeContextModReq()");
            return RFAILED;
         }
      }
      else if((ueCb->f1UeDb->actionType == UE_CTXT_CFG_QUERY) || (ueCb->f1UeDb->actionType == UE_CTXT_RRC_RECFG_COMPLETE))
      {
         if((BuildAndSendUeContextModRsp(ueCb) != ROK))
         {
            DU_LOG("\nERROR  -->  DU APP : Failed to build UE Context modification response");
            return RFAILED;
         }
      }
   }
   else
   {
      //TODO: To send the failure cause in UeContextModRsp
      
      DU_LOG("ERROR  -->  DU APP : Failed to process UE CNTXT MOD REQ at duProcUeContextModReq()");
      return RFAILED;
   }
   return ROK;
}

/*******************************************************************
*
* @brief Build and send dedicated RACH resource release request to MAC
*
* @details
*
*    Function : duBuildAndSendRachRsrcRelToMac
*
*    Functionality: Function to Build and send dedicated RACH resource 
*    release request to MAC
*
* @params[in] Cell ID
*             UE CB
* @return ROK - Success
*         RFAILED - Failure
*
* ****************************************************************/
uint8_t duBuildAndSendRachRsrcRelToMac(uint16_t cellId, DuUeCb *ueCb)
{
   Pst pst;
   MacRachRsrcRel *rachRsrcRel = NULLP;

   DU_ALLOC_SHRABL_BUF(rachRsrcRel, sizeof(MacRachRsrcRel));
   if(!rachRsrcRel)
   {
      DU_LOG("\nERROR  -->  DU APP : Failed to allocate memory for RACH Resource Release in \
            duBuildAndSendRachRsrcRelToMac()");
      return RFAILED;
   }

   rachRsrcRel->cellId = cellId;
   rachRsrcRel->ueId = ueCb->gnbDuUeF1apId;
   rachRsrcRel->crnti = ueCb->crnti;

   /* Fill Pst */
   FILL_PST_DUAPP_TO_MAC(pst, EVENT_MAC_RACH_RESOURCE_REL);

   if(((*packMacRachRsrcRelOpts[pst.selector])(&pst, rachRsrcRel)) != ROK)
   {
      DU_LOG("\nERROR  -->  DU_APP : Failure in sending RACH Resource Release to MAC at \
            duBuildAndSendRachRsrcRelToMac()");
      DU_FREE_SHRABL_BUF(DU_APP_MEM_REGION, DU_POOL, rachRsrcRel, sizeof(MacRachRsrcRel));
      return RFAILED;
   }

   return ROK;
}

/*******************************************************************
 *
* @brief delete DuMacUeCfg from duCb
*
* @details
*
*    Function : deleteMacUeCfg 
*
*    Functionality: delete DuMacUeCfg from duCb
*
* @params[in] Pointer to DuMacUeCfg
* @return ROK     - success
*         RFAILED - failure
*
*******************************************************************/

void deleteMacUeCfg(DuMacUeCfg *ueCfg)
{
   uint8_t lcCfgIdx=0;
   
   if(ueCfg->spCellCfgPres)
   {
      freeUeRecfgCellGrpInfo(ueCfg);
   }
   if(ueCfg->ambrCfg)
   {
      DU_FREE_SHRABL_BUF(DU_APP_MEM_REGION, DU_POOL,ueCfg->ambrCfg, sizeof(AmbrCfg));
   }
   for(lcCfgIdx=0; lcCfgIdx< ueCfg->numLcs; lcCfgIdx++)
   {
      freeMacLcCfg(&ueCfg->lcCfgList[lcCfgIdx].lcConfig);
   }
   memset(ueCfg, 0, sizeof(DuMacUeCfg));
}

/*******************************************************************
*
* @brief delete UE Configuration of a particular UE
*
* @details
*
*    Function : deleteUeCfg 
*
*    Functionality: delete UE Configuration of a particular UE 
*
* @params[in] uint16_t cellIdx, uint8_t ueId
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/
uint8_t  deleteUeCfg(uint16_t cellId, uint8_t ueId)
{
   uint8_t tnlIdx = 0;
   uint16_t cellIdx = 0;
   DuUeCb *ueCb = NULLP;
   
   GET_CELL_IDX(cellId, cellIdx);
   if(duCb.actvCellLst[cellIdx] != NULLP)
   {
      if((duCb.actvCellLst[cellIdx]->ueCb[ueId-1].duMacUeCfg.macUeCfgState == UE_DELETE_COMPLETE)\
            &&(duCb.actvCellLst[cellIdx]->ueCb[ueId-1].duRlcUeCfg.rlcUeCfgState == UE_DELETE_COMPLETE))
      {
         ueCb = &duCb.actvCellLst[cellIdx]->ueCb[ueId-1];
         deleteMacUeCfg(&ueCb->duMacUeCfg);
         deleteRlcUeCfg(&ueCb->duRlcUeCfg);
         if(ueCb->f1UeDb !=NULLP)
         {
            freeF1UeDb(ueCb->f1UeDb);
         }
         for(tnlIdx = 0; tnlIdx < duCb.numTeId; )
         {
            if(duCb.upTnlCfg[tnlIdx]->ueId == ueId)
            {
               duCb.upTnlCfg[tnlIdx]->configType = CONFIG_DEL;
               duProcEgtpTunnelCfg(tnlIdx, duCb.upTnlCfg[tnlIdx], duCb.upTnlCfg[tnlIdx]);
            }
            else
               tnlIdx++;
         }
         unsetBitInUeBitMap(cellId, ueId-1);
         duCb.actvCellLst[cellIdx]->numActvUes--;
         memset(ueCb, 0, sizeof(DuUeCb));
      }
      else
      {
         return RFAILED;
      }
   }
   else
   {
      DU_LOG("\nERROR  --> DU APP : deleteUeCfg(): CellIdx[%d] is not found", cellIdx);
      return RFAILED;
   }
   return ROK;
}


/*******************************************************************
*
* @brief Handle UE delete response from MAC
*
* @details
*
*    Function : DuProcMacUeDeleteRsp
*
*    Functionality: Handle UE delete response from MAC
*
* @params[in] Pointer to MacUeDeleteRsp and Pst
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/

uint8_t DuProcMacUeDeleteRsp(Pst *pst, MacUeDeleteRsp *deleteRsp)
{
   uint8_t  ret =ROK,ueId=0;
   uint16_t cellIdx=0;
   uint32_t gnbCuUeF1apId =0 , gnbDuUeF1apId =0;
   
   if(deleteRsp)
   {
      if(deleteRsp->status == SUCCESSFUL)
      {
         DU_LOG("\nINFO   -->  DU APP : MAC UE Delete Response : SUCCESS [UE IDX : %d]", deleteRsp->ueId);
         GET_CELL_IDX(deleteRsp->cellId, cellIdx);
         if(duCb.actvCellLst[cellIdx])
         {
            duCb.actvCellLst[cellIdx]->ueCb[deleteRsp->ueId -1].duMacUeCfg.macUeCfgState = UE_DELETE_COMPLETE;
            ueId = deleteRsp->ueId;
            gnbCuUeF1apId = duCb.actvCellLst[cellIdx]->ueCb[ueId-1].gnbCuUeF1apId;
            gnbDuUeF1apId = duCb.actvCellLst[cellIdx]->ueCb[ueId-1].gnbDuUeF1apId;
            if(deleteUeCfg(deleteRsp->cellId, ueId) == ROK)
            {
               ret = BuildAndSendUeContextReleaseComplete(deleteRsp->cellId, gnbCuUeF1apId, gnbDuUeF1apId);
               if(ret != ROK)
               {
                  DU_LOG("\nERROR  -->  DU APP : DuProcMacUeDeleteRsp(): failed to send UE context release complete");
               }
            }

         }
      }
      else
      {
         DU_LOG("\nERROR  -->  DU APP : DuProcMacUeDeleteRsp(): MAC UE Delete Response : FAILURE [UE IDX : %d]",\
         deleteRsp->ueId);
         ret =  RFAILED;
      }
      DU_FREE_SHRABL_BUF(pst->region, pst->pool, deleteRsp, sizeof(MacUeDeleteRsp));
   }
   else
   {
      DU_LOG("\nERROR  -->  DU APP : DuProcMacUeDeleteRsp(): MAC UE Delete Response is null");
      ret = RFAILED;
   }
   return ret;
}

/*******************************************************************
 *
 * @brief Processes UE Delete Rsp received from RLC 
 *
 * @details
 *
 *    Function : DuProcRlcUeDeleteRsp
 *
 *    Functionality:
 *     Processes UE Delete Rsp received from RLC 
 *
 *  @params[in]  Post structure
 *               Pointer to RlcUeDeleteRsp
 *  @return ROK     - success
 *          RFAILED - failure
 *
 * *****************************************************************/

uint8_t DuProcRlcUeDeleteRsp(Pst *pst, RlcUeDeleteRsp *delRsp)
{
   uint8_t  ueId = 0, ret = ROK;
   uint16_t cellIdx = 0,crnti=0;

   if(delRsp)
   {
      ueId = delRsp->ueId;
      GET_CELL_IDX(delRsp->cellId, cellIdx);

      if(delRsp->status == SUCCESSFUL)
      {
         DU_LOG("\nINFO   -->  DU_APP: RLC UE Delete Response : SUCCESS [UE IDX:%d]", ueId);
         if(duCb.actvCellLst[cellIdx]!=NULLP)
         {
            duCb.actvCellLst[cellIdx]->ueCb[ueId-1].duRlcUeCfg.rlcUeCfgState = UE_DELETE_COMPLETE;
            GET_CRNTI(crnti, ueId);
            if(sendUeDeleteReqToMac(delRsp->cellId, ueId, crnti) == RFAILED)
            {
               DU_LOG("\nERROR  -->  DU APP : duBuildAndSendUeDeleteReq(): Failed to build UE  delete req for MAC ");
               return RFAILED;
            }

         }
      }
      else
      {
         DU_LOG("\nERROR   -->  DU_APP: RLC UE Delete Response : FAILED [UE IDX:%d]", ueId);
         ret = RFAILED;
      }
      DU_FREE_SHRABL_BUF(pst->region, pst->pool, delRsp, sizeof(RlcUeDeleteRsp));

   }
   return ret;
}

/*******************************************************************
 *
 * @brief Sending UE Delete Req To Mac
*
* @details
*
*    Function : sendUeDeleteReqToMac
*
*    Functionality:
*     sending UE Delete Req To Mac
*
*  @params[in]    cellId, ueId, crnti 
*  @return ROK     - success
*          RFAILED - failure
*
*****************************************************************/

uint8_t sendUeDeleteReqToMac(uint16_t cellId, uint8_t ueId, uint16_t crnti)
{
   Pst pst;
   uint8_t ret=ROK;
   MacUeDelete *ueDelete = NULLP;

   DU_ALLOC_SHRABL_BUF(ueDelete, sizeof(MacUeDelete));
   if(ueDelete)
   {
      ueDelete->cellId = cellId;
      ueDelete->ueId   = ueId;
      ueDelete->crnti  = crnti;
      FILL_PST_DUAPP_TO_MAC(pst, EVENT_MAC_UE_DELETE_REQ);

      DU_LOG("\nDEBUG  -->  DU_APP: Sending UE delete Request to MAC ");
      ret = (*packMacUeDeleteReqOpts[pst.selector])(&pst, ueDelete);
      if(ret == RFAILED)
      {
         DU_LOG("\nERROR  -->  DU_APP: sendUeDeleteReqToMac(): Failed to send UE delete Req to MAC");
         DU_FREE_SHRABL_BUF(DU_APP_MEM_REGION, DU_POOL, ueDelete, sizeof(MacUeDelete));
      }
   }
   else
   {
      DU_LOG("\nERROR  -->   DU_APP: sendUeDeleteReqToMac(): Failed to allocate memory"); 
      ret = RFAILED;
   }
   return ret;
}

/*******************************************************************
 *
 * @brief Sending UE Delete Req To Rlc
 *
 * @details
 *
 *    Function : sendUeDeleteReqToRlc
 *
 *    Functionality:
 *     Sending UE Delete Req To Rlc
 *
 *  @params[in]  cellId, ueId 
 *  @return ROK     - success
 *          RFAILED - failure
 *
 *****************************************************************/

uint8_t sendUeDeleteReqToRlc(uint16_t cellId, uint8_t ueId)
{
   uint8_t ret;
   Pst pst;
   RlcUeDelete *ueDelete;

   DU_ALLOC_SHRABL_BUF(ueDelete, sizeof(RlcUeDelete));
   if(ueDelete !=NULLP)
   {
      ueDelete->cellId = cellId;
      ueDelete->ueId = ueId;
      FILL_PST_DUAPP_TO_RLC(pst, RLC_UL_INST, EVENT_RLC_UE_DELETE_REQ);

      ret = (*packRlcUeDeleteReqOpts[pst.selector])(&pst, ueDelete);
      if(ret == RFAILED)
      {
         DU_LOG("\nERROR  -->  DU_APP : sendUeDeleteReqToRlc():Failed to send UE Delete  Req to RLC"); 
         DU_FREE_SHRABL_BUF(DU_APP_MEM_REGION, DU_POOL, ueDelete, sizeof(RlcUeDelete));
      }
   }
   else
   {
      DU_LOG("\nERROR  -->   DU_APP: sendUeDeleteReqToRlc():Memory allocation failed");
      ret = RFAILED;
   }
   return ret;
}

/*******************************************************************
 *
 * @brief DU processes UE delete req from CU and sends to MAC and RLC 
 *
 * @details
 *
 *    Function : duBuildAndSendUeDeleteReq
 *
 *    Functionality: DU processes UE delete req from CU and sends to MAC 
 *                   and RLC 
 *
 * @params[in] cellId, crnti 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t duBuildAndSendUeDeleteReq(uint16_t cellId, uint16_t crnti)
{
   uint8_t  ueId =0;
   uint16_t cellIdx = 0;

   DU_LOG("\nDEBUG  -->  DU_APP: Processing UE Delete Request ");
   GET_CELL_IDX(cellId, cellIdx);
   GET_UE_ID(crnti, ueId);

   if(duCb.actvCellLst[cellIdx] != NULLP)
   {
      if(crnti != duCb.actvCellLst[cellIdx]->ueCb[ueId - 1].crnti)
      {
         DU_LOG("\nERROR  -->  DU APP : duBuildAndSendUeDeleteReq(): CRNTI [%d] not found", crnti);
         return RFAILED;
      }

      duCb.actvCellLst[cellIdx]->ueCb[ueId - 1].ueState = UE_DELETION_IN_PROGRESS; 
      if(sendUeDeleteReqToRlc(cellId, ueId) == RFAILED)
      {
         DU_LOG("\nERROR  -->  DU APP : DuProcMacUeDeleteRsp():Failed to build UE  delete req for RLC ");
         return RFAILED;
      }
   }
   else
   {
      DU_LOG("\nERROR  -->  DU APP : duBuildAndSendUeDeleteReq(): Cell Id is not found");
      return RFAILED;
   }

   return ROK;
}

/*******************************************************************
 *
 * @brief delete RlcUeCreate from duCb
 *
 * @details
 *
 *    Function : deleteRlcUeCfg
 *
 *    Functionality:
 *       delete  RlcUeCreate from duCb
 *
 *  @params[in] RlcUeCreate *ueCfg 
 *               
 *  @return ROK     - success
 *          RFAILED - failure
 *
 *****************************************************************/

void deleteRlcUeCfg(DuRlcUeCfg *ueCfg)
{
   uint8_t lcIdx = 0;
   RlcBearerCfg *lcCfg= NULLP;
   if(ueCfg)
   {
      for(lcIdx =0 ; lcIdx < ueCfg->numLcs ; lcIdx++)
      {
         lcCfg = &ueCfg->rlcLcCfg[lcIdx].rlcBearerCfg;
         switch(lcCfg->rlcMode)
         {
            case RLC_AM :
               {
                  DU_FREE_SHRABL_BUF(DU_APP_MEM_REGION, DU_POOL, lcCfg->u.amCfg, sizeof(AmBearerCfg));
                  break;
               }
            case RLC_UM_BI_DIRECTIONAL :
               {
                  DU_FREE_SHRABL_BUF(DU_APP_MEM_REGION, DU_POOL, lcCfg->u.umBiDirCfg, sizeof(UmBiDirBearerCfg));
                  break;
               }
            case RLC_UM_UNI_DIRECTIONAL_UL :
               {
                  DU_FREE_SHRABL_BUF(DU_APP_MEM_REGION, DU_POOL, lcCfg->u.umUniDirUlCfg, sizeof(UmUniDirUlBearerCfg));
                  break;
               }
            case RLC_UM_UNI_DIRECTIONAL_DL :
               {
                  DU_FREE_SHRABL_BUF(DU_APP_MEM_REGION, DU_POOL, lcCfg->u.umUniDirDlCfg, sizeof(UmUniDirDlBearerCfg));
                  break;
               }
         }
         DU_FREE_SHRABL_BUF(DU_APP_MEM_REGION, DU_POOL, lcCfg->snssai, sizeof(Snssai));
      }
      memset(ueCfg, 0, sizeof(DuRlcUeCfg));
   }
}

/*******************************************************************
*
* @brief Du process Ue Context Release Command
*
* @details
*
*    Function : duProcUeContextReleaseCommand 
*
*    Functionality: Du process Ue Context Release Command 
*
* @params[in] DuUeCb *duUeCb 
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/
uint8_t duProcUeContextReleaseCommand(uint16_t cellId, DuUeCb *duUeCb)
{
   uint8_t ret =ROK, ueId=0;
   uint16_t crnti = 0;

   if(duUeCb != NULLP)
   {
      crnti = duUeCb->crnti;
      GET_UE_ID(crnti, ueId);
      
      if(duUeCb->f1UeDb)
      {
         /* Send DL RRC msg for RRC release */
         if(duUeCb->f1UeDb->dlRrcMsg)
         {
            if(duUeCb->f1UeDb->dlRrcMsg->rrcMsgPdu != NULLP)
            {
               ret = duBuildAndSendDlRrcMsgToRlc(cellId, duCb.actvCellLst[duUeCb->f1UeDb->cellIdx]->ueCb[ueId-1].duRlcUeCfg,\
                     duUeCb->f1UeDb->dlRrcMsg);
               if(ret == RFAILED)
               {
                  DU_LOG("\nERROR  -->  DU APP : duProcUeContextReleaseCommand() : Failed to send DL RRC msg");
                  DU_FREE_SHRABL_BUF(DU_APP_MEM_REGION, DU_POOL, duUeCb->f1UeDb->dlRrcMsg->rrcMsgPdu,\
                        duUeCb->f1UeDb->dlRrcMsg->rrcMsgSize);
                  DU_FREE_SHRABL_BUF(DU_APP_MEM_REGION, DU_POOL, duUeCb->f1UeDb->dlRrcMsg, sizeof(F1DlRrcMsg));
               }
            }
         }
      }
      else
      {
         ret = duBuildAndSendUeDeleteReq(cellId,crnti);
         if(ret == RFAILED)
         {
            DU_LOG("\nERROR  -->  DU APP : duProcUeContextReleaseCommand(): Failed to build and send Ue Delete request");
         }
      }
   }
   return ret;
}

/*******************************************************************
 *
 * @brief Sending UE Reset Req To Mac
*
* @details
*
*    Function : sendUeResetReqToMac
*
*    Functionality:
*     sending UE Reset Req To Mac
*
*  @params[in]    cellId, ueId, crnti 
*  @return ROK     - success
*          RFAILED - failure
*
*****************************************************************/

uint8_t sendUeResetReqToMac(uint16_t cellId, uint8_t ueId)
{
   Pst pst;
   uint8_t ret=ROK;
   MacUeResetReq *ueReset = NULLP;

   DU_ALLOC_SHRABL_BUF(ueReset, sizeof(MacUeResetReq));
   if(ueReset)
   {
      ueReset->cellId = cellId;
      ueReset->ueId   = ueId;
      FILL_PST_DUAPP_TO_MAC(pst, EVENT_MAC_UE_RESET_REQ);

      DU_LOG("\nDEBUG  -->  DU_APP: Sending UE Reset Request to MAC ");
      ret = (*packMacUeResetReqOpts[pst.selector])(&pst, ueReset);
      if(ret == RFAILED)
      {
         DU_LOG("\nERROR  -->  DU_APP: sendUeResetReqToMac(): Failed to send UE Reset Req to MAC");
         DU_FREE_SHRABL_BUF(DU_APP_MEM_REGION, DU_POOL, ueReset, sizeof(MacUeResetReq));
      }
   }
   else
   {
      DU_LOG("\nERROR  -->   DU_APP: sendUeResetReqToMac(): Failed to allocate memory"); 
      ret = RFAILED;
   }
   return ret;
}

/*******************************************************************
 *
 * @brief DU processes UE reset req  and send it to MAC
 *
 * @details
 *
 *    Function : duBuildAndSendUeResetReq
 *
 *    Functionality: DU processes UE reset req and send to MAC
 *                   
 *
 * @params[in] cellId, crnti 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t duBuildAndSendUeResetReq(uint16_t cellId, uint16_t crnti)
{
   uint8_t  ueId =0;
   uint16_t cellIdx = 0;

   DU_LOG("\nDEBUG  -->  DU_APP : Building UE reset request");
   GET_CELL_IDX(cellId, cellIdx);
   GET_UE_ID(crnti, ueId);

   if(duCb.actvCellLst[cellIdx] != NULLP)
   {
      if(crnti != duCb.actvCellLst[cellIdx]->ueCb[ueId - 1].crnti)
      {
         DU_LOG("\nERROR  -->  DU APP : duBuildAndSendUeResetReq(): CRNTI [%d] not found", crnti);
         return RFAILED;
      }

      duCb.actvCellLst[cellIdx]->ueCb[ueId - 1].ueState = UE_RESET_IN_PROGRESS; 
      if(sendUeResetReqToMac(cellId, ueId) == RFAILED)
      {
         DU_LOG("\nERROR  -->  DU APP : DuProcMacUeResetRsp(): Failed to build UE reset req for MAC ");
         return RFAILED;
      }
   }
   else
   {
      DU_LOG("\nERROR  -->  DU APP : duBuildAndSendUeResetReq(): Cell Id %d not found", cellId);
      return RFAILED;
   }

   return ROK;
}

/*******************************************************************
*
* @brief Handle UE reset response from MAC
*
* @details
*
*    Function : DuProcMacUeResetRsp
*
*    Functionality: Handle UE reset response from MAC
*
* @params[in] Pointer to MacUeResetRsp and Pst
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/

uint8_t DuProcMacUeResetRsp(Pst *pst, MacUeResetRsp *resetRsp)
{
   uint8_t  ret =ROK;
   uint16_t cellIdx=0;
   
   if(resetRsp)
   {
      if(resetRsp->status == SUCCESSFUL)
      {
         DU_LOG("\nINFO   -->  DU APP : MAC UE Reset Response : SUCCESS [UE IDX : %d]", resetRsp->ueId);
         GET_CELL_IDX(resetRsp->cellId, cellIdx);
         if(duCb.actvCellLst[cellIdx])
         {
            duCb.actvCellLst[cellIdx]->ueCb[resetRsp->ueId -1].duMacUeCfg.macUeCfgState = UE_RESET_COMPLETE;
            /*TODO - Complete the processing after receiving successfully reset rsp*/
         }
      }
      else
      {
         DU_LOG("\nERROR  -->  DU APP : DuProcMacUeResetRsp(): MAC UE Reset Response : FAILURE [UE IDX : %d]",resetRsp->ueId);
         ret =  RFAILED;
      }
      DU_FREE_SHRABL_BUF(pst->region, pst->pool, resetRsp, sizeof(MacUeResetRsp));
   }
   else
   {
      DU_LOG("\nERROR  -->  DU APP : DuProcMacUeResetRsp(): MAC UE Reset Response is null");
      ret = RFAILED;
   }
   return ret;
}

/*******************************************************************
*
* @brief Handle UE sync status indication from MAC
*
* @details
*
*    Function : DuProcMacUeSyncStatusInd
*
*    Functionality: Handle UE sync status indication from MAC
*
* @params[in] Pointer to MacUeSyncStatusInd and Pst
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/

uint8_t DuProcMacUeSyncStatusInd(Pst *pst, MacUeSyncStatusInd *ueSyncStatusInd)
{
   uint8_t  ret =RFAILED;
   uint16_t cellIdx=0, crnti = 0;
   char *status;

   if(ueSyncStatusInd)
   {
      GET_CELL_IDX(ueSyncStatusInd->cellId, cellIdx);
      if(duCb.actvCellLst[cellIdx])
      {
         GET_CRNTI(crnti, ueSyncStatusInd->ueId);
         if(duCb.actvCellLst[cellIdx]->ueCb[ueSyncStatusInd->ueId-1].crnti == crnti)
         {
            switch(ueSyncStatusInd->status)
            {
               case IN_SYNC:
                  status = "IN_SYNC";
                  break;

               case OUT_OF_SYNC:
                  status = "OUT_OF_SYNC";
                  break;

               case OUT_OF_SUNC_MAX_RETRIES:
                  status = "OUT_OF_SUNC_MAX_RETRIES";
                  break;

               default:
                  status = "INVALID";
                  break;
                  
            }
            DU_LOG("\nINFO  -->   DU APP : MAC UE sync status for received UeId %d is %s", ueSyncStatusInd->ueId,status);
         }
         else
         {
            DU_LOG("\nERROR  -->  DU APP : DuProcMacUeSyncStatusInd(): MAC UE sync status indication : Ue Id [%d] not found",ueSyncStatusInd->cellId);
         }
      }
      else
      {
         DU_LOG("\nERROR  -->  DU APP : DuProcMacUeSyncStatusInd(): MAC UE sync status indication : Cell Id [%d] not found",ueSyncStatusInd->cellId);
      }
      DU_FREE_SHRABL_BUF(pst->region, pst->pool, ueSyncStatusInd, sizeof(MacUeSyncStatusInd));
   }
   else
   {
      DU_LOG("\nERROR  -->  DU APP : DuProcMacUeSyncStatusInd(): MAC UE sync status indication is null");
   }
   return ret;
}

/*******************************************************************
 *
 * @brief Sending UE Reestablishment Req To Rlc
 *
 * @details
 *
 *    Function : sendUeReestablishReqToRlc
 *
 *    Functionality:
 *     Sending UE Reestablishment Req To Rlc
 *
 *  @params[in]  cellId, ueId 
 *  @return ROK     - success
 *          RFAILED - failure
 *
 *****************************************************************/

uint8_t sendUeReestablishReqToRlc(uint16_t cellId, uint8_t ueId, uint8_t numLcToReestablish, uint8_t *lcId)
{
   uint8_t ret=ROK,idx=0;
   Pst pst;
   RlcUeReestablishReq *ueReestablish;

   DU_ALLOC_SHRABL_BUF(ueReestablish, sizeof(RlcUeReestablishReq));
   if(ueReestablish !=NULLP)
   {
      ueReestablish->cellId = cellId;
      ueReestablish->ueId = ueId;
      ueReestablish->numLcsToReestablish = numLcToReestablish;

      for(idx = 0;idx<numLcToReestablish; idx++)
      {
         ueReestablish->lcToReestablish[idx]= lcId[idx];
      }
      FILL_PST_DUAPP_TO_RLC(pst, RLC_UL_INST, EVENT_RLC_UE_REESTABLISH_REQ);

      ret = (*packRlcUeReestablishReqOpts[pst.selector])(&pst, ueReestablish);
      if(ret == RFAILED)
      {
         DU_LOG("\nERROR  -->  DU_APP : sendUeReestablishReqToRlc():Failed to send UE Reestablishment  Req to RLC"); 
         DU_FREE_SHRABL_BUF(DU_APP_MEM_REGION, DU_POOL, ueReestablish, sizeof(RlcUeReestablishReq));
      }
   }
   else
   {
      DU_LOG("\nERROR  -->   DU_APP: sendUeReestablishReqToRlc():Memory allocation failed");
      ret = RFAILED;
   }
   return ret;
}

/*******************************************************************
 *
 * @brief DU processes UE reestablishment req and sends to MAC and RLC 
 *
 * @details
 *
 *    Function : duBuildAndSendUeReestablishReq
 *
 *    Functionality: DU processes UE reestablishment req and sends to MAC 
 *                   and RLC 
 *
 * @params[in] cellId, crnti, numLcToReestablish, ListOflcid 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t duBuildAndSendUeReestablishReq(uint16_t cellId, uint16_t crnti, uint8_t numLcToReestablish, uint8_t *lcId)
{
   uint8_t  ueId =0;
   uint16_t cellIdx = 0;

   DU_LOG("\nDEBUG  -->  DU_APP: Building UE Reestablishment Request ");
   GET_CELL_IDX(cellId, cellIdx);
   GET_UE_ID(crnti, ueId);

   if(duCb.actvCellLst[cellIdx] != NULLP)
   {
      if(crnti != duCb.actvCellLst[cellIdx]->ueCb[ueId - 1].crnti)
      {
         DU_LOG("\nERROR  -->  DU APP : duBuildAndSendUeReestablishReq(): CRNTI [%d] not found", crnti);
         return RFAILED;
      }

      if(sendUeReestablishReqToRlc(cellId, ueId, numLcToReestablish, lcId) == RFAILED)
      {
         DU_LOG("\nERROR  -->  DU APP : duBuildAndSendUeReestablishReq(): Failed to send UE reestablishment req for RLC ");
         return RFAILED;
      }
   }
   else
   {
      DU_LOG("\nERROR  -->  DU APP : duBuildAndSendUeReestablishReq(): Cell Id %d is not found", cellId);
      return RFAILED;
   }

   return ROK;
}

/*******************************************************************
 *
 * @brief Processes UE Reestablishment Rsp received from RLC 
 *
 * @details
 *
 *    Function : DuProcRlcUeReestablishRsp
 *
 *    Functionality:
 *     Processes UE Reestablishment Rsp received from RLC 
 *
 *  @params[in]  Post structure
 *               Pointer to RlcUeReestablishRsp
 *  @return ROK     - success
 *          RFAILED - failure
 *
 * *****************************************************************/

uint8_t DuProcRlcUeReestablishRsp(Pst *pst, RlcUeReestablishRsp *ueReestablishRsp)
{
   uint8_t  ueId = 0, ret = RFAILED;
   uint16_t cellIdx = 0,crnti=0;

   if(ueReestablishRsp)
   {
      ueId = ueReestablishRsp->ueId;
      GET_CELL_IDX(ueReestablishRsp->cellId, cellIdx);

      if(ueReestablishRsp->status == SUCCESSFUL)
      {
         if(duCb.actvCellLst[cellIdx]!=NULLP)
         {
            GET_CRNTI(crnti, ueId);
            if(duCb.actvCellLst[cellIdx]->ueCb[ueId-1].crnti ==  crnti)
            {
               /*TODO: complete the processing of UE Reestablishment Response */
               DU_LOG("\nINFO   -->  DU_APP: RLC UE Reestablishment Response : SUCCESS [UE IDX:%d]", ueId);
               ret = ROK;
            }
            else
               DU_LOG("\nERROR  -->  DU APP : duBuildAndSendUeReestablishRsp(): CRNTI [%d] not found", crnti);
         }
         else
            DU_LOG("\nERROR  -->  DU APP : duBuildAndSendUeReestablishRsp(): Cell Id[%d] is not found", ueReestablishRsp->cellId);
            
      }
      else
      {
         DU_LOG("\nERROR   -->  DU_APP: RLC UE Reestablishment Response : FAILED [UE IDX:%d]", ueId);
      }
      DU_FREE_SHRABL_BUF(pst->region, pst->pool, ueReestablishRsp, sizeof(RlcUeReestablishRsp));

   }
   return ret;
}
/**********************************************************************
  End of file
***********************************************************************/
