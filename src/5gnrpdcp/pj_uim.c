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
  
        Name:    LTE-PDCP Layer - Upper Interface Functions
    
        Type:    C file
  
        Desc:    Source code for PDCP Upper Interface Module
                 This file contains following functions
      
        --PjUiCpjBndReq
        --PjUiCpjUbndReq
        --PjUiCpjCfgReq
        --PjUiCpjUeIdChgReq
        --PjUiCpjReEstReq
        --PjUiCpjSecCfgReq
        --PjUiCpjSduStaReq
     

        --PjUiPjuBndReq
        --PjUiPjuUbndReq
        --PjUiPjuDatReq
        --PjUiPjuSduStaReq 

     File:     pj_uim.c

**********************************************************************/
static const char* RLOG_MODULE_NAME="PDCP";
static int RLOG_MODULE_ID=1024;
static int RLOG_FILE_ID=227;
/** @file pj_uim.c
@brief PDCP Upper Interface Module 
*/


/* header (.h) include files */
#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */

#include "gen.h"           /* general */
#include "ssi.h"           /* system services */
#include "cm5.h"           /* common timer defines */
#include "cm_tkns.h"       /* common tokens defines */
#include "cm_mblk.h"       /* common memory allocation library defines */
#include "cm_llist.h"      /* common link list  defines  */
#include "cm_hash.h"       /* common hash list  defines */
#include "cm_lte.h"        /* common LTE defines */
#include "lpj.h"           /* LPJ define */
#include "cpj.h"           /* RRC layer */
#include "pju.h"           /* PDCP service user */
#include "pj_env.h"        /* RLC environment options */
#include "pj.h"            /* RLC defines */
#include "pj_ul.h"
#include "pj_dl.h"
#include "pj_err.h"
#include "pj_udx.h"  /* udx interface */

/* extern (.x) include files */
#include "gen.x"           /* general */
#include "ssi.x"           /* system services */

#include "cm5.x"           /* common timer library */
#include "cm_tkns.x"       /* common tokens */
#include "cm_mblk.x"       /* common memory allocation */
#include "cm_llist.x"      /* common link list */
#include "cm_hash.x"       /* common hash list */
#include "cm_lte.x"        /* common LTE includes */
#include "cm_lib.x"        /* common memory allocation library */
#include "lpj.x"           /* LM(PDCP) */
#include "cpj.x"           /* RRC layer */
#include "pju.x"           /* PDCP service user */
#include "pj.x"
#include "pj_udx.h"
#include "pj_udx.x"
#include "pj_ul.x"
#include "pj_dl.x"
#include "pj_lib.x"
/* kw004.201 Assigned build issue in LTE RLC pj_uim.c */

