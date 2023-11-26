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
uint8_t packMacCellStart(Pst *pst, CellStartInfo *cellStart)
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
      CMCHKPK(oduPackPointer,(PTR)cellStart, mBuf);

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
   CellStartInfo  *cellStart;

   if(pst->selector == ODU_SELECTOR_LWLC)
   {
      /* unpack the address of the structure */
      CMCHKUNPK(oduUnpackPointer, (PTR *)&cellStart, mBuf);
      ODU_PUT_MSG_BUF(mBuf); 
      return (*func)(pst, cellStart);
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
uint8_t packMacCellStop(Pst *pst, CellStopInfo *cellStop)
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
      CMCHKPK(oduPackPointer,(PTR)cellStop, mBuf);

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
   CellStopInfo *cellStop;
   
   if(pst->selector == ODU_SELECTOR_LWLC)
   {
      /* unpack the address of the structure */
      CMCHKUNPK(oduUnpackPointer, (PTR *)&cellStop, mBuf);
      ODU_PUT_MSG_BUF(mBuf);
      return (*func)(pst, cellStop);
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
 *             MacUeCreateReq pointer              
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t packDuMacUeCreateReq(Pst *pst, MacUeCreateReq *ueCfg)
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
      MacUeCreateReq *ueCfg;

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
 * @brief Pack and send UE create response from MAC to DU APP
 *
 * @details
 *
 *    Function : packDuMacUeCreateRsp
 *
 *    Functionality:
 *       Pack and send UE create response from MAC to DU APP
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t packDuMacUeCreateRsp(Pst *pst, MacUeCreateRsp *cfgRsp)
{
   Buffer *mBuf = NULLP;

   if(pst->selector == ODU_SELECTOR_LWLC)
   {
      if (ODU_GET_MSG_BUF(pst->region, pst->pool, &mBuf) != ROK)
      {
	 DU_LOG("\nERROR  --> MAC : Memory allocation failed at packDuMacUeCreateRsp");
	 return RFAILED;
      }
      /* pack the address of the structure */
      CMCHKPK(oduPackPointer,(PTR)cfgRsp, mBuf);
   }
   else
   {
      DU_LOG("\nERROR  -->  MAC: Only LWLC supported for packDuMacUeCreateRsp");
      return RFAILED;
   }

   return ODU_POST_TASK(pst,mBuf);
}
/*******************************************************************
 *
 * @brief Unpack UE Create Response from MAC to DU APP
 *
 * @details
 *
 *    Function :unpackDuMacUeCreateRsp 
 *
 *    Functionality: Unpack UE Create Response from MAC to DU APP
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t unpackDuMacUeCreateRsp(MacDuUeCreateRspFunc func, Pst *pst, Buffer *mBuf)
{
   if(pst->selector == ODU_SELECTOR_LWLC)
   {
      MacUeCreateRsp *cfgRsp = NULLP;

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
 *             MacUeRecfg pointer              
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t packDuMacUeReconfigReq(Pst *pst, MacUeRecfg *ueCfg)
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
      MacUeRecfg *ueRecfg;

      /* unpack the address of the structure */
      CMCHKUNPK(oduUnpackPointer, (PTR *)&ueRecfg, mBuf);
      ODU_PUT_MSG_BUF(mBuf);
      return (*func)(pst, ueRecfg);
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
 * @brief Pack and send UE Reconfig response from MAC to DU APP
 *
 * @details
 *
 *    Function : packDuMacUeRecfgRsp
 *
 *    Functionality:
 *       Pack and send UE Reconfig response from MAC to DU APP
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t packDuMacUeRecfgRsp(Pst *pst, MacUeRecfgRsp *recfgRsp)
{
   Buffer *mBuf = NULLP;

   if(pst->selector == ODU_SELECTOR_LWLC)
   {
      if (ODU_GET_MSG_BUF(pst->region, pst->pool, &mBuf) != ROK)
      {
         DU_LOG("\nERROR  --> MAC : Memory allocation failed at packDuMacUeRecfgRsp");
         return RFAILED;
      }
      /* pack the address of the structure */
      CMCHKPK(oduPackPointer,(PTR)recfgRsp, mBuf);
   }
   else
   {
      DU_LOG("\nERROR  -->  MAC: Only LWLC supported for packDuMacUeRecfgRsp");
      return RFAILED;
   }

   return ODU_POST_TASK(pst,mBuf);
}

/*******************************************************************
 *
 * @brief Unpack UE ReConfig Response from MAC to DU APP
 *
 * @details
 *
 *    Function :unpackDuMacUeRecfgRsp 
 *
 *    Functionality: Unpack UE ReConfig Response from MAC to DU APP
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t unpackDuMacUeRecfgRsp(MacDuUeRecfgRspFunc func, Pst *pst, Buffer *mBuf)
{
   if(pst->selector == ODU_SELECTOR_LWLC)
   {
      MacUeRecfgRsp *recfgRsp = NULLP;

      /* unpack the address of the structure */
      CMCHKUNPK(oduUnpackPointer, (PTR *)&recfgRsp, mBuf);
      ODU_PUT_MSG_BUF(mBuf);
      return (*func)(pst, recfgRsp);
   }

   ODU_PUT_MSG_BUF(mBuf);
   return RFAILED;
}

/*******************************************************************
 *
 * @brief Packs and Sends RACH Resource request from DUAPP to MAC
 *
 * @details
 *
 *    Function : packDuMacRachRsrcReq
 *
 *    Functionality:
 *       Packs and Sends RACH Resource request from DU APP to MAC
 *
 *
 * @params[in] Post structure pointer
 *             MacRachRsrcReq pointer              
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t packDuMacRachRsrcReq(Pst *pst, MacRachRsrcReq *rachRsrcReq)
{
   Buffer *mBuf = NULLP;

   if(pst->selector == ODU_SELECTOR_LWLC)
   {
      if (ODU_GET_MSG_BUF(pst->region, pst->pool, &mBuf) != ROK)
      {
         DU_LOG("\nERROR  --> MAC : Memory allocation failed at packDuMacRachRsrcReq,");
         return RFAILED;
      }
      /* pack the address of the structure */
      CMCHKPK(oduPackPointer, (PTR)rachRsrcReq, mBuf);
   }
   else
   {
      DU_LOG("\nERROR  -->  MAC: Only LWLC supported for packDuMacRachRsrcReq");
      return RFAILED;
   }

   return ODU_POST_TASK(pst,mBuf);
}

