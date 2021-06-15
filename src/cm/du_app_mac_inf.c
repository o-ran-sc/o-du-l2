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
#include "du_app_mac_inf.h"

/**************************************************************************
 * @brief Function to pack Loose Coupled 
 *        MAC cell config parameters required by MAC
 *
 * @details
 *
 *      Function : packMacCellCfg
 *
 *      Functionality:
 *           packs the macCellCfg parameters
 *
 * @param[in] Pst     *pst, Post structure of the primitive.
 * @param[in] MacCellCfg  *macCellCfg, mac cell config parameters.
 * @return ROK     - success
 *         RFAILED - failure
 *
 ***************************************************************************/
uint8_t packMacCellCfg(Pst *pst, MacCellCfg *macCellCfg)
{
   if(pst->selector == ODU_SELECTOR_LC)
   {
      /* we are now implemented only light wieght lossely coupled interface */
      return RFAILED;
   }
   else if(pst->selector == ODU_SELECTOR_LWLC)
   {
      Buffer *mBuf = NULLP;

      if (ODU_GET_MSG_BUF(pst->region, pst->pool, &mBuf) != ROK) 
      {
	 return RFAILED;
      }

      /* pack the address of the structure */
      CMCHKPK(oduPackPointer,(PTR)macCellCfg, mBuf);

      DU_LOG("\nDEBUG  -->  DU-APP : MAC CELL config sent");
      return ODU_POST_TASK(pst,mBuf);
   } 
   return ROK;
}

/**************************************************************************
 * @brief Function to pack Loose Coupled 
 *        MAC cell config parameters required by MAC
 *
 * @details
 *
 *      Function : unpackDuMacCellCfg
 *
 *      Functionality:
 *           packs the macCellCfg parameters
 *
 * @param[in] DuMacCellCfgReq func; function pointer
 * @param[in] Pst     *pst, Post structure of the primitive.
 * @param[in] Buffer *mBuf
 * @return ROK     - success
 *         RFAILED - failure
 *
 ***************************************************************************/
uint8_t  unpackDuMacCellCfg(DuMacCellCfgReq func, Pst *pst, Buffer *mBuf)
{
   uint16_t ret = ROK;
   MacCellCfg *macCellCfg;

   if(pst->selector == ODU_SELECTOR_LWLC)
   {
      /* unpack the address of the structure */
      CMCHKUNPK(oduUnpackPointer, (PTR *)&macCellCfg, mBuf);
      ret = (*func)(pst, macCellCfg);
   }
   else
   {
      /* only LWLC is implemented now */
      ret = ROK;
   }

   return ret;
}

/**************************************************************************
 * @brief Function to pack Loose Coupled 
 *        MAC cell config confirm message
 *
 * @details
 *
 *      Function : packMacCellCfgCfm
 *
 *      Functionality:
 *           packs the transaction ID  
 *
 * @param[in] Pst     *pst, Post structure of the primitive.
 * @param[in] MacCellCfgCfm  *macCellCfgCfm, mac cell config confirm.
 * @return ROK     - success
 *         RFAILED - failure
 *
 ***************************************************************************/
uint8_t  packMacCellCfgCfm(Pst *pst, MacCellCfgCfm *macCellCfgCfm)
{
   if(pst->selector == ODU_SELECTOR_LC)
   {
      Buffer *mBuf = NULLP;
      if (ODU_GET_MSG_BUF(pst->region, pst->pool, &mBuf) != ROK) 
      {
	 return RFAILED;
      }

      /* pack the transaction ID in CNF structure */
      CMCHKPK(oduUnpackUInt16, macCellCfgCfm->cellId, mBuf);
      CMCHKPK(oduUnpackUInt8, macCellCfgCfm->rsp, mBuf);

      return ODU_POST_TASK(pst,mBuf);
   }
   else if(pst->selector == ODU_SELECTOR_LWLC)
   {
      /* only LC is supported */
      return RFAILED;
   }
   return ROK;
}

/**************************************************************************
 * @brief Function to pack MAC cell config confirm message
 *
 * @details
 *
 *      Function : unpackMacCellCfgCfm
 *
 *      Functionality:
 *           packs the transaction ID  
 *
 * @param[in] DuMacCellCfgCfm func; function pointer
 * @param[in] Pst     *pst, Post structure of the primitive.
 * @param[in] Buffer *mBuf
 * @return ROK     - success
 *         RFAILED - failure
 *
 ***************************************************************************/
