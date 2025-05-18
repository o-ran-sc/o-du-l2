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

/* This file contains all utility functions for MAC CL */

#include "common_def.h"
#include "lwr_mac.h"

 /*******************************************************************
  *
  * @brief Fetch cellCb from Hash list
  *
  * @details
  *
  *    Function : rgClUtlGetCellCb
  *
  *    Functionality:
  *       - Searches for a cell entry at MAC CL 
  *
  * @params[in] cell Id
  * @return Pointer to cellCb - success
  *         NULLP - failure
  *
  * ****************************************************************/
LwrMacCellCb * lwrMacGetCellCb
(
   uint16_t cellId
)
{
   LwrMacCellCb *cellCb;

   if(cellId >= MAX_NUM_CELL)
   {
      DU_LOG("\nERROR  -->  Invalid Cell Id [%d]. rgClUtlGetCellCb failed.", cellId);
      return NULLP;
   }
   
   cellCb = &lwrMacCb.cellCb[cellId -1];

   return cellCb;
}

/*******************************************************************
 *
 * @brief Reverses bits in a number
 *
 * @details
 *
 *    Function : reverseBits
 *
 *    Functionality:
 *      Reverses bits in a number
 *
 * @params[in] Number to be reversed
 *             Number of bits to be reversed
 * @return Reversed number
 *
 * ****************************************************************/
uint32_t reverseBits(uint32_t num, uint8_t numBits)
{
   uint32_t reverse_num = 0;
   uint8_t bitIdx;
   for (bitIdx = 0; bitIdx < numBits; bitIdx++)
   {
      if((num & (1 << bitIdx)))
	 reverse_num |= 1 << ((numBits - 1) - bitIdx);
   }
   return reverse_num;
}

/*******************************************************************
 *
 * @brief Fills DL DCI payload byte by byte
 *
 * @details
 *
 *    Function : fillDlDciPayload
 *
 *    Functionality:
 *      Fills DL DCI payload byte by byte
 *
 * @params[in] Payload buffer pointer
 *             Current Byte position in buffer
 *             Current Bit Position in current byte
 *             Value to be filled
 *             Number of bits in value
 * @return void
 *
 * ****************************************************************/

void fillDlDciPayload(uint8_t *buf, uint8_t *bytePos, uint8_t *bitPos,\
      uint32_t val, uint8_t valSize)
{
   uint8_t temp;
   uint8_t bytePart1;
   uint32_t bytePart2;
   uint8_t bytePart1Size;
   uint8_t bytePart2Size;


#ifndef OAI_TESTING
   uint8_t temp;
   if(*bitPos + valSize <= 8)
   {
      bytePart1 = (uint8_t)val;
      bytePart1 = (~((~0) << valSize)) & bytePart1;
      buf[*bytePos] |= bytePart1;
      *bitPos += valSize;
   }
   else if(*bitPos + valSize > 8)
   {
      temp = (uint8_t)val;
      bytePart1Size = 8 - *bitPos;
      bytePart2Size = valSize - bytePart1Size;

      bytePart1 = ((~((~0) << bytePart1Size)) & temp) << *bitPos;
      bytePart2 = val >> bytePart1Size;

      buf[*bytePos] |= bytePart1;
      (*bytePos)--;
      *bitPos = 0;
      fillDlDciPayload(buf, bytePos, bitPos, bytePart2, bytePart2Size);
   }
#else
   if(*bitPos + valSize <= 8)
   {
      bytePart1 = (uint8_t)val;
      bytePart1 = ((~((~0) << valSize)) & bytePart1)<< (8 - (*bitPos + valSize));
      buf[*bytePos] |= bytePart1;
      *bitPos += valSize;
   }
   else if(*bitPos + valSize > 8)
   {
      bytePart1Size = 8 - *bitPos;
      bytePart2Size = valSize - bytePart1Size;

      bytePart1 = val >> bytePart2Size;

      buf[*bytePos] |= bytePart1;
      (*bytePos)--;
      *bitPos = 0;
      fillDlDciPayload(buf, bytePos, bitPos, val, bytePart2Size);
   }
   
#endif

}

/*
 * As per FAPI spec, 
 * Frequency domain resources is a bitmap defining non-overlapping groups of 6 PRBs in ascending order.
 * [TS38.213 10.1]. Bitmap of uint8 array. 45 bits.
 *
 * As per IAPI,
 * CORESET-freqdom.frequencyDomainResources : The bits of the bitmap have a one-to-one mapping with
 * non-overlapping groups of 6 RBs. The most significant bit of the first word corresponds to
 * the most significant bit defined in 38.331.
 *
 * FAPI and IAPI both are 45 bits. Mapped from bit 0 LS Byte for the FAPI and
 * bit 0 LS U32 entry for IAPI.
 * FAPI is to be filled in following format such that Intel L1 is able to decode it :
 *
 *            FAPI                                 IAPI 
 * FreqDomainResource[0] bits 7-0     ->    nFreqDomain[0] bits 7-0
 * FreqDomainResource[1] bits 7-0     ->    nFreqDomain[0] bits 15-8
 * FreqDomainResource[2] bits 7-0     ->    nFreqDomain[0] bits 23-16
 * FreqDomainResource[3] bits 7-0     ->    nFreqDomain[0] bits 31-24
 * FreqDomainResource[4] bits 7-0     ->    nFreqDomain[1] bits 7-0
 * FreqDomainResource[5] bits 7-0     ->    nFreqDomain[1] bits 15-8
 *
 * where for the last entry bits 7,6 and 5 are don't care in the FAPI and bits
 * 31-13 are don't care in the IAPI.
 */
