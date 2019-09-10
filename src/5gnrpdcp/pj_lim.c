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
  
     Name:    LTE-PDCP Layer - Lower Interface Functions
    
     Type:    C file
  
     Desc:     Source code for PDCP Lower Interface Module.
               This file contains following functions
      
                    --pjLimDatInd
                    --pjLimDatCfm
                    --pjLimStaInd

     File:     pj_lim.c

**********************************************************************/
     
static const char* RLOG_MODULE_NAME="PDCP";
static int RLOG_MODULE_ID=1024;
static int RLOG_FILE_ID=219;
/** @file pj_lim.c
@brief PDCP Lower Interface Module
*/


/* header (.h) include files */
#include "envopt.h"             /* environment options */
#include "envdep.h"             /* environment dependent */
#include "envind.h"             /* environment independent */

#include "gen.h"                /* general */
#include "ssi.h"                /* system services interface */
#include "cm5.h"                /* Timer Functions */
#include "cm_lte.h"             /* common LTE header file */
#include "cm_hash.h"            /* common hash module  file */
#include "cm_llist.h"           /* common list header file */
#include "cpj.h"                /* RRC layer */
#include "kwu.h"
#include "pju.h"                /* PDCP service user */
#include "lpj.h"                /* RRC layer */
#include "pj_env.h"             /* RLC environment options */
#include "pj.h"                 /* RLC layer */
#include "pj_err.h"

/* header/extern include files (.x) */

#include "gen.x"                /* general */
#include "ssi.x"                /* system services interface */
#include "cm_lib.x"             /* common library */
#include "cm5.x"                /* Timer Functions */
#include "cm_hash.x"            /* common hash module */
#include "cm_lte.x"             /* common LTE file */
#include "cm_llist.x"           /* common list header file */
#include "kwu.x"
#include "cpj.x"                /* RRC layer */
#include "pju.x"                /* PDCP service user */
#include "lpj.x"                /* LM Interface */
#include "pj.x"                 /* RLC layer */
#include "pj_udx.h"
#include "pj_udx.x"
#include "pj_ul.x" 
#include "pj_dl.x" 

