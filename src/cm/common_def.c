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

/*Spec 38.104, Table 5.4.2.1-1 ARFCN - FREQ mapping*/
/*{  F_REF(Mhz), ΔF_Global, F_REF-Offs, N_REF-offs, Range of N_REF }*/
uint32_t arfcnFreqTable[3][5] = {
  {   3000,   5,         0,        0,   599999 }, /*index 0*/
  {  24250,  15,      3000,   600000,  2016666 }, /*index 1*/
  { 100000,  60,  24250.08,  2016667,  3279165 }, /*index 2*/
};

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
    * Frequency allocation bit string is 45 bits long. Hence using 6 bytes (i.e. 48 bits) to represent it.
    * Each bit corresponds to a group of 6 RBs.
    *
    * For example if a coreset includes PRB 24 to 47, then on dividing the PRBs into group of 6, 
    * startPrbGrp = 24/6 = 4
    * numPrbGrp = 24/6 = 4
    * 
    * Frequency allocation bit string is 48 bits long i.e. Bit 47...0
    * Here, Bit 47 represents RB group 0, Bit 46 represent RB group 45 and so on.
    * Since startPrbGrp = 4 and numPrbGrp = 4, it means RB group 4,5,6 and 7 are used in coreset.
    * i.e. Bits 43, 42, 42 and 40 are masked to 1 and rest all bits are 0 in bitstring    
    */
   prbGrpStartBit = 47; 
   while(numPrbGrp)
   {
      mask = 1;
      mask = mask << (prbGrpStartBit - startPrbGrp);
      freqAllocBitMap = freqAllocBitMap | mask;
      startPrbGrp++;
      numPrbGrp--;
   }

   /* Copying 48 LSBs from 64-bit integer to the 45 MSBS in 6-byte array 
    * The first (left-most / most significant) bit corresponds to the first RB
    * group in the BWP, and so on
    */
   /* On right shifting freqAllocBitMap by 40 bits, the bits 47 to 40 of freqAllocBitMap 
    * will now become 8-LSB. Copying these 8-bits into freqDomain[].
    * Now shifting freqAllocBitMap by 32 bits, the bit 39 to 32 of freqAllocBitMap will
    * now become 8-LSB. Copying these 8-bits into next index of freqDomain.
    * and so on.
    */
   numBitsToRightShift = 40; 
   mask = 0x0000FF0000000000;
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
 * @brief Function to map Sub carrier spacing enum value to value in kHz
 *
 * @details
 *
 *    Function : convertScsEnumValToScsVal
 *
 *    Functionality:
 *       Function to map Sub carrier spacing enum value to value in kHz
 *
 * @params[in] sub-carrier spacing enum value
 * @return sub-carrier spacing value in kHz
 *
 * ****************************************************************/
uint16_t convertScsEnumValToScsVal(uint8_t scsEnumValue)
{
   switch(scsEnumValue)
   {
      case SCS_15KHZ:
      case SCS_30KHZ:
      case SCS_60KHZ:
      case SCS_120KHZ:
      case SCS_240KHZ:
         return (15 * pow(2,scsEnumValue));
      default:
         return 15;
   }
}