void convertFreqDomRsrcMapToIAPIFormat(uint8_t *sourceBitMap, uint8_t *destBitMap)
{
   int8_t  idx;
   uint8_t  numBitsToShift = 0;
   uint64_t freqDomainResources = 0;

   /* Bit operation to create a 64-bit integer that has
    * 48 LSBs [Bit 47 to Bit 0] mapped to sourceBitMap[0] to sourceBitMap[5]
    */
   for(idx = FREQ_DOM_RSRC_SIZE-1; idx >=0; idx--)
   {
      freqDomainResources |= ((uint64_t)sourceBitMap[idx] << numBitsToShift);
      numBitsToShift += 8;
   }

   /* Right shift 3 bits because bits[2-0] are unused in sourceBitMap[5] */
   freqDomainResources = freqDomainResources >> 3;

   /* Filling destBitMap such that LSB bit 0 of freqDomainResources maps to LSB 
    * of first word of destBitMap */
   numBitsToShift = 0;
   for(idx=0; idx<FREQ_DOM_RSRC_SIZE; idx++)
   {
      destBitMap[idx] = freqDomainResources >> numBitsToShift;
      numBitsToShift += 8;
   }
}

#ifdef NR_TDD
/**
 *@brief Returns Num of Slots in Current TDD periodicity
 *
 * @details
 * 
 * Function : calcNumSlotsInCurrPeriodicity 
 * 
 * This API returns numOfSlots In CurrPeriodicity as per numerology
 * 
 * @param[in] DlUlTxPeriodicity, numerology
 * @return  numSlotsInCurrPeriodicity 
 * **/

uint8_t calcNumSlotsInCurrPeriodicity(DlUlTxPeriodicity tddPeriod, uint8_t numerology)
{
   float periodicityInMsec = 0;
   
   switch(tddPeriod)
   {
      case TX_PRDCTY_MS_0P5:
      {
	 if(numerology <= 0)
	 {
	    DU_LOG("\nERROR  --> LWR_MAC_UTILS: numerology:%d cant be configured with 0.5ms periodicity",numerology);
	    return 0;
	 }
	 periodicityInMsec = 0.5;
         break;
      }
      case TX_PRDCTY_MS_0P625:
      {
	 if(numerology <= 2)
	 {
	    DU_LOG("\nERROR  --> LWR_MAC_UTILS: numerology:%d cant be configured with 0.625ms periodicity",numerology);
	    return 0;
	 }
         periodicityInMsec = 0.625;
         break;
      }
      case TX_PRDCTY_MS_1:
      {
         periodicityInMsec = 1;
         break;
      }
      case TX_PRDCTY_MS_1P25:
      {
	 if(numerology <= 1)
	 {
	    DU_LOG("\nERROR  --> LWR_MAC_UTILS: numerology:%d cant be configured with 1.25ms periodicity",numerology);
	    return 0;
	 }
         periodicityInMsec = 1.25;
         break;
      }
      case TX_PRDCTY_MS_2:
      {
         periodicityInMsec = 2;
         break;
      }
      case TX_PRDCTY_MS_2P5:
      {
	 if(numerology <= 0)
	 {
	    DU_LOG("\nERROR  --> LWR_MAC_UTILS: numerology:%d cant be configured with 2.5ms periodicity",numerology);
	    return 0;
	 }
         periodicityInMsec = 2.5;
         break;
      }
      case TX_PRDCTY_MS_5:
      {
         periodicityInMsec = 5;
         break;
      }
      case TX_PRDCTY_MS_10:
      {
         periodicityInMsec = 10;
         break;
      }
      default:
      {
	 DU_LOG("\nERROR  -->  LWR_MAC_UTILS : Invalid DlUlTxPeriodicity:%d", tddPeriod);
	 return 0;
      }
   }

   return (periodicityInMsec * pow(2,numerology));
}
#endif

#ifdef OAI_TESTING
uint16_t reverseBytes16(uint16_t num) {
    return (num >> 8) | (num << 8);
}

uint32_t reverseBytes32(uint32_t num) {
    return ((num >> 24) & 0x000000FF) |
           ((num >> 8) & 0x0000FF00) |
           ((num << 8) & 0x00FF0000) |
           ((num << 24) & 0xFF000000);
}

uint8_t get_tlv_padding(uint16_t tlv_length)
{
   DU_LOG("\nDEBUG  -->  LWR_MAC: get_tlv_padding tlv_length %lu = padding = %d\n",\
                tlv_length, ((4 - (tlv_length % 4)) % 4));
   return (4 - (tlv_length % 4)) % 4;
}

#endif

/**********************************************************************
  End of file
 **********************************************************************/
