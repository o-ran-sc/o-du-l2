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

        Name:    RLC - AM module file

        Type:    C source file

        Desc:    Source code for Acknowledged Mode Module functions such as,

                 Transmission of data/control PDUs
                 Retransmission (Feedback in terms of status)
                 Polling
                 Assemble SDUs
                 Reception - reordering
                 Duplicate detection for byte segments
                 Reassemble SDUs

        File:    kw_amm_ul.c

*********************************************************************21*/
static const char* RLOG_MODULE_NAME="AMM";
static int RLOG_MODULE_ID=2048;
static int RLOG_FILE_ID=190;

/* header include files (.h) */
#include "common_def.h"
#include "lkw.h"           /* LKW defines */
#include "ckw.h"           /* CKW defines */
#include "kwu.h"           /* KWU defines */
#include "rgu.h"           /* RGU defines */
#include "kw_udx.h"
#include "kw_err.h"        /* Err defines */
#include "kw_env.h"        /* RLC environment options */

#include "kw.h"            /* RLC defines */
#include "kw_ul.h"

/* extern (.x) include files */
#include "lkw.x"           /* LKW */
#include "ckw.x"           /* CKW */
#include "kwu.x"           /* KWU */
#include "rgu.x"           /* RGU */

#include "kw.x"
#include "kw_ul.x"
#include "kw_udx.x"

/* Variable for logging, declared in cl */
#ifndef RGL_SPECIFIC_CHANGES
#ifndef TENB_ACC
#ifndef LTE_PAL_ENB
extern U32 ulrate_rgu;
#endif
#endif
#endif
#ifndef RGL_SPECIFIC_CHANGES
#ifndef TENB_ACC
#ifndef TENB_T2K3K_SPECIFIC_CHANGES
#ifndef LTE_PAL_ENB
extern U32 isMemThreshReached(Region region);
#endif
#else
#ifndef LTE_PAL_ENB
extern U32  isMemThreshReached(Region region);
#endif
#endif
#endif
#endif
/** @file gp_amm_ul.c
@brief RLC Acknowledged Mode Uplink Module
**/
#define KW_MODULE (KW_DBGMASK_AM | KW_DBGMASK_UL) /* for debugging purpose */

/* private function declarations */

PRIVATE Void kwAmmUlAssembleCntrlInfo ARGS ((KwCb *gCb, KwUlRbCb *rbCb));

PRIVATE S16 kwAmmExtractHdr ARGS ((KwCb *gCb,
                                KwUlRbCb   *rbCb,
                                Buffer *pdu,
                                KwAmHdr *amHdr,
                                U8 *fByte));

PRIVATE Bool kwAmmUlPlacePduInRecBuf ARGS ((KwCb *gCb,
                                            Buffer *pdu,
                                            KwUlRbCb *rbCb,
                                            KwAmHdr *amHdr));

PRIVATE Void kwAmmTriggerStatus ARGS ((KwCb *gCb,
                                    KwUlRbCb *rbCb,
                                    KwSn sn,
                                    Bool discFlg));

PRIVATE S16  kwAmmUlReassembleSdus ARGS ((KwCb *gCb,
                                     KwUlRbCb *rbCb,
                                     KwAmRecBuf *recBuf));

PRIVATE Void kwAmmProcPduOrSeg ARGS ((KwCb *gCb,
                                      KwUlRbCb *rbCb,
                                      KwAmHdr *amHdr,
                                      Buffer *pdu));

PRIVATE Void kwAmmUpdExpByteSeg ARGS ((KwCb *gCb,KwAmUl *amUl, KwSeg* newSeg));

PRIVATE Void kwAmmExtractElmnt ARGS ((KwCb *gCb, Buffer *pdu, KwExtHdr *hdrInfo));

PRIVATE Void kwAmmUlHndlStatusPdu ARGS ((KwCb *gCb,
                                         KwUlRbCb *rbCb,
                                         Buffer *cntrlPdu,
                                         U8 *fByte));

/******************************************************************************

  AM Module contains the following funcitons:

  -  kwAmmProcessSdus
     -  kwAmmUlAssembleCntrlInfo
     -  kwResegRetxPdus
     -  kwAssembleSdus
     -  kwChkandSetPoll
  -  kwAmmProcessPdus
     -  kwAmmUlHndlStatusPdu
     -  kwAmmTriggerStatus
     -  kwAmmUlReassembleSdus

*******************************************************************************/
/** @addtogroup ammode */
/*@{*/

/**
 * @brief   Private function to fill NACK information in status Pdu as per 5GNR
 *
 * @param[in]   rbCb       Ul RbCb
 * @param[in]   sn         Sequence number of the PDU for which the NACK
 * @param[in]   isSegment  TRUE means NACK for segment; FALSE for PDU
 * @param[in]   soStart    SOStart
 * @param[in]   soEnd      SOEnd
 * @param[out]  statusPdu  status Pdu holder to be filled
 * @param[in]   prevNackSn It holds previous nack Sn
 *
 * @return  S16
 *    The number of bytes required to encode this NACK information
 *
 */
#ifdef ANSI
PRIVATE S16 kwAmmUlSetNackInfo
(
KwUlRbCb      *rbCb,
KwSn          sn,
Bool          isSegment,
U16           soStart,
U16           soEnd,
KwUdxDlStaPdu *statusPdu,
KwSn          *prevNackSn
)
#else
PRIVATE S16 kwAmmUlSetNackInfo(rbCb, sn, isSegment, soStart, statusPdu, prevNackSn)
KwUlRbCb      *rbCb;
KwSn          sn;
Bool          isSegment;
U16           soStart;
U16           soEnd;
KwUdxDlStaPdu *statusPdu,
KwSn          *prevNackSn;
#endif
{
   KwNackInfo   *nackInfo = (statusPdu->nackInfo + statusPdu->nackCount);
   S16           sizeToBeEncd = 0; /* Status PDu size to be encoded */

   TRC2(kwAmmUlSetNackInfo)

      /* In following cases we should increment the nackCnt & fill new NACK_SN info:
       *    1) First NACK_SN of the statusdPdu
       *    2) NACK_SN is not continuous with previous
       *    3) NACK_SN is same as previuos but segments are not continuous
       *    4) NACK_SN is continuous with previous but previous NACK_SN segments
       *       are not missing in sequence till end
       */
      if((*prevNackSn == 0xffffffff) || ((((*prevNackSn) + 1) & AMUL.snModMask) != sn) ||
            (((*prevNackSn) == sn) && (((nackInfo->soEnd + 1) != soStart))) ||
            ((nackInfo->isSegment) && (((*prevNackSn) + 1) == sn) && (nackInfo->soEnd != KW_ALL_BYTES_MISSING)))
      {
         if(nackInfo->nackRange)
         {
            if((nackInfo->soEnd) && (!nackInfo->soStart))
            {
               /*First nack_sn of this nackRange not segmented but last is segmented */
               sizeToBeEncd = 5; /*32 for soStart and soEnd and 8 for nackRange */ 
            }
            else
            {
               /*First nack_sn of this nackRange was segmented */
               sizeToBeEncd = 1; /*8 for nackRange */ 
            }
         }

         if(*prevNackSn != 0xffffffff)
         {
            /* Increment nackCount as this sn is continous */
            statusPdu->nackCount++;
            nackInfo = statusPdu->nackInfo + statusPdu->nackCount;
         }

         nackInfo->sn = sn;
         nackInfo->isSegment = isSegment;
         nackInfo->soStart = soStart;
         nackInfo->soEnd   = soEnd;
         nackInfo->nackRange = 0;

         if(isSegment)
         {
            sizeToBeEncd += ((AMUL.snLen == KW_AM_CFG_12BIT_SN_LEN)?6:7); /* NACK,E1,E2,Sostart,SoEnd */
         }
         else
         {
            sizeToBeEncd += ((AMUL.snLen == KW_AM_CFG_12BIT_SN_LEN)?2:3); /* NACK,E1,E2 */
         }
      }
      else
      {
         if(!(nackInfo->nackRange))
         {
            nackInfo->nackRange++;
         }
         /* This case means there are continuous SNs/Segments. If it is the next
          * Sn then increment nackRnage. if same SN but different segment then
          * dont increment nackRange */
         if((((*prevNackSn) + 1) & AMUL.snModMask) == sn)
         {
            nackInfo->nackRange++;
         }

         /* If NackRange is reached to max value then increment statusPdu->nackCount*/
         if(nackInfo->nackRange == 255)
         {
            statusPdu->nackCount++;
            if(nackInfo->isSegment)
            {
               sizeToBeEncd = 1; /* return only nackRangeSize*/
            }
            else if (isSegment)
            {
               /* First SN was not segmented of this nackRange but last SN is segmented */
               sizeToBeEncd = 5; /* return size of soSatrt + soEnd + nackRnage */
            }
         }

         if(isSegment)
         {
            nackInfo->isSegment = isSegment;
            nackInfo->soEnd = soEnd;
         }
         else if(nackInfo->isSegment)
         {
            nackInfo->soEnd = KW_ALL_BYTES_MISSING;
         }
         else
         {
            nackInfo->soStart = 0;
            nackInfo->soEnd =   0;
         }

      }
   *prevNackSn = sn;

   RETVALUE(sizeToBeEncd);
}

/**
 * @brief   Private handler to gather information required to create the STATUS
 *          PDU
 *
 * @details
 *    Scans the reception buffer and copies information to the UdxDlStaPdu
 *    structure about SN's  and segments not yet received. This data is
 *    sent to the DL instance so that it can create an appropriate (depending
 *    on the grants from MAC) STATUS PDU and send it to MAC.
 *
 * @param[in]  gCb      RLC instance control block
 * @param[in]  rbCb     Uplink RB control block
 *
 * @return  Void
 *
 */
