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

      DU_LOG("\nDU-APP : MAC CELL config sent");
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
 *    Function : packMacCellStartReq
 *
 *    Functionality:
 *      Packs and Sends Cell Start Request to MAC
 *
 * @params[in] Post structure pointer
 *             MacCellStartInfo pointer
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t packMacCellStartReq(Pst *pst, MacCellStartInfo  *cellStartInfo)
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
	 DU_LOG("\nDU APP : Memory allocation failed for cell start req pack");
	 return RFAILED;
      }

      /* pack the address of the structure */
      CMCHKPK(oduPackPointer,(PTR)cellStartInfo, mBuf);

   }
   return ODU_POST_TASK(pst,mBuf);
}

/*******************************************************************
 *
 * @brief Unpacks MAC Cell Start Request from DU APP
 *
 * @details
 *
 *    Function : unpackMaCellStartReq
 *
 *    Functionality:
 *      Unpacks MAC Cell Start Request from DU APP
 *
 * @params[in] Function pointer of cell start request handler
 *             Post structure pointer
 *             Cell Start Request Info Pointer
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t unpackMacCellStartReq(DuMacCellStartReq func, Pst *pst, Buffer *mBuf)
{
   MacCellStartInfo  *cellStartInfo;

   if(pst->selector == ODU_SELECTOR_LWLC)
   {
      /* unpack the address of the structure */
      CMCHKUNPK(oduUnpackPointer, (PTR *)&cellStartInfo, mBuf);
      ODU_PUT_MSG_BUF(mBuf); 
      return (*func)(pst, cellStartInfo);
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
 *    Function : packMacCellStopReq
 *
 *    Functionality:
 *       Packs and Send cell stop request to MAC
 *
 * @params[in] Post structure pointer
 *             Cell stop info structure 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t packMacCellStopReq(Pst *pst, MacCellStopInfo  *cellStopInfo)
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
	 DU_LOG("\nDU APP : Memory allocation failed for cell stop req pack");
	 return RFAILED;
      }

      /* pack the address of the structure */
      CMCHKPK(oduPackPointer,(PTR)cellStopInfo, mBuf);

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
 *    Function : unpackMaCellStopReq 
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
uint8_t unpackMacCellStopReq(DuMacCellStopReq func, Pst *pst, Buffer *mBuf)
{
   MacCellStopInfo  *cellStopInfo;

   if(pst->selector == ODU_SELECTOR_LWLC)
   {
      /* unpack the address of the structure */
      CMCHKUNPK(oduUnpackPointer, (PTR *)&cellStopInfo, mBuf);
      ODU_PUT_MSG_BUF(mBuf);
      return (*func)(pst, cellStopInfo);
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
 * @brief Packs and Sends slot ind from MAC to DUAPP
 *
 * @details
 *
 *    Function : packMacSlotInd
 *
 *    Functionality:
 *       Packs and Sends slot ind from MAC to DUAPP
 *
 * @params[in] Post structure pointer
 *             Slot Info pointer              
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t packMacSlotInd(Pst *pst, SlotIndInfo *slotInfo )
{
   Buffer *mBuf = NULLP;

   if (ODU_GET_MSG_BUF(pst->region, pst->pool, &mBuf) != ROK)
   {
      DU_LOG("\nDU APP : Memory allocation failed for cell start req pack");
      return RFAILED;
   }

   if(pst->selector == ODU_SELECTOR_LC)
   {
      CMCHKPK(oduUnpackUInt16, slotInfo->cellId, mBuf);
      CMCHKPK(oduUnpackUInt16, slotInfo->sfn, mBuf);
      CMCHKPK(oduUnpackUInt16, slotInfo->slot, mBuf);

      CM_FREE_SHRABL_BUF(pst->region, pst->pool, slotInfo, sizeof(SlotIndInfo));
      slotInfo = NULL;
   }
   else if(pst->selector == ODU_SELECTOR_LWLC)
   {
      /* pack the address of the structure */
      CMCHKPK(oduPackPointer,(PTR)slotInfo, mBuf);
   }
   else
   {
      ODU_PUT_MSG_BUF(mBuf);
   }

   return ODU_POST_TASK(pst,mBuf);
}

/*******************************************************************
 *
 * @brief Unpacks slot indication from MAC
 *
 * @details
 *
 *    Function : unpackMacSlotInd
 *
 *    Functionality:
 *         Unpacks slot indication from MAC
 *
 * @params[in] Pointer to Handler
 *             Post structure pointer
 *             Message Buffer
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t unpackMacSlotInd(DuMacSlotInd func, Pst *pst, Buffer *mBuf)
{
   if(pst->selector == ODU_SELECTOR_LWLC)
   {
      SlotIndInfo *slotInfo;

      /* unpack the address of the structure */
      CMCHKUNPK(oduUnpackPointer, (PTR *)&slotInfo, mBuf);
      ODU_PUT_MSG_BUF(mBuf);
      return (*func)(pst, slotInfo);
   }
   else if(pst->selector == ODU_SELECTOR_LC)
   {
      SlotIndInfo slotInfo;

      CMCHKUNPK(oduPackUInt16, &(slotInfo.slot), mBuf);
      CMCHKUNPK(oduPackUInt16, &(slotInfo.sfn), mBuf);
      CMCHKUNPK(oduPackUInt16, &(slotInfo.cellId), mBuf);

      ODU_PUT_MSG_BUF(mBuf);
      return (*func)(pst, &slotInfo);

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
uint8_t packMacStopInd(Pst *pst, MacCellStopInfo *cellStopId)
{
   Buffer *mBuf = NULLP;

   if (ODU_GET_MSG_BUF(pst->region, pst->pool, &mBuf) != ROK)
   {
      DU_LOG("\nDU APP : Memory allocation failed for stop Ind pack");
      return RFAILED;
   }

   if(pst->selector == ODU_SELECTOR_LC)
   {
      /*pack the payload here*/
      DU_LOG("\nDU APP : Packed CellId");
      CMCHKPK(oduUnpackUInt16, cellStopId->cellId, mBuf);
      CM_FREE_SHRABL_BUF(pst->region, pst->pool, cellStopId, sizeof(MacCellStopInfo));
      cellStopId = NULL;
   }
   else if(pst->selector == ODU_SELECTOR_LWLC)
   {
      /* pack the address of the structure */
      CMCHKPK(oduPackPointer,(PTR)cellStopId, mBuf);
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
      MacCellStopInfo *cellStopId;
      /* unpack the address of the structure */
      CMCHKUNPK(oduUnpackPointer, (PTR *)&cellStopId, mBuf);
      ODU_PUT_MSG_BUF(mBuf);
      return (*func)(pst, cellStopId);
   }
   else if(pst->selector == ODU_SELECTOR_LC)
   {
      MacCellStopInfo cellStopId;
      CMCHKUNPK(oduPackUInt16, &(cellStopId.cellId), mBuf);

      ODU_PUT_MSG_BUF(mBuf);
      return (*func)(pst, &cellStopId);

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
	 DU_LOG("\nMAC : Memory allocation failed at packMacUlCcchInd");
	 return RFAILED;
      }
      /* pack the address of the structure */
      CMCHKPK(oduPackPointer,(PTR)ulCcchIndInfo, mBuf);
   }
   else
   {
      DU_LOG("\nMAC: Only LWLC supported for UL CCCH Ind ");
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
      DU_LOG("\n Only LWLC supported for UL CCCH Ind ");
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
	 DU_LOG("\nMAC : Memory allocation failed at packMacDlCcchInd");
	 return RFAILED;
      }
      /* pack the address of the structure */
      CMCHKPK(oduPackPointer,(PTR)dlCcchIndInfo, mBuf);
   }
   else
   {
      DU_LOG("\nDU APP: Only LWLC supported for DL CCCH Ind ");
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
      DU_LOG("\n Only LWLC supported for DL CCCH Ind ");
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
	 DU_LOG("\nMAC : Memory allocation failed at packDuMacUeCreateReq");
	 return RFAILED;
      }
      /* pack the address of the structure */
      CMCHKPK(oduPackPointer,(PTR)ueCfg, mBuf);
   }
   else
   {
      DU_LOG("\nMAC: Only LWLC supported for packDuMacUeCreateReq");
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
      DU_LOG("\n Only LWLC supported for UE Create Request ");
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
	 DU_LOG("\nMAC : Memory allocation failed at packDuMacUeCfgRsp");
	 return RFAILED;
      }
      /* pack the address of the structure */
      CMCHKPK(oduPackPointer,(PTR)cfgRsp, mBuf);
   }
   else
   {
      DU_LOG("\nMAC: Only LWLC supported for packDuMacUeCfgRsp");
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
 *    Function : unpackDuMacUeCfgRsp
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
      MacUeCfgRsp *cfgRsp;

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
	 DU_LOG("\nMAC : Memory allocation failed at packDuMacUeReconfigReq");
	 return RFAILED;
      }
      /* pack the address of the structure */
      CMCHKPK(oduPackPointer,(PTR)ueCfg, mBuf);
   }
   else
   {
      DU_LOG("\nMAC: Only LWLC supported for packDuMacUeReconfigReq");
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
      DU_LOG("\n Only LWLC supported for UE Create Request ");
      ODU_PUT_MSG_BUF(mBuf);
   }

   return RFAILED;
}

