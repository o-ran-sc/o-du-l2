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

/* This file is the entry point for P7 Msg Handling */
/*Reference SCF225 version: 225.2.1 */
#ifdef NFAPI_ENABLED

#include "common_def.h"
#include "mac_utils.h"
#include "lwr_mac.h"
#include "lwr_mac_utils.h"
#include "lwr_mac_fsm.h"
#include "lwr_mac_upr_inf.h"
#include "fapi_vendor_extension.h"
#include "nfapi_interface.h"
#include "nfapi_common.h"
#include "nfapi_udp_p7.h"

/***********************************************************************
 *
 * @brief Pack parameters of Precoding and Beamforming
 *
 * @details
 *
 *    Function : nfapiFillDlPrecodingBeamform
 *
 *    Functionality:
 *       Pack parameters of Precoding and Beamforming and fill it in MsgBuffer
 *       of NFAPI
 *
 * @params[in] Buffer *mBuf, preCodingAndBeamforming
 * @return void
 *
 * ********************************************************************/

void nfapiFillDlPrecodingBeamform(fapi_precoding_bmform_t *preCodingAndBeamforming, Buffer *mBuf)
{
   CMCHKPK(oduPackPostUInt16, preCodingAndBeamforming->numPrgs, mBuf);
   CMCHKPK(oduPackPostUInt16, preCodingAndBeamforming->prgSize, mBuf);
   CMCHKPK(oduPackPostUInt8, preCodingAndBeamforming->digBfInterfaces, mBuf);
   for(uint8_t padIdx = 0; padIdx < 3; padIdx++)
   {
      CMCHKPK(oduPackPostUInt8, preCodingAndBeamforming->pad[padIdx], mBuf);
   }
   for(uint8_t prgIdx = 0; prgIdx < preCodingAndBeamforming->numPrgs; prgIdx++)
   {
      CMCHKPK(oduPackPostUInt16, preCodingAndBeamforming->pmi_bfi[prgIdx].pmIdx, mBuf);
      for(uint8_t padIdx = 0; padIdx < 2; padIdx++)
      {
         CMCHKPK(oduPackPostUInt8, preCodingAndBeamforming->pmi_bfi[prgIdx].pad[padIdx], mBuf);
      }
      for(uint8_t digBfIdx = 0; digBfIdx < preCodingAndBeamforming->digBfInterfaces; digBfIdx++)
      {
         CMCHKPK(oduPackPostUInt16, preCodingAndBeamforming->pmi_bfi[prgIdx].beamIdx[digBfIdx].beamidx, mBuf); 
      }
   }
}

/***********************************************************************
 *
 * @brief Fill P7 DL TTI REQ and send via UDP
 *
 * @details
 *
 *    Function : nfapiBuildAndSendDlTtiReq
 *
 *    Functionality:
 *       Fill P7 DL TTI REQ by packing each element of FAPI_DL_TTI_REQ 
 *
 * @params[in] FapiMsgBody, MsgLength
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ********************************************************************/

