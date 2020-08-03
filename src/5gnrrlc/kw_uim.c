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

/********************************************************************20**
  
     Name:    LTE-RLC Layer - Upper Interface Functions
    
     Type:    C file
  
     Desc:     Source code for RLC Upper Interface Module
               This file contains following functions
      
        --KwUiCkwBndReq
        --KwUiCkwUbndReq
        --RlcProcCfgReq

        --KwUiKwuBndReq
        --KwUiKwuUbndReq
        --KwUiKwuDatReq
        --KwUiKwuDiscSduReq 

     File:     kw_uim.c

**********************************************************************/
static const char* RLOG_MODULE_NAME="UIM";
static int RLOG_MODULE_ID=2048;
static int RLOG_FILE_ID=205;

/** 
 * @file kw_uim.c
 * @brief RLC Upper Interface Module
*/

#define KW_MODULE KW_DBGMASK_INF


/* header (.h) include files */
#include "common_def.h"
#include "lkw.h"           /* LKW defines */
#include "ckw.h"           /* CKW defines */
#include "kwu.h"           /* KWU defines */
#include "rgu.h"           /* RGU defines */
#include "kw_env.h"        /* RLC environment options */
#include "kw.h"            /* RLC defines */
#include "kw_dl.h"
#include "kw_ul.h"
#include "kw_udx.h"

/* extern (.x) include files */
#include "lkw.x"           /* LKW */
#include "ckw.x"           /* CKW */
#include "kwu.x"           /* KWU */
#include "rgu.x"           /* RGU */
#include "kw_err.h"
#include "kw.x"
#include "kw_udx.x"
#include "kw_dl.x"
#include "kw_ul.x"


/*****************************************************************************
 *                          CKW INTERFACE
 ****************************************************************************/

/**
 * @brief 
 *    Handler for binding the RLC upper layer service user with 
 *    the RLC layer.
 *
 * @details
 *    This function is used by RLC user to request for binding to 
 *    RLC. This function is called by the CKW interface to bind 
 *    RLC's SAP (identified by spId) with the service user's
 *    SAP (identified by suId). 
 *
 * @param[in] pst     Post structure
 * @param[in] suId    Service User ID
 * @param[in] spId    Service provider ID
 *
 * @return  S16
 *    -# ROK 
 *    -# RFAILED
 *
*/
#ifdef ANSI
PUBLIC S16 KwUiCkwBndReq 
(
Pst    *pst,
SuId   suId,
SpId   spId 
)
#else
PUBLIC S16 KwUiCkwBndReq (pst, suId, spId)
Pst    *pst; 
SuId   suId;
SpId   spId;
#endif
{
   KwCkwSapCb   *ckwSap;  
   RlcCb         *tRlcCb;

   TRC3(KwUiCkwBndReq);

#if (ERRCLASS & ERRCLS_INT_PAR)
   if (pst->dstInst >= MAX_RLC_INSTANCES)
   {
      RETVALUE(RFAILED);
   }
#endif

   tRlcCb = RLC_GET_RLCCB(pst->dstInst);

   RLOG2(L_DEBUG, "spId(%d), suId(%d)", spId, suId);
   ckwSap = &(tRlcCb->u.ulCb->ckwSap);
   /* Take action based on the current state of the SAP */
   switch(ckwSap->state)
   {
      /* SAP is configured but not bound */
      case KW_SAP_CFG:
      case KW_SAP_UBND:
      {
         /* copy bind configuration parameters in SSAP sap */
         ckwSap->suId = suId;
         ckwSap->pst.dstProcId = pst->srcProcId;
         ckwSap->pst.dstEnt = pst->srcEnt;
         ckwSap->pst.dstInst = pst->srcInst;

         /* Update the State */
         ckwSap->state = KW_SAP_BND;

         RLOG1(L_DEBUG, "KwUiCkwBndReq: state (%d)", ckwSap->state);
         break;
      }
      case KW_SAP_BND:
      {
         /* Sap is already bound check source, destination Entity and  
          * Proc Id  */
         if (ckwSap->pst.dstProcId != pst->srcProcId ||
             ckwSap->pst.dstEnt != pst->srcEnt ||
             ckwSap->pst.dstInst != pst->srcInst ||
             ckwSap->suId != suId)
         {
            KW_SEND_SAPID_ALARM(tRlcCb,
                                spId, 
                                LKW_EVENT_CKW_BND_REQ, 
                                LCM_CAUSE_INV_PAR_VAL);

            RLOG0(L_ERROR, "CKW SAP already Bound");
            KwUiCkwBndCfm(&(ckwSap->pst), ckwSap->suId, CM_BND_NOK);
            RETVALUE(RFAILED);
         }
         break;
      }
      default:
      {
#if (ERRCLASS & ERRCLS_INT_PAR)
         RLOG0(L_ERROR, "Invalid CKW SAP State in Bind Req");
         KW_SEND_SAPID_ALARM(tRlcCb,
                             spId, 
                             LKW_EVENT_CKW_BND_REQ, 
                             LCM_CAUSE_INV_STATE);
#endif /* ERRCLASS & ERRCLS_INT_PAR */
         KwUiCkwBndCfm(&(ckwSap->pst), ckwSap->suId, CM_BND_NOK);
         RETVALUE(RFAILED);
         break;
      }
   }

   KwUiCkwBndCfm(&(ckwSap->pst), ckwSap->suId, CM_BND_OK);
   RETVALUE(ROK);
} 