/******************************************************************
 *
 * @brief Fills Default UL LC Cfg
 *
 * @details
 *
 *    Function : fillDefUlLcCfg
 *
 *    Functionality: Fills Default UL LC Cfg
 *
 *
 *****************************************************************/
void fillDefUlLcCfg(UlLcCfg *ulLcCfg)
{
   ulLcCfg->priority = LC_PRIORITY_1;
   ulLcCfg->lcGroup =  0;
   ulLcCfg->schReqId = 0;
   ulLcCfg->pbr = PBR_KBPS_INFINITY;
   ulLcCfg->bsd = BSD_MS_1000;
}

/******************************************************************
 *
 * @brief Fills Initial DL Bandwidth Part
 *
 * @details
 *
 *    Function : fillDefInitDlBwp
 *
 *    Functionality: Fills Initial DL Bandwidth Part
 *
 *
 *****************************************************************/
void fillDefInitDlBwp(InitialDlBwp *initDlBwp)
{
   uint8_t idx = 0;
   uint8_t freqDomainResource[FREQ_DOM_RSRC_SIZE] = {0};
   uint8_t coreset0EndPrb, coreset1StartPrb, coreset1NumPrb;


   if(initDlBwp)
   {
      /* Filling PDCCH Config */
      initDlBwp->pdcchPresent = TRUE;
      if(initDlBwp->pdcchPresent)
      {
	 initDlBwp->pdcchCfg.numCRsetToAddMod = PDCCH_CTRL_RSRC_SET_ONE_ID;
	 memset(initDlBwp->pdcchCfg.cRSetToAddModList, 0, MAX_NUM_CRSET);
	 if(initDlBwp->pdcchCfg.numCRsetToAddMod <= MAX_NUM_CRSET)
	 {
	    initDlBwp->pdcchCfg.cRSetToAddModList[idx].cRSetId = \
	       PDCCH_CTRL_RSRC_SET_ONE_ID;
	    memset(initDlBwp->pdcchCfg.cRSetToAddModList[idx].freqDomainRsrc, 0,\
	       FREQ_DOM_RSRC_SIZE); 
	    coreset0EndPrb = CORESET0_END_PRB;
	    coreset1StartPrb = coreset0EndPrb +6;
	    coreset1NumPrb = CORESET1_NUM_PRB;
	    /* calculate the PRBs */
	    freqDomRscAllocType0(((coreset1StartPrb)/6), (coreset1NumPrb/6), freqDomainResource);
	    memcpy(initDlBwp->pdcchCfg.cRSetToAddModList[idx].freqDomainRsrc, freqDomainResource,
	       FREQ_DOM_RSRC_SIZE);

	    initDlBwp->pdcchCfg.cRSetToAddModList[idx].duration = \
	       PDCCH_CTRL_RSRC_SET_ONE_DURATION;
	    initDlBwp->pdcchCfg.cRSetToAddModList[idx].cceRegMappingType = \
	       CCE_REG_MAPPINGTYPE_PR_NONINTERLEAVED;
	    initDlBwp->pdcchCfg.cRSetToAddModList[idx].precoderGranularity = \
	       ALL_CONTIGUOUS_RBS;
	    initDlBwp->pdcchCfg.cRSetToAddModList[idx].dmrsScramblingId = \
	       SCRAMBLING_ID;
	 }
	 initDlBwp->pdcchCfg.numCRsetToRel = 0;
	 /* Filling Serach Space */
	 initDlBwp->pdcchCfg.numSearchSpcToAddMod = PDCCH_CTRL_RSRC_SET_ONE_ID;
	 memset(initDlBwp->pdcchCfg.searchSpcToAddModList, 0, MAX_NUM_CRSET);
	 if(initDlBwp->pdcchCfg.numSearchSpcToAddMod <= MAX_NUM_CRSET)
	 {
	    initDlBwp->pdcchCfg.searchSpcToAddModList[idx].searchSpaceId =\
	       PDCCH_SRCH_SPC_TWO_ID;
	    initDlBwp->pdcchCfg.searchSpcToAddModList[idx].cRSetId = \
	       PDCCH_CTRL_RSRC_SET_ONE_ID;
	    initDlBwp->pdcchCfg.searchSpcToAddModList[idx].\
	       mSlotPeriodicityAndOffset = SLOTPERIODICITYANDOFFSET_PR_SL1;
	    memset(initDlBwp->pdcchCfg.searchSpcToAddModList[idx].mSymbolsWithinSlot, 0,\
	       MONITORING_SYMB_WITHIN_SLOT_SIZE);
	    initDlBwp->pdcchCfg.searchSpcToAddModList[idx].mSymbolsWithinSlot[idx] =\
	       PDCCH_SYMBOL_WITHIN_SLOT;
	    initDlBwp->pdcchCfg.searchSpcToAddModList[idx].numCandidatesAggLevel1 =\
	       AGGREGATIONLEVEL_N8; 
	    initDlBwp->pdcchCfg.searchSpcToAddModList[idx].numCandidatesAggLevel2 =\
	       AGGREGATIONLEVEL_N8; 
	    initDlBwp->pdcchCfg.searchSpcToAddModList[idx].numCandidatesAggLevel4 =\
	       AGGREGATIONLEVEL_N4; 
	    initDlBwp->pdcchCfg.searchSpcToAddModList[idx].numCandidatesAggLevel8 =\
	       AGGREGATIONLEVEL_N2; 
	    initDlBwp->pdcchCfg.searchSpcToAddModList[idx].numCandidatesAggLevel16 =\
	       AGGREGATIONLEVEL_N1;
	    initDlBwp->pdcchCfg.searchSpcToAddModList[idx].searchSpaceType = \
	       SEARCHSPACETYPE_PR_UE_SPECIFIC;
	    initDlBwp->pdcchCfg.searchSpcToAddModList[idx].ueSpecificDciFormat =\
	       PDCCH_SRCH_SPC_TWO_UE_SPEC_DCI_FORMAT;

	    initDlBwp->pdcchCfg.numSearchSpcToRel = 0;

	 }
      }
      /* Filling PDSCH Config */
      initDlBwp->pdschPresent = TRUE;
      if(initDlBwp->pdschPresent)
      {
	 initDlBwp->pdschCfg.dmrsDlCfgForPdschMapTypeA.addPos = ADDITIONALPOSITION_POS0;
	 initDlBwp->pdschCfg.resourceAllocType = RESOURCEALLOCATION_TYPE1;
	 initDlBwp->pdschCfg.numTimeDomRsrcAlloc = 1;
	 initDlBwp->pdschCfg.timeDomRsrcAllociList[idx].mappingType = \
	    MAPPING_TYPEA;
	 initDlBwp->pdschCfg.timeDomRsrcAllociList[idx].startSymbol = PDSCH_START_SYMBOL; 
	 initDlBwp->pdschCfg.timeDomRsrcAllociList[idx].symbolLength = PDSCH_LENGTH_SYMBOL;
	 initDlBwp->pdschCfg.timeDomRsrcAllociList[idx].startSymbolAndLength = \
	    calcSliv(PDSCH_START_SYMBOL, PDSCH_LENGTH_SYMBOL);
	 initDlBwp->pdschCfg.rbgSize = RBG_SIZE_CONFIG1;
	 initDlBwp->pdschCfg.numCodeWordsSchByDci = CODEWORDS_SCHED_BY_DCI_N1;
	 initDlBwp->pdschCfg.bundlingType = TYPE_STATIC_BUNDLING;
	 initDlBwp->pdschCfg.bundlingInfo.StaticBundling.size = 0;
      }
   }

}

