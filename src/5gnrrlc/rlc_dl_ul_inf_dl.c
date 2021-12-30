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
 
    Name:   NR RLC layer
 
    Type:   C include file
 
    Desc:   Defines required by LTE MAC
 
    File:   rlc_dl_ul_inf_dl.c

**********************************************************************/

/* header include files (.h) */
#include "common_def.h"
#include "lkw.h"           /* LKW defines */
#include "ckw.h"           /* CKW defines */
#include "kwu.h"           /* KWU defines */
#include "rgu.h"           /* RGU defines */
#include "rlc_err.h"        /* Err defines */
#include "rlc_env.h"        /* RLC environment options */


/* extern (.x) include files */
#include "lkw.x"           /* LKW */
#include "ckw.x"           /* CKW */
#include "kwu.x"           /* KWU */
#include "rgu.x"           /* RGU */

#include "rlc_utils.h"            /* RLC defines */
#include "rlc_dl_ul_inf.h"
#include "rlc_dl.h"

#define RLC_MODULE RLC_DBGMASK_UDX
/* local defines */

S16 rlcDlmHndlStaRsp ARGS (( RlcCb  *gCb,RlcDlRbCb  *rbCb,
                RlcUdxStaPdu *pStaPdu, RlcUdxBufLst  *rlsPduLst));




/**
  * @brief
  * UDX APIs
  */

/**
 *
 * @brief 
 *    Handler to bind the DL with UL. 
 *
 * @param[in] pst   Post structure  
 * @param[in] suId  Service user SAP ID 
 * @param[in] spId  Service provider ID
 *
 * @return  S16
 *    -# ROK 
 *    -# RFAILED
 */
S16 rlcDlUdxBndReq 
(
Pst    *pst,  
SuId   suId, 
SpId   spId 
)
{
   RlcUdxDlSapCb   *udxSap;            /* pointer to session SAP */
   RlcCb           *tRlcCb;

#if (ERRCLASS & ERRCLS_INT_PAR)
   if (pst->dstInst >= MAX_RLC_INSTANCES)
   {
      return RFAILED;
   }
#endif
   tRlcCb = RLC_GET_RLCCB(pst->dstInst);

   RLCDBGP_BRIEF(tRlcCb, "rlcDlUdxBndReq(spId(%d), suId(%d))\n", 
                spId, suId);

   udxSap = (tRlcCb->u.dlCb->udxDlSap + spId);

   /* Verify CKW SAP State */
   switch(udxSap->state)
   {
      /* SAP is configured but not bound */
      case RLC_SAP_CFG:
      case RLC_SAP_UBND:
      {
         /* copy bind configuration parameters in SSAP sap */
         udxSap->suId = suId;
         udxSap->pst.dstProcId = pst->srcProcId;
         udxSap->pst.dstEnt = pst->srcEnt;
         udxSap->pst.dstInst = pst->srcInst;

         /* Update the State */
         udxSap->state = RLC_SAP_BND;

         DU_LOG("\nINFO   --> RLC_DL : UDX SAP state [%d]", udxSap->state);
         break;
      }
      /* SAP is already bound */
      case RLC_SAP_BND:
      {
         /* 
          * Sap is already bound check source, destination Entity and 
          * Proc Id
          */
         if (udxSap->pst.dstProcId != pst->srcProcId 
               || udxSap->pst.dstEnt != pst->srcEnt
               || udxSap->pst.dstInst != pst->srcInst
               || udxSap->suId != suId)
         {
            RLC_SEND_SAPID_ALARM(tRlcCb, spId, 
                                LKW_EVENT_UDX_BND_REQ, LCM_CAUSE_INV_PAR_VAL);

            DU_LOG("\nERROR  --> RLC_DL : UDX SAP already Bound");
            rlcDlUdxBndCfm(&(udxSap->pst), udxSap->suId, CM_BND_NOK);
         }
         break;
      }

     default:
      {
#if (ERRCLASS & ERRCLS_INT_PAR)
         RLC_SEND_SAPID_ALARM(tRlcCb,spId, 
                             LKW_EVENT_CKW_BND_REQ, LCM_CAUSE_INV_STATE);
#endif /* ERRCLASS & ERRCLS_INT_PAR */
         DU_LOG("\nERROR  --> RLC_DL : Invalid UDX SAP State in Bind Req");
         rlcDlUdxBndCfm(&(udxSap->pst), udxSap->suId, CM_BND_NOK);
         break;
      }
   }
   rlcDlUdxBndCfm(&(udxSap->pst), udxSap->suId, CM_BND_OK);
   return ROK;
} 