uint8_t nfapiBuildAndSendDlTtiReq(void *fapiMsg, uint32_t msgLen)
{
   Buffer  *mBuf = NULLP;
   uint8_t pduIdx = 0, freqIdx = 0, dciIndex = 0, ueGrpIdx = 0;
   uint8_t numBytes = 0, ret = ROK;

   p_fapi_api_queue_elem_t dlTtiElem = (p_fapi_api_queue_elem_t)fapiMsg;
   fapi_dl_tti_req_t *dlTtiReq = (fapi_dl_tti_req_t *)(dlTtiElem +1);
   
   if(ODU_GET_MSG_BUF(MAC_MEM_REGION, MAC_POOL, &mBuf) != ROK)
   {
      DU_LOG("\nERROR  --> NFAPI_VNF : Memory allocation failed in nfapiBuildAndSendDlTtiReq");
      return RFAILED;
   }
  
   nfapiFillP7Hdr(mBuf,(sizeof(fapi_dl_tti_req_msg_body) + sizeof(nFapi_msg_header)), 0, 0);
   nfapiFillMsgHdr(mBuf, vnfDb.vnfP7Info.p7SyncInfo.phyId, FAPI_DL_TTI_REQUEST, sizeof(fapi_dl_tti_req_msg_body));
   
   CMCHKPK(oduPackPostUInt16, dlTtiReq->sfn, mBuf);
   CMCHKPK(oduPackPostUInt16, dlTtiReq->slot, mBuf);
   CMCHKPK(oduPackPostUInt8, dlTtiReq->nPdus, mBuf);
   CMCHKPK(oduPackPostUInt8, dlTtiReq->nGroup, mBuf);
   for(pduIdx = 0; pduIdx < dlTtiReq->nPdus; pduIdx++)
   {
      CMCHKPK(oduPackPostUInt16, dlTtiReq->pdus[pduIdx].pduType, mBuf);
      CMCHKPK(oduPackPostUInt16, dlTtiReq->pdus[pduIdx].pduSize, mBuf);
      switch(dlTtiReq->pdus[pduIdx].pduType)
      {
         case PDCCH_PDU_TYPE:
         {
            CMCHKPK(oduPackPostUInt16, dlTtiReq->pdus[pduIdx].pdu.pdcch_pdu.bwpSize, mBuf); 
            CMCHKPK(oduPackPostUInt16, dlTtiReq->pdus[pduIdx].pdu.pdcch_pdu.bwpStart, mBuf); 
            CMCHKPK(oduPackPostUInt8, dlTtiReq->pdus[pduIdx].pdu.pdcch_pdu.subCarrierSpacing, mBuf); 
            CMCHKPK(oduPackPostUInt8, dlTtiReq->pdus[pduIdx].pdu.pdcch_pdu.cyclicPrefix, mBuf); 
            CMCHKPK(oduPackPostUInt8, dlTtiReq->pdus[pduIdx].pdu.pdcch_pdu.startSymbolIndex, mBuf); 
            CMCHKPK(oduPackPostUInt8, dlTtiReq->pdus[pduIdx].pdu.pdcch_pdu.durationSymbols, mBuf);
            for(freqIdx = 0; freqIdx < 6; freqIdx++)
            {
               CMCHKPK(oduPackPostUInt8, dlTtiReq->pdus[pduIdx].pdu.pdcch_pdu.freqDomainResource[freqIdx], mBuf);
            }
            CMCHKPK(oduPackPostUInt8, dlTtiReq->pdus[pduIdx].pdu.pdcch_pdu.cceRegMappingType, mBuf);
            CMCHKPK(oduPackPostUInt8, dlTtiReq->pdus[pduIdx].pdu.pdcch_pdu.regBundleSize, mBuf);
            CMCHKPK(oduPackPostUInt8, dlTtiReq->pdus[pduIdx].pdu.pdcch_pdu.interleaverSize, mBuf);
            CMCHKPK(oduPackPostUInt8, dlTtiReq->pdus[pduIdx].pdu.pdcch_pdu.coreSetType, mBuf);
            CMCHKPK(oduPackPostUInt16, dlTtiReq->pdus[pduIdx].pdu.pdcch_pdu.shiftIndex, mBuf);
            CMCHKPK(oduPackPostUInt8, dlTtiReq->pdus[pduIdx].pdu.pdcch_pdu.precoderGranularity, mBuf);
            CMCHKPK(oduPackPostUInt8, dlTtiReq->pdus[pduIdx].pdu.pdcch_pdu.pad, mBuf);
            CMCHKPK(oduPackPostUInt16, dlTtiReq->pdus[pduIdx].pdu.pdcch_pdu.numDlDci, mBuf);
            for(dciIndex = 0; dciIndex <  dlTtiReq->pdus[pduIdx].pdu.pdcch_pdu.numDlDci; dciIndex++)
            {
               CMCHKPK(oduPackPostUInt16, dlTtiReq->pdus[pduIdx].pdu.pdcch_pdu.dlDci[dciIndex].rnti, mBuf);
               CMCHKPK(oduPackPostUInt16, dlTtiReq->pdus[pduIdx].pdu.pdcch_pdu.dlDci[dciIndex].scramblingId, mBuf);
               CMCHKPK(oduPackPostUInt16, dlTtiReq->pdus[pduIdx].pdu.pdcch_pdu.dlDci[dciIndex].scramblingRnti, mBuf);
               CMCHKPK(oduPackPostUInt8, dlTtiReq->pdus[pduIdx].pdu.pdcch_pdu.dlDci[dciIndex].cceIndex, mBuf);
               CMCHKPK(oduPackPostUInt8, dlTtiReq->pdus[pduIdx].pdu.pdcch_pdu.dlDci[dciIndex].aggregationLevel, mBuf);
               
               nfapiFillDlPrecodingBeamform(&dlTtiReq->pdus[pduIdx].pdu.pdcch_pdu.dlDci[dciIndex].pc_and_bform, mBuf);
               
               CMCHKPK(oduPackPostUInt8, dlTtiReq->pdus[pduIdx].pdu.pdcch_pdu.dlDci[dciIndex].beta_pdcch_1_0, mBuf);
               CMCHKPK(oduPackPostUInt8, dlTtiReq->pdus[pduIdx].pdu.pdcch_pdu.dlDci[dciIndex].powerControlOffsetSS, mBuf);
               CMCHKPK(oduPackPostUInt16, dlTtiReq->pdus[pduIdx].pdu.pdcch_pdu.dlDci[dciIndex].payloadSizeBits, mBuf);
               numBytes = dlTtiReq->pdus[pduIdx].pdu.pdcch_pdu.dlDci[dciIndex].payloadSizeBits / 8;
               if(dlTtiReq->pdus[pduIdx].pdu.pdcch_pdu.dlDci[dciIndex].payloadSizeBits % 8)
                  numBytes += 1;

               for(uint8_t payloadIdx = 0; payloadIdx < numBytes; payloadIdx++)
               {
                  CMCHKPK(oduPackPostUInt8, dlTtiReq->pdus[pduIdx].pdu.pdcch_pdu.dlDci[dciIndex].payload[payloadIdx], mBuf);
               }
            }
            break;
         }
         case PDSCH_PDU_TYPE:
         {
            CMCHKPK(oduPackPostUInt16, dlTtiReq->pdus[pduIdx].pdu.pdsch_pdu.pduBitMap, mBuf);
            CMCHKPK(oduPackPostUInt16, dlTtiReq->pdus[pduIdx].pdu.pdsch_pdu.rnti, mBuf);
            CMCHKPK(oduPackPostUInt16, dlTtiReq->pdus[pduIdx].pdu.pdsch_pdu.pdu_index, mBuf);
            CMCHKPK(oduPackPostUInt16, dlTtiReq->pdus[pduIdx].pdu.pdsch_pdu.bwpSize, mBuf);
            CMCHKPK(oduPackPostUInt16, dlTtiReq->pdus[pduIdx].pdu.pdsch_pdu.bwpStart, mBuf);
            CMCHKPK(oduPackPostUInt8, dlTtiReq->pdus[pduIdx].pdu.pdsch_pdu.subCarrierSpacing, mBuf);
            CMCHKPK(oduPackPostUInt8, dlTtiReq->pdus[pduIdx].pdu.pdsch_pdu.cyclicPrefix, mBuf);
            CMCHKPK(oduPackPostUInt8, dlTtiReq->pdus[pduIdx].pdu.pdsch_pdu.nrOfCodeWords, mBuf);
            for(uint8_t padIdx = 0; padIdx < 3; padIdx++)
            {
               CMCHKPK(oduPackPostUInt8, dlTtiReq->pdus[pduIdx].pdu.pdsch_pdu.pad[padIdx], mBuf);
            }
            for(uint8_t cwIdx = 0; cwIdx <  dlTtiReq->pdus[pduIdx].pdu.pdsch_pdu.nrOfCodeWords; cwIdx++)
            {
               CMCHKPK(oduPackPostUInt16, dlTtiReq->pdus[pduIdx].pdu.pdsch_pdu.cwInfo[cwIdx].targetCodeRate, mBuf);
               CMCHKPK(oduPackPostUInt8, dlTtiReq->pdus[pduIdx].pdu.pdsch_pdu.cwInfo[cwIdx].qamModOrder, mBuf);
               CMCHKPK(oduPackPostUInt8, dlTtiReq->pdus[pduIdx].pdu.pdsch_pdu.cwInfo[cwIdx].mcsIndex, mBuf);
               CMCHKPK(oduPackPostUInt8, dlTtiReq->pdus[pduIdx].pdu.pdsch_pdu.cwInfo[cwIdx].mcsTable, mBuf);
               CMCHKPK(oduPackPostUInt8, dlTtiReq->pdus[pduIdx].pdu.pdsch_pdu.cwInfo[cwIdx].rvIndex, mBuf);
               for(uint8_t padIdx = 0; padIdx < 2; padIdx++)
               {
                  CMCHKPK(oduPackPostUInt8, dlTtiReq->pdus[pduIdx].pdu.pdsch_pdu.cwInfo[cwIdx].pad[padIdx], mBuf);
               }
               CMCHKPK(oduPackPostUInt32, dlTtiReq->pdus[pduIdx].pdu.pdsch_pdu.cwInfo[cwIdx].tbSize, mBuf);
            }
            CMCHKPK(oduPackPostUInt16, dlTtiReq->pdus[pduIdx].pdu.pdsch_pdu.dataScramblingId, mBuf);
            CMCHKPK(oduPackPostUInt8, dlTtiReq->pdus[pduIdx].pdu.pdsch_pdu.nrOfLayers, mBuf);
            CMCHKPK(oduPackPostUInt8, dlTtiReq->pdus[pduIdx].pdu.pdsch_pdu.transmissionScheme, mBuf);
            CMCHKPK(oduPackPostUInt8, dlTtiReq->pdus[pduIdx].pdu.pdsch_pdu.refPoint, mBuf);
            CMCHKPK(oduPackPostUInt8, dlTtiReq->pdus[pduIdx].pdu.pdsch_pdu.dmrsConfigType, mBuf);
            CMCHKPK(oduPackPostUInt16, dlTtiReq->pdus[pduIdx].pdu.pdsch_pdu.dlDmrsSymbPos, mBuf);
            CMCHKPK(oduPackPostUInt8, dlTtiReq->pdus[pduIdx].pdu.pdsch_pdu.scid, mBuf);
            CMCHKPK(oduPackPostUInt8, dlTtiReq->pdus[pduIdx].pdu.pdsch_pdu.numDmrsCdmGrpsNoData, mBuf);
            CMCHKPK(oduPackPostUInt8, dlTtiReq->pdus[pduIdx].pdu.pdsch_pdu.resourceAlloc, mBuf);
            CMCHKPK(oduPackPostUInt8, dlTtiReq->pdus[pduIdx].pdu.pdsch_pdu.pad1, mBuf);
            CMCHKPK(oduPackPostUInt16, dlTtiReq->pdus[pduIdx].pdu.pdsch_pdu.dlDmrsScramblingId, mBuf);
            CMCHKPK(oduPackPostUInt16, dlTtiReq->pdus[pduIdx].pdu.pdsch_pdu.dmrsPorts, mBuf);
            CMCHKPK(oduPackPostUInt16, dlTtiReq->pdus[pduIdx].pdu.pdsch_pdu.rbStart, mBuf);
            CMCHKPK(oduPackPostUInt16, dlTtiReq->pdus[pduIdx].pdu.pdsch_pdu.rbSize, mBuf);
            for(uint8_t rbBitMapIdx = 0; rbBitMapIdx < 36; rbBitMapIdx++)
            {
               CMCHKPK(oduPackPostUInt8, dlTtiReq->pdus[pduIdx].pdu.pdsch_pdu.rbBitmap[rbBitMapIdx], mBuf);
            }
            CMCHKPK(oduPackPostUInt8, dlTtiReq->pdus[pduIdx].pdu.pdsch_pdu.vrbToPrbMapping, mBuf);
            CMCHKPK(oduPackPostUInt8, dlTtiReq->pdus[pduIdx].pdu.pdsch_pdu.startSymbIndex, mBuf);
            CMCHKPK(oduPackPostUInt8, dlTtiReq->pdus[pduIdx].pdu.pdsch_pdu.nrOfSymbols, mBuf);
            CMCHKPK(oduPackPostUInt8, dlTtiReq->pdus[pduIdx].pdu.pdsch_pdu.ptrsPortIndex, mBuf);
            CMCHKPK(oduPackPostUInt8, dlTtiReq->pdus[pduIdx].pdu.pdsch_pdu.ptrsTimeDensity, mBuf);
            CMCHKPK(oduPackPostUInt8, dlTtiReq->pdus[pduIdx].pdu.pdsch_pdu.ptrsFreqDensity, mBuf);
            CMCHKPK(oduPackPostUInt8, dlTtiReq->pdus[pduIdx].pdu.pdsch_pdu.ptrsReOffset, mBuf);
            CMCHKPK(oduPackPostUInt8, dlTtiReq->pdus[pduIdx].pdu.pdsch_pdu.nEpreRatioOfPdschToPtrs, mBuf);
            
            nfapiFillDlPrecodingBeamform(&dlTtiReq->pdus[pduIdx].pdu.pdsch_pdu.preCodingAndBeamforming, mBuf);
            
            CMCHKPK(oduPackPostUInt8, dlTtiReq->pdus[pduIdx].pdu.pdsch_pdu.powerControlOffset, mBuf);
            CMCHKPK(oduPackPostUInt8, dlTtiReq->pdus[pduIdx].pdu.pdsch_pdu.powerControlOffsetSS, mBuf);
            CMCHKPK(oduPackPostUInt8, dlTtiReq->pdus[pduIdx].pdu.pdsch_pdu.isLastCbPresent, mBuf);
            CMCHKPK(oduPackPostUInt8, dlTtiReq->pdus[pduIdx].pdu.pdsch_pdu.isInlineTbCrc, mBuf);
            CMCHKPK(oduPackPostUInt32, dlTtiReq->pdus[pduIdx].pdu.pdsch_pdu.dlTbCrc, mBuf);
            CMCHKPK(oduPackPostUInt8, dlTtiReq->pdus[pduIdx].pdu.pdsch_pdu.mappingType, mBuf);
            CMCHKPK(oduPackPostUInt8, dlTtiReq->pdus[pduIdx].pdu.pdsch_pdu.nrOfDmrsSymbols, mBuf);
            CMCHKPK(oduPackPostUInt8, dlTtiReq->pdus[pduIdx].pdu.pdsch_pdu.dmrsAddPos, mBuf);
            CMCHKPK(oduPackPostUInt8, dlTtiReq->pdus[pduIdx].pdu.pdsch_pdu.pad2, mBuf);
            break;
         }
         case SSB_PDU_TYPE:
         {
            CMCHKPK(oduPackPostUInt16, dlTtiReq->pdus[pduIdx].pdu.ssb_pdu.physCellId, mBuf);
            CMCHKPK(oduPackPostUInt8, dlTtiReq->pdus[pduIdx].pdu.ssb_pdu.betaPss, mBuf);
            CMCHKPK(oduPackPostUInt8, dlTtiReq->pdus[pduIdx].pdu.ssb_pdu.ssbBlockIndex, mBuf);
            CMCHKPK(oduPackPostUInt8, dlTtiReq->pdus[pduIdx].pdu.ssb_pdu.ssbSubCarrierOffset, mBuf);
            CMCHKPK(oduPackPostUInt8, dlTtiReq->pdus[pduIdx].pdu.ssb_pdu.bchPayloadFlag, mBuf);
            CMCHKPK(oduPackPostUInt16, dlTtiReq->pdus[pduIdx].pdu.ssb_pdu.ssbOffsetPointA, mBuf);
            CMCHKPK(oduPackPostUInt32, dlTtiReq->pdus[pduIdx].pdu.ssb_pdu.bchPayload.bchPayload, mBuf);
            CMCHKPK(oduPackPostUInt8, dlTtiReq->pdus[pduIdx].pdu.ssb_pdu.bchPayload.phyMibPdu.dmrsTypeAPosition, mBuf);
            CMCHKPK(oduPackPostUInt8, dlTtiReq->pdus[pduIdx].pdu.ssb_pdu.bchPayload.phyMibPdu.pdcchConfigSib1, mBuf);
            CMCHKPK(oduPackPostUInt8, dlTtiReq->pdus[pduIdx].pdu.ssb_pdu.bchPayload.phyMibPdu.cellBarred, mBuf);
            CMCHKPK(oduPackPostUInt8, dlTtiReq->pdus[pduIdx].pdu.ssb_pdu.bchPayload.phyMibPdu.intraFreqReselection, mBuf);
    
            nfapiFillDlPrecodingBeamform(&dlTtiReq->pdus[pduIdx].pdu.ssb_pdu.preCodingAndBeamforming, mBuf);
            
            break;
         }
         default:
         {
            DU_LOG("\nERROR   --> NFAPI_VNF: Incorrect pduType:%d", dlTtiReq->pdus[pduIdx].pduType);
            ODU_PUT_MSG_BUF(mBuf);
            return RFAILED;
         }
      }
   }
   for(ueGrpIdx = 0; ueGrpIdx < dlTtiReq->nGroup; ueGrpIdx++)
   {
      CMCHKPK(oduPackPostUInt8, dlTtiReq->ue_grp_info[ueGrpIdx].nUe, mBuf);
      for(uint8_t padIdx = 0; padIdx < 3; padIdx++)
      {
         CMCHKPK(oduPackPostUInt8, dlTtiReq->ue_grp_info[ueGrpIdx].pad[padIdx], mBuf);
      }
      for(uint8_t ueIdx = 0; ueIdx < dlTtiReq->ue_grp_info[ueGrpIdx].nUe; ueIdx++)
      {
         CMCHKPK(oduPackPostUInt8, dlTtiReq->ue_grp_info[ueGrpIdx].pduIdx[ueIdx], mBuf); 
      }
   }
 
   ret = nfapiP7UdpSendMsg(mBuf);

   return ret;
}

/***********************************************************************
 *
 * @brief Pack parameters of UL Precoding and Beamforming
 *
 * @details
 *
 *    Function : nfapiFillUlPrecodingBeamform
 *
 *    Functionality:
 *       Pack parameters of Precoding and Beamforming and fill it in MsgBuffer
 *       of NFAPI
 *
 * @params[in] Buffer *mBuf, preCodingAndBeamforming
 * @return void
 *
 * ********************************************************************/

void nfapiFillUlRxBeamforming(fapi_ul_rx_bmform_pdu_t *ulBmform, Buffer *mBuf)
{
   CMCHKPK(oduPackPostUInt16, ulBmform->numPrgs, mBuf);
   CMCHKPK(oduPackPostUInt16, ulBmform->prgSize, mBuf);
   CMCHKPK(oduPackPostUInt8, ulBmform->digBfInterface, mBuf);
   for(uint8_t padIdx = 0; padIdx < 3; padIdx++)
   {
      CMCHKPK(oduPackPostUInt8, ulBmform->pad[padIdx], mBuf);
   }
   for(uint8_t prgIdx = 0; prgIdx < ulBmform->numPrgs; prgIdx++)
   {
      for(uint8_t digBfIdx = 0; digBfIdx < ulBmform->digBfInterface; digBfIdx++)
      {
         CMCHKPK(oduPackPostUInt16, ulBmform->rx_bfi[prgIdx].beamIdx[digBfIdx].beamidx, mBuf);
      }
   }
}

/***********************************************************************
 *
 * @brief Fill P7 UL TTI REQ and send via UDP
 *
 * @details
 *
 *    Function : nfapiBuildAndSendUlTtiReq
 *
 *    Functionality:
 *       Fill P7 UL TTI REQ by packing each element of FAPI_UL_TTI_REQ 
 *
 * @params[in] FapiMsgBody, MsgLength
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ********************************************************************/