#ifdef ANSI
PRIVATE Void kwAmmUlAssembleCntrlInfo
(
KwCb       *gCb,
KwUlRbCb   *rbCb
)
#else
PRIVATE Void kwAmmUlAssembleCntrlInfo(gCb, rbCb)
KwCb       *gCb;
KwUlRbCb   *rbCb;
#endif
{
   KwUdxDlStaPdu   *pStatusPdu;
   KwNackInfo      *nackInfo;
   KwSn            sn;                /* sequence number */
   KwSn            mSn;               /* Mod val of sequence number */
   KwSn            rxHighestStatus;              /* Mod val of rxHighestStatus */
   KwSeg           *seg;              /* pdu segment */
   U16             nackCnt = 0;       /* Index for staPdu */
   U16             seqSo;             /* segmment offset */
   KwUdxUlSapCb    *sapCb;
   U16             staPduEncSize = 3; /* size that would be of the encoded
                                          STATUS PDU, it is in bits; 15 for
                                          first fixed part of STATUS PDU */
   KwAmRecBuf      *recBuf = NULLP;
   KwSn            prevNackSn = 0xffffffff;

   TRC2(kwAmmUlAssembleCntrlInfo)


   sapCb = KW_GET_UDX_SAP(gCb);

   RLC_ALLOC_SHRABL_BUF(sapCb->pst.region,
                       sapCb->pst.pool,
                       pStatusPdu, 
                       sizeof(KwUdxDlStaPdu)); 

#if (ERRCLASS & ERRCLS_ADD_RES)
   /* Memory allocation failure can not be expected  */
   if(!pStatusPdu)
   {
     RETVOID;
   }
#endif

   sn = AMUL.rxNext;
   MODAMR(sn, mSn, AMUL.rxNext, AMUL.snModMask);
   MODAMR(AMUL.rxHighestStatus, rxHighestStatus, AMUL.rxNext, AMUL.snModMask);
   
   recBuf =  kwUtlGetRecBuf(AMUL.recBufLst, sn);

   while (mSn < rxHighestStatus )
   {
      /* For missing PDUs */
      if ((NULLP  == recBuf) && nackCnt < KW_MAX_NACK_CNT )
      {
         RLOG_ARG3(L_DEBUG,DBG_RBID,rbCb->rlcId.rbId, 
                  "Missing PDU's SN = %d UEID:%d CELLID:%d", 
                  sn,
                  rbCb->rlcId.ueId,
                  rbCb->rlcId.cellId);
         staPduEncSize += kwAmmUlSetNackInfo(rbCb,
                                             sn,
                                             FALSE, /* isSegment */
                                             0,     /* SOStart */
                                             0,     /* SOEnd */
                                             pStatusPdu,
                                             &prevNackSn);
      }
      else if (recBuf && (recBuf->pdu == NULLP) &&
               (recBuf->segLst.count > 0))
      {
         /* Scan through the byte segments of PDU and add this sn
            with soStart and soEnd info to staPdu */

         seqSo  = 0;
         KW_LLIST_FIRST_SEG(recBuf->segLst, seg);
         while (seg != NULLP && nackCnt < KW_MAX_NACK_CNT)
         {
            /* For missing byte segments */
            if (seg->amHdr.so != seqSo)
            {
               staPduEncSize += kwAmmUlSetNackInfo(rbCb,
                                                   sn,
                                                   TRUE,
                                                   seqSo,
                                                   seg->amHdr.so - 1,
                                                   pStatusPdu,
                                                   &prevNackSn);

               RLOG_ARG3(L_DEBUG,DBG_RBID,rbCb->rlcId.rbId,
                             "Missing byte segment's" 
                             " SN:%d UEID:%d CELLID:%d",
                             sn,
                             rbCb->rlcId.ueId,
                             rbCb->rlcId.cellId);
               RLOG_ARG4(L_DEBUG,DBG_RBID,rbCb->rlcId.rbId,
                             "soStart and soEnd = %d, %d UEID:%d CELLID:%d",
                             seqSo,
                             seg->amHdr.so - 1,
                             rbCb->rlcId.ueId,
                             rbCb->rlcId.cellId);
            }

            seqSo = seg->soEnd + 1;
            KW_LLIST_NEXT_SEG(recBuf->segLst, seg);
         }

         /* Check if the last segment is missing */
         KW_LLIST_LAST_SEG(recBuf->segLst, seg);
         if ((seg != NULLP) &&
             (seg->amHdr.si != KW_SI_LAST_SEG && nackCnt < KW_MAX_NACK_CNT))
         {
            staPduEncSize += kwAmmUlSetNackInfo(rbCb,
                                                sn,
                                                TRUE,
                                                seqSo,
                                                KW_ALL_BYTES_MISSING,
                                                pStatusPdu,
                                                &prevNackSn);

            RLOG_ARG3(L_DEBUG,DBG_RBID,rbCb->rlcId.rbId,
                          "kwAmmUlAssembleCntrlInfo: Missing (last) byte " 
                          "segment's SN:%d UEID:%d CELLID:%d",
                          sn,
                          rbCb->rlcId.ueId,
                          rbCb->rlcId.cellId);
            RLOG_ARG4(L_DEBUG,DBG_RBID,rbCb->rlcId.rbId,
                          "soStart and soEnd = %d, %d UEID:%d CELLID:%d",
                          seqSo,
                          KW_ALL_BYTES_MISSING,
                          rbCb->rlcId.ueId,
                          rbCb->rlcId.cellId);
         }
      }
      

      sn = (sn + 1) & (AMUL.snModMask); /* MOD 1024 */
      MODAMR(sn, mSn, AMUL.rxNext, AMUL.snModMask);
      
      /* Get the received Buffer the updated/next SN */
      recBuf =  kwUtlGetRecBuf(AMUL.recBufLst, sn);

      /* Find the next missing sequence number if nackCnt reaches maximum and
         still Reordering window has some missing AMDPDUs / AMDPDU segments. The
         next missing sequence number will be considered as the ack sequnece
         number in the status pdu.*/
      if((nackCnt == KW_MAX_NACK_CNT) &&
          ((recBuf == NULLP) ||
            ((recBuf->pdu == NULLP) &&
             (recBuf->segLst.count > 0))))
      {
         break;
      }
   }
 
   /*Unfortunately i have write below peice of code here because kwAmmsetNackInfo()
    * don't know that this is the last nackSn with nackRange*/
   nackInfo = &(pStatusPdu->nackInfo[pStatusPdu->nackCount]);
   if(nackInfo->nackRange)
   {
      if((nackInfo->soEnd) && (!nackInfo->soStart))
      {
         /*First nack_sn of this nackRange not segmented but last is segmented */
         staPduEncSize += 5; /*32 for soStart and soEnd and 8 for nackRange */ 
      }
      else
      {
         /*First nack_sn of this nackRange was segmented */
         staPduEncSize += 1; /*8 for nackRange */ 
      }
   }
   /* nackCount is used as an index to nackInfo array but in status Pdu it
    * should be equal to number nackInfo that are filled. hence incrementing by 1*/
   if(prevNackSn != 0xffffffff)
   {
      pStatusPdu->nackCount++;
   }
   /* Update ACK SN with the last sn for which feedback is not assembled */
   if ( mSn == rxHighestStatus)
   {
      pStatusPdu->ackSn = AMUL.rxHighestStatus;
   }
   else
   {
      pStatusPdu->ackSn = sn;
   }

   RLOG_ARG3(L_UNUSED,DBG_RBID,rbCb->rlcId.rbId,
            "kwAmmUlAssembleCntrlInfo: ACK PDU's SN = %d"
            "UEID:%d CELLID:%d",
            pStatusPdu->ackSn,
            rbCb->rlcId.ueId,
            rbCb->rlcId.cellId);

   pStatusPdu->controlBo = staPduEncSize; /*Its already in bytes */

   AMUL.staTrg = FALSE;
   AMUL.gatherStaPduInfo = FALSE;


   if (KwUlUdxStaPduReq(&sapCb->pst,
                        sapCb->spId,
                        &rbCb->rlcId,
                        pStatusPdu) != ROK)
   {
      RLOG_ARG2(L_ERROR,DBG_RBID,rbCb->rlcId.rbId,
               "Failed to Send Sta Pdu UEID:%d CELLID:%d",
               rbCb->rlcId.ueId,
               rbCb->rlcId.cellId);
      RLC_FREE_SHRABL_BUF_WC(sapCb->pst.region,
			    sapCb->pst.pool,
			    pStatusPdu, 
			    sizeof(KwUdxDlStaPdu));
   }

   RETVOID;
}

#ifdef XEON_SPECIFIC_CHANGES
extern U32  gRlcDatIndUL;
#endif

#ifdef T2K_TRIGGER_RLC_REEST
PUBLIC U32 drpRlcDrbPack;
#endif
/**
 * @brief Handler to process the PDUs received from MAC and send it to PDCP
 *
 * @details
 *    This function is invoked by UTL with the PDU(s) received from MAC.
 *    It reorders the received data PDUs and trigger status report as
 *    needed. Reassembles the SDUs in sequence and send it to PDCP.
 *    It also processes the control PDU
 *
 * @param[in]  gCb      RLC instance control block
 * @param[in]  rbCb     RB control block
 * @param[out] pduInfo  PDU Info received from MAC
 *
 *  @return Void
 *
 */
