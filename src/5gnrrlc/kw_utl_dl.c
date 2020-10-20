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

                  --rlcUtlSendToMac
                  --rlcUtlRcvFrmMac
                  --rlcUtlEmptySduQ
                  --rlcUtlSendDedLcBoStatus
                  --rlcUtlSendUlDataToDu 
                  --kwUtlShutDown

     File:     kw_utl_dl.c

**********************************************************************/
static const char* RLOG_MODULE_NAME="UTL";
static int RLOG_MODULE_ID=2048;
static int RLOG_FILE_ID=209;

/** @file kw_utl_dl.c
@brief RLC Utility Module 
*/

/* header (.h) include files */

#include "common_def.h"
/* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
#include <sys/time.h>
#endif
#include "ckw.h"           /* CKW defines */
#include "kwu.h"           /* CKW defines */
#include "lkw.h"           /* LKW defines */
#include "rgu.h"           /* RGU defiens */

#include "kw_env.h"        /* RLC environment options */
#include "kw.h"            /* RLC defines */
#include "kw_err.h"        /* Error defines */
#include "kw_udx.h"
#include "kw_dl.h"

/* extern (.x) include files */
#include "ckw.x"           /* CKW includes */
#include "kwu.x"           /* KWU includes */
#include "lkw.x"           /* LKW inlcudes */
#include "rgu.x"           /* RGU includes */

#include "kw.x"            /* RLC includes */
#include "kw_udx.x"        /* UDX interface includes */
#include "kw_dl.x"         /* RLC downlink includes */

#include "rlc_utils.h"
#include "rlc_mac_inf.h"
#include "rlc_lwr_inf_api.h"

#include "ss_rbuf.h"
#include "ss_rbuf.x" 

SsRngBufCnt rngCb;

#if (defined(MAC_RLC_HARQ_STA_RBUF) && defined(LTE_L2_MEAS))
uint32_t isDatReqProcessed;
#endif
#define RLC_MODULE (RLC_DBGMASK_DUT | RLC_DBGMASK_DL) /* for debugging purpose */
#if (defined(MAC_RLC_HARQ_STA_RBUF) && defined(LTE_L2_MEAS)) || defined (SS_RBUF)
void rlcUtlDlBatchProcHqStaInd ARGS ((Void));
#endif
Void ResetRLCStats(Void)
{
   RlcCb* dlInst = rlcCb[1]; 
   RlcCb* ulInst = rlcCb[0]; 
   memset(&gRlcStats, 0, sizeof(RLCStats));
   memset(&dlInst->genSts,0,sizeof(RlcGenSts));
   memset(&ulInst->genSts,0,sizeof(RlcGenSts));
}

#ifndef ALIGN_64BIT
Void PrintRLCStats(Void)
{
   RlcCb* dlInst = rlcCb[1]; 
   RlcCb* ulInst = rlcCb[0]; 

   RLOG4(L_ALWAYS,"RLC Stats: PDUs Sent = (%ld), PdusRext = (%ld), TimeOut = (%ld), SduDiscarded = (%ld)",
                     dlInst->genSts.pdusSent,
                     dlInst->genSts.pdusRetx,
                     dlInst->genSts.protTimeOut,
                     dlInst->genSts.numSduDisc);
   RLOG3(L_ALWAYS,"RLC Stats: PDUs Rcvd = (%ld), unexpPdus = (%ld), errorPdus = (%ld)",
                     ulInst->genSts.pdusRecv,
                     ulInst->genSts.unexpPdusRecv,
                     ulInst->genSts.errorPdusRecv);
   RLOG4(L_ALWAYS,"RLC Stats: AMDL: "
                     "StaPduSent:%lu NacksInStaPdu:%lu BytesUnused:%lu PollTimerExpires SRB:%lu ",
                     gRlcStats.amRlcStats.numDLStaPduSent, gRlcStats.amRlcStats.numDLNacksInStaPdu, 
                     gRlcStats.amRlcStats.numDLBytesUnused, gRlcStats.amRlcStats.numDLPollTimerExpiresSrb);
   RLOG3(L_ALWAYS,"RLC Stats: AMDL: "
                     "DRB:%lu MaxRetx:%lu RetransPdus:%lu", 
                     gRlcStats.amRlcStats.numDLPollTimerExpiresDrb, gRlcStats.amRlcStats.numDLMaxRetx, 
                     gRlcStats.amRlcStats.numDLRetransPdus);
   RLOG4(L_ALWAYS,"RLC Stats: AMUL: "
                     " PdusDiscarded:%lu ReOrdTimerExpires:%lu StaPduRcvd:%lu NackInStaPduRcvd:%lu ", 
                     gRlcStats.amRlcStats.numULPdusDiscarded, gRlcStats.amRlcStats.numULReOrdTimerExpires, 
                     gRlcStats.amRlcStats.numULStaPduRcvd, gRlcStats.amRlcStats.numULNackInStaPduRcvd);

   RTLIN_DUMP_DEBUG("RLC Stats: PDUs Sent = (%ld), PdusRext = (%ld), TimeOut = (%ld), SduDiscarded = (%ld)\n",
                     dlInst->genSts.pdusSent,
                     dlInst->genSts.pdusRetx,
                     dlInst->genSts.protTimeOut,
                     dlInst->genSts.numSduDisc);
   RTLIN_DUMP_DEBUG("RLC Stats: PDUs Rcvd = (%ld), unexpPdus = (%ld), errorPdus = (%ld)\n",
                     ulInst->genSts.pdusRecv,
                     ulInst->genSts.unexpPdusRecv,
                     ulInst->genSts.errorPdusRecv);
   RTLIN_DUMP_DEBUG("RLC Stats:"
                    "AMDL: StaPduSent:%lu NacksInStaPdu:%lu BytesUnused:%lu PollTimerExpires SRB:%lu DRB:%lu MaxRetx:%lu RetransPdus:%lu \n"
                    "AMUL: PdusDiscarded:%lu ReOrdTimerExpires:%lu StaPduRcvd:%lu NackInStaPduRcvd:%lu \n", 
                     gRlcStats.amRlcStats.numDLStaPduSent, gRlcStats.amRlcStats.numDLNacksInStaPdu, gRlcStats.amRlcStats.numDLBytesUnused, 
                     gRlcStats.amRlcStats.numDLPollTimerExpiresSrb, gRlcStats.amRlcStats.numDLPollTimerExpiresDrb, gRlcStats.amRlcStats.numDLMaxRetx, 
                     gRlcStats.amRlcStats.numDLRetransPdus, gRlcStats.amRlcStats.numULPdusDiscarded, gRlcStats.amRlcStats.numULReOrdTimerExpires, 
                     gRlcStats.amRlcStats.numULStaPduRcvd, gRlcStats.amRlcStats.numULNackInStaPduRcvd);
}
#else
Void PrintRLCStats(Void)
{
   RlcCb* dlInst = rlcCb[1]; 
   RlcCb* ulInst = rlcCb[0]; 

   printf ("\n================================ RLC STATS ===========================\n");
   RLOG4(L_ALWAYS,"RLC Stats: PDUs Sent = (%ld), PdusRext = (%ld), TimeOut = (%ld), SduDiscarded = (%ld)",
                     dlInst->genSts.pdusSent,
                     dlInst->genSts.pdusRetx,
                     dlInst->genSts.protTimeOut,
                     dlInst->genSts.numSduDisc);
   RLOG3(L_ALWAYS,"RLC Stats: PDUs Rcvd = (%ld), unexpPdus = (%ld), errorPdus = (%ld)",
                     ulInst->genSts.pdusRecv,
                     ulInst->genSts.unexpPdusRecv,
                     ulInst->genSts.errorPdusRecv);
   RLOG4(L_ALWAYS,"RLC Stats: AMDL: "
                     "StaPduSent:%lu NacksInStaPdu:%lu BytesUnused:%lu PollTimerExpires SRB:%lu ",
                     gRlcStats.amRlcStats.numDLStaPduSent, gRlcStats.amRlcStats.numDLNacksInStaPdu, 
                     gRlcStats.amRlcStats.numDLBytesUnused, gRlcStats.amRlcStats.numDLPollTimerExpiresSrb);
   RLOG3(L_ALWAYS,"RLC Stats: AMDL: "
                     "DRB:%lu MaxRetx:%lu RetransPdus:%lu", 
                     gRlcStats.amRlcStats.numDLPollTimerExpiresDrb, gRlcStats.amRlcStats.numDLMaxRetx, 
                     gRlcStats.amRlcStats.numDLRetransPdus);
   RLOG4(L_ALWAYS,"RLC Stats: AMUL: "
                     " PdusDiscarded:%lu ReOrdTimerExpires:%lu StaPduRcvd:%lu NackInStaPduRcvd:%lu ", 
                     gRlcStats.amRlcStats.numULPdusDiscarded, gRlcStats.amRlcStats.numULReOrdTimerExpires, 
                     gRlcStats.amRlcStats.numULStaPduRcvd, gRlcStats.amRlcStats.numULNackInStaPduRcvd);
 /*  RTLIN_DUMP_DEBUG("AM RLC Stats:"
                    "AMDL: SDUs Tx :(%u) SDU Bytes Tx :(%u) SDUs Retx :(%u) MaxRetx:(%u) WindowStalls: (%u) \n"
                    "AMUL: DropOutWinRx :(%u) SDUs Rx :(%u) SDU Bytes Rx :(%u) SDUNack Rx :(%u) Duplicate Pdu Rx :(%u) \n",
                     gRlcStats.amRlcStats.numRlcAmCellSduTx, gRlcStats.amRlcStats.numRlcAmCellSduBytesTx,
                     gRlcStats.amRlcStats.numRlcAmCellRetxPdu, gRlcStats.amRlcStats.numRlcAmMaxRetx, gRlcStats.amRlcStats.numRlcAmCellWinStall,
                     gRlcStats.amRlcStats.numRlcAmCellDropOutWinRx, gRlcStats.amRlcStats.numRlcAmCellSduRx,
                     gRlcStats.amRlcStats.numRlcAmCellSduBytesRx, gRlcStats.amRlcStats.numRlcAmCellNackRx, gRlcStats.amRlcStats.numRlcAmCellDupPduRx);
*/
   RTLIN_DUMP_DEBUG("RLC Stats: PDUs Sent = (%d), PdusRext = (%d), TimeOut = (%d), SduDiscarded = (%d)\n",
                     dlInst->genSts.pdusSent,
                     dlInst->genSts.pdusRetx,
                     dlInst->genSts.protTimeOut,
                     dlInst->genSts.numSduDisc);
   RTLIN_DUMP_DEBUG("RLC Stats: PDUs Rcvd = (%d), unexpPdus = (%d), errorPdus = (%d)\n",
                     ulInst->genSts.pdusRecv,
                     ulInst->genSts.unexpPdusRecv,
                     ulInst->genSts.errorPdusRecv);
   RTLIN_DUMP_DEBUG("AMDL: StaPduSent:%u NacksInStaPdu:%u BytesUnused:%u PollTimerExpires SRB:%u DRB:%u MaxRetx:%u RetransPdus:%u \n"
                    "      SDUs Tx :(%u) SDU Bytes Tx :(%u) SDUs Retx :(%u) WindowStalls: (%u) \n"

                    "AMUL: PdusDiscarded:%u ReOrdTimerExpires:%u StaPduRcvd:%u NackInStaPduRcvd:%u \n"
                    "      DropOutWinRx :(%u) SDUs Rx :(%u) SDU Bytes Rx :(%u) SDUNack Rx :(%u) Duplicate Pdu Rx:(%u) \n", 
                     gRlcStats.amRlcStats.numDLStaPduSent, gRlcStats.amRlcStats.numDLNacksInStaPdu, gRlcStats.amRlcStats.numDLBytesUnused, 
                     gRlcStats.amRlcStats.numDLPollTimerExpiresSrb, gRlcStats.amRlcStats.numDLPollTimerExpiresDrb,
                     gRlcStats.amRlcStats.numDLMaxRetx, gRlcStats.amRlcStats.numDLRetransPdus,
                     gRlcStats.amRlcStats.numRlcAmCellSduTx, gRlcStats.amRlcStats.numRlcAmCellSduBytesTx,
                     gRlcStats.amRlcStats.numRlcAmCellRetxPdu, gRlcStats.amRlcStats.numRlcAmCellWinStall,
                     gRlcStats.amRlcStats.numULPdusDiscarded, gRlcStats.amRlcStats.numULReOrdTimerExpires, 
                     gRlcStats.amRlcStats.numULStaPduRcvd, gRlcStats.amRlcStats.numULNackInStaPduRcvd,
                     gRlcStats.amRlcStats.numRlcAmCellDropOutWinRx, gRlcStats.amRlcStats.numRlcAmCellSduRx,
                     gRlcStats.amRlcStats.numRlcAmCellSduBytesRx, gRlcStats.amRlcStats.numRlcAmCellNackRx, gRlcStats.amRlcStats.numRlcAmCellDupPduRx);
}
#endif

