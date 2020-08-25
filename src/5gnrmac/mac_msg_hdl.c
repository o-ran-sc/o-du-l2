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

/* header include files -- defines (.h)  */
#include "common_def.h"
#include "lrg.h"
#include "rgu.h"
#include "lrg.x"
#include "rgu.x"
#include "du_app_mac_inf.h"
#include "mac_sch_interface.h"
#include "rlc_mac_inf.h"
#include "mac_upr_inf_api.h"
#include "lwr_mac.h"
#ifdef INTEL_FAPI
#include "fapi_interface.h"
#endif
#include "lwr_mac_fsm.h"
#include "lwr_mac_upr_inf.h"
#include "mac.h"
#include "mac_utils.h"

/* This file contains message handling functionality for MAC */

extern MacCb  macCb;

extern void sendToLowerMac(uint16_t msgType, uint32_t msgLen, void *msg);
uint16_t buildMacPdu(RlcData *dlData);
#ifdef EGTP_TEST
void macStubBuildUlData(Buffer *mBuf);
#endif

/* Function pointer for sending crc ind from MAC to SCH */
MacSchCrcIndFunc macSchCrcIndOpts[]=
{
   packMacSchCrcInd,
   macSchCrcInd,
   packMacSchCrcInd
};

/* Function pointer for sending DL RLC BO Info from MAC to SCH */
MacSchDlRlcBoInfoFunc macSchDlRlcBoInfoOpts[]=
{
   packMacSchDlRlcBoInfo,
   macSchDlRlcBoInfo,
   packMacSchDlRlcBoInfo
};

/*******************************************************************
 *
 * @brief Sends DL BO Info to SCH
 *
 * @details
 *
 *    Function : sendDlRlcBoInfoMacToSch
 *
 *    Functionality:
 *       Sends DL BO Info to SCH
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 ****************************************************************/
uint8_t sendDlRlcBoInfoMacToSch(DlRlcBOInfo *dlBoInfo)
{
   Pst pst;

   FILL_PST_MAC_TO_SCH(pst, EVENT_DL_RLC_BO_INFO_TO_SCH);
   return(*macSchDlRlcBoInfoOpts[pst.selector])(&pst, dlBoInfo);
}

/*******************************************************************
 *
 * @brief Sends CRC Indication to SCH
 *
 * @details
 *
 *    Function : sendCrcIndMacToSch 
 *
 *    Functionality:
 *       Sends CRC Indication to SCH
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 ****************************************************************/
uint8_t sendCrcIndMacToSch(CrcIndInfo *crcInd)
{
   Pst pst;

   FILL_PST_MAC_TO_SCH(pst, EVENT_CRC_IND_TO_SCH);
   return(*macSchCrcIndOpts[pst.selector])(&pst, crcInd);
}

/*******************************************************************
 *
 * @brief Processes CRC Indication from PHY
 *
 * @details
 *
 *    Function : fapiMacCrcInd
 *
 *    Functionality:
 *       Processes CRC Indication from PHY
 *
 * @params[in] Post Structure Pointer
 *             Crc Indication Pointer
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t fapiMacCrcInd(Pst *pst, CrcInd *crcInd)
{
   uint16_t     cellIdx;
   CrcIndInfo   crcIndInfo;

   DU_LOG("\nMAC : Received CRC indication");

   GET_CELL_IDX(crcInd->cellId, cellIdx);
   /* Considering one pdu and one preamble */ 
   crcIndInfo.cellId = macCb.macCell[cellIdx]->cellId;;
   crcIndInfo.crnti = crcInd->crcInfo[0].rnti;
   crcIndInfo.timingInfo.sfn = crcInd->timingInfo.sfn;
   crcIndInfo.timingInfo.slot = crcInd->timingInfo.slot;
   crcIndInfo.numCrcInd = crcInd->crcInfo[0].numCb;
   crcIndInfo.crcInd[0] = crcInd->crcInfo[0].cbCrcStatus[0];

   return(sendCrcIndMacToSch(&crcIndInfo));
}