#ifdef __cplusplus
EXTERN "C" {
#endif /* __cplusplus */
PRIVATE S16 pjLimDiscSduCfm ARGS((PjCb *gCb, 
                                  PjDlRbCb *pjRbCb, 
                                  KwuDiscSduInfo *discSduCfm));

/**
 *
 * @brief Handler for Sending sdu to Rlc.
 *       
 *
 * @b Description
 *
 *
 *  @param[in] pjRbCb      PDCP control block.
 *  @param[in] sduId         PDCP SDU id.
 *  @param[in] mBuf        Pointer to PDCP Sdu
 *
 * 
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 */

#ifdef ANSI
PUBLIC S16 pjDlmSendDatReq
(
PjCb       *gCb,
PjDlRbCb   *pjRbCb,
U32        sduId,
Buffer     *mBuf
)
#else
PUBLIC S16 pjDlmSendDatReq(gCb,pjRbCb,sduId, mBuf)
PjCb       *gCb;
PjDlRbCb   *pjRbCb;
U32        sduId;
Buffer     *mBuf;
#endif
{
   PjKwuSapCb     *pjSapCb;
   KwuDatReqInfo datReq;
   TRC2(pjDlmSendDatReq);

   pjSapCb = gCb->u.dlCb->kwuSap;
   datReq.rlcId.rbId = pjRbCb->rbId;
   datReq.rlcId.rbType = pjRbCb->rbType;
   datReq.rlcId.ueId   = pjRbCb->ueCb->key.ueId;
   datReq.rlcId.cellId = pjRbCb->ueCb->key.cellId;
#ifdef CCPU_OPT
   datReq.lcType = (U8)((pjRbCb->rbType == CM_LTE_SRB) ?
                     CM_LTE_LCH_DCCH : CM_LTE_LCH_DTCH);  /*KW_FIX*/
#endif

   datReq.sduId  = sduId;

   RETVALUE(PjLiKwuDatReq(&(pjSapCb->pst),pjSapCb->spId, &datReq, mBuf));
}

/**
 *
 * @brief Handler for Discarding SDU  to Rlc.
 *       
 *
 * @b Description
 *
 *
 *  @param[in] pjRbCb      PDCP control block.
 *  @param[in] sduId         PDCP SDU id.
 *
 * 
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 */

#ifdef ANSI
PUBLIC S16 pjDlmSendDiscSdu
(
PjCb    *gCb,
PjDlRbCb  *pjRbCb,
U32      sduId
)
#else
PUBLIC S16 pjDlmSendDiscSdu(gCb,pjRbCb,sduId)
PjCb    *gCb;
PjDlRbCb  *pjRbCb;
U32      sduId;
#endif
{
   KwuDiscSduInfo  *discSdu;
   PjKwuSapCb     *pjSapCb;
   TRC2(pjDlmSendDiscSdu);

   pjSapCb = gCb->u.dlCb->kwuSap;
   KwuDiscSduInfo discSduTmp;
   discSdu = &discSduTmp;

   discSdu->rlcId.rbId = pjRbCb->rbId;
   discSdu->rlcId.rbType = pjRbCb->rbType;
   discSdu->rlcId.ueId   = pjRbCb->ueCb->key.ueId;
   discSdu->rlcId.cellId = pjRbCb->ueCb->key.cellId; 
  
   discSdu->sduIds[0]  = sduId;
   discSdu->numSduIds  = 1;

   PjLiKwuDiscSduReq(&(pjSapCb->pst),pjSapCb->spId, discSdu);

   RETVALUE(ROK);
}



/*****************************************************************************
 *                          PDCP LOWER INTERFACE
 ****************************************************************************/
/**
 *
 * @brief 
 *
 *        Handler for getting the BndCfm from RLC.
 *
 * @b Description:
 *
 *        This function receives the PDU from RLC 
 *        and invokes the handler for processing the PDU.
 *
 *  @param[in] pjRbCb    -   PDCP RB Control Block
 *  @param[in] mBuf    -   PDU
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 *
 */
#ifdef ANSI
PUBLIC S16 PjLiKwuBndCfm
(
Pst           *pst,       /* Post */
SuId          suId,       /* Service User Id */
U8            status     /* Status */
)
#else
PUBLIC Void PjLiKwuBndCfm (pst, suId, datCfm)
Pst           *pst;        /* Post */
SuId          suId;        /* Service User Id */
U8          status;     /* Status */
#endif
{
   U16             event;              /* Event */
   U16             cause;              /* Cause */
   PjKwuSapCb      *kwuSap;            /* RGU SAP Control Block */ 
   PjCb   *tPjCb;
   
   TRC1(PjLiKwuBndCfm);

   if (pst->dstInst >= PJ_MAX_PDCP_INSTANCES)     /* KW_FIX */
   {
       RETVALUE(RFAILED);
   }
   tPjCb = PJ_GET_PJCB(pst->dstInst);

   RLOG2(L_DEBUG, "PjLiKwuBndCfm(SuId (%d), status (%d) ",suId, status);  
   UNUSED(pst);

#if (ERRCLASS & ERRCLS_INT_PAR)
   if (!(tPjCb->init.cfgDone & PJ_LMM_GENCFG_DONE))
   {
      /* INNC: ERROR CLASS */
      RLOG0(L_FATAL, "General configuration not done");
      /*PJ_SEND_SAPID_ALARM(tPjCb,suId, LPJ_EVENT_LI_BND_CFM, LPJ_CAUSE_INV_STATE);*/

      RETVALUE(RFAILED);
   }

   if (suId < 0)
   {
      /* INNC: ERROR CLASS */
      RLOG0(L_ERROR, "Invalid suId");
      /*PJ_SEND_SAPID_ALARM(tPjCb,suId, LPJ_EVENT_LI_BND_CFM, LPJ_CAUSE_INV_SUID);*/

      RETVALUE(RFAILED);
   }
#endif /* ERRCLASS & ERRCLS_INT_PAR */

   if (tPjCb->pjGenCfg.mode == LPJ_MODE_PDCP_UL)
   {
      kwuSap = tPjCb->u.ulCb->kwuSap;
   }
   else
   {
      kwuSap = tPjCb->u.dlCb->kwuSap;
   }

#ifdef DEBUGP
   RLOG1(L_DEBUG, "PjLiKwuBndCfm: For KWU SAP state=%d",
                  kwuSap->state);
#endif

   /* Check rguSap state */
   switch (kwuSap->state)
   {
      case PJ_SAP_BINDING:
      {
         pjStopTmr (tPjCb, (PTR)kwuSap, PJ_EVT_WAIT_KWU_BNDCFM);

         kwuSap->retryCnt = 0;
          
         if (status == CM_BND_OK)
         {
            kwuSap->state = PJ_SAP_BND;
            event = LCM_EVENT_BND_OK;
            cause = LPJ_CAUSE_SAP_BNDENB;
         }
         else
         {
            kwuSap->state = PJ_SAP_CFG;
            event = LCM_EVENT_BND_FAIL;
            cause = LPJ_CAUSE_UNKNOWN;
         }
      }
      break;
     default:
        event = LPJ_EVENT_RGU_BND_CFM;
        cause = LCM_CAUSE_INV_STATE;
        break;
   }

   /* Send an alarm with proper event and cause */
 /*  PJ_SEND_SAPID_ALARM(tPjCb, suId, event, cause);*/
#ifdef LTE_L2_MEAS
   pjLmmSendAlarm(tPjCb,LCM_CATEGORY_INTERFACE, event, cause, suId, 0,0);
#else
   pjLmmSendAlarm(tPjCb,LCM_CATEGORY_INTERFACE, event, cause, suId, 0);
#endif

   RETVALUE(ROK);
} /* pjLimDatInd */

/**
 *
 * @brief 
 *
 *        Handler for getting the PDU from RLC.
 *
 * @b Description:
 *
 *        This function receives the PDU from RLC 
 *        and invokes the handler for processing the PDU.
 *
 *  @param[in] pjRbCb    -   PDCP RB Control Block
 *  @param[in] mBuf    -   PDU
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 *
 */
#ifdef ANSI
PUBLIC S16 PjLiKwuDatInd
(
Pst           *pst,        /* Post */
SuId          suId,        /* Service User Id */
KwuDatIndInfo *datInd,     /* Data Indication Info */
Buffer        *mBuf        /* Buffer */
)
#else
PUBLIC Void PjLiKwuDatInd(pst, suId, datInd, mBuf)
Pst           *pst;        /* Post */
SuId          suId;        /* Service User Id */
KwuDatIndInfo *datInd;     /* Data Indication Info */
Buffer        *mBuf;       /* Buffer */
#endif
{
   S16         ret;           /* Return Value */
   PjUlRbCb     *pjRbCb;
   PjCb   *tPjCb;

   TRC1(PjLiKwuDatInd);

   if (pst->dstInst >= PJ_MAX_PDCP_INSTANCES)     /* KW_FIX*/
   {
       RETVALUE(RFAILED);
   }
   tPjCb = PJ_GET_PJCB(pst->dstInst);

   ret = pjDbmFetchUlRbCb (tPjCb,datInd->rlcId,&pjRbCb);
   if (ret != ROK)
   {
      RLOG_ARG2(L_ERROR, DBG_UEID,datInd->rlcId.ueId, "CellId[%u]:UL RbId[%d] not found",
            datInd->rlcId.cellId, datInd->rlcId.rbId);
      PJ_FREE_BUF(mBuf);
      RETVALUE(RFAILED);
   }
  
   pjLimDatInd(tPjCb, pjRbCb, mBuf, datInd->isOutOfSeq);  


   RETVALUE(ROK);
} /* pjLimDatInd */

   PUBLIC int pdcpDatCfmsReceived = 0;
/**
 *
 * @brief 
 *
 *        Handler for getting the datCfm from RLC.
 *
 * @b Description:
 *
 *        This function receives the PDU from RLC 
 *        and invokes the handler for processing the PDU.
 *
 *  @param[in] pjRbCb    -   PDCP RB Control Block
 *  @param[in] mBuf    -   PDU
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 *
 */
#ifdef ANSI
PUBLIC S16 PjLiKwuDatCfm
(
Pst           *pst,       /* Post */
SuId          suId,       /* Service User Id */
KwuDatCfmInfo *datCfm     /* Data Confirm Info */
)
#else
PUBLIC Void PjLiKwuDatCfm(pst, suId, datCfm)
Pst           *pst;        /* Post */
SuId          suId;        /* Service User Id */
KwuDatCfmInfo *datCfm;     /* Data Confirm Info */
#endif
{
   S16         ret;           /* Return Value */
   PjDlRbCb     *pjRbCb;      
   PjDatCfm   pjCfm;
   PjCb   *tPjCb;

   TRC1(PjLiKwuDatCfm);

   if (pst->dstInst >= PJ_MAX_PDCP_INSTANCES)    /* KW_FIX */
   {
       RETVALUE(RFAILED);
   }

   tPjCb = PJ_GET_PJCB(pst->dstInst);

   RLOG2(L_DEBUG, "PjLiKwuDatCfm(rbId(%d),rbType (%d), mBuf)",
                    datCfm->rlcId.rbId,datCfm->rlcId.rbType);

   ret = pjDbmFetchDlRbCb (tPjCb,datCfm->rlcId,&pjRbCb);
   if (ret != ROK)
   {
      RLOG_ARG2(L_ERROR, DBG_UEID,datCfm->rlcId.ueId, "CellId[%u]:DL RbId[%d] not found",
            datCfm->rlcId.cellId, datCfm->rlcId.rbId);
      PJ_FREE_SHRABL_BUF_PST(pst->region, pst->pool, datCfm, sizeof(KwuDatCfmInfo));
      RETVALUE(RFAILED);
   }

   pdcpDatCfmsReceived++; 

   pjCfm.rlcId = datCfm->rlcId;
   pjCfm.numSdu = (U16)(datCfm->numSduIds); /*KW_FIX*/
   cmMemcpy((U8*)pjCfm.sduId, (U8*)datCfm->sduIds, sizeof(U32)*datCfm->numSduIds);
   pjLimDatCfm (tPjCb, pjRbCb, &pjCfm);

   PJ_FREE_SHRABL_BUF_PST(pst->region, pst->pool, datCfm, sizeof(KwuDatCfmInfo));
   RETVALUE(ROK);
} /* pjLimDatInd */

/**
 *
 * @brief 
 *
 *        Handler for flow control indication from RLC.
 *
 * @b Description:
 *
 *        TODO
 *
 *  @param[in] pst           - Pointer to post structure
 *  @param[in] suId          - Service user Id
 *  @param[in] flowCntrlInfo - Flow control information
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 *
 */
#ifdef ANSI
PUBLIC S16 PjLiKwuFlowCntrlInd
(
Pst           *pst,       
SuId          suId,       
KwuFlowCntrlIndInfo *flowCntrlInfo
)
#else
PUBLIC Void PjLiKwuFlowCntrlInd(pst, suId, flowCntrlInfo)
Pst           *pst;       
SuId          suId;       
KwuFlowCntrlIndInfo *flowCntrlInfo;
#endif
{
   S16        ret;       
   PjDlRbCb   *pjRbCb;      
   PjCb       *tPjCb;

   TRC1(PjLiKwuFlowCntrlInd);

   if (pst->dstInst >= PJ_MAX_PDCP_INSTANCES)
   {
       RETVALUE(RFAILED);
   }

   tPjCb = PJ_GET_PJCB(pst->dstInst);

   RLOG2(L_DEBUG, "PjLiKwuFlowCntrlInd(rbId(%d),rbType (%d), mBuf)",
                     flowCntrlInfo->rlcId.rbId,flowCntrlInfo->rlcId.rbType);

   ret = pjDbmFetchDlRbCb (tPjCb,flowCntrlInfo->rlcId,&pjRbCb);
   if (ret != ROK)
   {
      RLOG_ARG2(L_ERROR, DBG_UEID,flowCntrlInfo->rlcId.ueId, "CellId[%u]:DL RbId[%d] not found",
            flowCntrlInfo->rlcId.cellId, flowCntrlInfo->rlcId.rbId);
      PJ_FREE_SHRABL_BUF_PST(pst->region, pst->pool, flowCntrlInfo, sizeof(KwuFlowCntrlIndInfo));
      RETVALUE(RFAILED);
   }

   /* if RLC is requesting to stop the flow means RB is overloaded */
   if (flowCntrlInfo->pktAdmitCnt) 
   {
      if (pjRbCb->mode == PJ_DRB_AM)
      {
         pjRbCb->dropOnlyOne = 1;
         pjRbCb->pktAdmitCnt= 0;
      }
      else
      {
         pjRbCb->dropOnlyOne = 0;
         pjRbCb->pktAdmitCnt = flowCntrlInfo->pktAdmitCnt;
      }
   }
   else
   {
      /*  flowCntrlInfo->pktAdmitCnt = 0 indicates PDB Recovered and no FC required
       *  reset FC state  */
      pjRbCb->dropOnlyOne = 0;
      pjRbCb->pktAdmitCnt = 0;
   }
   pjRbCb->pktCnt = 0;

   PJ_FREE_SHRABL_BUF_PST(pst->region, pst->pool, flowCntrlInfo, sizeof(KwuFlowCntrlIndInfo));
   RETVALUE(ROK);
} /* PjLiKwuFlowCntrlInd */
/**
 *
 * @brief 
 *
 *        Handler for getting the Status Ind from RLC.
 *
 * @b Description:
 *
 *        This function receives the PDU from RLC 
 *        and invokes the handler for processing the PDU.
 *
 *  @param[in] pjRbCb    -   PDCP RB Control Block
 *  @param[in] mBuf    -   PDU
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 *
 */
#ifdef ANSI
PUBLIC S16 PjLiKwuStaInd
(
Pst           *pst,       /* Post */
SuId          suId,       /* Service User Id */
KwuStaIndInfo *staInd     /* StatusIndication Info */
)
#else
PUBLIC Void PjLiKwuStaInd(pst, suId, datCfm)
Pst           *pst;        /* Post */
SuId          suId;        /* Service User Id */
KwuStaIndInfo *staInd;     /* Status Indication Info */
#endif
{
   S16         ret;           /* Return Value */
   PjDlRbCb     *pjRbCb;
   PjCb   *tPjCb;
   U32 minSduCnt;

   TRC1(PjLiKwuStaInd);

   tPjCb = PJ_GET_PJCB(pst->dstInst);
   if ((pst->dstInst >= PJ_MAX_PDCP_INSTANCES) || (tPjCb->init.cfgDone == FALSE))
   {
       PJ_FREE_SHRABL_BUF_PST(pst->region, pst->pool, staInd, sizeof(KwuStaIndInfo));
       RETVALUE(RFAILED);
   }


   RLOG2(L_DEBUG, "PjLiKwuStaInd(rbId(%d),rbType (%d), mBuf)",
                         staInd->rlcId.rbId,staInd->rlcId.rbType);

   ret = pjDbmFetchDlRbCb (tPjCb,staInd->rlcId,&pjRbCb);
   if (ret != ROK)
   {
      RLOG_ARG2(L_ERROR, DBG_UEID,staInd->rlcId.ueId, "CellId[%u]:DL RbId[%d] not found",
            staInd->rlcId.cellId, staInd->rlcId.rbId);
/* To confirm :: BRDCM team */
      PJ_FREE_SHRABL_BUF_PST(pst->region, pst->pool, staInd, sizeof(KwuStaIndInfo));
      RETVALUE(RFAILED);
  }
  minSduCnt = (KWU_MAX_STA_IND_SDU < PJ_MAX_DAT_CFM)?KWU_MAX_STA_IND_SDU:PJ_MAX_DAT_CFM;
  if(staInd->numSdu > minSduCnt)
  {
  	   staInd->numSdu = (U16)minSduCnt;    /*KW_FIX*/
	   RLOG_ARG2(L_ERROR, DBG_RBID , pjRbCb->rbId , "numSdu [%u] in StaInd is more than acceptable limit [%lu]",
              staInd->numSdu , minSduCnt);
  }
  pjLimStaInd(tPjCb, pjRbCb, (PjDatCfm *) staInd);
/* To confirm :: BRDCM team */
   PJ_FREE_SHRABL_BUF_PST(pst->region, pst->pool, staInd, sizeof(KwuStaIndInfo));
   RETVALUE(ROK);
} /* pjLimKwuStaInd */



/**
 *
 * @brief 
 *
 *        Handler for getting the ReEstablish Ind from RLC.
 *
 * @b Description:
 *
 *        This function receives the PDU from RLC 
 *        and invokes the handler for processing the PDU.
 *
 *  @param[in] pjRbCb    -   PDCP RB Control Block
 *  @param[in] mBuf    -   PDU
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 *
 */
#ifdef ANSI
PUBLIC S16 PjLiKwuReEstCmpInd
(
Pst           *pst,       /* Post */
SuId          suId,       /* Service User Id */
CmLteRlcId    rlcId       /* RlcId */
)
#else
PUBLIC Void PjLiKwuReEstCmpInd(pst, suId, rlcId)
Pst           *pst;        /* Post */
SuId          suId;        /* Service User Id */
CmLteRlcId    rlcId;       /* RlcId */
#endif
{
   S16        ret;           /* Return Value */
   PjUlRbCb   *ulRbCb;
   PjCb   *tPjCb;

   TRC1(PjLiKwuReEstCmpInd);

   tPjCb = PJ_GET_PJCB(pst->dstInst);
   if ((pst->dstInst >= PJ_MAX_PDCP_INSTANCES) || (tPjCb->init.cfgDone == FALSE))
   {
       RETVALUE(RFAILED);
   }


   RLOG2(L_DEBUG, "PjLiKwuReEstCmpInd(rbId(%d),rbType (%d), mBuf)",
                         rlcId.rbId,rlcId.rbType);

   ret = pjDbmFetchUlRbCb (tPjCb,rlcId,&ulRbCb);
   if (ret != ROK)
   {
      RLOG_ARG2(L_ERROR, DBG_UEID,rlcId.ueId, "CellId[%u]:UL RbId[%d] not found",
            rlcId.cellId, rlcId.rbId);
      RETVALUE(RFAILED);
   }
   pjUlmReEstCmpInd(tPjCb,ulRbCb);

   RETVALUE(ROK);
} /* pjLimDatInd */

#ifdef LTE_L2_MEAS
/**
 *
 * @brief 
 *
 *        Handler for getting the Ack Ind from RLC.
 *
 * @b Description:
 *
 *        This function receives the PDU from RLC 
 *        and invokes the handler for processing the PDU.
 *
 *  @param[in] pjRbCb    -   PDCP RB Control Block
 *  @param[in] mBuf    -   PDU
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 *
 */
#ifdef ANSI
PUBLIC S16 PjLiKwuDatAckInd
(
Pst           *pst,       /* Post */
SuId           suId,      /* Service User Id */
KwuDatAckInfo *ackInfo    /* Data Ack  Info */
)
#else
PUBLIC Void PjLiKwuDatAckInd(pst, suId, datCfm)
Pst           *pst;        /* Post */
SuId          suId;        /* Service User Id */
KwuDatAckInfo *ackInfo;    /* Data Ack Info */
#endif
{
   RETVALUE(ROK);
} /* pjLimDatInd */
#endif

/**
 *
 * @brief 
 *
 *        Handler for getting the DiscSdu Cfm from RLC.
 *
 * @b Description:
 *
 *        This function receives the PDU from RLC 
 *        and invokes the handler for processing the PDU.
 *
 *  @param[in] pjRbCb    -   PDCP RB Control Block
 *  @param[in] mBuf    -   PDU
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 *
 */
#ifdef ANSI
PUBLIC S16 PjLiKwuDiscSduCfm
(
Pst           *pst,       /* Post */
SuId           suId,      /* Service User Id */
KwuDiscSduInfo *discInfo    /* Data Ack  Info */
)
#else
PUBLIC Void PjLiKwuDiscSduCfm(pst, suId, discInfo)
Pst           *pst;        /* Post */
SuId          suId;        /* Service User Id */
KwuDiscSduInfo *discInfo;    /* Data Ack Info */
#endif
{
   S16         ret;           /* Return Value */
   PjDlRbCb     *pjRbCb;
   PjCb   *tPjCb;

   TRC1(PjLiKwuDiscSduCfm);
   tPjCb = PJ_GET_PJCB(pst->dstInst);


   RLOG2(L_DEBUG, "PjLiKwuDiscSduCfm (rbId(%d),rbType (%d), mBuf)",
                      discInfo->rlcId.rbId,discInfo->rlcId.rbType);

   ret = pjDbmFetchDlRbCb (tPjCb,discInfo->rlcId,&pjRbCb);
   if (ret != ROK)
   {
      RLOG_ARG2(L_ERROR, DBG_UEID,discInfo->rlcId.ueId, "CellId[%u]:DL RbId[%d] not found",
            discInfo->rlcId.cellId, discInfo->rlcId.rbId);
      PJ_FREE_SHRABL_BUF_PST(pst->region, pst->pool, discInfo, sizeof(KwuDiscSduInfo));
      RETVALUE(RFAILED);
  }

   pjLimDiscSduCfm(tPjCb, pjRbCb, discInfo);

   PJ_FREE_SHRABL_BUF_PST(pst->region, pst->pool, discInfo, sizeof(KwuDiscSduInfo));

   RETVALUE(ROK);
} /* pjLimDatInd */

/**
 *
 * @brief 
 *
 *        Handler for getting the PDU from RLC.
 *
 * @b Description:
 *
 *        This function receives the PDU from RLC 
 *        and invokes the handler for processing the PDU.
 *
 *  @param[in] pjRbCb    -   PDCP RB Control Block
 *  @param[in] mBuf    -   PDU
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 *
 */
#ifdef ANSI
PUBLIC Void pjLimDatInd
(
PjCb          *gCb,
PjUlRbCb        *pjRbCb,        /* RB Control Block */
Buffer        *mBuf,           /* RLC PDU */
Bool          isOutOfSeq
)
#else
PUBLIC Void pjLimDatInd(gCb, pjRbCb, mBuf, isOutOfSeq)
PjCb          *gCb;
PjUlRbCb      *pjRbCb;        /* RB Control Block */
Buffer        *mBuf;          /* RLC PDU */
Bool           isOutOfSeq;
#endif
{
#if (ERRCLASS & ERRCLS_DEBUG)
   S16         ret;           /* Return Value */
#endif

   TRC3(pjLimDatInd);

   PJ_STS_INC_GEN_CNT(gCb, rxPdus);

   /* Extract the header */
   if(pjRbCb->rbType == PJ_SRB)
   {
      if((pjRbCb->rbId == 1) || (pjRbCb->ueCb->libInfo.state == PJ_STATE_NORMAL))
      {
#if (ERRCLASS & ERRCLS_DEBUG)
         ret = pjUlmHdlSrbPkt(gCb, pjRbCb, mBuf);
#else
         pjUlmHdlSrbPkt(gCb, pjRbCb, mBuf);
#endif
      }
      else
      {
         /* in reestablishment state */
         PJ_FREE_BUF(mBuf);
      }
   }
   else if(pjRbCb->rbType == PJ_DRB)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      ret = pjUlmHdlDrbPkt(gCb, pjRbCb, mBuf, isOutOfSeq);
#else
      pjUlmHdlDrbPkt(gCb, pjRbCb, mBuf, isOutOfSeq);
#endif
   }

#if (ERRCLASS & ERRCLS_DEBUG)
   if (ret != ROK)
   {
      RLOG_ARG0(L_ERROR, DBG_RBID,pjRbCb->rbId, "pjLimDatInd: pjHdlProcessPdu Failed");
   }
#endif

   RETVOID;
} /* pjLimDatInd */