/*******************************************************************
 *
 * @brief Unpacks RACH resource Request received from DU APP
 *
 * @details
 *
 *    Function : unpackMacRachRsrcReq
 *
 *    Functionality:
 *         Unpacks RACH resource Request received from DU APP
 *
 * @params[in] Pointer to Handler
 *             Post structure pointer
 *             Message Buffer
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t unpackMacRachRsrcReq(DuMacRachRsrcReq func, Pst *pst, Buffer *mBuf)
{
   if(pst->selector == ODU_SELECTOR_LWLC)
   {
      MacRachRsrcReq *rachRsrcReq;

      /* unpack the address of the structure */
      CMCHKUNPK(oduUnpackPointer, (PTR *)&rachRsrcReq, mBuf);
      ODU_PUT_MSG_BUF(mBuf);
      return (*func)(pst, rachRsrcReq);
   }
   else
   {
      /* Nothing to do for other selectors */
      DU_LOG("\nERROR  -->  DU APP : Only LWLC supported for RACH resource Request ");
      ODU_PUT_MSG_BUF(mBuf);
   }

   return RFAILED;
}

/*******************************************************************
 *
 * @brief Packs and Sends RACH Resource release from DUAPP to MAC
 *
 * @details
 *
 *    Function : packDuMacRachRsrcRel
 *
 *    Functionality:
 *       Packs and Sends RACH Resource release from DU APP to MAC
 *
 *
 * @params[in] Post structure pointer
 *             MacRachRsrcRel pointer              
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t packDuMacRachRsrcRel(Pst *pst, MacRachRsrcRel *rachRsrcRel)
{
   Buffer *mBuf = NULLP;

   if(pst->selector == ODU_SELECTOR_LWLC)
   {
      if (ODU_GET_MSG_BUF(pst->region, pst->pool, &mBuf) != ROK)
      {
         DU_LOG("\nERROR  --> MAC : Memory allocation failed at packDuMacRachRsrcRel");
         return RFAILED;
      }
      /* pack the address of the structure */
      CMCHKPK(oduPackPointer, (PTR)rachRsrcRel, mBuf);
   }
   else
   {
      DU_LOG("\nERROR  -->  MAC: Only LWLC supported for packDuMacRachRsrcRel");
      return RFAILED;
   }

   return ODU_POST_TASK(pst,mBuf);
}

/*******************************************************************
 *
 * @brief Unpacks RACH Resource Release received from DU APP
 *
 * @details
 *
 *    Function : unpackMacRachRsrcRel
 *
 *    Functionality:
 *         Unpacks RACH Resource Release received from DU APP
 *
 * @params[in] Pointer to Handler
 *             Post structure pointer
 *             Message Buffer
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t unpackMacRachRsrcRel(DuMacRachRsrcRel func, Pst *pst, Buffer *mBuf)
{
   if(pst->selector == ODU_SELECTOR_LWLC)
   {
      MacRachRsrcRel *rachRsrcRel;

      /* unpack the address of the structure */
      CMCHKUNPK(oduUnpackPointer, (PTR *)&rachRsrcRel, mBuf);
      ODU_PUT_MSG_BUF(mBuf);
      return (*func)(pst, rachRsrcRel);
   }
   else
   {
      /* Nothing to do for other selectors */
      DU_LOG("\nERROR  -->  DU APP : Only LWLC supported for RACH Resource Release ");
      ODU_PUT_MSG_BUF(mBuf);
   }

   return RFAILED;
}

/*******************************************************************
 *
 * @brief Packs and Sends RACH Resource response from MAC to DU APP
 *
 * @details
 *
 *    Function : packDuMacRachRsrcRsp
 *
 *    Functionality:
 *       Packs and Sends RACH Resource response from MAC to DU APP
 *
 *
 * @params[in] Post structure pointer
 *             MacRachRsrcRsp pointer              
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t packDuMacRachRsrcRsp(Pst *pst, MacRachRsrcRsp *rachRsrcRsp)
{
   Buffer *mBuf = NULLP;

   if(pst->selector == ODU_SELECTOR_LWLC)
   {
      if (ODU_GET_MSG_BUF(pst->region, pst->pool, &mBuf) != ROK)
      {
         DU_LOG("\nERROR  --> MAC : Memory allocation failed at packDuMacRachRsrcRsp");
         return RFAILED;
      }
      /* pack the address of the structure */
      CMCHKPK(oduPackPointer, (PTR)rachRsrcRsp, mBuf);
   }
   else
   {
      DU_LOG("\nERROR  -->  MAC: Only LWLC supported for packDuMacRachRsrcRsp");
      return RFAILED;
   }

   return ODU_POST_TASK(pst,mBuf);
}

