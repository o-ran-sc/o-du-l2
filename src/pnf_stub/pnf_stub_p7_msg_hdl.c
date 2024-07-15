/*******************************************************************************
 * ################################################################################
 * #   Copyright (c) [2017-2019] [Radisys]
 * #
 * #
 * #
 * #   Licensed under the Apache License, Version 2.0 (the "License");
 * #
 * #   you may not use this file except in compliance with the License.
 * #
 * #   You may obtain a copy of the License at
 * #
 * #
 * #
 * #       http://www.apache.org/licenses/LICENSE-2.0
 * #
 * #
 * #
 * #   Unless required by applicable law or agreed to in writing, software
 * #
 * #   distributed under the License is distributed on an "AS IS" BASIS,
 * #
 * #   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * #
 * #   See the License for the specific language governing permissions and
 * #
 * #   limitations under the License.
 * #
 * ################################################################################
 * *******************************************************************************/

/* This functions contains main() for pnf simulator */
#include "common_def.h"
#include <unistd.h>
#include "pnf_stub_p5_msg_hdl.h"
#include "nfapi_interface.h"
#include "pnf_stub_sctp.h"
#include "pnf_stub_p7_udp.h"
#include "pnf_stub.h"

extern PnfGlobalCb pnfCb;

/*********************************************************************************
 * @Brief: Filling of Ul Node Sync
 *
 * @Function: fillUlNodeSync
 *
 * @Description: At PNF, realize delta SFN/SLOT from VNF and adjust PNF's
 *               SFN/Slot and calculate t3.
 *
 * @Params [IN]: delta_sfnSlot, ulSyncInfo
 * [OUT]: void
 *
 * ******************************************************************************/

void fillUlNodeSync(int32_t delta_sfnSlot, nFapi_ul_node_sync_info *ulSyncInfo)
{
    PnfSlotInfo  deltaSfnSlot;

    if(delta_sfnSlot != 0)
    {
       EXTRACT_SFN_SLOT_FROM_DELTA(abs(delta_sfnSlot), deltaSfnSlot);
       if(delta_sfnSlot < 0)
       {
          pnfCb.pnfSlotInfo.sfn -= deltaSfnSlot.sfn;
          pnfCb.pnfSlotInfo.slot -= deltaSfnSlot.slot;
       }
       else
       {
          pnfCb.pnfSlotInfo.sfn += deltaSfnSlot.sfn;
          pnfCb.pnfSlotInfo.slot += deltaSfnSlot.slot;
       }
    }
    else
    {
       DU_LOG("INFO   --> NFAPI_PNF: No Delta between PNF and VNF");
    }

    ulSyncInfo->t3 = CALC_TIME_USEC_FROM_SFNSLOT(pnfCb.pnfSlotInfo);
    return;
}

/*********************************************************************************
 * @Brief: Building and Sending Ul Node Sync
 *
 * @Function: buildAndSendUlNodeSync
 *
 * @Description: At PNF , encode all the parameters of UL Node sync and Send to
 *                 VNF via UDP
 *
 * @Params [IN]: Ptr to ulSyncInfo
 * [OUT]: ROK/RFAILED
 *
 * ******************************************************************************/

uint8_t buildAndSendUlNodeSync(nFapi_ul_node_sync_info *ulSyncInfo)
{
   Buffer *mBuf = NULLP;

   if (ODU_GET_MSG_BUF(PNF_APP_MEM_REG, PNF_POOL, &mBuf) != ROK)
   {
      DU_LOG("ERROR  --> NFAPI_PNF : Memory allocation failed in start response");
      return RFAILED;
   }
   nfapiFillP7Hdr(mBuf, (sizeof(nFapi_ul_node_sync_info) + sizeof(nFapi_msg_header)), 0, 0);
   nfapiFillMsgHdr(mBuf, 1, TAG_NFAPI_UL_NODE_SYNC, sizeof(nFapi_ul_node_sync_info));

   CMCHKPK(oduPackPostUInt32, ulSyncInfo->t1, mBuf);
   CMCHKPK(oduPackPostUInt32, ulSyncInfo->t2, mBuf);
   CMCHKPK(oduPackPostUInt32, ulSyncInfo->t3, mBuf);

   if(pnfP7UdpSendMsg(mBuf) != ROK)
   { 
      return RFAILED;
   }
   return ROK;
}

/*********************************************************************************
 * @Brief: Process and Handling of Dl Node Sync
 *
 * @Function Name: pnfDlNodeSyncHandler
 *
 *
 * @Functionality: 
 * At PNF , extract all the parameters of DL Node sync and uses t1, 
 * delta_sfnSlot  while processing and builing UL Node Sync
 *
 * @Params [IN]: Message Buffer received at UDP NFAPI P7 Interface
 * [OUT]: ROK/RFAILED
 *
 * *******************************************************************************/
