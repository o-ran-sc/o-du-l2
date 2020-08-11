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
   if((pst->selector == ODU_SELECTOR_LC) || (pst->selector == ODU_SELECTOR_LWLC))
   {
      return ROK;
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
   if((pst->selector == ODU_SELECTOR_LC) || (pst->selector == ODU_SELECTOR_LWLC))
   {
      return ROK;
   }
   else
   {
      return RFAILED;
   }
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
   if((pst->selector == ODU_SELECTOR_LC) || (pst->selector == ODU_SELECTOR_LWLC))
   {
      return ROK;
   }
   return RFAILED;
}

/*******************************************************************
 *
 * @brief Loose coupled packing of slot indication
 *
 * @details
 *
 *    Function : packLcSlotInd
 *
 *    Functionality:
 *        Loose coupled packing of slot indication
 *
 * @params[in] Post structure
 *             Slot indication info
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t packLcSlotInd (Pst *pst, SlotIndInfo *slotInd)
{
   Buffer *mBuf = NULLP;
   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK)
   {
      RETVALUE(RFAILED);
   }

   /* pack SFN and slot value */
   CMCHKPK(SPkU16,slotInd->sfn, mBuf);
   CMCHKPK(SPkU16,slotInd->slot, mBuf);

   RETVALUE(SPstTsk(pst,mBuf));
}

/*******************************************************************
 *
 * @brief Light weight loose coupled packing of slot indication
 *
 * @details
 *
 *    Function : packLwlcSlotInd
 *
 *    Functionality:
 *       Light weight loose coupled packing of slot indication
 *
 * @params[in] Post structure
 *             Slot indication info 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t packLwlcSlotInd (Pst *pst, SlotIndInfo *slotInd)
{
   return ROK;
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
uint8_t packStopInd(Pst *pst)
{
   if((pst->selector == ODU_SELECTOR_LC) || (pst->selector == ODU_SELECTOR_LWLC))
   {
      return ROK;
   }
   else
   {
      return RFAILED;
   }
}

/**********************************************************************
  End of file
 **********************************************************************/
