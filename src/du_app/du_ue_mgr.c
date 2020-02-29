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
 /* This file contains UE management handling functionality for DU APP */
#include "du_ue_mgr.h"
#include "du_cfg.h"

/******************************************************************
 *
 * @brief Send UE configuration to RLC
 *
 * @details
 *
 *    Function : duSendUeCreateReqToRlc
 *
 *    Functionality: Send UeCreateReqToRlc
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
S16 duSendUeCreateReqToRlc()
{
   U8  idx;
   Pst pst;
   CkwCfgInfo ueCfg;
   
   DU_SET_ZERO(&ueCfg, sizeof(ueCfg));
   DU_SET_ZERO(&pst, sizeof(Pst));

   ueCfg.ueId = 1;
   ueCfg.cellId = NR_CELL_ID;
   ueCfg.numEnt = 1;
   
   for(idx = 0; idx < ueCfg.numEnt; idx++)
   {
      ueCfg.entCfg[idx].rbId = 1;
      ueCfg.entCfg[idx].rbType = CM_LTE_DRB;
      ueCfg.entCfg[idx].entMode = CM_LTE_MODE_UM;
      ueCfg.entCfg[idx].dir = CKW_CFG_DIR_BOTH;

      switch(ueCfg.entCfg[idx].entMode)
      {
         case CM_LTE_MODE_TM:
         {
            break;
         }

         case CM_LTE_MODE_UM:
         {
            ueCfg.entCfg[idx].m.umInfo.dl.snLen = 1;      /* For 12 bit SN */
            ueCfg.entCfg[idx].m.umInfo.ul.snLen = 1;      /* For 12 bit SN */
            ueCfg.entCfg[idx].m.umInfo.ul.reOrdTmr = 10;  /* in msec */
            break;
         }

         case CM_LTE_MODE_AM:
         {
            break;
         }
         
         defualt:
            break;
      } /* End of switch(entMode) */
   } /* End of entity configuration for loop */

   /* Fill Pst */
   pst.selector  = DU_SELECTOR_LWLC;
   pst.srcEnt    = ENTDUAPP;
   pst.dstEnt    = ENTKW;
   pst.dstInst   = RLC_UL_INST;
   pst.dstProcId = DU_PROC;
   pst.srcProcId = DU_PROC;
   pst.region    = duCb.init.region;

   /* Sending to RLC */
   packUeCreateReq(&pst, &ueCfg);

   RETVALUE(ROK); 
} /* End of duSendUeCreateReqToRlc */

/**********************************************************************
         End of file
***********************************************************************/
