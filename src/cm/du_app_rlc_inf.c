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
 * @brief Packs and Sends UE create Request from DUAPP to RLC
 *
 * @details
 *
 *    Function : packDuRlcUlUeCreateReq
 *
 *    Functionality:
 *       Packs and Sends UE Create Request from DUAPP to RLC
 *
 *
 * @params[in] Post structure pointer
 *             RlcUeCfg pointer              
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t packDuRlcUlUeCreateReq(Pst *pst, RlcUeCfg *ueCfg)
{
   Buffer *mBuf = NULLP;
 
   if(pst->selector == ODU_SELECTOR_LWLC)
   {
      if (ODU_GET_MSG_BUF(pst->region, pst->pool, &mBuf) != ROK)
      {
         DU_LOG("\nRLC : Memory allocation failed at packDuRlcUeCreateReq");
         return RFAILED;
      }
      /* pack the address of the structure */
      CMCHKPK(oduPackPointer,(PTR)ueCfg, mBuf);
   }
   else
   {
      DU_LOG("\nRLC: Only LWLC supported for packDuRlcUeCreateReq");
      return RFAILED;
   }

    return ODU_POST_TASK(pst,mBuf);
}

/*******************************************************************
 *
 * @brief Unpacks UE Create Request received from DU APP
 *
 * @details
 *
 *    Function : unpackRlcUlUeCreateReq
 *
 *    Functionality:
 *         Unpacks UE Create Request received from DU APP
 *
 * @params[in] Pointer to Handler
 *             Post structure pointer
 *             Message Buffer
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t unpackRlcUlUeCreateReq(DuRlcUlUeCreateReq func, Pst *pst, Buffer *mBuf)
{
   if(pst->selector == ODU_SELECTOR_LWLC)
   {
      RlcUeCfg *ueCfg;
      /* unpack the address of the structure */
      CMCHKUNPK(oduUnpackPointer, (PTR *)&ueCfg, mBuf);
      ODU_PUT_MSG_BUF(mBuf);
      return (*func)(pst, ueCfg);
   }
   else
   {
      /* Nothing to do for other selectors */
      DU_LOG("\nRLC: Only LWLC supported for UE Create Request ");
      ODU_PUT_MSG_BUF(mBuf);
   }

   return RFAILED;
}

/*******************************************************************
 *
 * @brief Packs and Sends UE create Response from RLC to DUAPP
 *
 * @details
 *
 *    Function : packRlcUlDuUeCreateRsp
 *
 *    Functionality:
 *       Packs and Sends UE Create Rrsponse from RLC to DUAPP
 *
 *
 * @params[in] Post structure pointer
 *             RlcUeCfg pointer              
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t packRlcUlDuUeCreateRsp(Pst *pst, RlcUeCfgRsp *ueCfg)
{
   Buffer *mBuf = NULLP;
 
   if(pst->selector == ODU_SELECTOR_LWLC)
   {
      if (ODU_GET_MSG_BUF(pst->region, pst->pool, &mBuf) != ROK)
      {
         DU_LOG("\nRLC : Memory allocation failed at packRlcUlDuUeCreateRsp");
         return RFAILED;
      }
      /* pack the address of the structure */
      CMCHKPK(oduPackPointer,(PTR)ueCfg, mBuf);
   }
   else
   {
      DU_LOG("\nRLC: Only LWLC supported for packRlcUlDuUeCreateRsp");
      return RFAILED;
   }

    return ODU_POST_TASK(pst,mBuf);
}

