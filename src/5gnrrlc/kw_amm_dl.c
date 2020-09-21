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
  
        Name:    RLC - AM DL module file
    
        Type:    C source file
  
        Desc:    Source code for Acknowledged Mode Module functions such as, 

                 Transmission of data/control PDUs 
                 Retransmission (Feedback in terms of status)
                 Polling
                 Assemble SDUs
                 Reception - reordering
                 Duplicate detection for byte segments
                 Reassemble SDUs
                  
        File:   kw_amm_dl.c 
  
*********************************************************************21*/
static const char* RLOG_MODULE_NAME="AMM";
static int RLOG_MODULE_ID=2048;
static int RLOG_FILE_ID=189;
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
#include "kw_dl.h"


/* extern (.x) include files */
#include "lkw.x"           /* LKW */
#include "ckw.x"           /* CKW */
#include "kwu.x"           /* KWU */
#include "rgu.x"           /* RGU */

#include "kw.x"
#include "kw_udx.x"
#include "kw_dl.x"

//UDAY
#ifdef L2_OPTMZ
extern U32 rlcAmmStaPduList[512];
 U32 rlcAmmStaPduListCnt = 0;
#endif


/** @file gp_amm_dl.c
@brief RLC Acknowledged Mode Downlink Module
**/
#define RLC_MODULE (RLC_DBGMASK_AM | RLC_DBGMASK_DL)

U32 rlcStatusPduCnt, rlcStatusAckCnt, rlcStatusNcnt, rlcSduSndCnt;

/* local defines */

/* local externs */

/* forward references */
EXTERN Void rlcAmmDlHndlStatusPdu ARGS ((RlcCb  *gCb,
                                        RlcDlRbCb  *rbCb,
                                        RlcUdxStaPdu *pStaPdu));

/* public variable declarations */

/* This structure holds all the global structs we need. */

/* private variable declarations */

#define RLC_AM_RMV_HDR(_gCb, _rbCb, _retx) do { \
  if ((_retx)->yetToConst == FALSE) \
  {\
     Buffer    *_pduInfo; \
     SSegMsg((_retx)->seg, (_retx)->hdrSz, &_pduInfo); \
     RLC_FREE_BUF((_retx)->seg); \
     (_retx)->seg = _pduInfo; \
  }\
  (_rbCb)->m.amDl.estHdrSz -= retx->hdrSz;\
} while(0)

/* private function declarations */

PRIVATE Void  rlcResegRetxPdus ARGS ((RlcCb *gCb,
                                     RlcDlRbCb *rbCb, 
                                     RlcDatReq *rlcDatReq));

PRIVATE Void rlcRemRetxPdu ARGS ((RlcCb *gCb, 
                                 RlcDlRbCb *rbCb, 
                                 RlcRetx *retx));

PRIVATE Void rlcAmmCreateStatusPdu ARGS ((RlcCb *gCb, 
                                         RlcDlRbCb *rbCb,
                                         RlcDatReq *rlcDatReq));

PRIVATE Void rlcAmmDlMarkPduForReTx ARGS ((RlcCb *gCb,
                                          RlcDlRbCb *rbCb,
                                          RlcRetx *retx));

PRIVATE Void rlcAmmDlProcessSuccessfulTxPdu ARGS((RlcCb *gCb, 
                                                 RlcDlRbCb *rbCb,
                                                 RlcSn  sn,
                                                 KwuDatCfmInfo **datCfm));

PRIVATE Void rlcAmmDlSetTxNextAck ARGS((RlcAmDl *amDl, RlcSn sn));

PRIVATE Void rlcAmmDlCheckAndStopPollTmr ARGS((RlcCb *gCb,
                                              RlcDlRbCb *rbCb,
                                              RlcSn mAckSn));

PRIVATE Void  rlcAssembleSdus ARGS ((RlcCb *gCb,
                                    RlcDlRbCb *rbCb, 
                                    RlcDatReq *rlcDatReq));

PRIVATE Bool rlcAmmDlCheckAndSetPoll ARGS ((RlcCb *gCb,
                                           RlcDlRbCb *rbCb, 
                                           Bool newPdu, 
                                           MsgLen bufSz));

PRIVATE Void rlcAmmCreatePdu ARGS ((RlcCb *gCb,
                                   RlcDlRbCb *rbCb, 
                                   RlcAmHdr *amHdr, 
                                   RlcDlPduInfo *pduInfo,
                                   Buffer *pdu));

PRIVATE Void rlcAmmSndStaInd ARGS ((RlcCb *gCb,RlcDlRbCb *rbCb, RlcRetx *retx));

PRIVATE Void rlcGetNxtRetx ARGS ((RlcCb *gCb, RlcRetx **retx));

PRIVATE Void rlcConstructAmHdr ARGS ((RlcAmHdr *amHdr, 
                                     U8 *hdr,
                                     U8  snLen,
                                     U16 *idx));

PRIVATE Void rlcAmmDlUpdateTxAndReTxBufForAckSn ARGS ((RlcCb *gCb,
                                                      RlcDlRbCb *rbCb, 
                                                      RlcSn mAckSn,
                                                      CmLList *retx,
                                                      KwuDatCfmInfo **datCfm));

PRIVATE Void rlcAmmDlMoveFrmTxtoRetxBuffer ARGS ((RlcCb *gCb,
                                                 RlcAmDl *amDl, 
                                                 RlcRetx **retx, 
                                                 RlcSn sn));
 
PRIVATE Void rlcAmmDlCheckIsSDUDelivered ARGS((RlcCb *gCb,
                                              RlcDlRbCb *rbCb, 
                                              RlcSduMap *sduMap, 
                                              KwuDatCfmInfo **datCfm));

PRIVATE Void rlcAmmAddPduToRetxLst ARGS((RlcAmDl   *amDl,
                                        RlcRetx   *retx));

PRIVATE Void rlcAmmDlMoveSduByteSegFrmTxtoRetxBuffer ARGS(
(
RlcCb          *gCb,
RlcAmDl        *amDl,
RlcRetx        **retx,
RlcDlPduInfo   *pduInfo
));

PRIVATE Void rlcAmmDlHndlStatus4SduByteSegInTxBuf ARGS(
(
RlcCb          *gCb,
RlcDlRbCb      *rbCb,
RlcNackInfo    *nackSnInfo,
RlcRetx        **retx,
KwuDatCfmInfo **datCfm
));

PRIVATE Void rlcAmmDlUpdateTxAndReTxBufForNackSn ARGS(
(
 RlcCb          *gCb,
 RlcDlRbCb      *rbCb,
 RlcNackInfo    *nackSnInfo,
 CmLList       **retxNode,
 KwuDatCfmInfo **datCfm
 ));

PRIVATE Void RlcDlAmmGetNackSnInfoFrmNackRangeIdx ARGS(
(
RlcAmDl        *amDl,
RlcNackInfo    *nackInfo,
CmLList       *retxNode,
RlcNackInfo    *nackSnInfo,
U8            idx
));

PRIVATE Void rlcAmmDlUpdTxAndReTxBufForLessThanNackSn ARGS(
(
RlcCb            *gCb,
RlcDlRbCb        *rbCb,
RlcSn            sn,
RlcSn            mNackSn,
CmLList         **retxNode,
KwuDatCfmInfo   **datCfm
));
/*****************************************************************************

  AM Module contains the following funcitons:

  -  rlcAmmQSdu 
  -  rlcAmmProcessSdus
     -  rlcAmmDlAssembleCntrlInfo
     -  rlcResegRetxPdus
     -  rlcAssembleSdus
     -  rlcAmmDlCheckAndSetPoll
  -  rlcAmmProcessPdus
     -  rlcDlmHndlStaRsp
     -  kwTriggerStatus
     -  kwReassembleSdus
   
*******************************************************************************/
/** @addtogroup ammode */
/*@{*/

/**
 * @brief Function to send a Status Response to MAC for a dedicated logical
 *        channel
 *       
 * @details
 *    Function calculates the current bo and send a Status response for the
 *    dedicated logical channel if the bo is non zero
 *
 * @param[in] gCb    RLC instance control block
 * @param[in] rbCb   Radio Bearer control block
 * @param[in] amDl   AM downlink control block
 * 
 * @return  Void
*/
#ifdef ANSI
Void rlcAmmSendDStaRsp
(
RlcCb       *gCb,
RlcDlRbCb   *rbCb,
RlcAmDl     *amDl
)
#else
Void rlcAmmSendDStaRsp(gCb, rbCb, amDl)
RlcCb       *gCb;
RlcDlRbCb   *rbCb;
RlcAmDl     *amDl;
#endif
{
   S32 bo = rlcAmmCalculateBo(amDl);

   if(bo)
   {
      rlcUtlSndDStaRsp(gCb, rbCb, bo, amDl->estHdrSz, amDl->cntrlBo ?TRUE:FALSE,amDl->cntrlBo);
   }
   
   RETVOID;
}

/**
 * @brief Function to check if the pollSn is acked and stop the poll timer
 *
 * @param[in] gCb      RLC instance control block
 * @param[in] rbCb     Radio Bearer control block
 * @param[in] mAckSn   The last received ACKSN. The base modulus value should
 *                     be passed
 * 
 * @return  Void
*/
#ifdef ANSI
PRIVATE Void rlcAmmDlCheckAndStopPollTmr
(
RlcCb       *gCb,
RlcDlRbCb   *rbCb,
RlcSn       mAckSn
)
#else
PRIVATE Void rlcAmmDlCheckAndStopPollTmr(gCb, rbCb, mAckSn)
RlcCb       *gCb;
RlcDlRbCb   *rbCb;
RlcSn       mAckSn;
#endif
{
   RlcSn mPollSn;                                                   
                                                                   
   MODAMT(rbCb->m.amDl.pollSn, mPollSn, rbCb->m.amDl.txNextAck,rbCb->m.amDl.snModMask);      
                                                                   
   if (mPollSn <= mAckSn)                                        
   {                                                               
      if (rlcChkTmr(gCb, (PTR)rbCb, RLC_EVT_AMDL_POLL_RETX_TMR))    
      {                                                            
         rlcStopTmr(gCb, (PTR)rbCb, RLC_EVT_AMDL_POLL_RETX_TMR);    
      }                                                            
   }

   RETVOID;
}

/**
 * @brief Function to set VT(A) and VT(MS). Calculates the VT(MS) from VT(A)
 *
 * @param[in,out] amDl    AM downlink control block
 * @param[in]sn           Sequence number to be set as VT(A)
 * 
 * @return  Void
*/
#ifdef ANSI
PRIVATE Void rlcAmmDlSetTxNextAck
(
RlcAmDl   *amDl,
RlcSn      sn
)
#else
PRIVATE Void rlcAmmDlSetTxNextAck(amDl, sn)
RlcAmDl   *amDl;
RlcSn     sn
#endif
{
   amDl->txNextAck = sn;
   
   RETVOID;
}

/**
 * @brief Function to process a successfully re-transmitted PDU/segment
 *       
 * @details
 *    Checks if the SDU has been completely delivered or not. Removes the PDU
 *    from the re-transmission buffer
 *
 * @param[in] gCb    RLC instance control block
 * @param[in] rbCb   Downlink Radio Bearer control block
 * @param[in] retx   The PDU/segment which was successfully re-transmitted
 * 
 * @return  Void
*/
#ifdef ANSI
PRIVATE Void rlcAmmDlProcessSuccessfulReTx
(
RlcCb            *gCb,
RlcDlRbCb        *rbCb,
RlcRetx          *retx,
KwuDatCfmInfo   **datCfm
)
#else
PRIVATE Void rlcAmmDlProcessSuccessfulReTx(gCb, rbCb, retx, datCfm)
RlcCb            *gCb;
RlcDlRbCb        *rbCb;
RlcRetx          *retx;
KwuDatCfmInfo   **datCfm;
#endif
{
   rlcAmmDlCheckIsSDUDelivered(gCb, rbCb, &(retx->sduMap), datCfm);
   
   rlcRemRetxPdu(gCb, rbCb, retx);

   RETVOID;
}

/**
 * @brief Handler to Move the PDU from txBuf to re-transmission buffer 
 *
 * @details
 *    This function is used to move the PDU from the txBuf to re-transmit buffer
 *
 * @param[in]RlcCb   *gCb           RLC instance control block
 * @param[in]RlcAmDl *amDl          AM Downlink Control Block 
 * @param[in]RlcRetx **retx         node in the reTx buffer to be moved to, allocated by
 *                                 this function
 * @param[in]RlcDlPduInfo *pduInfo  TX PDU which needs to be moved
 * 
 * @return Void
 *            
 */

#ifdef ANSI
PRIVATE Void rlcAmmDlMoveSduByteSegFrmTxtoRetxBuffer
(
RlcCb          *gCb,
RlcAmDl        *amDl,
RlcRetx        **retx,
RlcDlPduInfo   *pduInfo
)
#else
PRIVATE Void rlcAmmDlMoveSduByteSegFrmTxtoRetxBuffer(gCb, amDl, retx, pduInfo)
RlcCb          *gCb;
RlcAmDl        *amDl;
RlcRetx        **retx;
RlcDlPduInfo   *pduInfo;
#endif
{
   TRC2(rlcAmmDlMoveSduByteSegFrmTxtoRetxBuffer);


   RLC_ALLOC_WC(gCb,*retx, sizeof(RlcRetx));

#if (ERRCLASS & ERRCLS_ADD_RES)
   if (*retx == NULLP)
   {
	   RLOG0(L_FATAL, "Memory allocation failed");
	   RETVOID;
   }
#endif /* ERRCLASS & ERRCLS_RES */

   (*retx)->seg = pduInfo->pdu;
   (*retx)->segSz = pduInfo->pduSz;
   /* MS_FIX for DL stall */
   (*retx)->soEnd = (pduInfo->amHdr.so + pduInfo->pduSz - 1);

   (*retx)->hdrSz = pduInfo->hdrSz;
   (*retx)->retxCnt = 1;
   (*retx)->yetToConst = 0;
   (*retx)->pendingReTrans = TRUE;

   /* initialize the list pointer to 0 instead of memset */
   (*retx)->lstEnt.next = 0;
   (*retx)->lstEnt.prev = 0;
   /* copy the sdu maps */
   RLC_MEM_CPY(&((*retx)->sduMap), 
		   &pduInfo->sduMap, 
		   sizeof(RlcSduMap));

   RLC_MEM_CPY(&((*retx)->amHdr), &pduInfo->amHdr, sizeof(RlcAmHdr));
   rlcAmmAddPduToRetxLst(amDl, (*retx));

   /* Update the BO appropriately */
   amDl->retxBo   += (*retx)->segSz;
   amDl->estHdrSz += (*retx)->hdrSz;

   gRlcStats.amRlcStats.numDLRetransPdus++;

   RETVOID;
} /*rlcAmmDlMoveSduByteSegFrmTxtoRetxBuffer */

/**
 * @brief Function to handle Status of Sdu byte segment for a nackSn 
 *
 * @details
 *    This function is used to move the PDU from the txBuf to re-transmit buffer
 *
 * @param[in]RlcCb       *gCb          RLC instance control block
 * @param[in]RlcDlRbCb   *rbCb         AM Downlink Control Block 
 * @param[in]RlcNackInfo *nackSnInfo   Nack Information of a NACK_SN 
 * @param[in]RlcRetx     **retx        node in the reTx buffer to be moved to, allocated by
 *                                    this function
 * @param[in]KwuDatCfmInfo **datCfm Ptr to datCfm
 * 
 * @return Void
 *            
 */

#ifdef ANSI
PRIVATE Void rlcAmmDlHndlStatus4SduByteSegInTxBuf
(
RlcCb          *gCb,
RlcDlRbCb      *rbCb,
RlcNackInfo    *nackSnInfo,
RlcRetx        **retx,
KwuDatCfmInfo ** datCfm
)
#else
PRIVATE Void rlcAmmDlHndlStatus4SduByteSegInTxBuf(gCb, rbCb, nackSnInfo, retx, datCfm)
(
RlcCb          *gCb;
RlcDlRbCb      *rbCb;
RlcNackInfo    *nackSnInfo;
RlcRetx        **retx;
KwuDatCfmInfo **datCfm;
)
#endif
{
   RlcTx    *txBuf=NULLP;
   CmLList *lnk;
   CmLList *nextLnk;

   TRC2(rlcAmmDlHndlStatus4SduByteSegInTxBuf)

   txBuf = rlcUtlGetTxBuf(AMDL.txBufLst, nackSnInfo->sn);
   if (txBuf == NULLP)
   {
	   RETVOID;
   }
   lnk = txBuf->pduLst.first;
   while(lnk)
   {
      RlcDlPduInfo *pduInfo = (RlcDlPduInfo *)(lnk->node);
      RlcSn        pduSoEnd = (pduInfo->amHdr.so + pduInfo->sduMap.sduSz - 1);

      /* If So of Sdu byte segment(pduInfo/seg) is < status pdu
         soStart that means it's ACKED*/
      if(pduSoEnd < nackSnInfo->soStart) 
      {
         rlcAmmDlCheckIsSDUDelivered(gCb,
               rbCb,  
               &(pduInfo->sduMap), 
               datCfm);

      }
      else if (pduSoEnd <= nackSnInfo->soEnd)
      {
         /* Move Sdu byte segment from TX buf to retx buf*/
         rlcAmmDlMoveSduByteSegFrmTxtoRetxBuffer(gCb, 
               &rbCb->m.amDl, 
               retx, 
               pduInfo);
      }
      else
      {
         lnk = lnk->next;
         continue;
      }
      nextLnk = lnk->next;
      /* Delete node from the txBuf Pdu lst */
      cmLListDelFrm(&txBuf->pduLst, lnk);
      RLC_FREE_WC(gCb, pduInfo, sizeof(RlcDlPduInfo));
      lnk = nextLnk;
   }
   if(!txBuf->pduLst.count)
   {
      /*No more Sdu byte segment are left. Hence delete txBuf*/
      rlcUtlDelTxBuf(AMDL.txBufLst, txBuf,gCb);
   }

   RETVOID;
}

/**
 * @brief Function to handle Status of Sdu byte segment for a nackSn 
 *
 * @details
 *    This function is used to move the PDU from the txBuf to re-transmit buffer
 *
 * @param[in]RlcCb   *gCb            RLC instance control block
 * @param[in]RlcDlRbCb *rbCb         AM Downlink Control Block 
 * @param[in]RlcNackInfo *nackSnInfo Nack Information of a NACK_SN
 * @param[in]RlcRetx **retx          node in the reTx buffer to be moved to, allocated by
 *                                  this function
 * @param[in]KwuDatCfmInfo **datCfm Ptr to datCfm
 * 
 * @return Void
 *            
 */
#ifdef ANSI
PRIVATE Void rlcAmmDlUpdateTxAndReTxBufForNackSn
(
 RlcCb          *gCb,
 RlcDlRbCb      *rbCb,
 RlcNackInfo    *nackSnInfo,
 CmLList        **retxNode,
 KwuDatCfmInfo **datCfm
 )
