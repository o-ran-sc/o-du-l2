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
      
        --RlcUiCkwBndReq
        --RlcUiCkwUbndReq
        --RlcProcCfgReq

        --RlcUiKwuBndReq
        --RlcUiKwuUbndReq
        --rlcProcDlData
        --RlcUiKwuDiscSduReq 

     File:     kw_uim.c

**********************************************************************/
static const char* RLOG_MODULE_NAME="UIM";
static int RLOG_MODULE_ID=2048;
static int RLOG_FILE_ID=205;

/** 
 * @file kw_uim.c
 * @brief RLC Upper Interface Module
*/

#define RLC_MODULE RLC_DBGMASK_INF


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
S16 RlcUiCkwBndReq 
(
Pst    *pst,
SuId   suId,
SpId   spId 
)
#else
S16 RlcUiCkwBndReq (pst, suId, spId)
Pst    *pst; 
SuId   suId;
SpId   spId;
#endif
{
   RlcCkwSapCb   *ckwSap;  
   RlcCb         *tRlcCb;

#if (ERRCLASS & ERRCLS_INT_PAR)
   if (pst->dstInst >= MAX_RLC_INSTANCES)
   {
      return RFAILED;
   }
#endif

   tRlcCb = RLC_GET_RLCCB(pst->dstInst);

   RLOG2(L_DEBUG, "spId(%d), suId(%d)", spId, suId);
   ckwSap = &(tRlcCb->u.ulCb->ckwSap);
   /* Take action based on the current state of the SAP */
   switch(ckwSap->state)
   {
      /* SAP is configured but not bound */
      case RLC_SAP_CFG:
      case RLC_SAP_UBND:
      {
         /* copy bind configuration parameters in SSAP sap */
         ckwSap->suId = suId;
         ckwSap->pst.dstProcId = pst->srcProcId;
         ckwSap->pst.dstEnt = pst->srcEnt;
         ckwSap->pst.dstInst = pst->srcInst;

         /* Update the State */
         ckwSap->state = RLC_SAP_BND;

         RLOG1(L_DEBUG, "RlcUiCkwBndReq: state (%d)", ckwSap->state);
         break;
      }
      case RLC_SAP_BND:
      {
         /* Sap is already bound check source, destination Entity and  
          * Proc Id  */
         if (ckwSap->pst.dstProcId != pst->srcProcId ||
             ckwSap->pst.dstEnt != pst->srcEnt ||
             ckwSap->pst.dstInst != pst->srcInst ||
             ckwSap->suId != suId)
         {
            RLC_SEND_SAPID_ALARM(tRlcCb,
                                spId, 
                                LKW_EVENT_CKW_BND_REQ, 
                                LCM_CAUSE_INV_PAR_VAL);

            RLOG0(L_ERROR, "CKW SAP already Bound");
            RlcUiCkwBndCfm(&(ckwSap->pst), ckwSap->suId, CM_BND_NOK);
            return (RFAILED);
         }
         break;
      }
      default:
      {
#if (ERRCLASS & ERRCLS_INT_PAR)
         RLOG0(L_ERROR, "Invalid CKW SAP State in Bind Req");
         RLC_SEND_SAPID_ALARM(tRlcCb,
                             spId, 
                             LKW_EVENT_CKW_BND_REQ, 
                             LCM_CAUSE_INV_STATE);
#endif /* ERRCLASS & ERRCLS_INT_PAR */
         RlcUiCkwBndCfm(&(ckwSap->pst), ckwSap->suId, CM_BND_NOK);
         return (RFAILED);
         break;
      }
   }

   RlcUiCkwBndCfm(&(ckwSap->pst), ckwSap->suId, CM_BND_OK);
   return (ROK);
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
S16 RlcUiCkwUbndReq
(
Pst      *pst,   
SpId     spId,  
Reason   reason 
)
#else
S16 RlcUiCkwUbndReq(pst, spId, reason)
Pst      *pst; 
SpId     spId; 
Reason   reason;
#endif
{
   RlcCb *tRlcCb;

#if (ERRCLASS & ERRCLS_INT_PAR)
   if (pst->dstInst >= MAX_RLC_INSTANCES)
   {
      return RFAILED;
   }
#endif /* ERRCLASS & ERRCLS_INT_PAR */
   tRlcCb = RLC_GET_RLCCB(pst->dstInst);

   RLOG2(L_DEBUG,"spId(%d), reason(%d)", 
                spId, 
                reason);

   UNUSED(reason);

#if (ERRCLASS & ERRCLS_INT_PAR)
   RLC_GET_AND_VALIDATE_CKWSAP(tRlcCb, 
                              (&(tRlcCb->u.ulCb->ckwSap)), 
                              EKW208, 
                              "RlcUiCkwUbndReq");
#endif /* ERRCLASS & ERRCLS_INT_PAR */

   /* disable upper sap (CKW) */
   tRlcCb->u.ulCb->ckwSap.state = RLC_SAP_CFG;
   return ROK;
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
S16 RlcProcCfgReq
(
Pst          *pst,
RlcCfgInfo   *cfg
)
#else
S16 RlcProcCfgReq(pst, cfg)
Pst          *pst;
RlcCfgInfo   *cfg;
#endif
{
   RlcCb        *tRlcCb;
   RlcUlCfgTmpData   *cfgTmpData;

   static uint32_t  transCount;

#if (ERRCLASS & ERRCLS_INT_PAR)
   if (pst->dstInst >= MAX_RLC_INSTANCES)
   {
      RLC_PST_FREE(pst->region, pst->pool, cfg, sizeof(RlcCfgInfo));
      return RFAILED;
   }
#endif
   tRlcCb = RLC_GET_RLCCB(pst->dstInst);

   RLC_ALLOC(tRlcCb, cfgTmpData, sizeof (RlcUlCfgTmpData));

   if (cfgTmpData == NULLP)
   {
      RLC_PST_FREE(pst->region, pst->pool, cfg, sizeof(RlcCfgInfo));
      return RFAILED;
   }
   
   cfgTmpData->uprLyrTransId = cfg->transId; /*Save User TransId*/
   cfgTmpData->transId = ++transCount;       /*Generate new TransId*/
   cfg->transId = cfgTmpData->transId;
   cfgTmpData->cfgInfo  = cfg;

 
   tRlcCb->u.ulCb->rlcUlUdxEventType = pst->event;
   if (rlcDbmAddUlTransaction(tRlcCb, cfgTmpData) != ROK)
   {
      RLOG0(L_ERROR, "Addition to UL transId Lst Failed");
      RLC_PST_FREE(pst->region, pst->pool, cfg, sizeof(RlcCfgInfo));
      
      return RFAILED;
   }
   
   rlcUlHdlCfgReq(tRlcCb, cfgTmpData, cfg);
   rlcUlUdxCfgReq(&(RLC_GET_UDX_SAP(tRlcCb)->pst),RLC_GET_UDX_SAP(tRlcCb)->spId,cfg); 

   return ROK;
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
S16 RlcUiCkwUeIdChgReq
(
Pst         *pst, 
SpId        spId, 
uint32_t    transId, 
CkwUeInfo   *ueInfo,
CkwUeInfo   *newUeInfo
)
#else
S16 RlcUiCkwUeIdChgReq(pst, spId, transId, ueInfo, newUeInfo)
Pst         *pst;
SpId        spId;
uint32_t    transId;
CkwUeInfo   *ueInfo;
CkwUeInfo   *newUeInfo;
#endif
{
   S16              ret = ROK;
   RlcCb             *tRlcCb;
   RlcUlCfgTmpData   *cfgTmpData = NULLP;

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
                   "RlcUiCkwUeIdChgReq(pst, spId(%d), transId(%ld))", 
                   spId, 
                   transId);
#else
      RLOG_ARG2(L_DEBUG,DBG_CELLID,newUeInfo->cellId, 
                   "RlcUiCkwUeIdChgReq(pst, spId(%d), transId(%d))\n", 
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

      if (rlcDbmAddUlTransaction(tRlcCb, cfgTmpData))
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
      return RFAILED;
   }

   if(ROK != rlcCfgValidateUeIdChng(tRlcCb,ueInfo,newUeInfo,cfgTmpData))
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cfgTmpData->ueInfo->cellId, 
            "Validation Failure for UeId change");
   }

   rlcUlUdxUeIdChgReq(&(RLC_GET_UDX_SAP(tRlcCb)->pst),
                     RLC_GET_UDX_SAP(tRlcCb)->spId,
                     transId, 
                     ueInfo,
                     newUeInfo);

   return ROK;
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
Void rlcUlHdlCfgReq
(
RlcCb             *gCb,
RlcUlCfgTmpData   *cfgTmpData,
RlcCfgInfo       *cfg
)
#else
Void rlcUlHdlCfgReq(gCb, cfgTmpData, cfg)
RlcCb             *gCb;
RlcUlCfgTmpData   *cfgTmpData;
RlcCfgInfo       *cfg;
#endif
{
   uint8_t   idx;  

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
         
            if(cfg->entCfg[idx].dir & RLC_DIR_UL)
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
                  cfg->entCfg[idx].dir = RLC_DIR_UL;
               }
            }
            if(cfg->entCfg[idx].dir == RLC_DIR_UL)
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
            if(cfg->entCfg[idx].dir & RLC_DIR_UL)
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
                  cfg->entCfg[idx].dir = RLC_DIR_UL;
                  break;
               }
            }
            if(cfg->entCfg[idx].dir == RLC_DIR_UL)
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
               cfg->entCfg[idx].dir = RLC_DIR_UL;
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
               cfg->entCfg[idx].dir = RLC_DIR_UL;
            }
            break;
         }
      }/* switch end */
   }/* for end */
   return;
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
S16 RlcUiKwuBndReq 
(
Pst    *pst,  
SuId   suId, 
SpId   spId 
)
#else
S16 RlcUiKwuBndReq (pst, suId, spId)
Pst    *pst;  
SuId   suId; 
SpId   spId; 
#endif
{
   RlcKwuSapCb   *rlckwuSap;     /* SAP Config Block */
   RlcCb         *tRlcCb;
   
#if (ERRCLASS & ERRCLS_INT_PAR)
   if (pst->dstInst >= MAX_RLC_INSTANCES)
   {
      return RFAILED;
   }
#endif
   tRlcCb = RLC_GET_RLCCB(pst->dstInst);
   RLOG2(L_DEBUG, "RlcUiKwuBndReq(pst, spId(%d), suId(%d))", spId, suId);

    /* Validation of input parameters */
#if (ERRCLASS & ERRCLS_INT_PAR)
   if(!((spId < (S16) tRlcCb->genCfg.maxKwuSaps) && (spId >=0))) 
   {
      RLOG0(L_ERROR,"Invalid spId");
      RLC_SEND_SAPID_ALARM(tRlcCb,spId, LKW_EVENT_KWU_BND_REQ, LCM_CAUSE_INV_SAP);
      return RFAILED; 
   }
#endif

   /* Get Sap control block */
   rlckwuSap = (tRlcCb->genCfg.rlcMode == LKW_RLC_MODE_DL) ?
            (tRlcCb->u.dlCb->rlcKwuDlSap + spId):
            (tRlcCb->u.ulCb->rlcKwuUlSap + spId);

   /* Take action based on the current state of the SAP */
   switch(rlckwuSap->state)
   {
      /* SAP is configured but not bound */
      case RLC_SAP_CFG:
      case RLC_SAP_UBND:
      {
         /* copy bind configuration parameters in sap */
         rlckwuSap->suId          = suId;
         rlckwuSap->pst.dstProcId = pst->srcProcId;
         rlckwuSap->pst.dstEnt    = pst->srcEnt;
         rlckwuSap->pst.dstInst   = pst->srcInst;

         /* Update the State */
         rlckwuSap->state = RLC_SAP_BND;

         RLOG1(L_DEBUG, "RlcUiKwuBndReq: state (%d)", rlckwuSap->state);
         break;
      }
      case RLC_SAP_BND:
      {
         /* Sap is already bound check source, destination Entity and Proc Id */
         if (rlckwuSap->pst.dstProcId != pst->srcProcId ||
             rlckwuSap->pst.dstEnt != pst->srcEnt ||
             rlckwuSap->pst.dstInst != pst->srcInst ||
             rlckwuSap->suId != suId)
         {
            RLC_SEND_SAPID_ALARM(tRlcCb,
                                spId, 
                                LKW_EVENT_KWU_BND_REQ, 
                                LCM_CAUSE_INV_PAR_VAL);
            RLOG1(L_ERROR,"RLC Mode [%d] : KWU SAP already Bound",
                  tRlcCb->genCfg.rlcMode);
            RlcUiKwuBndCfm(&(rlckwuSap->pst), rlckwuSap->suId, CM_BND_NOK);
            return (RFAILED);
         }
         break;
      }

     default:
      {
#if (ERRCLASS & ERRCLS_INT_PAR)
         RLOG1(L_ERROR,"RLC Mode [%d]:Invalid KWU SAP State in Bind Req",
               tRlcCb->genCfg.rlcMode);
         RLC_SEND_SAPID_ALARM(tRlcCb,
                             spId,
                             LKW_EVENT_KWU_BND_REQ, 
                             LCM_CAUSE_INV_STATE);
#endif /* ERRCLASS & ERRCLS_INT_PAR */
         RlcUiKwuBndCfm(&(rlckwuSap->pst), rlckwuSap->suId, CM_BND_NOK);
         return (RFAILED);
      }
   }
   RlcUiKwuBndCfm(&(rlckwuSap->pst), rlckwuSap->suId, CM_BND_OK);
   return (ROK);
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
S16 RlcUiKwuUbndReq
(
Pst      *pst,  
SpId     spId, 
Reason   reason 
)
#else
S16 RlcUiKwuUbndReq(pst, spId, reason)
Pst      *pst; 
SpId     spId; 
Reason   reason; 
#endif
{
   RlcKwuSapCb   *rlckwuSap;   /* KWU SAP control block */
   RlcCb         *tRlcCb;

#if (ERRCLASS & ERRCLS_INT_PAR)
   if ((pst->dstInst >= MAX_RLC_INSTANCES) ||
       (spId >= (S16) rlcCb[pst->dstInst]->genCfg.maxKwuSaps) ||
       (spId < 0))
   {
      return  (RFAILED);
   }
#endif

   tRlcCb = RLC_GET_RLCCB(pst->dstInst);

   RLOG2(L_DEBUG, "spId(%d), reason(%d)", 
                spId, 
                reason);

   /* Get Sap control block */
   rlckwuSap = (tRlcCb->genCfg.rlcMode == LKW_RLC_MODE_DL) ?
            (tRlcCb->u.dlCb->rlcKwuDlSap + spId):
            (tRlcCb->u.ulCb->rlcKwuUlSap + spId);

   rlckwuSap->state = RLC_SAP_CFG;

   return ROK;
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
 * @return  uint8_t
 *    -# ROK 
 *    -# RFAILED
 */
uint8_t rlcProcDlData(Pst *pst, KwuDatReqInfo *datReq, Buffer *mBuf)
{
   uint8_t       ret = ROK;   /* Return Value */
   RlcDlRbCb     *rbCb;       /* RB Control Block */
   RlcCb         *tRlcCb;

   DU_LOG("\nRLC : Received DL Data");

#if (ERRCLASS & ERRCLS_INT_PAR)
   if(pst->dstInst >= MAX_RLC_INSTANCES)
   {
      ODU_PUT_MSG_BUF(mBuf);
      return RFAILED;
   }
#endif

   tRlcCb = RLC_GET_RLCCB(pst->dstInst);

   /* Fetch the RbCb */
   rlcDbmFetchDlRbCbByRbId(tRlcCb, &datReq->rlcId, &rbCb);
   if(!rbCb)
   {
      DU_LOG("\nRLC : CellId[%u]:DL RbId [%d] not found",
            datReq->rlcId.cellId,datReq->rlcId.rbId);
      RLC_FREE_BUF(mBuf);

      return RFAILED;
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
             
            return RFAILED;
         }

         rlcTmmQSdu(tRlcCb,rbCb, datReq, mBuf);
         break;
      }
      case CM_LTE_MODE_UM:
      {
         rlcUmmQSdu(tRlcCb,rbCb, datReq, mBuf);

         break;
      }
      case CM_LTE_MODE_AM:
      {
         rlcAmmQSdu(tRlcCb,rbCb, mBuf, datReq);
         break;
      }
      default:
      {
         DU_LOG("\nRLC : Invalid RB Mode");
         break;
      }
   }
   return ret;
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
S16 RlcUiKwuDiscSduReq 
(
Pst              *pst,   
SpId             spId,  
KwuDiscSduInfo   *discSdu 
)
#else
S16 RlcUiKwuDiscSduReq(pst, spId, discSdu)
Pst              *pst;   
SpId             spId;  
KwuDiscSduInfo   *discSdu; 
#endif
{
   RLC_SHRABL_STATIC_BUF_FREE(pst->region, pst->pool, discSdu, sizeof(KwuDiscSduInfo));
   return ROK;
} 

/********************************************************************30**
         End of file
**********************************************************************/