/**
 * @brief 
 *    Handler for unbinding the RLC upper layer service user CKW with 
 *    the RLC layer.
 *
 * @details
 *    This function is used by RLC user to request for unbinding  
 *    with RLC.This function is called by the CKW interface to 
 *    unbind with RLC.  
 *
 * @param[in] pst     Post structure
 * @param[in] spId    Service provider SAP ID
 * @param[in] reason  Reason for Unbinding 
 *
 * @return  S16
 *    -# ROK 
 *    -# RFAILED
*/ 
#ifdef ANSI
PUBLIC S16 KwUiCkwUbndReq
(
Pst      *pst,   
SpId     spId,  
Reason   reason 
)
#else
PUBLIC S16 KwUiCkwUbndReq(pst, spId, reason)
Pst      *pst; 
SpId     spId; 
Reason   reason;
#endif
{
   RlcCb *tRlcCb;

   TRC3(KwUiCkwUbndReq)

#if (ERRCLASS & ERRCLS_INT_PAR)
   if (pst->dstInst >= MAX_RLC_INSTANCES)
   {
      RETVALUE(RFAILED);
   }
#endif /* ERRCLASS & ERRCLS_INT_PAR */
   tRlcCb = RLC_GET_RLCCB(pst->dstInst);

   RLOG2(L_DEBUG,"spId(%d), reason(%d)", 
                spId, 
                reason);

   UNUSED(reason);

#if (ERRCLASS & ERRCLS_INT_PAR)
   KW_GET_AND_VALIDATE_CKWSAP(tRlcCb, 
                              (&(tRlcCb->u.ulCb->ckwSap)), 
                              EKW208, 
                              "KwUiCkwUbndReq");
#endif /* ERRCLASS & ERRCLS_INT_PAR */

   /* disable upper sap (CKW) */
   tRlcCb->u.ulCb->ckwSap.state = KW_SAP_CFG;
   RETVALUE(ROK);
} 

/**
 * @brief  
 *    Handler for configuring RLC entities.
 *
 * @details
 *    This function is used by RRC to configure(add/delete/modify)
 *    one or more RLC entities. 
 *
 * @param[in] pst   -  Post structure  
 * @param[in] spId  -  Serive Provider ID 
 * @param[in] cfg   -  Configuration information for one or more RLC entities. 
 *
 * @return  S16
 *      -# ROK 
 *      -# RFAILED
 */
