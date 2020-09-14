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
  
     Name:    LTE-RLC Layer - Lower Interface Functions
    
     Type:    C file
  
     Desc:     Source code for RLC Lower Interface Module.
               This file contains following functions
      
        --RlcLiRguBndCfm
        --RlcLiRguCDatInd
        --RlcLiRguDDatInd
        --RlcLiRguCStaInd
        --RlcLiRguDStaInd
        --RlcLiRguHqStaInd

     File:     kw_lim.c

**********************************************************************/
static const char* RLOG_MODULE_NAME="LIM";
static int RLOG_MODULE_ID=2048;
static int RLOG_FILE_ID=196;

/** 
 * @file kw_lim.c
 * @brief RLC Lower Interface module
*/

#define RLC_MODULE RLC_DBGMASK_INF


/* header (.h) include files */
#include "common_def.h"
#include "lkw.h"           /* LKW defines */
#include "ckw.h"           /* CKW defines */
#include "kwu.h"           /* KWU defines */
#include "rgu.h"           /* RGU defines */
#include "kw_err.h"
#include "kw_env.h"        /* RLC environment options */

#include "kw.h"            /* RLC defines */
#include "kw_udx.h"
#include "kw_dl.h"
#include "kw_ul.h"

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

#ifdef __cplusplus
EXTERN "C" {
#endif /* __cplusplus */


/*****************************************************************************
 *                          RGU INTERFACE
 ****************************************************************************/
/**
 * @brief Handler for bind confirmation from MAC.
 *
 * @details
 *    This function handles the bind confirmation received from MAC. If the 
 *    bind was successful changes the state of the SAP to RLC_SAP_BND 
 *    else RLC_SAP_CFG. Sends an alarm to LM in any case
 *
 * @param[in] pst     Post structure
 * @param[in] suId    Service User ID
 * @param[in] status  Status whether the bind was successful or not
 *
 * @return  S16
 *    -# ROK 
 *    -# RFAILED 
 *
*/
#ifdef ANSI
S16 RlcLiRguBndCfm
(
Pst    *pst,   
SuId   suId, 
U8     status 
)
#else
S16 RlcLiRguBndCfm (pst, suId, status)
Pst    *pst; 
SuId   suId;
U8     status;
#endif
{
   U16          event;     /* Event */
   U16          cause;     /* Cause */
   RlcRguSapCb   *rguSap;   /* RGU SAP Control Block */
   RlcCb         *tRlcCb;

   TRC3(RlcLiRguBndCfm)

#if (ERRCLASS & ERRCLS_INT_PAR)
   if (pst->dstInst >= MAX_RLC_INSTANCES)
   {
      return  (RFAILED);
   }
#endif
   tRlcCb = RLC_GET_RLCCB(pst->dstInst);

   RLOG2(L_DEBUG,"RlcLiRguBndCfm(suId(%d), status(%d)", suId, status);

#if (ERRCLASS & ERRCLS_INT_PAR)
   if (tRlcCb->init.cfgDone != TRUE)
   {
      RLOG0(L_FATAL,"General configuration not done");
      
      RLC_SEND_SAPID_ALARM(tRlcCb,suId,LKW_EVENT_LI_BND_CFM, LCM_CAUSE_INV_STATE);

      return RFAILED;
   }

   if ((suId >= tRlcCb->genCfg.maxRguSaps) || (suId < 0))
   {
      RLOG0(L_ERROR, "Invalid suId");
      
      RLC_SEND_SAPID_ALARM(tRlcCb,suId, LKW_EVENT_LI_BND_CFM, LCM_CAUSE_INV_SUID);

      return RFAILED;
   }
#endif /* ERRCLASS & ERRCLS_INT_PAR */

   rguSap = (tRlcCb->genCfg.rlcMode == LKW_RLC_MODE_DL) ?
            &(tRlcCb->u.dlCb->rguDlSap[suId]) : &(tRlcCb->u.ulCb->rguUlSap[suId]);

   RLOG1(L_DEBUG, "RlcLiRguBndCfm: For RGU SAP state=%d", rguSap->state)

   switch (rguSap->state)
   {
      case RLC_SAP_BINDING:
      {
         rlcStopTmr (tRlcCb,(PTR)rguSap, RLC_EVT_WAIT_BNDCFM);

         rguSap->retryCnt = 0;
          
         if (status == CM_BND_OK)
         {
            rguSap->state = RLC_SAP_BND;
            event = LCM_EVENT_BND_OK;
            cause = LKW_CAUSE_SAP_BNDENB;
         }
         else
         {
            rguSap->state = RLC_SAP_CFG;
            event = LCM_EVENT_BND_FAIL;
            cause = LKW_CAUSE_UNKNOWN;
         }

         break;
      }
      default:
         event = LKW_EVENT_RGU_BND_CFM;
         cause = LCM_CAUSE_INV_STATE;
         break;
   }

   /* Send an alarm with proper event and cause */
   RLC_SEND_SAPID_ALARM(tRlcCb, suId, event, cause);

   return (ROK);
} /* RlcLiRguBndCfm */

/**
 * @brief Handler to process PDU received from MAC
 *
 * @details
 *    This function receives the PDU from MAC.
 *    seggregates common and dedicated logical channel
 *    PDU and call respective handler.
 *
 * @param[in] pst     Post structure
 * @param[in] suId    Service User ID
 * @param[in] datInd  Data Indication Information 
 *
 * @return  S16
 *    -# ROK 
 *    -# RFAILED 
 *
*/

uint8_t RlcProcUlData(Pst *pst, RlcMacData *ulData)
{
   U8              idx;
   U8              lcId;                    /* Logical Channel */
   U8              numDLch = 0;             /* Number of dedicated logical channel */
   Bool            dLchPduPres;             /* PDU received on dedicated logical channel */
   RguLchDatInd    dLchData[RGU_MAX_LC];  /* PDU info on dedicated logical channel */
   RguDDatIndInfo  *dLchUlDat;               /* UL data on dedicated logical channel */
   RguCDatIndInfo  *cLchUlDat;               /* UL data on common logical channel */
   uint16_t        copyLen;

   /* Initializing dedicated logical channel Database */
   for(idx = 0; idx < RGU_MAX_LC; idx++)
   {
      dLchData[idx].lcId = idx;
      dLchData[idx].pdu.numPdu = 0;
   }
   
   dLchPduPres = FALSE;
  
   /* Seggregate PDUs received on common and dedicated channels
    * and call common channel's handler */
   for(idx = 0; idx< ulData->numPdu; idx++)
   {
      if(ulData->pduInfo[idx].commCh)
      {
	 RLC_SHRABL_STATIC_BUF_ALLOC(pst->region, pst->pool, cLchUlDat, sizeof(RguCDatIndInfo));
	 cmMemset((U8*)cLchUlDat, (U8)0, sizeof(RguCDatIndInfo));

	 cLchUlDat->cellId = ulData->cellId;
	 GET_UE_IDX(ulData->rnti, cLchUlDat->rnti);
	 cLchUlDat->lcId   = ulData->pduInfo[idx].lcId;

	 /* Copy fixed buffer to message */
	 if(SGetMsg(RLC_MEM_REGION_UL, RLC_POOL, &cLchUlDat->pdu) != ROK)
	 {
	    DU_LOG("\nRLC : Memory allocation failed at RlcProcUlData");
	    return RFAILED;
	 }
	 reverseFixBuf(ulData->pduInfo[idx].pduBuf, ulData->pduInfo[idx].pduLen);
	 SCpyFixMsg(ulData->pduInfo[idx].pduBuf, cLchUlDat->pdu, 0, \
	       ulData->pduInfo[idx].pduLen, (MsgLen *)&copyLen);

	 RlcLiRguCDatInd(pst, 0, cLchUlDat);
      } 
      else
      {
	 if(!dLchPduPres)
	 {
	    RLC_SHRABL_STATIC_BUF_ALLOC(pst->region, pst->pool, dLchUlDat, sizeof(RguDDatIndInfo));
	    dLchPduPres = TRUE;
	 }

	 lcId = ulData->pduInfo[idx].lcId;
	 dLchData[lcId].pdu.mBuf[dLchData[lcId].pdu.numPdu] = ulData->pduInfo[idx].pduBuf;
	 dLchData[lcId].pdu.numPdu++; 
      }
   }
 
   /* If any PDU received on dedicated logical channel, copy into RguDDatIndInfo
    * and call its handler */ 
   if(dLchPduPres)
   {
      dLchUlDat->cellId = ulData->cellId;
      dLchUlDat->rnti   = ulData->rnti;

      for(idx = 0; idx < RGU_MAX_LC; idx++)
      {
	 if(dLchData[idx].pdu.numPdu)
	 {
	    cmMemcpy((U8 *)&dLchUlDat->lchData[numDLch], (U8 *)&dLchData[idx], sizeof(RguLchDatInd));
	    numDLch++;      
	 }
      }
      dLchUlDat->numLch = numDLch;
      RlcLiRguDDatInd(pst, 0, dLchUlDat);
   }
    

   RLC_FREE_SHRABL_BUF(pst->region, pst->pool, ulData, sizeof(RlcMacData));
   return ROK;
   
}/* End of RlcProcUlData */

int   rlcDDatIndRcvd;
int   rlcCDatIndRcvd;
/**
 * @brief Handler to process PDU received from MAC for common logical channels. 
 *
 * @details
 *    This function receives the PDU from MAC for common logical channels
 *    does checks before handing over the PDU to the TM module
 *
 * @param[in] pst     Post structure
 * @param[in] suId    Service User ID
 * @param[in] datInd  Data Indication Information 
 *
 * @return  S16
 *    -# ROK 
 *    -# RFAILED 
 *
*/
#ifdef ANSI
S16 RlcLiRguCDatInd
(
Pst              *pst,   
SuId             suId,  
RguCDatIndInfo   *datInd
)
#else
S16 RlcLiRguCDatInd(pst,suId,datInd)
Pst              *pst; 
SuId             suId;
RguCDatIndInfo   *datInd;
#endif
{
   RlcUlRbCb   *rbCb; 
   RlcCb       *tRlcCb; 

   rlcCDatIndRcvd++;
   TRC3(RlcLiRguCDatInd)

#if (ERRCLASS & ERRCLS_INT_PAR)
   if (pst->dstInst >= MAX_RLC_INSTANCES)
   {
      RLC_SHRABL_STATIC_BUF_FREE(pst->region, pst->pool, datInd, sizeof(RguCDatIndInfo));
      return  (RFAILED);
   }
#endif

   tRlcCb = RLC_GET_RLCCB(pst->dstInst);


#if (ERRCLASS & ERRCLS_DEBUG)
   if (tRlcCb->genCfg.rlcMode == LKW_RLC_MODE_DL)
   {
      RLC_SHRABL_STATIC_BUF_FREE(pst->region, pst->pool, datInd, sizeof(RguCDatIndInfo));
      return RFAILED;
   }
#endif

   /* kw006.201 ccpu00120058, Added array boundary condition check */
#if (ERRCLASS & ERRCLS_DEBUG)
   if(RLC_MAX_LCH_PER_CELL <= datInd->lcId)
   {
      RLOG_ARG1(L_ERROR,DBG_LCID,datInd->lcId, "Invalid LcId, Max is [%d]",
         RLC_MAX_LCH_PER_CELL);
      RLC_SHRABL_STATIC_BUF_FREE(pst->region, pst->pool, datInd, sizeof(RguCDatIndInfo));
      return RFAILED;
   }
#endif /* (ERRCLASS & ERRCLS_DEBUG) */

   /* Fetch RbCb from lcId */
   rlcDbmFetchUlRbCbFromLchId(tRlcCb, datInd->rnti, datInd->cellId, datInd->lcId, &rbCb);
   if (!rbCb)
   {
      RLOG_ARG1(L_ERROR, DBG_CELLID,datInd->cellId, "LcId [%d] not found",
         datInd->lcId);
      RLC_SHRABL_STATIC_BUF_FREE(pst->region, pst->pool, datInd, sizeof(RguCDatIndInfo));
      return RFAILED;
   }

   /* Dispatch to TM Module */
#ifdef CCPU_OPT
   rlcTmmRcvFrmLi(tRlcCb, rbCb, datInd->rnti, datInd->pdu);
#else /* CCPU_OPT */
   rlcTmmRcvFrmLi(tRlcCb, rbCb, datInd->pdu);
#endif /* CCPU_OPT */

   RLC_SHRABL_STATIC_BUF_FREE(pst->region, pst->pool, datInd, sizeof(RguCDatIndInfo));

   return (ROK);
} /* RlcLiRguCDatInd */

/**
 * @brief Handler to process PDU received from MAC for 
 *        dedicated logical channels. 
 *
 * @details
 *    This function receives the PDU from MAC for one or more dedicated 
 *    logical channels and passes it to the UTL module for further processing
 *
 * @param[in] pst     Post structure
 * @param[in] suId    Service User ID
 * @param[in] datInd  Data Indication Information 
 *
 * @return  S16
 *    -# ROK 
 *    -# RFAILED 
 *
*/
 
#ifdef ANSI
S16 RlcLiRguDDatInd
(
Pst              *pst,   
SuId             suId,  
RguDDatIndInfo   *datInd
)
#else
S16 RlcLiRguDDatInd(pst, suId, datInd)
Pst              *pst; 
SuId             suId;
RguDDatIndInfo   *datInd;
#endif
{
   TRC3(RlcLiRguDDatInd)

   rlcDDatIndRcvd++;
#if (ERRCLASS & ERRCLS_INT_PAR)
   if (pst->dstInst >= MAX_RLC_INSTANCES)
   {
       RLC_SHRABL_STATIC_BUF_FREE(pst->region, pst->pool, datInd, sizeof(RguDDatIndInfo));
       return  (RFAILED);
   }
#endif

#if (ERRCLASS & ERRCLS_DEBUG)
   if (((RlcCb*)RLC_GET_RLCCB(pst->dstInst))->genCfg.rlcMode == LKW_RLC_MODE_DL)
   {
       RLOG1(L_DEBUG,"RlcLiRguDDatInd(pst, suId(%d))recieved in DL Inst",suId);
       RLC_SHRABL_STATIC_BUF_FREE(pst->region, pst->pool, datInd, sizeof(RguDDatIndInfo));
       return RFAILED;
   }
#endif
   rlcUtlRcvFrmLi(RLC_GET_RLCCB(pst->dstInst),datInd);
#ifndef SS_RBUF 
#ifdef SS_LOCKLESS_MEMORY
   RLC_SHRABL_STATIC_BUF_FREE(pst->region, pst->pool, datInd, sizeof(RguDDatIndInfo));
#else
   RLC_PST_FREE(pst->region, pst->pool, datInd, sizeof(RguDDatIndInfo));
#endif
#endif

   return (ROK);
} /* RlcLiRguDDatInd */


/*******************************************************************
 *
 * @brief Handler for extracting common and dedicated channel
 *      Scheduling result report. 
 *
 * @details
 *
 *    Function : RlcMacProcSchedRep
 *
 *    Functionality:
 *     Handler for extracting common and dedicated channel
 *      Scheduling result report
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint16_t RlcMacProcSchedRep(Pst *pst, SuId suId, RlcMacSchedRepInfo *schRep)
{
   U8 idx;                     /* Iterator */
   U8 nmbDLch = 0;                 /* Number of dedicated logical channles */
   RguCStaIndInfo   *cLchSchInfo;    /* Common logical channel scheduling result */
   RguDStaIndInfo   *dLchSchInfo;  /* Dedicated logical channel scheduling result */

   DU_LOG("\nRLC : Received scheduling report from MAC");

   for(idx=0; idx < schRep->nmbLch; idx++)
   {
       /* If it is common channel, fill status indication information 
        * and trigger the handler for each common lch separately */
       if(schRep->lchSta[idx].commCh)
       {
          RLC_SHRABL_STATIC_BUF_ALLOC(pst->region, pst->pool, cLchSchInfo, sizeof(RguCStaIndInfo));
          cmMemset((U8*)cLchSchInfo, (U8)0, sizeof(RguCStaIndInfo)); 

          cLchSchInfo->cellId  = schRep->cellId;
          cLchSchInfo->lcId    = schRep->lchSta[idx].lchStaInd.lcId;
          //cLchSchInfo->transId = schRep->timeToTx;  /* TODO : fill transId suing timeToTx */
          cLchSchInfo->rnti    = schRep->rnti;

          RlcLiRguCStaInd(pst, suId, cLchSchInfo);
          
       }
       else
       {
          /* Fill status info structure if at least one dedicated channel 
           * scheduling report is received */
          if(nmbDLch == 0)
          {
             RLC_SHRABL_STATIC_BUF_ALLOC(pst->region, pst->pool, dLchSchInfo, sizeof(RguDStaIndInfo));

             dLchSchInfo->cellId = schRep->cellId;
             dLchSchInfo->nmbOfUeGrantPerTti = 1;
             dLchSchInfo->staInd[0].rnti = schRep->rnti;
             //dLchSchInfo->staInd[0].transId = schRep->timeToTx;  /* TODO : fill transId suing timeToTx */
             dLchSchInfo->staInd[0].nmbOfTbs = 1;
             //dLchSchInfo->staInd[0].fillCrlPdu = /* TODO : Check the value needed to be filled */
          }

          /* Fill logical channel scheduling info */
          cmMemcpy((U8 *)&dLchSchInfo->staInd[0].staIndTb[0].lchStaInd[nmbDLch], (U8 *)&schRep->lchSta[idx].lchStaInd, sizeof(RguLchStaInd));
          nmbDLch++;

       }

   }

   /* Calling handler for all dedicated channels scheduling*/
   if(nmbDLch)
   {
      dLchSchInfo->staInd[0].staIndTb[0].nmbLch = nmbDLch;
      RlcLiRguDStaInd(pst, suId, dLchSchInfo);
   }
   
   RLC_SHRABL_STATIC_BUF_FREE(pst->region, pst->pool, schRep, sizeof(RlcMacSchedRepInfo));

   return ROK;
}

