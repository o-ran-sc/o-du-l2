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

     Name:     LTE-RLC Layer 
  
     Type:     C file
  
     Desc:     Source code for RLC Transparent mode assembly and
               reassembly.This file contains following functions
                
                  --kwTmmQSdu
                  --kwTmmSndToLi
                  --kwTmmRcvFrmLi
                  --kwTmmReEstablish 

     File:     kw_tmm_dl.c

**********************************************************************/
static const char* RLOG_MODULE_NAME="TMM";
static int RLOG_MODULE_ID=2048;
static int RLOG_FILE_ID=200;
/** 
 * @file kw_tmm_dl.c
 * @brief RLC Transparent Mode module
*/

/* header (.h) include files */
#include "common_def.h"
#include "lkw.h"           /* LKW defines */
#include "ckw.h"           /* CKW defines */
#include "kwu.h"           /* KWU defines */
#include "rgu.h"           /* RGU defines */
#include "kw_env.h"        /* RLC environment options */

#include "kw.h"            /* RLC defines */
#include "kw_err.h"            /* RLC defines */
#include "kw_udx.h"
#include "kw_dl.h"

/* extern (.x) include files */
#include "lkw.x"           /* LKW */
#include "ckw.x"           /* CKW */
#include "kwu.x"           /* KWU */
#include "rgu.x"           /* RGU */

#include "kw.x"
#include "kw_udx.x"
#include "kw_dl.x"

#define KW_MODULE (KW_DBGMASK_TM | KW_DBGMASK_DL)

PRIVATE Void kwTmmSndStaRsp ARGS((RlcCb *gCb, RlcDlRbCb *rbCb, 
                                 MsgLen bo, KwuDatReqInfo *datReqInfo));
extern U32 rgMacGT ;  
/** @addtogroup tmmode */
/*@{*/

/**
 * @brief 
 *    Handler to queue the SDU in the SDU queue and update BO and report it to
 *    the lower layer.
 *
 * @details 
 *    This function is used to queue the received SDU in the SDU queue 
 *    maintained in the radio bearer control block. After queuing the SDU, BO 
 *    is updated and is reported to the lower layer. 
 *            
 *  @param[in] rbCb         RB control block. 
 *  @param[in] datReqInfo   Data Request Information.
 *  @param[in] mBuf         SDU Buffer.
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 */
#ifdef ANSI
PUBLIC Void kwTmmQSdu
(
RlcCb            *gCb,
RlcDlRbCb        *rbCb,      
KwuDatReqInfo   *datReqInfo, 
Buffer          *mBuf       
)
#else
PUBLIC Void kwTmmQSdu(gCb,rbCb,datReqInfo,mBuf)
RlcCb            *gCb;
RlcDlRbCb        *rbCb;       
KwuDatReqInfo   *datReqInfo;  
Buffer          *mBuf;         
#endif
{
   KwSdu   *sdu;              

   TRC2(kwTmmQSdu) 


      RLC_ALLOC(gCb,sdu,sizeof(KwSdu));
#if (ERRCLASS & ERRCLS_ADD_RES)
   if ( sdu == NULLP )
   {
      RLOG_ARG2(L_FATAL,DBG_RBID,rbCb->rlcId.rbId,
	    "Memory Allocation failed UEID:%d CELLID:%d",   
	    rbCb->rlcId.ueId,
	    rbCb->rlcId.cellId);   
      RETVOID;
   }
#endif /* ERRCLASS & ERRCLS_ADD_RES */
#ifdef CCPU_OPT   
   if ( rbCb->lch.lChType == CM_LTE_LCH_BCCH || 
	 rbCb->lch.lChType == CM_LTE_LCH_PCCH )
   {
      sdu->mode.tm.sfn = datReqInfo->tm.tmg.sfn;
      sdu->mode.tm.slot = datReqInfo->tm.tmg.slot;
#ifdef EMTC_ENABLE
      if(rbCb->lch.lChType == CM_LTE_LCH_PCCH)
      {
	 sdu->mode.tm.pnb = datReqInfo->pnb;
      }
#endif
   }
   else
   {
      sdu->mode.tm.rnti = datReqInfo->tm.rnti;
   }
#endif
   sdu->arrTime = rgMacGT;
   SFndLenMsg(mBuf,&sdu->sduSz); 
   sdu->mBuf = mBuf;

   cmLListAdd2Tail(&(rbCb->m.tm.sduQ), &(sdu->lstEnt));  
   sdu->lstEnt.node = (PTR)sdu; 

   kwTmmSndStaRsp(gCb, rbCb, sdu->sduSz, datReqInfo); 
   RETVOID;
}