#ifdef ANSI
PUBLIC S16 RlcProcCfgReq
(
Pst          *pst,
RlcCfgInfo   *cfg
)
#else
PUBLIC S16 RlcProcCfgReq(pst, cfg)
Pst          *pst;
RlcCfgInfo   *cfg;
#endif
{
   RlcCb        *tRlcCb;
   RlcUlCfgTmpData   *cfgTmpData;

   static U32  transCount;


   TRC3(RlcProcCfgReq)
      
#if (ERRCLASS & ERRCLS_INT_PAR)
   if (pst->dstInst >= MAX_RLC_INSTANCES)
   {
      RLC_PST_FREE(pst->region, pst->pool, cfg, sizeof(RlcCfgInfo));
      RETVALUE(RFAILED);
   }
#endif
   tRlcCb = RLC_GET_RLCCB(pst->dstInst);

   RLC_ALLOC(tRlcCb, cfgTmpData, sizeof (RlcUlCfgTmpData));

   if (cfgTmpData == NULLP)
   {
      RLC_PST_FREE(pst->region, pst->pool, cfg, sizeof(RlcCfgInfo));
      RETVALUE(RFAILED);
   }
   
   cfgTmpData->uprLyrTransId = cfg->transId; /*Save User TransId*/
   cfgTmpData->transId = ++transCount;       /*Generate new TransId*/
   cfg->transId = cfgTmpData->transId;
   cfgTmpData->cfgInfo  = cfg;

 
   if (kwDbmAddUlTransaction(tRlcCb, cfgTmpData) != ROK)
   {
      RLOG0(L_ERROR, "Addition to UL transId Lst Failed");
      RLC_PST_FREE(pst->region, pst->pool, cfg, sizeof(RlcCfgInfo));
      
      RETVALUE(RFAILED);
   }
   
   rlcUlHdlCfgReq(tRlcCb, cfgTmpData, cfg);
   rlcUlUdxCfgReq(&(KW_GET_UDX_SAP(tRlcCb)->pst),KW_GET_UDX_SAP(tRlcCb)->spId,cfg); 

   RETVALUE(ROK);
} 

/**
 * @brief 
 *    Handler to change the UeId
 *
 * @details 
 *    This primitive is used by RRC to change the UeId for the existing UE
 *    context.
 *     
 * @param[in] pst     -  Point to the pst structure
 * @param[in] spId    -  The ID of the service provider SAP in the RLC layer 
 * @param[in] transId -  Transaction ID. This field uniquily identifies
 *                       transaction between RRC and RLC
 * @param[in] ueInfo    -  Old UE Id Info for which the change request has come 
 * @param[in] newUeInfo -  New UE Id Info for existing UE context 
 * 
 * @return S16
 *    -# ROK
 *    -# RFAILED
 */
#ifdef ANSI
PUBLIC S16 KwUiCkwUeIdChgReq
(
Pst         *pst, 
SpId        spId, 
U32         transId, 
CkwUeInfo   *ueInfo,
CkwUeInfo   *newUeInfo
)
#else
PUBLIC S16 KwUiCkwUeIdChgReq(pst, spId, transId, ueInfo, newUeInfo)
Pst         *pst;
SpId        spId;
U32         transId;
CkwUeInfo   *ueInfo;
CkwUeInfo   *newUeInfo;
#endif
{
   S16              ret = ROK;
   RlcCb             *tRlcCb;
   RlcUlCfgTmpData   *cfgTmpData = NULLP;

   TRC3(KwUiCkwUeIdChgReq)

   do
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      if (pst->dstInst >= MAX_RLC_INSTANCES)
      {
         ret = RFAILED;
         break;
      }
#endif

      tRlcCb = RLC_GET_RLCCB(pst->dstInst);
#ifndef ALIGN_64BIT
      RLOG_ARG2(L_DEBUG,DBG_CELLID,newUeInfo->cellId,
                   "KwUiCkwUeIdChgReq(pst, spId(%d), transId(%ld))", 
                   spId, 
                   transId);
#else
      RLOG_ARG2(L_DEBUG,DBG_CELLID,newUeInfo->cellId, 
                   "KwUiCkwUeIdChgReq(pst, spId(%d), transId(%d))\n", 
                   spId, 
                   transId);
#endif

      RLC_ALLOC(tRlcCb, cfgTmpData, sizeof (RlcUlCfgTmpData));
      if (!cfgTmpData)
      {
         ret = RFAILED;
         break;
      }

      cfgTmpData->transId = transId;
      cfgTmpData->ueInfo  = ueInfo;
      cfgTmpData->newUeInfo  = newUeInfo; 

      if (kwDbmAddUlTransaction(tRlcCb, cfgTmpData))
      {
         RLOG0(L_ERROR, "Addition to UL transId Lst Failed");
         ret = RFAILED;
         break;
      }
   }while(0);

   if(ret)
   {
      /* there was an error in the processing, free up all the memory
       * that was passed and could have been allocated in this function
       */
      /* Freeing from proper region */
      RLC_PST_FREE(pst->region, pst->pool, newUeInfo, sizeof(CkwUeInfo));
      RLC_PST_FREE(pst->region, pst->pool, ueInfo, sizeof(CkwUeInfo));

      if(cfgTmpData)
      {
         RLC_FREE(tRlcCb, cfgTmpData, sizeof (RlcUlCfgTmpData));
      }
      RETVALUE(RFAILED);
   }

   if(ROK != rlcCfgValidateUeIdChng(tRlcCb,ueInfo,newUeInfo,cfgTmpData))
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cfgTmpData->ueInfo->cellId, 
            "Validation Failure for UeId change");
   }

   rlcUlUdxUeIdChgReq(&(KW_GET_UDX_SAP(tRlcCb)->pst),
                     KW_GET_UDX_SAP(tRlcCb)->spId,
                     transId, 
                     ueInfo,
                     newUeInfo);

   RETVALUE(ROK);
} 