uint8_t unpackMacCellCfgCfm(DuMacCellCfgCfm func, Pst *pst, Buffer *mBuf)
{
   MacCellCfgCfm macCellCfgCfm;

   if(pst->selector == ODU_SELECTOR_LC)
   {
      /* unpack the transaction ID in CNF structure */
      CMCHKUNPK(oduPackUInt8, &(macCellCfgCfm.rsp), mBuf);
      CMCHKUNPK(oduPackUInt16, &(macCellCfgCfm.cellId), mBuf);
      return (*func)(pst, &macCellCfgCfm);
   }
   else
   {
      /* only loose coupling is suported */
      return ROK;
   }
}

/*******************************************************************
 *
 * @brief Packs and Send Cell Start Request to MAC
 *
 * @details
 *
 *    Function : packMacCellStart
 *
 *    Functionality:
 *      Packs and Sends Cell Start Request to MAC
 *
 * @params[in] Post structure pointer
 *             Cell Id
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t packMacCellStart(Pst *pst, OduCellId *cellId)
{
   Buffer *mBuf = NULLP;

   if(pst->selector == ODU_SELECTOR_LC)
   {
      /* Loose coupling not supported */
      return RFAILED;
   }
   else if(pst->selector == ODU_SELECTOR_LWLC)
   {

      if (ODU_GET_MSG_BUF(pst->region, pst->pool, &mBuf) != ROK)
      {
	 DU_LOG("\nERROR  --> DU APP : Memory allocation failed for cell start req pack");
	 return RFAILED;
      }

      /* pack the address of the structure */
      CMCHKPK(oduPackPointer,(PTR)cellId, mBuf);

   }
   return ODU_POST_TASK(pst,mBuf);
}

/*******************************************************************
 *
 * @brief Unpacks MAC Cell Start Request from DU APP
 *
 * @details
 *
 *    Function : unpackMaCellStart
 *
 *    Functionality:
 *      Unpacks MAC Cell Start Request from DU APP
 *
 * @params[in] Function pointer of cell start request handler
 *             Post structure pointer
 *             Message Buffer
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t unpackMacCellStart(DuMacCellStart func, Pst *pst, Buffer *mBuf)
{
   OduCellId  *cellId;

   if(pst->selector == ODU_SELECTOR_LWLC)
   {
      /* unpack the address of the structure */
      CMCHKUNPK(oduUnpackPointer, (PTR *)&cellId, mBuf);
      ODU_PUT_MSG_BUF(mBuf); 
      return (*func)(pst, cellId);
   }
   else
   {
      /* Nothing to do for loose coupling */
      ODU_PUT_MSG_BUF(mBuf);
      return ROK;
   }
}

