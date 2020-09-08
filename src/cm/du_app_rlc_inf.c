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
#include "du_app_rlc_inf.h"

/*******************************************************************
 *
 * @brief Pack and send UL RRC message transfer from RLC to DU APP 
 *
 * @details
 *
 *    Function : packRlcUlRrcMsgToDu
 *
 *    Functionality:
 *       Pack and send UL RRC message transfer from RLC to DU APP
 *
 * @params[in] Post structure
 *             UL RRC Msg transfer info
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t packRlcUlRrcMsgToDu(Pst *pst, RlcUlRrcMsgInfo *ulRrcMsgInfo)
{
   Buffer *mBuf = NULLP;
 
   if(pst->selector == ODU_SELECTOR_LWLC)
   {
      if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK)
      {
         DU_LOG("\nRLC : Memory allocation failed at packRlcUlRrcMsgToDu");
         return RFAILED;
      }
      /* pack the address of the structure */
      CMCHKPK(cmPkPtr,(PTR)ulRrcMsgInfo, mBuf);
   }
   else
   {
      DU_LOG("\nRLC: Only LWLC supported for packRlcUlRrcMsgToDu");
      return RFAILED;
   }

    return SPstTsk(pst,mBuf);
}

/*******************************************************************
 *
 * @brief Unpack UL RRC Msg Transfer received at DU APP from RLC
 *
 * @details
 *
 *    Function : unpackRlcUlRrcMsgToDu
 *
 *    Functionality:
 *      Unpack UL RRC Msg Transfer received at DU APP from RLC
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t unpackRlcUlRrcMsgToDu(RlcUlRrcMsgToDuFunc func, Pst *pst, Buffer *mBuf)
{
   if(pst->selector == ODU_SELECTOR_LWLC)
   {
      RlcUlRrcMsgInfo *ulRrcMsgInfo;
      /* unpack the address of the structure */
      CMCHKUNPK(cmUnpkPtr, (PTR *)&ulRrcMsgInfo, mBuf);
      SPutMsg(mBuf);
      return (*func)(pst, ulRrcMsgInfo);
   }
   else
   {
      /* Nothing to do for other selectors */
      DU_LOG("\nRLC: Only LWLC supported for UL RRC Message transfer ");
      SPutMsg(mBuf);
   }

   return RFAILED;
}

/**********************************************************************
         End of file
***********************************************************************/
