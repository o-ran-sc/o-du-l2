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
                  --rlcUmmReAssembleSdus
                  --kwUmmReEstablish 

     File:     rlc_umm_dl.c

**********************************************************************/
/** 
 * @file rlc_umm_dl.c
 * @brief RLC Unacknowledged Mode downlink module
*/

/* header (.h) include files */
#include "common_def.h"
#include "ckw.h"                /* RRC layer */
#include "lkw.h"                /* RRC layer */
#include "kwu.h"                /* RLC service user */
#include "lkw.h"                /* LM Interface */
#include "rgu.h"                /* MAC layer */
#include "rlc_env.h"             /* RLC environment options */
#include "rlc_err.h"




/* header/extern include files (.x) */

#include "ckw.x"                /* RRC layer */
#include "kwu.x"                /* RLC service user */
#include "lkw.x"                /* LM Interface */
#include "rgu.x"                /* MAC later */

#include "rlc_utils.h"                 /* RLC layer */
#include "rlc_dl_ul_inf.h"
#include "rlc_dl.h"

#define RLC_MODULE (RLC_DBGMASK_UM | RLC_DBGMASK_DL)

/* variables for logging :declared in BRDCM cl */
#ifndef TENB_ACC
uint32_t dldrops_kwu_um;
uint32_t dlpkt_um;
uint32_t buffer_occ;
uint32_t dlrate_kwu;
#endif

static Void rlcUmmCreatePdu ARGS ((RlcCb *gCb,
                                  RlcDlRbCb *rbCb, 
                                  Buffer *pdu,
                                  RlcUmHdr *umHdr,
                                  KwPduInfo *datReqPduInfo));

/** @addtogroup ummode */
/*@{*/

/**
 * @brief  Handler to queue a SDU in the SDU queue, update BO and report 
 *         it to the lower layer.
 *       
 * @details
 *    This function is used to queue the received SDU in the 
 *    SDU queue maintained in the radio bearer control block.
 *    After queuing the SDU, BO is updated and is reported
 *    to the lower layer. 
 *
 * @param[in] gCb      RLC Instance control block
 * @param[in] rbCb     RB control block 
 * @param[in] datReq   Ptr to the datReq sent from PDCP
 * @param[in] mBuf     Sdu data
 *
 * @return  Void
*/  
void rlcUmmQSdu(RlcCb *gCb, RlcDlRbCb *rbCb, RlcDatReqInfo *datReq, Buffer *mBuf)
{
   MsgLen   len;    /* SDU buffer length */
   RlcSdu    *sdu;   /* SDU */

   RLC_UPD_L2_DL_TOT_SDU_STS(gCb,rbCb);

   RLC_ALLOC_WC(gCb, sdu, (Size)sizeof(RlcSdu));
#if (ERRCLASS & ERRCLS_ADD_RES)
   if ( sdu == NULLP )
   {
      DU_LOG("\nERROR  -->  RLC DL : Memory allocation failed in rlcUmmQSdu for UEID:%d CELLID:%d",\
               rbCb->rlcId.ueId,
               rbCb->rlcId.cellId);
      ODU_PUT_MSG_BUF(mBuf);
      return;
   }
#endif /* ERRCLASS & ERRCLS_ADD_RES */

/* Discard new changes starts */
   rlcUtlGetCurrTime(&sdu->arrTime);
/* Discard new changes ends */
   ODU_GET_MSG_LEN(mBuf,&len);

   sdu->mBuf = mBuf;
   sdu->sduSz = len;
   sdu->actSz = len;
   sdu->mode.um.sduId = datReq->sduId;
   sdu->mode.um.isSegmented = FALSE;
#ifndef RGL_SPECIFIC_CHANGES
#ifndef TENB_ACC
#ifndef LTE_PAL_ENB
   {
         dlrate_kwu += len;
   }
#endif   
#endif
#endif
   rbCb->m.umDl.bo += len;
   rbCb->m.umDl.bo += RLC_MAX_HDRSZ;
   cmLListAdd2Tail(&(rbCb->m.umDl.sduQ), &sdu->lstEnt);
   sdu->lstEnt.node = (PTR)sdu;

   if(!rlcDlUtlIsReestInProgress(rbCb))
   {
      rlcUtlSendDedLcBoStatus(gCb, rbCb, rbCb->m.umDl.bo, 0, FALSE,0);
   }
   
   /* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS_RLC
   /* Update numActUe if it is not active */
   if((rbCb->rbL2Cb.measOn & LKW_L2MEAS_ACT_UE) &&
      (rbCb->ueCb->numActRb[rbCb->qci]++ == 0))
   {
     rlcCb.rlcL2Cb.numActUe[rbCb->qci]++;
   }
#endif

   return;    
}