/*******************************************************************
 *
 * @brief 
 *    Handler for storing all DL PDU Info into RLC-MAC interface
 *    struct and sending to lower interface
 *
 * @details
 *    This function stores DL PDU info for all logical channels
 *    of per UE grant per TTI and sends to MAC
 *
 *    Function : rlcSendDedLcDlData 
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t rlcSendDedLcDlData(Pst *post, SpId spId, RguDDatReqInfo *datReqInfo)
{
   uint8_t          ueIdx;      /* UE info list iterator */ 
   uint8_t          tbIdx;      /* TB info list iterator */
   uint8_t          lchIdx;     /* Lch info list iterator */
   uint8_t          pduIdx;     /* RLC PDU list iterator */
   RguDDatReqPerUe  datPerUe;   /* DL data info per UE */
   RguDatReqTb      datPerTb;   /* DL data info per TB */
   RguLchDatReq     datPerLch;  /* DL data info per Lch */
   RlcData          *dlData;    /* DL data to be sent to MAC */
   Pst              pst;        /* Post structure */
   uint16_t         pduLen;     /* PDU length */
   uint16_t         copyLen;    /* Number of bytes copied */

   dlData = NULLP;
   RLC_ALLOC_SHRABL_BUF(RLC_MEM_REGION_DL, RLC_POOL,
                       dlData, sizeof(RlcData));
#if (ERRCLASS & ERRCLS_ADD_RES)
   if ( dlData == NULLP )
   {
      DU_LOG("\nRLC: rlcSendDedLcDlData: Memory allocation failed for dl data");
      RLC_FREE_SHRABL_BUF(RLC_MEM_REGION_DL, RLC_POOL,
          datReqInfo, sizeof(RguDDatReqInfo));
      return RFAILED;
   }
#endif /* ERRCLASS & ERRCLS_ADD_RES */

   for(ueIdx = 0; ueIdx < datReqInfo->nmbOfUeGrantPerTti; ueIdx++)
   {
      datPerUe = datReqInfo->datReq[ueIdx];

      memset(dlData, 0, sizeof(RlcData));

      dlData->cellId = datReqInfo->cellId;
      dlData->rnti = datPerUe.rnti;

      /* Retrieving sfn/slot from transId. It was filled in RlcProcSchedResultRpt */
      dlData->slotInfo.sfn = datPerUe.transId >> 16;
      dlData->slotInfo.slot = datPerUe.transId & 0xffff;
      dlData->numPdu = 0;

      for(tbIdx = 0; tbIdx < datPerUe.nmbOfTbs; tbIdx++)
      {
         datPerTb = datPerUe.datReqTb[tbIdx];
         for(lchIdx = 0; lchIdx < datPerTb.nmbLch; lchIdx++)
         {
            datPerLch = datPerTb.lchData[lchIdx];
            for(pduIdx = 0; pduIdx < datPerLch.pdu.numPdu; pduIdx++)
            {
               dlData->pduInfo[dlData->numPdu].commCh = FALSE;
               dlData->pduInfo[dlData->numPdu].lcId = datPerLch.lcId;

               /* Copy Message to fixed buffer to send */
	       ODU_GET_MSG_LEN(datPerLch.pdu.mBuf[pduIdx], (MsgLen *)&pduLen);
	       RLC_ALLOC_SHRABL_BUF(RLC_MEM_REGION_DL, RLC_POOL,
	          dlData->pduInfo[dlData->numPdu].pduBuf, pduLen);

	       if (dlData->pduInfo[dlData->numPdu].pduBuf == NULLP )
	       {
	          DU_LOG("\nRLC: rlcSendDedLcDlData: Memory allocation failed");
		  for(pduIdx = 0; pduIdx < dlData->numPdu; pduIdx++)
		  {
		     RLC_FREE_SHRABL_BUF(pst.region, pst.pool, dlData->pduInfo[pduIdx].pduBuf,\
		       dlData->pduInfo[pduIdx].pduLen);
		  }
		  RLC_FREE_SHRABL_BUF(RLC_MEM_REGION_DL, RLC_POOL,
		     dlData, sizeof(RlcData));
		  RLC_FREE_SHRABL_BUF(RLC_MEM_REGION_DL, RLC_POOL,
		     datReqInfo, sizeof(RguDDatReqInfo));
		  return RFAILED;
	       }

               ODU_COPY_MSG_TO_FIX_BUF(datPerLch.pdu.mBuf[pduIdx], 0, pduLen, \
	          dlData->pduInfo[dlData->numPdu].pduBuf, (MsgLen *)&copyLen);
	       dlData->pduInfo[dlData->numPdu].pduLen = pduLen;

               /* Free message */
	       ODU_PUT_MSG_BUF(datPerLch.pdu.mBuf[pduIdx]);

               dlData->numPdu++;
            }/* For per PDU */
         }/* For Data per Lch */
      }/* For Data per Tb */

      /* Sending DL Data per UE to MAC */
      memset(&pst, 0, sizeof(Pst));
      FILL_PST_RLC_TO_MAC(pst, RLC_DL_INST, EVENT_DL_DATA_TO_MAC);
      if(RlcSendDlDataToMac(&pst, dlData) != ROK)
      {
         for(pduIdx = 0; pduIdx < dlData->numPdu; pduIdx++)
	 {
	    RLC_FREE_SHRABL_BUF(pst.region, pst.pool, dlData->pduInfo[pduIdx].pduBuf,\
	       dlData->pduInfo[pduIdx].pduLen);
	 }
         RLC_FREE_SHRABL_BUF(pst.region, pst.pool, dlData, sizeof(RlcData));
      }
   } /* For Data per UE */

   RLC_FREE_SHRABL_BUF(RLC_MEM_REGION_DL, RLC_POOL,
            datReqInfo, sizeof(RguDDatReqInfo));
   
   return ROK;
}/* End of rlcSendDedLcDlData */

/**
 *
 * @brief 
 *        Handler for sending the data to multiple logical channels of a UE
 *
 * @details:
 *        This function sends the data for one or more logical channels
 *        after processing the SDUs and forming the PDUs.It calls
 *        UMM or AMM functions to form the PDUs for the requested sizes
 *        by MAC. 
 *
 * @param[in] gCb          RLC instance Control block 
 * @param[in] staIndInfo   Status Indication Information containing the 
 *                         size of PDU(s) for one or more logical channels  
 *
 * @return  S16
 *      -# ROK 
 *      -# RFAILED
 *
 */
