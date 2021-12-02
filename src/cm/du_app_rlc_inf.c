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
 *    Function : packDuRlcUeCreateReq
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
uint8_t packDuRlcUeCreateReq(Pst *pst, RlcUeCfg *ueCfg)
{
   Buffer *mBuf = NULLP;
 
   if(pst->selector == ODU_SELECTOR_LWLC)
   {
      if (ODU_GET_MSG_BUF(pst->region, pst->pool, &mBuf) != ROK)
      {
         DU_LOG("\nERROR  --> RLC : Memory allocation failed at packDuRlcUeCreateReq");
         return RFAILED;
      }
      /* pack the address of the structure */
      CMCHKPK(oduPackPointer,(PTR)ueCfg, mBuf);
   }
   else
   {
      DU_LOG("\nERROR  -->  RLC: Only LWLC supported for packDuRlcUeCreateReq");
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
 *    Function : unpackRlcUeCreateReq
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
uint8_t unpackRlcUeCreateReq(DuRlcUeCreateReq func, Pst *pst, Buffer *mBuf)
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
      DU_LOG("\nERROR  -->  RLC: Only LWLC supported for UE Create Request ");
      ODU_PUT_MSG_BUF(mBuf);
   }

   return RFAILED;
}

/*******************************************************************
 *
 * @brief Packs and Sends UE Cfg Response from RLC to DUAPP
 *
 * @details
 *
 *    Function : packRlcDuUeCfgRsp
 *
 *    Functionality:
 *       Packs and Sends UE Cfg Rrsponse from RLC to DUAPP
 *
 *
 * @params[in] Post structure pointer
 *             RlcUeCfg pointer              
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t packRlcDuUeCfgRsp(Pst *pst, RlcUeCfgRsp *ueCfg)
{
   Buffer *mBuf = NULLP;
 
   if(pst->selector == ODU_SELECTOR_LWLC)
   {
      if (ODU_GET_MSG_BUF(pst->region, pst->pool, &mBuf) != ROK)
      {
         DU_LOG("\nERROR  --> RLC : Memory allocation failed at packRlcDuUeCfgRsp");
         return RFAILED;
      }
      /* pack the address of the structure */
      CMCHKPK(oduPackPointer,(PTR)ueCfg, mBuf);
   }
   else
   {
      DU_LOG("\nERROR  -->  RLC: Only LWLC supported for packRlcDuUeCfgRsp");
      return RFAILED;
   }

    return ODU_POST_TASK(pst,mBuf);
}

/*******************************************************************
 *
 * @brief Unpacks UE Cfg Response received from DU APP
 *
 * @details
 *
 *    Function : unpackRlcUeCfgRsp
 *
 *    Functionality:
 *         Unpacks UE Cfg Response received from DU APP
 *
 * @params[in] Pointer to Handler
 *             Post structure pointer
 *             Message Buffer
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t unpackRlcUeCfgRsp(RlcDuUeCfgRsp func, Pst *pst, Buffer *mBuf)
{
   if(pst->selector == ODU_SELECTOR_LWLC)
   {
      RlcUeCfgRsp *cfgRsp = NULLP;
      /* unpack the address of the structure */
      CMCHKUNPK(oduUnpackPointer, (PTR *)&cfgRsp, mBuf);
      ODU_PUT_MSG_BUF(mBuf);
      return (*func)(pst, cfgRsp);
   }
   else
   {
      /* Nothing to do for other selectors */
      DU_LOG("\nERROR  -->  RLC: Only LWLC supported for UE Cfg Response ");
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
         DU_LOG("\nERROR  --> RLC : Memory allocation failed at packRlcUlRrcMsgToDu");
         return RFAILED;
      }
      /* pack the address of the structure */
      CMCHKPK(oduPackPointer,(PTR)ulRrcMsgInfo, mBuf);
      return ODU_POST_TASK(pst,mBuf);
   }
   else
   {
      DU_LOG("\nERROR  -->  RLC: Only LWLC supported for packRlcUlRrcMsgToDu");
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
      DU_LOG("\nERROR  -->  RLC: Only LWLC supported for UL RRC Message transfer ");
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
         DU_LOG("\nERROR  --> RLC : Memory allocation failed at packRlcDlRrcMsgToRlc");
         return RFAILED;
      }
      /* pack the address of the structure */
      CMCHKPK(oduPackPointer,(PTR)dlRrcMsgInfo, mBuf);
      return ODU_POST_TASK(pst,mBuf);
   }
   else
   {
      DU_LOG("\nERROR  -->  RLC: Only LWLC supported for packDlRrcMsgToRlc");
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
      DU_LOG("\nERROR  -->  RLC: Only LWLC supported for UL RRC Message transfer ");
      ODU_PUT_MSG_BUF(mBuf);
   }
   return RFAILED;
}