/**
 * @brief   Handler to form PDU(s) and update the BO. 
 *       
 * @details
 *     -# This function forms pdu(s) from the SDU(s) in the
 *        SDU queue and returns them.
 *     -# This function also updates the BO along with the 
 *        along with the estimated Header size.
 *
 * @param[in]  rbCb     RB control block 
 * @param[out] pduInfo  Pdu Info to be filled and the PDU size to be 
 *                      formed and the updated BO
 * @param[in]  pduSz    The size for which PDUs have to constructed
 *
 * @return  S16
 *    -# ROK       In case of success
 *    -# RFAILED   If allocation of Sdu fails
*/  
void rlcUmmProcessSdus(RlcCb *gCb, RlcDlRbCb *rbCb, RlcDatReq *datReq)
{
   CmLList     *firstNode;   /* First Node in SDU queue */
   Buffer      *pdu;         /* Buffer for holding the formed PDU */
   KwPduInfo   *pduInfo;     /* PDU Info pointer */
   int16_t     pduSz;        /* PDU Size to be constructed */
   RlcUmHdr    umHdr;        /* Header */
   uint32_t    rlcHdrSz;
   uint32_t    rlcSduSz;
   uint32_t    rlcPduSz;
   uint32_t    macHdrSz;
   
   /* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
   RlcContSduLst         contSduLst;  /*Contained sduLst */
   int32_t               dataVol    = rbCb->m.umDl.bo;
   uint32_t*             totMacGrant= &(datReq->totMacGrant);
   RlcL2MeasDlIpTh       *dlIpThPut = &rbCb->l2MeasIpThruput.dlIpTh;
   uint8_t               *sduIdx    = &dlIpThPut->lastSduIdx;
   bool                  newIdx = FALSE;
   int32_t               oldBo;
   RlclchInfo            lchInfo = {0};
   uint32_t              segSduCnt = 0;
#endif
   Ticks                curTime  = 0;
   int16_t              timeDiff = 0;
   RlcSdu                *sdu;

   pdu = NULLP;
   pduInfo = &(datReq->pduInfo);
   pduSz = datReq->pduSz;
   
#ifdef LTE_L2_MEAS   
   contSduLst.numSdus = 0;
   contSduLst.lcId = rbCb->lch.lChId;
   oldBo = rbCb->m.umDl.bo; 
   lchInfo.lcId = rbCb->lch.lChId;
   lchInfo.numSdus = 0;
#endif

   /* Discard new changes starts */
   rlcUtlGetCurrTime(&curTime);

   /* ccpu00143043 */
   while ((pduSz > 0) && (rbCb->m.umDl.sduQ.count > 0) && (pduInfo->numPdu < RLC_MAX_PDU))
   {
      CM_LLIST_FIRST_NODE(&rbCb->m.umDl.sduQ,firstNode);
      sdu = (RlcSdu *)(firstNode->node);

      if ((sdu->mode.um.isSegmented == FALSE) && (rbCb->discTmrInt > 0) && 
            (rbCb->rlcId.rbType == CM_LTE_DRB))
      {
         timeDiff = RLC_TIME_DIFF(curTime,sdu->arrTime); 

         if (timeDiff >= rbCb->discTmrInt)
         {
#ifdef LTE_L2_MEAS 
            RLC_UPD_L2_DL_DISC_SDU_STS(gCb, rbCb);
#endif
            rbCb->m.umDl.bo -= sdu->sduSz;
            RLC_REMOVE_SDU(gCb,&rbCb->m.umDl.sduQ,sdu);
            continue;
         }
      }
#ifdef LTE_L2_MEAS
      newIdx = FALSE;
#endif
      /* When forming a new PDU, pdu == NULLP
           -# Eliminate MAC header size for each pdu
           -# Substract the fixed header length based on SN length
      */
      /* account for the RLC header size
         minimum header size will be 1 , if Sdu is not segmented */
      rlcHdrSz = RLC_MIN_HDRSZ;
      if(sdu->mode.um.isSegmented)
      {
         /* value of rbCb->m.umDl.snLen will be 1 for 6 bit SN and 2 for 12 bit SN and 2 bytes of SO */
         rlcHdrSz = (rbCb->m.umDl.snLen + 2);
      }
      macHdrSz = RLC_MAC_HDR_SZ2; /*Minimum MacHdr size */
      rlcSduSz = sdu->sduSz;
      rlcPduSz = ((rlcSduSz + rlcHdrSz) < (pduSz - macHdrSz))? (rlcSduSz + rlcHdrSz) : (pduSz - macHdrSz);
      rlcSduSz = rlcPduSz - rlcHdrSz;

      /*Estimate MAC Hdr based on calculated rlcPduSz */
      macHdrSz = (rlcPduSz > 255 ) ? RLC_MAC_HDR_SZ3 : RLC_MAC_HDR_SZ2;

      if(macHdrSz != RLC_MAC_HDR_SZ2)
      {
          rlcSduSz = sdu->sduSz;
          rlcPduSz = ((rlcSduSz + rlcHdrSz) < (pduSz - macHdrSz))? (rlcSduSz + rlcHdrSz) : (pduSz - macHdrSz);
          rlcSduSz = rlcPduSz - rlcHdrSz;
          macHdrSz = (rlcPduSz > 255 ) ? RLC_MAC_HDR_SZ3 : RLC_MAC_HDR_SZ2;
      }

      if(sdu->mode.um.isSegmented == FALSE)
      {
          /* RLC SDU is estimated to be segmented first time */
          if(rlcSduSz < sdu->sduSz)
          {
              rlcHdrSz = rbCb->m.umDl.snLen;
              rlcSduSz = sdu->sduSz;
              rlcPduSz = ((rlcSduSz + rlcHdrSz) < (pduSz - macHdrSz))? (rlcSduSz + rlcHdrSz) : (pduSz - macHdrSz);
              rlcSduSz = rlcPduSz - rlcHdrSz;
              /*Estimate MAC Hdr based on calculated rlcPduSz */
              macHdrSz = (rlcPduSz > 255 ) ? RLC_MAC_HDR_SZ3 : RLC_MAC_HDR_SZ2;
          }
      }

      pduSz -= (rlcHdrSz + macHdrSz);

      if(pduSz <= 0)
      {
          break;
      }
     
      /* No Segmentation scenario :
         If SDU size is less than or equal to the requested PDU size
         -# Allocate memory and copy SDU into it.
         -# Update BO
         -# Remove SDU from the Queue.
      */
      if (sdu->sduSz <= pduSz)
      {
         if (!pdu)
         {
            pdu = sdu->mBuf;
            sdu->mBuf = NULLP;
         }
         rbCb->m.umDl.bo -= sdu->sduSz;
	 rbCb->m.umDl.bo -= RLC_MAX_HDRSZ;
         pduSz -= sdu->sduSz;

#ifdef LTE_L2_MEAS
        if(RLC_MEAS_IS_DL_ANY_MEAS_ON_FOR_RB(gCb,rbCb))
        {
           if(sdu->mode.um.isSegmented)
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
                 sdu->mode.um.sduId, newIdx);
           /* ccpu00143043 */
           if ( lchInfo.numSdus < RLC_L2MEAS_SDUIDX)
           {
              lchInfo.sduInfo[lchInfo.numSdus].arvlTime = sdu->arrTime; 
              lchInfo.sduInfo[lchInfo.numSdus].isRetxPdu = FALSE;
              lchInfo.numSdus++;
           }
        }
#endif
/* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS_RLC
         rlcUtlUpdSduSnMap(rbCb, sdu, datReq, TRUE);
         if((rbCb->rbL2Cb.measOn & LKW_L2MEAS_DL_DELAY) ||
                 (rbCb->rbL2Cb.measOn & LKW_L2MEAS_UU_LOSS))
          {
             /* ccpu00143043 */
             if ( lchInfo.numSdus < RLC_L2MEAS_SDUIDX)
             {
                lchInfo.arvlTime[lchInfo.numSdus] = sdu->arrTime; 
                lchInfo.numSdus++;
             }
          }
#endif /*  LTE_L2_MEAS */

         if(sdu->mode.um.isSegmented)
         {
             umHdr.si = RLC_SI_LAST_SEG;
             umHdr.so = sdu->actSz - sdu->sduSz;
             sdu->mode.um.isSegmented = FALSE;
         }
         else
         {
              umHdr.si = 0;
              umHdr.so = 0;
         }
         rlcUmmCreatePdu(gCb, rbCb, pdu, &umHdr, pduInfo);
         RLC_REMOVE_SDU(gCb,&(rbCb->m.umDl.sduQ),sdu); /* kw003.201 */
         rlcUtlIncrementKwuStsSduTx(gCb->u.dlCb->rlcKwuDlSap + rbCb->k1wuSapId);
         pdu = NULLP;
      }
      /* Segmentation scenario :
         If size of SDU is greater than PDU size 
           -# Allocate memory and Segment the Sdu.
           -# Update BO
           -# Add segment to the PDU
           -# Set the second bit of the segmentation info.
           -# Create the complete PDU and place in pduInfo.
      */ 
      else 
      {
         Buffer *remSdu;
       
         ODU_SEGMENT_MSG(sdu->mBuf,pduSz,&remSdu);
        
#ifdef LTE_L2_MEAS
        if(RLC_MEAS_IS_DL_IP_MEAS_ON_FOR_RB(gCb, rbCb))
        {
           if(sdu->mode.um.isSegmented)
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
                 sdu->mode.um.sduId, newIdx);
        }
        if(RLC_MEAS_IS_DL_UU_LOSS_MEAS_ON_FOR_RB(gCb,rbCb))
        {
           if(sdu->actSz == sdu->sduSz)
           {
              segSduCnt++;
           }
        }
