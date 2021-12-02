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

     Name:     NR RLC Layer 
  
     Type:     C file
  
     Desc:     Source code for RLC Unacknowledged mode assembly and
               reassembly.This file contains following functions

                  --rlcUmmQSdu
                  --rlcUmmProcessSdus
                  --rlcUmmProcessPdus
                  --rlcUmUlReAssembleSdus
                  --kwUmmReEstablish 

     File:     rlc_umm_ul.c

**********************************************************************/
/** 
 * @file rlc_umm_ul.c
 * @brief RLC Unacknowledged Mode uplink module
*/

/* header (.h) include files */
#include "common_def.h"
#include "ckw.h"                /* RRC layer */
#include "lkw.h"                /* RRC layer */
#include "kwu.h"                /* RLC service user */
#include "lkw.h"                /* LM Interface */
#include "rgu.h"                /* MAC layer */
#include "rlc_env.h"             /* RLC environment options */

/* header/extern include files (.x) */

#include "ckw.x"                /* RRC layer */
#include "kwu.x"                /* RLC service user */
#include "lkw.x"                /* LM Interface */
#include "rgu.x"                /* MAC later */

#include "rlc_utils.h"                 /* RLC layer */
#include "rlc_err.h"
#include "rlc_ul.h"

#define RLC_MODULE (RLC_DBGMASK_UM | RLC_DBGMASK_UL)

static uint8_t rlcUmmExtractHdr ARGS ((RlcCb *gCb, 
                                   RlcUlRbCb *rbCb,
                                   Buffer *pdu,
                                   RlcUmHdr *umHdr));

uint8_t rlcUmmReAssembleSdus ARGS ((RlcCb *gCb,
                                    RlcUlRbCb *rbCb,
                                    RlcUmRecBuf *umRecBuf));

bool rlcUmmAddRcvdSeg ARGS ((RlcCb *gCb,
                             RlcUlRbCb   *rbCb,
                             RlcUmHdr    *umHdr,
                             Buffer      *pdu,
                             uint16_t    pduSz));

void rlcUmmRelAllSegs(RlcCb *gCb, RlcUmRecBuf *recBuf);

#ifndef TENB_ACC
#ifndef LTE_PAL_ENB
uint32_t isMemThreshReached(Region region);
#endif
#endif
/**
 * @brief  Finds and sets the next VR(UR) depending on the 
 *         passed sequence number
 *       
 * @details
 *    Finds the next VR(UR) depending on the passed SN. Updates VR(UR) to 
 *    the SN of the first UMD PDU with SN >= _nextSn that has not been reassembled
 *
 * @param[in] umUl      pointer to Um mode uplink control block
 * @param[in] nextSn    Sequence number after which the VR(UR) is to set to
 *
 * @return  Void
*/ 
static void rlcUmmFindRxNextReassembly (RlcCb *gCb, RlcUmUl* umUl, RlcSn nextSn)
{
   RlcSn ur = RLC_UM_GET_VALUE(umUl->vrUr, *umUl);
   RlcSn prevRxNextReassembly = umUl->vrUr;

   RlcSn nextSnToCompare = RLC_UM_GET_VALUE(nextSn,*umUl);
   RlcUmRecBuf *recBuf = rlcUtlGetUmRecBuf(umUl->recBufLst,nextSn);
   RlcUmRecBuf *prevRecBuf = rlcUtlGetUmRecBuf(umUl->recBufLst,prevRxNextReassembly);
   while (ur < nextSnToCompare)
   {
      if((ur + 1) == nextSnToCompare)
      {
         if ((recBuf == NULLP) || (!(recBuf->allSegRcvd))) /* if the buffer is empty, SN not received */
         {
            umUl->vrUr = nextSn;
            if(prevRecBuf)
            {
               rlcUmmRelAllSegs(gCb, prevRecBuf);
               rlcUtlDelUmRecBuf(gCb, umUl->recBufLst, prevRecBuf);
            }
            break;
         }
         else
         {
            if(prevRecBuf)
            {
               rlcUmmRelAllSegs(gCb, prevRecBuf);
               rlcUtlDelUmRecBuf(gCb, umUl->recBufLst, prevRecBuf);
            }
         }
         prevRecBuf = rlcUtlGetUmRecBuf(umUl->recBufLst,nextSn);
         nextSn = (nextSn + 1) & umUl->modBitMask;
         nextSnToCompare = RLC_UM_GET_VALUE(nextSn,*umUl);
         recBuf = rlcUtlGetUmRecBuf(umUl->recBufLst,nextSn);
      }
      else
      {
         umUl->vrUr++;
         ur = RLC_UM_GET_VALUE(umUl->vrUr, *umUl);
         if(prevRecBuf)
         {
            rlcUmmRelAllSegs(gCb, prevRecBuf);
            rlcUtlDelUmRecBuf(gCb, umUl->recBufLst, prevRecBuf);
         }
         prevRecBuf = rlcUtlGetUmRecBuf(umUl->recBufLst,umUl->vrUr);
      }
   }
}