/*******************************************************************
 *
 * @brief Unpacks RACH resource Response received from MAC
 *
 * @details
 *
 *    Function : unpackDuMacRachRsrcRsp
 *
 *    Functionality:
 *         Unpacks RACH resource Response received from MAC
 *
 * @params[in] Pointer to Handler
 *             Post structure pointer
 *             Message Buffer
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t unpackDuMacRachRsrcRsp(MacDuRachRsrcRspFunc func, Pst *pst, Buffer *mBuf)
{
   if(pst->selector == ODU_SELECTOR_LWLC)
   {
      MacRachRsrcRsp *rachRsrcRsp;

      /* unpack the address of the structure */
      CMCHKUNPK(oduUnpackPointer, (PTR *)&rachRsrcRsp, mBuf);
      ODU_PUT_MSG_BUF(mBuf);
      return (*func)(pst, rachRsrcRsp);
   }
   else
   {
      /* Nothing to do for other selectors */
      DU_LOG("\nERROR  -->  DU APP : Only LWLC supported for RACH resource Response ");
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
      MacCellDeleteReq *cellDelete=NULLP;

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

uint8_t packDuMacCellDeleteReq(Pst *pst, MacCellDeleteReq *cellDelete)
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

/*******************************************************************
 *
 * @brief Pack and send Slice Cfg request from MAC to DU APP
 *
 * @details
 *
 *    Function : packDuMacSliceCfgReq
 *
 *    Functionality:
 *       Pack and send Slice Cfg request from MAC to DU APP
 *
 * @params[in] Pst *pst, MacSliceCfgReq *sliceCfgReq
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t packDuMacSliceCfgReq(Pst *pst, MacSliceCfgReq *sliceCfgReq)
{
   Buffer *mBuf = NULLP;

   if(pst->selector == ODU_SELECTOR_LWLC)
   {
      if (ODU_GET_MSG_BUF(pst->region, pst->pool, &mBuf) != ROK)
      {
         DU_LOG("\nERROR  --> MAC : Memory allocation failed in packDuMacSliceCfgReq");
         return RFAILED;
      }
      CMCHKPK(oduPackPointer,(PTR)sliceCfgReq, mBuf);
   }
   else
   {
      DU_LOG("\nERROR  -->  MAC: Only LWLC supported in packDuMacSliceCfgReq");
      return RFAILED;
   }

   return ODU_POST_TASK(pst,mBuf);

}
/*******************************************************************
*
* @brief Unpacks Slice Cfg request received from DU APP
*
* @details
*
*    Function : unpackMacSliceCfgReq 
*
*    Functionality:
*         Unpacks Slice Cfg Request received from DU APP
*
* @params[in] Pointer to Handler
*             Post structure pointer
*             Message Buffer
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/
uint8_t unpackMacSliceCfgReq(DuMacSliceCfgReq func, Pst *pst, Buffer *mBuf)
{
    if(pst->selector == ODU_SELECTOR_LWLC)
    {
       MacSliceCfgReq *sliceCfgReq;
       /* unpack the address of the structure */
       CMCHKUNPK(oduUnpackPointer, (PTR *)&sliceCfgReq, mBuf);
       ODU_PUT_MSG_BUF(mBuf);
       return (*func)(pst, sliceCfgReq);
    }
    else
    {
       /* Nothing to do for other selectors */
       DU_LOG("\nERROR  -->  DU APP : Only LWLC supported for Slice Cfg Request ");
       ODU_PUT_MSG_BUF(mBuf);
    }

    return RFAILED;
}

/*******************************************************************
 *
 * @brief Pack and send Slice config response from MAC to DU APP
 *
 * @details
 *
 *    Function : packDuMacSliceCfgRsp
 *
 *    Functionality:
 *       Pack and send Slice config response from MAC to DU APP
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t packDuMacSliceCfgRsp(Pst *pst, MacSliceCfgRsp *cfgRsp)
{
   Buffer *mBuf = NULLP;

   if(pst->selector == ODU_SELECTOR_LWLC)
   {
      if (ODU_GET_MSG_BUF(pst->region, pst->pool, &mBuf) != ROK)
      {
         DU_LOG("\nERROR  --> MAC : Memory allocation failed at packDuMacSliceCfgRsp");
         return RFAILED;
      }
      /* pack the address of the structure */
      CMCHKPK(oduPackPointer,(PTR)cfgRsp, mBuf);
   }
   else
   {
      DU_LOG("\nERROR  -->  MAC: Only LWLC supported for packDuMacSliceCfgRsp");
      return RFAILED;
   }

   return ODU_POST_TASK(pst,mBuf);
}

/*******************************************************************
 *
 * @brief Unpack Slice Config Response from MAC to DU APP
 *
 * @details
 *
 *    Function :unpackDuMacSliceCfgRsp 
 *
 *    Functionality: Unpack Slice Config Response from MAC to DU APP
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t unpackDuMacSliceCfgRsp(MacDuSliceCfgRspFunc func, Pst *pst, Buffer *mBuf)
{
   if(pst->selector == ODU_SELECTOR_LWLC)
   {
      MacSliceCfgRsp *cfgRsp = NULLP;

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
 * @brief Pack and send Slice ReCfg request from MAC to DU APP
 *
 * @details
 *
 *    Function : packDuMacSliceRecfgReq
 *
 *    Functionality:
 *       Pack and send Slice ReCfg request from MAC to DU APP
 *
 * @params[in] Pst *pst, MacSliceRecfgReq *sliceRecfgReq
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t packDuMacSliceRecfgReq(Pst *pst, MacSliceRecfgReq *sliceRecfgReq)
{
   Buffer *mBuf = NULLP;

   if(pst->selector == ODU_SELECTOR_LWLC)
   {
      if (ODU_GET_MSG_BUF(pst->region, pst->pool, &mBuf) != ROK)
      {
         DU_LOG("\nERROR  --> MAC : Memory allocation failed in packDuMacSliceRecfgReq");
         return RFAILED;
      }
      CMCHKPK(oduPackPointer,(PTR)sliceRecfgReq, mBuf);
   }
   else
   {
      DU_LOG("\nERROR  -->  MAC: Only LWLC supported in packDuMacSliceRecfgReq");
      return RFAILED;
   }

   return ODU_POST_TASK(pst,mBuf);

}
/*******************************************************************
*
* @brief Unpacks Slice ReCfg request received from DU APP
*
* @details
*
*    Function : unpackMacSliceCfgReq 
*
*    Functionality:
*         Unpacks Slice ReCfg Request received from DU APP
*
* @params[in] Pointer to Handler
*             Post structure pointer
*             Message Buffer
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/
uint8_t unpackMacSliceRecfgReq(DuMacSliceRecfgReq func, Pst *pst, Buffer *mBuf)
{
    if(pst->selector == ODU_SELECTOR_LWLC)
    {
       MacSliceRecfgReq *sliceRecfgReq;
       /* unpack the address of the structure */
       CMCHKUNPK(oduUnpackPointer, (PTR *)&sliceRecfgReq, mBuf);
       ODU_PUT_MSG_BUF(mBuf);
       return (*func)(pst, sliceRecfgReq);
    }
    else
    {
       /* Nothing to do for other selectors */
       DU_LOG("\nERROR  -->  DU APP : Only LWLC supported for Slice ReCfg Request ");
       ODU_PUT_MSG_BUF(mBuf);
    }

    return RFAILED;
}