/**
 * @brief 
 *    Handler for Configuration Request 
 *
 * @param[in] gCb      RLC Instance Control Block  
 * @param[in] cfgTmpData  Configuration stored in Transaction Block  
 * @param[in] cfg      Configuration block
 *
 * @return  S16
 *    -# ROK 
 *    -# RFAILED
 */
#ifdef ANSI
PUBLIC Void rlcUlHdlCfgReq
(
RlcCb             *gCb,
RlcUlCfgTmpData   *cfgTmpData,
RlcCfgInfo       *cfg
)
#else
PUBLIC Void rlcUlHdlCfgReq(gCb, cfgTmpData, cfg)
RlcCb             *gCb;
RlcUlCfgTmpData   *cfgTmpData;
RlcCfgInfo       *cfg;
#endif
{
   U8   idx;  

   TRC3(rlcUlHdlCfgReq)

   cfgTmpData->ueId = cfg->ueId;
   cfgTmpData->cellId = cfg->cellId;
   for (idx = 0; idx < cfg->numEnt; idx++)
   {
      cfgTmpData->cfgEntData[idx].entUlCfgCfm.status.status = CKW_CFG_CFM_OK;
      cfgTmpData->cfgEntData[idx].entUlCfgCfm.rbId = cfg->entCfg[idx].rbId; 
      cfgTmpData->cfgEntData[idx].entUlCfgCfm.rbType = cfg->entCfg[idx].rbType;
      switch(cfg->entCfg[idx].cfgType)
      {
         case CKW_CFG_ADD:
         case CKW_CFG_MODIFY:
         case CKW_CFG_DELETE:
         {
         
            if(cfg->entCfg[idx].dir & KW_DIR_UL)
            {
               /* Configuration is for UL , thus validating */
               if(ROK != rlcCfgValidateUlRb(gCb,
                                           &cfg->entCfg[idx], 
                                           &cfgTmpData->cfgEntData[idx],
                                           cfgTmpData))
               {
                  RLOG_ARG2(L_ERROR,DBG_UEID, cfgTmpData->ueId,
                        "CELLID [%u]:Validation Failure for UL RB [%d]",
                         cfg->cellId,cfg->entCfg[idx].rbId);
                  cfgTmpData->cfgEntData[idx].entUlCfgCfm.status.status = CKW_CFG_CFM_NOK;
                  /*Validation is getting failed so no need to do configuration at DL.
                   *Set dir as UL, so that no configuration is done at DL */
                  cfg->entCfg[idx].dir = KW_DIR_UL;
               }
            }
            if(cfg->entCfg[idx].dir == KW_DIR_UL)
            {
               /*If the configuration is for UL only then apply it */   
               if (cfgTmpData->cfgEntData[idx].entUlCfgCfm.status.status == CKW_CFG_CFM_OK)
               {
                  rlcCfgApplyUlRb(gCb, 
                                 &cfg->entCfg[idx], 
                                 &cfgTmpData->cfgEntData[idx],
                                 cfgTmpData);
               }
            }
            break;
         }
         case CKW_CFG_REESTABLISH:   
         {
            if(cfg->entCfg[idx].dir & KW_DIR_UL)
            {
               if(ROK != rlcCfgValidateReEstRb(gCb,
                                              cfg->ueId, 
                                              cfg->cellId, 
                                              &cfg->entCfg[idx], 
                                              &cfgTmpData->cfgEntData[idx]))
               {
                  RLOG_ARG2(L_ERROR,DBG_UEID,cfg->ueId,
                        "CellID [%u]:Validation Failure for Reest UL RB [%d]",
                        cfg->cellId,cfg->entCfg[idx].rbId);
                  cfgTmpData->cfgEntData[idx].entUlCfgCfm.status.status = CKW_CFG_CFM_NOK;
                  /* Setting dir as UL, so that no configuration is done at DL */
                  cfg->entCfg[idx].dir = KW_DIR_UL;
                  break;
               }
            }
            if(cfg->entCfg[idx].dir == KW_DIR_UL)
            {
               /*If the configuration is for UL only then apply it */   
               if (cfgTmpData->cfgEntData[idx].entUlCfgCfm.status.status == CKW_CFG_CFM_OK)
               {
                  rlcCfgApplyReEstUlRb(gCb,  
                                      cfg->ueId, 
                                      cfg->cellId,
                                      TRUE,
                                      &cfgTmpData->cfgEntData[idx]);
               }
            }
            break;
         }
         case CKW_CFG_DELETE_UE :   
         {
            if(ROK != rlcCfgValidateDelUlUe(gCb,  
                                           &cfg->entCfg[idx], 
                                           &cfgTmpData->cfgEntData[idx],
                                           cfgTmpData))
            {
               RLOG_ARG1(L_ERROR,DBG_CELLID,cfg->cellId,
                     "UL UEID [%d]:Validation Failure",
                     cfgTmpData->ueId);
               cfgTmpData->cfgEntData[idx].entUlCfgCfm.status.status = CKW_CFG_CFM_NOK;
               /* Setting dir as UL, so that no configuration is done at DL */
               cfg->entCfg[idx].dir = KW_DIR_UL;
            }
            break;
         }
         case CKW_CFG_DELETE_CELL :   
         {
            if(ROK != rlcCfgValidateDelUlCell(gCb,
                                             cfg->cellId, 
                                             &cfg->entCfg[idx], 
                                             &cfgTmpData->cfgEntData[idx],
                                             cfgTmpData))
            {
               RLOG_ARG0(L_ERROR,DBG_CELLID,cfg->cellId,
                     "Del UL Cell Validation Failure");
               cfgTmpData->cfgEntData[idx].entUlCfgCfm.status.status = CKW_CFG_CFM_NOK;
               /* Setting dir as UL, so that no configuration is done at DL */
               cfg->entCfg[idx].dir = KW_DIR_UL;
            }
            break;
         }
      }/* switch end */
   }/* for end */
   RETVOID;
}


