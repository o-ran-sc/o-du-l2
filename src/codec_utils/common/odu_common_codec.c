#include "odu_common_codec.h"

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
S16 buildPlmnId(Plmn plmn, OCTET_STRING_t *octe)
{
   U8 mncCnt;
   mncCnt = 2;
   octe->buf[0] = ((plmn.mcc[1] << 4) | (plmn.mcc[0]));
   if(mncCnt == 2)
   {
      octe->buf[1]  = ((0xf0) | (plmn.mcc[2]));
      octe->buf[2] = ((plmn.mnc[1] << 4) | (plmn.mnc[0]));
   }
   else
   {
      octe->buf[1] = ((plmn.mnc[0] << 4) | (plmn.mcc[2]));
      octe->buf[2] = ((plmn.mnc[2] << 4) | (plmn.mnc[1]));
   }
   RETVALUE(ROK);
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
 *             U8 unusedBits
 *             U8 byteSize
 *             U8 val
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

S16 fillBitString(BIT_STRING_t *id, U8 unusedBits, U8 byteSize, U8 val)
{
   U8 tmp;
   if(id->buf == NULLP)
   {
      RETVALUE(RFAILED);
   }

   for (tmp = 0 ; tmp < (byteSize-1); tmp++)
   {
      id->buf[tmp] = tmp;
   }
   id->buf[byteSize-1]   = val;
   id->bits_unused = unusedBits;
   RETVALUE(ROK);
}


/**********************************************************************
  End of file
 **********************************************************************/