uint8_t rlcUtlSendToMac(RlcCb *gCb, SuId suId, KwDStaIndInfo *staIndInfo)
{
   uint8_t           numPdu = 0;
   uint16_t          ueIdx;
   RlcDlUeCb         *ueCb;         /* UE control block */
   uint32_t          count;         /* Loop Counter */
   uint32_t          numTb;         /* Number of Tbs */
   RlcDlRbCb         *rbCb;         /* RB Control Block */
   RlcDatReq         datReq;        /* PDUs Information */
   RguDDatReqInfo   *datReqInfo;   /* Data Request Information */
   RlcRguSapCb      *rguSap;       /* MAC SAP CB */
   uint32_t         totNumPdu;     /* Total number of PDUS */
   RguStaIndTb      *staIndTb = NULLP;
   RguDatReqTb      *datReqTb = NULLP;
   RguDStaIndPerUe  *staInd = NULLP;
#ifdef LTE_L2_MEAS   
   uint32_t   grantPerLch[RGU_MAX_LC] = {0};
#endif
/* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS_LOSS_DELAY
   uint8_t        snIdx1;
   uint8_t        snIdx2;
#endif /* LTE_L2_MEAS */
   uint32_t  idx;

//Debug
   uint32_t staIndSz=0,datIndSz = 0;

   datReqInfo = NULLP;
   RLC_ALLOC_SHRABL_BUF(RLC_MEM_REGION_DL, RLC_POOL,
                       datReqInfo,sizeof(RguDDatReqInfo));
#if (ERRCLASS & ERRCLS_ADD_RES)
      if ( datReqInfo == NULLP )
      {
         DU_LOG("\nRLC: rlcUtlSendToMac: Memory allocation failed");
         return RFAILED;
      }
#endif /* ERRCLASS & ERRCLS_ADD_RES */
   for(idx = 0; idx < staIndInfo->nmbOfUeGrantPerTti; idx++)
   {
      staInd = &staIndInfo->staInd[idx];
      /* Fetch Ue control block */
      GET_UE_IDX(staInd->rnti, ueIdx);
      if(ROK != rlcDbmFetchDlUeCb(gCb, ueIdx, staIndInfo->cellId,&ueCb))
      {
         /* Fetch UeCb failed */
         DU_LOG("\nRLC: rlcUtlSendToMac: UeId[%u]:ueCb not found",
            staInd->rnti);
         /* If ueCb is not found for current rnti then continue to look for next rnti*/
         continue; 
      }
      /* kw002.201 Removed the allocation of RlcDatReq */
      /* kw004.201 Used SSI function to initialize the variable */
      memset(&datReq, 0, sizeof(RlcDatReq) ); 
      totNumPdu = 0;
      for (numTb = 0; numTb < staInd->nmbOfTbs; numTb++)
      {
         staIndTb = &(staInd->staIndTb[numTb]);
         datReqTb = &(datReqInfo->datReq[idx].datReqTb[numTb]);
#ifdef LTE_L2_MEAS
         ueCb->tbIdx = (ueCb->tbIdx+1) % RLC_MAX_TB_PER_UE;
#endif   
         for (count = 0;count < staIndTb->nmbLch; count++)
         {
#ifdef LTE_L2_MEAS
            /*Calculate the total grant size from MAC */
            if((staIndTb->lchStaInd[count].lcId >= RGU_MAX_LC) 
                || (staIndTb->lchStaInd[count].lcId == 0))
            {
               /* TODO : Need to figure out why this is happening */
               break;
            }
            else
            {
               grantPerLch[staIndTb->lchStaInd[count].lcId] += staIndTb->lchStaInd[count].totBufSize;
            }
#endif   
            rbCb = ueCb->lCh[staIndTb->lchStaInd[count].lcId - 1].dlRbCb;

            if (rbCb && (!rlcDlUtlIsReestInProgress(rbCb)))
            { 
//Debug
               staIndSz += staIndTb->lchStaInd[count].totBufSize;
               datReq.pduSz = staIndTb->lchStaInd[count].totBufSize;
#ifdef LTE_L2_MEAS            
               datReq.totMacGrant = grantPerLch[staIndTb->lchStaInd[count].lcId];
#endif
               rlcUtlGetCurrTime(&datReq.boRep.oldestSduArrTime);
               if ( CM_LTE_MODE_UM == rbCb->mode )
               {
                  rlcUmmProcessSdus(gCb,rbCb,&datReq);
               }
               else if ( CM_LTE_MODE_AM == rbCb->mode )
               {
                  rlcAmmProcessSdus(gCb,rbCb,&datReq,staInd->fillCtrlPdu);
               }
#ifdef LTE_L2_MEAS            
               grantPerLch[staIndTb->lchStaInd[count].lcId] = datReq.totMacGrant;
#endif
               if ( 0 == datReq.pduInfo.numPdu )
               {
                  continue;
               }
               totNumPdu += datReq.pduInfo.numPdu;
               memcpy(&(datReqTb->lchData[count].pdu),
                  &(datReq.pduInfo),sizeof(KwPduInfo));

               for (;numPdu < datReqTb->lchData[count].pdu.numPdu ; numPdu ++)
               {
                  MsgLen len = 0;
                  ODU_GET_MSG_LEN(datReqTb->lchData[count].pdu.mBuf[numPdu],&len);
                  datIndSz += len; 
               }
               datReqTb->lchData[count].setMaxUlPrio = FALSE;
               if (RLC_AM_IS_POLL_BIT_SET(AMDL) && 
                   (AMDL.sduQ.count > 1))
               { 
                  /* Poll bit is set indicate to MAC*/
                  datReqTb->lchData[count].setMaxUlPrio = TRUE;
               }
               datReqTb->lchData[count].boReport.bo = datReq.boRep.bo;

#ifdef CCPU_OPT
               datReqTb->lchData[count].boReport.estRlcHdrSz = 
                  datReq.boRep.estHdrSz;
               datReqTb->lchData[count].boReport.staPduPrsnt = 
                  datReq.boRep.staPduPrsnt;
#endif /* CCPU_OPT */
               datReqTb->lchData[count].boReport.staPduBo = 
                  datReq.boRep.staPduBo;
               datReqTb->lchData[count].lcId = staIndTb->lchStaInd[count].lcId;

#ifdef L2_OPTMZ
               /* Set if Bearer is UM */
               if ( CM_LTE_MODE_UM == rbCb->mode )
               {
                  datReqTb->lchData[count].freeBuff = TRUE;
               }
               else
               {
                  datReqTb->lchData[count].freeBuff = FALSE;
               }
#endif

               /* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS_LOSS_DELAY
               datReqTb->rguSnInfo->lchMap[count].lChId =
                  staIndTb->lchStaInd[count].lcId;
               /* In some cases L2 Measurement for few of the lcId may be off, 
                * in this case we are assigning snList to 0xffff 
                */
               for(snIdx1 = 0; snIdx1 < RGU_MAX_PDU; snIdx1++)
               {
                  datReqTb->rguSnInfo->lchMap[count].snList[snIdx1] = 0xffff;
               }
               if(tbSnMap->numSn != 0)
               {
                  snIdx2 = 0;
                  for(snIdx1=tbSnMap->prevNumSn;snIdx1 < tbSnMap->numSn;snIdx1++)
                  {
                     datReqTb->rguSnInfo->lchMap[count].snList[snIdx2++] = 
                        tbSnMap->snSduMap[snIdx1].sn;
                  }
                  tbSnMap->prevNumSn = tbSnMap->numSn;
               }
#endif
               datReqTb->lchData[count].boReport.oldestSduArrTime = 
                                             datReq.boRep.oldestSduArrTime;
               /* kw004.201 Used SSI function to initialize the variable */
               memset(&datReq, 0, sizeof(RlcDatReq) ); 
            }
         }
#ifdef LTE_L2_MEAS
         if(ueCb->l2MeasTbCb[ueCb->tbIdx]!= NULLP)
         {
            datReqTb->tbId = ueCb->tbIdx;
         }
         else
         {
            datReqTb->tbId = RLC_INVALID_TBID; 
         }
#endif
         datReqTb->nmbLch = staIndTb->nmbLch;
         /*adding the check to make sure that lcId is not sent as 0
          * when no data is there in datReq */
         if ( 0 == totNumPdu )
         {
            datReqTb->lchData[0].lcId = staIndTb->lchStaInd[0].lcId;
         }
         /* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS_LOSS_DELAY
         if(tbSnMap->numSn == 0)
         {
            RLC_FREE(tbSnMap,sizeof(RlcTbSnMap));
            RLC_FREE(datReqTb->rguSnInfo,sizeof(RguSnMapInfo));
            datReqTb->rguSnInfo = NULLP;
            rlcCb.rlcL2Cb.curTbSnMap = NULLP;
            datReqTb->snMapPres = FALSE;
         }
         else
         {
            cmHashListInsert(&(rlcCb.rlcL2Cb.tbHlCp),(PTR)tbSnMap,
                  (uint8_t *) &(tbSnMap->tbId), (uint16_t)sizeof(tbSnMap->tbId));
            rlcCb.rlcL2Cb.curTbSnMap = NULLP;
         }
#endif /* LTE_L2_MEAS */
      }
      datReqInfo->datReq[idx].nmbOfTbs = staInd->nmbOfTbs;
      datReqInfo->datReq[idx].transId = staInd->transId;
      datReqInfo->datReq[idx].rnti    = staInd->rnti;
   }
   datReqInfo->cellId  = staIndInfo->cellId;
   datReqInfo->nmbOfUeGrantPerTti = staIndInfo->nmbOfUeGrantPerTti;

   rguSap = &(gCb->u.dlCb->rguDlSap[suId]);
   rlcSendDedLcDlData(&rguSap->pst,rguSap->spId,datReqInfo); 
   return ROK;
}

/**
 *
 * @brief 
 *        Handler for sending Status Response to MAC. 
 *
 * @detail:
 *        This function is used by  RLC entity for sending
 *        status response to MAC after receiving a SDU from
 *        PDCP.
 *
 * @param[in]  gCb           RLC instance Control block
 * @param[in]  rbCb          Radio Bearer Control Block 
 * @param[in]  bo            Buffer Occupancy
 * @param[in]  estHdrSz      Estimated Header Size
 * @param[in]  staPduPrsnt   Status PDU present or not
 *
 * @return  S16
 *      -# ROK 
 *      -# RFAILED
 */
uint8_t rlcUtlSendDedLcBoStatus(RlcCb *gCb, RlcDlRbCb *rbCb, int32_t bo, \
   int32_t estHdrSz, bool staPduPrsnt, uint32_t staPduBo)
{
   Pst           pst;           /* Post info */
   RlcBoStatus   *boStatus;      /* Buffer occupancy status information */

#ifndef TENB_ACC
   if ((rbCb->lastRprtdBoToMac > (uint32_t)8000) && (rbCb->boUnRprtdCnt < (uint32_t)5) 
       && (!staPduPrsnt) && ((CM_LTE_MODE_AM == rbCb->mode ) && (AMDL.nxtRetx == NULLP)))
   {
      rbCb->boUnRprtdCnt++;
      return ROK;
   }
#endif
   rbCb->boUnRprtdCnt = (uint32_t)0;
   rbCb->lastRprtdBoToMac = (uint32_t)bo;

   RLC_ALLOC_SHRABL_BUF(RLC_MEM_REGION_DL, RLC_POOL, \
      boStatus, sizeof(RlcBoStatus));

   boStatus->cellId = rbCb->rlcId.cellId;
   boStatus->ueIdx = rbCb->rlcId.ueId;
   boStatus->commCh = FALSE; 
   boStatus->lcId = rbCb->lch.lChId;
   boStatus->bo = bo;

   FILL_PST_RLC_TO_MAC(pst, RLC_DL_INST, EVENT_BO_STATUS_TO_MAC);
   /* Send Status Response to MAC layer */
   if(RlcSendBoStatusToMac(&pst, boStatus) != ROK)
   {
      RLC_FREE_SHRABL_BUF(pst.region, pst.pool, boStatus, sizeof(RlcBoStatus));
   }

   return ROK;
}

/**
 *
 * @brief 
 *        Handler for emptying the SDU queue. 
 * 
 * @detail:
 *        This function is used to empty the SDU queue when
 *        a re-establishment request is received from the 
 *        upper layer.
 *
 * @param[in]  gCb    RLC instance control block
 * @param[in]  rbCb   Radio bearer control block
 * @param[in]  sduQ   SDU queue to be emptied
 *
 * @return Void 
 */
/* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS_RLC
#ifdef ANSI
Void rlcUtlEmptySduQ
(
RlcCb        *gCb, 
RlcDlRbCb    *rbCb,
CmLListCp   *sduQ 
)
#else
Void rlcUtlEmptySduQ(gCb,rbCb, sduQ)
RlcCb        *gCb;
RlcDlRbCb    *rbCb;
CmLListCp   *sduQ;
#endif
#else
#ifdef ANSI
Void rlcUtlEmptySduQ
(
RlcCb        *gCb,
CmLListCp   *sduQ 
)
#else
Void rlcUtlEmptySduQ(gCb,sduQ)
RlcCb        *gCb;
CmLListCp   *sduQ; 
#endif
#endif
{
#ifdef LTE_L2_MEAS_RLC
   CmLListCp   *sduSnMapQ;             /* SDU Sequence number map queue */
   CmLList     *firstSduSnMap;         /* First Node in SDU SnMap Queue */
   RlcSduSnMap  *sduSnMap;              /* SDU Sn Map */

   sduSnMapQ = NULLP;
   firstSduSnMap = NULLP;
   sduSnMap = NULLP;


   sduSnMapQ = &rbCb->sduSnMapQ;
   CM_LLIST_FIRST_NODE(sduSnMapQ, firstSduSnMap);

   while(firstSduSnMap)
   {
      sduSnMap = (RlcSduSnMap *)firstSduSnMap->node;
      if(sduSnMap != NULLP)
      {
         cmLListDelFrm(&(rbCb->sduSnMapQ), &(sduSnMap->lstEnt));
         RLC_FREE(sduSnMap, sizeof(RlcSduSnMap));
         CM_LLIST_FIRST_NODE(sduSnMapQ, firstSduSnMap);
      }
      else
      {
         CM_LLIST_NEXT_NODE(sduSnMapQ, firstSduSnMap);
      }
   }
#endif
   return;
}

/**
 *
 * @brief 
 *    Handler for calculating the Length Indicator (LI) length for a SDU 
 * 
 * @detail:
 *    This function is used to calculate the LI (Length Indicator) length
 *    which has to be substracted from the pduSize to correctly match the
 *    formed PDU(s) size with the  size requested by MAC.
 *
 * @param[in]      gCb      RLC instance control block 
 * @param[in]      numLi    Number of LIs already present 
 * @param[in]      msgLen   Size of the SDU
 * @param[in/out]  pduSz    Size of the pDU to be formed
 *
 * @return  void 
 */
void rlcUtlCalcLiForSdu(RlcCb *gCb, uint16_t numLi, MsgLen msgLen, int16_t *pduSz)
{
   if ( (*pduSz > msgLen)  && (msgLen < RLC_2K_BYTE))
   {
      if(0 == (numLi & RLC_BIT0)) /* check if number of LIs are odd or even */
      {  
         /* if number of LI's are even 2 bytes needed */
         *pduSz -= 2;
      }
      else
      {
         /* if number of LI's are odd one byte needed */
         *pduSz -= 1;
      }
   }
   return;
}