/**
 * @brief Handler for trigerring the data transfer from RLC to MAC
 *        for common logical channels.
 *
 * @details
 *    This function receives the size of the PDU to be transmitted
 *    and acts as a trigger for forming PDU and sending it to MAC. 
 *
 * @param[in] pst       Post structure
 * @param[in] suId      Service User ID
 * @param[in] staInd    Status Indication Information for Common Logical 
 *                      Channels
 *
 * @return  S16
 *    -# ROK 
 *    -# RFAILED 
 *
*/ 
#ifdef ANSI
S16 RlcLiRguCStaInd
(
Pst              *pst,  
SuId             suId,
RguCStaIndInfo   *staInd  
)
#else
S16 RlcLiRguCStaInd(pst,suId,staInd)
Pst              *pst;   
SuId             suId; 
RguCStaIndInfo   *staInd; 
#endif
{
   RlcDlRbCb   *rbCb;  
   RlcCb       *tRlcCb;

   TRC3(RlcLiRguCStaInd)

#if (ERRCLASS & ERRCLS_INT_PAR)
   if (pst->dstInst >= MAX_RLC_INSTANCES)
   {
      RLC_SHRABL_STATIC_BUF_FREE(pst->region, pst->pool, staInd, sizeof(RguCStaIndInfo));
      return  (RFAILED);
   }
#endif

   tRlcCb = RLC_GET_RLCCB(pst->dstInst);


#if (ERRCLASS & ERRCLS_INT_PAR)
   if ((suId >= tRlcCb->genCfg.maxRguSaps) || (suId < 0))
   {
      RLCLOGERROR(tRlcCb,
            ERRCLS_INT_PAR, 
            EKW040, 
            (ErrVal) suId,
            "RlcLiRguCStaInd: Invalid RGU suId\n");
      return (RFAILED); 
   }
   if (tRlcCb->genCfg.rlcMode == LKW_RLC_MODE_UL)
   {
       RLOG_ARG1(L_ERROR,DBG_LCID,staInd->lcId,
             "Received in RLC UL CELLID:%d",
             staInd->cellId);
       RLC_SHRABL_STATIC_BUF_FREE(pst->region, pst->pool, staInd, sizeof(RguCStaIndInfo));
       return RFAILED;
   }
#endif

   rbCb = NULLP;

   /* kw006.201 ccpu00120058, added boundary condition check */
#if (ERRCLASS & ERRCLS_DEBUG)
   if(RLC_MAX_LCH_PER_CELL < staInd->lcId)
   {
      RLOG_ARG2(L_ERROR,DBG_LCID,staInd->lcId, 
            "Invalid LcId, Max is [%d] CELLID:%d",
            RLC_MAX_LCH_PER_CELL,
            staInd->cellId);
      RLC_SHRABL_STATIC_BUF_FREE(pst->region, pst->pool, staInd, sizeof(RguCStaIndInfo));
      return RFAILED;
   }
#endif /* (ERRCLASS & ERRCLS_DEBUG) */
   /* Fertch RbCb from lcId */
   rlcDbmFetchDlRbCbFromLchId(tRlcCb,0, staInd->cellId, staInd->lcId, &rbCb);
   if(!rbCb)                                               
   {
      RLOG_ARG1(L_ERROR, DBG_CELLID,staInd->cellId, 
            "LcId [%d] not found CELLID:%d",
            staInd->lcId);
      RLC_SHRABL_STATIC_BUF_FREE(pst->region, pst->pool, staInd, sizeof(RguCStaIndInfo));
      return RFAILED;
   }

   /* Dispatch to TM Module */
   rbCb->transId = staInd->transId;
   /* ccpu00136940 */
  /* If trace flag is enabled send the trace indication */
   if(tRlcCb->init.trc == TRUE)
   {
     /* Populate the trace params */
      rlcLmmSendTrc(tRlcCb,EVTRGUCSTAIND, NULLP);
   }                            
   rlcTmmSndToLi(tRlcCb, suId, rbCb, staInd);
#ifndef SS_RBUF
#ifdef SS_LOCKLESS_MEMORY
   RLC_SHRABL_STATIC_BUF_FREE(pst->region, pst->pool, staInd, sizeof(RguCStaIndInfo));
#else
   RLC_PST_FREE(pst->region, pst->pool, staInd, sizeof(RguCStaIndInfo));
#endif
#else
   RLC_SHRABL_STATIC_BUF_FREE(pst->region, pst->pool, staInd, sizeof(RguCStaIndInfo));
#endif
   return (ROK);
} /* RlcLiRguCStaInd */