/*****************************************************************************
 *                          KWU INTERFACE
 ****************************************************************************/
/**
 * @brief  
 *    Handler for binding the RLC upper layer service user with 
 *    the RLC layer.
 *
 * @details
 *    This function is used by RLC user to request for binding to 
 *    RLC.This function is called by the KWU interface to bind 
 *    RLC's SAP (identified by spId) with the service user's
 *    SAP (identified by suId). 
 *
 * @param[in] pst   Post structure  
 * @param[in] suId  Service user SAP ID 
 * @param[in] spId  Service provider ID
 *
 * @return  S16
 *    -# ROK
 *    -# RFAILED
 *
 */
#ifdef ANSI
PUBLIC S16 KwUiKwuBndReq 
(
Pst    *pst,  
SuId   suId, 
SpId   spId 
)
#else
PUBLIC S16 KwUiKwuBndReq (pst, suId, spId)
Pst    *pst;  
SuId   suId; 
SpId   spId; 
#endif
{
   KwKwuSapCb   *kwuSap;     /* SAP Config Block */
   RlcCb         *tRlcCb;
   
   TRC3(KwUiKwuBndReq)

#if (ERRCLASS & ERRCLS_INT_PAR)
   if (pst->dstInst >= MAX_RLC_INSTANCES)
   {
      RETVALUE(RFAILED);
   }
#endif
   tRlcCb = RLC_GET_RLCCB(pst->dstInst);
   RLOG2(L_DEBUG, "KwUiKwuBndReq(pst, spId(%d), suId(%d))", spId, suId);

    /* Validation of input parameters */
#if (ERRCLASS & ERRCLS_INT_PAR)
   if(!((spId < (S16) tRlcCb->genCfg.maxKwuSaps) && (spId >=0))) 
   {
      RLOG0(L_ERROR,"Invalid spId");
      KW_SEND_SAPID_ALARM(tRlcCb,spId, LKW_EVENT_KWU_BND_REQ, LCM_CAUSE_INV_SAP);
      RETVALUE(RFAILED); 
   }
#endif

   /* Get Sap control block */
   kwuSap = (tRlcCb->genCfg.rlcMode == LKW_RLC_MODE_DL) ?
            (tRlcCb->u.dlCb->kwuDlSap + spId):
            (tRlcCb->u.ulCb->kwuUlSap + spId);

   /* Take action based on the current state of the SAP */
   switch(kwuSap->state)
   {
      /* SAP is configured but not bound */
      case KW_SAP_CFG:
      case KW_SAP_UBND:
      {
         /* copy bind configuration parameters in sap */
         kwuSap->suId          = suId;
         kwuSap->pst.dstProcId = pst->srcProcId;
         kwuSap->pst.dstEnt    = pst->srcEnt;
         kwuSap->pst.dstInst   = pst->srcInst;

         /* Update the State */
         kwuSap->state = KW_SAP_BND;

         RLOG1(L_DEBUG, "KwUiKwuBndReq: state (%d)", kwuSap->state);
         break;
      }
      case KW_SAP_BND:
      {
         /* Sap is already bound check source, destination Entity and Proc Id */
         if (kwuSap->pst.dstProcId != pst->srcProcId ||
             kwuSap->pst.dstEnt != pst->srcEnt ||
             kwuSap->pst.dstInst != pst->srcInst ||
             kwuSap->suId != suId)
         {
            KW_SEND_SAPID_ALARM(tRlcCb,
                                spId, 
                                LKW_EVENT_KWU_BND_REQ, 
                                LCM_CAUSE_INV_PAR_VAL);
            RLOG1(L_ERROR,"RLC Mode [%d] : KWU SAP already Bound",
                  tRlcCb->genCfg.rlcMode);
            KwUiKwuBndCfm(&(kwuSap->pst), kwuSap->suId, CM_BND_NOK);
            RETVALUE(RFAILED);
         }
         break;
      }

     default:
      {
#if (ERRCLASS & ERRCLS_INT_PAR)
         RLOG1(L_ERROR,"RLC Mode [%d]:Invalid KWU SAP State in Bind Req",
               tRlcCb->genCfg.rlcMode);
         KW_SEND_SAPID_ALARM(tRlcCb,
                             spId,
                             LKW_EVENT_KWU_BND_REQ, 
                             LCM_CAUSE_INV_STATE);
#endif /* ERRCLASS & ERRCLS_INT_PAR */
         KwUiKwuBndCfm(&(kwuSap->pst), kwuSap->suId, CM_BND_NOK);
         RETVALUE(RFAILED);
      }
   }
   KwUiKwuBndCfm(&(kwuSap->pst), kwuSap->suId, CM_BND_OK);
   RETVALUE(ROK);
} 