/**
 * @brief 
 *    Handler for unbinding the DL from UL. 
 *
 *  @param[in] pst     Post structure  
 *  @param[in] spId    Service provider SAP ID 
 *  @param[in] reason  Reason for Unbinding 
 *
 *  @return  S16
 *      -# ROK 
 */
S16 rlcDlUdxUbndReq
(
Pst      *pst,    
SpId     spId,   
Reason   reason 
)
{
   RlcUdxDlSapCb   *udxSap; 
   RlcCb           *tRlcCb;

#if (ERRCLASS & ERRCLS_INT_PAR)
   if (pst->dstInst >= MAX_RLC_INSTANCES)
   {
      return RFAILED;
   }
#endif

   tRlcCb = RLC_GET_RLCCB(pst->dstInst);

   DU_LOG("\nDEBUG  --> RLC_DL : Unbind Req for spId[%d], reason[%d]", 
                spId, reason);
   UNUSED(reason);
   /* disable upper sap (CKW) */
   udxSap = (tRlcCb->u.dlCb->udxDlSap + spId);

#if (ERRCLASS & ERRCLS_INT_PAR)
   RLC_GET_AND_VALIDATE_UDXSAP(tRlcCb,udxSap, EKW208, "KwUiDlUdxndReq");
#endif /* ERRCLASS & ERRCLS_INT_PAR */
   udxSap->state = RLC_SAP_CFG;
   return ROK;
}


/**
 * @brief 
 *    Handler for configuring RLC entities.
 *
 * @details
 *    This function is used by RRC to configure(add/delete/modify)
 *    one or more RLC entities. 
 *        - CKW_CFG_ADD          => rlcCfgAddRb
 *        - CKW_CFG_MODIFY       => rlcCfgReCfgRb
 *        - CKW_CFG_DELETE       => rlcCfgDelRb
 *        - CKW_CFG_REESTABLISH  => rlcCfgReEstRb
 *        - CKW_CFG_DELETE_UE    => rlcCfgDelUe
 *
 * @param[in] pst   -  Post structure  
 * @param[in] spId  -  Serive Provider ID 
 * @param[in] cfg   -  Configuration information for one or more RLC entities. 
 *
 * @return  S16
 *    -# ROK 
 *    -# RFAILED
 */
