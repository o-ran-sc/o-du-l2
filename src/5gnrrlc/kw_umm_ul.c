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
  
     Desc:     Source code for RLC Unacknowledged mode assembly and
               reassembly.This file contains following functions

                  --rlcUmmQSdu
                  --rlcUmmProcessSdus
                  --rlcUmmProcessPdus
                  --rlcUmmReAssembleSdus
                  --kwUmmReEstablish 

     File:     kw_umm_ul.c

**********************************************************************/
/** 
 * @file kw_umm_ul.c
 * @brief RLC Unacknowledged Mode uplink module
*/

/* header (.h) include files */
#include "common_def.h"
#include "ckw.h"                /* RRC layer */
#include "lkw.h"                /* RRC layer */
#include "kwu.h"                /* RLC service user */
#include "lkw.h"                /* LM Interface */
#include "rgu.h"                /* MAC layer */
#include "kw_env.h"             /* RLC environment options */

#include "kw.h"                 /* RLC layer */
#include "kw_err.h"
#include "kw_ul.h"


/* header/extern include files (.x) */

#include "ckw.x"                /* RRC layer */
#include "kwu.x"                /* RLC service user */
#include "lkw.x"                /* LM Interface */
#include "rgu.x"                /* MAC later */

#include "kw.x"                 /* RLC layer */
#include "kw_ul.x"

#define RLC_MODULE (RLC_DBGMASK_UM | RLC_DBGMASK_UL)

static uint8_t rlcUmmExtractHdr ARGS ((RlcCb *gCb, 
                                   RlcUlRbCb *rbCb,
                                   Buffer *pdu,
                                   RlcUmHdr *umHdr));

static void rlcUmmReAssembleSdus ARGS ((RlcCb *gCb,
                                        RlcUlRbCb *rbCb,
                                        RlcUmRecBuf *umRecBuf));

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
 *    the SN of the first UMD PDU with SN >= _nextSn that has not been received
 *
 * @param[in] umUl      pointer to Um mode uplink control block
 * @param[in] nextSn    Sequence number after which the VR(UR) is to set to
 *
 * @return  Void
*/ 
static void rlcUmmFindNextVRUR (RlcUmUl* umUl, RlcSn nextSn)
{
   RlcSn ur = RLC_UM_GET_VALUE(umUl->vrUr, *umUl);
   
   RlcSn nextSnToCompare = RLC_UM_GET_VALUE(nextSn,*umUl);
   
   while (ur < nextSnToCompare)
   {
      if (!(umUl->recBuf[nextSn])) /* if the buffer is empty, SN not received */
      {
         umUl->vrUr = nextSn;
         break;
      }
      nextSn = (nextSn + 1) & umUl->modBitMask; 
      nextSnToCompare = RLC_UM_GET_VALUE(nextSn,*umUl);
   }
}

/**
 * @brief  Checks whether a sequence number is within the 
 *         re-ordering window or not
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
static int16_t rlcUmmCheckSnInReordWindow (RlcSn sn, const RlcUmUl* const umUl)  
{
   return (RLC_UM_GET_VALUE(sn, *umUl) < RLC_UM_GET_VALUE(umUl->vrUh, *umUl)); 
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
   RlcUmRecBuf   **recBuf;   /* UM Reception Buffer */

   bool         tmrRunning;   /* Boolean for checking Tmr */

   count = 0;

   /* pduCount should be the min of RGU_MAX_PDU and pduInfo->numPdu */
   pduCount = (pduInfo->numPdu < RGU_MAX_PDU)? pduInfo->numPdu : RGU_MAX_PDU;
   
   vrUh   = &(rbCb->m.umUl.vrUh);
   vrUr   = &(rbCb->m.umUl.vrUr);
   vrUx   = &(rbCb->m.umUl.vrUx);
   recBuf = (rbCb->m.umUl.recBuf);

   while (count < pduCount)
   {
      RlcSn   ur; 
      RlcSn   uh; 
      RlcSn   seqNum;
      Buffer *pdu = pduInfo->mBuf[count];
      RlcUmRecBuf *tmpRecBuf;
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
      /* create a buffer to be later inserted into the reception buffer */
      RLC_ALLOC_WC(gCb, tmpRecBuf, sizeof(RlcUmRecBuf));
#if (ERRCLASS & ERRCLS_ADD_RES)
      if (tmpRecBuf == NULLP)
      {   
         DU_LOG("\nRLC : rlcUmmProcessPdus: Memory allocation failed UEID:%d CELLID:%d",\
            rbCb->rlcId.ueId, rbCb->rlcId.cellId);
         ODU_PUT_MSG_BUF(pdu);
	 return;
      }
#endif /* ERRCLASS & ERRCLS_ADD_RES */      
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
        RLC_FREE_BUF(pdu);
        RLC_FREE_WC(gCb, tmpRecBuf, sizeof(RlcUmRecBuf));
        /*Fix for CR ccpu00144030: If threshhold is hit then also count
         *should be incrmented */
        count++;
        continue;
     }
