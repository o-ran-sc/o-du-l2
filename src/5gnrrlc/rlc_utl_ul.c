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
  
     Desc:     Source code for RLC Utility Module
               This file contains following functions

                  --rlcUtlSendToMac
                  --rlcUtlRcvFrmMac
                  --rlcUtlEmptySduQ
                  --rlcUtlSendUlDataToDu 
                  --kwUtlShutDown

     File:     rlc_utl_ul.c

**********************************************************************/

/** @file rlc_utl_ul.c
@brief RLC Utility Module 
*/

/* header (.h) include files */
#include "common_def.h"
/* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
#include <sys/time.h>
#endif

#include "ckw.h"           /* CKW defines */
#include "kwu.h"           /* KWU defines */
#include "lkw.h"           /* LKW defines */
#include "rgu.h"           /* RGU defines */
#include "rlc_env.h"        /* RLC environment options */
#include "rlc_err.h"        /* Error defines */


/* extern (.x) include files */
#include "ckw.x"           /* CKW includes */
#include "kwu.x"           /* KWU includes */
#include "lkw.x"           /* LKW includes */
#include "rgu.x"           /* RGU includes */

#include "rlc_utils.h"            /* RLC defines */
#include "rlc_ul.h"         /* RLC Uplink defines */
#include "du_app_rlc_inf.h"
#include "rlc_upr_inf_api.h"

#ifdef SS_RBUF
S16 SMrkUlPkt(Buffer *mbuf);
#endif
RlcAmRecBuf* rlcUtlGetRecBuf(CmLListCp *recBufLst, RlcSn sn);
#define RLC_MODULE (RLC_DBGMASK_DUT | RLC_DBGMASK_UL) /* for debugging purpose */


/**
 *
 * @brief 
 *        Handler for receiving data for multiple logical channels from MAC.
 *
 * @details
 *        This function receives the data sent by MAC for one or more 
 *        logical channels.It calls the UMM or AMM functions to process
 *        the PDUs and send them to the uppper layer.  
 *
 * @param[in] gCb          - RLC instance control block 
 * @param[in] datIndInfo   - Data Indication Information containing the PDU(s)
 *                            for one or more logical channels  
 *
 * @return  S16
 *      -# ROK 
 *      -# RFAILED
 *
 */
