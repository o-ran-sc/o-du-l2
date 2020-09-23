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

#include "common_def.h"

/**
 * @brief frequency domain allocation function. 
 *
 * @details
 *
 *     Function: freqDomRscAllocType0
 *     
 *     This function does allocation in frequency domain resource.
 *     This is a bitmap defining  non-overlapping groups of 6 PRBs in ascending order.
 *     
 *  @param[in]  startPrb - start PRB from where the freq alloc starts.  
 *  @param[in]  prbSize - number of PRBs to be allocted.
 *  @param[in]  freqDomain - 6 bytes of info, each bit represents a group of 6 PRB.
 *  @return   void
 **/
void freqDomRscAllocType0(uint16_t startPrb, uint16_t prbSize, uint8_t *freqDomain)
{
   uint8_t remBits = prbSize; /* each bit represents 6 PRBs */
   uint8_t firstByte = 1;
   uint8_t numBits,startBit,byteCount = 5;

   while(remBits)
   {
      /* when the startPrb is not in this byteCount */
      if(startPrb/8)
      {
         startPrb -= 8;
         byteCount--;
         continue;
      }

      /* max bytecount is 6 nearly equal to 45 bits*/
      if(byteCount >= 6)
          break;

      /* when we are filling the second byte, then the start should be equal to 0 */
      if(firstByte)
         startBit = startPrb;
      else
         startBit = 0;

      /* calculate the number of bits to be set in this byte */
      if((remBits+startPrb) <= 8)
         numBits = remBits;
      else
         numBits = 8 - startBit;

      /* bit operation to set the bits */
		SET_BITS_MSB((startBit % 8),numBits,freqDomain[byteCount])
      firstByte = 0;

      /* the ramaining bits should be subtracted with the numBits set in this byte */
      remBits -= numBits;
      byteCount--;
   }
}

/*******************************************************************
 *
 * @brief Reverse and copy fixed buffer to mBuf 
 *
 * @details
 *
 *    Function : oduCpyFixBufToMsg
 *
 *    Functionality: Reverse and copy fixed buffer to mBuf
 *
 * @params[in] Fixed buffer, msg buffer, length of message
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
void oduCpyFixBufToMsg(uint8_t *fixBuf, Buffer *mBuf, uint16_t len)                            
{
   uint8_t idx, revIdx, temp;
   uint16_t copyLen;

   /* ODU_COPY_FIX_BUF_TO_MSG copies fixed buffer in reverse order. \
    * Hence reversing the fixed buffer before copying in order to \
    * maintain the actual order*/
   for(idx = 0, revIdx = len-1; idx < len/2; idx++, revIdx--)   
   {                                                            
        temp = fixBuf[idx];                                          
        fixBuf[idx] = fixBuf[revIdx];                                   
        fixBuf[revIdx] = temp;                                       
   }                                                            
   ODU_COPY_FIX_BUF_TO_MSG(fixBuf, mBuf, 0, len, (MsgLen *)&copyLen);
}

uint8_t getDrbLcId()
{
   static uint8_t macLcId = MIN_DRB_LCID;

   if(macLcId >= MIN_DRB_LCID && macLcId <  MAX_DRB_LCID)
   {
      macLcId = macLcId % MAX_DRB_LCID;
   }
   else
   {
      macLcId = MIN_DRB_LCID;
   }
   return macLcId++;
}

/*******************************************************************
 *
 * @brief Configures the DU Parameters
 *
 * @details
 *
 *    Function : calcSliv
 *
 *    Functionality:
 *       - calculate SLIV value from start and length field
 *
 * @params[in] start symbol
 * @params[in] length of symbols
 * @return SLIV value
 *
 * ****************************************************************/
uint16_t calcSliv(uint8_t startSymbol, uint8_t lengthSymbol)
{
   uint16_t sliv = 0;
   if((lengthSymbol-1) <= 7)
   {
      sliv = NUM_SYMBOLS_PER_SLOT * (lengthSymbol-1) + startSymbol;
   }
   else
   {
      sliv = NUM_SYMBOLS_PER_SLOT * (NUM_SYMBOLS_PER_SLOT - lengthSymbol + 1) \
	     + (NUM_SYMBOLS_PER_SLOT - 1 - startSymbol);
   }
   return sliv;
}
/**********************************************************************
         End of file
**********************************************************************/