#else
PRIVATE Void rlcAmmDlUpdateTxAndReTxBufForNackSn(gCb, rbCb, nackSnInfo, retxNode, datCfm)
(
 RlcCb          *gCb;
 RlcDlRbCb      *rbCb;
 RlcNackInfo    *nackSnInfo;
 CmLList       **retxNode;
 KwuDatCfmInfo **datCfm;
 )
#endif
{
   RlcTx    *txBuf;
   RlcRetx  *retx;

   TRC2(rlcAmmDlUpdateTxAndReTxBufForNackSn)

   /* Now process the NACK_SN received. Now the NACK_SN is    */
   /* either the first element of RETX or is in TX array      */
   /* To remove the remaining acks from the pdu byte segments */

   /* if the NACK_SN is in the transmit buffer, move it to the re-
         transmit buffer */
   txBuf = rlcUtlGetTxBuf(AMDL.txBufLst, nackSnInfo->sn);
   if (txBuf != NULLP)
   {
      if(nackSnInfo->isSegment)
      {
         /* Go through all the AMD PDUs of a particular SN
            and check if segment is ACKED if yes then mark succesfully sent,
            if segment is NACKed then move it to to retx lst */
         rlcAmmDlHndlStatus4SduByteSegInTxBuf(gCb, rbCb, nackSnInfo, &retx, datCfm);
      }
      else
      {
         /*e2= 0 and e3= 0: Move complete PDU from TX buf to retx buf*/
         rlcAmmDlMoveFrmTxtoRetxBuffer(gCb, 
               &rbCb->m.amDl, 
               &retx, 
               nackSnInfo->sn);
      } 

#if (ERRCLASS & ERRCLS_ADD_RES)
      if(retx)
#endif
      {
	      (*retxNode) = retx->lstEnt.next;
      }

      RETVOID;
   }

   /* process the pdus/segments in the re-transmit buffer with 
      this NACK_SN */
   while (*retxNode)
   {
      retx = (RlcRetx *)((*retxNode)->node);
      if (retx->amHdr.sn != nackSnInfo->sn)
      {
         break;
      }
      if ((nackSnInfo->isSegment) &&
            ((retx->soEnd < nackSnInfo->soStart) /*|| (retx->amHdr.so > soEnd)*/))
      {
         RLOG_ARG3(L_DEBUG, DBG_RBID, rbCb->rlcId.rbId,
               "rlcHndlStaRsp: Handle ACK for byte segment, Its "
               "sn = %d UEID:%d CELLID:%d",
               nackSnInfo->sn, 
               rbCb->rlcId.ueId,
               rbCb->rlcId.cellId);
         RLOG_ARG4(L_DEBUG, DBG_RBID, rbCb->rlcId.rbId,
               "soStart and soEnd = %d, %d, UEID:%d CELLID:%d",
               retx->amHdr.so, retx->soEnd, 
               rbCb->rlcId.ueId,
               rbCb->rlcId.cellId);

         (*retxNode) = (*retxNode)->next;
         rlcAmmDlProcessSuccessfulReTx(gCb,rbCb, retx, datCfm);
      }
      else if((!nackSnInfo->isSegment) || (retx->soEnd <= nackSnInfo->soEnd))
      {
         /* This case covers the NACKED segments and also the case */
         /* when there are segments and the entire SN is nacked.   */
         /* This case also covers the case of nonsegmented retx PDU*/

         (*retxNode) = (*retxNode)->next;
         /* Mark the retx PDU we found for further retransmission  */
         rlcAmmDlMarkPduForReTx(gCb, rbCb, retx);
      }
      else
      { 
         /* If we are here that means this segment and segments after this are ACKed*/
         break;
      }
   } /* end of retxNode while loop*/
   RETVOID;
}

/**
 * @brief Function to get nack Sn information from nackRange index 
 *
 * @details
 *    This function is used to get nack Sn information from nackRange index
 *
* @param[in]RlcAmDl        *amDl,
* @param[in]RlcUdxStaPdu   *StaPdu,
* @param[in]RlcNackInfo    *nackSnInfo,
* @param[in]RlcRetx        *retx;
* @param[in]RlcSn          sn, 
* @param[in]U8            idx
 * 
 * @return Void
 *            
 */
#ifdef ANSI
PRIVATE Void RlcDlAmmGetNackSnInfoFrmNackRangeIdx
(
RlcAmDl        *amDl,
RlcNackInfo    *nackInfo,
CmLList        *retxNode,
RlcNackInfo    *nackSnInfo,
U8            idx
)
#else
PRIVATE Void RlcDlAmmGetNackSnInfoFrmNackRangeIdx(amDl, nackInfo, retxNode, nackSnInfo, idx)
(
RlcAmDl        *amDl;
RlcNackInfo    *nackInfo;
CmLList       *retxNode;
RlcNackInfo    *nackSnInfo;
U8            idx;
)
#endif
{
   RlcTx     *txBuf;
   RlcRetx   *retx;
   CmLList  *node;

   TRC2(RlcDlAmmGetNackSnInfoFrmNackRangeIdx)

   nackSnInfo->isSegment = FALSE;

   if((!nackInfo->isSegment) || (!idx && nackSnInfo->nackRange && (!nackInfo->soStart)))
   {
      nackSnInfo->soStart = 0;
      nackSnInfo->soEnd = 0;
      RETVOID;
   }
   txBuf = rlcUtlGetTxBuf(amDl->txBufLst, nackSnInfo->sn);
   if(txBuf != NULLP)
   {
      node = txBuf->pduLst.first;
      while(node)
      {
         RlcDlPduInfo *pduInfo = (RlcDlPduInfo *)(node->node);
         U16         pduSoEnd = pduInfo->amHdr.so + pduInfo->sduMap.sduSz - 1; 
         if((!idx) && (pduInfo->amHdr.so == nackInfo->soStart))
         {
            nackSnInfo->isSegment = TRUE;
            nackSnInfo->soStart = pduInfo->amHdr.so;
            nackSnInfo->soEnd = pduSoEnd;
            break;
         }
         else if((idx == nackSnInfo->nackRange - 1) && \
               (pduSoEnd == nackInfo->soEnd))
         {
            nackSnInfo->isSegment = TRUE;
            nackSnInfo->soStart = pduInfo->amHdr.so;
            nackSnInfo->soEnd = pduSoEnd;
            break;
         }
         node = node->next;
      }
   }
   if(!nackSnInfo->isSegment)
   { 
      while (retxNode)
      {
         retx = (RlcRetx *)(retxNode->node);
         if(retx->amHdr.sn != nackSnInfo->sn)
         {
            break;
         }
         if((!idx) && (retx->amHdr.so == nackInfo->soStart))
         {
            nackSnInfo->isSegment = TRUE;
            nackSnInfo->soStart = retx->amHdr.so;
            nackSnInfo->soEnd = retx->soEnd;
            break;
         }
         else if((idx == nackSnInfo->nackRange - 1) && \
               (retx->soEnd == nackInfo->soEnd))
         {
            nackSnInfo->isSegment = TRUE;
            nackSnInfo->soStart = retx->amHdr.so;
            nackSnInfo->soEnd = retx->soEnd;
            break;
         }
         retxNode = retxNode->next;
      }
   }
}

/**
 * @brief Function to update transmission buffers and send confimations to
 *        PDCP on the reception of Status PDU
 *
 * @details
 *    First processes the NACKs received
 *    -# Removes the pdus which are acked by each of the NACK SN from the
 *       transmission and re-transmission buffer
 *    -# If NACKed SN in in the transmisson buffer, moves it to re-transmission
 *       buffer
 *    -# Removes PDU segments of the NACKed SN which have been successfully 
 *       received by the other end. For the un-successful ones, marks them for
 *       re-transmission
 *    -# When PDUs/segments are removed from the buffer, indicates to upper
 *       layer if the SDU is completely delivered
 *    -# Removes the PDUs/segments which are acked by the ACK_SN but not by the
 *       NACK_SNs
 *
 * @param[in] gCb         RLC Instance control block
 * @param[in] rbCb        Downlink Radio Bearer control block
 * @param[in] pStaPdu     The decoded Status Pdu
 * 
 * @return  Void
*/
#ifdef ANSI
Void rlcAmmDlHndlStatusPdu
(
RlcCb          *gCb,
RlcDlRbCb      *rbCb,
RlcUdxStaPdu   *pStaPdu
)
#else
Void rlcAmmDlHndlStatusPdu(gCb, rbCb, pStaPdu)
RlcCb          *gCb;
RlcDlRbCb      *rbCb;
RlcUdxStaPdu   *pStaPdu;
#endif
{
   RlcSn      mAckSn;
   S32       oldRetxBo;
   CmLList   *retxNode;
   KwuDatCfmInfo* datCfm; 
   RlcKwuSapCb    *rlckwuSap;
   RlcSn       mTxNext;

   TRC2(rlcAmmDlHndlStatusPdu)
   rlcStatusPduCnt++;

   rlckwuSap = gCb->u.dlCb->rlcKwuDlSap + RLC_UI_PDCP;
   /* store the re-transmission bo, to check if it changes due to the
      processing of the status pdu */
   oldRetxBo = AMDL.retxBo;

   /* Allocate memory for datCfm Info */
   RLC_SHRABL_STATIC_BUF_ALLOC(rlckwuSap->pst.region, rlckwuSap->pst.pool, datCfm, sizeof(KwuDatCfmInfo));

#if (ERRCLASS & ERRCLS_ADD_RES)
   if (datCfm == NULLP)
   {
      RLOG_ARG2(L_FATAL,DBG_RBID,rbCb->rlcId.rbId, 
               "Memory allocation failed UEID:%d CELLID:%d",
               rbCb->rlcId.ueId,
               rbCb->rlcId.cellId);
      RLC_SHRABL_STATIC_BUF_FREE(rlckwuSap->pst.region, rlckwuSap->pst.pool, datCfm, sizeof(KwuDatCfmInfo));
      RETVOID;
   }
#endif /* ERRCLASS & ERRCLS_RES */

   datCfm->numSduIds = 0;
   datCfm->rlcId = rbCb->rlcId;

   MODAMT(pStaPdu->ackSn, mAckSn, AMDL.txNextAck,AMDL.snModMask);
   MODAMT(AMDL.txNext,mTxNext, AMDL.txNextAck,AMDL.snModMask);

   if(mAckSn > mTxNext)
   {
      RLOG_ARG4(L_WARNING,DBG_RBID, rbCb->rlcId.rbId, 
               "Invalid ACK SN = %d received. Current Vta =%d"
               "UEID:%d CELLID:%d", 
               pStaPdu->ackSn,
               AMDL.txNextAck,
               rbCb->rlcId.ueId,
               rbCb->rlcId.cellId);
/*      RLC_SHRABL_STATIC_BUF_ALLOC(rlckwuSap->pst.region, rlckwuSap->pst.pool, datCfm, sizeof(KwuDatCfmInfo)); */
      RLC_SHRABL_STATIC_BUF_FREE(rlckwuSap->pst.region, rlckwuSap->pst.pool, datCfm, sizeof(KwuDatCfmInfo));
      RETVOID;
   }
 
   /* Venki - stopping the poll retx timer */
   /*Stop PollRetx Tmr */
   rlcAmmDlCheckAndStopPollTmr(gCb, rbCb, mAckSn);

   /* Set the first node in retx list to retxNode */
   retxNode = AMDL.retxLst.first;

   /* If NACK exists in control PDU */
   if (pStaPdu->nackCnt)
   {
      RlcSn   sn;
      RlcNackInfo nackSnInfo;
      RlcSn   mNackSn;
      RlcSn   txNextAck;
      RlcSn   transWinStartSn = AMDL.txNextAck; /*used to track the SN from which 
                                           to start processing the transmission
                                           buffer */
      U32    idx = 0;

      /* if any NACKs then txNextAck should be equal to the first NACK_SN*/
      txNextAck = pStaPdu->nackInfo[0].sn;

      rlcStatusNcnt += pStaPdu->nackCnt;

      /* For NACKs */
      while (idx < pStaPdu->nackCnt)
      {
         nackSnInfo.isSegment = pStaPdu->nackInfo[idx].isSegment;
         nackSnInfo.nackRange = pStaPdu->nackInfo[idx].nackRange;
         nackSnInfo.sn = pStaPdu->nackInfo[idx].sn;
         
         RLOG_ARG3(L_DEBUG,DBG_RBID, rbCb->rlcId.rbId, 
                  "rlcHndlStaRsp: NACK SN = %d UEID:%d CELLID:%d",
                  nackSnInfo.sn,
                  rbCb->rlcId.ueId,
                  rbCb->rlcId.cellId);

         nackSnInfo.soStart = pStaPdu->nackInfo[idx].soStart;
         nackSnInfo.soEnd   = pStaPdu->nackInfo[idx].soEnd;

         /* e2 is used as a boolean indicating presence of SOStart or SOEnd */

         sn = transWinStartSn;

         /* move transWinStartSn to nackSnInfo.sn + 1, as the pdu's before that
            will be removed from the buffer */
         transWinStartSn = (nackSnInfo.sn + (nackSnInfo.nackRange ?\
                  (nackSnInfo.nackRange - 1) : 0) + 1) & AMDL.snModMask;
        
        /* Clear the acked SNs from the retx list */
         MODAMT(nackSnInfo.sn, mNackSn, AMDL.txNextAck,AMDL.snModMask);

         if ((mNackSn > mAckSn) || (mNackSn >= mTxNext))
         {
            /* Erroneous NACK_SN, we should raise an error towards L3 */
            RLOG_ARG2(L_ERROR,DBG_RBID, rbCb->rlcId.rbId, 
                     "Status Pdu is not correct UEID:%d CELLID:%d",
                     rbCb->rlcId.ueId,
                     rbCb->rlcId.cellId);
            RLC_SHRABL_STATIC_BUF_FREE(rlckwuSap->pst.region, rlckwuSap->pst.pool, datCfm, sizeof(KwuDatCfmInfo));
            RETVOID;
         }

         /* clear all the SNs < NACK_SN from re-transmission list */   
         rlcAmmDlUpdTxAndReTxBufForLessThanNackSn(gCb, rbCb, sn, mNackSn,
                                                 &retxNode, &datCfm);
         
         if(!nackSnInfo.nackRange)
         {
            rlcAmmDlUpdateTxAndReTxBufForNackSn(gCb, rbCb, &nackSnInfo, &retxNode, &datCfm);
            gRlcStats.amRlcStats.numRlcAmCellNackRx++;
         }
         else
         {
            U8 idx1 = 0;
            /* Update issegment, soStart, soEnd ,sn  in nackSnInfo and handle
             * nack sn*/
            do
            {
               RlcDlAmmGetNackSnInfoFrmNackRangeIdx(&AMDL, &pStaPdu->nackInfo[idx],
                                                   retxNode, &nackSnInfo, idx1);
               
               rlcAmmDlUpdateTxAndReTxBufForNackSn(gCb, rbCb, &nackSnInfo,
                                                  &retxNode, &datCfm);
               nackSnInfo.sn = ((nackSnInfo.sn + 1) & (AMDL.snModMask)); 
               gRlcStats.amRlcStats.numRlcAmCellNackRx++;

            }while((++idx1) < (nackSnInfo.nackRange));
         }

         idx++;
      } /* End of nackCnt while loop */

      /* Remove the PDUs with are further acked by the ACK_SN after taking
         care of all the NACK_SN related acknowledgments*/
      rlcAmmDlUpdateTxAndReTxBufForAckSn(gCb,rbCb, mAckSn, retxNode, &datCfm);

      /* Update txNextAck */
      rlcAmmDlSetTxNextAck(&AMDL,txNextAck);
   }
   else
   {
     rlcStatusAckCnt++;
      /* For All ACKs */
      RLOG_ARG2(L_UNUSED,DBG_RBID, rbCb->rlcId.rbId,
               "rlcHndlStaRsp: Received All ACKS UEID:%d CELLID:%d",
               rbCb->rlcId.ueId,
               rbCb->rlcId.cellId);

      /* For the remaining ACKs  after last nackSn */
      rlcAmmDlUpdateTxAndReTxBufForAckSn(gCb,rbCb, mAckSn, retxNode, &datCfm);
      
      /* update txNextAck */
      rlcAmmDlSetTxNextAck(&AMDL, pStaPdu->ackSn); 
   }

   if(datCfm->numSduIds != 0)
   {
      if(datCfm->numSduIds > 1024)
      {
         RLOG_ARG4(L_DEBUG,DBG_RBID,datCfm->rlcId.rbId, 
                  "Sending [%lu] SDU Cfms to PDCP & [%lu] lost for"
                  "UEID:%d CELLID:%d",
                  datCfm->numSduIds, 
                  datCfm->numSduIds-1024,
                  rbCb->rlcId.ueId,
                  rbCb->rlcId.cellId);
         datCfm->numSduIds = 1024;
      }
      rlcSduSndCnt += datCfm->numSduIds;
      /* Sap control block */
      RlcUiKwuDatCfm(&rlckwuSap->pst, rlckwuSap->suId, datCfm);
   }
   else
   {
      RLC_SHRABL_STATIC_BUF_FREE(rlckwuSap->pst.region, rlckwuSap->pst.pool, datCfm, sizeof(KwuDatCfmInfo));
   }

   /* Fix for memory corruption */
   RLC_LLIST_FIRST_RETX(AMDL.retxLst, AMDL.nxtRetx);
   /* BO update, if retransmission BO has changed. AMDL.retxBo would have
      canged inside the above called functions */
   if (oldRetxBo != AMDL.retxBo)
   {
      rlcAmmSendDStaRsp(gCb, rbCb, &AMDL);
   }

   RETVOID;
}

/**
 * @brief Function to calculate the current buffer occupancy
 *
 * @details 
 *    Function to calculate the current bo depending on the control, 
 *          re-transmit, transmit bo's and the state of the transmit window. 
 *          If the transmit window is stalled, then the transmit bo is not 
 *          taken into account
 *
 * @param[in] amDl AM mode donwlink control block 
 *
 *  @return S16
 *      Calculated bo
*/
#ifdef ANSI
S32 rlcAmmCalculateBo
(
RlcAmDl *amDl
)
#else
S32 rlcAmmCalculateBo(amDl)
RlcAmDl *amDl;
#endif
{
   S32 bo;

   /* Make sure non of the bo's are negative */      
   if (amDl->bo < 0) 
   {   
      amDl->bo = 0;                         
   }
   
   if (amDl->cntrlBo < 0) 
   {
      amDl->cntrlBo = 0;               
   }
   
   if (amDl->retxBo < 0) 
   {
      amDl->retxBo = 0;                 
   }

   bo = amDl->cntrlBo + amDl->retxBo;

   /* if window is not stalled then add the transmit bo also */
   if (! RLC_AM_IS_TRANS_WIN_STALLED(amDl)) 
   {
      bo += amDl->bo;
   }
   
   return bo;
}
U32 kwRxSdu;