S16 rlcDlUdxCfgReq
(
Pst          *pst,
SpId         spId,
RlcCfgInfo   *cfg
)
{
   RlcCfgCfmInfo   *cfgCfm; 
   uint8_t         idx;    
   RlcCb           *tRlcCb;
   Pst             *pstUdxCfm;

#if (ERRCLASS & ERRCLS_INT_PAR)
   if (pst->dstInst >= MAX_RLC_INSTANCES)
   {
      return RFAILED;
   }
#endif

   tRlcCb = RLC_GET_RLCCB(pst->dstInst);

   pstUdxCfm = &(tRlcCb->u.dlCb->udxDlSap[spId].pst);
   RLCDBGP_BRIEF(tRlcCb,"spId(%d)\n", spId);
   /* Allocate memory and memset to 0 for cfmInfo */
   RLC_ALLOC_SHRABL_BUF_WC(pstUdxCfm->region,
                          pstUdxCfm->pool,
                          cfgCfm,
                          sizeof(RlcCfgCfmInfo));

#if (ERRCLASS & ERRCLS_ADD_RES)
   if (cfgCfm == NULLP)
   {
      DU_LOG("\nERROR  --> RLC_DL : Memory Allocation Failed.");
      /* kw002.201 Freeing from proper region */
      /* RLC_PST_FREE(pst->region, pst->pool, cfg, sizeof(RlcCfgInfo)); */
      return RFAILED;
   }
#endif /* ERRCLASS & ERRCLS_ADD_RES */

   /* For every entity configuration process by cfgType */
   for (idx = 0; idx < cfg->numEnt; idx++)
   {
      RlcEntCfgCfmInfo   *entCfgCfm;
      RlcEntCfgInfo      *entCfg;

      entCfg  = (RlcEntCfgInfo *)&(cfg->entCfg[idx]);
      entCfgCfm   = (RlcEntCfgCfmInfo *)&(cfgCfm->entCfgCfm[idx]);

      switch (entCfg->cfgType)
      {
         case CKW_CFG_ADD:
            {
               if (entCfg->dir & RLC_DIR_DL)
               { 
                  /* Add a new RB entity configuration */
                  if (rlcCfgAddDlRb(tRlcCb,cfg->ueId, cfg->cellId,\
                              entCfg, entCfgCfm) != ROK)
                  {
                     DU_LOG("\nERROR  --> RLC_DL : Addition Failed due to[%d]",
                           entCfgCfm->status.reason);
                  }
               }
               break;
            }
         case CKW_CFG_MODIFY:
            {
               if (entCfg->dir & RLC_DIR_DL)
               {
                  /* Re-configure the existing RB entity configuration */
                  if (rlcCfgReCfgDlRb(tRlcCb,cfg->ueId, cfg->cellId,\
                           entCfg, entCfgCfm) != ROK)
                  {
                     DU_LOG("\nERROR  --> RLC_DL : ReCfg Failed due to[%d]",
                           entCfgCfm->status.reason);
                  }
               }
               break;
            }

         case CKW_CFG_DELETE:
            {
               if (entCfg->dir & RLC_DIR_DL)
               {
                  /* Delete the existing RB entity configuration */
                  if (rlcCfgDelDlRb(tRlcCb,cfg->ueId, cfg->cellId,\
                        entCfg, entCfgCfm) != ROK)
                  {
                     DU_LOG("\nERROR  --> RLC_DL : Deletion Failed due to[%d]",
                           entCfgCfm->status.reason);
                  } 
               }
               break;
            }

         case CKW_CFG_REESTABLISH:
            {
               if (entCfg->dir & RLC_DIR_DL)
               {
                  /*if direction is both then, re-establishment end indication
                   * should be sent only from the UL instance, only if DIR is
                   * DL only then DL instance will send indication.*/
                  Bool sndReEst = TRUE;
                  if (entCfg->dir & RLC_DIR_UL)
                  {
                     sndReEst = FALSE;
                  }
                  /* Re-establish the existing RB entity configuration */
                  if (rlcCfgReEstDlRb(tRlcCb,cfg->ueId, cfg->cellId,
                                     sndReEst,entCfg, entCfgCfm) != ROK)
                  {
                     DU_LOG("\nERROR  --> RLC_DL : Reest Failed due to[%d]",
                           entCfgCfm->status.reason);
                  }
               }
               break;
            }

         case CKW_CFG_DELETE_UE:
            {
               /* Delete all RB entity configuration under UE */
               if (rlcCfgDelDlUe(tRlcCb,cfg->ueId, cfg->cellId,
                                entCfg, entCfgCfm) != ROK)
               {
                  DU_LOG("\nERROR  --> RLC_DL : deletion Failed due to[%d]",
                           entCfgCfm->status.reason);
               }
               else
               {
                  DU_LOG("\nDEBUG  -->  RLC_DL: UE information is deleted for UEId[%d] and CellId[%d]",\
                  cfg->ueId, cfg->cellId);
               }
               break;
            }
         case CKW_CFG_DELETE_CELL:
            {
               if (rlcCfgDelDlCell(tRlcCb,cfg->cellId,entCfg,entCfgCfm) 
                                                                != ROK )
               {
                  DU_LOG("\nERROR  --> RLC_DL : deletion Failed due to[%d]",
                           entCfgCfm->status.reason);
               } 
               break;
            }

         default:
            {
               RLC_CFG_FILL_CFG_CFM(entCfgCfm, entCfg->rbId, entCfg->rbType,\
                                   CKW_CFG_CFM_NOK, CKW_CFG_REAS_INVALID_CFG);
               DU_LOG("\nERROR  --> RLC_DL : Invalid CfgType");
            }
      }
   }

   /* Assign number of entity configuraitons and suId */
   cfgCfm->transId = cfg->transId;
   cfgCfm->ueId = cfg->ueId;
   cfgCfm->cellId = cfg->cellId;
   cfgCfm->numEnt = cfg->numEnt;

   /* kw002.201 Freeing from proper region */
   /* RLC_PST_FREE(pst->region, pst->pool, cfg, sizeof(RlcCfgInfo)); */
   /* Send Configuration confirm primitive */
   rlcDlUdxCfgCfm(&(tRlcCb->u.dlCb->udxDlSap[spId].pst),
                 tRlcCb->u.dlCb->udxDlSap[spId].suId, 
                 cfgCfm);

   return ROK;
} 