/**
 *
 * @brief 
 *    Function to set that re-establishment has started for an RB
 * 
 * @detail:
 *    This function is used to set the reestInProgress flag to TRUE.
 *    This also sets the estimated header size to 0 and sends bo as
 *    0 to MAC so that RLC does not need to transmit any data.
 *    If the poll re-transmit timer is running for the RB;
 *    it is stopped
 *
 * @param[in]      gCb      RLC instance control block 
 * @param[in]      rbCb     RB for which re-establishment has started 
 *
 * @return  void 
 */
#ifdef ANSI
Void rlcDlUtlSetReestInProgressForRB
(
RlcCb     *gCb,
RlcDlRbCb *rbCb
)
#else
Void rlcDlUtlSetReestInProgressForRB(gCb,rbCb)
RlcCb     *gCb;
RlcDlRbCb *rbCb;
#endif 
{
      
   rbCb->reestInProgress = TRUE;
   
   if(rbCb->mode == CM_LTE_MODE_AM )
   {
      rbCb->m.amDl.estHdrSz = 0;

      if(rlcChkTmr(gCb, (PTR)rbCb, RLC_EVT_AMDL_POLL_RETX_TMR))
      {
         rlcStopTmr(gCb, (PTR)rbCb, RLC_EVT_AMDL_POLL_RETX_TMR);
      }
   }
   else
   {
      rbCb->m.umDl.estHdrSz= 0;
   }

   rlcUtlSendDedLcBoStatus(gCb, rbCb, 0, 0, FALSE,0);

   return;
}

/**
 *
 * @brief 
 *    Function to check if re-establishment is ongoing for an RB
 * 
 * @param[in]      rbCb     RB for which re-establishment is to be checked 
 *
 * @return  Bool
 *          TRUE  : Re-establishment is in progress
 *          FALSE : Re-establishment is not in progress
 */
bool rlcDlUtlIsReestInProgress(RlcDlRbCb *rbCb)
{
   return (rbCb->reestInProgress);
}

/**
 *
 * @brief 
 *    Function to set re-establishment to FALSE
 * 
 * @param[in]      rbCb     RB for which re-establishment is to be reset 
 *
 * @return  Void
 */
#ifdef ANSI
Void rlcDlUtlResetReestInProgress
(
RlcDlRbCb *rbCb
)
#else
Void rlcDlUtlResetReestInProgress(rbCb)
RlcDlRbCb *rbCb;
#endif 
{

   rbCb->reestInProgress = FALSE;
}

/**
 *
 * @brief 
 *    Function to set that re-establishment has started for all the RBs
 *    of an UE; except for SRB1
 *
 * @detail: For SRB1 only the poll-retransmit timer is stopped
 * 
 * @param[in]      gCb      RLC instance control block 
 * @param[in]      ueCb     UE for which re-establishment has started 
 *
 * @return  void 
 */
#ifdef ANSI
Void rlcDlUtlSetReestInProgressForAllRBs
(
RlcCb     *gCb,
RlcDlUeCb *ueCb
)
#else
Void rlcDlUtlSetReestInProgressForAllRBs(gCb,ueCb)
RlcCb     *gCb;
RlcDlUeCb *ueCb;
#endif 
{
   uint32_t        rbIdx;
   
   for(rbIdx = 0;rbIdx < RLC_MAX_SRB_PER_UE;rbIdx++)
   {
      if(ueCb->srbCb[rbIdx] != NULLP)
      {
         if(ueCb->srbCb[rbIdx]->rlcId.rbId != 1)
         {
            rlcDlUtlSetReestInProgressForRB(gCb,ueCb->srbCb[rbIdx]); 
         }
         else
         {
            /* For SRB we just need to stop the poll re-transmit timer */
            if(rlcChkTmr(gCb, (PTR)ueCb->srbCb[rbIdx], RLC_EVT_AMDL_POLL_RETX_TMR))
            {
               rlcStopTmr(gCb, (PTR)ueCb->srbCb[rbIdx], RLC_EVT_AMDL_POLL_RETX_TMR);
            }
         }
      }
   }
   
   for(rbIdx = 0;rbIdx < RLC_MAX_DRB_PER_UE;rbIdx++)
   {
      if(ueCb->drbCb[rbIdx] != NULLP)
      {
         rlcDlUtlSetReestInProgressForRB(gCb,ueCb->drbCb[rbIdx]);
      }
   }
      
   return;
}

/**
 * @brief  Function to increment number of SDUs transmitted 
 *         in KWU SAP statistics
 *
 *
 * @param[in]  rlckwuSap     KWU SAP in which to increment the counter
 *
 * @return  Void
 */
void rlcUtlIncrementKwuStsSduTx(RlcKwuSapCb *rlckwuSap)
{
   rlckwuSap->sts.sduTx++;
   return;
}

/**
 * @brief  Function to increment number of bytes and PDUs transmitted 
 *         in General statistics
 *
 *
 * @param[in]  genSts   KWU SAP in which to increment the counter
 * @param[in]  pdu      The PDU which is sent
 *
 * @return  Void
 */
void rlcUtlIncrementGenStsBytesAndPdusSent(RlcGenSts *genSts, Buffer *pdu)
{
   MsgLen bytesSent;
   ODU_GET_MSG_LEN(pdu, &bytesSent);
   genSts->bytesSent += bytesSent;
   genSts->pdusSent++;
   return;
}

/**
 * @brief  Function to initialize the data structures used to free memory
 *
 *
 * @param[in]  gCb        RLC instance control block
 * @param[out] toBeFreed  Pointer to the freeing structure. This is 
 *                        initialized here
 *
 * @return  Void
 */
#ifdef ANSI
Void rlcUtlInitToBeFreed
(
RlcCb                *gCb,
RlcDlDataToBeFreed   *toBeFreed
)
#else
Void rlcUtlInitToBeFreed(gCb, toBeFreed)
RlcCb                *gCb;
RlcDlDataToBeFreed   *toBeFreed;
#endif
{
   cmLListInit(&(toBeFreed->sduLst));
   cmLListInit(&(toBeFreed->rbLst));
   cmLListInit(&(toBeFreed->reTxLst));
   cmLListInit(&(toBeFreed->txLst));
   
   return;
}

/**
 * @brief  Function to initialize the DL self Pst structure
 *
 *
 * @param[in]  gCb   RLC instance control block
 *
 * @return  Void
 */
#ifdef ANSI
Void rlcUtlInitializeSelfPst
(
RlcCb *gCb  
)
#else
Void rlcUtlInitializeSelfPst(gCb)
RlcCb *gCb;
#endif
{
   Pst *selfPst = &gCb->u.dlCb->selfPst;
   
   RLC_MEM_SET(selfPst, 0, sizeof(Pst));
   selfPst->srcProcId = SFndProcId();
   selfPst->dstProcId = SFndProcId();
   selfPst->dstEnt = gCb->init.ent;
   selfPst->dstInst = gCb->init.inst; /* this is for the DL instance */
   selfPst->srcEnt = gCb->init.ent;
   selfPst->srcInst = gCb->init.inst;  /* DL instance will send to itself */
   selfPst->prior  = PRIOR3;
   selfPst->event  = UDX_EVT_DL_CLEANUP_MEM;
}

/**
 * @brief  Function to send a DL cleanup event
 *
 *
 * @param[in]  gCb   RLC instance control block
 *
 * @return  Void
 */
void rlcUtlRaiseDlCleanupEvent(RlcCb *gCb)
{
#ifdef KWSELFPSTDLCLEAN
   if(!gCb->u.dlCb->eventInQueue)
   {
      ODU_POST_TASK(&gCb->u.dlCb->selfPst, gCb->u.dlCb->selfPstMBuf);
      gCb->u.dlCb->eventInQueue = TRUE;
   }
#endif
   return;
}

/**
 * @brief  Function to add a SDU to the to be freed sdu list
 *
 *
 * @param[in]  gCb   RLC instance control block
 * @param[in]  sdu   SDU to be added to the list
 *
 * @return  Void
 */
void rlcUtlAddSduToBeFreedQueue(RlcCb *gCb, RlcSdu *sdu)
{
   cmLListAdd2Tail(&(gCb->u.dlCb->toBeFreed.sduLst), &(sdu->lstEnt));
   return;
}

/**
 * @brief  Function to add a re-transmitted pdu to the to be freed list
 *
 *
 * @param[in]  gCb   RLC instance control block
 * @param[in]  retx  Re-transmitted pdu to be added to the list
 *
 * @return  Void
 */
#ifdef ANSI
Void rlcUtlAddReTxPduToBeFreedQueue
(
RlcCb     *gCb,
RlcRetx   *retx
)
#else
Void rlcUtlAddReTxPduToBeFreedQueue(gCb, retx)
RlcCb     *gCb;
RlcRetx   *retx;
#endif
{
   cmLListAdd2Tail(&(gCb->u.dlCb->toBeFreed.reTxLst), &(retx->lstEnt));
   return;
}

/**
 * @brief  Function to add a transmitted pdu to the to be freed list
 *
 *
 * @param[in]  gCb   RLC instance control block
 * @param[in]  pdu   PDU to be added to the list
 *
 * @return  Void
 */
#ifdef ANSI
Void rlcUtlAddTxPduToBeFreedQueue
(
RlcCb   *gCb,
RlcTx   *pdu
)
#else
Void rlcUtlAddTxPduToBeFreedQueue(gCb, pdu)
RlcCb   *gCb;
RlcTx   *pdu;
#endif
{
   pdu->rlsLnk.node = (PTR)pdu;
   cmLListAdd2Tail(&(gCb->u.dlCb->toBeFreed.txLst), &(pdu->rlsLnk));
   return;
}

/*
 * @brief
 *    function to free/release the Acknowledged mode RBCB buffers
 *
 * @details
 *    This primitive Frees the Acknowledged Mode RbCb transmission Buffer,
 *    retransmission Buffer and reciption Buffers
 *
 * @param [in]      gCb       - RLC instance control block
 * @param [in]      rbCb      - Downlink RB Control Block
 * @param [in,out]  toBeFreed - Number of buffers to be freed
 *
 * @return Bool
 *     - TRUE if more data to be freed
 *     - FALSE if all the data has been freed
 */
