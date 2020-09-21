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
  
     Desc:     Source code for RLC Transparent mode assembly and
               reassembly.This file contains following functions
                
                  --rlcTmmQSdu
                  --rlcTmmSndToLi
                  --rlcTmmRcvFrmLi
                  --kwTmmReEstablish 

     File:     kw_tmm_ul.c

**********************************************************************/
static const char* RLOG_MODULE_NAME="TMM";
static int RLOG_MODULE_ID=2048;
static int RLOG_FILE_ID=201;

/** @file kw_tmm_ul.c
@brief RLC Transparent Mode module
*/

/* header (.h) include files */
#include "common_def.h"
#include "lkw.h"           /* LKW defines */
#include "ckw.h"           /* CKW defines */
#include "kwu.h"           /* KWU defines */
#include "rgu.h"           /* RGU defines */
#include "kw_env.h"        /* RLC environment options */

#include "kw.h"            /* RLC defines */
#include "kw_err.h"            /* RLC defines */
#include "kw_ul.h"

/* extern (.x) include files */
#include "lkw.x"           /* LKW */
#include "ckw.x"           /* CKW */
#include "kwu.x"           /* KWU */
#include "rgu.x"           /* RGU */

#include "kw.x"
#include "kw_ul.x"
#include "du_app_rlc_inf.h"
#include "rlc_utils.h"
#include "rlc_upr_inf_api.h"

#if defined(PRE_DEF_UE_CTX) || defined(PRE_DEF_UE_CTX_HO)
#ifdef EG_GEN_LOAD_5GTF
extern U32 loadStart;
#endif