/******************************************************************
 *
 * @brief Fills Initial UL Bandwidth Part
 *
 * @details
 *
 *    Function : fillDefInitUlBwp
 *
 *    Functionality: Fills Initial UL Bandwidth Part
 *
 *
 *****************************************************************/
void fillDefInitUlBwp(InitialUlBwp *initUlBwp)
{
   uint8_t idx;
   if(initUlBwp)
   {
      initUlBwp->pucchPresent = FALSE;

      /*Filling PUSCH Config */
      initUlBwp->puschPresent = TRUE;
      if(initUlBwp->puschPresent)
      {
         initUlBwp->puschCfg.dataScramblingId = SCRAMBLING_ID;
	 initUlBwp->puschCfg.dmrsUlCfgForPuschMapTypeA.addPos = ADDITIONALPOSITION_POS0; 
	 initUlBwp->puschCfg.dmrsUlCfgForPuschMapTypeA.transPrecodDisabled. \
	    scramblingId0 = SCRAMBLING_ID; 
	 initUlBwp->puschCfg.resourceAllocType = RESOURCEALLOCATION_TYPE1;
	 initUlBwp->puschCfg.numTimeDomRsrcAlloc = 1;
	 idx = 0;
	 if(initUlBwp->puschCfg.numTimeDomRsrcAlloc <= MAX_NUM_UL_ALLOC)
	 {
	    initUlBwp->puschCfg.timeDomRsrcAllocList[idx].k2 = PUSCH_K2;
	    initUlBwp->puschCfg.timeDomRsrcAllocList[idx].mappingType =\
	       MAPPING_TYPEA;
	    initUlBwp->puschCfg.timeDomRsrcAllocList[idx].startSymbol = PUSCH_START_SYMBOL;
	    initUlBwp->puschCfg.timeDomRsrcAllocList[idx].symbolLength = PUSCH_LENGTH_SYMBOL;
	    initUlBwp->puschCfg.timeDomRsrcAllocList[idx].startSymbolAndLength =\
	       calcSliv(PUSCH_START_SYMBOL, PUSCH_LENGTH_SYMBOL);
	 }
	 initUlBwp->puschCfg.transformPrecoder = TRANSFORM_PRECODER_DISABLED;
      }
   }
   else
   {
      DU_LOG("\n DUAPP: Memory is NULL of InitalUlBwp");
   }

}
/******************************************************************
 *
 * @brief Fills SpCell Group Info
 *
 * @details
 *
 *    Function : fillDefSpCellGrpInfo
 *
 *    Functionality: Fills Sp Cell Group Info
 *
 *
 *****************************************************************/
