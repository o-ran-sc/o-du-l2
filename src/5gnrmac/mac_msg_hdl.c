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
#include "lrg.x"
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

MacCb  macCb;

uint16_t buildMacPdu(RlcData *dlData);
#ifdef EGTP_TEST
void macStubBuildUlData(Buffer *mBuf);
#endif

/* Function pointer for sending crc ind from MAC to SCH */
MacSchCrcIndFunc macSchCrcIndOpts[]=
{
   packMacSchCrcInd,
   MacSchCrcInd,
   packMacSchCrcInd
};

/* Function pointer for sending DL RLC BO Info from MAC to SCH */
MacSchDlRlcBoInfoFunc macSchDlRlcBoInfoOpts[]=
{
   packMacSchDlRlcBoInfo,
   MacSchDlRlcBoInfo,
   packMacSchDlRlcBoInfo
};

/* Function pointer for sending short BSR from MAC to SCH */
MacSchBsrFunc macSchBsrOpts[]=
{
   packMacSchBsr,
   MacSchBsr,
   packMacSchBsr
};

/* Function pointer for sending SR Uci ind from MAC to SCH */
MacSchSrUciIndFunc macSchSrUciIndOpts[]=
{
   packMacSchSrUciInd,
   MacSchSrUciInd,
   packMacSchSrUciInd
};

