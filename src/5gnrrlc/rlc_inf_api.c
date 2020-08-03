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
/* This file defines API towards RLC interface */
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

#include "du_app_rlc_inf.h"

RlcUlDuUeCreateRsp rlcUlUeCreateRspOpts[] =
{
   packRlcUlDuUeCreateRsp,     /* 0 - loosely coupled */
   duProcRlcUlUeCreateRsp,     /* 1 - tightly coupled */
   packRlcUlDuUeCreateRsp,     /* 2 - LWLC loosely coupled */
};

/*******************************************************************
 *
 * @brief Fills RLC UL UE Cfg Rsp from CkwCfgRsp 
 *
 * @details
 *
 *    Function : fillRlcUlUeCfgRsp
 *
 *    Functionality: 
 *     Fills RLC UL UE Cfg Rsp from CkwCfgRsp
 * 
 *  @params[in]  Pointer to RlcCfgCfm
 *               Pointer to RlcUeCfgRsp
 *
 *  @return void
 * 
 *****************************************************************/

void fillRlcUlUeCfgRsp(RlcUeCfgRsp *rlcCfgRsp, RlcCfgCfmInfo *ckwCfgRsp)
{
   uint8_t idx;
 
   rlcCfgRsp->cellId = ckwCfgRsp->cellId;
   rlcCfgRsp->ueIdx  = ckwCfgRsp->ueId;
   for(idx = 0; idx < ckwCfgRsp->numEnt; idx++)
   {
      if(ckwCfgRsp->entCfgCfm[idx].status.status == CKW_CFG_CFM_OK)
      {
         rlcCfgRsp->result = RLC_DU_APP_RSP_OK;
	 rlcCfgRsp->reason = ckwCfgRsp->entCfgCfm[idx].status.reason;
      }
      else
      {
         rlcCfgRsp->result = RLC_DU_APP_RSP_NOK;
	 rlcCfgRsp->reason = ckwCfgRsp->entCfgCfm[idx].status.reason;
      }
   }
}
/***********************************************************
 *
 * @brief  
 *
 *        Handler for the ue create response to DUAPP
 *
 * @b Description:
 *
 *        This function reports  ue create response to DUAPP
 *
 *  @param[in] post         Post structure  
 *  @param[in] cfgRsp       ue create Config Response
 *
 *  @return  uint16_t
 *      -# ROK 
 *      -# RFAILED
 *
 *************************************************************/
uint8_t SendRlcUlUeCreateRspToDu(Pst *pst, RlcCfgCfmInfo *cfgRsp)
{
   /* jump to specific primitive depending on configured selector */

   /* Mapping Old api to New Api */
   RlcUeCfgRsp *ueCreateRsp = NULLP;
   RLC_ALLOC_SHRABL_BUF(pst->region, pst->pool, ueCreateRsp, sizeof(RlcUeCfgRsp));
   if(ueCreateRsp)
   {
      fillRlcUlUeCfgRsp(ueCreateRsp, cfgRsp);
      return (*rlcUlUeCreateRspOpts[pst->selector])(pst, ueCreateRsp);
   }
   else
   {
      DU_LOG("RLC: Memory allocation failed for ueCreateRsp at SendRlcUlUeCreateRspToDu()");
      return RFAILED;
   }
} /* end of SendRlcUlUeCreateRspToDu */

/**********************************************************************
         End of file
**********************************************************************/