#ifdef ANSI
static Bool rlcUtlFreeDlAmRbMemory
(
RlcCb       *gCb,
RlcDlRbCb   *rbCb,
uint32_t    *toBeFreed
)
#else
static Bool rlcUtlFreeDlAmRbMemory(gCb, rbCb, toBeFreed)
RlcCb       *gCb;
RlcDlRbCb   *rbCb;
uint32_t    *toBeFreed
#endif
{
   RlcRetx   *retx;   /* retransmission buffer */
   RlcSn     mTxNext;    /* send state variable */
   RlcTx      *txBuf;

   MODAMT(AMDL.txNext, mTxNext, AMDL.txNextAck,AMDL.snModMask);

   /* TODO : to be checked changed from <= to < */
   while ((0 < mTxNext) && *toBeFreed)
   {
      txBuf =  rlcUtlGetTxBuf(AMDL.txBufLst, AMDL.txNextAck);
      if (txBuf && txBuf->pduLst.first)
      {
         while(txBuf->pduLst.first)
	 {
	    RlcDlPduInfo *pduInfo = (RlcDlPduInfo *)(txBuf->pduLst.first->node);
	    RLC_FREE_BUF(pduInfo->pdu);
	    /* Delete node from the txBuf Pdu lst */
	    cmLListDelFrm(&txBuf->pduLst, txBuf->pduLst.first);
	    RLC_FREE_WC(gCb, pduInfo, sizeof(RlcDlPduInfo));
	 }
         rlcUtlDelTxBuf(AMDL.txBufLst, txBuf, gCb);
         if(gCb->u.dlCb->shutdownReceived == 0)
         {   
            (*toBeFreed)--;
	 }
      }
      AMDL.txNextAck = (AMDL.txNextAck + 1) & AMDL.snModMask;
      MODAMT(AMDL.txNext, mTxNext, AMDL.txNextAck,AMDL.snModMask);
   }
   if(*toBeFreed == 0)
   {
      return (TRUE);
   }
   
#ifndef LTE_TDD 
      RLC_FREE(gCb,AMDL.txBufLst, (RLC_TX_BUF_BIN_SIZE * sizeof(CmLListCp)));
#endif

   RLC_LLIST_FIRST_RETX(AMDL.retxLst, retx);
   while (retx && (*toBeFreed)) /* Till to be freed becomes 0 */
   {

      RLC_FREE_BUF(retx->seg);

      cmLListDelFrm(&AMDL.retxLst, &retx->lstEnt);
      RLC_FREE_WC(gCb, retx, sizeof(RlcRetx));

      RLC_LLIST_FIRST_RETX(AMDL.retxLst, retx);
      if(gCb->u.dlCb->shutdownReceived == 0)
      {
      (*toBeFreed)--;
   }

   }

   AMDL.nxtRetx = NULLP;

   /* clean up if there is info about STATUS PDU to be sent */
   if(AMDL.pStaPdu)
   {
      Pst *udxPst;
      udxPst = &gCb->u.dlCb->udxDlSap->pst;
      RLC_FREE_SHRABL_BUF_WC(udxPst->region,
			    udxPst->pool,
			    AMDL.pStaPdu, 
			    sizeof(RlcUdxDlStaPdu));
      AMDL.pStaPdu = NULLP;
   }

   if(*toBeFreed == 0)
   {
      return (TRUE);
   }  
   if(gCb->u.dlCb->shutdownReceived)
   {   
      (*toBeFreed)--;
   }
   
   return (FALSE);
} 

/**
 * @brief  Function to free memory from the DL instance
 *
 *
 * @param[in]  gCb   RLC instance control block
 *
 * @return  Void
 */
#ifdef ANSI
Void rlcUtlFreeDlMemory
(
RlcCb *gCb  
)
#else
Void rlcUtlFreeDlMemory(gCb)
RlcCb *gCb;
#endif
{
   uint32_t toBeFreed; 

   /* safety check, in case some event was still lying in the queue after
      the dlCb was deleted*/
   if(!gCb->u.dlCb)
   {
      return;
   }

   RlcDlDataToBeFreed* pToBeFreed = &gCb->u.dlCb->toBeFreed;
   /* ccpu00136940 */
   if(gCb->u.dlCb->shutdownReceived)
   {
      toBeFreed = pToBeFreed->txLst.count + pToBeFreed->reTxLst.count + pToBeFreed->sduLst.count + pToBeFreed->rbLst.count; 
   }
   else
   {
      if ((pToBeFreed->txLst.count + pToBeFreed->reTxLst.count + pToBeFreed->sduLst.count) > (3 * RLC_MAX_TO_BE_FREED))
      {
#if !defined(KWSELFPSTDLCLEAN) && defined(MAC_RLC_HARQ_STA_RBUF) && defined(LTE_L2_MEAS)
         if (isDatReqProcessed)
         {
            toBeFreed = (2 *RLC_MAX_TO_BE_FREED);
         }
         else
#endif
         {
            toBeFreed = (3 *RLC_MAX_TO_BE_FREED)/2;
         }
      }
      else
      {
         toBeFreed = RLC_MAX_TO_BE_FREED;
      }
   }   
   CmLListCp        *lst;

   gCb->u.dlCb->eventInQueue = FALSE; /* reset as we have received the event
                                         and are processing it */

   /* Free from the ReTx list */
   lst  = &pToBeFreed->reTxLst;
#ifndef L2_OPTMZ
   while((lst->first) && toBeFreed && (pToBeFreed->reTxLst.count > 100))
#else
   while((lst->first) && toBeFreed)
#endif
   {
      RlcRetx* seg = (RlcRetx *)(lst->first->node);
      cmLListDelFrm(lst, lst->first);
      RLC_FREE_BUF_WC(seg->seg);
      RLC_FREE_WC(gCb,seg, sizeof(RlcRetx));
      toBeFreed--;
   }   

   /* Free from the Tx list */
   lst  = &pToBeFreed->txLst;
#ifndef L2_OPTMZ
   while((lst->first) && toBeFreed && (pToBeFreed->txLst.count > 100))
#else
   while((lst->first) && toBeFreed)
#endif
   {
      RlcTx* pdu = (RlcTx *)(lst->first->node);
      cmLListDelFrm(lst, lst->first);
      while(pdu->pduLst.first)
      {
         RlcDlPduInfo *pduInfo = (RlcDlPduInfo *)(pdu->pduLst.first->node);
         
	 cmLListDelFrm(&pdu->pduLst, pdu->pduLst.first);
	 RLC_FREE_BUF_WC(pduInfo->pdu);
	 RLC_FREE_WC(gCb, pduInfo, sizeof(RlcDlPduInfo));
      }
      RLC_FREE_WC(gCb,pdu, sizeof(RlcTx));
      toBeFreed--;
   }

   /* Free from the SDU queue */
   lst  = &pToBeFreed->sduLst;
#ifndef L2_OPTMZ
   while((lst->first) && toBeFreed && (pToBeFreed->sduLst.count > 100))
#else
   while((lst->first) && toBeFreed)
#endif
   {
      RlcSdu* sdu = (RlcSdu *)(lst->first->node);
      RLC_RMV_SDU(gCb, lst, sdu);
      toBeFreed--;
   }      

   /* Free from the RBs */
   lst  = &pToBeFreed->rbLst;
#ifndef L2_OPTMZ
   while((lst->first) && toBeFreed && (pToBeFreed->rbLst.count > 100))
#else
   while((lst->first) && toBeFreed)
#endif
   {
      RlcDlRbCb* rbCb = (RlcDlRbCb *)(lst->first->node);
      Bool moreToBeFreed = rlcUtlFreeDlAmRbMemory(gCb, rbCb,&toBeFreed);
      if(!moreToBeFreed)
      {
         cmLListDelFrm(lst, lst->first);
         RLC_FREE_WC(gCb, rbCb, sizeof(RlcDlRbCb));
      }
   } 

   if ((toBeFreed == 0) && !(gCb->u.dlCb->shutdownReceived))
   {
      rlcUtlRaiseDlCleanupEvent(gCb);
   }

   return;
}


#ifdef LTE_L2_MEAS
/**
 *
 * @brief Function to initialise measurement
 *
 * @b Description
 *
 * @param[in]  gCb     RLC Instance Control Block
 *
 *  @return  Void
 *
 */
S16 rlcUtlL2MeasDlInit(RlcCb *gCb)
{
   uint16_t   cntr;

   gCb->u.dlCb->rlcL2Cb.rlcNumMeas=0;
   for(cntr = 0; cntr < LKW_MAX_L2MEAS; cntr++)
   {
      memset(&(gCb->u.dlCb->rlcL2Cb.rlcL2EvtCb[cntr]), 0, sizeof(RlcL2MeasEvtCb));
   }
   gCb->u.dlCb->rlcL2Cb.rlcL2EvtCb[RLC_L2MEAS_DL_DISC].measCb.measType = LKW_L2MEAS_DL_DISC;
   gCb->u.dlCb->rlcL2Cb.rlcL2EvtCb[RLC_L2MEAS_DL_IP].measCb.measType = LKW_L2MEAS_DL_IP;
   gCb->u.dlCb->rlcL2Cb.rlcL2EvtCb[RLC_L2MEAS_DL_DELAY].measCb.measType= LKW_L2MEAS_DL_DELAY;
   gCb->u.dlCb->rlcL2Cb.rlcL2EvtCb[RLC_L2MEAS_UU_LOSS].measCb.measType= LKW_L2MEAS_UU_LOSS;
   gCb->u.dlCb->rlcL2Cb.rlcL2EvtCb[RLC_L2MEAS_ACT_UE].measCb.measType= LKW_L2MEAS_ACT_UE;

   return ROK;
}
/**
 *
 * @brief Function to detect the data Burst start Condition in a DTCH
 *
 * @b Description
 *
 * @param[in]  rbCb     RB control block
 * @param[in]  contSduLst  Array of Contained SDUs in the DTCH   
 * @param[in]  dataVol  Available data in the DTCH
 * @param[in]  schPduSz    Total grant Size given by MAC
 *
 *  @return  Void
 *
 */
#ifdef ANSI
Void rlcUtlUpdateBurstSdus
(
RlcCb            *gCb,
RlcDlRbCb        *rbCb,
RlcContSduLst    *contSduLst,
S32              dataVol,
uint32_t         schPduSz
)
#else
Void  rlcUtlUpdateBurstSdus (gCb, rbCb, contSduLst, dataVol, schPduSz)
RlcCb            *gCb;
RlcDlRbCb        *rbCb;
RlcContSduLst    *contSduLst;
S32              dataVol;
uint32_t         schPduSz;
#endif
{

   RlcL2MeasDlIpTh   *l2MeasDlIpThruput = NULLP;
   RlcL2MeasTb       *l2MeasTb = NULLP;
   uint8_t           idx;
   uint8_t           currTbIdx;
   volatile uint32_t startTime = 0;
   RlcContSduLst   *dstContSduLst;


   /*starting Task*/
   SStartTask(&startTime, PID_RLC_DLIP_TPT_BURSTCALC);

   l2MeasDlIpThruput = &rbCb->l2MeasIpThruput.dlIpTh;

   if(RLC_MEAS_IS_DL_IP_MEAS_ON_FOR_RB(gCb, rbCb))
   {
      if(dataVol > schPduSz)
      {
         if(l2MeasDlIpThruput->isBurstAct == FALSE)
         {
            l2MeasDlIpThruput->burstStartTime = glblTtiCnt;
            l2MeasDlIpThruput->isBurstAct = TRUE;
            l2MeasDlIpThruput->burstEndSduId = 0;
         }
         else
         {  /* This is the case when another burst started before RLC gets the 
               l2MeasDlIpThruput->burstEndSduId = 0; */
         }
      }
      else
      {  /* Store the burstEndSduId here */
         if((l2MeasDlIpThruput->isBurstAct == TRUE) &&
               (!l2MeasDlIpThruput->burstEndSduId))
         {
            l2MeasDlIpThruput->burstEndSduId = 
               l2MeasDlIpThruput->outStngSduArr[l2MeasDlIpThruput->lastSduIdx].sduId;
         }
      }
      if(l2MeasDlIpThruput->isBurstAct == TRUE)
      {
         l2MeasTb = rlcUtlGetCurMeasTb(gCb,rbCb);
         /* Get the lChId from index 0, because index 0 is always for DL */
         if(l2MeasTb->numLcId >= RLC_MAX_ACTV_DRB) 
         {
            /* ccpu00143043 */
            return;
         }
         l2MeasTb->sduInfo[l2MeasTb->numLcId].lcId = rbCb->lch.lChId;
         /* Copy all the sduIdx from sduInfo to tb sduInfo */
         currTbIdx = l2MeasTb->sduInfo[l2MeasTb->numLcId].numSdus;
         dstContSduLst = &l2MeasTb->sduInfo[l2MeasTb->numLcId];
         /* ccpu00143043 */
         for(idx = 0; ((idx < contSduLst->numSdus)
                  && (currTbIdx < RLC_L2MEAS_MAX_OUTSTNGSDU)) ; idx++)
         {
            dstContSduLst->sduIdx[currTbIdx++] = contSduLst->sduIdx[idx];
         }
         l2MeasTb->sduInfo[l2MeasTb->numLcId].numSdus += idx;
         l2MeasTb->numLcId++;
      }/* End of isBurstAct */
   }/* End of if measOn */

   /*stopping Task*/
   SStopTask(startTime, PID_RLC_DLIP_TPT_BURSTCALC);
   return;
}
/**
 * @brief
 *        This function is used to store locally the sduIdx of the sdu in the 
 *        outstanding SDU array
 * 
 * @b Description:
 *        Stores the Sdu Idx in the contained  SDU Array and increments 
 *        the num contained  Sdus 
 *
 *  @param[in]  sduIdx  the Index of the SDU in the outstanding SDU array
 *  @param[out] contSduLst This stores the indices of the SDUs 
 *
 *  @return  Void
 */