/**
 * @brief  Checks whether a sequence number is within the 
 *         re-assembly window or not
 *       
 * @param[in] sn        Sequence Number to be checked
 * @param[in] umUl      pointer to Um mode uplink control block
 *
 * @return  S16
 *      -# TRUE 
 *      -# FALSE
 *
 * @return  Void
*/
static int16_t rlcUmmCheckSnInReassemblyWindow (RlcSn sn, const RlcUmUl* const umUl)  
{
   return (RLC_UM_GET_VALUE(sn, *umUl) < RLC_UM_GET_VALUE(umUl->vrUh, *umUl)); 
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
 * @param[in]  umUl  AM Uplink Control Block
 * @param[in]  seg   Newly received segment
 *
 * @return void
 *
 */

void rlcUmmUpdExpByteSeg(RlcCb *gCb, RlcUmUl *umUl, RlcUmSeg *seg)
{
   uint16_t  newExpSo; /* The new expected SO */
   RlcSn     sn = seg->umHdr.sn;
   bool      lstRcvd=FALSE;
   RlcUmRecBuf *recBuf = NULLP;
   
   recBuf = rlcUtlGetUmRecBuf(umUl->recBufLst, sn);
   if ((recBuf == NULLP) || (recBuf && (seg->umHdr.so != recBuf->expSo)))
   {
      return;
   }
   recBuf->noMissingSeg = FALSE;
   newExpSo   = seg->soEnd + 1;
   recBuf->expSo = newExpSo;
   if(seg->umHdr.si == RLC_SI_LAST_SEG)
   {  
      lstRcvd = TRUE;
   } 
   RLC_UMM_LLIST_NEXT_SEG(recBuf->segLst, seg);
   while(seg)
   {
      /* keep going ahead as long as the expectedSo match with the header so
         else store the expSo for later checking again */
      if(seg->umHdr.si == RLC_SI_LAST_SEG)
      {  
         lstRcvd = TRUE;
      } 
      if (seg->umHdr.so == newExpSo)
      {
         newExpSo = seg->soEnd + 1;
         recBuf->expSo = newExpSo;
         RLC_UMM_LLIST_NEXT_SEG(recBuf->segLst, seg);
      }
      else
      {
         recBuf->expSo = newExpSo;
         return;
      }
   }
   if (lstRcvd == TRUE)
   {
      recBuf->allSegRcvd = TRUE;
   }
   recBuf->noMissingSeg = TRUE;

   return;
}
/**
 * @brief Private handler to store the received segment
 *
 * @details
 *    Private handler invokded by rlcUmmUlPlacePduInRecBuf to add a received
 *    segment in reception buffer of a RBCB.
 *    - It is responsible for detecting duplicate segments
 *    - Adding it at appropriate position in the received buffer
 *    - Calling ExpByteSeg to set expSo field in the receiver buffer
 *
 * @param[in]  gCb      RLC instance control block
 * @param[in]  rbCb     Radio Bearer Contro Block
 * @param[in]  umHdr    UM Header received
 * @param[in]  pdu      Buffer received other than the headers
 * @param[in]  pduSz    size of the PDU buffer received
 *
 * @return  bool
 *   -#TRUE  Successful insertion into the receiver buffer
 *   -#FALSE Possibly a duplicate segment
 */

bool rlcUmmAddRcvdSeg(RlcCb *gCb, RlcUlRbCb *rbCb, RlcUmHdr *umHdr, Buffer *pdu, uint16_t pduSz)
{
   RlcUmRecBuf   *recBuf = NULLP;
   RlcUmSeg      *seg;
   RlcUmSeg      *tseg;
   uint16_t      soEnd;       /* Holds the SoEnd of received segment */
   uint16_t      expSo = 0;   /* Expected SO */

   soEnd = umHdr->so + pduSz - 1;
   recBuf =  rlcUtlGetUmRecBuf(RLC_UMUL.recBufLst, umHdr->sn);

   if (NULLP == recBuf)
   {
      RLC_ALLOC(gCb,recBuf, sizeof(RlcUmRecBuf));
      if (recBuf == NULLP)
      {
         DU_LOG("\nERROR  -->  RLC_UL : Failed to allocate memory to recv buf for UEID:%d CELLID:%d in rlcUmmAddRcvdSeg()",
            rbCb->rlcId.ueId, rbCb->rlcId.cellId);

         ODU_PUT_MSG_BUF(pdu);
         return FALSE;
      }
      rlcUtlStoreUmRecBuf(RLC_UMUL.recBufLst, recBuf, umHdr->sn);
   }
   else
   {
      if (recBuf->allSegRcvd == TRUE)
      {
         ODU_PUT_MSG_BUF(pdu);
         return FALSE;
      }
   }
 			
   RLC_UMM_LLIST_FIRST_SEG(recBuf->segLst, seg);
   while ((seg != NULLP) && (seg->umHdr.so < umHdr->so))
   {
      expSo = seg->umHdr.so + seg->segSz;
      RLC_UMM_LLIST_NEXT_SEG(recBuf->segLst, seg);
   }

   if (expSo > umHdr->so)
   {
      DU_LOG("\nDEBUG  -->  RLC_UL : Received duplicate segment in rlcUmmAddRcvdSeg()");
      /* This is a duplicate segment */
      ODU_PUT_MSG_BUF(pdu);
      return FALSE;
   }

   if ((seg) && (seg->umHdr.so <= soEnd))
   {
      DU_LOG("\nDEBUG  -->  RLC_UL : Received duplicate segment in rlcUmmAddRcvdSeg()");
      /* This is a duplicate segment */
      ODU_PUT_MSG_BUF(pdu);
      return FALSE;
   }

   /* If we have come this far, we have to add this segment to the   */
   /* reception buffer as we either have eliminated duplicates or    */
   /* have found none.                                               */
   RLC_ALLOC_WC(gCb, tseg, sizeof(RlcUmSeg));
   if (tseg == NULLP)
   {
      DU_LOG("\nERROR -->  RLC_UL : Failed to allocate memory to segment for UEID:%d CELLID:%d in rlcUmmAddRcvdSeg()",
         rbCb->rlcId.ueId, rbCb->rlcId.cellId);
      ODU_PUT_MSG_BUF(pdu);
      return FALSE;
   }

   tseg->seg = pdu;
   tseg->segSz = pduSz;
   RLC_MEM_CPY(&tseg->umHdr, umHdr, sizeof(RlcUmHdr));
   RLC_MEM_CPY(&recBuf->umHdr, umHdr, sizeof(RlcUmHdr));
   recBuf->sn = umHdr->sn;
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
   rlcUmmUpdExpByteSeg(gCb, &RLC_UMUL, tseg);
   return TRUE;
}

/**
 * @brief Private handler to release all stored segments
 *
 * @details
 *    Private handler invokded by rlcUmmRelAllSegs to release the
 *    stored segements in case a complete PDU is received later.
 *
 * @param[in]  gCb      RLC instance control block
 * @param[in]  recBuf   Buffer that stores a received PDU or segments
 *
 * @return  void
 *
 */
void rlcUmmRelAllSegs(RlcCb *gCb, RlcUmRecBuf *recBuf)
{
   RlcUmSeg *seg;

   RLC_UMM_LLIST_FIRST_SEG(recBuf->segLst, seg);
   while (seg != NULLP)
   {
      ODU_PUT_MSG_BUF(seg->seg);
      cmLListDelFrm(&(recBuf->segLst), &(seg->lstEnt));
      RLC_FREE(gCb, seg, sizeof(RlcUmSeg));
      RLC_UMM_LLIST_FIRST_SEG(recBuf->segLst, seg);
   }

   return;
}

/**
 * @brief Private handler to reassemble from a segment or a PDU
 *
 * @details
 *    Private handler invokded by rlcUmmReAssembleSdus with either a
 *    PDU or a segment of a PDU. This is also called in the case of
 *    reestablishment and hence out of sequence joining is also to
 *    be supported
 *
 *
 * @param[in]  gCb     RLC instance control block
 * @param[in]  rbCb    Uplink RB control block
 * @param[in]  umHdr   UM header received for this segment/PDU
 * @param[in]  pdu     PDU to be reassembled
 *
 * @return  ROK/RFILED
 *
 */

uint8_t rlcUmmProcSeg(RlcCb *gCb, RlcUlRbCb *rbCb, RlcUmHdr *umHdr, Buffer *pdu)
{

   if ((RLC_UMUL.expSn != umHdr->sn) || (RLC_UMUL.expSo != umHdr->so))
   {
      /* Release the existing SDU as we have PDUs or */
      /* segments that are out of sequence           */
      DU_LOG("\nDEBUG  -->  RLC_UL : Received Segments are out of sequence in rlcUmmProcSeg()");
      ODU_PUT_MSG_BUF(RLC_UMUL.assembleSdu);
      return RFAILED;
   }

   if (umHdr->si == RLC_SI_FIRST_SEG)
   {/* first Segment of the SDU */
      if (RLC_UMUL.assembleSdu != NULLP)
      { /* Some old SDU may be present */
         ODU_PUT_MSG_BUF(RLC_UMUL.assembleSdu);
      }
      RLC_UMUL.assembleSdu = pdu;
      pdu = NULLP;
   }
   else if(umHdr->si == RLC_SI_MID_SEG)
   {/* Middle Segment of the SDU */
      ODU_CAT_MSG(RLC_UMUL.assembleSdu,pdu, M1M2);
      ODU_PUT_MSG_BUF(pdu);
      pdu = NULLP;
   }
   else if(umHdr->si ==  RLC_SI_LAST_SEG)
   {
      ODU_CAT_MSG(pdu, RLC_UMUL.assembleSdu, M2M1);
      ODU_PUT_MSG_BUF(RLC_UMUL.assembleSdu);
   }

   if (pdu != NULLP)
   {
      RLC_UMUL.assembleSdu = NULLP;
      rlcUtlSendUlDataToDu(gCb,rbCb, pdu);
   }

   return ROK;
}
/**
 *
 * @brief Private handler to reassemble SDUs
 *
 * @details
 *    Private handler invokded by rlcUmmProcessPdus with the PDU
 *    from the reception buffer in sequence to reassemble SDUs and
 *    send it to PDCP.
 *
 *        - With the stored header info, FI and LSF segment / concatenate
 *          PDUs or byte segments of PDUs to get the associated SDU.
 *
 * @param[in]  rbCb     RB control block
 * @param[in]  pdu      PDU to be reassembled
 *
 *  @return  uint8_t
 *      -# ROK
 *      -# RFAILED
 *
 */
uint8_t rlcUmmReAssembleSdus(RlcCb *gCb, RlcUlRbCb *rbCb, RlcUmRecBuf *recBuf)
{
   RlcUmSeg        *seg;

   /* This is a set of segments */
   RLC_UMM_LLIST_FIRST_SEG(recBuf->segLst, seg);
   RLC_UMUL.expSn = recBuf->sn;
   RLC_UMUL.expSo = 0;
   while(seg)
   {
      if(rlcUmmProcSeg(gCb, rbCb, &seg->umHdr, seg->seg) == RFAILED)
      {
         rlcUmmRelAllSegs(gCb, recBuf);
	 break;
      }
      RLC_UMUL.expSo = seg->soEnd + 1;
      cmLListDelFrm(&(recBuf->segLst),&(seg->lstEnt));
      RLC_FREE(gCb, seg, sizeof(RlcSeg));
      RLC_UMM_LLIST_FIRST_SEG(recBuf->segLst, seg);
   }
   RLC_UMUL.expSn = (recBuf->umHdr.sn + 1) & (RLC_UMUL.modBitMask);
   RLC_UMUL.expSo = 0;
   return ROK;
}

/**
 * @brief  Handler to process the Data Indication from the lower layer 
 *         and send the PDUs to re-assembly unit. 
 *       
 * @details
 *    This function processes the PDUs received from the lower layer
 *    re-orders them and sends them one after the other in sequence 
 *    to the re-assembly unit.
 *
 * @param[in] gCb      RLC Instance control block
 * @param[in] rbCb     RB control block 
 * @param[in] pduInfo  Pdu information 
 *
 *  @return Void
*/
/* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
void rlcUmmProcessPdus(RlcCb *gCb, RlcUlRbCb  *rbCb, KwPduInfo *pduInfo, uint32_t ttiCnt)
#else
void rlcUmmProcessPdus(RlcCb *gCb, RlcUlRbCb *rbCb, KwPduInfo *pduInfo)
#endif
{
   RlcSn         *vrUh;      /* vr(uh) */
   RlcSn         *vrUr;      /* vr(ur) */
   RlcSn         *vrUx;      /* vr(ux) */
   uint16_t      curSn;      /* Current Sequence Number */
   uint32_t      pduCount;   /* PDU count */
   uint32_t      count;      /* Loop counter */
   RlcUmRecBuf   *recBuf;    /* UM Reception Buffer */
   RlcUmHdr      umHdr;      /* Um header*/
   bool          tmrRunning; /* Boolean for checking Tmr */
   MsgLen        pduSz;      /* Pdu Size */
   RlcSn         tRxNextReassembly;
   RlcSn         tRxNextReassemblyNxt;
   RlcSn         tRxNextHighest;
   RlcTptPerSnssai *snssaiTputNode = NULLP;

   count = 0;

   /* pduCount should be the min of RGU_MAX_PDU and pduInfo->numPdu */
   pduCount = (pduInfo->numPdu < RGU_MAX_PDU)? pduInfo->numPdu : RGU_MAX_PDU;

   vrUh   = &(rbCb->m.umUl.vrUh);
   vrUr   = &(rbCb->m.umUl.vrUr);
   vrUx   = &(rbCb->m.umUl.vrUx);

   while (count < pduCount)
   {
      RlcSn   ur; 
      RlcSn   seqNum;
      Buffer  *pdu = pduInfo->mBuf[count];

      pduSz = 0;  /*re-intialize the size*/
      snssaiTputNode = NULLP;

      gCb->genSts.pdusRecv++;
#ifndef RGL_SPECIFIC_CHANGES
#ifndef TENB_ACC
#ifndef LTE_PAL_ENB
      uint32_t ulrate_rgu;
      MsgLen len;
      ODU_GET_MSG_LEN(pdu, &len);
      ulrate_rgu += len;
#endif
#endif      
#endif      
      /* ccpu00142274 - UL memory based flow control*/ 
#ifndef RGL_SPECIFIC_CHANGES
#ifndef TENB_ACC
#ifndef LTE_PAL_ENB
      /* Changed the condition to TRUE from ROK  */
#ifndef XEON_SPECIFIC_CHANGES    
      if(isMemThreshReached(rlcCb[0]->init.region) == TRUE)
      {
         uint32_t rlculdrop;
         rlculdrop++;
         ODU_PUT_MSG_BUF(pdu);
         count++;
         continue;
      }
#endif     
#endif
#endif
#endif
      /* get the pdu header */
      if (rlcUmmExtractHdr(gCb, rbCb, pdu, &umHdr))  
      {
         DU_LOG("\nERROR  --> RLC_UL: rlcUmmProcessPdus: Header Extraction Failed UEID:%d CELLID:%d",\
               rbCb->rlcId.ueId, rbCb->rlcId.cellId);
         ODU_PUT_MSG_BUF(pdu);
         count++;
         gCb->genSts.errorPdusRecv++;
         continue;
      }

      if(rbCb->snssai)
      {
         snssaiTputNode = rlcHandleSnssaiTputlist(gCb, rbCb->snssai, SEARCH, DIR_UL);
         if(snssaiTputNode != NULLP)
         {
            ODU_GET_MSG_LEN(pdu, &pduSz);
            snssaiTputNode->dataVol += (uint64_t)pduSz;
            DU_LOG("\nINFO  -->  RLC_UL: UMM_UL SNSSAI List PduLen:%d, lcId:%d, total :%ld",\
                  pduSz, rbCb->lch.lChId, snssaiTputNode->dataVol);
         }
      }

      /* Check if the PDU should be delivered to upper layer */
      if(umHdr.si == 0)
      {
         rlcUtlSendUlDataToDu(gCb, rbCb, pdu);
         ODU_PUT_MSG_BUF(pdu);
         count++;
         continue;
      }

      curSn = umHdr.sn;

      /* Check if the PDU should be discarded or not */
      ur = RLC_UM_GET_VALUE(RLC_UMUL.vrUr, RLC_UMUL); 
      seqNum = RLC_UM_GET_VALUE(curSn, RLC_UMUL); 

      if (seqNum < ur)
      {
         /* PDU needs to be discarded */
         DU_LOG("\nINFO  -->  RLC_UL: rlcUmmProcessPdus: Received an unexpected pdu with sn %d \
               UEID:%d CELLID:%d", curSn, rbCb->rlcId.ueId, rbCb->rlcId.cellId);

         ODU_PUT_MSG_BUF(pdu);
         count++;
         gCb->genSts.unexpPdusRecv++;
         continue;
      } 

#ifdef LTE_L2_MEAS
      rlcUtlCalUlIpThrPut(gCb,rbCb, pdu, ttiCnt);
#endif 

      ODU_GET_MSG_LEN(pdu, &pduSz);
      /* Place sdu segment into recption buffer */
      if(rlcUmmAddRcvdSeg(gCb, rbCb, &umHdr, pdu, pduSz) == TRUE)      
      {
         recBuf = rlcUtlGetUmRecBuf(RLC_UMUL.recBufLst, umHdr.sn);
         if (recBuf == NULLP)
         {
            DU_LOG("\nERROR  -->  RLC_UL: rlcUmmProcessPdus: recBuf is NULLP UEID:%d CELLID:%d", \
                  rbCb->rlcId.ueId, rbCb->rlcId.cellId);
            ODU_PUT_MSG_BUF(pdu);
            count++;
            continue;
         }

         /* If all bytes segments of curSn are received, deliver assembled SDU to upper layer */
         if(recBuf != NULLP && recBuf->allSegRcvd)
         {
            rlcUmmReAssembleSdus(gCb, rbCb, recBuf);
            DU_LOG("\nDEBUG  -->  RLC_UL: rlcUmmProcessPdus: Assembled the Sdus for  sn = %d UEID:%d CELLID:%d",\
                  umHdr.sn, rbCb->rlcId.ueId, rbCb->rlcId.cellId);

            /* if curSn is same as the RX_NEXT_Reassembly */
            if (seqNum == ur)
            {
               /* set RX_NEXT_Reassembly to next SN > current RX_NEXT_Reassembly which is not reassembled yet */
               RlcSn nextVrUr = (*vrUr + 1) & RLC_UMUL.modBitMask;
               rlcUmmFindRxNextReassembly(gCb, &RLC_UMUL, nextVrUr);
            }
         }
         /* If curSn is outside re-assembly window */
         else if (!rlcUmmCheckSnInReassemblyWindow(curSn,&RLC_UMUL))
         {  
            DU_LOG("\nDEBUG  -->  RLC_UL: rlcUmmProcessPdus: curent sn is outSide the re-Assembly window. \
                  sn = %d UEID:%d CELLID:%d", umHdr.sn, rbCb->rlcId.ueId, rbCb->rlcId.cellId);

            /* update RX_NEXT_Highest */
            *vrUh  = (curSn + 1) & RLC_UMUL.modBitMask;

            /* Discard all pdus outside the modified re-assembly window */
            if (!rlcUmmCheckSnInReassemblyWindow(*vrUr,&RLC_UMUL))
            {
               RlcSn sn = *vrUr; /* Stores SNs which need to be discarded. First SN is VR(UR) */
               RlcSn lowerEdge;  /* to hold the lower-edge of the re-assembly window */
               RlcSn packetCount;

               /* Set RX_NEXT_Reassembly to next SN >= (RX_NEXT_Highest - windowSize) that has not been reassembled yet */
               *vrUr = (*vrUh - RLC_UMUL.umWinSz) &  RLC_UMUL.modBitMask;
               lowerEdge = *vrUr;
               packetCount = (lowerEdge - sn) & RLC_UMUL.modBitMask;

               while (packetCount)
               {
                  recBuf = rlcUtlGetUmRecBuf(RLC_UMUL.recBufLst, sn);
                  if (recBuf)
                  {
                     rlcUmmRelAllSegs(gCb, recBuf);
                     rlcUtlDelUmRecBuf(gCb, RLC_UMUL.recBufLst, recBuf);
                  }
                  sn = (sn + 1) & RLC_UMUL.modBitMask;
                  packetCount--;
               }
               recBuf = rlcUtlGetUmRecBuf(RLC_UMUL.recBufLst, *vrUr);
               if (recBuf != NULLP && recBuf->allSegRcvd)
               {
                  /* set rxNextReassembly to next SN > current rxNextReassembly which is not received */
                  RlcSn nextRxNextReassembly = (*vrUr + 1) & RLC_UMUL.modBitMask;
                  rlcUmmFindRxNextReassembly(gCb ,&RLC_UMUL, nextRxNextReassembly);
               }
            }
         }

         tmrRunning = rlcChkTmr(gCb,(PTR)rbCb, EVENT_RLC_UMUL_REASSEMBLE_TMR);
         tRxNextReassembly = RLC_UM_GET_VALUE(*vrUr ,RLC_UMUL);
         tRxNextReassemblyNxt = (*vrUr + 1) & rbCb->m.umUl.modBitMask;
         tRxNextHighest = RLC_UM_GET_VALUE(*vrUh, RLC_UMUL);
         tRxNextReassemblyNxt = RLC_UM_GET_VALUE(tRxNextReassemblyNxt ,RLC_UMUL);

         /* If reassemby timer is running */
         if (tmrRunning) 
         {
            RlcSn  tRxTimerTigger = RLC_UM_GET_VALUE(*vrUx, RLC_UMUL);
            uint8_t ret = rlcUmmCheckSnInReassemblyWindow(*vrUx, &RLC_UMUL);
            recBuf = rlcUtlGetUmRecBuf(RLC_UMUL.recBufLst,*vrUr);

            if ((tRxTimerTigger <= tRxNextReassembly) || ((!ret) && (tRxTimerTigger != tRxNextHighest)) ||
                  (tRxNextHighest ==  tRxNextReassemblyNxt && recBuf && recBuf->noMissingSeg))
            {
               rlcStopTmr(gCb,(PTR)rbCb, EVENT_RLC_UMUL_REASSEMBLE_TMR);             
               tmrRunning = FALSE;
               DU_LOG("\nINFO  --> RLC_UL: rlcUmmProcessPdus: Stopped ReAssembly Timer rxTimerTigger = %d \
                     rxNextReassembly = %d rxNextHighest = %d ", tRxTimerTigger, tRxNextReassembly, tRxNextHighest);
            }
         }

         /* If Reassembly timer is not running */
         if (!tmrRunning)
         {
            recBuf = rlcUtlGetUmRecBuf(RLC_UMUL.recBufLst, curSn);
            if ((tRxNextHighest > tRxNextReassemblyNxt) || ((tRxNextHighest == tRxNextReassemblyNxt)
                     && (recBuf && (!recBuf->noMissingSeg))))
            {
               DU_LOG("\nDEBUG  -->  RLC_UL: rlcUmmProcessPdus: Start ReAssembly Timer tRxNextReassemblyNxt = %d \
                     tRxNextHighest %d", tRxNextReassemblyNxt, tRxNextHighest);
               rlcStartTmr(gCb, (PTR)rbCb, EVENT_RLC_UMUL_REASSEMBLE_TMR); 
               *vrUx = *vrUh;
            }
         }
      }
      else
      {
         DU_LOG("\nERROR  -->  RLC_UL: rlcUmmProcessPdus:Failed to assemble the PDU for SN  = %d UEID:%d CELLID:%d",\
               umHdr.sn, rbCb->rlcId.ueId, rbCb->rlcId.cellId);

      }
      count++;
   }/* end while count < pduCount */
#ifdef LTE_L2_MEAS
   rlcUtlCalUlIpThrPutIncTTI(gCb, rbCb,ttiCnt);
#endif /* LTE_L2_MEAS */
   return;   
}