/*******************************************************************
 *
 * @brief Pack and send Slice config response from MAC to DU APP
 *
 * @details
 *
 *    Function : packDuMacSliceRecfgRsp
 *
 *    Functionality:
 *       Pack and send Slice config response from MAC to DU APP
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t packDuMacSliceRecfgRsp(Pst *pst, MacSliceRecfgRsp *sliceRecfgRsp)
{
   Buffer *mBuf = NULLP;

   if(pst->selector == ODU_SELECTOR_LWLC)
   {
      if (ODU_GET_MSG_BUF(pst->region, pst->pool, &mBuf) != ROK)
      {
         DU_LOG("\nERROR  --> MAC : Memory allocation failed at packDuMacSliceRecfgRsp");
         return RFAILED;
      }
      /* pack the address of the structure */
      CMCHKPK(oduPackPointer,(PTR)sliceRecfgRsp, mBuf);
   }
   else
   {
      DU_LOG("\nERROR  -->  MAC: Only LWLC supported for packDuMacSliceRecfgRsp");
      return RFAILED;
   }

   return ODU_POST_TASK(pst,mBuf);
}

/*******************************************************************
 *
 * @brief Unpack Slice ReConfig Response from MAC to DU APP
 *
 * @details
 *
 *    Function :unpackDuMacSliceRecfgRsp 
 *
 *    Functionality: Unpack Slice ReConfig Response from MAC to DU APP
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t unpackDuMacSliceRecfgRsp(MacDuSliceRecfgRspFunc func, Pst *pst, Buffer *mBuf)
{
   if(pst->selector == ODU_SELECTOR_LWLC)
   {
      MacSliceRecfgRsp *sliceRecfgRsp = NULLP;

      /* unpack the address of the structure */
      CMCHKUNPK(oduUnpackPointer, (PTR *)&sliceRecfgRsp, mBuf);
      ODU_PUT_MSG_BUF(mBuf);
      return (*func)(pst, sliceRecfgRsp);
   }

   ODU_PUT_MSG_BUF(mBuf);
   return RFAILED;
}

/*******************************************************************
 *
 * @brief Pack and send Slot ind from MAC to DU APP
 *
 * @details
 *
 *    Function : packMacSlotInd
 *
 *    Functionality:
 *       Pack and send Slot ind from MAC to DU APP
 *
 * @params[in] Pst *pst, SlotTimingInfo *slotIndInfo 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t packMacSlotInd(Pst *pst, SlotTimingInfo *slotIndInfo)
{
   Buffer *mBuf = NULLP;

   if(pst->selector == ODU_SELECTOR_LWLC)
   {
      if (ODU_GET_MSG_BUF(pst->region, pst->pool, &mBuf) != ROK)
      {
         DU_LOG("\nERROR  --> MAC : Memory allocation failed at packMacSlotInd");
         return RFAILED;
      }
      /* pack the address of the structure */
      CMCHKPK(oduPackPointer,(PTR)slotIndInfo, mBuf);
   }
   else
   {
      DU_LOG("\nERROR  -->  MAC: Only LWLC supported for packMacSlotInd");
      return RFAILED;
   }

   return ODU_POST_TASK(pst,mBuf);
}

/*******************************************************************
 *
 * @brief Unpack Slot indication from MAC to DU APP
 *
 * @details
 *
 *    Function :unpackDuMacSlotInd 
 *
 *    Functionality: Unpack Slot Indication from MAC to DU APP
 *
 * @params[in] DuMacSlotInd func, Pst *pst, Buffer *mBuf 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t unpackDuMacSlotInd(DuMacSlotInd func, Pst *pst, Buffer *mBuf)
{
   if(pst->selector == ODU_SELECTOR_LWLC)
   {
      SlotTimingInfo *slotIndInfo;
      
      /* unpack the address of the structure */
      CMCHKUNPK(oduUnpackPointer, (PTR *)&slotIndInfo, mBuf);
      ODU_PUT_MSG_BUF(mBuf);
      return (*func)(pst, slotIndInfo);
   }

   ODU_PUT_MSG_BUF(mBuf);
   return RFAILED;
}

/*******************************************************************
 *
 * @brief Pack and send Dl Pcch indication to MAC
 *
 * @details
 *
 *    Function : packDuMacDlPcchInd
 *
 *    Functionality:
 *       Pack and send Dl Pcch indication to MAC
 *
 * @params[in] Post structure
 *             DlPcchInd *pcchInd;
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t packDuMacDlPcchInd(Pst *pst, DlPcchInd *pcchInd)
{
   Buffer *mBuf = NULLP;

   if(pst->selector == ODU_SELECTOR_LWLC)
   {
      if (ODU_GET_MSG_BUF(pst->region, pst->pool, &mBuf) != ROK)
      {
         DU_LOG("\nERROR  --> MAC : packDuMacDlPcchInd(): Memory allocation failed ");
         return RFAILED;
      }
      CMCHKPK(oduPackPointer,(PTR)pcchInd, mBuf);
      return ODU_POST_TASK(pst,mBuf);
   }
   else
   {
      DU_LOG("\nERROR  -->  MAC: packDuMacDlPcchInd(): Only LWLC supported ");
   }
   return RFAILED;
}

/*******************************************************************
 *
 * @brief Unpacks downlink pcch indication received from DU APP
 *
 * @details
 *
 *    Function : unpackMacDlPcchInd
 *
 *    Functionality:
 *         Unpacks downlink pcch indication received from DU APP
 *
 * @params[in] Pointer to Handler
 *             Post structure pointer
 *             Message Buffer
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t unpackMacDlPcchInd(DuMacDlPcchInd func, Pst *pst, Buffer *mBuf)
{
   if(pst->selector == ODU_SELECTOR_LWLC)
   {
      DlPcchInd *pcchInd=NULLP;

      /* unpack the address of the structure */
      CMCHKUNPK(oduUnpackPointer, (PTR *)&pcchInd, mBuf);
      ODU_PUT_MSG_BUF(mBuf);
      return (*func)(pst, pcchInd);
   }
   else
   {
      /* Nothing to do for other selectors */
      DU_LOG("\nERROR  -->  DU APP : unpackMacDlPcchInd(): Only LWLC supported");
      ODU_PUT_MSG_BUF(mBuf);
   }

   return RFAILED;
}

