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
 * @brief Packs and Sends UE Cfg Response from RLC to DUAPP
 *
 * @details
 *
 *    Function : packRlcUlDuUeCfgRsp
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
uint8_t packRlcUlDuUeCfgRsp(Pst *pst, RlcUeCfgRsp *ueCfg)
{
   Buffer *mBuf = NULLP;
 
   if(pst->selector == ODU_SELECTOR_LWLC)
   {
      if (ODU_GET_MSG_BUF(pst->region, pst->pool, &mBuf) != ROK)
      {
         DU_LOG("\nRLC : Memory allocation failed at packRlcUlDuUeCfgRsp");
         return RFAILED;
      }
      /* pack the address of the structure */
      CMCHKPK(oduPackPointer,(PTR)ueCfg, mBuf);
   }
   else
   {
      DU_LOG("\nRLC: Only LWLC supported for packRlcUlDuUeCfgRsp");
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
 *    Function : unpackRlcUlUeCfgRsp
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
uint8_t unpackRlcUlUeCfgRsp(RlcUlDuUeCfgRsp func, Pst *pst, Buffer *mBuf)
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
      DU_LOG("\nRLC: Only LWLC supported for UE Cfg Response ");
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
          DU_LOG("\nRLC : Memory allocation failed at packRrcDeliveryReportToDu");
          return RFAILED;
       }
       /* pack the address of the structure */
       CMCHKPK(oduPackPointer,(PTR)rrcDeliveryReport, mBuf);
       return ODU_POST_TASK(pst,mBuf);
    }
    else
    {
       DU_LOG("\nRLC: Only LWLC supported for packRrcDeliveryReportToDu");
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
       DU_LOG("\nRLC: Only LWLC supported for RRC delivery Message transfer ");
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
 *    Function : packDuRlcUlUeReconfigReq
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
uint8_t packDuRlcUlUeReconfigReq(Pst *pst, RlcUeCfg *ueCfg)
{
   Buffer *mBuf = NULLP;
 
   if(pst->selector == ODU_SELECTOR_LWLC)
   {
      if (ODU_GET_MSG_BUF(pst->region, pst->pool, &mBuf) != ROK)
      {
         DU_LOG("\nRLC : Memory allocation failed at packDuRlcUeReconfigReq");
         return RFAILED;
      }
      /* pack the address of the structure */
      CMCHKPK(oduPackPointer,(PTR)ueCfg, mBuf);
   }
   else
   {
      DU_LOG("\nRLC: Only LWLC supported for packDuRlcUeReconfigReq");
      return RFAILED;
   }

    return ODU_POST_TASK(pst,mBuf);
}

/*******************************************************************
 * @brief Unpacks UE Reconfig Request received from DU APP
 *
 * @details
 *
 *    Function : unpackRlcUlUeReconfigReq
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
uint8_t unpackRlcUlUeReconfigReq(DuRlcUlUeReconfigReq func, Pst *pst, Buffer *mBuf)
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
/******************************************************************
 *
 * @brief Fills Rlc AM Information
 *
 * @details
 *
 *    Function : fillDefaultAmInfo
 *
 *    Functionality: Fills Rlc AM Information
 *
 *
 *****************************************************************/
void fillDefaultAmInfo(AmBearerCfg *amCfg)
{
   /* DL AM */
   amCfg->dlAmCfg.snLenDl     = AM_SIZE_12;
   amCfg->dlAmCfg.pollRetxTmr = POLL_RETX_TMR_45MS;
   amCfg->dlAmCfg.pollPdu     = POLL_PDU_TMR_INFINITY;
   amCfg->dlAmCfg.pollByte    = POLL_BYTES_INFINITY;
   amCfg->dlAmCfg.maxRetxTh   = RETX_TH_8;   
 
   /* UL AM */
   amCfg->ulAmCfg.snLenUl     = AM_SIZE_12;
   amCfg->ulAmCfg.reAssemTmr  = RE_ASM_40MS; 
   amCfg->ulAmCfg.statProhTmr = PROH_35MS;
}

/******************************************************************
 *
 * @brief Fills RLC UM Bi Directional Information
 *
 * @details
 *
 *    Function : fillDefaultUmBiInfo
 *
 *    Functionality: Fills RLC UM Bi Directional Information
 *
 *
 *****************************************************************/
void fillDefaultUmBiInfo(UmBiDirBearerCfg *umBiDirCfg)
{
   /* UL UM BI DIR INFO */
   umBiDirCfg->ulUmCfg.snLenUlUm = UM_SIZE_12;
   umBiDirCfg->ulUmCfg.reAssemTmr = RE_ASM_40MS;

   /* DL UM BI DIR INFO */
   umBiDirCfg->dlUmCfg.snLenDlUm  = UM_SIZE_12;
}

/******************************************************************
 *
 * @brief Fills RLC UM Uni Directional UL Information
 *
 * @details
 *
 *    Function : fillDefaultUmUlInfo
 *
 *    Functionality: Fills RLC UM Uni Directional Info
 *
 *
 *****************************************************************/
void fillDefaultUmUlInfo(UmUniDirUlBearerCfg *UmUlCfg)
{
   UmUlCfg->ulUmCfg.snLenUlUm = UM_SIZE_12;
   UmUlCfg->ulUmCfg.reAssemTmr = RE_ASM_40MS;
}

/******************************************************************
 *
 * @brief Fills RLC UM Uni Directional DL Information
 *
 * @details
 *
 *    Function : fillDefaultUmDlInfo
 *
 *    Functionality: Fills RLC UM Uni Directional DL Info
 *
 *
 *****************************************************************/
void fillDefaultUmDlInfo(UmUniDirDlBearerCfg *UmDlCfg)
{
   UmDlCfg->dlUmCfg.snLenDlUm  = UM_SIZE_12;
}

/******************************************************************
 *
 * @brief Builds Rlc Mode Default Configuration
 *
 * @details
 *
 *    Function : buildRlcModeDefaultCfg
 *
 *    Functionality: Builds Rlc Mode Default Configuration
 *
 *
 *****************************************************************/

uint8_t buildRlcModeDefaultCfg(uint8_t rlcMode, RlcBearerCfg *lcCfg)
{
   if(lcCfg)
   {
      switch(rlcMode)
      {
         case RLC_AM :
            {
               fillDefaultAmInfo(&lcCfg->u.amCfg);
               break;
            }
         case RLC_UM_BI_DIRECTIONAL :
            {
               fillDefaultUmBiInfo(&lcCfg->u.umBiDirCfg);
               break;
            }
         case RLC_UM_UNI_DIRECTIONAL_UL :
            {
               fillDefaultUmUlInfo(&lcCfg->u.umUniDirUlCfg);
               break;
            }
         case RLC_UM_UNI_DIRECTIONAL_DL :
            {
               fillDefaultUmDlInfo(&lcCfg->u.umUniDirDlCfg);
               break;
            }
         default:
            DU_LOG("\nDUAPP: Invalid rlcMode %d at extractRlcCfgToAddMod()", rlcMode);
            return RFAILED;
      }

   }
   else
   {
      DU_LOG("\nDUAPP: Received Lc Config is NULL");
      return RFAILED;
   }
   return ROK;
}
/**********************************************************************
         End of file
***********************************************************************/