/**
 * @brief Handler to queue the SDUs received from PDCP
 *       
 * @details
 *    This function is invoked by UIM to queue the SDU received from PDCP in the
 *    SDU queue of the corresponding RbCb. It also updates the BO and report the
 *    same to MAC.
 *    -  Allocate memory for and assign received buffer to the SDU
 *    -  Add SDU in the sduQ of RlcAmDl 
 *    -  Calculate bo with the buffer received
 *    -  Accumulate bo with retransmission bo and control pdu's bo if available
 *    -  Estimate the header size for the bo; Fill in StaRspInfo and send it 
 *       to MAC
 *
 * @param[in] gCb     RLC Instance control block
 * @param[in] rbCb    RB control block 
 * @param[in] mBuf    Sdu to be queued
 * @param[in] datReq  Ptr to the datReq sent from PDCP
 *
 * @return Void
 *      -# RETVOID
*/
#ifdef ANSI
Void rlcAmmQSdu
(
RlcCb            *gCb,
RlcDlRbCb        *rbCb,
Buffer          *mBuf,
KwuDatReqInfo   *datReq
)
#else
Void rlcAmmQSdu(gCb, rbCb, mBuf, datReq)
RlcCb            *gCb;
RlcDlRbCb        *rbCb;
Buffer          *mBuf;
KwuDatReqInfo   *datReq;
#endif
{
   RlcSdu       *sdu;
#ifdef LTE_L2_MEAS
#ifndef L2_L3_SPLIT
#ifdef TENB_STATS
   U32         rlcWinSz; 
#endif
#endif
#endif
   TRC2(rlcAmmQSdu)

   /* Allocate sdu */
   RLC_ALLOC_WC(gCb,sdu, sizeof(RlcSdu)); 

#if (ERRCLASS & ERRCLS_ADD_RES)
   if (sdu == NULLP)
   {
      RLOG_ARG2(L_FATAL,DBG_RBID,rbCb->rlcId.rbId,
               "Memory allocation failed UEID:%d CELLID:%d",
               rbCb->rlcId.ueId,
               rbCb->rlcId.cellId);
      RETVOID;
   }
#endif /* ERRCLASS & ERRCLS_RES */

   RLC_UPD_L2_DL_TOT_SDU_STS(gCb,rbCb);
   /* Discard new changes starts */
   rlcUtlGetCurrTime(&sdu->arrTime);
   /* Discard new changes ends */
   /* Assign values to sdu */
   SFndLenMsg(mBuf, &sdu->sduSz);

   sdu->mBuf = mBuf;
   sdu->actSz = sdu->sduSz;
   sdu->mode.am.sduId = datReq->sduId;
   /* initialize values for AM mode to 0 */
   sdu->mode.am.rcvdSz = 0;
   sdu->mode.am.isSegmented = 0;
#ifndef RGL_SPECIFIC_CHANGES
#ifdef MSPD
{
extern U32 dlrate_kwu;
dlrate_kwu += sdu->sduSz;
}
#endif
#endif   
   /* Update nxtTx to point to the added sdu if this is the first SDU in the
    * queue */
   if (AMDL.nxtTx == NULLP)
   {
      RLOG_ARG2(L_UNUSED,DBG_RBID, rbCb->rlcId.rbId,
               "rlcAmmQSdu: Received SDU will be transmitted next"
               "UEID:%d CELLID:%d",
                rbCb->rlcId.ueId,
                rbCb->rlcId.cellId);
      AMDL.nxtTx = sdu;
   }

   /* Add sdu to the sdu list */
   cmLListAdd2Tail(&AMDL.sduQ, &sdu->lstEnt);
   sdu->lstEnt.node = (PTR)sdu;
#ifdef LTE_L2_MEAS
#ifndef L2_L3_SPLIT
#ifdef TENB_STATS
   if (rbCb->ueCb->tenbStats)
   {
      if (AMDL.sduQ.count > rbCb->ueCb->tenbStats->stats.nonPersistent.rlc.dlMaxPktsInSduQ)
      {
         rbCb->ueCb->tenbStats->stats.nonPersistent.rlc.dlMaxPktsInSduQ = AMDL.sduQ.count;
      }
      rlcWinSz = RLC_AM_TRANS_WIN_SIZE(&AMDL);
      if (rlcWinSz > rbCb->ueCb->tenbStats->stats.nonPersistent.rlc.dlMaxWindowSz)
      {
         rbCb->ueCb->tenbStats->stats.nonPersistent.rlc.dlMaxWindowSz = rlcWinSz;
      }
   }
#endif
#endif
#endif
   /* Update BO and estimate header size for the current BO */ 
   AMDL.bo = AMDL.bo + sdu->sduSz;
  if(AMDL.snLen == RLC_AM_CFG_12BIT_SN_LEN)
  {
     AMDL.estHdrSz += 2;
  }
  else
  {
     AMDL.estHdrSz += 3;
  }
#ifdef LTE_L2_MEAS_RLC
   /* Update numActUe if it is not active */
   if((rbCb->rbL2Cb.measOn & LKW_L2MEAS_ACT_UE) &&
      (rbCb->ueCb->numActRb[rbCb->qci] == 0))
   {
      rbCb->ueCb->numActRb[rbCb->qci]++;
      gCb.rlcL2Cb.numActUe[rbCb->qci]++;
   }
#endif

   if(!rlcDlUtlIsReestInProgress(rbCb))
   {
      rlcAmmSendDStaRsp(gCb, rbCb, &AMDL);
   }

   RETVOID;
} 

/**
 *
 * @brief Private handler to construct control PDU
 *
 * @details
 *    This function sets the pduSz correctly after eliminating the fixed
 *    header sizes and the MAC header size. It copies the already prepared
 *    STATUS PDU to the data to be sent to MAC.
 *
 * @param[in]  gCb        RLC instance control block
 * @param[in]  rbCb       Downlink RB control block 
 * @param[in]  kwdatReq   DatReq to be sent to MAC 
 *
 *  @return Void 
 *
 */
#ifdef ANSI
PRIVATE Void rlcAmmDlAssembleCntrlInfo 
(
RlcCb       *gCb,
RlcDlRbCb   *rbCb,
RlcDatReq   *rlcDatReq
)
#else
PRIVATE Void rlcAmmDlAssembleCntrlInfo(gCb, rbCb, rlcDatReq)
RlcCb       *gCb;
RlcDlRbCb   *rbCb;
RlcDatReq   *rlcDatReq;
#endif
{
   RlcUdxDlSapCb   *sapCb;
   MsgLen         macHdrEstmt;

   TRC2(rlcAmmDlAssembleCntrlInfo)

   macHdrEstmt = (rbCb->m.amDl.cntrlBo < 256) ?
                  RLC_MAC_HDR_SZ2 : RLC_MAC_HDR_SZ3;
   /* Eliminate fixed hdr size (14bits including ACK_SN) */
   if (rlcDatReq->pduSz >= (RLC_CNTRL_PDU_FIXED_HDRSZ + macHdrEstmt))
   {
      /* Check the TB size whether it is sufficcient enough to fit the 
         status Pdu into it otherwise make arrangement such that it can fit 
         into in a way of possible NACks*/
      /* ccpu00135743 : fix for MAC Hdr size calc */ 
      rlcDatReq->pduSz -= macHdrEstmt;
      
      /* Create the status Pdu with the required NACKs */
      rlcAmmCreateStatusPdu(gCb,rbCb,rlcDatReq);

      sapCb = RLC_GET_DL_SAPCB(gCb, rbCb);
	   rlcDlUdxStaProhTmrStart(&(gCb->u.dlCb->udxDlSap->pst), 
                             sapCb->suId, &(rbCb->rlcId));
	  
      /* Update number of pdus in pduInfo */
      rlcDatReq->pduInfo.mBuf[rlcDatReq->pduInfo.numPdu] = AMDL.mBuf; 
      rlcDatReq->pduInfo.numPdu++;
      gRlcStats.amRlcStats.numDLStaPduSent++;

      RLC_FREE_SHRABL_BUF(gCb->u.dlCb->udxDlSap->pst.region, 
                         gCb->u.dlCb->udxDlSap->pst.pool,
                         AMDL.pStaPdu,
                         sizeof(RlcUdxDlStaPdu));

      AMDL.pStaPdu = NULLP;
      AMDL.mBuf = NULLP;
      gRlcStats.amRlcStats.numDLStaPduSent++;
   }

   RETVOID;
}

/**
 * @brief Handler to form the PDUs with the size indicated by MAC
 *
 * @details
 *    This function is invoked by UTL with the PDU size indicated by 
 *    MAC (after eliminating MAC header size). It assembles control 
 *    Info / data (New SDUs / Retx PDUs), check if polling needs to be 
 *    set for the data PDU and returns PDU(s) and updated BO with 
 *    estimated header size to be sent to MAC.
 *
 *    - Check if the control BO is available and call rlcAssembleCntrlInfo 
 *      to assemble control Information
 *    - Check if the pdu size is available to form PDUs from retransmission 
 *      buffer and call rlcResegRetxPdus
 *    - Check if the pdu size is available and assemble SDUs from sduQ 
 *      if exist, using rlcAssembleSdus
 *    - PDU Info and bo are filled in and then sent to MAC from the 
 *      utility function
 *
 * @param[in]  gCb          RLC instance control block
 * @param[in]  rbCb         RB control block 
 * @param[in]  kwdatReq     DatReq to be sent to MAC 
 * @param[in]  fillCtrlPdu  Indicates whether cntrl PDU to be filled or not 
 *
 * @return Void
 *
 */
#ifdef ANSI
Void rlcAmmProcessSdus
(
RlcCb       *gCb,
RlcDlRbCb   *rbCb,
RlcDatReq   *rlcDatReq,
Bool       fillCtrlPdu
)
#else
Void rlcAmmProcessSdus(gCb, rbCb, rlcDatReq,fillCtrlPdu)
RlcCb       *gCb;
RlcDlRbCb   *rbCb;
RlcDatReq   *rlcDatReq;
Bool       fillCtrlPdu;
#endif    
{
   TRC2(rlcAmmProcessSdus)


   /* Assemble control information. fillCtrlPdu parameter check is added for CA
    * It is used to force cntrl Pdu scheduling on PCell. for Non CA case this
    * flag will always be TRUE. In CA case, for PCELL it is TRUE and for SCEll
    * it is FALSE */ 

   if ((AMDL.cntrlBo != 0) 
#ifdef LTE_ADV
        && (fillCtrlPdu)
#endif
                               )
   {
      rlcDatReq->boRep.staPduPrsnt = TRUE;
      rlcDatReq->boRep.staPduBo = AMDL.cntrlBo;

      if (AMDL.pStaPdu != NULLP)
      {
         rlcAmmDlAssembleCntrlInfo (gCb, rbCb, rlcDatReq);
      }
      else
      {
         RLOG_ARG2(L_ERROR, DBG_RBID, rbCb->rlcId.rbId,
                  "Miscomputation of control Bo. UEID:%d CELLID:%d",
                  rbCb->rlcId.ueId,
                  rbCb->rlcId.cellId);
      }
      AMDL.cntrlBo = 0;
   }   

   /* Retransmit PDUs /portions of PDUs available in retxLst */
   if ((rlcDatReq->pduSz > 0) && (AMDL.nxtRetx != NULLP))
   {
      rlcResegRetxPdus (gCb,rbCb, rlcDatReq);
   }

   /* Assemble SDUs to form new PDUs */ 
   if ((rlcDatReq->pduSz > 0) && (AMDL.nxtTx != 0))
   {
      rlcAssembleSdus(gCb,rbCb, rlcDatReq); 
   }
  
   if (AMDL.nxtRetx != NULLP)
   {
      rlcDatReq->boRep.oldestSduArrTime = AMDL.nxtRetx->sduMap.sdu->arrTime;
   }
   else if (AMDL.nxtTx != NULLP)
   {
      rlcDatReq->boRep.oldestSduArrTime = AMDL.nxtTx->arrTime;
   }
   /* Accumulate bo */
   rlcDatReq->boRep.bo = rlcAmmCalculateBo(&AMDL);
   rlcDatReq->boRep.staPduBo = AMDL.cntrlBo;

   /* Hdr estimation is moved to kwAmmCreatePDu */
   rlcDatReq->boRep.estHdrSz = AMDL.estHdrSz;

   if(rlcDatReq->pduSz > 0)
   {
      gRlcStats.amRlcStats.numDLBytesUnused += rlcDatReq->pduSz;
   }
   RETVOID;
} 

/**
 * @brief Private handler split a PDU/segment into two
 *
 * @details
 *    Its a private function called by kwResegRetxPdu to split a segment
 *    or a retransmit PDU into two segments splitting at the passed size.
 *    This function is called only for those PDUs that dont have any LIs.
 *
 * @param[in]     gCb    RLC instance control block
 * @param[in]     rbCb   RB control block 
 * @param[in,out] crnt   The PDU to be split, first part of split pdu remians
 *                       in this
 * @param[out]    next   The second part of the split pdu
 * @param[in]     size   The size witin crnt, at which to split   
 *
 * @return  Void
 *
 */
#ifdef ANSI
PRIVATE Void rlcSplitPdu
(
RlcCb       *gCb,
RlcDlRbCb   *rbCb,
RlcRetx     *crnt,
RlcRetx     *next,
U16        size
)
#else
PRIVATE Void rlcSplitPdu(gCb, rbCb, crnt, next, size)
RlcCb       *gCb;
RlcDlRbCb   *rbCb;
RlcRetx     *crnt;
RlcRetx     *next;
U16        size;
#endif
{
   U8            si;
   RlcAmDl        *amDl = &AMDL;

   TRC2(rlcSplitPdu)
   /* Set the SN for the new segment */
   next->amHdr.sn = crnt->amHdr.sn;

   /* Set the protocol specific fields appropriately */
   si = crnt->amHdr.si;
   crnt->amHdr.si = si | RLC_SI_FIRST_SEG;
   next->amHdr.si = si | RLC_SI_LAST_SEG;
   
   crnt->amHdr.p   = 0;

   /* Update seg size */
   next->segSz = crnt->segSz - size;
   crnt->segSz = size;

   /* Set the SO fields appropriately */
   /* MS_FIX for DL stall */
   next->soEnd = crnt->soEnd;

   /* Set the SO fields appropriately */
   /* SO of next will be after the end of current */
   next->amHdr.so = crnt->amHdr.so + crnt->segSz;
   /* SO End of current will be one less than the start of next */
   crnt->soEnd = next->amHdr.so - 1;
   
   /* intialize the other fields in the amHdr of next to 0 */
   next->amHdr.p = 0;
   next->amHdr.dc = 0;
   
   /* This macro is called for No LI case - one SDU */
   /* Update the size of SDU in each node's sduMap  */
   next->sduMap.sdu = crnt->sduMap.sdu;
   crnt->sduMap.sduSz = crnt->segSz;
   next->sduMap.sduSz = next->segSz;

   /* Segment the payload into two parts based on the size passed */
   SSegMsg(crnt->seg, size, &next->seg);
   next->retxCnt     = crnt->retxCnt;
   next->yetToConst  = TRUE;
   next->pendingReTrans    = crnt->pendingReTrans;

   /* Compute the header size and update the BO appropriately */
   if(amDl->snLen == RLC_AM_CFG_12BIT_SN_LEN)
   {
      next->hdrSz = RLC_AM_SEG_12BIT_SN_WITH_SO_HDRSZ;
      if(crnt->amHdr.si == RLC_SI_FIRST_SEG)
      {
         crnt->hdrSz = RLC_AM_SEG_12BIT_SN_WITHOUT_SO_HDRSZ;
      }
      else
      {
         crnt->hdrSz = RLC_AM_SEG_12BIT_SN_WITH_SO_HDRSZ;
      }
   }
   else
   {
      next->hdrSz = RLC_AM_SEG_18BIT_SN_WITH_SO_HDRSZ;
      if(crnt->amHdr.si == RLC_SI_FIRST_SEG)
      {
         crnt->hdrSz = RLC_AM_SEG_18BIT_SN_WITHOUT_SO_HDRSZ;
      }
      else
      {
         crnt->hdrSz = RLC_AM_SEG_18BIT_SN_WITH_SO_HDRSZ;
      }
   }

   /* Add the next to the retx list */
   AMDL.retxLst.crnt = &crnt->lstEnt;
   CM_LLIST_INS_AFT_CRNT(AMDL.retxLst, next); 
   AMDL.nxtRetx = next;
   amDl->estHdrSz += next->hdrSz;
   
   RETVOID;
}

/**
 * @brief Private handler to retransmit PDUs or PDU segments
 *
 * @details
 *    Its a private function called by kwProcessSdus, to create the 
 *    PDUs / its segments from the retransmission buffer available in RbCb.
 *       
 *    - Eliminate the fixed header size and MAC header size while 
 *      forming PDUs/segments
 *    - While pdusize is available and retxBuf has data (pdu or portion 
 *      of pdu) to be sent, form the pdu as it is if it matches with the 
 *      pdusize else segment the PDUs/portion of PDUs
 *    - Call rlcAmmDlCheckAndSetPoll function to check and set the poll bit as 
 *      required 
 *    - Concatenate data and header info and fill pduInfo 
 *    - Update retxCnt and send indication to PDCP if it reaches maxRetx 
 *      threshold
 *
 * @param[in]  gCb       RLC instance control block
 * @param[in]  rbCb      RB control block
 * @param[in]  kwdatReq  DatReq to be sent to MAC 
 *
 * @return  Void
 *
 */