/**
 * @brief 
 *    Handler for unbinding the RLC upper layer service user with 
 *    the RLC layer.
 *
 * @details
 *    This function is used by RLC user to request for unbinding  
 *    with RLC.This function is called by the KWU interface to 
 *    unbind with RLC. 
 *
 * @param[in] pst     Post structure  
 * @param[in] spId    Service provider SAP ID 
 * @param[in] reason  Reason for Unbinding 
 *
 * @return  S16
 *     -# ROK 
 *     -# RFAILED
 */
#ifdef ANSI
PUBLIC S16 KwUiKwuUbndReq
(
Pst      *pst,  
SpId     spId, 
Reason   reason 
)
#else
PUBLIC S16 KwUiKwuUbndReq(pst, spId, reason)
Pst      *pst; 
SpId     spId; 
Reason   reason; 
#endif
{
   KwKwuSapCb   *kwuSap;   /* KWU SAP control block */
   RlcCb         *tRlcCb;

   TRC3(KwUiKwuUbndReq)

#if (ERRCLASS & ERRCLS_INT_PAR)
   if ((pst->dstInst >= MAX_RLC_INSTANCES) ||
       (spId >= (S16) rlcCb[pst->dstInst]->genCfg.maxKwuSaps) ||
       (spId < 0))
   {
      RETVALUE (RFAILED);
   }
#endif

   tRlcCb = RLC_GET_RLCCB(pst->dstInst);

   RLOG2(L_DEBUG, "spId(%d), reason(%d)", 
                spId, 
                reason);

   /* Get Sap control block */
   kwuSap = (tRlcCb->genCfg.rlcMode == LKW_RLC_MODE_DL) ?
            (tRlcCb->u.dlCb->kwuDlSap + spId):
            (tRlcCb->u.ulCb->kwuUlSap + spId);

   kwuSap->state = KW_SAP_CFG;

   RETVALUE(ROK);
} 