/**
 *@brief 
 *   This primitive is used by RRC to change the UeId for the existing UE
 *   context.
 *
 * @param pst     -  Pointer to the pst structure
 * @param spId    -  The ID of the service provider SAP in the RLC layer 
 * @param transId -  Transaction ID. This field uniquily identifies
 *                   transaction between RRC and RLC
 * @param ueInfo    -  Old UE Id Info for which the change request has come 
 * @param newUeInfo -  New UE Id Info for existing UE context 
 * 
 * @return 
 *    -# ROK
 *    -# RFAILED
 */
S16 rlcDlUdxUeIdChgReq
(
Pst         *pst, 
SpId        spId, 
uint32_t    transId, 
CkwUeInfo   *ueInfo,
CkwUeInfo   *newUeInfo
)
{
   CmStatus       status;
   RlcCb           *tRlcCb;

#if (ERRCLASS & ERRCLS_INT_PAR)
   if (pst->dstInst >= MAX_RLC_INSTANCES)
   {
      return RFAILED;
   }
#endif

   tRlcCb = RLC_GET_RLCCB(pst->dstInst);
#ifndef ALIGN_64BIT
   RLCDBGP_BRIEF(tRlcCb, "(spId(%d), transId(%ld))\n", 
                spId, transId);
#else
   RLCDBGP_BRIEF(tRlcCb, "(spId(%d), transId(%d))\n", 
                spId, transId);
#endif

   status.reason = CKW_CFG_REAS_NONE;
   status.status = CKW_CFG_CFM_OK;
   
   if (rlcCfgDlUeIdChng(tRlcCb, ueInfo, newUeInfo, &status) != ROK)
   {
      DU_LOG("\nERROR  --> RLC_DL : Failure due to[%d]",
             status.reason);
   }
   rlcDlUdxUeIdChgCfm(&(tRlcCb->u.dlCb->udxDlSap[spId].pst),
                     tRlcCb->u.dlCb->udxDlSap[spId].suId, 
                     transId, 
                     status);

   return ROK;
} 

