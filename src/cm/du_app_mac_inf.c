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

#include "envopt.h"        /* Environment options */
#include "envdep.h"        /* Environment dependent */
#include "envind.h"        /* Environment independent */
#include "gen.h"           /* General */
#include "ssi.h"           /* System services */
#include "ss_queue.h"
#include "ss_task.h"
#include "ss_msg.h"

#include "gen.x"           /* general */
#include "ssi.x"           /* system services */
#include "cm_tkns.x"       /* Common Token Definitions */
#include "cm_llist.x"      /* Common Link List Definitions */
#include "cm_lib.x"        /* Common Library Definitions */
#include "cm_hash.x"       /* Common Hash List Definitions */
#include "cm_lte.x"        /* Common LTE Defines */

#include "du_cfg.h"
#include "du_app_mac_inf.h"
#include "cm.h"

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
int  packMacCellCfg(Pst *pst, MacCellCfg *macCellCfg)
{
   if(pst->selector == DU_SELECTOR_LC)
   {
      /* we are now implemented only light wieght lossely coupled interface */
      return RFAILED;
   }
   else if(pst->selector == DU_SELECTOR_LWLC)
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
int  unpackDuMacCellCfg(
   DuMacCellCfgReq func,
   Pst *pst,
   Buffer *mBuf)
{
   U16 ret = ROK;
   MacCellCfg *macCellCfg;

   if(pst->selector == DU_SELECTOR_LWLC)
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
int  packMacCellCfgCfm(Pst *pst, MacCellCfgCfm *macCellCfgCfm)
{
   if(pst->selector == DU_SELECTOR_LC)
   {
      Buffer *mBuf = NULLP;
      if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) 
      {
         return RFAILED;
      }

      /* pack the transaction ID in CNF structure */
		CMCHKPK(SPkU8, macCellCfgCfm->rsp, mBuf);
      CMCHKPK(SPkU16, macCellCfgCfm->transId, mBuf);

      return SPstTsk(pst,mBuf);
   }
   else if(pst->selector == DU_SELECTOR_LWLC)
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
int unpackMacCellCfgCfm(
   DuMacCellCfgCfm func, 
   Pst *pst,
   Buffer *mBuf)
{
   MacCellCfgCfm macCellCfgCfm;

   if(pst->selector == DU_SELECTOR_LC)
   {
      /* unpack the transaction ID in CNF structure */
      CMCHKUNPK(SUnpkU16, &(macCellCfgCfm.transId), mBuf);
      CMCHKUNPK(SUnpkU8, &(macCellCfgCfm.rsp), mBuf);

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
uint16_t packMacCellStartReq(Pst *pst, MacCellStartInfo  *cellStartInfo)
{
   if(pst->selector == DU_SELECTOR_LC)
   {
      /* Loose coupling not supported */
      return RFAILED;
   }
   else if(pst->selector == DU_SELECTOR_LWLC)
   {
      Buffer *mBuf = NULLP;

      if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK)
      {
         DU_LOG("\nDU APP : Memory allocation failed for cell start req pack");
         return RFAILED;
      }

      /* pack the address of the structure */
      CMCHKPK(cmPkPtr,(PTR)cellStartInfo, mBuf);

      return SPstTsk(pst,mBuf);
   }
   return ROK;
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
uint16_t unpackMacCellStartReq(DuMacCellStartReq func, Pst *pst, Buffer *mBuf)
{
   MacCellStartInfo  *cellStartInfo;
 
   if(pst->selector == DU_SELECTOR_LWLC)
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
uint16_t packMacCellStopReq(Pst *pst, MacCellStopInfo  *cellStopInfo)
{
   if(pst->selector == DU_SELECTOR_LC)
   {
      /* Loose coupling not supported */
      return RFAILED;
   }
   else if(pst->selector == DU_SELECTOR_LWLC)
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
uint16_t unpackMacCellStopReq(DuMacCellStopReq func, Pst *pst, Buffer *mBuf)
{
   MacCellStopInfo  *cellStopInfo;
  
   if(pst->selector == DU_SELECTOR_LWLC)
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
uint16_t packMacSlotInd(Pst *pst, SlotInfo *slotInfo )
{
   Buffer *mBuf = NULLP;
 
   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK)
   {
      DU_LOG("\nDU APP : Memory allocation failed for cell start req pack");
      return RFAILED;
   }
 
   if(pst->selector == DU_SELECTOR_LC)
   {
      CMCHKPK(SPkU16, slotInfo->cellId, mBuf);
      CMCHKPK(SPkU16, slotInfo->sfn, mBuf);
      CMCHKPK(SPkU16, slotInfo->slot, mBuf);

      CM_FREE_SHRABL_BUF(pst->region, pst->pool, slotInfo, sizeof(SlotInfo));
		slotInfo = NULL;
   }
   else if(pst->selector == DU_SELECTOR_LWLC)
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
uint16_t unpackMacSlotInd(DuMacSlotInd func, Pst *pst, Buffer *mBuf)
{
   if(pst->selector == DU_SELECTOR_LWLC)
   {
      SlotInfo *slotInfo;

      /* unpack the address of the structure */
      CMCHKUNPK(cmUnpkPtr, (PTR *)&slotInfo, mBuf);
      SPutMsg(mBuf);
      return (*func)(pst, slotInfo);
   }
   else if(pst->selector == DU_SELECTOR_LC)
   {
      SlotInfo slotInfo;

      CMCHKUNPK(SUnpkU16, &(slotInfo.slot), mBuf);
      CMCHKUNPK(SUnpkU16, &(slotInfo.sfn), mBuf);
      CMCHKUNPK(SUnpkU16, &(slotInfo.cellId), mBuf);

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
uint16_t packMacStopInd(Pst *pst, MacCellStopInfo *cellStopId)
{
   Buffer *mBuf = NULLP;

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK)
   {
      DU_LOG("\nDU APP : Memory allocation failed for stop Ind pack");
      return RFAILED;
   }

   if(pst->selector == DU_SELECTOR_LC)
   {
      /*pack the payload here*/
      DU_LOG("\nDU APP : Packed CellId");
      CMCHKPK(SPkU16, cellStopId->cellId, mBuf);
      CM_FREE_SHRABL_BUF(pst->region, pst->pool, cellStopId, sizeof(MacCellStopInfo));
      cellStopId = NULL;
   }
   else if(pst->selector == DU_SELECTOR_LWLC)
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
uint16_t unpackMacStopInd(DuMacStopInd func, Pst *pst, Buffer *mBuf)
{
   if(pst->selector == DU_SELECTOR_LWLC)
   {
      MacCellStopInfo *cellStopId;
      /* unpack the address of the structure */
      CMCHKUNPK(cmUnpkPtr, (PTR *)&cellStopId, mBuf);
      SPutMsg(mBuf);
      return (*func)(pst, cellStopId);
   }
   else if(pst->selector == DU_SELECTOR_LC)
   {
      MacCellStopInfo cellStopId;
      CMCHKUNPK(SUnpkU16, &(cellStopId.cellId), mBuf);

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
/**********************************************************************
         End of file
**********************************************************************/