#ifdef ANSI
PRIVATE Void rlcResegRetxPdus 
(
RlcCb       *gCb,
RlcDlRbCb   *rbCb,
RlcDatReq   *rlcDatReq
)
#else
PRIVATE Void rlcResegRetxPdus(gCb, rbCb, rlcDatReq)
RlcCb       *gCb;
RlcDlRbCb   *rbCb;
RlcDatReq   *rlcDatReq;
#endif
{
   RlcAmDl   *amDl;
   RlcRetx   *retx;
   U8       hdr[RLC_MAX_HDRSZ];
   U16      idx; 
   Buffer   *pdu;
   MsgLen   pduSz; 
#ifdef LTE_L2_MEAS
   U16        sduIdx;
   RlcL2MeasTb *l2MeasTb;
   RlclchInfo  *lchInfo;
   U8         numSdus;
#endif

   TRC2(rlcResegRetxPdus)


   amDl  = &AMDL;
#ifdef LTE_L2_MEAS
   /* TODO : This shoould be taken care in new Trasmissions */
   /* This lchInfo should be retrieved there */
   l2MeasTb = rlcUtlGetCurMeasTb(gCb, rbCb);
   if (l2MeasTb == NULLP)
   {
      RETVOID;
   }
   /* TODO : This lcid needs to be searched in case of normal Tx */
   /* In retx here, its fine as this will be higher priority */
   lchInfo = &l2MeasTb->lchInfo[l2MeasTb->numLchInfo];
   if (l2MeasTb->numLchInfo >= RLC_MAX_ACTV_DRB)
   {
      RETVOID;
   }
   l2MeasTb->numLchInfo++;
   lchInfo->lcId = rbCb->lch.lChId;
   lchInfo->numSdus = 0;
#endif

   while ((rlcDatReq->pduSz > 0) && (amDl->nxtRetx != NULLP)&&
          (rlcDatReq->pduInfo.numPdu < RLC_MAX_PDU))
   {
      U16 tmpSz;
      
      retx = amDl->nxtRetx;
      /* kw003.201 : Add header size to seg size to determine if the      */
      /*             the segment can be completed within the allocation   */
      /* kw003.201 - Eliminate MAC Header Size based on bites needed      */
      tmpSz = RLC_MIN((retx->segSz + retx->hdrSz), rlcDatReq->pduSz);
      pduSz = (retx->segSz + retx->hdrSz);
      /* 5GNR_RLC: length field in 5GNR MAC Hdr is 8/16 btis*/
      rlcDatReq->pduSz -= (tmpSz < 255) ?  RLC_MAC_HDR_SZ2 : RLC_MAC_HDR_SZ3;

      /* kw003.201 - We should have at least one more than basic header */
      if (rlcDatReq->pduSz <= retx->hdrSz)
      {
         RETVOID;
      }
      rlcGetNxtRetx(gCb, &(amDl->nxtRetx));

      /* Send retx buf without segmentation */
      if (rlcDatReq->pduSz >= pduSz)
      {
         U8 pollBit;
         
         RLOG_ARG2(L_DEBUG,DBG_RBID,rbCb->rlcId.rbId, 
                  "rlcResegRetxPdus: Send retx buf without segmentation "
                  "UEID:%d CELLID:%d", 
                  rbCb->rlcId.ueId,
                  rbCb->rlcId.cellId);

         if (retx->yetToConst)
         {
            /* Construct hdr with the available hdr values */ 
            rlcConstructAmHdr(&retx->amHdr, hdr, amDl->snLen, &idx);
            /* Add header to the pdu/segment */
            SAddPreMsgMultInOrder(hdr, idx + 1, retx->seg);
            retx->yetToConst = FALSE;
         } 

         /* kw003.201 - Check if poll bit needs to be set. Retx size does */
         /* not affect the poll bit so it is being passed as zero         */ 
         pollBit = rlcAmmDlCheckAndSetPoll(gCb,rbCb, FALSE, 0);
         RLC_UPD_POLL_BIT(gCb, retx, pollBit);

         rlcDatReq->pduSz  -= pduSz;
         AMDL.estHdrSz    -= retx->hdrSz;
#ifdef LTE_L2_MEAS   

         if (rbCb->rlcId.rbType == CM_LTE_DRB)
         {
            numSdus = 0;
            for (sduIdx = lchInfo->numSdus ; 
                  ((numSdus < retx->numSdu) && (sduIdx < RLC_L2MEAS_SDUIDX)) ; 
                  sduIdx++, numSdus++)
            {
               lchInfo->sduInfo[sduIdx].arvlTime = retx->sduMap[numSdus].sdu->arrTime;
               lchInfo->sduInfo[sduIdx].isRetxPdu = TRUE; /* TODO : for later use */
            }
            lchInfo->numSdus += numSdus;
         }
#endif
      }
      else
      {
         RlcRetx *tNode;
         
         /* Segment this pdu / portion of pdu. Insert this segment into */
         /* retxLst and update offset                                   */
         RLOG_ARG2(L_DEBUG,DBG_RBID,rbCb->rlcId.rbId,
                  "rlcResegRetxPdus: Segment retx buf UEID:%d CELLID:%d",
                  rbCb->rlcId.ueId,
                  rbCb->rlcId.cellId);

         /* Eliminate fixed header size if the pdu is segmented for the */
         /* first time                                                  */
         if(amDl->snLen == RLC_AM_CFG_12BIT_SN_LEN)
         {
            if(retx->amHdr.si < RLC_SI_LAST_SEG)
            {
               rlcDatReq->pduSz -= RLC_AM_SEG_12BIT_SN_WITHOUT_SO_HDRSZ;
            }
            else
            {
               rlcDatReq->pduSz -= RLC_AM_SEG_12BIT_SN_WITH_SO_HDRSZ;
            }
         }
         else
         {
            if(retx->amHdr.si < RLC_SI_LAST_SEG)
            {
               rlcDatReq->pduSz -= RLC_AM_SEG_18BIT_SN_WITHOUT_SO_HDRSZ;
            }
            else
            {
               rlcDatReq->pduSz -= RLC_AM_SEG_18BIT_SN_WITH_SO_HDRSZ;
            }
         }
         if (rlcDatReq->pduSz <= 0)
         {
            RETVOID;
         }

         /* Allocate memory for tracking a new segment */
         RLC_ALLOC_WC(gCb,tNode, sizeof(RlcRetx)); 
#if (ERRCLASS & ERRCLS_ADD_RES)
         if (tNode == NULLP)
         {
            RLOG_ARG2(L_FATAL,DBG_RBID,rbCb->rlcId.rbId,
                     "Memory allocation failed UEID:%d CELLID:%d",
                     rbCb->rlcId.ueId,
                     rbCb->rlcId.cellId);
            RETVOID;
         }
#endif /* ERRCLASS & ERRCLS_RES */

         /* initialize the list pointer to 0 instead of memset */
         tNode->lstEnt.next = 0;
         tNode->lstEnt.prev = 0;
         
         /* Segment header and data */
         RLC_AM_RMV_HDR(gCb, rbCb, retx);

         /* kw003.201 - Split the payload and update other fields */
         rlcSplitPdu(gCb,rbCb, retx, tNode, rlcDatReq->pduSz); 

#ifdef LTE_L2_MEAS
         numSdus = 0;
         /* ccpu00143043 */
         sduIdx = lchInfo->numSdus;
         for (numSdus = 0, sduIdx = lchInfo->numSdus; 
              ((numSdus < retx->numSdu) && (sduIdx < RLC_L2MEAS_SDUIDX));
              numSdus++, sduIdx++)
         {
            lchInfo->sduInfo[sduIdx].arvlTime = 
                  retx->sduMap[numSdus].sdu->arrTime;
            lchInfo->sduInfo[sduIdx].isRetxPdu = TRUE;
         }
         lchInfo->numSdus = sduIdx;
         if ((retx->amHdr.lsf == 0) && (lchInfo->numSdus > 0))
         {
            lchInfo->numSdus--;
         }
#endif
         /* Construct hdr with the available hdr values */
         rlcConstructAmHdr(&retx->amHdr, hdr, amDl->snLen, &idx);
         SAddPreMsgMultInOrder(hdr, idx + 1, retx->seg);

         retx->hdrSz = idx + 1;

         /* Poll bit need not be set for this seg, since its second  */
         /* half remains in retxLst                                  */
         RLC_UPD_POLL_BIT(gCb, retx, FALSE);
         retx->yetToConst = FALSE;
         rlcDatReq->pduSz = 0; 
      }

      rlcCpyMsg(gCb,retx->seg, &pdu);

      /* Update pduInfo */
      rlcDatReq->pduInfo.mBuf[rlcDatReq->pduInfo.numPdu] = pdu;
      rlcDatReq->pduInfo.numPdu++;
      /* kw005.201 ccpu00117318, updating the statistics */
      gCb->genSts.pdusRetx += 1;
      gRlcStats.amRlcStats.numRlcAmCellRetxPdu++;
      retx->soEnd = retx->amHdr.so + retx->segSz - 1;
      retx->pendingReTrans    = FALSE;
      amDl->retxBo -= retx->segSz;
   }
#ifndef ALIGN_64BIT
   RLOG_ARG3(L_DEBUG,DBG_RBID,rbCb->rlcId.rbId, 
                 "rlcResegRetxPdus: retxBo after resegmentation = %ld"
                 "UEID:%d CELLID:%d", 
                 amDl->retxBo,
                 rbCb->rlcId.ueId,
                 rbCb->rlcId.cellId);
#else
   RLOG_ARG3(L_DEBUG,DBG_RBID,rbCb->rlcId.rbId,
                 "rlcResegRetxPdus: retxBo after resegmentation = %d "
                 "UEID:%d CELLID:%d", 
                 amDl->retxBo,
                 rbCb->rlcId.ueId,
                 rbCb->rlcId.cellId);
#endif

   RETVOID;
}


/**
 * @brief Private handler to assemble SDUs to form new data PDU(s)
 *
 * @details
 *    Its a private function called by kwProcessSdus, to create the new data 
 *    PDUs from the SDU queue of RbCb.
 *
 *    - While pdusize is available, segment/concatenate SDUs or else if it
 *      matches the pdu size form PDUs accordingly.
 *    - RLC header and MAC header size are eliminated while forming the PDUs
 *    - Call rlcAmmDlCheckAndSetPoll function to check and set the poll bit 
 *      as required 
 *    - Concatenate data and header info and fill pduInfo  
 *
 * @param[in]  rbCb     RB control block
 * @param[in]  kwdatReq DatReq to be sent to MAC 
 *
 * @return  Void 
 *
 */
