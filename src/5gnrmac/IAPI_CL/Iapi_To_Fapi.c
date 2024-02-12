#include "common_def.h"
#ifdef INTEL_FAPI
#include "nr5g_fapi_internal.h"
#include "fapi_vendor_extension.h"
#endif
#ifdef INTEL_WLS_MEM
#include "wls_lib.h"
#endif
#ifdef INTEL_API
#include "common_mac_phy_api.h"
#include "nr5g_mac_phy_api.h"
#endif
#include "lwr_mac.h"
#include "lwr_mac_fsm.h"
#include "lwr_mac_phy.h"
#include "utilities.h"

void convertConfigRespIapiToFapi(void *iapiMsgBuf, fapi_config_resp_t *fapiMsg)
{
   PCONFIGRESPONSEStruct iapiMsg = (PCONFIGRESPONSEStruct)iapiMsgBuf;

   memset(fapiMsg, 0, sizeof(fapi_config_resp_t));
   fapiMsg->header.msg_id = FAPI_CONFIG_RESPONSE;
   fapiMsg->header.length = sizeof(fapi_config_resp_t) - sizeof(fapi_msg_t);
   fapiMsg->error_code = iapiMsg->nStatus;

   return;
}

void convertStopIndIapiToFapi(void *iapiMsgBuf, fapi_stop_ind_t *fapiMsg)
{
   PSTOPRESPONSEStruct iapiMsg = (PSTOPRESPONSEStruct)iapiMsgBuf;

   memset(fapiMsg, 0, sizeof(fapi_stop_ind_t));
   fapiMsg->header.msg_id = FAPI_STOP_INDICATION;
   fapiMsg->header.length = sizeof(fapi_stop_ind_t) - sizeof(fapi_msg_t);

   return;
}

void convertSlotIndIapiToFapi(void *iapiMsgBuf, fapi_slot_ind_t *fapiMsg)
{
   PSlotIndicationStruct iapiMsg = (PSlotIndicationStruct)iapiMsgBuf;

   memset(fapiMsg, 0, sizeof(fapi_slot_ind_t));
   fapiMsg->header.msg_id = FAPI_SLOT_INDICATION;
   fapiMsg->header.length = sizeof(fapi_slot_ind_t) - sizeof(fapi_msg_t);
   fapiMsg->sfn = iapiMsg->sSFN_Slot.nSFN;
   fapiMsg->slot = iapiMsg->sSFN_Slot.nSlot;

   return;
}

int8_t checkStartSymbSlotFreqIdxPresent(fapi_rach_indication_t *fapiMsg, uint8_t startSymbIdx, \
   uint16_t startSlotIdx, uint16_t freqIdx)
{
   int8_t pduIdx = -1;

   for(pduIdx = 0; pduIdx < fapiMsg->numPdus; pduIdx++)
   {
      if((fapiMsg->rachPdu[pduIdx].symbolIndex == startSymbIdx) &&
         (fapiMsg->rachPdu[pduIdx].slotIndex == startSlotIdx) &&
         (fapiMsg->rachPdu[pduIdx].freqIndex == freqIdx))
      {
         return pduIdx;
      }
   }
   return -1;
}