/*******************************************************************
*
* @brief packs RRC delivery report sending from  RLC to DU APP
*
* @details
*
*    Function : packRrcDeliveryReportToDu
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
uint8_t packRrcDeliveryReportToDu(Pst *pst, RrcDeliveryReport *rrcDeliveryReport)
{
    Buffer *mBuf = NULLP;

    if(pst->selector == ODU_SELECTOR_LWLC)
    {
       if (ODU_GET_MSG_BUF(pst->region, pst->pool, &mBuf) != ROK)
       {
          DU_LOG("\nERROR  --> RLC : Memory allocation failed at packRrcDeliveryReportToDu");
          return RFAILED;
       }
       /* pack the address of the structure */
       CMCHKPK(oduPackPointer,(PTR)rrcDeliveryReport, mBuf);
       return ODU_POST_TASK(pst,mBuf);
    }
    else
    {
       DU_LOG("\nERROR  -->  RLC: Only LWLC supported for packRrcDeliveryReportToDu");
    }
    return RFAILED;
 }

/*******************************************************************
*
* @brief Unpacks RRC Delivery Report info received at DU APP from RIC
*
* @details
*
*    Function : unpackRrcDeliveryReportToDu
*
*    Functionality:
*      Unpacks RRC Delivery Report info received at DU APP from RIC
*
* @params[in] Pointer to handler function
*             Post structure
*             Messae buffer to be unpacked
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/
uint8_t unpackRrcDeliveryReportToDu(RlcRrcDeliveryReportToDuFunc func, Pst *pst, Buffer *mBuf)
{
    if(pst->selector == ODU_SELECTOR_LWLC)
    {
       RrcDeliveryReport *rrcDeliveryReport;
       /* unpack the address of the structure */
       CMCHKUNPK(oduUnpackPointer, (PTR *)&rrcDeliveryReport, mBuf);
       ODU_PUT_MSG_BUF(mBuf);
       return (*func)(pst, rrcDeliveryReport);
    }
    else
    {
       /* Nothing to do for other selectors */
       DU_LOG("\nERROR  -->  RLC: Only LWLC supported for RRC delivery Message transfer ");
       ODU_PUT_MSG_BUF(mBuf);
    }

    return RFAILED;
}

/*******************************************************************
 *
 * @brief Packs and Sends UE Reconfig Request from DUAPP to RLC
 *
 * @details
 *
 *    Function : packDuRlcUeReconfigReq
 *
 *    Functionality:
 *       Packs and Sends UE Reconfig Request from DUAPP to RLC
 *
 *
 * @params[in] Post structure pointer
 *             RlcUeCfg pointer              
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t packDuRlcUeReconfigReq(Pst *pst, RlcUeCfg *ueCfg)
{
   Buffer *mBuf = NULLP;
 
   if(pst->selector == ODU_SELECTOR_LWLC)
   {
      if (ODU_GET_MSG_BUF(pst->region, pst->pool, &mBuf) != ROK)
      {
         DU_LOG("\nERROR  --> RLC : Memory allocation failed at packDuRlcUeReconfigReq");
         return RFAILED;
      }
      /* pack the address of the structure */
      CMCHKPK(oduPackPointer,(PTR)ueCfg, mBuf);
   }
   else
   {
      DU_LOG("\nERROR  -->  RLC: Only LWLC supported for packDuRlcUeReconfigReq");
      return RFAILED;
   }

    return ODU_POST_TASK(pst,mBuf);
}