/**
 *
 * @brief 
 *
 *        Handler for getting the Data Cfm from RLC.
 *
 * @b Description:
 *
 *        This function receives the Data Cfm from RLC 
 *        and invokes the handler for processing it.
 *
 *  @param[in] pjRbCb    -   PDCP RB Control Block
 *  @param[in] datCfm    -   Data Confirmation Information 
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 *
 */
#ifdef ANSI
PUBLIC Void pjLimDatCfm
(
PjCb          *gCb,
PjDlRbCb        *pjRbCb,     /* RB Control Block */
PjDatCfm      *datCfm      /* Data Confirmation Information */
)
#else
PUBLIC Void pjLimDatCfm(gCb, pjRbCb, datCfm)
PjCb          *gCb;
PjDlRbCb        *pjRbCb;     /* RB Control Block */
PjDatCfm      *datCfm;     /* Data Confirmation Information */
#endif
{
#if (ERRCLASS & ERRCLS_DEBUG)
   S16         ret = ROK;  /* Return Value */
#endif

   TRC3(pjLimDatCfm);


   /* Process the PDU */
#if (ERRCLASS & ERRCLS_DEBUG)
   ret = pjDlmProcessCfm(gCb, pjRbCb, datCfm, PJ_CFM_OK);
   if (ret != ROK)
   {
      RLOG_ARG0(L_ERROR, DBG_RBID,pjRbCb->rbId, "pjLimDatCfm: pjDlmProcessCfm Failed");
   }
#else
   pjDlmProcessCfm(gCb, pjRbCb, datCfm, PJ_CFM_OK);
#endif
   /* pj002.201 : Removed freeing and allocation of datCfm */
   RETVOID;
} /* pjLimDatCfm */