/*******************************************************************
 *
 * @brief Process Rx Data Ind at MAC
 *
 * @details
 *
 *    Function : fapiMacRxDataInd
 *
 *    Functionality:
 *       Process Rx Data Ind at MAC
 *
 * @params[in] Post structure
 *             Rx Data Indication
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t fapiMacRxDataInd(Pst *pst, RxDataInd *rxDataInd)
{
   uint16_t pduIdx;

   DU_LOG("\nMAC : Received Rx Data indication");

   /* TODO : compare the handle received in RxDataInd with handle send in PUSCH
    * PDU, which is stored in raCb */

   for(pduIdx = 0; pduIdx < rxDataInd->numPdus; pduIdx++)
   {
      unpackRxData(rxDataInd->cellId, &rxDataInd->pdus[pduIdx]);
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Processes DL data from RLC
 *
 * @details
 *
 *    Function : MacProcRlcDlData 
 *
 *    Functionality:
 *      Processes DL data from RLC
 *
 * @params[in] Post structure
 *             DL data
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t MacProcRlcDlData(Pst* pstInfo, RlcData *dlData)
{
   uint8_t pduIdx;

#ifdef EGTP_TEST
   /* Build and send UL data to RLC*/
   RlcData   *ulData;
   Buffer    *mBuf;
   uint16_t  copyLen;
   Pst       pst;
   uint8_t   idx;

   /* Building dummy UL data message */
   if(SGetMsg(MAC_MEM_REGION, MAC_POOL, &mBuf) != ROK)
   {
      return RFAILED;
   }
   macStubBuildUlData(mBuf);

   /* Filling RLC Ul Data*/
   MAC_ALLOC_SHRABL_BUF(ulData, sizeof(RlcData));
   if(!ulData)
   {
      return RFAILED;
   }

   ulData->cellId = dlData->cellId;
   GET_CRNTI(ulData->rnti, dlData->rnti);
   memcpy(&ulData->slotInfo, &dlData->slotInfo, sizeof(SlotIndInfo));
   ulData->numPdu = 1;
   ulData->pduInfo[0].commCh = false;
   ulData->pduInfo[0].lcId = dlData->pduInfo[0].lcId;

   /* Copy UL data message to fixed buffer pdu */
   SFndLenMsg(mBuf, (MsgLen *)&ulData->pduInfo[0].pduLen);
   MAC_ALLOC_SHRABL_BUF(ulData->pduInfo[0].pduBuf, ulData->pduInfo[0].pduLen);
   if(!ulData->pduInfo[0].pduBuf)
   {
      return RFAILED;
   }
   SCpyMsgFix(mBuf, 0, ulData->pduInfo[0].pduLen, ulData->pduInfo[0].pduBuf, \
      (MsgLen *)&copyLen);
   SPutMsg(mBuf);

   /* Fill Pst and sen to RLC */
   FILL_PST_MAC_TO_RLC(pst, RLC_UL_INST, EVENT_UL_DATA_TO_RLC);
   MacSendUlDataToRlc(&pst, ulData);
#endif

   for(pduIdx = 0; pduIdx < dlData->numPdu; pduIdx++)
   {
      MAC_FREE_SHRABL_BUF(pstInfo->region, pstInfo->pool, dlData->pduInfo[pduIdx].pduBuf,\
         dlData->pduInfo[pduIdx].pduLen);
   }
   if(pstInfo->selector == ODU_SELECTOR_LWLC)
   {
      MAC_FREE_SHRABL_BUF(pstInfo->region, pstInfo->pool, dlData, sizeof(RlcData));
   }

   return ROK;
}

/*******************************************************************
 *
 * @brief Processes BO status from RLC
 *
 * @details
 *
 *    Function : MacProcRlcBOStatus
 *
 *    Functionality:
 *      Processes BO status from RLC
 *
 * @params[in] Post structure
 *             BO status
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t MacProcRlcBOStatus(Pst* pst, RlcBOStatus* boStatus)
{
   DlRlcBOInfo  dlBoInfo;

   dlBoInfo.cellId = boStatus->cellId;
   GET_CRNTI(dlBoInfo.crnti, boStatus->ueIdx);
   dlBoInfo.lcId = boStatus->lcId;
   dlBoInfo.dataVolume = boStatus->bo;

   sendDlRlcBoInfoMacToSch(&dlBoInfo); 

   if(pst->selector == ODU_SELECTOR_LWLC)
   {
      MAC_FREE_SHRABL_BUF(pst->region, pst->pool, boStatus, sizeof(RlcBOStatus));
   }

   return ROK;
}

/*******************************************************************
 *
 * @brief Send LC schedule result report to RLC
 *
 * @details
 *
 *    Function : sendSchResultRepToRlc 
 *
 *    Functionality: Send LC schedule result report to RLC
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t sendSchResultRepToRlc(DlSchedInfo dlInfo, SlotIndInfo slotInfo)
{
   Pst      pst;
   uint8_t  lcIdx;
   RlcSchResultRpt  *schRep = NULLP;
   
   MAC_ALLOC_SHRABL_BUF(schRep, sizeof(RlcSchResultRpt));
   if(!schRep)
   {
      DU_LOG("\nMAC: Memory allocation failure in sendSchResultRepToRlc");
      return RFAILED;
   }

   schRep->cellId = dlInfo.cellId;
   schRep->rnti = dlInfo.dlMsgAlloc->crnti;
   schRep->numLc = dlInfo.dlMsgAlloc->numLc;
   schRep->slotInfo.sfn = slotInfo.sfn;
   schRep->slotInfo.slot = slotInfo.slot;

   for(lcIdx = 0; lcIdx < schRep->numLc; lcIdx++)
   {
      schRep->lcSch[lcIdx].lcId = dlInfo.dlMsgAlloc->lcSchInfo[lcIdx].lcId;
      schRep->lcSch[lcIdx].bufSize = dlInfo.dlMsgAlloc->lcSchInfo[lcIdx].schBytes;
      schRep->lcSch[lcIdx].commCh = false;

      if((schRep->lcSch[lcIdx].lcId == SRB1_LCID) || \
         (schRep->lcSch[lcIdx].lcId == SRB2_LCID))
      {
         schRep->lcSch[lcIdx].commCh = true;
      }
   }

   /* Fill Pst */
   FILL_PST_MAC_TO_RLC(pst, RLC_DL_INST, EVENT_SCH_RESULT_TO_RLC);
   if(MacSendSchResultRptToRlc(&pst, schRep) != ROK)
   {
      DU_LOG("\nMAC: Failed to send Schedule result report to RLC");
      MAC_FREE_SHRABL_BUF(MAC_MEM_REGION, MAC_POOL, schRep, sizeof(RlcSchResultRpt));
      return RFAILED;
   }

   return ROK;
}