#ifdef __cplusplus
EXTERN "C" {
#endif /* __cplusplus */



/*****************************************************************************
 *                          CPJ INTERFACE
 ****************************************************************************/
/**
 *
 * @brief 
 *
 *        Handler for binding the PDCP upper layer service user with 
 *        the PDCP layer.
 *
 * @b Description:
 *
 *        1. This function is used by PDCP user to request for binding to 
 *        PDCP. @n
 *        2. This function is called by the CPJ interface to bind 
 *        PDCP's SAP (identified by spId) with the service user's
 *        SAP (identified by suId). @n
 *
 *  @param[in] pst   Post structure  
 *  @param[in] suId  Service user SAP ID 
 *  @param[in] spId  Service provider ID
 *
 *  @return  S16
 *      -# ROK 
 *
 */
#ifdef ANSI
PUBLIC S16 PjUiCpjBndReq 
(
Pst  *pst,   
SuId suId,  
SpId spId   
)
#else
PUBLIC S16 PjUiCpjBndReq (pst, suId, spId)
Pst *pst;    
SuId suId;   
SpId spId;   
#endif
{
   PjCpjSapCb      *cpjSap;            /* pointer to session SAP */
   S16             ret;                /* return value */
   PjCb            *tPjCb;

   TRC3(PjUiCpjBndReq)

   ret = ROK;

   if (pst->dstInst >= PJ_MAX_PDCP_INSTANCES)   /* KW_FIX */
   {
       RETVALUE(RFAILED);
   }

   tPjCb = PJ_GET_PJCB(pst->dstInst);

   RLOG2(L_DEBUG, "PjUiCpjBndReq(pst, spId(%d), suId(%d))", spId, suId);

  

   cpjSap = &(tPjCb->u.ulCb->cpjSap);

   /* Verify CPJ SAP State */
   switch(cpjSap->state)
   {
      /* SAP is configured but not bound */
      case PJ_SAP_CFG:
      case PJ_SAP_UBND:
      {
         /* copy bind configuration parameters in SSAP sap */
         cpjSap->suId = suId;
         cpjSap->pst.dstProcId = pst->srcProcId;
         cpjSap->pst.dstEnt = pst->srcEnt;
         cpjSap->pst.dstInst = pst->srcInst;

         /* Update the State */
         cpjSap->state = PJ_SAP_BND;

         RLOG1(L_DEBUG, "PjUiCpjBndReq: state (%d)", cpjSap->state);
         break;
      }

      /* SAP is already bound */
      case PJ_SAP_BND:
      {
         /* 
          * Sap is already bound check source, destination Entity and 
          * Proc Id
          */
         if (cpjSap->pst.dstProcId != pst->srcProcId 
               || cpjSap->pst.dstEnt != pst->srcEnt
               || cpjSap->pst.dstInst != pst->srcInst
               || cpjSap->suId != suId)
         {
            PJ_SEND_SAPID_ALARM(tPjCb, spId, LPJ_EVENT_CPJ_BND_REQ, LCM_CAUSE_INV_PAR_VAL);

            RLOG1(L_DEBUG, "PjUiCpjBndReq: SAP already bound -- state(%d)", cpjSap->state);
            ret = RFAILED;
         }
         break;
      }

     default:
      {
         PJ_SEND_SAPID_ALARM(tPjCb,spId, LPJ_EVENT_CPJ_BND_REQ, LCM_CAUSE_INV_STATE);
         RLOG0(L_ERROR, "Invalid CPJ SAP State in Bind Req");
         ret = RFAILED;
         break;
      }
   }

   if (ret == ROK)
   {
      PjUiCpjBndCfm(&(cpjSap->pst), cpjSap->suId, CM_BND_OK);
   }
   else
   {
      PjUiCpjBndCfm(&(cpjSap->pst), cpjSap->suId, CM_BND_NOK);
   }

   RETVALUE(ROK);
} /* PjUiCpjBndReq */


/**
 *
 * @brief 
 *
 *        Handler for unbinding the RLC upper layer service user CPJ with 
 *        the RLC layer.
 *
 * @b Description:
 *
 *        1. This function is used by RLC user to request for unbinding  
 *        with RLC. @n
 *        2. This function is called by the CPJ interface to 
 *        unbind with RLC. @n
 *
 *  @param[in] pst     Post structure  
 *  @param[in] spId    Service provider SAP ID 
 *  @param[in] reason  Reason for Unbinding 
 *
 *  @return  S16
 *      -# ROK 
 *
 */
#ifdef ANSI
PUBLIC S16 PjUiCpjUbndReq
(
Pst      *pst,            
SpId     spId,           
Reason   reason           
)
#else
PUBLIC S16 PjUiCpjUbndReq(pst, spId, reason)
Pst      *pst;       
SpId     spId;      
Reason   reason;      
#endif
{
   PjCpjSapCb  *cpjSap;            /* CPJ SAP control block */
#if (ERRCLASS & ERRCLS_INT_PAR)
   S16         ret;                /* Return Value */
#endif
   PjCb   *tPjCb;

   TRC3(PjUiCpjUbndReq)

   if (pst->dstInst >= PJ_MAX_PDCP_INSTANCES)   /* KW_FIX */
   {
       RETVALUE(RFAILED);
   }

   tPjCb = PJ_GET_PJCB(pst->dstInst);
   RLOG2(L_DEBUG, "PjUiCpjUbndReq(pst, spId(%d), reason(%d))", spId, reason);

   UNUSED(pst);
   UNUSED(reason);

   /* disable upper sap (CPJ) */
   cpjSap = &(tPjCb->u.ulCb->cpjSap);


#if (ERRCLASS & ERRCLS_INT_PAR)
   ret    = ROK;
   PJ_GET_AND_VALIDATE_CPJSAP(tPjCb,cpjSap, EPJXXX, "PjUiCPjUbndReq", ret);
   if ( ret != ROK )
   {
      RETVALUE(RFAILED);
   }
#endif /* ERRCLASS & ERRCLS_INT_PAR */

   cpjSap->state = PJ_SAP_CFG;

   RETVALUE(ROK);
} /* CpUiCpjUbndReq */


/**
 *
 * @brief 
 *
 *        Handler for configuring PDCP entities.
 *
 * @b Description:
 *
 *        This function is used by RRC to configure(add/delete/modify/
 *        reestalish/delete all RB in a UE) one or more PDCP entities. 
 *
 *        - CPJ_CFG_ADD          => pjCfgAddRb
 *        - CPJ_CFG_MODIFY       => pjCfgReCfgRb
 *        - CPJ_CFG_DELETE       => pjCfgDelRb
 *        - CPJ_CFG_REESTABLISH  => pjCfgReEstRb
 *        - CPJ_CFG_DELETE_UE    => pjCfgDelUe
 *
 *  @param[in] pst   -  Post structure  
 *  @param[in] spId  -  Serive Provider ID 
 *  @param[in] cfg   -  Configuration information for one or more PDCP entities. 
 *
 *  @return  S16
 *      -# ROK 
 *
 */
#ifdef ANSI
PUBLIC S16 PjUiCpjCfgReq
(
Pst             *pst,
SpId            spId,
CpjCfgReqInfo   *cfg
)
#else
PUBLIC S16 PjUiCpjCfgReq(pst, spId, cfg)
Pst            *pst;
SpId           spId;
CpjCfgReqInfo  *cfg;
#endif
{
   PjCpjSapCb           *cpjSap;       /* CPJ SAP Control Block */
   PjUdxUlSapCb         *udxSap;       /* UDX SAP Control Block */ 
   S16                  ret;           /* Return Value */
   PjCb                 *tPjCb;
   PjCfgInfo            *cfgInfo;      /* added here n below */
   UdxCfgReqInfo        *udxCfgReqInfo;       
   

   TRC3(PjUiCpjCfgReq)

   ret       = ROK;

   if (pst->dstInst >= PJ_MAX_PDCP_INSTANCES)   /* KW_FIX */
   {
       RETVALUE(RFAILED);
   }
   tPjCb = PJ_GET_PJCB(pst->dstInst);
#ifndef ALIGN_64BIT
   RLOG2(L_DEBUG, "PjUiPjuCfgReq(pst, spId(%d), TransId(%ld))", spId, cfg->transId);
#else
   RLOG2(L_DEBUG, "PjUiPjuCfgReq(pst, spId(%d), TransId(%d))", spId, cfg->transId);
#endif

   /* Get the cpjSap */
   cpjSap = &(tPjCb->u.ulCb->cpjSap);

   /* Validate SAP ID under ERRORCLS */
   PJ_VALDATE_SAP(tPjCb, spId, cpjSap, ret);
   if (ret != ROK)
   {
      /* pj002.201 Freeing from proper region */
      PJ_PST_FREE(pst->region, pst->pool, cfg, sizeof(CpjCfgReqInfo));
      RETVALUE(RFAILED);
   }

   udxSap = PJ_GET_UL_UDX_SAP(tPjCb);
   PJ_ALLOC_BUF_SHRABL(udxSap->pst,udxCfgReqInfo,sizeof (UdxCfgReqInfo),ret);
   if(ret != ROK)
   {
      RLOG0(L_FATAL, "Memory Allocation failed.");
      PJ_PST_FREE(pst->region, pst->pool, cfg, sizeof(CpjCfgReqInfo));
      RETVALUE(RFAILED);
   }


   PJ_MEM_CPY(udxCfgReqInfo,cfg,sizeof(UdxCfgReqInfo));
   PJ_ALLOC(tPjCb, cfgInfo, sizeof (PjCfgInfo));

   if (cfgInfo == NULLP)
   {
      RLOG0(L_FATAL, "Memory Allocation failed.");
      PJ_PST_FREE(pst->region, pst->pool, cfg, sizeof(CpjCfgReqInfo));
      PJ_FREE_SHRABL_BUF_PST(pst->region, pst->pool, udxCfgReqInfo, sizeof(UdxCfgReqInfo));
      RETVALUE(RFAILED);
   }

   cfgInfo->uprLyrTransId = cfg->transId;
   cfgInfo->transId = ++tPjCb->u.ulCb->transId; 
   udxCfgReqInfo->transId = cfgInfo->transId;
   cfgInfo->cfgInfo  = cfg;
 
   ret = pjDbmAddUlTransIdLst(tPjCb, cfgInfo);
   if (ret != ROK)
   {
      RLOG0(L_ERROR, "Addition to UL transId List Failed.");
      PJ_PST_FREE(pst->region, pst->pool, cfg, sizeof(CpjCfgReqInfo));
      PJ_FREE_SHRABL_BUF_PST(pst->region, pst->pool, udxCfgReqInfo, sizeof(UdxCfgReqInfo));
      PJ_FREE(tPjCb, cfgInfo, sizeof (PjCfgInfo));

      RETVALUE(RFAILED);
   }

   udxSap = PJ_GET_UL_UDX_SAP(tPjCb);
   
   PjUlUdxCfgReq(&(udxSap->pst), udxSap->spId,udxCfgReqInfo); 

   RETVALUE(ret);
} /* PjUiPjuCfgReq */

/**
 *@details This primitive is used by RRC to change the UeId for the existing UE
           context.

 * @param pst     -  Point to the pst structure
 * @param spId    -  The ID of the service provider SAP in the PDCP layer 
 * @param transId -  Transaction ID. This field uniquily identifies
 *                   transaction between RRC and PDCP.
 * @param ueInfo    -  Old UE Id Info for which the change request has come 
 * @param newUeInfo -  New UE Id Info for existing UE context 
 * @return ROK
 */
#ifdef ANSI
PUBLIC S16 PjUiCpjUeIdChgReq
(
Pst        *pst, 
SpId       spId, 
U32        transId, 
CpjUeInfo  *ueInfo,
CpjUeInfo  *newUeInfo
)
#else
PUBLIC S16 PjUiCpjUeIdChgReq(pst,spId,transId,ueInfo,newUeInfo)
Pst        *pst;
SpId       spId;
U32        transId;
CpjUeInfo  *ueInfo;
CpjUeInfo  *newUeInfo;
#endif
{
   S16         ret;           /* Return Value */
   PjCpjSapCb  *cpjSap;       /* CPJ SAP Control Block */
   PjUdxUlSapCb  *udxSap;     /* UDX SAP Control Block */ 
   PjCfgInfo   *cfgInfo;      

   UdxUeInfo  *udxUeInfo;        
   UdxUeInfo  *udxNewUeInfo;
   PjCb         *tPjCb;
   
   TRC3(PjUiCpjUeIdChgReq)

   tPjCb = PJ_GET_PJCB(pst->dstInst);

#ifndef ALIGN_64BIT
   RLOG2(L_DEBUG, "PjUiCpjUeIdChgReq(pst, spId(%d), transId(%ld))", spId, transId);
#else
   RLOG2(L_DEBUG, "PjUiCpjUeIdChgReq(pst, spId(%d), transId(%d))", spId, transId);
#endif

   ret = ROK;
   cpjSap = &(tPjCb->u.ulCb->cpjSap);
   
   /* Validate SAP ID under ERRORCLS */
   PJ_VALDATE_SAP(tPjCb, spId, cpjSap, ret);
   if (ret != ROK)
   {
      /* kw002.201 Freeing from proper region */
      PJ_PST_FREE(pst->region, pst->pool, ueInfo, sizeof(CpjUeInfo));
      PJ_PST_FREE(pst->region, pst->pool, newUeInfo, sizeof(CpjUeInfo));
      RETVALUE(RFAILED);
   }

   /* Allocate memory for CkwUeInfo */
   udxSap = PJ_GET_UL_UDX_SAP(tPjCb);
   PJ_ALLOC_BUF_SHRABL(udxSap->pst, udxUeInfo, 
                     sizeof (UdxUeInfo), ret);
    if(ret != ROK)
   {
      RLOG0(L_FATAL, "Memory Allocation failed.");
      /* Free memory for CkwUeInfo */
      /* Freeing from proper region */
      PJ_PST_FREE(pst->region, pst->pool, newUeInfo, sizeof(CpjUeInfo));
      PJ_PST_FREE(pst->region, pst->pool, ueInfo, sizeof(CpjUeInfo));
      RETVALUE(RFAILED);
   }
   PJ_ALLOC_BUF_SHRABL(udxSap->pst, udxNewUeInfo, 
      sizeof (UdxUeInfo), ret);
   if(ret != ROK)
   {
      RLOG0(L_FATAL, "Memory Allocation failed.");
      /* Free memory for CkwUeInfo */
      /* Freeing from proper region */
      PJ_FREE_SHRABL_BUF_PST(udxSap->pst.region,udxSap->pst.pool, udxUeInfo, sizeof(UdxUeInfo));
      PJ_PST_FREE(pst->region, pst->pool, newUeInfo, sizeof(CpjUeInfo));
      PJ_PST_FREE(pst->region, pst->pool, ueInfo, sizeof(CpjUeInfo));
      RETVALUE(RFAILED);
   }

   PJ_MEM_CPY(udxUeInfo,ueInfo, sizeof(UdxUeInfo)); 
   PJ_MEM_CPY(udxNewUeInfo,newUeInfo, sizeof (UdxUeInfo));

   PJ_ALLOC(tPjCb, cfgInfo, sizeof (PjCfgInfo));
   if (cfgInfo == NULLP)
   {
      RLOG0(L_FATAL, "Memory Allocation failed.");
      PJ_FREE_SHRABL_BUF_PST(udxSap->pst.region,udxSap->pst.pool, udxUeInfo, sizeof(UdxUeInfo));
      PJ_FREE_SHRABL_BUF_PST(udxSap->pst.region,udxSap->pst.pool, udxNewUeInfo, sizeof(UdxUeInfo));
      PJ_PST_FREE(pst->region, pst->pool, newUeInfo, sizeof(CpjUeInfo));
      PJ_PST_FREE(pst->region, pst->pool, ueInfo, sizeof(CpjUeInfo));
      RETVALUE(RFAILED);
   }

   cfgInfo->uprLyrTransId = transId;
   cfgInfo->transId = ++tPjCb->u.ulCb->transId; 
   cfgInfo->ueInfo  = ueInfo;
   cfgInfo->newUeInfo  = newUeInfo; 

   if((ret = pjDbmAddUlTransIdLst(tPjCb, cfgInfo)) != ROK)
   {
      RLOG0(L_ERROR, "Addition to UL transId List Failed.");
      PJ_FREE_SHRABL_BUF_PST(udxSap->pst.region,udxSap->pst.pool, udxUeInfo, sizeof(UdxUeInfo));
      PJ_FREE_SHRABL_BUF_PST(udxSap->pst.region,udxSap->pst.pool, udxNewUeInfo, sizeof(UdxUeInfo));
      PJ_PST_FREE(pst->region, pst->pool, newUeInfo, sizeof(CpjUeInfo));
      PJ_PST_FREE(pst->region, pst->pool, ueInfo, sizeof(CpjUeInfo));
      RETVALUE(RFAILED);
   }

   
   PjUlUdxUeIdChgReq(&(udxSap->pst),udxSap->spId,cfgInfo->transId,
                                         udxUeInfo,udxNewUeInfo);

   RETVALUE(ROK);
} /* PjUiCpjUeIdChgReq */


/**
 *@details  
 * 1. RRC uses this primitive to configure PDCP security parameters.
 * Integrity protection and/or Ciphering are configured by RRC based on the
 * selSecAct flag.  @n
 * 2. Integirty Protection/Ciphering are configured per UE and applicable
 * to all pdcp entities in that UE. Both Integrity protection and ciphering are
 * re-configured during re-establishment.  @n
 *
 * @param pst     -  Point to the pst structure
 * @param spId    -  The ID of the service provider SAP in the PDCP layer 
 * @param secCfg  -  Security Configuration Info per UE.
 *
 * @return 
 *       -# ROK
 *       -# RFAILED
 */
#ifdef ANSI
PUBLIC S16 PjUiCpjSecCfgReq
(
Pst              *pst,
SpId             spId,
CpjSecCfgReqInfo *secCfg
)
#else
PUBLIC S16 PjUiCpjSecCfgReq(pst,spId,secCfg)
Pst              *pst;
SpId             spId;
CpjSecCfgReqInfo *secCfg;
#endif
{
   S16               ret;           /* Return Value */
   PjUdxUlSapCb      *udxSap;       /* UDX SAP Control Block */ 
   PjCb   *tPjCb;
   PjCfgInfo        *cfgInfo;      
   UdxSecCfgReqInfo *udxSecCfgReqInfo;

   TRC2(PjUiCpjSecCfgReq)

   if (pst->dstInst >= PJ_MAX_PDCP_INSTANCES)    /* KW_FIX */
   {
       RETVALUE(RFAILED);
   }
   tPjCb = PJ_GET_PJCB(pst->dstInst);

#ifndef ALIGN_64BIT 
   RLOG2(L_DEBUG, "PjUiCpjSecCfgReq(pst, spId(%d), TransId(%ld))", spId,
               secCfg->transId);
#else
   RLOG2(L_DEBUG, "PjUiCpjSecCfgReq(pst, spId(%d), TransId(%d))", spId,
               secCfg->transId);
#endif

   ret = ROK;

   udxSap = PJ_GET_UL_UDX_SAP(tPjCb);
   PJ_ALLOC_BUF_SHRABL(udxSap->pst, udxSecCfgReqInfo,sizeof (UdxSecCfgReqInfo), ret);
   if(ret != ROK)
   {
      RLOG0(L_FATAL, "Memory Allocation failed.");
      RETVALUE(RFAILED);
   }

   PJ_MEM_CPY(udxSecCfgReqInfo,secCfg,sizeof(UdxSecCfgReqInfo));
   PJ_ALLOC(tPjCb, cfgInfo, sizeof (PjCfgInfo));

   if (cfgInfo == NULLP)
   {
      RLOG0(L_FATAL, "Memory Allocation failed.");
      RETVALUE(RFAILED);
   }
   
   cfgInfo->uprLyrTransId = secCfg->transId;
   cfgInfo->transId = ++tPjCb->u.ulCb->transId; 
   udxSecCfgReqInfo->transId = cfgInfo->transId;
   cfgInfo->secCfgInfo  = secCfg;
 
   ret = pjDbmAddUlTransIdLst(tPjCb, cfgInfo);
   if (ret != ROK)
   {
      RLOG0(L_ERROR, "Addition to UL transId List Failed.");
      RETVALUE(RFAILED);
   }

   PjUlUdxSecCfgReq(&(udxSap->pst), udxSap->spId,udxSecCfgReqInfo); 

   RETVALUE(ret); 
}

/**
 * @details
 * 1. RRC uses this primitive to re-establish the PDCP entity. @n
 * 2. This primitive is initiation of re-establishment procedure. @n
 * 3. This prmitive and corresponding confirmation shall be followed by
 *    PjUiCpjCfgReq procedure for entire re-establishment procedure to complete.
 *
 * @param[in] pst    - Pointer to post structure.
 * @param[in] spId   - Service provider ID. This is the reference number used by
 *                     PDCP to identify its SAP.
 * @param[in] reEstReq - PDCP re-establishment request structure. 
 *
 */
#ifdef ANSI
PUBLIC S16 PjUiCpjReEstReq 
(
Pst             *pst, 
SpId            spId, 
CpjReEstReqInfo *reEstReq
)
#else 
PUBLIC S16 PjUiCpjReEstReq(pst,spId,reEstReq)
Pst             *pst;
SpId            spId;
CpjReEstReqInfo *reEstReq;
#endif
{
   S16               ret;           /* Return Value */
   PjCpjSapCb        *cpjSap;       /* CPJ SAP Control Block */
   PjUdxUlSapCb      *udxSap;     /* UDX SAP Control Block */ 
   PjCb   *tPjCb;
   PjCfgInfo         *cfgInfo;      
   UdxReEstReqInfo   *udxReEstReqInfo;
   
   TRC2(PjUiCpjReEstReq);

   if (pst->dstInst >= PJ_MAX_PDCP_INSTANCES)        /* KW_FIX */
   {
      PJ_PST_FREE(pst->region, pst->pool, reEstReq, sizeof(CpjReEstReqInfo));
      RETVALUE(RFAILED);
   }
   tPjCb = PJ_GET_PJCB(pst->dstInst);
   RLOG1(L_DEBUG, "PjUiCpjReEstReq(pst, spId(%d))", spId);

   ret = ROK;
   cpjSap = &(tPjCb->u.ulCb->cpjSap);

   /* Validate SAP ID under ERRORCLS */
   PJ_VALDATE_SAP(tPjCb, spId, cpjSap, ret);
   if (ret != ROK)
   {
      /* pj002.201 Freeing from proper region */  
      PJ_PST_FREE(pst->region, pst->pool, reEstReq, sizeof(CpjReEstReqInfo));
      RETVALUE(RFAILED);
   }

   udxSap = PJ_GET_UL_UDX_SAP(tPjCb);
   PJ_ALLOC_BUF_SHRABL(udxSap->pst, udxReEstReqInfo, 
      sizeof (UdxReEstReqInfo), ret);
   if(ret != ROK)
   {
      RLOG0(L_FATAL, "Memory Allocation failed.");
      PJ_PST_FREE(pst->region, pst->pool, reEstReq, sizeof(CpjReEstReqInfo));
      RETVALUE(RFAILED);
   }

   PJ_MEM_CPY(udxReEstReqInfo,reEstReq,sizeof(UdxReEstReqInfo));

   PJ_ALLOC(tPjCb, cfgInfo, sizeof (PjCfgInfo));
   if (cfgInfo == NULLP)
   {
      RLOG0(L_FATAL, "Memory Allocation failed.");
      PJ_PST_FREE(pst->region, pst->pool, reEstReq, sizeof(CpjReEstReqInfo));
      RETVALUE(RFAILED);
   }

   cfgInfo->uprLyrTransId = reEstReq->transId;
   cfgInfo->transId = ++tPjCb->u.ulCb->transId; 
   udxReEstReqInfo->transId = cfgInfo->transId;
   cfgInfo->reEstReq  = reEstReq;
 
   ret = pjDbmAddUlTransIdLst(tPjCb, cfgInfo);
   if (ret != ROK)
   {
      RLOG0(L_ERROR, "Addition to UL transId List Failed.");
      PJ_PST_FREE(pst->region, pst->pool, reEstReq, sizeof(CpjReEstReqInfo));
      PJ_FREE(tPjCb, cfgInfo, sizeof (PjCfgInfo));
      RETVALUE(RFAILED);
   }

   udxSap = PJ_GET_UL_UDX_SAP(tPjCb);
   PjUlUdxReEstReq(&(udxSap->pst), udxSap->spId,udxReEstReqInfo); 

   RETVALUE(ROK); 
}


/**
 * @details
 * 1. RRC uses this primitive to request count value for all DRBs in a UE. @n
 * 2. PDCP sends the UL and DL count values in the confirmation of this request. 
 *
 * @param[in] pst    - Pointer to post structure.
 * @param[in] spId   - Service provider ID. This is the reference number used
 * by PDCP to identify its SAP.
 * @param[in] countReq - PDCP Count Request Information. 
 *
 * @return ROK
 */
#ifdef ANSI
PUBLIC S16 PjUiCpjCountReq 
(
Pst             *pst, 
SpId            spId, 
CpjCountReqInfo *countReq
)
#else
PUBLIC S16 PjUiCpjCountReq(pst,spId,countReq)
Pst             *pst;
SpId            spId;
CpjCountReqInfo *countReq;
#endif
{
   S16               ret;           /* Return Value */
   PjUdxUlSapCb      *udxSap;       /* UDX SAP Control Block */ 
   PjCb   *tPjCb;
   PjCfgInfo       *cfgInfo;     
   UdxCountReqInfo *udxCountReqInfo;  

   TRC2(PjUiCpjCountReq);

   if (pst->dstInst >= PJ_MAX_PDCP_INSTANCES)    /* KW_FIX */
   {
      PJ_PST_FREE(pst->region, pst->pool, countReq, sizeof(CpjCountReqInfo));
      RETVALUE(RFAILED);
   }
  

   tPjCb = PJ_GET_PJCB(pst->dstInst);
   RLOG1(L_DEBUG, "PjUiCpjCountReq(pst, spId(%d))", spId);

   ret = ROK;
  
   udxSap = PJ_GET_UL_UDX_SAP(tPjCb);
   PJ_ALLOC_BUF_SHRABL(udxSap->pst, udxCountReqInfo, sizeof (UdxCountReqInfo), ret);
   if(ret != ROK)
   {
      RLOG0(L_FATAL, "Memory Allocation failed.");
      RETVALUE(RFAILED);
   }

   PJ_MEM_CPY(udxCountReqInfo,countReq,sizeof(UdxCountReqInfo));
   PJ_ALLOC(tPjCb, cfgInfo, sizeof (PjCfgInfo));

   if (cfgInfo == NULLP)
   {
      RLOG0(L_FATAL, "Memory Allocation failed.");
      PJ_PST_FREE(pst->region, pst->pool, countReq, sizeof(CpjCountReqInfo));
      RETVALUE(RFAILED);
   }

   cfgInfo->uprLyrTransId = countReq->transId;
   cfgInfo->transId = ++tPjCb->u.ulCb->transId; 
   udxCountReqInfo->transId = cfgInfo->transId;
   cfgInfo->cntReq  = countReq;
 
   ret = pjDbmAddUlTransIdLst(tPjCb, cfgInfo);
   if (ret != ROK)
   {
      RLOG0(L_ERROR, "Addition to UL transId List Failed.");
      PJ_PST_FREE(udxSap->pst.region, udxSap->pst.pool, udxCountReqInfo, sizeof(UdxCountReqInfo));
      PJ_PST_FREE(pst->region, pst->pool, countReq, sizeof(CpjCountReqInfo));
      PJ_FREE(tPjCb, cfgInfo, sizeof (PjCfgInfo));
      RETVALUE(RFAILED);
   }

   
   PjUlUdxCountReq(&(udxSap->pst), udxSap->spId,udxCountReqInfo); 

   RETVALUE(ROK);
}

/**
 * 
 * @details
 * 1. RRC uses this primitive to request the status of the SDUs for DRBs in an UE. @n
 * 2. Its used as a reestablishment request during handover.  @n
 * 3. This primitive is used to forward the handover control Information to the target
 * eNodeB.
 *
 * @param[in] pst    - Pointer to post structure.
 * @param[in] spId   - Service provider ID. This is the reference number used
 * by PDCP to identify its SAP.
 * @param[in] sduStaReq - PDCP SDU Status Request Information. 
 *
 * @return ROK
 */
#ifdef ANSI
PUBLIC S16 PjUiCpjSduStaReq 
(
Pst              *pst, 
SpId             spId, 
CpjSduStaReqInfo *staReq
)
#else
PUBLIC S16 PjUiCpjSduStaReq(pst,spId,staReq)
Pst              *pst;
SpId             spId;
CpjSduStaReqInfo *staReq;
#endif
{

   S16               ret;           /* Return Value */
   PjCpjSapCb        *cpjSap;       /* CPJ SAP Control Block */
   PjUlUeCb          *ueCb;
   PjUdxUlSapCb      *udxSap;       /* UDX SAP Control Block */ 
   PjCb              *tPjCb;
   PjCfgInfo         *cfgInfo;      /* added here n below */
   UdxSduStaReqInfo  *udxSduStaReq;

   TRC2(PjUiCpjSduStaReq);

   if (pst->dstInst >= PJ_MAX_PDCP_INSTANCES)       /* KW_FIX */
   {
      PJ_PST_FREE(pst->region, pst->pool, staReq, sizeof(CpjSduStaReqInfo));	
      RETVALUE(RFAILED);
   }

   tPjCb = PJ_GET_PJCB(pst->dstInst);

   RLOG1(L_DEBUG, "PjUiCpjSduStaReq(pst, spId(%d))", spId);

   ret = ROK;
   cpjSap = NULLP;

   cpjSap = &(tPjCb->u.ulCb->cpjSap);

   /* Validate SAP ID under ERRORCLS */
   PJ_VALDATE_SAP(tPjCb, spId, cpjSap, ret);
   if (ret != ROK)
   {
      /* kw002.201 Freeing from proper region */
      PJ_PST_FREE(pst->region, pst->pool, staReq, sizeof(CpjSduStaReqInfo));
      RETVALUE(RFAILED);
   }

   ret = pjDbmFetchUlUeCb(tPjCb, staReq->ueId, staReq->cellId, &ueCb);
   /* UeCb is not present */
   if( ROK != ret || NULLP == ueCb)
   { 
      RLOG_ARG1(L_ERROR, DBG_CELLID, staReq->cellId,"UL UeId[%u] not found",
            staReq->ueId);
      CpjSduStaCfmInfo  *cfmInfo;
      /* Memory leak fix ccpu00135359 */
      cpjSap = &tPjCb->u.ulCb->cpjSap;
      if(SGetSBuf(cpjSap->pst.region,cpjSap->pst.pool,(Data **)&cfmInfo, 
         sizeof (CpjSduStaCfmInfo)) != ROK)
      {
         RLOG0(L_FATAL, "Memory Allocation failed.");
         PJ_PST_FREE(pst->region, pst->pool, staReq, sizeof(CpjSduStaReqInfo));
         RETVALUE(RFAILED);
      }
      cfmInfo->ueId = staReq->ueId;
      cfmInfo->cellId = staReq->cellId;
      cfmInfo->transId = staReq->transId;
      cfmInfo->status = CPJ_CFG_CFM_NOK; 
      cfmInfo->reason = CPJ_CFG_REAS_UE_UNKWN;

      /* Caller should return cfm with failure */
      RLOG_ARG1(L_DEBUG,DBG_UEID,staReq->ueId, "Prc Sdu Sta Req - UE Cb is not"
            " found in cellId(%d)", staReq->cellId);

      /* Send Failure SduStaCfm */
      PjUiCpjSduStaCfm(&tPjCb->u.ulCb->cpjSap.pst, 
                        tPjCb->u.ulCb->cpjSap.suId, cfmInfo);

      PJ_PST_FREE(pst->region, pst->pool, staReq, sizeof(CpjSduStaReqInfo));

      RETVALUE(RFAILED);
   }
 
   /* Updating UE state to REEST during HO */
   ueCb->libInfo.numReEstDrb = 0;
   ueCb->libInfo.state = PJ_STATE_REEST_HO;

   udxSap = PJ_GET_UL_UDX_SAP(tPjCb);
   ret = ROK;
   PJ_ALLOC_BUF_SHRABL(udxSap->pst, udxSduStaReq, 
      sizeof (UdxSduStaReqInfo), ret);
   if(ret != ROK)
   {
      RLOG0(L_FATAL, "Memory Allocation failed.");
      PJ_PST_FREE(pst->region, pst->pool, staReq, sizeof(CpjSduStaReqInfo));
      RETVALUE(RFAILED);
   }

   PJ_MEM_CPY(udxSduStaReq,staReq,sizeof(UdxSduStaReqInfo));

   PJ_ALLOC(tPjCb, cfgInfo, sizeof (PjCfgInfo));
   if (cfgInfo == NULLP)
   {
      RLOG0(L_FATAL, "Memory Allocation failed.");
      PJ_PST_FREE(pst->region, pst->pool, staReq, sizeof(CpjSduStaReqInfo));
      PJ_PST_FREE(pst->region, pst->pool, udxSduStaReq, sizeof(UdxSduStaReqInfo));
      RETVALUE(RFAILED);
   }

   cfgInfo->uprLyrTransId = staReq->transId;
   cfgInfo->transId = ++tPjCb->u.ulCb->transId; 
   udxSduStaReq->transId = cfgInfo->transId;
   cfgInfo->staReq  = staReq;
 
   ret = pjDbmAddUlTransIdLst(tPjCb, cfgInfo);
   if (ret != ROK)
   {
      RLOG0(L_ERROR, "Addition to UL transId List Failed.");
      PJ_PST_FREE(pst->region, pst->pool, staReq, sizeof(CpjSduStaReqInfo));
      PJ_PST_FREE(udxSap->pst.region, udxSap->pst.pool, udxSduStaReq, sizeof(UdxSduStaReqInfo));
      PJ_FREE(tPjCb, cfgInfo, sizeof(PjCfgInfo));

      RETVALUE(RFAILED);
   }

   
   PjUlUdxSduStaReq(&(udxSap->pst), udxSap->spId,udxSduStaReq); 

   RETVALUE(ret);
}


/**
 * 
 * @details
 * 1. RRC uses this primitive to resume data during Re-establishment and
 * handover  
 *
 * @param[in] pst    - Pointer to post structure.
 * @param[in] spId   - Service provider ID. This is the reference number used
 * by PDCP to identify its SAP.
 * @param[in] sduStaReq - PDCP SDU Status Request Information. 
 *
 * @return ROK
 */
#ifdef ANSI
PUBLIC S16 PjUiCpjDatResumeReq 
(
Pst                   *pst, 
SpId                  spId, 
CpjDatResumeReqInfo   *datResReq
)
#else
PUBLIC S16 PjUiCpjDatResumeReq(pst,spId, *dataResReq)
Pst                   *pst;
SpId                  spId;
CpjDatResumeReqInfo   *datResReq;
#endif
{

   S16               ret;           /* Return Value */
   PjCpjSapCb        *cpjSap;       /* CPJ SAP Control Block */
   PjUdxUlSapCb      *udxSap;       /* UDX SAP Control Block */ 
   PjUlUeCb          *ueCb;
   PjCfgInfo         *cfgInfo;      
   PjCb              *tPjCb;
   UdxDatResumeInfo  *udxDatResumeInfo;
   TRC2(PjUiCpjDatResumeReq);

   tPjCb = PJ_GET_PJCB(pst->dstInst);

   RLOG1(L_DEBUG, "PjUiCpjDatResumeReq(pst, spId(%d))", spId);

   ret = ROK;
   cpjSap = NULLP;

   cpjSap = &(tPjCb->u.ulCb->cpjSap);

   /* Validate SAP ID under ERRORCLS */
   PJ_VALDATE_SAP(tPjCb, spId, cpjSap, ret);
   if (ret != ROK)
   {
      RETVALUE(RFAILED);
   }
   ret = pjDbmFetchUlUeCb(tPjCb, datResReq->ueId,
                                 datResReq->cellId, &ueCb);
   if (ret != ROK)
   {
      RLOG_ARG1(L_ERROR, DBG_CELLID, datResReq->cellId,"UL UeId[%u] not found",
            datResReq->ueId);
      PJ_PST_FREE(pst->region, pst->pool, datResReq, sizeof(CpjDatResumeReqInfo));
      RETVALUE(RFAILED);
   }

    /* Memory leak fix ccpu00135359 */
   udxSap = PJ_GET_UL_UDX_SAP(tPjCb);
   ret = ROK;
   PJ_ALLOC_BUF_SHRABL(udxSap->pst, udxDatResumeInfo, 
      sizeof (UdxDatResumeInfo), ret);
   if(ret != ROK)
   {
      RLOG0(L_FATAL, "Memory Allocation failed.");
      PJ_PST_FREE(pst->region, pst->pool, datResReq, sizeof(CpjDatResumeReqInfo));
      RETVALUE(RFAILED);
   }

   PJ_ALLOC(tPjCb, cfgInfo, sizeof (PjCfgInfo));
   if (cfgInfo == NULLP)
   {
      RLOG0(L_FATAL, "Memory Allocation failed.");
      PJ_PST_FREE(pst->region, pst->pool, datResReq, sizeof(CpjDatResumeReqInfo));
      PJ_PST_FREE(udxSap->pst.region, udxSap->pst.pool, udxDatResumeInfo, sizeof(UdxDatResumeInfo));
      RETVALUE(RFAILED);
   }

   cfgInfo->uprLyrTransId = datResReq->transId;
   cfgInfo->transId = ++tPjCb->u.ulCb->transId; 
   udxDatResumeInfo->transId = cfgInfo->transId;
   cfgInfo->datResReq  = datResReq;
   udxDatResumeInfo->ueId = datResReq->ueId;
   udxDatResumeInfo->cellId = datResReq->cellId;
 
   ret = pjDbmAddUlTransIdLst(tPjCb, cfgInfo);
   if (ret != ROK)
   {
      RLOG0(L_ERROR, "Addition to UL transId List Failed.");
      PJ_PST_FREE(pst->region, pst->pool, datResReq, sizeof(CpjDatResumeReqInfo));
      PJ_FREE(tPjCb, cfgInfo, sizeof(PjCfgInfo));
      PJ_PST_FREE(udxSap->pst.region, udxSap->pst.pool, udxDatResumeInfo, sizeof(UdxDatResumeInfo));

      RETVALUE(RFAILED);
   }
   
   PjUlUdxDatResumeReq(&(udxSap->pst), udxDatResumeInfo); 

   RETVALUE(ret);
}

/**
 * 
 * @details
 * 1. RRC uses this primitive to start data forwarding during
 * handover  
 *
 * @param[in] pst    - Pointer to post structure.
 * @param[in] spId   - Service provider ID. This is the reference number used
 * by PDCP to identify its SAP.
 * @param[in] sduStaReq - PDCP SDU Status Request Information. 
 *
 * @return ROK
 */
#ifdef ANSI
PUBLIC S16 PjUiCpjDatForwardReq 
(
Pst                 *pst, 
SpId                spId, 
CpjDataFwdReqInfo   *dataFwdReq
)
#else
PUBLIC S16 PjUiCpjDatForwardReq(pst,spId, *dataFwdReq)
Pst                 *pst;
SpId                spId;
CpjDataFwdReqInfo   *dataFwdReq;
#endif
{

   S16               ret;           /* Return Value */
   PjCpjSapCb        *cpjSap;       /* CPJ SAP Control Block */
   PjUdxUlSapCb      *udxSap;       /* UDX SAP Control Block */ 
   PjUlUeCb          *ueCb;
   CmLteRnti     ueId;                   /*!< UE ID */
   CmLteCellId   cellId;                 /*!< Cell ID */
   UdxDatForwardInfo  *udxDatForwardInfo;

   PjCb              *tPjCb;
   TRC2(PjUiCpjDatForwardReq);

   tPjCb = PJ_GET_PJCB(pst->dstInst);

   RLOG1(L_DEBUG, "PjUiCpjDatForwardReq(pst, spId(%d))", spId);

   ret = ROK;
   cpjSap = NULLP;

   ueId = dataFwdReq->ueId;
   cellId = dataFwdReq->cellId;
   cpjSap = &(tPjCb->u.ulCb->cpjSap);

   /* Validate SAP ID under ERRORCLS */
   PJ_VALDATE_SAP(tPjCb, spId, cpjSap, ret);
   if (ret != ROK)
   {
      PJ_PST_FREE(pst->region, pst->pool, dataFwdReq, sizeof(CpjDataFwdReqInfo));
      RETVALUE(RFAILED);
   }

   ret = pjDbmFetchUlUeCb(tPjCb, ueId, cellId, &ueCb);
   if (ret != ROK)
   {
      RLOG_ARG1(L_ERROR, DBG_CELLID, cellId,"UL UeId[%u] not found",ueId);
      PJ_PST_FREE(pst->region, pst->pool, dataFwdReq, sizeof(CpjDataFwdReqInfo));
      RETVALUE(RFAILED);
   }
   /* Store the UL Data Forwarding Req or not in ueCb */
   ueCb->isUlDatFwdReqd = dataFwdReq->isUlDatFwdReq;
  
   ret = ROK;
   /* Memory leak fix ccpu00135359 */
   udxSap = PJ_GET_UL_UDX_SAP(tPjCb);
   PJ_ALLOC_BUF_SHRABL(udxSap->pst, udxDatForwardInfo, 
      sizeof (UdxDatForwardInfo), ret);
   if(ret != ROK)
   {
      RLOG0(L_FATAL, "Memory Allocation failed.");
      PJ_PST_FREE(pst->region, pst->pool, dataFwdReq, sizeof(CpjDataFwdReqInfo));
      RETVALUE(RFAILED);
   }

   /* TODO :- Need to add code for handling request */

   /* Forwarding request to DL over UDX interface */
   udxDatForwardInfo->ueId = ueId;
   udxDatForwardInfo->cellId = cellId;
   udxDatForwardInfo->transId = dataFwdReq->transId;
  
   PJ_PST_FREE(pst->region, pst->pool, dataFwdReq, sizeof(CpjDataFwdReqInfo)); 
   if(ueCb->isUlDatFwdReqd == TRUE)
   {
      /* Snd ULFwd - TRUE to DL 
       * So that DL will not send isLastDatInd Flag to DAM */
      udxDatForwardInfo->ulFwding = TRUE;
   }
   PjUlUdxDatForwardReq(&(udxSap->pst), udxDatForwardInfo); 

   RETVALUE(ret);
}


/*****************************************************************************
 *                          PJU INTERFACE
 ****************************************************************************/
/**
 *
 * @brief 
 *
 *        Handler for binding the PDCP upper layer service user with 
 *        the PDCP layer.
 *
 * @b Description:
 *
 *        1. This function is used by PDCP user to request for binding to 
 *        PDCP. @n
 *        2. This function is called by the PJU interface to bind 
 *        PDCP's SAP (identified by spId) with the service user's
 *        SAP (identified by suId). 
 *
 *  @param[in] pst   Post structure  
 *  @param[in] suId  Service user SAP ID 
 *  @param[in] spId  Service provider ID
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 */
#ifdef ANSI
PUBLIC S16 PjUiPjuBndReq 
(
Pst       *pst,    
SuId      suId,   
SpId      spId     
)
#else
PUBLIC S16 PjUiPjuBndReq (pst, suId, spId)
Pst       *pst;    
SuId      suId;    
SpId      spId;    
#endif
{
   S16            ret;           /* Return Value */
   PjPjuSapCb     *pjuSap;       /* SAP Config Block */
   PjCb   *tPjCb;
   
   TRC3(PjUiPjuBndReq)

   if (pst->dstInst >= PJ_MAX_PDCP_INSTANCES)  /* KW_FIX */
   {
      RETVALUE(RFAILED);
   }

   tPjCb = PJ_GET_PJCB(pst->dstInst);
   RLOG2(L_DEBUG, "PjUiPjuBndReq(pst, spId(%d), suId(%d))", spId, suId);

   ret = ROK;
   pjuSap = NULLP;

   /* Validation of input parameters */
#if (ERRCLASS & ERRCLS_INT_PAR)
   if(!((spId < (S16) tPjCb->pjGenCfg.maxPjuSaps) && (spId >=0))) 
   {
      if(pjuSap != NULLP) 
      {
         RLOG0(L_ERROR, "Invalid PJU SAP State in Bind Req");
         PJ_SEND_SAPID_ALARM(tPjCb,spId, LPJ_EVENT_PJU_BND_REQ, LCM_CAUSE_INV_SAP);
         RETVALUE(RFAILED); 
      }
   }
#endif

   /* Get Sap control block */
   if (tPjCb->pjGenCfg.mode == LPJ_MODE_PDCP_UL)
   {
      pjuSap = (PjPjuSapCb*)(tPjCb->u.ulCb->pjuSap + spId);
   }
   else
   {
      pjuSap = (PjPjuSapCb*)(tPjCb->u.dlCb->pjuSap + spId);
   }

   /* Verify PJU SAP State */
   switch(pjuSap->state)
   {
      /* SAP is configured but not bound */
      case PJ_SAP_CFG:
      case PJ_SAP_UBND:
      {
         /* copy bind configuration parameters in SSAP sap */
         pjuSap->suId          = suId;
         pjuSap->pst.dstProcId = pst->srcProcId;
         pjuSap->pst.dstEnt    = pst->srcEnt;
         pjuSap->pst.dstInst   = pst->srcInst;

         /* Update the State */
         pjuSap->state = PJ_SAP_BND;

         /* DEBUG_PRINT */
         RLOG1(L_DEBUG, "PjUiPjuBndReq: state (%d)", pjuSap->state);
         break;
      }

      /* SAP is already bound */
      case PJ_SAP_BND:
      {
         /* 
          * Sap is already bound check source, destination Entity and 
          * Proc Id
          */
         if (pjuSap->pst.dstProcId != pst->srcProcId 
               || pjuSap->pst.dstEnt != pst->srcEnt
               || pjuSap->pst.dstInst != pst->srcInst
               || pjuSap->suId != suId)
         {
            RLOG4(L_DEBUG, "Ui Pju Bnd Req suId(%d)<>suId(%d),"
                  "pst->srcProcId(%d)<>pjuSap->pst.dstProcId(%d)",
                  suId, pjuSap->suId, pst->srcProcId, pjuSap->pst.dstProcId);

            RLOG4(L_DEBUG,"Ui Pju Bnd Req pst->srcEnt(%d)<>pjuSap->pst.dstEnt"
                  "(%d),pst->srcInst(%d)<>pjuSap->pst.dstInst(%d))",
              pst->srcEnt,pjuSap->pst.dstEnt,pst->srcInst,pjuSap->pst.dstInst);

            PJ_SEND_SAPID_ALARM(tPjCb, spId, LPJ_EVENT_PJU_BND_REQ, LCM_CAUSE_INV_PAR_VAL);
            ret = RFAILED;
         }
         break;
      }

     default:
      {
         /* LOG ERROR */
         RLOG0(L_ERROR, "Invalid PJU SAP State in Bind Req");
         PJ_SEND_SAPID_ALARM(tPjCb,spId, LPJ_EVENT_PJU_BND_REQ, LCM_CAUSE_INV_STATE);
         ret = RFAILED;
         break;
      }
   }

   if (ret == ROK)
   {
      PjUiPjuBndCfm(&(pjuSap->pst), pjuSap->suId, CM_BND_OK);
   }
   else
   {
      PjUiPjuBndCfm(&(pjuSap->pst), pjuSap->suId, CM_BND_NOK);
   }

   RETVALUE(ROK);

} /* PjUiPjuBndReq */


/**
 *
 * @brief 
 *
 *        Handler for unbinding the PDCP upper layer service user with 
 *        the PDCP layer.
 *
 * @b Description:
 *
 *        1. This function is used by PDCP user to request for unbinding  
 *        with PDCP. @n
 *        2. This function is called by the PJU interface to 
 *        unbind with PDCP. 
 *
 *  @param[in] pst     Post structure  
 *  @param[in] suId    Service user SAP ID 
 *  @param[in] reason  Reason for Unbinding 
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 *
 */
#ifdef ANSI
PUBLIC S16 PjUiPjuUbndReq
(
Pst      *pst,   
SpId     spId,    
Reason   reason     
)
#else
PUBLIC S16 PjUiPjuUbndReq(pst, spId, reason)
Pst      *pst;       
SpId     spId;     
Reason   reason;      
#endif
{
   PjPjuSapCb  *pjuSap;            /* PJU SAP control block */
#if (ERRCLASS & ERRCLS_INT_PAR)
   S16         ret; 
#endif
   PjCb   *tPjCb;

   TRC3(PjUiPjuUbndReq)

   if (pst->dstInst >= PJ_MAX_PDCP_INSTANCES)      /* KW_FIX */
   {
      RETVALUE(RFAILED);
   }

   tPjCb = PJ_GET_PJCB(pst->dstInst);
   /* DEBUG_PRINT */
   RLOG2(L_DEBUG, "PjUiPjuUbndReq(pst, spId(%d), reason(%d))", spId, reason);

   pjuSap = NULLP;

   UNUSED(pst);
   UNUSED(reason);

   /* Get Sap control block */
   if (tPjCb->pjGenCfg.mode == LPJ_MODE_PDCP_UL)
   {
      pjuSap = (PjPjuSapCb*)(tPjCb->u.ulCb->pjuSap + spId);
   }
   else
   {
      pjuSap = (PjPjuSapCb*)(tPjCb->u.dlCb->pjuSap + spId);
   }


#if (ERRCLASS & ERRCLS_INT_PAR)
   PJ_GET_AND_VALIDATE_PJUSAP(tPjCb, pjuSap, EPJXXX, "PjUiPjuUbndReq", ret);
   if (ret != ROK )
   {
      RETVALUE(RFAILED);
   }
#endif /* ERRCLASS & ERRCLS_INT_PAR */

   pjuSap->state = PJ_SAP_CFG;

   RETVALUE(ROK);
} /* PjUiPjuUbndReq */

/**
 * @details
 * This primitive is used by the PDCP service user to request PDCP
 * for data transfer.
 *
 * @param[in] pst    - Pointer to Pst structure.
 * @param[in] spId  - Service provider ID. This is the reference number
 * used by PDCP service user (Relay) to identify its SAP. This reference number
 * will be provided by PDCP in any further primitives sent by PDCP
 * to the service user.
 * @param[in] pdcpId - Pointer to CmLtePdcpId structure, PDCP id is used to 
 * uniquely identify an PDCP entity. 
 * @param[in] sduId  - SDU ID. 
 * @param[in] mBuf   - Pointer to data buffer.
 *
 * @return ROK
 */
#ifdef ANSI
PUBLIC S16 PjUiPjuDatReq
(
Pst            *pst,            
SpId           spId,               
CmLtePdcpId    *pdcpId,         
PjuSduId       sduId,           
Buffer         *mBuf          
)
#else
PUBLIC S16 PjUiPjuDatReq(pst, spId, pdcpId, sduId, mBuf)
Pst             *pst;       
SpId            spId;           
CmLtePdcpId     *pdcpId;     
PjuSduId         sduId;   
Buffer          *mBuf;          
#endif
{
#ifndef NO_ERRCLS
   S16         ret;           /* Return Value */
#endif
   PjDlRbCb   *pjRbCb;       /* RB Control Block */
   PjDlUeCb   *ueCb;         /* UE Control Block */
#ifndef CCPU_OPT
   MsgLen      len;           /* Size of the SDU */
#endif
#ifndef NO_ERRCLS
   PjPjuSapCb  *pjuSap;       /* SAP Config Block */
#endif
   PjCb   *tPjCb;
   U32   count;         /* count for the PDU */
#ifndef CCPU_OPT
   PjuDatCfmInfo *datCfm;
   CmLtePdcpId *tPdcpId;
   PjPjuSapCb  *tPjuSap;
#endif /*CCPU_OPT */
   TRC3(PjUiPjuDatReq)

#ifdef CA_PHY_BRDCM_61765 
   EXTERN U32 dlrate_pju1;
   MsgLen len;
   SFndLenMsg(mBuf, (MsgLen *) &len);
   dlrate_pju1 += len;
#endif

   if ((pst->dstInst >= PJ_MAX_PDCP_INSTANCES) || (mBuf == NULLP))      /* KW_FIX */
   {
      PJ_PST_FREE(pst->region, pst->pool, pdcpId, sizeof(CmLtePdcpId));
      PJ_FREE_BUF(mBuf);
      RETVALUE(RFAILED);
   }

   tPjCb = PJ_GET_PJCB(pst->dstInst);

#ifndef NO_ERRCLS
   pjuSap = NULLP;
#endif

#ifndef NO_ERRCLS
   ret = ROK;
   pjuSap = (PjPjuSapCb*)(tPjCb->u.dlCb->pjuSap + spId);
   /* Validate SAP ID under ERRORCLS */
   PJ_VALDATE_SAP(tPjCb, spId, pjuSap, ret);
   if (ret != ROK)
   {
      PJ_FREE_BUF(mBuf);
      RETVALUE(RFAILED);
   }
#endif
   /* Fetch the RbCb */
   if (pjDbmFetchDlUeCb(tPjCb, pdcpId->ueId, pdcpId->cellId, &ueCb) != ROK)
   {
      RLOG_ARG1(L_ERROR, DBG_CELLID, pdcpId->cellId,"DL UeId[%u] not found",
            pdcpId->ueId);
      PJ_FREE_BUF(mBuf);
      RETVALUE(RFAILED);
   }
   /* Fetch the RbCb */
   
   PJ_DBM_FETCH_DL_RBCB(pdcpId->rbId, pdcpId->rbType, ueCb,pjRbCb);
   if (pjRbCb == NULLP)
   {     
      RLOG_ARG2(L_ERROR, DBG_UEID,pdcpId->ueId, "CellId[%u]: DL RbId[%d] not found",
            pdcpId->cellId, pdcpId->rbId);
      PJ_FREE_BUF(mBuf);
      RETVALUE(RFAILED);
   }

   /* Update the total packets received for L2 Measurements */
   PJ_UPD_L2_DLDISC_PKT_STS(tPjCb,pjRbCb);
   
#ifdef PTPJLIB 
   /* kw003.201 Control for packet administration */
   if (pjLibPktAdmCtrl(pjRbCb, mBuf) != ROK)
   {
      RLOG_ARG2(L_DEBUG,DBG_UEID,pdcpId->ueId, "Pju Dat Req: PDCP pkt dropped for"
           " cellId(%d),rbId(%d)", pdcpId->cellId, pdcpId->rbId);
      PJ_FREE_BUF(mBuf);
      gPdcpStats.numPdcpSdusDiscarded++;
      PJ_UPD_L2_DLDISC_STS(tPjCb, pjRbCb);
      {
#ifdef CA_PHY_BRDCM_61765
         EXTERN U32 gDbgPdcpDropCnt;
         gDbgPdcpDropCnt++;
#endif
      }
      RETVALUE(RFAILED);
   }
#endif
#ifndef CCPU_OPT
   SFndLenMsg(mBuf, &len);
   if ( len > PJ_MAX_SDU_SIZE )
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      RLOG_ARG2(L_ERROR, DBG_RBID, pjRbCb->rbId , "Processing DL Sdu failed, PDU size [%lu] > MAX_LEN [%lu]",
         len, PJ_MAX_SDU_SIZE);
#endif /* ERRCLASS & ERRCLS_INT_PAR */

      if ( pjRbCb->dlCb.cfmReqd)
      {
         if (pjRbCb->rbType == PJ_SRB)
            tPjuSap   = (PjPjuSapCb*)(tPjCb->pjuSap + PJ_SRB_SAP);
         else
            tPjuSap   = (PjPjuSapCb*)(tPjCb->pjuSap + PJ_DRB_SAP);

    datCfm = &datCfmTmp;
    tpdcpId = &tpdcpIdTmp;

         datCfm->cfmSta[datCfm->numSdus].sduId = sduId;
         datCfm->cfmSta[datCfm->numSdus].status = PJU_MAX_SDU_SIZ_EXCEED;
         datCfm->numSdus++;
         tPdcpId->rbId   = pjRbCb->rbId;
         tPdcpId->rbType = pjRbCb->rbType;
         tPdcpId->ueId   = pjRbCb->ueCb->key.ueId;
         tPdcpId->cellId = pjRbCb->ueCb->key.cellId;
         PjUiPjuDatCfm(&(tPjuSap->pst), tPjuSap->suId, tPdcpId, datCfm);
      }

      tPjCb->pjGenSts.numCmpFails++;

      PJ_FREE_BUF(mBuf);
      RETVALUE(RFAILED);
   }   
#endif

   if((pjRbCb->state != PJ_STATE_NORMAL) && (pjRbCb->rbType != PJ_SRB))
   {
#ifdef PRE_DEF_UE_CTX_HO      
      pjRbCb->state = PJ_STATE_NORMAL;
#endif       
      if(pjRbCb->dlCb.dlPktQ.count < 500)
      {
/*         MSPD_DBG(" Queuing the Packets in Reestablish/Ho state ");*/
         pjDlmEnqueueDlPkt(tPjCb, pjRbCb, PJ_DATA_NRM_PKT, NULL, sduId, mBuf); 
         RETVALUE(ROK);
      }
      else
      {
         PJ_FREE_BUF(mBuf);
         gPdcpStats.numPdcpSdusDiscarded++;
         PJ_UPD_L2_DLDISC_STS(tPjCb, pjRbCb);
         RETVALUE(RFAILED);
      }
   }
   PJ_DLM_UPD_VAR(pjRbCb,count);
   pjDlmProcessSdus(tPjCb, pjRbCb, mBuf, sduId,count);

   /* kw005.201 ccpu00117318, updated statistics */
   RETVALUE(ROK);
} /* PjUiPjuDatReq */