void convertRachIndIapiToFapi(void *iapiMsgBuf, fapi_rach_indication_t *fapiMsg)
{
   int8_t pduIdx = -1;
   uint8_t preambleEntryIdx = 0;
   UlSlotInfo *ulSlotInfo = NULLP;
   PRXRACHIndicationStruct iapiMsg = (PRXRACHIndicationStruct)iapiMsgBuf;
   PreambleStruct *iapiPreambleInfo = NULLP;
   fapi_rach_pdu_t *fapiRachPdu = NULLP;

   memset(fapiMsg, 0, sizeof(fapi_rach_indication_t));
   fapiMsg->header.msg_id = FAPI_RACH_INDICATION;
   fapiMsg->header.length = sizeof(fapi_rach_indication_t) - sizeof(fapi_msg_t);
   fapiMsg->sfn = iapiMsg->sSFN_Slot.nSFN;
   fapiMsg->slot = iapiMsg->sSFN_Slot.nSlot;

   ulSlotInfo = getUlSlotInfo(fapiMsg->sfn, fapiMsg->slot);
   if(!ulSlotInfo->rachPresent)
      return;

   for(preambleEntryIdx=0; preambleEntryIdx < iapiMsg->nNrOfPreamb; preambleEntryIdx++)
   {
      iapiPreambleInfo = &iapiMsg->sPreambleStruct[preambleEntryIdx];

      pduIdx = checkStartSymbSlotFreqIdxPresent(fapiMsg, iapiPreambleInfo->nStartSymbIdx, \
         iapiPreambleInfo->nStartSlotdx, iapiPreambleInfo->nFreqIdx);

      if(pduIdx == -1)
      {
         fapiRachPdu = &fapiMsg->rachPdu[fapiMsg->numPdus];
         fapiRachPdu->phyCellId = ulSlotInfo->rachInfo.phyCellId;
         fapiRachPdu->symbolIndex = iapiPreambleInfo->nStartSymbIdx;
         fapiRachPdu->slotIndex = iapiPreambleInfo->nStartSlotdx;
         fapiRachPdu->freqIndex = iapiPreambleInfo->nFreqIdx;
         fapiRachPdu->avgRssi = 0xFF;
         fapiRachPdu->avgSnr = 0xFF;
         fapiMsg->numPdus++;
      }
      else
      {
         fapiRachPdu = &fapiMsg->rachPdu[pduIdx];
      }

      fapiRachPdu->preambleInfo[fapiRachPdu->numPreamble].preambleIndex = iapiPreambleInfo->nPreambIdx;
      fapiRachPdu->preambleInfo[fapiRachPdu->numPreamble].timingAdvance = iapiPreambleInfo->nTa;
      fapiRachPdu->preambleInfo[fapiRachPdu->numPreamble].preamblePwr = iapiPreambleInfo->nPreambPwr;
      fapiRachPdu->numPreamble++;
   }
}

void convertRxUlSchIndIapiToFapi(void *iapiMsgBuf, fapi_rx_data_indication_t *fapiMsg)
{
   uint8_t idx=0, pduIdx = 0;
   UlSlotInfo *ulSlotInfo = NULLP;
   PuschInfo *puschInfo = NULLP;
   PRXULSCHIndicationStruct iapiMsg = (PRXULSCHIndicationStruct)iapiMsgBuf;

   memset(fapiMsg, 0, sizeof(fapi_rx_data_indication_t));
   fapiMsg->header.msg_id = FAPI_RX_DATA_INDICATION;
   fapiMsg->header.length = sizeof(fapi_rx_data_indication_t) - sizeof(fapi_msg_t);
   fapiMsg->sfn = iapiMsg->sSFN_Slot.nSFN;
   fapiMsg->slot = iapiMsg->sSFN_Slot.nSlot;

   ulSlotInfo = getUlSlotInfo(fapiMsg->sfn, fapiMsg->slot);

   fapiMsg->numPdus = iapiMsg->nUlsch;
   for(idx=0; idx<fapiMsg->numPdus; idx++)
   {
      puschInfo = NULLP;
      puschInfo = fetchPuschInfo(iapiMsg->sULSCHPDUDataStruct[idx].nUEId, ulSlotInfo);
      if(puschInfo)
      {
         fapiMsg->pdus[pduIdx].handle = puschInfo->handle;
         fapiMsg->pdus[pduIdx].rnti = iapiMsg->sULSCHPDUDataStruct[idx].nRNTI;
         fapiMsg->pdus[pduIdx].harqId = puschInfo->harqId;
         fapiMsg->pdus[pduIdx].ul_cqi = puschInfo->ulCqi;
         fapiMsg->pdus[pduIdx].timingAdvance = puschInfo->timingAdvance;
         fapiMsg->pdus[pduIdx].rssi = 880;
         fapiMsg->pdus[pduIdx].pdu_length = iapiMsg->sULSCHPDUDataStruct[idx].nPduLen;
         fapiMsg->pdus[pduIdx].pduData = (void *)iapiMsg->sULSCHPDUDataStruct[idx].pPayload;
         pduIdx++;
      }
   }
}