#ifdef ANSI
PRIVATE Void rlcAssembleSdus 
(
RlcCb       *gCb,
RlcDlRbCb   *rbCb,
RlcDatReq   *rlcDatReq
)
#else
PRIVATE Void rlcAssembleSdus (gCb, rbCb, rlcDatReq)
RlcCb       *gCb;
RlcDlRbCb   *rbCb;
RlcDatReq   *rlcDatReq;
#endif
{
   Buffer          *pdu         = NULLP;
   MsgLen          macGrntSz    = rlcDatReq->pduSz;
   RlcAmDl          *amDl        = &AMDL;
   RlcSdu           *sdu         = amDl->nxtTx;
   RlcSduMap        sduMap;
   Bool            nxtTxUpd     = FALSE;
   KwuDiscSduInfo  *discSduInfo = NULLP;
   RlcKwuSapCb* rlckwuSap = gCb->u.dlCb->rlcKwuDlSap + RLC_UI_PDCP;
#ifdef LTE_L2_MEAS
   RlcContSduLst     contSduLst;  /*Contained sduLst */
   S32              dataVol    = amDl->bo;
   U32              *totMacGrant = &rlcDatReq->totMacGrant;
   RlcL2MeasDlIpTh   *dlIpThPut = &rbCb->l2MeasIpThruput.dlIpTh;
   U8               *sduIdx    = &dlIpThPut->lastSduIdx;
   Bool             newIdx;
   Bool             isSduSegmented;
   S32              oldBo;
   RlclchInfo        lchInfo;
   RlclchInfo        *dstLchInfo;
   U32              segSduCnt = 0;
   U32              lchIdx;
   U32              numSdus = 0;
   U32              currSduIdx = 0;
   RlcL2MeasTb       *l2MeasTb;
#endif
   /* Discard new changes starts */
   Ticks                timeDiff = 0;
   Ticks                curTime  = 0;
   U8                   numNewPdu = 0;
   RlcTx                 *txBuf = NULLP;
   /* Discard new changes ends */
   VOLATILE U32         startTime = 0;
   U32                  hdrEstmt;
   U32                  fixedHdrSz;
   U32                  pduSz;
   RlcAmHdr              *amHdr = NULLP;
   RlcDlPduInfo          *pduInfo = NULLP;

   TRC2(rlcAssembleSdus)


#ifdef LTE_L2_MEAS   
   contSduLst.numSdus = 0; 
   contSduLst.lcId = rbCb->lch.lChId;
   oldBo = amDl->bo;
   lchInfo.lcId = rbCb->lch.lChId;
   lchInfo.numSdus = 0;
#endif
   /* Discard new changes starts */
   /* Allocate memory for discSdu Info */
   RLC_SHRABL_STATIC_BUF_ALLOC(rlckwuSap->pst.region,
                              rlckwuSap->pst.pool,
                              discSduInfo, 
                              sizeof(KwuDiscSduInfo));

#if (ERRCLASS & ERRCLS_ADD_RES)
   if (discSduInfo == NULLP)
   {
      RLOG_ARG2(L_FATAL,DBG_RBID,rbCb->rlcId.rbId, 
               "Memory allocation failed UEID:%d CELLID:%d",
               rbCb->rlcId.ueId,
               rbCb->rlcId.cellId);
      RETVOID;
   }
#endif /* ERRCLASS & ERRCLS_RES */

   discSduInfo->numSduIds = 0;
   discSduInfo->rlcId = rbCb->rlcId;

   rlcUtlGetCurrTime(&curTime);
   amDl->sduQ.crnt =  &sdu->lstEnt;
   /* Eliminate fixed header size */
   /*5GNR: value of RLC_AM_PDU_FIXED_HDRSZ will be 2 or 3 depending on SN Size*/
   if(amDl->snLen == RLC_AM_CFG_12BIT_SN_LEN)
   {
      fixedHdrSz   = RLC_AM_PDU_12BIT_SN_HDRSZ;
   }
   else
   {
      fixedHdrSz   = RLC_AM_PDU_18BIT_SN_HDRSZ;
   }

   while ((macGrntSz > fixedHdrSz) && (sdu != NULLP) &&
          (rlcDatReq->pduInfo.numPdu < RLC_MAX_PDU) && 
          (numNewPdu < RLC_MAX_NEW_DL_PDU))
   {
#ifdef LTE_L2_MEAS   
      isSduSegmented = sdu->mode.am.isSegmented;
#endif
      /* Discard new changes starts */
      if ((sdu->mode.am.isSegmented == FALSE) && (rbCb->discTmrInt > 0) && \
            (rbCb->rlcId.rbType == CM_LTE_DRB))
      {
         //leftAmSdus[rbCb->qci]--;
         timeDiff = RLC_TIME_DIFF(curTime,sdu->arrTime); 
         if (timeDiff > rbCb->discTmrInt)
         {
            CmLList* nxtNode;
            /*starting Task*/
            SStartTask(&startTime, PID_RLC_AMM_DISC_SDUS);
#ifdef LTE_L2_MEAS 
            RLC_UPD_L2_DL_DISC_SDU_STS(gCb,rbCb);
            /* TODO need to send disc cfm to pdcp */
#endif
            /* Update bo for boReport */
            amDl->bo -= sdu->sduSz;

            /* Get next sdu for assembly */
            nxtNode = sdu->lstEnt.next;

            /* store the info for sending it to PDCP */
            if(discSduInfo->numSduIds > 500)
            {
               RLOG_ARG2(L_ERROR,DBG_RBID, rbCb->rlcId.rbId, 
                     "This is a big error, we shouldn't be here"
                     "UEID:%d CELLID:%d",
                     rbCb->rlcId.ueId,
                     rbCb->rlcId.cellId);
               break;
            }

            discSduInfo->sduIds[discSduInfo->numSduIds] = sdu->mode.am.sduId;
            discSduInfo->numSduIds++;

            cmLListDelFrm(&amDl->sduQ, &sdu->lstEnt);

            rlcUtlAddSduToBeFreedQueue(gCb, sdu);
            rlcUtlRaiseDlCleanupEvent(gCb);

            /* We need to restore the crnt in the linked list which
             * would have become NULL in the DelFrm above */
            amDl->sduQ.crnt = nxtNode;

            if(nxtNode)
               sdu = (RlcSdu*)nxtNode->node;
            else
               sdu = NULLP;

            /*stopping Task*/
            SStopTask(startTime, PID_RLC_AMM_DISC_SDUS);
            continue;
         }
      }
      nxtTxUpd = FALSE;

#ifdef LTE_L2_MEAS
      newIdx = FALSE;
#endif
      /** kw003.201 - Check for window stall when you are
       *  creating a new PDU
       */
      if (RLC_AM_IS_TRANS_WIN_STALLED(amDl))
      {
         //int *a = NULLP;
         printf("\n Window stalled  \n");
         gRlcStats.amRlcStats.numRlcAmCellWinStall++;
         //*a = 10;
         break;
      }

      hdrEstmt = fixedHdrSz; 

      if (sdu->mode.am.isSegmented)
      {
         /* Adding two byte for SO */
         hdrEstmt += 2;
      } 
      /* Eliminate MAC header */
      /* ccpu00135743 : Fix for MAC Hdr size calculation */
      /*5GNR: value of mac hdr length field changed to 8/16bits  */
      pduSz = RLC_MIN(macGrntSz, (sdu->sduSz + hdrEstmt));
      hdrEstmt += (pduSz < 255) ? RLC_MAC_HDR_SZ2 : RLC_MAC_HDR_SZ3;

      macGrntSz -= hdrEstmt;
      /* kw005.201 Check for PDU Size is large enough.
       * Fix for ccpu00118973 
       * */
      if(macGrntSz <= 0)
      {
         break;
      }

      /* Dont create new txBuf for segmented SDU */
      if (!sdu->mode.am.isSegmented)
      {
         /* Update txBuf */
         RLC_ALLOC_WC(gCb,txBuf, sizeof(RlcTx));

         cmLListInit(&txBuf->pduLst);

#if (ERRCLASS & ERRCLS_ADD_RES)
         if (txBuf == NULLP)
         {
            U32 avblMem = 0;
            SRegInfoShow(gCb->init.region, &avblMem);
            RLOG_ARG2(L_FATAL,DBG_RBID,rbCb->rlcId.rbId, 
                  "Memory allocation failed UEID:%d CELLID:%d",
                  rbCb->rlcId.ueId,
                  rbCb->rlcId.cellId);
            RETVOID;
         }
#endif /* ERRCLASS & ERRCLS_RES */

         rlcUtlStoreTxBuf(amDl->txBufLst, txBuf, amDl->txNext);
      }
      else
      {
         txBuf = rlcUtlGetTxBuf(amDl->txBufLst, amDl->txNext);
      }

      RLC_ALLOC_WC(gCb,pduInfo, sizeof(RlcDlPduInfo));
#if (ERRCLASS & ERRCLS_ADD_RES)
      if (pduInfo == NULLP)
      {
         U32 avblMem = 0;
         SRegInfoShow(gCb->init.region, &avblMem);
         RLOG_ARG2(L_FATAL,DBG_RBID,rbCb->rlcId.rbId, 
               "Memory allocation failed UEID:%d CELLID:%d",
               rbCb->rlcId.ueId,
               rbCb->rlcId.cellId);
         RETVOID;
      }
#endif /* ERRCLASS & ERRCLS_RES */

      /*Initialize DL segment structure */
      pduInfo->lstEnt.next = NULLP;
      pduInfo->lstEnt.prev = NULLP;
      pduInfo->lstEnt.node = NULLP;

      pduInfo->pdu = NULLP;
      pduInfo->amHdr.dc = 0;
      pduInfo->amHdr.p = 0;
      pduInfo->amHdr.si = 0;
      pduInfo->amHdr.so = 0;

      pduInfo->amHdr.sn = amDl->txNext;

      if (macGrntSz >= sdu->sduSz)
      {
         pdu = sdu->mBuf;     
         sdu->mBuf = NULLP; 
         /* Update Framing Info */
         if (sdu->mode.am.isSegmented) 
         {
            /*5GNR RLC: SN should be same for all segment of a SDU*/
            pduInfo->amHdr.sn = sdu->mode.am.sn;
            pduInfo->amHdr.si = RLC_SI_LAST_SEG; /* binary 10 */
            pduInfo->amHdr.so = sdu->actSz - sdu->sduSz;
            sdu->mode.am.isSegmented = FALSE;


            gRlcStats.amRlcStats.numRlcAmCellSduTx++;
            //printf("\n 5GNRLOG: last segment of lcId %d SduId %u So %u macGrntSz %u sduActSz %u sdu->sduSz %u\n",
            //          rbCb->lch.lChId, sdu->mode.am.sduId, pduInfo->amHdr.so, macGrntSz, sdu->actSz, sdu->sduSz);
         }
         else
         {
            gRlcStats.amRlcStats.numRlcAmCellSduTx++;
         }
         amHdr          = &pduInfo->amHdr; 
         /* Create PDU with hdr and data */
         rlcAmmCreatePdu(gCb,rbCb, amHdr, pduInfo, pdu);

         //printf("\n Segmentation not required case: numPdu %d pdu  %p \n",rlcDatReq->pduInfo.numPdu, pdu);

#ifdef LTE_L2_MEAS_RLC
         rlcUtlUpdSduSnMap(rbCb, sdu, rlcDatReq, TRUE);
#endif /*  LTE_L2_MEAS */

         /* kw005.201 ccpu00117318, updating the statistics */
         rlcUtlIncrementKwuStsSduTx(gCb->u.dlCb->rlcKwuDlSap + rbCb->k1wuSapId);
#ifdef LTE_L2_MEAS
         if(RLC_MEAS_IS_DL_ANY_MEAS_ON_FOR_RB(gCb,rbCb))
         {
            if(isSduSegmented)
            {
               *sduIdx    = dlIpThPut->lastSduIdx;
            }
            else
            {
               RLC_GETSDUIDX(*sduIdx);
               newIdx = TRUE;
            }
            rlcUtlUpdateContainedSduLst(*sduIdx, &contSduLst);
            rlcUtlUpdateOutStandingSduLst(dlIpThPut, *sduIdx, sdu->actSz, 
                  sdu->mode.am.sduId, newIdx);
            /* Update the arrival time for each SDU */
            /* ccpu00143043 */
            if ( lchInfo.numSdus < RLC_L2MEAS_SDUIDX)
            {
               lchInfo.sduInfo[lchInfo.numSdus].arvlTime = sdu->arrTime; 
               lchInfo.numSdus++;
            }
         }
#endif
         sduMap.sduSz    = sdu->sduSz;
      }
      else 
      {
         /* Segmentation
          * Allocate buffer for next PDU
          * Remove the segmented portion from SDUQ 
          * Calculate the hdr with LI for SDU */

         Buffer  *remSeg = NULLP;

         //printf("\n SDU segmentation case: numPdu %d pdu %p \n", rlcDatReq->pduInfo.numPdu, pdu);
#ifdef LTE_L2_MEAS
         if(RLC_MEAS_IS_DL_IP_MEAS_ON_FOR_RB(gCb,rbCb) ||
               RLC_MEAS_IS_DL_DELAY_MEAS_ON_FOR_RB(gCb,rbCb) || 
               RLC_MEAS_IS_DL_UU_LOSS_MEAS_ON_FOR_RB(gCb,rbCb) )
         {
            /* If actual size of the sdu is equal to msgLen
             * then it is first segment of the SDU */
            if(sdu->actSz == sdu->sduSz)
            {
               RLC_GETSDUIDX(*sduIdx);
               newIdx = TRUE;
            }
            else
            {
               *sduIdx    = dlIpThPut->lastSduIdx;
            }
            rlcUtlUpdateContainedSduLst(*sduIdx, &contSduLst);
            rlcUtlUpdateOutStandingSduLst(dlIpThPut, *sduIdx, sdu->actSz, 
                  sdu->mode.am.sduId, newIdx);
            if(RLC_MEAS_IS_DL_UU_LOSS_MEAS_ON_FOR_RB(gCb,rbCb))
            {
               /* If actual size of the sdu is equal to msgLen
                * then it is first segment of the SDU */
               if(sdu->actSz == sdu->sduSz)
               {
                  segSduCnt++;
               }
            }
         }
#endif

         /* Segment the SDU to the size of the PDU and update header Info */
         SSegMsg(sdu->mBuf, macGrntSz, &remSeg);
         pdu = sdu->mBuf;      
         sdu->mBuf = remSeg;

         /* Update SI and SN */
         if (sdu->mode.am.isSegmented) 
         {
            /*5GNR RLC: SN should be same for all segment of a SDU.
             * Sdu was already segmented and segmenting again*/
            pduInfo->amHdr.sn = sdu->mode.am.sn;
            pduInfo->amHdr.si = RLC_SI_MID_SEG; /* binary 11 */
            pduInfo->amHdr.so = sdu->actSz - sdu->sduSz;

            //printf("\n 5GNRLOG: mid segment of lcId %d SduId %u So %u macGrntSz %u sduActSz %u sdu->sduSz %u\n",
            //         rbCb->lch.lChId, sdu->mode.am.sduId, txBuf->amHdr.so, macGrntSz, sdu->actSz, sdu->sduSz);
         }
         else
         {
            /*5GNR RLC: This means it is the first*/
            pduInfo->amHdr.si = RLC_SI_FIRST_SEG; /* binary 01 */
            /*5GNR_RLC: Store SN so that in sub-seqent SDU segments will use this SN*/
            sdu->mode.am.sn = pduInfo->amHdr.sn;
            pduInfo->amHdr.so = 0;

            //printf("\n 5GNRLOG: First segment of lcId %d SduId %u So %u macGrntSz %u sduActSz %u sdu->sduSz %u\n", 
            //         rbCb->lch.lChId, sdu->mode.am.sduId, txBuf->amHdr.so, macGrntSz, sdu->actSz, sdu->sduSz);
         }

         amHdr = &pduInfo->amHdr; 
         /* Create PDU with hdr and data */
         rlcAmmCreatePdu(gCb,rbCb, amHdr, pduInfo, pdu);

         sdu->mode.am.isSegmented = TRUE;
         sdu->sduSz -= macGrntSz;
         sduMap.sduSz = macGrntSz;

#ifdef LTE_L2_MEAS_RLC
         rlcUtlUpdSduSnMap(rbCb, sdu, rlcDatReq, FALSE);
#endif /*  LTE_L2_MEAS */

         amDl->nxtTx = sdu;
         nxtTxUpd    = TRUE;
      }

      /* Update bo for boReport */
      amDl->bo -= sduMap.sduSz;

      sduMap.sdu = sdu;

      /* Update pduInfo */
      rlcDatReq->pduInfo.mBuf[rlcDatReq->pduInfo.numPdu] = pdu; 
      rlcDatReq->pduInfo.numPdu++;
      numNewPdu++;
      /* kw005.201 ccpu00117318, updating the statistics */
      gCb->genSts.pdusSent++;
      gRlcStats.amRlcStats.numRlcAmCellSduBytesTx = gRlcStats.amRlcStats.numRlcAmCellSduBytesTx + sduMap.sduSz;
      /* Update the RLC Tx buffer with the new PDU info */
      RLC_MEM_CPY(&pduInfo->sduMap, &sduMap, sizeof(RlcSduMap));
      pdu = NULLP;

      macGrntSz -= sduMap.sduSz;
      /* Get next sdu for assembly */
      RLC_LLIST_NEXT_SDU(amDl->sduQ, sdu);

   } /*End of pduSz loop */

   rlcDatReq->pduSz = macGrntSz;
   /* Updating nxtTx to sdu in the Q */
   if (!nxtTxUpd)
      amDl->nxtTx = sdu;

#ifdef LTE_L2_MEAS
   if(RLC_MEAS_IS_DL_ANY_MEAS_ON_FOR_RB(gCb,rbCb) && 
         (rbCb->rlcId.rbType == CM_LTE_DRB))
   {
      numSdus = 0;
      currSduIdx = 0;
      l2MeasTb = rlcUtlGetCurMeasTb(gCb, rbCb);
      rlcUtlUpdateBurstSdus(gCb, rbCb, &contSduLst, dataVol, *totMacGrant);
      if ((lchInfo.numSdus != 0) && (l2MeasTb != NULLP))
      {
         for (lchIdx = 0; ((lchIdx < l2MeasTb->numLchInfo)
                  && (lchIdx < RLC_MAX_ACTV_DRB )); lchIdx++)
         {
            if (l2MeasTb->lchInfo[lchIdx].lcId == rbCb->lch.lChId)
            {
               /* Lch Info already added in Retx procedure */
               break;
            }
         }
         if (lchIdx < RLC_MAX_ACTV_DRB)
         {
            if (lchIdx == l2MeasTb->numLchInfo)
            {
               l2MeasTb->lchInfo[lchIdx].lcId = rbCb->lch.lChId;
               l2MeasTb->lchInfo[lchIdx].numSdus = 0;
               l2MeasTb->numLchInfo++;
            }
            dstLchInfo = &l2MeasTb->lchInfo[lchIdx];
            currSduIdx = l2MeasTb->lchInfo[lchIdx].numSdus;
            while ((numSdus < lchInfo.numSdus) && (currSduIdx < RLC_L2MEAS_SDUIDX)) 
            {
               dstLchInfo->sduInfo[currSduIdx].arvlTime = lchInfo.sduInfo[numSdus].arvlTime;
               dstLchInfo->sduInfo[currSduIdx].isRetxPdu = FALSE;
               numSdus++;
               currSduIdx++;
            }
            l2MeasTb->lchInfo[lchIdx].numSdus += numSdus;
         }
      }
      /* Fix Klock warning */
      if(l2MeasTb != NULLP)
      {
         l2MeasTb->txSegSduCnt += segSduCnt;
      }
   }
   *totMacGrant -= (oldBo - amDl->bo);
#endif

   if(discSduInfo->numSduIds != 0)
   {
      /* Sap control block */
      RlcUiKwuDiscSduCfm(&rlckwuSap->pst, rlckwuSap->suId, discSduInfo);
   }
   else
   {
      RLC_SHRABL_STATIC_BUF_FREE(rlckwuSap->pst.region, rlckwuSap->pst.pool, discSduInfo, sizeof(KwuDiscSduInfo));
   }


#ifndef ALIGN_64BIT
   RLOG_ARG3(L_UNUSED,DBG_RBID,rbCb->rlcId.rbId, 
         "rlcAssembleSdus: BO after assembly = %ld UEID:%d CELLID:%d",
         amDl->bo,
         rbCb->rlcId.ueId,
         rbCb->rlcId.cellId);
#else
   RLOG_ARG3(L_UNUSED,DBG_RBID,rbCb->rlcId.rbId,
         "rlcAssembleSdus: BO after assembly = %d UEID:%d CELLID:%d",
         amDl->bo,
         rbCb->rlcId.ueId,
         rbCb->rlcId.cellId);
#endif

   RETVOID;
}

/**
 * @brief Private handler to check if the poll bit needs to be set for data PDU
 *
 * @details
 *    Its a private function called by kwProcessSdus, to checks if the 
 *    polling bit needs to be set for any RLC data PDU and updates the 
 *    same.
 *    - For the new PDUs, if the counters exceed the configured 
 *      pduWoPoll/byteWoPoll values, return poll bit.
 *    - For the PDUs/portion of PDUs, if the SDU list / retxBuf is 
 *      empty, return poll bit.
 *    - Update the pollPdu, pollByte counters and Poll_SN; start staProhTmr 
 *
 * @param[in]  rCb      RLC instance control block
 * @param[in]  rbCb     RB control block 
 * @param[in]  newPdu   Flag to indicate if its a new AMD PDU. 
 * @param[in]  bufSz    Length of the PDU
 *
 * @return  Bool 
 *      -# 1 - To set the poll bit
 *      -# 0 - Poll bit is not set
 *
 */
#ifdef ANSI
PRIVATE Bool rlcAmmDlCheckAndSetPoll
(
RlcCb       *gCb,
RlcDlRbCb   *rbCb,
Bool       newPdu,
MsgLen     bufSz
)
#else
PRIVATE Bool rlcAmmDlCheckAndSetPoll(gCb, rbCb, newPdu, bufSz)
RlcCb       *gCb;
RlcDlRbCb   *rbCb;
Bool       newPdu;
MsgLen     bufSz;
#endif
{ 
   Bool     pollBit = FALSE;
   RlcAmDl   *amDl = &(rbCb->m.amDl);

   TRC2(rlcAmmDlCheckAndSetPoll)

 
   /* If it's a new PDU increment PDU without poll and bytes without poll
    and check if they cross the configured number of poll pdu and poll bytes*/ 
   if (newPdu)
   {
      amDl->pduWoPoll++;
      /* Patch kw004.201 */
      amDl->byteWoPoll += bufSz;

     if (((amDl->pollPdu != -1) && (amDl->pduWoPoll >= amDl->pollPdu)) || 
         ((amDl->pollByte != -1) && (amDl->byteWoPoll >= amDl->pollByte))) 
     {
        pollBit = TRUE;
     }
   }

   /* Check if both tx/retx buffer are empty or if tx window is stalled */
   if (((amDl->nxtTx == NULLP) && (amDl->nxtRetx == NULLP)) ||
       RLC_AM_IS_TRANS_WIN_STALLED(amDl))
   {
      pollBit = TRUE;
   }
   
   if (pollBit)
   {
      amDl->pduWoPoll  = 0;
      amDl->byteWoPoll = 0;

      amDl->pollSn = (amDl->txNext - 1) & amDl->snModMask;

      RLOG_ARG3(L_UNUSED,DBG_RBID,rbCb->rlcId.rbId, 
                "rlcAmmDlCheckAndSetPoll: Poll SN = %d UEID:%d CELLID:%d", 
                amDl->pollSn,
                rbCb->rlcId.ueId,
                rbCb->rlcId.cellId);

      /* kw005.201: Fix for poll retransmission timer. 
       * Timer is stopped if it is already running and 
       * then starting the timer. Fixes crs 
       * ccpu00117216 and ccpu00118284 .
       * */
      if( TRUE == rlcChkTmr(gCb,(PTR)rbCb,RLC_EVT_AMDL_POLL_RETX_TMR) )
      {
         rlcStopTmr(gCb,(PTR)rbCb, RLC_EVT_AMDL_POLL_RETX_TMR);
      }

      rlcStartTmr(gCb,(PTR)rbCb, RLC_EVT_AMDL_POLL_RETX_TMR);
   }

   return (pollBit);
}

/**
 * @brief Private handler to create AMD PDU 
 *
 * @details
 *    This function constructs header and concatenate it with the data for
 *    the PDU. It also updates the txBuf with the created PDU.
 *
 * @param[in]  gCB         RLC instance control block
 * @param[in]  rbCb        Downlink RB control block
 * @param[in]  amHdr       AM header
 * @param[in]  RlcDlPduInfo Pointer to PduInfo
 * @param[in]  pdu         PDU buffer 
 *
 *  @return  Void
 *
 */
#ifdef ANSI
PRIVATE Void rlcAmmCreatePdu
(  
RlcCb       *gCb,
RlcDlRbCb   *rbCb,
RlcAmHdr    *amHdr,
RlcDlPduInfo *pduInfo,
Buffer     *pdu
)
#else
PRIVATE Void rlcAmmCreatePdu(gCb, rbCb, pduInfo, amHdr, pdu)
RlcCb       *gCb;
RlcDlRbCb   *rbCb;
RlcAmHdr    *amHdr;
RlcDlPduInfo *pduInfo;
Buffer     *pdu;
#endif
{
   U8       hdr[RLC_MAX_HDRSZ];
   U16      idx;
   RlcTx     *txBuf;
   MsgLen   pduSz;
   RlcAmDl   *amDl = &(rbCb->m.amDl);

   TRC2(rlcAmmCreatePdu)


   /* Update sn */
   amHdr->sn = amDl->txNext;

   /*5GNR RLC: Increment txNext only if no segmentation of it is a last segment */
   if((!amHdr->si) || (amHdr->si == RLC_SI_LAST_SEG))
   {
      //printf("\n 5GNRLOG: no segment/last seg SDU with lcId %d Sn %u txNext %u So %u\n",
      //          rbCb->lch.lChId, amHdr->sn, amDl->txNext, amHdr->so);
      amDl->txNext = (amDl->txNext + 1) & amDl->snModMask;
   }

   /* Update hdr Info */
   SFndLenMsg(pdu, &pduSz);

   /* passing newPDU = TRUE*/
   amHdr->p = rlcAmmDlCheckAndSetPoll(gCb,rbCb, TRUE, pduSz);

   /* Construct header with the available hdr Info, set isSegment to FALSE */
   rlcConstructAmHdr(amHdr, hdr, amDl->snLen, &idx);

   /* Concatenate hdr and data */
   SAddPreMsgMultInOrder(hdr, idx+1, pdu);
   
   txBuf = rlcUtlGetTxBuf(amDl->txBufLst, amHdr->sn);
   rlcCpyMsg(gCb,pdu,&(pduInfo->pdu));
   pduInfo->pduSz = pduSz;
   pduInfo->hdrSz = idx+1;

   /*Update estHdrSz. deduct current hdrSz */
    amDl->estHdrSz -= pduInfo->hdrSz;
    /* Reestimate estHdrSz for mid and last seg */
    if(amHdr->si & 0x1)
    {
       amDl->estHdrSz += ((amHdr->si == RLC_SI_MID_SEG)? pduInfo->hdrSz : (pduInfo->hdrSz + 2));
    }

   cmLListAdd2Tail(&txBuf->pduLst, &pduInfo->lstEnt);
   pduInfo->lstEnt.node = (PTR)pduInfo;

   gCb->genSts.bytesSent += pduSz;
   
   RETVOID;
}

/**
 * @brief Private handler to remove the retx PDU from the rbCb
 *
 * @details
 *    This function releases a retx PDU stored on DL portion of rbCb.
 *    It also updates the BO if wtForAck flag is not set which implies
 *    that it is not sent out yet.
 *
 * @param[in]  gCb        RLC instance control block
 * @param[in]  retx       retransmit PDU to be removed
 * @param[in]  rbCb       Radio Bearer Control Block
 *
 * @return Void 
 *
 */
