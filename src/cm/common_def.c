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
 *     Function: fillCoresetFeqDomAllocMap
 *     
 *     This function does allocation in frequency domain resource.
 *     This is a bitmap defining  non-overlapping groups of 6 PRBs in ascending order.
 *     
 *  @param[in]  startPrb - start PRB from where the freq alloc starts.  
 *  @param[in]  prbSize - number of PRBs to be allocted.
 *  @param[in]  freqDomain - 6 bytes of info, each bit represents a group of 6 PRB.
 *  @return   void
 **/
void fillCoresetFeqDomAllocMap(uint16_t startPrbGrp, uint16_t numPrbGrp, uint8_t *freqDomain)
{
   uint8_t  idx;
   uint8_t  prbGrpStartBit = 0;
   uint8_t  numBitsToRightShift = 0;
   uint64_t mask = 0;
   uint64_t freqAllocBitMap = 0;

   /* 
    * Frequency allocation bit string is 45 bits long.
    * Hence using 6 bytes (i.e. 48 bits) to represent it.
    * Each bit corresponds to a group of 6 RBs.
    */
   prbGrpStartBit = 47; 
   while(numPrbGrp)
   {
      mask = 1;
      printf("startPrbGrp [%d] numPrbGrp [%d] diff [%d]\n",startPrbGrp, numPrbGrp,  (prbGrpStartBit - startPrbGrp));
      mask = mask << (prbGrpStartBit - startPrbGrp);
      freqAllocBitMap = freqAllocBitMap | mask;
      startPrbGrp++;
      numPrbGrp--;
   }

   /* Copying 48 LSBs from 64-bit integer to the 45 MSBS in 6-byte array 
    * The first (left-most / most significant) bit corresponds to the first RB
    * group in the BWP, and so on
    */
   mask = 0x0000FF0000000000;
   numBitsToRightShift = 40;
   for(idx=0; idx<FREQ_DOM_RSRC_SIZE; idx++)
   {
      freqDomain[idx] = (freqAllocBitMap & mask) >> numBitsToRightShift;
      numBitsToRightShift -= 8;
      mask = mask >> 8;
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
   uint16_t idx = 0, revIdx = 0, temp = 0, copyLen = 0;

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

/*******************************************************************
 *
 * @brief Builds PLMN ID 
 *
 * @details
 *
 *    Function : plmnBuildId
 *
 *    Functionality: Building the PLMN ID
 *
 * @params[in] PLMNID plmn
 * @params[out] PLMNID in string format
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t buildPlmnId(Plmn plmn, uint8_t *buf)
{
   uint8_t mncCnt;
   mncCnt = 2;
   buf[0] = ((plmn.mcc[1] << 4) | (plmn.mcc[0]));
   if(mncCnt == 2)
   {
      buf[1]  = ((0xf0) | (plmn.mcc[2]));
      buf[2] = ((plmn.mnc[1] << 4) | (plmn.mnc[0]));
   }
   else
   {
      buf[1] = ((plmn.mnc[0] << 4) | (plmn.mcc[2]));
      buf[2] = ((plmn.mnc[2] << 4) | (plmn.mnc[1]));
   }
   return ROK;
}

/*******************************************************************
*
* @brief  SGetSBuf with debug logs
*
* @details
*
*    Function : SGetSBufNewForDebug
*
*    Functionality: SGetSBuf with debug logs
*
* @params[in] file name, fun name, region, pool, data ptr, size
*
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/
uint8_t SGetSBufNewForDebug(char *file, char *func, char *line, Region region, Pool pool, Data **ptr, Size size)
{
   if(SGetSBuf(region, pool, ptr, size) == ROK)
   {
#ifdef ODU_MEMORY_DEBUG_LOG
      printf("\nCM_ALLOC=== SGetSBufNewForDebug %s +%d, %s, %d, %p\n",\
         file, line, func, size, *ptr);
#endif
      return ROK;
   }
   else
      return RFAILED;
}

/*******************************************************************
*
* @brief  SPutSBuf with debug logs
*
* @details
*
*    Function : SPutSBufNewForDebug
*
*    Functionality: SPutSBuf with debug logs
*
* @params[in] file name, fun name, region, pool, data ptr, size
*
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/
uint8_t SPutSBufNewForDebug(char *file, char *func, char *line, Region region, Pool pool, Data *ptr, Size size)
{
   if(SPutSBuf(region, pool, ptr, size) == ROK)
   {
#ifdef ODU_MEMORY_DEBUG_LOG
      printf("\nCM_FREE=== SPutSBufNewForDebug %s +%d, %s, %d, %p\n",\
         file, line, func, size, ptr);
#endif
      return ROK;
   }
   else
      return RFAILED;
}


/*******************************************************************
*
* @brief  SGetStaticBuf with debug logs
*
* @details
*
*    Function : SGetStaticBufNewForDebug
*
*    Functionality: SGetStaticBuf with debug logs
*
* @params[in] file name, fun name, region, pool, data ptr, size, memType
*
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/
uint8_t SGetStaticBufNewForDebug(char *file, char *func, char *line, \
Region region, Pool pool, Data **ptr, Size size, uint8_t memType)
{
   if(SGetStaticBuffer(region, pool, ptr, size, memType) == ROK)
   {
#ifdef ODU_MEMORY_DEBUG_LOG
      printf("\nCM_ALLOC=== SGetStaticBufNewForDebug %s +%d, %s, %d, %p\n",\
         file, line, func, size, *ptr);
#endif
      return ROK;
   }
   else
      return RFAILED;
}

/*******************************************************************
*
* @brief  SPutStaticBuf with debug logs
*
* @details
*
*    Function : SPutStaticBufNewForDebug 
*
*    Functionality: SPutStaticBuf with debug logs
*
* @params[in] file name, fun name, region, pool, data ptr, size, memType
*
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/
uint8_t SPutStaticBufNewForDebug(char *file, char *func, char *line, \
Region region, Pool pool, Data *ptr, Size size, uint8_t memType)
{
   if(SPutStaticBuffer(region, pool, ptr, size, memType) == ROK)
   {
#ifdef ODU_MEMORY_DEBUG_LOG
      printf("\nCM_FREE=== SPutStaticBufNewForDebug %s +%d, %s, %d, %p\n",\
         file, line, func, size, ptr);
#endif
      return ROK;
   }
   else
      return RFAILED;
}
/**********************************************************************
         End of file
**********************************************************************/