void fillUciPucchF0F1(fapi_uci_o_pucch_f0f1_t *uciPucchF0F1, PULUCIPDUDataStruct uciPdu, uint8_t pucchFormat)
{
   uciPucchF0F1->handle = uciPdu->nUEId;
   uciPucchF0F1->pucchFormat = pucchFormat;
   uciPucchF0F1->pduBitmap = 0;
   //uciPucchF0F1->ul_cqi
   uciPucchF0F1->rnti = uciPdu->nRNTI;
   uciPucchF0F1->timingAdvance = uciPdu->nTA = 31;
   uciPucchF0F1->rssi = 880;

   if(uciPdu->nSRPresent)
   {
      uciPucchF0F1->pduBitmap |= 0x01;
      uciPucchF0F1->srInfo.srIndication = 1;
      uciPucchF0F1->srInfo.srConfidenceLevel = 0xff;
   }

   if(uciPdu->pucchDetected == 1)
   {
      uciPucchF0F1->pduBitmap |= 0x02;
      uciPucchF0F1->harqInfo.numHarq = uciPdu->nPduBitLen;
      uciPucchF0F1->harqInfo.harqConfidenceLevel = 0xff;
      memcpy(uciPucchF0F1->harqInfo.harqValue, uciPdu->nUciBits, uciPucchF0F1->harqInfo.numHarq * sizeof(uint8_t));
   }
   else
       uciPucchF0F1->rssi = 0;
}

void fillUciPucchF2F3F4(fapi_uci_o_pucch_f2f3f4_t *uciPucchF2F3F4, PULUCIPDUDataStruct uciPdu, uint8_t pucchFormat)
{
   uciPucchF2F3F4->handle = uciPdu->nUEId;
   uciPucchF2F3F4->pduBitmap = 0;
   uciPucchF2F3F4->pucchFormat = pucchFormat-2;
   //uciPucchF2F3F4->ul_cqi
   uciPucchF2F3F4->rnti = uciPdu->nRNTI;
   uciPucchF2F3F4->timingAdvance = uciPdu->nTA = 31;
   uciPucchF2F3F4->rssi = 880;

   if(uciPdu->nSRPresent)
      uciPucchF2F3F4->pduBitmap |= 0x01;
   if(uciPdu->pucchDetected == 1)
      uciPucchF2F3F4->pduBitmap |= 0x02;
   else
      uciPucchF2F3F4->rssi = 0;

   uciPucchF2F3F4->num_uci_bits = uciPdu->nPduBitLen;
   memcpy(uciPucchF2F3F4->uciBits, uciPdu->nUciBits, uciPucchF2F3F4->num_uci_bits * sizeof(uint8_t));
}