#ifdef ANSI
PRIVATE Void rlcRemRetxPdu
(  
RlcCb          *gCb,
RlcDlRbCb      *rbCb,
RlcRetx        *retx
)
#else
PRIVATE Void rlcRemRetxPdu(gCb, rbCb, retx)
RlcCb          *gCb;
RlcDlRbCb      *rbCb;
RlcRetx        *retx;
#endif
{
   TRC2(rlcRemRetxPdu)

   cmLListDelFrm(&AMDL.retxLst, &retx->lstEnt); 

   if( AMDL.retxLst.count == 0)
   {
      AMDL.nxtRetx = NULLP;
   }

   if(retx->pendingReTrans == TRUE)
   {
      AMDL.retxBo -= retx->segSz;
      AMDL.estHdrSz -= retx->hdrSz;
   }

   rlcUtlAddReTxPduToBeFreedQueue(gCb, retx);
   rlcUtlRaiseDlCleanupEvent(gCb);

   RETVOID;
}

/**
 * @brief Private handler to mark a retx PDU for further retransmission
 *
 * @details
 *    This function sets a retx PDU that has not been ACKed in the   
 *    received Status PDU for futher retransmission. If the retransmission
 *    limit is reached, it releases the retx PDU and informs the higher
 *    layers about the same.
 *
 * @param[in]  gCb        RLC instance control block 
 * @param[in]  retx       retransmit PDU to be removed
 * @param[in]  rbCb       Radio Bearer Control Block
 *
 * @return Void 
 *
 */
#ifdef ANSI
PRIVATE Void rlcAmmDlMarkPduForReTx
(
RlcCb          *gCb,
RlcDlRbCb      *rbCb,
RlcRetx        *retx
)
#else
PRIVATE Void rlcAmmDlMarkPduForReTx(*gCb, rbCb, retx)
RlcCb          *gCb;
RlcDlRbCb      *rbCb;
RlcRetx        *retx;
#endif
{
   TRC2(rlcAmmDlMarkPduForReTx)
   if (AMDL.maxReTxReached == TRUE)
   {
      RETVOID;
   }
  
   if(retx->pendingReTrans == FALSE)
   {
      retx->pendingReTrans = TRUE;
      ++retx->retxCnt;

      AMDL.retxBo   += retx->segSz;
      AMDL.estHdrSz += retx->hdrSz;

      if (retx->retxCnt > AMDL.maxRetx)
      {
         /* RLC_DL_MAX_RETX fix */
         /* Marking the RB stalled for DL scheduling. This is to avoid unnecessary */
         /* preparation of RLC PDUs and adding the same to Tx Buffer */
         /* This condition is to avoid sending StaIndication more than once */
         if (TRUE != rbCb->m.amDl.maxReTxReached)
         {
            rbCb->m.amDl.maxReTxReached = TRUE;
            rbCb->m.amDl.bo = 0;
            rbCb->m.amDl.cntrlBo = 0;
            rbCb->m.amDl.retxBo = 0;
            /* Sending BO update to SCH */
            rlcUtlSndDStaRsp(gCb, rbCb, 0,0,0,0);
            rlcAmmSndStaInd(gCb, rbCb, retx);
            gRlcStats.amRlcStats.numDLMaxRetx++;
         }

         rlcRemRetxPdu(gCb,rbCb, retx);
         
         RETVOID;
      }


      if (AMDL.nxtRetx == NULLP)
      {
         AMDL.nxtRetx = retx;
      }

      gRlcStats.amRlcStats.numDLRetransPdus++;
   }


   RETVOID;
}

/**
 * @brief Private handler to check if SDU is completely deliverd and
 *        send higher layers data confirmation
 *
 * @details 
 *    This function sends higher layers data confirmation for SDUs which
 *    have been successfully delivered to the peer RLC entity.
 *
 * @param[in]  gCb      RLC instance control block
 * @param[in]  rbCb     Radio Bearer Control Block
 * @param[in]  sduLst   List of SDUs that were part of the PDU
 * @param[in]  numSdu   Number of SDUs in the list
 *
 * @return Void 
 *
 */
#ifdef ANSI
PRIVATE Void rlcAmmDlCheckIsSDUDelivered
(
RlcCb            *gCb,
RlcDlRbCb        *rbCb,
RlcSduMap        *sduMap,
KwuDatCfmInfo   **datCfm
)
#else
PRIVATE Void rlcAmmDlCheckIsSDUDelivered(gCb, rbCb, sduMap, datCfm)
RlcCb            *gCb;
RlcDlRbCb        *rbCb;
RlcSduMap        *sduMap;
KwuDatCfmInfo   **datCfm;
#endif
{
   RlcSdu *sdu;
   
   TRC2(rlcAmmDlCheckIsSDUDelivered)
   
   sdu = sduMap->sdu;

   sdu->mode.am.rcvdSz += sduMap->sduSz;

   /* send a dat cfm if all the bytes of the sdu have been received */
   if (sdu->mode.am.rcvdSz == sdu->actSz)
   {
      /* Send DatCfm for this sdu */
      if((*datCfm)->numSduIds < KWU_MAX_DAT_CFM)
      {
         (*datCfm)->sduIds[(*datCfm)->numSduIds++] = sdu->mode.am.sduId;
      }
      else
      {
         /* This is an error that should never happen, we should resize
          * the #define to a larger value or check why we need to 
          * send so many confirms in one go
          * Confrims to PDCP are being dropped in this case
          */
         RlcKwuSapCb    *rlckwuSap;
         rlckwuSap = gCb->u.dlCb->rlcKwuDlSap + RLC_UI_PDCP;
         RlcUiKwuDatCfm(&rlckwuSap->pst, rlckwuSap->suId, *datCfm);

         RLC_SHRABL_STATIC_BUF_ALLOC(rlckwuSap->pst.region, rlckwuSap->pst.pool, *datCfm, sizeof(KwuDatCfmInfo));

#if (ERRCLASS & ERRCLS_ADD_RES)
         if (*datCfm == NULLP)
         {
            RLOG_ARG2(L_FATAL,DBG_RBID,rbCb->rlcId.rbId, 
                  "Memory allocation failed UEID:%d CELLID:%d",
                  rbCb->rlcId.ueId,
                  rbCb->rlcId.cellId);
            RETVOID;
         }
#endif /* ERRCLASS & ERRCLS_RES */

         (*datCfm)->numSduIds = 0;
         (*datCfm)->rlcId = rbCb->rlcId;
         /* ccpu00135618: say total 1026 sduIds to copy the 1025 sduId after
          * new allocation of datCfm */
         (*datCfm)->sduIds[(*datCfm)->numSduIds++] = sdu->mode.am.sduId;
      }

      /* Remove SDU from the sduQ */
      cmLListDelFrm(&AMDL.sduQ, &sdu->lstEnt);
      rlcUtlAddSduToBeFreedQueue(gCb, sdu);
      rlcUtlRaiseDlCleanupEvent(gCb);
   }

   RETVOID;
} 

/**
 * @brief Private handler to mark a PDU successful.
 *
 * @details
 *    This function is called when we receive a STATUS pdu that marks
 *    a PDU as successful. It releases the PDU from RLC entity and 
 *    informs PDCP of successful SDUs delivered as a result of this PDU.
 *
 * @param[in]  gCb        RLC instance control block  
 * @param[in]  rbCb       Radio Bearer Control Block
 * @param[in]  sn         SN that is successfully delivered to the peer 
 *
 * @return Void 
 *
 */
#ifdef ANSI
PRIVATE Void rlcAmmDlProcessSuccessfulTxPdu
(
RlcCb            *gCb,
RlcDlRbCb        *rbCb,
RlcSn            sn,
KwuDatCfmInfo   **datCfm
)
#else
PRIVATE Void rlcAmmDlProcessSuccessfulTxPdu(gCb, rbCb, sn, datCfm)
RlcCb            *gCb;
RlcDlRbCb        *rbCb;
RlcSn            sn;
KwuDatCfmInfo   **datCfm;
#endif
{
   TRC2(rlcAmmDlProcessSuccessfulTxPdu)
   CmLList *pduNode;
  
   RlcTx *txBuf = rlcUtlGetTxBuf(AMDL.txBufLst, sn);
   
   if (txBuf == NULLP)
   {
	   RETVOID;
   }
   pduNode = txBuf->pduLst.first;
   while(pduNode)
   {
      RlcDlPduInfo *pduInfo = (RlcDlPduInfo *)(pduNode->node);
      rlcAmmDlCheckIsSDUDelivered(gCb,
                              rbCb,  
                              &(pduInfo->sduMap), 
                              datCfm);
      pduNode = pduNode->next;
   }
   
   rlcUtlAddTxPduToBeFreedQueue(gCb, txBuf);
   rlcUtlRaiseDlCleanupEvent(gCb);
   /* so that it is not processed again */
   rlcUtlRemovTxBuf(AMDL.txBufLst, txBuf, gCb);

   RETVOID;
}

/**
 * @brief  Handler to send Status Indication to PDCP
 *
 * @details
 *    This function is used to send status indication to PDCP when the 
 *    maximum retransmission threshold value is reached for a PDU.
 * 
 * @param[in] gCb    RLC instance control block
 * @param[in] rbCb   RB control block 
 * @param[in] retx   The PDU/segment that failed max re-transmissions
 *
 * @return  Void
 *
 */
#ifdef ANSI
PRIVATE Void rlcAmmSndStaInd
(
RlcCb       *gCb,
RlcDlRbCb   *rbCb,
RlcRetx     *retx
)
#else
PRIVATE Void rlcAmmSndStaInd(gCb, rbCb, retx)
RlcCb       *gCb;
RlcDlRbCb   *rbCb;
RlcRetx     *retx;
#endif
{
   KwuStaIndInfo   *staInd;
   RlcKwuSapCb      *rlckwuSap;

   TRC2(rlcAmmSndStaInd);


   /* Sap control block */
   rlckwuSap = gCb->u.dlCb->rlcKwuDlSap + RLC_UI_PDCP;

   /* Allocate memory for staInd Info */
   RLC_SHRABL_STATIC_BUF_ALLOC(rlckwuSap->pst.region, rlckwuSap->pst.pool, staInd, sizeof(KwuStaIndInfo));

#if (ERRCLASS & ERRCLS_ADD_RES)
   if (staInd == NULLP)
   {
      RLOG_ARG2(L_FATAL,DBG_RBID,rbCb->rlcId.rbId, 
            "Memory allocation failed UEID:%d CELLID:%d",
            rbCb->rlcId.ueId,
            rbCb->rlcId.cellId);
      RETVOID;
   }
#endif /* ERRCLASS & ERRCLS_RES */

   /* Fill staInd Info */ 
   RLC_MEM_CPY(&staInd->rlcId, &rbCb->rlcId, sizeof(CmLteRlcId));    
   
   staInd->numSdu = 1;
   staInd->sduId[0] = retx->sduMap.sdu->mode.am.sduId;

#ifdef KW_PDCP
#else
   RlcUiKwuStaInd(&rlckwuSap->pst, rlckwuSap->suId, staInd);
#endif /* KW_PDCP */

   RETVOID;
}

/**
 * @brief  Handler to get the next node to be retransmitted from retxLst
 *
 * @details
 *    This function is used to get the next node to be retransmitted 
 *    from the retxLst
 *            
 *  @param[in] gCb     RLC instance control block 
 *  @param[in] retx    The PDU/segment after which to find a node to be 
 *                     retransmitted
 *
 *  @return  Void 
 *
 */
#ifdef ANSI
PRIVATE Void rlcGetNxtRetx 
(
RlcCb     *gCb,
RlcRetx   **retx
)
#else
PRIVATE Void rlcGetNxtRetx(gCb, retx)
RlcCb     *gCb;
RlcRetx   **retx;
#endif
{
   CmLList *tNode;

   TRC2(rlcGetNxtRetx);

   do
   {
      tNode = &((*retx)->lstEnt);
      tNode = tNode->next;
      
      if (tNode)
      {
         *retx = (RlcRetx *)tNode->node;
      }
      else
      {
         *retx = NULLP;
         RETVOID;
      }
   }while((*retx)->pendingReTrans == FALSE);

   RETVOID;
}

/**
 * @brief  Handler to process the re-establishment request received from UIM
 *
 * @param[in]  gCb         RLC instance control block
 * @param[in]  rlcId       Identity of the RB in the UE/Cell for which 
 *                         re-establishment is to be done
 * @param[in]  rbCb        Downlink RB control block (rbCb is freed in this
 *                         function)
 *
 * @return  Void 
 *
 */
#ifdef ANSI
Void rlcAmmDlReEstablish
(
RlcCb         *gCb,
CmLteRlcId   rlcId,
RlcDlRbCb     *rbCb
)
#else
Void rlcAmmDlReEstablish(gCb, rlcId, rbCb)
RlcCb         *gCb;
CmLteRlcId   rlcId;
RlcDlRbCb     *rbCb;
#endif
{
   /* create a new AM DL RB, reset it and replace in the UeCb*/
   RlcDlUeCb   *ueCb;
   RlcDlRbCb   *resetRb;
   RlcAmDl* newAmDl;
   RlcAmDl* oldAmDl;
   RLC_ALLOC(gCb, resetRb, sizeof(RlcDlRbCb));
   
   /* ccpu00135170 Removing KLOCK warning */
   if(resetRb == NULLP)
   {
      RETVOID;
   }

   RLC_MEM_CPY(resetRb, rbCb, sizeof(RlcDlRbCb));
   RLC_MEM_SET(&resetRb->m.amDl, 0 , sizeof(RlcAmDl));

/* AGHOSH changes start */
   /* restore the old AM values */
   newAmDl = &resetRb->m.amDl;
   oldAmDl = &rbCb->m.amDl;

   newAmDl->pollPdu = oldAmDl->pollPdu;
   newAmDl->pollByte = oldAmDl->pollByte;
   newAmDl->maxRetx = oldAmDl->maxRetx;
   newAmDl->snLen   = oldAmDl->snLen;
   newAmDl->snModMask   = oldAmDl->snModMask;
   newAmDl->pollRetxTmrInt = oldAmDl->pollRetxTmrInt;
   rbCb->boUnRprtdCnt = (U32)0;
   rbCb->lastRprtdBoToMac = (U32)0;
   cmInitTimers(&(resetRb->m.amDl.pollRetxTmr), 1); 
/* AGHOSH changes end */
 
   if (ROK != rlcDbmFetchDlUeCb(gCb,rlcId.ueId, rlcId.cellId, &ueCb))
   {
      RLOG_ARG2(L_ERROR,DBG_CELLID, rlcId.cellId,
               "UeId [%d]: UeCb not found RBID;%d",
               rlcId.ueId,
               rlcId.rbId);
      RETVOID;
   }
   
   if(rlcId.rbType == CM_LTE_SRB)
   {
      ueCb->srbCb[rlcId.rbId] = resetRb;
   }
   else
   {
      ueCb->drbCb[rlcId.rbId] = resetRb;
   }
   /* update into the logical channel array also */
   ueCb->lCh[rbCb->lch.lChId - 1].dlRbCb = resetRb;
 
   if((resetRb->rlcId.rbType == CM_LTE_SRB)
                &&(resetRb->rlcId.rbId == 1))
   {
      /* To stop the traffic on SRB2 and other DRBs*/
      rlcDlUtlSetReestInProgressForAllRBs(gCb, ueCb);
   }
   else 
   {
      rlcDlUtlSetReestInProgressForRB(gCb, resetRb);      
   }

   /* allocate the TX array again */
#ifndef  LTE_TDD
   U32 hashIndex;
   RLC_ALLOC(gCb,
		   resetRb->m.amDl.txBufLst,
		   (RLC_TX_BUF_BIN_SIZE * sizeof(CmLListCp)));
   for(hashIndex = 0; hashIndex < RLC_TX_BUF_BIN_SIZE; hashIndex++)
   {
	   cmLListInit(&(resetRb->m.amDl.txBufLst[hashIndex]));
   }
#endif
   /* send the old rb of deletion */
   rlcAmmFreeDlRbCb(gCb,rbCb); 


   /* TODO: for now we are re-settting the re-establishment flag here
      this needs to be fixed
      There should be a proper intreface to resume the RBs */
   if(rlcId.rbType == CM_LTE_SRB)
   {
      rlcDlUtlResetReestInProgress(ueCb->srbCb[rlcId.rbId]);
   }
   else
   {
      rlcDlUtlResetReestInProgress(ueCb->drbCb[rlcId.rbId]);
   }      
      
   RETVOID;
}

/**
 * @brief  Handler to discard a SDU.
 *
 * @details
 *    This function is used to discard a SDU after receiving
 *    the Discard Request from UIM. The SDU is discarded if its 
 *    available and is not mapped to any PDU yet.
 *   
 * @param[in] gCb     RLC instance control block
 * @param[in] rbCb    RB control block 
 * @param[in] sduId   Sdu ID of the SDU to be discarded
 *
 *  @return  S16
 *     -# ROK      In case of successful discard
 *     -# RFAILED  In case the SDU is not found or already mapped
 */
#ifdef ANSI
S16 rlcAmmDiscSdu
(
RlcCb       *gCb,
RlcDlRbCb   *rbCb,
U32        sduId 
)
#else
S16 rlcAmmDiscSdu(gCb, rbCb, sduId)
RlcCb       *gCb;
RlcDlRbCb   *rbCb;  
U32        sduId; 
#endif
{
   TRC2(rlcAmmDiscSdu);
   return (RFAILED);
} 

/**
 * @brief  Handler for Poll retransmit timer expiry
 *
 * @details
 *    This function is used to handle events upon expiry of Poll 
 *    retransmit timer.
 *  
 * @param[in] gCb    RLC instance control block
 * @param[in] rbCb   Downlink RB control block 
 *
 * @return  Void 
 */
