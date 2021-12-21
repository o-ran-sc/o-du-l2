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


/* header include files -- defines (.h) */
#include "common_def.h"
#include "lrg.h"           /* Layer manager interface includes*/
#include "lrg.x"           /* layer management typedefs for MAC */
#include "du_app_mac_inf.h"
#include "mac_sch_interface.h"
#include "lwr_mac_upr_inf.h"
#include "mac.h"
#include "mac_utils.h"

/*******************************************************************
 *
 * @brief pack the bits
 *
 * @details
 *
 *    Function : packBytes
 *
 *    Functionality:
 *     pack the bits in the corresponding byte
 *
 * @params[in] buffer pointer, byte and bit position, value and its size
 * @return void
 *
 * ****************************************************************/
void packBytes(uint8_t *buf, uint16_t *bytePos, uint8_t *bitPos, uint32_t val, uint8_t valSize)
{
   uint32_t  temp;
   uint8_t   bytePart1;
   uint32_t  bytePart2;
   uint8_t   bytePart1Size;
   uint32_t  bytePart2Size;

   if(*bitPos - valSize + 1 >= 0)
   {
      bytePart1 = (uint8_t)val;
      bytePart1 = (bytePart1 << (*bitPos -valSize +1));
      buf[*bytePos] |= bytePart1;
      if(*bitPos - valSize < 0)
      {
	 *bitPos = 7;
	 (*bytePos)++;
      }
      else
	 *bitPos -= valSize;
   }
   else
   {
      temp = 0;
      bytePart1Size = *bitPos +1;
      bytePart2Size = valSize - bytePart1Size;

      bytePart1 = (val >> bytePart2Size) << (*bitPos -bytePart1Size +1);
      bytePart2 =  (~((~temp) << bytePart2Size)) & val;

      buf[*bytePos] |= bytePart1;
      (*bytePos)++;
      *bitPos = 7;
      packBytes(buf, bytePos, bitPos, bytePart2, bytePart2Size);
   }  
}

/*******************************************************************
 *
 * @brief fill the RAR PDU
 *
 * @details
 *
 *    Function : fillRarPdu
 *
 *    Functionality:
 *     The RAR PDU will be MUXed and formed
 *
 * @params[in] RAR info
 * @return void
 *
 * ****************************************************************/
