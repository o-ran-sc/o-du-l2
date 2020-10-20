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
 * @brief Builds PLMN ID 
 *
 * @details
 *
 *    Function : plmnBuildId
 *
 *    Functionality: Building the PLMN ID
 *
 * @params[in] PLMNID plmn
 *             OCTET_STRING_t *octe
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

   for (tmp = 0 ; tmp < (byteSize-1); tmp++)
   {
      id->buf[tmp] = tmp;
   }
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
uint8_t bitStringToInt(BIT_STRING_t *bitString, uint16_t *val)
{
   uint16_t idx;
   if(bitString->buf == NULL || bitString->size <= 0)
   {
      DU_LOG("\nDU_APP : Bit string is empty");
      return RFAILED;
   }

   for(idx=0; idx< bitString->size-1; idx++)
   {
      *val |= bitString->buf[idx];
      *val <<= 8;
    }
   *val |= bitString->buf[idx];
   *val >>= bitString->bits_unused;

   return ROK;
}



/**********************************************************************
  End of file
 **********************************************************************/
