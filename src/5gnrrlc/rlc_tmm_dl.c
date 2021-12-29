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
  
     Desc:     Source code for RLC Transparent mode assembly and
               reassembly.This file contains following functions
                
                  --rlcTmmQSdu
                  --rlcTmmSendToMac
                  --rlcTmmRcvFrmMac
                  --kwTmmReEstablish 

     File:     rlc_tmm_dl.c

**********************************************************************/
/** 
 * @file rlc_tmm_dl.c
 * @brief RLC Transparent Mode module
*/

/* header (.h) include files */
#include "common_def.h"
#include "lkw.h"           /* LKW defines */
#include "ckw.h"           /* CKW defines */
#include "kwu.h"           /* KWU defines */
#include "rgu.h"           /* RGU defines */
#include "rlc_env.h"        /* RLC environment options */
#include "rlc_err.h"            /* RLC defines */



/* extern (.x) include files */
#include "lkw.x"           /* LKW */
#include "ckw.x"           /* CKW */
#include "kwu.x"           /* KWU */
#include "rgu.x"           /* RGU */

#include "rlc_utils.h"            /* RLC defines */
#include "rlc_dl_ul_inf.h"
#include "rlc_dl.h"
#include "rlc_mac_inf.h"
#include "rlc_lwr_inf_api.h"

#define RLC_MODULE (RLC_DBGMASK_TM | RLC_DBGMASK_DL)

static Void rlcTmmSendBoSta ARGS((RlcCb *gCb, RlcDlRbCb *rbCb, 
                                 MsgLen bo, RlcDatReqInfo *datReqInfo));
