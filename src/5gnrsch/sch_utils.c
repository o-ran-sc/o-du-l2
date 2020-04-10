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

/************************************************************************
 
     Name:     sch_utils.c
  
     Type:     C source file
  
     Desc:     C source code for scheduler utilities
  
     File:     sch_utils.c
  
**********************************************************************/

/** @file sch_utils.c
@brief This file implements the schedulers util functions.
*/

/* header files */
#include "sch_utils.h"

/* spec-38.213 Table 13-1 */
int8_t coresetIdxTable[MAX_CORESET_INDEX][4] = {
{   1,   24,   2,   0}, /* index 0  */
{   1,   24,   2,   2}, /* index 1  */
{   1,   24,   2,   4}, /* index 2  */
{   1,   24,   3,   0}, /* index 3  */
{   1,   24,   3,   2}, /* index 4  */
{   1,   24,   3,   4}, /* index 5  */
{   1,   48,   1,  12}, /* index 6  */
{   1,   48,   1,  16}, /* index 7  */
{   1,   48,   2,  12}, /* index 8  */
{   1,   48,   2,  16}, /* index 9  */
{   1,   48,   3,  12}, /* index 10 */
{   1,   48,   3,  16}, /* index 11 */
{   1,   96,   1,  38}, /* index 12 */
{   1,   96,   2,  38}, /* index 13 */
{   1,   96,   3,  38}, /* index 14 */
{   0,    0,   0,   0}, /* index 15 */
};

/* spec-38.213 Table 13-11 */
/* m value is scaled to 2, when using it in formula, divide by 2 */
/* firstSymbol will vary depends on i, hence not filled */
int8_t searchSpaceIdxTable[MAX_SEARCH_SPACE_INDEX][4] = {
{   0,    1,   2,   0}, /* index 0  */
{   0,    2,   1,   0}, /* index 1  */
{   2,    1,   2,   0}, /* index 2  */
{   2,    2,   1,   0}, /* index 3  */
{   5,    1,   2,   0}, /* index 4  */
{   5,    2,   1,   0}, /* index 5  */
{   7,    1,   2,   0}, /* index 6  */
{   7,    2,   1,   0}, /* index 7  */
{   0,    1,   4,   0}, /* index 8  */
{   5,    1,   4,   0}, /* index 9  */
{   0,    1,   2,   0}, /* index 10 */
{   0,    1,   2,   0}, /* index 11 */
{   2,    1,   2,   0}, /* index 12 */
{   2,    1,   2,   0}, /* index 13 */
{   5,    1,   2,   0}, /* index 14 */
{   5,    1,   2,   0}, /* index 15 */
};
/**
 * @brief frequency domain allocation function. 
 *
 * @details
 *
 *     Function : freqDomResourceAlloc
 *     
 *     This function does allocation in frequency domain resource. using 
 *     bitwise operator, the bits are set for the PRBs.
 *     
 *  @param[in]  startPrb - start PRB from where the freq alloc starts.  
 *  @param[in]  prbSize - number of PRBs to be allocted.
 *  @param[in]  freqDomain - 6 bytes of info, each bit represents a group of 6 PRB.
 *  @return   void
 **/
void freqDomResourceAlloc(uint16_t startPrb, uint16_t prbSize, uint8_t *freqDomain)
{
   uint8_t remBits = prbSize/6; /* each bit represents 6 PRBs */
   uint8_t firstByte = 1;
   uint8_t numBits,startBit,byteCount = 0;

   while(remBits)
   {
      /* when the startPrb is not in this byteCount */
      if(startPrb/8)
      {
         startPrb -= 8;
         byteCount++;
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
		SET_BITS((startBit % 8),numBits,freqDomain[byteCount])
      firstByte = 0;

      /* the ramaining bits should be subtracted with the numBits set in this byte */
      remBits -= numBits;
      byteCount++;
   }
}

/**********************************************************************
         End of file
**********************************************************************/