#ifdef ANSI
Void rlcAmmPollRetxTmrExp
(
RlcCb       *gCb,
RlcDlRbCb   *rbCb
)
#else
Void rlcAmmPollRetxTmrExp(gCb, rbCb)
RlcCb       *gCb;
RlcDlRbCb   *rbCb;
#endif
{
   RlcRetx        *retx; 
   RlcAmDl        *amDl = &(rbCb->m.amDl);
   RlcSn          sn;
   RlcTx          *txBuf;
   TRC2(rlcAmmPollRetxTmrExp);

   
   /* kw003.201 - Correcting the logic for determmining whether to do   */
   /*             any transmission of PDU. As per the spec section      */
   /*             5.2.2.3, if there is any to transmit or retransmit,   */
   /*             do nothing. Else, pick up the VT(S) -1 for retx       */
   /*             We have nothing to transmit if window is stalled or   */
   /*             there are no SDUs to be transmitted or if there are   */
   /*             PDUs to be retransmitted.                             */
   if(CM_LTE_SRB == rbCb->rlcId.rbType)
   {
      gRlcStats.amRlcStats.numDLPollTimerExpiresSrb++;
   }
   else
   {
      gRlcStats.amRlcStats.numDLPollTimerExpiresDrb++;
   }

   if (((amDl->nxtTx == NULLP) && (amDl->nxtRetx == NULLP)) || 
        RLC_AM_IS_TRANS_WIN_STALLED(amDl)) 
   {
      sn = (amDl->txNext - 1) & amDl->snModMask;
      txBuf = rlcUtlGetTxBuf(amDl->txBufLst, sn);

      if (txBuf != NULLP)
      {
         rlcAmmDlMoveFrmTxtoRetxBuffer(gCb,amDl, &retx, sn); 
         
         if (AMDL.nxtRetx == NULLP)
         {
            AMDL.nxtRetx = retx;
         }
         
         rlcAmmSendDStaRsp(gCb, rbCb, &AMDL);         
         RETVOID;
      }
      /* Get the last node in retxLst */
      RLC_LLIST_LAST_RETX(amDl->retxLst, retx);

      /* Unset wtForAck flag for the NACK PDUs */ 
      if (retx != NULLP)
      {
         rlcAmmDlMarkPduForReTx(gCb, rbCb, retx);
         rlcAmmSendDStaRsp(gCb, rbCb, &AMDL);         
      }
   }

   RETVOID;
} 

/**
 * @brief  Handler to update Acks for the remaining PDUs after the last accessed
 *         NACK PDU.
 *
 * @details 
 *    This function is used to handle ACKs for the PDUs remaining after the 
 *    last accessed NACK PDU, It updates the txBuf/retxBuf for the ACKs and 
 *    sends DatCfm to PDCP for the same.
 *
 * @param[in]  gCb         RLC instance control block
 * @param[in]  rbCb        Downlink Radio Bearer control block
 * @param[in]  mAckSn      The ACK SN after doing the base modulus
 * @param[in]  rextNode    Next node in the re-transmission buffer          
 *
 * @return  Void
 *
 */

#ifdef ANSI
PRIVATE Void rlcAmmDlUpdateTxAndReTxBufForAckSn
(
RlcCb            *gCb,
RlcDlRbCb        *rbCb,
RlcSn            mAckSn,
CmLList         *retxNode,
KwuDatCfmInfo   **datCfm
)
#else
PRIVATE Void rlcAmmDlUpdateTxAndReTxBufForAckSn(gCb, rbCb, mAckSn, retxNode, datCfm)
RlcCb            *gCb;
RlcDlRbCb        *rbCb;
RlcSn            mAckSn;
CmLList         *retxNode;
KwuDatCfmInfo   **datCfm;
#endif
{
   RlcSn    mSn;
   RlcSn    sn;
   RlcRetx  *retx;
   RlcTx    *txBuf;

   TRC2(rlcAmmDlUpdateTxAndReTxBufForAckSn);

   /* Remove pdus/segs from retxLst */ 
   while (retxNode)
   {
      retx = (RlcRetx *)(retxNode->node);
      retxNode = retxNode->next;
      MODAMT(retx->amHdr.sn, mSn, AMDL.txNextAck,AMDL.snModMask);
      if (mSn < mAckSn) 
      {
         rlcAmmDlProcessSuccessfulReTx(gCb,rbCb, retx, datCfm);
      }
   }

   /* For the remaining; pdus not acknowldeged by the NACK_SN but being
      acknowledged by the ACK_SN*/
   /* start from the starting of the transmission window and remove till just
      before ACK_SN*/
   mSn = 0;       /* same as MODAMT(AMDL.txNextAck, mSn, AMDL.txNextAck);*/
   sn = AMDL.txNextAck;
   while(mSn < mAckSn)
   {
      txBuf = rlcUtlGetTxBuf(AMDL.txBufLst, sn);
      if (txBuf != NULLP)
      {
         RLOG_ARG3(L_UNUSED,DBG_RBID,rbCb->rlcId.rbId, 
                 "rlcAmmDlUpdateTxAndReTxBufForAckSn: ACK for PDU "
                 "with sn = %ld UEID:%ld CELLID:%ld",
                 sn,
                 rbCb->rlcId.ueId,
                 rbCb->rlcId.cellId);

         rlcAmmDlProcessSuccessfulTxPdu(gCb,rbCb, sn, datCfm);
      }
      
      sn = (sn + 1) & AMDL.snModMask;
      MODAMT(sn, mSn, AMDL.txNextAck,AMDL.snModMask);
   }

   RETVOID;
}

/**
 * @brief  Handler to update Acks for the remaining PDUs after the last accessed
 *         NACK PDU.
 *
 * @details 
 *    This function is used to handle ACKs for the PDUs remaining after the 
 *    last accessed NACK PDU, It updates the txBuf/retxBuf for the ACKs and 
 *    sends DatCfm to PDCP for the same.
 *
 * @param[in]  gCb         RLC instance control block
 * @param[in]  rbCb        Downlink Radio Bearer control block
 * @param[in]  mAckSn      The ACK SN after doing the base modulus
 * @param[in]  rextNode    Next node in the re-transmission buffer          
 *
 * @return  Void
 *
 */
#ifdef ANSI
PRIVATE Void rlcAmmDlUpdTxAndReTxBufForLessThanNackSn
(
RlcCb            *gCb,
RlcDlRbCb        *rbCb,
RlcSn            sn,
RlcSn            mNackSn,
CmLList         **retxNode,
KwuDatCfmInfo   **datCfm
)
#else
PRIVATE Void rlcAmmDlUpdTxAndReTxBufForLessThanNackSn(gCb, rbCb, sn, mNackSn, retxNode, datCfm)
RlcCb            *gCb;
RlcDlRbCb        *rbCb;
RlcSn            sn;
RlcSn            mNackSn;
CmLList         **retxNode;
KwuDatCfmInfo   **datCfm;
#endif
{
   RlcSn    mSn;
   RlcRetx  *retx;
   RlcTx    *txBuf=NULLP;

   TRC2(rlcAmmDlUpdTxAndReTxBufForLessThanNackSn);

   while (*retxNode)
   {
      retx = (RlcRetx *)((*retxNode)->node);
      MODAMT(retx->amHdr.sn, mSn, AMDL.txNextAck,AMDL.snModMask);
      if (mSn < mNackSn)
      {
         (*retxNode) = (*retxNode)->next;
         rlcAmmDlProcessSuccessfulReTx(gCb,rbCb, retx, datCfm);
      }
      else
      {
         break;
      }
   }

   /* Remove all pdus with SN < NACK_SN from the transmission buffer */ 
   MODAMT(sn, mSn, AMDL.txNextAck,AMDL.snModMask);
   while (mSn < mNackSn)
   {
      /* this if check seems redundant,why should mSn ever be mTxSn 
         (which actually is VT(A) */
      txBuf = rlcUtlGetTxBuf(AMDL.txBufLst, sn);
      if ((txBuf != NULLP)) 
      {
         RLOG_ARG3(L_DEBUG,DBG_RBID, rbCb->rlcId.rbId, 
               "rlcHndlStaRsp: Handle ACK (sn = %d) UEID:%d CELLID:%d",
               sn,
               rbCb->rlcId.ueId,
               rbCb->rlcId.cellId);

         /* Remove pdus from txBuf */
         rlcAmmDlProcessSuccessfulTxPdu(gCb,rbCb, sn, datCfm);
      }

      sn = (sn + 1) & AMDL.snModMask;
      MODAMT(sn, mSn, AMDL.txNextAck,AMDL.snModMask);
   }
   
   RETVOID;
}


/**
 * @brief  Handler to form construct AM header
 *
 * @details 
 *    This function is used to construct am header with the available header
 *    elements.
 *
 * @param[in] gCb    RLC instance control block
 * @param[in] amHdr  AM Header
 * @param[in] isSeg  Check for Segmentation of PDU
 * @param[in] hdr    Header field
 * @param[in] idx    Index
 *  
 * @return Void            
 *
 */
#ifdef ANSI
PRIVATE Void rlcConstructAmHdr
(
RlcAmHdr   *amHdr,
U8        *hdr,
U8         snLen,
U16       *idx
)
#else
PRIVATE Void rlcConstructAmHdr(amHdr, hdr, snLen, idx)
RlcAmHdr   *amHdr;
U8        *hdr;
U8         snLen;
U16       *idx;
#endif
{
   TRC2(rlcConstructAmHdr);
                                                            
   *idx = 0;
    hdr[0] = RLC_DATA_BITMASK;
    
    hdr[0] = hdr[0] | (amHdr->p << 6);
    hdr[0] = hdr[0] | ((amHdr->si & 0x3) << 4);
   if(snLen == RLC_AM_CFG_12BIT_SN_LEN)
   {
      hdr[0] = hdr[0] | (U8)((amHdr->sn & 0xF00) >> 8);
      hdr[1] =  (U8)(amHdr->sn & 0x0FF);
      (*idx)++;
   }
   else
   {
      hdr[0] = hdr[0] | (U8)((amHdr->sn & 0x30000) >> 16);
      hdr[1] =  (U8)((amHdr->sn & 0xFF00) >> 8);
      (*idx)++;
      hdr[2] =  (U8)(amHdr->sn & 0xFF);
      (*idx)++;
   }
    
   if ((amHdr->si == RLC_SI_MID_SEG) || (amHdr->si == RLC_SI_LAST_SEG))                                      
   {
      (*idx)++;
      hdr[(*idx)] = (U8)((amHdr->so & 0xFF00)>> 8);
      (*idx)++;
      hdr[(*idx)] = (U8)(amHdr->so & 0xFF);
   }                                                        

   RETVOID;
}

/**
 * @brief  This function adds a retx PDU to list of retx PDUs
 *
 * @details
 *    kw003.201 - Poll expiry may cause an SN to be added to retx 
 *                out of sequence and hence all additions to retx 
 *                must validate that they are added in sequence   
 *
 * @param[in] amDl   AM Downlink Control Block
 * @param[in] retx   Retransmit PDU
 * 
 * @return Void
 *            
 */
#ifdef ANSI
PRIVATE Void rlcAmmAddPduToRetxLst
(
RlcAmDl   *amDl,
RlcRetx   *retx
)
#else
PRIVATE Void rlcAmmAddPduToRetxLst(amDl, retx)
RlcAmDl   *amDl;
RlcRetx   *retx;
#endif
{
   CmLList   *node;
   RlcRetx    *tRetx;
   RlcSn      tMSn;
   RlcSn      retxMSn;
   
   TRC2(rlcAmmAddPduToRetxLst);
 
   node = amDl->retxLst.last;
   MODAMT(retx->amHdr.sn, retxMSn, amDl->txNextAck,amDl->snModMask);
   while(node != NULLP)
   {
      tRetx = (RlcRetx *)(node->node);
      MODAMT(tRetx->amHdr.sn, tMSn, amDl->txNextAck,amDl->snModMask);
      if (tMSn > retxMSn)
      {
         node = node->prev;
      }
      else
      {
         break;
      }
   }
   if (node)
   {
      amDl->retxLst.crnt = node;
      cmLListInsAfterCrnt(&amDl->retxLst, &retx->lstEnt);
      retx->lstEnt.node = (PTR)retx;
   }
   else
   {
      amDl->retxLst.crnt = amDl->retxLst.first;
      cmLListInsCrnt(&amDl->retxLst, &retx->lstEnt);
      retx->lstEnt.node = (PTR)retx;
   }

   if (amDl->nxtRetx == NULLP)
   {
      amDl->nxtRetx = retx;
   }

   RETVOID;
}

/**
 * @brief Handler to Move the PDU from txBuf to re-transmission buffer 
 *
 * @details
 *    This function is used to move the PDU from the txBuf to re-transmit buffer
 *
 * @param[in]  gCb         RLC instance control block
 * @param[in]  amDl        AM Downlink Control Block 
 * @param[in]  retx        node in the reTx buffer to be moved to, allocated by
 *                         this function
 * @param[in]  sn          SN in the tx buffer which needs to be moved
 * 
 * @return Void
 *            
 */

#ifdef ANSI
PRIVATE Void rlcAmmDlMoveFrmTxtoRetxBuffer
(
RlcCb          *gCb,
RlcAmDl        *amDl,
RlcRetx        **retx,
RlcSn          sn
)
#else
PRIVATE Void rlcAmmDlMoveFrmTxtoRetxBuffer(gCb, amDl, retx, sn)
RlcCb          *gCb;
RlcAmDl        *amDl;
RlcRetx        **retx;
RlcSn          sn; 
#endif
{
   RlcTx* txBuf = rlcUtlGetTxBuf(amDl->txBufLst, sn);
   TRC2(rlcAmmDlMoveFrmTxtoRetxBuffer);

   if (txBuf == NULLP)
   {
	   RETVOID;
   }
   while(txBuf->pduLst.first)
   {
      RlcDlPduInfo *pduInfo = (RlcDlPduInfo *)(txBuf->pduLst.first->node);
      RLC_ALLOC_WC(gCb,*retx, sizeof(RlcRetx));

#if (ERRCLASS & ERRCLS_ADD_RES)
      if (*retx == NULLP)
      {
	      RLOG0(L_FATAL, "Memory allocation failed");
	      RETVOID;
      }
#endif /* ERRCLASS & ERRCLS_RES */

      /* Move Sdu byte segment from TX buf to retx buf*/
      rlcAmmDlMoveSduByteSegFrmTxtoRetxBuffer(gCb, 
            amDl, 
            retx, 
            pduInfo);
      
      /* Delete node from the txBuf Pdu lst */
      cmLListDelFrm(&txBuf->pduLst, txBuf->pduLst.first);
      RLC_FREE_WC(gCb, pduInfo, sizeof(RlcDlPduInfo));
   }
   /* Remove PDU from txBuf */
   rlcUtlDelTxBuf(amDl->txBufLst, txBuf,gCb); 
   
   RETVOID;
}



/*
 * @brief
 *    function to free/release the Acknowledged mode RBCB buffers
 *
 * @details
 *    This primitive Frees the Acknowledged Mode RbCb transmission Buffer,
 *    retransmission Buffer and reciption Buffers
 *
 * @param [in]   gCb    - RLC instance control block
 * @param [in]   rbCb   - Downlink RB Control Block
 *
 * @return Void
 */
#ifdef ANSI
Void rlcAmmFreeDlRbCb
(
RlcCb       *gCb,
RlcDlRbCb   *rbCb
)
#else
Void rlcAmmFreeDlRbCb(gCb,rbCb)
RlcCb       *gCb;
RlcDlRbCb   *rbCb;
#endif
{
   /* stop the re-transmission timer */
   if(TRUE == rlcChkTmr(gCb,(PTR)rbCb,RLC_EVT_AMDL_POLL_RETX_TMR))
   {
      rlcStopTmr(gCb,(PTR)rbCb, RLC_EVT_AMDL_POLL_RETX_TMR);
   }

   /* store the entire Rb pointer */      
   rbCb->rlsLnk.node = (PTR)rbCb;
   cmLListAdd2Tail(&gCb->u.dlCb->toBeFreed.rbLst, &rbCb->rlsLnk);

   /* the sdu queue */
   cmLListCatLList(&(gCb->u.dlCb->toBeFreed.sduLst),&(rbCb->m.amDl.sduQ));

   rlcUtlRaiseDlCleanupEvent(gCb);
   
   RETVOID;
}

/**
 * @brief  Handler to create STATUS Pdu
 *
 * @details
 *    This function is used to create status pdu 
 *
 * @param[in] gCb        RLC instance control block
 * @param[in] rbCb       Downlink RB control block
 * @param[in] rlcDatReq   The data to be passed to MAC
 *
 * @return Void
 *
 */
