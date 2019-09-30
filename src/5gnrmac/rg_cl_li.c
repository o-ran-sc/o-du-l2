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

/* This file contains handler for FAPI messages to be sent or received from PHY */

#include "envdep.h"
#include "gen.h"
#include "ssi.h"
#include "cm_hash.h"

#include "gen.x"
#include "ssi.x"
#include "cm_hash.x"

#include "lcl.h"
#include "rg_cl.h"
#include "rg_cl_phy.h"
/*#include "wls_lib.h"*/

EXTERN S16 sendToPhy ARGS((U16 msgType, U32 msgLen, void *msg));

/*******************************************************************
  *
  * @brief Fills FAPI message header 
  *
  * @details
  *
  *    Function : fillMsgHdr 
  *
  *    Functionality:
  *         -Fills FAPI message header
  *
  * @params[in] Pointer to header
  *             Number of messages
  *             Messae Type
  *             Length of message
  * @return void
  *
  * ****************************************************************/
PUBLIC void fillMsgHdr(MsgHdr *hdr, U8 nMsg, U16 msgType, U32 msgLen)
{
   hdr->nMsg = nMsg;
   hdr->msgType = msgType;
   hdr->msgLen = msgLen;
}

 /*******************************************************************
  *
  * @brief Fills TLV
  *
  * @details
  *
  *    Function : fillTlv
  *
  *    Functionality:
  *         -Fills TLV
  *
  * @params[in] Pointer to TLV
  *             Tag
  *             Length
  *             Value
  * @return Void
  *
  * ****************************************************************/
PUBLIC void fillTlv(L1L2Tlv *tlv, U16 tag, U16 len, U16 value)
{
   tlv->tag = tag;
   tlv->length = len;
   tlv->value = value;
}


 /*******************************************************************
  *
  * @brief Fills next config TLV
  *
  * @details
  *
  *    Function : fillConfigTLV
  *
  *    Functionality:
  *         - Finds pointer to next TLV in config req
  *         - Fills the TLV
  *         - Calculates Message length
  *
  * @params[in] 
  * @return ROK     - void
  *
  * ****************************************************************/
PUBLIC void fillConfigTLV(L1L2Tlv *nextTlv, U8 *configTlvs, U16 tag, U16 length, U16 value, U16 *msgLen)
{
   nextTlv = (L1L2Tlv *)(configTlvs + *msgLen);
   fillTlv(nextTlv, tag, length, value);
   *msgLen = *msgLen + sizeof(tag) + sizeof(length) + length;
}


 /*******************************************************************
  *
  * @brief Build and send FAPI config req to PHY
  *
  * @details
  *
  *    Function : rgClBldAndSndFAPICfgReq
  *
  *    Functionality:
  *         -Build and send FAPI config req to PHY
  *
  * @params[in] 
  * @return ROK     - success
  *         RFAILED - failure
  *
  * ****************************************************************/