/**
 * @brief Handler for trigerring the data transfer from RLC to MAC
 *        for dedicated logical channels.
 *
 * @details
 *    This function receives the size of the PDUs to be transmitted to
 *    MAC via one or more dedicated logical channels and acts as a trigger
 *    for forming PDUs and sending them to MAC. 
 *
 * @param[in] pst       Post structure
 * @param[in] suId      Service User ID
 * @param[in] staInd    Status Indication Information for Dedicated Logical 
 *                      Channels
 *
 * @return  S16
 *    -# ROK 
 *    -# RFAILED 
 *
*/ 
#ifdef ANSI
S16 RlcLiRguDStaInd
(
Pst              *pst,  
SuId             suId,
RguDStaIndInfo   *staInd 
)
#else
S16 RlcLiRguDStaInd(pst, suId, staInd)
Pst              *pst; 
SuId             suId; 
RguDStaIndInfo   *staInd; 
#endif
{
   RlcCb        *gCb;
   TRC3(RlcLiRguDStaInd)

#if (ERRCLASS & ERRCLS_INT_PAR)
   if (pst->dstInst >= MAX_RLC_INSTANCES)
   {
      RLC_SHRABL_STATIC_BUF_FREE(pst->region, pst->pool, staInd, sizeof(RguDStaIndInfo));
      return  (RFAILED);
   }
#endif

   gCb = RLC_GET_RLCCB(pst->dstInst);

#if (ERRCLASS & ERRCLS_INT_PAR)
   if (((RlcCb*)RLC_GET_RLCCB(pst->dstInst))->genCfg.rlcMode == LKW_RLC_MODE_UL)
   {
       RLOG_ARG0(L_ERROR,DBG_CELLID,staInd->cellId,"Received in RLC UL ");
       RLC_SHRABL_STATIC_BUF_FREE(pst->region, pst->pool, staInd, sizeof(RguDStaIndInfo));
       return RFAILED;
   }
   if ((suId >= gCb->genCfg.maxRguSaps) || (suId < 0))
   {
      RLCLOGERROR(gCb,
            ERRCLS_INT_PAR, 
            EKW040, 
            (ErrVal) suId,
            "RlcLiRguDStaInd: Invalid RGU suId\n");
      return (RFAILED); 
   }
#endif
   rlcUtlSndToLi(gCb, suId, staInd);

   /* kw002.201 :Freeing from proper region */
   RLC_SHRABL_STATIC_BUF_FREE(pst->region, pst->pool, staInd, sizeof(RguDStaIndInfo));
   return (ROK);
} /* RlcLiRguDStaInd */

