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
  
     Name:    NR RLC Layer - Upper Interface Functions
    
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
/** 
 * @file rlc_upr_inf_mgr.c
 * @brief RLC Upper Interface Module
*/

#define RLC_MODULE RLC_DBGMASK_INF


/* header (.h) include files */
#include "common_def.h"
#include "lkw.h"           /* LKW defines */
#include "ckw.h"           /* CKW defines */
#include "kwu.h"           /* KWU defines */
#include "rgu.h"           /* RGU defines */
#include "rlc_env.h"        /* RLC environment options */
#include "rlc_err.h"


/* extern (.x) include files */
#include "lkw.x"           /* LKW */
#include "ckw.x"           /* CKW */
#include "kwu.x"           /* KWU */
#include "rgu.x"           /* RGU */

#include "rlc_utils.h"            /* RLC defines */
#include "rlc_dl_ul_inf.h"
#include "rlc_dl.h"
#include "rlc_ul.h"
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
S16 RlcUiCkwBndReq 
(
Pst    *pst,
SuId   suId,
SpId   spId 
)
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

   DU_LOG("\nDEBUG  -->  RLC_UL : spId(%d), suId(%d)", spId, suId);
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

         DU_LOG("\nDEBUG  -->  RLC_UL : RlcUiCkwBndReq: state (%d)", ckwSap->state);
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

            DU_LOG("\nERROR  -->  RLC_UL : CKW SAP already Bound");
            RlcUiCkwBndCfm(&(ckwSap->pst), ckwSap->suId, CM_BND_NOK);
            return (RFAILED);
         }
         break;
      }
      default:
      {
#if (ERRCLASS & ERRCLS_INT_PAR)
         DU_LOG("\nERROR  -->  RLC_UL : Invalid CKW SAP State in Bind Req");
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
S16 RlcUiCkwUbndReq
(
Pst      *pst,   
SpId     spId,  
Reason   reason 
)
{
   RlcCb *tRlcCb;

#if (ERRCLASS & ERRCLS_INT_PAR)
   if (pst->dstInst >= MAX_RLC_INSTANCES)
   {
      return RFAILED;
   }
#endif /* ERRCLASS & ERRCLS_INT_PAR */
   tRlcCb = RLC_GET_RLCCB(pst->dstInst);

   DU_LOG("\nDEBUG  -->  RLC_UL : spId(%d), reason(%d)", 
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
S16 RlcProcCfgReq
(
Pst          *pst,
RlcCfgInfo   *cfg
)
{
   uint8_t cfgIdx = 0;
   RlcCb        *tRlcCb;
   RlcUlCfgTmpData   *cfgTmpData;

   static uint32_t  transCount;

#if (ERRCLASS & ERRCLS_INT_PAR)
   if (pst->dstInst >= MAX_RLC_INSTANCES)
   {
      for(cfgIdx=0; cfgIdx<cfg->numEnt; cfgIdx++)
      {
         RLC_PST_FREE(pst->region, pst->pool, cfg->entCfg[cfgIdx].snssai, sizeof(Snssai)); 
      }
      RLC_PST_FREE(pst->region, pst->pool, cfg, sizeof(RlcCfgInfo));
      return RFAILED;
   }
#endif
   tRlcCb = RLC_GET_RLCCB(pst->dstInst);

   RLC_ALLOC(tRlcCb, cfgTmpData, sizeof (RlcUlCfgTmpData));

   if (cfgTmpData == NULLP)
   {
      for(cfgIdx=0; cfgIdx<cfg->numEnt; cfgIdx++)
      {
         RLC_PST_FREE(pst->region, pst->pool, cfg->entCfg[cfgIdx].snssai, sizeof(Snssai)); 
      }
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
      DU_LOG("\nERROR  -->  RLC_UL : Addition to UL transId Lst Failed");
      for(cfgIdx=0; cfgIdx<cfg->numEnt; cfgIdx++)
      {
         RLC_PST_FREE(pst->region, pst->pool, cfg->entCfg[cfgIdx].snssai, sizeof(Snssai)); 
      }
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
S16 RlcUiCkwUeIdChgReq
(
Pst         *pst, 
SpId        spId, 
uint32_t    transId, 
CkwUeInfo   *ueInfo,
CkwUeInfo   *newUeInfo
)
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
      DU_LOG("\nDEBUG  -->  RLC_UL : RlcUiCkwUeIdChgReq(pst, spId(%d), transId(%ld))", 
                   spId, 
                   transId);
#else
      DU_LOG("\nDEBUG  -->  RLC_UL : RlcUiCkwUeIdChgReq(pst, spId(%d), transId(%d))\n", 
                 spId, transId);
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
         DU_LOG("\nERROR  -->  RLC_UL : Addition to UL transId Lst Failed");
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
      DU_LOG("\nERROR  -->  RLC_UL : Validation Failure for UeId change");
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
Void rlcUlHdlCfgReq
(
RlcCb           *gCb,
RlcUlCfgTmpData *cfgTmpData,
RlcCfgInfo      *cfg
)
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
                  DU_LOG("\nERROR  -->  RLC_UL : CELLID [%u]:Validation Failure for UL RB [%d]",
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
                  DU_LOG("\nERROR  -->  RLC_UL : CellID [%u]:Validation Failure for Reest UL RB [%d]",
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
               DU_LOG("\nERROR  -->  RLC_UL : UL UEID [%d]:Validation Failure",
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
               DU_LOG("\nERROR  -->  RLC_UL : Del UL Cell Validation Failure");
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
S16 RlcUiKwuBndReq 
(
Pst    *pst,  
SuId   suId, 
SpId   spId 
)
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
   DU_LOG("\nDEBUG  -->  RLC_UL : RlcUiKwuBndReq(pst, spId(%d), suId(%d))", spId, suId);

    /* Validation of input parameters */
#if (ERRCLASS & ERRCLS_INT_PAR)
   if(!((spId < (S16) tRlcCb->genCfg.maxKwuSaps) && (spId >=0))) 
   {
      DU_LOG("\nERROR  -->  RLC_UL : Invalid spId");
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

         DU_LOG("\nDEBUG  -->  RLC_UL : RlcUiKwuBndReq: state (%d)", rlckwuSap->state);
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
            DU_LOG("\nERROR  -->  RLC_UL : RLC Mode [%d] : KWU SAP already Bound",
                  tRlcCb->genCfg.rlcMode);
            RlcUiKwuBndCfm(&(rlckwuSap->pst), rlckwuSap->suId, CM_BND_NOK);
            return (RFAILED);
         }
         break;
      }

     default:
      {
#if (ERRCLASS & ERRCLS_INT_PAR)
         DU_LOG("\nERROR  -->  RLC_UL : RLC Mode [%d]:Invalid KWU SAP State in Bind Req",
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
S16 RlcUiKwuUbndReq
(
Pst      *pst,  
SpId     spId, 
Reason   reason 
)
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

   DU_LOG("\nDEBUG  -->  RLC_UL : spId(%d), reason(%d)", 
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
uint8_t rlcProcDlData(Pst *pst, RlcDatReqInfo *datReq, Buffer *mBuf)
{
   uint8_t       ret = ROK;   /* Return Value */
   RlcDlRbCb     *rbCb;       /* RB Control Block */
   RlcCb         *tRlcCb;

   DU_LOG("\nDEBUG  -->  RLC_DL : Received DL Data");

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
      DU_LOG("\nERROR  -->  RLC_DL : CellId[%u]:DL RbId [%d] not found",
            datReq->rlcId.cellId,datReq->rlcId.rbId);
      ODU_PUT_MSG_BUF(mBuf);

      return RFAILED;
   }

   /* Dispatch according to mode of the rbCb */
   switch (rbCb->mode)
   {
      case RLC_MODE_TM:
      {
         /* Verify the user */
         if (pst->srcEnt != ENTNH)
         {
            /* kw002.201 Freeing from proper region */
            RLC_SHRABL_STATIC_BUF_FREE(pst->region, pst->pool, datReq, 
                        sizeof(RlcDatReqInfo));
            ODU_PUT_MSG_BUF(mBuf);
             
            return RFAILED;
         }

         rlcTmmQSdu(tRlcCb,rbCb, datReq, mBuf);
         break;
      }
      case RLC_MODE_UM:
      {
         rlcUmmQSdu(tRlcCb,rbCb, datReq, mBuf);
         break;
      }
      case RLC_MODE_AM:
      {
         rlcAmmQSdu(tRlcCb,rbCb, mBuf, datReq);
         break;
      }
      default:
      {
         DU_LOG("\nERROR  -->  RLC_DL : Invalid RB Mode");
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
S16 RlcUiKwuDiscSduReq 
(
Pst              *pst,   
SpId             spId,  
KwuDiscSduInfo   *discSdu 
)
{
   RLC_SHRABL_STATIC_BUF_FREE(pst->region, pst->pool, discSdu, sizeof(KwuDiscSduInfo));
   return ROK;
} 


/**
 * @brief 
 *    Handler for Creating, Searching or Deleting SnssnaiTput List. 
 *
 * @details
 *    This function is called whenever a new LC is configured with a snssai.
 *    This function is called to search for Snssai Node during RLC SDU formation
 *
 * @param[in] gCb            RlcCb 
 * @param[in] snssai         Snssai to be handled 
 * @param[in] Action         Type of action to be handled(Create,Search,Delete)
 *
 * @return  RlcTptPerSnssai
 *    -# Snssai Node
 *   
 */
RlcTptPerSnssai* rlcHandleSnssaiTputlist(RlcCb *gCb, Snssai *snssai, RlcSnssaiActionType action, Direction dir)
{
   CmLListCp *snssaiList = NULLP;
   CmLList  *node = NULLP;
   RlcTptPerSnssai *snssaiNode = NULLP;
   bool found = FALSE;

   if(dir == DIR_DL)
   {
      snssaiList = gCb->rlcThpt.snssaiTputInfo.dlTputPerSnssaiList;
      if(action == CREATE)
      {
         if(snssaiList == NULLP)
         {
            RLC_ALLOC(gCb, gCb->rlcThpt.snssaiTputInfo.dlTputPerSnssaiList, sizeof(CmLListCp));
            snssaiList = gCb->rlcThpt.snssaiTputInfo.dlTputPerSnssaiList;
            cmLListInit(snssaiList);
         }
      }
      else
      {
         if(snssaiList == NULLP)
         {
            DU_LOG("\nERROR --> RLC: SNSSAI DL list doesnt exist!");
            return NULLP;
         }
      }
   }
   else if(dir == DIR_UL)
   {
      snssaiList = gCb->rlcThpt.snssaiTputInfo.ulTputPerSnssaiList;
      if(action == CREATE)
      {
         if(snssaiList == NULLP)
         {
            RLC_ALLOC(gCb, gCb->rlcThpt.snssaiTputInfo.ulTputPerSnssaiList, sizeof(CmLListCp));
            snssaiList = gCb->rlcThpt.snssaiTputInfo.ulTputPerSnssaiList;
            cmLListInit(snssaiList);
         }
      }
      else
      {
         if(snssaiList == NULLP)
         {
            DU_LOG("\nERROR --> RLC: SNSSAI UL list doesnt exist!");
            return NULLP;
         }
      }
   }
   else
   {
      DU_LOG("\nERROR  -->  RLC : Direction:%d is invalid", dir);
      return NULLP;
   }

   node = snssaiList->first;

   /*Traversing the LC LinkList*/
   while(node)
   {
      snssaiNode = (RlcTptPerSnssai *)node->node;
      if(memcmp(snssaiNode->snssai, snssai, sizeof(Snssai)) == 0)
      { 
         DU_LOG("\nDEBUG  --> RLC : SNSSAI found in LL");
         found = TRUE;
         break;
      }
      node = node->next;
   }//end of while

   switch(action)
   {
      case SEARCH:
         {
            if(!found)
            {
               snssaiNode = NULLP;
            }
            return (snssaiNode);
         }

      case CREATE:
         {
            if(found)
               return (snssaiNode);

            snssaiNode = NULLP;
            /*Allocate the List*/
            RLC_ALLOC(gCb, snssaiNode, sizeof(RlcTptPerSnssai));
            if(snssaiNode)
            {
               RLC_ALLOC(gCb, snssaiNode->snssai, sizeof(Snssai));
               if(snssaiNode->snssai == NULLP)
               {
                 DU_LOG("\nERROR  --> RLC : Allocation of SNSSAI node failed");
                 return NULLP;
               }
               memcpy(snssaiNode->snssai,snssai,sizeof(Snssai));
               snssaiNode->dataVol = 0;
            }
            else
            {
               DU_LOG("\nERROR  --> RLC : Allocation of SNSSAI node failed");
               return NULLP;
            }

            node = NULLP;
            RLC_ALLOC(gCb, node, sizeof(CmLList));
            if(node)
            {
               node->node = (PTR)snssaiNode;
               cmLListAdd2Tail(snssaiList, node);
            }
            else
            {
               DU_LOG("\nERROR  --> RLC : Allocation of SNSSAI node failed");
               return NULLP;
            }
            DU_LOG("\nDEBUG  --> RLC : SNSSAI node added successfully");
            return (snssaiNode);
         }

      case DELETE:
         {
            if(found && node)
            {
               node = cmLListDelFrm(snssaiList, node);
               RLC_FREE(gCb, node, sizeof(CmLList));
               RLC_FREE(gCb, snssaiNode, sizeof(RlcTptPerSnssai));
               DU_LOG("\nDEBUG  --> RLC : SNSSAI node found and deletion performed");

               if(snssaiList->count == 0)
               {
                  RLC_FREE(gCb, snssaiList, sizeof(CmLListCp));
                  DU_LOG("\nINFO   --> RLC : This SNSSAI was last in the list thus freeing the list also");
               }
            }
            else
            {
               DU_LOG("\nERROR  --> RLC : SNSSAI node not found in List thus no deletion performed");
            }
            return NULLP;
         }
   }
   return (snssaiNode);
}

/**
 * @brief 
 *    Handler for Deleting SnssnaiTput List. 
 *
 * @details
 *    This function is called during Shutdown to remove all the snssai entries
 *    and deallocate the SNSSAI tput list as well
 *
 * @param[in] gCb            RlcCb 
 *
 * @return uint_8 (ROK/RFAILED)
 *   
 */
uint8_t rlcDelTputSnssaiList(RlcCb *gCb, Direction dir)
{
   CmLListCp *snssaiList = NULLP;
   CmLList  *node = NULLP, *next = NULLP;
   RlcTptPerSnssai *snssaiNode = NULLP;
   if(dir == DIR_DL)
   {
      snssaiList = gCb->rlcThpt.snssaiTputInfo.dlTputPerSnssaiList;
   }
   else if(dir == DIR_UL)
   {
      snssaiList = gCb->rlcThpt.snssaiTputInfo.ulTputPerSnssaiList;
   }
   else
   {
      DU_LOG("\nERROR  -->  RLC: Invalid direction:%d",dir);
      return RFAILED;
   }
   if(snssaiList == NULLP)
   {
      DU_LOG("\nERROR -->  RLC: SnssaiList not exist");
      return RFAILED;
   }
   node = snssaiList->first;

   /*Traversing the LC LinkList*/
   while(node)
   {
      snssaiNode = (RlcTptPerSnssai *)node->node;
      next = node->next;
      node = cmLListDelFrm(snssaiList, node);
      RLC_FREE(gCb, node, sizeof(CmLList));
      RLC_FREE(gCb, snssaiNode, sizeof(RlcTptPerSnssai));
      node = next;
   }
   if(snssaiList->count == 0)
   {
      RLC_FREE(gCb, snssaiList, sizeof(CmLListCp));
      DU_LOG("\nDEBUG   -->  RLC : This SNSSAI was last in the list thus freeing the list also");
   }
   return ROK;
}

/**
 * @brief 
 *    Handler for calculating the Tput for each SNSSAI in Tput list after expiry. 
 *
 * @details
 *    This function is called whenever SNSSAI Tput timer expires and calculate
 *    Tput for each Snssai in list
 *
 * @param[in] SnssaiList     A list of Snssai
 *
 * @return void 
 *   
 */
uint8_t rlcCalculateTputPerSnssai(CmLListCp *snssaiList, Direction dir)
{
   CmLList  *node = NULLP;
   RlcTptPerSnssai *snssaiNode = NULLP;
   uint8_t snssaiCnt = 0;

   node = snssaiList->first;
   if(node == NULLP)
   {
      DU_LOG("\n No SNSSAI in list");
      return(snssaiCnt);
   }
   /*Traversing the LC LinkList*/
   while(node)
   {
      snssaiNode = (RlcTptPerSnssai *)node->node;
      snssaiNode->tpt =  (double)(snssaiNode->dataVol * 8)/(double)(ODU_SNSSAI_THROUGHPUT_PRINT_TIME_INTERVAL * 0.001);
     
      if(dir == DIR_DL)
      {
         DU_LOG("\nDEBUG  -->  RLC_DL: SNSSAI(sst:%d,sd [%d,%d, %d]), DL Tpt : %.5lf", snssaiNode->snssai->sst,\
               snssaiNode->snssai->sd[0], snssaiNode->snssai->sd[1],snssaiNode->snssai->sd[2] , snssaiNode->tpt);
      }
      if(dir == DIR_UL)
      {
         DU_LOG("\nDEBUG  -->  RLC_UL: SNSSAI(sst:%d,sd [%d,%d, %d]), UL Tpt : %.5lf", snssaiNode->snssai->sst,\
               snssaiNode->snssai->sd[0], snssaiNode->snssai->sd[1],snssaiNode->snssai->sd[2] , snssaiNode->tpt);
      }

      snssaiNode->dataVol = 0;
      node = node->next;
      snssaiCnt++;
   }
   return(snssaiCnt);
}

/********************************************************************30**
         End of file
**********************************************************************/
