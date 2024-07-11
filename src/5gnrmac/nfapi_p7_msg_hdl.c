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
#include "lwr_mac_fsm.h"
#include "fapi_vendor_extension.h"
#include "nfapi_interface.h"
#include "nfapi_common.h"
#include "nfapi_udp_p7.h"

extern NfapiVnfDb vnfDb;

/***********************************************************************
 *
 * @brief Pack parameters of Precoding and Beamforming
 *
 * @details
 *
 *    Function : nfapiFillPrecodingBeamform
 *
 *    Functionality:
 *       Pack parameters of Precoding and Beamforming and fill it in MsgBuffer
 *       of NFAPI
 *
 * @params[in] Buffer *mBuf, preCodingAndBeamforming
 * @return void
 *
 * ********************************************************************/

void nfapiFillPrecodingBeamform(fapi_precoding_bmform_t *preCodingAndBeamforming, Buffer *mBuf)
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
      DU_LOG("\nERROR  --> NFAPI_VNF : Memory allocation failed in packPnfParamReq");
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
               
               nfapiFillPrecodingBeamform(&dlTtiReq->pdus[pduIdx].pdu.pdcch_pdu.dlDci[dciIndex].pc_and_bform, mBuf);
               
               CMCHKPK(oduPackPostUInt8, dlTtiReq->pdus[pduIdx].pdu.pdcch_pdu.dlDci[dciIndex].beta_pdcch_1_0, mBuf);
               CMCHKPK(oduPackPostUInt8, dlTtiReq->pdus[pduIdx].pdu.pdcch_pdu.dlDci[dciIndex].powerControlOffsetSS, mBuf);
               CMCHKPK(oduPackPostUInt16, dlTtiReq->pdus[pduIdx].pdu.pdcch_pdu.dlDci[dciIndex].payloadSizeBits, mBuf);
               numBytes = dlTtiReq->pdus[pduIdx].pdu.pdcch_pdu.dlDci[dciIndex].payloadSizeBits / 8;
               if(dlTtiReq->pdus[pduIdx].pdu.pdcch_pdu.dlDci[dciIndex].payloadSizeBits % 8)
                  numBytes += 1;

               for(uint8_t payloadIdx = 0; payloadIdx < numBytes; payloadIdx++)
               {
                  CMCHKPK(oduPackPostUInt16, dlTtiReq->pdus[pduIdx].pdu.pdcch_pdu.dlDci[dciIndex].payload[payloadIdx], mBuf);
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
            
            nfapiFillPrecodingBeamform(&dlTtiReq->pdus[pduIdx].pdu.pdsch_pdu.preCodingAndBeamforming, mBuf);
            
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
            CMCHKPK(oduPackPostUInt8, dlTtiReq->pdus[pduIdx].pdu.ssb_pdu.ssbOffsetPointA, mBuf);
            CMCHKPK(oduPackPostUInt32, dlTtiReq->pdus[pduIdx].pdu.ssb_pdu.bchPayload.bchPayload, mBuf);
            CMCHKPK(oduPackPostUInt8, dlTtiReq->pdus[pduIdx].pdu.ssb_pdu.bchPayload.phyMibPdu.dmrsTypeAPosition, mBuf);
            CMCHKPK(oduPackPostUInt8, dlTtiReq->pdus[pduIdx].pdu.ssb_pdu.bchPayload.phyMibPdu.pdcchConfigSib1, mBuf);
            CMCHKPK(oduPackPostUInt8, dlTtiReq->pdus[pduIdx].pdu.ssb_pdu.bchPayload.phyMibPdu.cellBarred, mBuf);
            CMCHKPK(oduPackPostUInt8, dlTtiReq->pdus[pduIdx].pdu.ssb_pdu.bchPayload.phyMibPdu.intraFreqReselection, mBuf);
    
            nfapiFillPrecodingBeamform(&dlTtiReq->pdus[pduIdx].pdu.ssb_pdu.preCodingAndBeamforming, mBuf);
            
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
   switch(msgType)
   {
      case FAPI_DL_TTI_REQUEST:
      {
         DU_LOG("\nINFO   --> NFAPI_VNF:DL_TTI_REQ received from LWR_MAC as FAPI_MSG_BODY");
         nfapiBuildAndSendDlTtiReq(fapiP7Msg, msgLen);
         break;
      }
      default:
      {
         DU_LOG("\nERROR  --> NFAPI_VNF: Incorrect MsgType:%d",msgType);
         break;
      }
   }
   LWR_MAC_FREE(fapiP7Msg, msgLen);   
   return ROK;
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
uint8_t  nfapiP7MsgHandler(Buffer *mBuf)
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

      default:
      {
         DU_LOG("ERROR --> NFAPI_VNF: Wrong MsgId:%d", msgHdr.msg_id);
         return RFAILED;
      }
   }

   return ROK;
}

#endif