/**
 * @brief Handler for handling the flow cntrl Ind from MAC
 *  to RLC      
 *
 * @details
 *   This function receives the flow control indication from
 *   MAC and calls rlcUtlTrigPdbFlowCntrl
 *
 * @param[in] pst       Post structure
 * @param[in] suId      Service User ID
 * @param[in] flowCntrlInd flow control Indication Information 
 *           from MAC  
 *
 * @return  S16
 *    -# ROK 
 *    -# RFAILED 
 *
*/ 
#ifdef ANSI
S16 RlcLiRguFlowCntrlInd
(
Pst              *pst,  
SuId             suId,
RguFlowCntrlInd *flowCntrlInd
)
#else
S16 RlcLiRguFlowCntrlInd(pst, suId, flowCntrlInd)
Pst              *pst; 
SuId             suId; 
RguFlowCntrlInd  *flowCntrlInd; 
#endif
{
   RlcCb       *tRlcCb; 
   RlcDlRbCb   *rbCb = NULLP;
   U32        idx;
   U32        lcIdx;

   tRlcCb = RLC_GET_RLCCB(pst->dstInst);
   for (idx = 0; idx < flowCntrlInd->numUes; idx++)
   {
      for (lcIdx = 0; lcIdx < flowCntrlInd->ueFlowCntrlInfo[idx].numLcs; lcIdx++)  
      {
         RguLcFlowCntrlInfo *lcInfo = &(flowCntrlInd->ueFlowCntrlInfo[idx].lcInfo[lcIdx]); 
         rlcDbmFetchDlRbCbFromLchId(tRlcCb, flowCntrlInd->ueFlowCntrlInfo[idx].ueId, flowCntrlInd->cellId, lcInfo->lcId, &rbCb);
         if (rbCb)
         {
           
            if (lcInfo->pktAdmitCnt == 0) /* Special case */
            {
               rlcUtlTrigPdbFlowCntrl(tRlcCb, rbCb, lcInfo->pktAdmitCnt);
               continue;
            }
            if (rbCb->mode == CM_LTE_MODE_AM)
            {
               if ((rbCb->m.amDl.retxLst.count != 0) || 
                   ((rbCb->m.amDl.bo == 0) || 
                    (rbCb->m.amDl.bo < lcInfo->maxBo4FlowCtrl)))
               {
                  continue;
               }
            }
            else /* UM */
            {
               if ((rbCb->m.umDl.bo == 0) ||
                   (rbCb->m.umDl.bo < lcInfo->maxBo4FlowCtrl))
               {
                  continue;
               }
            }
            rlcUtlTrigPdbFlowCntrl(tRlcCb, rbCb, lcInfo->pktAdmitCnt);
         }
      }
   }
   return ROK;
}
/* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS

/**
 *
 * @brief  
 *
 *        Handler for indicating the Harq Status of the data sent.
 *
 * @b Description:
 *
 *        This function receives the harq status of the data sent to MAC.
 *        This information is used for two things.
 *        1. Computing the UuLoss of UM
 *        2. Computing the DL Delay for UM and AM.
 *
 *  @param[in] pst     -   Post structure  
 *  @param[in] suId    -   Service User ID
 *  @param[in] staInd  -   Harq Status Indication Information. 
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 *
 */