#ifdef FLAT_BUFFER_OPT
/**
 * @details
 * This primitive is used by the PDCP service user to request PDCP
 * for data transfer.
 *
 * @param[in] pst    - Pointer to Pst structure.
 * @param[in] spId  - Service provider ID. This is the reference number
 * used by PDCP service user (Relay) to identify its SAP. This reference number
 * will be provided by PDCP in any further primitives sent by PDCP
 * to the service user.
 * @param[in] pdcpId - Pointer to CmLtePdcpId structure, PDCP id is used to 
 * uniquely identify an PDCP entity. 
 * @param[in] sduId  - SDU ID. 
 * @param[in] mBuf   - Pointer to Flat buffer.
 *
 * @return ROK
 */

#ifdef ANSI
PUBLIC S16 PjUiPjuDatReqFB
(
Pst            *pst,            
SpId           spId,               
CmLtePdcpId    *pdcpId,         
PjuSduId       sduId,           
FlatBuffer         *mBuf          
)
#else
PUBLIC S16 PjUiPjuDatReqFB(pst, spId, pdcpId, sduId, mBuf)
Pst             *pst;       
SpId            spId;           
CmLtePdcpId     *pdcpId;     
PjuSduId         sduId;   
FlatBuffer          *mBuf;          
#endif
{
   PjDlRbCb   *pjRbCb;       /* RB Control Block */
   PjDlUeCb   *ueCb;         /* UE Control Block */
#ifndef CCPU_OPT
   MsgLen      len;          /* Size of the SDU */
#endif
#ifndef NO_ERRCLS
   PjPjuSapCb     *pjuSap;       /* SAP Config Block */
#endif
   PjCb           *tPjCb = NULLP;/* KW_FIX */
   PjTxEnt        *txEnt=NULLP;
#ifndef NO_ERRCLS
   S16         ret;          /* Return Value */
   PjUeKey  key;
#endif

#ifdef L2_PDCP_OPTMZ   
   U8       hashKey;          /* Computed HashKey */
   PjBuf       *buf; 
#endif 
   TRC3(PjUiPjuDatReqFB)

   if ((pst->dstInst >= PJ_MAX_PDCP_INSTANCES) || (mBuf == NULLP))      /* KW_FIX */
   {
      /* PJ_FREE_FLAT_BUF(tPjCb, mBuf); */
      RETVALUE(RFAILED);
   }

   tPjCb = PJ_GET_PJCB(pst->dstInst);

#ifndef NO_ERRCLS
   ret = ROK;
   pjuSap = NULLP;
#endif

#ifdef DEBUGP
   /* DEBUG_PRINT */
   RLOG1(L_DEBUG, "PjUiPjuDatReq(pst, spId(%d))", spId);
#endif

#ifndef NO_ERRCLS
   pjuSap = (PjPjuSapCb*)(tPjCb->u.dlCb->pjuSap + spId);
   /* Validate SAP ID under ERRORCLS */
   PJ_VALDATE_SAP(tPjCb, spId, pjuSap, ret);
   if (ret != ROK)
   {
      PJ_FREE_FLAT_BUF(tPjCb, mBuf);
      RETVALUE(RFAILED);
   }
#endif
   /* Fetch the UeCb */
   if (pjDbmFetchDlUeCb(tPjCb, pdcpId->ueId, pdcpId->cellId, &ueCb) != ROK)
   {
      RLOG_ARG1(L_ERROR, DBG_CELLID, pdcpId->cellId,"DL UeId[%u] not found",
            pdcpId->ueId);
      PJ_FREE_FLAT_BUF(tPjCb, mBuf);
      RETVALUE(RFAILED);
   }
   /* Fetch the RbCb */
   PJ_DBM_FETCH_DL_RBCB(pdcpId->rbId, pdcpId->rbType, ueCb,pjRbCb);
   
   if (pjRbCb == NULLP)
   {     
      RLOG_ARG2(L_ERROR, DBG_UEID,pdcpId->ueId, "CellId[%u]: DL RbId[%d] not found",
            pdcpId->cellId, pdcpId->rbId);
      PJ_FREE_FLAT_BUF(tPjCb, mBuf);
      RETVALUE(RFAILED);
   }

   /* Update the total packets received for L2 Measurements */
   PJ_UPD_L2_DLDISC_PKT_STS(tPjCb,pjRbCb);

#ifdef PTPJLIB 
   /* kw003.201 Control for packet administration */
   
   if (pjLibPktAdmCtrl(pjRbCb, NULL) != ROK)
   {
      RLOG_ARG2(L_DEBUG,DBG_UEID,pdcpId->ueId,"PDCP pkt Q is Full & pkt dropped"
            " for cellId(%d), rbId(%d)", pdcpId->cellId, pdcpId->rbId);
      PJ_FREE_FLAT_BUF(tPjCb, mBuf);
      gPdcpStats.numPdcpSdusDiscarded++;
      PJ_UPD_L2_DLDISC_STS(tPjCb, pjRbCb);
      RETVALUE(RFAILED);
   }
#endif

   if((ueCb->libInfo.state != PJ_STATE_NORMAL) && (pjRbCb->rbType != PJ_SRB))
   {
      if(pjRbCb->dlCb.dlPktQ.count < 500)
      {
         pjDlmEnqueueDlPktFB(tPjCb, pjRbCb, PJ_DATA_NRM_PKT, NULL, sduId, mBuf); 
         RETVALUE(ROK);
      }
      else
      {
         PJ_FREE_FLAT_BUF(tPjCb, mBuf);
         gPdcpStats.numPdcpSdusDiscarded++;
         PJ_UPD_L2_DLDISC_STS(tPjCb, pjRbCb);
         RETVALUE(RFAILED);
      }
   }

   PJ_ALLOC(tPjCb, txEnt, sizeof(PjTxEnt));
#if (ERRCLASS & ERRCLS_ADD_RES)
   if (txEnt == NULLP)
   {
      RLOG0(L_FATAL, "Memory Allocation failed.");
      PJ_FREE_FLAT_BUF(gCb,sdu);
      RETVALUE(RFAILED);
   }
#endif /* ERRCLASS & ERRCLS_RES */ 
   PJ_DLM_UPD_VAR(pjRbCb,txEnt->count);     
   PJ_DLM_GET_SN(pjRbCb,txEnt->count,txEnt->sn);
   pjRbCb->dlCb.count = txEnt->count; 
   txEnt->state = PJ_SDU_RECEIVED;
   txEnt->sduId = sduId;
   txEnt->fb   = *mBuf;
   txEnt->pdu   = NULLP;
   txEnt->sdu   = NULLP;
   /* Insert TxEnt into the transmission buffer */
#ifndef L2_PDCP_OPTMZ
   pjDbmInsTxEnt(tPjCb, &(pjRbCb->dlCb.txBuf), txEnt); 
   /* kw005.201 added support for L2 Measurement */
#else
   if (pjRbCb->ueCb->secInfo.secAct &&  pjRbCb->ueCb->secInfo.cipherInfo.algoType != 0)   /* If its not NULL Ciphering and Security is enabled */
   {
   txEnt->datCfmEnt.node = NULLP;
   txEnt->reEstPktEnt.node = NULLP;
   txEnt->sduSubmitEnt.node = (PTR)NULLP;

   buf=&(pjRbCb->dlCb.txBuf);
   hashKey = (U8)PJ_HASH_FN(buf, txEnt->count); /*KW_FIX*/

   txEnt->lstEnt.node = (PTR)txEnt;
   cmLListAdd2Tail(&buf->datQ[hashKey], &txEnt->lstEnt);
   buf->numEntries ++;
   /* No Need to add TxEnt in DataCfm Quee as no Cfm from RLC */
   if (pjRbCb->mode == PJ_DRB_AM) 
   {
      txEnt->datCfmEnt.node = (PTR)txEnt;
      cmLListAdd2Tail(&pjRbCb->dlCb.txBuf.datCfmQ, &txEnt->datCfmEnt);
   }
   txEnt->state  = PJ_PDU_SUBMITTED;
   }
   else 
   {
   pjDbmInsTxEnt(tPjCb, &(pjRbCb->dlCb.txBuf), txEnt); 
   }
#endif 

#ifndef RGL_SPECIFIC_CHANGES
#ifdef MSPD
   {
      extern U32 dlrate_pju;
      dlrate_pju += mBuf->len;
   }
#endif
#endif
   pjDlmHdlCmp(tPjCb, pjRbCb, txEnt);
   /*pjDlmProcessSdusFB(tPjCb, pjRbCb, mBuf, sduId, sn, txHfn);*/

   /* kw005.201 ccpu00117318, updated statistics */
   RETVALUE(ROK);
} /* PjUiPjuDatReqFB */
#endif

