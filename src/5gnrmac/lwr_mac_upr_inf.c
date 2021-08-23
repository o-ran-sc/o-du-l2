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
#include "lwr_mac_upr_inf.h"

/*******************************************************************
 *
 * @brief Packs and Sends CRC Ind to MAC
 *
 * @details
 *
 *    Function : packCrcInd
 *
 *    Functionality:
 *      Packs and Sends CRC Ind to MAC
 *
 * @params[in] Post structure pointer
 *             CRC indication
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t packCrcInd(Pst *pst, CrcInd *crcInd)
{
   if(pst->selector == ODU_SELECTOR_LWLC)
   {
      Buffer *mBuf = NULLP;
      if(ODU_GET_MSG_BUF(pst->region, pst->pool, &mBuf) != ROK)
      {
	 DU_LOG("\nERROR  --> LWR_MAC: Memory allocation failed in packCrcInd");
	 return RFAILED;
      }
      CMCHKPK(oduPackPointer,(PTR)crcInd, mBuf);
      return ODU_POST_TASK(pst, mBuf);
   }
   return RFAILED;
}

/**************************************************************************
 * @brief Function unpack crcInd
 *
 * @details
 *
 *      Function : unpackCrcInd
 *
 *      Functionality:
 *           unpacks crc indication
 *
 * @param[in] function pointer
 * @param[in] Pst     *pst, Post structure of the primitive.
 * @param[in] Buffer *mBuf
 * @return ROK     - success
 *         RFAILED - failure
 *
 ***************************************************************************/
uint8_t unpackCrcInd(CrcIndFunc func, Pst *pst, Buffer *mBuf)
{
   if(pst->selector == ODU_SELECTOR_LWLC)
   {
      CrcInd *crcInd = NULLP;

      /* unpack the address of the structure */
      CMCHKUNPK(oduUnpackPointer, (PTR *)&crcInd, mBuf);
      ODU_PUT_MSG_BUF(mBuf);
      return (*func)(pst, crcInd);
   }
   return RFAILED;
}


/*******************************************************************
 *
 * @brief Packs and Sends Rx Data Ind to MAC
 *
 * @details
 *
 *    Function : packRxDataInd
 *
 *    Functionality:
 *      Packs and Sends Rx data Ind to MAC
 *
 * @params[in] Post structure pointer
 *             Rx Data indication
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t packRxDataInd(Pst *pst, RxDataInd *rxDataInd)
{
   if(pst->selector == ODU_SELECTOR_LWLC)
   {
      Buffer *mBuf = NULLP;
      if(ODU_GET_MSG_BUF(pst->region, pst->pool, &mBuf) != ROK)
      {
	 DU_LOG("\nERROR  -->  LWR_MAC: Memory allocation failed in packRxDataInd");
	 return RFAILED;
      }
      CMCHKPK(oduPackPointer,(PTR)rxDataInd, mBuf);
      return ODU_POST_TASK(pst, mBuf);
   }
   return RFAILED;
}

/**************************************************************************
 * @brief Function unpack RxDataInd
 *
 * @details
 *
 *      Function : unpackRxDataInd
 *
 *      Functionality:
 *           unpacks Rx Data indication
 *
 * @param[in] function pointer
 * @param[in] Pst     *pst, Post structure of the primitive.
 * @param[in] Buffer *mBuf
 * @return ROK     - success
 *         RFAILED - failure
 *
 ***************************************************************************/
uint8_t unpackRxDataInd(RxDataIndFunc func, Pst *pst, Buffer *mBuf)
{
   if(pst->selector == ODU_SELECTOR_LWLC)
   {
      RxDataInd *rxDataInd = NULLP;

      /* unpack the address of the structure */
      CMCHKUNPK(oduUnpackPointer, (PTR *)&rxDataInd, mBuf);
      ODU_PUT_MSG_BUF(mBuf);
      return (*func)(pst, rxDataInd);
   }
   return RFAILED;
}


/*******************************************************************
 *
 * @brief Packs and Sends RACH Ind to MAC
 *
 * @details
 *
 *    Function : packRachInd
 *
 *    Functionality:
 *         Packs and Sends RACH Ind to MAC
 *
 * @params[in] Post structure pointer
 *             RACH indication
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t packRachInd(Pst *pst, RachInd *rachInd)
{
   if(pst->selector == ODU_SELECTOR_LWLC)
   {
      Buffer *mBuf = NULLP;
      if(ODU_GET_MSG_BUF(pst->region, pst->pool, &mBuf) != ROK)
      {
	 DU_LOG("\nERROR  -->  LWR_MAC: Memory allocation failed in packRachInd");
	 return RFAILED;
      }
      CMCHKPK(oduPackPointer,(PTR)rachInd, mBuf);
      return ODU_POST_TASK(pst, mBuf);
   }
   return RFAILED;
}

/**************************************************************************
 * @brief Function unpacks RachInd
 *
 * @details
 *
 *      Function : unpackRachInd
 *
 *      Functionality:
 *           unpacks Rach indication
 *
 * @param[in] function pointer
 * @param[in] Pst     *pst, Post structure of the primitive.
 * @param[in] Buffer *mBuf
 * @return ROK     - success
 *         RFAILED - failure
 *
 ***************************************************************************/
uint8_t unpackRachInd(RachIndFunc func, Pst *pst, Buffer *mBuf)
{
   if(pst->selector == ODU_SELECTOR_LWLC)
   {
      RachInd *rachInd = NULLP;

      /* unpack the address of the structure */
      CMCHKUNPK(oduUnpackPointer, (PTR *)&rachInd, mBuf);
      ODU_PUT_MSG_BUF(mBuf);
      return (*func)(pst, rachInd);
   }
   return RFAILED;
}

