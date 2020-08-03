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

#include <stdio.h>

/* header include files (.h) */
#include "common_def.h"
#include "rgu.h"           /* RGU interface includes*/
#include "lkw.h"           /* LKW defines */
#include "ckw.h"           /* CKW defines */
#include "kwu.h"           /* KWU defines */
#include "kw_env.h"        /* RLC environment options */
#include "kw.h"            /* RLC defines */


/* header/extern include files (.x) */
#include "rgu.x"           
#include "lkw.x"           /* LKW */
#include "ckw.x"           /* CKW */
#include "kwu.x"           /* KWU */
#include "kw_err.h"
#include "kw.x"

#include "rlc.h"
#include "du_app_rlc_inf.h"

extern U16 getTransId();
/*******************************************************************
 *
 * @brief Handles Ue Create Request from DU APP
 *
 * @details
 *
 *    Function : RlcUlHdlUeCreateReq
 *
 *    Functionality:
 *      Handles Ue create Request from DU APP
 *
 * @params[in] Post structure pointer
 *             RlcUeCfg pointer 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t RlcUlHdlUeCreateReq(Pst *pst, RlcUeCfg *ueCfg)
{
   uint8_t idx;
   uint8_t ret = ROK;
   CkwCfgInfo *rlcUeCfg = NULLP;

   KwCb *rlcCb = NULLP;
   rlcCb = KW_GET_KWCB(pst->dstInst);
	KW_ALLOC(rlcCb, rlcUeCfg, sizeof(CkwCfgInfo));
	if(rlcUeCfg)
	{
      memset(rlcUeCfg, 0, sizeof(CkwCfgInfo));

      rlcUeCfg->ueId   = ueCfg->ueIdx;
      rlcUeCfg->cellId = ueCfg->cellId;
      rlcUeCfg->numEnt = ueCfg->numLcs;
      rlcUeCfg->transId = getTransId();
 
      for(idx = 0; idx < ueCfg->numLcs; idx++)
      {
         rlcUeCfg->entCfg[idx].rbId           = ueCfg->rlcBearerCfg[idx].rbId;
         rlcUeCfg->entCfg[idx].rbType         = ueCfg->rlcBearerCfg[idx].rbType;   // SRB or DRB
         rlcUeCfg->entCfg[idx].lCh[0].lChId   = ueCfg->rlcBearerCfg[idx].lcId;   
         rlcUeCfg->entCfg[idx].lCh[0].type    = ueCfg->rlcBearerCfg[idx].lcType;
         rlcUeCfg->entCfg[idx].entMode        = ueCfg->rlcBearerCfg[idx].rlcMode;
         rlcUeCfg->entCfg[idx].cfgType        = CKW_CFG_ADD;
         switch(rlcUeCfg->entCfg[idx].entMode)
         {

            case RLC_AM:
            {
               rlcUeCfg->entCfg[idx].dir      = RLC_CFG_DIR_BOTH;
               /* DL AM INFO */
               rlcUeCfg->entCfg[idx].m.amInfo.dl.snLen = ueCfg->rlcBearerCfg[idx].u.amCfg.dlAmCfg.snLenDl; 
               rlcUeCfg->entCfg[idx].m.amInfo.dl.pollRetxTmr = ueCfg->rlcBearerCfg[idx].u.amCfg.dlAmCfg.pollRetxTmr;
               rlcUeCfg->entCfg[idx].m.amInfo.dl.pollPdu = ueCfg->rlcBearerCfg[idx].u.amCfg.dlAmCfg.pollPdu; 
               rlcUeCfg->entCfg[idx].m.amInfo.dl.pollByte = ueCfg->rlcBearerCfg[idx].u.amCfg.dlAmCfg.pollByte; 
               rlcUeCfg->entCfg[idx].m.amInfo.dl.maxRetx = ueCfg->rlcBearerCfg[idx].u.amCfg.dlAmCfg.maxRetxTh;

               /* UL AM INFO */
               rlcUeCfg->entCfg[idx].m.amInfo.ul.snLen = ueCfg->rlcBearerCfg[idx].u.amCfg.ulAmCfg.snLenUl; 
               rlcUeCfg->entCfg[idx].m.amInfo.ul.staProhTmr = ueCfg->rlcBearerCfg[idx].u.amCfg.ulAmCfg.statProhTmr;
               rlcUeCfg->entCfg[idx].m.amInfo.ul.reOrdTmr = ueCfg->rlcBearerCfg[idx].u.amCfg.ulAmCfg.reAssemTmr;

               break;
            }

            case RLC_UM_BI_DIRECTIONAL:
            {
               rlcUeCfg->entCfg[idx].dir      = RLC_CFG_DIR_BOTH;
               rlcUeCfg->entCfg[idx].m.umInfo.ul.snLen = ueCfg->rlcBearerCfg[idx].u.amCfg.ulAmCfg.snLenUl;
 
               rlcUeCfg->entCfg[idx].m.umInfo.ul.reOrdTmr = ueCfg->rlcBearerCfg[idx].u.amCfg.ulAmCfg.reAssemTmr;
               rlcUeCfg->entCfg[idx].m.umInfo.dl.snLen = ueCfg->rlcBearerCfg[idx].u.amCfg.dlAmCfg.snLenDl; 

              break;
            }

            case RLC_UM_UNI_DIRECTIONAL_UL:
            {
               rlcUeCfg->entCfg[idx].dir      = RLC_CFG_DIR_UL;

               rlcUeCfg->entCfg[idx].m.umInfo.ul.snLen = ueCfg->rlcBearerCfg[idx].u.amCfg.ulAmCfg.snLenUl;
 
               rlcUeCfg->entCfg[idx].m.umInfo.ul.reOrdTmr = ueCfg->rlcBearerCfg[idx].u.amCfg.ulAmCfg.reAssemTmr;
              break;
            }

            case RLC_UM_UNI_DIRECTIONAL_DL:
            {
               rlcUeCfg->entCfg[idx].dir      = RLC_CFG_DIR_DL;
               rlcUeCfg->entCfg[idx].m.umInfo.dl.snLen = ueCfg->rlcBearerCfg[idx].u.amCfg.dlAmCfg.snLenDl; 
              break;
            }
            default:
               break;
         } /* End of switch(entMode) */
      }
      ret = KwUiCkwCfgReq(pst, rlcUeCfg);
      }
      else
      {
         DU_LOG("\nRLC: Failed to allocate memory ");
	 ret = RFAILED;
      }
   KW_FREE_SHRABL_BUF(pst->region, pst->pool, ueCfg, sizeof(RlcUeCfg));

   return ret;
}
/**********************************************************************
         End of file
**********************************************************************/
