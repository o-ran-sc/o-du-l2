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

/**********************************************************************
 
    Name:   LTE RLC layer
 
    Type:   C include file
 
    Desc:   Defines required by LTE MAC
 
    File:   kw_udx_ul.c
 
**********************************************************************/
static const char* RLOG_MODULE_NAME="UDX";
static int RLOG_MODULE_ID=262144;
static int RLOG_FILE_ID=204;

/** @file kw_udx_ul.c
@brief  UDX Uplink Module 
*/
 
/* header include files (.h) */
#include "common_def.h"
#include "lkw.h"           /* LKW defines */
#include "ckw.h"           /* CKW defines */
#include "kwu.h"           /* KWU defines */
#include "rgu.h"           /* RGU defines */
#include "kw_err.h"        /* Err defines */
#include "kw_env.h"        /* RLC environment options */

#include "kw.h"            /* RLC defines */
#include "kw_udx.h"
#include "kw_ul.h"
#include "kw_dl.h"

/* extern (.x) include files */
#include "lkw.x"           /* LKW */
#include "ckw.x"           /* CKW */
#include "kwu.x"           /* KWU */
#include "rgu.x"           /* RGU */

#include "kw.x"
#include "kw_udx.x"
#include "kw_dl.x"
#include "kw_ul.x"

#include "rlc_utils.h"
#include "du_app_rlc_inf.h"
#include "rlc_mgr.h"

#define RLC_MODULE RLC_DBGMASK_UDX
/* local defines */

/* local externs */

/* forward references */

/* public variable declarations */
EXTERN S16 rlcHdlCrlcUlCfgReq ARGS((RlcCb  *gCb,RlcUlCfgTmpData *cfgTmpData,
                                  RlcCfgCfmInfo *cfmInfo, RlcCfgCfmInfo *cfgCfm));

/**
  * @brief
  * UDX APIs
  */
/**
 * @brief 
 *    Handler for bind confirmation from DL.
 *
 * @details
 *    This function handles the bind confirmation received
 *    from DL. 
 *
 * @param[in] pst     Post structure  
 * @param[in] suId    Service User ID
 * @param[in] status  Status of confirmation 
 * 
 * @return  S16
 *    -# ROK 
 *    -# RFAILED
 */

#ifdef ANSI
S16 rlcUlUdxBndCfm
(
Pst    *pst,  
SuId   suId, 
U8     status 
)
#else
S16 rlcUlUdxBndCfm (pst, suId, status)
Pst    *pst; 
SuId   suId;   
U8     status; 
#endif
{
   U16            event;        
   U16            cause;       
   RlcUdxUlSapCb   *udxSap;   /* RGU SAP Control Block */
   RlcCb           *tRlcCb;

   TRC3(rlcUlUdxBndCfm)

#if (ERRCLASS & ERRCLS_INT_PAR)
   if (pst->dstInst >= MAX_RLC_INSTANCES)
   {
      return  (RFAILED);
   }
#endif
   tRlcCb = RLC_GET_RLCCB(pst->dstInst);

   RLCDBGP_BRIEF(tRlcCb, "rlcUlUdxBndCfm(post, suId(%d), status(%d)\n", 
                suId, status);

#if (ERRCLASS & ERRCLS_INT_PAR)
   if (tRlcCb->init.cfgDone != TRUE)
   {
      RLOG0(L_FATAL, "General configuration not done");
      RLC_SEND_SAPID_ALARM(tRlcCb,suId, 
                          LKW_EVENT_LI_BND_CFM, LCM_CAUSE_INV_STATE);

      return RFAILED;
   }

   if (suId < 0)
   {
      RLOG0(L_ERROR, "Invalid suId");
      RLC_SEND_SAPID_ALARM(tRlcCb,suId, 
                           LKW_EVENT_LI_BND_CFM, LCM_CAUSE_INV_SUID);

      return RFAILED;
   }
#endif /* ERRCLASS & ERRCLS_INT_PAR */

   udxSap = tRlcCb->u.ulCb->udxUlSap + suId;

   RLCDBGP_DETAIL(tRlcCb, "RlcLiRguBndCfm: For RGU SAP state=%d\n", 
                 udxSap->state);

   /* Check rguSap state */
   switch (udxSap->state)
   {
      case RLC_SAP_BINDING:
      {
         if(TRUE == rlcChkTmr(tRlcCb,(PTR)udxSap,RLC_EVT_WAIT_BNDCFM))
         {
             rlcStopTmr (tRlcCb,(PTR)udxSap, RLC_EVT_WAIT_BNDCFM);
         }
         udxSap->retryCnt = 0;
          
         if (status == CM_BND_OK)
         {
            udxSap->state = RLC_SAP_BND;
            event = LCM_EVENT_BND_OK;
            cause = LKW_CAUSE_SAP_BNDENB;
         }
         else
         {
            udxSap->state = RLC_SAP_CFG;
            event = LCM_EVENT_BND_FAIL;
            cause = LKW_CAUSE_UNKNOWN;
         }
      }
      break;

     default:
        event = LKW_EVENT_RGU_BND_CFM;
        cause = LCM_CAUSE_INV_STATE;
        break;
   }
   /* Send an alarm with proper event and cause */
   RLC_SEND_SAPID_ALARM(tRlcCb,suId, event, cause);

   return ROK;
} 