#ifdef ANSI
PRIVATE Void rlcAmmCreateStatusPdu
(
RlcCb       *gCb,
RlcDlRbCb   *rbCb,
RlcDatReq   *rlcDatReq
)
#else
PRIVATE Void rlcAmmCreateStatusPdu(gCb, rbCb, rlcDatReq)
RlcCb       *gCb;
RlcDlRbCb   *rbCb;
RlcDatReq   *rlcDatReq;
#endif
{
    RlcSn          sn;                      /* sequence number */
    RlcSn          ack_sn;                  /* Ack sequence number */
    Buffer        *mBuf;           /* control pdu buffer */
    MsgLen        cntrlPduSz;          /* control pdu size */
    U8            cntrlPdu[RLC_MAX_CNTRL_FIELDS];   /* control pdu to be added to mBuf */
    RlcUdxDlStaPdu   *pStaPdu;
    U16             bytesToEncode = 0; /* bytes required to encode the STATUS PDU */
    U16             encIdx = 0;
    U16             prevEncIdx = 0;
    RlcNackInfo      *rlcNackInfo;
    U16           nkCnt = 0;

    TRC2(rlcAmmCreateStatusPdu)


    pStaPdu = AMDL.pStaPdu;


    /* D/C| CPT| */
    /* 0 - Control
       1 - Data */
    cntrlPdu[0] = 0x00;
    cntrlPdu[2] = 0x00;

    /* ACK SN Field will be set in the end based on available Grant */

    encIdx = bytesToEncode = 3; /* Num Octets before NACK SN info encoding*/

    ack_sn = pStaPdu->ackSn;

    if (rbCb->m.amDl.snLen == RLC_AM_CFG_12BIT_SN_LEN)
    {

       /* If alteast one NACK SN Info then set the E1 field */
       if (pStaPdu->nackCount)
       {
          /* 12 BIT SN CASE:
             In Third Octet:
             7  6 5 4 3 2 1 0
             E1 R R R R R R R 
           */
          cntrlPdu[2] = 0x80;
       }

       for(nkCnt = 0;nkCnt < pStaPdu->nackCount; nkCnt++)
       {           
          sn = pStaPdu->nackInfo[nkCnt].sn;

          rlcNackInfo = &(pStaPdu->nackInfo[nkCnt]);

          bytesToEncode += 2; /* 2 Octets for NACK SN */

          /* Check if E2 : isSegment is set */
          if (rlcNackInfo->isSegment)
          {
             bytesToEncode += 4; /* 4 Octets: SOstart, SOend */ 
          }

          /* Check if E3 : nackRange is set */
          if (rlcNackInfo->nackRange)
          {
             bytesToEncode += 1; /* 1 Octet: NACK range */
          }

          /* Check if this NACK info can be accomodated in the Grant */
          if( rlcDatReq->pduSz >= bytesToEncode)
          {
             /* If there is a NACK SN before this then set its 
                E1 bit */
             if(prevEncIdx)
             {
                /* NACKSN  E1 E2 E3 R */
                cntrlPdu[prevEncIdx + 1] |= 0x8;
             }

             /* 12 BIT Nack SN encode */
             cntrlPdu[encIdx] = (sn & 0xFF0) >> 4;

             /* Next Octet */
             cntrlPdu[encIdx + 1] = (sn & 0xF) << 4;

             if (rlcNackInfo->isSegment)
             {
                /*Set E2 Bit */
                cntrlPdu[encIdx + 1] |= 0x4;


                /* Add soStart and soEnd */
                /* SOstart */
                cntrlPdu[encIdx + 2] = (rlcNackInfo->soStart) >> 8; 
                cntrlPdu[encIdx + 3] = rlcNackInfo->soStart & 0xFF;

                /* SOend */
                cntrlPdu[encIdx + 4] = (rlcNackInfo->soEnd) >> 8;
                cntrlPdu[encIdx + 5] = rlcNackInfo->soEnd & 0xFF;
             }

             if (rlcNackInfo->nackRange)
             {
                /*Set E3 Bit */
                cntrlPdu[encIdx + 1] |= 0x2;
                if(rlcNackInfo->isSegment)
                {
                   cntrlPdu[encIdx + 6] = rlcNackInfo->nackRange;
                }
                else
                {
                   cntrlPdu[encIdx + 2] = rlcNackInfo->nackRange;
                }
             }

             gRlcStats.amRlcStats.numDLNacksInStaPdu++;
          }
          /* Set ACK SN now */
          else
          {
             ack_sn = rlcNackInfo->sn;

             /* Not even one nack can be accomodated */
             if (nkCnt == 0)
             {
               cntrlPdu[2] = 0x0;
             }

             break;
          }

          prevEncIdx = encIdx;
          encIdx = bytesToEncode;

       }/* Loop is done for the NackCount */

        /* set ACK SN */
       {

          RLOG_ARG3(L_UNUSED,DBG_RBID,rbCb->rlcId.rbId, 
                "rlcAssembleCntrlInfo: ACK PDU's SN = %d"
                "UEID:%d CELLID:%d",
                ack_sn,
                rbCb->rlcId.ueId,
                rbCb->rlcId.cellId);

          cntrlPdu[0] |= (ack_sn & 0xF00)>> 8; 
          cntrlPdu[1] =  (U8)ack_sn;
       }

    }
    else if (rbCb->m.amDl.snLen == RLC_AM_CFG_18BIT_SN_LEN)
    {
       /* If alteast one NACK SN Info then set the E1 field */
       if (pStaPdu->nackCount)
       {
          /* 12 BIT SN CASE:
             In Third Octet:
             7  6 5 4 3 2 1 0
             ACKSN       E1 R 
           */
          cntrlPdu[2] = 0x2;
       }

       for(nkCnt = 0;nkCnt < pStaPdu->nackCount; nkCnt++)
       {           
          sn = pStaPdu->nackInfo[nkCnt].sn;

          rlcNackInfo = &(pStaPdu->nackInfo[nkCnt]);

          bytesToEncode += 3; /* 3 Octets for NACK SN */

          /* Check if E2 : isSegment is set */
          if (rlcNackInfo->isSegment)
          {
             bytesToEncode += 4; /* 4 Octets: SOstart, SOend */ 
          }

          /* Check if E3 : nackRange is set */
          if (rlcNackInfo->nackRange)
          {
             bytesToEncode += 1; /* 1 Octet: NACK range */
          }

          /* Check if this NACK info can be accomodated in the Grant */
          if( rlcDatReq->pduSz >= bytesToEncode)
          {
             /* If there is a NACK SN before this then set its 
                E1 bit */
             if(prevEncIdx)
             {
                /* NACKSN  E1 E2 E3 R R R */
                cntrlPdu[prevEncIdx + 2] |= 0x20;
             }

             /* 18 BIT Nack SN encode */
             cntrlPdu[encIdx] = (U8)((sn & 0x3FC00) >> 10);

             /* Next Octet */
             cntrlPdu[encIdx + 1] = (U8)((sn & 0x3FC) >> 2);

             /* Next Octet */
             cntrlPdu[encIdx + 2] = (U8)((sn & 0x3)<< 6);

             if (rlcNackInfo->isSegment)
             {
                /* NACKSN  E1 E2 E3 R R R */
                /*Set E2 Bit */
                cntrlPdu[encIdx + 2] |= 0x10;


                /* Add soStart and soEnd */
                /* SOstart */
                cntrlPdu[encIdx + 3] = (rlcNackInfo->soStart) >> 8;
                cntrlPdu[encIdx + 4] = (U8)rlcNackInfo->soStart;

                /* SOend */
                cntrlPdu[encIdx + 5] = (rlcNackInfo->soEnd) >> 8; 
                cntrlPdu[encIdx + 6] = (U8)(rlcNackInfo->soEnd);
             }

             if (rlcNackInfo->nackRange)
             {
                /* NACKSN  E1 E2 E3 R R R */
                /*Set E3 Bit */
                cntrlPdu[encIdx + 2] |= 0x08;

                if (rlcNackInfo->isSegment)
                {
                   cntrlPdu[encIdx + 7] = rlcNackInfo->nackRange;
                }
                else
                {
                   cntrlPdu[encIdx + 3] = rlcNackInfo->nackRange;
                }
             }

             gRlcStats.amRlcStats.numDLNacksInStaPdu++;
          }
          /* Set ACK SN now */
          else
          {
             ack_sn = rlcNackInfo->sn;

             /* Not even one nack can be accomodated */
             if (nkCnt == 0)
             {
               cntrlPdu[2] &= 0xFD;
             }

             break;
          }

          prevEncIdx = encIdx;
          encIdx = bytesToEncode;

       }/* Loop is done for the NackCount */
   
       /* set ACK SN */
       {

          RLOG_ARG3(L_UNUSED,DBG_RBID,rbCb->rlcId.rbId, 
                "rlcAssembleCntrlInfo: ACK PDU's SN = %d"
                "UEID:%d CELLID:%d",
                ack_sn,
                rbCb->rlcId.ueId,
                rbCb->rlcId.cellId);


          cntrlPdu[0] |=  (ack_sn & 0x3C000) >> 14;
          cntrlPdu[1] =  (ack_sn & 0x3FC0) >> 6;
          cntrlPdu[2] |= (ack_sn & 0x3F)<< 2;
       }

    }
    else
    {
       /* ERROR Log */
       RLOG_ARG3(L_ERROR,DBG_RBID,rbCb->rlcId.rbId, 
             "rlcAssembleCntrlInfo:Conf SN LEN  %d  is INVALID !!!! UEID:%d CELLID:%d",
             rbCb->m.amDl.snLen,
             rbCb->rlcId.ueId,
             rbCb->rlcId.cellId);
    }


#ifndef L2_OPTMZ
    SGetMsg(RLC_GET_MEM_REGION(gCb), RLC_GET_MEM_POOL(gCb),&mBuf);
#else
    mBuf = (Buffer *)rlcAmmStaPduList[rlcAmmStaPduListCnt++];
    SResetMBuf(mBuf);
    if(rlcAmmStaPduListCnt > 511)
       rlcAmmStaPduListCnt = 0;
#endif

    cntrlPduSz = encIdx;
    SAddPstMsgMult (cntrlPdu, cntrlPduSz, mBuf);

    rlcDatReq->pduSz -= cntrlPduSz;
    /* Add mBuf to AMDL.mBuf */
    AMDL.mBuf = mBuf;
 
    RETVOID;
}

#ifdef RLC_STA_PROC_IN_MAC/* RLC Status PDU Processing */

S16 rlcProcDlStatusPdu(Pst *udxPst,SuId suId,
      CmLteCellId cellId,CmLteRnti rnti,CmLteLcId lcId,Buffer *rlcSdu);

#ifdef ANSI
PRIVATE Void rgAmmExtractElmnt
(
RlcCb       *gCb,
Buffer     *pdu,
RlcExtHdr   *hdrInfo
)
#else
PRIVATE Void rgAmmExtractElmnt(gCb, pdu, hdrInfo)
RlcCb       *gCb;
Buffer     *pdu;
RlcExtHdr   *hdrInfo;
#endif
{
   U8   hdr;
   U8   pLen = hdrInfo->pLen;
   U8   len  = (U8)hdrInfo->len;
   U16  val;
   U8   tHdr;
   U8   fLen;
   U8   rLen;
   /* U8   rLen1 = 0; */
   U16  tVal;

   TRC2(rlcAmmExtractElmnt);

   hdr = hdrInfo->hdr;

   if (pLen == 0)
   {
      SRemPreMsg(&hdr, pdu);
      pLen = 8;
   }
   tHdr = hdr;
   if (len <= 8)
   {
      val = tHdr >> (RLC_BYTE_LEN - (len));
      hdr =  hdr << len;
      pLen -= len;
   }
   else /*if (len > 8) */
   {
      fLen = pLen;
      val = tHdr;
      val = val >> (RLC_BYTE_LEN - fLen);
      val = val << (len - fLen);
      rLen = len - fLen;
      SRemPreMsg(&hdr, pdu);
      tHdr = hdr;
      if (rLen <= 8)
      {
         hdr = hdr >> (RLC_BYTE_LEN - rLen);
         val = val | hdr;
         hdr = tHdr << rLen;
         pLen = (RLC_BYTE_LEN - rLen);
      }
      else
      {
        rLen = rLen - RLC_BYTE_LEN;
        tVal = hdr;
        tVal = tVal << rLen;
        val = val | tVal;

        SRemPreMsg(&hdr, pdu);
        tHdr = hdr;
        hdr = hdr >> (RLC_BYTE_LEN - rLen);
        val = val | hdr;
        hdr = tHdr << rLen;
        pLen = (RLC_BYTE_LEN - rLen);
      }
   }

   hdrInfo->pLen = pLen;
   hdrInfo->hdr = hdr;
   hdrInfo->val = val;

   RETVOID;
}




#ifdef ANSI
PRIVATE Void rgAmmUlHndlStatusPdu
(
Pst        *udxPst,
SuId       suId,
RlcCb       *gCb,
RlcDlRbCb   *rbCb,
Buffer     *cntrlPdu,
U8         *fByte
)
#else
PRIVATE Void rgAmmUlHndlStatusPdu(udxPst,suId,gCb, rbCb, cntrlPdu, fByte)
Pst        *udxPst;
SuId       suId;
RlcCb       *gCb;
RlcDlRbCb   *rbCb;
Buffer     *cntrlPdu;
U8         *fByte;
#endif
{
   U8             e1;
   RlcExtHdr       hdrInfo;
   RlcUdxStaPdu    *pStaPdu;
   U8             e3; /* NACK RANGE : 5GNR */
   U32            snLen;
   U32            snRange;
   U32            resrvdBitsAckSn;
   U32            resrvdBitsNackSn;


   TRC2(rgAmmUlHndlStatusPdu)

   RLCDBGP_BRIEF(gCb, "rgAmmUlHndlStatusPdu(rbCb, cntrlPdu, fByte) \n");

   RLC_MEM_ZERO(&hdrInfo, sizeof(RlcExtHdr));

   /* Extract the Control PDU */
   hdrInfo.hdr  = (*fByte << 1);
   hdrInfo.pLen = 4;

   /* D/C has been shifted in the calling function */
   if (hdrInfo.hdr & 0xE0)
   {
      RLCDBGP_ERROR(gCb, "rgAmmUlHndlStatusPdu: Reserved value for CPT received \n");
      RETVOID;
   }

   RLC_ALLOC_SHRABL_BUF(udxPst->region, 
                       udxPst->pool, 
                       pStaPdu, 
                       sizeof(RlcUdxStaPdu));

#if (ERRCLASS & ERRCLS_ADD_RES)
   /* Memory allocation failure can not be expected  */
   if(!pStaPdu)
   {
     RETVOID;
   }
#endif   

   if (rbCb->m.amDl.snLen == RLC_AM_CFG_12BIT_SN_LEN)
   {
      snLen = 12;
      resrvdBitsAckSn = RLC_STA_PDU_R_BITS_ACKSN_12BITS;
      resrvdBitsNackSn = RLC_STA_PDU_R_BITS_NACKSN_12BITS;
   }
   else if (rbCb->m.amDl.snLen == RLC_AM_CFG_18BIT_SN_LEN)
   {
      snLen = 18;
      resrvdBitsAckSn = RLC_STA_PDU_R_BITS_ACKSN_18BITS;
      resrvdBitsNackSn = RLC_STA_PDU_R_BITS_NACKSN_18BITS;
   }
   else
   {
      snLen = RLC_SN_LEN;
      resrvdBitsAckSn = 0;
      resrvdBitsAckSn = 0;
   }

   pStaPdu->nackCnt = 0;
   /* For CPT */
   hdrInfo.hdr = hdrInfo.hdr << RLC_CPT_LEN;

   /* ACK Sn */
   hdrInfo.len = RLC_SN_LEN;
   rgAmmExtractElmnt(gCb, cntrlPdu, &hdrInfo);
   pStaPdu->ackSn = hdrInfo.val;

   /* Check if NACK Exists */
   hdrInfo.len = RLC_E1_LEN;
   rgAmmExtractElmnt(gCb, cntrlPdu, &hdrInfo);
   e1 = (U8)hdrInfo.val;
   RLCDBGP_DETAIL(gCb, "rgAmmUlHndlStatusPdu: ACK SN = %d \n", pStaPdu->ackSn);

   /* Extract the Reserved Bits after ACK SN field */
   hdrInfo.len = resrvdBitsAckSn;
   rgAmmExtractElmnt(gCb, cntrlPdu, &hdrInfo);


   /* If NACK exists in control PDU */
   /* For ACKs and NACKs */
   while (e1 && (pStaPdu->nackCnt < RLC_MAX_NACK_CNT))
   {
      hdrInfo.len = snLen;
      rgAmmExtractElmnt(gCb, cntrlPdu, &hdrInfo);
      pStaPdu->nackInfo[pStaPdu->nackCnt].sn = hdrInfo.val;

      hdrInfo.len = RLC_E1_LEN;
      rgAmmExtractElmnt(gCb, cntrlPdu, &hdrInfo);
      e1 = (U8)hdrInfo.val;

      /* Extract e2 */
      /* hdrInfo.len = RLC_E1_LEN; --> previusly stored value (for e1) is
         already present*/
      rgAmmExtractElmnt(gCb, cntrlPdu, &hdrInfo);
      /*  e2 = (U8) hdrInfo.val;*/

      /* Store e2 value */
      pStaPdu->nackInfo[pStaPdu->nackCnt].isSegment = (U8) hdrInfo.val;

      /* Extract e3 : 5GNR */
      /* hdrInfo.len = RLC_E1_LEN; --> previusly stored value (for e1) is
         already present*/
      rgAmmExtractElmnt(gCb, cntrlPdu, &hdrInfo);
      e3 = (U8) hdrInfo.val;

      /* Extract Reserved Bits after NACK SN */
      hdrInfo.len = resrvdBitsNackSn;
      rgAmmExtractElmnt(gCb, cntrlPdu, &hdrInfo);

      /* Test for resegmentation */
      if (pStaPdu->nackInfo[pStaPdu->nackCnt].isSegment)
      {
         hdrInfo.len = RLC_SO_LEN_5GNR; /* 5GNR : SO Len 16 Bits */
         rgAmmExtractElmnt(gCb, cntrlPdu, &hdrInfo);
         pStaPdu->nackInfo[pStaPdu->nackCnt].soStart = hdrInfo.val;

         rgAmmExtractElmnt(gCb, cntrlPdu, &hdrInfo);
         pStaPdu->nackInfo[pStaPdu->nackCnt].soEnd   = hdrInfo.val;

         RLCDBGP_DETAIL(gCb,
                       "rgAmmUlHndlStatusPdu: soStart and soEnd = %d %d \n",
                       pStaPdu->nackInfo[pStaPdu->nackCnt].soStart,
                       pStaPdu->nackInfo[pStaPdu->nackCnt].soEnd);
      }
      else
      {
         hdrInfo.len = 0;
         pStaPdu->nackInfo[pStaPdu->nackCnt].soStart = 0;
         pStaPdu->nackInfo[pStaPdu->nackCnt].soEnd   = 0;

      }
      /* NACK RANGE Field is SET */
      if (e3)
      {
         /* Extract NACK range field */
         hdrInfo.len = RLC_NACK_RANGE_LEN;
         rgAmmExtractElmnt(gCb, cntrlPdu, &hdrInfo);
         snRange = (U8)hdrInfo.val;

         pStaPdu->nackInfo[pStaPdu->nackCnt].nackRange = snRange;

      }
      pStaPdu->nackCnt++;
   }

   gRlcStats.amRlcStats.numULStaPduRcvd++;
   gRlcStats.amRlcStats.numULNackInStaPduRcvd += pStaPdu->nackCnt;

   /* In case we have reached the MAX NACK CNT, then we should modify the ACK_SN
      to the last NACK SN + 1 and discard the original ACK_SN*/
   if(pStaPdu->nackCnt == RLC_MAX_NACK_CNT)
   {
      pStaPdu->ackSn = (pStaPdu->nackInfo[RLC_MAX_NACK_CNT-1].sn + 1) & amDl->snModMask;
   }


   /* Parse & send Status PDU to RLC-DL */
   //rlcUlUdxStaUpdReq(&(sapCb->pst), sapCb->spId, &rbCb->rlcId, pStaPdu);
   rlcUlUdxStaUpdReq(udxPst, suId, &rbCb->rlcId, pStaPdu);

   RETVOID;
}

S16 rlcProcDlStatusPdu(Pst *udxPst,SuId suId,
      CmLteCellId cellId,CmLteRnti rnti,CmLteLcId lcId,Buffer *rlcSdu)
{
   RlcDlRbCb      *rbCb = NULLP;   
   RlcDlUeCb      *ueCb = NULLP; 
   U8        fByte;
   U8        temp;
   S16       retVal = RFAILED;
   RlcCb      *gCb;
   Pst       dlRlcPst = *udxPst;

   gCb = RLC_GET_RLCCB(1); /* DL RLC instance */

   if( ROK != rlcDbmFetchDlUeCb(gCb,rnti,cellId,&(ueCb)))
   {
     printf("\n RLC UECb Not found...\n");
     return RFAILED;
   }


   rbCb = ueCb->lCh[lcId - 1].dlRbCb;

   /* Skip if mode is not AM */
   if((rbCb ==  NULLP) || (rbCb->mode != CM_LTE_MODE_AM))
   {
      return RFAILED;
   }

   if(ROK != SExamMsg((Data *)(&fByte),
            rlcSdu, 0))
   {
      printf("\n Failure in Rlc Hdr SExamMsg\n");
      return RFAILED;
   }

   if(RLC_CNTRL_PDU == ((fByte & RLC_DC_POS) >> RLC_DC_SHT))
   {
      SRemPreMsg(&temp, rlcSdu);
      dlRlcPst.selector = 1;/* LWLC*/
      rgAmmUlHndlStatusPdu(&dlRlcPst,suId,gCb, rbCb, rlcSdu, &fByte);
      retVal = ROK;
   }

   return (retVal);
}


#endif


/*@}*/

  
/********************************************************************30**
  
         End of file
**********************************************************************/