/*******************************************************************
 *
 * @brief Packs and Send cell stop request to MAC
 *
 * @details
 *
 *    Function : packMacCellStop
 *
 *    Functionality:
 *       Packs and Send cell stop request to MAC
 *
 * @params[in] Post structure pointer
 *             Cell Id 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t packMacCellStop(Pst *pst, OduCellId  *cellId)
{
   if(pst->selector == ODU_SELECTOR_LC)
   {
      /* Loose coupling not supported */
      return RFAILED;
   }
   else if(pst->selector == ODU_SELECTOR_LWLC)
   {
      Buffer *mBuf = NULLP;

      if (ODU_GET_MSG_BUF(pst->region, pst->pool, &mBuf) != ROK)
      {
	 DU_LOG("\nERROR  --> DU APP : Memory allocation failed for cell stop req pack");
	 return RFAILED;
      }

      /* pack the address of the structure */
      CMCHKPK(oduPackPointer,(PTR)cellId, mBuf);

      return ODU_POST_TASK(pst,mBuf);
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Unpacks cell stop request from DU APP 
 *
 * @details
 *
 *    Function : unpackMaCellStop 
 *
 *    Functionality:
 *       Unpacks cell stop request from DU APP 
 *
 * @params[in] Handler function pointer
 *             Post structure pointer
 *             Message Buffer
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t unpackMacCellStop(DuMacCellStop func, Pst *pst, Buffer *mBuf)
{
   OduCellId *cellId;
   
   if(pst->selector == ODU_SELECTOR_LWLC)
   {
      /* unpack the address of the structure */
      CMCHKUNPK(oduUnpackPointer, (PTR *)&cellId, mBuf);
      ODU_PUT_MSG_BUF(mBuf);
      return (*func)(pst, cellId);
   }
   else
   {
      /* Nothing to do for loose coupling */
      ODU_PUT_MSG_BUF(mBuf);
      return ROK;
   }
}

/*******************************************************************
 *
 * @brief Packs and Sends cell up ind from MAC to DUAPP
 *
 * @details
 *
 *    Function : packMacCellUpInd
 *
 *    Functionality:
 *       Packs and Sends cell up ind from MAC to DUAPP
 *
 * @params[in] Post structure pointer
 *             Cell Id
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t packMacCellUpInd(Pst *pst, OduCellId *cellId)
{
   Buffer *mBuf = NULLP;

   if (ODU_GET_MSG_BUF(pst->region, pst->pool, &mBuf) != ROK)
   {
      DU_LOG("\nERROR  --> DU APP : Memory allocation failed for packMacCellUpInd");
      return RFAILED;
   }

   if(pst->selector == ODU_SELECTOR_LC)
   {
      CMCHKPK(oduUnpackUInt16, cellId->cellId, mBuf);
      CM_FREE_SHRABL_BUF(pst->region, pst->pool, cellId, sizeof(OduCellId));
      cellId = NULL;
   }
   else if(pst->selector == ODU_SELECTOR_LWLC)
   {
      /* pack the address of the structure */
      CMCHKPK(oduPackPointer,(PTR)cellId, mBuf);
   }
   else
   {
      ODU_PUT_MSG_BUF(mBuf);
   }

   return ODU_POST_TASK(pst, mBuf);
}