/**
 * @brief 
 *    Handles UDX Configuration Confirm 
 *
 * @details
 *    This function handles the UDX configuration Confirm from DL Instance
 *
 * @param[in] pst     Post structure  
 * @param[in] suId    Service User ID
 * @param[in] cfmInfo Confirm Information 
 * 
 * @return  S16
 *    -# ROK 
 *    -# RFAILED
 */
#ifdef ANSI
S16 rlcUlUdxCfgCfm
(
Pst             *pst,  
SuId            suId, 
RlcCfgCfmInfo   *cfmInfo  
)
#else
S16 rlcUlUdxCfgCfm (pst, suId, cfmInfo)
Pst             *pst;   
SuId            suId;  
RlcCfgCfmInfo   *cfmInfo;  
#endif
{
   RlcCb             *tRlcCb;
   RlcCfgCfmInfo    *cfgCfm;
   RlcUlCfgTmpData   *cfgTmpData;
   Pst rspPst;
   memset(&rspPst, 0, sizeof(Pst));

   TRC3(rlcUlUdxCfgCfm)

#if (ERRCLASS & ERRCLS_INT_PAR)
   if (pst->dstInst >= MAX_RLC_INSTANCES)
   {
      RLC_FREE_SHRABL_BUF(pst->region,
                         pst->pool,
	                 cfmInfo,
                         sizeof(RlcCfgCfmInfo));
      return  (RFAILED);
   }
#endif
   tRlcCb = RLC_GET_RLCCB(pst->dstInst);
   RLCDBGP_BRIEF(tRlcCb, " suId(%d)\n", suId);

#if (ERRCLASS & ERRCLS_INT_PAR)
   if (suId < 0)
   {
      RLOG0(L_ERROR, "Invalid suId");
      RLC_SEND_SAPID_ALARM(tRlcCb,suId, 
                           LKW_EVENT_LI_BND_CFM, LCM_CAUSE_INV_SUID);
      RLC_FREE_SHRABL_BUF(pst->region,
                         pst->pool,
	                 cfmInfo,
                         sizeof(RlcCfgCfmInfo));
      return RFAILED;
   }
#endif /* ERRCLASS & ERRCLS_INT_PAR */

   if(ROK != rlcDbmFindUlTransaction(tRlcCb,cfmInfo->transId, &cfgTmpData))
   {
      RLOG0(L_ERROR, "Invalid transId");
      RLC_FREE_SHRABL_BUF(pst->region,
                         pst->pool,
	                 cfmInfo,
                         sizeof(RlcCfgCfmInfo));
      return  (RFAILED);
   }

   if(ROK != rlcDbmDelUlTransaction(tRlcCb, cfgTmpData))
   {
      RLC_FREE_SHRABL_BUF(pst->region,
                         pst->pool,
	                 cfmInfo,
                         sizeof(RlcCfgCfmInfo));
       return RFAILED;
   }
      /* Allocate memory and memset to 0 for cfmInfo */
   RLC_ALLOC(tRlcCb,cfgCfm, sizeof(RlcCfgCfmInfo));
#if (ERRCLASS & ERRCLS_ADD_RES)
   if (cfgCfm == NULLP)
   {
      RLOG0(L_FATAL, "Memory Allocation failed.");
      RLC_FREE_SHRABL_BUF(pst->region,
                         pst->pool,
	                 cfmInfo,
                         sizeof(RlcCfgCfmInfo));
       return RFAILED;
   }
#endif /* ERRCLASS & ERRCLS_ADD_RES */
   rlcHdlCrlcUlCfgReq(tRlcCb,cfgTmpData, cfmInfo, cfgCfm);
   FILL_PST_RLC_TO_DUAPP(rspPst, tRlcCb->genCfg.lmPst.dstProcId, RLC_UL_INST, EVENT_RLC_UL_UE_CREATE_RSP);
   SendRlcUlUeCreateRspToDu(&rspPst, cfgCfm);

   /* free the memory from DL */
   RLC_FREE_SHRABL_BUF(pst->region,
                      pst->pool,
		      cfmInfo,
		      sizeof(RlcCfgCfmInfo));

   /* free the cfgInfo that came from LM */
   RLC_PST_FREE(pst->region, pst->pool, cfgTmpData->cfgInfo, sizeof(RlcCfgInfo));
   RLC_FREE(tRlcCb,cfgTmpData,sizeof(RlcUlCfgTmpData));
   
   return ROK;
} 