/**
 * @brief  Handler to process the re-establishment request received 
 *         from the upper layer.
 *       
 * @details
 *    This function does the following functions : 
 *       - If direction of the RB is downlink : 
 *         Remove all the SDUs in the SDU queue.
 *       - If direction of the RB is uplink   : 
 *         Call rlcUmUlReAssembleSdus() for each PDU with SN < VR(UH)
 *
 * @param[in] gCb        RLC Instance control block
 * @param[in] rlcID      Identity of the RLC entity for which 
 *                       re-establishment is to be done
 * @param[in] rbCb       RB control block for which re-establishment 
 *                       is to be done
 *
 * @return  Void
*/ 
Void rlcUmmUlReEstablish
(
RlcCb         *gCb,
CmLteRlcId   *rlcId,
RlcUlRbCb     *rbCb
)
{
   RlcSn         curSn;
   RlcSn         vrUh;
   RlcUmRecBuf   *recBuf;   /* UM Reception Buffer */
   RlcKwuSapCb   *rlcKwSap;   /* KWU SAP Information */

   curSn = rbCb->m.umUl.vrUr;
   vrUh  = RLC_UM_GET_VALUE(rbCb->m.umUl.vrUh,rbCb->m.umUl);

   if(TRUE == rlcChkTmr(gCb,(PTR)rbCb,EVENT_RLC_UMUL_REASSEMBLE_TMR))
   {
       rlcStopTmr(gCb,(PTR)rbCb,EVENT_RLC_UMUL_REASSEMBLE_TMR);
   }
   
   while (RLC_UM_GET_VALUE(curSn, rbCb->m.umUl) < vrUh)
   {
      recBuf = rlcUtlGetUmRecBuf(RLC_UMUL.recBufLst, curSn);
      if ( recBuf != NULLP )
      {
         if(recBuf->allRcvd == TRUE)
	 {
           rlcUmmReAssembleSdus(gCb,rbCb,recBuf);
	 }
	 else
	 {
	    /* Remove PDU and segments */
	    if(recBuf->pdu)
	    {
	       ODU_PUT_MSG_BUF(recBuf->pdu);
	    }
            /* Release all the segments*/
	    rlcUmmRelAllSegs(gCb,recBuf);
         }
	 rlcUtlDelUmRecBuf(gCb, RLC_UMUL.recBufLst, recBuf);
      } 
      curSn = (curSn + 1) & rbCb->m.umUl.modBitMask;
   }
   rbCb->m.umUl.vrUr = 0;
   rbCb->m.umUl.vrUh = 0;
   rbCb->m.umUl.vrUx = 0;

   rlcKwSap = gCb->u.ulCb->rlcKwuUlSap + RLC_UI_PDCP;

   /* In the UM Mode always send reestablish-indication to Upper Latyer*/
   RlcUiKwuReEstCmpInd(&rlcKwSap->pst, rlcKwSap->suId, *rlcId);

   return;
}