#ifdef ANSI
Void rlcUtlUpdateContainedSduLst
(
uint8_t            sduIdx,
RlcContSduLst  *contSduLst
)
#else
Void rlcUtlUpdateContainedSduLst(sduIdx, contSduLst)
uint8_t            sduIdx;
RlcContSduLst  *contSduLst;
#endif
{
   if (contSduLst->numSdus < RLC_L2MEAS_MAX_OUTSTNGSDU)
   {
    contSduLst->sduIdx[contSduLst->numSdus] = sduIdx;
    contSduLst->numSdus++;
   }
    return;
}

/**
 * @brief
 *        This function is used to store the sduId of the sdu in the 
 *        outstanding SDU array
 * 
 * @b Description:
 *        Stores the Sdu Id in the outstanding SDU Array and increments 
 *        the num contained  Sdus 
 *
 *  @param[out] dlIpThPut  The structure in which the outstanding sdus are 
 *                         updated
 *  @param[in] sduIdx   The Idx at which the sdu ID is stored  
 *  @param[in] sduLen   The size if sdu in bytes  
 *  @param[in] newIdx   Indicates if the sdu is already present in the 
 *                      outstanding array  
 *
 *  @return  Void
 */
#ifdef ANSI
Void rlcUtlUpdateOutStandingSduLst
(
RlcL2MeasDlIpTh   *dlIpThPut, 
uint8_t               sduIdx, 
MsgLen           sduLen, 
uint32_t              sduId,
Bool             newIdx
)
#else
Void rlcUtlUpdateOutStandingSduLst(dlIpThPut, sduIdx, sduLen, sduId, newIdx)
RlcL2MeasDlIpTh   *dlIpThPut;
uint8_t               sduIdx;
MsgLen           sduLen; 
uint32_t              sduId;
Bool             newIdx;
#endif
{
   if (sduIdx < RLC_L2MEAS_MAX_OUTSTNGSDU)
   {
   if(newIdx == TRUE)
   {      
     dlIpThPut->outStngSduArr[sduIdx].numTb = 0;
   }
   dlIpThPut->outStngSduArr[sduIdx].numTb++;
   dlIpThPut->outStngSduArr[sduIdx].sduId = sduId;
   dlIpThPut->outStngSduArr[sduIdx].sduLen = sduLen;
   }
   return;
}
#ifdef ANSI
RlcL2MeasTb * rlcUtlGetCurMeasTb
(
RlcCb     *gCb,
RlcDlRbCb *rbCb
)
#else
RlcL2MeasTb * rlcUtlGetCurMeasTb(gCb, rbCb)
RlcCb     *gCb;
RlcDlRbCb *rbCb;
#endif
{
   RlcL2MeasTb  *curL2MeasTb;
   uint16_t         idx;

   if((curL2MeasTb = rbCb->ueCb->l2MeasTbCb[rbCb->ueCb->tbIdx]) == NULLP)
      {
         /* Intentionally avoiding the RLC_ALLOC macro to avoid  memset */
         if (SGetSBuf(gCb->init.region,
                  gCb->init.pool,
                  (Data **)&curL2MeasTb,
                  (Size)sizeof(RlcL2MeasTb)) != ROK)
         {
            return (NULLP);
         }
         rbCb->ueCb->l2MeasTbCb[rbCb->ueCb->tbIdx] = curL2MeasTb;
         /* Initialize the Meas Tb details */
         curL2MeasTb->numLcId = 0;
         curL2MeasTb->numLchInfo = 0;
         curL2MeasTb->txSegSduCnt = 0;
         for (idx = 0; idx < RLC_MAX_ACTV_DRB; idx++)
         {
            curL2MeasTb->sduInfo[idx].numSdus = 0;
         }
         for (idx = 0; idx < RLC_MAX_ACTV_DRB; idx++)
         {
            curL2MeasTb->lchInfo[idx].numSdus = 0;
         }
      }
   return (curL2MeasTb);
}


/**
 *
 * @brief Handler for Processing harq status indication
 *
 *
 * @b Description
 *        This function is called when the MAC sends a harq ind Mesage.
 *        This is used only for UuLoss and Dl Delay and DL Ipthoughput
 *        L2 Measurements.
 *
 *  @param[in] staInd Harq status indication received from MAC.
 *  @param[in] ueCb   UeCb corresponding to the Tb Id.
 *  @param[in] tbIdx  TB index, 0 for SISO and 0,1 for MIMO.
 *
 *
 *  @return  S16
 *      -# ROK
 */

