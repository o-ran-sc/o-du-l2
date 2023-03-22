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
#include "rlc_env.h"        /* RLC environment options */
#include "rlc_err.h"


/* header/extern include files (.x) */
#include "rgu.x"           
#include "lkw.x"           /* LKW */
#include "ckw.x"           /* CKW */
#include "kwu.x"           /* KWU */

#include "rlc_utils.h"            /* RLC defines */
#include "du_app_rlc_inf.h"
#include "rlc_mgr.h"

RlcDuUeCreateRsp rlcUeCreateRspOpts[] =
{
   packRlcDuUeCreateRsp,     /* 0 - loosely coupled */
   DuProcRlcUeCreateRsp,     /* 1 - tightly coupled */
   packRlcDuUeCreateRsp     /* 2 - LWLC loosely coupled */
};

RlcDuUeReconfigRsp rlcUeReconfigRspOpts[] =
{
   packRlcDuUeReconfigRsp,     /* 0 - loosely coupled */
   DuProcRlcUeReconfigRsp,     /* 1 - tightly coupled */
   packRlcDuUeReconfigRsp     /* 2 - LWLC loosely coupled */
};

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
uint8_t SendRlcUeCreateRspToDu(Pst *pst, RlcCfgCfmInfo *cfgRsp)
{
   /* jump to specific primitive depending on configured selector */
   uint8_t ret = ROK;
   RlcCb   *gCb;
   RlcUeCreateRsp *ueRsp = NULLP;

   gCb = RLC_GET_RLCCB(pst->srcInst);
   RLC_ALLOC_SHRABL_BUF(pst->region, pst->pool, ueRsp, sizeof(RlcUeCreateRsp));
   if(!ueRsp)
   {  
      DU_LOG("\nERROR  -->  RLC: Memory allocation failed for ueRsp at SendRlcUeCreateRspToDu()");
      ret = RFAILED;
   }
   else
   {
      /* Mapping Old api to New Api */
      ret = fillRlcUeCreateRsp(ueRsp, cfgRsp);
      if(!ret)
      {
         ret = (*rlcUeCreateRspOpts[pst->selector])(pst, ueRsp);
         if(ret)
         {
            DU_LOG("\nERROR  -->  RLC: Failed at SendRlcUeCreateRspToDu()");
            RLC_FREE_SHRABL_BUF(pst->region, pst->pool, ueRsp, sizeof(RlcUeCreateRsp));
         }
      }
      else
      {
         DU_LOG("\nERROR  -->  RLC: Failed at fillRlcUeCreateRsp() for event %d", pst->event);
         RLC_FREE_SHRABL_BUF(pst->region, pst->pool, ueRsp, sizeof(RlcUeCreateRsp));
      }
   }
   RLC_FREE(gCb, cfgRsp, sizeof(RlcCfgCfmInfo));
   return ret;
} /* end of SendRlcUeCreateRspToDu */

/***********************************************************
 *
 * @brief  
 *
 *        Handler for the ue Reconfig response to DUAPP
 *
 * @b Description:
 *
 *        This function reports  ue Reconfig response to DUAPP
 *
 *  @param[in] post         Post structure  
 *  @param[in] cfgRsp       ue reconfig Config Response
 *
 *  @return  uint16_t
 *      -# ROK 
 *      -# RFAILED
 *
 *************************************************************/
uint8_t SendRlcUeReconfigRspToDu(Pst *pst, RlcCfgCfmInfo *cfgRsp)
{
   /* jump to specific primitive depending on configured selector */
   uint8_t ret = ROK;
   RlcCb   *gCb;
   RlcUeReconfigRsp *ueRsp = NULLP;

   gCb = RLC_GET_RLCCB(pst->srcInst);
   RLC_ALLOC_SHRABL_BUF(pst->region, pst->pool, ueRsp, sizeof(RlcUeReconfigRsp));
   if(!ueRsp)
   {  
      DU_LOG("\nERROR  -->  RLC: Memory allocation failed for ueRsp at SendRlcUeReconfigRspToDu()");
      ret = RFAILED;
   }
   else
   {
      /* Mapping Old api to New Api */
      ret = fillRlcUeCreateRsp(ueRsp, cfgRsp);
      if(!ret)
      {
         ret = (*rlcUeReconfigRspOpts[pst->selector])(pst, ueRsp);
         if(ret)
         {
            DU_LOG("\nERROR  -->  RLC: Failed at SendRlcUeReconfigRspToDu()");
            RLC_FREE_SHRABL_BUF(pst->region, pst->pool, ueRsp, sizeof(RlcUeReconfigRsp));
         }
      }
      else
      {
         DU_LOG("\nERROR  -->  RLC: Failed at fillRlcUeCreateRsp() for event %d", pst->event);
         RLC_FREE_SHRABL_BUF(pst->region, pst->pool, ueRsp, sizeof(RlcUeReconfigRsp));
      }
   }
   RLC_FREE(gCb, cfgRsp, sizeof(RlcCfgCfmInfo));
   return ret;
} /* end of SendRlcUeReconfigRspToDu */

/**********************************************************************
         End of file
**********************************************************************/