void fillDefSpCellGrpInfo(SpCellCfg *spCell)
{
   if(spCell)
   {
      spCell->servCellIdx = SERV_CELL_IDX;
      /* Filling Initial Dl Bwp */
      fillDefInitDlBwp(&spCell->servCellCfg.initDlBwp);

      spCell->servCellCfg.numDlBwpToAdd    = 0; 
      spCell->servCellCfg.firstActvDlBwpId = ACTIVE_DL_BWP_ID;
      spCell->servCellCfg.defaultDlBwpId   = ACTIVE_DL_BWP_ID;
      spCell->servCellCfg.bwpInactivityTmr = NULLP;
      spCell->servCellCfg.pdschServCellCfg.maxMimoLayers = NULLP;
      spCell->servCellCfg.pdschServCellCfg.maxCodeBlkGrpPerTb = NULLP;
      spCell->servCellCfg.pdschServCellCfg.codeBlkGrpFlushInd = NULLP;
      spCell->servCellCfg.pdschServCellCfg.xOverhead = NULLP;
      spCell->servCellCfg.pdschServCellCfg.numHarqProcForPdsch =\
         NUM_HARQ_PROC_FOR_PDSCH_N_16;
      /* Filling Initial UL Bwp*/
      fillDefInitUlBwp(&spCell->servCellCfg.initUlBwp);
      spCell->servCellCfg.numUlBwpToAdd     = 0; 
      spCell->servCellCfg.firstActvUlBwpId  = ACTIVE_DL_BWP_ID; 
   }
   else
   {
      DU_LOG("\n DU_APP: Memory is NULL for SpCellGrp");
   }
}