/*******************************************************************
 *
 * @brief Sends DL BO Info to SCH
 *
 * @details
 *
 *    Function : sendDlRlcBoInfoToSch
 *
 *    Functionality:
 *       Sends DL BO Info to SCH
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 ****************************************************************/
uint8_t sendDlRlcBoInfoToSch(DlRlcBoInfo *dlBoInfo)
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
   DU_LOG("\nDEBUG  -->  MAC : Received CRC indication");
   GET_CELL_IDX(crcInd->cellId, cellIdx);
   /* Considering one pdu and one preamble */ 
   crcIndInfo.cellId = macCb.macCell[cellIdx]->cellId;
   crcIndInfo.crnti = crcInd->crcInfo[0].rnti;
   crcIndInfo.timingInfo.sfn = crcInd->timingInfo.sfn;
   crcIndInfo.timingInfo.slot = crcInd->timingInfo.slot;
   crcIndInfo.numCrcInd = crcInd->crcInfo[0].numCb;
   crcIndInfo.crcInd[0] = crcInd->crcInfo[0].cbCrcStatus[0];

   MAC_FREE_SHRABL_BUF(pst->region, pst->pool, crcInd, sizeof(CrcInd));
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
   DU_LOG("\nDEBUG  -->  MAC : Received Rx Data indication");
   /* TODO : compare the handle received in RxDataInd with handle send in PUSCH
    * PDU, which is stored in raCb */

   for(pduIdx = 0; pduIdx < rxDataInd->numPdus; pduIdx++)
   {
      unpackRxData(rxDataInd->cellId, rxDataInd->timingInfo, &rxDataInd->pdus[pduIdx]);
      MAC_FREE_SHRABL_BUF(pst->region, pst->pool, rxDataInd->pdus[pduIdx].pduData,\
         rxDataInd->pdus[pduIdx].pduLength);
   }
   MAC_FREE_SHRABL_BUF(pst->region, pst->pool, rxDataInd, sizeof(RxDataInd));
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
   uint8_t   pduIdx = 0;
   uint8_t   lcIdx = 0;
   uint8_t   *txPdu = NULLP;
   uint16_t  cellIdx = 0, txPduLen = 0;
   MacDlData macDlData;
   MacDlSlot *currDlSlot = NULLP;
   DlRlcBoInfo dlBoInfo;

   memset(&macDlData , 0, sizeof(MacDlData));
   DU_LOG("\nDEBUG  -->  MAC: Received DL data for sfn=%d slot=%d numPdu= %d", \
      dlData->slotInfo.sfn, dlData->slotInfo.slot, dlData->numPdu);
   /* Copy the pdus to be muxed into mac Dl data */
   macDlData.numPdu = dlData->numPdu;
   for(pduIdx = 0;  pduIdx < dlData->numPdu; pduIdx++)
   {
      macDlData.pduInfo[pduIdx].lcId = dlData->pduInfo[pduIdx].lcId;
      macDlData.pduInfo[pduIdx].pduLen = dlData->pduInfo[pduIdx].pduLen;
      macDlData.pduInfo[pduIdx].dlPdu = dlData->pduInfo[pduIdx].pduBuf;
   }

   GET_CELL_IDX(dlData->cellId, cellIdx);
   /* Store DL data in the scheduled slot */
   if(macCb.macCell[cellIdx] ==NULLP)
   {
      DU_LOG("\nERROR  -->  MAC : MacProcRlcDlData(): macCell does not exists");
      return RFAILED;
   }
   currDlSlot = &macCb.macCell[cellIdx]->dlSlot[dlData->slotInfo.slot];
   if(currDlSlot->dlInfo.dlMsgAlloc)
   {
      txPduLen = currDlSlot->dlInfo.dlMsgAlloc->dlMsgPdschCfg.codeword[0].tbSize - TX_PAYLOAD_HDR_LEN;
      MAC_ALLOC(txPdu, txPduLen);
      if(!txPdu)
      {
         DU_LOG("\nERROR  -->  MAC : Memory allocation failed in MacProcRlcDlData");
         return RFAILED;
      }
      macMuxPdu(&macDlData, NULLP, txPdu, txPduLen);

      currDlSlot->dlInfo.dlMsgAlloc->dlMsgInfo.dlMsgPduLen = txPduLen;
      currDlSlot->dlInfo.dlMsgAlloc->dlMsgInfo.dlMsgPdu = txPdu;
   }

   for(lcIdx = 0; lcIdx < dlData->numLc; lcIdx++)
   {
      if(dlData->boStatus[lcIdx].bo)
      {
         memset(&dlBoInfo, 0, sizeof(DlRlcBoInfo));
         dlBoInfo.cellId = dlData->boStatus[lcIdx].cellId;
         GET_CRNTI(dlBoInfo.crnti, dlData->boStatus[lcIdx].ueIdx);
         dlBoInfo.lcId = dlData->boStatus[lcIdx].lcId;
         dlBoInfo.dataVolume = dlData->boStatus[lcIdx].bo;
         sendDlRlcBoInfoToSch(&dlBoInfo);
      }
   }

   /* Free memory */
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
 * @brief Builds and Sends UL Data to RLC
 *
 * @details
 *
 *    Function : macProcUlData
 *
 *    Functionality: Builds and Sends UL Data to RLC
 *
 * @params[in] CellId
 *             CRNTI
 *             Slot information
 *             LC Id on which payload was received
 *             Pointer to the payload
 *             Length of payload
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t macProcUlData(uint16_t cellId, uint16_t rnti, SlotTimingInfo slotInfo, \
uint8_t lcId, uint16_t pduLen, uint8_t *pdu)
{
   Pst         pst;
   RlcData     *ulData;

   /* Filling RLC Ul Data*/
   MAC_ALLOC_SHRABL_BUF(ulData, sizeof(RlcData));
   if(!ulData)
   {
      DU_LOG("\nERROR  -->  MAC : Memory allocation failed while sending UL data to RLC");
      return RFAILED;
   }
   memset(ulData, 0, sizeof(RlcData));
   ulData->cellId = cellId; 
   ulData->rnti = rnti;
   memcpy(&ulData->slotInfo, &slotInfo, sizeof(SlotTimingInfo));
   ulData->slotInfo.cellId = cellId;

   /* Filling pdu info */
   if(lcId == SRB1_LCID || lcId == SRB2_LCID)
   {
      ulData->pduInfo[ulData->numPdu].commCh = true;
   }
   ulData->pduInfo[ulData->numPdu].lcId = lcId;
   ulData->pduInfo[ulData->numPdu].pduBuf = pdu;
   ulData->pduInfo[ulData->numPdu].pduLen = pduLen;
   ulData->numPdu++;

   /* Filling Post and send to RLC */
   memset(&pst, 0, sizeof(Pst));
   FILL_PST_MAC_TO_RLC(pst, 0, EVENT_UL_DATA_TO_RLC);
   MacSendUlDataToRlc(&pst, ulData);

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
uint8_t MacProcRlcBoStatus(Pst* pst, RlcBoStatus* boStatus)
{
   DlRlcBoInfo  dlBoInfo;

   dlBoInfo.cellId = boStatus->cellId;
   GET_CRNTI(dlBoInfo.crnti, boStatus->ueIdx);
   dlBoInfo.lcId = boStatus->lcId;
   dlBoInfo.dataVolume = boStatus->bo;

   sendDlRlcBoInfoToSch(&dlBoInfo); 

   if(pst->selector == ODU_SELECTOR_LWLC)
   {
      MAC_FREE_SHRABL_BUF(pst->region, pst->pool, boStatus, sizeof(RlcBoStatus));
   }

   return ROK;
}

/*******************************************************************
 *
 * @brief Send LC schedule result report to RLC
 *
 * @details
 *
 *    Function : sendSchRptToRlc 
 *
 *    Functionality: Send LC schedule result report to RLC
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t sendSchedRptToRlc(DlSchedInfo dlInfo, SlotTimingInfo slotInfo)
{
   Pst      pst;
   uint8_t  lcIdx;
   RlcSchedResultRpt  *schedRpt = NULLP;
   
   MAC_ALLOC_SHRABL_BUF(schedRpt, sizeof(RlcSchedResultRpt));
   if(!schedRpt)
   {
      DU_LOG("\nERROR  -->  MAC: Memory allocation failure in sendSchResultRepToRlc");
      return RFAILED;
   }

   DU_LOG("\nDEBUG  -->  MAC: Send scheduled result report for sfn %d slot %d", slotInfo.sfn, slotInfo.slot);
   schedRpt->cellId = dlInfo.cellId;
   schedRpt->rnti = dlInfo.dlMsgAlloc->crnti;
   schedRpt->numLc = dlInfo.dlMsgAlloc->numLc;
   schedRpt->slotInfo.sfn = slotInfo.sfn;
   schedRpt->slotInfo.slot = slotInfo.slot;

   for(lcIdx = 0; lcIdx < schedRpt->numLc; lcIdx++)
   {
      schedRpt->lcSch[lcIdx].lcId = dlInfo.dlMsgAlloc->lcSchInfo[lcIdx].lcId;
      schedRpt->lcSch[lcIdx].bufSize = dlInfo.dlMsgAlloc->lcSchInfo[lcIdx].schBytes;
      schedRpt->lcSch[lcIdx].commCh = false;
   }

   /* Fill Pst */
   FILL_PST_MAC_TO_RLC(pst, RLC_DL_INST, EVENT_SCHED_RESULT_TO_RLC);
   if(MacSendSchedResultRptToRlc(&pst, schedRpt) != ROK)
   {
      DU_LOG("\nERROR  -->  MAC: Failed to send Schedule result report to RLC");
      MAC_FREE_SHRABL_BUF(MAC_MEM_REGION, MAC_POOL, schedRpt, sizeof(RlcSchedResultRpt));
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
 *    Function : MacProcCellStart
 *
 *    Functionality:
 *      Handles cell start reuqest from DU APP
 *
 * @params[in] Post structure pointer
 *             Cell Id 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t MacProcCellStart(Pst *pst, OduCellId  *cellId)
{
   DU_LOG("\nINFO  -->  MAC : Handling cell start request");
   gSlotCount = 0;
   sendToLowerMac(START_REQUEST, 0, cellId);

   MAC_FREE_SHRABL_BUF(pst->region, pst->pool, cellId, \
	 sizeof(OduCellId));

   return ROK;
}

/*******************************************************************
 *
 * @brief Handles cell stop from DU APP
 *
 * @details
 *
 *    Function : MacProcCellStop
 *
 *    Functionality:
 *        Handles cell stop from DU APP
 *
 * @params[in] Post structure pointer
 *             Cell Id
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t MacProcCellStop(Pst *pst, OduCellId  *cellId)
{
#ifdef INTEL_FAPI
   uint16_t      cellIdx; 

   DU_LOG("\nINFO  -->  MAC : Sending cell stop request to Lower Mac");
   GET_CELL_IDX(cellId->cellId, cellIdx);
   if(macCb.macCell[cellIdx])
   {
      macCb.macCell[cellIdx]->state = CELL_TO_BE_STOPPED;
   }
#endif

   MAC_FREE_SHRABL_BUF(pst->region, pst->pool, cellId, \
	 sizeof(OduCellId));

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
   uint8_t      ueIdx = 0;
   uint16_t     cellIdx;
   uint16_t     idx;
   DlRlcBoInfo  dlBoInfo;
   memset(&dlBoInfo, 0, sizeof(DlRlcBoInfo));

   DU_LOG("\nDEBUG  -->  MAC : Handling DL CCCH IND");

   GET_CELL_IDX(dlCcchIndInfo->cellId, cellIdx);

   dlBoInfo.cellId = dlCcchIndInfo->cellId;
   dlBoInfo.crnti = dlCcchIndInfo->crnti;

   if(dlCcchIndInfo->msgType == RRC_SETUP)
   {
      dlBoInfo.lcId = SRB0_LCID;    // SRB ID 0 for msg4
      /* (MSG4 pdu + 3 bytes sub-header) + (Contention resolution id MAC CE + 1 byte sub-header) */
      dlBoInfo.dataVolume = (dlCcchIndInfo->dlCcchMsgLen + 3) + (MAX_CRI_SIZE + 1);

      /* storing Msg4 Pdu in raCb */
      GET_UE_IDX(dlBoInfo.crnti, ueIdx);
      ueIdx = ueIdx -1;
      if(macCb.macCell[cellIdx]->macRaCb[ueIdx].crnti == dlCcchIndInfo->crnti)
      {
	 macCb.macCell[cellIdx]->macRaCb[ueIdx].msg4PduLen = dlCcchIndInfo->dlCcchMsgLen;
	 MAC_ALLOC(macCb.macCell[cellIdx]->macRaCb[ueIdx].msg4Pdu, \
	    macCb.macCell[cellIdx]->macRaCb[ueIdx].msg4PduLen);
	 if(macCb.macCell[cellIdx]->macRaCb[ueIdx].msg4Pdu)
	 {
	    for(idx = 0; idx < dlCcchIndInfo->dlCcchMsgLen; idx++)
	    {
	       macCb.macCell[cellIdx]->macRaCb[ueIdx].msg4Pdu[idx] =\
	          dlCcchIndInfo->dlCcchMsg[idx];
	    }
	 }
      }
   }
   sendDlRlcBoInfoToSch(&dlBoInfo);

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
 *    Function : macProcUlCcchInd
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
uint8_t macProcUlCcchInd(uint16_t cellId, uint16_t crnti, uint16_t rrcContSize, uint8_t *rrcContainer)
{
   Pst pst;
   uint8_t ret = ROK;
   UlCcchIndInfo *ulCcchIndInfo = NULLP;

   MAC_ALLOC_SHRABL_BUF(ulCcchIndInfo, sizeof(UlCcchIndInfo));
   if(!ulCcchIndInfo)
   {
      DU_LOG("\nERROR  -->  MAC: Memory failed in macProcUlCcchInd");
      return RFAILED;
   }

   ulCcchIndInfo->cellId = cellId;
   ulCcchIndInfo->crnti  = crnti;
   ulCcchIndInfo->ulCcchMsgLen = rrcContSize;
   ulCcchIndInfo->ulCcchMsg = rrcContainer;

   /* Fill Pst */
   FILL_PST_MAC_TO_DUAPP(pst, EVENT_MAC_UL_CCCH_IND);

   if(MacDuAppUlCcchInd(&pst, ulCcchIndInfo) != ROK)
   {
      DU_LOG("\nERROR  -->  MAC: Failed to send UL CCCH Ind to DU APP");
      MAC_FREE_SHRABL_BUF(MAC_MEM_REGION, MAC_POOL, ulCcchIndInfo->ulCcchMsg, ulCcchIndInfo->ulCcchMsgLen);
      MAC_FREE_SHRABL_BUF(MAC_MEM_REGION, MAC_POOL, ulCcchIndInfo, sizeof(UlCcchIndInfo));
      ret = RFAILED;
   }
   return ret;
}

/*******************************************************************
 *
 * @brief Processes received short BSR
 *
 * @details
 *
 *    Function : macProcShortBsr
 *
 *    Functionality:
 *        MAC sends Short BSR to SCH
 *
 * @params[in] cell ID
 *             crnti
 *             lcg ID
 *             buffer size
 *
 *
 * ****************************************************************/
uint8_t macProcShortBsr(uint16_t cellId, uint16_t crnti, uint8_t lcgId, uint32_t bufferSize)
{
   Pst                  pst;
   UlBufferStatusRptInd bsrInd;

   memset(&pst, 0, sizeof(Pst));
   memset(&bsrInd, 0, sizeof(UlBufferStatusRptInd));

   bsrInd.cellId                 = cellId;
   bsrInd.crnti                  = crnti;
   bsrInd.bsrType                = SHORT_BSR;
   bsrInd.numLcg                 = 1; /* short bsr reports one lcg at a time */
   bsrInd.dataVolInfo[0].lcgId   = lcgId;
   bsrInd.dataVolInfo[0].dataVol = bufferSize;

   FILL_PST_MAC_TO_SCH(pst, EVENT_SHORT_BSR);
   return(*macSchBsrOpts[pst.selector])(&pst, &bsrInd);
}

/*******************************************************************
 *
 * @brief Builds and send SR UCI Indication to SCH
 *
 * @details
 *
 *    Function : buildAndSendSrInd
 *
 *    Functionality:
 *       Builds and send SR UCI Indication to SCH
 *
 * @params[in] SrUciIndInfo Pointer
 *             crnti value
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t buildAndSendSrInd(UciInd *macUciInd, uint8_t crnti)
{
   uint16_t cellIdx;
   Pst pst;
   SrUciIndInfo   srUciInd;
   memset(&pst, 0, sizeof(Pst));
   memset(&srUciInd, 0, sizeof(SrUciIndInfo));

   GET_CELL_IDX(macUciInd->cellId, cellIdx);
   srUciInd.cellId       = macCb.macCell[cellIdx]->cellId;
   srUciInd.crnti        = crnti;
   srUciInd.slotInd.sfn  = macUciInd->slotInd.sfn;
   srUciInd.slotInd.slot = macUciInd->slotInd.slot;
   srUciInd.numSrBits++;
   memset(srUciInd.srPayload, 0, MAX_SR_BITS_IN_BYTES);

   /* Fill Pst */
   FILL_PST_MAC_TO_SCH(pst, EVENT_UCI_IND_TO_SCH);

   return(*macSchSrUciIndOpts[pst.selector])(&pst, &srUciInd);
}

