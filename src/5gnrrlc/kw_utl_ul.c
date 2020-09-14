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
  
     Desc:     Source code for RLC Utility Module
               This file contains following functions

                  --rlcUtlSndToLi
                  --rlcUtlRcvFrmLi
                  --rlcUtlEmptySduQ
                  --rlcUtlSndDatInd 
                  --kwUtlShutDown

     File:     kw_utl_ul.c

**********************************************************************/
static const char* RLOG_MODULE_NAME="UTL";
static int RLOG_MODULE_ID=2048;
static int RLOG_FILE_ID=210;

/** @file kw_utl_ul.c
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

#include "kw_env.h"        /* RLC environment options */
#include "kw.h"            /* RLC defines */
#include "kw_err.h"        /* Error defines */
#include "kw_ul.h"         /* RLC Uplink defines */

/* extern (.x) include files */
#include "ckw.x"           /* CKW includes */
#include "kwu.x"           /* KWU includes */
#include "lkw.x"           /* LKW includes */
#include "rgu.x"           /* RGU includes */

#include "kw.x"            /* RLC inlcudes */
#include "kw_ul.x"         /* RLC uplink includes */
#include "ss_rbuf.h"
#include "ss_rbuf.x"
#ifdef SS_RBUF
S16 SMrkUlPkt(Buffer *mbuf);
#endif
KwAmRecBuf* kwUtlGetRecBuf(CmLListCp *recBufLst, KwSn sn);
#define KW_MODULE (KW_DBGMASK_DUT | KW_DBGMASK_UL) /* for debugging purpose */


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
#ifdef ANSI
S16 rlcUtlRcvFrmLi
(
RlcCb           *gCb,                              
KwDatIndInfo   *datIndInfo                       
)
#else
S16 rlcUtlRcvFrmLi(gCb,datIndInfo)  
RlcCb           *gCb;                     
KwDatIndInfo   *datIndInfo;             
#endif
{
   U32         count;      /* Loop Counter */
   KwPduInfo   *pduInfo;   /* PDU Information */
   RlcUlRbCb    *rbCb;      /* RB Control Block */
   RlcUlUeCb    *ueCb;      /* UE Control Block */
/* kw005.201 added support for L2 Measurement */

   TRC2(rlcUtlRcvFrmLi)


   ueCb = NULLP;
   
   /* fetch UeCb  */
   if( ROK != rlcDbmFetchUlUeCb(gCb,datIndInfo->rnti,datIndInfo->cellId,&(ueCb)))
   {
      /* Fetch UeCb failed */
      RLOG_ARG1(L_ERROR,DBG_CELLID,datIndInfo->cellId,
               "UEID:%d UeCb not found",
               datIndInfo->rnti);
      /* free the buffers inside the datIndInfo */
      U32 i,j;
      for(i = 0; i< datIndInfo->numLch; i++)
      {
         for(j = 0; j < datIndInfo->lchData[i].pdu.numPdu; j++)
         {
            if(datIndInfo->lchData[i].pdu.mBuf[j])
            {
               RLC_FREE_BUF_WC(datIndInfo->lchData[i].pdu.mBuf[j]);
            }
         }
      }
      
      RETVALUE(RFAILED);
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
         U32 j;
         for(j = 0; j < pduInfo->numPdu; j++)
         {
            if(pduInfo->mBuf[j])
            {
               RLC_FREE_BUF_WC(pduInfo->mBuf[j]);
            }
         }
         continue;
      }

#ifdef SS_RBUF
      SMrkUlPkt(pduInfo->mBuf[0]);
#endif
      if ( rbCb->mode == CM_LTE_MODE_UM )
      {
/* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
         rlcUmmProcessPdus(gCb,rbCb, pduInfo, datIndInfo->ttiCnt);
#else
         rlcUmmProcessPdus(gCb,rbCb,pduInfo);
#endif
      }
      else if (rbCb->mode == CM_LTE_MODE_AM )
      {
/* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
         rlcAmmProcessPdus(gCb,rbCb,  pduInfo, datIndInfo->ttiCnt);
#else
         rlcAmmProcessPdus(gCb,rbCb,pduInfo);
#endif
      }
   }
   RETVALUE(ROK);
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
#ifdef ANSI
S16 rlcUtlSndDatInd
(
RlcCb       *gCb,
RlcUlRbCb   *rbCb,                   
Buffer     *sdu                    
)
#else
S16 rlcUtlSndDatInd(gCb,rbCb,sdu)
RlcCb       *gCb;
RlcUlRbCb   *rbCb;                  
Buffer     *sdu;                    
#endif
{
#ifndef KW_PDCP
   KwuDatIndInfo   *datIndInfo;   /* Data Indication Information */
   KwuDatIndInfo datIndInfoTmp;
#endif

   TRC3(rlcUtlSndDatInd)


#ifndef KW_PDCP
   /* Creating static memory for KwuDatIndInfo. #else will be 
    * removed once the testing is done on all platforms */
   datIndInfo = &datIndInfoTmp;

#if (ERRCLASS & ERRCLS_ADD_RES )
   if ( datIndInfo == NULLP  )
   {
      RLOG_ARG2(L_FATAL,DBG_RBID,rbCb->rlcId.rbId,
               "Memory allocation failed UEID:%d CELLID:%d",
               rbCb->rlcId.ueId,
               rbCb->rlcId.cellId);
      RLC_FREE_BUF(sdu);
      RETVALUE(RFAILED);
   }
#endif /* ERRCLASS & ERRCLS_ADD_RES */

   KW_MEM_CPY(&(datIndInfo->rlcId),&(rbCb->rlcId),sizeof(CmLteRlcId));
   /* Set the "isOutofSeq" flag for each packet 
    * If packets are in-sequence set flag as TRUE else FALSE */
   datIndInfo->isOutOfSeq = rbCb->m.amUl.isOutOfSeq; 
#endif /* KW_PDCP */
   
   /* If trace flag is enabled send the trace indication */
   if(gCb->init.trc == TRUE)
   {
      /* Populate the trace params */
      rlcLmmSendTrc(gCb,KWU_EVT_DAT_IND, sdu);
   }
#ifndef KW_PDCP

   rlcUiKwuDatInd(&gCb->genCfg.lmPst, datIndInfo, sdu);
#endif   
   RETVALUE(ROK);
} /* rlcUtlSndDatInd */


PRIVATE Void dumpRLCUlRbInformation(RlcUlRbCb* ulRbCb)
{
   if(ulRbCb->mode == CM_LTE_MODE_UM)
   {
      U32 i;
      U32 pdusInReceptionBuffer = 0;
      U32 windSz  = ulRbCb->m.umUl.umWinSz << 1;

      for(i = 0; i< windSz; i++)
      {
         if(ulRbCb->m.umUl.recBuf[i] != NULLP)
         {
            pdusInReceptionBuffer++;
         }
      }
      
      RLOG_ARG3(L_DEBUG,DBG_RBID,ulRbCb->rlcId.rbId,
               "UM UL UEID:%d CELLID:%d Reception Buffer size = %d", 
                       (int)ulRbCb->rlcId.ueId,
                       (int)ulRbCb->rlcId.cellId,
                       (int)pdusInReceptionBuffer);
   }
   else if(ulRbCb->mode == CM_LTE_MODE_AM)
   {
      U32 i;
      U32 pdusInReceptionBuffer = 0;
      U32 totalSegs = 0;
      U32 windSz  = KW_AM_GET_WIN_SZ(ulRbCb->m.amUl.snLen) << 1;
      
      for(i = 0; i< windSz; i++)
      {
         KwAmRecBuf *recBuf = kwUtlGetRecBuf(ulRbCb->m.amUl.recBufLst, i);
         if(recBuf != NULLP)
         {
            pdusInReceptionBuffer++;
            totalSegs += (recBuf->segLst.count);
         }
      }

      RLOG_ARG4(L_DEBUG,DBG_RBID,ulRbCb->rlcId.rbId,
                "AM UL UEID:%d CELLID:%d Reception Buf size = %d"
                "total segs = %d", 
                       (int)ulRbCb->rlcId.ueId,
                       (int)ulRbCb->rlcId.cellId,
                       (int)pdusInReceptionBuffer, 
                       (int)totalSegs);
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
      U32 i;
      for(i = 0; i< KW_MAX_SRB_PER_UE; i++)
      {
         RlcUlRbCb* ulRbCb = ueCb->srbCb[i]; 
         if(ulRbCb != NULLP)
         {
            dumpRLCUlRbInformation(ulRbCb);
         }
      }
      for(i = 0; i< KW_MAX_DRB_PER_UE; i++)
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
   U16             cntr;

   gCb->u.ulCb->kwL2Cb.kwNumMeas=0;
   for(cntr = 0; cntr < LKW_MAX_L2MEAS; cntr++)
   {
      cmMemset((U8 *)&(gCb->u.ulCb->kwL2Cb.kwL2EvtCb[cntr]), 0, sizeof(KwL2MeasEvtCb));
   }
   gCb->u.ulCb->kwL2Cb.kwL2EvtCb[KW_L2MEAS_UL_IP].measCb.measType = LKW_L2MEAS_UL_IP;
   RETVALUE(ROK);
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
#ifdef ANSI
 Void rlcUtlCalUlIpThrPutIncTTI
(
RlcCb                  *gCb,
RlcUlRbCb              *rbCb,
U32                   ttiCnt
)
#else
Void rlcUtlCalUlIpThrPutIncTTI(gCb, rbCb, ttiCnt)
RlcCb                  *gCb;
RlcUlRbCb              *rbCb;
U32                   ttiCnt;
#endif
{
   VOLATILE U32     startTime = 0;
   TRC2(rlcUtlCalUlIpThrPutIncTTI)

      /*starting Task*/
      SStartTask(&startTime, PID_RLC_IP_TPT_INCTTI);
#ifndef ALIGN_64BIT
   RLOG_ARG4(L_UNUSED, DBG_RBID,rbCb->rlcId.rbId,"Log for ul ip throughput:"
         "RB_MeasOn:%d ttiCnt :%ld UEID:%d CELLID:%d", 
         rbCb->rbL2Cb.measOn,ttiCnt,
         rbCb->rlcId.ueId,
         rbCb->rlcId.cellId);
#else
   RLOG_ARG4(L_UNUSED,DBG_RBID,rbCb->rlcId.rbId, "Log for ul ip throughput:"
         "RB_MeasOn:%d ttiCnt :%d UEID:%d CELLID:%d", 
         rbCb->rbL2Cb.measOn,ttiCnt,
         rbCb->rlcId.ueId,
         rbCb->rlcId.cellId);
#endif

   /*Check if UL IP throughput measurement is ON for this RB or not*/
   if(KW_MEAS_IS_UL_IP_MEAS_ON_FOR_RB(gCb,rbCb))              
   {
      if (TRUE  == rbCb->ueCb->isUlBurstActive)
      {
         if (ttiCnt < rbCb->l2MeasIpThruput.prevTtiCnt)
         {
            /*Removed Error Print*/
         }
         if (rbCb->l2MeasIpThruput.prevTtiCnt != 0)
         {
            rbCb->rbL2Cb.l2Sts[KW_L2MEAS_UL_IP]->ulIpThruput.timeSummation += 
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
#ifdef ANSI
 Void rlcUtlCalUlIpThrPut
(
RlcCb                  *gCb,
RlcUlRbCb              *rbCb,
Buffer                *pdu,
U32                   ttiCnt
)
#else
Void rlcUtlCalUlIpThrPut(gCb, rbCb, pdu, ttiCnt)
   RlcCb                  *gCb;
   RlcUlRbCb              *rbCb;
   Buffer                *pdu;
   U32                   ttiCnt;
#endif
{
   MsgLen        rlcSduSz = 0;  /*Holds length of Rlc Sdu*/
   VOLATILE U32     startTime = 0;
   TRC2(rlcUtlCalUlIpThrPut)


   /*starting Task*/
   SStartTask(&startTime, PID_RLC_IP_TPT_INCVOL);

   /*Check if UL IP throughput measurement is ON for this RB or not*/
   if(KW_MEAS_IS_UL_IP_MEAS_ON_FOR_RB(gCb, rbCb) &&              
         (TRUE  == rbCb->ueCb->isUlBurstActive) &&
         (rbCb->ueCb->firstPacketTTI) &&
         (ttiCnt != rbCb->ueCb->firstPacketTTI))
   {
      SFndLenMsg(pdu, &rlcSduSz);

      rbCb->rbL2Cb.l2Sts[KW_L2MEAS_UL_IP]->ulIpThruput.volSummation += rlcSduSz;

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

#ifdef ANSI
S16 rlcUtlHdlL2TmrExp
(
RlcCb          *gCb,
KwL2MeasEvtCb *measEvtCb
)
#else
S16 rlcUtlHdlL2TmrExp(measEvtCb)
RlcCb          *gCb;
KwL2MeasEvtCb *measEvtCb;
#endif
{
   TRC3(rlcUtlHdlL2TmrExp)

#ifdef LTE_L2_MEAS_RLC
   U16             qciIdx;
   KwL2MeasCb     *measCb;
   
   /* Clean up the RB data structures */
   if((measEvtCb->measCb.measType & LKW_L2MEAS_ACT_UE) &&
      (measEvtCb->measCb.val.nonIpThMeas.numSamples))
   {
      measCb = &measEvtCb->measCb;

      for(qciIdx = 0; qciIdx < measCb->val.nonIpThMeas.numQci;qciIdx++)
      {
         measCb->val.nonIpThMeas.measData[measCb->val.nonIpThMeas.qci[qciIdx]].actUe.numActvUe   +=   
                     rlcCb.kwL2Cb.numActUe[measCb->val.nonIpThMeas.qci[qciIdx]];
         measCb->val.nonIpThMeas.measData[measCb->val.nonIpThMeas.qci[qciIdx]].actUe.sampOc++;
      }
      measEvtCb->val.nonIpThMeas.measCb.numSamples--;
      rlcStartTmr(gCb, (PTR)measEvtCb, KW_EVT_L2_TMR); 
      RETVALUE(ROK);
   }
#endif

   rlcUtlSndUlL2MeasCfm(gCb, measEvtCb);

   RETVALUE(ROK);
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

#ifdef ANSI
S16 rlcUtlSndUlL2MeasCfm
(
RlcCb                  *gCb,
KwL2MeasEvtCb         *measEvtCb
)
#else
S16 rlcUtlSndUlL2MeasCfm(gCb, measEvtCb)
RlcCb                  *gCb;
KwL2MeasEvtCb         *measEvtCb;
#endif
{
   U32                     qciIdx;
   KwL2MeasCb              *measCb;
   KwL2MeasCfmEvt          measCfmEvt;

   U64                     ulDataVol;
   U64                     ulTime;
   U16                     cntr;
   /* Discard new changes starts */
   U8                      qci = 0;
   U32                     cfmIdx =0;
   /* Discard new changes ends */

   TRC3(rlcUtlSndUlL2MeasCfm)

   /* kw006.201 ccpu00120058 emoved 64 bit compilation warning */
#ifndef ALIGN_64BIT
   RLOG1(L_DEBUG,"rlcUtlSndUlL2MeasCfm(transId(%ld))", measEvtCb->transId);
#else
   RLOG1(L_DEBUG,"rlcUtlSndUlL2MeasCfm(transId(%d))", measEvtCb->transId);
#endif

   /* Clean up the RB data structures */
   measCb = &measEvtCb->measCb;
   
   cmMemset((U8*)&measCfmEvt, 0, sizeof(KwL2MeasCfmEvt));
   measCfmEvt.transId = measEvtCb->transId;

   measCfmEvt.measType = measCb->measType;
   measCfmEvt.status.status = LCM_PRIM_OK;
   measCfmEvt.status.reason = LCM_REASON_NOT_APPL;
   
   if( measCb->measType & LKW_L2MEAS_UL_IP)
   {
      KwL2MeasCbUeMeasInfo *pUeInfoLstCb  = measCb->val.ipThMeas.ueInfoLst;
      KwL2MeasCfmUeInfoLst *pUeInfoLstCfm = measCfmEvt.val.ipThMeas.ueInfoLst;
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
   KwMiLkwL2MeasCfm(&gCb->genCfg.lmPst, &measCfmEvt);
   RETVALUE(ROK);
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

#ifdef ANSI
S16 rlcUtlSndUlL2MeasNCfm
(
RlcCb           *gCb,
KwL2MeasReqEvt *measReqEvt,
KwL2MeasCfmEvt *measCfmEvt
)
#else
S16 rlcUtlSndUlL2MeasNCfm(gCb, measReqEvt, measCfmEvt)
RlcCb           *gCb;
KwL2MeasReqEvt *measReqEvt;
KwL2MeasCfmEvt *measCfmEvt;
#endif
{
   TRC3(rlcUtlSndUlL2MeasNCfm)

   KwMiLkwL2MeasCfm(&gCb->genCfg.lmPst, measCfmEvt);
   RETVALUE(ROK);
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

#ifdef ANSI
S16 rlcUtlValidateL2Meas
(
KwL2MeasReqEvt *measReqEvt,
KwL2MeasCfmEvt *measCfmEvt,
CmLteLcId      *lChId,
U8             *numLCh
)
#else
S16 rlcUtlValidateL2Meas(measReqEvt, measCfmEvt, lChId, numLCh)
KwL2MeasReqEvt *measReqEvt;
KwL2MeasCfmEvt *measCfmEvt;
CmLteLcId      *lChId;
U8             *numLCh;
#endif
{
   U8      measType;
   S16     ret;
   U8      qciIdx;
   U8      qci;
   U8      idx;
   U8      *qciVal;
   U8      numQci;
   KwRbCb  *rbCb;

   KwUeCb     *ueCb;
   RbCb       **rbCbLst;
   U8         rbIdx;
   U8         lsbNibble = 0;
   U8         msbNibble = 0;
   U8         numFaild = 0;


   TRC3(rlcUtlValidateL2Meas)
   
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
      RETVALUE(RFAILED);
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
      RETVALUE(RFAILED);
   }

   /* Check for total maximum number of Measurement Control Block */
   if(rlcCb.kwL2Cb.kwNumMeas >= LKW_MAX_L2MEAS )
   {
      measCfmEvt->transId = measReqEvt->transId;
      measCfmEvt->measType = measType;
      measCfmEvt->status.status = LCM_PRIM_NOK;
      measCfmEvt->status.reason = LKW_CAUSE_EXCEED_NUMMEAS;
      RETVALUE(RFAILED);
   }

   /* Check that number of samples should be a non-zero value */
   if(((measType & LKW_L2MEAS_ACT_UE) &&
      (measReqEvt->measReq.val.nonIpThMeas.numSamples == 0)))
   {
      measCfmEvt->transId = measReqEvt->transId;
      measCfmEvt->measType = measType;
      measCfmEvt->status.status = LCM_PRIM_NOK;
      measCfmEvt->status.reason = LKW_CAUSE_ZERO_NUMSAM;
      RETVALUE(RFAILED);
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
      RETVALUE(RFAILED);
   }
   {
      numQci = measReqEvt->measReq.val.nonIpThMeas.numQci;
      qciVal = measReqEvt->measReq.val.nonIpThMeas.qci;
   }
   /* Check whether qci is configured or not */
   for(qciIdx = 0; qciIdx < numQci; qciIdx++)
   {
      qci = qciVal[qciIdx];
      ret = cmHashListFind(&(rlcCb.kwL2Cb.qciHlCp), 
            (U8 *)&qci, (U16)sizeof(qci), 0, (PTR *)&rbCb);
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
      RETVALUE(RFAILED);
   }
   {
      for(qciIdx = 0; qciIdx < numQci; qciIdx++)
      {
         if(rlcCb.kwL2Cb.measOn[qci] & measReqEvt->measReq.measType)
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
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}/* rlcUtlValidateL2Meas */
#endif

#ifdef ANSI
S16 rlcUtlValidateIpThL2Meas
(
KwL2MeasReqEvt *measReqEvt,
KwL2MeasCfmEvt *measCfmEvt
)
#else
S16 rlcUtlValidateIpThL2Meas(measReqEvt, measCfmEvt)
KwL2MeasReqEvt *measReqEvt;
KwL2MeasCfmEvt *measCfmEvt;
#endif
{
   U8      measType;
   U8         lsbNibble = 0;
   U8         msbNibble = 0;

   TRC3(rlcUtlValidateIpThL2Meas)
   
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
      RETVALUE(RFAILED);
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
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
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
#ifdef ANSI

Void rlcUtlResetUlL2MeasInKwRb
(
RlcCb       *gCb,
KwL2MeasCb *measCb,
U8             measType
)
#else
Void rlcUtlResetUlL2MeasInKwRb(measCb, measType)
RlcCb       *gCb;
KwL2MeasCb *measCb;
U8             measType;
#endif
{
   U32           rbIdx;
   U32           ueIdx;
   U32           qciIdx;
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

            for (rbIdx = 0; rbIdx < KW_MAX_DRB_PER_UE; rbIdx++)
            {
               if (ueCb->drbCb[rbIdx])
               {
                  ueCb->drbCb[rbIdx]->rbL2Cb.measOn &= ~measType;
               }
            }
         }
      }
   }
} /* rlcUtlResetUlL2MeasInKwRb */

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
#ifdef ANSI
Void rlcUtlPlcMeasDatInL2Sts
(
KwL2Cntr       *measData, 
KwL2MeasRbCb   *rbL2Cb,
U8             measType
)
#else
Void rlcUtlPlcMeasDatInL2Sts(measData, rbL2Cb, measType)
KwL2Cntr       *measData; 
KwL2MeasRbCb   *rbL2Cb;
U8             measType;
#endif
{
   TRC3(rlcUtlPlcMeasDatInL2Sts)
   
   /* We should check the number of measType in the request. This can be done 
    * by looking at each bit in the measType. Also store the measData in the 
    * correct index of l2Sts in RbCb.
    * */

    if(measType & LKW_L2MEAS_ACT_UE)
    {
         rbL2Cb->l2Sts[KW_L2MEAS_ACT_UE] = measData;
    }
    if(measType & LKW_L2MEAS_UU_LOSS)
    {
         rbL2Cb->l2Sts[KW_L2MEAS_UU_LOSS] = measData;
    }
    if(measType & LKW_L2MEAS_DL_IP )
    {
         rbL2Cb->l2Sts[KW_L2MEAS_DL_IP] = measData;
    }
    if(measType & LKW_L2MEAS_UL_IP)
    {
         rbL2Cb->l2Sts[KW_L2MEAS_UL_IP] = measData;
    }
    if(measType & LKW_L2MEAS_DL_DISC)
    {
         rbL2Cb->l2Sts[KW_L2MEAS_DL_DISC] = measData;
    }
    if(measType & LKW_L2MEAS_DL_DELAY)
    {
         rbL2Cb->l2Sts[KW_L2MEAS_DL_DELAY] = measData;
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
#ifdef ANSI
Void rlcUtlStoreRecBuf 
(
CmLListCp        *recBufLst,
KwAmRecBuf       *recBuf,
KwSn              sn
)
#else
Void rlcUtlStoreRecBuf(recBufLst, recBuf, sn)
CmLListCp        *recBufLst;
KwAmRecBuf       *recBuf;
KwSn              sn;
#endif
{
   U32             hashKey; 

   TRC3(rlcUtlStoreRecBuf)
   
   hashKey = (sn % KW_RCV_BUF_BIN_SIZE ); 
   recBuf->lnk.node = (PTR)recBuf;
   cmLListAdd2Tail(&(recBufLst[hashKey]), &recBuf->lnk);

   RETVOID;
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
#ifdef ANSI
KwAmRecBuf* kwUtlGetRecBuf 
(
CmLListCp        *recBufLst,
KwSn             sn
)
#else
KwAmRecBuf* kwUtlGetRecBuf(recBufLst, sn)
CmLListCp        *recBufLst;
KwSn             sn;
#endif
{
   U32                 hashKey; 
   CmLListCp           *recBufLstCp;
   KwAmRecBuf          *recBuf;
   CmLList             *node = NULLP;

   TRC3(kwUtlGetRecBuf)

   hashKey = (sn % KW_RCV_BUF_BIN_SIZE ); 
 
   recBufLstCp = &recBufLst[hashKey];
   CM_LLIST_FIRST_NODE(recBufLstCp, node);
   while(node)
   {
      recBuf = (KwAmRecBuf *) node->node;
      if(recBuf->amHdr.sn == sn)
      {
         RETVALUE(recBuf);
      }
      CM_LLIST_NEXT_NODE(recBufLstCp, node);
   }
   RETVALUE(NULLP);
} /* rlcUtlStoreRecBuf */
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
#ifdef ANSI
Void rlcUtlDelRecBuf 
(
CmLListCp        *recBufLst,
KwAmRecBuf       *recBuf,
RlcCb              *gCb                              
)
#else
Void rlcUtlDelRecBuf(recBufLst, recBufi, gCb)
CmLListCp        *recBufLst;
KwAmRecBuf       *recBuf;
RlcCb             *gCb;                              
#endif
{
   U32                 hashKey; 
   CmLListCp           *recBufLstCp;

   TRC3(rlcUtlDelRecBuf)

   hashKey = (recBuf->amHdr.sn % KW_RCV_BUF_BIN_SIZE ); 
 
   recBufLstCp = &recBufLst[hashKey];
   cmLListDelFrm(recBufLstCp, &recBuf->lnk);
   RLC_FREE_WC(gCb, recBuf, sizeof(KwAmRecBuf));

   RETVOID;
} /* rlcUtlDelRecBuf */




/********************************************************************30**
         End of file
**********************************************************************/
