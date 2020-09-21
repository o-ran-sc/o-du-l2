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
#define RLC_MODULE (RLC_DBGMASK_AM | RLC_DBGMASK_UL) /* for debugging purpose */

/* private function declarations */

PRIVATE Void rlcAmmUlAssembleCntrlInfo ARGS ((RlcCb *gCb, RlcUlRbCb *rbCb));

PRIVATE S16 rlcAmmExtractHdr ARGS ((RlcCb *gCb,
                                RlcUlRbCb   *rbCb,
                                Buffer *pdu,
                                RlcAmHdr *amHdr,
                                U8 *fByte));

PRIVATE Bool rlcAmmUlPlacePduInRecBuf ARGS ((RlcCb *gCb,
                                            Buffer *pdu,
                                            RlcUlRbCb *rbCb,
                                            RlcAmHdr *amHdr));

PRIVATE Void rlcAmmTriggerStatus ARGS ((RlcCb *gCb,
                                    RlcUlRbCb *rbCb,
                                    RlcSn sn,
                                    Bool discFlg));

PRIVATE S16  rlcAmmUlReassembleSdus ARGS ((RlcCb *gCb,
                                     RlcUlRbCb *rbCb,
                                     RlcAmRecBuf *recBuf));

PRIVATE Void rlcAmmProcPduOrSeg ARGS ((RlcCb *gCb,
                                      RlcUlRbCb *rbCb,
                                      RlcAmHdr *amHdr,
                                      Buffer *pdu));

PRIVATE Void rlcAmmUpdExpByteSeg ARGS ((RlcCb *gCb,RlcAmUl *amUl, RlcSeg* newSeg));

PRIVATE Void rlcAmmExtractElmnt ARGS ((RlcCb *gCb, Buffer *pdu, RlcExtHdr *hdrInfo));

PRIVATE Void rlcAmmUlHndlStatusPdu ARGS ((RlcCb *gCb,
                                         RlcUlRbCb *rbCb,
                                         Buffer *cntrlPdu,
                                         U8 *fByte));