/**
 * @brief Handler for receiving the data(SDU) from upper layer. 
 *
 * @details
 *    This function is used by RLC service user (PDCP) to 
 *    transfer data (SDU) to RLC.
 *
 * @param[in] pst         Post structure  
 * @param[in] spId        Service Provider SAP ID 
 * @param[in] datreq  Data Request Information
 * @param[in] mBuf        Data Buffer (SDU) 
 *
 * @return  S16
 *    -# ROK 
 *    -# RFAILED
 */
#ifdef ANSI
PUBLIC S16 KwUiKwuDatReq
(
Pst             *pst,   
KwuDatReqInfo   *datReq, 
Buffer          *mBuf   
)
#else
PUBLIC S16 KwUiKwuDatReq(pst, datReq, mBuf)
Pst             *pst;  
KwuDatReqInfo   *datReq; 
Buffer          *mBuf;  
#endif
{
   S16          ret = ROK;   /* Return Value */
   RlcDlRbCb     *rbCb;       /* RB Control Block */
   RlcCb         *tRlcCb;

   TRC3(KwUiKwuDatReq)

   DU_LOG("\nRLC : Received DL Data");

#if (ERRCLASS & ERRCLS_INT_PAR)
   if(pst->dstInst >= MAX_RLC_INSTANCES)
   {
      SPutMsg(mBuf);
      RETVALUE(RFAILED);
   }
#endif

   tRlcCb = RLC_GET_RLCCB(pst->dstInst);

   /* Fetch the RbCb */
   kwDbmFetchDlRbCbByRbId(tRlcCb, &datReq->rlcId, &rbCb);
   if(!rbCb)
   {
      RLOG_ARG2(L_WARNING, DBG_UEID,datReq->rlcId.ueId, "CellId[%u]:DL RbId [%d] not found",
            datReq->rlcId.cellId,datReq->rlcId.rbId);
      RLC_FREE_BUF(mBuf);

      RETVALUE(RFAILED);
   }

   /* Dispatch according to mode of the rbCb */
   switch (rbCb->mode)
   {
      case CM_LTE_MODE_TM:
      {
         /* Verify the user */
         if (pst->srcEnt != ENTNH)
         {
            /* kw002.201 Freeing from proper region */
            RLC_SHRABL_STATIC_BUF_FREE(pst->region, pst->pool, datReq, 
                        sizeof(KwuDatReqInfo));
            RLC_FREE_BUF(mBuf);
             
            RETVALUE(RFAILED);
         }

         kwTmmQSdu(tRlcCb,rbCb, datReq, mBuf);
         break;
      }
      case CM_LTE_MODE_UM:
      {
         kwUmmQSdu(tRlcCb,rbCb, datReq, mBuf);

         break;
      }
      case CM_LTE_MODE_AM:
      {
         kwAmmQSdu(tRlcCb,rbCb, mBuf, datReq);
         break;
      }
      default:
      {
         RLOG0(L_ERROR, "Invalid RB Mode");
         break;
      }
   }
   RETVALUE(ret);
} 


/**
 * @brief 
 *    Handler for discarding a SDU. 
 *
 * @details
 *    This function is used by RLC AM  and RLC UM entities. 
 *    This function is called by the service user to discard a particular
 *    RLC SDU if it is present in the SDU queue of the RB control block 
 *    and if it is not mapped to any PDU. 
 *
 * @param[in] pst            Post structure  
 * @param[in] spId           Service Provider SAP ID 
 * @param[in] discSdu        SDU discard Information 
 *
 * @return  S16
 *    -# ROK 
 *    -# RFAILED
 */
#ifdef ANSI
PUBLIC S16 KwUiKwuDiscSduReq 
(
Pst              *pst,   
SpId             spId,  
KwuDiscSduInfo   *discSdu 
)
#else
PUBLIC S16 KwUiKwuDiscSduReq(pst, spId, discSdu)
Pst              *pst;   
SpId             spId;  
KwuDiscSduInfo   *discSdu; 
#endif
{
   RLC_SHRABL_STATIC_BUF_FREE(pst->region, pst->pool, discSdu, sizeof(KwuDiscSduInfo));
   RETVALUE(ROK);
} 

/********************************************************************30**
         End of file
**********************************************************************/