PUBLIC S16 rgClBldAndSndFAPICfgReq
(
ClCellCb   *cellCb
)
{
   U16            msgLen;
   U32            FAPIConfigReqSize;
   U8             *configTlvs;
   L1L2ConfigReq  *FAPIConfigReq;
   L1L2Tlv        *nextTlv;
   ClCellCfg      cellCfg;

   FAPIConfigReqSize = sizeof(L1L2ConfigReq)
                       + (cellCb->cellCfg.numTlv * sizeof(L1L2Tlv));

   /* TO DO :Change SGetSBuf to SGetSBufWls() later */
   if(SGetSBuf(0,0,(Data **)&FAPIConfigReq, FAPIConfigReqSize) != ROK )
   {
      printf("\nMemory allocation failed for PHY Config Req");
      RETVALUE(RFAILED);
   }
  
   nextTlv = NULLP;
   msgLen = 0;
   cellCfg = cellCb->cellCfg;
   configTlvs = (U8 *)((U8 *)FAPIConfigReq + MSG_HDR_SIZE + 2);

   FAPIConfigReq->nTlv = cellCfg.numTlv;
   FAPIConfigReq->carrierId = cellCb->cellId;

   /* Filling cell configuration */
   fillConfigTLV(nextTlv, configTlvs, CFG_TAG_PHY_CELL_ID, sizeof(U16), cellCfg.phyCellId, &msgLen);
   fillConfigTLV(nextTlv, configTlvs, CFG_TAG_FRAME_DUP_TYPE, sizeof(U16), cellCfg.dupType, &msgLen);
   
   /* Filling DL carrier configuration */
   if(cellCfg.dlCarrCfg.pres)
   {
      fillConfigTLV(nextTlv, configTlvs, CFG_TAG_DL_BW, sizeof(U16), cellCfg.dlCarrCfg.bw, &msgLen);
      fillConfigTLV(nextTlv, configTlvs, CFG_TAG_DL_FREQ, sizeof(U16), cellCfg.dlCarrCfg.freq, &msgLen);
      fillConfigTLV(nextTlv, configTlvs, CFG_TAG_DLK0, sizeof(U16),cellCfg.dlCarrCfg.k0[0], &msgLen);
      fillConfigTLV(nextTlv, configTlvs, CFG_TAG_DL_GRID_SIZE, sizeof(U16),cellCfg.dlCarrCfg.gridSize[0], &msgLen);
      fillConfigTLV(nextTlv, configTlvs, CFG_TAG_NUM_TX_ANT, sizeof(U16), cellCfg.dlCarrCfg.numAnt, &msgLen);
   }

   /* Filling UL carrier configuration */
   if(cellCfg.ulCarrCfg.pres)
   {
      fillConfigTLV(nextTlv, configTlvs, CFG_TAG_UL_BW, sizeof(U16), cellCfg.ulCarrCfg.bw, &msgLen);
      fillConfigTLV(nextTlv, configTlvs, CFG_TAG_UL_FREQ, sizeof(U16), cellCfg.ulCarrCfg.freq, &msgLen);
      fillConfigTLV(nextTlv, configTlvs, CFG_TAG_ULK0, sizeof(U16), cellCfg.ulCarrCfg.k0[0], &msgLen);
      fillConfigTLV(nextTlv, configTlvs, CFG_TAG_UL_GRID_SIZE, sizeof(U16), cellCfg.ulCarrCfg.gridSize[0], &msgLen);
      fillConfigTLV(nextTlv, configTlvs, CFG_TAG_NUM_RX_ANT, sizeof(U16), cellCfg.ulCarrCfg.numAnt, &msgLen);
   }

   fillConfigTLV(nextTlv, configTlvs, CFG_TAG_FREQ_SHIFT_7P5KHZ, sizeof(U16), cellCfg.freqShft, &msgLen);

   /* Filling SSB configuration */
   fillConfigTLV(nextTlv, configTlvs, CFG_TAG_SS_PBCH_PWR, sizeof(U16), cellCfg.ssbCfg.ssbPbchPwr, &msgLen);
   fillConfigTLV(nextTlv, configTlvs, CFG_TAG_BCH_PAYLOAD, sizeof(U16), cellCfg.ssbCfg.bchPayload, &msgLen);
   fillConfigTLV(nextTlv, configTlvs, CFG_TAG_SCS_COMM, sizeof(U16), cellCfg.ssbCfg.scsCmn, &msgLen);
   fillConfigTLV(nextTlv, configTlvs, CFG_TAG_SSB_OFFS_POINT_A, sizeof(U16), cellCfg.ssbCfg.ssbPrbOffset, &msgLen);
   fillConfigTLV(nextTlv, configTlvs, CFG_TAG_BETA_PSS, sizeof(U16), cellCfg.ssbCfg.betaPss, &msgLen);
   fillConfigTLV(nextTlv, configTlvs, CFG_TAG_SSB_PERIOD, sizeof(U16), cellCfg.ssbCfg.ssbPeriod, &msgLen);
   fillConfigTLV(nextTlv, configTlvs, CFG_TAG_SSB_SUBC_OFFS, sizeof(U16), cellCfg.ssbCfg.ssbSubcOffset, &msgLen);
   fillConfigTLV(nextTlv, configTlvs, CFG_TAG_MIB, sizeof(U16), cellCfg.ssbCfg.mibPdu, &msgLen);
   fillConfigTLV(nextTlv, configTlvs, CFG_TAG_SSB_MASK, sizeof(U16), cellCfg.ssbCfg.nSSBMask[0], &msgLen);
   fillConfigTLV(nextTlv, configTlvs, CFG_TAG_SS_PBCH_MULT_CARR_BAND, sizeof(U16), cellCfg.ssbCfg.multCarrBand, &msgLen);
   fillConfigTLV(nextTlv, configTlvs, CFG_TAG_MULT_CELL_SS_PBCH_CARR, sizeof(U16), cellCfg.ssbCfg.multCellCarr, &msgLen);

   /* Filling PRACH configuration */
   fillConfigTLV(nextTlv, configTlvs, CFG_TAG_PRACH_SEQ_LEN, sizeof(U16), cellCfg.prachCfg.prachSeqLen, &msgLen);
   fillConfigTLV(nextTlv, configTlvs, CFG_TAG_PRACH_SUBC_SPAC, sizeof(U16), cellCfg.prachCfg.prachSubcSpacing, &msgLen);
   fillConfigTLV(nextTlv, configTlvs, CFG_TAG_RES_SET_CFG, sizeof(U16), cellCfg.prachCfg.prachRstSetCfg, &msgLen);
   fillConfigTLV(nextTlv, configTlvs, CFG_TAG_NUM_PRACH_FD_OCC, sizeof(U16), cellCfg.prachCfg.prachFdm, &msgLen);
   fillConfigTLV(nextTlv, configTlvs, CFG_TAG_PRACH_ROOT_SEQ_ID, sizeof(U16), cellCfg.prachCfg.fdm[0].rootSeqIdx, &msgLen);
   fillConfigTLV(nextTlv, configTlvs, CFG_TAG_NUM_ROOT_SEQ, sizeof(U16), cellCfg.prachCfg.fdm[0].numRootSeq, &msgLen);
   fillConfigTLV(nextTlv, configTlvs, CFG_TAG_K1, sizeof(U16), cellCfg.prachCfg.fdm[0].k1, &msgLen);
   fillConfigTLV(nextTlv, configTlvs, CFG_TAG_PRACH_ZERO_CORR_CFG, sizeof(U16), cellCfg.prachCfg.fdm[0].zeroCorrZoneCfg, &msgLen);
   fillConfigTLV(nextTlv, configTlvs, CFG_TAG_NUM_UNUSED_ROOT_SEQ, sizeof(U16), cellCfg.prachCfg.fdm[0].numUnusedRootSeq, &msgLen);
   fillConfigTLV(nextTlv, configTlvs, CFG_TAG_UNUSED_ROOT_SEQ, sizeof(U16), *(cellCfg.prachCfg.fdm[0].unsuedRootSeq), &msgLen);
   fillConfigTLV(nextTlv, configTlvs, CFG_TAG_SSB_PER_RACH, sizeof(U16), cellCfg.prachCfg.ssbPerRach, &msgLen);
   fillConfigTLV(nextTlv, configTlvs, CFG_TAG_PRACH_MULT_CARR_BAND, sizeof(U16), cellCfg.prachCfg.prachMultCarrBand, &msgLen);

   /* Filling TDD configuration */
   fillConfigTLV(nextTlv, configTlvs, CFG_TAG_TDD_PERIOD, sizeof(U16), cellCfg.tddCfg.tddPeriod, &msgLen);
   fillConfigTLV(nextTlv, configTlvs, CFG_TAG_SLOT_CFG, sizeof(U16), cellCfg.tddCfg.slotCfg[0][0], &msgLen);

   /* Filling RSSI measurement configuration */
   fillConfigTLV(nextTlv, configTlvs, CFG_TAG_RSSI_MEAS, sizeof(U16), cellCfg.rssiUnit, &msgLen);
  
   msgLen = msgLen + sizeof(L1L2ConfigReq);

   /* Filling message header */
   fillMsgHdr(&FAPIConfigReq->hdr, 1, MSG_TYPE_CONFIG_REQ, msgLen);

   /* Sending msg to PHY */
   sendToPhy(MSG_TYPE_CONFIG_REQ, FAPIConfigReqSize, (void *)FAPIConfigReq);

   RETVALUE(ROK);
}