uint8_t pnfDlNodeSyncHandler(Buffer *mBuf)
{
    uint8_t                 ret = ROK;
    nFapi_dl_node_sync_info dlNodeSync;
    nFapi_ul_node_sync_info ulSyncInfo;
    PnfSlotInfo             vnfFrameInfo;

    CMCHKPK(oduUnpackUInt32, &(dlNodeSync.t1), mBuf);
    CMCHKPK(SUnpkS32, &(dlNodeSync.delta_sfnSlot), mBuf);
    CMCHKPK(oduUnpackUInt8, &(dlNodeSync.scs), mBuf);

    DU_LOG(" PNF_NFAPI: t1:%u, delta:%d, scs:%d",dlNodeSync.t1, dlNodeSync.delta_sfnSlot, dlNodeSync.scs);

    EXTRACT_SFN_SLOT_FROM_TIME(dlNodeSync.t1, vnfFrameInfo);

    ulSyncInfo.t1 = dlNodeSync.t1;
    ulSyncInfo.t2 = CALC_TIME_USEC_FROM_SFNSLOT(pnfCb.pnfSlotInfo);
    
    fillUlNodeSync(dlNodeSync.delta_sfnSlot, &ulSyncInfo);
    
    ret = buildAndSendUlNodeSync(&ulSyncInfo);
    return ret;
}

/*********************************************************************************
 *
 * @Function Name: pnfDlTtiReq
 *
 *
 * @Functionality: 
 *    Handles the P7 DL TTI REQ by unpacking the MsgBuffer received
 *
 * @Params [IN]: Message Buffer received at UDP NFAPI P7 Interface
 *
 * *******************************************************************************/
uint8_t pnfDlTtiReq(Buffer *mBuf)
{
   fapi_dl_tti_req_msg_body fapiMsgBody;

   CMCHKPK(oduUnpackUInt16, &fapiMsgBody.sfn, mBuf);
   CMCHKPK(oduUnpackUInt16, &fapiMsgBody.slot, mBuf);
   CMCHKPK(oduUnpackUInt8, &fapiMsgBody.nPdus, mBuf);
   DU_LOG("INFO   --> NFAPI_PNF: DL_TTI_REQ SFN/SLOT:%d/%d, nPdu:%d",fapiMsgBody.sfn, fapiMsgBody.slot, fapiMsgBody.nPdus);
   return ROK;
}

/*********************************************************************************
 * @Brief: Unpackng Ul RxBeamforming received in UL_TTI_REQ
 *
 * @Function: pnfUnpackUlRxBeamforming
 *
 * @Description: Unpackng Ul RxBeamforming received in UL_TTI_REQ from VNF
 *
 * @Params [IN]: fapi_ul_rx_bmform_pdu_t, Buffer
 * [OUT]: void
 *
 * ******************************************************************************/

void pnfUnpackUlRxBeamforming(fapi_ul_rx_bmform_pdu_t *ulBmform, Buffer *mBuf)
{
   CMCHKPK(oduUnpackUInt16, &ulBmform->numPrgs, mBuf);
   CMCHKPK(oduUnpackUInt16, &ulBmform->prgSize, mBuf);
   CMCHKPK(oduUnpackUInt8, &ulBmform->digBfInterface, mBuf);
   for(uint8_t padIdx = 0; padIdx < 3; padIdx++)
   {
      CMCHKPK(oduUnpackUInt8, &ulBmform->pad[padIdx], mBuf);
   }
   for(uint8_t prgIdx = 0; prgIdx < ulBmform->numPrgs; prgIdx++)
   {
      for(uint8_t digBfIdx = 0; digBfIdx < ulBmform->digBfInterface; digBfIdx++)
      {
         CMCHKPK(oduUnpackUInt8, &ulBmform->rx_bfi[prgIdx].beamIdx[digBfIdx].beamidx, mBuf);
      }
   }
}

/*********************************************************************************
 * @Brief: Build And Send FAPI_RACH_IND to VNF (MSG1/RACH)
 *
 * @Function: pnfBuildAndSendRachInd
 *
 * @Description: Build And Send FAPI_RACH_IND to VNF
 * 
 * @Params [IN]: Sfn, Slot, raPreambleIdx
 * [OUT]: void
 *
 * ******************************************************************************/