/*******************************************************************
 *
 * @brief Unpacks UE Create Response received from DU APP
 *
 * @details
 *
 *    Function : unpackRlcUlUeCreateRsp
 *
 *    Functionality:
 *         Unpacks UE Create Response received from DU APP
 *
 * @params[in] Pointer to Handler
 *             Post structure pointer
 *             Message Buffer
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t unpackRlcUlUeCreateRsp(RlcUlDuUeCreateRsp func, Pst *pst, Buffer *mBuf)
{
   if(pst->selector == ODU_SELECTOR_LWLC)
   {
      RlcUeCfgRsp *cfgRsp;
      /* unpack the address of the structure */
      CMCHKUNPK(oduUnpackPointer, (PTR *)&cfgRsp, mBuf);
      ODU_PUT_MSG_BUF(mBuf);
      return (*func)(pst, cfgRsp);
   }
   else
   {
      /* Nothing to do for other selectors */
      DU_LOG("\nRLC: Only LWLC supported for UE Create Response ");
      ODU_PUT_MSG_BUF(mBuf);
   }

   return RFAILED;
}

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
      if (ODU_GET_MSG_BUF(pst->region, pst->pool, &mBuf) != ROK)
      {
         DU_LOG("\nRLC : Memory allocation failed at packRlcUlRrcMsgToDu");
         return RFAILED;
      }
      /* pack the address of the structure */
      CMCHKPK(oduPackPointer,(PTR)ulRrcMsgInfo, mBuf);
      return ODU_POST_TASK(pst,mBuf);
   }
   else
   {
      DU_LOG("\nRLC: Only LWLC supported for packRlcUlRrcMsgToDu");
   }
   return RFAILED;
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
      CMCHKUNPK(oduUnpackPointer, (PTR *)&ulRrcMsgInfo, mBuf);
      ODU_PUT_MSG_BUF(mBuf);
      return (*func)(pst, ulRrcMsgInfo);
   }
   else
   {
      /* Nothing to do for other selectors */
      DU_LOG("\nRLC: Only LWLC supported for UL RRC Message transfer ");
      ODU_PUT_MSG_BUF(mBuf);
   }

   return RFAILED;
}

/*******************************************************************
 *
 * @brief Pack and post DL RRC Message from DU APP to RLC 
 *
 * @details
 *
 *    Function : packDlRrcMsgToRlc
 *
 *    Functionality: Pack and post DL RRC Message from DU APP to RLC
 *
 * @params[in] Post structure
 *             DL RRC Message info
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t packDlRrcMsgToRlc(Pst *pst, RlcDlRrcMsgInfo *dlRrcMsgInfo)
{
   Buffer *mBuf = NULLP;

   if(pst->selector == ODU_SELECTOR_LWLC)
   {
      if (ODU_GET_MSG_BUF(pst->region, pst->pool, &mBuf) != ROK)
      {
         DU_LOG("\nRLC : Memory allocation failed at packRlcUlRrcMsgToDu");
         return RFAILED;
      }
      /* pack the address of the structure */
      CMCHKPK(oduPackPointer,(PTR)dlRrcMsgInfo, mBuf);
      return ODU_POST_TASK(pst,mBuf);
   }
   else
   {
      DU_LOG("\nRLC: Only LWLC supported for packDlRrcMsgToRlc");
   }
   return RFAILED;
}

/*******************************************************************
 *
 * @brief Unpacks DL RRC Message info received at RLC from DU APP
 *
 * @details
 *
 *    Function : unpackDlRrcMsgToRlc
 *
 *    Functionality:
 *      Unpacks the DL RRC Message info received at RLC from DU APP
 *
 * @params[in] Pointer to handler function
 *             Post structure
 *             Messae buffer to be unpacked
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t unpackDlRrcMsgToRlc(DuDlRrcMsgToRlcFunc func, Pst *pst, Buffer *mBuf)
{
   if(pst->selector == ODU_SELECTOR_LWLC)
   {
      RlcDlRrcMsgInfo *dlRrcMsgInfo;
      /* unpack the address of the structure */
      CMCHKUNPK(oduUnpackPointer, (PTR *)&dlRrcMsgInfo, mBuf);
      ODU_PUT_MSG_BUF(mBuf);
      return (*func)(pst, dlRrcMsgInfo);
   }
   else
   {
      /* Nothing to do for other selectors */
      DU_LOG("\nRLC: Only LWLC supported for UL RRC Message transfer ");
      ODU_PUT_MSG_BUF(mBuf);
   }
   return RFAILED;
}

/**********************************************************************
         End of file
***********************************************************************/