/**
 * @brief 
 *    Handler to form a pdu and send it to the lower layer.
 * 
 * @details 
 *    This function forms one pdu from the first SDU in the SDU queue and sends 
 *    it to the lower layer.
 *             
 * @param[in] gCb     RLC Instance Control Block
 * @param[in] rbCb    RB control block. 
 * @param[in] staInd  Status Indication of common logical channel 
 *
 * @return  S16
 *    -# ROK 
 *    -# RFAILED         
 */
#ifdef ANSI
   PUBLIC Void kwTmmSndToLi
(
 RlcCb             *gCb,
 SuId             suId,
 RlcDlRbCb         *rbCb,              
 RguCStaIndInfo   *staInd
 )
#else
PUBLIC Void kwTmmSndToLi(gCb, suId, rbCb, staInd)
   RlcCb             *gCb;
   SuId             suId;
   RlcDlRbCb         *rbCb;             
   RguCStaIndInfo   *staInd;
#endif
{
   CmLList          *node;          /* Current Link List Node */
   KwSdu            *sdu;           /* SDU */
   RlcMacData       *dlData;
   S16   timeDiff = 0;
   Ticks curTime  = 0;

   TRC2(kwTmmSndToLi)


      CM_LLIST_FIRST_NODE(&(rbCb->m.tm.sduQ), 
	    node);

   /* (Sfn,slot) at which the message should be transmitted is 
    * validated with alloted (sfn,slot)in the MAC layer */
   while (node != NULLP)
   {
      sdu = (KwSdu *)(node->node);
      if ( rbCb->lch.lChType == CM_LTE_LCH_BCCH ||
	    rbCb->lch.lChType == CM_LTE_LCH_PCCH )
      {
	 U16 sfn, slot;
	 /* MS_FIX: syed sfn is of 10 bytes rather than 8 */
#ifdef EMTC_ENABLE
	 /* As part of CATM feature cross slot scheduling is implemented , so there is some delta(currently 2)
	    between MPDCCH and PDSCH,RLC expects cell crntTime of transmission of control dlsf, so one extra 
	    information is provided in staInd, so that sfn,slot should calculate from paging Timing information 
	    in case of EMTC paging, instead of transId */
	 if(staInd->isEmtcPaging)
	 {
	    sfn      = staInd->pagingTimingInfo.sfn;
	    slot = staInd->pagingTimingInfo.slot;
	 }
	 else
#endif
	 {
	    sfn = (staInd->transId >> 8) & 0x3FF;
	    slot = staInd->transId & 0xFF;
	 }

	 /* Table
	  * tm.slot - current slot 
	  * 0,sfn        7,sfn-1
	  * 4,sfn        1,sfn
	  * 5,sfn        2,sfn
	  * 9,sfn        6,sfn
	  */
	 /* MS_FIX: syed Incorrect sfn determination. 
	  * Take care of SFN wraparound. TODO: It is better for RLC
	  * not to be aware of SCH DELTAs. So we should look for 
	  * sending actual transmission time to RLC. */
	 if ((slot + TFU_DELTA) >= 10)
	 {
	    sfn = (sfn + 1)%1024;
	 }

	 if ((sdu->mode.tm.sfn != sfn) ||
	       (sdu->mode.tm.slot != ((slot+TFU_DELTA)%10)))
	 {
	    node = node->next;
	    RLOG_ARG4(L_DEBUG,DBG_RBID,rbCb->rlcId.rbId,
		  "Releasing SDU of RNTI = %d for RNTI = %d UEID:%d CELLID:%d",
		  sdu->mode.tm.rnti, 
		  staInd->rnti,
		  rbCb->rlcId.ueId,
		  rbCb->rlcId.cellId);   
	    RLOG_ARG4(L_DEBUG,DBG_RBID,rbCb->rlcId.rbId,
		  "sfn %d slot %d  UEID:%d CELLID:%d",
		  sfn, 
		  slot,
		  rbCb->rlcId.ueId,
		  rbCb->rlcId.cellId);   
	    cmLListDelFrm(&(rbCb->m.tm.sduQ), &sdu->lstEnt);
	    RLC_FREE_BUF(sdu->mBuf);
	    RLC_FREE(gCb, sdu, sizeof(KwSdu));
	 }
	 else
	 {
	    break;
	 }
      }
      else
      {
	 curTime = rgMacGT;
	 if (curTime < sdu->arrTime)
	 {
	    timeDiff = (10240 - sdu->arrTime) + curTime;
	 }
	 else
	 {
	    timeDiff = curTime - sdu->arrTime;
	 }
	 RLOG_ARG4(L_DEBUG, DBG_RBID,rbCb->rlcId.rbId,
	       "TMM: TmSdu Sta Indication received for Rnti %d Sdu Rnti %d "
	       " UEID:%d CELLID:%d", 
	       staInd->rnti, 
	       sdu->mode.tm.rnti,
	       rbCb->rlcId.ueId,
	       rbCb->rlcId.cellId);   
	 RLOG_ARG4(L_DEBUG, DBG_RBID,rbCb->rlcId.rbId,
	       "TMM: TmSdu Sta Indication received : timeDiff %d SduQCnt %lu"
	       " UEID:%d CELLID:%d", 
	       timeDiff, 
	       rbCb->m.tm.sduQ.count,
	       rbCb->rlcId.ueId,
	       rbCb->rlcId.cellId);   
	 if (timeDiff > 40)
	 {
	    /* Memory leak needs to be fixed */
	    node = node->next;
	    RLOG_ARG3(L_DEBUG, DBG_RBID,rbCb->rlcId.rbId,
		  " timeDiff greater than 40, so deleting the Sdu %u "
		  " UEID:%d CELLID:%d", 
		  sdu->mode.tm.rnti,
		  rbCb->rlcId.ueId,
		  rbCb->rlcId.cellId);   
	    cmLListDelFrm(&(rbCb->m.tm.sduQ), &sdu->lstEnt);
	    RLC_FREE_BUF(sdu->mBuf);
	    RLC_FREE(gCb, sdu, sizeof(KwSdu));
	    continue;
	 }

	 if (sdu->mode.tm.rnti != staInd->rnti)
	 {
	    /* Memory leak needs to be fixed */
	    node = node->next;
	    RLOG_ARG4(L_DEBUG,DBG_RBID,rbCb->rlcId.rbId, 
		  "TMM: Searching for Rnti %d Skipping Sdu for Rnti %d"
		  " UEID:%d CELLID:%d", 
		  staInd->rnti,
		  sdu->mode.tm.rnti, 
		  rbCb->rlcId.ueId,
		  rbCb->rlcId.cellId);   
	    RLOG_ARG4(L_DEBUG,DBG_RBID,rbCb->rlcId.rbId, 
		  " timeDiff %d sdu->arrTime %d"
		  " UEID:%d CELLID:%d", 
		  timeDiff, 
		  sdu->arrTime,
		  rbCb->rlcId.ueId,
		  rbCb->rlcId.cellId);   
	    RLOG_ARG4(L_DEBUG,DBG_RBID,rbCb->rlcId.rbId, 
		  "curTime %d SduQCnt %lu and continuing"
		  " UEID:%d CELLID:%d", 
		  curTime, 
		  rbCb->m.tm.sduQ.count,
		  rbCb->rlcId.ueId,
		  rbCb->rlcId.cellId);   
	    continue;
	 }
	 else
	 {
	    RLOG_ARG3(L_DEBUG, DBG_RBID,rbCb->rlcId.rbId,
		  "TMM: TmSdu found %u UEID:%d CELLID:%d",
		  sdu->mode.tm.rnti,
		  rbCb->rlcId.ueId,
		  rbCb->rlcId.cellId);   
	    break;
	 }
      }

   }
   if ( node == NULLP )
   {
      RLOG_ARG2(L_ERROR,DBG_RBID,rbCb->rlcId.rbId,
	    "SDU not found TM Queue is empty UEID:%d CELLID:%d",
	    rbCb->rlcId.ueId,
	    rbCb->rlcId.cellId);   
      RETVOID;
   }
   sdu = (KwSdu *)node->node;

   RLC_ALLOC_SHRABL_BUF(gCb->u.dlCb->rguDlSap[suId].pst.region,
	 gCb->u.dlCb->rguDlSap[suId].pst.pool,
	 dlData,(Size)sizeof(RlcMacData));
#if (ERRCLASS & ERRCLS_ADD_RES)
   if ( dlData == NULLP )
   {
      RLOG_ARG2(L_FATAL,DBG_RBID,rbCb->rlcId.rbId,
	    "Memory Allocation failed UEID:%d CELLID:%d",   
	    rbCb->rlcId.ueId,
	    rbCb->rlcId.cellId);   
      RETVOID; 
   }
#endif /* ERRCLASS & ERRCLS_ADD_RES */

   dlData->slotInfo.sfn = sdu->mode.tm.sfn;
   dlData->slotInfo.slot = sdu->mode.tm.slot;
   dlData->cellId = rbCb->rlcId.cellId;
   dlData->rnti = sdu->mode.tm.rnti;
   dlData->numPdu = 1;
   dlData->pduInfo[0].commCh = TRUE;
   dlData->pduInfo[0].lcId = rbCb->lch.lChId;
   dlData->pduInfo[0].pduBuf =  sdu->mBuf;

   /* kw005.201 ccpu00117318, updating the statistics */
   gCb->genSts.bytesSent += sdu->sduSz;
   gCb->genSts.pdusSent++;

   kwUtlIncrementKwuStsSduTx(gCb->u.dlCb->kwuDlSap + rbCb->kwuSapId);   

   /* remove SDU from queue */ 
   sdu->mBuf = NULLP;
   cmLListDelFrm(&(rbCb->m.tm.sduQ),
	 &sdu->lstEnt); 
   RLC_FREE(gCb,sdu, sizeof(KwSdu));

   /* If trace flag is enabled send the trace indication */
   if(gCb->init.trc == TRUE)
   {
      /* Populate the trace params */
      kwLmmSendTrc(gCb,EVTRLCDLDAT, NULLP);
   }

   RlcMacSendDlData(&(gCb->u.dlCb->rguDlSap[suId].pst),
	 gCb->u.dlCb->rguDlSap[suId].spId,
	 dlData);
   RETVOID;
}

