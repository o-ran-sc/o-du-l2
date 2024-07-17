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
         CMCHKPK(oduUnpackUInt16, &ulBmform->rx_bfi[prgIdx].beamIdx[digBfIdx].beamidx, mBuf);
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

/*******************************************************************
 *
 * @brief Build and Send CRC Indication
 *
 * @details
 *
 *    Function : pnfBuildAndSendCrcInd
 *
 *    Functionality:
 *      Build and Send CRC Indication
 *
 * @params[in] Slot
 *             SFN 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint16_t pnfBuildAndSendCrcInd(uint16_t slot, uint16_t sfn, fapi_ul_pusch_pdu_t puschPdu)
{
   Buffer *mBuf       = NULLP;
   uint8_t numCrc = 0, ret = ROK;
   static uint8_t ind = 0;

   uint8_t result[]={0,//MSG3
                     0,//BSR
                     0,//MSG5 RRC Setup Complete
                     0,//Security Mode Complete
                     0,//Registraion Complete
                     0,//RRC Reconfiguration Complete
                     0,//UL DATA -1
                     0,//UL DATA -2
                     0,//UL DATA -3
                     0,//UL DATA -4
                     0,0,0,0,0,
                     0,0,0,0,0,
                     0,0,0,0,0,
                     0,0,0,0,0,
                     0,0,0,0,0,
                     0,0,0,0,0,
                     0,0,0,0,0,
                     0,0,0,0,0,
                     0,0,0,0,0,
                     0,0,0,0,0,
                     0,0,0,0,0,
                     0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

   if (ODU_GET_MSG_BUF(PNF_APP_MEM_REG, PNF_POOL, &mBuf) != ROK)
   {
      DU_LOG("ERROR  --> NFAPI_PNF : Memory allocation failed in pnfBuildAndSendCrcInd()");
      return RFAILED;
   }
   nfapiFillP7Hdr(mBuf, (sizeof(fapi_crc_ind_msg_body) + sizeof(nFapi_msg_header)), 0, 0);
   nfapiFillMsgHdr(mBuf, 1, FAPI_CRC_INDICATION, sizeof(fapi_crc_ind_msg_body));

   //Fill fapi_crc_ind_msg_body
   CMCHKPK(oduPackPostUInt16, sfn, mBuf);
   CMCHKPK(oduPackPostUInt16, slot, mBuf);
   CMCHKPK(oduPackPostUInt16, 1, mBuf); //numCrcs

    numCrc = 1;
   //Fill fapi_crc_ind_info_t
   while(numCrc != 0)
   {
      CMCHKPK(oduPackPostUInt32, puschPdu.handle, mBuf); //handle
      CMCHKPK(oduPackPostUInt16, puschPdu.rnti, mBuf); //rnti
      CMCHKPK(oduPackPostUInt8, puschPdu.puschData.harqProcessId, mBuf); //rnti
      CMCHKPK(oduPackPostUInt8, 0, mBuf); //tbCrcStatus
      CMCHKPK(oduPackPostUInt8, 0, mBuf); //ul_cqi
      CMCHKPK(oduPackPostUInt8, 0, mBuf); //pad
      CMCHKPK(oduPackPostUInt16, 1, mBuf); //numCb
      CMCHKPK(oduPackPostUInt16, 0, mBuf); //timingAdvance
      CMCHKPK(oduPackPostUInt16, 0, mBuf); //rssi

      //cbCrcStatus[0]
      CMCHKPK(oduPackPostUInt8, result[ind % sizeof(result)], mBuf);

      ret = (result[ind%sizeof(result)] == 0) ? ROK:RFAILED;
      ind++;
      numCrc--;
   }
   /* Sending CRC indication to VNF */
   DU_LOG("INFO   -->  NFAPI_PNF: Sending CRC Indication to VNF");
   if(pnfP7UdpSendMsg(mBuf) != ROK)
   { 
      DU_LOG("ERROR  -->  NFAPI_PNF: UDP Send Failed for CRC Indication to VNF");
      return RFAILED;
   }
   return ret;
}