uint8_t pnfBuildAndSendRachInd(uint16_t slot, uint16_t sfn, uint8_t raPreambleIdx)
{
   Buffer *mBuf = NULLP;

   if (ODU_GET_MSG_BUF(PNF_APP_MEM_REG, PNF_POOL, &mBuf) != ROK)
   {
      DU_LOG("ERROR  --> NFAPI_PNF : Memory allocation failed in start response");
      return RFAILED;
   }
   nfapiFillP7Hdr(mBuf, (sizeof(fapi_rach_ind_msg_body) + sizeof(nFapi_msg_header)), 0, 0);
   nfapiFillMsgHdr(mBuf, 1, FAPI_RACH_INDICATION, sizeof(fapi_rach_ind_msg_body));

   CMCHKPK(oduPackPostUInt16, sfn, mBuf);
   CMCHKPK(oduPackPostUInt16, slot, mBuf);
   CMCHKPK(oduPackPostUInt8, 1, mBuf); //numPdus

   //fapi_rach_pdu_t
   CMCHKPK(oduPackPostUInt16, NR_PCI, mBuf); //phyCellId
   CMCHKPK(oduPackPostUInt8, 0, mBuf); //symbolIndex
   CMCHKPK(oduPackPostUInt8, slot, mBuf); //slotIndex
   CMCHKPK(oduPackPostUInt8, 0, mBuf); //freqIndex
   CMCHKPK(oduPackPostUInt8, 0, mBuf); //avgRssi
   CMCHKPK(oduPackPostUInt8, 0, mBuf); //avgSnr
   CMCHKPK(oduPackPostUInt8, 1, mBuf); //numPreamble

   //fapi_preamble_info_t
   CMCHKPK(oduPackPostUInt8, raPreambleIdx, mBuf); //preambleIndex
   CMCHKPK(oduPackPostUInt8, 0, mBuf); //pad
   CMCHKPK(oduPackPostUInt16, 0, mBuf); //timingAdvance
   CMCHKPK(oduPackPostUInt32, 0, mBuf); //preamblePwr

   /* Sending RACH indication to VNF */
   DU_LOG("INFO   -->  NFAPI_PNF: Sending RACH Indication to VNF");
   
   if(pnfP7UdpSendMsg(mBuf) != ROK)
   { 
      return RFAILED;
   }
   return ROK;
}

/*********************************************************************************
 * @Brief: Processes UL_TTI_REQ received from VNF
 *
 * @Function: pnfProcUlTtiReq
 *
 * @Description:  Processes UL_TTI_REQ received from VNF and based on PDU type
 * received, PNF proceeds with the next steps for UE attach
 *
 * @Params [IN]: fapi_ul_tti_req_msg_body 
 * [OUT]: void
 *
 * ******************************************************************************/

void pnfProcUlTtiReq(fapi_ul_tti_req_msg_body *pnfUlTtiReq)
{
   uint8_t numPdus = pnfUlTtiReq->nPdus;

   while(numPdus)
   {
      if(pnfUlTtiReq->pdus[numPdus-1].pduType == PRACH_PDU_TYPE)
      {
         DU_LOG("INFO   --> NFAPI_PNF: PRACH PDU");
         if(pnfCb.pnfUeCb[UE_IDX_0].rachIndSent == false)
         {
            pnfCb.pnfUeCb[UE_IDX_0].isCFRA = false;
            pnfCb.pnfUeCb[UE_IDX_0].rachIndSent = true;
            pnfBuildAndSendRachInd(pnfUlTtiReq->slot, pnfUlTtiReq->sfn, CB_RA_PREAMBLE_IDX);
            pnfCb.numActiveUe++;
         }
      }
      if(pnfUlTtiReq->pdus[numPdus-1].pduType == PUSCH_PDU_TYPE)
      {
         DU_LOG("INFO   --> NFAPI_PNF: PUSCH PDU");
         /*TODO: CRC_IND and RX_DATA_IND to be built and Sent*/
      }
      if(pnfUlTtiReq->pdus[numPdus-1].pduType == PUSCH_PDU_TYPE)
      {
         DU_LOG("INFO   --> NFAPI_PNF: PUSCH PDU");
         /*TODO: UCI_IND to be built and sent*/ 
      }
      numPdus--;
   }
   return ROK;
}

/*********************************************************************************
 *
 * @Function Name: pnfUlTtiReq
 *
 *
 * @Functionality: 
 *    Handles the P7 UL TTI REQ by unpacking the MsgBuffer received
 *
 * @Params [IN]: Message Buffer received at UDP NFAPI P7 Interface
 *
 * *******************************************************************************/