uint8_t nfapiBuildAndSendUlTtiReq(void *fapiMsg, uint32_t msgLen)
{
   Buffer  *mBuf = NULLP;
   uint8_t pduIdx = 0, ueGrpIdx = 0;
   uint8_t ret = ROK;

   p_fapi_api_queue_elem_t ulTtiElem = (p_fapi_api_queue_elem_t)fapiMsg;
   fapi_ul_tti_req_t *ulTtiReq = (fapi_ul_tti_req_t *)(ulTtiElem +1);
   
   if(ODU_GET_MSG_BUF(MAC_MEM_REGION, MAC_POOL, &mBuf) != ROK)
   {
      DU_LOG("\nERROR  --> NFAPI_VNF : Memory allocation failed in nfapiBuildAndSendUlTtiReq");
      return RFAILED;
   }
  
   nfapiFillP7Hdr(mBuf,(sizeof(fapi_ul_tti_req_msg_body) + sizeof(nFapi_msg_header)), 0, 0);
   nfapiFillMsgHdr(mBuf, vnfDb.vnfP7Info.p7SyncInfo.phyId, FAPI_UL_TTI_REQUEST, sizeof(fapi_ul_tti_req_msg_body));
  
   CMCHKPK(oduPackPostUInt16, ulTtiReq->sfn, mBuf);
   CMCHKPK(oduPackPostUInt16, ulTtiReq->slot, mBuf);
   CMCHKPK(oduPackPostUInt8, ulTtiReq->nPdus, mBuf);
   CMCHKPK(oduPackPostUInt8, ulTtiReq->rachPresent, mBuf);
   CMCHKPK(oduPackPostUInt8, ulTtiReq->nUlsch, mBuf);
   CMCHKPK(oduPackPostUInt8, ulTtiReq->nUlcch, mBuf);
   CMCHKPK(oduPackPostUInt8, ulTtiReq->nGroup, mBuf);
   
   for(pduIdx = 0; pduIdx < ulTtiReq->nPdus; pduIdx++)
   {
       CMCHKPK(oduPackPostUInt16, ulTtiReq->pdus[pduIdx].pduType, mBuf); 
       CMCHKPK(oduPackPostUInt16, ulTtiReq->pdus[pduIdx].pduSize, mBuf); 
       switch(ulTtiReq->pdus[pduIdx].pduType)
       {
          case PRACH_PDU_TYPE:
          {
             CMCHKPK(oduPackPostUInt16, ulTtiReq->pdus[pduIdx].pdu.prach_pdu.physCellId, mBuf); 
             CMCHKPK(oduPackPostUInt8, ulTtiReq->pdus[pduIdx].pdu.prach_pdu.numPrachOcas, mBuf); 
             CMCHKPK(oduPackPostUInt8, ulTtiReq->pdus[pduIdx].pdu.prach_pdu.prachFormat, mBuf); 
             CMCHKPK(oduPackPostUInt8, ulTtiReq->pdus[pduIdx].pdu.prach_pdu.numRa, mBuf); 
             CMCHKPK(oduPackPostUInt8, ulTtiReq->pdus[pduIdx].pdu.prach_pdu.prachStartSymbol, mBuf); 
             CMCHKPK(oduPackPostUInt16, ulTtiReq->pdus[pduIdx].pdu.prach_pdu.numCs, mBuf);
             nfapiFillUlRxBeamforming(&ulTtiReq->pdus[pduIdx].pdu.prach_pdu.beamforming, mBuf);
             break;
          }
          case PUSCH_PDU_TYPE:
          {
             CMCHKPK(oduPackPostUInt16, ulTtiReq->pdus[pduIdx].pdu.pusch_pdu.pduBitMap, mBuf);
             CMCHKPK(oduPackPostUInt16, ulTtiReq->pdus[pduIdx].pdu.pusch_pdu.rnti, mBuf);
             CMCHKPK(oduPackPostUInt32, ulTtiReq->pdus[pduIdx].pdu.pusch_pdu.handle, mBuf);
             CMCHKPK(oduPackPostUInt16, ulTtiReq->pdus[pduIdx].pdu.pusch_pdu.bwpSize, mBuf);
             CMCHKPK(oduPackPostUInt16, ulTtiReq->pdus[pduIdx].pdu.pusch_pdu.bwpStart, mBuf);
             CMCHKPK(oduPackPostUInt8, ulTtiReq->pdus[pduIdx].pdu.pusch_pdu.subCarrierSpacing, mBuf);
             CMCHKPK(oduPackPostUInt8, ulTtiReq->pdus[pduIdx].pdu.pusch_pdu.cyclicPrefix, mBuf);
             CMCHKPK(oduPackPostUInt8, ulTtiReq->pdus[pduIdx].pdu.pusch_pdu.mcsIndex, mBuf);
             CMCHKPK(oduPackPostUInt8, ulTtiReq->pdus[pduIdx].pdu.pusch_pdu.mcsTable, mBuf);
             CMCHKPK(oduPackPostUInt16, ulTtiReq->pdus[pduIdx].pdu.pusch_pdu.targetCodeRate, mBuf);
             CMCHKPK(oduPackPostUInt8, ulTtiReq->pdus[pduIdx].pdu.pusch_pdu.qamModOrder, mBuf);
             CMCHKPK(oduPackPostUInt8, ulTtiReq->pdus[pduIdx].pdu.pusch_pdu.transformPrecoding, mBuf);
             CMCHKPK(oduPackPostUInt16, ulTtiReq->pdus[pduIdx].pdu.pusch_pdu.dataScramblingId, mBuf);
             CMCHKPK(oduPackPostUInt8, ulTtiReq->pdus[pduIdx].pdu.pusch_pdu.nrOfLayers, mBuf);
             CMCHKPK(oduPackPostUInt8, ulTtiReq->pdus[pduIdx].pdu.pusch_pdu.dmrsConfigType, mBuf);
             CMCHKPK(oduPackPostUInt16, ulTtiReq->pdus[pduIdx].pdu.pusch_pdu.ulDmrsSymbPos, mBuf);
             CMCHKPK(oduPackPostUInt16, ulTtiReq->pdus[pduIdx].pdu.pusch_pdu.ulDmrsScramblingId, mBuf);
             CMCHKPK(oduPackPostUInt8, ulTtiReq->pdus[pduIdx].pdu.pusch_pdu.scid, mBuf);
             CMCHKPK(oduPackPostUInt8, ulTtiReq->pdus[pduIdx].pdu.pusch_pdu.numDmrsCdmGrpsNoData, mBuf);
             CMCHKPK(oduPackPostUInt16, ulTtiReq->pdus[pduIdx].pdu.pusch_pdu.dmrsPorts, mBuf);
             CMCHKPK(oduPackPostUInt16, ulTtiReq->pdus[pduIdx].pdu.pusch_pdu.nTpPuschId, mBuf);
             CMCHKPK(oduPackPostUInt16, ulTtiReq->pdus[pduIdx].pdu.pusch_pdu.tpPi2Bpsk, mBuf);
             for(uint8_t rbBitMapIdx = 0; rbBitMapIdx < 36; rbBitMapIdx++)
             {
                CMCHKPK(oduPackPostUInt8, ulTtiReq->pdus[pduIdx].pdu.pusch_pdu.rbBitmap[rbBitMapIdx], mBuf);
             }
             CMCHKPK(oduPackPostUInt16, ulTtiReq->pdus[pduIdx].pdu.pusch_pdu.rbStart, mBuf);
             CMCHKPK(oduPackPostUInt16, ulTtiReq->pdus[pduIdx].pdu.pusch_pdu.rbSize, mBuf);
             CMCHKPK(oduPackPostUInt8, ulTtiReq->pdus[pduIdx].pdu.pusch_pdu.vrbToPrbMapping, mBuf);
             CMCHKPK(oduPackPostUInt8, ulTtiReq->pdus[pduIdx].pdu.pusch_pdu.frequencyHopping, mBuf);
             CMCHKPK(oduPackPostUInt16, ulTtiReq->pdus[pduIdx].pdu.pusch_pdu.txDirectCurrentLocation, mBuf);
             CMCHKPK(oduPackPostUInt8, ulTtiReq->pdus[pduIdx].pdu.pusch_pdu.resourceAlloc, mBuf);
             CMCHKPK(oduPackPostUInt8, ulTtiReq->pdus[pduIdx].pdu.pusch_pdu.uplinkFrequencyShift7p5khz, mBuf);
             CMCHKPK(oduPackPostUInt8, ulTtiReq->pdus[pduIdx].pdu.pusch_pdu.startSymbIndex, mBuf);
             CMCHKPK(oduPackPostUInt8, ulTtiReq->pdus[pduIdx].pdu.pusch_pdu.nrOfSymbols, mBuf);
             CMCHKPK(oduPackPostUInt8, ulTtiReq->pdus[pduIdx].pdu.pusch_pdu.mappingType, mBuf);
             CMCHKPK(oduPackPostUInt8, ulTtiReq->pdus[pduIdx].pdu.pusch_pdu.nrOfDmrsSymbols, mBuf);
             CMCHKPK(oduPackPostUInt8, ulTtiReq->pdus[pduIdx].pdu.pusch_pdu.dmrsAddPos, mBuf);
             CMCHKPK(oduPackPostUInt8, ulTtiReq->pdus[pduIdx].pdu.pusch_pdu.pad, mBuf);

             //Fill fapi_pusch_data_t
             CMCHKPK(oduPackPostUInt8, ulTtiReq->pdus[pduIdx].pdu.pusch_pdu.puschData.rvIndex, mBuf);
             CMCHKPK(oduPackPostUInt8, ulTtiReq->pdus[pduIdx].pdu.pusch_pdu.puschData.harqProcessId, mBuf);
             CMCHKPK(oduPackPostUInt8, ulTtiReq->pdus[pduIdx].pdu.pusch_pdu.puschData.newDataIndicator, mBuf);
             CMCHKPK(oduPackPostUInt8, ulTtiReq->pdus[pduIdx].pdu.pusch_pdu.puschData.pad, mBuf);
             CMCHKPK(oduPackPostUInt32, ulTtiReq->pdus[pduIdx].pdu.pusch_pdu.puschData.tbSize, mBuf);
             CMCHKPK(oduPackPostUInt16, ulTtiReq->pdus[pduIdx].pdu.pusch_pdu.puschData.numCb, mBuf);
             CMCHKPK(oduPackPostUInt8, ulTtiReq->pdus[pduIdx].pdu.pusch_pdu.puschData.cbPresentAndPosition[0], mBuf);
             CMCHKPK(oduPackPostUInt8, ulTtiReq->pdus[pduIdx].pdu.pusch_pdu.puschData.cbPresentAndPosition[1], mBuf);

             //Fill fapi_pusch_uci_t
             CMCHKPK(oduPackPostUInt16, ulTtiReq->pdus[pduIdx].pdu.pusch_pdu.puschUci.harqAckBitLength, mBuf);  
             CMCHKPK(oduPackPostUInt16, ulTtiReq->pdus[pduIdx].pdu.pusch_pdu.puschUci.csiPart1BitLength, mBuf);  
             CMCHKPK(oduPackPostUInt16, ulTtiReq->pdus[pduIdx].pdu.pusch_pdu.puschUci.csiPart2BitLength, mBuf);  
             CMCHKPK(oduPackPostUInt8, ulTtiReq->pdus[pduIdx].pdu.pusch_pdu.puschUci.alphaScaling, mBuf);  
             CMCHKPK(oduPackPostUInt8, ulTtiReq->pdus[pduIdx].pdu.pusch_pdu.puschUci.betaOffsetHarqAck, mBuf);  
             CMCHKPK(oduPackPostUInt8, ulTtiReq->pdus[pduIdx].pdu.pusch_pdu.puschUci.betaOffsetCsi1, mBuf);  
             CMCHKPK(oduPackPostUInt8, ulTtiReq->pdus[pduIdx].pdu.pusch_pdu.puschUci.betaOffsetCsi2, mBuf);  
             CMCHKPK(oduPackPostUInt8, ulTtiReq->pdus[pduIdx].pdu.pusch_pdu.puschUci.pad[0], mBuf);  
             CMCHKPK(oduPackPostUInt8, ulTtiReq->pdus[pduIdx].pdu.pusch_pdu.puschUci.pad[1], mBuf);

             //Fill fapi_pusch_ptrs_t
             CMCHKPK(oduPackPostUInt8, ulTtiReq->pdus[pduIdx].pdu.pusch_pdu.puschPtrs.numPtrsPorts, mBuf);
             CMCHKPK(oduPackPostUInt8, ulTtiReq->pdus[pduIdx].pdu.pusch_pdu.puschPtrs.ptrsTimeDensity, mBuf);
             CMCHKPK(oduPackPostUInt8, ulTtiReq->pdus[pduIdx].pdu.pusch_pdu.puschPtrs.ptrsFreqDensity, mBuf);
             CMCHKPK(oduPackPostUInt8, ulTtiReq->pdus[pduIdx].pdu.pusch_pdu.puschPtrs.ulPtrsPower, mBuf);
             for(uint8_t portIdx = 0; portIdx < ulTtiReq->pdus[pduIdx].pdu.pusch_pdu.puschPtrs.numPtrsPorts; portIdx++)
             {
                CMCHKPK(oduPackPostUInt16, ulTtiReq->pdus[pduIdx].pdu.pusch_pdu.puschPtrs.ptrsInfo[portIdx].ptrsPortIndex, mBuf);
                CMCHKPK(oduPackPostUInt8, ulTtiReq->pdus[pduIdx].pdu.pusch_pdu.puschPtrs.ptrsInfo[portIdx].ptrsDmrsPort, mBuf);
                CMCHKPK(oduPackPostUInt8, ulTtiReq->pdus[pduIdx].pdu.pusch_pdu.puschPtrs.ptrsInfo[portIdx].ptrsReOffset, mBuf);
             }

             //Fill fapi_dfts_ofdm_t
             CMCHKPK(oduPackPostUInt16, ulTtiReq->pdus[pduIdx].pdu.pusch_pdu.dftsOfdm.lowPaprSequenceNumber, mBuf); 
             CMCHKPK(oduPackPostUInt8, ulTtiReq->pdus[pduIdx].pdu.pusch_pdu.dftsOfdm.lowPaprGroupNumber, mBuf);
             CMCHKPK(oduPackPostUInt8, ulTtiReq->pdus[pduIdx].pdu.pusch_pdu.dftsOfdm.ulPtrsSampleDensity, mBuf);
             CMCHKPK(oduPackPostUInt8, ulTtiReq->pdus[pduIdx].pdu.pusch_pdu.dftsOfdm.ulPtrsTimeDensityTransformPrecoding, mBuf);
             CMCHKPK(oduPackPostUInt8, ulTtiReq->pdus[pduIdx].pdu.pusch_pdu.dftsOfdm.pad, mBuf);

             //Fill fapi_ul_rx_bmform_pdu_t
             nfapiFillUlRxBeamforming(&ulTtiReq->pdus[pduIdx].pdu.pusch_pdu.beamforming, mBuf);
             break;
          }
          case PUCCH_PDU_TYPE:
          {
             CMCHKPK(oduPackPostUInt16, ulTtiReq->pdus[pduIdx].pdu.pucch_pdu.rnti, mBuf);
             CMCHKPK(oduPackPostUInt8, ulTtiReq->pdus[pduIdx].pdu.pucch_pdu.pad1[0], mBuf);
             CMCHKPK(oduPackPostUInt8, ulTtiReq->pdus[pduIdx].pdu.pucch_pdu.pad1[1], mBuf);
             CMCHKPK(oduPackPostUInt32, ulTtiReq->pdus[pduIdx].pdu.pucch_pdu.handle, mBuf);
             CMCHKPK(oduPackPostUInt16, ulTtiReq->pdus[pduIdx].pdu.pucch_pdu.bwpSize, mBuf);
             CMCHKPK(oduPackPostUInt16, ulTtiReq->pdus[pduIdx].pdu.pucch_pdu.bwpStart, mBuf);
             CMCHKPK(oduPackPostUInt8, ulTtiReq->pdus[pduIdx].pdu.pucch_pdu.subCarrierSpacing, mBuf);
             CMCHKPK(oduPackPostUInt8, ulTtiReq->pdus[pduIdx].pdu.pucch_pdu.cyclicPrefix, mBuf);
             CMCHKPK(oduPackPostUInt8, ulTtiReq->pdus[pduIdx].pdu.pucch_pdu.formatType, mBuf);
             CMCHKPK(oduPackPostUInt8, ulTtiReq->pdus[pduIdx].pdu.pucch_pdu.multiSlotTxIndicator, mBuf);
             CMCHKPK(oduPackPostUInt8, ulTtiReq->pdus[pduIdx].pdu.pucch_pdu.pi2Bpsk, mBuf);
             CMCHKPK(oduPackPostUInt8, ulTtiReq->pdus[pduIdx].pdu.pucch_pdu.pad2, mBuf);
             CMCHKPK(oduPackPostUInt16, ulTtiReq->pdus[pduIdx].pdu.pucch_pdu.prbStart, mBuf);
             CMCHKPK(oduPackPostUInt16, ulTtiReq->pdus[pduIdx].pdu.pucch_pdu.prbSize, mBuf);
             CMCHKPK(oduPackPostUInt8, ulTtiReq->pdus[pduIdx].pdu.pucch_pdu.startSymbolIndex, mBuf);
             CMCHKPK(oduPackPostUInt8, ulTtiReq->pdus[pduIdx].pdu.pucch_pdu.nrOfSymbols, mBuf);
             CMCHKPK(oduPackPostUInt8, ulTtiReq->pdus[pduIdx].pdu.pucch_pdu.freqHopFlag, mBuf);
             CMCHKPK(oduPackPostUInt8, ulTtiReq->pdus[pduIdx].pdu.pucch_pdu.groupHopFlag, mBuf);
             CMCHKPK(oduPackPostUInt8, ulTtiReq->pdus[pduIdx].pdu.pucch_pdu.sequenceHopFlag, mBuf);
             CMCHKPK(oduPackPostUInt8, ulTtiReq->pdus[pduIdx].pdu.pucch_pdu.pad3, mBuf);
             CMCHKPK(oduPackPostUInt16, ulTtiReq->pdus[pduIdx].pdu.pucch_pdu.secondHopPrb, mBuf);
             CMCHKPK(oduPackPostUInt16, ulTtiReq->pdus[pduIdx].pdu.pucch_pdu.hoppingId, mBuf);
             CMCHKPK(oduPackPostUInt16, ulTtiReq->pdus[pduIdx].pdu.pucch_pdu.initialCyclicShift, mBuf);
             CMCHKPK(oduPackPostUInt16, ulTtiReq->pdus[pduIdx].pdu.pucch_pdu.dataScramblingId, mBuf);
             CMCHKPK(oduPackPostUInt8, ulTtiReq->pdus[pduIdx].pdu.pucch_pdu.timeDomainOccIdx, mBuf);
             CMCHKPK(oduPackPostUInt8, ulTtiReq->pdus[pduIdx].pdu.pucch_pdu.preDftOccIdx, mBuf);
             CMCHKPK(oduPackPostUInt8, ulTtiReq->pdus[pduIdx].pdu.pucch_pdu.preDftOccLen, mBuf);
             CMCHKPK(oduPackPostUInt8, ulTtiReq->pdus[pduIdx].pdu.pucch_pdu.addDmrsFlag, mBuf);
             CMCHKPK(oduPackPostUInt16, ulTtiReq->pdus[pduIdx].pdu.pucch_pdu.dmrsScramblingId, mBuf);
             CMCHKPK(oduPackPostUInt8, ulTtiReq->pdus[pduIdx].pdu.pucch_pdu.dmrsCyclicShift, mBuf);
             CMCHKPK(oduPackPostUInt8, ulTtiReq->pdus[pduIdx].pdu.pucch_pdu.srFlag, mBuf);
             CMCHKPK(oduPackPostUInt16, ulTtiReq->pdus[pduIdx].pdu.pucch_pdu.bitLenHarq, mBuf);
             CMCHKPK(oduPackPostUInt8, ulTtiReq->pdus[pduIdx].pdu.pucch_pdu.pad4[0], mBuf);
             CMCHKPK(oduPackPostUInt8, ulTtiReq->pdus[pduIdx].pdu.pucch_pdu.pad4[1], mBuf);
             CMCHKPK(oduPackPostUInt16, ulTtiReq->pdus[pduIdx].pdu.pucch_pdu.bitLenCsiPart1, mBuf);
             CMCHKPK(oduPackPostUInt16, ulTtiReq->pdus[pduIdx].pdu.pucch_pdu.bitLenCsiPart2, mBuf);
             
             //Fill fapi_ul_rx_bmform_pdu_t
             nfapiFillUlRxBeamforming(&ulTtiReq->pdus[pduIdx].pdu.pucch_pdu.beamforming, mBuf);
             
             break;
          }
          default:
          {
             DU_LOG("ERROR  --> NFAPI_VNF: Incorrect PDUTYPE:%d",ulTtiReq->pdus[pduIdx].pduType);
             ODU_PUT_MSG_BUF(mBuf);
             return RFAILED;
          }
       }
   }
   for(ueGrpIdx = 0; ueGrpIdx < ulTtiReq->nGroup; ueGrpIdx++)
   {
      CMCHKPK(oduPackPostUInt8, ulTtiReq->ueGrpInfo[ueGrpIdx].nUe, mBuf);
      for(uint8_t padIdx = 0; padIdx < 3; padIdx++)
      {
         CMCHKPK(oduPackPostUInt8, ulTtiReq->ueGrpInfo[ueGrpIdx].pad[padIdx], mBuf);
      }
      for(uint8_t ueIdx = 0; ueIdx < ulTtiReq->ueGrpInfo[ueGrpIdx].nUe; ueIdx++)
      {
         CMCHKPK(oduPackPostUInt8, ulTtiReq->ueGrpInfo[ueGrpIdx].pduIdx[ueIdx], mBuf); 
      }
   }
   
   ret = nfapiP7UdpSendMsg(mBuf);

   return ret;
}