void fillRarPdu(RarInfo *rarInfo)
{
   uint8_t   *rarPdu = rarInfo->rarPdu;
   uint8_t   bitPos = 0;
   uint16_t  bytePos= 0, bwpSize = 0, rbStart = 0, rbLen = 0;
   uint8_t   numHopInfoBitsInFreqAlloc = 0;
   uint8_t   actualFreqRsrcAllocSize = 0;

   /* RAR subheader fields */
   uint8_t   EBit = 0;
   uint8_t   TBit = 0;

   /* RAR payload fields */
   uint8_t   RBit = 0;
   uint16_t  msg3FreqResource = 0;
   uint8_t   paddingLcid = 63;

   /* Considering 2 bytes of padding in RAR PDU. 
    * 1st byte is MAC sub-header for padding
    * 2nd byte contains padding bits
    */
   uint8_t   paddingSize = 8;

   /* Fill RAR pdu fields */
   EBit = 0;
   TBit = 1;
   RBit = 0;

   rarInfo->rarPduLen = RAR_PAYLOAD_SIZE;

   /* Initialize buffer */
   for(bytePos = 0; bytePos < rarInfo->rarPduLen; bytePos++)
      rarPdu[bytePos] = 0;

   bytePos = 0;
   bitPos = 7;

   /* Packing fields into RAR PDU */
   packBytes(rarPdu, &bytePos, &bitPos, EBit, E_BIT_SIZE); 
   packBytes(rarPdu, &bytePos, &bitPos, TBit, T_BIT_SIZE);
   packBytes(rarPdu, &bytePos, &bitPos, rarInfo->RAPID, RAPID_SIZE);
   packBytes(rarPdu, &bytePos, &bitPos, RBit, R_BIT_SIZE);
   packBytes(rarPdu, &bytePos, &bitPos, rarInfo->ta, TIMING_ADVANCE_SIZE);
 
   /* Packing MSG3 UL Grant in RAR */
   packBytes(rarPdu, &bytePos, &bitPos, rarInfo->ulGrant.freqHopFlag, FREQ_HOP_FLAG_SIZE);

   /* Calculating freq domain resource allocation field value
    * bwpSize = Size of BWP
    * RBStart = Starting Resource block
    * RBLen = length of contiguously allocted RBs
    * Spec 38.214 Sec 6.1.2.2.2
    */
   bwpSize = rarInfo->ulGrant.bwpSize;
   rbStart = rarInfo->ulGrant.msg3FreqAlloc.startPrb;
   rbLen = rarInfo->ulGrant.msg3FreqAlloc.numPrb;

   if((rbLen >=1) && (rbLen <= bwpSize - rbStart))
   {
      if((rbLen - 1) <= floor(bwpSize / 2))
         msg3FreqResource = (bwpSize * (rbLen-1)) + rbStart;
      else
         msg3FreqResource = (bwpSize * (bwpSize - rbLen + 1)) \
                            + (bwpSize - 1 - rbStart);
   }

   /* Calculating frequency domain resource allocation field size 
    * and packing frequency domain resource allocation accordingly 
    * Spec 38.213 Sec 8.3 
    */
   if(bwpSize < 180)
   {
      actualFreqRsrcAllocSize = ceil(log2(bwpSize * (bwpSize + 1) / 2));
      packBytes(rarPdu, &bytePos, &bitPos, 0, FREQ_RSRC_ALLOC_SIZE - actualFreqRsrcAllocSize);
      packBytes(rarPdu, &bytePos, &bitPos, msg3FreqResource, actualFreqRsrcAllocSize);
   }
   else
   {
      if(rarInfo->ulGrant.freqHopFlag == 0)
      {
         numHopInfoBitsInFreqAlloc = 1;
         packBytes(rarPdu, &bytePos, &bitPos, 0, numHopInfoBitsInFreqAlloc);
         
         actualFreqRsrcAllocSize = abs(log2(bwpSize * (bwpSize + 1) / 2));
         packBytes(rarPdu, &bytePos, &bitPos, 0, actualFreqRsrcAllocSize - FREQ_RSRC_ALLOC_SIZE);
         packBytes(rarPdu, &bytePos, &bitPos, msg3FreqResource, \
            actualFreqRsrcAllocSize - numHopInfoBitsInFreqAlloc);
      }
      else
      {
         /* TODO : If frequency hopping is supported,
          * Fetch the Number of bits to store hopping information in frequency
          * resource allocation field and the value to be filled from Spec 38.213, Table 8.3-1. 
          * Fill the frequency resource allocation field as described in Spec 38.213 sec 8.3
          */
      }
   }

   /* Packing time domain resource allocation for UL grant */
   packBytes(rarPdu, &bytePos, &bitPos, rarInfo->ulGrant.k2Index, TIME_RSRC_ALLOC_SIZE);

   packBytes(rarPdu, &bytePos, &bitPos, rarInfo->ulGrant.mcs, MCS_SIZE);
   packBytes(rarPdu, &bytePos, &bitPos, rarInfo->ulGrant.tpc, TPC_COMMAND_SIZE);
   packBytes(rarPdu, &bytePos, &bitPos, rarInfo->ulGrant.csiReq, CSI_REQUEST_SIZE);

   packBytes(rarPdu, &bytePos, &bitPos, rarInfo->tcrnti, T_CRNTI_SIZE);

   /* padding of 2 bytes */
   packBytes(rarPdu, &bytePos, &bitPos, RBit, R_BIT_SIZE*2);
   packBytes(rarPdu, &bytePos, &bitPos, paddingLcid, LC_ID_SIZE);
   packBytes(rarPdu, &bytePos, &bitPos, 0, paddingSize);

}

/*******************************************************************
 *
 * @brief Database required to form MAC PDU
 *
 * @details
 *
 *    Function : createMacRaCb
 *
 *    Functionality:
 *     stores the required params for muxing
 *
 * @params[in] Pointer to cellId,
 *                        crnti
 * @return void
 *
 * ****************************************************************/
void createMacRaCb(RachIndInfo *rachIndInfo)
{
   uint8_t  ueId = 0, ueIdx = 0;
   uint16_t crnti = 0;
   uint16_t cellIdx = 0;

   GET_CELL_IDX(rachIndInfo->cellId, cellIdx);
   
   crnti = getNewCrnti(&macCb.macCell[cellIdx]->crntiMap);
   if(crnti == -1)
      return;

   GET_UE_ID(crnti, ueId);
   ueIdx = ueId -1;

   /* store in rach ind structure */
   rachIndInfo->crnti  = crnti;

   /* store in raCb */
   macCb.macCell[cellIdx]->macRaCb[ueIdx].cellId = rachIndInfo->cellId;
   macCb.macCell[cellIdx]->macRaCb[ueIdx].crnti  = crnti;
}

/*************************************************
 * @brief fill RLC DL Data
 *
 * @details
 *
 * Function : fillMsg4DlData
 *      This function sends Dl Data
 *      to form MAC SDUs
 *           
 * @param[in]  MacDlData *dlData
 *             msg4Pdu pointer
 ************************************************/

void fillMsg4DlData(MacDlData *dlData, uint16_t msg4PduLen, uint8_t *msg4Pdu)
{
   dlData->pduInfo[dlData->numPdu].lcId = MAC_LCID_CCCH;
   dlData->pduInfo[dlData->numPdu].pduLen = msg4PduLen;
   memcpy(dlData->pduInfo[dlData->numPdu].dlPdu, msg4Pdu, msg4PduLen);
   dlData->numPdu++;
}