uint32_t rgMacGT ;  
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
Void rlcTmmQSdu
(
RlcCb            *gCb,
RlcDlRbCb        *rbCb,      
RlcDatReqInfo   *datReqInfo, 
Buffer          *mBuf       
)
{
   RlcSdu   *sdu;              
 
   RLC_ALLOC(gCb,sdu,sizeof(RlcSdu));
#if (ERRCLASS & ERRCLS_ADD_RES)
   if ( sdu == NULLP )
   {
      DU_LOG("\nERROR  -->  RLC_DL : Memory Allocation failed UEID:%d CELLID:%d",   
            rbCb->rlcId.ueId, rbCb->rlcId.cellId);   
      return;
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

   rlcTmmSendBoSta(gCb, rbCb, sdu->sduSz, datReqInfo); 
   return;
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
void rlcTmmSendToMac(RlcCb *gCb, SuId suId, RlcDlRbCb *rbCb, RguCStaIndInfo *staInd)
{
   Pst              pst;
   CmLList          *node;          /* Current Link List Node */
   RlcSdu           *sdu;           /* SDU */
   RlcData          *dlData;
   uint16_t         pduLen;
   uint16_t         copyLen;
   int16_t          timeDiff = 0;
   Ticks            curTime  = 0;

   CM_LLIST_FIRST_NODE(&(rbCb->m.tm.sduQ), 
                       node);

   /* (Sfn,slot) at which the message should be transmitted is 
    * validated with alloted (sfn,slot)in the MAC layer */
   while (node != NULLP)
   {
      sdu = (RlcSdu *)(node->node);
      if ( rbCb->lch.lChType == CM_LTE_LCH_BCCH ||
            rbCb->lch.lChType == CM_LTE_LCH_PCCH )
      {
         uint16_t sfn, slot;
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
            DU_LOG("\nINFO  -->  RLC_DL : rlcTmmSendToMac: Releasing SDU of RNTI = %d for RNTI = %d \
	       UEID:%d CELLID:%d", sdu->mode.tm.rnti, staInd->rnti, rbCb->rlcId.ueId,
               rbCb->rlcId.cellId);   
            DU_LOG("\nINFO  -->  RLC_DL : rlcTmmSendToMac: sfn %d slot %d  UEID:%d CELLID:%d",
                  sfn, slot, rbCb->rlcId.ueId, rbCb->rlcId.cellId);   
            cmLListDelFrm(&(rbCb->m.tm.sduQ), &sdu->lstEnt);
            ODU_PUT_MSG_BUF(sdu->mBuf);
            RLC_FREE(gCb, sdu, sizeof(RlcSdu));
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
         DU_LOG("\nDEBUG  -->  RLC_DL : rlcTmmSendToMac: TMM: TmSdu Sta Indication received \
	    for Rnti %d Sdu Rnti %d UEID:%d CELLID:%d", staInd->rnti, 
            sdu->mode.tm.rnti, rbCb->rlcId.ueId, rbCb->rlcId.cellId);   
	 DU_LOG("\nDEBUG  -->  RLC_DL : rlcTmmSendToMac: TMM: TmSdu Sta Indication received : \
	    timeDiff %d SduQCnt %d UEID:%d CELLID:%d", timeDiff, rbCb->m.tm.sduQ.count,
            rbCb->rlcId.ueId, rbCb->rlcId.cellId);   
         if (timeDiff > 40)
         {
            /* Memory leak needs to be fixed */
            node = node->next;
            DU_LOG("\nERROR  -->  RLC_DL : rlcTmmSendToMac: timeDiff greater than 40, so deleting\
	       the Sdu %u UEID:%d CELLID:%d", sdu->mode.tm.rnti, rbCb->rlcId.ueId,
               rbCb->rlcId.cellId);   
            cmLListDelFrm(&(rbCb->m.tm.sduQ), &sdu->lstEnt);
            ODU_PUT_MSG_BUF(sdu->mBuf);
            RLC_FREE(gCb, sdu, sizeof(RlcSdu));
            continue;
         }

         if (sdu->mode.tm.rnti != staInd->rnti)
         {
            /* Memory leak needs to be fixed */
            node = node->next;
	    DU_LOG("\nDEBUG  -->  RLC_DL : rlcTmmSendToMac: TMM: Searching for Rnti %d Skipping \
	       Sdu for Rnti %d UEID:%d CELLID:%d", staInd->rnti, sdu->mode.tm.rnti, 
               rbCb->rlcId.ueId, rbCb->rlcId.cellId);   
	    DU_LOG("\nINFO  -->  RLC_DL : rlcTmmSendToMac: timeDiff %d sdu->arrTime %d UEID:%d CELLID:%d", 
                timeDiff, sdu->arrTime, rbCb->rlcId.ueId, rbCb->rlcId.cellId);   
            DU_LOG("\nINFO  -->  RLC_DL : rlcTmmSendToMac: curTime %d SduQCnt %d and continuing"
               " UEID:%d CELLID:%d", curTime, rbCb->m.tm.sduQ.count, rbCb->rlcId.ueId,
               rbCb->rlcId.cellId);   
            continue;
         }
         else
         {
            DU_LOG("\nDEBUG  -->  RLC_DL : rlcTmmSendToMac: TMM: TmSdu found %u UEID:%d CELLID:%d",
               sdu->mode.tm.rnti, rbCb->rlcId.ueId, rbCb->rlcId.cellId);   
            break;
         }
      }

   }
   if ( node == NULLP )
   {
      DU_LOG("\nERROR  -->  RLC_DL : rlcTmmSendToMac: SDU not found TM Queue is empty UEID:%d CELLID:%d",
         rbCb->rlcId.ueId, rbCb->rlcId.cellId);   
      return;
   }
   sdu = (RlcSdu *)node->node;

    RLC_ALLOC_SHRABL_BUF(RLC_MEM_REGION_DL, RLC_POOL,
                        dlData,(Size)sizeof(RlcData));
#if (ERRCLASS & ERRCLS_ADD_RES)
   if ( dlData == NULLP )
   {
      DU_LOG("\nERROR  -->  RLC_DL : rlcTmmSendToMac: Memory Allocation failed UEID:%d CELLID:%d",   
         rbCb->rlcId.ueId, rbCb->rlcId.cellId);   
      return; 
   }
#endif /* ERRCLASS & ERRCLS_ADD_RES */

   dlData->slotInfo.sfn = sdu->mode.tm.sfn;
   dlData->slotInfo.slot = sdu->mode.tm.slot;
   dlData->cellId = rbCb->rlcId.cellId;
   dlData->rnti = sdu->mode.tm.rnti;
   dlData->numPdu = 1;
   dlData->pduInfo[0].commCh = TRUE;
   dlData->pduInfo[0].lcId = rbCb->lch.lChId;

   /* Copy Message to fixed buffer to send */
   ODU_GET_MSG_LEN(sdu->mBuf, (MsgLen *)&pduLen);
   RLC_ALLOC_SHRABL_BUF(RLC_MEM_REGION_DL, RLC_POOL,
      dlData->pduInfo[0].pduBuf, pduLen);
   if (dlData->pduInfo[0].pduBuf == NULLP )
   {
      DU_LOG("\nERROR  -->  RLC_DL : Memory allocation failed");
      return;
   }
   ODU_COPY_MSG_TO_FIX_BUF(sdu->mBuf, 0, pduLen, \
      dlData->pduInfo[0].pduBuf, (MsgLen *)&copyLen);
   dlData->pduInfo[0].pduLen = pduLen;

   /* Free message */
   ODU_PUT_MSG_BUF(sdu->mBuf);

   /* kw005.201 ccpu00117318, updating the statistics */
   gCb->genSts.bytesSent += sdu->sduSz;
   gCb->genSts.pdusSent++;

   rlcUtlIncrementKwuStsSduTx(gCb->u.dlCb->rlcKwuDlSap + rbCb->k1wuSapId);   

   /* remove SDU from queue */ 
   sdu->mBuf = NULLP;
   cmLListDelFrm(&(rbCb->m.tm.sduQ),
                 &sdu->lstEnt); 
   RLC_FREE(gCb,sdu, sizeof(RlcSdu));

   /* Fill Pst structure. Copying rguSap->pst to pst to avoid any
    * changes in rguSap->pst */
   memset(&pst, 0, sizeof(Pst));
   FILL_PST_RLC_TO_MAC(pst, RLC_DL_INST, EVENT_DL_DATA_TO_MAC);

   if(RlcSendDlDataToMac(&pst, dlData) != ROK)
   {
      RLC_FREE_SHRABL_BUF(pst.region, pst.pool, dlData->pduInfo[0].pduBuf, \
         dlData->pduInfo[0].pduLen);
      RLC_FREE_SHRABL_BUF(pst.region, pst.pool, dlData, sizeof(RlcData));
   } 

   return;
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
Void rlcDlTmmReEstablish
(
RlcCb       *gCb,
RlcDlRbCb   *rbCb    
)
{

#ifdef LTE_L2_MEAS_RLC
   rlcUtlEmptySduQ(gCb, rbCb, &rbCb->m.tm.sduQ);
#else
   rlcUtlEmptySduQ(gCb,&rbCb->m.tm.sduQ);
#endif
   
   return;
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

static void rlcTmmSendBoSta(RlcCb *gCb, RlcDlRbCb *rbCb, MsgLen bo, RlcDatReqInfo *datReqInfo)
{
   Pst              pst;            /* Post structure */    
   RlcBoStatus      *boStatus;      /* Buffer occupancy status information */

   RLC_ALLOC_SHRABL_BUF(RLC_MEM_REGION_DL, RLC_POOL,
                       boStatus, sizeof(RlcBoStatus));
   if ( boStatus == NULLP )
   {
      DU_LOG("\nERROR  -->  RLC_DL : Memory Allocation failed UEID:%d CELLID:%d",\
            rbCb->rlcId.ueId, rbCb->rlcId.cellId);   
      return;
   }

   boStatus->cellId = rbCb->rlcId.cellId;
   boStatus->ueId = rbCb->rlcId.ueId;
   boStatus->commCh = TRUE;
   boStatus->lcId = rbCb->lch.lChId;
   boStatus->bo = bo;

   /* If trace flag is enabled send the trace indication */
   if(gCb->init.trc == TRUE)
   {
      /* Populate the trace params */
      rlcLmmSendTrc(gCb, EVENT_BO_STATUS_TO_MAC, NULLP);
   }

   /* Fill Pst structure. Copying rguSap->pst to pst to avoid any
    * changes in rguSap->pst */
   memset(&pst, 0, sizeof(Pst));
   FILL_PST_RLC_TO_MAC(pst, RLC_DL_INST, EVENT_DL_DATA_TO_MAC);

   if(RlcSendBoStatusToMac(&pst, boStatus) != ROK)
   {
      RLC_FREE_SHRABL_BUF(pst.region, pst.pool, boStatus, sizeof(RlcBoStatus));
   }

   return;
} 

#ifdef _cplusplus
}
#endif
/*@}*/
/********************************************************************30**
         End of file
**********************************************************************/