/*******************************************************************
 *
 * @brief Handles FAPI config response
 *
 * @details
 *
 *    Function : rgClHndlCfgReq
 *
 *    Functionality:
 *         -Handles FAPI config request
 *
 * @params[in] Message pointer
 *             
 * @return void
 *
 *****************************************************************/

S16 rgClHndlCfgReq(void *msg)
{
   ClCellCb   *cellCb = NULLP;
   L1L2ConfigRsp *configRsp;

   configRsp = (L1L2ConfigRsp *)msg;

   if(configRsp->status != MSG_OK)
   {
      printf("\nPHY configuration failed");
      RETVALUE(RFAILED);
   }
   
   if((cellCb = (ClCellCb *)rgClUtlGetCellCb(configRsp->carrierId)) != NULLP)
   {
       printf("\nCell Id[%d] not found", configRsp->carrierId);
       RETVALUE(RFAILED);
   }
  
   cellCb->phyState = PHY_STATE_CONFIGURED;
   clGlobalCp.phyState = PHY_STATE_CONFIGURED;

   printf("\nReceived successful PHY configuration response");

   SPutSBuf(0, 0, (Data *)msg, configRsp->hdr.msgLen );
   
   RETVALUE(ROK);
}

/**********************************************************************
         End of file
**********************************************************************/