/*************************************************
 * @brief fill Mac Ce Info
 *
 * @details
 *
 * Function : fillMacCe
 *      This function fills Mac ce identities
 *           
 * @param[in]  RlcMacData *dlData
 *             Msg3Pdu Data
 ************************************************/

void fillMacCe(MacCeInfo *macCeInfo, uint8_t *msg3Pdu)
{
   uint8_t idx;
   macCeInfo->numCes = 1;
   for(idx = 0; idx < macCeInfo->numCes; idx++)
   {
      macCeInfo->macCe[idx].macCeLcid = MAC_LCID_CRI;
      memcpy(macCeInfo->macCe[idx].macCeValue, \
	    msg3Pdu, MAX_CRI_SIZE);
   }
}

/*******************************************************************
 *
 * @brief Forms MAC PDU
 *
 * @details
 *
 *    Function : macMuxPdu
 *
 *    Functionality:
 *     The MAC PDU will be MUXed and formed
 *
 * @params[in] MacDlData *, MacCeInfo *, txPdu *, tbSize
 * @return void
 * ****************************************************************/

void macMuxPdu(MacDlData *dlData, MacCeInfo *macCeData, uint8_t *txPdu, uint16_t tbSize)
{
   uint16_t bytePos = 0;
   uint8_t bitPos = 7;
   uint8_t pduIdx = 0;
   uint8_t macPdu[tbSize];
   memset(macPdu, 0, (tbSize * sizeof(uint8_t)));

   /* subheader fields */
   uint8_t RBit = 0;              /* Reserved bit */
   uint8_t FBit =0;                  /* Format Indicator */
   uint8_t lcid =0;                  /* LCID */
   uint16_t lenField = 0;         /* Length field */

   /* subheader field size (in bits) */
   uint8_t RBitSize = 1;
   uint8_t FBitSize = 1;
   uint8_t lcidSize = 6;
   uint8_t lenFieldSize = 0;      /* 8-bit or 16-bit L field  */

   /* PACK ALL MAC CE */
   if(macCeData != NULLP)
   {
      for(pduIdx = 0; pduIdx < macCeData->numCes; pduIdx++)
      {
         lcid = macCeData->macCe[pduIdx].macCeLcid;
         switch(lcid)
         {
            case MAC_LCID_CRI:
               {
                  /* Packing fields into MAC PDU R/R/LCID */
                  packBytes(macPdu, &bytePos, &bitPos, RBit, (RBitSize * 2));
                  packBytes(macPdu, &bytePos, &bitPos, lcid, lcidSize);
                  memcpy(&macPdu[bytePos], macCeData->macCe[pduIdx].macCeValue,\
                        MAX_CRI_SIZE);
                  bytePos += MAX_CRI_SIZE;
                  break;
               }
            default:
               DU_LOG("\nERROR  -->  MAC: Invalid LCID %d in mac pdu",lcid);
               break;
         }
      }
   }

   /* PACK ALL MAC SDUs */
   for(pduIdx = 0; pduIdx < dlData->numPdu; pduIdx++)
   {
      lcid = dlData->pduInfo[pduIdx].lcId;
      switch(lcid)
      {
         case MAC_LCID_CCCH:
         case MAC_LCID_MIN ... MAC_LCID_MAX :
            {
               lenField = dlData->pduInfo[pduIdx].pduLen;
               if(dlData->pduInfo[pduIdx].pduLen > 255)
               {
                  FBit = 1;
                  lenFieldSize = 16;

               }
               else
               {
                  FBit = 0;
                  lenFieldSize = 8;
               }
               /* Packing fields into MAC PDU R/F/LCID/L */
               packBytes(macPdu, &bytePos, &bitPos, RBit, RBitSize);
               packBytes(macPdu, &bytePos, &bitPos, FBit, FBitSize);
               packBytes(macPdu, &bytePos, &bitPos, lcid, lcidSize);
               packBytes(macPdu, &bytePos, &bitPos, lenField, lenFieldSize);
               memcpy(&macPdu[bytePos], dlData->pduInfo[pduIdx].dlPdu, lenField);
               bytePos += lenField;
               break;
            }

         default:
            DU_LOG("\nERROR  -->  MAC: Invalid LCID %d in mac pdu",lcid);
            break;
      }
   }
   if(bytePos < tbSize && (tbSize-bytePos >= 1))
   {
      /* padding remaining bytes */
      RBitSize = 2;
      lcid = MAC_LCID_PADDING;
      packBytes(macPdu, &bytePos, &bitPos, RBit, RBitSize);
      packBytes(macPdu, &bytePos, &bitPos, lcid, lcidSize);
   }

   /*Storing the muxed pdu */
   if(txPdu != NULLP)
   {
      memcpy(txPdu, macPdu, tbSize);
   }
}

/**********************************************************************
  End of file
 **********************************************************************/