uint8_t pnfUlTtiReq(Buffer *mBuf)
{
   uint8_t pduCnt = 0;
   fapi_ul_tti_req_msg_body fapiMsgBody;
   
   CMCHKPK(oduUnpackUInt16, &fapiMsgBody.sfn, mBuf);
   CMCHKPK(oduUnpackUInt16, &fapiMsgBody.slot, mBuf);
   CMCHKPK(oduUnpackUInt8, &fapiMsgBody.nPdus, mBuf);
   CMCHKPK(oduUnpackUInt8, &fapiMsgBody.rachPresent, mBuf);
   CMCHKPK(oduUnpackUInt8, &fapiMsgBody.nUlsch, mBuf);
   CMCHKPK(oduUnpackUInt8, &fapiMsgBody.nUlcch, mBuf);
   CMCHKPK(oduUnpackUInt8, &fapiMsgBody.nGroup, mBuf);
   

   DU_LOG("INFO   --> NFAPI_PNF: UL_TTI_REQ SFN/SLOT:%d/%d, nPdu:%d",fapiMsgBody.sfn, fapiMsgBody.slot, fapiMsgBody.nPdus);
   
   for(pduCnt = 0; pduCnt < fapiMsgBody.nPdus; pduCnt++)
   {
      CMCHKPK(oduUnpackUInt16, &fapiMsgBody.pdus[pduCnt].pduType, mBuf); 
      CMCHKPK(oduUnpackUInt16, &fapiMsgBody.pdus[pduCnt].pduSize, mBuf); 
      switch(fapiMsgBody.pdus[pduCnt].pduType)  
      {
         case PRACH_PDU_TYPE:
         {
             CMCHKPK(oduUnpackUInt16, &fapiMsgBody.pdus[pduCnt].pdu.prach_pdu.physCellId, mBuf); 
             CMCHKPK(oduUnpackUInt8, &fapiMsgBody.pdus[pduCnt].pdu.prach_pdu.numPrachOcas, mBuf); 
             CMCHKPK(oduUnpackUInt8, &fapiMsgBody.pdus[pduCnt].pdu.prach_pdu.prachFormat, mBuf); 
             CMCHKPK(oduUnpackUInt8, &fapiMsgBody.pdus[pduCnt].pdu.prach_pdu.numRa, mBuf); 
             CMCHKPK(oduUnpackUInt8, &fapiMsgBody.pdus[pduCnt].pdu.prach_pdu.prachStartSymbol, mBuf); 
             CMCHKPK(oduUnpackUInt8, &fapiMsgBody.pdus[pduCnt].pdu.prach_pdu.numCs, mBuf);
             pnfUnpackUlRxBeamforming(&fapiMsgBody.pdus[pduCnt].pdu.prach_pdu.beamforming, mBuf);
             break;
            break;
         }
         case PUSCH_PDU_TYPE:
         {
            CMCHKPK(oduUnpackUInt16, &fapiMsgBody.pdus[pduCnt].pdu.pusch_pdu.pduBitMap, mBuf);
            CMCHKPK(oduUnpackUInt16, &fapiMsgBody.pdus[pduCnt].pdu.pusch_pdu.pduBitMap, mBuf);
            CMCHKPK(oduUnpackUInt32, &fapiMsgBody.pdus[pduCnt].pdu.pusch_pdu.handle, mBuf);
            CMCHKPK(oduUnpackUInt16, &fapiMsgBody.pdus[pduCnt].pdu.pusch_pdu.bwpSize, mBuf);
            CMCHKPK(oduUnpackUInt16, &fapiMsgBody.pdus[pduCnt].pdu.pusch_pdu.bwpStart, mBuf);
            CMCHKPK(oduUnpackUInt8, &fapiMsgBody.pdus[pduCnt].pdu.pusch_pdu.subCarrierSpacing, mBuf);
            CMCHKPK(oduUnpackUInt8, &fapiMsgBody.pdus[pduCnt].pdu.pusch_pdu.cyclicPrefix, mBuf);
            CMCHKPK(oduUnpackUInt8, &fapiMsgBody.pdus[pduCnt].pdu.pusch_pdu.mcsIndex, mBuf);
            CMCHKPK(oduUnpackUInt8, &fapiMsgBody.pdus[pduCnt].pdu.pusch_pdu.mcsTable, mBuf);
            CMCHKPK(oduUnpackUInt16, &fapiMsgBody.pdus[pduCnt].pdu.pusch_pdu.targetCodeRate, mBuf);
            CMCHKPK(oduUnpackUInt8, &fapiMsgBody.pdus[pduCnt].pdu.pusch_pdu.qamModOrder, mBuf);
            CMCHKPK(oduUnpackUInt8, &fapiMsgBody.pdus[pduCnt].pdu.pusch_pdu.transformPrecoding, mBuf);
            CMCHKPK(oduUnpackUInt16, &fapiMsgBody.pdus[pduCnt].pdu.pusch_pdu.dataScramblingId, mBuf);
            CMCHKPK(oduUnpackUInt8, &fapiMsgBody.pdus[pduCnt].pdu.pusch_pdu.nrOfLayers, mBuf);
            CMCHKPK(oduUnpackUInt8, &fapiMsgBody.pdus[pduCnt].pdu.pusch_pdu.dmrsConfigType, mBuf);
            CMCHKPK(oduUnpackUInt16, &fapiMsgBody.pdus[pduCnt].pdu.pusch_pdu.ulDmrsSymbPos, mBuf);
            CMCHKPK(oduUnpackUInt16, &fapiMsgBody.pdus[pduCnt].pdu.pusch_pdu.ulDmrsScramblingId, mBuf);
            CMCHKPK(oduUnpackUInt8, &fapiMsgBody.pdus[pduCnt].pdu.pusch_pdu.scid, mBuf);
            CMCHKPK(oduUnpackUInt8, &fapiMsgBody.pdus[pduCnt].pdu.pusch_pdu.numDmrsCdmGrpsNoData, mBuf);
            CMCHKPK(oduUnpackUInt16, &fapiMsgBody.pdus[pduCnt].pdu.pusch_pdu.dmrsPorts, mBuf);
            CMCHKPK(oduUnpackUInt16, &fapiMsgBody.pdus[pduCnt].pdu.pusch_pdu.nTpPuschId, mBuf);
            CMCHKPK(oduUnpackUInt16, &fapiMsgBody.pdus[pduCnt].pdu.pusch_pdu.tpPi2Bpsk, mBuf);
            for(uint8_t rbBitMapIdx = 0; rbBitMapIdx < 36; rbBitMapIdx++)
            {
               CMCHKPK(oduUnpackUInt8, &fapiMsgBody.pdus[pduCnt].pdu.pusch_pdu.rbBitmap[rbBitMapIdx], mBuf);
            }
            CMCHKPK(oduUnpackUInt16, &fapiMsgBody.pdus[pduCnt].pdu.pusch_pdu.rbStart, mBuf);
            CMCHKPK(oduUnpackUInt16, &fapiMsgBody.pdus[pduCnt].pdu.pusch_pdu.rbSize, mBuf);
            CMCHKPK(oduUnpackUInt8, &fapiMsgBody.pdus[pduCnt].pdu.pusch_pdu.vrbToPrbMapping, mBuf);
            CMCHKPK(oduUnpackUInt8, &fapiMsgBody.pdus[pduCnt].pdu.pusch_pdu.frequencyHopping, mBuf);
            CMCHKPK(oduUnpackUInt16, &fapiMsgBody.pdus[pduCnt].pdu.pusch_pdu.txDirectCurrentLocation, mBuf);
            CMCHKPK(oduUnpackUInt8, &fapiMsgBody.pdus[pduCnt].pdu.pusch_pdu.resourceAlloc, mBuf);
            CMCHKPK(oduUnpackUInt8, &fapiMsgBody.pdus[pduCnt].pdu.pusch_pdu.uplinkFrequencyShift7p5khz, mBuf);
            CMCHKPK(oduUnpackUInt8, &fapiMsgBody.pdus[pduCnt].pdu.pusch_pdu.startSymbIndex, mBuf);
            CMCHKPK(oduUnpackUInt8, &fapiMsgBody.pdus[pduCnt].pdu.pusch_pdu.nrOfSymbols, mBuf);
            CMCHKPK(oduUnpackUInt8, &fapiMsgBody.pdus[pduCnt].pdu.pusch_pdu.mappingType, mBuf);
            CMCHKPK(oduUnpackUInt8, &fapiMsgBody.pdus[pduCnt].pdu.pusch_pdu.nrOfDmrsSymbols, mBuf);
            CMCHKPK(oduUnpackUInt8, &fapiMsgBody.pdus[pduCnt].pdu.pusch_pdu.dmrsAddPos, mBuf);
            CMCHKPK(oduUnpackUInt8, &fapiMsgBody.pdus[pduCnt].pdu.pusch_pdu.pad, mBuf);

            //Fill fapi_pusch_data_t
            CMCHKPK(oduUnpackUInt8, &fapiMsgBody.pdus[pduCnt].pdu.pusch_pdu.puschData.rvIndex, mBuf);
            CMCHKPK(oduUnpackUInt8, &fapiMsgBody.pdus[pduCnt].pdu.pusch_pdu.puschData.harqProcessId, mBuf);
            CMCHKPK(oduUnpackUInt8, &fapiMsgBody.pdus[pduCnt].pdu.pusch_pdu.puschData.newDataIndicator, mBuf);
            CMCHKPK(oduUnpackUInt8, &fapiMsgBody.pdus[pduCnt].pdu.pusch_pdu.puschData.pad, mBuf);
            CMCHKPK(oduUnpackUInt32, &fapiMsgBody.pdus[pduCnt].pdu.pusch_pdu.puschData.tbSize, mBuf);
            CMCHKPK(oduUnpackUInt16, &fapiMsgBody.pdus[pduCnt].pdu.pusch_pdu.puschData.numCb, mBuf);
            CMCHKPK(oduUnpackUInt8, &fapiMsgBody.pdus[pduCnt].pdu.pusch_pdu.puschData.cbPresentAndPosition[0], mBuf);
            CMCHKPK(oduUnpackUInt8, &fapiMsgBody.pdus[pduCnt].pdu.pusch_pdu.puschData.cbPresentAndPosition[1], mBuf);

            //Fill fapi_pusch_uci_t
            CMCHKPK(oduUnpackUInt16, &fapiMsgBody.pdus[pduCnt].pdu.pusch_pdu.puschUci.harqAckBitLength, mBuf);  
            CMCHKPK(oduUnpackUInt16, &fapiMsgBody.pdus[pduCnt].pdu.pusch_pdu.puschUci.csiPart1BitLength, mBuf);  
            CMCHKPK(oduUnpackUInt16, &fapiMsgBody.pdus[pduCnt].pdu.pusch_pdu.puschUci.csiPart2BitLength, mBuf);  
            CMCHKPK(oduUnpackUInt8, &fapiMsgBody.pdus[pduCnt].pdu.pusch_pdu.puschUci.alphaScaling, mBuf);  
            CMCHKPK(oduUnpackUInt8, &fapiMsgBody.pdus[pduCnt].pdu.pusch_pdu.puschUci.betaOffsetHarqAck, mBuf);  
            CMCHKPK(oduUnpackUInt8, &fapiMsgBody.pdus[pduCnt].pdu.pusch_pdu.puschUci.betaOffsetCsi1, mBuf);  
            CMCHKPK(oduUnpackUInt8, &fapiMsgBody.pdus[pduCnt].pdu.pusch_pdu.puschUci.betaOffsetCsi2, mBuf);  
            CMCHKPK(oduUnpackUInt8, &fapiMsgBody.pdus[pduCnt].pdu.pusch_pdu.puschUci.pad[0], mBuf);  
            CMCHKPK(oduUnpackUInt8, &fapiMsgBody.pdus[pduCnt].pdu.pusch_pdu.puschUci.pad[1], mBuf);

            //Fill fapi_pusch_ptrs_t
            CMCHKPK(oduUnpackUInt8, &fapiMsgBody.pdus[pduCnt].pdu.pusch_pdu.puschPtrs.numPtrsPorts, mBuf);
            CMCHKPK(oduUnpackUInt8, &fapiMsgBody.pdus[pduCnt].pdu.pusch_pdu.puschPtrs.ptrsTimeDensity, mBuf);
            CMCHKPK(oduUnpackUInt8, &fapiMsgBody.pdus[pduCnt].pdu.pusch_pdu.puschPtrs.ptrsFreqDensity, mBuf);
            CMCHKPK(oduUnpackUInt8, &fapiMsgBody.pdus[pduCnt].pdu.pusch_pdu.puschPtrs.ulPtrsPower, mBuf);
            for(uint8_t portIdx = 0; portIdx < &fapiMsgBody.pdus[pduCnt].pdu.pusch_pdu.puschPtrs.numPtrsPorts; portIdx++)
            {
               CMCHKPK(oduUnpackUInt16, &fapiMsgBody.pdus[pduCnt].pdu.pusch_pdu.puschPtrs.ptrsInfo[portIdx].ptrsPortIndex, mBuf);
               CMCHKPK(oduUnpackUInt8, &fapiMsgBody.pdus[pduCnt].pdu.pusch_pdu.puschPtrs.ptrsInfo[portIdx].ptrsDmrsPort, mBuf);
               CMCHKPK(oduUnpackUInt8, &fapiMsgBody.pdus[pduCnt].pdu.pusch_pdu.puschPtrs.ptrsInfo[portIdx].ptrsReOffset, mBuf);
            }

            //Fill fapi_dfts_ofdm_t
            CMCHKPK(oduUnpackUInt16, &fapiMsgBody.pdus[pduCnt].pdu.pusch_pdu.dftsOfdm.lowPaprSequenceNumber, mBuf); 
            CMCHKPK(oduUnpackUInt8, &fapiMsgBody.pdus[pduCnt].pdu.pusch_pdu.dftsOfdm.lowPaprGroupNumber, mBuf);
            CMCHKPK(oduUnpackUInt8, &fapiMsgBody.pdus[pduCnt].pdu.pusch_pdu.dftsOfdm.ulPtrsSampleDensity, mBuf);
            CMCHKPK(oduUnpackUInt8, &fapiMsgBody.pdus[pduCnt].pdu.pusch_pdu.dftsOfdm.ulPtrsTimeDensityTransformPrecoding, mBuf);
            CMCHKPK(oduUnpackUInt8, &fapiMsgBody.pdus[pduCnt].pdu.pusch_pdu.dftsOfdm.pad, mBuf);

            //Fill fapi_ul_rx_bmform_pdu_t
            pnfUnpackUlRxBeamforming(&fapiMsgBody.pdus[pduCnt].pdu.pusch_pdu.beamforming, mBuf);
            break;
         }
         case PUCCH_PDU_TYPE:
         {
            CMCHKPK(oduUnpackUInt16, &fapiMsgBody.pdus[pduCnt].pdu.pucch_pdu.rnti, mBuf);
            CMCHKPK(oduUnpackUInt8, &fapiMsgBody.pdus[pduCnt].pdu.pucch_pdu.pad1[0], mBuf);
            CMCHKPK(oduUnpackUInt8, &fapiMsgBody.pdus[pduCnt].pdu.pucch_pdu.pad1[1], mBuf);
            CMCHKPK(oduUnpackUInt32, &fapiMsgBody.pdus[pduCnt].pdu.pucch_pdu.handle, mBuf);
            CMCHKPK(oduUnpackUInt16, &fapiMsgBody.pdus[pduCnt].pdu.pucch_pdu.bwpSize, mBuf);
            CMCHKPK(oduUnpackUInt16, &fapiMsgBody.pdus[pduCnt].pdu.pucch_pdu.bwpStart, mBuf);
            CMCHKPK(oduUnpackUInt8, &fapiMsgBody.pdus[pduCnt].pdu.pucch_pdu.subCarrierSpacing, mBuf);
            CMCHKPK(oduUnpackUInt8, &fapiMsgBody.pdus[pduCnt].pdu.pucch_pdu.cyclicPrefix, mBuf);
            CMCHKPK(oduUnpackUInt8, &fapiMsgBody.pdus[pduCnt].pdu.pucch_pdu.formatType, mBuf);
            CMCHKPK(oduUnpackUInt8, &fapiMsgBody.pdus[pduCnt].pdu.pucch_pdu.multiSlotTxIndicator, mBuf);
            CMCHKPK(oduUnpackUInt8, &fapiMsgBody.pdus[pduCnt].pdu.pucch_pdu.pi2Bpsk, mBuf);
            CMCHKPK(oduUnpackUInt8, &fapiMsgBody.pdus[pduCnt].pdu.pucch_pdu.pad2, mBuf);
            CMCHKPK(oduUnpackUInt16, &fapiMsgBody.pdus[pduCnt].pdu.pucch_pdu.prbStart, mBuf);
            CMCHKPK(oduUnpackUInt16, &fapiMsgBody.pdus[pduCnt].pdu.pucch_pdu.prbSize, mBuf);
            CMCHKPK(oduUnpackUInt8, &fapiMsgBody.pdus[pduCnt].pdu.pucch_pdu.startSymbolIndex, mBuf);
            CMCHKPK(oduUnpackUInt8, &fapiMsgBody.pdus[pduCnt].pdu.pucch_pdu.nrOfSymbols, mBuf);
            CMCHKPK(oduUnpackUInt8, &fapiMsgBody.pdus[pduCnt].pdu.pucch_pdu.freqHopFlag, mBuf);
            CMCHKPK(oduUnpackUInt8, &fapiMsgBody.pdus[pduCnt].pdu.pucch_pdu.groupHopFlag, mBuf);
            CMCHKPK(oduUnpackUInt8, &fapiMsgBody.pdus[pduCnt].pdu.pucch_pdu.sequenceHopFlag, mBuf);
            CMCHKPK(oduUnpackUInt8, &fapiMsgBody.pdus[pduCnt].pdu.pucch_pdu.pad3, mBuf);
            CMCHKPK(oduUnpackUInt16, &fapiMsgBody.pdus[pduCnt].pdu.pucch_pdu.secondHopPrb, mBuf);
            CMCHKPK(oduUnpackUInt16, &fapiMsgBody.pdus[pduCnt].pdu.pucch_pdu.hoppingId, mBuf);
            CMCHKPK(oduUnpackUInt16, &fapiMsgBody.pdus[pduCnt].pdu.pucch_pdu.initialCyclicShift, mBuf);
            CMCHKPK(oduUnpackUInt16, &fapiMsgBody.pdus[pduCnt].pdu.pucch_pdu.dataScramblingId, mBuf);
            CMCHKPK(oduUnpackUInt8, &fapiMsgBody.pdus[pduCnt].pdu.pucch_pdu.timeDomainOccIdx, mBuf);
            CMCHKPK(oduUnpackUInt8, &fapiMsgBody.pdus[pduCnt].pdu.pucch_pdu.preDftOccIdx, mBuf);
            CMCHKPK(oduUnpackUInt8, &fapiMsgBody.pdus[pduCnt].pdu.pucch_pdu.preDftOccLen, mBuf);
            CMCHKPK(oduUnpackUInt8, &fapiMsgBody.pdus[pduCnt].pdu.pucch_pdu.addDmrsFlag, mBuf);
            CMCHKPK(oduUnpackUInt16, &fapiMsgBody.pdus[pduCnt].pdu.pucch_pdu.dmrsScramblingId, mBuf);
            CMCHKPK(oduUnpackUInt8, &fapiMsgBody.pdus[pduCnt].pdu.pucch_pdu.dmrsCyclicShift, mBuf);
            CMCHKPK(oduUnpackUInt8, &fapiMsgBody.pdus[pduCnt].pdu.pucch_pdu.srFlag, mBuf);
            CMCHKPK(oduUnpackUInt16, &fapiMsgBody.pdus[pduCnt].pdu.pucch_pdu.bitLenHarq, mBuf);
            CMCHKPK(oduUnpackUInt8, &fapiMsgBody.pdus[pduCnt].pdu.pucch_pdu.pad4[0], mBuf);
            CMCHKPK(oduUnpackUInt8, &fapiMsgBody.pdus[pduCnt].pdu.pucch_pdu.pad4[1], mBuf);
            CMCHKPK(oduUnpackUInt16, &fapiMsgBody.pdus[pduCnt].pdu.pucch_pdu.bitLenCsiPart1, mBuf);
            CMCHKPK(oduUnpackUInt16, &fapiMsgBody.pdus[pduCnt].pdu.pucch_pdu.bitLenCsiPart2, mBuf);

            //Unpack fapi_ul_rx_bmform_pdu_t
            pnfUnpackUlRxBeamforming(&fapiMsgBody.pdus[pduCnt].pdu.pucch_pdu.beamforming, mBuf);

            break;
         }
         default:
         {
            DU_LOG("ERROR  --> NFAPI_VNF: Incorrect PduType:%d",fapiMsgBody.pdus[pduCnt].pduType);
            return;
         }
      }
   }

   pnfProcUlTtiReq(&fapiMsgBody);

   return ROK;
}
/*********************************************************************************
 *
 * @Function Name: pnfP7MsgHandler
 *
 *
 * @Functionality: 
 *    Handles the P7 MEssages at PNF SIM
 *
 * @Params [IN]: Message Buffer received at UDP NFAPI P7 Interface
 *
 * *******************************************************************************/
