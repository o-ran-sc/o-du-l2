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

#include "rlc.h"
#include "du_app_rlc_inf.h"

#define KW_MODULE KW_DBGMASK_UDX
/* local defines */

/* local externs */

/* forward references */

/* public variable declarations */
EXTERN S16 kwHdlCkwUlCfgReq ARGS((KwCb  *gCb,RlcUlCfgTmpData *cfgTmpData,
                                  RlcCfgCfmInfo *cfmInfo, RlcCfgCfmInfo *cfgCfm));

extern uint8_t SendRlcUlUeCreateRspToDu(Pst *pst, RlcCfgCfmInfo *cfgRsp);
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
PUBLIC S16 RlcUlUdxBndCfm
(
Pst    *pst,  
SuId   suId, 
U8     status 
)
#else
PUBLIC S16 RlcUlUdxBndCfm (pst, suId, status)
Pst    *pst; 
SuId   suId;   
U8     status; 
#endif
{
   U16            event;        
   U16            cause;       
   KwUdxUlSapCb   *udxSap;   /* RGU SAP Control Block */
   KwCb           *tKwCb;

   TRC3(RlcUlUdxBndCfm)

#if (ERRCLASS & ERRCLS_INT_PAR)
   if (pst->dstInst >= KW_MAX_RLC_INSTANCES)
   {
      RETVALUE (RFAILED);
   }
#endif
   tKwCb = RLC_GET_RLCCB(pst->dstInst);

   KWDBGP_BRIEF(tKwCb, "RlcUlUdxBndCfm(post, suId(%d), status(%d)\n", 
                suId, status);

#if (ERRCLASS & ERRCLS_INT_PAR)
   if (tKwCb->init.cfgDone != TRUE)
   {
      RLOG0(L_FATAL, "General configuration not done");
      KW_SEND_SAPID_ALARM(tKwCb,suId, 
                          LKW_EVENT_LI_BND_CFM, LCM_CAUSE_INV_STATE);

      RETVALUE(RFAILED);
   }

   if (suId < 0)
   {
      RLOG0(L_ERROR, "Invalid suId");
      KW_SEND_SAPID_ALARM(tKwCb,suId, 
                           LKW_EVENT_LI_BND_CFM, LCM_CAUSE_INV_SUID);

      RETVALUE(RFAILED);
   }
#endif /* ERRCLASS & ERRCLS_INT_PAR */

   udxSap = tKwCb->u.ulCb->udxUlSap + suId;

   KWDBGP_DETAIL(tKwCb, "KwLiRguBndCfm: For RGU SAP state=%d\n", 
                 udxSap->state);

   /* Check rguSap state */
   switch (udxSap->state)
   {
      case KW_SAP_BINDING:
      {
         if(TRUE == kwChkTmr(tKwCb,(PTR)udxSap,KW_EVT_WAIT_BNDCFM))
         {
             kwStopTmr (tKwCb,(PTR)udxSap, KW_EVT_WAIT_BNDCFM);
         }
         udxSap->retryCnt = 0;
          
         if (status == CM_BND_OK)
         {
            udxSap->state = KW_SAP_BND;
            event = LCM_EVENT_BND_OK;
            cause = LKW_CAUSE_SAP_BNDENB;
         }
         else
         {
            udxSap->state = KW_SAP_CFG;
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
   KW_SEND_SAPID_ALARM(tKwCb,suId, event, cause);

   RETVALUE(ROK);
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
PUBLIC S16 RlcUlUdxCfgCfm
(
Pst             *pst,  
SuId            suId, 
RlcCfgCfmInfo   *cfmInfo  
)
#else
PUBLIC S16 RlcUlUdxCfgCfm (pst, suId, cfmInfo)
Pst             *pst;   
SuId            suId;  
RlcCfgCfmInfo   *cfmInfo;  
#endif
{
   KwCb             *tKwCb;
   RlcCfgCfmInfo    *cfgCfm;
   RlcUlCfgTmpData   *cfgTmpData;
   Pst rspPst;
   memset(&rspPst, 0, sizeof(Pst));

   TRC3(RlcUlUdxCfgCfm)

#if (ERRCLASS & ERRCLS_INT_PAR)
   if (pst->dstInst >= KW_MAX_RLC_INSTANCES)
   {
      RLC_FREE_SHRABL_BUF(pst->region,
                         pst->pool,
	                 cfmInfo,
                         sizeof(RlcCfgCfmInfo));
      RETVALUE (RFAILED);
   }
#endif
   tKwCb = RLC_GET_RLCCB(pst->dstInst);
   KWDBGP_BRIEF(tKwCb, " suId(%d)\n", suId);

#if (ERRCLASS & ERRCLS_INT_PAR)
   if (suId < 0)
   {
      RLOG0(L_ERROR, "Invalid suId");
      KW_SEND_SAPID_ALARM(tKwCb,suId, 
                           LKW_EVENT_LI_BND_CFM, LCM_CAUSE_INV_SUID);
      RLC_FREE_SHRABL_BUF(pst->region,
                         pst->pool,
	                 cfmInfo,
                         sizeof(RlcCfgCfmInfo));
      RETVALUE(RFAILED);
   }
#endif /* ERRCLASS & ERRCLS_INT_PAR */

   if(ROK != kwDbmFindUlTransaction(tKwCb,cfmInfo->transId, &cfgTmpData))
   {
      RLOG0(L_ERROR, "Invalid transId");
      RLC_FREE_SHRABL_BUF(pst->region,
                         pst->pool,
	                 cfmInfo,
                         sizeof(RlcCfgCfmInfo));
      RETVALUE (RFAILED);
   }

   if(ROK != kwDbmDelUlTransaction(tKwCb, cfgTmpData))
   {
      RLC_FREE_SHRABL_BUF(pst->region,
                         pst->pool,
	                 cfmInfo,
                         sizeof(RlcCfgCfmInfo));
       RETVALUE(RFAILED);
   }
      /* Allocate memory and memset to 0 for cfmInfo */
   RLC_ALLOC(tKwCb,cfgCfm, sizeof(RlcCfgCfmInfo));
#if (ERRCLASS & ERRCLS_ADD_RES)
   if (cfgCfm == NULLP)
   {
      RLOG0(L_FATAL, "Memory Allocation failed.");
      RLC_FREE_SHRABL_BUF(pst->region,
                         pst->pool,
	                 cfmInfo,
                         sizeof(RlcCfgCfmInfo));
       RETVALUE(RFAILED);
   }
#endif /* ERRCLASS & ERRCLS_ADD_RES */
   kwHdlCkwUlCfgReq(tKwCb,cfgTmpData, cfmInfo, cfgCfm);
   FILL_PST_RLC_TO_DUAPP(rspPst, tKwCb->genCfg.lmPst.dstProcId, RLC_UL_INST, EVENT_RLC_UL_UE_CREATE_RSP);
   SendRlcUlUeCreateRspToDu(&rspPst, cfgCfm);

   /* free the memory from DL */
   RLC_FREE_SHRABL_BUF(pst->region,
                      pst->pool,
		      cfmInfo,
		      sizeof(RlcCfgCfmInfo));

   /* free the cfgInfo that came from LM */
   KW_PST_FREE(pst->region, pst->pool, cfgTmpData->cfgInfo, sizeof(RlcCfgInfo));
   RLC_FREE(tKwCb,cfgTmpData,sizeof(RlcUlCfgTmpData));
   
   RETVALUE(ROK);
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
PUBLIC S16 RlcUlUdxUeIdChgCfm
(
Pst        *pst,          
SuId       suId,           
U32        transId,
CmStatus   status
)
#else
PUBLIC S16 RlcUlUdxUeIdChgCfm (pst, suId, cfmInfo)
Pst        *pst;         
SuId       suId;        
U32        transId;
CmStatus   status;
#endif
{
   KwCb             *tKwCb;
   RlcUlCfgTmpData   *cfgTmpData;

   TRC3(RlcUlUdxUeIdChgCfm)

#if (ERRCLASS & ERRCLS_INT_PAR)
   if (pst->dstInst >= KW_MAX_RLC_INSTANCES)
   {
      RETVALUE (RFAILED);
   }
#endif
   tKwCb = RLC_GET_RLCCB(pst->dstInst);

   KWDBGP_BRIEF(tKwCb, " suId(%d) \n", suId);

#if (ERRCLASS & ERRCLS_INT_PAR)
   if (suId < 0)
   {
      RLOG0(L_ERROR, "Invalid suId");
      RETVALUE(RFAILED);
   }
#endif /* ERRCLASS & ERRCLS_INT_PAR */

   if(ROK != kwDbmFindUlTransaction(tKwCb, transId, &cfgTmpData))
   {
      RLOG0(L_ERROR, "Invalid transId");
      RETVALUE (RFAILED);
   }

   if(ROK != kwDbmDelUlTransaction(tKwCb, cfgTmpData))
   {
      RETVALUE(RFAILED);
   }

   if(status.status == CKW_CFG_CFM_OK)
   {
       if(cfgTmpData->ueCb != NULLP) 
       {
      kwCfgApplyUlUeIdChng(tKwCb, cfgTmpData->ueInfo, cfgTmpData->newUeInfo, cfgTmpData);
   }
   }
   KwUiCkwUeIdChgCfm(&(tKwCb->u.ulCb->ckwSap.pst),
                     tKwCb->u.ulCb->ckwSap.suId,
                     transId,cfgTmpData->ueInfo,status);
   /* only newUeInfo needs to be freed here, ueInfo would be freed at the 
      interface or by he receipient in case of tight coupling */
   KW_PST_FREE(pst->region, pst->pool, cfgTmpData->newUeInfo, sizeof(CkwUeInfo));
   RLC_FREE_WC(tKwCb, cfgTmpData, sizeof (RlcUlCfgTmpData));
   RETVALUE(ROK);
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
PUBLIC S16  RlcUlUdxStaProhTmrStart
(
Pst*         pst,
SuId         suId,
CmLteRlcId   *rlcId
)
{
   KwCb       *tKwCb= NULLP;
   RlcUlRbCb   *rbCb;        

#if (ERRCLASS & ERRCLS_INT_PAR)
   if (pst->dstInst >= KW_MAX_RLC_INSTANCES)
   {
      RETVALUE (RFAILED);
   }
#endif
   tKwCb = RLC_GET_RLCCB(pst->dstInst);

   kwDbmFetchUlRbCbByRbId(tKwCb, rlcId, &rbCb);
   if (rbCb == NULLP)
   {    
      RLOG_ARG2(L_ERROR, DBG_UEID,rlcId->ueId, "CellId [%u]:RbId[%d] not found",
            rlcId->cellId,rlcId->rbId);
      RETVALUE(RFAILED);
   }

   /* Start staProhTmr */
   kwStartTmr(tKwCb,(PTR)rbCb, KW_EVT_AMUL_STA_PROH_TMR);

   RETVALUE (ROK);
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
PUBLIC S16 kwHdlCkwUlCfgReq
(
KwCb             *gCb,
RlcUlCfgTmpData   *cfgTmpData,
RlcCfgCfmInfo    *cfmInfo,
RlcCfgCfmInfo    *cfgCfm
)
#else
PUBLIC S16 kwHdlCkwUlCfgReq(gCb,cfgTmpData,cfmInfo,cfmInfo)
KwCb             *gCb;
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
      CkwEntCfgCfmInfo   *entCfgCfm;
      CkwEntCfgCfmInfo   *entDlCfgCfm;
      CkwEntCfgInfo      *entCfg;

      entCfg  = &(cfg->entCfg[idx]);
      entCfgCfm   = &(cfgCfm->entCfgCfm[idx]);
      entDlCfgCfm = &(cfmInfo->entCfgCfm[idx]);
      switch (entCfg->cfgType)
      {
         case CKW_CFG_ADD:
         case CKW_CFG_MODIFY:
         case CKW_CFG_DELETE:
            {
               if (entCfg->dir == KW_DIR_UL)
               {
                  KW_MEM_CPY(entCfgCfm, 
                             &cfgTmpData->cfgEntData[idx].entUlCfgCfm, 
                             sizeof(CkwEntCfgCfmInfo)); 
               }
               else if (entCfg->dir == KW_DIR_DL)
               {
                  KW_MEM_CPY(entCfgCfm, entDlCfgCfm, sizeof(CkwEntCfgCfmInfo)); 
               }
               else if(entCfg->dir == KW_DIR_BOTH)
               {
                  if (entDlCfgCfm->status.status != CKW_CFG_CFM_OK)
                  {
                     kwCfgRollBackUlRb(gCb,
                                       cfg->ueId,
                                       &cfg->entCfg[idx], 
                                       &cfgTmpData->cfgEntData[idx]);
                  }
                  else
                  {
                     kwCfgApplyUlRb(gCb,  
                                    &cfg->entCfg[idx], 
                                    &cfgTmpData->cfgEntData[idx],
                                    cfgTmpData);
                  }
                  KW_MEM_CPY(entCfgCfm, entDlCfgCfm, sizeof(CkwEntCfgCfmInfo)); 
               }
               else
               {
		            KW_CFG_FILL_CFG_CFM(entCfgCfm, entCfg->rbId, entCfg->rbType,
				           CKW_CFG_CFM_NOK, CKW_CFG_REAS_INVALID_DIR);
                  RLOG_ARG2(L_ERROR,DBG_UEID,cfg->ueId,"RBID[%d] direction[%d] is invalid",
                         entCfg->rbId,entCfg->dir);
               }
               break;
            }

         case CKW_CFG_REESTABLISH:
            {
               if (entCfg->dir == KW_DIR_UL)
               {
                  KW_MEM_CPY(entCfgCfm, 
                             &cfgTmpData->cfgEntData[idx].entUlCfgCfm, 
                             sizeof(CkwEntCfgCfmInfo)); 
               }
               else if (entCfg->dir == KW_DIR_DL)
               {
                  KW_MEM_CPY(entCfgCfm, entDlCfgCfm, sizeof(CkwEntCfgCfmInfo)); 
               }
               else
               {
                  if (entCfg->dir & KW_DIR_UL)
                  {
                     /* Reestablish indication is sent from UL Instance only*/
                     if (entDlCfgCfm->status.status == CKW_CFG_CFM_OK)
                     {
                        kwCfgApplyReEstUlRb (gCb, cfg->ueId, 
                                             cfg->cellId, TRUE, 
                                             &cfgTmpData->cfgEntData[idx]);
                     }
                     KW_MEM_CPY(entCfgCfm, entDlCfgCfm, sizeof(CkwEntCfgCfmInfo)); 
                  }
               }
               break;
            }
         case CKW_CFG_DELETE_UE:
            {
               if (cfgTmpData->cfgEntData[idx].entUlCfgCfm.status.status == CKW_CFG_CFM_OK)
               {
                  kwCfgApplyDelUlUe(gCb, cfgTmpData);
                  KW_MEM_CPY(entCfgCfm, 
                             &cfgTmpData->cfgEntData[idx].entUlCfgCfm, 
                             sizeof(CkwEntCfgCfmInfo)); 
               }
               else
               {
                   KW_MEM_CPY(entCfgCfm, entDlCfgCfm,
                              sizeof(CkwEntCfgCfmInfo)); 
               }
               break;
            }
         case CKW_CFG_DELETE_CELL:
            {
               if (cfgTmpData->cfgEntData[idx].entUlCfgCfm.status.status == CKW_CFG_CFM_OK)
               {
                  kwCfgApplyDelUlCell(gCb, cfgTmpData);
                  KW_MEM_CPY(entCfgCfm, 
                             &cfgTmpData->cfgEntData[idx].entUlCfgCfm, 
                             sizeof(CkwEntCfgCfmInfo)); 
               }
               else
               {
                   KW_MEM_CPY(entCfgCfm, entDlCfgCfm,
                              sizeof(CkwEntCfgCfmInfo)); 
               }
               break;
            }
         default:
            {
               KW_CFG_FILL_CFG_CFM(entCfgCfm, entCfg->rbId, entCfg->rbType,
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

   RETVALUE(ROK);
}
#ifdef __cplusplus
}
#endif /* __cplusplus */


/**********************************************************************
         End of file
**********************************************************************/