void convertRxUciIndIapiToFapi(void *iapiMsgBuf, fapi_uci_indication_t *fapiMsg)
{
   uint8_t idx=0, pduIdx = 0;
   UlSlotInfo *ulSlotInfo = NULLP;
   PucchInfo *pucchInfo = NULLP;
   PRXUCIIndicationStruct iapiMsg = (PRXUCIIndicationStruct)iapiMsgBuf;

   memset(fapiMsg, 0, sizeof(fapi_uci_indication_t));
   fapiMsg->header.msg_id = FAPI_UCI_INDICATION;
   fapiMsg->header.length = sizeof(fapi_uci_indication_t) - sizeof(fapi_msg_t);
   fapiMsg->sfn = iapiMsg->sSFN_Slot.nSFN;
   fapiMsg->slot = iapiMsg->sSFN_Slot.nSlot;

   ulSlotInfo = getUlSlotInfo(fapiMsg->sfn, fapiMsg->slot);

   fapiMsg->numUcis = iapiMsg->nUCI;
   for(idx=0; idx < fapiMsg->numUcis; idx++)
   {
      pucchInfo = fetchPucchInfo(iapiMsg->sULUCIPDUDataStruct[idx].nUEId, ulSlotInfo);
      fapiMsg->uciPdu[idx].pduType = pucchInfo->pucchFormat;

      switch(fapiMsg->uciPdu[idx].pduType)
      {
         case 0:
         case 1:
            {
               fapiMsg->uciPdu[idx].pduSize = sizeof(fapi_uci_o_pucch_f0f1_t);
               fillUciPucchF0F1(&fapiMsg->uciPdu[idx].uci.uciPucchF0F1, &iapiMsg->sULUCIPDUDataStruct[idx], \
                  pucchInfo->pucchFormat);
               break;
            }

         case 2:
         case 3:
         case 4:
            {
               fapiMsg->uciPdu[idx].pduSize = sizeof(fapi_uci_o_pucch_f2f3f4_t);
               fillUciPucchF2F3F4(&fapiMsg->uciPdu[idx].uci.uciPucchF2F3F4, &iapiMsg->sULUCIPDUDataStruct[idx], \
                  pucchInfo->pucchFormat);
               break;
            }
      }
   }
}

void fillUciPusch(fapi_uci_o_pusch_t *uciPusch, ULSCHUCIPDUDataStruct *uciPdu, PuschInfo *puschInfo )
{
   uciPusch->handle=puschInfo->handle;
   if(uciPdu->nUciDetected)
   {
      uciPusch->pduBitmap |= 0x02;
   } 
   else if(uciPdu->nUciCsiP1Detected)
   {
      uciPusch->pduBitmap |= 0x04;
   }
   else if(uciPdu->nUciCsiP2Detected)
   {
      uciPusch->pduBitmap |= 0x08;
   }
   uciPusch->ul_cqi = puschInfo->ulCqi;
   uciPusch->rnti=uciPdu->nRNTI;
   uciPusch->timingAdvance=puschInfo->timingAdvance;
   uciPusch->rssi=0xffff;


   uciPusch->harqInfo.harqCrc= !uciPdu->nUciCrc;
   uciPusch->harqInfo.harqBitLen= uciPdu->nPduUciAckLen;
   memcpy(uciPusch->harqInfo.harqPayload, uciPdu->nUciAckBits, ceil(uciPusch->harqInfo.harqBitLen/8));

   uciPusch->csiPart1info.csiPart1Crc=! uciPdu->nUciCsiP2Crc;
   uciPusch->csiPart1info.csiPart1BitLen=uciPdu->nPduUciCsiP2Len;
   memcpy(uciPusch->csiPart1info.csiPart1Payload, uciPdu->nUciCsiP2Bits, ceil(uciPusch->csiPart1info.csiPart1BitLen/8));

   uciPusch->csiPart2info.csiPart2Crc=! uciPdu->nUciCsiP2Crc;
   uciPusch->csiPart2info.csiPart2BitLen=uciPdu->nPduUciCsiP2Len;
   memcpy(uciPusch->csiPart2info.csiPart2Payload, uciPdu->nUciCsiP2Bits, ceil(uciPusch->csiPart2info.csiPart2BitLen/8));

}

