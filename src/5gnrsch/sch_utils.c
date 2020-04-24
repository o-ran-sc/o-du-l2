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

/* RACH tables */

/* spec-38.211 Table 6.3.3.2-1 */
/* Lra, delFRa, delF, numRb, k' */
uint16_t numRbForPrachTable[MAX_RACH_NUM_RB_IDX][5] = {
{   839,  1.25,  15,   6,   7    }, /* index 0 */
{   839,  1.25,  30,   3,   1    }, /* index 1 */
{   839,  1.25,  60,   2,   133  }, /* index 2 */
{   839,  5,     15,   24,  12   }, /* index 3 */
{   839,  5,     30,   12,  10   }, /* index 4 */
{   839,  5,     60,   6,   7    }, /* index 5 */
{   139,  15,    15,   12,  2    }, /* index 6 */
{   139,  15,    30,   6,   2    }, /* index 7 */
{   139,  15,    60,   3,   2    }, /* index 8 */
{   139,  30,    15,   24,  2    }, /* index 9 */
{   139,  30,    30,   12,  2    }, /* index 10 */
{   139,  30,    60,   6,   2    }, /* index 11 */
{   139,  60,    60,   12,  2    }, /* index 12 */
{   139,  60,    120,  6,   2    }, /* index 13 */
{   139,  120,   60,   24,  2    }, /* index 14 */
{   139,  120,   120,  12,  2    }, /* index 15 */
};

/* prach config index Table 6.3.3.2-2 spec 38.211 
 * PRACH format given as follows:
 * 0 = 0
 * 1 = 1
 * 2 = 2
 * 3 = 3
 * 4 = A1
 * 5 = A2
 * 6 = A3
 * 7 = B1
 * 8 = B4
 * 9 = C0
 * 10 = C2
 * 11 = A1/B1
 * 12 = A2/B2
 * 13 = A3/B3 
 * Subframe num is represented considering 0-9 bits and
 * value 1 corresponds to the subframe that is valid 
 */
 //Sphoorthi TODO: complete this table