/******************************************************************
 *
 * @brief Fills Physical Cell Group Info
 *
 * @details
 *
 *    Function : fillDefPhyCellGrpInfo
 *
 *    Functionality: Fills Physical Cell Group Info
 *
 *
 *****************************************************************/
void fillDefPhyCellGrpInfo(PhyCellGrpCfg *cellGrp)
{
   if(cellGrp)
   {
      cellGrp->pdschHarqAckCodebook = PDSCH_HARQ_ACK_CODEBOOK_DYNAMIC;
      cellGrp->pNrFr1 = P_NR_FR1;
   }
   else
   {
      DU_LOG("\nDUAPP: Memory is NULL for Physical Cell Group");
   }
}

/******************************************************************
 *
 * @brief Fills Mac Cell Group Info
 *
 * @details
 *
 *    Function : fillDefMacCellGrpInfo
 *
 *    Functionality: Fills Mac Cell Group Info
 *
 *
 *****************************************************************/
void fillDefMacCellGrpInfo(MacCellGrpCfg *cellGrp)
{
   uint8_t idx;

   if(cellGrp)
   {
      /* Filling Scheduling Request Config */
      cellGrp->schReqCfg.addModListCount = 1;
      if(cellGrp->schReqCfg.addModListCount <= MAX_NUM_SR_CFG_PER_CELL_GRP)
      {
	 for(idx = 0; idx < cellGrp->schReqCfg.addModListCount; idx++)
	 {
	    cellGrp->schReqCfg.addModList[idx].schedReqId    = SCH_REQ_ID;
	    cellGrp->schReqCfg.addModList[idx].srProhibitTmr = SR_PROHIBIT_MS_32;
	    cellGrp->schReqCfg.addModList[idx].srTransMax    = SR_TRANS_MAX_N_16;
	 }
      }
      cellGrp->schReqCfg.relListCount = 0;

      /* Filling Tag config */
      cellGrp->tagCfg.addModListCount = 1;
      if(cellGrp->tagCfg.addModListCount <= MAC_NUM_TAGS)
      {
	 for(idx = 0; idx < cellGrp->tagCfg.addModListCount; idx++)
	 {
	    cellGrp->tagCfg.addModList[idx].tagId = TAG_ID;
	    cellGrp->tagCfg.addModList[idx].timeAlignTimer = TIME_ALIGNMENT_TIMER_INFINITY;
	 }
      }
      cellGrp->tagCfg.relListCount = 0;

      /* Filling BSR config */
      cellGrp->bsrTmrCfg.periodicTimer = BSR_PERIODIC_TIMER_SF_10;
      cellGrp->bsrTmrCfg.retxTimer = BSR_RETX_TIMER_SF_320;
      cellGrp->bsrTmrCfg.srDelayTimer = BSR_SR_DELAY_TMR_2560;

      /* Filling PHR config */
      cellGrp->phrCfgSetupPres = true;
      cellGrp->phrCfg.periodicTimer = PHR_PERIODIC_TIMER_INFINITY;
      cellGrp->phrCfg.prohibitTimer = PHR_PROHIBIT_TIMER_SF_0;
      cellGrp->phrCfg.txPowerFactor = PHR_TX_PWR_FACTOR_CHANGE_INFINITY;
      cellGrp->phrCfg.multiplePHR   = false;
      cellGrp->phrCfg.dummy         = false;
      cellGrp->phrCfg.phrType2OtherCell = false;
      cellGrp->phrCfg.phrOtherCG = PHR_MODE_OTHER_CG_REAL;  

   }
   else
   {
      DU_LOG("\nDUAPP: Memory is NULL for Master Cell Group");
   }
}
/**********************************************************************
  End of file
 **********************************************************************/