uint8_t rlcUtlRcvFrmMac(RlcCb *gCb, KwDatIndInfo  *datIndInfo)
{
   uint32_t    count;      /* Loop Counter */
   KwPduInfo   *pduInfo;   /* PDU Information */
   RlcUlRbCb    *rbCb;      /* RB Control Block */
   RlcUlUeCb    *ueCb;      /* UE Control Block */

   ueCb = NULLP;
   
   /* fetch UeCb  */
   if( ROK != rlcDbmFetchUlUeCb(gCb,datIndInfo->rnti,datIndInfo->cellId,&(ueCb)))
   {
      /* Fetch UeCb failed */
      DU_LOG("\nERROR  -->  RLC_UL : rlcUtlRcvFrmMac : UEID:%d UeCb not found",
               datIndInfo->rnti);
      /* free the buffers inside the datIndInfo */
      uint32_t i,j;
      for(i = 0; i< datIndInfo->numLch; i++)
      {
         for(j = 0; j < datIndInfo->lchData[i].pdu.numPdu; j++)
         {
            if(datIndInfo->lchData[i].pdu.mBuf[j])
            {
               ODU_PUT_MSG_BUF(datIndInfo->lchData[i].pdu.mBuf[j]);
            }
         }
      }
      
      return RFAILED;
   }

#ifdef LTE_L2_MEAS

   if (RGU_L2M_UL_BURST_START == datIndInfo->burstInd)
   {
      ueCb->isUlBurstActive = TRUE;
   }
   else
   {
      ueCb->firstPacketTTI = 0;
      ueCb->isUlBurstActive = FALSE;
   }
#endif
   for ( count = 0;count < datIndInfo->numLch; count++ )
   {
      rbCb = ueCb->lCh[datIndInfo->lchData[count].lcId - 1].ulRbCb;
      /* kw002.201 Removed allocation of pduInfo */ 
      pduInfo = &(datIndInfo->lchData[count].pdu);
      /* Fix for CR ccpu00138374,sometimes rbCb is NULL in UL path,
       * So inorder to avoid the crash, added this preventive check
       */
      if(rbCb == NULLP)
      {
         uint32_t j;
         for(j = 0; j < pduInfo->numPdu; j++)
         {
            if(pduInfo->mBuf[j])
            {
               ODU_PUT_MSG_BUF(pduInfo->mBuf[j]);
            }
         }
         continue;
      }

#ifdef SS_RBUF
      SMrkUlPkt(pduInfo->mBuf[0]);
#endif
      if ( rbCb->mode == RLC_MODE_UM )
      {
/* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
         rlcUmmProcessPdus(gCb,rbCb, pduInfo, datIndInfo->ttiCnt);
#else
         rlcUmmProcessPdus(gCb,rbCb,pduInfo);
#endif
      }
      else if (rbCb->mode == RLC_MODE_AM )
      {
/* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
         rlcAmmProcessPdus(gCb,rbCb,  pduInfo, datIndInfo->ttiCnt);
#else
         rlcAmmProcessPdus(gCb,rbCb,pduInfo);
#endif
      }
   }
   return ROK;
}

/**
 *
 * @brief 
 *    Handler for sending Data Indication to the upper layer. 
 * 
 * @Details
 *    This function is used to send re-assembled SDU to the upper layer.
 *
 * @param[in]  gCb    - RLC instance Control Block 
 * @param[in]  rbCb   - RB Control Block 
 * @param[in]  sdu    - SDU to be sent to upper layer 
 *
 * @return  S16
 *      -# ROK 
 */
uint8_t rlcUtlSendUlDataToDu(RlcCb *gCb, RlcUlRbCb *rbCb, Buffer *sdu)
{
#ifndef KW_PDCP
   KwuDatIndInfo   *datIndInfo = NULLP;   /* Data Indication Information */
   KwuDatIndInfo datIndInfoTmp;
#endif
   RlcUlRrcMsgInfo  *ulRrcMsgInfo = NULLP;
   RlcUlUserDatInfo *ulUserDatInfo = NULLP;
   uint16_t        msgLen = 0, copyLen = 0;
   Pst             pst;

#ifndef KW_PDCP
   /* Creating static memory for KwuDatIndInfo. #else will be 
    * removed once the testing is done on all platforms */
   datIndInfo = &datIndInfoTmp;
   if (datIndInfo == NULLP)
   {
      DU_LOG("\nERROR  -->  RLC_UL: rlcUtlSendUlDataToDu: Memory allocation failed UEID:%d \
         CELLID:%d", rbCb->rlcId.ueId, rbCb->rlcId.cellId);
      ODU_PUT_MSG_BUF(sdu);
      return RFAILED;
   }

   RLC_MEM_CPY(&(datIndInfo->rlcId),&(rbCb->rlcId),sizeof(CmLteRlcId));
   /* Set the "isOutofSeq" flag for each packet 
    * If packets are in-sequence set flag as TRUE else FALSE */
   datIndInfo->isOutOfSeq = rbCb->m.amUl.isOutOfSeq; 
#endif /* KW_PDCP */
   
   if(rbCb->rlcId.rbType == RB_TYPE_SRB)
   {
      /* Filling UL RRC Message Info if UL msg is not received on SRBs*/
      RLC_ALLOC_SHRABL_BUF(RLC_MEM_REGION_UL, RLC_POOL,
	    ulRrcMsgInfo, sizeof(RlcUlRrcMsgInfo));
      if (ulRrcMsgInfo)
      {
	      ulRrcMsgInfo->cellId = rbCb->rlcId.cellId;
	      ulRrcMsgInfo->ueId = rbCb->rlcId.ueId;
	      ulRrcMsgInfo->lcId = rbCb->lch.lChId;
	      ODU_GET_MSG_LEN(sdu, (MsgLen *)&msgLen);
	      RLC_ALLOC_SHRABL_BUF(RLC_MEM_REGION_UL, RLC_POOL,
	             ulRrcMsgInfo->rrcMsg, msgLen);
	      if (ulRrcMsgInfo->rrcMsg)
	      {
	        ODU_COPY_MSG_TO_FIX_BUF(sdu, 0, msgLen, ulRrcMsgInfo->rrcMsg, (MsgLen *)&copyLen);
	        ulRrcMsgInfo->msgLen = msgLen;

	        /* Sending UL RRC Message transfeer to DU APP */
	        memset(&pst, 0, sizeof(Pst));
	        FILL_PST_RLC_TO_DUAPP(pst, RLC_UL_INST, EVENT_UL_RRC_MSG_TRANS_TO_DU);
	        rlcSendUlRrcMsgToDu(&pst, ulRrcMsgInfo);
	     }
	     else
	     {
	       DU_LOG("\nERROR  -->  RLC_UL: rlcUtlSendUlDataToDu: Memory allocation failed for rrcMsg");
	       RLC_FREE_SHRABL_BUF(RLC_MEM_REGION_UL, RLC_POOL, ulRrcMsgInfo, sizeof(RlcUlRrcMsgInfo));
	       return RFAILED;
	     }
      }
      else
      {
	 DU_LOG("\nERROR  -->  RLC_UL: rlcUtlSendUlDataToDu: Memory allocation failed for ulRrcMsgInfo");
	 return RFAILED;
      }
   }
   else if(rbCb->rlcId.rbType == RB_TYPE_DRB)
   {
      /* Filling UL User Data Info if UL msg is received on a DRB */
      RLC_ALLOC_SHRABL_BUF(RLC_MEM_REGION_UL, RLC_POOL, ulUserDatInfo, sizeof(RlcUlUserDatInfo));
      if (ulUserDatInfo)
      {
         ulUserDatInfo->cellId = rbCb->rlcId.cellId;
	      ulUserDatInfo->ueId = rbCb->rlcId.ueId;
         ulUserDatInfo->rbId = rbCb->rlcId.rbId;
	      ODU_GET_MSG_LEN(sdu, (MsgLen *)&msgLen);
	      RLC_ALLOC_SHRABL_BUF(RLC_MEM_REGION_UL, RLC_POOL, ulUserDatInfo->userData, msgLen);
	      if (ulUserDatInfo->userData)
	      {
	        ODU_COPY_MSG_TO_FIX_BUF(sdu, 0, msgLen, ulUserDatInfo->userData, (MsgLen *)&copyLen);
	        ulUserDatInfo->msgLen = msgLen;
	       
            DU_LOG("\nDEBUG  -->  RLC_UL -> DUAPP : UL DATA [DRB ID:%d]", ulUserDatInfo->rbId);

	       /* Sending UL RRC Message transfeer to DU APP */
	       memset(&pst, 0, sizeof(Pst));
	       FILL_PST_RLC_TO_DUAPP(pst, RLC_UL_INST, EVENT_UL_USER_DATA_TRANS_TO_DU);
	       rlcSendUlUserDataToDu(&pst, ulUserDatInfo);
	     }
	     else
	     {
	        DU_LOG("\nERROR  -->  RLC_UL: rlcUtlSendUlDataToDu: Memory allocation failed for user data");
	        RLC_FREE_SHRABL_BUF(RLC_MEM_REGION_UL, RLC_POOL, ulUserDatInfo, sizeof(RlcUlUserDatInfo));
	        return RFAILED;
	     }
     }
     else
     {
	     DU_LOG("\nERROR  -->  RLC_UL: rlcUtlSendUlDataToDu: Memory allocation failed for ulUserDatInfo");
	     return RFAILED;
     }
   }
   return ROK;
} /* rlcUtlSendUlDataToDu */


static Void dumpRLCUlRbInformation(RlcUlRbCb* ulRbCb)
{
   if(ulRbCb->mode == RLC_MODE_UM)
   {
      uint32_t i;
      uint32_t pdusInReceptionBuffer = 0;
      uint32_t totalSegs = 0;

      for(i = 0; i< RLC_RCV_BUF_BIN_SIZE; i++)
      {
         RlcUmRecBuf *recBuf = rlcUtlGetUmRecBuf(ulRbCb->m.umUl.recBufLst, i);
         if(recBuf != NULLP)
         {
	     pdusInReceptionBuffer++;
	     totalSegs += (recBuf->segLst.count);
         }
      }
      
      DU_LOG("\nDEBUG  -->  RLC_UL: UM UL UEID:%d CELLID:%d Reception Buffer size = %d Total segs = %d", 
         ulRbCb->rlcId.ueId, ulRbCb->rlcId.cellId, pdusInReceptionBuffer, totalSegs);
   }
   else if(ulRbCb->mode == RLC_MODE_AM)
   {
      uint32_t i;
      uint32_t pdusInReceptionBuffer = 0;
      uint32_t totalSegs = 0;
      uint32_t windSz  = RLC_AM_GET_WIN_SZ(ulRbCb->m.amUl.snLen) << 1;
      
      for(i = 0; i< windSz; i++)
      {
         RlcAmRecBuf *recBuf = rlcUtlGetRecBuf(ulRbCb->m.amUl.recBufLst, i);
         if(recBuf != NULLP)
         {
            pdusInReceptionBuffer++;
            totalSegs += (recBuf->segLst.count);
         }
      }

      DU_LOG("\nDEBUG  -->  RLC_UL: AM UL UEID:%d CELLID:%d Reception Buf size = %d total segs = %d", 
         ulRbCb->rlcId.ueId, ulRbCb->rlcId.cellId, pdusInReceptionBuffer, totalSegs);
   }
}

Void DumpRLCUlDebugInformation(Void)
{
   RlcCb* ulInst = rlcCb[0]; /* TODO : Check whether UL is 0 or 1 */
   RlcUlCb* ulCb = ulInst->u.ulCb;
   RlcUlUeCb *ueCb = NULLP; 

   /* Until no more ueCb is ueLstCp hash list get and delete ueCb */
   while (ROK == cmHashListGetNext(&ulCb->ueLstCp, 
                                   (PTR) ueCb, 
                                   (PTR *)&ueCb))
   {
      uint32_t i;
      for(i = 0; i< RLC_MAX_SRB_PER_UE; i++)
      {
         RlcUlRbCb* ulRbCb = ueCb->srbCb[i]; 
         if(ulRbCb != NULLP)
         {
            dumpRLCUlRbInformation(ulRbCb);
         }
      }
      for(i = 0; i< RLC_MAX_DRB_PER_UE; i++)
      {
         RlcUlRbCb* ulRbCb = ueCb->drbCb[i]; 
         if(ulRbCb != NULLP)
         {
            dumpRLCUlRbInformation(ulRbCb);
         }
      }
   }
}


/*
 * kwUtlFreeUlRbCb() function is split into two functions 
 *  -  rlcAmmFreeUlRbCb() ---> gp_amm_ul.c 
 *  -  rlcUmmFreeUlRbCb() ---> gp_umm_ul.c
 * and placed in respective files mentioned above
 */



/* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS

/**
 *
 * @brief 
 *    Handler for initialisation of measurement 
 * 
 * @param[in]  gCb    - RLC instance Control Block 
 *
 * @return  S16
 *      -# ROK 
 */
S16 rlcUtlL2MeasUlInit(RlcCb *gCb)
{
   uint16_t             cntr;

   gCb->u.ulCb->rlcL2Cb.rlcNumMeas=0;
   for(cntr = 0; cntr < LKW_MAX_L2MEAS; cntr++)
   {
      memset(&(gCb->u.ulCb->rlcL2Cb.rlcL2EvtCb[cntr]), 0, sizeof(RlcL2MeasEvtCb));
   }
   gCb->u.ulCb->rlcL2Cb.rlcL2EvtCb[RLC_L2MEAS_UL_IP].measCb.measType = LKW_L2MEAS_UL_IP;
   return ROK;
}
/**
 *
 * @brief
 *
 *        Handler to calculate the Ul Ip throughput for a LCH
 *
 * @b Description:
 *
 *
 * @param[in] rbCb         RB control block
 * @param[in] pdu          Pdu of LCH
 *
 *  @return  Void
 *
 */
Void rlcUtlCalUlIpThrPutIncTTI(RlcCb *gCb,RlcUlRbCb *rbCb,uint32_t ttiCnt)
{
   volatile uint32_t     startTime = 0;

      /*starting Task*/
      SStartTask(&startTime, PID_RLC_IP_TPT_INCTTI);
#ifndef ALIGN_64BIT
   DU_LOG("\nDEBUG  -->  RLC_UL : Log for ul ip throughput:"
         "RB_MeasOn:%d ttiCnt :%ld UEID:%d CELLID:%d", 
         rbCb->rbL2Cb.measOn,ttiCnt,
         rbCb->rlcId.ueId,
         rbCb->rlcId.cellId);
#else
   DU_LOG("\nDEBUG  -->  RLC_UL : Log for ul ip throughput:"
         "RB_MeasOn:%d ttiCnt :%d UEID:%d CELLID:%d", 
         rbCb->rbL2Cb.measOn,ttiCnt,
         rbCb->rlcId.ueId,
         rbCb->rlcId.cellId);
#endif

   /*Check if UL IP throughput measurement is ON for this RB or not*/
   if(RLC_MEAS_IS_UL_IP_MEAS_ON_FOR_RB(gCb,rbCb))              
   {
      if (TRUE  == rbCb->ueCb->isUlBurstActive)
      {
         if (ttiCnt < rbCb->l2MeasIpThruput.prevTtiCnt)
         {
            /*Removed Error Print*/
         }
         if (rbCb->l2MeasIpThruput.prevTtiCnt != 0)
         {
            rbCb->rbL2Cb.l2Sts[RLC_L2MEAS_UL_IP]->ulIpThruput.timeSummation += 
               (ttiCnt - rbCb->l2MeasIpThruput.prevTtiCnt);
         }
         else
         {
            rbCb->ueCb->firstPacketTTI = ttiCnt;
         }
         rbCb->l2MeasIpThruput.prevTtiCnt = ttiCnt;
      }
      else
      {
         rbCb->l2MeasIpThruput.prevTtiCnt = 0;
      }
   }   

   /*stopping Task*/
   SStopTask(startTime, PID_RLC_IP_TPT_INCTTI);
} /* rlcUtlCalUlIpThrPutIncTTI */


/**
 *
 * @brief
 *
 *        Handler to calculate the Ul Ip throughput for a LCH
 *
 * @b Description:
 *
 *
 * @param[in] rbCb         RB control block
 * @param[in] pdu          Pdu of LCH
 *
 *  @return  Void
 *
 */
Void rlcUtlCalUlIpThrPut
(
RlcCb        *gCb,
RlcUlRbCb    *rbCb,
Buffer       *pdu,
uint32_t     ttiCnt
)
{
   MsgLen        rlcSduSz = 0;  /*Holds length of Rlc Sdu*/
   volatile uint32_t     startTime = 0;

   /*starting Task*/
   SStartTask(&startTime, PID_RLC_IP_TPT_INCVOL);

   /*Check if UL IP throughput measurement is ON for this RB or not*/
   if(RLC_MEAS_IS_UL_IP_MEAS_ON_FOR_RB(gCb, rbCb) &&              
         (TRUE  == rbCb->ueCb->isUlBurstActive) &&
         (rbCb->ueCb->firstPacketTTI) &&
         (ttiCnt != rbCb->ueCb->firstPacketTTI))
   {
      SFndLenMsg(pdu, &rlcSduSz);

      rbCb->rbL2Cb.l2Sts[RLC_L2MEAS_UL_IP]->ulIpThruput.volSummation += rlcSduSz;

   }
   /*stopping Task*/
   SStopTask(startTime, PID_RLC_IP_TPT_INCVOL);
} /* rlcUtlCalUlIpThrPut */


/**
 *
 * @brief Handler for L2 Measurement timer expiry.
 *
 *
 * @b Description
 *        This function is called when the l2 measurement timer expires. 
 *        This function sends a consolidates the mesaurements taken during
 *        this time and sends the confirm .
 *
 *  @param[in] measEvtCb    Measurement Event Control Block.
 *
 *
 *  @return  S16
 *      -# ROK
 */

S16 rlcUtlHdlL2TmrExp(RlcCb *gCb,RlcL2MeasEvtCb *measEvtCb)
{

#ifdef LTE_L2_MEAS_RLC
   uint16_t             qciIdx;
   RlcL2MeasCb     *measCb;
   
   /* Clean up the RB data structures */
   if((measEvtCb->measCb.measType & LKW_L2MEAS_ACT_UE) &&
      (measEvtCb->measCb.val.nonIpThMeas.numSamples))
   {
      measCb = &measEvtCb->measCb;

      for(qciIdx = 0; qciIdx < measCb->val.nonIpThMeas.numQci;qciIdx++)
      {
         measCb->val.nonIpThMeas.measData[measCb->val.nonIpThMeas.qci[qciIdx]].actUe.numActvUe   +=   
                     rlcCb.rlcL2Cb.numActUe[measCb->val.nonIpThMeas.qci[qciIdx]];
         measCb->val.nonIpThMeas.measData[measCb->val.nonIpThMeas.qci[qciIdx]].actUe.sampOc++;
      }
      measEvtCb->val.nonIpThMeas.measCb.numSamples--;
      rlcStartTmr(gCb, (PTR)measEvtCb, EVENT_RLC_L2_TMR); 
      return (ROK);
   }
#endif

   rlcUtlSndUlL2MeasCfm(gCb, measEvtCb);

   return (ROK);
} /* rlcUtlHdlL2TmrExp */
/**
 *
 * @brief Handler for Sending L2 Measurement confirm.
 *
 *
 * @b Description
 *        This function sends a consolidates the mesaurements taken during
 *        this time and sends the confirm .
 *
 *  @param[in] measEvtCb    Measurement Event Control Block.
 *
 *
 *  @return  S16
 *      -# ROK
 */

S16 rlcUtlSndUlL2MeasCfm(RlcCb *gCb,RlcL2MeasEvtCb  *measEvtCb)
{
   uint32_t            qciIdx;
   RlcL2MeasCb         *measCb;
   RlcL2MeasCfmEvt     measCfmEvt;
   uint64_t            ulDataVol;
   uint64_t            ulTime;
   uint16_t            cntr;
   /* Discard new changes starts */
   uint8_t             qci = 0;
   uint32_t            cfmIdx =0;
   /* Discard new changes ends */

   /* kw006.201 ccpu00120058 emoved 64 bit compilation warning */
#ifndef ALIGN_64BIT
   DU_LOG("\nDEBUG  -->  RLC_UL : rlcUtlSndUlL2MeasCfm(transId(%ld))", measEvtCb->transId);
#else
   DU_LOG("\nDEBUG  -->  RLC_UL : rlcUtlSndUlL2MeasCfm(transId(%d))", measEvtCb->transId);
#endif

   /* Clean up the RB data structures */
   measCb = &measEvtCb->measCb;
   
   memset(&measCfmEvt, 0, sizeof(RlcL2MeasCfmEvt));
   measCfmEvt.transId = measEvtCb->transId;

   measCfmEvt.measType = measCb->measType;
   measCfmEvt.status.status = LCM_PRIM_OK;
   measCfmEvt.status.reason = LCM_REASON_NOT_APPL;
   
   if( measCb->measType & LKW_L2MEAS_UL_IP)
   {
      RlcL2MeasCbUeMeasInfo *pUeInfoLstCb  = measCb->val.ipThMeas.ueInfoLst;
      RlcL2MeasCfmUeInfoLst *pUeInfoLstCfm = measCfmEvt.val.ipThMeas.ueInfoLst;
      for(cntr = 0;(cntr < measCb->val.ipThMeas.numUes) && (cntr < gCb->genCfg.maxUe);cntr++)        
      {
         pUeInfoLstCfm[cfmIdx].numCfm = 0;
         if (pUeInfoLstCb[cntr].isValid == TRUE)
         {
            pUeInfoLstCfm[cfmIdx].ueId = pUeInfoLstCb[cntr].ueId;
            pUeInfoLstCfm[cfmIdx].cellId = pUeInfoLstCb[cntr].cellId;

            for(qciIdx = 0; qciIdx < pUeInfoLstCb[cntr].numQci; qciIdx++)
            {
               qci = pUeInfoLstCb[cntr].qci[qciIdx];
               pUeInfoLstCfm[cfmIdx].measCfm[pUeInfoLstCfm[cfmIdx].numCfm].qci = qci;

               if(measCb->measType & LKW_L2MEAS_UL_IP)
               {
                  ulDataVol = pUeInfoLstCb[cntr].measData[qci].ulIpThruput.volSummation;
                  ulTime = pUeInfoLstCb[cntr].measData[qci].ulIpThruput.timeSummation;
                  if(0 == ulTime)
                  {
                     pUeInfoLstCfm[cfmIdx].measCfm[pUeInfoLstCfm[cfmIdx].numCfm].val.ipThrput.ulIpThPut = 0;
                  }
                  else
                  {
                     pUeInfoLstCfm[cfmIdx].measCfm[pUeInfoLstCfm[cfmIdx].numCfm].val.ipThrput.ulIpThPut = (ulDataVol / ulTime);

                     /* Converting it to kbps */
                     pUeInfoLstCfm[cfmIdx].measCfm[pUeInfoLstCfm[cfmIdx].numCfm].val.ipThrput.ulIpThPut *= 8;

                  }
                  /* Reset the values after reporting to Application */
                  pUeInfoLstCb[cntr].measData[qci].ulIpThruput.volSummation = 0;
                  pUeInfoLstCb[cntr].measData[qci].ulIpThruput.timeSummation = 0;
               }
               pUeInfoLstCfm[cfmIdx].numCfm++;
            }
            cfmIdx++;
         }
      }
      measCfmEvt.val.ipThMeas.numUes = cfmIdx; 
   }
   RlcMiLkwL2MeasCfm(&gCb->genCfg.lmPst, &measCfmEvt);
   return (ROK);
} /* rlcUtlSndUlL2MeasCfm */
/**
 *
 * @brief Handler for Sending Negative confirm .
 *
 *
  @b Description
 *        This function is called when the l2 measurement cannot be started
 *        This function sends  negative confirm for all the requests
 *
 * @param[in] gCb - RLC instance control block
 * @param[in] measReqEvt    Measurement Req Structure
 * @param[in] measCfmEvt  Confirmation to be sent to layer manager
 *
 *
 *  @return  S16
 *      -# ROK
 */

S16 rlcUtlSndUlL2MeasNCfm(RlcCb *gCb,RlcL2MeasReqEvt *measReqEvt,RlcL2MeasCfmEvt *measCfmEvt)
{

   RlcMiLkwL2MeasCfm(&gCb->genCfg.lmPst, measCfmEvt);
   return ROK;
} /* kwUtlSndL2MeasNCfm */

#ifdef LTE_L2_MEAS_RLC
/**
 * @brief  Validates the measurement request parameters. 
 *
 * @details
 *
 *     Function :rlcUtlValidateL2Meas 
 *
 *  @param[in]  measReqEvt L2 measurement request received from layer manager.
 *  @param[out] measCfmEvt L2 measurement confirm to be prepared.
 *  @param[out] lChId      List of LCh for the given Ue corresponding to QCIs
                           given in measurement request.
 *  @param[out] numLCh     Number of LCh in array lChId.
 **/

S16 rlcUtlValidateL2Meas
(
RlcL2MeasReqEvt *measReqEvt,
RlcL2MeasCfmEvt *measCfmEvt,
CmLteLcId       *lChId,
uint8_t         *numLCh
)
{
   uint8_t    measType;
   S16        ret;
   uint8_t    qciIdx;
   uint8_t    qci;
   uint8_t    idx;
   uint8_t    *qciVal;
   uint8_t    numQci;
   RlcUlRbCb  *rbCb;
   RlcUlUeCb  *ueCb;
   RbCb       **rbCbLst;
   uint8_t    rbIdx;
   uint8_t    lsbNibble = 0;
   uint8_t    msbNibble = 0;
   uint8_t    numFaild = 0;

   idx = 0;
   rbCb = NULLP;
   ret = ROK;
   measType = measReqEvt->measReq.measType;
   /* Check for the range of measType */
   /* LKW_L2MEAS_DL_IP+ LKW_L2MEAS_UL_IP = 0x0030*/
   if((measType == 0x00) ||
       measType > 0x30)
   {
      measCfmEvt->transId = measReqEvt->transId;
      measCfmEvt->measType = measType;
      measCfmEvt->status.status = LCM_PRIM_NOK;
      measCfmEvt->status.reason = LKW_CAUSE_INVALID_MEASTYPE;
      return RFAILED;
   }
   /*User can either request for Active UE,*
    *Dl delay, Dl discard, Uu Loss OR Dl ip throughput, Ul ip throughput. */
   lsbNibble = measType & 0x0F;
   msbNibble = measType & 0xF0;

   if( (lsbNibble != 0) && (msbNibble != 0) )
   {
      measCfmEvt->transId = measReqEvt->transId;
      measCfmEvt->measType = measType;
      measCfmEvt->status.status = LCM_PRIM_NOK;
      measCfmEvt->status.reason = LKW_CAUSE_INVALID_MEASTYPE;
      return RFAILED;
   }

   /* Check for total maximum number of Measurement Control Block */
   if(rlcCb.rlcL2Cb.rlcNumMeas >= LKW_MAX_L2MEAS )
   {
      measCfmEvt->transId = measReqEvt->transId;
      measCfmEvt->measType = measType;
      measCfmEvt->status.status = LCM_PRIM_NOK;
      measCfmEvt->status.reason = LKW_CAUSE_EXCEED_NUMMEAS;
      return RFAILED;
   }

   /* Check that number of samples should be a non-zero value */
   if(((measType & LKW_L2MEAS_ACT_UE) &&
      (measReqEvt->measReq.val.nonIpThMeas.numSamples == 0)))
   {
      measCfmEvt->transId = measReqEvt->transId;
      measCfmEvt->measType = measType;
      measCfmEvt->status.status = LCM_PRIM_NOK;
      measCfmEvt->status.reason = LKW_CAUSE_ZERO_NUMSAM;
      return RFAILED;
   }
   /* Check that measurement period  should be completely divisible *
    * number of sample.                                             */
   if(((measType & LKW_L2MEAS_ACT_UE) &&
      ((measReqEvt->measPeriod % 
        measReqEvt->measReq.val.nonIpThMeas.numSamples) != 0)))
   {
      measCfmEvt->transId = measReqEvt->transId;
      measCfmEvt->measType = measType;
      measCfmEvt->status.status = LCM_PRIM_NOK;
      measCfmEvt->status.reason = LKW_CAUSE_INVALID_NUMSAM;
      return RFAILED;
   }
   {
      numQci = measReqEvt->measReq.val.nonIpThMeas.numQci;
      qciVal = measReqEvt->measReq.val.nonIpThMeas.qci;
   }
   /* Check whether qci is configured or not */
   for(qciIdx = 0; qciIdx < numQci; qciIdx++)
   {
      qci = qciVal[qciIdx];
      ret = cmHashListFind(&(rlcCb.rlcL2Cb.qciHlCp), 
            (uint8_t *)&qci, (uint16_t)sizeof(qci), 0, (PTR *)&rbCb);
      if(ret != ROK)
      {
         measCfmEvt->val.nonIpThMeas.measCfm[measCfmEvt->val.nonIpThMeas.numCfm].qci = qci;
         measCfmEvt->val.nonIpThMeas.numCfm++;

      }
   }
   if(measCfmEvt->val.nonIpThMeas.numCfm > 0)
   {
      measCfmEvt->status.status = LCM_PRIM_NOK;
      measCfmEvt->status.reason = LKW_CAUSE_INVALID_QCI;
      measCfmEvt->measType = measType;
      measCfmEvt->transId = measReqEvt->transId;
      return RFAILED;
   }
   {
      for(qciIdx = 0; qciIdx < numQci; qciIdx++)
      {
         if(rlcCb.rlcL2Cb.measOn[qci] & measReqEvt->measReq.measType)
         {
            /* measurement is already ongoing */
            measCfmEvt->status.status = LCM_PRIM_NOK;
            measCfmEvt->status.reason = LKW_CAUSE_MEAS_ALREADY_ENA;
            measCfmEvt->val.nonIpThMeas.measCfm[measCfmEvt->val.nonIpThMeas.numCfm].qci = qci;
            measCfmEvt->measType = measType;
            measCfmEvt->val.nonIpThMeas.numCfm++;
         }
      }
   }
   if(measCfmEvt->val.nonIpThMeas.numCfm > 0)
   {
      measCfmEvt->transId = measReqEvt->transId;
      return RFAILED;
   }

   return (ROK);
}/* rlcUtlValidateL2Meas */
#endif

S16 rlcUtlValidateIpThL2Meas(RlcL2MeasReqEvt *measReqEvt,RlcL2MeasCfmEvt *measCfmEvt)
{
   uint8_t      measType;
   uint8_t         lsbNibble = 0;
   uint8_t         msbNibble = 0;

   measType = measReqEvt->measReq.measType;
   /* Check for the range of measType */
   /* LKW_L2MEAS_DL_IP+ LKW_L2MEAS_UL_IP = 0x0030*/
   if((measType == 0x00) ||
       measType > 0x30)
   {
      measCfmEvt->transId = measReqEvt->transId;
      measCfmEvt->measType = measType;
      measCfmEvt->status.status = LCM_PRIM_NOK;
      measCfmEvt->status.reason = LKW_CAUSE_INVALID_MEASTYPE;
      return RFAILED;
   }
   /*User can either request for Active UE,*
    *Dl delay, Dl discard, Uu Loss OR Dl ip throughput, Ul ip throughput. */
   lsbNibble = measType & 0x0F;
   msbNibble = measType & 0xF0;

   if( (lsbNibble != 0) && (msbNibble != 0) )
   {
      measCfmEvt->transId = measReqEvt->transId;
      measCfmEvt->measType = measType;
      measCfmEvt->status.status = LCM_PRIM_NOK;
      measCfmEvt->status.reason = LKW_CAUSE_INVALID_MEASTYPE;
      return RFAILED;
   }
   return (ROK);
}/* rlcUtlValidateL2Meas */

/**
 *
 * @brief Handler for resetting the RB data structures
 *
 *
 * @b Description
 *        This function resets the RB data structure after the expiry of 
 *        measurement timer.
 *
 *  @param[in] measCb    Measurement Control Block.
 *
 *
 *  @return  Void
 */

Void rlcUtlResetUlL2MeasInRlcRb(RlcCb *gCb,RlcL2MeasCb *measCb,uint8_t measType)
{
   uint32_t           rbIdx;
   uint32_t           ueIdx;
   uint32_t           qciIdx;
   RlcUlUeCb         *ueCb = NULL;



   if (measCb->measType & LKW_L2MEAS_UL_IP)
   {
      for(ueIdx = 0; ueIdx < measCb->val.ipThMeas.numUes; ueIdx++)
      {           
         if (measCb->val.ipThMeas.ueInfoLst[ueIdx].isValid == TRUE)
         {
            for (qciIdx =0; qciIdx < measCb->val.ipThMeas.ueInfoLst[ueIdx].numQci; qciIdx++)
            {
               if (measType & LKW_L2MEAS_UL_IP)
               {
                  measCb->val.ipThMeas.ueInfoLst[ueIdx].measData[qciIdx].ulIpThruput.volSummation = 0;
                  measCb->val.ipThMeas.ueInfoLst[ueIdx].measData[qciIdx].ulIpThruput.timeSummation = 0;
               }
            }

            if(ROK  != rlcDbmFetchUlUeCb(gCb, measCb->val.ipThMeas.ueInfoLst[ueIdx].ueId,
                     measCb->val.ipThMeas.ueInfoLst[ueIdx].cellId, &ueCb))
            {
               continue;
            }

            for (rbIdx = 0; rbIdx < RLC_MAX_DRB_PER_UE; rbIdx++)
            {
               if (ueCb->drbCb[rbIdx])
               {
                  ueCb->drbCb[rbIdx]->rbL2Cb.measOn &= ~measType;
               }
            }
         }
      }
   }
} /* rlcUtlResetUlL2MeasInRlcRb */

/**
 *
 * @brief Handler for storing address of MeasData in rbCb at right index
 *
 *
 * @b Description
 *        This function is called when LM sends measReq message to RLC.
 *
 *  @param[in] 
 *  @param[out] 
 *  @param[in] 
 *
 *
 *  @return  S16
 *      -# ROK
 */
Void rlcUtlPlcMeasDatInL2Sts(RlcL2Cntr *measData,RlcL2MeasRbCb *rbL2Cb,uint8_t measType)
{
   /* We should check the number of measType in the request. This can be done 
    * by looking at each bit in the measType. Also store the measData in the 
    * correct index of l2Sts in RbCb.
    * */

    if(measType & LKW_L2MEAS_ACT_UE)
    {
         rbL2Cb->l2Sts[RLC_L2MEAS_ACT_UE] = measData;
    }
    if(measType & LKW_L2MEAS_UU_LOSS)
    {
         rbL2Cb->l2Sts[RLC_L2MEAS_UU_LOSS] = measData;
    }
    if(measType & LKW_L2MEAS_DL_IP )
    {
         rbL2Cb->l2Sts[RLC_L2MEAS_DL_IP] = measData;
    }
    if(measType & LKW_L2MEAS_UL_IP)
    {
         rbL2Cb->l2Sts[RLC_L2MEAS_UL_IP] = measData;
    }
    if(measType & LKW_L2MEAS_DL_DISC)
    {
         rbL2Cb->l2Sts[RLC_L2MEAS_DL_DISC] = measData;
    }
    if(measType & LKW_L2MEAS_DL_DELAY)
    {
         rbL2Cb->l2Sts[RLC_L2MEAS_DL_DELAY] = measData;
    }
}/* End of rlcUtlPlcMeasDatInL2Sts */
#endif /* LTE_L2_MEAS */

/**
 *
 * @brief Store the UL buffer in hashList  
 *
 *
 * @b Description
 *
 *   Use the SN % binSize as key and store the received UL buffer
 *  @param[in] recBufLst       List CP array
 *  @param[in] recBuf          received buffer
 *  @param[in] sn              sn of the received buffer 
 *
 *
 *  @return  Void
 */
void rlcUtlStoreRecBuf(CmLListCp *recBufLst, RlcAmRecBuf *recBuf, RlcSn sn)
{
   uint32_t    hashKey; 
   
   hashKey = (sn % RLC_RCV_BUF_BIN_SIZE ); 
   recBuf->lnk.node = (PTR)recBuf;
   cmLListAdd2Tail(&(recBufLst[hashKey]), &recBuf->lnk);

   return;
} /* rlcUtlStoreRecBuf */

/**
 *
 * @brief Retrieve the UL buffer from the list
 *
 *
 * @Description
 *
 *   Use the SN % binSize as key and retrieve the UL buffer
 *  @param[in] recBufLst       List CP array
 *  @param[in] sn              sn of the received buffer 
 *
 *
 *  @return  Void
 */
RlcAmRecBuf* rlcUtlGetRecBuf(CmLListCp *recBufLst, RlcSn sn)
{
   uint32_t            hashKey; 
   CmLListCp           *recBufLstCp;
   RlcAmRecBuf         *recBuf;
   CmLList             *node = NULLP;

   hashKey = (sn % RLC_RCV_BUF_BIN_SIZE ); 
 
   recBufLstCp = &recBufLst[hashKey];
   CM_LLIST_FIRST_NODE(recBufLstCp, node);
   while(node)
   {
      recBuf = (RlcAmRecBuf *) node->node;
      if(recBuf->amHdr.sn == sn)
      {
         return recBuf;
      }
      CM_LLIST_NEXT_NODE(recBufLstCp, node);
   }
   return NULLP;
} /* rlcUtlGetRecBuf */
/**
 *
 * @brief Delete the UL buffer from the list
 *
 *
 * @Description
 *
 *   Use the SN % binSize as key and retrieve the UL buffer
 *  @param[in] recBufLst       List CP array
 *  @param[in] sn              sn of the received buffer 
 *
 *
 *  @return  Void
 */
void rlcUtlDelRecBuf(CmLListCp *recBufLst, RlcAmRecBuf *recBuf, RlcCb *gCb)
{
   uint32_t            hashKey; 
   CmLListCp           *recBufLstCp;

   hashKey = (recBuf->amHdr.sn % RLC_RCV_BUF_BIN_SIZE ); 
 
   recBufLstCp = &recBufLst[hashKey];
   cmLListDelFrm(recBufLstCp, &recBuf->lnk);
   RLC_FREE(gCb, recBuf, sizeof(RlcAmRecBuf));

   return;
} /* rlcUtlDelRecBuf */

/**
 *
 * @brief Store the UL buffer in hashList  
 *
 * @b Description
 *
 *  Use the SN % binSize as key and store the received UL buffer
 *  @param[in] recBufLst       List CP array
 *  @param[in] recBuf          received buffer
 *  @param[in] sn              sn of the received buffer 
 *
 *  @return  Void
 */
void rlcUtlStoreUmRecBuf(CmLListCp *recBufLst, RlcUmRecBuf *recBuf, RlcSn sn)
{
   uint32_t    hashKey; 
   
   hashKey = (sn % RLC_RCV_BUF_BIN_SIZE ); 
   recBuf->lnk.node = (PTR)recBuf;
   cmLListAdd2Tail(&(recBufLst[hashKey]), &recBuf->lnk);

   return;
} /* rlcUtlStoreUmRecBuf */

/**
 *
 * @brief Retrieve the UL buffer from the list
 *
 * @Description
 *
 *   Use the SN % binSize as key and retrieve the UL buffer
 *  @param[in] recBufLst       List CP array
 *  @param[in] sn              sn of the received buffer 
 *
 *  @return  Void
 */
RlcUmRecBuf* rlcUtlGetUmRecBuf(CmLListCp *recBufLst, RlcSn sn)
{
   uint32_t            hashKey; 
   CmLListCp           *recBufLstCp;
   RlcUmRecBuf         *recBuf;
   CmLList             *node = NULLP;

   hashKey = (sn % RLC_RCV_BUF_BIN_SIZE ); 
 
   recBufLstCp = &recBufLst[hashKey];
   CM_LLIST_FIRST_NODE(recBufLstCp, node);
   while(node)
   {
      recBuf = (RlcUmRecBuf *) node->node;
      if(recBuf->umHdr.sn == sn)
      {
         return recBuf;
      }
      CM_LLIST_NEXT_NODE(recBufLstCp, node);
   }
   return NULLP;
} /* rlcUtlGetUmRecBuf */

/**
 *
 * @brief Delete the UM Mode UL buffer from the list
 *
 *
 * @details
 *   Use the SN % binSize as key and retrieve the UL buffer
 *
 *  @param[in] *recBufLst       List CP array
 *  @param[in] *recBuf          Received buffer
 *
 *  @return  Void
 */
void rlcUtlDelUmRecBuf(RlcCb *gCb, CmLListCp *recBufLst, RlcUmRecBuf  *recBuf)
{
   uint32_t   hashKey;
   CmLListCp  *recBufLstCp;

   hashKey = (recBuf->sn % RLC_RCV_BUF_BIN_SIZE );
   recBufLstCp = &recBufLst[hashKey];
   cmLListDelFrm(recBufLstCp, &recBuf->lnk);
   RLC_FREE(gCb, recBuf, sizeof(RlcUmRecBuf));

   return;
} /* rlcUtlDelUmRecBuf */

/********************************************************************30**
         End of file
**********************************************************************/