/*******************************************************************
*
* @brief Packs and Sends UE Reset Request from DUAPP to MAC
*
* @details
*
*    Function : packDuMacUeResetReq
*
*    Functionality:
*       Packs and Sends UE Reset Request from DUAPP to MAC
*
*
* @params[in] Post structure pointer
*             MacUeResetReq pointer
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/
uint8_t packDuMacUeResetReq(Pst *pst, MacUeResetReq *ueDel)
{
    Buffer *mBuf = NULLP;

    if(pst->selector == ODU_SELECTOR_LWLC)
    {
       if (ODU_GET_MSG_BUF(pst->region, pst->pool, &mBuf) != ROK)
       {
          DU_LOG("\nERROR  --> MAC : Memory allocation failed at packDuMacUeResetReq");
          return RFAILED;
       }
       /* pack the address of the structure */
       CMCHKPK(oduPackPointer,(PTR)ueDel, mBuf);
    }
    else
    {
       DU_LOG("\nERROR  -->  MAC: Only LWLC supported for packDuMacUeResetReq");
       return RFAILED;
    }
    return ODU_POST_TASK(pst,mBuf);
}

/*******************************************************************
*
* @brief Unpacks UE Reset Request received from DU APP
*
* @details
*
*    Function : unpackMacUeResetReq 
*
*    Functionality:
*         Unpacks UE Reset Request received from DU APP
*
* @params[in] Pointer to Handler
*             Post structure pointer
*             Message Buffer
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/
uint8_t unpackMacUeResetReq(DuMacUeResetReq func, Pst *pst, Buffer *mBuf)
{
    if(pst->selector == ODU_SELECTOR_LWLC)
    {
       MacUeResetReq *ueReset;

       /* unpack the address of the structure */
       CMCHKUNPK(oduUnpackPointer, (PTR *)&ueReset, mBuf);
       ODU_PUT_MSG_BUF(mBuf);
       return (*func)(pst, ueReset);
    }
    else
    {
       /* Nothing to do for other selectors */
       DU_LOG("\nERROR  -->  DU APP : Only LWLC supported for UE Reset Request ");
       ODU_PUT_MSG_BUF(mBuf);
    }

    return RFAILED;
}

/*******************************************************************
 *
 * @brief Pack and send UE reset response from MAC to DU APP
 *
 * @details
 *
 *    Function : packDuMacUeResetRsp
 *
 *    Functionality:
 *       Pack and send UE  reset response from MAC to DU APP
 *
 * @params[in]
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t packDuMacUeResetRsp(Pst *pst, MacUeResetRsp *resetRsp)
{
   Buffer *mBuf = NULLP;
   
   if(pst->selector == ODU_SELECTOR_LWLC)
   {
      if (ODU_GET_MSG_BUF(pst->region, pst->pool, &mBuf) != ROK)
      {
         DU_LOG("\nERROR  --> MAC : Memory allocation failed at packDuMacUeResetRsp");
         return RFAILED;
      }
      /* pack the address of the structure */
      CMCHKPK(oduPackPointer,(PTR)resetRsp, mBuf);
   }
   else
   {
      DU_LOG("\nERROR  -->  MAC: Only LWLC supported for packDuMacUeResetRsp");
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
*    Function :unpackDuMacUeResetRsp 
*
*    Functionality: Unpack UE Config Response from MAC to DU APP
*
* @params[in]
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/
uint8_t unpackDuMacUeResetRsp(MacDuUeResetRspFunc func, Pst *pst, Buffer *mBuf)
{
    if(pst->selector == ODU_SELECTOR_LWLC)
    {
       MacUeResetRsp *ueResetRsp = NULLP;

       /* unpack the address of the structure */
       CMCHKUNPK(oduUnpackPointer, (PTR *)&ueResetRsp, mBuf);
       ODU_PUT_MSG_BUF(mBuf);
       return (*func)(pst, ueResetRsp);
    }

    ODU_PUT_MSG_BUF(mBuf);
    return RFAILED;
}

/*******************************************************************
 *
 * @brief Pack and send UE Sync Status Indication from MAC to DU APP
 *
 * @details
 *
 *    Function : packDuMacUeSyncStatusInd
 *
 *    Functionality:
 *       Pack and send UE Sync Status Indication from MAC to DU APP
 *
 * @params[in]
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t packDuMacUeSyncStatusInd(Pst *pst, MacUeSyncStatusInd *SyncStatusInd)
{
   Buffer *mBuf = NULLP;
   
   if(pst->selector == ODU_SELECTOR_LWLC)
   {
      if (ODU_GET_MSG_BUF(pst->region, pst->pool, &mBuf) != ROK)
      {
         DU_LOG("\nERROR  --> MAC : Memory allocation failed at packDuMacUeSyncStatusInd");
         return RFAILED;
      }
      /* pack the address of the structure */
      CMCHKPK(oduPackPointer,(PTR)SyncStatusInd, mBuf);
   }
   else
   {
      DU_LOG("\nERROR  -->  MAC: Only LWLC supported for packDuMacUeSyncStatusInd");
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
*    Function :unpackDuMacUeSyncStatusInd 
*
*    Functionality: Unpack UE Config Response from MAC to DU APP
*
* @params[in]
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/
uint8_t unpackDuMacUeSyncStatusInd(MacDuUeSyncStatusIndFunc func, Pst *pst, Buffer *mBuf)
{
    if(pst->selector == ODU_SELECTOR_LWLC)
    {
       MacUeSyncStatusInd *ueSyncStatusInd = NULLP;

       /* unpack the address of the structure */
       CMCHKUNPK(oduUnpackPointer, (PTR *)&ueSyncStatusInd, mBuf);
       ODU_PUT_MSG_BUF(mBuf);
       return (*func)(pst, ueSyncStatusInd);
    }

    ODU_PUT_MSG_BUF(mBuf);
    return RFAILED;
}

/*******************************************************************
*
* @brief Packs and Sends Dl Broadcast Request from DUAPP to MAC
*
* @details
*
*    Function : packDuMacDlBroadcastReq
*
*    Functionality:
*       Packs and Sends Dl Broadcast Request from DUAPP to MAC
*
*
* @params[in] Post structure pointer
*             MacDlBroadcastReq pointer
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/
uint8_t packDuMacDlBroadcastReq(Pst *pst, MacDlBroadcastReq *ueDel)
{
    Buffer *mBuf = NULLP;

    if(pst->selector == ODU_SELECTOR_LWLC)
    {
       if (ODU_GET_MSG_BUF(pst->region, pst->pool, &mBuf) != ROK)
       {
          DU_LOG("\nERROR  --> MAC : Memory allocation failed at packDuMacDlBroadcastReq");
          return RFAILED;
       }
       /* pack the address of the structure */
       CMCHKPK(oduPackPointer,(PTR)ueDel, mBuf);
    }
    else
    {
       DU_LOG("\nERROR  -->  MAC: Only LWLC supported for packDuMacDlBroadcastReq");
       return RFAILED;
    }
    return ODU_POST_TASK(pst,mBuf);
}