#endif
         if(sdu->mode.um.isSegmented)
         {
            umHdr.si = RLC_SI_MID_SEG;
            umHdr.so = sdu->actSz - sdu->sduSz;
         }
         else
         {
            umHdr.si = RLC_SI_FIRST_SEG;
            umHdr.so = 0;
            sdu->mode.um.isSegmented = TRUE;
         }
	 pdu = sdu->mBuf;
	 sdu->sduSz -= pduSz;
	 rbCb->m.umDl.bo -= pduSz;
	 sdu->mBuf = remSdu;
	 pduSz = 0;

/* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS_RLC
         rlcUtlUpdSduSnMap(rbCb, sdu, datReq, FALSE);
#endif /*  LTE_L2_MEAS */

         rlcUmmCreatePdu(gCb, rbCb, pdu, &umHdr, pduInfo);
         pdu = NULLP;
      }
/* kw005.201 added support for L2 Measurement */
   }
#ifdef LTE_L2_MEAS_RLC
   if((rbCb->rbL2Cb.measOn) && 
      (rbCb->m.umDl.sduQ.count == 0) && 
      (dataWasPrsnt))
   {
      if(--(rbCb->ueCb->numActRb[rbCb->qci]) == 0)
      {
         rlcCb.rlcL2Cb.numActUe[rbCb->qci]--;
      }
   }