/*******************************************************************
 * @brief convert scs offset value into the enum value received from O1 
 *
 * @details
 *
 *    Function : convertScsValToScsEnum
 *
 *    Functionality:
 *       - convert scs periodicity value 
 *
 * @params[in] uint32_t num
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t convertScsValToScsEnum(uint32_t num)
{
   switch(num)
   {
      case 15:
         return SCS_15KHZ;

      case 30:
         return SCS_30KHZ;

      case 60:
         return SCS_60KHZ;

      case 120:
         return SCS_120KHZ;
      
      case 240:
         return SCS_240KHZ;
      
      default:
         return SCS_15KHZ;
   }
}

/*******************************************************************
 * @brief convert SSB periodicity value into the enum value received from O1 
 *
 * @details
 *
 *    Function : convertSSBPeriodicityToEnum
 *
 *    Functionality:
 *       - convert scs periodicity value 
 *
 * @params[in] uint32_t num
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t convertSSBPeriodicityToEnum(uint32_t num)
{
   switch(num)
   {
      case 5:
         return SSB_5MS;

      case 10:
         return SSB_10MS;

      case 20:
         return SSB_20MS;

      case 40:
         return SSB_40MS;

      case 80:
         return SSB_80MS;

      case 160:
         return SSB_160MS;

      default:
         return SSB_5MS;
   }
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
uint8_t SGetSBufNewForDebug(char *file, const char *func, int line, Region region, Pool pool, Data **ptr, Size size)
{
   if(SGetSBuf(region, pool, ptr, size) == ROK)
   {
#ifdef ODU_MEMORY_DEBUG_LOG
      if (strncmp(func,"cmInetRecvMsg",sizeof("cmInetRecvMsg")))
      {
         printf("\nCM,ALLOC,=== SGetSBufNewForDebug %s +%d, %s, %d, %p \n",\
         file, line, func, size, *ptr);
      }
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
uint8_t SPutSBufNewForDebug(char *file, const char *func, int line, Region region, Pool pool, Data *ptr, Size size)
{
   if(SPutSBuf(region, pool, ptr, size) == ROK)
   {
#ifdef ODU_MEMORY_DEBUG_LOG
      if (strncmp(func,"cmInetRecvMsg",sizeof("cmInetRecvMsg")))
      {
         printf("\nCM,FREE,=== SPutSBufNewForDebug %s +%d, %s, %d, %p \n",\
         file, line, func, size, ptr);
      }
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
uint8_t SGetStaticBufNewForDebug(char *file, const char *func, int line, \
Region region, Pool pool, Data **ptr, Size size, uint8_t memType)
{
   if(SGetStaticBuffer(region, pool, ptr, size, memType) == ROK)
   {
#ifdef ODU_MEMORY_DEBUG_LOG
      printf("\nCM,ALLOC,=== SGetStaticBufNewForDebug %s +%d, %s, %d, %p \n",\
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
uint8_t SPutStaticBufNewForDebug(char *file, const char *func, int line, \
Region region, Pool pool, Data *ptr, Size size, uint8_t memType)
{
   if(SPutStaticBuffer(region, pool, ptr, size, memType) == ROK)
   {
#ifdef ODU_MEMORY_DEBUG_LOG
      printf("\nCM,FREE,=== SPutStaticBufNewForDebug %s +%d, %s, %d, %p \n",\
         file, line, func, size, ptr);
#endif
      return ROK;
   }
   else
      return RFAILED;
}


/*******************************************************************
*
* @brief  countSetBits in an integer
*
* @details
*
*    Function : countSetBits 
*
*    Functionality: countSetBits in unsigned integer
*
* @params[in] uint32_t number/Bitmask
*
* @return [out] uint8_t count of Set Bits
*
* ****************************************************************/
uint8_t countSetBits(uint32_t num)
{
   uint8_t count = 0;

   while(num)
   {
      count += num & 1;
      num >>= 1;
   }
   return(count);
}

/*******************************************************************
*
* @brief  convert ARFCN to freq in kHZ
*
* @details
*
*    Function : convertArfcnToFreqKhz
*
*    Functionality: convert ARFCN to freq in kHZ as per Table below
*                  3GPP TS 38.104, Table 5.4.2.1-1
*       Formula: F_REF = F_REF-Offs + ΔF_Global (N_REF – N_REF-Offs)
*
* @params[in] uint32_t number
*
* @return [out] uint32_t Freq in kHZ
*
* ****************************************************************/
uint32_t convertArfcnToFreqKhz(uint32_t arfcn)
{
   uint8_t indexTable = 0;
   uint32_t freq = 0;

   for(indexTable = 0; indexTable < 3; indexTable++)
   {
      if(arfcn <= arfcnFreqTable[indexTable][4])
      {
         freq = (arfcnFreqTable[indexTable][2] * 1000) + (arfcnFreqTable[indexTable][1] * (arfcn - arfcnFreqTable[indexTable][3]));
         return (freq);
      }
   }
   DU_LOG("ERROR  -->  DUAPP: ARFCN vaid range is between 0 and 3279165");
   return (freq);
}


/*******************************************************************
*
* @brief  convert Freq(MHZ) to ARFCN
*
* @details
*
*    Function : convertFreqToArfcn
*
*    Functionality: convert freq to ARFCN as per Table below
*                  3GPP TS 38.104, Table 5.4.2.1-1
*       Formula: NREF = NREF-Offs +  (FREF – FREF-Offs) / ΔFGlobal
*
* @params[in] uint32_t Freq(kHZ)
*
* @return [out] uint32_t ARFCN(number)
*
* ****************************************************************/
uint32_t convertFreqToArfcn(uint32_t freq)
{
   uint8_t indexTable = 0;
   uint32_t arfcn = 0;

   for(indexTable = 0; indexTable < 3; indexTable++)
   {
      if(freq < (arfcnFreqTable[indexTable][0] * 1000))
      {
         arfcn = arfcnFreqTable[indexTable][3] + ((freq - (arfcnFreqTable[indexTable][2] * 1000)) / (arfcnFreqTable[indexTable][1]));
         return (arfcn);
      }
   }
   DU_LOG("ERROR  -->  DUAPP: FREQ vaid range is between 0 and 100000 MHz");
   return (arfcn);
}

/**********************************************************************
         End of file
**********************************************************************/
