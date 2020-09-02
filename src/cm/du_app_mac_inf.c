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

      if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) 
      {
	 return RFAILED;
      }

      /* pack the address of the structure */
      CMCHKPK(cmPkPtr,(PTR)macCellCfg, mBuf);

      DU_LOG("\nDU-APP : MAC CELL config sent");
      return SPstTsk(pst,mBuf);
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
   U16 ret = ROK;
   MacCellCfg *macCellCfg;

   if(pst->selector == ODU_SELECTOR_LWLC)
   {
      /* unpack the address of the structure */
      CMCHKUNPK(cmUnpkPtr, (PTR *)&macCellCfg, mBuf);
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
      if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) 
      {
	 return RFAILED;
      }

      /* pack the transaction ID in CNF structure */
      CMCHKPK(packUint16, macCellCfgCfm->cellId, mBuf);
      CMCHKPK(packUint8, macCellCfgCfm->rsp, mBuf);

      return SPstTsk(pst,mBuf);
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
      CMCHKUNPK(unPackUint8, &(macCellCfgCfm.rsp), mBuf);
      CMCHKUNPK(unPackUint16, &(macCellCfgCfm.cellId), mBuf);
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

      if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK)
      {
	 DU_LOG("\nDU APP : Memory allocation failed for cell start req pack");
	 return RFAILED;
      }

      /* pack the address of the structure */
      CMCHKPK(cmPkPtr,(PTR)cellStartInfo, mBuf);

   }
   return SPstTsk(pst,mBuf);
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
      CMCHKUNPK(cmUnpkPtr, (PTR *)&cellStartInfo, mBuf);
      SPutMsg(mBuf); 
      return (*func)(pst, cellStartInfo);
   }
   else
   {
      /* Nothing to do for loose coupling */
      SPutMsg(mBuf);
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

      if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK)
      {
	 DU_LOG("\nDU APP : Memory allocation failed for cell stop req pack");
	 return RFAILED;
      }

      /* pack the address of the structure */
      CMCHKPK(cmPkPtr,(PTR)cellStopInfo, mBuf);

      return SPstTsk(pst,mBuf);
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
      CMCHKUNPK(cmUnpkPtr, (PTR *)&cellStopInfo, mBuf);
      SPutMsg(mBuf);
      return (*func)(pst, cellStopInfo);
   }
   else
   {
      /* Nothing to do for loose coupling */
      SPutMsg(mBuf);
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

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK)
   {
      DU_LOG("\nDU APP : Memory allocation failed for cell start req pack");
      return RFAILED;
   }

   if(pst->selector == ODU_SELECTOR_LC)
   {
      CMCHKPK(packUint16, slotInfo->cellId, mBuf);
      CMCHKPK(packUint16, slotInfo->sfn, mBuf);
      CMCHKPK(packUint16, slotInfo->slot, mBuf);

      CM_FREE_SHRABL_BUF(pst->region, pst->pool, slotInfo, sizeof(SlotIndInfo));
      slotInfo = NULL;
   }
   else if(pst->selector == ODU_SELECTOR_LWLC)
   {
      /* pack the address of the structure */
      CMCHKPK(cmPkPtr,(PTR)slotInfo, mBuf);
   }
   else
   {
      SPutMsg(mBuf);
   }

   return SPstTsk(pst,mBuf);
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
      CMCHKUNPK(cmUnpkPtr, (PTR *)&slotInfo, mBuf);
      SPutMsg(mBuf);
      return (*func)(pst, slotInfo);
   }
   else if(pst->selector == ODU_SELECTOR_LC)
   {
      SlotIndInfo slotInfo;

      CMCHKUNPK(unPackUint16, &(slotInfo.slot), mBuf);
      CMCHKUNPK(unPackUint16, &(slotInfo.sfn), mBuf);
      CMCHKUNPK(unPackUint16, &(slotInfo.cellId), mBuf);

      SPutMsg(mBuf);
      return (*func)(pst, &slotInfo);

   }
   else
   {
      /* Nothing to do for loose coupling */
      SPutMsg(mBuf);
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

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK)
   {
      DU_LOG("\nDU APP : Memory allocation failed for stop Ind pack");
      return RFAILED;
   }

   if(pst->selector == ODU_SELECTOR_LC)
   {
      /*pack the payload here*/
      DU_LOG("\nDU APP : Packed CellId");
      CMCHKPK(packUint16, cellStopId->cellId, mBuf);
      CM_FREE_SHRABL_BUF(pst->region, pst->pool, cellStopId, sizeof(MacCellStopInfo));
      cellStopId = NULL;
   }
   else if(pst->selector == ODU_SELECTOR_LWLC)
   {
      /* pack the address of the structure */
      CMCHKPK(cmPkPtr,(PTR)cellStopId, mBuf);
   }
   else
   {
      SPutMsg(mBuf);
   }

   return SPstTsk(pst,mBuf);
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
      CMCHKUNPK(cmUnpkPtr, (PTR *)&cellStopId, mBuf);
      SPutMsg(mBuf);
      return (*func)(pst, cellStopId);
   }
   else if(pst->selector == ODU_SELECTOR_LC)
   {
      MacCellStopInfo cellStopId;
      CMCHKUNPK(unPackUint16, &(cellStopId.cellId), mBuf);

      SPutMsg(mBuf);
      return (*func)(pst, &cellStopId);

   }
   else
   {
      /* Nothing to do for loose coupling */
      SPutMsg(mBuf);
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
      if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK)
      {
	 DU_LOG("\nMAC : Memory allocation failed at packMacUlCcchInd");
	 return RFAILED;
      }
      /* pack the address of the structure */
      CMCHKPK(cmPkPtr,(PTR)ulCcchIndInfo, mBuf);
   }
   else
   {
      DU_LOG("\nMAC: Only LWLC supported for UL CCCH Ind ");
      return RFAILED;
   }

   return SPstTsk(pst,mBuf);
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
      CMCHKUNPK(cmUnpkPtr, (PTR *)&ulCcchIndInfo, mBuf);
      SPutMsg(mBuf);
      return (*func)(pst, ulCcchIndInfo);
   }
   else
   {
      /* Nothing to do for other selectors */
      DU_LOG("\n Only LWLC supported for UL CCCH Ind ");
      SPutMsg(mBuf);
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
      if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK)
      {
	 DU_LOG("\nMAC : Memory allocation failed at packMacDlCcchInd");
	 return RFAILED;
      }
      /* pack the address of the structure */
      CMCHKPK(cmPkPtr,(PTR)dlCcchIndInfo, mBuf);
   }
   else
   {
      DU_LOG("\nDU APP: Only LWLC supported for DL CCCH Ind ");
      return RFAILED;
   }

   return SPstTsk(pst,mBuf);
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
      CMCHKUNPK(cmUnpkPtr, (PTR *)&dlCcchIndInfo, mBuf);
      SPutMsg(mBuf);
      return (*func)(pst, dlCcchIndInfo);
   }
   else
   {
      /* Nothing to do for other selectors */
      DU_LOG("\n Only LWLC supported for DL CCCH Ind ");
      SPutMsg(mBuf);
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
      if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK)
      {
	 DU_LOG("\nMAC : Memory allocation failed at packDuMacUeCreateReq");
	 return RFAILED;
      }
      /* pack the address of the structure */
      CMCHKPK(cmPkPtr,(PTR)ueCfg, mBuf);
   }
   else
   {
      DU_LOG("\nMAC: Only LWLC supported for packDuMacUeCreateReq");
      return RFAILED;
   }

   return SPstTsk(pst,mBuf);
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
      CMCHKUNPK(cmUnpkPtr, (PTR *)&ueCfg, mBuf);
      SPutMsg(mBuf);
      return (*func)(pst, ueCfg);
   }
   else
   {
      /* Nothing to do for other selectors */
      DU_LOG("\n Only LWLC supported for UE Create Request ");
      SPutMsg(mBuf);
   }

   return RFAILED;
}