/*******************************************************************
*
* @brief Unpacks Dl Broadcast Request received from DU APP
*
* @details
*
*    Function : unpackMacDlBroadcastReq 
*
*    Functionality:
*         Unpacks Dl Broadcast Request received from DU APP
*
* @params[in] Pointer to Handler
*             Post structure pointer
*             Message Buffer
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/
uint8_t unpackMacDlBroadcastReq(DuMacDlBroadcastReq func, Pst *pst, Buffer *mBuf)
{
    if(pst->selector == ODU_SELECTOR_LWLC)
    {
       MacDlBroadcastReq *dlBroadcast;

       /* unpack the address of the structure */
       CMCHKUNPK(oduUnpackPointer, (PTR *)&dlBroadcast, mBuf);
       ODU_PUT_MSG_BUF(mBuf);
       return (*func)(pst, dlBroadcast);
    }
    else
    {
       /* Nothing to do for other selectors */
       DU_LOG("\nERROR  -->  DU APP : Only LWLC supported for Dl Broadcast Request ");
       ODU_PUT_MSG_BUF(mBuf);
    }

    return RFAILED;
}
/*******************************************************************
 *
 * @brief Searches for first unset bit in ueBitMap
 *
 * @details
 *
 *    Function : getFreeBitFromUeBitMap
 *
 *    Functionality: Searches for first unset bit in ueBitMap of
 *       a cell. Search starts from LSB to MSB. Returns index of the
 *       free bit, considering LSB at index 0 and increasing index
 *       towards MSB.
 *
 * @params[in] Cell Id
 * @return Index of free bit - success 
 *         -1 - failure
 *
 * ****************************************************************/
int8_t getFreeBitFromUeBitMap(uint16_t cellId)
{
   uint8_t  bitIdx = 0;  /* bit position */
   uint16_t cellIdx = 0; /* Index to acces ueBitMapPerCell[] */
   uint64_t mask = 1;    /* bit mask */

   GET_CELL_IDX(cellId, cellIdx);
   while(bitIdx < 64) 
   {   
      /* Find the first unset bit in Bit Map */
      if((ueBitMapPerCell[cellIdx] & (mask << bitIdx)) == 0)
      {   
         SET_ONE_BIT(bitIdx, ueBitMapPerCell[cellIdx]);
         return bitIdx;
      }   
      else
         bitIdx++;
   }   
   return -1; 
}

/*******************************************************************
 *
 * @brief Unset a previously set bit in UeBitMap
 *
 * @details
 *
 *    Function : unsetBitInUeBitMap
 *
 *    Functionality: Searches for bit at given index and unset it.
 *
 * @params[in] Cell Id
 *             Bit Index
 * @return void 
 *
 * ****************************************************************/
void unsetBitInUeBitMap(uint16_t cellId, uint8_t bitPos)
{
    uint16_t cellIdx;

   GET_CELL_IDX(cellId, cellIdx);
   UNSET_ONE_BIT(bitPos, ueBitMapPerCell[cellIdx]);
}

/*******************************************************************
*
* @brief Packs and Sends Statistics Request from DUAPP to MAC
*
* @details
*
*    Function : packDuMacStatsReq
*
*    Functionality:
*       Packs and Sends statistics request from DUAPP to MAC
*
*
* @params[in] Post structure pointer
*             StatsReq pointer
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/
uint8_t packDuMacStatsReq(Pst *pst, MacStatsReq *statsReq)
{
    Buffer *mBuf = NULLP;

    if(pst->selector == ODU_SELECTOR_LWLC)
    {
       if (ODU_GET_MSG_BUF(pst->region, pst->pool, &mBuf) != ROK)
       {
          DU_LOG("\nERROR  --> MAC : Memory allocation failed at packDuMacStatsReq");
          return RFAILED;
       }
       /* pack the address of the structure */
       CMCHKPK(oduPackPointer,(PTR)statsReq, mBuf);
    }
    else
    {
       DU_LOG("\nERROR  -->  MAC: Only LWLC supported for packDuMacStatsReq");
       return RFAILED;
    }
    return ODU_POST_TASK(pst,mBuf);
}

/*******************************************************************
*
* @brief Unpacks Statistics Request received from DU APP
*
* @details
*
*    Function : unpackMacStatsReq 
*
*    Functionality:
*         Unpacks Statistics Request received from DU APP
*
* @params[in] Pointer to Handler
*             Post structure pointer
*             Message Buffer
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/
uint8_t unpackMacStatsReq(DuMacStatsReqFunc func, Pst *pst, Buffer *mBuf)
{
    if(pst->selector == ODU_SELECTOR_LWLC)
    {
       MacStatsReq *statsReq;

       /* unpack the address of the structure */
       CMCHKUNPK(oduUnpackPointer, (PTR *)&statsReq, mBuf);
       ODU_PUT_MSG_BUF(mBuf);
       return (*func)(pst, statsReq);
    }
    else
    {
       /* Nothing to do for other selectors */
       DU_LOG("\nERROR  -->  DU APP : Only LWLC supported for Statistics Request ");
       ODU_PUT_MSG_BUF(mBuf);
    }

    return RFAILED;
}

/*******************************************************************
*
* @brief Packs and Sends Statistics Response from MAC to DUAPP
*
* @details
*
*    Function : packDuMacStatsRsp
*
*    Functionality:
*       Packs and Sends statistics response from MAC to DUAPP
*
*
* @params[in] Post structure pointer
*             StatsRsp pointer
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/
uint8_t packDuMacStatsRsp(Pst *pst, MacStatsRsp *statsRsp)
{
    Buffer *mBuf = NULLP;

    if(pst->selector == ODU_SELECTOR_LWLC)
    {
       if (ODU_GET_MSG_BUF(pst->region, pst->pool, &mBuf) != ROK)
       {
          DU_LOG("\nERROR  --> MAC : Memory allocation failed at packDuMacStatsRsp");
          return RFAILED;
       }
       /* pack the address of the structure */
       CMCHKPK(oduPackPointer,(PTR)statsRsp, mBuf);
    }
    else
    {
       DU_LOG("\nERROR  -->  MAC: Only LWLC supported for packDuMacStatsRsp");
       return RFAILED;
    }
    return ODU_POST_TASK(pst,mBuf);
}