/******************************************************************************

  AM Module contains the following funcitons:

  -  rlcAmmProcessSdus
     -  rlcAmmUlAssembleCntrlInfo
     -  rlcResegRetxPdus
     -  rlcAssembleSdus
     -  kwChkandSetPoll
  -  rlcAmmProcessPdus
     -  rlcAmmUlHndlStatusPdu
     -  rlcAmmTriggerStatus
     -  rlcAmmUlReassembleSdus

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
PRIVATE S16 rlcAmmUlSetNackInfo
(
RlcUlRbCb      *rbCb,
RlcSn          sn,
Bool          isSegment,
U16           soStart,
U16           soEnd,
RlcUdxDlStaPdu *statusPdu,
RlcSn          *prevNackSn
)
#else
PRIVATE S16 rlcAmmUlSetNackInfo(rbCb, sn, isSegment, soStart, statusPdu, prevNackSn)
RlcUlRbCb      *rbCb;
RlcSn          sn;
Bool          isSegment;
U16           soStart;
U16           soEnd;
RlcUdxDlStaPdu *statusPdu,
RlcSn          *prevNackSn;
#endif
{
   RlcNackInfo   *nackInfo = (statusPdu->nackInfo + statusPdu->nackCount);
   S16           sizeToBeEncd = 0; /* Status PDu size to be encoded */

   TRC2(rlcAmmUlSetNackInfo)

      /* In following cases we should increment the nackCnt & fill new NACK_SN info:
       *    1) First NACK_SN of the statusdPdu
       *    2) NACK_SN is not continuous with previous
       *    3) NACK_SN is same as previuos but segments are not continuous
       *    4) NACK_SN is continuous with previous but previous NACK_SN segments
       *       are not missing in sequence till end
       */
      if((*prevNackSn == 0xffffffff) || ((((*prevNackSn) + 1) & AMUL.snModMask) != sn) ||
            (((*prevNackSn) == sn) && (((nackInfo->soEnd + 1) != soStart))) ||
            ((nackInfo->isSegment) && (((*prevNackSn) + 1) == sn) && (nackInfo->soEnd != RLC_ALL_BYTES_MISSING)))
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
            sizeToBeEncd += ((AMUL.snLen == RLC_AM_CFG_12BIT_SN_LEN)?6:7); /* NACK,E1,E2,Sostart,SoEnd */
         }
         else
         {
            sizeToBeEncd += ((AMUL.snLen == RLC_AM_CFG_12BIT_SN_LEN)?2:3); /* NACK,E1,E2 */
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
            nackInfo->soEnd = RLC_ALL_BYTES_MISSING;
         }
         else
         {
            nackInfo->soStart = 0;
            nackInfo->soEnd =   0;
         }

      }
   *prevNackSn = sn;

   return (sizeToBeEncd);
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
PRIVATE Void rlcAmmUlAssembleCntrlInfo
(
RlcCb       *gCb,
RlcUlRbCb   *rbCb
)
#else
PRIVATE Void rlcAmmUlAssembleCntrlInfo(gCb, rbCb)
RlcCb       *gCb;
RlcUlRbCb   *rbCb;
#endif
{
   RlcUdxDlStaPdu   *pStatusPdu;
   RlcNackInfo      *nackInfo;
   RlcSn            sn;                /* sequence number */
   RlcSn            mSn;               /* Mod val of sequence number */
   RlcSn            rxHighestStatus;              /* Mod val of rxHighestStatus */
   RlcSeg           *seg;              /* pdu segment */
   U16             nackCnt = 0;       /* Index for staPdu */
   U16             seqSo;             /* segmment offset */
   RlcUdxUlSapCb    *sapCb;
   U16             staPduEncSize = 3; /* size that would be of the encoded
                                          STATUS PDU, it is in bits; 15 for
                                          first fixed part of STATUS PDU */
   RlcAmRecBuf      *recBuf = NULLP;
   RlcSn            prevNackSn = 0xffffffff;

   TRC2(rlcAmmUlAssembleCntrlInfo)


   sapCb = RLC_GET_UDX_SAP(gCb);

   RLC_ALLOC_SHRABL_BUF(sapCb->pst.region,
                       sapCb->pst.pool,
                       pStatusPdu, 
                       sizeof(RlcUdxDlStaPdu)); 

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
   
   recBuf =  rlcUtlGetRecBuf(AMUL.recBufLst, sn);

   while (mSn < rxHighestStatus )
   {
      /* For missing PDUs */
      if ((NULLP  == recBuf) && nackCnt < RLC_MAX_NACK_CNT )
      {
         RLOG_ARG3(L_DEBUG,DBG_RBID,rbCb->rlcId.rbId, 
                  "Missing PDU's SN = %d UEID:%d CELLID:%d", 
                  sn,
                  rbCb->rlcId.ueId,
                  rbCb->rlcId.cellId);
         staPduEncSize += rlcAmmUlSetNackInfo(rbCb,
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
         RLC_LLIST_FIRST_SEG(recBuf->segLst, seg);
         while (seg != NULLP && nackCnt < RLC_MAX_NACK_CNT)
         {
            /* For missing byte segments */
            if (seg->amHdr.so != seqSo)
            {
               staPduEncSize += rlcAmmUlSetNackInfo(rbCb,
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
            RLC_LLIST_NEXT_SEG(recBuf->segLst, seg);
         }

         /* Check if the last segment is missing */
         RLC_LLIST_LAST_SEG(recBuf->segLst, seg);
         if ((seg != NULLP) &&
             (seg->amHdr.si != RLC_SI_LAST_SEG && nackCnt < RLC_MAX_NACK_CNT))
         {
            staPduEncSize += rlcAmmUlSetNackInfo(rbCb,
                                                sn,
                                                TRUE,
                                                seqSo,
                                                RLC_ALL_BYTES_MISSING,
                                                pStatusPdu,
                                                &prevNackSn);

            RLOG_ARG3(L_DEBUG,DBG_RBID,rbCb->rlcId.rbId,
                          "rlcAmmUlAssembleCntrlInfo: Missing (last) byte " 
                          "segment's SN:%d UEID:%d CELLID:%d",
                          sn,
                          rbCb->rlcId.ueId,
                          rbCb->rlcId.cellId);
            RLOG_ARG4(L_DEBUG,DBG_RBID,rbCb->rlcId.rbId,
                          "soStart and soEnd = %d, %d UEID:%d CELLID:%d",
                          seqSo,
                          RLC_ALL_BYTES_MISSING,
                          rbCb->rlcId.ueId,
                          rbCb->rlcId.cellId);
         }
      }
      

      sn = (sn + 1) & (AMUL.snModMask); /* MOD 1024 */
      MODAMR(sn, mSn, AMUL.rxNext, AMUL.snModMask);
      
      /* Get the received Buffer the updated/next SN */
      recBuf =  rlcUtlGetRecBuf(AMUL.recBufLst, sn);

      /* Find the next missing sequence number if nackCnt reaches maximum and
         still Reordering window has some missing AMDPDUs / AMDPDU segments. The
         next missing sequence number will be considered as the ack sequnece
         number in the status pdu.*/
      if((nackCnt == RLC_MAX_NACK_CNT) &&
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
            "rlcAmmUlAssembleCntrlInfo: ACK PDU's SN = %d"
            "UEID:%d CELLID:%d",
            pStatusPdu->ackSn,
            rbCb->rlcId.ueId,
            rbCb->rlcId.cellId);

   pStatusPdu->controlBo = staPduEncSize; /*Its already in bytes */

   AMUL.staTrg = FALSE;
   AMUL.gatherStaPduInfo = FALSE;


   if (rlcUlUdxStaPduReq(&sapCb->pst,
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
			    sizeof(RlcUdxDlStaPdu));
   }

   RETVOID;
}

#ifdef XEON_SPECIFIC_CHANGES
extern U32  gRlcDatIndUL;
#endif

#ifdef T2K_TRIGGER_RLC_REEST
U32 drpRlcDrbPack;
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
Void rlcAmmProcessPdus
(
RlcCb                    *gCb,
RlcUlRbCb                *rbCb,
KwPduInfo               *pduInfo,
U32                     ttiCnt
)
#else
Void rlcAmmProcessPdus(gCb, rbCb, pduInfo, ulTimeInfo)
RlcCb                    *gCb;
RlcUlRbCb                *rbCb;
KwPduInfo               *pduInfo;
U32                     ttiCnt;
#endif
#else
#ifdef ANSI
Void rlcAmmProcessPdus
(
RlcCb        *gCb,
RlcUlRbCb    *rbCb,
KwPduInfo   *pduInfo
)
#else
Void rlcAmmProcessPdus(gCb, rbCb, pduInfo)
RlcCb        *gCb;
RlcUlRbCb    *rbCb;
KwPduInfo   *pduInfo;
#endif
#endif
{
   Buffer    *pdu;
   RlcAmUl    *amUl;
   RlcAmHdr   amHdr;
   U8        numPdu = 0;
   U8        numPduToProcess;
   RlcSn      sn;
   RlcSn      tSn;
   RlcSn      mSn;
   U8        fByte;
   Bool      discFlg;
#ifdef LTE_L2_MEAS_RLC
   MsgLen              rlcSduSz;  /*Holds length of Rlc Sdu*/
#endif /* LTE_L2_MEAS */

   TRC2(rlcAmmProcessPdus)


   amUl = &AMUL;

   numPduToProcess = RLC_MIN(pduInfo->numPdu, RGU_MAX_PDU);
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
      RLC_MEM_ZERO(&amHdr, sizeof(RlcAmHdr));
      /* Avoided the allocation of amHdr and sending
         a single pointer */
      if (rlcAmmExtractHdr(gCb, rbCb, pdu, &amHdr, &fByte) != ROK)
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
         rlcAmmUlHndlStatusPdu(gCb, rbCb, pdu, &fByte);
         RLC_FREE_BUF(pdu);
         continue;
      }
      if((amHdr.si == RLC_SI_LAST_SEG) && (!amHdr.so))
      {
         RLOG_ARG3(L_UNUSED,DBG_RBID,rbCb->rlcId.rbId,
               "rlcAmmProcessPdus: Dropping PDU because SO can't be zero for last segment sn:%u "
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
      if(isMemThreshReached(rlcCb[0]->init.region) == TRUE)
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
      if(isMemThreshReached(rlcCb[0]->init.region) != ROK)
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
      if (rlcAmmUlPlacePduInRecBuf(gCb,pdu, rbCb, &amHdr) == TRUE)
      {
         RlcAmRecBuf      *recBuf;
         Bool   tmrRunning;
         RlcSn   tVrMr;
         RlcSn   mrxNextHighestRcvd;

#ifdef LTE_L2_MEAS
         rlcUtlCalUlIpThrPut(gCb, rbCb, pdu, ttiCnt);
#endif /* LTE_L2_MEAS */

         /* Update rxNextHighestRcvd */
         MODAMR(sn, mSn, amUl->rxNext, amUl->snModMask);
         MODAMR(amUl->rxNextHighestRcvd, mrxNextHighestRcvd, amUl->rxNext, amUl->snModMask);
         if (mSn >= mrxNextHighestRcvd)
         {
            amUl->rxNextHighestRcvd = ((sn + 1) & (amUl->snModMask)); 

            RLOG_ARG3(L_UNUSED,DBG_RBID,rbCb->rlcId.rbId,
                  "rlcAmmProcessPdus: Updated rxNextHighestRcvd = %d UEID:%d CELLID:%d",
                  amUl->rxNextHighestRcvd,
                  rbCb->rlcId.ueId,
                  rbCb->rlcId.cellId);
         }
         
         recBuf = rlcUtlGetRecBuf(amUl->recBufLst, sn);
         if ((NULLP != recBuf) && ( recBuf->allRcvd))
         {
            /* deliver the reassembled RLC SDU to upper layer, 
 	       But not removed from the table */
            rlcAmmUlReassembleSdus(gCb, rbCb, recBuf);
            recBuf->isDelvUpperLayer = TRUE;

            MODAMR(amUl->vrMr, tVrMr, amUl->rxNext, amUl->snModMask);
            
            /* Update rxHighestStatus */
            if (sn == amUl->rxHighestStatus)
            {
               tSn = (sn + 1) & (amUl->snModMask) ; /* MOD (2 Pwr SN LEN- 1) */

               recBuf = rlcUtlGetRecBuf(amUl->recBufLst, tSn);
               /* Scan through till the upper edge of the window */
               MODAMR(tSn, mSn, amUl->rxNext, amUl->snModMask);
               while (mSn <= tVrMr)
               {
                  if ((NULLP == recBuf) || (!recBuf->allRcvd))
                  {
                     RLOG_ARG3(L_UNUSED,DBG_RBID,rbCb->rlcId.rbId,
                           "rlcAmmProcessPdus: Updated rxHighestStatus:%d "
                           "UEID:%d CELLID:%d",
                           tSn,
                           rbCb->rlcId.ueId,
                           rbCb->rlcId.cellId);

                     amUl->rxHighestStatus = tSn;
                     break;
                  }
                  tSn = (tSn + 1) & (amUl->snModMask); /* MOD (2 Pwr SN LEN- 1) */
                  recBuf = rlcUtlGetRecBuf(amUl->recBufLst, tSn); 
                  mSn++;
               }
            }


            /* Update rxNext */
            if (sn == amUl->rxNext)
            {
               tSn = sn;
               recBuf = rlcUtlGetRecBuf(amUl->recBufLst, tSn);
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
                      rlcUtlDelRecBuf(amUl->recBufLst, recBuf, gCb);
                  }
                  else
                  {
                     amUl->rxNext = tSn;
                     amUl->vrMr = (amUl->rxNext + (RLC_AM_GET_WIN_SZ(amUl->snLen))) & (amUl->snModMask);
                     break;
                  }
                  tSn = (tSn + 1) & (amUl->snModMask); 
                  recBuf = rlcUtlGetRecBuf(amUl->recBufLst, tSn);
                  mSn++;
               }
            }
         }

         /* Check if reOrdTmr is running and update rxNextStatusTrig accordingly */
         tmrRunning = rlcChkTmr(gCb,(PTR)rbCb, RLC_EVT_AMUL_REORD_TMR);
         if (tmrRunning)
         {
            Bool snInWin = RLC_AM_CHK_SN_WITHIN_RECV_WINDOW(amUl->rxNextStatusTrig, amUl);

            if ( (amUl->rxNextStatusTrig == amUl->rxNext) || ( (!snInWin) &&
                                             (amUl->rxNextStatusTrig != amUl->vrMr) ) )
            {
               rlcStopTmr(gCb,(PTR)rbCb, RLC_EVT_AMUL_REORD_TMR);
               tmrRunning = FALSE;
            }
         }

         if (!tmrRunning)
         {
            if (amUl->rxNextHighestRcvd > amUl->rxNext)
            {
               rlcStartTmr(gCb,(PTR)rbCb, RLC_EVT_AMUL_REORD_TMR);
               amUl->rxNextStatusTrig = amUl->rxNextHighestRcvd;

               RLOG_ARG3(L_DEBUG,DBG_RBID,rbCb->rlcId.rbId,
                        "rlcAmmProcessPdus: Updated rxNextStatusTrig = %d UEID:%d CELLID:%d",
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
         rlcAmmTriggerStatus(gCb,rbCb, sn, discFlg);
      }
   }

#ifdef LTE_L2_MEAS
   rlcUtlCalUlIpThrPutIncTTI(gCb, rbCb,ttiCnt);
#endif /* LTE_L2_MEAS */
   gCb->genSts.pdusRecv += pduInfo->numPdu;
   if (amUl->gatherStaPduInfo)
   {
      rlcAmmUlAssembleCntrlInfo(gCb,rbCb);
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
PRIVATE S16 rlcAmmExtractHdr
(
RlcCb       *gCb,
RlcUlRbCb   *rbCb,
Buffer     *pdu,
RlcAmHdr    *amHdr,
U8         *fByte
)
#else
PRIVATE S16 rlcAmmExtractHdr(gCb, rbCb, pdu, amHdr, fByte)
RlcCb       *gCb;
RlcUlRbCb   *rbCb;
Buffer     *pdu;
RlcAmHdr    *amHdr;
U8         *fByte;
#endif
{
   U8         snByte;
   RlcSn        sn = 0;
   MsgLen     pduSz;
   RlcExtHdr   hdrInfo;

   TRC2(rlcAmmExtractHdr)


   RLC_MEM_ZERO(&hdrInfo, sizeof(RlcExtHdr));

   /* Extract fixed part of the header */
   SFndLenMsg(pdu,&pduSz);
   SRemPreMsg(fByte, pdu);
   amHdr->dc = (*fByte & RLC_DC_POS) >> RLC_DC_SHT;
   if (RLC_CNTRL_PDU == amHdr->dc)
   {
   //printf ("++++++++++++ 5GNRLOG HDR extracted CTRL : \n");
      return ROK;
   }

   amHdr->p  = (*fByte & RLC_POLL_POS) >> RLC_POLL_SHT;

   amHdr->si = (*fByte & RLC_SI_POS)   >> RLC_SI_SHT;

   /* 12 BIT SN */
   if (rbCb->m.amUl.snLen == RLC_AM_CFG_12BIT_SN_LEN)
   {
      SRemPreMsg(&snByte, pdu);
      sn = (RlcSn)(((*fByte & RLC_SN_POS_12BIT) << RLC_BYTE_LEN ) | snByte);
      amHdr->sn = sn;
   }
   else if (rbCb->m.amUl.snLen == RLC_AM_CFG_18BIT_SN_LEN)
   {
      SRemPreMsg(&snByte, pdu);
      sn = (RlcSn)(((*fByte & RLC_SN_POS_18BIT) << RLC_BYTE_LEN ) | snByte);

      SRemPreMsg(&snByte, pdu);
      sn = ((sn << RLC_BYTE_LEN) | snByte);

      amHdr->sn = sn;
   }
   if ((amHdr->si != 0) && (amHdr->si != 0x01))
   {
      hdrInfo.len = RLC_SO_LEN_5GNR;
      rlcAmmExtractElmnt(gCb, pdu, &hdrInfo);
      amHdr->so = hdrInfo.val;
      pduSz -= 2;
   }

   //printf ("++++++++++++ 5GNRLOG HDR extracted DATA : sn %d  \n", sn);
   return ROK;
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
PRIVATE S16 rlcAmmExtractHdrOld
(
RlcCb       *gCb,
Buffer     *pdu,
RlcAmHdr    *amHdr,
U8         *fByte
)
#else
PRIVATE S16 rlcAmmExtractHdrOld(gCb, pdu, amHdr, fByte)
RlcCb       *gCb;
Buffer     *pdu;
RlcAmHdr    *amHdr;
U8         *fByte;
#endif
{
   U8         e;
   U8         snByte;
   U16        sn;
   MsgLen     pduSz;
   MsgLen     totalSz = 0;
   RlcExtHdr   hdrInfo;

   TRC2(rlcAmmExtractHdrOld)


   RLC_MEM_ZERO(&hdrInfo, sizeof(RlcExtHdr));

   /* Extract fixed part of the header */
   SFndLenMsg(pdu,&pduSz);
   SRemPreMsg(fByte, pdu);
   amHdr->dc = (*fByte & RLC_DC_POS) >> RLC_DC_SHT;
   if (RLC_CNTRL_PDU == amHdr->dc)
   {
      return ROK;
   }
   /* kw002.201 : Changed the extraction of hdr elements to avoid */
   /*             function calls                                  */
   amHdr->rf = (*fByte & RLC_RF_POS)   >> RLC_RF_SHT;
   amHdr->p  = (*fByte & RLC_POLL_POS) >> RLC_POLL_SHT;
   amHdr->fi = (*fByte & RLC_FI_POS)   >> RLC_FI_SHT;
   e = amHdr->e  = (*fByte & RLC_E_POS)>> RLC_E_SHT;
    
   SRemPreMsg(&snByte, pdu);
   sn = (U16)(((*fByte & RLC_SN_POS) << RLC_BYTE_LEN ) | snByte);
   amHdr->sn = sn;
   if (amHdr->rf == 1)
   {
      /* Extract extn part of the header */
      hdrInfo.len = RLC_LSF_LEN;
      rlcAmmExtractElmnt(gCb, pdu, &hdrInfo);
      amHdr->lsf = (U8)hdrInfo.val;

      hdrInfo.len = RLC_SO_LEN;
      rlcAmmExtractElmnt(gCb, pdu, &hdrInfo);
      amHdr->so = hdrInfo.val;
      pduSz -= 2;
   }

   amHdr->numLi = 0;
   /* Extract LIs */
   while (e && (amHdr->numLi < RLC_MAX_UL_LI))
   {
      hdrInfo.len = RLC_E_LEN;
      rlcAmmExtractElmnt(gCb, pdu, &hdrInfo);
      e = amHdr->e = (U8)hdrInfo.val;

      /* Extract LI value*/
      hdrInfo.len = RLC_LI_LEN;
      rlcAmmExtractElmnt(gCb, pdu, &hdrInfo);
      /* li = hdrInfo.val;*/

      /* check if LI is zero */
      if (! hdrInfo.val)
      {
         RLOG0(L_ERROR, "Received LI as 0");
         return RFAILED;
      }

      /* store the extracted  LI value */
      amHdr->li[amHdr->numLi++] = hdrInfo.val;
      totalSz += hdrInfo.val;  /* incrment the size by LI value */
   }

   /*ccpu00122597:PDU is dropped if liCnt exceeds RLC_MAX_LI*/
   if(e && (amHdr->numLi >= RLC_MAX_UL_LI))
   {
      RLOG2(L_ERROR,"LI Count [%u] exceeds Max LI Count[%u]", 
            amHdr->numLi, RLC_MAX_UL_LI);
      return RFAILED;
   }

   /*                                first 2 bytes + Add one for  Odd LI*/
   pduSz -= ( amHdr->numLi + (amHdr->numLi >> 1) + 2 + (amHdr->numLi & 1) );

   if ( totalSz >= pduSz )
   {   
      RLOG3(L_ERROR,"SN [%d]:Corrupted PDU as TotSz[%lu] PduSz[%lu] ",
               amHdr->sn, totalSz, pduSz);
      return RFAILED;
   }

   return ROK;
}
#endif

/**
 * @brief Private handler to process the status PDU
 *
 * @details
 *    Private handler invokded by rlcAmmProcessPdus to process the
 *    control PDU (status report) received from its peer RLC entity.
 *
 *        - Decode the values from the received control pdu
 *        - Create a RlcUdxStaPdu structure, copy the values onto it and
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
PRIVATE Void rlcAmmUlHndlStatusPdu
(
RlcCb       *gCb,
RlcUlRbCb   *rbCb,
Buffer     *cntrlPdu,
U8         *fByte
)
#else
PRIVATE Void rlcAmmUlHndlStatusPdu(gCb, rbCb, cntrlPdu, fByte)
RlcCb       *gCb;
RlcUlRbCb   *rbCb;
Buffer     *cntrlPdu;
U8         *fByte;
#endif
{
   U8             e1;
   RlcExtHdr       hdrInfo;
   RlcUdxStaPdu    *pStaPdu;
   RlcUdxUlSapCb   *sapCb;
   U8             e3; /* NACK RANGE : 5GNR */
   U32            snLen;
   U32            snRange;
   U32            resrvdBitsAckSn=0;
   U32            resrvdBitsNackSn=0;

   TRC2(rlcAmmUlHndlStatusPdu)


   RLC_MEM_ZERO(&hdrInfo, sizeof(RlcExtHdr));

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

   sapCb = RLC_GET_UDX_SAP(gCb);

   RLC_ALLOC_SHRABL_BUF(sapCb->pst.region, 
                       sapCb->pst.pool, 
                       pStaPdu, 
                       sizeof(RlcUdxStaPdu));

#if (ERRCLASS & ERRCLS_ADD_RES)
   /* Memory allocation failure can not be expected  */
   if(!pStaPdu)
   {
     RETVOID;
   }
#endif   

   if (rbCb->m.amUl.snLen == RLC_AM_CFG_12BIT_SN_LEN)
   {
      snLen = 12;
      resrvdBitsAckSn = RLC_STA_PDU_R_BITS_ACKSN_12BITS;
      resrvdBitsNackSn = RLC_STA_PDU_R_BITS_NACKSN_12BITS;
   }
   else if (rbCb->m.amUl.snLen == RLC_AM_CFG_18BIT_SN_LEN)
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
   hdrInfo.len = snLen;
   rlcAmmExtractElmnt(gCb, cntrlPdu, &hdrInfo);
   pStaPdu->ackSn = hdrInfo.val;

   //printf ("++++++++++++ 5GNRLOG HNDL STATUS acksn %d : \n",  pStaPdu->ackSn);
   /* Check if NACK Exists */
   hdrInfo.len = RLC_E1_LEN;
   rlcAmmExtractElmnt(gCb, cntrlPdu, &hdrInfo);
   e1 = (U8)hdrInfo.val;
   RLOG_ARG3(L_UNUSED,DBG_RBID,rbCb->rlcId.rbId, 
            "rlcAmmUlHndlStatusPdu: ACK SN = %d UEID:%d CELLID:%d",
            pStaPdu->ackSn,
            rbCb->rlcId.ueId,
            rbCb->rlcId.cellId);

   /* Extract the Reserved Bits after ACK SN field */
   hdrInfo.len = resrvdBitsAckSn;
   rlcAmmExtractElmnt(gCb, cntrlPdu, &hdrInfo);

   /* If NACK exists in control PDU */
   /* For ACKs and NACKs */
   while (e1 && (pStaPdu->nackCnt < RLC_MAX_NACK_CNT))
   {
      hdrInfo.len = snLen;
      rlcAmmExtractElmnt(gCb, cntrlPdu, &hdrInfo);
      pStaPdu->nackInfo[pStaPdu->nackCnt].sn = hdrInfo.val;

      hdrInfo.len = RLC_E1_LEN;
      rlcAmmExtractElmnt(gCb, cntrlPdu, &hdrInfo);
      e1 = (U8)hdrInfo.val;

      /* Extract e2 */
      /* hdrInfo.len = RLC_E1_LEN; --> previusly stored value (for e1) is
         already present*/
      rlcAmmExtractElmnt(gCb, cntrlPdu, &hdrInfo);
      /*  e2 = (U8) hdrInfo.val;*/

      /* Store e2 value */
      pStaPdu->nackInfo[pStaPdu->nackCnt].isSegment = (U8) hdrInfo.val;

      /* Extract e3 : 5GNR */
      /* hdrInfo.len = RLC_E1_LEN; --> previusly stored value (for e1) is
         already present*/
      rlcAmmExtractElmnt(gCb, cntrlPdu, &hdrInfo);
      e3 = (U8) hdrInfo.val;

      /* Extract Reserved Bits after NACK SN */
      hdrInfo.len = resrvdBitsNackSn;
      rlcAmmExtractElmnt(gCb, cntrlPdu, &hdrInfo);

      /* Test for resegmentation */
      if (pStaPdu->nackInfo[pStaPdu->nackCnt].isSegment)
      {
         hdrInfo.len = RLC_SO_LEN_5GNR; /* 5GNR : SO Len 16 Bits */
         rlcAmmExtractElmnt(gCb, cntrlPdu, &hdrInfo);
         pStaPdu->nackInfo[pStaPdu->nackCnt].soStart = hdrInfo.val;

         rlcAmmExtractElmnt(gCb, cntrlPdu, &hdrInfo);
         pStaPdu->nackInfo[pStaPdu->nackCnt].soEnd   = hdrInfo.val;

         RLOG_ARG4(L_DEBUG,DBG_RBID,rbCb->rlcId.rbId,
                       "rlcAmmUlHndlStatusPdu: soStart and soEnd = %d %d"
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
         hdrInfo.len = RLC_NACK_RANGE_LEN;
         rlcAmmExtractElmnt(gCb, cntrlPdu, &hdrInfo);
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
      pStaPdu->ackSn = (pStaPdu->nackInfo[RLC_MAX_NACK_CNT-1].sn + 1) & (rbCb->m.amUl.snModMask);
   }


   /* Parse & send Status PDU to RLC-DL */
   rlcUlUdxStaUpdReq(&(sapCb->pst), sapCb->spId, &rbCb->rlcId, pStaPdu);

   RETVOID;
}

/**
 * @brief Private handler to release all stored segments
 *
 * @details
 *    Private handler invokded by rlcAmmUlPlacePduInRecBuf to release the
 *    stored segements in case a complete PDU is received later.
 *
 * @param[in]  gCb      RLC instance control block
 * @param[in]  recBuf   Buffer that stores a received PDU or segments
 *
 * @return  Void
 *
 */
#ifdef ANSI
PRIVATE Void rlcAmmUlRlsAllSegs
(
RlcCb         *gCb,
RlcAmRecBuf   *recBuf
)
#else
PRIVATE Void rlcAmmUlRlsAllSegs(gCb,recBuf)
RlcCb         *gCb;
RlcAmRecBuf   *recBuf;
#endif
{
   RlcSeg *seg;

   TRC2(rlcAmmUlRlsAllSegs)

   RLC_LLIST_FIRST_SEG(recBuf->segLst, seg);
   while (seg != NULLP)
   {
      RLC_FREE_BUF_WC(seg->seg);
      cmLListDelFrm(&(recBuf->segLst),&(seg->lstEnt));
      RLC_FREE_WC(gCb,seg, sizeof(RlcSeg));
      RLC_LLIST_FIRST_SEG(recBuf->segLst, seg);
   }

   RETVOID;
}

/**
 * @brief Private handler to store the received segment
 *
 * @details
 *    Private handler invokded by rlcAmmUlPlacePduInRecBuf to add a received
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
PRIVATE Bool rlcAmmAddRcvdSeg
(
RlcCb       *gCb,
RlcUlRbCb   *rbCb,
RlcAmHdr    *amHdr,
Buffer     *pdu,
U16        pduSz
)
#else
PRIVATE Bool rlcAmmAddRcvdSeg(gCb, rbCb, amHdr, pdu, pduSz)
RlcCb       *gCb;
RlcUlRbCb   *rbCb;
RlcAmHdr    *amHdr;
Buffer     *pdu;
U16        pduSz;
#endif
{
   RlcAmRecBuf   *recBuf = NULLP;
   RlcSeg        *seg;
   RlcSeg        *tseg;
   U16          soEnd;       /* Holds the SoEnd of received segment */
   U16          expSo = 0;   /* Expected SO */

   TRC2(rlcAmmAddRcvdSeg)

   soEnd = amHdr->so + pduSz - 1;
   recBuf =  rlcUtlGetRecBuf(AMUL.recBufLst, amHdr->sn);

   if (NULLP == recBuf)
   {
      RLC_ALLOC(gCb,recBuf, sizeof(RlcAmRecBuf));
#if (ERRCLASS & ERRCLS_ADD_RES)
      if (recBuf == NULLP)
      {
         RLOG_ARG2(L_FATAL,DBG_RBID,rbCb->rlcId.rbId,
                  "Memory allocation failed UEID:%d CELLID:%d",
                  rbCb->rlcId.ueId,
                  rbCb->rlcId.cellId);

         RLC_FREE_BUF(pdu);
         return (FALSE);
      }
#endif /* ERRCLASS & ERRCLS_RES */
      rlcUtlStoreRecBuf(AMUL.recBufLst, recBuf, amHdr->sn);
   }
   else
   {
      if (recBuf->allRcvd == TRUE)
      {
         RLC_FREE_BUF(pdu);
         return (FALSE);
      }
   }
 			
   recBuf->isDelvUpperLayer = FALSE;
   /* kw003.201 - Move past the segments that are different than the */
   /*             one received.                                      */
   RLC_LLIST_FIRST_SEG(recBuf->segLst, seg);
   while ((seg != NULLP) && (seg->amHdr.so < amHdr->so))
   {
      expSo = seg->amHdr.so + seg->segSz;
      RLC_LLIST_NEXT_SEG(recBuf->segLst, seg);
   }

   /* The received segment should start after the end of previous seg */
   if (expSo > amHdr->so)
   {
      /* This is a duplicate segment */
      gRlcStats.amRlcStats.numRlcAmCellDupPduRx++;
      RLC_FREE_BUF(pdu);
      return (FALSE);
   }

   if ((seg) && (seg->amHdr.so <= soEnd))
   {
      /* This is a duplicate segment */
      gRlcStats.amRlcStats.numRlcAmCellDupPduRx++;
      RLC_FREE_BUF(pdu);
      return (FALSE);
   }

   /* If we have come this far, we have to add this segment to the   */
   /* reception buffer as we either have eliminated duplicates or    */
   /* have found none.                                               */
   RLC_ALLOC_WC(gCb,tseg, sizeof(RlcSeg));
#if (ERRCLASS & ERRCLS_ADD_RES)
   if (tseg == NULLP)
   {
      RLOG_ARG2(L_FATAL,DBG_RBID,rbCb->rlcId.rbId, 
               "Memory allocation failed UEID:%d CELLID:%d",
               rbCb->rlcId.ueId,
               rbCb->rlcId.cellId);
      RLC_FREE_BUF(pdu);
      return (FALSE);
   }
#endif /* ERRCLASS & ERRCLS_RES */

   tseg->seg = pdu;
   tseg->segSz = pduSz;
   RLC_MEM_CPY(&tseg->amHdr, amHdr, sizeof(RlcAmHdr));
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
   rlcAmmUpdExpByteSeg(gCb,&AMUL,tseg);

   return (TRUE);
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
PRIVATE Bool rlcAmmUlPlacePduInRecBuf
(
RlcCb       *gCb,
Buffer     *pdu,
RlcUlRbCb   *rbCb,
RlcAmHdr    *amHdr
)
#else
PRIVATE Bool rlcAmmUlPlacePduInRecBuf(gCb, pdu, rbCb, amHdr)
RlcCb       *gCb;
Buffer     *pdu;
RlcUlRbCb   *rbCb;
RlcAmHdr    *amHdr;
#endif
{
   RlcSn     sn;
   MsgLen   pduSz;
   RlcAmUl   *amUl = &(rbCb->m.amUl);

   TRC2(rlcAmmUlPlacePduInRecBuf)


   sn = amHdr->sn;
   SFndLenMsg(pdu, &pduSz);

   gCb->genSts.bytesRecv += pduSz;
   gRlcStats.amRlcStats.numRlcAmCellSduBytesRx += pduSz; 
   if (!RLC_AM_CHK_SN_WITHIN_RECV_WINDOW(sn, amUl))
   {
      gRlcStats.amRlcStats.numRlcAmCellDropOutWinRx++;
      RLOG_ARG3(L_UNUSED,DBG_RBID,rbCb->rlcId.rbId,
                    "rlcAmmUlPlacePduInRecBuf: SN  %d outside the window"
                    "UEID:%d CELLID:%d",
                    sn,
                    rbCb->rlcId.ueId,
                    rbCb->rlcId.cellId);

      gCb->genSts.unexpPdusRecv++;
      RLC_FREE_BUF(pdu);
      return (FALSE);
   }

   if (amHdr->si == 0)
   {
      RlcAmRecBuf *recBuf = rlcUtlGetRecBuf(amUl->recBufLst, sn);

      /* We received a complete PDU. Either we already have it, in which */
      /* case we just ignore the new PDU and discard it. Otherwise,      */
      /* store the received PDU in the reception buffer                  */
      if (NULLP == recBuf)
      {
         RLC_ALLOC(gCb, recBuf, sizeof(RlcAmRecBuf));
#if (ERRCLASS & ERRCLS_ADD_RES)
         if (recBuf == NULLP)
         {
            RLOG_ARG2(L_FATAL,DBG_RBID,rbCb->rlcId.rbId,
                     "Memory allocation failed UEID:%d CELLID:%d",
                     rbCb->rlcId.ueId,
                     rbCb->rlcId.cellId);
            RLC_FREE_BUF(pdu);
            return (FALSE);
         }
#endif /* ERRCLASS & ERRCLS_RES */
         rlcUtlStoreRecBuf(AMUL.recBufLst, recBuf, sn);
      }
      else if (recBuf->allRcvd != TRUE)
      {
         rlcAmmUlRlsAllSegs(gCb,recBuf);
      }
      else
      {
         gRlcStats.amRlcStats.numRlcAmCellDupPduRx++;
         gCb->genSts.unexpPdusRecv++;
         RLC_FREE_BUF(pdu);
         return (FALSE);
      }
      recBuf->isDelvUpperLayer = FALSE;
      recBuf->pdu = pdu;
      recBuf->pduSz = pduSz;
      recBuf->allRcvd = TRUE;
      gRlcStats.amRlcStats.numRlcAmCellSduRx++;
      RLC_MEM_CPY(&recBuf->amHdr, amHdr, sizeof(RlcAmHdr));
      return (TRUE);
   }
   else
   {
      /* We received a segment. We need to add that to the existing */
      /* segments, if any.                                          */
      return (rlcAmmAddRcvdSeg(gCb,rbCb, amHdr, pdu, pduSz));
   }
}

/**
 * @brief Private handler to trigger status report
 *
 * @details
 *    Private handler invokded by rlcAmmProcessPdus to check if the
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
PRIVATE Void rlcAmmTriggerStatus
(
RlcCb       *gCb,
RlcUlRbCb   *rbCb,
RlcSn       sn,
Bool       discFlg
)
#else
PRIVATE Void rlcAmmTriggerStatus(gCb,rbCb, sn, discFlg)
RlcCb       *gCb;
RlcUlRbCb   *rbCb;
RlcSn       sn;
Bool       discFlg;
#endif
{
   Bool     tmrRunning;
   RlcSn     tSn;
   RlcSn     tVrMr;
   RlcSn     trxHighestStatus;
   RlcAmUl   *amUl = &(rbCb->m.amUl);

   TRC2(rlcAmmTriggerStatus)


   MODAMR(amUl->vrMr, tVrMr, amUl->rxNext, amUl->snModMask);
   MODAMR(amUl->rxHighestStatus, trxHighestStatus, amUl->rxNext, amUl->snModMask);
   MODAMR(sn , tSn, amUl->rxNext, amUl->snModMask);

   /* kw005.201 Product CR ccpu00117114       *
    * The "=" in the 2nd condition is removed */
   if ((discFlg) || (tSn < trxHighestStatus) || (tSn >= tVrMr))
   {
      RLOG_ARG2(L_UNUSED,DBG_RBID,rbCb->rlcId.rbId, 
               "rlcAmmTriggerStatus: Set Status Trigger UEID:%d CELLID:%d",
                     rbCb->rlcId.ueId,
                     rbCb->rlcId.cellId);

      amUl->staTrg = TRUE;
      amUl->gatherStaPduInfo = FALSE;

      /* Check if staProhTmr is running */
      tmrRunning = rlcChkTmr(gCb,(PTR) rbCb, RLC_EVT_AMUL_STA_PROH_TMR);

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
PRIVATE Void rlcAmmProcPduOrSeg
(
RlcCb       *gCb,
RlcUlRbCb   *rbCb,
RlcAmHdr    *amHdr,
Buffer     *pdu
)
#else
PRIVATE Void rlcAmmProcPduOrSeg(gCb, rbCb, amHdr, pdu)
RlcCb       *gCb;
RlcUlRbCb   *rbCb;
RlcAmHdr    *amHdr;
Buffer     *pdu;
#endif
{

   TRC2(rlcAmmProcPduOrSeg)

   if ((AMUL.expSn != amHdr->sn) || (AMUL.expSo != amHdr->so))
   {
      /* Release the existing partial SDU as we have PDUs or */
      /* segments that are out of sequence                   */
      rbCb->m.amUl.isOutOfSeq = TRUE;
      RLC_FREE_BUF(AMUL.partialSdu);
   }

   //if (amHdr->fi & RLC_FI_FIRST_SEG)
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
      rlcUtlSndDatInd(gCb,rbCb, pdu);
   }

   RETVOID;
}

/**
 *
 * @brief Private handler to reassemble SDUs
 *
 * @details
 *    Private handler invokded by rlcAmmProcessPdus with the PDU
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
PRIVATE S16 rlcAmmUlReassembleSdus
(
RlcCb         *gCb,
RlcUlRbCb     *rbCb,
RlcAmRecBuf   *recBuf
)
#else
PRIVATE S16 rlcAmmUlReassembleSdus(gCb, rbCb, recBuf)
RlcCb         *gCb;
RlcUlRbCb     *rbCb;
RlcAmRecBuf   *recBuf;
#endif
{
   RlcSeg        *seg;

   TRC2(rlcAmmUlReassembleSdus)
   //if (recBuf->amHdr.rf == 0)
   if (recBuf->amHdr.si == 0)
   {
      /* This is a PDU */
      rlcAmmProcPduOrSeg(gCb,rbCb, &recBuf->amHdr, recBuf->pdu);
      /* Assign NULLP to recBuf->pdu as this PDU is sent to PDCP */
      recBuf->pdu = NULLP;
      AMUL.expSn = (recBuf->amHdr.sn + 1) & (AMUL.snModMask); /* MOD 1024 */
      AMUL.expSo = 0;
   }
   else
   {
      /* This is a set of segments */
      RLC_LLIST_FIRST_SEG(recBuf->segLst, seg);
      AMUL.expSn = recBuf->amHdr.sn;
      AMUL.expSo = 0;
      while(seg)
      {
         rlcAmmProcPduOrSeg(gCb,rbCb, &seg->amHdr, seg->seg);
         AMUL.expSo = seg->soEnd + 1;

         cmLListDelFrm(&(recBuf->segLst),&(seg->lstEnt));
         RLC_FREE_WC(gCb, seg, sizeof(RlcSeg));

         RLC_LLIST_FIRST_SEG(recBuf->segLst, seg);
      }
      AMUL.expSn = (recBuf->amHdr.sn + 1) & (AMUL.snModMask); /* MOD 1024 */
      AMUL.expSo = 0;
   }

   return ROK;
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
Void rlcAmmUlReEstablish
(
RlcCb         *gCb,
CmLteRlcId   rlcId,
Bool         sendReEst,
RlcUlRbCb     *rbCb
)
#else
Void rlcAmmUlReEstablish(gCb, rlcId, sendReEst, rbCb)
RlcCb         *gCb;
CmLteRlcId   rlcId;
Bool         sendReEst;
RlcUlRbCb     *rbCb;
#endif
{
   RlcSn   sn;
   RlcSn   mSn;
   RlcSn   mVrMr;

#ifndef KW_PDCP
   RlcKwuSapCb *rlcKwuSap;
#endif
   RlcAmRecBuf   *recBuf = NULLP;

   TRC2(rlcAmmUlReEstablish);


   sn = AMUL.rxNext;

   MODAMR(AMUL.vrMr, mVrMr, AMUL.rxNext, AMUL.snModMask);
   MODAMR(sn, mSn, AMUL.rxNext, AMUL.snModMask);

   /* Reassemble SDUs from PDUs with SN less than upper edge of the window */
   while (mSn < mVrMr)
   {
      recBuf = rlcUtlGetRecBuf(AMUL.recBufLst, sn);
      if (NULLP != recBuf)
      {
         if (recBuf->allRcvd == TRUE)
         {
            rlcAmmUlReassembleSdus(gCb,rbCb, recBuf);
         }
         else
         {
            /* Remove PDU and segments */
            if(recBuf->pdu)
            {
               RLC_FREE_BUF_WC(recBuf->pdu);
            }
            /* Release all the segments*/
            rlcAmmUlRlsAllSegs(gCb,recBuf);
         }
         rlcUtlDelRecBuf(AMUL.recBufLst, recBuf, gCb);
      }
      sn = (sn + 1) & (AMUL.snModMask); /* MOD 1024 */
      MODAMR(sn, mSn, AMUL.rxNext, AMUL.snModMask);
   }
   /* Discard remaining PDUs and bytesegments in recBuf */

   /* Stop all timers and reset variables */
   if(TRUE == rlcChkTmr(gCb,(PTR)rbCb,RLC_EVT_AMUL_REORD_TMR))
   {
       rlcStopTmr(gCb,(PTR)rbCb, RLC_EVT_AMUL_REORD_TMR);
   }
   if(TRUE == rlcChkTmr(gCb,(PTR)rbCb,RLC_EVT_AMUL_STA_PROH_TMR))
   {
       rlcStopTmr(gCb,(PTR)rbCb, RLC_EVT_AMUL_STA_PROH_TMR);
   }

   AMUL.rxNext  = 0;
   AMUL.rxNextHighestRcvd  = 0;
   AMUL.rxNextStatusTrig  = 0;
   rbCb->m.amUl.vrMr = (rbCb->m.amUl.rxNext + RLC_AM_GET_WIN_SZ(rbCb->m.amUl.snLen)) & (rbCb->m.amUl.snModMask);
   AMUL.rxHighestStatus = 0;
   AMUL.staTrg  = FALSE;
   AMUL.gatherStaPduInfo = FALSE;
   AMUL.expSn = 0;
   AMUL.expSo = 0;
   if (AMUL.partialSdu != NULLP)
   {
     RLC_FREE_BUF(AMUL.partialSdu);
   }
   rlcKwuSap = gCb->u.ulCb->rlcKwuUlSap + RLC_UI_PDCP;

   if(sendReEst)
   {
      RlcUiKwuReEstCmpInd(&rlcKwuSap->pst, rlcKwuSap->suId, rlcId);
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
Void rlcAmmReOrdTmrExp
(
RlcCb        *gCb,
RlcUlRbCb    *rbCb
)
#else
Void rlcAmmReOrdTmrExp(rbCb)
RlcCb        *gCb;
RlcUlRbCb    *rbCb;
#endif
{
   RlcAmUl *amUl = &(rbCb->m.amUl);
   RlcSn sn;
   RlcSn mSn;
   RlcSn mVrMr;
   RlcSn mrxHighestStatus;
   RlcSn mrxNextHighestRcvd;
   Bool tmrRunning = FALSE;
   RlcAmRecBuf   *recBuf = NULLP;

   TRC2(rlcAmmReOrdTmrExp);


   /* Update rxHighestStatus */
   sn = amUl->rxNextStatusTrig;

   MODAMR(sn, mSn, amUl->rxNext, amUl->snModMask);
   MODAMR(amUl->vrMr, mVrMr, amUl->rxNext, amUl->snModMask);
   recBuf = rlcUtlGetRecBuf(AMUL.recBufLst, sn);

   while (mSn < mVrMr)
   {
      if ((recBuf == NULLP) ||
          ((recBuf != NULLP) && (!recBuf->allRcvd)) )
      {
         amUl->rxHighestStatus = sn;
         amUl->staTrg = TRUE;
         amUl->gatherStaPduInfo = FALSE;

         /* Check if staProhTmr is running */
         tmrRunning = rlcChkTmr(gCb,(PTR) rbCb, RLC_EVT_AMUL_STA_PROH_TMR);

         if (!tmrRunning)
         {
            gRlcStats.amRlcStats.numULReOrdTimerExpires++;
            amUl->gatherStaPduInfo = TRUE;
            rlcAmmUlAssembleCntrlInfo(gCb, rbCb);
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
      rlcStartTmr(gCb,(PTR)rbCb, RLC_EVT_AMUL_REORD_TMR);
      amUl->rxNextStatusTrig = amUl->rxNextHighestRcvd;
   }

   RETVOID;
} /* rlcAmmReOrdTmrExp */

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
Void rlcAmmStaProTmrExp
(
RlcCb        *gCb,
RlcUlRbCb    *rbCb
)
#else
Void rlcAmmStaProTmrExp(gCb, rbCb)
RlcCb        *gCb;
RlcUlRbCb    *rbCb;
#endif
{
   RlcAmUl *amUl = &(rbCb->m.amUl);

   TRC2(rlcAmmStaProTmrExp);


   amUl->gatherStaPduInfo = FALSE;

   if (amUl->staTrg == TRUE)
   {
      amUl->gatherStaPduInfo = TRUE;
      /* kw002.201 : Sending StaRsp after StaProhibit tmr expiry */
      rlcAmmUlAssembleCntrlInfo(gCb,rbCb);
   }

   RETVOID;
} /* rlcAmmStaProTmrExp */

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
PRIVATE Void rlcAmmExtractElmnt
(
RlcCb       *gCb,
Buffer     *pdu,
RlcExtHdr   *hdrInfo
)
#else
PRIVATE Void rlcAmmExtractElmnt(gCb, pdu, hdrInfo)
RlcCb       *gCb;
Buffer     *pdu;
RlcExtHdr   *hdrInfo;
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
PRIVATE Void rlcAmmUpdExpByteSeg
(
RlcCb     *gCb,
RlcAmUl   *amUl,
RlcSeg    *seg
)
#else
PRIVATE Void rlcAmmUpdExpByteSeg(gCb, amUl, seg)
RlcCb     *gCb;
RlcAmUl   *amUl;
RlcSeg    *seg;
#endif
{
   U16    newExpSo; /* The new expected SO */
   RlcSn   sn = seg->amHdr.sn;
   Bool   lstRcvd=FALSE;
   RlcAmRecBuf *recBuf = NULLP;
   
   TRC2(rlcAmmUpdExpByteSeg);


   recBuf = rlcUtlGetRecBuf(amUl->recBufLst, sn);
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
   RLC_LLIST_NEXT_SEG(recBuf->segLst, seg);
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
         RLC_LLIST_NEXT_SEG(recBuf->segLst, seg);
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
Void rlcAmmFreeUlRbCb
(
RlcCb       *gCb,
RlcUlRbCb   *rbCb
)
#else
Void rlcAmmFreeUlRbCb(gCb,rbCb)
RlcCb       *gCb;
RlcUlRbCb   *rbCb;
#endif
{
   RlcSn         curSn = 0;           /* Sequence number of PDU */
   RlcSn         windSz;              /* PDU window size */
   RlcAmRecBuf *recBuf = NULLP;

   TRC2(rlcAmmFreeUlRbCb)


   windSz  =  (RLC_AM_GET_WIN_SZ(rbCb->m.amUl.snLen)) << 1;

   if(TRUE == rlcChkTmr(gCb,(PTR)rbCb,RLC_EVT_AMUL_REORD_TMR))
   {
      rlcStopTmr(gCb,(PTR)rbCb, RLC_EVT_AMUL_REORD_TMR);
   }
   if(TRUE == rlcChkTmr(gCb,(PTR)rbCb,RLC_EVT_AMUL_STA_PROH_TMR))
   {
      rlcStopTmr(gCb,(PTR)rbCb, RLC_EVT_AMUL_STA_PROH_TMR);
   }


   /* on the first loop winSz is always greater than zero
    while( ( curSn < windSz ) hence changing to do while */
   do
   {
      recBuf = rlcUtlGetRecBuf(rbCb->m.amUl.recBufLst, curSn);
      if ( recBuf != NULLP )
      {
         if (recBuf->pdu != NULLP)
         {
            RLC_FREE_BUF_WC(recBuf->pdu);
         }
         /* Release all the segments */
         rlcAmmUlRlsAllSegs(gCb,recBuf);
         rlcUtlDelRecBuf(rbCb->m.amUl.recBufLst, recBuf, gCb);
      }
      curSn++;
   }while ( curSn < windSz );

#ifndef LTE_TDD 
      RLC_FREE_WC(gCb,rbCb->m.amUl.recBufLst, (RLC_RCV_BUF_BIN_SIZE * sizeof(CmLListCp)));
      rbCb->m.amUl.recBufLst = NULLP;
#endif

   if(rbCb->m.amUl.partialSdu != NULLP)
   {
      RLC_FREE_BUF_WC(rbCb->m.amUl.partialSdu);
   }
   RETVOID;
} /* rlcAmmFreeUlRbCb */


/*@}*/


/********************************************************************30**

         End of file
**********************************************************************/
