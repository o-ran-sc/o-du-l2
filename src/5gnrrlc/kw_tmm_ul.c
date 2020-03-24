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
                
                  --kwTmmQSdu
                  --kwTmmSndToLi
                  --kwTmmRcvFrmLi
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
#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */

#include "gen.h"           /* general */
#include "ssi.h"           /* system services */
#include "cm5.h"           /* common timer defines */
#include "cm_tkns.h"       /* common tokens defines */
#include "cm_mblk.h"       /* common memory allocation library defines */
#include "cm_llist.h"      /* common link list  defines  */
#include "cm_hash.h"       /* common hash list  defines */
#include "cm_lte.h"        /* common LTE defines */
#include "lkw.h"           /* LKW defines */
#include "ckw.h"           /* CKW defines */
#include "kwu.h"           /* KWU defines */
#include "rgu.h"           /* RGU defines */
#include "kw_env.h"        /* RLC environment options */

#include "kw.h"            /* RLC defines */
#include "kw_err.h"            /* RLC defines */
#include "kw_ul.h"

/* extern (.x) include files */
#include "gen.x"           /* general */
#include "ssi.x"           /* system services */

#include "cm5.x"           /* common timer library */
#include "cm_tkns.x"       /* common tokens */
#include "cm_mblk.x"       /* common memory allocation */
#include "cm_llist.x"      /* common link list */
#include "cm_hash.x"       /* common hash list */
#include "cm_lte.x"        /* common LTE includes */
#include "cm_lib.x"        /* common memory allocation library */
#include "lkw.x"           /* LKW */
#include "ckw.x"           /* CKW */
#include "kwu.x"           /* KWU */
#include "rgu.x"           /* RGU */

#include "kw.x"
#include "kw_ul.x"

#if defined(PRE_DEF_UE_CTX) || defined(PRE_DEF_UE_CTX_HO)
#ifdef EG_GEN_LOAD_5GTF
extern U32 loadStart;
#endif

EXTERN S16 PjUiPjuDatInd(Pst* pst,SuId suId,CmLtePdcpId * pdcpId,Buffer *mBuf);
#ifdef ANSI
PUBLIC Void AddUeContext
(
CmLteRnti   crnti,
U8 rrcMsgType
)
#else
PUBLIC Void AddUeContext(crnti,rrcMsgType)
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
  TRC2(kwTmmRcvFrmLi) 

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
   KwUiKwuDatInd(&ulPst1, datIndInfo, pdu);
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

#define KW_MODULE (KW_DBGMASK_TM | KW_DBGMASK_UL)

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
PUBLIC Void kwTmmRcvFrmLi
(
KwCb        *gCb,
KwUlRbCb    *rbCb,  
CmLteRnti   tCrnti,  
Buffer      *pdu      
)
#else
PUBLIC Void kwTmmRcvFrmLi(gCb,rbCb, tCrnti, pdu)
KwCb        *gCb;
KwUlRbCb    *rbCb;  
CmLteRnti   tCrnti; 
Buffer      *pdu;  
#endif
#else
#ifdef ANSI
PUBLIC Void kwTmmRcvFrmLi
(
KwCb       *gCb,
KwUlRbCb   *rbCb,         
Buffer     *pdu            
)
#else
PUBLIC Void kwTmmRcvFrmLi(gCb,rbCb, pdu)
KwCb       *gCb;
KwUlRbCb   *rbCb;         
Buffer     *pdu;         
#endif
#endif 
{
   KwuDatIndInfo   *datIndInfo;   /* Data Indication Information */
   MsgLen          msgLen;
 
   TRC2(kwTmmRcvFrmLi) 

   /* Creating static memory for KwuDatIndInfo. #else will be 
    * removed once the sanity testing is performed for all platforms */
   KwuDatIndInfo datIndInfoTmp;
   datIndInfo = &datIndInfoTmp;
#if (ERRCLASS & ERRCLS_ADD_RES)
   if ( datIndInfo == NULLP )
   {   
      RLOG_ARG2(L_FATAL,DBG_RBID,rbCb->rlcId.rbId,
            "Memory Allocation failed UEID:%d CELLID:%d",
            rbCb->rlcId.ueId,
            rbCb->rlcId.cellId);   
      RETVOID;
   }
#endif /* ERRCLASS & ERRCLS_ADD_RES */
   KW_MEM_CPY(&(datIndInfo->rlcId),&(rbCb->rlcId),sizeof(CmLteRlcId));
#ifdef CCPU_OPT 
   if ( rbCb->lch.lChType == CM_LTE_LCH_CCCH ) 
   {
      datIndInfo->tCrnti = tCrnti;
   }
#endif 
   gCb->genSts.pdusRecv++;
   SFndLenMsg(pdu, &msgLen);
   gCb->genSts.bytesRecv += msgLen;
   /* If trace flag is enabled send the trace indication */
   if(gCb->init.trc == TRUE)
   {
      /* Populate the trace params */
      kwLmmSendTrc(gCb,KWU_EVT_DAT_IND, pdu);
   }
   KwUiKwuDatInd( &gCb->u.ulCb->kwuUlSap->pst, 
                  //gCb->u.ulCb->kwuUlSap->suId, 
                  datIndInfo, pdu);
   
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
PUBLIC Void kwTmmUlReEstablish
(
KwCb        *gCb,
KwUlRbCb    *rbCb        
)
#else
PUBLIC Void kwTmmUlReEstablish(rbCb)
KwCb        *gCb;
KwRbCb      *rbCb;          
#endif
{
   TRC2(kwUlTmmReEstablish)

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