EXTERN S16 PjUiPjuDatInd(Pst* pst,SuId suId,CmLtePdcpId * pdcpId,Buffer *mBuf);
#ifdef ANSI
Void AddUeContext
(
CmLteRnti   crnti,
U8 rrcMsgType
)
#else
Void AddUeContext(crnti,rrcMsgType)
CmLteRnti   crnti,
U8 rrcMsgType 
#endif
{
   KwuDatIndInfo   *datIndInfo;   /* Data Indication Information */
   U8 rrcConReq[6] ={ 0x50, 0x30, 0x30, 0x30, 0x30, 0x34 };
   U8 rrcConSetupComplete[34] ={ 0x20, 0x00, 0x3e, 0x0e, 0x82, 0x02, 0x10, 0x12, 0x20, 0x02, 0x20, 0x64, 0xa8, 0x2c, 0x48, 0x05, 0x00, 0x80, 0x00, 0x08, 0x04, 0x03, 0xa0, 0x02, 0xa0, 0x10, 0x12, 0x20, 0x02, 0x20, 0x64, 0xa8, 0x2c, 0x48};
#ifndef CA_PAL_5GTF 
   U8 rrcUeCapabilityInfo[12] ={0x38, 0x01, 0x00, 0x80, 0x1b, 0xff, 0x0c, 0x00, 0x20, 0x00, 0x80, 0x00};
#else
  /* U8 rrcUeCapabilityInfo[44] ={0x38,0x01,0x02,0x84,0x9b,0xff,0x0c,0x00,0x20,0x00,0x80,0x1f,0xfe,0xf4,0x4f,0xe0,0x40,0x03,0x80,0x11,0x04,0x0c,0x20,0x88,0x20,0x7f,0xff,0xff,0xff,0xf3,0xff,0x81,0xff,0xff,0xff,0xff,0x7f,0xf0,0x3f,0xff,0xff,0xff,0xe0,0x00};
*/
U8 rrcUeCapabilityInfo[] = 
{
 0x38,0x01,0x03,0x34,0x9b,0xff,0x0c,0x00,0x20,0x00,0x80,0x1f,0xfe,0xf4,0x4f,0xe0
 ,0x40,0x09,0x80,0x11,0x04,0x0c,0x20,0x88,0x20,0x63,0x04,0x41,0x03,0x20,0x22,0x08
 ,0x19,0x61,0x10,0x40,0xff,0xff,0xff,0xff,0xe7,0xff,0x03,0xff,0xff,0xff,0xfe,0xff
 ,0xe0,0x7f,0xff,0xff,0xff,0xc0,0x00
};

#endif
   U8 rrcSecurityModeComplete[2] ={0x28, 0x80};
   U8 rrcReconfigComplete[2] ={0x10, 0x80};

   Buffer *pdu;

   Pst ulPst1 ={100,100,217,0,215,0,PRIOR0,0,81,1,1,0,0};
#ifdef PJ
   Pst ulPst2 ={100,100,217,0,216,0,PRIOR0,0,68,0,1,0,0};
#endif
  TRC2(rlcTmmRcvFrmLi) 

  if(1 == rrcMsgType)
  {

   KwuDatIndInfo datIndInfoTmp;
   datIndInfo = &datIndInfoTmp;
   datIndInfo->rlcId.rbId = 5;
   datIndInfo->rlcId.rbType = 0;
   datIndInfo->rlcId.ueId = 0;
   datIndInfo->rlcId.cellId = 1;
   datIndInfo->tCrnti = crnti;
   datIndInfo->isOutOfSeq = 16;

   SGetMsg(DFLT_REGION, DFLT_POOL, (Buffer **) &pdu);
  
   SAddPstMsgMult(rrcConReq,6,pdu);

   RLOG1(L_INFO,"Profiling Framework Sending RRC Connection Req to RRC for UE :%d\n",crnti);
   printf("Profiling Framework Sending RRC Connection Req to RRC for UE :%d\n",crnti);
   RlcUiKwuDatInd(&ulPst1, datIndInfo, pdu);
 }
 else if(2 == rrcMsgType)
 {
#ifdef PJ
    CmLtePdcpId pdcpId;
    pdcpId.cellId = 1;
    pdcpId.ueId = crnti;
    pdcpId.rbId = 1;
    pdcpId.rbType = 0;
#endif   
    SGetMsg(DFLT_REGION, DFLT_POOL, (Buffer **) &pdu);
  
    SAddPstMsgMult(rrcConSetupComplete,34,pdu);

    RLOG1(L_INFO,"Profiling Framework: Sending RRC Connection Setup Complete to RRC for UE :%d\n",crnti);
    printf("Profiling Framework: Sending RRC Connection Setup Complete to RRC for UE :%d\n",crnti);
#ifdef PJ
    PjUiPjuDatInd(&ulPst2, 1, &pdcpId, pdu);
#endif
 }
 
 else if(3 == rrcMsgType)
 {
#ifdef PJ
    CmLtePdcpId pdcpId;
    pdcpId.cellId = 1;
    pdcpId.ueId = crnti;
    pdcpId.rbId = 1;
    pdcpId.rbType = 0;
#endif   
    SGetMsg(DFLT_REGION, DFLT_POOL, (Buffer **) &pdu);
  
#ifndef CA_PAL_5GTF 
    SAddPstMsgMult(rrcUeCapabilityInfo,12,pdu);
#else    
    SAddPstMsgMult(rrcUeCapabilityInfo,sizeof(rrcUeCapabilityInfo),pdu);
#endif
    RLOG1(L_INFO,"Profiling Framework: Sending RRC UE Capability Info to RRC for UE :%d\n",crnti);
    printf("Profiling Framework: Sending RRC UE Capability Info to RRC for UE :%d\n",crnti);
#ifdef PJ
    PjUiPjuDatInd(&ulPst2, 1, &pdcpId, pdu);
#endif

 }
 else if(4 == rrcMsgType)
 {
#ifdef PJ
    CmLtePdcpId pdcpId;
    pdcpId.cellId = 1;
    pdcpId.ueId = crnti;
    pdcpId.rbId = 1;
    pdcpId.rbType = 0;
#endif
   
    SGetMsg(DFLT_REGION, DFLT_POOL, (Buffer **) &pdu);
  
    SAddPstMsgMult(rrcSecurityModeComplete,2,pdu);

    RLOG1(L_INFO,"Profiling Framework: Sending RRC Security Mode Complete to RRC for UE :%d\n",crnti);
    printf("Profiling Framework: Sending RRC Security Mode Complete to RRC for UE :%d\n",crnti);
#ifdef PJ
    PjUiPjuDatInd(&ulPst2, 1, &pdcpId, pdu);
#endif
 }
 else if(5 == rrcMsgType)
 {
#ifdef PJ
    CmLtePdcpId pdcpId;
    pdcpId.cellId = 1;
    pdcpId.ueId = crnti;
    pdcpId.rbId = 1;
    pdcpId.rbType = 0;
#endif
   
    SGetMsg(DFLT_REGION, DFLT_POOL, (Buffer **) &pdu);
  
    SAddPstMsgMult(rrcReconfigComplete,2,pdu);

    RLOG1(L_INFO,"Profiling Framework: Sending RRC Reconfig Complete to RRC for UE :%d\n",crnti);
    printf("Profiling Framework: Sending RRC Reconfig Complete to RRC for UE :%d\n",crnti);
#ifdef PJ
    PjUiPjuDatInd(&ulPst2, 1, &pdcpId, pdu);
#ifdef EG_GEN_LOAD_5GTF
    loadStart=1;
#endif
#endif
 }
 RETVOID;
}
#endif 
/** @addtogroup tmmode */
/*@{*/

#define RLC_MODULE (RLC_DBGMASK_TM | RLC_DBGMASK_UL)