/**
* @brief 
*    Request for status PDU from  ULM to DLM.
*
* @param[in]   pst   -  Post Structure
* @param[in]   spId  -  Service Provider Id
* @param[in]   rlcId -  Rlc Information Id
* @param[in]   pStaPdu - Status PDU 
*  
* @return   S16
*    -# ROK
*    -# RFAILED
**/
S16  rlcDlUdxStaPduReq
(
Pst             *pst,
SpId            spId,
CmLteRlcId      *rlcId,
RlcUdxDlStaPdu   *pStaPdu
)
{
   RlcDlRbCb   *rbCb;
   RlcCb       *tRlcCb;

   tRlcCb =  RLC_GET_RLCCB (pst->dstInst);

   rlcDbmFetchDlRbCbByRbId(tRlcCb, rlcId, &rbCb); /* Fetch DBM RbCb */
   if (!rbCb)
   {
      DU_LOG("\nERROR  --> RLC_DL : CellId [%u]:RbId[%d] not found",
            rlcId->cellId,rlcId->rbId);
      RLC_FREE_SHRABL_BUF(pst->region, 
			 pst->pool, 
			 pStaPdu, 
			 sizeof(RlcUdxDlStaPdu));
      return RFAILED;
   }

   RLC_AMDL.cntrlBo = pStaPdu->controlBo;
   /* If there already exists a STAUS PDU, free it and take the new one
      into account */
   if(RLC_AMDL.pStaPdu)
   {
      RLC_FREE_SHRABL_BUF(pst->region, 
			 pst->pool, 
			 RLC_AMDL.pStaPdu, 
			 sizeof(RlcUdxDlStaPdu));
   }
   
   RLC_AMDL.pStaPdu = pStaPdu;
   rlcAmmSendDedLcBoStatus(tRlcCb, rbCb, &RLC_AMDL);             

   return  (ROK);
}

/**
* @brief 
*    It handles the status update recieved from ULM.
*
* @param[in]   pst   -  Post Structure
* @param[in]   spId  -  Service Provider Id
* @param[in]   rlcId -  Rlc Information Id
* @param[in]   pStaPdu - Status PDU 
*  
* @return   S16
*    -# ROK
*    -# RFAILED
**/
S16  rlcDlUdxStaUpdReq
(
Pst*          pst,
SpId          spId,
CmLteRlcId    *rlcId,
RlcUdxStaPdu   *pStaPdu
)
{
   RlcCb          *tRlcCb;
   RlcDlRbCb      *rbCb;

   tRlcCb = RLC_GET_RLCCB(pst->dstInst);

   rlcDbmFetchDlRbCbByRbId(tRlcCb, rlcId, &rbCb);
   if (!rbCb)
   {
      DU_LOG("\nERROR  --> RLC_DL : CellId [%u]:RbId[%d] not found",
            rlcId->cellId,rlcId->rbId);
      return RFAILED;
   }

   rlcAmmDlHndlStatusPdu(tRlcCb, rbCb, pStaPdu);

   RLC_FREE_SHRABL_BUF(pst->region,
		      pst->pool, 
		      pStaPdu, 
		      sizeof(RlcUdxStaPdu));

   return ROK;
}

#ifdef LTE_L2_MEAS
/**
*/
S16 rlcDlUdxL2MeasReq 
(
Pst            *pst, 
RlcL2MeasReqEvt *measReqEvt 
)
{
   uint32_t  cntr;
   uint8_t   measType;
   volatile uint32_t     startTime = 0;
   RlcCb     *tRlcCb;

   /*starting Task*/
   SStartTask(&startTime, PID_RLC_MEAS_START);

   tRlcCb =  RLC_GET_RLCCB(pst->dstInst);

   /* Initialize measCfmEvt */

  /* validate the received measReqEvt */
 /*LTE_L2_MEAS_PHASE2*/

   measType = measReqEvt->measReq.measType;

   if(measType & LKW_L2MEAS_DL_IP) 
   {
      /* if measurement is for DL IP enable for all QCI */
      for(cntr = 0; cntr < LKW_MAX_QCI; cntr++)
      {
         tRlcCb->u.dlCb->rlcL2Cb.measOn[cntr] |= LKW_L2MEAS_DL_IP;
      }
   }
   else
   {
      /* for nonIpThroughput meas, enable only for the sent QCIs */
      uint32_t i;
      for(i = 0; i < LKW_MAX_QCI; i++)
      {
         tRlcCb->u.dlCb->rlcL2Cb.measOn[i] |= measType;
      }
   }

   /* We need to copy the transId for sending back confirms later */
   for(cntr = 0; cntr < LKW_MAX_L2MEAS; cntr++)
   {
      RlcL2MeasEvtCb* measEvtCb = &(tRlcCb->u.dlCb->rlcL2Cb.rlcL2EvtCb[cntr]);
      if(measEvtCb->measCb.measType & measType)
      {
         measEvtCb->transId= measReqEvt->transId;
      }
   }

   /*stopping Task*/
   SStopTask(startTime, PID_RLC_MEAS_START);
   return ROK;
} /* rlcDlUdxMeasReq */