/*******************************************************************
 *
 * @brief Unpacks cell up indication from MAC
 *
 * @details
 *
 *    Function : unpackMacCellUpInd
 *
 *    Functionality:
 *         Unpacks cell up indication from MAC
 *
 * @params[in] Pointer to Handler
 *             Post structure pointer
 *             Message Buffer
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t unpackMacCellUpInd(DuMacCellUpInd func, Pst *pst, Buffer *mBuf)
{
   if(pst->selector == ODU_SELECTOR_LWLC)
   {
      OduCellId *cellId;

      /* unpack the address of the structure */
      CMCHKUNPK(oduUnpackPointer, (PTR *)&cellId, mBuf);
      ODU_PUT_MSG_BUF(mBuf);
      return (*func)(pst, cellId);
   }
   else if(pst->selector == ODU_SELECTOR_LC)
   {
      OduCellId cellId;
      CMCHKUNPK(oduPackUInt16, &cellId.cellId, mBuf);
      ODU_PUT_MSG_BUF(mBuf);
      return (*func)(pst, &cellId);
   }
   else
   {
      /* Nothing to do for loose coupling */
      ODU_PUT_MSG_BUF(mBuf);
      return ROK;
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Packs and Sends stop ind from MAC to DUAPP
 *
 * @details
 *
 *    Function : packMacStopInd
 *
 *    Functionality:
 *       Packs and Sends stop ind from MAC to DUAPP
 *
 * @params[in] Post structure pointer
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t packMacStopInd(Pst *pst, OduCellId *cellId)
{
   Buffer *mBuf = NULLP;

   if (ODU_GET_MSG_BUF(pst->region, pst->pool, &mBuf) != ROK)
   {
      DU_LOG("\nERROR  --> DU APP : Memory allocation failed for stop Ind pack");
      return RFAILED;
   }

   if(pst->selector == ODU_SELECTOR_LC)
   {
      /*pack the payload here*/
      CMCHKPK(oduUnpackUInt16, cellId->cellId, mBuf);
      CM_FREE_SHRABL_BUF(pst->region, pst->pool, cellId, sizeof(OduCellId));
      cellId = NULL;
   }
   else if(pst->selector == ODU_SELECTOR_LWLC)
   {
      /* pack the address of the structure */
      CMCHKPK(oduPackPointer,(PTR)cellId, mBuf);
   }
   else
   {
      ODU_PUT_MSG_BUF(mBuf);
   }

   return ODU_POST_TASK(pst,mBuf);
}

/*******************************************************************
 *
 * @brief Unpacks stop indication from MAC
 *
 * @details
 *
 *    Function : unpackMacStopInd
 *
 *    Functionality:
 *         Unpacks stop indication from MAC
 *
 * @params[in] Pointer to Handler
 *             Post structure pointer
 *             Message Buffer
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t unpackMacStopInd(DuMacStopInd func, Pst *pst, Buffer *mBuf)
{
   if(pst->selector == ODU_SELECTOR_LWLC)
   {
      OduCellId *cellId=NULLP;
      /* unpack the address of the structure */
      CMCHKUNPK(oduUnpackPointer, (PTR *)&cellId, mBuf);
      ODU_PUT_MSG_BUF(mBuf);
      return (*func)(pst, cellId);
   }
   else if(pst->selector == ODU_SELECTOR_LC)
   {
      OduCellId cellId;
      CMCHKUNPK(oduPackUInt16, &(cellId.cellId), mBuf);

      ODU_PUT_MSG_BUF(mBuf);
      return (*func)(pst, &cellId);

   }
   else
   {
      /* Nothing to do for loose coupling */
      ODU_PUT_MSG_BUF(mBuf);
      return ROK;
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Packs and Sends UL CCCH Ind from MAC to DUAPP
 *
 * @details
 *
 *    Function : packMacUlCcchInd
 *
 *    Functionality:
 *       Packs and Sends UL CCCH Ind from MAC to DUAPP
 *
 * @params[in] Post structure pointer
 *             UL CCCH Ind pointer              
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t packMacUlCcchInd(Pst *pst, UlCcchIndInfo *ulCcchIndInfo)
{
   Buffer *mBuf = NULLP;

   if(pst->selector == ODU_SELECTOR_LWLC)
   {
      if (ODU_GET_MSG_BUF(pst->region, pst->pool, &mBuf) != ROK)
      {
	 DU_LOG("\nERROR  --> MAC : Memory allocation failed at packMacUlCcchInd");
	 return RFAILED;
      }
      /* pack the address of the structure */
      CMCHKPK(oduPackPointer,(PTR)ulCcchIndInfo, mBuf);
   }
   else
   {
      DU_LOG("\nERROR  -->  MAC: Only LWLC supported for UL CCCH Ind ");
      return RFAILED;
   }

   return ODU_POST_TASK(pst,mBuf);
}

/*******************************************************************
 *
 * @brief Unpacks UL CCCH indication from MAC
 *
 * @details
 *
 *    Function : unpackMacUlCcchInd
 *
 *    Functionality:
 *         Unpacks UL CCCH indication from MAC
 *
 * @params[in] Pointer to Handler
 *             Post structure pointer
 *             Message Buffer
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t unpackMacUlCcchInd(DuMacUlCcchInd func, Pst *pst, Buffer *mBuf)
{
   if(pst->selector == ODU_SELECTOR_LWLC)
   {
      UlCcchIndInfo *ulCcchIndInfo;

      /* unpack the address of the structure */
      CMCHKUNPK(oduUnpackPointer, (PTR *)&ulCcchIndInfo, mBuf);
      ODU_PUT_MSG_BUF(mBuf);
      return (*func)(pst, ulCcchIndInfo);
   }
   else
   {
      /* Nothing to do for other selectors */
      DU_LOG("\nERROR  -->  MAC : Only LWLC supported for UL CCCH Ind ");
      ODU_PUT_MSG_BUF(mBuf);
   }
   return RFAILED;
}