/**
 * @brief
 *    Handler to process the Data Indication from the lower layer and send the 
 *    SDU to upper layer.
 *
 * @details
 *    This function processes the PDU receivied from the lower layer and sends 
 *    the same pdu as SDU to the upper layer.It sends CRNTI along with the SDU 
 *    for CCCH.
 *            
 * @param[in] gCb     RLC Instance Control Block
 * @param[in] rbCb    RB control block. 
 * @param[in] tCrnti  TCRNTI  
 * @param[in] pdu     PDU
 * 
 * @return  S16
 *    -# ROK 
 *    -# RFAILED 
 */
#ifdef CCPU_OPT
#ifdef ANSI
Void rlcTmmRcvFrmLi
(
RlcCb        *gCb,
RlcUlRbCb    *rbCb,  
CmLteRnti   tCrnti,  
Buffer      *pdu      
)
#else
Void rlcTmmRcvFrmLi(gCb,rbCb, tCrnti, pdu)
RlcCb        *gCb;
RlcUlRbCb    *rbCb;  
CmLteRnti   tCrnti; 
Buffer      *pdu;  
#endif
#else
#ifdef ANSI
Void rlcTmmRcvFrmLi
(
RlcCb       *gCb,
RlcUlRbCb   *rbCb,         
Buffer     *pdu            
)
#else
Void rlcTmmRcvFrmLi(gCb,rbCb, pdu)
RlcCb       *gCb;
RlcUlRbCb   *rbCb;         
Buffer     *pdu;         
#endif
#endif 
{
   RlcUlRrcMsgInfo  *ulRrcMsgInfo;
   uint16_t         msgLen;
   uint16_t         copyLen;    /* Number of bytes copied */
   Pst              pst;
 
   TRC2(rlcTmmRcvFrmLi) 

   gCb->genSts.pdusRecv++;
   SFndLenMsg(pdu, (MsgLen *)&msgLen);
   gCb->genSts.bytesRecv += msgLen;
   /* If trace flag is enabled send the trace indication */
   if(gCb->init.trc == TRUE)
   {
      /* Populate the trace params */
      rlcLmmSendTrc(gCb, EVENT_UL_RRC_MSG_TRANS_TO_DU, pdu);
   }
  
   /* Filling UL RRC Message Info */
   RLC_ALLOC_SHRABL_BUF(RLC_MEM_REGION_UL, RLC_POOL,
      ulRrcMsgInfo, sizeof(RlcUlRrcMsgInfo));
   if (ulRrcMsgInfo)
   {
      ulRrcMsgInfo->cellId = rbCb->rlcId.cellId;
      ulRrcMsgInfo->ueIdx = rbCb->rlcId.ueId;
      ulRrcMsgInfo->lcId = rbCb->lch.lChId;
      RLC_ALLOC_SHRABL_BUF(RLC_MEM_REGION_UL, RLC_POOL,
         ulRrcMsgInfo->rrcMsg, msgLen);
      if (ulRrcMsgInfo->rrcMsg)
      {
         SCpyMsgFix(pdu, 0, msgLen, ulRrcMsgInfo->rrcMsg, (MsgLen *)&copyLen);
         ulRrcMsgInfo->msgLen = msgLen;

         /* Sending UL RRC Message transfeer to DU APP */
         memset(&pst, 0, sizeof(Pst));
         FILL_PST_RLC_TO_DUAPP(pst, SFndProcId(), RLC_UL_INST, EVENT_UL_RRC_MSG_TRANS_TO_DU);
         rlcSendUlRrcMsgToDu(&pst, ulRrcMsgInfo);
      }
      else
      {
         DU_LOG("\nRLC : Memory allocation failed");
      }
   }
   else
   {
      DU_LOG("\nRLC : Memory allocation failed");
   }
 
   RETVOID;
}

/**
 * @brief 
 *    Handler to process the re-establishment request received from the upper 
 *    layer. It is just a hook for future enhancement. 
 *
 *            
 * @param[in] gCb   RLC Instance Control Block
 * @param[in] rbCb  RB control block. 
 *
 * @return  S16
 *    -# ROK 
 *       
 **/
#ifdef ANSI
Void rlcTmmUlReEstablish
(
RlcCb        *gCb,
RlcUlRbCb    *rbCb        
)
#else
Void rlcTmmUlReEstablish(rbCb)
RlcCb        *gCb;
RlcUlRbCb    *rbCb;          
#endif
{
   TRC2(rlcUlTmmReEstablish)

   RLOG_ARG0(L_DEBUG,DBG_RBID,rbCb->rlcId.rbId,"do nothing for TMM for ReEstablish");
   RETVOID;
}

#ifdef _cplusplus
}
#endif
/*@}*/
/********************************************************************30**
         End of file
**********************************************************************/