/*******************************************************************
*
* @brief Unpacks Statistics Response received from MAC
*
* @details
*
*    Function : unpackDuMacStatsRsp
*
*    Functionality:
*         Unpacks Statistics Response received from MAC
*
* @params[in] Pointer to Handler
*             Post structure pointer
*             Message Buffer
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/
uint8_t unpackDuMacStatsRsp(MacDuStatsRspFunc func, Pst *pst, Buffer *mBuf)
{
    if(pst->selector == ODU_SELECTOR_LWLC)
    {
       MacStatsRsp *statsRsp;

       /* unpack the address of the structure */
       CMCHKUNPK(oduUnpackPointer, (PTR *)&statsRsp, mBuf);
       ODU_PUT_MSG_BUF(mBuf);
       return (*func)(pst, statsRsp);
    }
    else
    {
       /* Nothing to do for other selectors */
       DU_LOG("\nERROR  -->  DU APP : Only LWLC supported for Statistics Response ");
       ODU_PUT_MSG_BUF(mBuf);
    }

    return RFAILED;
}

/*******************************************************************
*
* @brief Packs and Sends Statistics Indication from MAC to DUAPP
*
* @details
*
*    Function : packDuMacStatsInd
*
*    Functionality:
*       Packs and Sends statistics response from MAC to DUAPP
*
*
* @params[in] Post structure pointer
*             StatsInd pointer
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/
uint8_t packDuMacStatsInd(Pst *pst, MacStatsInd *statsInd)
{
    Buffer *mBuf = NULLP;

    if(pst->selector == ODU_SELECTOR_LWLC)
    {
       if (ODU_GET_MSG_BUF(pst->region, pst->pool, &mBuf) != ROK)
       {
          DU_LOG("\nERROR  --> MAC : Memory allocation failed at packDuMacStatsInd");
          return RFAILED;
       }
       /* pack the address of the structure */
       CMCHKPK(oduPackPointer,(PTR)statsInd, mBuf);
    }
    else
    {
       DU_LOG("\nERROR  -->  MAC: Only LWLC supported for packDuMacStatsInd");
       return RFAILED;
    }
    return ODU_POST_TASK(pst,mBuf);
}

/*******************************************************************
*
* @brief Unpacks Statistics Indication received from MAC
*
* @details
*
*    Function : unpackDuMacStatsInd
*
*    Functionality:
*         Unpacks Statistics Indication received from MAC
*
* @params[in] Pointer to Handler
*             Post structure pointer
*             Message Buffer
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/
uint8_t unpackDuMacStatsInd(MacDuStatsIndFunc func, Pst *pst, Buffer *mBuf)
{
    if(pst->selector == ODU_SELECTOR_LWLC)
    {
       MacStatsInd *statsInd;

       /* unpack the address of the structure */
       CMCHKUNPK(oduUnpackPointer, (PTR *)&statsInd, mBuf);
       ODU_PUT_MSG_BUF(mBuf);
       return (*func)(pst, statsInd);
    }
    else
    {
       /* Nothing to do for other selectors */
       DU_LOG("\nERROR  -->  DU APP : Only LWLC supported for Statistics Indication ");
       ODU_PUT_MSG_BUF(mBuf);
    }

    return RFAILED;
}

/*******************************************************************
*
* @brief Packs and Sends Statistics Delete Request from DUAPP to MAC
*
* @details
*
*    Function : packDuMacStatsDeleteReq
*
*    Functionality:
*       Packs and Sends statistics Delete Request from DUAPP to MAC
*
*
* @params[in] Post structure pointer
*             StatsDeleteReq pointer
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/
uint8_t packDuMacStatsDeleteReq(Pst *pst, MacStatsDeleteReq *statsDeleteReq)
{
    Buffer *mBuf = NULLP;

    if(pst->selector == ODU_SELECTOR_LWLC)
    {
       if (ODU_GET_MSG_BUF(pst->region, pst->pool, &mBuf) != ROK)
       {
          DU_LOG("\nERROR  --> MAC : Memory allocation failed at packDuMacStatsDeleteReq");
          return RFAILED;
       }
       /* pack the address of the structure */
       CMCHKPK(oduPackPointer,(PTR)statsDeleteReq, mBuf);
    }
    else
    {
       DU_LOG("\nERROR  -->  MAC: Only LWLC supported for packDuMacStatsDeleteReq");
       return RFAILED;
    }
    return ODU_POST_TASK(pst,mBuf);
}

/*******************************************************************
*
* @brief Unpacks Statistics Delete Request received from DU APP
*
* @details
*
*    Function : unpackMacStatsDeleteReq
*
*    Functionality:
*         Unpacks Statistics Delete Request received from DU APP
*
* @params[in] Pointer to Handler
*             Post structure pointer
*             Message Buffer
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/
uint8_t unpackMacStatsDeleteReq(DuMacStatsDeleteReqFunc func, Pst *pst, Buffer *mBuf)
{
    if(pst->selector == ODU_SELECTOR_LWLC)
    {
       MacStatsDeleteReq *statsDeleteReq;

       /* unpack the address of the structure */
       CMCHKUNPK(oduUnpackPointer, (PTR *)&statsDeleteReq, mBuf);
       ODU_PUT_MSG_BUF(mBuf);
       return (*func)(pst, statsDeleteReq);
    }
    else
    {
       /* Nothing to do for other selectors */
       DU_LOG("\nERROR  -->  DU APP : Only LWLC supported for Statistics Delete Request ");
       ODU_PUT_MSG_BUF(mBuf);
    }

    return RFAILED;
}

/*******************************************************************
*
* @brief Packs and Sends Statistics Delete Response from MAC to DUAPP
*
* @details
*
*    Function : packDuMacStatsDeleteRsp
*
*    Functionality:
*       Packs and Sends Statistics Delete Response from MAC to DUAPP
*
*
* @params[in] Post structure pointer
*             StatsDeleteRsp pointer
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/
uint8_t packDuMacStatsDeleteRsp(Pst *pst, MacStatsDeleteRsp *statsRsp)
{
    Buffer *mBuf = NULLP;

    if(pst->selector == ODU_SELECTOR_LWLC)
    {
       if (ODU_GET_MSG_BUF(pst->region, pst->pool, &mBuf) != ROK)
       {
          DU_LOG("\nERROR  --> MAC : Memory allocation failed at packDuMacStatsDeleteRsp");
          return RFAILED;
       }
       /* pack the address of the structure */
       CMCHKPK(oduPackPointer,(PTR)statsRsp, mBuf);
    }
    else
    {
       DU_LOG("\nERROR  -->  MAC: Only LWLC supported for packDuMacStatsDeleteRsp");
       return RFAILED;
    }
    return ODU_POST_TASK(pst,mBuf);
}