/**
 * @brief 
 *    Handler for UeId change confirm 
 *
 * @details
 *    This function handles the UeId Change Confirm from DL Instance 
 *
 *  @param[in] pst     Post structure  
 *  @param[in] suId    Service User ID
 *  @param[in] transId Transaction Id 
 *  @param[in] status  Status of confirmation 
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 */

#ifdef ANSI
S16 rlcUlUdxUeIdChgCfm
(
Pst        *pst,          
SuId       suId,           
U32        transId,
CmStatus   status
)
#else
S16 rlcUlUdxUeIdChgCfm (pst, suId, cfmInfo)
Pst        *pst;         
SuId       suId;        
U32        transId;
CmStatus   status;
#endif
{
   RlcCb             *tRlcCb;
   RlcUlCfgTmpData   *cfgTmpData;

   TRC3(rlcUlUdxUeIdChgCfm)

#if (ERRCLASS & ERRCLS_INT_PAR)
   if (pst->dstInst >= MAX_RLC_INSTANCES)
   {
      return  (RFAILED);
   }
#endif
   tRlcCb = RLC_GET_RLCCB(pst->dstInst);

   RLCDBGP_BRIEF(tRlcCb, " suId(%d) \n", suId);

#if (ERRCLASS & ERRCLS_INT_PAR)
   if (suId < 0)
   {
      RLOG0(L_ERROR, "Invalid suId");
      return RFAILED;
   }
#endif /* ERRCLASS & ERRCLS_INT_PAR */

   if(ROK != rlcDbmFindUlTransaction(tRlcCb, transId, &cfgTmpData))
   {
      RLOG0(L_ERROR, "Invalid transId");
      return  (RFAILED);
   }

   if(ROK != rlcDbmDelUlTransaction(tRlcCb, cfgTmpData))
   {
      return RFAILED;
   }

   if(status.status == CKW_CFG_CFM_OK)
   {
       if(cfgTmpData->ueCb != NULLP) 
       {
      rlcCfgApplyUlUeIdChng(tRlcCb, cfgTmpData->ueInfo, cfgTmpData->newUeInfo, cfgTmpData);
   }
   }
   RlcUiCkwUeIdChgCfm(&(tRlcCb->u.ulCb->ckwSap.pst),
                     tRlcCb->u.ulCb->ckwSap.suId,
                     transId,cfgTmpData->ueInfo,status);
   /* only newUeInfo needs to be freed here, ueInfo would be freed at the 
      interface or by he receipient in case of tight coupling */
   RLC_PST_FREE(pst->region, pst->pool, cfgTmpData->newUeInfo, sizeof(CkwUeInfo));
   RLC_FREE_WC(tRlcCb, cfgTmpData, sizeof (RlcUlCfgTmpData));
   return ROK;
} 