/*******************************************************************
 *
 * @brief Packs and Sends DL CCCH Ind from DUAPP to MAC
 *
 * @details
 *
 *    Function : packMacDlCcchInd
 *
 *    Functionality:
 *       Packs and Sends DL CCCH Ind from DUAPP to MAC
 *
 *
 * @params[in] Post structure pointer
 *             DL CCCH Ind pointer              
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t packMacDlCcchInd(Pst *pst, DlCcchIndInfo *dlCcchIndInfo)
{
   Buffer *mBuf = NULLP;

   if(pst->selector == ODU_SELECTOR_LWLC)
   {
      if (ODU_GET_MSG_BUF(pst->region, pst->pool, &mBuf) != ROK)
      {
	 DU_LOG("\nERROR  --> MAC : Memory allocation failed at packMacDlCcchInd");
	 return RFAILED;
      }
      /* pack the address of the structure */
      CMCHKPK(oduPackPointer,(PTR)dlCcchIndInfo, mBuf);
   }
   else
   {
      DU_LOG("\nERROR  -->  DU APP : Only LWLC supported for DL CCCH Ind ");
      return RFAILED;
   }

   return ODU_POST_TASK(pst,mBuf);
}

/*******************************************************************
 *
 * @brief Unpacks DL CCCH indication from DU APP
 *
 * @details
 *
 *    Function : unpackMacDlCcchInd
 *
 *    Functionality:
 *         Unpacks DL CCCH indication from DU APP
 *
 * @params[in] Pointer to Handler
 *             Post structure pointer
 *             Message Buffer
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t unpackMacDlCcchInd(DuMacDlCcchInd func, Pst *pst, Buffer *mBuf)
{
   if(pst->selector == ODU_SELECTOR_LWLC)
   {
      DlCcchIndInfo *dlCcchIndInfo;

      /* unpack the address of the structure */
      CMCHKUNPK(oduUnpackPointer, (PTR *)&dlCcchIndInfo, mBuf);
      ODU_PUT_MSG_BUF(mBuf);
      return (*func)(pst, dlCcchIndInfo);
   }
   else
   {
      /* Nothing to do for other selectors */
      DU_LOG("\nERROR  -->  DU APP : Only LWLC supported for DL CCCH Ind ");
      ODU_PUT_MSG_BUF(mBuf);
   }

   return RFAILED;
}

/*******************************************************************
 *
 * @brief Packs and Sends UE create Request from DUAPP to MAC
 *
 * @details
 *
 *    Function : packDuMacUeCreateReq
 *
 *    Functionality:
 *       Packs and Sends UE Create Request from DUAPP to MAC
 *
 *
 * @params[in] Post structure pointer
 *             MacUeCfg pointer              
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t packDuMacUeCreateReq(Pst *pst, MacUeCfg *ueCfg)
{
   Buffer *mBuf = NULLP;

   if(pst->selector == ODU_SELECTOR_LWLC)
   {
      if (ODU_GET_MSG_BUF(pst->region, pst->pool, &mBuf) != ROK)
      {
	 DU_LOG("\nERROR  --> MAC : Memory allocation failed at packDuMacUeCreateReq");
	 return RFAILED;
      }
      /* pack the address of the structure */
      CMCHKPK(oduPackPointer,(PTR)ueCfg, mBuf);
   }
   else
   {
      DU_LOG("\nERROR  -->  MAC: Only LWLC supported for packDuMacUeCreateReq");
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
 *    Function : unpackMacUeCreateReq
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
uint8_t unpackMacUeCreateReq(DuMacUeCreateReq func, Pst *pst, Buffer *mBuf)
{
   if(pst->selector == ODU_SELECTOR_LWLC)
   {
      MacUeCfg *ueCfg;

      /* unpack the address of the structure */
      CMCHKUNPK(oduUnpackPointer, (PTR *)&ueCfg, mBuf);
      ODU_PUT_MSG_BUF(mBuf);
      return (*func)(pst, ueCfg);
   }
   else
   {
      /* Nothing to do for other selectors */
      DU_LOG("\nERROR  -->  DU APP : Only LWLC supported for UE Create Request ");
      ODU_PUT_MSG_BUF(mBuf);
   }

   return RFAILED;
}

/*******************************************************************
 *
 * @brief Pack and send UE config response from MAC to DU APP
 *
 * @details
 *
 *    Function : packDuMacUeCfgRsp
 *
 *    Functionality:
 *       Pack and send UE config response from MAC to DU APP
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t packDuMacUeCfgRsp(Pst *pst, MacUeCfgRsp *cfgRsp)
{
   Buffer *mBuf = NULLP;

   if(pst->selector == ODU_SELECTOR_LWLC)
   {
      if (ODU_GET_MSG_BUF(pst->region, pst->pool, &mBuf) != ROK)
      {
	 DU_LOG("\nERROR  --> MAC : Memory allocation failed at packDuMacUeCfgRsp");
	 return RFAILED;
      }
      /* pack the address of the structure */
      CMCHKPK(oduPackPointer,(PTR)cfgRsp, mBuf);
   }
   else
   {
      DU_LOG("\nERROR  -->  MAC: Only LWLC supported for packDuMacUeCfgRsp");
      return RFAILED;
   }

   return ODU_POST_TASK(pst,mBuf);
}
/*******************************************************************
 *
 * @brief Unpack UE Config Response from MAC to DU APP
 *
 * @details
 *
 *    Function :unpackDuMacUeCfgRsp 
 *
 *    Functionality: Unpack UE Config Response from MAC to DU APP
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t unpackDuMacUeCfgRsp(MacDuUeCfgRspFunc func, Pst *pst, Buffer *mBuf)
{
   if(pst->selector == ODU_SELECTOR_LWLC)
   {
      MacUeCfgRsp *cfgRsp = NULLP;

      /* unpack the address of the structure */
      CMCHKUNPK(oduUnpackPointer, (PTR *)&cfgRsp, mBuf);
      ODU_PUT_MSG_BUF(mBuf);
      return (*func)(pst, cfgRsp);
   }

   ODU_PUT_MSG_BUF(mBuf);
   return RFAILED;
}