/***********************************************************************
 *
 * @brief Fill P7 UL DCI REQ and send via UDP
 *
 * @details
 *
 *    Function : nfapiBuildAndSendUlDciReq
 *
 *    Functionality:
 *       Fill P7 UL DCI REQ by packing each element of FAPI_UL_DCI_REQ 
 *
 * @params[in] FapiMsgBody, MsgLength
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ********************************************************************/

uint8_t nfapiBuildAndSendUlDciReq(void *fapiMsg, uint32_t msgLen)
{
   Buffer  *mBuf = NULLP;
   uint8_t pduIdx = 0;
   uint8_t ret = ROK, freqIdx = 0;
   uint16_t numBytes = 0;

   p_fapi_api_queue_elem_t ulDciElem = (p_fapi_api_queue_elem_t)fapiMsg;
   fapi_ul_dci_req_t *ulDciReq = (fapi_ul_dci_req_t *)(ulDciElem +1);
   
   if(ODU_GET_MSG_BUF(MAC_MEM_REGION, MAC_POOL, &mBuf) != ROK)
   {
      DU_LOG("\nERROR  --> NFAPI_VNF : Memory allocation failed in nfapiBuildAndSendUlDciReq");
      return RFAILED;
   }
  
   nfapiFillP7Hdr(mBuf,(sizeof(fapi_ul_dci_req_msg_body) + sizeof(nFapi_msg_header)), 0, 0);
   nfapiFillMsgHdr(mBuf, vnfDb.vnfP7Info.p7SyncInfo.phyId, FAPI_UL_DCI_REQUEST, sizeof(fapi_ul_dci_req_msg_body));
  
   CMCHKPK(oduPackPostUInt16, ulDciReq->sfn, mBuf);
   CMCHKPK(oduPackPostUInt16, ulDciReq->slot, mBuf);
   CMCHKPK(oduPackPostUInt8, ulDciReq->numPdus, mBuf);

   for(pduIdx = 0; pduIdx < ulDciReq->numPdus; pduIdx++) 
   {
       CMCHKPK(oduPackPostUInt16, ulDciReq->pdus[pduIdx].pduType, mBuf);
       CMCHKPK(oduPackPostUInt16, ulDciReq->pdus[pduIdx].pduSize, mBuf);

       //Fill fapi_dl_pdcch_pdu_t
       CMCHKPK(oduPackPostUInt16, ulDciReq->pdus[pduIdx].pdcchPduConfig.bwpSize, mBuf);
       CMCHKPK(oduPackPostUInt16, ulDciReq->pdus[pduIdx].pdcchPduConfig.bwpStart, mBuf);
       CMCHKPK(oduPackPostUInt8, ulDciReq->pdus[pduIdx].pdcchPduConfig.subCarrierSpacing, mBuf);
       CMCHKPK(oduPackPostUInt8, ulDciReq->pdus[pduIdx].pdcchPduConfig.cyclicPrefix, mBuf);
       CMCHKPK(oduPackPostUInt8, ulDciReq->pdus[pduIdx].pdcchPduConfig.startSymbolIndex, mBuf);
       CMCHKPK(oduPackPostUInt8, ulDciReq->pdus[pduIdx].pdcchPduConfig.durationSymbols, mBuf);
       for(freqIdx = 0; freqIdx < 6; freqIdx++)
       {
          CMCHKPK(oduPackPostUInt8, ulDciReq->pdus[pduIdx].pdcchPduConfig.freqDomainResource[freqIdx], mBuf);
       }
       CMCHKPK(oduPackPostUInt8, ulDciReq->pdus[pduIdx].pdcchPduConfig.cceRegMappingType, mBuf);
       CMCHKPK(oduPackPostUInt8, ulDciReq->pdus[pduIdx].pdcchPduConfig.regBundleSize, mBuf);
       CMCHKPK(oduPackPostUInt8, ulDciReq->pdus[pduIdx].pdcchPduConfig.interleaverSize, mBuf);
       CMCHKPK(oduPackPostUInt8, ulDciReq->pdus[pduIdx].pdcchPduConfig.coreSetType, mBuf);
       CMCHKPK(oduPackPostUInt16, ulDciReq->pdus[pduIdx].pdcchPduConfig.shiftIndex, mBuf);
       CMCHKPK(oduPackPostUInt8, ulDciReq->pdus[pduIdx].pdcchPduConfig.precoderGranularity, mBuf);
       CMCHKPK(oduPackPostUInt8, ulDciReq->pdus[pduIdx].pdcchPduConfig.pad, mBuf);
       CMCHKPK(oduPackPostUInt16, ulDciReq->pdus[pduIdx].pdcchPduConfig.numDlDci, mBuf);
       for(uint16_t dciIndex = 0; dciIndex <  ulDciReq->pdus[pduIdx].pdcchPduConfig.numDlDci; dciIndex++)
       {
          CMCHKPK(oduPackPostUInt16, ulDciReq->pdus[pduIdx].pdcchPduConfig.dlDci[dciIndex].rnti, mBuf); 
          CMCHKPK(oduPackPostUInt16, ulDciReq->pdus[pduIdx].pdcchPduConfig.dlDci[dciIndex].scramblingId, mBuf); 
          CMCHKPK(oduPackPostUInt16, ulDciReq->pdus[pduIdx].pdcchPduConfig.dlDci[dciIndex].scramblingRnti, mBuf); 
          CMCHKPK(oduPackPostUInt8, ulDciReq->pdus[pduIdx].pdcchPduConfig.dlDci[dciIndex].cceIndex, mBuf); 
          CMCHKPK(oduPackPostUInt8, ulDciReq->pdus[pduIdx].pdcchPduConfig.dlDci[dciIndex].aggregationLevel, mBuf); 
          nfapiFillDlPrecodingBeamform(&ulDciReq->pdus[pduIdx].pdcchPduConfig.dlDci[dciIndex].pc_and_bform, mBuf);
          CMCHKPK(oduPackPostUInt8, ulDciReq->pdus[pduIdx].pdcchPduConfig.dlDci[dciIndex].beta_pdcch_1_0, mBuf); 
          CMCHKPK(oduPackPostUInt8, ulDciReq->pdus[pduIdx].pdcchPduConfig.dlDci[dciIndex].powerControlOffsetSS, mBuf); 
          CMCHKPK(oduPackPostUInt16, ulDciReq->pdus[pduIdx].pdcchPduConfig.dlDci[dciIndex].payloadSizeBits, mBuf);
          numBytes = ulDciReq->pdus[pduIdx].pdcchPduConfig.dlDci[dciIndex].payloadSizeBits / 8;
          if(ulDciReq->pdus[pduIdx].pdcchPduConfig.dlDci[dciIndex].payloadSizeBits % 8)
              numBytes += 1;

          for(uint16_t payloadIdx = 0; payloadIdx < numBytes; payloadIdx++)
          {
             CMCHKPK(oduPackPostUInt8, ulDciReq->pdus[pduIdx].pdcchPduConfig.dlDci[dciIndex].payload[payloadIdx], mBuf);
          }
       }
   }
   ret = nfapiP7UdpSendMsg(mBuf);

   return ret;
}