#ifdef ANSI
S16 rlcUtlProcHarqInd
(
RlcCb            *gCb,
RguHarqStatusInd *hqStaInd,
RlcDlUeCb        *ueCb,
uint8_t          tbIdx
)
#else
S16 rlcUtlProcHarqInd(gCb, hqStaInd, ueCb, tbIdx)
RlcCb             *gCb;
RguHarqStatusInd  *hqStaInd;
RlcDlUeCb         *ueCb;
uint8_t            tbIdx;
#endif
{
#ifdef LTE_L2_MEAS
   RlcDlRbCb        *rlcRbCb;           /* KW Control Block */
   RlcL2MeasTb       *l2MeasTb = NULLP;         /* Measurement TbCb */
   uint8_t           lcIdx;             /* Logical channel id index */
   uint8_t           sduIndx;            /* sdu index to out standing sdu list in rbCb */
   uint32_t          numSdus;           /* number of sdus in the outstanding sdu list */
   RlcOutStngSduInfo *outStngSduArr;  /* Outstanding sdu list */
   Ticks             ackTime;
   Ticks             delay;
   uint32_t          totlSduCnt = 0;
#endif
   uint8_t           timeAddedFlag;
   S16               ret;
   volatile uint32_t     startTime = 0;
   /*kw005.201 Code added for DL IP thruput measurement*/

   /*starting Task*/
   SStartTask(&startTime, PID_RLC_DLIP_TPT_PRCHARQIND);

   ret = ROK;
   if(hqStaInd->tbId[tbIdx] >= RLC_INVALID_TBID)
   {
      return ROK;
   }

   /* Find the L2 measurement tbCb to process DL Ip thruput*/
   l2MeasTb = ueCb->l2MeasTbCb[hqStaInd->tbId[tbIdx]];
   if(l2MeasTb == NULLP)
   {
      return ROK;
   }
   /* For each logical channel in the tbCb, process 
    * and get the DL IP thruput */
   ackTime = SGetTtiCount();
   for(lcIdx = 0; ((lcIdx < l2MeasTb->numLcId) && (lcIdx < RLC_MAX_ACTV_DRB)); lcIdx++)
   {
      timeAddedFlag = FALSE;
      if((rlcRbCb = ueCb->lCh[l2MeasTb->sduInfo[lcIdx].lcId - 1].dlRbCb)
            == NULLP)
      {
         continue;
      }
      /* fix for DL IP stop*/
      if (!gCb->u.dlCb->rlcL2Cb.measOn[rlcRbCb->qci]
            || (rlcRbCb->rlcId.rbType == CM_LTE_SRB))
      {
         continue;
      }
      
      /* Get the outstanding SDUs using sdu index stored in Container sduList
       * and check for HARQ ACK/NACK */
      numSdus = l2MeasTb->sduInfo[lcIdx].numSdus;
      /* ccpu00143043 */
      if ((numSdus >= RLC_L2MEAS_MAX_OUTSTNGSDU) || (numSdus == 0))
      {
         break;
      }
      totlSduCnt += numSdus;

      if (RLC_MEAS_IS_DL_IP_MEAS_ON_FOR_RB(gCb,rlcRbCb))
      {
         for(sduIndx = 0; sduIndx < numSdus; sduIndx++)
         {
            outStngSduArr =&(rlcRbCb->l2MeasIpThruput.dlIpTh.outStngSduArr[\
                  l2MeasTb->sduInfo[lcIdx].sduIdx[sduIndx]]);
            if(hqStaInd->status[tbIdx] == TRUE)
            {
               /* If ACK is for burst End Sdu Id set burstActive 
                * to FALSE  and accumulate time */
               if((rlcRbCb->l2MeasIpThruput.dlIpTh.burstEndSduId == 
                        outStngSduArr->sduId) && (outStngSduArr->numTb == 1))
               {
                  rlcRbCb->l2MeasIpThruput.dlIpTh.isBurstAct = FALSE;
                  /*Update the l2Sts structure for calculating throughput*/
                  rlcRbCb->rbL2Cb.l2Sts[RLC_L2MEAS_DL_IP]->dlIpThruput.volSummation
                     += outStngSduArr->sduLen;

                  rlcRbCb->rbL2Cb.l2Sts[RLC_L2MEAS_DL_IP]->dlIpThruput.timeSummation
                     += glblTtiCnt - rlcRbCb->l2MeasIpThruput.dlIpTh.burstStartTime;
                  outStngSduArr->sduId = 0;
                  outStngSduArr->sduLen = 0;
                  outStngSduArr->numTb = 0;
                  rlcRbCb->l2MeasIpThruput.dlIpTh.burstEndSduId = 0;
               }

               /* If burst is active and this sdu is only transmitted in single TB then
                * accumulate volume  and clear the outstanding sduList */
               if((rlcRbCb->l2MeasIpThruput.dlIpTh.isBurstAct == TRUE) &&
                     (--(outStngSduArr->numTb) == 0))
               {
                  rlcRbCb->rbL2Cb.l2Sts[RLC_L2MEAS_DL_IP]->dlIpThruput.volSummation
                     += outStngSduArr->sduLen;

                  if(timeAddedFlag == FALSE)
                  {
                     rlcRbCb->rbL2Cb.l2Sts[RLC_L2MEAS_DL_IP]->dlIpThruput.timeSummation
                        += glblTtiCnt - rlcRbCb->l2MeasIpThruput.dlIpTh.burstStartTime;
                     rlcRbCb->l2MeasIpThruput.dlIpTh.burstStartTime = glblTtiCnt;
                     timeAddedFlag = TRUE;
                  }
                  outStngSduArr->sduId = 0;
                  outStngSduArr->sduLen = 0;
               }
            }/* End of status == TRUE */
            else
            {
               if(rlcRbCb->l2MeasIpThruput.dlIpTh.isBurstAct == TRUE)
               {
                  if((rlcRbCb->l2MeasIpThruput.dlIpTh.burstEndSduId == 
                           outStngSduArr->sduId))
                  {
                     rlcRbCb->l2MeasIpThruput.dlIpTh.isBurstAct = FALSE;
                     rlcRbCb->l2MeasIpThruput.dlIpTh.burstEndSduId = 0;
                  }
                  /* Clear the outstanding sdu list */
                  outStngSduArr->sduId = 0;
                  outStngSduArr->sduLen = 0;
                  outStngSduArr->numTb = 0;
               }
            }
         }
      } 
   }

   for(lcIdx = 0; ((lcIdx < l2MeasTb->numLchInfo) && (lcIdx < RLC_MAX_ACTV_DRB)); lcIdx++)
   {
      if((rlcRbCb = ueCb->lCh[l2MeasTb->lchInfo[lcIdx].lcId - 1].dlRbCb)
            == NULLP)
      {
         continue;
      }
      numSdus = l2MeasTb->lchInfo[lcIdx].numSdus;
      if ( numSdus == 0 )
      {
         continue;
      }
      /* ccpu00143043 */
      if ((numSdus > RLC_L2MEAS_MAX_OUTSTNGSDU) || (numSdus == 0))
      {
         break;
      }
      /* Update stats */
      if(hqStaInd->status[tbIdx] == TRUE)
      {
         for(sduIndx = 0; sduIndx < numSdus; sduIndx++)
         {
            delay = RLC_TIME_DIFF(ackTime,l2MeasTb->lchInfo[lcIdx].sduInfo[sduIndx].arvlTime);
            RLC_UPD_PDCP_L2_DLDELAY_STS(gCb,rlcRbCb, delay); 
         }
         /* Case of sduInfo not updated */
         if (totlSduCnt == 0)
         {
            totlSduCnt = numSdus;
         }
         RLC_UPD_L2_UU_LOSS_POS_PKTS(gCb,rlcRbCb, (totlSduCnt + l2MeasTb->txSegSduCnt));
      }
      else
      {
         /* Case of sduInfo not updated */
         if (totlSduCnt == 0)
         {
            totlSduCnt = numSdus;
         }
         RLC_UPD_L2_UU_LOSS_PKTS(gCb,rlcRbCb, (totlSduCnt + l2MeasTb->txSegSduCnt));
      }
   }
   /* Free this tb, deallocate the memory */
   RLC_FREE(gCb, l2MeasTb, sizeof(RlcL2MeasTb));
   ueCb->l2MeasTbCb[hqStaInd->tbId[tbIdx]] = NULLP;
   
   /*stopping Task*/
   SStopTask(startTime, PID_RLC_DLIP_TPT_PRCHARQIND);

   return (ret);
}/* end of  rlcUtlProcHarqInd */ 

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
S16 rlcUtlSndDlL2MeasCfm
(
RlcCb                  *gCb,
RlcL2MeasEvtCb         *measEvtCb
)
#else
S16 rlcUtlSndDlL2MeasCfm(gCb, measEvtCb)
RlcCb                  *gCb;
RlcL2MeasEvtCb         *measEvtCb;
#endif
{
   uint32_t         qciIdx;
   RlcL2MeasCb      *measCb = NULLP;
   RlcL2MeasCfmEvt   measCfmEvt;
   uint32_t          posPkts;
   uint32_t          dLoss;
   uint64_t          dlDataVol;
   uint64_t          dlTime;
   uint16_t          cntr;
   /* Discard new changes starts */
   uint8_t           qci = 0;
   uint32_t          cfmIdx =0;
   /* Discard new changes ends */

   /* kw006.201 ccpu00120058 emoved 64 bit compilation warning */
#ifndef ALIGN_64BIT
   RLOG1(L_DEBUG,"rlcUtlSndL2MeasCfm(transId(%ld))", measEvtCb->transId);
#else
   RLOG1(L_DEBUG,"rlcUtlSndL2MeasCfm(transId(%d))", measEvtCb->transId);
#endif

   /* Clean up the RB data structures */
   measCb = &measEvtCb->measCb;
   
   memset(&measCfmEvt, 0, sizeof(RlcL2MeasCfmEvt));
   measCfmEvt.transId = measEvtCb->transId;

   measCfmEvt.measType = measCb->measType;
   measCfmEvt.status.status = LCM_PRIM_OK;
   measCfmEvt.status.reason = LCM_REASON_NOT_APPL;
   
   if(measCb->measType & LKW_L2MEAS_DL_IP)
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

               dlDataVol = pUeInfoLstCb[cntr].measData[qci].dlIpThruput.volSummation;
               dlTime = pUeInfoLstCb[cntr].measData[qci].dlIpThruput.timeSummation;

               if((0 == dlTime) || !(gCb->u.dlCb->rlcL2Cb.measOn[qci] & LKW_L2MEAS_DL_IP) )
               {
                  pUeInfoLstCfm[cfmIdx].measCfm[pUeInfoLstCfm[cfmIdx].numCfm].val.ipThrput.dlIpThPut = 0;
               }
               else
               {
                  pUeInfoLstCfm[cfmIdx].measCfm[pUeInfoLstCfm[cfmIdx].numCfm].val.ipThrput.dlIpThPut = 
                     (dlDataVol / dlTime);
               }
               pUeInfoLstCfm[cfmIdx].measCfm[pUeInfoLstCfm[cfmIdx].numCfm].val.ipThrput.dlIpThPut *= 8;

               /* Reset the values after reporting to Application */
               pUeInfoLstCb[cntr].measData[qci].dlIpThruput.volSummation = 0;
               pUeInfoLstCb[cntr].measData[qci].dlIpThruput.timeSummation = 0;

               measCfmEvt.val.ipThMeas.ueInfoLst[cfmIdx].numCfm++;
            }
            cfmIdx++;
         }
      }
      measCfmEvt.val.ipThMeas.numUes = cfmIdx; 
   }
   else
   {
      RlcL2Cntr *pMeasData = measCb->val.nonIpThMeas.measData;
      RlcL2MeasCfmNonIpThMeas *pMeasCfmNonIp = &measCfmEvt.val.nonIpThMeas;

      pMeasCfmNonIp->numCfm = 0;

      for(qciIdx = 0; qciIdx < LKW_MAX_QCI; qciIdx++)
      {
         qci = measCb->val.nonIpThMeas.qci[qciIdx];
         if (qci > 0)
         {
            pMeasCfmNonIp->measCfm[pMeasCfmNonIp->numCfm].qci = qci;

            if(measCb->measType & LKW_L2MEAS_UU_LOSS)
            {
               dLoss = pMeasData[qci].uuLoss.dLoss;
               posPkts = pMeasData[qci].uuLoss.posPkts;
               if(((posPkts + dLoss) != 0))
               {
                  pMeasCfmNonIp->measCfm[pMeasCfmNonIp->numCfm].val.nonIpThrput.uuLoss  =
                     ((dLoss  * 1000000) / (posPkts + dLoss));
               }
               pMeasData[qci].uuLoss.dLoss = 0;
               pMeasData[qci].uuLoss.posPkts = 0;
            }
            if(measCb->measType & LKW_L2MEAS_DL_DISC)
            {

               pMeasCfmNonIp->measCfm[pMeasCfmNonIp->numCfm].val.nonIpThrput.dlDiscRate = 0;
               if(pMeasData[qci].dlDisc.totSdus != 0)
               {
                  pMeasCfmNonIp->measCfm[pMeasCfmNonIp->numCfm].val.nonIpThrput.dlDiscRate = 
                     (((pMeasData[qci].dlDisc.discSdus)  * 1000000) / (pMeasData[qci].dlDisc.totSdus));
               }

               pMeasData[qci].dlDisc.totSdus = 0;
               pMeasData[qci].dlDisc.discSdus = 0;
            }
            if(measCb->measType & LKW_L2MEAS_DL_DELAY)
            {
               if (pMeasData[qci].dlPjSduDelay.numSdus > 0)
               {
                  pMeasCfmNonIp->measCfm[pMeasCfmNonIp->numCfm].val.nonIpThrput.dlSduDelay = 
                    (pMeasData[qci].dlPjSduDelay.sduDelay / pMeasData[qci].dlPjSduDelay.numSdus);
                  pMeasData[qci].dlPjSduDelay.sduDelay = 0;
                  pMeasData[qci].dlPjSduDelay.numSdus = 0;
               }
            }
            pMeasCfmNonIp->numCfm++;
         }
      }
   }
   /* Fix Klock warning */
   RlcMiLkwL2MeasCfm(&gCb->genCfg.lmPst, &measCfmEvt);
   return ROK;
} /* rlcUtlSndL2MeasCfm */
/**
 *
 * @brief Handler for Sending Negative confirm .
 *
 *
  @b Description
 *        This function is called when the l2 measurement cannot be started
 *        This function sends  negative confirm for all the requests
 *
 *  @param[in] measReqEvt    Measurement Req Structure
 *
 *
 *  @return  S16
 *      -# ROK
 */

#ifdef ANSI
S16 rlcUtlSndDlL2MeasNCfm
(
RlcCb           *gCb,
RlcL2MeasReqEvt *measReqEvt,
RlcL2MeasCfmEvt *measCfmEvt
)
#else
S16 rlcUtlSndDlL2MeasNCfm(gCb,measReqEvt, measCfmEvt)
RlcCb           *gCb;
RlcL2MeasReqEvt *measReqEvt;
RlcL2MeasCfmEvt *measCfmEvt;
#endif
{

   RlcMiLkwL2MeasCfm(&gCb->genCfg.lmPst, measCfmEvt);
   return ROK;
} /* kwUtlSndL2MeasNCfm */
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

Void rlcUtlResetDlL2MeasInRlcRb
(
RlcCb       *gCb,
RlcL2MeasCb *measCb,
uint8_t      measType
)
#else
Void rlcUtlResetDlL2MeasInRlcRb(gCb, measCb, measType)
RlcCb       *gCb;
RlcL2MeasCb *measCb;
uint8_t      measType;
#endif
{
   uint32_t           ueIdx;
   uint32_t           qciIdx;
   RlcDlUeCb      *ueCb = NULL;



   if (measCb->measType & LKW_L2MEAS_DL_IP)
   {
      for(ueIdx = 0; ueIdx < measCb->val.ipThMeas.numUes; ueIdx++)
      {           
         if (measCb->val.ipThMeas.ueInfoLst[ueIdx].isValid == TRUE)
         {
            for (qciIdx =0; qciIdx < measCb->val.ipThMeas.ueInfoLst[ueIdx].numQci; qciIdx++)
            {
               if (measType & LKW_L2MEAS_DL_IP)
               {
                  measCb->val.ipThMeas.ueInfoLst[ueIdx].measData[qciIdx].dlIpThruput.volSummation = 0;
                  measCb->val.ipThMeas.ueInfoLst[ueIdx].measData[qciIdx].dlIpThruput.timeSummation = 0;
                  gCb->u.dlCb->rlcL2Cb.measOn[qciIdx] &= ~measType; 
               }
            }

            if(ROK  != rlcDbmFetchDlUeCb(gCb,measCb->val.ipThMeas.ueInfoLst[ueIdx].ueId,
                     measCb->val.ipThMeas.ueInfoLst[ueIdx].cellId, &ueCb))
            {
               continue;
            }

         }
      }
   }
   else
   {
      /* for now the only meas should be DL discard in this case */
      if (measCb->measType & LKW_L2MEAS_DL_DISC)
      {
         uint32_t i;
         for(i = 0; i < measCb->val.nonIpThMeas.numQci; i++)
         {
            uint8_t qciVal = measCb->val.nonIpThMeas.qci[i];

            measCb->val.nonIpThMeas.measData[qciVal].dlDisc.discSdus = 0;
            measCb->val.nonIpThMeas.measData[qciVal].dlDisc.totSdus  = 0;
         }
         
      }
      if (measCb->measType & LKW_L2MEAS_DL_DELAY)
      {
         uint32_t i;
         for(i = 0; i < measCb->val.nonIpThMeas.numQci; i++)
         {
            uint8_t qciVal = measCb->val.nonIpThMeas.qci[i];

            measCb->val.nonIpThMeas.measData[qciVal].dlPjSduDelay.sduDelay = 0;
         }
      }
      measCb->val.nonIpThMeas.numQci = 0;
   }
} /* rlcUtlResetDlL2MeasInRlcRb */
#endif