/*******************************************************************
 *
 * @brief Build and send Rx data indication
 *
 * @details
 *
 *    Function : pnfBuildAndSendRxDataInd
 *
 *    Functionality:
 *       Build and send Rx data indication
 *
 * @params[in] SFN
 *             Slot
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint16_t pnfBuildAndSendRxDataInd(uint16_t slot, uint16_t sfn, fapi_ul_pusch_pdu_t puschPdu)
{
   uint8_t idx = 0, ueId = 0;
   fapi_rx_data_indication_t *rxDataInd =NULLP;
   fapi_pdu_ind_info_t       *pduInfo =NULLP;
   uint8_t  *pdu = NULLP;
   uint16_t byteIdx = 0;
   PnfMsgType type = 0;
   Buffer *mBuf = NULLP;

   GET_UE_ID(puschPdu.rnti, ueId);
   if(pnfCb.pnfUeCb[ueId-1].isCFRA)
   {
      if(!pnfCb.pnfUeCb[ueId-1].msgRrcReconfigComp)
      {
         /* In CF-RA in case of handover, RRC Reconfiguration Complete is sent
          * by UE once RAR is received from DU */
         pnfCb.pnfUeCb[ueId-1].ueId = ueId;
         pnfCb.pnfUeCb[ueId-1].crnti = puschPdu.rnti;
         pnfCb.pnfUeCb[ueId-1].msgRrcReconfigComp = true;
         type = MSG_TYPE_RRC_RECONFIG_COMPLETE;
      }
      else
         return ROK; 
   }
   else
   {
      if(!pnfCb.pnfUeCb[ueId-1].msg3Sent)
      {
         pnfCb.pnfUeCb[ueId-1].ueId = ueId;
         pnfCb.pnfUeCb[ueId-1].crnti = puschPdu.rnti;
         pnfCb.pnfUeCb[ueId-1].msg3Sent = true;
         type = MSG_TYPE_MSG3;
         sleep(1);
      }
      else if(!pnfCb.pnfUeCb[ueId-1].msg5ShortBsrSent)
      {
         pnfCb.pnfUeCb[ueId-1].msg5ShortBsrSent = true;
         type = MSG_TYPE_SHORT_BSR;
      }
      else if(!pnfCb.pnfUeCb[ueId-1].msg5Sent)
      {
         pnfCb.pnfUeCb[ueId-1].msg5Sent = true;
         type = MSG_TYPE_MSG5;
      }
      else if(!pnfCb.pnfUeCb[ueId-1].msgNasAuthenticationComp)
      {
        pnfCb.pnfUeCb[ueId-1].msgNasAuthenticationComp = true;
        type = MSG_TYPE_NAS_AUTHENTICATION_COMPLETE;
      }
      else if(!pnfCb.pnfUeCb[ueId-1].msgNasSecurityModeComp)
      {
         pnfCb.pnfUeCb[ueId-1].msgNasSecurityModeComp = true;
         type = MSG_TYPE_NAS_SECURITY_MODE_COMPLETE;
      }
      else if(!pnfCb.pnfUeCb[ueId-1].msgRrcSecurityModeComp)
      {
         pnfCb.pnfUeCb[ueId-1].msgRrcSecurityModeComp = true;
         type = MSG_TYPE_RRC_SECURITY_MODE_COMPLETE;
      }
      else if(!pnfCb.pnfUeCb[ueId-1].msgRegistrationComp)
      {
         pnfCb.pnfUeCb[ueId-1].msgRegistrationComp = true;
         type = MSG_TYPE_REGISTRATION_COMPLETE; 
      }
      else if(!pnfCb.pnfUeCb[ueId-1].msgRrcReconfigComp)
      {
         pnfCb.pnfUeCb[ueId-1].msgRrcReconfigComp = true;
         type = MSG_TYPE_RRC_RECONFIG_COMPLETE;
      }
      else
         return ROK;
   }

   if (ODU_GET_MSG_BUF(PNF_APP_MEM_REG, PNF_POOL, &mBuf) != ROK)
   {
      DU_LOG("ERROR  --> NFAPI_PNF : Memory allocation failed in pnfBuildAndSendRxDataInd");
      return RFAILED;
   }
   nfapiFillP7Hdr(mBuf, (sizeof(fapi_rx_data_ind_msg_body) + sizeof(nFapi_msg_header)), 0, 0);
   nfapiFillMsgHdr(mBuf, 1, FAPI_RX_DATA_INDICATION, (sizeof(fapi_rx_data_ind_msg_body) + puschPdu.puschData.tbSize));

   CMCHKPK(oduPackPostUInt16, sfn, mBuf);
   CMCHKPK(oduPackPostUInt16, slot, mBuf);
   CMCHKPK(oduPackPostUInt16, 1, mBuf);
   
   //Fill fapi_pdu_ind_info_t
   CMCHKPK(oduPackPostUInt32, puschPdu.handle, mBuf); 
   CMCHKPK(oduPackPostUInt16, puschPdu.rnti, mBuf); 
   CMCHKPK(oduPackPostUInt8, puschPdu.puschData.harqProcessId, mBuf); 
   CMCHKPK(oduPackPostUInt8, 0, mBuf); //ul_cqi
   CMCHKPK(oduPackPostUInt16, 0, mBuf); //timingAdvance
   CMCHKPK(oduPackPostUInt16, 0, mBuf); //rssi
   CMCHKPK(oduPackPostUInt16, puschPdu.puschData.tbSize, mBuf);//pdu_length
   CMCHKPK(oduPackPostUInt8, 0, mBuf);//pad[0]
   CMCHKPK(oduPackPostUInt8, 0, mBuf);//pad[1]

   switch(type)
   {
      case MSG_TYPE_MSG3: 
         {
            DU_LOG("DEBUG  -->  NFAPI_PNF: Forming MSG3 PDU ");
            /* For Initial RRC setup Request,
               MAC subheader format is R/R/LCId (1byte)
               LCId is CCCH(0)
               From 38.321 section 6.1.1
               */
            CMCHKPK(oduPackPostUInt8, 0, mBuf); //LCID = CCCH
            byteIdx++;
            /* Hardcoding MAC PDU */
            CMCHKPK(oduPackPostUInt8, 16, mBuf);
            byteIdx++;
            CMCHKPK(oduPackPostUInt8, 0, mBuf);
            byteIdx++;
            CMCHKPK(oduPackPostUInt8, 0, mBuf);
            byteIdx++;
            CMCHKPK(oduPackPostUInt8, 0, mBuf);
            byteIdx++;
            CMCHKPK(oduPackPostUInt8, 0, mBuf);
            byteIdx++;
            CMCHKPK(oduPackPostUInt8, 103, mBuf);
            byteIdx++;
            break;
         }
#if 0
      case MSG_TYPE_SHORT_BSR:
         {
            DU_LOG("DEBUG  -->  NFAPI_PNF: Forming SHORT BSR PDU ");
            uint8_t lcgId = 1;
            uint8_t bufferSizeIdx = 6;

            /* For Short BSR
               MAC subheader format is R/R/LcId (1Byte)
               LCId is 61
               From 38.321 section 6.1.1
               */
            CMCHKPK(oduPackPostUInt8, 61, mBuf); //LCID = 61
            byteIdx++;
            CMCHKPK(oduPackPostUInt8, (lcgId << 5) | bufferSizeIdx, mBuf); //LCG_ID and BufferSize
            byteIdx++;

            break;
         }

      case MSG_TYPE_MSG5:
      {
         /* For RRC setup complete
          *
          * MAC subheader format is R/F/LCId/L (2/3 bytes)
          * LCId is 1 for SRB1
          * L is length of PDU i.e 6bytes here 
          * From 38.321 section 6.1.1
          *
          * RLC subheader for AM PDU is D/C/P/SI/SN (2 bytes for 12-bit SN)
          * From 38.322, section 6.2.2.4
          */
         DU_LOG("DEBUG  -->  PHY_STUB: Forming MSG5 PDU");
         uint8_t  msg5PduLen = 33; /* Length of MSG5 */
         msg5PduLen += 2; /* RLC subheader */
         uint8_t msg5[] = {1, msg5PduLen, 128, pnfCb.pnfUeCb[ueId-1].rlcSnForSrb1++, 0, pnfCb.pnfUeCb[ueId-1].pdcpSn++, 16, 0, \
            5, 223, 128, 16, 94, 64, 3, 64, 68, 252, 97, 0, 0, 0, 0, 4, 0, 0, 4, 68, 11, 128, 184, 56, 0, 0, 0, 0, 0};

         msg5PduLen += 2;  /* 2 bytes of MAC header */
         memcpy(pdu, &msg5, msg5PduLen);
         byteIdx += msg5PduLen; /* 4 bytes of header : MAC+RLC */
         break;
      }

      case MSG_TYPE_NAS_AUTHENTICATION_COMPLETE:
      {
        /* For Authentication response where RRC Container is dummy
          *
          * MAC subheader format is R/F/LCId/L (2/3 bytes)
          * LCId is 1 for SRB1
          * L is length of PDU i.e 6bytes here 
          * From 38.321 section 6.1.1
          *
          * RLC subheader for AM PDU is D/C/P/SI/SN (2 bytes for 12-bit SN)
          * From 38.322, section 6.2.2.4
          */
         DU_LOG("DEBUG  -->  PHY_STUB: Forming AUTHENTICATION RESPONSE PDU");
         uint8_t  pduLen = 37; /* Length of PDU */
         pduLen += 2; /* RLC subheader */
         uint8_t msg[] = {1, pduLen, 128, pnfCb.pnfUeCb[ueId-1].rlcSnForSrb1++, 0, pnfCb.pnfUeCb[ueId-1].pdcpSn++, 0x3a, \
                          0x0e, 0x3f, 0x00, 0xca, 0x95, 0xe9, 0x19, 0x41, 0x3f, 0x00, 0x2b, 0x96, 0x88, 0x06, 0xd7, 0x16, 0xc6, \
                          0x8b, 0xea, 0xae, 0x45, 0xd1, 0x01, 0xfd, 0x34, 0xd4, 0xfd, 0xd5, 0x71, 0x00, 0x00, 0x00, 0x00, 0x00};

         pduLen += 2;  /* 2 bytes of MAC header */
         memcpy(pdu, &msg, pduLen);
         byteIdx += pduLen; /* 4 bytes of header : MAC+RLC */
         break;
      }
      
      case MSG_TYPE_NAS_SECURITY_MODE_COMPLETE:
      {
        /* For NAS security mode complete where RRC Container is dummy
          *
          * MAC subheader format is R/F/LCId/L (2/3 bytes)
          * LCId is 1 for SRB1
          * L is length of PDU i.e 6bytes here 
          * From 38.321 section 6.1.1
          *
          * RLC subheader for AM PDU is D/C/P/SI/SN (2 bytes for 12-bit SN)
          * From 38.322, section 6.2.2.4
          */
         DU_LOG("DEBUG  -->  PHY_STUB: Forming NAS SECURITY MODE COMPLETE PDU");
         uint8_t  pduLen = 93; /* Length of PDU */
         pduLen += 2; /* RLC subheader */
         uint8_t msg[] = {1, pduLen, 128, pnfCb.pnfUeCb[ueId-1].rlcSnForSrb1++, 0, pnfCb.pnfUeCb[ueId-1].pdcpSn++, 0x3a, 0x2a, 0x3f, 
                          0x02, 0x75, 0xa0, 0xa0, 0xc0, 0x80, 0x3f, 0x00, 0x2f, 0x3b, 0x80, 0x04, 0x9a, 0xa2, 0x81, 0x09, 0x80, 0xc0, 
                          0x28, 0x04, 0xf8, 0xb8, 0x80, 0x1d, 0xbf, 0x00, 0x20, 0x8c, 0x80, 0x05, 0xf9, 0x00, 0x78, 0x88, 0x7a, 0x88, 
                          0xd9, 0x00, 0x00, 0x00, 0x03, 0x08, 0x00, 0x81, 0x97, 0x02, 0x78, 0x38, 0x78, 0x38, 0x17, 0x82, 0x82, 0x00, 
                          0x80, 0x00, 0x00, 0xa9, 0x00, 0x78, 0x88, 0x00, 0x00, 0x00, 0x8b, 0x83, 0xf8, 0x38, 0x60, 0x20, 0x0c, 0xc0, 
                          0x50, 0x0c, 0x00, 0x80, 0x3a, 0x00, 0x00, 0x48, 0x29, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00};

         pduLen += 2;  /* 2 bytes of MAC header */
         memcpy(pdu, &msg, pduLen);
         byteIdx += pduLen; /* 4 bytes of header : MAC+RLC */
         break;
      }

      case MSG_TYPE_RRC_SECURITY_MODE_COMPLETE:
      {
         /* For security mode complete where RRC Container is dummy
          *
          * MAC subheader format is R/F/LCId/L (2/3 bytes)
          * LCId is 1 for SRB1
          * L is length of PDU i.e 6bytes here 
          * From 38.321 section 6.1.1
          *
          * RLC subheader for AM PDU is D/C/P/SI/SN (2 bytes for 12-bit SN)
          * From 38.322, section 6.2.2.4
          */
         DU_LOG("DEBUG  -->  PHY_STUB: Forming RRC SECURITY MODE COMPLETE PDU");
         uint8_t  pduLen = 12; /* Length of PDU */
         pduLen += 2; /* RLC subheader */
         uint8_t msg[] = {1, pduLen, 128, pnfCb.pnfUeCb[ueId-1].rlcSnForSrb1++, 0, pnfCb.pnfUeCb[ueId-1].pdcpSn++, 0x2a, 0x40, \
            0, 0, 0, 0, 0, 0, 0, 0};

         pduLen += 2;  /* 2 bytes of MAC header */
         memcpy(pdu, &msg, pduLen);
         byteIdx += pduLen; /* 4 bytes of header : MAC+RLC */
         break;
      }

      case MSG_TYPE_REGISTRATION_COMPLETE:
      {
         /* For rrc reconfig complete where RRC Container is dummy
          *
          * MAC subheader format is R/F/LCId/L (2/3 bytes)
          * LCId is 1 for SRB1
          * L is length of PDU i.e 6bytes here
          * From 38.321 section 6.1.1
          * 
          * RLC subheader for AM PDU is D/C/P/SI/SN (2 bytes for 12-bit SN)
          * From 38.322, section 6.2.2.4
          */
         DU_LOG("DEBUG  -->  PHY_STUB: Forming RRC REGISTRATION COMPLETE PDU");
         uint8_t  pduLen = 12; /* Length of PDU */
         pduLen += 2; /* RLC subheader */
         uint8_t msg[] = {1, pduLen, 128, pnfCb.pnfUeCb[ueId-1].rlcSnForSrb1++, 0, pnfCb.pnfUeCb[ueId-1].pdcpSn++, 0x3a, 0x81, \
            0xbf, 0, 0x21, 0x80, 0, 0, 0, 0};

         pduLen += 2;  /* 2 bytes of MAC header */
         memcpy(pdu, &msg, pduLen);
         byteIdx += pduLen; /* 4 bytes of header : MAC+RLC */
         break;
      }

      case MSG_TYPE_RRC_RECONFIG_COMPLETE:
      {
         /* For rrc reconfig complete where RRC Container is dummy
          *
          * MAC subheader format is R/F/LCId/L (2/3 bytes)
          * LCId is 1 for SRB1
          * L is length of PDU i.e 6bytes here
          * From 38.321 section 6.1.1
          *
          * RLC subheader for AM PDU is D/C/P/SI/SN (2 bytes for 12-bit SN)
          * From 38.322, section 6.2.2.4
          */
         DU_LOG("DEBUG  -->  PHY_STUB: Forming RRC RECONFIGURATION COMPLETE PDU");
         uint8_t  pduLen = 13; /* PDU length */
         pduLen += 2; /* RLC sub header */
         uint8_t msg[] = {1, pduLen, 128, pnfCb.pnfUeCb[ueId-1].rlcSnForSrb1++, 0, pnfCb.pnfUeCb[ueId-1].pdcpSn++, 8, 64, 0, 0,\
            0, 0, 0, 0, 0, 0, 0};

         pduLen += 2;  /* 2bytes of MAC header */
         memcpy(pdu, &msg, pduLen);
         byteIdx += pduLen; /* 4 bytes of header : MAC+RLC*/
         break;

      }
#endif
      default:
      break;
   } /* End of switch(type) */

   /* Filling MAC SDU for Padding bytes*/
   if(byteIdx < puschPdu.puschData.tbSize)
   {
      /* For Padding
         MAC subheader format is R/R/LCId (1byte)
         LCId is 63 for padding
         From 38.321 section 6.1.1
         */
      CMCHKPK(oduPackPostUInt8, 63, mBuf);
      byteIdx++;

      for(; byteIdx < puschPdu.puschData.tbSize; byteIdx++)
      {
         CMCHKPK(oduPackPostUInt8, 0, mBuf);
      }
   }

   /* Sending Rx data indication to MAC */
   DU_LOG("INFO   --> NFAPI_PNF: Sending Rx data Indication to VNF");

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
   uint8_t ret = ROK;

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
         if(ROK == pnfBuildAndSendCrcInd(pnfUlTtiReq->slot, pnfUlTtiReq->sfn, pnfUlTtiReq->pdus[numPdus-1].pdu.pusch_pdu))
         {
            if(pnfBuildAndSendRxDataInd(pnfUlTtiReq->slot, pnfUlTtiReq->sfn, pnfUlTtiReq->pdus[numPdus-1].pdu.pusch_pdu) != ROK)
            {
               DU_LOG("ERROR  --> NFAPI_PNF: Failure in Sending RX_DATA_IND");
               ret = RFAILED;
               break;
            }
         }
         else
         {
            DU_LOG("ERROR  --> NFAPI_PNF: Failure in CRC Ind thus PNF will not send RX_DATA_IND");
            ret = RFAILED;
            break;
         }
      }
      if(pnfUlTtiReq->pdus[numPdus-1].pduType == PUCCH_PDU_TYPE)
      {
         DU_LOG("INFO   --> NFAPI_PNF: PUCCH PDU");
         /*TODO: UCI_IND to be built and sent*/ 
      }
      numPdus--;
   }
   return ret;
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
             CMCHKPK(oduUnpackUInt16, &fapiMsgBody.pdus[pduCnt].pdu.prach_pdu.numCs, mBuf);
             pnfUnpackUlRxBeamforming(&fapiMsgBody.pdus[pduCnt].pdu.prach_pdu.beamforming, mBuf);
             break;
            break;
         }
         case PUSCH_PDU_TYPE:
         {
            CMCHKPK(oduUnpackUInt16, &fapiMsgBody.pdus[pduCnt].pdu.pusch_pdu.pduBitMap, mBuf);
            CMCHKPK(oduUnpackUInt16, &fapiMsgBody.pdus[pduCnt].pdu.pusch_pdu.rnti, mBuf);
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
            for(uint8_t portIdx = 0; portIdx < fapiMsgBody.pdus[pduCnt].pdu.pusch_pdu.puschPtrs.numPtrsPorts; portIdx++)
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