/*******************************************************************
 *
 * @brief Handles cell start reuqest from DU APP
 *
 * @details
 *
 *    Function : MacProcCellStartReq
 *
 *    Functionality:
 *      Handles cell start reuqest from DU APP
 *
 * @params[in] Post structure pointer
 *             Cell Start Request info pointer 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t MacProcCellStartReq(Pst *pst, MacCellStartInfo  *cellStartInfo)
{
   DU_LOG("\nMAC : Handling cell start request");
   sendToLowerMac(START_REQUEST, 0, cellStartInfo);

   MAC_FREE_SHRABL_BUF(pst->region, pst->pool, cellStartInfo, \
	 sizeof(MacCellStartInfo));

   return ROK;
}

/*******************************************************************
 *
 * @brief Handles cell stop request from DU APP
 *
 * @details
 *
 *    Function : MacProcCellStartReq
 *
 *    Functionality:
 *        Handles cell stop request from DU APP
 *
 * @params[in] Post structure pointer
 *             Mac Cell stop information
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t MacProcCellStopReq(Pst *pst, MacCellStopInfo  *cellStopInfo)
{
#ifdef INTEL_FAPI
   DU_LOG("\nMAC : Sending cell stop request to Lower Mac");
   sendToLowerMac(FAPI_STOP_REQUEST, 0, cellStopInfo);
#endif

   MAC_FREE_SHRABL_BUF(pst->region, pst->pool, cellStopInfo, \
	 sizeof(MacCellStopInfo));

   return ROK;
}

/*******************************************************************
 *
 * @brief Handles DL CCCH Ind from DU APP
 *
 * @details
 *
 *    Function : MacProcDlCcchInd 
 *
 *    Functionality:
 *      Handles DL CCCH Ind from DU APP
 *
 * @params[in] Post structure pointer
 *             DL CCCH Ind pointer 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t MacProcDlCcchInd(Pst *pst, DlCcchIndInfo *dlCcchIndInfo)
{
   uint16_t cellIdx;
   uint8_t  ueIdx = 0;
   uint16_t idx;
   DlRlcBOInfo  dlBoInfo;
   memset(&dlBoInfo, 0, sizeof(DlRlcBOInfo));

   DU_LOG("\nMAC : Handling DL CCCH IND");

   GET_CELL_IDX(dlCcchIndInfo->cellId, cellIdx);

   dlBoInfo.cellId = dlCcchIndInfo->cellId;
   dlBoInfo.crnti = dlCcchIndInfo->crnti;
   
   if(dlCcchIndInfo->msgType == RRC_SETUP)
   {
      dlBoInfo.lcId = SRB_ID_0;    // SRB ID 0 for msg4
      dlBoInfo.dataVolume = dlCcchIndInfo->dlCcchMsgLen;

      /* storing Msg4 Pdu in raCb */
      GET_UE_IDX(dlBoInfo.crnti, ueIdx);
      if(macCb.macCell[cellIdx]->macRaCb[ueIdx-1].crnti == dlCcchIndInfo->crnti)
      {
	 macCb.macCell[cellIdx]->macRaCb[ueIdx-1].msg4PduLen = dlCcchIndInfo->dlCcchMsgLen;
	 MAC_ALLOC(macCb.macCell[cellIdx]->macRaCb[ueIdx-1].msg4Pdu, \
	    macCb.macCell[cellIdx]->macRaCb[ueIdx-1].msg4PduLen);
	 if(macCb.macCell[cellIdx]->macRaCb[ueIdx-1].msg4Pdu)
	 {
	    for(idx = 0; idx < dlCcchIndInfo->dlCcchMsgLen; idx++)
	    {
	       macCb.macCell[cellIdx]->macRaCb[ueIdx-1].msg4Pdu[idx] =\
	          dlCcchIndInfo->dlCcchMsg[idx];
	    }
	 }
      }
   }
   sendDlRlcBoInfoMacToSch(&dlBoInfo);

   MAC_FREE_SHRABL_BUF(pst->region, pst->pool, dlCcchIndInfo->dlCcchMsg, \
	 dlCcchIndInfo->dlCcchMsgLen);
   MAC_FREE_SHRABL_BUF(pst->region, pst->pool, dlCcchIndInfo, sizeof(DlCcchIndInfo));
   return ROK;

}