#endif /* LTE_L2_MEAS */
#ifdef LTE_L2_MEAS
   rlcUtlUpdateBurstSdus(gCb, rbCb, &contSduLst, dataVol, *totMacGrant);
   /* Need to check into optimizing this code : TODO */
   if(RLC_MEAS_IS_DL_ANY_MEAS_ON_FOR_RB(gCb,rbCb) && (lchInfo.numSdus != 0))
   {
      RlcL2MeasTb *l2MeasTb = rlcUtlGetCurMeasTb(gCb, rbCb);
      /* ccpu00143043 */
      /* Fix Klock warning */
      if ((lchInfo.numSdus != 0) && (l2MeasTb != NULLP) &&
          (l2MeasTb->numLchInfo < RLC_MAX_ACTV_DRB))
      {   
         memcpy( &l2MeasTb->lchInfo[l2MeasTb->numLchInfo],  &lchInfo, sizeof(RlclchInfo));
         l2MeasTb->numLchInfo++;
      }
      l2MeasTb->txSegSduCnt += segSduCnt;
   }
   *totMacGrant -= (oldBo - rbCb->m.umDl.bo);
#endif 

   datReq->boRep.bo = rbCb->m.umDl.bo;
   datReq->boRep.estHdrSz = 0;
   datReq->boRep.staPduPrsnt = FALSE;
   if (rbCb->m.umDl.sduQ.count > 0)
   {
      datReq->boRep.oldestSduArrTime = 
        ((RlcSdu *)(rbCb->m.umDl.sduQ.first->node))->arrTime;
   }
   return; 
}