/**
@brief 
This function processes L2 Measurement stop request received from the layer manager.
After receving this request, RLC stops L2 Measurement
 *  @param[in] pst      post structure
 *  @param[in] measType meas Type 
 *  @return S16
 *      -# Success : ROK
 *      -# Failure : RFAILED
*/

S16 rlcDlUdxL2MeasStopReq
(
Pst            *pst,
uint8_t         measType
)
{
  /* S16 ret = ROK;*/
   RlcL2MeasEvtCb *measEvtCb = NULLP;
   uint16_t        cntr;
   uint8_t         status = ROK;
/*   RlcL2MeasCfmEvt          measCfmEvt;  */
   volatile uint32_t     startTime = 0;
   RlcCb     *tRlcCb=NULLP;
   
   /*starting Task*/
   SStartTask(&startTime, PID_RLC_MEAS_STOP);

   tRlcCb =  RLC_GET_RLCCB(pst->dstInst);
/*   memset(&measCfmEvt, 0, sizeof(RlcL2MeasCfmEvt)); */
   /* reset the counters for the measurement type passed */
   for(cntr = 0; cntr < LKW_MAX_L2MEAS; cntr++)
   {
      measEvtCb = &(tRlcCb->u.dlCb->rlcL2Cb.rlcL2EvtCb[cntr]);
      if(measEvtCb->measCb.measType & measType)
      {
         rlcUtlResetDlL2MeasInRlcRb(tRlcCb, &measEvtCb->measCb, measType);

      }
   }

   /* switch off the measurements for the type passed */
   for(cntr = 0; cntr < LKW_MAX_QCI; cntr++)
   {
      tRlcCb->u.dlCb->rlcL2Cb.measOn[cntr] &= ~measType;
   }
   
   status = LCM_PRIM_OK;
   /* Stop confirm is removed as UL thread is already sending it */
   
   /*stopping Task*/
   SStopTask(startTime, PID_RLC_MEAS_STOP);

   return ROK;
}
/**
@brief 
This function processes L2 Measurement Send request received from the layer manager.
After receving this request, RLC sends L2 Measurement
 *  @param[in] pst      post structure
 *  @param[in] measType meas Type 
 *  @return S16
 *      -# Success : ROK
 *      -# Failure : RFAILED
*/

S16 rlcDlUdxL2MeasSendReq
(
Pst            *pst,
uint8_t         measType
)
{
   RlcL2MeasEvtCb *measEvtCb;
   uint16_t            cntr;
   volatile uint32_t     startTime = 0;
   RlcCb     *tRlcCb;

   tRlcCb =  RLC_GET_RLCCB(pst->dstInst);
   for(cntr = 0; cntr < LKW_MAX_L2MEAS; cntr++)
   {
      measEvtCb = &(tRlcCb->u.dlCb->rlcL2Cb.rlcL2EvtCb[cntr]);
      if(measEvtCb->measCb.measType & measType)
      {
         /*starting Task*/
         SStartTask(&startTime, PID_RLC_MEAS_REPORT);

         rlcUtlSndDlL2MeasCfm(tRlcCb, measEvtCb);

         /*stopping Task*/
         SStopTask(startTime, PID_RLC_MEAS_REPORT);
      }
   }

   return ROK;
}
#endif /* LTE_L2_MEAS */

#ifdef __cplusplus
}
#endif /* __cplusplus */


/**********************************************************************
         End of file
**********************************************************************/