void convertRxUlschUciIndIapiToFapi(void *iapiMsgBuf, fapi_uci_indication_t *fapiMsg)
{
   uint8_t idx=0, pduIdx = 0;
   UlSlotInfo *ulSlotInfo = NULLP;
   PuschInfo *puschInfo = NULLP;
   PRXULSCHUCIIndicationStruct iapiMsg = (PRXULSCHUCIIndicationStruct) iapiMsgBuf;

   memset(fapiMsg, 0, sizeof(fapi_uci_indication_t));
   fapiMsg->header.msg_id = FAPI_UCI_INDICATION;
   fapiMsg->header.length = sizeof(fapi_uci_indication_t) - sizeof(fapi_msg_t);
   fapiMsg->sfn = iapiMsg->sSFN_Slot.nSFN;
   fapiMsg->slot = iapiMsg->sSFN_Slot.nSlot;
   ulSlotInfo = getUlSlotInfo(fapiMsg->sfn, fapiMsg->slot);
   fapiMsg->numUcis = iapiMsg->nUlschUci;
   for(idx=0; idx < fapiMsg->numUcis; idx++)
   {
      puschInfo = fetchPuschInfo(iapiMsg->sULSCHUCIPDUDataStruct[idx].nUEId, ulSlotInfo);
      fapiMsg->uciPdu[idx].pduSize = sizeof(fapi_uci_o_pusch_t);
      fillUciPusch(&fapiMsg->uciPdu[idx].uci.uciPusch, &iapiMsg->sULSCHUCIPDUDataStruct[idx], puschInfo);
   }
}

void convertErrorIndIapiToFapi(void *iapiMsgBuf, fapi_error_ind_t *fapiMsg)
{
   PERRORIndicationStruct iapiMsg = (PERRORIndicationStruct)iapiMsgBuf;

   memset(fapiMsg, 0, sizeof(fapi_error_ind_t));
   fapiMsg->header.msg_id = FAPI_ERROR_INDICATION;
   fapiMsg->header.length = sizeof(fapi_error_ind_t) - sizeof(fapi_msg_t);
   fapiMsg->sfn = iapiMsg->sSFN_Slot.nSFN;
   fapiMsg->slot = iapiMsg->sSFN_Slot.nSlot;
   //fapiMsg->message_id;
   fapiMsg->error_code = iapiMsg->nStatus;
}

void convertCrcIndIapiToFapi(void *iapiMsgBuf, fapi_crc_ind_t *fapiMsg)
{
   uint8_t idx=0;
   UlSlotInfo *ulSlotInfo = NULLP;
   PuschInfo *puschInfo = NULLP;
   PCRCIndicationStruct iapiMsg = (PCRCIndicationStruct)iapiMsgBuf;

   memset(fapiMsg, 0, sizeof(fapi_crc_ind_t));
   fapiMsg->header.msg_id = FAPI_CRC_INDICATION;
   fapiMsg->header.length = sizeof(fapi_crc_ind_t) - sizeof(fapi_msg_t);
   fapiMsg->sfn = iapiMsg->sSFN_Slot.nSFN;
   fapiMsg->slot = iapiMsg->sSFN_Slot.nSlot;
   ulSlotInfo = getUlSlotInfo(fapiMsg->sfn, fapiMsg->slot);

   fapiMsg->numCrcs = iapiMsg->nCrc;
   for(idx=0; idx<fapiMsg->numCrcs; idx++)
   {
      puschInfo = fetchPuschInfo(iapiMsg->sULCRCStruct[idx].nUEId, ulSlotInfo);
      if(puschInfo)
      {
         fapiMsg->crc[idx].handle = puschInfo->handle;
         fapiMsg->crc[idx].rnti = iapiMsg->sULCRCStruct[idx].nRNTI;
         fapiMsg->crc[idx].harqId = puschInfo->harqId;
         fapiMsg->crc[idx].tbCrcStatus = iapiMsg->sULCRCStruct[idx].nCrcFlag;
         //fapiMsg->crc[idx].ul_cqi = iapiMsg->sULCRCStruct[idx].nSNR;
         fapiMsg->crc[idx].numCb = 0;
         fapiMsg->crc[idx].timingAdvance = iapiMsg->sULCRCStruct[idx].nTA = 31;
         fapiMsg->crc[idx].rssi = 880;
         /* Not to be filled since .numCb = 0 */
         //fapiMsg->crc[idx].cbCrcStatus[] = iapiMsg->sULCRCStruct[idx].;
      }
   }
}