#ifdef ANSI
S16 RlcLiRguHqStaInd
(
Pst                *pst, 
SuId               suId,
RguHarqStatusInd   *staInd 
)
#else
S16 RlcLiRguHqStaInd(pst,suId,staInd)
Pst                *pst;  
SuId               suId; 
RguHarqStatusInd   *staInd;
#endif
{

   RlcUeKey   ueKey;
   S16       ret; 
   RlcDlUeCb    *ueCb;
   U8        tbIdx;
   RlcCb       *tRlcCb; 

   TRC3(RlcLiRguHqStaInd)

   tRlcCb = RLC_GET_RLCCB(pst->dstInst);
   ueKey.cellId = staInd->cellId;
   ueKey.ueId   = staInd->ueId;

   ret = rlcDbmFetchDlUeCb(tRlcCb, ueKey.ueId, ueKey.cellId, &ueCb);
   if (ret != ROK )
   {
      return RFAILED;
   }

   /*Call rlcUtlProcHarqInd as many times as number of Tbs present*/
   for ( tbIdx = 0; tbIdx < staInd->numTbs; tbIdx++)
   {
      rlcUtlProcHarqInd(tRlcCb, staInd, ueCb, tbIdx);
   }

   return (ROK);
} /* RlcLiRguHqStaInd */
#endif /* LTE_L2_MEAS */

#ifdef __cplusplus
}
#endif /* __cplusplus */

/********************************************************************30**
         End of file
**********************************************************************/