#endif     
#endif
#endif
#endif
      /* get the pdu header */
      if (rlcUmmExtractHdr(gCb, rbCb, pdu, &(tmpRecBuf->umHdr)))  
      {
         DU_LOG("\nRLC : rlcUmmProcessPdus: Header Extraction Failed UEID:%d CELLID:%d",\
             rbCb->rlcId.ueId, rbCb->rlcId.cellId);

         /* Header extraction is a problem. 
          * log an error and free the allocated memory */
         /* ccpu00136940 */
         RLC_FREE_WC(gCb, tmpRecBuf, sizeof(RlcUmRecBuf));
         ODU_PUT_MSG_BUF(pdu);
         count++;
         /* kw005.201 ccpu00117318, updating the statistics */
         gCb->genSts.errorPdusRecv++;
         continue;
      }
      curSn = tmpRecBuf->umHdr.sn;

      /* Check if the PDU should be discarded or not */
      ur = RLC_UM_GET_VALUE(RLC_UMUL.vrUr, RLC_UMUL); 
      uh = RLC_UM_GET_VALUE(RLC_UMUL.vrUh, RLC_UMUL); 
      seqNum = RLC_UM_GET_VALUE(curSn, RLC_UMUL); 

      if (((ur < seqNum) && (seqNum < uh) && (RLC_UMUL.recBuf[curSn])) || 
          (seqNum < ur)) 
      {
         /* PDU needs to be discarded */
         DU_LOG("\nRLC : rlcUmmProcessPdus: Received a duplicate pdu with sn %d \
	    UEID:%d CELLID:%d", curSn, rbCb->rlcId.ueId, rbCb->rlcId.cellId);

         RLC_FREE_BUF(pdu);
         RLC_FREE_WC(gCb, tmpRecBuf, sizeof(RlcUmRecBuf));
         count++;
         /* kw005.201 ccpu00117318, updating the statistics */
         gCb->genSts.unexpPdusRecv++;
         continue;
      } 

      /* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS

      /* kw006.201 ccpu00120058, reduced code complexity by adding new function */
      rlcUtlCalUlIpThrPut(gCb,rbCb, pdu, ttiCnt);