/*******************************************************************
 * @brief Unpacks UE Reconfig Request received from DU APP
 *
 * @details
 *
 *    Function : unpackRlcUeReconfigReq
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
uint8_t unpackRlcUeReconfigReq(DuRlcUeReconfigReq func, Pst *pst, Buffer *mBuf)
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
      DU_LOG("\nERROR  -->  RLC: Only LWLC supported for UE Create Request ");
      ODU_PUT_MSG_BUF(mBuf);
   }
   return RFAILED;
}

/*******************************************************************
 *
 * @brief Pack and send DL RRC message Response from RLC to DU APP
 *
 * @details
 *
 *    Function : packRlcDlRrcMsgRspToDu
 *
 *    Functionality:
 *       Pack and send DL RRC message Response from RLC to DU APP
 *
 * @params[in] Post structure
 *             DL RRC Msg Response 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t packRlcDlRrcMsgRspToDu(Pst *pst, RlcDlRrcMsgRsp *dlRrcMsgRsp)
{
   Buffer *mBuf = NULLP;

   if(pst->selector == ODU_SELECTOR_LWLC)
   {
      if (ODU_GET_MSG_BUF(pst->region, pst->pool, &mBuf) != ROK)
      {
         DU_LOG("\nERROR  --> RLC : Memory allocation failed at packRlcDlRrcMsgRspToDu");
         return RFAILED;
      }
      /* pack the address of the structure */
      CMCHKPK(oduPackPointer,(PTR)dlRrcMsgRsp, mBuf);
      return ODU_POST_TASK(pst,mBuf);
   }
   else
   {
      DU_LOG("\nERROR  -->  RLC: Only LWLC supported for packRlcDlRrcMsgRspToDu");
   }
   return RFAILED;
}

/*******************************************************************
 *
 * @brief Unpack DL RRC Msg Response received at DU APP from RLC
 *
 * @details
 *
 *    Function : unpackRlcDlRrcMsgRspToDu
 *
 *    Functionality:
 *      Unpack DL RRC Msg Response received at DU APP from RLC
 *
 * @params[in]
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t unpackRlcDlRrcMsgRspToDu(RlcDlRrcMsgRspToDuFunc func, Pst *pst, Buffer *mBuf)
{
   if(pst->selector == ODU_SELECTOR_LWLC)
   {
      RlcDlRrcMsgRsp *dlRrcMsgRsp;
      /* unpack the address of the structure */
      CMCHKUNPK(oduUnpackPointer, (PTR *)&dlRrcMsgRsp, mBuf);
      ODU_PUT_MSG_BUF(mBuf);
      return (*func)(pst, dlRrcMsgRsp);
   }
   else
   {
      /* Nothing to do for other selectors */
      DU_LOG("\nERROR  -->  RLC: Only LWLC supported for DL RRC Message transfer ");
      ODU_PUT_MSG_BUF(mBuf);
   }
   return RFAILED;
}

/*******************************************************************
 *
 * @brief Pack and send UL user data from RLC to DU APP
 *
 * @details
 *
 *    Function : packRlcUlUserDataToDu
 *
 *    Functionality:
 *       Pack and send UL User Data from RLC to DU APP
 *
 * @params[in] Post structure
 *             UL user data
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t packRlcUlUserDataToDu(Pst *pst, RlcUlUserDatInfo *ulUserData)
{
   Buffer *mBuf = NULLP;

   if(pst->selector == ODU_SELECTOR_LWLC)
   {
      if (ODU_GET_MSG_BUF(pst->region, pst->pool, &mBuf) != ROK)
      {
         DU_LOG("\nERROR  -->  RLC UL: Memory allocation failed at packRlcUlUserDataToDu");
         return RFAILED;
      }
      /* pack the address of the structure */
      CMCHKPK(oduPackPointer,(PTR)ulUserData, mBuf);
      return ODU_POST_TASK(pst,mBuf);
   }
   else
   {
      DU_LOG("\nERROR  -->  RLC UL: Only LWLC supported for packRlcUlUserDataToDu");
   }
   return RFAILED;
}

/*******************************************************************
 *
 * @brief Unpack UL user data received at DU APP from RLC
 *
 * @details
 *
 *    Function : unpackRlcUlUserDataToDu
 *
 *    Functionality:
 *      Unpack UL user data received at DU APP from RLC
 *
 * @params[in]
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t unpackRlcUlUserDataToDu(RlcUlUserDataToDuFunc func, Pst *pst, Buffer *mBuf)
{
   if(pst->selector == ODU_SELECTOR_LWLC)
   {
      RlcUlUserDatInfo *ulUserData;
      /* unpack the address of the structure */
      CMCHKUNPK(oduUnpackPointer, (PTR *)&ulUserData, mBuf);
      ODU_PUT_MSG_BUF(mBuf);
      return (*func)(pst, ulUserData);
   }
   else
   {
      /* Nothing to do for other selectors */
      DU_LOG("\nERROR  --> RLC UL: Only LWLC supported for UL User data transfer ");
      ODU_PUT_MSG_BUF(mBuf);
   }

   return RFAILED;
}