/*******************************************************************
 *
 * @brief Processes UCI Indication from PHY
 *
 * @details
 *
 *    Function : fapiMacUciInd
 *
 *    Functionality:
 *       Processes UCI Indication from PHY
 *
 * @params[in] Post Structure Pointer
 *             UCI Indication Pointer
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t FapiMacUciInd(Pst *pst, UciInd *macUciInd)
{
   uint8_t     pduIdx = 0;
   uint8_t     ret = ROK;
   uint16_t    nPdus;
   uint16_t    crnti;

   if(macUciInd)
   {
      nPdus = macUciInd->numUcis;
      while(nPdus)
      {
         switch(macUciInd->pdus[pduIdx].pduType)
         {
            case UCI_IND_PUSCH:
               break;
            case UCI_IND_PUCCH_F0F1:
               if(macUciInd->pdus[pduIdx].uci.uciPucchF0F1.srInfo.srIndPres)
               {
                  DU_LOG("\nDEBUG  -->  MAC : Received SR UCI indication");
		  crnti = macUciInd->pdus[pduIdx].uci.uciPucchF0F1.crnti; 
		  ret = buildAndSendSrInd(macUciInd, crnti);
               }
               break;
            case UCI_IND_PUCCH_F2F3F4:
               break;
            default:
               DU_LOG("\nERROR  -->  MAC: Invalid Pdu Type %d at FapiMacUciInd", macUciInd->pdus[pduIdx].pduType);
               ret = RFAILED;
               break;
         }
         pduIdx++;
         nPdus--;
      }
   }
   else
   {
      DU_LOG("\nERROR  -->  MAC: Received Uci Ind is NULL at FapiMacUciInd()");
      ret = RFAILED;
   }
   MAC_FREE_SHRABL_BUF(pst->region, pst->pool, macUciInd, sizeof(UciInd));
   return ret;
}


/**********************************************************************
  End of file
 **********************************************************************/