#endif 

      recBuf[curSn] = tmpRecBuf;

      recBuf[curSn]->pdu = pdu;
      ODU_GET_MSG_LEN(pdu,&(recBuf[curSn]->pduSz));
      /* kw005.201 ccpu00117318, updating the statistics */
      gCb->genSts.bytesRecv += recBuf[curSn]->pduSz;
      
      if (!rlcUmmCheckSnInReordWindow(curSn,&RLC_UMUL))
      {  /* currSn is outside re-ordering window */
         *vrUh  = (curSn + 1) & RLC_UMUL.modBitMask;

         /* re-assemble all pdus outside the modified re-ordering window */
         /* the first SN is VR(UR) */
         if (!rlcUmmCheckSnInReordWindow(*vrUr,&RLC_UMUL))
         {
            /* TODO : should it be VR(UR) + 1 ?... check, earlier it was so */
            RlcSn sn = *vrUr; /* SN's which need to be re-assembled */
            RlcSn lowerEdge;  /* to hold the lower-edge of the 
                                re-ordering window */

            /* The new value ov VR(UR) is the lower end of the window i
             * and SN's still this value need to be re-assembled */
            
            *vrUr = (*vrUh - RLC_UMUL.umWinSz) &  RLC_UMUL.modBitMask;         
            lowerEdge = RLC_UM_GET_VALUE(*vrUr ,RLC_UMUL);
            
            while (RLC_UM_GET_VALUE(sn, RLC_UMUL) < lowerEdge)
            {
               if (recBuf[sn])
               {
                  rlcUmmReAssembleSdus(gCb,rbCb,recBuf[sn]);
                  RLC_FREE_WC(gCb,recBuf[sn],sizeof(RlcUmRecBuf));
                  recBuf[sn] = NULLP;
               }
               sn = (sn + 1) & RLC_UMUL.modBitMask;
            }
         }
      }
      if (recBuf[*vrUr])
      {
         RlcSn sn       = *vrUr;
         RlcSn tSn      = RLC_UM_GET_VALUE(sn,RLC_UMUL); 
         RlcSn tVrUr;       

         /* set VR(UR) to next SN > current VR(UR) which is not received */
         RlcSn nextVrUr = (*vrUr + 1) & RLC_UMUL.modBitMask;
         rlcUmmFindNextVRUR(&RLC_UMUL, nextVrUr);

         /* re-assemble SDUs with SN < Vr(UR) */
         tVrUr = RLC_UM_GET_VALUE(*vrUr,RLC_UMUL);
         while (recBuf[sn] && tSn < tVrUr)
         {
            rlcUmmReAssembleSdus(gCb,rbCb,recBuf[sn]);
            RLC_FREE_WC(gCb,recBuf[sn],sizeof(RlcUmRecBuf));
            recBuf[sn] = NULLP;
            sn = (sn + 1) & RLC_UMUL.modBitMask;
            tSn = RLC_UM_GET_VALUE(sn, RLC_UMUL);
         }
      }

      tmrRunning = rlcChkTmr(gCb,(PTR)rbCb, RLC_EVT_UMUL_REORD_TMR);

      if (tmrRunning) 
      {
         RlcSn tVrUx = RLC_UM_GET_VALUE(*vrUx, RLC_UMUL);
         RlcSn tVrUr = RLC_UM_GET_VALUE(*vrUr ,RLC_UMUL);

         RlcSn tVrUh = RLC_UM_GET_VALUE(*vrUh, RLC_UMUL);

         S16 ret = rlcUmmCheckSnInReordWindow(*vrUx, &RLC_UMUL);

         if ( (tVrUx <= tVrUr) || ((!ret) && (tVrUx != tVrUh))) 
         {
            rlcStopTmr(gCb,(PTR)rbCb,RLC_EVT_UMUL_REORD_TMR);
            tmrRunning = FALSE;
         }
      }

      if (!tmrRunning)
      {
         if (RLC_UM_GET_VALUE(*vrUh, RLC_UMUL) > RLC_UM_GET_VALUE(*vrUr, RLC_UMUL))
         {
            rlcStartTmr(gCb,(PTR)rbCb,RLC_EVT_UMUL_REORD_TMR); 
            *vrUx = *vrUh;
         }
      }
      count++;
   }/* end while count < pduCount */
#ifdef LTE_L2_MEAS
   rlcUtlCalUlIpThrPutIncTTI(gCb, rbCb,ttiCnt);
#endif /* LTE_L2_MEAS */
   return;   
}