/***********************************************************************
 *
 * @brief Fill P7 TX DATA REQ and send via UDP
 *
 * @details
 *
 *    Function : nfapiBuildAndSendTxDataReq
 *
 *    Functionality:
 *       Fill P7 TX DATA REQ by packing each element of FAPI_TX_DATA_REQ 
 *
 * @params[in] FapiMsgBody, MsgLength
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ********************************************************************/

uint8_t nfapiBuildAndSendTxDataReq(void *fapiMsg, uint32_t msgLen)
{
   Buffer  *mBuf = NULLP;
   uint8_t pduIdx = 0;
   uint8_t ret = ROK;
   uint16_t payloadSize = 0;

   p_fapi_api_queue_elem_t txDataReqElem = (p_fapi_api_queue_elem_t)fapiMsg;
   fapi_tx_data_req_t *txDataReq = (fapi_tx_data_req_t *)(txDataReqElem +1);
   
   if(ODU_GET_MSG_BUF(MAC_MEM_REGION, MAC_POOL, &mBuf) != ROK)
   {
      DU_LOG("\nERROR  --> NFAPI_VNF : Memory allocation failed in nfapiBuildAndSendTxDataReq");
      return RFAILED;
   }
  
   nfapiFillP7Hdr(mBuf,(sizeof(fapi_tx_data_req_msg_body) + sizeof(nFapi_msg_header)), 0, 0);
   nfapiFillMsgHdr(mBuf, vnfDb.vnfP7Info.p7SyncInfo.phyId, FAPI_TX_DATA_REQUEST, sizeof(fapi_tx_data_req_msg_body));
  
   CMCHKPK(oduPackPostUInt16, txDataReq->sfn, mBuf);
   CMCHKPK(oduPackPostUInt16, txDataReq->slot, mBuf);
   CMCHKPK(oduPackPostUInt16, txDataReq->num_pdus, mBuf);
   for(pduIdx = 0; pduIdx < txDataReq->num_pdus; pduIdx++)
   {
      CMCHKPK(oduPackPostUInt32, txDataReq->pdu_desc[pduIdx].pdu_length, mBuf);
      CMCHKPK(oduPackPostUInt16, txDataReq->pdu_desc[pduIdx].pdu_index, mBuf);
      CMCHKPK(oduPackPostUInt16, txDataReq->pdu_desc[pduIdx].num_tlvs, mBuf);
      for(uint8_t tlvIdx = 0; tlvIdx < txDataReq->pdu_desc[pduIdx].num_tlvs; tlvIdx++)
      {
          CMCHKPK(oduPackPostUInt16, txDataReq->pdu_desc[pduIdx].tlvs[tlvIdx].tl.tag, mBuf);
          CMCHKPK(oduPackPostUInt16, txDataReq->pdu_desc[pduIdx].tlvs[tlvIdx].tl.length, mBuf);
          payloadSize = txDataReq->pdu_desc[pduIdx].tlvs[tlvIdx].tl.length;
          for(uint16_t payloadByte = 0; payloadByte < payloadSize; payloadByte++)
          {
             CMCHKPK(oduPackPostUInt8, txDataReq->pdu_desc[pduIdx].tlvs[tlvIdx].value[payloadByte], mBuf);
          }
      }
      
   }
   ret = nfapiP7UdpSendMsg(mBuf);

   return ret;
}

/***********************************************************************
 *
 * @brief Fill P7 Transparent Msgs and Build NFAPI msg and send via UDP
 *
 * @details
 *
 *    Function : nfapiFillAndSendP7TransMsg
 *
 *    Functionality:
 *       Fill P7 Transparent Msgs and Build NFAPI msg and send via UDP
 *
 * @params[in]  : MsgType(P7 Transparent Msg), MsgLen and FAPI_MSG_BODY
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ********************************************************************/

uint8_t nfapiFillAndSendP7TransMsg(uint8_t msgType, uint32_t msgLen, void *fapiP7Msg)
{
   uint8_t ret = ROK;

   switch(msgType)
   {
      case FAPI_DL_TTI_REQUEST:
      {
         DU_LOG("\nINFO   --> NFAPI_VNF:DL_TTI_REQ received from LWR_MAC as FAPI_MSG_BODY");
         ret = nfapiBuildAndSendDlTtiReq(fapiP7Msg, msgLen);
         break;
      }
      case FAPI_UL_TTI_REQUEST:
      {
         DU_LOG("\nINFO   --> NFAPI_VNF:UL_TTI_REQ received from LWR_MAC as FAPI_MSG_BODY");
         ret = nfapiBuildAndSendUlTtiReq(fapiP7Msg, msgLen);
         break;
      }
      case FAPI_UL_DCI_REQUEST:
      {
         DU_LOG("\nINFO   --> NFAPI_VNF:UL_DCI_REQ received from LWR_MAC as FAPI_MSG_BODY");
         ret = nfapiBuildAndSendUlDciReq(fapiP7Msg, msgLen);
         break;
      }
      case FAPI_TX_DATA_REQUEST:
      {
         DU_LOG("\nINFO   --> NFAPI_VNF:TX_DATA_REQ received from LWR_MAC as FAPI_MSG_BODY");
         ret = nfapiBuildAndSendTxDataReq(fapiP7Msg, msgLen);
         break;
      }
      default:
      {
         DU_LOG("\nERROR  --> NFAPI_VNF: Incorrect MsgType:%d",msgType);
         ret = RFAILED;
         break;
      }
   }
   LWR_MAC_FREE(fapiP7Msg, msgLen);   
   return ret;
}