/**
 * @brief  Handler to extract the header from a PDU
 *       
 * @details
 *    This function is used to extract the header of a PDU and store it 
 *    along with the PDU buffer.The sequence number, segmentation info 
 *    and segmentation offset are extracted by this function.
 *
 * @param[in] gCb      RLC Instance control block
 * @param[in] rbCb     Rb Control block for which the pdu is received
 * @param[in] pdu      PDU buffer
 * @param[out] umHdr   UM header to be filled after extraction
 *
 * @return  S16
 *      -# TRUE 
 *      -# FALSE
*/
static uint8_t rlcUmmExtractHdr(RlcCb *gCb, RlcUlRbCb *rbCb, Buffer *pdu, RlcUmHdr *umHdr)
{
   Data      dst[2];    /* Destination Buffer */
   uint8_t   ret;       /* Return Value */

   memset(umHdr, 0, sizeof(RlcUmHdr));

   ret = ODU_REM_PRE_MSG(dst,pdu);
   if (ret != ROK)
   {
      DU_LOG("\nERROR  -->  RLC_UL: rlcUmmExtractHdr : ODU_REM_PRE_MSG Failed for SI\
        UEID:%d CELLID:%d", rbCb->rlcId.ueId, rbCb->rlcId.cellId);
      return RFAILED;
   }
   umHdr->si = (dst[0]) >> 6;

   /* If SI = 0, the RLC PDU contains complete RLC SDU. Header extraction complete. 
    * No other fields present in header */
   if(umHdr->si == 0)
      return ROK;

   /* If SI != 0, one SDU segment is present in RLC PDU. Hence extracting SN */
   if (rbCb->m.umUl.snLen == RLC_UM_CFG_6BIT_SN_LEN)
   {
      /* Extractin 6-bit SN */
      umHdr->sn = (dst[0]) & 0x3F; 
   }
   else
   {
      /* Extracting 12 bits SN */ 
      umHdr->sn = (dst[0]) & 0x0F;
      ret = ODU_REM_PRE_MSG(dst,pdu);
      if (ret != ROK)
      {
	 DU_LOG("\nERROR  -->  RLC_UL: rlcUmmExtractHdr : ODU_REM_PRE_MSG Failed for SN\
	       UEID:%d CELLID:%d", rbCb->rlcId.ueId, rbCb->rlcId.cellId);
	 return RFAILED;
      }
      umHdr->sn = (umHdr->sn << 8) | dst[0];
   }

   /* SO field is present for middle and last segments of SDU, not present for first segment */
   if((umHdr->si == RLC_SI_LAST_SEG) || (umHdr->si == RLC_SI_MID_SEG))
   {
      ret = ODU_REM_PRE_MSG_MULT(dst,2,pdu);
      if (ret != ROK)
      {
         DU_LOG("\nERROR  -->  RLC_UL: rlcUmmExtractHdr : ODU_REM_PRE_MSG_MULT Failed for 16 bits SO \
            UEID:%d CELLID:%d", rbCb->rlcId.ueId, rbCb->rlcId.cellId);
         return RFAILED;
      }
      umHdr->so = dst[0];
      umHdr->so = (umHdr->so << 8) | dst[1];
   }
   return ROK; 
}
   