/**
 * @brief 
 *    Udx Status Prohibit Timer Start
 *
 *  @param[in] pst     Post structure  
 *  @param[in] suId    Service User ID
 *  @param[in] rlcId   rlc Id 
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 */
S16  rlcUlUdxStaProhTmrStart
(
Pst*         pst,
SuId         suId,
CmLteRlcId   *rlcId
)
{
   RlcCb       *tRlcCb= NULLP;
   RlcUlRbCb   *rbCb;        

#if (ERRCLASS & ERRCLS_INT_PAR)
   if (pst->dstInst >= MAX_RLC_INSTANCES)
   {
      return  (RFAILED);
   }
#endif
   tRlcCb = RLC_GET_RLCCB(pst->dstInst);

   rlcDbmFetchUlRbCbByRbId(tRlcCb, rlcId, &rbCb);
   if (rbCb == NULLP)
   {    
      RLOG_ARG2(L_ERROR, DBG_UEID,rlcId->ueId, "CellId [%u]:RbId[%d] not found",
            rlcId->cellId,rlcId->rbId);
      return RFAILED;
   }

   /* Start staProhTmr */
   rlcStartTmr(tRlcCb,(PTR)rbCb, RLC_EVT_AMUL_STA_PROH_TMR);

   return  (ROK);
} 

/**
 * @brief 
 *    Handler for configuration confirm from DL.
 *
 * @param[in] gCb        - RLC Instance Control Block
 * @param[in] cfgTmpData - Configuration Temporary Data 
 * @param[in] cfmInfo    - DL Configuration Confirm Info
 * @param[out] cfgCfm    - Configuration Confirm to be sent to RRC
 *
 * @return  S16
 *    -# ROK 
 *    -# RFAILED
 */
