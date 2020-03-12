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

#include "mac_interface.h"

/**************************************************************************
 * @brief Function to pack Loose Coupled 
 *        MAC cell config parameters required by MAC
 *
 * @details
 *
 *      Function : packLcMacCellCfg
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
S16 packLcMacCellCfg(Pst *pst, MacCellCfg *macCellCfg)
{
   return ROK;
}

/**************************************************************************
 * @brief Function to pack tightly Coupled 
 *        MAC cell config parameters required by MAC
 *
 * @details
 *
 *      Function : packTcMacCellCfg
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
S16 packTcMacCellCfg(Pst *pst, MacCellCfg *macCellCfg)
{
   return ROK;
}

/**************************************************************************
 * @brief Function to pack light weight Loose Coupled 
 *        MAC cell config parameters required by MAC
 *
 * @details
 *
 *      Function : packLwLcMacCellCfg
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
S16 packLwLcMacCellCfg(Pst *pst, MacCellCfg *macCellCfg)
{
   Buffer *mBuf = NULLP;

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) 
   {
      RETVALUE(RFAILED);
   }

   /* pack the address of the structure */
   CMCHKPK(cmPkPtr,(PTR)macCellCfg, mBuf);

   RETVALUE(SPstTsk(pst,mBuf));
}

/**************************************************************************
 * @brief Function to pack Loose Coupled 
 *        MAC cell config parameters required by MAC
 *
 * @details
 *
 *      Function : unpackLwLcMacCellCfg
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
void cmUnpackLwLcMacCellCfg(
   DuMacCellCfgReq func,
   Pst *pst,
   Buffer *mBuf)
{
   MacCellCfg *macCellCfg;

   /* unpack the address of the structure */
   CMCHKUNPK(cmUnpkPtr, (PTR *)&macCellCfg, mBuf);

   RETVALUE((*func)(pst, macCellCfg));
}

/**************************************************************************
 * @brief Function to pack Loose Coupled 
 *        MAC cell config confirm message
 *
 * @details
 *
 *      Function : cmPackLcMacCellCfgCfm
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
U16 cmPackLcMacCellCfgCfm(Pst *pst, MacCellCfgCfm *macCellCfgCfm)
{
   Buffer *mBuf = NULLP;
   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) 
   {
      RETVALUE(RFAILED);
   }

   /* pack the transaction ID in CNF structure */
   CMCHKPK(SPkU16, macCellCfgCfm->transId, mBuf);

   RETVALUE(SPstTsk(pst,mBuf));
}

/**************************************************************************
 * @brief Function to pack tightly Coupled 
 *        MAC cell config confirm message
 *
 * @details
 *
 *      Function : cmPackTcMacCellCfgCfm
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
U16 cmPackTcMacCellCfgCfm(Pst *pst, MacCellCfgCfm *macCellCfgCfm)
{
   return ROK;
}

/**************************************************************************
 * @brief Function to pack light weight Loose Coupled 
 *        MAC cell config confirm message
 *
 * @details
 *
 *      Function : cmPackLwlcMacCellCfgCfm
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
U16 cmPackLwlcMacCellCfgCfm(Pst *pst, MacCellCfgCfm *macCellCfgCfm)
{
   return ROK;
}

/**************************************************************************
 * @brief Function to pack MAC cell config confirm message
 *
 * @details
 *
 *      Function : cmUnpackLcMacCellCfgCfm
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
S16 cmUnpackLcMacCellCfgCfm(
   DuMacCellCfgCfm func, 
   Pst *pst,
   Buffer *mBuf)
{
   MacCellCfgCfm macCellCfgCfm;
   /* pack the transaction ID in CNF structure */
   CMCHKUNPK(SUnpkU16, &(macCellCfgCfm.transId), mBuf);

   RETVALUE((*func)(&macCellCfgCfm));
}