/*******************************************************************
 *
 * @brief Pack and send slot indication to MAC
 *
 * @details
 *
 *    Function : packSlotInd
 *
 *    Functionality:
 *       Pack and send slot indication to MAC
 *
 * @params[in] Post structure
 *             Slot indication info
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t packSlotInd (Pst *pst, SlotTimingInfo *slotInd)
{
   if(pst->selector == ODU_SELECTOR_LWLC)
   {
      Buffer *mBuf = NULLP;
      if(ODU_GET_MSG_BUF(pst->region, pst->pool, &mBuf) != ROK)
      {
	 DU_LOG("\nERROR  -->  LWR_MAC: Memory allocation failed in packSlotInd");
	 return RFAILED;
      }
      CMCHKPK(oduPackPointer,(PTR)slotInd, mBuf);
      return ODU_POST_TASK(pst, mBuf);
   }
   return RFAILED;
}

/**************************************************************************
 * @brief Function unpacks slot indication
 *
 * @details
 *
 *      Function : unpackSlotInd
 *
 *      Functionality:
 *           unpacks slot indication
 *
 * @param[in] function pointer,
 * @param[in] Pst     *pst,
 * @param[in] Buffer *mBuf
 * @return ROK     - success
 *         RFAILED - failure
 *
 ***************************************************************************/
uint8_t unpackSlotInd(SlotIndFunc func, Pst *pst, Buffer *mBuf)
{
   if(pst->selector == ODU_SELECTOR_LWLC)
   {
      SlotTimingInfo *slotInd = NULLP;

      /* unpack the address of the structure */
      CMCHKUNPK(oduUnpackPointer, (PTR *)&slotInd, mBuf);
      ODU_PUT_MSG_BUF(mBuf);
      return (*func)(pst, slotInd);
   }
   return RFAILED;
}


/*******************************************************************
 *
 * @brief Packs and Sends Stop Ind to MAC
 *
 * @details
 *
 *    Function : packStopInd
 *
 *    Functionality:
 *         Packs and Sends Stop Ind to MAC
 *
 * @params[in] Post structure pointer
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t packStopInd(Pst *pst, uint16_t *cellId)
{
   if(pst->selector == ODU_SELECTOR_LWLC)
   {
      Buffer *mBuf = NULLP;
      if(ODU_GET_MSG_BUF(pst->region, pst->pool, &mBuf) != ROK)
      {
	 DU_LOG("\nERROR  -->  LWR_MAC: Memory allocation failed in packStopInd");
	 return RFAILED;
      }
      CMCHKPK(oduPackPointer,(PTR)cellId, mBuf);
      return ODU_POST_TASK(pst, mBuf);
   }
   return RFAILED;
}

/**************************************************************************
 * @brief Function unpack stop indication
 *
 * @details
 *
 *      Function : unpackStopInd
 *
 *      Functionality:
 *           unpacks stop indication
 *
 * @param[in] function pointer
 * @param[in] Pst     *pst,
 * @param[in] Buffer *mBuf
 * @return ROK     - success
 *         RFAILED - failure
 *
 ***************************************************************************/
uint8_t unpackStopInd(StopIndFunc func, Pst *pst, Buffer *mBuf)
{
   if(pst->selector == ODU_SELECTOR_LWLC)
   {
      uint16_t *cellId = NULLP;

      /* unpack the address of the structure */
      CMCHKUNPK(oduUnpackPointer, (PTR *)&cellId, mBuf);
      ODU_PUT_MSG_BUF(mBuf);
      return (*func)(pst, cellId);
   }
   return RFAILED;
}

/*******************************************************************
 *
 * @brief Packs and Sends UCI Ind to MAC
 *
 * @details
 *
 *    Function : packUciInd
 *
 *    Functionality:
 *         Packs and Sends Uci Ind to MAC
 *
 * @params[in] Post structure pointer
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t packUciInd(Pst *pst, UciInd *uciInd)
{
   if(pst->selector == ODU_SELECTOR_LWLC)
   {
      Buffer *mBuf = NULLP;
      if(ODU_GET_MSG_BUF(pst->region, pst->pool, &mBuf) != ROK)
      {
	 DU_LOG("\nERROR  -->  LWR_MAC: Memory allocation failed in packUciInd");
	 return RFAILED;
      }
      CMCHKPK(oduPackPointer,(PTR)uciInd, mBuf);
      return ODU_POST_TASK(pst, mBuf);
   }
   return RFAILED;
}

/**************************************************************************
 * @brief Function unpack UCI indication
 *
 * @details
 *
 *      Function : unpackUciInd
 *
 *      Functionality:
 *           unpacks UCI indication
 *
 * @param[in] function pointer
 * @param[in] Pst     *pst,
 * @param[in] Buffer *mBuf
 * @return ROK     - success
 *         RFAILED - failure
 *
 ***************************************************************************/
uint8_t unpackUciInd(UciIndFunc func, Pst *pst, Buffer *mBuf)
{
   if(pst->selector == ODU_SELECTOR_LWLC)
   {
      UciInd *uciInd = NULLP;

      /* unpack the address of the structure */
      CMCHKUNPK(oduUnpackPointer, (PTR *)&uciInd, mBuf);
      ODU_PUT_MSG_BUF(mBuf);
      return (*func)(pst, uciInd);
   }
   return RFAILED;
}


/**********************************************************************
  End of file
 **********************************************************************/