#ifdef ANSI
S16 rlcHdlCrlcUlCfgReq
(
RlcCb             *gCb,
RlcUlCfgTmpData   *cfgTmpData,
RlcCfgCfmInfo    *cfmInfo,
RlcCfgCfmInfo    *cfgCfm
)
#else
S16 rlcHdlCrlcUlCfgReq(gCb,cfgTmpData,cfmInfo,cfmInfo)
RlcCb             *gCb;
RlcUlCfgTmpData   *cfgTmpData;
RlcCfgCfmInfo    *cfmInfo;
RlcCfgCfmInfo    *cfgCfm;
#endif
{
   RlcCfgInfo   *cfg;
   U32          idx;
   U32          maxEnt;
   
   cfg = cfgTmpData->cfgInfo;
   maxEnt = (cfg->numEnt < CKW_MAX_ENT_CFG)? cfg->numEnt:CKW_MAX_ENT_CFG;

   for (idx = 0; idx < maxEnt; idx++)
   {
      RlcEntCfgCfmInfo   *entCfgCfm;
      RlcEntCfgCfmInfo   *entDlCfgCfm;
      RlcEntCfgInfo      *entCfg;

      entCfg  = &(cfg->entCfg[idx]);
      entCfgCfm   = &(cfgCfm->entCfgCfm[idx]);
      entDlCfgCfm = &(cfmInfo->entCfgCfm[idx]);
      switch (entCfg->cfgType)
      {
         case CKW_CFG_ADD:
         case CKW_CFG_MODIFY:
         case CKW_CFG_DELETE:
            {
               if (entCfg->dir == RLC_DIR_UL)
               {
                  RLC_MEM_CPY(entCfgCfm, 
                             &cfgTmpData->cfgEntData[idx].entUlCfgCfm, 
                             sizeof(RlcEntCfgCfmInfo)); 
               }
               else if (entCfg->dir == RLC_DIR_DL)
               {
                  RLC_MEM_CPY(entCfgCfm, entDlCfgCfm, sizeof(RlcEntCfgCfmInfo)); 
               }
               else if(entCfg->dir == RLC_DIR_BOTH)
               {
                  if (entDlCfgCfm->status.status != CKW_CFG_CFM_OK)
                  {
                     rlcCfgRollBackUlRb(gCb,
                                       cfg->ueId,
                                       &cfg->entCfg[idx], 
                                       &cfgTmpData->cfgEntData[idx]);
                  }
                  else
                  {
                     rlcCfgApplyUlRb(gCb,  
                                    &cfg->entCfg[idx], 
                                    &cfgTmpData->cfgEntData[idx],
                                    cfgTmpData);
                  }
                  RLC_MEM_CPY(entCfgCfm, entDlCfgCfm, sizeof(RlcEntCfgCfmInfo)); 
               }
               else
               {
		            RLC_CFG_FILL_CFG_CFM(entCfgCfm, entCfg->rbId, entCfg->rbType,
				           CKW_CFG_CFM_NOK, CKW_CFG_REAS_INVALID_DIR);
                  RLOG_ARG2(L_ERROR,DBG_UEID,cfg->ueId,"RBID[%d] direction[%d] is invalid",
                         entCfg->rbId,entCfg->dir);
               }
               break;
            }

         case CKW_CFG_REESTABLISH:
            {
               if (entCfg->dir == RLC_DIR_UL)
               {
                  RLC_MEM_CPY(entCfgCfm, 
                             &cfgTmpData->cfgEntData[idx].entUlCfgCfm, 
                             sizeof(RlcEntCfgCfmInfo)); 
               }
               else if (entCfg->dir == RLC_DIR_DL)
               {
                  RLC_MEM_CPY(entCfgCfm, entDlCfgCfm, sizeof(RlcEntCfgCfmInfo)); 
               }
               else
               {
                  if (entCfg->dir & RLC_DIR_UL)
                  {
                     /* Reestablish indication is sent from UL Instance only*/
                     if (entDlCfgCfm->status.status == CKW_CFG_CFM_OK)
                     {
                        rlcCfgApplyReEstUlRb (gCb, cfg->ueId, 
                                             cfg->cellId, TRUE, 
                                             &cfgTmpData->cfgEntData[idx]);
                     }
                     RLC_MEM_CPY(entCfgCfm, entDlCfgCfm, sizeof(RlcEntCfgCfmInfo)); 
                  }
               }
               break;
            }
         case CKW_CFG_DELETE_UE:
            {
               if (cfgTmpData->cfgEntData[idx].entUlCfgCfm.status.status == CKW_CFG_CFM_OK)
               {
                  rlcCfgApplyDelUlUe(gCb, cfgTmpData);
                  RLC_MEM_CPY(entCfgCfm, 
                             &cfgTmpData->cfgEntData[idx].entUlCfgCfm, 
                             sizeof(RlcEntCfgCfmInfo)); 
               }
               else
               {
                   RLC_MEM_CPY(entCfgCfm, entDlCfgCfm,
                              sizeof(RlcEntCfgCfmInfo)); 
               }
               break;
            }
         case CKW_CFG_DELETE_CELL:
            {
               if (cfgTmpData->cfgEntData[idx].entUlCfgCfm.status.status == CKW_CFG_CFM_OK)
               {
                  rlcCfgApplyDelUlCell(gCb, cfgTmpData);
                  RLC_MEM_CPY(entCfgCfm, 
                             &cfgTmpData->cfgEntData[idx].entUlCfgCfm, 
                             sizeof(RlcEntCfgCfmInfo)); 
               }
               else
               {
                   RLC_MEM_CPY(entCfgCfm, entDlCfgCfm,
                              sizeof(RlcEntCfgCfmInfo)); 
               }
               break;
            }
         default:
            {
               RLC_CFG_FILL_CFG_CFM(entCfgCfm, entCfg->rbId, entCfg->rbType,
                                   CKW_CFG_CFM_NOK, CKW_CFG_REAS_INVALID_CFG);

               RLOG0(L_ERROR,"Invalid configuration type");
            }
      }
   }

   /* Assign number of entity configurations and suId */
   cfgCfm->transId = cfgTmpData->uprLyrTransId;
   cfgCfm->ueId = cfg->ueId;
   cfgCfm->cellId = cfg->cellId;
   cfgCfm->numEnt = cfg->numEnt;

   return ROK;
}
#ifdef __cplusplus
}
#endif /* __cplusplus */


/**********************************************************************
         End of file
**********************************************************************/