uint8_t  pnfP7MsgHandler(Buffer *mBuf)
{
   nFapi_p7_hdr p7Hdr;
   nFapi_msg_header msgHdr;
   uint8_t ret = ROK;

   nFapiExtractP7Hdr(&p7Hdr, mBuf);
   nFapiExtractMsgHdr(&msgHdr, mBuf);

   switch(msgHdr.msg_id)
   {
      case TAG_NFAPI_DL_NODE_SYNC:
      {
         DU_LOG("INFO   --> NFAPI_PNF: DL_NODE_SYNC recevied.");
         ret = pnfDlNodeSyncHandler(mBuf);
         break;
      }
      case FAPI_DL_TTI_REQUEST:
      {
         DU_LOG("\nINFO   --> NFAPI_PNF: DL_TTI_REQ recevied.");
         ret = pnfDlTtiReq(mBuf);
         break;
      }
      case FAPI_UL_TTI_REQUEST:
      {
         DU_LOG("\nINFO   --> NFAPI_PNF: UL_TTI_REQ recevied.");
         ret = pnfUlTtiReq(mBuf);
         break;
      }
      default:
      {
         DU_LOG("ERROR  --> NFAPI_PNF: Wrong MSGID of NFAPI P7 Message:%d",msgHdr.msg_id);
         ret = RFAILED;
         break;
      }
   }

   return ret;
}

/**********************************************************************
End of file
**********************************************************************/