/**
 * 
 * @details
 * This primitive is used by PDCP user to forward the SDUs along with its SN
 * as part of handover (SDUs forwarded from PDCP on source eNB via X2-U 
 * Interface to the PDCP on target eNB)
 *
 * @param[in] pst    - Pointer to Pst structure.
 * @param[in] spId   - Service provider ID. This is the reference number
 * used by PDCP service user (Relay) to identify its SAP. This reference number
 * will be provided by PDCP in any further primitives sent by PDCP
 * to the service user.
 * @param[in] pdcpId - Pointer to CmLtePdcpId structure, PDCP id is used to 
 * uniquely identify a PDCP entity. 
 * @param[in] datFwdReq - Pointer to data forward information structure.
 *
 *  @return ROK.
 */
#ifdef ANSI
PUBLIC S16 PjUiPjuDatFwdReq 
(
Pst              *pst, 
SpId             spId, 
CmLtePdcpId      *pdcpId, 
PjuDatFwdReqInfo *datFwdReq
)
#else
PUBLIC S16 PjUiPjuDatFwdReq(pst,spId,pdcpId,datFwdReq)
Pst              *pst;
SpId             spId;
CmLtePdcpId      *pdcpId;
PjuDatFwdReqInfo *datFwdReq;
#endif
{

   S16         ret;           /* Return Value */
   PjPjuSapCb  *pjuSap;       /* SAP Config Block */
   PjCb   *tPjCb;

   TRC3(PjUiPjuDatFwdReq)
   /* ccpu00137221 : KW Fix */
   if ( NULLP == datFwdReq )
   {
      /* Log an error */
#if (ERRCLASS & ERRCLS_INT_PAR)
      if (pst->dstInst < PJ_MAX_PDCP_INSTANCES)
      {
         tPjCb = PJ_GET_PJCB(pst->dstInst);
         if (tPjCb != NULLP)
         {
            RLOG0(L_ERROR, "Hndl DatFwdReq failed ,tPjCb is NULLP");
         }
      }
#endif /* ERRCLASS & ERRCLS_INT_PAR */
      RETVALUE(RFAILED);
   }
   if (pst->dstInst >= PJ_MAX_PDCP_INSTANCES)
   {
      PJ_PST_FREE(pst->region, pst->pool, datFwdReq->datFwdInfo, 
                 (datFwdReq->numSdus * sizeof(PjuDatFwdInfo)));
      PJ_PST_FREE(pst->region, pst->pool, datFwdReq, sizeof(PjuDatFwdReqInfo));
      RETVALUE(RFAILED);
   }
   tPjCb = PJ_GET_PJCB(pst->dstInst);

   ret = ROK;
   pjuSap = NULLP;
 
   RLOG1(L_DEBUG, "PjUiPjuDatFwdReq(pst, spId(%d))", spId);

   if (( datFwdReq->dir == PJ_DIR_DL )  &&
       (tPjCb->pjGenCfg.mode == LPJ_MODE_PDCP_DL))
     
   {
      PjDlUeCb      *dlUeCb;         /* UE Control Block */
      PjDlRbCb      *dlRbCb;       /* RB Control Block */
      pjuSap = (PjPjuSapCb*)(tPjCb->u.dlCb->pjuSap + spId);
      /* Validate SAP ID under ERRORCLS */
      PJ_VALDATE_SAP(tPjCb, spId, pjuSap, ret);
      if (ret != ROK)
      {
         /* Freeing from proper region */
         PJ_PST_FREE(pst->region, pst->pool, datFwdReq->datFwdInfo, 
                    (datFwdReq->numSdus * sizeof(PjuDatFwdInfo)));
         PJ_PST_FREE(pst->region, pst->pool, datFwdReq, sizeof(PjuDatFwdReqInfo));
         RETVALUE(RFAILED);
      }

      if (pjDbmFetchDlUeCb(tPjCb, pdcpId->ueId, pdcpId->cellId, &dlUeCb) != ROK)
      {
         RLOG_ARG1(L_ERROR, DBG_CELLID, pdcpId->cellId,"DL UeId[%u] not found",
            pdcpId->ueId);
         /* Freeing from proper region */
         PJ_PST_FREE(pst->region, pst->pool, datFwdReq->datFwdInfo, 
                    (datFwdReq->numSdus * sizeof(PjuDatFwdInfo)));
         PJ_PST_FREE(pst->region, pst->pool, datFwdReq, sizeof(PjuDatFwdReqInfo));
         RETVALUE(RFAILED);
      }
      /* Fetch the RbCb */
      PJ_DBM_FETCH_DL_RBCB(pdcpId->rbId, pdcpId->rbType, dlUeCb,dlRbCb);
      if (dlRbCb == NULLP) 
      {
         RLOG_ARG2(L_ERROR, DBG_UEID,pdcpId->ueId, "CellId[%u]: DL RbId[%d] not found",
            pdcpId->cellId, pdcpId->rbId);
         /* Freeing from proper region */
         PJ_PST_FREE(pst->region, pst->pool, datFwdReq->datFwdInfo, 
                    (datFwdReq->numSdus * sizeof(PjuDatFwdInfo)));
         PJ_PST_FREE(pst->region, pst->pool, datFwdReq, sizeof(PjuDatFwdReqInfo));

         RETVALUE(RFAILED);
      }
     
      if (dlRbCb->state == PJ_STATE_HO)
      {
          pjDlmEnqueueDlPkt(tPjCb,dlRbCb, PJ_DATA_FWD_PKT, datFwdReq, 0, NULLP);
      }
      else
      {
        ret = pjDlmHndlDatFwdReq(tPjCb, dlRbCb, datFwdReq); 
      }

      PJ_PST_FREE(pst->region, pst->pool, datFwdReq->datFwdInfo, 
                 (datFwdReq->numSdus * sizeof(PjuDatFwdInfo)));
      PJ_PST_FREE(pst->region, pst->pool, datFwdReq, sizeof(PjuDatFwdReqInfo));

   }
   else  if (( datFwdReq->dir == PJ_DIR_UL )  &&
             (tPjCb->pjGenCfg.mode == LPJ_MODE_PDCP_UL))
   {
      PjUlRbCb      *ulRbCb;       /* RB Control Block */
      PjUlUeCb      *ulUeCb;
      pjuSap = (PjPjuSapCb*)(tPjCb->u.ulCb->pjuSap + spId);
      /* Validate SAP ID under ERRORCLS */
      pjuSap->state = PJ_SAP_BND;
      PJ_VALDATE_SAP(tPjCb, spId, pjuSap, ret);
      if (ret != ROK)
      {
         /* Freeing from proper region */
         PJ_PST_FREE(pst->region, pst->pool, datFwdReq->datFwdInfo, 
                    (datFwdReq->numSdus * sizeof(PjuDatFwdInfo)));
         PJ_PST_FREE(pst->region, pst->pool, datFwdReq, sizeof(PjuDatFwdReqInfo));
         RETVALUE(RFAILED);
      }

      if (pjDbmFetchUlUeCb(tPjCb, pdcpId->ueId, pdcpId->cellId, &ulUeCb) != ROK)
      {
         RLOG_ARG1(L_ERROR, DBG_CELLID, pdcpId->cellId,"UL UeId[%u] not found",
            pdcpId->ueId);
         /* Freeing from proper region */
         PJ_PST_FREE(pst->region, pst->pool, datFwdReq->datFwdInfo, 
                    (datFwdReq->numSdus * sizeof(PjuDatFwdInfo)));
         PJ_PST_FREE(pst->region, pst->pool, datFwdReq, sizeof(PjuDatFwdReqInfo));
         RETVALUE(RFAILED);
      }
      /* Fetch the RbCb */
      PJ_DBM_FETCH_UL_RBCB(pdcpId->rbId, pdcpId->rbType, ulUeCb,ulRbCb);
      if (ulRbCb == NULLP)
      {
         RLOG_ARG2(L_ERROR, DBG_UEID,pdcpId->ueId, "CellId[%u]: UL RbId[%d] not found",
            pdcpId->cellId, pdcpId->rbId);
         /* Freeing from proper region */
         PJ_PST_FREE(pst->region, pst->pool, datFwdReq->datFwdInfo, 
                    (datFwdReq->numSdus * sizeof(PjuDatFwdInfo)));
         PJ_PST_FREE(pst->region, pst->pool, datFwdReq, sizeof(PjuDatFwdReqInfo));

         RETVALUE(RFAILED);
      }
     
      ret = pjUlmHdlDatFwdReq(tPjCb, ulRbCb, datFwdReq);
      PJ_PST_FREE(pst->region, pst->pool, datFwdReq->datFwdInfo, 
                   (datFwdReq->numSdus * sizeof(PjuDatFwdInfo)));
      PJ_PST_FREE(pst->region, pst->pool, datFwdReq, sizeof(PjuDatFwdReqInfo));
   }
  
   /* Freeing from proper region */

   if ( ret != ROK )
   {
        /* Log an error */
#if (ERRCLASS & ERRCLS_INT_PAR)
      RLOG0(L_ERROR, "pjDlm/pjUlm HndlDatFwdReq failed");
#endif /* ERRCLASS & ERRCLS_INT_PAR */

   }
   RETVALUE(ret);
}