/**
 *
 * @brief 
 *
 *        Handler for trigerring the error indication when RLC times out
 *        for sending AM data.
 *
 * @b Description:
 *
 *        This function receives the pduIds of the PDU that were not
 *        successfully sent to the layer.
 *
 *  @param[in] pjRbCb  -   PDCP RB Control Block
 *  @param[in] staInd  -   Status Indication Information.
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 *
 */
#ifdef ANSI
PUBLIC Void pjLimStaInd
(
PjCb         *gCb,
PjDlRbCb       *pjRbCb,            /* PDCP RB Control Block */
PjDatCfm     *staInd             /* Status Indication Information */
)
#else
PUBLIC Void pjLimStaInd(gCb, pjRbCb, staInd)
PjCb         *gCb;
PjDlRbCb       *pjRbCb;           /* PDCP RB control Block */
PjDatCfm     *staInd;           /* Status Indication Information */
#endif
{
#if (ERRCLASS & ERRCLS_DEBUG)
   S16         ret = ROK;       /* Return Value */
#endif

   TRC3(pjLimStaInd)

   if (gCb->init.cfgDone == FALSE)
   {
      RETVOID;
   }
   RLOG1(L_DEBUG, "pjLimStaInd(pjRbCb(%d), staInd)", pjRbCb->rbId);

   /* Call the confirm code */
#if (ERRCLASS & ERRCLS_DEBUG)
   ret = pjDlmProcessCfm(gCb, pjRbCb, staInd, PJ_CFM_NOK);
   if (ret != ROK)
   {
      RLOG_ARG0(L_ERROR, DBG_RBID,pjRbCb->rbId, "pjLimStaInd: pjDlmProcessCfm Failed");
   }
#else
   pjDlmProcessCfm(gCb, pjRbCb, staInd, PJ_CFM_NOK);
#endif
   /* pj002.201 : Removed freeing and allocation of staInd */
   RETVOID;
} /* pjLimStaInd */