void IapiToFapiConvertAndSend(void *iapiMsg)
{
   /*
    * Convert from IAPI to FAPI
    * No need to fill message header queue element
    * Also for each message, no need to fill message's queue element, we can
    * directly fill message and sent in the 3rd param below:
    *
    * procPhyMessages(currElem->msg_type, 0, (void *)(currElem + 1));
    *
    * Also, this function is called separately for each messages in a queue, 
    * hence we dont need a loop to read next element in the queue.
    */
   L1L2MessageHdr   *msgHdr = NULLP;
   PMAC2PHY_QUEUE_EL pElem = NULLP;
   void *buffer;
   void *fapiMsg = NULLP;

   pElem = (PMAC2PHY_QUEUE_EL)iapiMsg;
   buffer = (void *)(pElem + 1);
   msgHdr = (L1L2MessageHdr *)buffer;

   switch(msgHdr->nMessageType) 
   {
      case MSG_TYPE_PHY_CONFIG_RESP:
         {
            fapi_config_resp_t fapiConfigResp;
            convertConfigRespIapiToFapi(buffer, &fapiConfigResp);
            fapiMsg = (void *)&fapiConfigResp;
            break;
         }
      case MSG_TYPE_PHY_SLOT_IND:
         {
            fapi_slot_ind_t fapiSlotInd;
            convertSlotIndIapiToFapi(buffer, &fapiSlotInd);
            fapiMsg = (void *)&fapiSlotInd;
            break;
         }
      case MSG_TYPE_PHY_RX_RACH_IND:
         {
            fapi_rach_indication_t fapiRachInd;
            convertRachIndIapiToFapi(buffer, &fapiRachInd);
            fapiMsg = (void *)&fapiRachInd;
            break;
         }
      case MSG_TYPE_PHY_ERR_IND:
         {
            fapi_error_ind_t fapiErrorInd;
            convertErrorIndIapiToFapi(buffer, &fapiErrorInd);
            fapiMsg = (void *)&fapiErrorInd;
            break;
         }
      case MSG_TYPE_PHY_RX_ULSCH_UCI_IND:
         {
            fapi_uci_indication_t fapiUciInd;
            convertRxUlschUciIndIapiToFapi(buffer, &fapiUciInd);
            fapiMsg = (void *)&fapiUciInd;
            break;
         }
      case MSG_TYPE_PHY_CRC_IND:
         {
            fapi_crc_ind_t fapiCrcInd;
            convertCrcIndIapiToFapi(buffer, &fapiCrcInd);
            fapiMsg = (void *)&fapiCrcInd;
            break;
         }
      case MSG_TYPE_PHY_UCI_IND:
         {
            fapi_uci_indication_t fapiUciInd;
            convertRxUciIndIapiToFapi(buffer, &fapiUciInd);
            fapiMsg = (void *)&fapiUciInd;
            break;
         }
      case MSG_TYPE_PHY_RX_SRS_IND:
         {
            break;
         }
      case MSG_TYPE_PHY_RX_ULSCH_IND:
         {
            fapi_rx_data_indication_t fapiRxDataInd;
            convertRxUlSchIndIapiToFapi(buffer, &fapiRxDataInd);
            fapiMsg = (void *)&fapiRxDataInd;
            break;
         }
      case MSG_TYPE_PHY_STOP_RESP:
         {
            fapi_stop_ind_t fapiStopInd;
            convertStopIndIapiToFapi(buffer, &fapiStopInd);
            fapiMsg = (void *)&fapiStopInd;
            break;
         }
   }

   procPhyMessages(((fapi_msg_t*)(fapiMsg))->msg_id, 0, fapiMsg);
}