/*******************************************************************
*
* @brief Unpacks Statistics Delete Response received from MAC
*
* @details
*
*    Function : unpackDuMacStatsDeleteRsp
*
*    Functionality:
*         Unpacks Statistics Delete Response received from MAC
*
* @params[in] Pointer to Handler
*             Post structure pointer
*             Message Buffer
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/
uint8_t unpackDuMacStatsDeleteRsp(MacDuStatsDeleteRspFunc func, Pst *pst, Buffer *mBuf)
{
    if(pst->selector == ODU_SELECTOR_LWLC)
    {
       MacStatsDeleteRsp *statsRsp;

       /* unpack the address of the structure */
       CMCHKUNPK(oduUnpackPointer, (PTR *)&statsRsp, mBuf);
       ODU_PUT_MSG_BUF(mBuf);
       return (*func)(pst, statsRsp);
    }
    else
    {
       /* Nothing to do for other selectors */
       DU_LOG("\nERROR  -->  DU APP : Only LWLC supported for Statistics Delete Response ");
       ODU_PUT_MSG_BUF(mBuf);
    }

    return RFAILED;
}

/*******************************************************************
*
* @brief Packs and Sends Statistics Modification Request from DUAPP to MAC
*
* @details
*
*    Function : packDuMacStatsModificationReq
*
*    Functionality:
*       Packs and Sends statistics Modification request from DUAPP to MAC
*
*
* @params[in] Post structure pointer
*             StatsModificationReq pointer
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/
uint8_t packDuMacStatsModificationReq(Pst *pst, MacStatsModificationReq *statsModificationReq)
{
    Buffer *mBuf = NULLP;

    if(pst->selector == ODU_SELECTOR_LWLC)
    {
       if (ODU_GET_MSG_BUF(pst->region, pst->pool, &mBuf) != ROK)
       {
          DU_LOG("\nERROR  --> MAC : Memory allocation failed at packDuMacStatsModificationReq");
          return RFAILED;
       }
       /* pack the address of the structure */
       CMCHKPK(oduPackPointer,(PTR)statsModificationReq, mBuf);
    }
    else
    {
       DU_LOG("\nERROR  -->  MAC: Only LWLC supported for packDuMacStatsModificationReq");
       return RFAILED;
    }
    return ODU_POST_TASK(pst,mBuf);
}

/*******************************************************************
*
* @brief Unpacks Statistics Modification Request received from DU APP
*
* @details
*
*    Function : unpackMacStatsModificationReq
*
*    Functionality:
*         Unpacks Statistics Modification Request received from DU APP
*
* @params[in] Pointer to Handler
*             Post structure pointer
*             Message Buffer
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/
uint8_t unpackMacStatsModificationReq(DuMacStatsModificationReqFunc func, Pst *pst, Buffer *mBuf)
{
    if(pst->selector == ODU_SELECTOR_LWLC)
    {
       MacStatsModificationReq *statsModificationReq=NULLP;

       /* unpack the address of the structure */
       CMCHKUNPK(oduUnpackPointer, (PTR *)&statsModificationReq, mBuf);
       ODU_PUT_MSG_BUF(mBuf);
       return (*func)(pst, statsModificationReq);
    }
    else
    {
       /* Nothing to do for other selectors */
       DU_LOG("\nERROR  -->  DU APP : Only LWLC supported for Statistics Modification Request ");
       ODU_PUT_MSG_BUF(mBuf);
    }

    return RFAILED;
}

/*******************************************************************
*
* @brief Packs and Sends Statistics Modification Response from MAC to DUAPP
*
* @details
*
*    Function : packDuMacStatsModificationRsp
*
*    Functionality:
*       Packs and Sends statistics Modification response from MAC to DUAPP
*
*
* @params[in] Post structure pointer
*             StatsModificationRsp pointer
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/
uint8_t packDuMacStatsModificationRsp(Pst *pst, MacStatsModificationRsp *statsModificationRsp)
{
    Buffer *mBuf = NULLP;

    if(pst->selector == ODU_SELECTOR_LWLC)
    {
       if (ODU_GET_MSG_BUF(pst->region, pst->pool, &mBuf) != ROK)
       {
          DU_LOG("\nERROR  --> MAC : Memory allocation failed at packDuMacStatsModificationRsp");
          return RFAILED;
       }
       /* pack the address of the structure */
       CMCHKPK(oduPackPointer,(PTR)statsModificationRsp, mBuf);
    }
    else
    {
       DU_LOG("\nERROR  -->  MAC: Only LWLC supported for packDuMacStatsModificationRsp");
       return RFAILED;
    }
    return ODU_POST_TASK(pst,mBuf);
}

/*******************************************************************
*
* @brief Unpacks Statistics Modification Response received from MAC
*
* @details
*
*    Function : unpackDuMacStatsModificationRsp
*
*    Functionality:
*         Unpacks Statistics Modification Response received from MAC
*
* @params[in] Pointer to Handler
*             Post structure pointer
*             Message Buffer
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/
uint8_t unpackDuMacStatsModificationRsp(MacDuStatsModificationRspFunc func, Pst *pst, Buffer *mBuf)
{
    if(pst->selector == ODU_SELECTOR_LWLC)
    {
       MacStatsModificationRsp *statsModificationRsp=NULLP;

       /* unpack the address of the structure */
       CMCHKUNPK(oduUnpackPointer, (PTR *)&statsModificationRsp, mBuf);
       ODU_PUT_MSG_BUF(mBuf);
       return (*func)(pst, statsModificationRsp);
    }
    else
    {
       /* Nothing to do for other selectors */
       DU_LOG("\nERROR  -->  DU APP : Only LWLC supported for Statistics Modification Response ");
       ODU_PUT_MSG_BUF(mBuf);
    }

    return RFAILED;
}

/**********************************************************************
  End of file
 **********************************************************************/