/*******************************************************************
 *
 * @brief Build and Sends DL_NODE_SYNC
 *
 * @details
 *
 *    Function : nfapiBuildAndSendDlNodeSync
 *
 *    Functionality:
 *           Build and Sends DL Node Synch
 *        (Params can be referred to Table 4-1 Dl Node Sync Parameters)
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t nfapiBuildAndSendDlNodeSync()
{
   uint8_t  scs = 0;
   Buffer   *mBuf = NULLP;
   
   if(ODU_GET_MSG_BUF(MAC_MEM_REGION, MAC_POOL, &mBuf) != ROK)
   {
      DU_LOG("ERROR  --> NFAPI_VNF : Memory allocation failed in packPnfParamReq");
      return RFAILED;
   }
  
   
   vnfDb.vnfP7Info.p7SyncInfo.prev_t1 = CALC_TIME_USEC_FROM_SFNSLOT(vnfDb.vnfP7Info.p7SyncInfo.frameInfo);
   scs = 15 * (pow(2, vnfDb.numerology));
  
   nfapiFillP7Hdr(mBuf,( sizeof(nFapi_dl_node_sync_info) + sizeof(nFapi_msg_header)), 0, 0);
   nfapiFillMsgHdr(mBuf, vnfDb.vnfP7Info.p7SyncInfo.phyId, TAG_NFAPI_DL_NODE_SYNC, sizeof(nFapi_dl_node_sync_info));
   CMCHKPK(oduPackPostUInt32, vnfDb.vnfP7Info.p7SyncInfo.prev_t1, mBuf);
   CMCHKPK(SPkPostS32, vnfDb.vnfP7Info.p7SyncInfo.delta_sfn_slot, mBuf);
   CMCHKPK(oduPackPostUInt8, scs, mBuf);

   memset(&vnfDb.vnfP7Info.p7SyncInfo.delta_sfn_slot, 0, sizeof(int32_t)); 
   return(nfapiP7UdpSendMsg(mBuf));
}

/*******************************************************************
 *
 * @brief Validating T1 received from PNF@ UL_NODE_SYNC
 *
 * @details
 *
 *    Function : nfapiValidatingT1
 *
 *    Functionality:
 *           Matching the T1 received from UL Node Sync to validate that
 *           correct UL Node Sync or not
 *
 * @params[in] T1 received from UL_NODE_SYNC
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t nfapiValidatingT1(uint32_t ulNodeSyncT1)
{
   DU_LOG("DEBUG  --> NFAPI_VNF: t1@VNF:%u, t1_from_pnf:%u",\
            vnfDb.vnfP7Info.p7SyncInfo.prev_t1, ulNodeSyncT1);
   
   if(vnfDb.vnfP7Info.p7SyncInfo.prev_t1 == ulNodeSyncT1)
   {
      DU_LOG("INFO   --> NFAPI_VNF: T1 matching so this UL Node Sync can be processed");
      return ROK;
   }
   else
   {
      DU_LOG("ERROR  --> NFAPI_VNF: Mismatch T1");
      return RFAILED;
   }
}


/*******************************************************************
 *
 * @brief Processes and handles UL_NODE_SYNC
 *
 * @details
 *
 *    Function : nfapiP7ProcUlNodeSync
 *
 *    Functionality:
 *           Processes UL Node Sync and determine if VNF and PNF are in Sync
 *           If in un-sync, calculate the slot difference.
 *
 * @params[in] Buffer *mBuf 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
/*Following parameters can be referred to Table 4-2 Ul Node Sync Parameters*/
uint8_t nfapiP7ProcUlNodeSync(Buffer *mBuf)
{
   int8_t                  cmpStatus = 0;
   uint8_t                 ret = ROK;
   uint16_t                numSlotDiff = 0;
   NfapiFrameInfo          t3_sfnSlot;
   nFapi_ul_node_sync_info ulNodeSyncInfo; 

   CMCHKPK(oduUnpackUInt32, &(ulNodeSyncInfo.t1), mBuf);
   CMCHKPK(oduUnpackUInt32, &(ulNodeSyncInfo.t2), mBuf);
   CMCHKPK(oduUnpackUInt32, &(ulNodeSyncInfo.t3), mBuf);

   if(nfapiValidatingT1(ulNodeSyncInfo.t1) == ROK)
   {
      memset(&vnfDb.vnfP7Info.p7SyncInfo.delta_sfn_slot, 0, sizeof(int32_t)); 
      vnfDb.vnfP7Info.p7SyncInfo.prev_t2 = ulNodeSyncInfo.t2;
      vnfDb.vnfP7Info.p7SyncInfo.prev_t3 = ulNodeSyncInfo.t3;
   
      EXTRACT_SFN_SLOT_FROM_TIME(vnfDb.vnfP7Info.p7SyncInfo.prev_t3, t3_sfnSlot);
      CMP_INFO(t3_sfnSlot, vnfDb.vnfP7Info.p7SyncInfo.frameInfo, cmpStatus);
      if(cmpStatus == 0)
      {
         if(!vnfDb.vnfP7Info.p7SyncInfo.inSync)
         {
            if(nfapiSendSlotIndToMac() != ROK)
            {
               DU_LOG("\nERROR  -> NFAPI_VNF: Memory Corruption issue while sending SLOT IND to MAC");
               return RFAILED;
            }
         }
         vnfDb.vnfP7Info.p7SyncInfo.inSync = TRUE;
      }
      else
      {
         vnfDb.vnfP7Info.p7SyncInfo.inSync = FALSE;
         if(cmpStatus == -1)
         { /*PNF is ahead.*/
            DU_LOG("INFO   --> NFAPI_VNF: PNF is ahead.");
            CALC_TIME_DIFF(t3_sfnSlot, vnfDb.vnfP7Info.p7SyncInfo.frameInfo, numSlotDiff);
         }
         else if(cmpStatus == 1)
         {
            DU_LOG("INFO   --> NFAPI_VNF: VNF is ahead.");
            CALC_TIME_DIFF(vnfDb.vnfP7Info.p7SyncInfo.frameInfo, t3_sfnSlot, numSlotDiff);
         }
         vnfDb.vnfP7Info.p7SyncInfo.delta_sfn_slot = cmpStatus * numSlotDiff;
         ret = nfapiBuildAndSendDlNodeSync();
      }
      DU_LOG("DEBUG  --> NFAPI_VNF: delta:%d insyn:%d", vnfDb.vnfP7Info.p7SyncInfo.delta_sfn_slot, vnfDb.vnfP7Info.p7SyncInfo.inSync);
      return ret;
   }
   else
   {
      DU_LOG("ERROR  --> NFAPI_VNF: T1 Validation failed");
      return RFAILED;
   }
   return ret;
}

/*******************************************************************
 *
 * @brief Build and Sends EVENT_RACH_IND to MAC
 *
 * @details
 *
 *    Function  nfapiSendRachIndToMac
 *
 *    Functionality:
 *           Build and Posts RACH_IND event to MAC
 *
 * @params[in] fapi_rach_ind_msg_body(received from PNF) 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t nfapiSendRachIndToMac(fapi_rach_ind_msg_body *nfapiRachInd)
{
   Pst          pst;
   uint8_t      pduIdx = 0, ret = ROK;
   uint8_t      prmbleIdx;
   RachPduInfo  *rachPdu = NULLP;
   RachInd      *rachInd = NULLP;

   MAC_ALLOC_SHRABL_BUF(rachInd, sizeof(RachInd));
   if(!rachInd)
   {
      DU_LOG("ERROR  -->  NFAPI_VNF : Memory Allocation failed in nfapiSendRachIndToMac");
      return RFAILED;
   }
   rachInd->cellId = vnfDb.cellId;
   rachInd->timingInfo.sfn = nfapiRachInd->sfn;
   rachInd->timingInfo.slot = nfapiRachInd->slot;
   rachInd->numPdu = nfapiRachInd->numPdus;
   for(pduIdx=0; pduIdx < rachInd->numPdu; pduIdx++)
   {
      rachPdu = &rachInd->rachPdu[pduIdx];
      rachPdu->pci = nfapiRachInd->rachPdu[pduIdx].phyCellId;
      rachPdu->symbolIdx = nfapiRachInd->rachPdu[pduIdx].symbolIndex;
      rachPdu->slotIdx = nfapiRachInd->rachPdu[pduIdx].slotIndex;
      rachPdu->freqIdx = nfapiRachInd->rachPdu[pduIdx].freqIndex;
      rachPdu->numPream = nfapiRachInd->rachPdu[pduIdx].numPreamble; 
      for(prmbleIdx=0; prmbleIdx < rachPdu->numPream; prmbleIdx++)
      {
	      rachPdu->preamInfo[prmbleIdx].preamIdx = \
	       nfapiRachInd->rachPdu[pduIdx].preambleInfo[prmbleIdx].preambleIndex;
	      rachPdu->preamInfo[prmbleIdx].timingAdv = \
	       nfapiRachInd->rachPdu[pduIdx].preambleInfo[prmbleIdx].timingAdvance;
      }
   }
   /* Fill post and sent to MAC */
   FILL_PST_LWR_MAC_TO_MAC(pst, EVENT_RACH_IND_TO_MAC);
   pst.selector = ODU_SELECTOR_LWLC;
   ret = packRachInd(&pst, rachInd);
   return ret;
}

/*******************************************************************
 *
 * @brief Process FAPI_RACH_IND from PNF
 *
 * @details
 *
 *    Function : nfapiP7ProcRachInd
 *
 *    Functionality:
 *           Processes FAPI_RACH_IND received from PNF
 *           Parametes can be referred from SCF222v2222.10.03, Sec 3.4.11
 *
 * @params[in] UDP Buffer 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t nfapiP7ProcRachInd(Buffer *mBuf)
{
    uint8_t pduCnt = 0, preambleCnt = 0, ret = ROK;
    fapi_rach_ind_msg_body  nfapiRachInd;

    CMCHKPK(oduUnpackUInt16, &(nfapiRachInd.sfn), mBuf); 
    CMCHKPK(oduUnpackUInt16, &(nfapiRachInd.slot), mBuf); 
    CMCHKPK(oduUnpackUInt8, &(nfapiRachInd.numPdus), mBuf);

    //Extract fapi_rach_pdu_t
    for(pduCnt = 0; pduCnt < nfapiRachInd.numPdus; pduCnt++)
    {
       CMCHKPK(oduUnpackUInt16, &(nfapiRachInd.rachPdu[pduCnt].phyCellId), mBuf);
       CMCHKPK(oduUnpackUInt8, &(nfapiRachInd.rachPdu[pduCnt].symbolIndex), mBuf);
       CMCHKPK(oduUnpackUInt8, &(nfapiRachInd.rachPdu[pduCnt].slotIndex), mBuf);
       CMCHKPK(oduUnpackUInt8, &(nfapiRachInd.rachPdu[pduCnt].freqIndex), mBuf);
       CMCHKPK(oduUnpackUInt8, &(nfapiRachInd.rachPdu[pduCnt].avgRssi), mBuf);
       CMCHKPK(oduUnpackUInt8, &(nfapiRachInd.rachPdu[pduCnt].avgSnr), mBuf);
       CMCHKPK(oduUnpackUInt8, &(nfapiRachInd.rachPdu[pduCnt].numPreamble), mBuf);
       for(preambleCnt = 0; preambleCnt < nfapiRachInd.rachPdu[pduCnt].numPreamble; preambleCnt++)
       {
          CMCHKPK(oduUnpackUInt8, &(nfapiRachInd.rachPdu[pduCnt].preambleInfo[preambleCnt].preambleIndex), mBuf);
          CMCHKPK(oduUnpackUInt8, &(nfapiRachInd.rachPdu[pduCnt].preambleInfo[preambleCnt].pad), mBuf);
          CMCHKPK(oduUnpackUInt16, &(nfapiRachInd.rachPdu[pduCnt].preambleInfo[preambleCnt].timingAdvance), mBuf);
          CMCHKPK(oduUnpackUInt32, &(nfapiRachInd.rachPdu[pduCnt].preambleInfo[preambleCnt].preamblePwr), mBuf);
       }
    }
    
    ret = nfapiSendRachIndToMac(&nfapiRachInd);
    return ret;
}

/*******************************************************************
 *
 * @brief Build And Sends Crc indication to MAC
 *
 * @details
 *
 *    Function : nfapiSendCrcIndToMac
 *
 *    Functionality:
 *      Builds and Sends EVENT_CRC_IND to MAC
 *
 * @params[in] fapi_crc_indication_t message pointer
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t nfapiSendCrcIndToMac(fapi_crc_ind_msg_body *nfapiCrcInd)
{
   Pst          pst;
   uint8_t      crcInfoIdx = 0;
   uint8_t      crcStatusIdx, ret = ROK;
   CrcInfo      *crcIndInfo = NULLP;
   CrcInd       *crcInd = NULLP;

   MAC_ALLOC_SHRABL_BUF(crcInd, sizeof(CrcInd));
   if(!crcInd)
   {
      DU_LOG("ERROR  -->  NFAPI_VNF : Memory Allocation failed in nfapiSendCrcIndToMac");
      return RFAILED;
   }

   crcInd->cellId = vnfDb.cellId;
   crcInd->timingInfo.sfn = nfapiCrcInd->sfn;
   crcInd->timingInfo.slot = nfapiCrcInd->slot;
   crcInd->numCrc = nfapiCrcInd->numCrcs;

   for(crcInfoIdx = 0; crcInfoIdx < crcInd->numCrc; crcInfoIdx++)
   {
      crcIndInfo = &crcInd->crcInfo[crcInfoIdx];
      crcIndInfo->handle      = nfapiCrcInd->crc[crcInfoIdx].handle;
      crcIndInfo->rnti        = nfapiCrcInd->crc[crcInfoIdx].rnti;
      crcIndInfo->harqId      = nfapiCrcInd->crc[crcInfoIdx].harqId;
      crcIndInfo->tbCrcStatus = nfapiCrcInd->crc[crcInfoIdx].tbCrcStatus;
      crcIndInfo->numCb       = nfapiCrcInd->crc[crcInfoIdx].numCb;
      for(crcStatusIdx = 0; crcStatusIdx < crcIndInfo->numCb; crcStatusIdx++)
      {
	      crcIndInfo->cbCrcStatus[crcStatusIdx] = \
	         nfapiCrcInd->crc[crcInfoIdx].cbCrcStatus[crcStatusIdx];
      }
      crcIndInfo->ul_cqi  = nfapiCrcInd->crc[crcInfoIdx].ul_cqi;
      crcIndInfo->timingAdvance = nfapiCrcInd->crc[crcInfoIdx].timingAdvance;
      crcIndInfo->rssi = nfapiCrcInd->crc[crcInfoIdx].rssi;
   }

   /* Fill post and sent to MAC */
   FILL_PST_LWR_MAC_TO_MAC(pst, EVENT_CRC_IND_TO_MAC);
   pst.selector = ODU_SELECTOR_LWLC;
   ret = packCrcInd(&pst, crcInd);
   return ret;
}