/**
 *
 * @brief 
 *
 *        Handler for informing the PDCP that RLC has successfully discarded
 *        certain PDUs.
 *
 * @b Description:
 *
 *        This function receives the pointer to the PDCP control block of the
 *        RB that has finished transmitting its data to PDCP.
 *
 *  @param[in] pjRbCb       -   PDCP RB Control Block
 *  @param[in] discSduCfm   -   Info about the discarded SDUs
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 *
 */
#ifdef ANSI
PRIVATE S16 pjLimDiscSduCfm
(
PjCb       *gCb,
PjDlRbCb   *pjRbCb,        /* PDCP RB Control Block */
KwuDiscSduInfo *discSduCfm /* Info about the discarded SDUs*/
)
#else
PRIVATE S16 pjLimDiscSduCfm(gCb, pjRbCb, count)
PjCb       *gCb;
PjDlRbCb   *pjRbCb;         /* PDCP RB control Block */
KwuDiscSduInfo *discSduCfm; /* Info about the discarded SDUs*/
#endif
{
   PjuDatCfmInfo datCfm;       /* DatCfm structure */
   CmLtePdcpId   pdcpId;       /* PDCP ID */
   PjTxEnt       *txEnt;        /* Transmission Entity */
   U32           count = 0;
   U16           cfmsToUpperLayer = 0;    /*KW_FIX*/

   TRC3(pjLimDiscSduCfm);

   RLOG1(L_DEBUG, "pjLimDiscSduCfm(pjRbCb(%d))", pjRbCb->rbId);

   if((gCb->init.cfgDone == FALSE) || (discSduCfm->numSduIds == 0))
   {
      RETVALUE(ROK);
   }
   
   while(count < discSduCfm->numSduIds)
   {
      txEnt = pjDbmGetTxEnt(gCb, &(pjRbCb->dlCb.txBuf), discSduCfm->sduIds[count]); 

      if(txEnt != NULLP)
      {
         if( pjRbCb->dlCb.cfmReqd)
         {
            datCfm.cfmSta[cfmsToUpperLayer].sduId  = txEnt->sduId;
            datCfm.cfmSta[cfmsToUpperLayer++].status = PJU_DISCARD_TMR_EXP;
         }

         if( txEnt->pdu == txEnt->sdu )
         {
            txEnt->sdu = NULLP;
         }

         /* if the PDU being discarded was a one for which confirm was expected
            then we need to set the confirm expectation correctly */

         if(pjRbCb->dlCb.cfmExp < txEnt->count)
         {
            /* need to check if count should be just incremented by one or should the txHfn
               also need to be considered */
            pjRbCb->dlCb.cfmExp = txEnt->count + 1;
         }
         
         pjDbmDelTxEnt(gCb, &(pjRbCb->dlCb.txBuf), txEnt->count);

#ifdef LTE_L2_MEAS 
         PJ_UPD_L2_DLDISC_STS(gCb,pjRbCb);
#endif
      }

      count++;
   }

   if(pjRbCb->dlCb.cfmReqd)
   {
      PjPjuSapCb    *pjuSap;       /* PJU SAP */

      datCfm.numSdus = cfmsToUpperLayer;

      pdcpId.rbId   = pjRbCb->rbId;
      pdcpId.rbType = pjRbCb->rbType;
      pdcpId.ueId   = pjRbCb->ueCb->key.ueId;
      pdcpId.cellId = pjRbCb->ueCb->key.cellId;

      if (pjRbCb->rbType == PJ_SRB)
      {
         pjuSap = &(gCb->u.dlCb->pjuSap[PJ_SRB_SAP]);
      }
      else
      {
         pjuSap = &(gCb->u.dlCb->pjuSap[PJ_DRB_SAP]);
      }

      /* If trace flag is enabled send the trace indication */
      if(gCb->init.trc == TRUE)
      {
         /* Populate the trace params */
         pjLmmSendTrc(gCb, EVTPJUDATCFM, NULLP);
      }

      PjUiPjuDatCfm(&pjuSap->pst, pjuSap->suId, &pdcpId, &datCfm);
   }


   RETVALUE(ROK);
} /* pjLimDiscSduCfm */
#ifdef __cplusplus
}
#endif /* __cplusplus */

/********************************************************************30**
         End of file
**********************************************************************/