/**
 * @brief  Handler to reassemble the SDUs and send them to the upper layer.
 *       
 * @details
 *    This function processes the received in-sequence PDU and
 *    re-assembles the SDUs and sends them to the upper layer.
 *
 * @param[in] gCb        RLC Instance control block
 * @param[in] rbCb       RB control block
 * @param[in] umRecBuf   Reception Buffer to be Re-Assembled 
 *
 * @return  Void
*/
static void rlcUmmReAssembleSdus(RlcCb *gCb, RlcUlRbCb *rbCb, RlcUmRecBuf *umRecBuf)
{
   uint32_t  liCount;        /* LI count */
   uint32_t  count;          /* Loop counter */
   uint8_t   fi;             /* Framing Info */
   uint16_t  sn;             /* Sequence Number of current PDU */
   MsgLen    len;            /* PDU Length */
   Buffer    *sdu;           /* SDU to be sent to upper layer */
   Buffer    *remPdu;        /* Remaining PDU */
   Buffer    **partialSdu;   /* Partial SDU */

   liCount = umRecBuf->umHdr.numLi;
   fi = umRecBuf->umHdr.fi;
   sn =  umRecBuf->umHdr.sn;
 
   for (count = 0; (count <= liCount);count++)
   {
      if (count < liCount )
         len = umRecBuf->umHdr.li[count];
      else
      {
         if (!(umRecBuf->pdu))
         {
            return;
         }
         ODU_GET_MSG_LEN(umRecBuf->pdu,&len);
      }
         
      /* get the sdu out of the pdu */
      ODU_SEGMENT_MSG(umRecBuf->pdu,len,&remPdu);
      sdu = umRecBuf->pdu;
      umRecBuf->pdu = remPdu;
      
      partialSdu = &(rbCb->m.umUl.partialSdu);
      /* While re-assembling the SDUs, consider the first LI and perform 
       * the following steps.
          -# If the first bit of FI(Framing Info of 2  bits) is set => 
                -# The current Data field in the PDU is a segment. 
                   So form a SDU only if the 
                       rbCb->m.um.umUl.partialSdu exists and the SNs are 
                       in-sequence.
                -# If there are no LIs and the second bit of LI is 1 
                   then a partial SDU is formed which would not be sent 
                   to the upper layer.
          -# else
                -# If rbCb->partialSdu is not NULL then flush it off.
                -# If LI count > 0 or LI count is 0 and second bit 
                   of FI is not 1 
                         The SDU is complete.So send it to upper layer.
                -# else
                         The SDU is partial and is placed 
                         in rbCb->m.um.umUl.partialSdu;
      */
             
      if (0 == count )
      {
         if (fi & 2)
         {
            if ((*partialSdu) && 
                (sn == ((rbCb->m.umUl.sn + 1) & rbCb->m.umUl.modBitMask)))
            {
               ODU_CAT_MSG(*partialSdu,sdu,M1M2);
               RLC_FREE_BUF(sdu);
               if (liCount > 0 || !(fi & 1))
               {
                  rlcUtlSendUlDataToDu(gCb,rbCb,*partialSdu);
                  *partialSdu = NULLP;
               }
            }
            else
            {
                /* Partial Sdu stored is not valid now.So free it */
               if (*partialSdu)
               {
                  RLC_FREE_BUF(*partialSdu);
                  *partialSdu = NULLP;
               }
                  
               RLC_FREE_BUF(sdu);
               sdu = NULLP;
            }
         }
         else
         {
            if (*partialSdu)
            {
               RLC_FREE_BUF(*partialSdu);  /* RLC mem leak fix */
               *partialSdu = NULLP;
            }
            
            if (liCount > 0 || !( fi & 1))
            {
               rlcUtlSendUlDataToDu(gCb,rbCb,sdu);
            }
            else
            {
                  *partialSdu = sdu; 
            }
         }
      }
      /*
          If the SDU pointer has the last Data field of the PDU
            -# If FI is 1,place the SDU in rbCb->m.um.umDl.partialSdu
            -# else send the SDU to upper layer.
      */
       else if (count == liCount)
      {
         if (fi & 1)
         {
            *partialSdu = sdu;
         }
         else
         {
            rlcUtlSendUlDataToDu(gCb, rbCb, sdu);
         }
      }
      /*  
         If the LI is something other than the first one, 
         just send the SDU to the upper layer */
      else
      {
         rlcUtlSendUlDataToDu(gCb, rbCb, sdu);
      }
   }
   rbCb->m.umUl.sn = sn;

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
 *         Call rlcUmmReAssembleSdus() for each PDU with SN < VR(UH)
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
   RlcUmRecBuf   **recBuf;   /* UM Reception Buffer */
   RlcKwuSapCb   *rlcKwSap;   /* KWU SAP Information */

   curSn = rbCb->m.umUl.vrUr;
   vrUh  = RLC_UM_GET_VALUE(rbCb->m.umUl.vrUh,rbCb->m.umUl);
   recBuf =  rbCb->m.umUl.recBuf;

   if(TRUE == rlcChkTmr(gCb,(PTR)rbCb,RLC_EVT_UMUL_REORD_TMR))
   {
       rlcStopTmr(gCb,(PTR)rbCb,RLC_EVT_UMUL_REORD_TMR);
   }
   
   while (RLC_UM_GET_VALUE(curSn,rbCb->m.umUl) < vrUh)
   {
      if ( recBuf[curSn] != NULLP )
      {
         rlcUmmReAssembleSdus(gCb,rbCb,recBuf[curSn]);
         RLC_FREE_WC(gCb,recBuf[curSn],sizeof(RlcUmRecBuf));
         recBuf[curSn] = NULLP;
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
 *    along with the PDU buffer.The sequence number,framing info 
 *    and LIs are extracted by this function.
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
   uint8_t   e;         /* Extension Bit */
   Data      dst[2];    /* Destination Buffer */
   int32_t   totalSz;   /* Sum of LIs */
   MsgLen    pduSz;     /* PDU size */
#if (ERRCLASS & ERRCLS_DEBUG)
   uint8_t   ret;       /* Return Value */
#endif

   ODU_GET_MSG_LEN(pdu,&pduSz);
 
   if ( rbCb->m.umUl.snLen == 1)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      ret = ODU_REM_PRE_MSG(dst,pdu);
      if (ret != ROK)
      {
         DU_LOG("\nRLC : rlcUmmExtractHdr : ODU_REM_PRE_MSG Failed for 5 bit SN \
	    UEID:%d CELLID:%d", rbCb->rlcId.ueId, rbCb->rlcId.cellId);
         return RFAILED;
      }
#else
      ODU_REM_PRE_MSG(dst,pdu);
#endif
      pduSz--;
      umHdr->sn = (dst[0]) & 0x1F; 
      umHdr->fi = (dst[0]) >> 6;
      e       = (dst[0]>>5) & 0x01;
   }
   else
   {
      /* snLen - sequnce length will be 10 bits requiring 2 bytes */ 
#if (ERRCLASS & ERRCLS_DEBUG)
      ret = ODU_REM_PRE_MSG_MULT(dst,2,pdu);
      if (ret != ROK)
      {
         DU_LOG("\nRLC : rlcUmmExtractHdr : ODU_REM_PRE_MSG_MULT Failed for 10 bits SN \
	    UEID:%d CELLID:%d", rbCb->rlcId.ueId, rbCb->rlcId.cellId);
         return RFAILED;
      }
#else
      ODU_REM_PRE_MSG_MULT(dst,2,pdu);
#endif
      pduSz -= 2;
   
      /* kw005.201 R9 Upgrade 3gpp spec 36.322 ver9.3.0 CR0082      *
       * Removed the "if" condition for checking the reserved field *
       * Added mask 0x03 for extracting the FI field.          */

      umHdr->fi = ( (dst[0] ) >> 3) & 0x03;
      e       = ( (dst[0] ) >> 2) & 0x01;
      umHdr->sn = (  dst[0] & 0x03) << 8;
      umHdr->sn  |= dst[1];
   }

   umHdr->numLi = 0;
   
   totalSz = 0;
   while(e && umHdr->numLi < RLC_MAX_UL_LI )
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      ret = ODU_REM_PRE_MSG_MULT(dst,2,pdu);
      if (ret != ROK)
      {
         DU_LOG("\nRLC : rlcUmmExtractHdr : ODU_REM_PRE_MSG_MULT Failed UEID:%d CELLID:%d",\
            rbCb->rlcId.ueId, rbCb->rlcId.cellId);
         return RFAILED;
      }
#else
      ODU_REM_PRE_MSG_MULT(dst,2,pdu);
#endif
      umHdr->li[umHdr->numLi] = ((dst[0]) & 0x7F) << 4;
      umHdr->li[umHdr->numLi] |= dst[1] >> 4;
      if ( 0 == umHdr->li[umHdr->numLi] )
      {
         DU_LOG("\nRLC : rlcUmmExtractHdr : Received LI as 0 UEID:%d CELLID:%d",
            rbCb->rlcId.ueId, rbCb->rlcId.cellId);
         return RFAILED; 
      }
      totalSz += umHdr->li[umHdr->numLi];
      if ( pduSz <=  totalSz )
      {
         DU_LOG("\nRLC : rlcUmmExtractHdr : SN [%d]: UEID:%d CELLID:%d",\
            umHdr->sn, rbCb->rlcId.ueId, rbCb->rlcId.cellId);
         DU_LOG("\nRLC : rlcUmmExtractHdr : Corrupted PDU as TotSz[%d] PduSz[%d] \
	    UEID:%d CELLID:%d ", totalSz, pduSz, rbCb->rlcId.ueId, rbCb->rlcId.cellId);
         return RFAILED; /* the situation where in the PDU size 
                            is something that does not match with 
                            the size in LIs*/
      }
      umHdr->numLi++;
      pduSz -= 2;

      e = ((dst[0]) & 0x80) >> 7;
   
      if ( e && umHdr->numLi < RLC_MAX_UL_LI)
      {
         uint8_t tmp = ((dst[1]) & 0x08) >> 3;
         umHdr->li[umHdr->numLi] = ( dst[1] & 0x07) << 8;


#if (ERRCLASS & ERRCLS_DEBUG)
         ret = ODU_REM_PRE_MSG(dst,pdu);
         if (ret != ROK)
         {
            DU_LOG("\nRLC : rlcUmmExtractHdr : ODU_REM_PRE_MSG Failed UEID:%d CELLID:%d",
               rbCb->rlcId.ueId, rbCb->rlcId.cellId);
            return RFAILED;
         }
#else
         ODU_REM_PRE_MSG(dst,pdu);
#endif
         umHdr->li[umHdr->numLi] |= ( dst[0] );    /* The first byte lies in 
                                                   the first 8 bits.We want 
                                                   them in the last 8 bits */
         if ( 0 == umHdr->li[umHdr->numLi] )
         {
            DU_LOG("\nRLC : rlcUmmExtractHdr :Received LI as 0 UEID:%d CELLID:%d",
               rbCb->rlcId.ueId, rbCb->rlcId.cellId);
            return RFAILED; 
         }
         totalSz += umHdr->li[umHdr->numLi];
         pduSz--;
         umHdr->numLi++;

         if (pduSz < totalSz)
         {
            return RFAILED; /* the situation where in the PDU size is 
                               something that does not match with the 
                               size in LIs*/
         }

         e = tmp;
      }
   } /* while(e && umHdr->numLi < RLC_MAX_LI ) */
   if (e)
   {
      /* PDU was constructed with LIs that exceeded RLC_MAX_LI */
      return RFAILED;
   }
   return ROK; 
}
   
