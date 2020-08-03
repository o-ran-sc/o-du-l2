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
#include "tfu.h"           /* TFU Interface defines */
#include "lrg.h"
#include "lkw.h"           /* LKW defines */
#include "ckw.h"           /* CKW defines */
#include "kwu.h"           /* KWU defines */
#include "kw_env.h"        /* RLC environment options */
#include "kw.h"            /* RLC defines */


/* header/extern include files (.x) */
#include "rgu.x"           
#include "tfu.x"           /* RGU Interface includes */
#include "lrg.x"
#include "lkw.x"           /* LKW */
#include "ckw.x"           /* CKW */
#include "kwu.x"           /* KWU */
#include "kw_err.h"
#include "kw.x"
#include "du_app_rlc_inf.h"

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
   CkwCfgInfo *rlcUeCfg;
 
   memset(rlcUeCfg, 0, sizeof(CkwCfgInfo));
   rlcUeCfg = (CkwCfgInfo *)malloc(sizeof(CkwCfgInfo));

   rlcUeCfg->ueId   = ueCfg->ueIdx;
   rlcUeCfg->cellId = ueCfg->cellIdx;
   rlcUeCfg->numEnt = ueCfg->numLcs;
   rlcUeCfg->transId = 1;
 
   for(idx = 0; idx < ueCfg->numLcs; idx++)
   {
      rlcUeCfg->entCfg[idx].rbId           = RB_ID;
      rlcUeCfg->entCfg[idx].rbType         = ueCfg->rlcBearerCfg[idx].lcId;   // SRB or DRB
      rlcUeCfg->entCfg[idx].lCh[0].lChId   = ueCfg->rlcBearerCfg[idx].lcId;   
      rlcUeCfg->entCfg[idx].lCh[0].type    = LCH_DTCH;
      rlcUeCfg->entCfg[idx].entMode        = ueCfg->rlcBearerCfg[idx].rlcMode;
      rlcUeCfg->entCfg[idx].dir            = ueCfg->rlcBearerCfg[idx].rlcMode;
      rlcUeCfg->entCfg[idx].cfgType        = CKW_CFG_ADD;
      switch(rlcUeCfg->entCfg[idx].entMode)
      {

         case RLC_MODE_AM:
         {
            /* DL AM INFO */
            rlcUeCfg->entCfg[idx].m.amInfo.dl.snLen = ueCfg->rlcBearerCfg[idx].amInfo.dlAm.snLenDl; 
            rlcUeCfg->entCfg[idx].m.amInfo.dl.pollRetxTmr = ueCfg->rlcBearerCfg[idx].amInfo.dlAm.pollRetxTmr;
            rlcUeCfg->entCfg[idx].m.amInfo.dl.pollPdu = ueCfg->rlcBearerCfg[idx].amInfo.dlAm.pollPdu; 
            rlcUeCfg->entCfg[idx].m.amInfo.dl.pollByte = ueCfg->rlcBearerCfg[idx].amInfo.dlAm.pollByte; 
            rlcUeCfg->entCfg[idx].m.amInfo.dl.maxRetx = ueCfg->rlcBearerCfg[idx].amInfo.dlAm.maxRetxTh;

            /* UL AM INFO */
            rlcUeCfg->entCfg[idx].m.amInfo.ul.snLen = ueCfg->rlcBearerCfg[idx].amInfo.ulAm.snLenUl; 
            rlcUeCfg->entCfg[idx].m.amInfo.ul.staProhTmr = ueCfg->rlcBearerCfg[idx].amInfo.ulAm.statProhTmr;
            rlcUeCfg->entCfg[idx].m.amInfo.ul.reOrdTmr = ueCfg->rlcBearerCfg[idx].amInfo.ulAm.reAssemTmr;

            break;
         }

         case RLC_MODE_UM_BI_DIRECTIONAL:
         {
            rlcUeCfg->entCfg[idx].m.umInfo.ul.snLen = ueCfg->rlcBearerCfg[idx].amInfo.ulAm.snLenUl;
 
            rlcUeCfg->entCfg[idx].m.umInfo.ul.reOrdTmr = ueCfg->rlcBearerCfg[idx].amInfo.ulAm.reAssemTmr;
            rlcUeCfg->entCfg[idx].m.umInfo.dl.snLen = ueCfg->rlcBearerCfg[idx].amInfo.dlAm.snLenDl; 

           break;
         }

         case RLC_MODE_UM_UNI_DIRECTIONAL_UL:
         {

            rlcUeCfg->entCfg[idx].m.umInfo.ul.snLen = ueCfg->rlcBearerCfg[idx].amInfo.ulAm.snLenUl;
 
            rlcUeCfg->entCfg[idx].m.umInfo.ul.reOrdTmr = ueCfg->rlcBearerCfg[idx].amInfo.ulAm.reAssemTmr;
           break;
         }

         case RLC_MODE_UM_UNI_DIRECTIONAL_DL:
         {
            rlcUeCfg->entCfg[idx].m.umInfo.dl.snLen = ueCfg->rlcBearerCfg[idx].amInfo.dlAm.snLenDl; 
           break;
         }
         default:
            break;
      } /* End of switch(entMode) */
      KW_FREE_SHRABL_BUF(pst->region, pst->pool, ueCfg, sizeof(RlcUeCfg));
      ret = KwUiCkwCfgReq(pst, rlcUeCfg);
   }

   return ret;
}
/**********************************************************************
         End of file
**********************************************************************/
