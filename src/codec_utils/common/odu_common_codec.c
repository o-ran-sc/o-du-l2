#include "common_def.h"
#include "OCTET_STRING.h"
#include "BIT_STRING.h"
#include "asn_codecs.h"
#include "odu_common_codec.h"

/*******************************************************************
 *
 * @brief Writes the encoded chunks into a buffer
 *
 * @details
 *
 *    Function : PrepFinalEncBuf
 *
 *    Functionality:Fills the encoded buffer
 *
 * @params[in] void *buffer,initial encoded data
 * @params[in] size_t size,size of buffer
 * @params[in] void *encodedBuf,final buffer
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
int PrepFinalEncBuf(const void *buffer, size_t size, void *encodedBuf)
{
   memcpy(encodedBuf + encBufSize, buffer, size);
   encBufSize += size;
   return 0;
} /* PrepFinalEncBuf */

/*******************************************************************
 *
 * @brief Fills the RicId
 *
 * @details
 *
 *    Function : FillRicId
 *
 *    Functionality: Fills the RicId
 *
 * @params[in] BIT_STRING_t *nbid,
 *             uint8_t unusedBits
 *             uint8_t byteSize
 *             uint8_t val
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t fillBitString(BIT_STRING_t *id, uint8_t unusedBits, uint8_t byteSize, uint8_t val)
{
   uint8_t tmp;
   if(id->buf == NULLP)
   {
      return RFAILED;
   }

   memset(id->buf, 0, byteSize-1);
   id->buf[byteSize-1]   = val;
   id->bits_unused = unusedBits;
   return ROK;
}

/*******************************************************************
 *
 * @brief Converts bit strings to integer
 *
 * @details
 *
 *    Function : bitStringToInt
 *
 *    Functionality:
 *      - Converts ASN bit string format IEs to integer type
 *
 * @params[in] void
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t bitStringToInt(BIT_STRING_t *bitString, void *value)
{
   uint16_t idx;
   uint32_t *val = NULLP;

   if(bitString->buf == NULL || bitString->size <= 0)
   {
      DU_LOG("\nDU_APP : Bit string is empty");
      return RFAILED;
   }

   if(value)
      val = (uint32_t *)value;
   else
      return RFAILED;

   for(idx=0; idx< bitString->size-1; idx++)
   {
      *val |= bitString->buf[idx];
      *val <<= 8;
   }
   *val |= bitString->buf[idx];
   *val >>= bitString->bits_unused;

   return ROK;
}

/*******************************************************************
 *
 * @brief Function to decode teId value from the octect String
 *
 * @details
 *
 *    Function : teIdStringToInt
 *
 *    Functionality: Function to decode teId value from the octect string
 *                   It can used as generic function to convert 
 *                   octect string to uint32_t value 
 *
 * @params[in]  buf, value
 * @return void
 *
 * ****************************************************************/
void teIdStringToInt(uint8_t *buf, uint32_t *val)
{
   uint32_t temp1 = 0, temp2 = 0, temp3 = 0;

   temp1 |= buf[0];
   temp1 <<= 24;

   temp2 |= buf[1];
   temp2 <<= 16;

   temp3 |= buf[2];
   temp3 <<= 8;
   
   *val = temp1|temp2|temp3|buf[3];
}

/*******************************************************************
 *
 * @brief Function to encode teId value to the octect String
 *
 * @details
 *
 *    Function : fillTeIdString
 *
 *    Functionality: Function to encode teId value to the octect String
 *                   It can used as generic function to encode 
 *                   uint32_t value to octect string
 *
 * @params[in]  bufSize, value, buf
 * @return void
 *
 * ****************************************************************/

void fillTeIdString(uint8_t bufSize, uint32_t val, uint8_t *buf)
{
   uint8_t bitPos;

   for(bitPos = 0; bitPos < TEID_BIT_SIZE; bitPos += 8, bufSize--)
   {
      /*extracting bitBits from the bitPos*/
      buf[bufSize] = (((1 << 8) - 1) & (val >> (bitPos))); 
   }
}

/**********************************************************************
  End of file
 **********************************************************************/
