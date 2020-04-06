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
U16 packMacCellCfg(Pst *pst, MacCellCfg *macCellCfg)
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
         RETVALUE(RFAILED);
      }

      /* pack the address of the structure */
      CMCHKPK(cmPkPtr,(PTR)macCellCfg, mBuf);

      DU_LOG("\nDU-APP : MAC CELL config sent");
      RETVALUE(SPstTsk(pst,mBuf));
   } 
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
void unpackDuMacCellCfg(
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
U16 packMacCellCfgCfm(Pst *pst, MacCellCfgCfm *macCellCfgCfm)
{
   if(pst->selector == DU_SELECTOR_LC)
   {
      Buffer *mBuf = NULLP;
      if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) 
      {
         RETVALUE(RFAILED);
      }

      /* pack the transaction ID in CNF structure */
		CMCHKPK(SPkU8, macCellCfgCfm->rsp, mBuf);
      CMCHKPK(SPkU16, macCellCfgCfm->transId, mBuf);

      RETVALUE(SPstTsk(pst,mBuf));
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
U16 unpackMacCellCfgCfm(
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

      RETVALUE((*func)(&macCellCfgCfm));
   }
   else
   {
      /* only loose coupling is suported */
   }
   return ROK;
}

/**********************************************************************
         End of file
**********************************************************************/