#ifdef LTE_L2_MEAS
#ifdef ANSI
PUBLIC Void kwAmmProcessPdus
(
KwCb                    *gCb,
KwUlRbCb                *rbCb,
KwPduInfo               *pduInfo,
U32                     ttiCnt
)
#else
PUBLIC Void kwAmmProcessPdus(gCb, rbCb, pduInfo, ulTimeInfo)
KwCb                    *gCb;
KwUlRbCb                *rbCb;
KwPduInfo               *pduInfo;
U32                     ttiCnt;
#endif
#else
#ifdef ANSI
PUBLIC Void kwAmmProcessPdus
(
KwCb        *gCb,
KwUlRbCb    *rbCb,
KwPduInfo   *pduInfo
)
#else
PUBLIC Void kwAmmProcessPdus(gCb, rbCb, pduInfo)
KwCb        *gCb;
KwUlRbCb    *rbCb;
KwPduInfo   *pduInfo;
#endif
#endif
{
   Buffer    *pdu;
   KwAmUl    *amUl;
   KwAmHdr   amHdr;
   U8        numPdu = 0;
   U8        numPduToProcess;
   KwSn      sn;
   KwSn      tSn;
   KwSn      mSn;
   U8        fByte;
   Bool      discFlg;
#ifdef LTE_L2_MEAS_RLC
   MsgLen              rlcSduSz;  /*Holds length of Rlc Sdu*/
#endif /* LTE_L2_MEAS */

   TRC2(kwAmmProcessPdus)


   amUl = &AMUL;

   numPduToProcess = KW_MIN(pduInfo->numPdu, RGU_MAX_PDU);
   RLOG_ARG4(L_UNUSED,DBG_RBID,rbCb->rlcId.rbId, 
            "numPdu[%ld],numPduToProcess[%ld] UEID:%ld CELLID:%ld",
            numPdu,
            numPduToProcess,
            rbCb->rlcId.ueId,
            rbCb->rlcId.cellId);

   //printf ("++++++++++++  5GNRLOG numPduToProcess %d \n", numPduToProcess);
   while (numPdu < numPduToProcess)
   {
   //printf ("++++++++++++  5GNRLOG processing pdu  %d \n", numPdu);
      discFlg = FALSE;
      pdu = pduInfo->mBuf[numPdu++];

      if (! pdu)
      {

         RLOG_ARG2(L_ERROR,DBG_RBID,rbCb->rlcId.rbId,
                  "Null Pdu UEID:%d CELLID:%d",
                  rbCb->rlcId.ueId,
                  rbCb->rlcId.cellId);
         gCb->genSts.errorPdusRecv++;
         break;
      }
#ifndef RGL_SPECIFIC_CHANGES
#ifndef TENB_ACC
#ifndef LTE_PAL_ENB
      MsgLen len;
      SFndLenMsg(pdu, &len);
      ulrate_rgu += len;
#endif
#endif
#endif      
      /* Extract AM PDU/SEG header Info */
      KW_MEM_ZERO(&amHdr, sizeof(KwAmHdr));
      /* Avoided the allocation of amHdr and sending
         a single pointer */
      if (kwAmmExtractHdr(gCb, rbCb, pdu, &amHdr, &fByte) != ROK)
      {
         RLOG_ARG2(L_ERROR,DBG_RBID,rbCb->rlcId.rbId,
                  "Header Extraction Failed UEID:%d CELLID:%d",
                  rbCb->rlcId.ueId,
                  rbCb->rlcId.cellId);
         RLC_FREE_BUF(pdu);
         gCb->genSts.errorPdusRecv++;
         continue;
      }
      /* Check if its a control PDU */
      if (amHdr.dc == 0)
      {
         kwAmmUlHndlStatusPdu(gCb, rbCb, pdu, &fByte);
         RLC_FREE_BUF(pdu);
         continue;
      }
      if((amHdr.si == KW_SI_LAST_SEG) && (!amHdr.so))
      {
         RLOG_ARG3(L_UNUSED,DBG_RBID,rbCb->rlcId.rbId,
               "kwAmmProcessPdus: Dropping PDU because SO can't be zero for last segment sn:%u "
               "UEID:%d CELLID:%d",
               amHdr.sn,
               rbCb->rlcId.ueId,
               rbCb->rlcId.cellId);
         RLC_FREE_BUF(pdu);
         continue;
      }
#ifndef RGL_SPECIFIC_CHANGES
#ifdef LTE_TDD
#ifndef TENB_ACC
#ifndef TENB_T2K3K_SPECIFIC_CHANGES
#ifndef LTE_PAL_ENB
    /* Changed the condition to TRUE from ROK  */
      if(isMemThreshReached(kwCb[0]->init.region) == TRUE)
      {
         extern U32 rlculdrop;
	 rlculdrop++;
	 RLC_FREE_BUF(pdu);
	 continue;
      }
#endif
#else
#ifndef LTE_PAL_ENB
      /*ccpu00142274 - UL memory based flow control*/
      if(isMemThreshReached(kwCb[0]->init.region) != ROK)
      {
         extern U32 rlculdrop;
         rlculdrop++;
         RLC_FREE_BUF(pdu);
         continue;
      }
#endif
#endif
#endif
#endif
#endif

#ifdef T2K_TRIGGER_RLC_REEST
      if(drpRlcDrbPack > 1000)
      {
         if(rbCb->rlcId.rbType == CM_LTE_DRB)
         {
            RLC_FREE_BUF(pdu);
            continue;
         }
      }
      drpRlcDrbPack++;
#endif
      /* Reordering data PDU */
      sn = amHdr.sn;
      if (kwAmmUlPlacePduInRecBuf(gCb,pdu, rbCb, &amHdr) == TRUE)
      {
         KwAmRecBuf      *recBuf;
         Bool   tmrRunning;
         KwSn   tVrMr;
         KwSn   mrxNextHighestRcvd;

#ifdef LTE_L2_MEAS
         kwUtlCalUlIpThrPut(gCb, rbCb, pdu, ttiCnt);
#endif /* LTE_L2_MEAS */

         /* Update rxNextHighestRcvd */
         MODAMR(sn, mSn, amUl->rxNext, amUl->snModMask);
         MODAMR(amUl->rxNextHighestRcvd, mrxNextHighestRcvd, amUl->rxNext, amUl->snModMask);
         if (mSn >= mrxNextHighestRcvd)
         {
            amUl->rxNextHighestRcvd = ((sn + 1) & (amUl->snModMask)); 

            RLOG_ARG3(L_UNUSED,DBG_RBID,rbCb->rlcId.rbId,
                  "kwAmmProcessPdus: Updated rxNextHighestRcvd = %d UEID:%d CELLID:%d",
                  amUl->rxNextHighestRcvd,
                  rbCb->rlcId.ueId,
                  rbCb->rlcId.cellId);
         }
         
         recBuf = kwUtlGetRecBuf(amUl->recBufLst, sn);
         if ((NULLP != recBuf) && ( recBuf->allRcvd))
         {
            /* deliver the reassembled RLC SDU to upper layer, 
 	       But not removed from the table */
            kwAmmUlReassembleSdus(gCb, rbCb, recBuf);
            recBuf->isDelvUpperLayer = TRUE;

            MODAMR(amUl->vrMr, tVrMr, amUl->rxNext, amUl->snModMask);
            
            /* Update rxHighestStatus */
            if (sn == amUl->rxHighestStatus)
            {
               tSn = (sn + 1) & (amUl->snModMask) ; /* MOD (2 Pwr SN LEN- 1) */

               recBuf = kwUtlGetRecBuf(amUl->recBufLst, tSn);
               /* Scan through till the upper edge of the window */
               MODAMR(tSn, mSn, amUl->rxNext, amUl->snModMask);
               while (mSn <= tVrMr)
               {
                  if ((NULLP == recBuf) || (!recBuf->allRcvd))
                  {
                     RLOG_ARG3(L_UNUSED,DBG_RBID,rbCb->rlcId.rbId,
                           "kwAmmProcessPdus: Updated rxHighestStatus:%d "
                           "UEID:%d CELLID:%d",
                           tSn,
                           rbCb->rlcId.ueId,
                           rbCb->rlcId.cellId);

                     amUl->rxHighestStatus = tSn;
                     break;
                  }
                  tSn = (tSn + 1) & (amUl->snModMask); /* MOD (2 Pwr SN LEN- 1) */
                  recBuf = kwUtlGetRecBuf(amUl->recBufLst, tSn); 
                  mSn++;
               }
            }


            /* Update rxNext */
            if (sn == amUl->rxNext)
            {
               tSn = sn;
               recBuf = kwUtlGetRecBuf(amUl->recBufLst, tSn);
               MODAMR(tSn, mSn, amUl->rxNext, amUl->snModMask);
               /* Scan through till the upper edge of the window */
               while (mSn <= tVrMr)
               {
                  if ((NULLP != recBuf) && (recBuf->allRcvd) &&
	              (TRUE == recBuf->isDelvUpperLayer))
		  {
	             /* RecBuf should remove from table 
			since PDU is already sent to upper layer */
 		      recBuf->isDelvUpperLayer = FALSE;
                      kwUtlDelRecBuf(amUl->recBufLst, recBuf, gCb);
                  }
                  else
                  {
                     amUl->rxNext = tSn;
                     amUl->vrMr = (amUl->rxNext + (KW_AM_GET_WIN_SZ(amUl->snLen))) & (amUl->snModMask);
                     break;
                  }
                  tSn = (tSn + 1) & (amUl->snModMask); 
                  recBuf = kwUtlGetRecBuf(amUl->recBufLst, tSn);
                  mSn++;
               }
            }
         }

         /* Check if reOrdTmr is running and update rxNextStatusTrig accordingly */
         tmrRunning = kwChkTmr(gCb,(PTR)rbCb, KW_EVT_AMUL_REORD_TMR);
         if (tmrRunning)
         {
            Bool snInWin = KW_AM_CHK_SN_WITHIN_RECV_WINDOW(amUl->rxNextStatusTrig, amUl);

            if ( (amUl->rxNextStatusTrig == amUl->rxNext) || ( (!snInWin) &&
                                             (amUl->rxNextStatusTrig != amUl->vrMr) ) )
            {
               kwStopTmr(gCb,(PTR)rbCb, KW_EVT_AMUL_REORD_TMR);
               tmrRunning = FALSE;
            }
         }

         if (!tmrRunning)
         {
            if (amUl->rxNextHighestRcvd > amUl->rxNext)
            {
               kwStartTmr(gCb,(PTR)rbCb, KW_EVT_AMUL_REORD_TMR);
               amUl->rxNextStatusTrig = amUl->rxNextHighestRcvd;

               RLOG_ARG3(L_DEBUG,DBG_RBID,rbCb->rlcId.rbId,
                        "kwAmmProcessPdus: Updated rxNextStatusTrig = %d UEID:%d CELLID:%d",
                        amUl->rxNextStatusTrig,
                        rbCb->rlcId.ueId,
                        rbCb->rlcId.cellId);
            }
         }
      }
      else
      {
         discFlg = TRUE;
         gRlcStats.amRlcStats.numULPdusDiscarded++;
      }

      if (amHdr.p)
      {
         kwAmmTriggerStatus(gCb,rbCb, sn, discFlg);
      }
   }

#ifdef LTE_L2_MEAS
   kwUtlCalUlIpThrPutIncTTI(gCb, rbCb,ttiCnt);
#endif /* LTE_L2_MEAS */
   gCb->genSts.pdusRecv += pduInfo->numPdu;
   if (amUl->gatherStaPduInfo)
   {
      kwAmmUlAssembleCntrlInfo(gCb,rbCb);
   }

   RETVOID;
}