/*******************************************************************
 *
 * @brief Packs and Sends UE Reconig Request from DUAPP to MAC
 *
 * @details
 *
 *    Function : packDuMacUeReconfigReq
 *
 *    Functionality:
 *       Packs and Sends UE Reconfig Request from DUAPP to MAC
 *
 *
 * @params[in] Post structure pointer
 *             MacUeCfg pointer              
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t packDuMacUeReconfigReq(Pst *pst, MacUeCfg *ueCfg)
{
   Buffer *mBuf = NULLP;

   if(pst->selector == ODU_SELECTOR_LWLC)
   {
      if (ODU_GET_MSG_BUF(pst->region, pst->pool, &mBuf) != ROK)
      {
	 DU_LOG("\nERROR  --> MAC : Memory allocation failed at packDuMacUeReconfigReq");
	 return RFAILED;
      }
      /* pack the address of the structure */
      CMCHKPK(oduPackPointer,(PTR)ueCfg, mBuf);
   }
   else
   {
      DU_LOG("\nERROR  -->  MAC: Only LWLC supported for packDuMacUeReconfigReq");
      return RFAILED;
   }

   return ODU_POST_TASK(pst,mBuf);
}
/*******************************************************************
 *
 * @brief Unpacks UE Reconfig Request received from DU APP
 *
 * @details
 *
 *    Function : unpackMacUeReconfigReq
 *
 *    Functionality:
 *         Unpacks UE Reconfig Request received from DU APP
 *
 * @params[in] Pointer to Handler
 *             Post structure pointer
 *             Message Buffer
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t unpackMacUeReconfigReq(DuMacUeReconfigReq func, Pst *pst, Buffer *mBuf)
{
   if(pst->selector == ODU_SELECTOR_LWLC)
   {
      MacUeCfg *ueCfg;

      /* unpack the address of the structure */
      CMCHKUNPK(oduUnpackPointer, (PTR *)&ueCfg, mBuf);
      ODU_PUT_MSG_BUF(mBuf);
      return (*func)(pst, ueCfg);
   }
   else
   {
      /* Nothing to do for other selectors */
      DU_LOG("\nERROR  -->  DU APP : Only LWLC supported for UE Create Request ");
      ODU_PUT_MSG_BUF(mBuf);
   }

   return RFAILED;
}
/*******************************************************************
*
* @brief Packs and Sends UE Delete Request from DUAPP to MAC
*
* @details
*
*    Function : packDuMacUeDeleteReq
*
*    Functionality:
*       Packs and Sends UE Delete Request from DUAPP to MAC
*
*
* @params[in] Post structure pointer
*             MacUeDelete pointer
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/
uint8_t packDuMacUeDeleteReq(Pst *pst, MacUeDelete *ueDel)
{
    Buffer *mBuf = NULLP;

    if(pst->selector == ODU_SELECTOR_LWLC)
    {
       if (ODU_GET_MSG_BUF(pst->region, pst->pool, &mBuf) != ROK)
       {
          DU_LOG("\nERROR  --> MAC : Memory allocation failed at packDuMacUeDeleteReq");
          return RFAILED;
       }
       /* pack the address of the structure */
       CMCHKPK(oduPackPointer,(PTR)ueDel, mBuf);
    }
    else
    {
       DU_LOG("\nERROR  -->  MAC: Only LWLC supported for packDuMacUeDeleteReq");
       return RFAILED;
    }

    return ODU_POST_TASK(pst,mBuf);
}
/*******************************************************************
*
* @brief Unpacks UE Delete Request received from DU APP
*
* @details
*
*    Function : unpackMacUeDeleteReq 
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
uint8_t unpackMacUeDeleteReq(DuMacUeDeleteReq func, Pst *pst, Buffer *mBuf)
{
    if(pst->selector == ODU_SELECTOR_LWLC)
    {
       MacUeDelete *ueDelete;

       /* unpack the address of the structure */
       CMCHKUNPK(oduUnpackPointer, (PTR *)&ueDelete, mBuf);
       ODU_PUT_MSG_BUF(mBuf);
       return (*func)(pst, ueDelete);
    }
    else
    {
       /* Nothing to do for other selectors */
       DU_LOG("\nERROR  -->  DU APP : Only LWLC supported for UE Delete Request ");
       ODU_PUT_MSG_BUF(mBuf);
    }

    return RFAILED;
}
/*******************************************************************
 *
 * @brief Pack and send UE delete response from MAC to DU APP
 *
 * @details
 *
 *    Function : packDuMacUeDeleteRsp
 *
 *    Functionality:
 *       Pack and send UE  delete response from MAC to DU APP
 *
 * @params[in]
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t packDuMacUeDeleteRsp(Pst *pst, MacUeDeleteRsp *deleteRsp)
{
   Buffer *mBuf = NULLP;
   
   if(pst->selector == ODU_SELECTOR_LWLC)
   {
      if (ODU_GET_MSG_BUF(pst->region, pst->pool, &mBuf) != ROK)
      {
         DU_LOG("\nERROR  --> MAC : Memory allocation failed at packDuMacUeDeleteRsp");
         return RFAILED;
      }
      /* pack the address of the structure */
      CMCHKPK(oduPackPointer,(PTR)deleteRsp, mBuf);
   }
   else
   {
      DU_LOG("\nERROR  -->  MAC: Only LWLC supported for packDuMacUeDeleteRsp");
      return RFAILED;
   }
   
   return ODU_POST_TASK(pst,mBuf);
   
}

