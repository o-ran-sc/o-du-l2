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

#ifdef EGTP_TEST
U32 sduId = 0;
#endif

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
   CkwCfgInfo *ueCfg;
   
   DU_SET_ZERO(&ueCfg, sizeof(ueCfg));
   DU_SET_ZERO(&pst, sizeof(Pst));

   DU_ALLOC(ueCfg, sizeof(CkwCfgInfo));

#ifdef EGTP_TEST
   ueCfg->ueId = UE_ID;
#endif
   ueCfg->cellId = NR_CELL_ID;
   ueCfg->numEnt = 1;
   
   for(idx = 0; idx < ueCfg->numEnt; idx++)
   {
#ifdef EGTP_TEST
      ueCfg->entCfg[idx].rbId           = RB_ID;
      ueCfg->entCfg[idx].rbType         = CM_LTE_DRB;
      ueCfg->entCfg[idx].lCh[0].lChId   = LC_ID;
      ueCfg->entCfg[idx].lCh[0].type    = CM_LTE_LCH_DTCH;
#endif
      ueCfg->entCfg[idx].entMode        = CM_LTE_MODE_UM;
      ueCfg->entCfg[idx].dir            = CKW_CFG_DIR_BOTH;
      switch(ueCfg->entCfg[idx].entMode)
      {
         case CM_LTE_MODE_TM:
         {
            break;
         }

         case CM_LTE_MODE_UM:
         {
            ueCfg->entCfg[idx].m.umInfo.dl.snLen = 1;      /* For 12 bit SN */
            ueCfg->entCfg[idx].m.umInfo.ul.snLen = 1;      /* For 12 bit SN */
            ueCfg->entCfg[idx].m.umInfo.ul.reOrdTmr = 10;  /* in msec */
            break;
         }

         case CM_LTE_MODE_AM:
         {
            break;
         }
         
         default:
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
   packUeCreateReq(&pst, ueCfg);

   RETVALUE(ROK); 
} /* End of duSendUeCreateReqToRlc */

/*******************************************************************
 *
 * @brief Handles EGTP data from CU 
 *
 * @details
 *
 *    Function : duHdlEgtpData
 *
 *    Functionality: 
 *      Processes EGTP header and sends data to RLC
 *
 * @params[in]  Pointer to EGTP Message 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
S16 duHdlEgtpDlData(EgtpMsg  *egtpMsg)
{
   /* TODO : Extract RbId/UeID/CellID/SduId from database
      using tunnel id in egtp header */
   
   DU_LOG("\nDU_APP : Processing DL data");
   
   Pst pst;
   KwuDatReqInfo datReqInfo;

#ifdef EGTP_TEST
   datReqInfo.rlcId.rbId = RB_ID;
   datReqInfo.rlcId.rbType = CM_LTE_DRB;
   datReqInfo.rlcId.ueId = UE_ID;
   datReqInfo.rlcId.cellId = NR_CELL_ID;
   
   datReqInfo.sduId = ++sduId;
   datReqInfo.lcType = CM_LTE_LCH_DTCH;
#endif
   /* Filling pst and Sending to RLC DL */
   pst.selector  = DU_SELECTOR_LWLC;
   pst.srcEnt    = ENTDUAPP;
   pst.dstEnt    = ENTKW;
   pst.dstInst   = RLC_DL_INST;
   pst.dstProcId = DU_PROC;
   pst.srcProcId = DU_PROC;
   pst.region    = duCb.init.region;

   cmPkKwuDatReq(&pst, &datReqInfo, egtpMsg->msg);
   return ROK;
}

/*******************************************************************
 *
 * @brief Handles UL data and send to CU
 *
 * @details
 *
 *    Function : duHdlRlcUlData
 *
 *    Functionality: 
 *     Processes UL Data from RLC and sends to CU
 * 
 *  @params[in]  Pointer to EGTP Message 
 *  @return ROK     - success
 *          RFAILED - failure
 * 
 *****************************************************************/

PUBLIC S16 duHdlRlcUlData(Pst *pst, KwuDatIndInfo* datInd, Buffer *mBuf)
{
   DU_LOG("\nDU_APP : Received UL Data at DU_APP");
 
   /* Send UL data to CU via EGTP */
   duSendEgtpDatInd(mBuf);
   SPutMsg(mBuf);

   return ROK;
}

/**********************************************************************
         End of file
***********************************************************************/