/**
 * @brief Private handler to extract header Information of the PDU
 *
 * @details
 *    This function extracts the header elements of the PDU and store them
 *    in db for future reference.
 *
 *    fByte - is the first byte removed from the PDU as part of calling
 *            functions
 *
 * @param[in]  gCb     RLC instance control block
 * @param[in]  rbCb    Uplink RB control block
 * @param[in]  pdu     Received PDU
 * @param[out] amHdr   Pointer to the extracted AM header
 * @param[out] fByte   First byte removed from the PDU
 *
 * @return S16
 *     -# ROK
 *     -# RFAILED
 *
 */
#ifdef ANSI
PRIVATE S16 kwAmmExtractHdr
(
KwCb       *gCb,
KwUlRbCb   *rbCb,
Buffer     *pdu,
KwAmHdr    *amHdr,
U8         *fByte
)
#else
PRIVATE S16 kwAmmExtractHdr(gCb, rbCb, pdu, amHdr, fByte)
KwCb       *gCb;
KwUlRbCb   *rbCb;
Buffer     *pdu;
KwAmHdr    *amHdr;
U8         *fByte;
#endif
{
   U8         snByte;
   KwSn        sn = 0;
   MsgLen     pduSz;
   KwExtHdr   hdrInfo;

   TRC2(kwAmmExtractHdr)


   KW_MEM_ZERO(&hdrInfo, sizeof(KwExtHdr));

   /* Extract fixed part of the header */
   SFndLenMsg(pdu,&pduSz);
   SRemPreMsg(fByte, pdu);
   amHdr->dc = (*fByte & KW_DC_POS) >> KW_DC_SHT;
   if (KW_CNTRL_PDU == amHdr->dc)
   {
   //printf ("++++++++++++ 5GNRLOG HDR extracted CTRL : \n");
      RETVALUE(ROK);
   }

   amHdr->p  = (*fByte & KW_POLL_POS) >> KW_POLL_SHT;

   amHdr->si = (*fByte & KW_SI_POS)   >> KW_SI_SHT;

   /* 12 BIT SN */
   if (rbCb->m.amUl.snLen == KW_AM_CFG_12BIT_SN_LEN)
   {
      SRemPreMsg(&snByte, pdu);
      sn = (KwSn)(((*fByte & KW_SN_POS_12BIT) << KW_BYTE_LEN ) | snByte);
      amHdr->sn = sn;
   }
   else if (rbCb->m.amUl.snLen == KW_AM_CFG_18BIT_SN_LEN)
   {
      SRemPreMsg(&snByte, pdu);
      sn = (KwSn)(((*fByte & KW_SN_POS_18BIT) << KW_BYTE_LEN ) | snByte);

      SRemPreMsg(&snByte, pdu);
      sn = ((sn << KW_BYTE_LEN) | snByte);

      amHdr->sn = sn;
   }
   if ((amHdr->si != 0) && (amHdr->si != 0x01))
   {
      hdrInfo.len = KW_SO_LEN_5GNR;
      kwAmmExtractElmnt(gCb, pdu, &hdrInfo);
      amHdr->so = hdrInfo.val;
      pduSz -= 2;
   }

   //printf ("++++++++++++ 5GNRLOG HDR extracted DATA : sn %d  \n", sn);
   RETVALUE(ROK);
}

#ifdef OLD
/**
 * @brief Private handler to extract header Information of the PDU
 *
 * @details
 *    This function extracts the header elements of the PDU and store them
 *    in db for future reference.
 *
 *    fByte - is the first byte removed from the PDU as part of calling
 *            functions
 *
 * @param[in]  gCb     RLC instance control block
 * @param[in]  rbCb    Uplink RB control block
 * @param[in]  pdu     Received PDU
 * @param[out] amHdr   Pointer to the extracted AM header
 * @param[out] fByte   First byte removed from the PDU
 *
 * @return S16
 *     -# ROK
 *     -# RFAILED
 *
 */