/*******************************************************************
*
* @brief Unpack UE Config Response from MAC to DU APP
*
* @details
*
*    Function :unpackDuMacUeDeleteRsp 
*
*    Functionality: Unpack UE Config Response from MAC to DU APP
*
* @params[in]
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/
uint8_t unpackDuMacUeDeleteRsp(MacDuUeDeleteRspFunc func, Pst *pst, Buffer *mBuf)
{
    if(pst->selector == ODU_SELECTOR_LWLC)
    {
       MacUeDeleteRsp *ueDeleteRsp = NULLP;

       /* unpack the address of the structure */
       CMCHKUNPK(oduUnpackPointer, (PTR *)&ueDeleteRsp, mBuf);
       ODU_PUT_MSG_BUF(mBuf);
       return (*func)(pst, ueDeleteRsp);
    }

    ODU_PUT_MSG_BUF(mBuf);
    return RFAILED;
}

/*******************************************************************
 *
 * @brief Unpacks Cell Delete Request received from DU APP
 *
 * @details
 *
 *    Function : unpackMacCellDeleteReq
 *
 *    Functionality:
 *         Unpacks Cell Delete Request received from DU APP
 *
 * @params[in] Pointer to Handler
 *             Post structure pointer
 *             Message Buffer
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t unpackMacCellDeleteReq(DuMacCellDeleteReq func, Pst *pst, Buffer *mBuf)
{
   if(pst->selector == ODU_SELECTOR_LWLC)
   {
      MacCellDelete *cellDelete=NULLP;

      /* unpack the address of the structure */
      CMCHKUNPK(oduUnpackPointer, (PTR *)&cellDelete, mBuf);
      ODU_PUT_MSG_BUF(mBuf);
      return (*func)(pst, cellDelete);
   }
   else
   {
      /* Nothing to do for other
       * selectors */
      DU_LOG("\nERROR  -->  DU APP : unpackMacCellDeleteReq(): Only LWLC supported for CELL Delete Request ");
      ODU_PUT_MSG_BUF(mBuf);
   }

   return RFAILED;
}