/*******************************************************************
 *
 * @brief Pack and send UE config response from MAC to DU APP
 *
 * @details
 *
 *    Function : packDuMacUeCreateRsp
 *
 *    Functionality:
 *       Pack and send UE config response from MAC to DU APP
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t packDuMacUeCreateRsp(Pst *pst, MacUeCfgRsp *cfgRsp)
{
   Buffer *mBuf = NULLP;

   if(pst->selector == ODU_SELECTOR_LWLC)
   {
      if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK)
      {
	 DU_LOG("\nMAC : Memory allocation failed at packDuMacUeCfgRsp");
	 return RFAILED;
      }
      /* pack the address of the structure */
      CMCHKPK(cmPkPtr,(PTR)cfgRsp, mBuf);
   }
   else
   {
      DU_LOG("\nMAC: Only LWLC supported for packDuMacUeCfgRsp");
      return RFAILED;
   }

   return SPstTsk(pst,mBuf);
}

/*******************************************************************
 *
 * @brief Unpack UE Config Response from MAC to DU APP
 *
 * @details
 *
 *    Function : unpackDuMacUeCreateRsp
 *
 *    Functionality: Unpack UE Config Response from MAC to DU APP
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t unpackDuMacUeCreateRsp(DuMacUeCreateRspFunc func, Pst *pst, Buffer *mBuf)
{
   if(pst->selector == ODU_SELECTOR_LWLC)
   {
      MacUeCfgRsp *cfgRsp;

      /* unpack the address of the structure */
      CMCHKUNPK(cmUnpkPtr, (PTR *)&cfgRsp, mBuf);
      SPutMsg(mBuf);
      return (*func)(pst, cfgRsp);
   }

   SPutMsg(mBuf);
   return RFAILED;
}

/**********************************************************************
  End of file
 **********************************************************************/