/*******************************************************************
 *
 * @brief Pack and send DL user data from DUAPP to RLC
 *
 * @details
 *
 *    Function : packRlcDlUserDataToRlc
 *
 *    Functionality:
 *       Pack and send DL User Data from DUAPP to RLC
 *
 * @params[in] Post structure
 *             DL user data
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t packRlcDlUserDataToRlc(Pst *pst, RlcDlUserDataInfo *dlUserData)
{
   if(pst->selector == ODU_SELECTOR_LWLC)
   {
      /* pack the address of the structure */
      CMCHKPK(oduPackPointer,(PTR)dlUserData, dlUserData->dlMsg);
      return ODU_POST_TASK(pst,dlUserData->dlMsg);
   }
   else
   {
      DU_LOG("\nERROR  -->  RLC DL: Only LWLC supported for packRlcUlUserDataToDu");
   }
   return RFAILED;
}

/*******************************************************************
 *
 * @brief Unpack DL user data received at RLC from DUAPP
 *
 * @details
 *
 *    Function : unpackRlcDlUserDataToRlc
 *
 *    Functionality:
 *      Unpack DL user data received at RLC from DUAPP
 *
 * @params[in]
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t unpackRlcDlUserDataToRlc(DuRlcDlUserDataToRlcFunc func, Pst *pst, Buffer *mBuf)
{
   if(pst->selector == ODU_SELECTOR_LWLC)
   {
      RlcDlUserDataInfo *dlUserData;
      /* unpack the address of the structure */
      CMCHKUNPK(oduUnpackPointer, (PTR *)&dlUserData, mBuf);
      return (*func)(pst, dlUserData);
   }
   else
   {
      /* Nothing to do for other selectors */
      DU_LOG("\nERROR  --> RLC DL: Only LWLC supported for UL User data transfer ");
      ODU_PUT_MSG_BUF(mBuf);
   }

   return RFAILED;
}

/*******************************************************************
*
* @brief Packs and Sends UE Delete Request from DUAPP to RLC
*
* @details
*
*    Function : packDuRlcUeDeleteReq 
*
*    Functionality:
*       Packs and Sends UE Delete Request from DUAPP to RLC
*
*
* @params[in] Post structure pointer
*             RlcUeDelete pointer
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/

uint8_t packDuRlcUeDeleteReq(Pst *pst, RlcUeDelete *ueDelete)
{
   Buffer *mBuf = NULLP;

   if(pst->selector == ODU_SELECTOR_LWLC)
   {
      if (ODU_GET_MSG_BUF(pst->region, pst->pool, &mBuf) != ROK)
      {
         DU_LOG("\nERROR  --> RLC : Memory allocation failed at packDuRlcUeDeleteReq");
         return RFAILED;
      }
      /* pack the address of the structure */
      CMCHKPK(oduPackPointer,(PTR)ueDelete, mBuf);
   }
   else
   {
      DU_LOG("\nERROR  -->  RLC: Only LWLC supported for packDuRlcUeDeleteReq");
      return RFAILED;
   }

   return ODU_POST_TASK(pst,mBuf);
}

/*******************************************************************
* @brief Unpacks UE Delete Request received from DU APP
*
* @details
*
*    Function : unpackRlcUeDeleteReq 
*
*    Functionality:
*         Unpacks UE Delete Request received from DU APP
*
* @params[in] Pointer to Handler
*             Post structure pointer
*             Message Buffer
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/

uint8_t unpackRlcUeDeleteReq(DuRlcUeDeleteReq func, Pst *pst, Buffer *mBuf)
{
    if(pst->selector == ODU_SELECTOR_LWLC)
    {
       RlcUeDelete *ueDelete;
       /* unpack the address of the structure */
       CMCHKUNPK(oduUnpackPointer, (PTR *)&ueDelete, mBuf);
       ODU_PUT_MSG_BUF(mBuf);
       return (*func)(pst, ueDelete);
    }
    else
    {
       /* Nothing to do for other selectors */
       DU_LOG("\nERROR  -->  RLC: Only LWLC supported for UE Delete Request ");
       ODU_PUT_MSG_BUF(mBuf);
    }
    return RFAILED;
}