/*******************************************************************
 *
 * @brief Pack and send Cell delete request to MAC
 *
 * @details
 *
 *    Function : packDuMacCellDeleteReq
 *
 *    Functionality:
 *       Pack and send Cell delete request to MAC
 *
 * @params[in] Post structure
 *             MacCellDelete *cellDelete;
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t packDuMacCellDeleteReq(Pst *pst, MacCellDelete *cellDelete)
{
   Buffer *mBuf = NULLP;

   if(pst->selector == ODU_SELECTOR_LWLC)
   {
      if (ODU_GET_MSG_BUF(pst->region, pst->pool, &mBuf) != ROK)
      {
         DU_LOG("\nERROR  --> MAC : packDuMacCellDeleteReq(): Memory allocation failed ");
         return RFAILED;
      }
      CMCHKPK(oduPackPointer,(PTR)cellDelete, mBuf);
      return ODU_POST_TASK(pst,mBuf);
   }
   else
   {
      DU_LOG("\nERROR  -->  MAC: packDuMacCellDeleteReq(): Only LWLC supported ");
   }
   return RFAILED;
}

/*******************************************************************
 *
 * @brief Pack and send CELL delete response from MAC to DU APP
 *
 * @details
 *
 *    Function : packDuMacCellDeleteRsp
 *
 *    Functionality:
 *       Pack and send CELL  delete response from MAC to DU APP
 *
 * @params[in] Pst *pst, MacCellDeleteRsp *deleteRsp
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t packDuMacCellDeleteRsp(Pst *pst, MacCellDeleteRsp *cellDeleteRsp)
{
   Buffer *mBuf = NULLP;

   if(pst->selector == ODU_SELECTOR_LWLC)
   {
      if (ODU_GET_MSG_BUF(pst->region, pst->pool, &mBuf) != ROK)
      {
         DU_LOG("\nERROR  --> MAC : packDuMacCellDeleteRsp(): Memory allocation failed ");
         return RFAILED;
      }
      CMCHKPK(oduPackPointer,(PTR)cellDeleteRsp, mBuf);
   }
   else
   {
      DU_LOG("\nERROR  -->  MAC: packDuMacCellDeleteRsp(): Only LWLC supported ");
      return RFAILED;
   }

   return ODU_POST_TASK(pst,mBuf);

}

/*******************************************************************
 *
 * @brief Unpack cell delete response from MAC to DU APP
 *
 * @details
 *
 *    Function : unpackDuMacCellDeleteRsp
 *
 *    Functionality: Unpack cell delete response from MAC to DU APP
 *
 * @params[in] MacDuCellDeleteRspFunc func, Pst *pst, Buffer *mBuf
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t unpackDuMacCellDeleteRsp(MacDuCellDeleteRspFunc func, Pst *pst, Buffer *mBuf)
{
   if(pst->selector == ODU_SELECTOR_LWLC)
   {
      MacCellDeleteRsp *cellDeleteRsp = NULLP;

      CMCHKUNPK(oduUnpackPointer, (PTR *)&cellDeleteRsp, mBuf);
      ODU_PUT_MSG_BUF(mBuf);
      return (*func)(pst, cellDeleteRsp);
   }
   else
   {  
      DU_LOG("\nERROR  -->  DU APP : unpackDuMacCellDeleteRsp(): Only LWLC supported ");
      ODU_PUT_MSG_BUF(mBuf);
   }
   return RFAILED;
}

/**********************************************************************
  End of file
 **********************************************************************/