/*******************************************************************
 *
 * @brief Sends UL CCCH Ind to DU APP
 *
 * @details
 *
 *    Function : macSendUlCcchInd
 *
 *    Functionality:
 *        MAC sends UL CCCH Ind to DU APP
 *
 * @params[in] Post structure pointer
 *            
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t macSendUlCcchInd(uint8_t *rrcContainer, uint16_t cellId, uint16_t crnti)
{
   Pst pst;
   uint8_t ret = ROK;
   UlCcchIndInfo *ulCcchIndInfo = NULLP;

   MAC_ALLOC_SHRABL_BUF(ulCcchIndInfo, sizeof(UlCcchIndInfo));
   if(!ulCcchIndInfo)
   {
      DU_LOG("\nMAC: Memory failed in macSendUlCcchInd");
      return RFAILED;
   }

   ulCcchIndInfo->cellId = cellId;
   ulCcchIndInfo->crnti  = crnti;
   ulCcchIndInfo->ulCcchMsg = rrcContainer;

   /* Fill Pst */
   FILL_PST_MAC_TO_DUAPP(pst, EVENT_MAC_UL_CCCH_IND);

   if(MacDuAppUlCcchInd(&pst, ulCcchIndInfo) != ROK)
   {
      DU_LOG("\nMAC: Failed to send UL CCCH Ind to DU APP");
      MAC_FREE_SHRABL_BUF(MAC_MEM_REGION, MAC_POOL, ulCcchIndInfo->ulCcchMsg,
	    strlen((const char*)ulCcchIndInfo->ulCcchMsg));
      MAC_FREE_SHRABL_BUF(MAC_MEM_REGION, MAC_POOL, ulCcchIndInfo, sizeof(UlCcchIndInfo));
      ret = RFAILED;
   }
   return ret;
}

/**********************************************************************
  End of file
 **********************************************************************/