/*******************************************************************
 *
 * @brief Process FAPI_CRC_IND from PNF
 *
 * @details
 *
 *    Function : nfapiP7ProcCrcInd
 *
 *    Functionality:
 *           Processes FAPI_CRC_IND received from PNF
 *           Parametes can be referred from SCF222v2222.10.03, Sec 3.4.8
 *
 * @params[in] UDP Buffer 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t nfapiP7ProcCrcInd(Buffer *mBuf)
{
    uint8_t crcCnt = 0, cbCnt = 0, ret = ROK;
    fapi_crc_ind_msg_body  nfapiCrcInd;

    CMCHKPK(oduUnpackUInt16, &(nfapiCrcInd.sfn), mBuf); 
    CMCHKPK(oduUnpackUInt16, &(nfapiCrcInd.slot), mBuf); 
    CMCHKPK(oduUnpackUInt16, &(nfapiCrcInd.numCrcs), mBuf);

    for(crcCnt = 0; crcCnt < nfapiCrcInd.numCrcs; crcCnt++)
    {
       CMCHKPK(oduUnpackUInt32, &(nfapiCrcInd.crc[crcCnt].handle), mBuf);
       CMCHKPK(oduUnpackUInt16, &(nfapiCrcInd.crc[crcCnt].rnti), mBuf);
       CMCHKPK(oduUnpackUInt8,  &(nfapiCrcInd.crc[crcCnt].harqId), mBuf);
       CMCHKPK(oduUnpackUInt8,  &(nfapiCrcInd.crc[crcCnt].tbCrcStatus), mBuf);
       CMCHKPK(oduUnpackUInt8,  &(nfapiCrcInd.crc[crcCnt].ul_cqi), mBuf);
       CMCHKPK(oduUnpackUInt8,  &(nfapiCrcInd.crc[crcCnt].pad), mBuf);
       CMCHKPK(oduUnpackUInt16,  &(nfapiCrcInd.crc[crcCnt].numCb), mBuf);
       CMCHKPK(oduUnpackUInt16,  &(nfapiCrcInd.crc[crcCnt].timingAdvance), mBuf);
       CMCHKPK(oduUnpackUInt16,  &(nfapiCrcInd.crc[crcCnt].rssi), mBuf);

       for(cbCnt = 0; cbCnt < nfapiCrcInd.crc[crcCnt].numCb; cbCnt++)
       {
          CMCHKPK(oduUnpackUInt8,  &(nfapiCrcInd.crc[crcCnt].cbCrcStatus[cbCnt]), mBuf);
       }
    }
    ret = nfapiSendCrcIndToMac(&nfapiCrcInd);
    return ret;
}

/*******************************************************************
 *
 * @brief Build And Sends Rx Data indication to MAC
 *
 * @details
 *
 *    Function : nfapiSendRxDataIndToMac
 *
 *    Functionality:
 *      Builds and Sends EVENT_RX_DATA_IND to MAC
 *
 * @params[in] fapi_rx_data_indication_t message pointer
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t nfapiSendRxDataIndToMac(fapi_rx_data_ind_msg_body *nfapiRxDataInd)
{
   Pst           pst;
   uint8_t       pduIdx = 0, ret = ROK;
   RxDataInd     *rxDataInd = NULLP;
   RxDataIndPdu  *pdu = NULLP;   

   MAC_ALLOC_SHRABL_BUF(rxDataInd, sizeof(RxDataInd));
   if(!rxDataInd)
   {
      DU_LOG("ERROR  -->  NFAPI_VNF : Memory Allocation failed in nfapiSendRxDataIndToMac");
      return RFAILED;
   }

   rxDataInd->cellId = vnfDb.cellId;
   rxDataInd->timingInfo.sfn = nfapiRxDataInd->sfn; 
   rxDataInd->timingInfo.slot = nfapiRxDataInd->slot;
   rxDataInd->numPdus = nfapiRxDataInd->numPdus;

   for(pduIdx = 0; pduIdx < rxDataInd->numPdus; pduIdx++)
   {
      pdu = &rxDataInd->pdus[pduIdx];
      pdu->handle = nfapiRxDataInd->pdus[pduIdx].handle;
      pdu->rnti = nfapiRxDataInd->pdus[pduIdx].rnti;
      pdu->harqId = nfapiRxDataInd->pdus[pduIdx].harqId;
      pdu->pduLength = nfapiRxDataInd->pdus[pduIdx].pdu_length;
      pdu->ul_cqi = nfapiRxDataInd->pdus[pduIdx].ul_cqi;
      pdu->timingAdvance = nfapiRxDataInd->pdus[pduIdx].timingAdvance;
      pdu->rssi = nfapiRxDataInd->pdus[pduIdx].rssi;

      MAC_ALLOC_SHRABL_BUF(pdu->pduData, pdu->pduLength);
      if(pdu->pduData == NULLP)
      {
         DU_LOG("ERROR  -->  NFAPI_VNF : Memory Allocation failed in nfapiSendRxDataIndToMac");
         MAC_FREE_SHRABL_BUF(MAC_MEM_REGION, MAC_POOL, rxDataInd, sizeof(RxDataInd));
         return RFAILED;
      }
      memcpy(pdu->pduData, nfapiRxDataInd->pdus[pduIdx].pduData, pdu->pduLength);
   }

   /* Fill post and sent to MAC */
   FILL_PST_LWR_MAC_TO_MAC(pst, EVENT_RX_DATA_IND_TO_MAC);
   pst.selector = ODU_SELECTOR_LWLC;
   ret = packRxDataInd(&pst, rxDataInd);
   return ret;
}