/**
 * @brief   Handler to process the re-establishment request received from i
 *          the upper layer. 
 *       
 * @details
 *     This function does the following functions : 
 *         Remove all the SDUs in the SDU queue.
 *
 * @param[in] gCb        RLC Instance control block
 * @param[in] rlcID      Identity of the RLC entity for which 
 *                       re-establishment is to be done
 * @param[in] sendReEst  Whether to send re-establishment complete 
 *                       indication to  upper layer or not
 * @param[in] rbCb       RB control block for which re-establishment 
 *                       is to be done
 *
 * @return  Void
*/ 
Void rlcDlUmmReEstablish(RlcCb *gCb,CmLteRlcId rlcId,Bool sendReEst,RlcDlRbCb *rbCb)
{
   /* The re-establishment indication is sent from the UL only */

   rlcUmmFreeDlRbCb(gCb, rbCb);

   rbCb->m.umDl.txNext = 0;

   /* this would have been set when re-establishment was triggered
      for SRB 1 */
   rlcDlUtlResetReestInProgress(rbCb);
   
   return;
}
/**
 * @brief   Handler to create the header and complete a PDU.
 *       
 * @details
 *     This function is used to create the header of a PDU and concatenate  it
 *     with the data part of the PDU.
 *     Also updates the statistics
 *     Sets the passed pdu to NULLP
 *
 * @param[in]     gCb            RLC instance control block
 * @param[in,out] rbCb           RB control block 
 * @param[in]     pdu            PDU  
 * @param[in]     umHdr          UM mode header
 * @param[out]    datReqPduInfo  Holder in which to copy the created PDU pointer
 *
 * @return  Void
*/ 
static void rlcUmmCreatePdu(RlcCb *gCb, RlcDlRbCb *rbCb, Buffer *pdu, RlcUmHdr *umHdr, KwPduInfo *datReqPduInfo)
{
   RlcSn     sn;                   /*  Sequence Number */
   uint8_t   hdr[RLC_MAX_HDRSZ];   /* Stores header */
   uint32_t  idx = 0;              /* To index to the hdr array */
   
   /* stats updated before for bytes sent before adding RLC headers */
   rlcUtlIncrementGenStsBytesAndPdusSent(&gCb->genSts, pdu);
         
   /* If SI = 0, 1 byte header conatining SI/R */
   if(umHdr->si == 0)
   {
      hdr[idx++] = 0;
   }
   else
   {
      /* Add SN based on SN length */
      sn = rbCb->m.umDl.txNext;
      if (rbCb->m.umDl.snLen == RLC_UM_CFG_6BIT_SN_LEN) 
      {
         hdr[idx++] = (umHdr->si << 6) | sn;
      }
      else
      {
         hdr[idx++] = (umHdr->si << 6) | (sn >> 8);
	 hdr[idx++] = sn & 0xff ;
      }

      /* Add SO for middle and last segments*/
      if((umHdr->si == RLC_SI_MID_SEG) | (umHdr->si == RLC_SI_LAST_SEG))
      {
         hdr[idx++] = (umHdr->so >> 8);
	 hdr[idx++] = umHdr->so & 0xff;
      }

      /* Increment TX_Next if this is last segment of current SDU */
      if(umHdr->si == RLC_SI_LAST_SEG)
         rbCb->m.umDl.txNext = (rbCb->m.umDl.txNext + 1) & rbCb->m.umDl.modBitMask;

   }

   /* add the header to the beginning of the pdu */
   ODU_ADD_PRE_MSG_MULT_IN_ORDER(hdr, idx, pdu);

   datReqPduInfo->mBuf[datReqPduInfo->numPdu++] = pdu;
   return;
}