static Void dumpRLCDlRbInformation(RlcDlRbCb* dlRbCb)
{
   if(dlRbCb->mode == CM_LTE_MODE_UM)
   {
      RLOG_ARG3(L_DEBUG,DBG_RBID,dlRbCb->rlcId.rbId,
               "UM Downlink UEID:%d CELLID:%d Q size = %d",
                       dlRbCb->rlcId.ueId,
                       dlRbCb->rlcId.cellId,
                       (int)dlRbCb->m.umDl.sduQ.count);
   }
   else if(dlRbCb->mode == CM_LTE_MODE_AM)
   {
      uint32_t j, numTxPdus=0;
      for(j = 0; j <= (RLC_AM_GET_WIN_SZ(dlRbCb->m.amDl.snLen)); j++)
      {
         RlcTx *txBuf = rlcUtlGetTxBuf(dlRbCb->m.amDl.txBufLst, j);
         if(txBuf != NULLP)
         {
            numTxPdus++;
         }
      }
      RLOG_ARG4(L_DEBUG,DBG_RBID,dlRbCb->rlcId.rbId,
               "AM Downlink UEID:%d CELLID:%d Sizes SDU Q = %d TX Q = %d ",
                       dlRbCb->rlcId.ueId,
                       dlRbCb->rlcId.cellId,
                       (int)dlRbCb->m.amDl.sduQ.count,
                       (int)numTxPdus);
      RLOG_ARG3(L_DEBUG,DBG_RBID,dlRbCb->rlcId.rbId,
               "AM Downlink UEID:%d CELLID:%d RETX Q= %d",
                       dlRbCb->rlcId.ueId,
                       dlRbCb->rlcId.cellId,
                       (int)dlRbCb->m.amDl.retxLst.count);
   }
}

Void DumpRLCDlDebugInformation(Void)
{
   RlcCb* dlInst = rlcCb[1]; /* TODO : Check whether DL is 0 or 1 */

   RlcDlCb *dlCb = dlInst->u.dlCb;

   RlcDlUeCb *ueCb = NULLP; 
   RTLIN_DUMP_DEBUG("RLC Information\n");
   RTLIN_DUMP_DEBUG("===============\n");
   /* Until no more ueCb is ueLstCp hash list get and delete ueCb */
   while (ROK == cmHashListGetNext(&dlCb->ueLstCp, 
                                   (PTR) ueCb, 
                                   (PTR *)&ueCb))
   {
      uint32_t i;
      for(i = 0; i< RLC_MAX_SRB_PER_UE; i++)
      {
         RlcDlRbCb* dlRbCb = ueCb->srbCb[i]; 
         if( dlRbCb != NULLP)
         {
            dumpRLCDlRbInformation(dlRbCb);
         }
      }
      for(i = 0; i< RLC_MAX_DRB_PER_UE; i++)
      {
         RlcDlRbCb* dlRbCb = ueCb->drbCb[i]; 
         if( dlRbCb != NULLP)
         {
            dumpRLCDlRbInformation(dlRbCb);
         }
      }
   }

   RlcDlDataToBeFreed* pToBeFreed = &dlCb->toBeFreed;

   RTLIN_DUMP_DEBUG("toBeFreed RETX list size = %d\n",(int)pToBeFreed->reTxLst.count);
   RTLIN_DUMP_DEBUG("toBeFreed TX list size   = %d\n",(int)pToBeFreed->txLst.count);
   RTLIN_DUMP_DEBUG("toBeFreed SDU list size  = %d\n",(int)pToBeFreed->sduLst.count);
   RTLIN_DUMP_DEBUG("toBeFreed RB list size   = %d\n",(int)pToBeFreed->rbLst.count);
}

/**
 *
 * @b Description
 *        This function frees downlink memory 
 *
 *  @param[in] Void
 *
 *
 *  @return  Void
 */

#ifdef ANSI
void rlcUtlFreeDlMem
(
 Void
)
#else
void rlcUtlFreeDlMem()
Void;
#endif
{
  rlcUtlFreeDlMemory(RLC_GET_RLCCB(RLC_DL_INST));
}

/**
 *
 * @b Description
 *        This function returns current time
 *
 *  @param[in] uint32_t 
 *
 *
 *  @return  Void
 */

void rlcUtlGetCurrTime(uint32_t  *currTime)
{
   /* Need t define else part for PAL */
   *currTime = SGetTtiCount();
}

#if defined(MAC_RLC_HARQ_STA_RBUF) || defined (SS_RBUF)
#ifdef LTE_L2_MEAS
#ifdef ANSI
void rlcUtlDlBatchProcHqStaInd
(
 Void
)
#else
void rlcUtlDlBatchProcHqStaInd()
Void;
#endif
{
   /* Read from Ring Buffer and process PDCP packets */
   //Pst pst = {0};

   Void *elmIndx = NULLP;
   RguHarqStaInd *staInd = NULLP;

#if defined(MAC_RLC_HARQ_STA_RBUF) && defined(LTE_L2_MEAS)
   isDatReqProcessed = TRUE;
#endif
   elmIndx = (Void *)SRngGetRIndx(SS_RNG_BUF_MAC_HARQ);
   while(NULLP != elmIndx)
   {
      staInd = (RguHarqStaInd *)elmIndx;
      RlcLiRguHqStaInd(&(staInd->pst), 0, &(staInd->hqStatusInd));

      elmIndx = NULLP;
      staInd  = NULLP;
      SRngIncrRIndx(SS_RNG_BUF_MAC_HARQ);

      if((elmIndx = (Void *)SRngGetRIndx(SS_RNG_BUF_MAC_HARQ)) == NULLP)
      {
#if defined(MAC_RLC_HARQ_STA_RBUF) && defined(LTE_L2_MEAS)
         isDatReqProcessed = FALSE;
#endif
         break;
      }
   }
}
#endif
#endif

/**
 * @brief evaluate and trigger PDB based flow control to PDCP 
 *
 * @details 
 *
 * @param[in]   rbCb  RB control block 
 *
 *
 *  @return  Void
 *
 */
#ifdef ANSI
Void rlcUtlTrigPdbFlowCntrl
(
RlcCb       *gCb,
RlcDlRbCb   *rbCb,
uint32_t    pktAdmitCnt
)
#else
Void rlcUtlTrigPdbFlowCntrl(gCb,rbCb,pktAdmitCnt)
RlcCb       *gCb;
RlcDlRbCb   *rbCb;
uint32_t    pktAdmitCnt;
#endif
{
   KwuFlowCntrlIndInfo    *flowCntrlInfo;
   RlcKwuSapCb*            rlckwuSap;

   rlckwuSap = gCb->u.dlCb->rlcKwuDlSap + RLC_UI_PDCP;

   RLC_SHRABL_STATIC_BUF_ALLOC(rlckwuSap->pst.region, 
                              rlckwuSap->pst.pool, 
                              flowCntrlInfo, 
                              sizeof(KwuFlowCntrlIndInfo));
   flowCntrlInfo->rlcId       = rbCb->rlcId;
   flowCntrlInfo->pktAdmitCnt = pktAdmitCnt;
   RlcUiKwuFlowCntrlInd(&rlckwuSap->pst, rlckwuSap->suId, flowCntrlInfo);
}

/**
 *
 * @brief Store the DL buffer in hashList  
 *
 *
 * @b Description
 *
 *   Use the SN % binSize as key and store the received UL buffer
 *  @param[in] txBufLst       List CP array
 *  @param[in] txBuf          transmitted buffer
 *  @param[in] sn              sn of the received buffer 
 *
 *
 *  @return  Void
 */
void rlcUtlStoreTxBuf(CmLListCp *txBufLst, RlcTx *txBuf, RlcSn sn)
{
   uint32_t   hashKey; 

   hashKey = (sn % RLC_TX_BUF_BIN_SIZE );
   txBuf->sn = sn; 
   txBuf->lnk.node = (PTR)txBuf;
   cmLListAdd2Tail(&(txBufLst[hashKey]), &txBuf->lnk);

   return;
} /* rlcUtlStoreRecBuf */

/**
 *
 * @brief Retrieve the DL buffer from the list
 *
 *
 * @Description
 *
 *   Use the SN % binSize as key and retrieve the DL buffer
 *  @param[in] txBufLst       List CP array
 *  @param[in] sn              sn of the transmitted buffer 
 *
 *
 *  @return  Void
 */
RlcTx* rlcUtlGetTxBuf(CmLListCp *txBufLst, RlcSn sn)
{
   uint32_t            hashKey; 
   CmLListCp           *txBufLstCp;
   RlcTx               *txBuf;
   CmLList             *node = NULLP;

   hashKey = (sn % RLC_TX_BUF_BIN_SIZE ); 
 
   txBufLstCp = &txBufLst[hashKey];
   CM_LLIST_FIRST_NODE(txBufLstCp, node);
   while(node)
   {
      txBuf = (RlcTx *) node->node;
      if(txBuf->sn == sn)
      {
         return (txBuf);
      }
      CM_LLIST_NEXT_NODE(txBufLstCp, node);
   }
   return NULLP;
} /* rlcUtlStoreTxBuf */
/**
 *
 * @brief Delete the DL buffer from the list
 *
 *
 * @Description
 *
 *   Use the SN % binSize as key and retrieve the DL buffer
 *  @param[in] txBufLst       List CP array
 *  @param[in] sn              sn of the transmitted bffer 
 *
 *
 *  @return  Void
 */
#ifdef ANSI
Void rlcUtlDelTxBuf 
(
CmLListCp        *txBufLst,
RlcTx            *txBuf,
RlcCb              *gCb                              
)
#else
Void rlcUtlDelTxBuf(txBufLst, txBuf, gCb)
CmLListCp        *txBufLst;
RlcTx             *txBuf;
RlcCb             *gCb;                              
#endif
{
   uint32_t            hashKey; 
   CmLListCp           *txBufLstCp;

   hashKey = (txBuf->sn % RLC_TX_BUF_BIN_SIZE ); 
 
   txBufLstCp = &txBufLst[hashKey];
   //printf("D-sn(%d)\n", txBuf->hdr.sn);
   cmLListDelFrm(txBufLstCp, &txBuf->lnk);
   RLC_FREE_WC(gCb, txBuf, sizeof(RlcTx));
   return;
} /* rlcUtlDelTxBuf */

/**
 *
 * @brief Remove the DL buffer from the list
 *
 *
 * @Description
 *
 *   Use the SN % binSize as key and retrieve the DL buffer
 *  @param[in] txBufLst       List CP array
 *  @param[in] sn              sn of the transmitted bffer 
 *
 *
 *  @return  Void
 */
#ifdef ANSI
Void rlcUtlRemovTxBuf 
(
CmLListCp        *txBufLst,
RlcTx            *txBuf,
RlcCb              *gCb                              
)
#else
Void rlcUtlRemovTxBuf(txBufLst, txBuf, gCb)
CmLListCp        *txBufLst;
RlcTx             *txBuf;
RlcCb             *gCb;                              
#endif
{
   uint32_t            hashKey; 
   CmLListCp           *txBufLstCp;

   hashKey = (txBuf->sn % RLC_TX_BUF_BIN_SIZE ); 
 
   txBufLstCp = &txBufLst[hashKey];
   //printf("D-sn(%d)\n", txBuf->hdr.sn);
   cmLListDelFrm(txBufLstCp, &txBuf->lnk);
   return;
} /* rlcUtlRemovTxBuf */



/********************************************************************30**
         End of file
**********************************************************************/