/*******************************************************************
 *
 * @brief Process FAPI_RX_DATA_IND from PNF
 *
 * @details
 *
 *    Function : nfapiP7ProcRxDataInd
 *
 *    Functionality:
 *           Processes FAPI_RX_DATA_IND received from PNF
 *           Parametes can be referred from SCF222v2222.10.03, Sec 3.4.7
 *
 * @params[in] UDP Buffer 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t nfapiP7ProcRxDataInd(Buffer *mBuf)
{
    uint8_t pduCnt = 0, ret = ROK;
    uint16_t byteIdx = 0;
    fapi_rx_data_ind_msg_body  nfapiRxDataInd;

    CMCHKPK(oduUnpackUInt16, &(nfapiRxDataInd.sfn), mBuf); 
    CMCHKPK(oduUnpackUInt16, &(nfapiRxDataInd.slot), mBuf); 
    CMCHKPK(oduUnpackUInt16, &(nfapiRxDataInd.numPdus), mBuf);

    //Extract fapi_pdu_ind_info_t
    for(pduCnt = 0; pduCnt < nfapiRxDataInd.numPdus; pduCnt++)
    {
       CMCHKPK(oduUnpackUInt32, &(nfapiRxDataInd.pdus[pduCnt].handle), mBuf); 
       CMCHKPK(oduUnpackUInt16, &(nfapiRxDataInd.pdus[pduCnt].rnti), mBuf); 
       CMCHKPK(oduUnpackUInt8, &(nfapiRxDataInd.pdus[pduCnt].harqId), mBuf); 
       CMCHKPK(oduUnpackUInt8, &(nfapiRxDataInd.pdus[pduCnt].ul_cqi), mBuf); 
       CMCHKPK(oduUnpackUInt16, &(nfapiRxDataInd.pdus[pduCnt].timingAdvance), mBuf); 
       CMCHKPK(oduUnpackUInt16, &(nfapiRxDataInd.pdus[pduCnt].rssi), mBuf);
       CMCHKPK(oduUnpackUInt16, &(nfapiRxDataInd.pdus[pduCnt].pdu_length), mBuf);
       CMCHKPK(oduUnpackUInt8, &(nfapiRxDataInd.pdus[pduCnt].pad[0]), mBuf);
       CMCHKPK(oduUnpackUInt8, &(nfapiRxDataInd.pdus[pduCnt].pad[1]), mBuf);

       MAC_ALLOC(nfapiRxDataInd.pdus[pduCnt].pduData, nfapiRxDataInd.pdus[pduCnt].pdu_length);   
       if(nfapiRxDataInd.pdus[pduCnt].pduData == NULLP)
       {
          DU_LOG("ERROR  --> NFAPI_VNF: Memory Allocation failed in nfapiP7ProcRxDataInd()");
          while(pduCnt != 0)
          {
             pduCnt--;
             MAC_FREE(nfapiRxDataInd.pdus[pduCnt].pduData, nfapiRxDataInd.pdus[pduCnt].pdu_length);
          }
          return RFAILED;
       }
       for(byteIdx = 0; byteIdx < nfapiRxDataInd.pdus[pduCnt].pdu_length; byteIdx++)
       {
          CMCHKPK(oduUnpackUInt8, &(nfapiRxDataInd.pdus[pduCnt].pduData[byteIdx]), mBuf);
       }
    }

    ret = nfapiSendRxDataIndToMac(&nfapiRxDataInd);
    for(pduCnt = 0; pduCnt < nfapiRxDataInd.numPdus; pduCnt++)
    {
       MAC_FREE(nfapiRxDataInd.pdus[pduCnt].pduData, nfapiRxDataInd.pdus[pduCnt].pdu_length);
    }
    return ret;
}

/*******************************************************************
 *
 * @brief Fills Uci Ind Pdu Info carried on Pucch Format 0/Format 1
 *
 * @details
 *
 *    Function : nfapiFillUciIndPucchF0F1
 *
 *    Functionality:
 *       Fills Uci Ind Pdu Info carried on Pucch Format 0/Format 1
 *
 *@params[in] UciPucchF0F1 *
 *            fapi_uci_o_pucch_f0f1_t *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t nfapiFillUciIndPucchF0F1(UciPucchF0F1 *pduInfo, fapi_uci_o_pucch_f0f1_t *fapiPduInfo)
{
   uint8_t harqIdx;
  
   if(fapiPduInfo == NULLP)
   {
      DU_LOG("\nERROR  --> NFAPI_VNF: nfapiFillUciIndPucchF0F1(): UCI PDU is NULL ");
      return RFAILED;
   }

   pduInfo->handle        = fapiPduInfo->handle;
   pduInfo->pduBitmap     = fapiPduInfo->pduBitmap;
   pduInfo->pucchFormat   = fapiPduInfo->pucchFormat;
   pduInfo->ul_cqi        = fapiPduInfo->ul_cqi;
   pduInfo->crnti         = fapiPduInfo->rnti;
   pduInfo->timingAdvance = fapiPduInfo->timingAdvance;
   pduInfo->rssi          = fapiPduInfo->rssi;   
   if(fapiPduInfo->srInfo.srIndication)
   {
      pduInfo->srInfo.srIndPres = fapiPduInfo->srInfo.srIndication;
      pduInfo->srInfo.srConfdcLevel = fapiPduInfo->srInfo.srConfidenceLevel;
   }
   if(fapiPduInfo->harqInfo.numHarq)
   {
      pduInfo->harqInfo.numHarq = fapiPduInfo->harqInfo.numHarq;
      pduInfo->harqInfo.harqConfdcLevel = fapiPduInfo->harqInfo.harqConfidenceLevel;
      for(harqIdx = 0; harqIdx < pduInfo->harqInfo.numHarq; harqIdx++)
      {
         pduInfo->harqInfo.harqValue[harqIdx] = fapiPduInfo->harqInfo.harqValue[harqIdx];
      }
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Build And Sends UCI indication to MAC
 *
 * @details
 *
 *    Function : nfapiSendUciIndToMac
 *
 *    Functionality:
 *      Builds and Sends EVENT_UCI_IND to MAC
 *
 * @params[in] fapi_uci_indication_t message pointer
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t nfapiSendUciIndToMac(fapi_uci_ind_msg_body *nfapiUciInd)
{
   uint8_t pduIdx;
   uint8_t ret = ROK;
   Pst     pst;
   UciInd  *macUciInd = NULLP;

   MAC_ALLOC_SHRABL_BUF(macUciInd, sizeof(UciInd));
   if(!macUciInd)
   {
      DU_LOG("ERROR  -->  NFAPI_VNF: Memory Allocation failed in nfapiSendUciIndToMac");
      return RFAILED;
   }

   DU_LOG("DEBUG  -->  NFAPI_VNF: Processing UCI Indication");
   memset(macUciInd, 0, sizeof(UciInd));
   macUciInd->cellId = vnfDb.cellId;
   macUciInd->slotInd.sfn = nfapiUciInd->sfn; 
   macUciInd->slotInd.slot = nfapiUciInd->slot;
   macUciInd->numUcis = nfapiUciInd->numUcis;

   for(pduIdx = 0; pduIdx < macUciInd->numUcis; pduIdx++)
   {
      macUciInd->pdus[pduIdx].pduType = nfapiUciInd->uciPdu[pduIdx].pduType;
      switch(macUciInd->pdus[pduIdx].pduType)
      {
         case UCI_IND_PUSCH:
         break;
         case UCI_IND_PUCCH_F0F1:
         {
            UciPucchF0F1 *pduInfo = NULLP;
            macUciInd->pdus[pduIdx].pduSize = nfapiUciInd->uciPdu[pduIdx].pduSize;
            pduInfo = &macUciInd->pdus[pduIdx].uci.uciPucchF0F1;
            ret = nfapiFillUciIndPucchF0F1(pduInfo, &nfapiUciInd->uciPdu[pduIdx].uci.uciPucchF0F1);
            break;
         }
         case UCI_IND_PUCCH_F2F3F4:
            break;
         default:
         {
            DU_LOG("ERROR  -->  NFAPI_VNF: Invalid Pdu Type %d at nfapiSendUciIndToMac()", macUciInd->pdus[pduIdx].pduType);
	         ret = RFAILED;
            break;
         }
      }
   }
   if(!ret)
   {
      FILL_PST_LWR_MAC_TO_MAC(pst, EVENT_UCI_IND_TO_MAC);
      pst.selector = ODU_SELECTOR_LWLC;
      ret = packUciInd(&pst, macUciInd);
   }
   else
   {
      DU_LOG("ERROR  -->  NFAPI_VNF: Failed sending UCI Ind to MAC");
   }
   return ret;
}

/*******************************************************************
 *
 * @brief Process FAPI_UCI_IND from PNF
 *
 * @details
 *
 *    Function : nfapiP7ProcUciInd
 *
 *    Functionality:
 *           Processes FAPI_UCI_IND received from PNF
 *           Parametes can be referred from SCF222v2222.10.03, Sec 3.4.9
 *
 * @params[in] UDP Buffer 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t nfapiP7ProcUciInd(Buffer *mBuf)
{
    uint8_t uciIdx = 0, ret = ROK;
    uint16_t harqIdx = 0;
    fapi_uci_ind_msg_body  nfapiUciInd;

    CMCHKPK(oduUnpackUInt16, &(nfapiUciInd.sfn), mBuf); 
    CMCHKPK(oduUnpackUInt16, &(nfapiUciInd.slot), mBuf); 
    CMCHKPK(oduUnpackUInt16, &(nfapiUciInd.numUcis), mBuf); 

    for(uciIdx = 0; uciIdx < nfapiUciInd.numUcis; uciIdx++)
    {
       CMCHKPK(oduUnpackUInt16, &(nfapiUciInd.uciPdu[uciIdx].pduType), mBuf); 
       CMCHKPK(oduUnpackUInt16, &(nfapiUciInd.uciPdu[uciIdx].pduSize), mBuf);
       switch(nfapiUciInd.uciPdu[uciIdx].pduType)
       {
          case UCI_IND_PUSCH:
          break;
          case UCI_IND_PUCCH_F0F1:
          {
             CMCHKPK(oduUnpackUInt32, &(nfapiUciInd.uciPdu[uciIdx].uci.uciPucchF0F1.handle), mBuf);
             CMCHKPK(oduUnpackUInt8, &(nfapiUciInd.uciPdu[uciIdx].uci.uciPucchF0F1.pduBitmap), mBuf);
             CMCHKPK(oduUnpackUInt8, &(nfapiUciInd.uciPdu[uciIdx].uci.uciPucchF0F1.pucchFormat), mBuf);
             CMCHKPK(oduUnpackUInt8, &(nfapiUciInd.uciPdu[uciIdx].uci.uciPucchF0F1.ul_cqi), mBuf);
             CMCHKPK(oduUnpackUInt8, &(nfapiUciInd.uciPdu[uciIdx].uci.uciPucchF0F1.pad), mBuf);
             CMCHKPK(oduUnpackUInt16, &(nfapiUciInd.uciPdu[uciIdx].uci.uciPucchF0F1.rnti), mBuf);
             CMCHKPK(oduUnpackUInt16, &(nfapiUciInd.uciPdu[uciIdx].uci.uciPucchF0F1.timingAdvance), mBuf);
             CMCHKPK(oduUnpackUInt16, &(nfapiUciInd.uciPdu[uciIdx].uci.uciPucchF0F1.rssi), mBuf);
             CMCHKPK(oduUnpackUInt8, &(nfapiUciInd.uciPdu[uciIdx].uci.uciPucchF0F1.pad1[0]), mBuf);
             CMCHKPK(oduUnpackUInt8, &(nfapiUciInd.uciPdu[uciIdx].uci.uciPucchF0F1.pad1[1]), mBuf);

             //Extract fapi_sr_f0f1_info_t
             CMCHKPK(oduUnpackUInt8, &(nfapiUciInd.uciPdu[uciIdx].uci.uciPucchF0F1.srInfo.srIndication), mBuf);
             CMCHKPK(oduUnpackUInt8, &(nfapiUciInd.uciPdu[uciIdx].uci.uciPucchF0F1.srInfo.srConfidenceLevel), mBuf);
             CMCHKPK(oduUnpackUInt8, &(nfapiUciInd.uciPdu[uciIdx].uci.uciPucchF0F1.srInfo.pad[0]), mBuf);
             CMCHKPK(oduUnpackUInt8, &(nfapiUciInd.uciPdu[uciIdx].uci.uciPucchF0F1.srInfo.pad[1]), mBuf);

             //Extract fapi_harq_f0f1_info_t
             CMCHKPK(oduUnpackUInt8, &(nfapiUciInd.uciPdu[uciIdx].uci.uciPucchF0F1.harqInfo.numHarq), mBuf);
             CMCHKPK(oduUnpackUInt8, &(nfapiUciInd.uciPdu[uciIdx].uci.uciPucchF0F1.harqInfo.harqConfidenceLevel), mBuf);
             for(harqIdx = 0;harqIdx <nfapiUciInd.uciPdu[uciIdx].uci.uciPucchF0F1.harqInfo.numHarq;harqIdx++)
             {
                CMCHKPK(oduUnpackUInt8, &(nfapiUciInd.uciPdu[uciIdx].uci.uciPucchF0F1.harqInfo.harqValue[harqIdx]), mBuf);
             
             }

             break;
          }
          case UCI_IND_PUCCH_F2F3F4:
          break;
          default:
          {
             DU_LOG("ERROR  --> NFAPI_VNF: Incorrect UCI PDUType:%d",nfapiUciInd.uciPdu[uciIdx].pduType);
             return RFAILED;
          }
       }
    }

    ret = nfapiSendUciIndToMac(&nfapiUciInd);
    return ret; 
}


/*******************************************************************
 *
 * @brief Processed the NFAPI P7 message from UDP socket 
 *
 * @details
 *
 *    Function : nfapiP7MsgHandler
 *
 *    Functionality:
 *      Extracts the Hdr of P7 msgs and re-direct msgs to its particular
 *      handling.
 *      
 *
 * @params[in] Buffer received in UDP
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t nfapiP7MsgHandler(Buffer *mBuf)
{
   nFapi_p7_hdr p7Hdr;
   nFapi_msg_header msgHdr;

   nFapiExtractP7Hdr(&p7Hdr, mBuf);
   nFapiExtractMsgHdr(&msgHdr, mBuf);

   switch(msgHdr.msg_id)
   {
      case TAG_NFAPI_UL_NODE_SYNC:
      {
         DU_LOG("INFO  --> NFAPI_VNF: Received UL Node Synch");
         nfapiP7ProcUlNodeSync(mBuf);
         break;
      }
      case FAPI_RACH_INDICATION:
      {
         DU_LOG("INFO  --> NFAPI_VNF: Received RACH INDICATIOn");
         nfapiP7ProcRachInd(mBuf);
         break;
      }
      case FAPI_CRC_INDICATION:
      {
         DU_LOG("INFO  --> NFAPI_VNF: Received CRC INDICATIOn");
         nfapiP7ProcCrcInd(mBuf);
         break;
      }
      case FAPI_RX_DATA_INDICATION:
      {
         DU_LOG("INFO  --> NFAPI_VNF: Received RX DATA INDICATIOn");
         nfapiP7ProcRxDataInd(mBuf);
         break;
      }
      case FAPI_UCI_INDICATION:
      {
         DU_LOG("INFO  --> NFAPI_VNF: Received UCI INDICATIOn");
         nfapiP7ProcUciInd(mBuf);
         break;
      }
      default:
      {
         DU_LOG("ERROR --> NFAPI_VNF: Wrong MsgId:%d", msgHdr.msg_id);
         return RFAILED;
      }
   }
   return ROK;
}

#endif