uint16_t prachCfgIdxTable[MAX_PRACH_CONFIG_IDX][8] = {
{ 0, 16,  1,    2,  0,  0,  0,  0 }, /* index 0   */
{ 0, 16,  1,   16,  0,  0,  0,  0 }, /* index 1   */
{ 0, 16,  1,  128,  0,  0,  0,  0 }, /* index 2   */
{ 0, 16,  1,  512,  0,  0,  0,  0 }, /* index 3   */
{ 0,  8,  1,    2,  0,  0,  0,  0 }, /* index 4   */
{ 0,  8,  1,   16,  0,  0,  0,  0 }, /* index 5   */
{ 0,  8,  1,  128,  0,  0,  0,  0 }, /* index 6   */
{ 0,  8,  1,  512,  0,  0,  0,  0 }, /* index 7   */
{ 0,  4,  1,    2,  0,  0,  0,  0 }, /* index 8   */
{ 0,  4,  1,   16,  0,  0,  0,  0 }, /* index 9   */
{ 0,  4,  1,  128,  0,  0,  0,  0 }, /* index 10  */
{ 0,  4,  1,  512,  0,  0,  0,  0 }, /* index 11  */
{ 0,  2,  1,    2,  0,  0,  0,  0 }, /* index 12  */
{ 0,  2,  1,   16,  0,  0,  0,  0 }, /* index 13  */
{ 0,  2,  1,  128,  0,  0,  0,  0 }, /* index 14  */
{ 0,  2,  1,  512,  0,  0,  0,  0 }, /* index 15  */
{ 0,  1,  0,    2,  0,  0,  0,  0 }, /* index 16  */
{ 0,  1,  0,   16,  0,  0,  0,  0 }, /* index 17  */
{ 0,  1,  0,  128,  0,  0,  0,  0 }, /* index 18  */
{ 0,  1,  0,   66,  0,  0,  0,  0 }, /* index 19  */
{ 0,  1,  0,  132,  0,  0,  0,  0 }, /* index 20  */
{ 0,  1,  0,  264,  0,  0,  0,  0 }, /* index 21  */
{ 0,  1,  0,  146,  0,  0,  0,  0 }, /* index 22  */
{ 0,  1,  0,  292,  0,  0,  0,  0 }, /* index 23  */
{ 0,  1,  0,  584,  0,  0,  0,  0 }, /* index 24  */
{ 0,  1,  0,  341,  0,  0,  0,  0 }, /* index 25  */
{ 0,  1,  0,  682,  0,  0,  0,  0 }, /* index 26  */
{ 0,  1,  0, 1023,  0,  0,  0,  0 }, /* index 27  */
{ 1, 16,  1,    2,  0,  0,  0,  0 }, /* index 28  */
{ 1, 16,  1,   16,  0,  0,  0,  0 }, /* index 29  */
{ 1, 16,  1,  128,  0,  0,  0,  0 }, /* index 30  */
{ 1, 16,  1,  512,  0,  0,  0,  0 }, /* index 31  */
{ 1,  8,  1,    2,  0,  0,  0,  0 }, /* index 32  */
{ 1,  8,  1,   16,  0,  0,  0,  0 }, /* index 33  */
{ 1,  8,  1,  128,  0,  0,  0,  0 }, /* index 34  */
{ 1,  8,  1,  512,  0,  0,  0,  0 }, /* index 35  */
{ 1,  4,  1,    2,  0,  0,  0,  0 }, /* index 36  */
{ 1,  4,  1,   16,  0,  0,  0,  0 }, /* index 37  */
{ 1,  4,  1,  128,  0,  0,  0,  0 }, /* index 38  */
{ 1,  4,  1,  512,  0,  0,  0,  0 }, /* index 39  */
{ 1,  2,  1,    2,  0,  0,  0,  0 }, /* index 40  */
{ 1,  2,  1,   16,  0,  0,  0,  0 }, /* index 41  */
{ 1,  2,  1,  128,  0,  0,  0,  0 }, /* index 42  */
{ 1,  2,  1,  512,  0,  0,  0,  0 }, /* index 43  */
{ 1,  1,  0,    2,  0,  0,  0,  0 }, /* index 44  */
{ 1,  1,  0,   16,  0,  0,  0,  0 }, /* index 45  */
{ 1,  1,  0,  128,  0,  0,  0,  0 }, /* index 46  */
{ 1,  1,  0,   64,  0,  0,  0,  0 }, /* index 47  */
{ 1,  1,  0,  132,  0,  0,  0,  0 }, /* index 48  */
{ 1,  1,  0,  264,  0,  0,  0,  0 }, /* index 49  */
{ 1,  1,  0,  146,  0,  0,  0,  0 }, /* index 50  */
{ 1,  1,  0,  292,  0,  0,  0,  0 }, /* index 51  */
{ 1,  1,  0,  584,  0,  0,  0,  0 }, /* index 52  */
{ 2, 16,  1,    2,  0,  0,  0,  0 }, /* index 53  */
{ 2,  8,  1,    2,  0,  0,  0,  0 }, /* index 54  */
{ 2,  4,  0,    2,  0,  0,  0,  0 }, /* index 55  */
{ 2,  2,  0,    2,  0,  0,  0,  0 }, /* index 56  */
{ 2,  2,  0,   32,  0,  0,  0,  0 }, /* index 57  */
{ 2,  1,  0,    2,  0,  0,  0,  0 }, /* index 58  */
{ 2,  1,  0,   32,  0,  0,  0,  0 }, /* index 59  */
{ 3, 16,  1,    2,  0,  0,  0,  0 }, /* index 60  */
{ 3, 16,  1,   16,  0,  0,  0,  0 }, /* index 61  */
{ 3, 16,  1,  128,  0,  0,  0,  0 }, /* index 62  */
{ 3, 16,  1,  512,  0,  0,  0,  0 }, /* index 63  */
{ 3,  8,  1,    2,  0,  0,  0,  0 }, /* index 64  */
{ 3,  8,  1,   16,  0,  0,  0,  0 }, /* index 65  */
{ 3,  8,  1,  128,  0,  0,  0,  0 }, /* index 66  */
{ 3,  4,  1,    2,  0,  0,  0,  0 }, /* index 67  */
{ 3,  4,  1,   16,  0,  0,  0,  0 }, /* index 68  */
{ 3,  4,  1,  128,  0,  0,  0,  0 }, /* index 69  */
{ 3,  4,  1,  512,  0,  0,  0,  0 }, /* index 70  */
{ 3,  2,  1,    2,  0,  0,  0,  0 }, /* index 71  */
{ 3,  2,  1,   16,  0,  0,  0,  0 }, /* index 72  */
{ 3,  2,  1,  128,  0,  0,  0,  0 }, /* index 73  */
{ 3,  2,  1,  512,  0,  0,  0,  0 }, /* index 74  */
{ 3,  1,  0,    2,  0,  0,  0,  0 }, /* index 75  */
{ 3,  1,  0,   16,  0,  0,  0,  0 }, /* index 76  */
{ 3,  1,  0,  128,  0,  0,  0,  0 }, /* index 77  */
{ 3,  1,  0,   66,  0,  0,  0,  0 }, /* index 78  */
{ 3,  1,  0,  132,  0,  0,  0,  0 }, /* index 79  */
{ 3,  1,  0,  264,  0,  0,  0,  0 }, /* index 80  */
{ 3,  1,  0,  146,  0,  0,  0,  0 }, /* index 81  */
{ 3,  1,  0,  292,  0,  0,  0,  0 }, /* index 82  */
{ 3,  1,  0,  584,  0,  0,  0,  0 }, /* index 83  */
{ 3,  1,  0,  341,  0,  0,  0,  0 }, /* index 84  */
{ 3,  1,  0,  682,  0,  0,  0,  0 }, /* index 85  */
{ 3,  1,  0, 1023,  0,  0,  0,  0 }, /* index 86  */
{ 4, 16,  0,  528,  0,  1,  6,  2 }, /* index 87  */
{ 4, 16,  1,   16,  0,  2,  6,  2 }, /* index 88  */
{ 4,  8,  0,  528,  0,  1,  6,  2 }, /* index 89  */
{ 4,  8,  1,   16,  0,  2,  6,  2 }, /* index 90  */
{ 4,  4,  0,  528,  0,  1,  6,  2 }, /* index 91  */
{ 4,  4,  1,  528,  0,  1,  6,  2 }, /* index 92  */
{ 4,  4,  0,   16,  0,  2,  6,  2 }, /* index 93  */
{ 4,  2,  0,  528,  0,  1,  6,  2 }, /* index 94  */
{ 4,  2,  0,    2,  0,  2,  6,  2 }, /* index 95  */
{ 4,  2,  0,   16,  0,  2,  6,  2 }, /* index 96  */
{ 4,  2,  0,  128,  0,  2,  6,  2 }, /* index 97  */
{ 4,  1,  0,   16,  0,  1,  6,  2 }, /* index 98  */
{ 4,  1,  0,   66,  0,  1,  6,  2 }, /* index 99  */
{ 4,  1,  0,  528,  0,  1,  6,  2 }, /* index 100  */
{ 4,  1,  0,    2,  0,  2,  6,  2 }, /* index 101  */
{ 4,  1,  0,  128,  0,  2,  6,  2 }, /* index 102  */
{ 4,  1,  0,  132,  0,  2,  6,  2 }, /* index 103  */
{ 4,  1,  0,  146,  0,  2,  6,  2 }, /* index 104  */
{ 4,  1,  0,  341,  0,  2,  6,  2 }, /* index 105  */
{ 4,  1,  0, 1023,  0,  2,  6,  2 }, /* index 106  */
{ 4,  1,  0,  682,  0,  2,  6,  2 }, /* index 107  */
{11,  2,  0,  528,  0,  1,  7,  2 }, /* index 108  */
{11,  2,  0,   16,  0,  2,  7,  2 }, /* index 109  */
{11,  1,  0,   16,  0,  1,  7,  2 }, /* index 110  */
{11,  1,  0,   66,  0,  1,  7,  2 }, /* index 111  */
{11,  1,  0,  528,  0,  1,  7,  2 }, /* index 112  */
{11,  1,  0,    2,  0,  2,  7,  2 }, /* index 113  */
{11,  1,  0,  128,  0,  2,  7,  2 }, /* index 114  */
{11,  1,  0,  146,  0,  2,  7,  2 }, /* index 115  */
{11,  1,  0,  341,  0,  2,  7,  2 }, /* index 116  */
{ 5, 16,  1,  580,  0,  1,  3,  4 }, /* index 117  */
{ 5, 16,  1,   16,  0,  2,  3,  4 }, /* index 118  */
{ 5,  8,  1,  580,  0,  1,  3,  4 }, /* index 119  */
{ 5,  8,  1,   16,  0,  2,  3,  4 }, /* index 120  */
{ 5,  4,  0,  580,  0,  1,  3,  4 }, /* index 121  */
{ 5,  4,  0,   16,  0,  2,  3,  4 }, /* index 122  */
{ 5,  2,  1,  580,  0,  1,  3,  4 }, /* index 123  */
{ 5,  2,  0,    2,  0,  2,  3,  4 }, /* index 124  */
{ 5,  2,  0,   16,  0,  2,  3,  4 }, /* index 125  */
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
   uint8_t remBits = prbSize; /* each bit represents 6 PRBs */
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