/**
 * @brief Handles expiry of re-assembly timer
 *
 * @param[in] gCb     RLC Instance control block
 * @param[in] rbCb    Rb Control block for which re-assembly timer expired
 *
 * @return  Void
*/
void rlcUmmReAsmblTmrExp
(
RlcCb       *gCb,
RlcUlRbCb   *rbCb     
)
{
   RlcSn tRxNextHighest;
   RlcSn tRxNextReassembly;
   RlcUmRecBuf *recBuf;

   DU_LOG("\nINFO  -->  RLC_UL: rlcUmmReAsmblTmrExp: UM Re-assembly timer expired");

   /* set VR(UR) to SN >= VR(UX) that has not been reassembled */
   rlcUmmFindRxNextReassembly(gCb, &RLC_UMUL, RLC_UMUL.vrUx);

   tRxNextHighest = RLC_UM_GET_VALUE(RLC_UMUL.vrUh, RLC_UMUL);
   tRxNextReassembly = (RLC_UMUL.vrUr + 1) & rbCb->m.umUl.modBitMask;
   tRxNextReassembly = RLC_UM_GET_VALUE(tRxNextReassembly, RLC_UMUL);
   recBuf = rlcUtlGetUmRecBuf(RLC_UMUL.recBufLst,RLC_UMUL.vrUr);
   if ((tRxNextHighest > tRxNextReassembly) || ((tRxNextHighest == tRxNextReassembly) &&
            ((recBuf) && !(recBuf->noMissingSeg))))
   {
      rlcStartTmr(gCb, (PTR)rbCb, EVENT_RLC_UMUL_REASSEMBLE_TMR);
      RLC_UMUL.vrUx = RLC_UMUL.vrUh;
   }
}