#ifdef ANSI
PRIVATE S16 kwAmmExtractHdrOld
(
KwCb       *gCb,
Buffer     *pdu,
KwAmHdr    *amHdr,
U8         *fByte
)
#else
PRIVATE S16 kwAmmExtractHdrOld(gCb, pdu, amHdr, fByte)
KwCb       *gCb;
Buffer     *pdu;
KwAmHdr    *amHdr;
U8         *fByte;
#endif
{
   U8         e;
   U8         snByte;
   U16        sn;
   MsgLen     pduSz;
   MsgLen     totalSz = 0;
   KwExtHdr   hdrInfo;

   TRC2(kwAmmExtractHdrOld)


   KW_MEM_ZERO(&hdrInfo, sizeof(KwExtHdr));

   /* Extract fixed part of the header */
   SFndLenMsg(pdu,&pduSz);
   SRemPreMsg(fByte, pdu);
   amHdr->dc = (*fByte & KW_DC_POS) >> KW_DC_SHT;
   if (KW_CNTRL_PDU == amHdr->dc)
   {
      RETVALUE(ROK);
   }
   /* kw002.201 : Changed the extraction of hdr elements to avoid */
   /*             function calls                                  */
   amHdr->rf = (*fByte & KW_RF_POS)   >> KW_RF_SHT;
   amHdr->p  = (*fByte & KW_POLL_POS) >> KW_POLL_SHT;
   amHdr->fi = (*fByte & KW_FI_POS)   >> KW_FI_SHT;
   e = amHdr->e  = (*fByte & KW_E_POS)>> KW_E_SHT;

   SRemPreMsg(&snByte, pdu);
   sn = (U16)(((*fByte & KW_SN_POS) << KW_BYTE_LEN ) | snByte);
   amHdr->sn = sn;
   if (amHdr->rf == 1)
   {
      /* Extract extn part of the header */
      hdrInfo.len = KW_LSF_LEN;
      kwAmmExtractElmnt(gCb, pdu, &hdrInfo);
      amHdr->lsf = (U8)hdrInfo.val;

      hdrInfo.len = KW_SO_LEN;
      kwAmmExtractElmnt(gCb, pdu, &hdrInfo);
      amHdr->so = hdrInfo.val;
      pduSz -= 2;
   }

   amHdr->numLi = 0;
   /* Extract LIs */
   while (e && (amHdr->numLi < KW_MAX_UL_LI))
   {
      hdrInfo.len = KW_E_LEN;
      kwAmmExtractElmnt(gCb, pdu, &hdrInfo);
      e = amHdr->e = (U8)hdrInfo.val;

      /* Extract LI value*/
      hdrInfo.len = KW_LI_LEN;
      kwAmmExtractElmnt(gCb, pdu, &hdrInfo);
      /* li = hdrInfo.val;*/

      /* check if LI is zero */
      if (! hdrInfo.val)
      {
         RLOG0(L_ERROR, "Received LI as 0");
         RETVALUE(RFAILED);
      }

      /* store the extracted  LI value */
      amHdr->li[amHdr->numLi++] = hdrInfo.val;
      totalSz += hdrInfo.val;  /* incrment the size by LI value */
   }

   /*ccpu00122597:PDU is dropped if liCnt exceeds KW_MAX_LI*/
   if(e && (amHdr->numLi >= KW_MAX_UL_LI))
   {
      RLOG2(L_ERROR,"LI Count [%u] exceeds Max LI Count[%u]", 
            amHdr->numLi, KW_MAX_UL_LI);
      RETVALUE(RFAILED);
   }

   /*                                first 2 bytes + Add one for  Odd LI*/
   pduSz -= ( amHdr->numLi + (amHdr->numLi >> 1) + 2 + (amHdr->numLi & 1) );

   if ( totalSz >= pduSz )
   {   
      RLOG3(L_ERROR,"SN [%d]:Corrupted PDU as TotSz[%lu] PduSz[%lu] ",
               amHdr->sn, totalSz, pduSz);
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}
#endif

/**
 * @brief Private handler to process the status PDU
 *
 * @details
 *    Private handler invokded by kwAmmProcessPdus to process the
 *    control PDU (status report) received from its peer RLC entity.
 *
 *        - Decode the values from the received control pdu
 *        - Create a KwUdxStaPdu structure, copy the values onto it and
 *          send it to the DL instance for further processing
 *
 * @param[in]  gCb       RLC instance control block
 * @param[in]  rbCb      Uplink RB control block
 * @param[in]  cntrlPdu  Control PDU received from MAC
 * @param[in]  fByte     First byte already removed from the STATUS PDU
 *
 *  @return  Void
 *
 */
#ifdef ANSI
PRIVATE Void kwAmmUlHndlStatusPdu
(
KwCb       *gCb,
KwUlRbCb   *rbCb,
Buffer     *cntrlPdu,
U8         *fByte
)
#else
PRIVATE Void kwAmmUlHndlStatusPdu(gCb, rbCb, cntrlPdu, fByte)
KwCb       *gCb;
KwUlRbCb   *rbCb;
Buffer     *cntrlPdu;
U8         *fByte;
#endif
{
   U8             e1;
   KwExtHdr       hdrInfo;
   KwUdxStaPdu    *pStaPdu;
   KwUdxUlSapCb   *sapCb;
   U8             e3; /* NACK RANGE : 5GNR */
   U32            snLen;
   U32            snRange;
   U32            resrvdBitsAckSn=0;
   U32            resrvdBitsNackSn=0;

   TRC2(kwAmmUlHndlStatusPdu)


   KW_MEM_ZERO(&hdrInfo, sizeof(KwExtHdr));

   /* Extract the Control PDU */
   hdrInfo.hdr  = (*fByte << 1);
   hdrInfo.pLen = 4;

   /* D/C has been shifted in the calling function */
   if (hdrInfo.hdr & 0xE0)
   {
      RLOG_ARG2(L_ERROR,DBG_RBID,rbCb->rlcId.rbId,
               "Reserved value for CPT received UEID:%d CELLID:%d",
               rbCb->rlcId.ueId,
               rbCb->rlcId.cellId);

      RETVOID;
   }

   sapCb = KW_GET_UDX_SAP(gCb);

   RLC_ALLOC_SHRABL_BUF(sapCb->pst.region, 
                       sapCb->pst.pool, 
                       pStaPdu, 
                       sizeof(KwUdxStaPdu));

#if (ERRCLASS & ERRCLS_ADD_RES)
   /* Memory allocation failure can not be expected  */
   if(!pStaPdu)
   {
     RETVOID;
   }
#endif   

   if (rbCb->m.amUl.snLen == KW_AM_CFG_12BIT_SN_LEN)
   {
      snLen = 12;
      resrvdBitsAckSn = KW_STA_PDU_R_BITS_ACKSN_12BITS;
      resrvdBitsNackSn = KW_STA_PDU_R_BITS_NACKSN_12BITS;
   }
   else if (rbCb->m.amUl.snLen == KW_AM_CFG_18BIT_SN_LEN)
   {
      snLen = 18;
      resrvdBitsAckSn = KW_STA_PDU_R_BITS_ACKSN_18BITS;
      resrvdBitsNackSn = KW_STA_PDU_R_BITS_NACKSN_18BITS;
   }
   else
   {
      snLen = KW_SN_LEN;
      resrvdBitsAckSn = 0;
      resrvdBitsAckSn = 0;
   }

   pStaPdu->nackCnt = 0;
   /* For CPT */
   hdrInfo.hdr = hdrInfo.hdr << KW_CPT_LEN;

   /* ACK Sn */
   hdrInfo.len = snLen;
   kwAmmExtractElmnt(gCb, cntrlPdu, &hdrInfo);
   pStaPdu->ackSn = hdrInfo.val;

   //printf ("++++++++++++ 5GNRLOG HNDL STATUS acksn %d : \n",  pStaPdu->ackSn);
   /* Check if NACK Exists */
   hdrInfo.len = KW_E1_LEN;
   kwAmmExtractElmnt(gCb, cntrlPdu, &hdrInfo);
   e1 = (U8)hdrInfo.val;
   RLOG_ARG3(L_UNUSED,DBG_RBID,rbCb->rlcId.rbId, 
            "kwAmmUlHndlStatusPdu: ACK SN = %d UEID:%d CELLID:%d",
            pStaPdu->ackSn,
            rbCb->rlcId.ueId,
            rbCb->rlcId.cellId);

   /* Extract the Reserved Bits after ACK SN field */
   hdrInfo.len = resrvdBitsAckSn;
   kwAmmExtractElmnt(gCb, cntrlPdu, &hdrInfo);

   /* If NACK exists in control PDU */
   /* For ACKs and NACKs */
   while (e1 && (pStaPdu->nackCnt < KW_MAX_NACK_CNT))
   {
      hdrInfo.len = snLen;
      kwAmmExtractElmnt(gCb, cntrlPdu, &hdrInfo);
      pStaPdu->nackInfo[pStaPdu->nackCnt].sn = hdrInfo.val;

      hdrInfo.len = KW_E1_LEN;
      kwAmmExtractElmnt(gCb, cntrlPdu, &hdrInfo);
      e1 = (U8)hdrInfo.val;

      /* Extract e2 */
      /* hdrInfo.len = KW_E1_LEN; --> previusly stored value (for e1) is
         already present*/
      kwAmmExtractElmnt(gCb, cntrlPdu, &hdrInfo);
      /*  e2 = (U8) hdrInfo.val;*/

      /* Store e2 value */
      pStaPdu->nackInfo[pStaPdu->nackCnt].isSegment = (U8) hdrInfo.val;

      /* Extract e3 : 5GNR */
      /* hdrInfo.len = KW_E1_LEN; --> previusly stored value (for e1) is
         already present*/
      kwAmmExtractElmnt(gCb, cntrlPdu, &hdrInfo);
      e3 = (U8) hdrInfo.val;

      /* Extract Reserved Bits after NACK SN */
      hdrInfo.len = resrvdBitsNackSn;
      kwAmmExtractElmnt(gCb, cntrlPdu, &hdrInfo);

      /* Test for resegmentation */
      if (pStaPdu->nackInfo[pStaPdu->nackCnt].isSegment)
      {
         hdrInfo.len = KW_SO_LEN_5GNR; /* 5GNR : SO Len 16 Bits */
         kwAmmExtractElmnt(gCb, cntrlPdu, &hdrInfo);
         pStaPdu->nackInfo[pStaPdu->nackCnt].soStart = hdrInfo.val;

         kwAmmExtractElmnt(gCb, cntrlPdu, &hdrInfo);
         pStaPdu->nackInfo[pStaPdu->nackCnt].soEnd   = hdrInfo.val;

         RLOG_ARG4(L_DEBUG,DBG_RBID,rbCb->rlcId.rbId,
                       "kwAmmUlHndlStatusPdu: soStart and soEnd = %d %d"
                       "UEID:%d CELLID:%d",
                       pStaPdu->nackInfo[pStaPdu->nackCnt].soStart,
                       pStaPdu->nackInfo[pStaPdu->nackCnt].soEnd,      
                       rbCb->rlcId.ueId,
                       rbCb->rlcId.cellId);
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
         hdrInfo.len = KW_NACK_RANGE_LEN;
         kwAmmExtractElmnt(gCb, cntrlPdu, &hdrInfo);
         snRange = (U8)hdrInfo.val;

         pStaPdu->nackInfo[pStaPdu->nackCnt].nackRange = snRange;

      }
      pStaPdu->nackCnt++;
   }

   gRlcStats.amRlcStats.numULStaPduRcvd++;
   gRlcStats.amRlcStats.numULNackInStaPduRcvd += pStaPdu->nackCnt;

   /* In case we have reached the MAX NACK CNT, then we should modify the ACK_SN
      to the last NACK SN + 1 and discard the original ACK_SN*/
   if(pStaPdu->nackCnt == KW_MAX_NACK_CNT)
   {
      pStaPdu->ackSn = (pStaPdu->nackInfo[KW_MAX_NACK_CNT-1].sn + 1) & (rbCb->m.amUl.snModMask);
   }


   /* Parse & send Status PDU to RLC-DL */
   KwUlUdxStaUpdReq(&(sapCb->pst), sapCb->spId, &rbCb->rlcId, pStaPdu);

   RETVOID;
}

/**
 * @brief Private handler to release all stored segments
 *
 * @details
 *    Private handler invokded by kwAmmUlPlacePduInRecBuf to release the
 *    stored segements in case a complete PDU is received later.
 *
 * @param[in]  gCb      RLC instance control block
 * @param[in]  recBuf   Buffer that stores a received PDU or segments
 *
 * @return  Void
 *
 */
#ifdef ANSI
PRIVATE Void kwAmmUlRlsAllSegs
(
KwCb         *gCb,
KwAmRecBuf   *recBuf
)
#else
PRIVATE Void kwAmmUlRlsAllSegs(gCb,recBuf)
KwCb         *gCb;
KwAmRecBuf   *recBuf;
#endif
{
   KwSeg *seg;

   TRC2(kwAmmUlRlsAllSegs)

   KW_LLIST_FIRST_SEG(recBuf->segLst, seg);
   while (seg != NULLP)
   {
      RLC_FREE_BUF_WC(seg->seg);
      cmLListDelFrm(&(recBuf->segLst),&(seg->lstEnt));
      RLC_FREE_WC(gCb,seg, sizeof(KwSeg));
      KW_LLIST_FIRST_SEG(recBuf->segLst, seg);
   }

   RETVOID;
}

/**
 * @brief Private handler to store the received segment
 *
 * @details
 *    Private handler invokded by kwAmmUlPlacePduInRecBuf to add a received
 *    segment in reception buffer of a RBCB.
 *    - It is responsible for detecting duplicate segments
 *    - Adding it at appropriate position in the received buffer
 *    - Calling ExpByteSeg to set expSo field in the receiver buffer
 *
 * @param[in]  gCb      RLC instance control block
 * @param[in]  rbCb     Radio Bearer Contro Block
 * @param[in]  amHdr    AM Header received
 * @param[in]  pdu      Buffer received other than the headers
 * @param[in]  pduSz    size of the PDU buffer received
 *
 * @return  Bool
 *   -#TRUE  Successful insertion into the receiver buffer
 *   -#FALSE Possibly a duplicate segment
 */
#ifdef ANSI
PRIVATE Bool kwAmmAddRcvdSeg
(
KwCb       *gCb,
KwUlRbCb   *rbCb,
KwAmHdr    *amHdr,
Buffer     *pdu,
U16        pduSz
)
#else
PRIVATE Bool kwAmmAddRcvdSeg(gCb, rbCb, amHdr, pdu, pduSz)
KwCb       *gCb;
KwUlRbCb   *rbCb;
KwAmHdr    *amHdr;
Buffer     *pdu;
U16        pduSz;
#endif
{
   KwAmRecBuf   *recBuf = NULLP;
   KwSeg        *seg;
   KwSeg        *tseg;
   U16          soEnd;       /* Holds the SoEnd of received segment */
   U16          expSo = 0;   /* Expected SO */

   TRC2(kwAmmAddRcvdSeg)

   soEnd = amHdr->so + pduSz - 1;
   recBuf =  kwUtlGetRecBuf(AMUL.recBufLst, amHdr->sn);

   if (NULLP == recBuf)
   {
      RLC_ALLOC(gCb,recBuf, sizeof(KwAmRecBuf));
#if (ERRCLASS & ERRCLS_ADD_RES)
      if (recBuf == NULLP)
      {
         RLOG_ARG2(L_FATAL,DBG_RBID,rbCb->rlcId.rbId,
                  "Memory allocation failed UEID:%d CELLID:%d",
                  rbCb->rlcId.ueId,
                  rbCb->rlcId.cellId);

         RLC_FREE_BUF(pdu);
         RETVALUE(FALSE);
      }
#endif /* ERRCLASS & ERRCLS_RES */
      kwUtlStoreRecBuf(AMUL.recBufLst, recBuf, amHdr->sn);
   }
   else
   {
      if (recBuf->allRcvd == TRUE)
      {
         RLC_FREE_BUF(pdu);
         RETVALUE(FALSE);
      }
   }
 			
   recBuf->isDelvUpperLayer = FALSE;
   /* kw003.201 - Move past the segments that are different than the */
   /*             one received.                                      */
   KW_LLIST_FIRST_SEG(recBuf->segLst, seg);
   while ((seg != NULLP) && (seg->amHdr.so < amHdr->so))
   {
      expSo = seg->amHdr.so + seg->segSz;
      KW_LLIST_NEXT_SEG(recBuf->segLst, seg);
   }

   /* The received segment should start after the end of previous seg */
   if (expSo > amHdr->so)
   {
      /* This is a duplicate segment */
      gRlcStats.amRlcStats.numRlcAmCellDupPduRx++;
      RLC_FREE_BUF(pdu);
      RETVALUE(FALSE);
   }

   if ((seg) && (seg->amHdr.so <= soEnd))
   {
      /* This is a duplicate segment */
      gRlcStats.amRlcStats.numRlcAmCellDupPduRx++;
      RLC_FREE_BUF(pdu);
      RETVALUE(FALSE);
   }

   /* If we have come this far, we have to add this segment to the   */
   /* reception buffer as we either have eliminated duplicates or    */
   /* have found none.                                               */
   RLC_ALLOC_WC(gCb,tseg, sizeof(KwSeg));
#if (ERRCLASS & ERRCLS_ADD_RES)
   if (tseg == NULLP)
   {
      RLOG_ARG2(L_FATAL,DBG_RBID,rbCb->rlcId.rbId, 
               "Memory allocation failed UEID:%d CELLID:%d",
               rbCb->rlcId.ueId,
               rbCb->rlcId.cellId);
      RLC_FREE_BUF(pdu);
      RETVALUE(FALSE);
   }
#endif /* ERRCLASS & ERRCLS_RES */

   tseg->seg = pdu;
   tseg->segSz = pduSz;
   KW_MEM_CPY(&tseg->amHdr, amHdr, sizeof(KwAmHdr));
   recBuf->amHdr.si = amHdr->si;
   recBuf->amHdr.sn = amHdr->sn;
   tseg->soEnd = soEnd;
   if (seg == NULLP)
   {
      cmLListAdd2Tail(&recBuf->segLst, &tseg->lstEnt);
   }
   else
   {
      recBuf->segLst.crnt = &seg->lstEnt;
      cmLListInsCrnt(&recBuf->segLst, &tseg->lstEnt);
   }
   tseg->lstEnt.node = (PTR)tseg;
   kwAmmUpdExpByteSeg(gCb,&AMUL,tseg);

   RETVALUE(TRUE);
}

/**
 * @brief Private handler to place the PDU in the reception buffer
 *
 * @details
 *    This function checks if the received PDU's SN falls within the
 *    receiving window, after which it places the same in the reception
 *    buffer if its not a duplicate.
 *
 * @param[in]  gCb   RLC instance control block
 * @param[in]  pdu   Received PDU
 * @param[in]  rbCb  Uplink AM Radio Bearer
 * @param[out] amUl  AM UL Info
 *
 * @return Bool
 *     -# TRUE
 *     -# FALSE
 *
 */
#ifdef ANSI
PRIVATE Bool kwAmmUlPlacePduInRecBuf
(
KwCb       *gCb,
Buffer     *pdu,
KwUlRbCb   *rbCb,
KwAmHdr    *amHdr
)
#else
PRIVATE Bool kwAmmUlPlacePduInRecBuf(gCb, pdu, rbCb, amHdr)
KwCb       *gCb;
Buffer     *pdu;
KwUlRbCb   *rbCb;
KwAmHdr    *amHdr;
#endif
{
   KwSn     sn;
   MsgLen   pduSz;
   KwAmUl   *amUl = &(rbCb->m.amUl);

   TRC2(kwAmmUlPlacePduInRecBuf)


   sn = amHdr->sn;
   SFndLenMsg(pdu, &pduSz);

   gCb->genSts.bytesRecv += pduSz;
   gRlcStats.amRlcStats.numRlcAmCellSduBytesRx += pduSz; 
   if (!KW_AM_CHK_SN_WITHIN_RECV_WINDOW(sn, amUl))
   {
      gRlcStats.amRlcStats.numRlcAmCellDropOutWinRx++;
      RLOG_ARG3(L_UNUSED,DBG_RBID,rbCb->rlcId.rbId,
                    "kwAmmUlPlacePduInRecBuf: SN  %d outside the window"
                    "UEID:%d CELLID:%d",
                    sn,
                    rbCb->rlcId.ueId,
                    rbCb->rlcId.cellId);

      gCb->genSts.unexpPdusRecv++;
      RLC_FREE_BUF(pdu);
      RETVALUE(FALSE);
   }

   if (amHdr->si == 0)
   {
      KwAmRecBuf *recBuf = kwUtlGetRecBuf(amUl->recBufLst, sn);

      /* We received a complete PDU. Either we already have it, in which */
      /* case we just ignore the new PDU and discard it. Otherwise,      */
      /* store the received PDU in the reception buffer                  */
      if (NULLP == recBuf)
      {
         RLC_ALLOC(gCb, recBuf, sizeof(KwAmRecBuf));
#if (ERRCLASS & ERRCLS_ADD_RES)
         if (recBuf == NULLP)
         {
            RLOG_ARG2(L_FATAL,DBG_RBID,rbCb->rlcId.rbId,
                     "Memory allocation failed UEID:%d CELLID:%d",
                     rbCb->rlcId.ueId,
                     rbCb->rlcId.cellId);
            RLC_FREE_BUF(pdu);
            RETVALUE(FALSE);
         }
#endif /* ERRCLASS & ERRCLS_RES */
         kwUtlStoreRecBuf(AMUL.recBufLst, recBuf, sn);
      }
      else if (recBuf->allRcvd != TRUE)
      {
         kwAmmUlRlsAllSegs(gCb,recBuf);
      }
      else
      {
         gRlcStats.amRlcStats.numRlcAmCellDupPduRx++;
         gCb->genSts.unexpPdusRecv++;
         RLC_FREE_BUF(pdu);
         RETVALUE(FALSE);
      }
      recBuf->isDelvUpperLayer = FALSE;
      recBuf->pdu = pdu;
      recBuf->pduSz = pduSz;
      recBuf->allRcvd = TRUE;
      gRlcStats.amRlcStats.numRlcAmCellSduRx++;
      KW_MEM_CPY(&recBuf->amHdr, amHdr, sizeof(KwAmHdr));
      RETVALUE(TRUE);
   }
   else
   {
      /* We received a segment. We need to add that to the existing */
      /* segments, if any.                                          */
      RETVALUE(kwAmmAddRcvdSeg(gCb,rbCb, amHdr, pdu, pduSz));
   }
}

/**
 * @brief Private handler to trigger status report
 *
 * @details
 *    Private handler invokded by kwAmmProcessPdus to check if the
 *    status report need to be sent, and update the status trigger
 *    flag accordingly based on status prohibit timer.
 *
 *    - Check if the received pdu's sn is less than rxHighestStatus, set the
 *      staTrg flag.
 *    - If staProhTmr is not running, calculate cntrlBo, else it'll be
 *      updated at the expiry of staProhTmr.
 *    - Expiry of reOrdTmr also will set staTrg flag.
 *
 * @param[in]  gCb       RLC instance control block
 * @param[in]  rbCb      Uplink RB control block
 * @param[in]  sn        Sequence number of the pdu based on which to check if
 *                       status needs to be triggered
 * @param[in]  discFlg   Whether this pdu was discarded or not
 *
 * @return  Void
 *
 */
#ifdef ANSI
PRIVATE Void kwAmmTriggerStatus
(
KwCb       *gCb,
KwUlRbCb   *rbCb,
KwSn       sn,
Bool       discFlg
)
#else
PRIVATE Void kwAmmTriggerStatus(gCb,rbCb, sn, discFlg)
KwCb       *gCb;
KwUlRbCb   *rbCb;
KwSn       sn;
Bool       discFlg;
#endif
{
   Bool     tmrRunning;
   KwSn     tSn;
   KwSn     tVrMr;
   KwSn     trxHighestStatus;
   KwAmUl   *amUl = &(rbCb->m.amUl);

   TRC2(kwAmmTriggerStatus)


   MODAMR(amUl->vrMr, tVrMr, amUl->rxNext, amUl->snModMask);
   MODAMR(amUl->rxHighestStatus, trxHighestStatus, amUl->rxNext, amUl->snModMask);
   MODAMR(sn , tSn, amUl->rxNext, amUl->snModMask);

   /* kw005.201 Product CR ccpu00117114       *
    * The "=" in the 2nd condition is removed */
   if ((discFlg) || (tSn < trxHighestStatus) || (tSn >= tVrMr))
   {
      RLOG_ARG2(L_UNUSED,DBG_RBID,rbCb->rlcId.rbId, 
               "kwAmmTriggerStatus: Set Status Trigger UEID:%d CELLID:%d",
                     rbCb->rlcId.ueId,
                     rbCb->rlcId.cellId);

      amUl->staTrg = TRUE;
      amUl->gatherStaPduInfo = FALSE;

      /* Check if staProhTmr is running */
      tmrRunning = kwChkTmr(gCb,(PTR) rbCb, KW_EVT_AMUL_STA_PROH_TMR);

      if (!tmrRunning)
      {
         amUl->gatherStaPduInfo = TRUE;
      }
   }

   RETVOID;
}

/**
 * @brief Private handler to reassemble from a segment or a PDU
 *
 * @details
 *    Private handler invokded by kwAmmReassembleSdus with either a
 *    PDU or a segment of a PDU. This is also called in the case of
 *    reestablishment and hence out of sequence joining is also to
 *    be supported
 *
 *
 * @param[in]  gCb     RLC instance control block
 * @param[in]  rbCb    Uplink RB control block
 * @param[in]  amHdr   AM header received for this segment/PDU
 * @param[in]  pdu     PDU to be reassembled
 *
 * @return  Void
 *
 */
#ifdef ANSI
PRIVATE Void kwAmmProcPduOrSeg
(
KwCb       *gCb,
KwUlRbCb   *rbCb,
KwAmHdr    *amHdr,
Buffer     *pdu
)
#else
PRIVATE Void kwAmmProcPduOrSeg(gCb, rbCb, amHdr, pdu)
KwCb       *gCb;
KwUlRbCb   *rbCb;
KwAmHdr    *amHdr;
Buffer     *pdu;
#endif
{

   TRC2(kwAmmProcPduOrSeg)

   if ((AMUL.expSn != amHdr->sn) || (AMUL.expSo != amHdr->so))
   {
      /* Release the existing partial SDU as we have PDUs or */
      /* segments that are out of sequence                   */
      rbCb->m.amUl.isOutOfSeq = TRUE;
      RLC_FREE_BUF(AMUL.partialSdu);
   }

   //if (amHdr->fi & KW_FI_FIRST_SEG)
   if (amHdr->si == 0x01)
   {/* first Segment of the SDU */
      if (AMUL.partialSdu != NULLP)
      { /* Some old SDU may be present */
         RLC_FREE_BUF_WC(AMUL.partialSdu);
      }
      AMUL.partialSdu = pdu;
      pdu = NULLP;
   }
   else if(amHdr->si == 0x03)
   {/* Middle or last segment of the SUD */
      SCatMsg(AMUL.partialSdu,pdu, M1M2);
      RLC_FREE_BUF_WC(pdu);
      pdu = NULLP;
   }
   else if (amHdr->si ==  0x02)
   {
      SCatMsg(pdu,AMUL.partialSdu,M2M1);
      RLC_FREE_BUF_WC(AMUL.partialSdu);
   }

   if (pdu != NULLP)
   {
      AMUL.partialSdu = NULLP;
      kwUtlSndDatInd(gCb,rbCb, pdu);
   }

   RETVOID;
}

/**
 *
 * @brief Private handler to reassemble SDUs
 *
 * @details
 *    Private handler invokded by kwAmmProcessPdus with the PDU
 *    from the reception buffer in sequence to reassemble SDUs and
 *    send it to PDCP.
 *
 *        - With the stored header info, FI and LSF segment / concatenate
 *          PDUs or byte segments of PDUs to get the associated SDU.
 *
 * @param[in]  rbCb     RB control block
 * @param[in]  pdu      PDU to be reassembled
 *
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 *
 */
#ifdef ANSI
PRIVATE S16 kwAmmUlReassembleSdus
(
KwCb         *gCb,
KwUlRbCb     *rbCb,
KwAmRecBuf   *recBuf
)
#else
PRIVATE S16 kwAmmUlReassembleSdus(gCb, rbCb, recBuf)
KwCb         *gCb;
KwUlRbCb     *rbCb;
KwAmRecBuf   *recBuf;
#endif
{
   KwSeg        *seg;

   TRC2(kwAmmUlReassembleSdus)
   //if (recBuf->amHdr.rf == 0)
   if (recBuf->amHdr.si == 0)
   {
      /* This is a PDU */
      kwAmmProcPduOrSeg(gCb,rbCb, &recBuf->amHdr, recBuf->pdu);
      /* Assign NULLP to recBuf->pdu as this PDU is sent to PDCP */
      recBuf->pdu = NULLP;
      AMUL.expSn = (recBuf->amHdr.sn + 1) & (AMUL.snModMask); /* MOD 1024 */
      AMUL.expSo = 0;
   }
   else
   {
      /* This is a set of segments */
      KW_LLIST_FIRST_SEG(recBuf->segLst, seg);
      AMUL.expSn = recBuf->amHdr.sn;
      AMUL.expSo = 0;
      while(seg)
      {
         kwAmmProcPduOrSeg(gCb,rbCb, &seg->amHdr, seg->seg);
         AMUL.expSo = seg->soEnd + 1;

         cmLListDelFrm(&(recBuf->segLst),&(seg->lstEnt));
         RLC_FREE_WC(gCb, seg, sizeof(KwSeg));

         KW_LLIST_FIRST_SEG(recBuf->segLst, seg);
      }
      AMUL.expSn = (recBuf->amHdr.sn + 1) & (AMUL.snModMask); /* MOD 1024 */
      AMUL.expSo = 0;
   }

   RETVALUE(ROK);
}

/**
 * @brief   Handler to process the re-establishment request received from UIM
 *
 * @param[in] gCb         RLC instance control block
 * @param[in] rlcId       RLC identifier
 * @param[in] sendReEst   Whether to send back restablishment complete or not
 * @param[in] rbCb        Uplink RB control block
 *
 * @return  Void
 *
 */
#ifdef ANSI
PUBLIC Void kwAmmUlReEstablish
(
KwCb         *gCb,
CmLteRlcId   rlcId,
Bool         sendReEst,
KwUlRbCb     *rbCb
)
#else
PUBLIC Void kwAmmUlReEstablish(gCb, rlcId, sendReEst, rbCb)
KwCb         *gCb;
CmLteRlcId   rlcId;
Bool         sendReEst;
KwUlRbCb     *rbCb;
#endif
{
   KwSn   sn;
   KwSn   mSn;
   KwSn   mVrMr;

#ifndef KW_PDCP
   KwKwuSapCb *kwKwSap;
#endif
   KwAmRecBuf   *recBuf = NULLP;

   TRC2(kwAmmUlReEstablish);


   sn = AMUL.rxNext;

   MODAMR(AMUL.vrMr, mVrMr, AMUL.rxNext, AMUL.snModMask);
   MODAMR(sn, mSn, AMUL.rxNext, AMUL.snModMask);

   /* Reassemble SDUs from PDUs with SN less than upper edge of the window */
   while (mSn < mVrMr)
   {
      recBuf = kwUtlGetRecBuf(AMUL.recBufLst, sn);
      if (NULLP != recBuf)
      {
         if (recBuf->allRcvd == TRUE)
         {
            kwAmmUlReassembleSdus(gCb,rbCb, recBuf);
         }
         else
         {
            /* Remove PDU and segments */
            if(recBuf->pdu)
            {
               RLC_FREE_BUF_WC(recBuf->pdu);
            }
            /* Release all the segments*/
            kwAmmUlRlsAllSegs(gCb,recBuf);
         }
         kwUtlDelRecBuf(AMUL.recBufLst, recBuf, gCb);
      }
      sn = (sn + 1) & (AMUL.snModMask); /* MOD 1024 */
      MODAMR(sn, mSn, AMUL.rxNext, AMUL.snModMask);
   }
   /* Discard remaining PDUs and bytesegments in recBuf */

   /* Stop all timers and reset variables */
   if(TRUE == kwChkTmr(gCb,(PTR)rbCb,KW_EVT_AMUL_REORD_TMR))
   {
       kwStopTmr(gCb,(PTR)rbCb, KW_EVT_AMUL_REORD_TMR);
   }
   if(TRUE == kwChkTmr(gCb,(PTR)rbCb,KW_EVT_AMUL_STA_PROH_TMR))
   {
       kwStopTmr(gCb,(PTR)rbCb, KW_EVT_AMUL_STA_PROH_TMR);
   }

   AMUL.rxNext  = 0;
   AMUL.rxNextHighestRcvd  = 0;
   AMUL.rxNextStatusTrig  = 0;
   rbCb->m.amUl.vrMr = (rbCb->m.amUl.rxNext + KW_AM_GET_WIN_SZ(rbCb->m.amUl.snLen)) & (rbCb->m.amUl.snModMask);
   AMUL.rxHighestStatus = 0;
   AMUL.staTrg  = FALSE;
   AMUL.gatherStaPduInfo = FALSE;
   AMUL.expSn = 0;
   AMUL.expSo = 0;
   if (AMUL.partialSdu != NULLP)
   {
     RLC_FREE_BUF(AMUL.partialSdu);
   }
   kwKwSap = gCb->u.ulCb->kwuUlSap + KW_UI_PDCP;

   if(sendReEst)
   {
      KwUiKwuReEstCmpInd(&kwKwSap->pst, kwKwSap->suId, rlcId);
      rbCb->m.amUl.isOutOfSeq = FALSE;
   }

   RETVOID;
}

/**
 * @brief  Handler for reorder timer expiry
 *
 * @details
 *    This function is used to handle events upon expiry of reorder timer
 *
 *  @param[in] gCb   RLC instance control block
 *  @param[in] rbCb  RB control block
 *
 *  @return  Void
 *
 */

#ifdef ANSI
PUBLIC Void kwAmmReOrdTmrExp
(
KwCb        *gCb,
KwUlRbCb    *rbCb
)
#else
PUBLIC Void kwAmmReOrdTmrExp(rbCb)
KwCb        *gCb;
KwUlRbCb    *rbCb;
#endif
{
   KwAmUl *amUl = &(rbCb->m.amUl);
   KwSn sn;
   KwSn mSn;
   KwSn mVrMr;
   KwSn mrxHighestStatus;
   KwSn mrxNextHighestRcvd;
   Bool tmrRunning = FALSE;
   KwAmRecBuf   *recBuf = NULLP;

   TRC2(kwAmmReOrdTmrExp);


   /* Update rxHighestStatus */
   sn = amUl->rxNextStatusTrig;

   MODAMR(sn, mSn, amUl->rxNext, amUl->snModMask);
   MODAMR(amUl->vrMr, mVrMr, amUl->rxNext, amUl->snModMask);
   recBuf = kwUtlGetRecBuf(AMUL.recBufLst, sn);

   while (mSn < mVrMr)
   {
      if ((recBuf == NULLP) ||
          ((recBuf != NULLP) && (!recBuf->allRcvd)) )
      {
         amUl->rxHighestStatus = sn;
         amUl->staTrg = TRUE;
         amUl->gatherStaPduInfo = FALSE;

         /* Check if staProhTmr is running */
         tmrRunning = kwChkTmr(gCb,(PTR) rbCb, KW_EVT_AMUL_STA_PROH_TMR);

         if (!tmrRunning)
         {
            gRlcStats.amRlcStats.numULReOrdTimerExpires++;
            amUl->gatherStaPduInfo = TRUE;
            kwAmmUlAssembleCntrlInfo(gCb, rbCb);
         }

         break;
      }
      sn = (sn + 1) & (amUl->snModMask); 
      MODAMR(sn, mSn, amUl->rxNext, amUl->snModMask);
   }

   /* Update rxNextStatusTrig */
   MODAMR(amUl->rxNextHighestRcvd, mrxNextHighestRcvd, amUl->rxNext, amUl->snModMask);
   MODAMR(amUl->rxHighestStatus, mrxHighestStatus, amUl->rxNext, amUl->snModMask);
   if (mrxNextHighestRcvd > mrxHighestStatus)
   {
      kwStartTmr(gCb,(PTR)rbCb, KW_EVT_AMUL_REORD_TMR);
      amUl->rxNextStatusTrig = amUl->rxNextHighestRcvd;
   }

   RETVOID;
} /* kwAmmReOrdTmrExp */

/**
 * @brief  Handler for status prohibit timer expiry
 *
 * @details
 *    This function is used to handle events upon expiry of status prohibit
 *    timer
 *
 *  @param[in] gCb   RLC instance control block
 *  @param[in] rbCb   RB control block
 *
 *  @return  Void
 *
 */

#ifdef ANSI
PUBLIC Void kwAmmStaProTmrExp
(
KwCb        *gCb,
KwUlRbCb    *rbCb
)
#else
PUBLIC Void kwAmmStaProTmrExp(gCb, rbCb)
KwCb        *gCb;
KwUlRbCb    *rbCb;
#endif
{
   KwAmUl *amUl = &(rbCb->m.amUl);

   TRC2(kwAmmStaProTmrExp);


   amUl->gatherStaPduInfo = FALSE;

   if (amUl->staTrg == TRUE)
   {
      amUl->gatherStaPduInfo = TRUE;
      /* kw002.201 : Sending StaRsp after StaProhibit tmr expiry */
      kwAmmUlAssembleCntrlInfo(gCb,rbCb);
   }

   RETVOID;
} /* kwAmmStaProTmrExp */

/**
 * @brief  Handler to extract an element of AM Header
 *
 * @details
 *    This function is used to extract an element of AM header.
 *
 * @param[in]     pdu      The pdu to be decoded
 * @param[in,out] hdrInfo  Container to hold the decoded info
 *
 * @return Void
 *
 */

#ifdef ANSI
PRIVATE Void kwAmmExtractElmnt
(
KwCb       *gCb,
Buffer     *pdu,
KwExtHdr   *hdrInfo
)
#else
PRIVATE Void kwAmmExtractElmnt(gCb, pdu, hdrInfo)
KwCb       *gCb;
Buffer     *pdu;
KwExtHdr   *hdrInfo;
#endif
{
   U8   hdr;
   U8   pLen = hdrInfo->pLen;
   U8   len  = (U8)hdrInfo->len;
   U32  val;
   U8   tHdr;
   U8   fLen;
   U8   rLen;
   /* U8   rLen1 = 0; */
   U16  tVal;

   TRC2(kwAmmExtractElmnt);

   hdr = hdrInfo->hdr;

   if (pLen == 0)
   {
      SRemPreMsg(&hdr, pdu);
      pLen = 8;
   }
   tHdr = hdr;
   if (len <= 8)
   {
      val = tHdr >> (KW_BYTE_LEN - (len));
      hdr =  hdr << len;
      pLen -= len;
   }
   else /*if (len > 8) */
   {
      fLen = pLen;
      val = tHdr;
      val = val >> (KW_BYTE_LEN - fLen);
      val = val << (len - fLen);
      rLen = len - fLen;
      SRemPreMsg(&hdr, pdu);
      tHdr = hdr;
      if (rLen <= 8)
      {
         hdr = hdr >> (KW_BYTE_LEN - rLen);
         val = val | hdr;
         hdr = tHdr << rLen;
         pLen = (KW_BYTE_LEN - rLen);
      }
      else
      {
        rLen = rLen - KW_BYTE_LEN;
        tVal = hdr;
        tVal = tVal << rLen;
        val = val | tVal;

        SRemPreMsg(&hdr, pdu);
        tHdr = hdr;
        hdr = hdr >> (KW_BYTE_LEN - rLen);
        val = val | hdr;
        hdr = tHdr << rLen;
        pLen = (KW_BYTE_LEN - rLen);
      }
   }

   hdrInfo->pLen = pLen;
   hdrInfo->hdr = hdr;
   hdrInfo->val = val;

   RETVOID;
}

/**
 * @brief  Handler to updated expected byte seg
 *
 * @details
 *    This function is used to update expected byte segment. The next segment
 *    expected is indicated by the SO of the segment which is expected. Intially
 *    the segment with SO 0 is expected and then in order. When all the segments
 *    are received (which would happen when an expected SO is encountered
 *    with LSF set) the allRcvd flag is set to TRUE
 *
 * @param[in]  gCb   RLC instance control block
 * @param[in]  amUl  AM Uplink Control Block
 * @param[in]  seg   Newly received segment
 *
 * @return Void
 *
 */

#ifdef ANSI
PRIVATE Void kwAmmUpdExpByteSeg
(
KwCb     *gCb,
KwAmUl   *amUl,
KwSeg    *seg
)
#else
PRIVATE Void kwAmmUpdExpByteSeg(gCb, amUl, seg)
KwCb     *gCb;
KwAmUl   *amUl;
KwSeg    *seg;
#endif
{
   U16    newExpSo; /* The new expected SO */
   KwSn   sn = seg->amHdr.sn;
   Bool   lstRcvd=FALSE;
   KwAmRecBuf *recBuf = NULLP;
   
   TRC2(kwAmmUpdExpByteSeg);


   recBuf = kwUtlGetRecBuf(amUl->recBufLst, sn);
   if ((recBuf == NULLP) || (recBuf && (seg->amHdr.so != recBuf->expSo)))
   {
      RETVOID;
   }

   newExpSo   = seg->soEnd + 1;
   recBuf->expSo = newExpSo;
   //lstRcvd = seg->amHdr.lsf;
   if(seg->amHdr.si == 0x2)
   {  
      lstRcvd = TRUE;
   } 
   /* kw003.201 - This should update seg with the one after newSeg */
   KW_LLIST_NEXT_SEG(recBuf->segLst, seg);
   while(seg)
   {
      /* keep going ahead as long as the expectedSo match with the header so
         else store the expSo for later checking again */
      if(seg->amHdr.si == 0x2)
      {  
         lstRcvd = TRUE;
      } 
      if (seg->amHdr.so == newExpSo)
      {
         newExpSo = seg->soEnd + 1;
         recBuf->expSo = newExpSo;
         //lstRcvd = seg->amHdr.lsf;
         KW_LLIST_NEXT_SEG(recBuf->segLst, seg);
      }
      else
      {
         recBuf->expSo = newExpSo;
         RETVOID;
      }
   }
   if (lstRcvd == TRUE)
   {
      recBuf->allRcvd = TRUE;
      gRlcStats.amRlcStats.numRlcAmCellSduRx++;
   }

   RETVOID;
}

/**
 * @brief
 *   Function to release/free the Acknowledged Mode Module  RbCb buffers
 *
 * @details
 *   This primitive Frees the AM RbCb transmission Buffer, retransmission
 *   Buffer and reciption Buffers
 *
 * @param [in]   gCb   - RLC instance Control Block
 * @param [in]   rbCb  - RB Control Block
 *
 * @return   void
 */
#ifdef ANSI
PUBLIC Void kwAmmFreeUlRbCb
(
KwCb       *gCb,
KwUlRbCb   *rbCb
)
#else
PUBLIC Void kwAmmFreeUlRbCb(gCb,rbCb)
KwCb       *gCb;
KwUlRbCb   *rbCb;
#endif
{
   KwSn         curSn = 0;           /* Sequence number of PDU */
   KwSn         windSz;              /* PDU window size */
   KwAmRecBuf *recBuf = NULLP;

   TRC2(kwAmmFreeUlRbCb)


   windSz  =  (KW_AM_GET_WIN_SZ(rbCb->m.amUl.snLen)) << 1;

   if(TRUE == kwChkTmr(gCb,(PTR)rbCb,KW_EVT_AMUL_REORD_TMR))
   {
      kwStopTmr(gCb,(PTR)rbCb, KW_EVT_AMUL_REORD_TMR);
   }
   if(TRUE == kwChkTmr(gCb,(PTR)rbCb,KW_EVT_AMUL_STA_PROH_TMR))
   {
      kwStopTmr(gCb,(PTR)rbCb, KW_EVT_AMUL_STA_PROH_TMR);
   }


   /* on the first loop winSz is always greater than zero
    while( ( curSn < windSz ) hence changing to do while */
   do
   {
      recBuf = kwUtlGetRecBuf(rbCb->m.amUl.recBufLst, curSn);
      if ( recBuf != NULLP )
      {
         if (recBuf->pdu != NULLP)
         {
            RLC_FREE_BUF_WC(recBuf->pdu);
         }
         /* Release all the segments */
         kwAmmUlRlsAllSegs(gCb,recBuf);
         kwUtlDelRecBuf(rbCb->m.amUl.recBufLst, recBuf, gCb);
      }
      curSn++;
   }while ( curSn < windSz );

#ifndef LTE_TDD 
      RLC_FREE_WC(gCb,rbCb->m.amUl.recBufLst, (KW_RCV_BUF_BIN_SIZE * sizeof(CmLListCp)));
      rbCb->m.amUl.recBufLst = NULLP;
#endif

   if(rbCb->m.amUl.partialSdu != NULLP)
   {
      RLC_FREE_BUF_WC(rbCb->m.amUl.partialSdu);
   }
   RETVOID;
} /* kwAmmFreeUlRbCb */


/*@}*/


/********************************************************************30**

         End of file
**********************************************************************/