/*******************************************************************
*
* @brief Packs and Sends UE Del Response from RLC to DUAPP
*
* @details
*
*    Function : packRlcDuUeDeleteRsp
*
*    Functionality:
*       Packs and Sends UE Del Response from RLC to DUAPP
*
*
* @params[in] Post structure pointer
*             RlcUeDeleteRsp *ueDelRsp
*
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/

uint8_t packRlcDuUeDeleteRsp(Pst *pst, RlcUeDeleteRsp *ueDelRsp)
{
   Buffer *mBuf = NULLP;

   if(pst->selector == ODU_SELECTOR_LWLC)
   {
      if (ODU_GET_MSG_BUF(pst->region, pst->pool, &mBuf) != ROK)
      {
         DU_LOG("\nERROR  --> RLC : Memory allocation failed at packRlcDuUeDeleteRsp");
         return RFAILED;
      }
      /* pack the address of the structure */
      CMCHKPK(oduPackPointer,(PTR)ueDelRsp, mBuf);
   }
   else
   {
      DU_LOG("\nERROR  -->  RLC: Only LWLC supported for packRlcDuUeDeleteRsp");
      return RFAILED;
   }

   return ODU_POST_TASK(pst,mBuf);
}

/*******************************************************************
*
* @brief Unpacks UE Del Response received from DU APP
*
* @details
*
*    Function : unpackRlcUeDeleteRsp
*
*    Functionality:
*         Unpacks UE Del Response received from DU APP
*
* @params[in] Pointer to Handler
*             Post structure pointer
*             Message Buffer
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/

uint8_t unpackRlcUeDeleteRsp(RlcDuUeDeleteRsp func, Pst *pst, Buffer *mBuf)
{
    if(pst->selector == ODU_SELECTOR_LWLC)
    {
       RlcUeDeleteRsp *ueDeleteRsp = NULLP;
       /* unpack the address of the structure */
       CMCHKUNPK(oduUnpackPointer, (PTR *)&ueDeleteRsp, mBuf);
       ODU_PUT_MSG_BUF(mBuf);
       return (*func)(pst, ueDeleteRsp);
    }
    else
    {
       /* Nothing to do for other selectors */
       DU_LOG("\nERROR  -->  RLC: Only LWLC supported for UE Del Response ");
       ODU_PUT_MSG_BUF(mBuf);
    }

    return RFAILED;
}

/*******************************************************************
*
* @brief Packs and Sends Slice PM from RLC to DUAPP
*
* @details
*
*    Function : packRlcDuSlicePm
*
*    Functionality:
*       Packs and Sends Slice Performance Metrics from RLC to DUAPP
*
*
* @params[in] Post structure pointer
*             SlicePmList *sliceStats
*
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/

uint8_t packRlcDuSlicePm(Pst *pst, SlicePmList *sliceStats)
{
   Buffer *mBuf = NULLP;

   if(pst->selector == ODU_SELECTOR_LWLC)
   {
      if (ODU_GET_MSG_BUF(pst->region, pst->pool, &mBuf) != ROK)
      {
         DU_LOG("\nERROR  --> RLC : Memory allocation failed at packRlcDuSlicePm");
         return RFAILED;
      }
      /* pack the address of the structure */
      CMCHKPK(oduPackPointer,(PTR)sliceStats, mBuf);
   }
   else
   {
      DU_LOG("\nERROR  -->  RLC: Only LWLC supported for packRlcDuSlicePm");
      return RFAILED;
   }

   return ODU_POST_TASK(pst,mBuf);
}

/*******************************************************************
*
* @brief Unpacks Slice PM received from RLC
*
* @details
*
*    Function : unpackRlcSlicePm
*
*    Functionality:
*         Unpacks Slice Performance Metrics received from RLC
*
* @params[in] Pointer to Handler
*             Post structure pointer
*             Message Buffer
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/

uint8_t unpackRlcSlicePm(RlcSlicePmToDuFunc func, Pst *pst, Buffer *mBuf)
{
    if(pst->selector == ODU_SELECTOR_LWLC)
    {
       SlicePmList *sliceStats = NULLP;
       /* unpack the address of the structure */
       CMCHKUNPK(oduUnpackPointer, (PTR *)&sliceStats, mBuf);
       ODU_PUT_MSG_BUF(mBuf);
       return (*func)(pst, sliceStats);
    }
    else
    {
       /* Nothing to do for other selectors */
       DU_LOG("\nERROR  -->  RLC: Only LWLC supported for Slice Metrics ");
       ODU_PUT_MSG_BUF(mBuf);
    }

    return RFAILED;
}
/**********************************************************************
         End of file
***********************************************************************/