/**
 *
 * @brief 
 *    Handler to process the re-establishment request received from the upper 
 *    layer. 
 *
 * @details
 *    This function empties the SDU queue for the RB in the downlink.
 *            
 * @param[in] gCb   RLC Instance Control Block 
 * @param[in] rbCb  RB control block. 
 *
 * @return  S16
 *    -# ROK 
 */
#ifdef ANSI
   PUBLIC Void rlcDlTmmReEstablish
(
 RlcCb       *gCb,
 RlcDlRbCb   *rbCb    
 )
#else
PUBLIC Void rlcDlTmmReEstablish(gCb,rbCb)
   RlcCb       *gCb;
   RlcDlRbCb   *rbCb;     
#endif
{
   TRC2(rlcDlTmmReEstablish)


#ifdef LTE_L2_MEAS_RLC
      kwUtlEmptySduQ(gCb, rbCb, &rbCb->m.tm.sduQ);
#else
   kwUtlEmptySduQ(gCb,&rbCb->m.tm.sduQ);
#endif

   RETVOID;
}
/**
 *
 * @brief 
 *    Handler to send Status Response to the lower layer.
 *
 * @details
 *    This function is used to the BO to the lower layer after receiving a data
 *    request from the upper layer. 
 *            
 * @param[in] gCb          RLC Instance Control Block 
 * @param[in] rbCb         RB control block. 
 * @param[in] bo           Buffer Occupancy 
 * @param[in] datReqInfo   Data Request Information.
 *
 * @return  S16
 *    -# ROK 
 *    -# RFAILED 
 */