/**
 * @brief Handles expiry of re-ordering timer
 *
 * @param[in] gCb     RLC Instance control block
 * @param[in] rbCb    Rb Control block for which re-order timer expired
 *
 * @return  Void
*/
Void rlcUmmReOrdTmrExp
(
RlcCb       *gCb,
RlcUlRbCb   *rbCb     
)
{
   RlcSn prevVrUr;   /* prevVrUr */
   prevVrUr = RLC_UMUL.vrUr;

   /* set VR(UR) to SN >= VR(UX) that has not been received */
   rlcUmmFindNextVRUR(&RLC_UMUL, RLC_UMUL.vrUx);

   while (RLC_UM_GET_VALUE(prevVrUr,RLC_UMUL) < 
          RLC_UM_GET_VALUE(RLC_UMUL.vrUr,RLC_UMUL))
   {
      if (RLC_UMUL.recBuf[prevVrUr])
      {
         rlcUmmReAssembleSdus(gCb, rbCb, RLC_UMUL.recBuf[prevVrUr]);
         if(RLC_UMUL.recBuf[prevVrUr]->pdu != NULLP) /* RLC mem leak fix */
         {
            RLC_FREE_BUF(RLC_UMUL.recBuf[prevVrUr]->pdu);
         }
         RLC_FREE_WC(gCb, RLC_UMUL.recBuf[prevVrUr], sizeof(RlcUmRecBuf));
         RLC_UMUL.recBuf[prevVrUr] = NULLP;
      }

      prevVrUr = (prevVrUr + 1) & rbCb->m.umUl.modBitMask;
   }

   if (RLC_UM_GET_VALUE(RLC_UMUL.vrUh, RLC_UMUL) > 
       RLC_UM_GET_VALUE(RLC_UMUL.vrUr, RLC_UMUL))
   {
      rlcStartTmr(gCb,(PTR)rbCb,RLC_EVT_UMUL_REORD_TMR);
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
   RlcSn         curSn = 0;           /* sequence number of PDU */
   RlcSn         windSz;              /* PDU window size */
   RlcUmRecBuf   **umRecBuf;          /* UM module receive buffer */

   windSz  = rbCb->m.umUl.umWinSz << 1;

   umRecBuf =  rbCb->m.umUl.recBuf;

   if(TRUE == rlcChkTmr(gCb,(PTR)rbCb,RLC_EVT_UMUL_REORD_TMR))
   {
      rlcStopTmr(gCb,(PTR)rbCb,RLC_EVT_UMUL_REORD_TMR);
   }
   while (curSn < windSz)
   {
      if (umRecBuf[curSn] != NULLP)
      {
         RLC_FREE_BUF_WC(umRecBuf[curSn]->pdu);
         umRecBuf[curSn]->pdu = NULLP;

         RLC_FREE_WC(gCb, umRecBuf[curSn], sizeof(RlcUmRecBuf));
         umRecBuf[curSn] = NULLP;
      }
      curSn++;
   }
   RLC_FREE_WC(gCb,rbCb->m.umUl.recBuf, (windSz ) * sizeof(RlcUmRecBuf*));
   rbCb->m.umUl.recBuf = NULLP;
   return;
} 


/********************************************************************30**
         End of file
**********************************************************************/