/**
 * @brief Handler for Bind request from RRM towards CL.
 *
 * @details
 *
 *     Function : YsUiCpjKdfReq
 *     
 *     This function handles the bind request from RRM.
 *     
 *           
 *  @param[in]  Pst  *pst
 *  @param[in]  SuId suId
 *  @param[in]  SpId spId
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 PjUiCpjKdfReq
(
Pst  *pst,
SpId spId,
CpjCfgTransId        transId,
CpjKdfReqInfo  *kdfReqInfo
)
#else
PUBLIC S16 PjUiCpjKdfReq(pst, suId, spId)
Pst  *pst;
SuId suId;
SpId spId;
#endif
{
   S16           ret;
   CpjKdfCfmInfo *kdfCfmInf;
   PjCb          *tPjCb;
   Pst           cfmPst;
   PjCpjSapCb    *cpjSap;


   TRC2(PjUiCpjKdfReq)

   ret = ROK;

   if (pst->dstInst >= PJ_MAX_PDCP_INSTANCES)      /* KW_FIX */
   {
      RETVALUE(RFAILED);
   }
   tPjCb = PJ_GET_PJCB(pst->dstInst);

   /* Memory leak fix ccpu00135359 */
   cpjSap = &(tPjCb->u.ulCb->cpjSap);
   if(SGetSBuf(cpjSap->pst.region,cpjSap->pst.pool,(Data **)&kdfCfmInf, 
      sizeof (CpjKdfCfmInfo)) != ROK)
   {
      RLOG0(L_FATAL, "Memory Allocation failed.");
      RETVALUE(RFAILED);
   }

   cfmPst.dstInst = pst->srcInst;
   cfmPst.dstEnt  = pst->srcEnt;
   cfmPst.dstProcId = pst->srcProcId;
   cfmPst.srcInst = pst->dstInst;
   cfmPst.srcEnt = pst->dstEnt;
   cfmPst.srcProcId = pst->dstProcId;
   cfmPst.selector = 0;
   cfmPst.region = tPjCb->init.region;
   cfmPst.pool = tPjCb->init.pool;
   cfmPst.prior = PRIOR0;
   cfmPst.route = RTESPEC;

   RLOG1(L_DEBUG, "Key derivation request for Key type %d",
            kdfReqInfo->keyDerType);

   switch(kdfReqInfo->keyDerType)
   {
      case CPJ_KDF_TYPE1:
         kdfCfmInf->keyDerType = CPJ_KDF_TYPE1;
         ret = pjLibObdPrcKenbCfg(tPjCb, &(kdfReqInfo->u.kEnbInf), 
               &(kdfCfmInf->u.kEnbInf));
         break;

      case CPJ_KDF_TYPE2:
         kdfCfmInf->keyDerType = CPJ_KDF_TYPE2;
         ret = pjLibObdPrcKenbStarCfg(tPjCb, &(kdfReqInfo->u.kEnbStarInf),
               &(kdfCfmInf->u.kEnbStarInf));
         break;

      case CPJ_KDF_TYPE3:
         break;
      default:
         break;
   }
   /* Send Cfg confirm */
   if(ret != ROK)
   {
      PjUiCpjKdfCfm(&cfmPst, PJ_KDF_SAP,
            transId, kdfCfmInf, CPJ_CFG_CFM_NOK);
   }// Sudhanshu WR Change
   else
   {
      PjUiCpjKdfCfm(&cfmPst, PJ_KDF_SAP,
            transId, kdfCfmInf, CPJ_CFG_CFM_OK);
   }


   SPutSBuf (pst->region, pst->pool, (Data *)kdfReqInfo,
         sizeof(CpjKdfReqInfo));
   kdfReqInfo = NULLP;

   RETVALUE(ret);

}  /* PjUiCpjKdfReq */


#ifdef __cplusplus
}
#endif /* __cplusplus */
/********************************************************************30**
         End of file
**********************************************************************/