#ifdef ANSI
   PRIVATE Void kwTmmSndStaRsp
(
 RlcCb            *gCb,
 RlcDlRbCb        *rbCb,                 
 MsgLen          bo,                    
 KwuDatReqInfo   *datReqInfo         
 )
#else
PRIVATE Void kwTmmSndStaRsp(rbCb,bo,datReqInfo)
   RlcCb            *gCb;
   RlcDlRbCb        *rbCb;               
   MsgLen          bo;                
   KwuDatReqInfo   *datReqInfo;   
#endif
{
   //   RguCStaRspInfo   *staRspInfo;   /* Status Response Information */
   RlcMacBOStatus   *boStatus;      /* Buffer occupancy status information */
   KwRguSapCb       *rguSap;       /* SAP Information */

   TRC3(kwTmmSndStaRsp)


      rguSap = &(gCb->u.dlCb->rguDlSap[rbCb->rguSapId]);

   RLC_ALLOC_SHRABL_BUF(gCb->u.dlCb->rguDlSap[rbCb->rguSapId].pst.region,
	 gCb->u.dlCb->rguDlSap[rbCb->rguSapId].pst.pool,
	 boStatus, sizeof(RguCStaRspInfo));
#if (ERRCLASS & ERRCLS_ADD_RES)
   if ( boStatus == NULLP )
   {
      RLOG_ARG2(L_FATAL,DBG_RBID,rbCb->rlcId.rbId,
	    "Memory Allocation failed UEID:%d CELLID:%d",
	    rbCb->rlcId.ueId,
	    rbCb->rlcId.cellId);   
      RETVOID;
   }
#endif /* ERRCLASS & ERRCLS_ADD_RES */

   boStatus->cellId = rbCb->rlcId.cellId;
   boStatus->rnti = rbCb->rlcId.ueId;
   boStatus->commCh = TRUE;
   boStatus->lcId = rbCb->lch.lChId;
   boStatus->bo = bo;

   /* If trace flag is enabled send the trace indication */
   if(gCb->init.trc == TRUE)
   {
      /* Populate the trace params */
      kwLmmSendTrc(gCb, EVTRLCBOSTA, NULLP);
   }

   RlcMacSendBOStatus(&rguSap->pst, rguSap->spId, boStatus);

   RETVOID;
} 

#ifdef _cplusplus
}
#endif
/*@}*/
/********************************************************************30**
  End of file
 **********************************************************************/