/**
 * @brief   Handler to discard a SDU.
 *       
 * @details
 *     This function is used to discard a SDU after receiving
 *     the Discard Request from the upper layer.The SDU is discarded if 
 *     it is present and is not mapped to any PDU yet. 
 *     The discards coming from the upper layer would be coming in 
 *     sequence according to the sduId, so we should find the sduId at the 
 *     head of the sduQ. Discards if there is a match else does nothing.
 *
 * @param[in] rbCb   RB control block 
 * @param[in] sduId  SDU ID of the SDU to be discarded
 *
 * @return  Void
*/
Void rlcUmmDiscSdu(RlcCb *gCb,RlcDlRbCb *rbCb,uint32_t sduId)
{
   CmLList *tmpNode;  /* Temporary Node in SDU queue */
   CM_LLIST_FIRST_NODE(&rbCb->m.umDl.sduQ,tmpNode);

   if (tmpNode)
   {
      RlcSdu *sdu = (RlcSdu *)tmpNode->node;
      
      if (sdu->mode.um.sduId == sduId && sdu->mode.um.isSegmented == FALSE)
      {
/* kw005.201 added support for L2 Measurement */
         RLC_REMOVE_SDU(gCb,&rbCb->m.umDl.sduQ,sdu);
         gCb->genSts.numSduDisc++;         
      }
   }

   return;
}

/*
 *
 * @brief
 *    function to free/release the UnAcknowledged mode RBCB buffers
 *
 * @details
 *    This primitive Frees the Unacknowldged Mode RbCb sdu queue
 *
 * @param [in]   gCb    - RLC instance control block
 * @param [in]   rbCb   - RB Control Block
 *
 * @return Void
 */
Void rlcUmmFreeDlRbCb(RlcCb *gCb,RlcDlRbCb *rbCb)
{

   /* cat the SDU queue to the to be freed list */
   RLC_FREE (gCb,rbCb->snssai, sizeof (Snssai));
   cmLListCatLList(&(gCb->u.dlCb->toBeFreed.sduLst),&(rbCb->m.umDl.sduQ));
   rlcUtlRaiseDlCleanupEvent(gCb);

   return;
} /* rlcUmmFreeDlRbCb */

/********************************************************************30**
         End of file
**********************************************************************/