/**
 * @brief
 *   Function to release/free the UnAcknowledged Mode Module  RbCb buffers
 *
 * @details
 *   This primitive Frees the UM RbCb transmission Buffer, retransmission
 *   Buffer and reciption Buffers
 *
 * @param [in]   gCb   - RLC instance Control Block
 * @param [in]   rbCb  - RB Control Block
 *
 * @return   void
 */

Void rlcUmmFreeUlRbCb
(
RlcCb       *gCb,
RlcUlRbCb   *rbCb
)
{
   RlcSn         curSn = 0;           /* Sequence number of PDU */
   RlcUmRecBuf   *recBuf = NULLP;

   if(TRUE == rlcChkTmr(gCb,(PTR)rbCb,EVENT_RLC_UMUL_REASSEMBLE_TMR))
   {
      rlcStopTmr(gCb,(PTR)rbCb, EVENT_RLC_UMUL_REASSEMBLE_TMR);
   }

   do
   {
      recBuf = rlcUtlGetUmRecBuf(rbCb->m.umUl.recBufLst, curSn);
      if ( recBuf != NULLP )
      {
         if (recBuf->pdu != NULLP)
         {
            ODU_PUT_MSG_BUF(recBuf->pdu);
         }
         /* Release all the segments */
         rlcUmmRelAllSegs(gCb,recBuf);
         rlcUtlDelUmRecBuf(gCb, rbCb->m.umUl.recBufLst, recBuf);
      }
      curSn++;
   }while ( curSn < RLC_RCV_BUF_BIN_SIZE);

   RLC_FREE(gCb,rbCb->snssai, sizeof (Snssai));
   RLC_FREE(gCb,rbCb->m.umUl.recBufLst, (RLC_RCV_BUF_BIN_SIZE * sizeof(CmLListCp)));
   rbCb->m.umUl.recBufLst = NULLP;

   if(rbCb->m.umUl.assembleSdu != NULLP)
   {
      ODU_PUT_MSG_BUF(rbCb->m.umUl.assembleSdu);
   }
   return;
} /* rlcAmmFreeUlRbCb */

/********************************************************************30**
         End of file
**********************************************************************/
