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
  
        Name:    LTE-RLC Layer - Lower Interface
    
        Type:    C file
  
        Desc:    C source code for the lower interface of LTE-RLC
 
        File:    kw_ptli.c
  
*********************************************************************21*/

/** @file kw_ptli.c 
@brief RLC Lower Interface
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
#ifdef KW_PDCP
#include "cpj.h"           /* CPJ defines */
#include "pju.h"           /* PJU defines */
#include "lpj.h"           /* LPJ defines */
#endif
#include "kw_err.h"
#include "kw_env.h"        /* RLC environment options */
#include "kw.h"            /* RLC defines */

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
#ifdef KW_PDCP
#include "cpj.x"           /* CPJ defines */
#include "pju.x"           /* PJU defines */
#include "lpj.x"           /* LPJ defines */
#endif
#include "kw.x"
#include "ss_rbuf.h"
#include "ss_rbuf.x"

#ifdef EGTP_TEST
#include "mac_stub.h"
#endif /* EGTP_TEST */

#ifndef LCKWLIRGU
#define PTKWRGU
#endif

#ifndef RG
#define PTKWRGU
#endif

#ifdef __cplusplus
EXTERN "C" {
#endif /* __cplusplus */



#ifdef RLC_MAC_DAT_REQ_RBUF
PUBLIC S16 kwLiRguDatReqRbuf(Pst *Post,SpId spId,Void *datReq);
#endif

#ifdef RLC_MAC_STA_RSP_RBUF
PUBLIC S16 kwLiRguStaRspRbuf(Pst *Post,SpId spId,Void  *staRsp);
#endif
#if defined(MAC_RLC_HARQ_STA_RBUF) && defined(LTE_L2_MEAS)
EXTERN S16 KwDlHarqStaBatchProc (Void);
#endif


/*********************************************************************
 *             Primitives for RGU interface 
 ********************************************************************/

/* RGU Bind Request primitive */

PUBLIC RguBndReq kwLiRguBndReqMt[] =
{
#ifdef LCKWLIRGU
   cmPkRguBndReq,            /* 0 - loosely coupled */
#endif /* LCRGUIRGU */
#ifdef RG 
   RgUiRguBndReq,            /* 1 - tightly coupled, MAC  */
#endif /* RG */
#ifdef LCKWLIRGU
   cmPkRguBndReq,            /* 0 - loosely coupled */
#endif /* LCRGUIRGU */
};

/* RGU Unbind Request primitive */

PUBLIC RguBndReq kwLiRguUbndReqMt[] =
{
#ifdef LCKWLIRGU
   cmPkRguUbndReq,            /* 0 - loosely coupled */
#endif /* LCRGUIRGU */
#ifdef RG 
   RgUiRguUbndReq,            /* 1 - tightly coupled, MAC  */
#endif /* RG */
#ifdef LCKWLIRGU
   cmPkRguUbndReq,            /* 0 - loosely coupled */
#endif /* LCRGUIRGU */
};

/* RGU Dedicated Channel Data Request primitive */

PUBLIC RlcMacDlData rlcMacSendDlDataOpts[] =
{
#ifdef EGTP_TEST
   macStubSendDlData,
   macStubSendDlData,
   macStubSendDlData,
#else /* EGTP_TEST */
#ifdef LCKWLIRGU
   packDlData,            /* 0 - loosely coupled */
#endif /* LCRGUIRGU */
#ifdef RG 
   MacRlcProcDlData,      /* 1 - tightly coupled, MAC  */
#endif /* RG */
#ifdef LCKWLIRGU
   packDlData,            /* 0 - loosely coupled */
#endif /* LCRGUIRGU */
#endif /* EGTP_TEST */
};


/* RLC logical Channel Status primitive */

PUBLIC RlcMacBoStatus rlcMacSendBOStatusOpts[] =
{
#ifdef EGTP_TEST
   macStubBOStatus,
   macStubBOStatus,
   macStubBOStatus,
#else /* EGTP_TEST */
#ifdef LCKWLIRGU
   packBOStatus,            /* 0 - loosely coupled */
#endif /* LCRGUIRGU */
#ifdef RG 
   MacRlcProcBOStatus,            /* 1 - tightly coupled, MAC  */
#endif /* RG */
#ifdef LCKWLIRGU
   packBOStatus,            /* 0 - LWLC loosely coupled */
#endif /* LCRGUIRGU */
#endif /* EGTP_TEST */
};

/* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
#ifdef LTE_RLC_R9
/* RGU L2 Measurement Ul Ip Throughput Measurement Request primitive */

PUBLIC RguL2MUlThrpMeasReq kwLiRguL2MUlThrpMeasReqMt[] =
{
#ifdef LCKWLIRGU
   cmPkRguL2MUlThrpMeasReq,            /* 0 - loosely coupled */
#endif /* LCRGUIRGU */
#ifdef RG 
   RgUiRguL2MUlThrpMeasReq,            /* 1 - tightly coupled, MAC  */
#endif /* RG */
};
#endif /* LTE_RLC_R9 */
#endif /*  LTE_L2_MEAS */

/****************************************************************************
 *                         RGU Interface Mt functions
 ***************************************************************************/
/**
 *
 * @brief 
 *
 *        Handler for RGU SAP bind Request.
 *
 * @b Description:
 *
 *        This function is used by RLC to request for binding to 
 *        MAC for accessing MAC services.This function binds MAC's 
 *        SAP (identified by spId) with the service user's SAP 
 *        (identified by suId).
 *
 *  @param[in] pst   Post structure  
 *  @param[in] suId  Service user SAP ID 
 *  @param[in] spId  Service provider ID
 *
 *  @return  S16
 *      -# ROK 
 */

#ifdef ANSI
PUBLIC S16 KwLiRguBndReq
(
Pst  *post,                       /* post structure */
SuId suId,                      /* Service User Id */
SpId spId                       /* Service Provider Id */
)
#else
PUBLIC S16 KwLiRguBndReq(post, suId, spId)
Pst  *post;                       /* post structure */
SuId suId;                      /* Service User Id */
SpId spId;                      /* Service Provider Id */
#endif
{
   TRC3(KwLiRguBndReq)

   /* jump to specific primitive depending on configured selector */
   (*kwLiRguBndReqMt[post->selector])(post, suId, spId);

   RETVALUE(ROK);

} /* end of KwLiRguBndReq */


/**
 *
 * @brief 
 *
 *        Handler for bind confirmation from MAC.
 *
 * @b Description:
 *
 *        This function handles the bind confirmation received
 *        from MAC. 
 *
 *  @param[in] post     - Post structure  
 *  @param[in] suId    - Service provider SAP ID 
 *  @param[in] reason  - Reason of confirmation
 *
 *  @return  S16
 *      -# ROK 
 */

#ifdef ANSI
PUBLIC S16 KwLiRguUbndReq
(
Pst         *post,
SpId        spId,
Reason      reason
)
#else
PUBLIC S16 KwLiRguUbndReq(post, spId, reason)
Pst         *post;
SpId        spId;
Reason      reason;
#endif
{
   TRC3(KwLiRguUbndReq)

   /* jump to specific primitive depending on configured selector */
   (*kwLiRguUbndReqMt[post->selector])(post, spId, reason);

   RETVALUE(ROK);

} /* end of KwLiRguUbndReq */

  
/**
 *
 * @brief 
 *
 *        Handler for sending PDU(s) from RLC to MAC for dedicated logical channels. 
 *
 * @b Description:
 *
 *        This function sends PDU(s) to MAC via one or more dedicated 
 *        logical channels along with the Buffer Occupancy of these
 *        channels.
 *
 *  @param[in] post         Post structure  
 *  @param[in] spId        Service Provider ID
 *  @param[in] datIndInfo  Data Request Information 
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 *
 */
#ifdef ANSI
PUBLIC S16 RlcMacSendDlData
(
Pst               *post,
SpId              spId,
RlcMacData       *dlData
)
#else
PUBLIC S16 RlcMacSendDlData(post, spId, dlData)
Pst               *post;
SpId              spId;
RlcMacData        *dlData;
#endif
{
   TRC3(RlcMacSendDlData)
#ifdef RLC_MAC_DAT_REQ_RBUF
        post->event=EVTRGUDDATREQ;
      if((kwLiRguDatReqRbuf(post, spId, datReq)) != ROK)
      {

      SPutStaticBuffer(post->region, post->pool,                      
                      (Data *) datReq, sizeof(RguDDatReqInfo), 0);             
         RETVALUE(RFAILED);
      }
#else
   /* jump to specific primitive depending on configured selector */
   (*rlcMacSendDlDataOpts[post->selector])(post, spId, dlData);
#endif 
   RETVALUE(ROK);

} /* end of KwLiRguDDatReq */


  
/**
 *
 * @brief  
 *
 *        Handler for reporting the Buffer Occupancy to MAC 
 *        for logical channels.
 *
 * @b Description:
 *
 *        This function reports the Buffer Occupancy of one or more
 *         logical channels to MAC. 
 *
 *  @param[in] post         Post structure  
 *  @param[in] spId        Service Provider ID
 *  @param[in] boSta       BO Status Information 
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 *
 */
#ifdef ANSI
PUBLIC S16 RlcMacSendBOStatus
(
Pst               *post,
SpId              spId,
RlcMacBOStatus    *boSta
)
#else
PUBLIC S16 RlcMacSendBOStatus(post, spId, staRsp)
Pst               *post;
SpId              spId;
RlcMacBOStatus    *boSta;
#endif
{
   TRC3(RlcMacSendBOStatus)
#if defined(SPLIT_RLC_DL_TASK) && defined(RLC_MAC_STA_RSP_RBUF)
       post->event= EVTRGUDSTARSP;
      if((kwLiRguStaRspRbuf(post, spId, staRsp)) != ROK)
      {
         RETVALUE(RFAILED);
      }
#endif 
   /* jump to specific primitive depending on configured selector */
   (*rlcMacSendBOStatusOpts[post->selector])(post, spId, boSta);

   RETVALUE(ROK);

} /* end of RlcMacSendBOStatus */


/* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
#ifdef LTE_RLC_R9
  
/**
 *
 * @brief 
 *
 *        Handler for sending ulThrpMeasReqInfo from RLC to MAC for UL ip throughput measurement. 
 *
 * @b Description:
 *
 *        This function sends ulThrpMeasReqInfo from RLC to MAC whenver UL ip throughput
 * measurement is ON for a single or multiple qci in a UE. This is an indication for MAC
 * to start the T2/T1 time stamps for the coresponding LCHs in the UE.
 *
 *  @param[in] post                    Post structure  
 *  @param[in] spId                   Service Provider ID
 *  @param[in] ulThrpMeasReqInfo      Ul ip measurement request info
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 *
 */
#ifdef ANSI
PUBLIC S16 KwLiRguL2MUlThrpMeasReq
(
Pst                     *post,
SpId                    spId,
RguL2MUlThrpMeasReqInfo *l2mUlThrpMeasReq
)
#else
PUBLIC S16 KwLiRguL2MUlThrpMeasReq(post, spId, l2mUlThrpMeasReq)
Pst                     *post;
SpId                    spId;
RguL2MUlThrpMeasReqInfo *l2mUlThrpMeasReq;
#endif
{
   TRC3(KwLiRguL2MUlThrpMeasReq)

   /* jump to specific primitive depending on configured selector */
   (*kwLiRguL2MUlThrpMeasReqMt[post->selector])(post, spId, l2mUlThrpMeasReq);

   RETVALUE(ROK);

} /* end of KwLiRguL2MUlThrpMeasReq */

#endif /* LTE_RLC_R9 */
#endif /* LTE_L2_MEAS */


#ifdef MAC_RLC_UL_RBUF
PUBLIC S16 kwUlBatchProc ARGS ((Void));
EXTERN Void kwUtlFreeUlRBuf ARGS((void));

#ifdef ANSI
PUBLIC S16 kwUlBatchProc
(
Void
)
#else
PUBLIC S16 kwUlBatchProc()
Void;
#endif
{
/* Read from Ring Buffer and process PDCP packets */
   RguDDatIndInfo   *datInd;
   Void *elmIndx = NULLP;
   PRIVATE Pst rlcUlRbfuPst={1,1,ENTKW,0,ENTRG,0,PRIOR0,RTESPEC,EVTRLCULDAT,0,0,0,0};
/* Read from Ring Buffer and process PDCP packets */

#ifndef SS_RBUF
   RguDedDatInd1 *rguDatInd = NULLP;
   U8 rngBufDeqIndx = 0;

   elmIndx = SRngGetRIndx(SS_RNG_BUF_ULMAC_TO_ULRLC);
   while(NULLP != elmIndx)
   {
      rguDatInd = (RguDedDatInd1 *)elmIndx;
      datInd = (RguDDatIndInfo*) rguDatInd->msg; 
      SsRngInfoTbl[SS_RNG_BUF_ULMAC_TO_ULRLC].nPktProc++;;//Number of pkt processed in tti
      if(datInd != NULLP)
      {
         KwLiRguDDatInd(&rlcUlRbfuPst, 0, datInd);
      }
      else
      {
         RLOG0(L_ERROR,"Received NULL buffer");
      }
      rguDatInd->msg=NULLP;
      SRngIncrRIndx(SS_RNG_BUF_ULMAC_TO_ULRLC);
      elmIndx = NULLP;
      rguDatInd = NULLP;

      rngBufDeqIndx++;

      //if(rngBufDeqIndx >= SS_RNG_MAX_ULMAC_TO_ULRLC_DQ_CNT)
       // break;

      if((elmIndx = SRngGetRIndx(SS_RNG_BUF_ULMAC_TO_ULRLC)) == NULLP)
      break;
   }
#else
   elmIndx = SRngGetRIndx(SS_RNG_BUF_ULMAC_TO_ULRLC);
   while(NULLP != elmIndx)
   {
      datInd = (RguDDatIndInfo *)elmIndx;
      KwLiRguDDatInd(&rlcUlRbfuPst, 0, datInd);

      elmIndx = NULLP;
      datInd = NULLP;
      SRngIncrRIndx(SS_RNG_BUF_ULMAC_TO_ULRLC);

      if((elmIndx = SRngGetRIndx(SS_RNG_BUF_ULMAC_TO_ULRLC)) == NULLP)
      	break;
   }
#endif
   RETVALUE(ROK);

}

/**
 *
 * @brief 
 *        Handler to clear Ring buffer from UL RLC
 *
 * @details
 *         This function clears all the ring buffer content from  UL RLC
 *
 * @return  S16
 *      -# ROK 
 *      -# RFAILED
 *
 */
#ifdef ANSI
PUBLIC Void kwUtlFreeUlRBuf(void)
#else
PUBLIC Void kwUtlFreeUlRBuf()  
#endif
{
   RguDDatIndInfo *datInd;
   PTR            elem;
   U8             numLch;
   U8             numPdu;

   TRC2(kwUtlFreeUlRBuf)
   /* Free SS_RNG_BUF_ULMAC_TO_ULRLC  */
   while((SDeqSRngBuf (SS_RNG_BUF_ULMAC_TO_ULRLC, &elem) == ROK))
   {
      datInd = (RguDDatIndInfo *)elem; 
      for(numLch = 0; numLch< datInd->numLch; numLch++)
      {
         for(numPdu = 0; numPdu < datInd->lchData[numLch].pdu.numPdu; numPdu++)
         {
            if(datInd->lchData[numLch].pdu.mBuf[numPdu])
            {
               KW_FREE_BUF_WC(datInd->lchData[numLch].pdu.mBuf[numPdu]);
            }
         }
      }
      KW_PST_FREE(0, 0, datInd, sizeof(RguDDatIndInfo)); 
   }
}
#endif
#ifdef RLC_MAC_STA_RSP_RBUF
#ifdef ANSI
PUBLIC S16 kwLiRguStaRspRbuf
(
Pst               *post,
SpId              spId,
Void             *staRsp
)
#else
PUBLIC S16 kwLiRguStaRspRbuf(post, spId, staRsp)
Pst               *post;
SpId              spId;
Void              *staRsp;
#endif /* ANSI */
{
   S16 ret1 = ROK;
   
   Void *elem = NULLP;

   RguDStaRspInfo  *staRspInfo = NULL;
   elem = SRngGetWIndx(SS_RNG_BUF_DLRLC_TO_DLMAC);
   if (NULLP != elem)
   {
      staRspInfo = (RguDStaRspInfo *)elem;
      cmMemcpy((U8 *)staRspInfo, (U8 *)staRsp, sizeof(RguDStaRspInfo)); 
      staRspInfo->post = *post;
      SRngIncrWIndx(SS_RNG_BUF_DLRLC_TO_DLMAC);
      SsRngInfoTbl[SS_RNG_BUF_DLRLC_TO_DLMAC].pktRate++;
   }   
   else
   {
      RLOG0(L_ERROR,"RLC DL STA RSP RBUF is FULL!!! ");
      SsRngInfoTbl[SS_RNG_BUF_DLRLC_TO_DLMAC].pktDrop++;
      ret1 = RFAILED;
   }
   RETVALUE(ret1);
} /* cmPkKwuDatReq */

#endif
#ifdef RLC_MAC_DAT_REQ_RBUF
#ifdef ANSI
PUBLIC S16 kwLiRguDatReqRbuf
(
Pst               *post,
SpId              spId,
Void             *datReq
)
#else
PUBLIC S16 kwLiRguDatReqRbuf(post, spId, datReq)
Pst               *post;
SpId              spId;
Void             *datReq;
#endif /* ANSI */
{
   S16 ret1 = ROK;
   
   Void *elem = NULLP;
   RguInfoRingElem *datReqRing=NULLP;
   elem = SRngGetWIndx(SS_RNG_BUF_DLRLC_TO_DLMAC_DAT_REQ);
   if (NULLP != elem)
   {
      datReqRing = (RguInfoRingElem *) elem;
      datReqRing->spId = spId;
      datReqRing->event = post->event;
      datReqRing->msg =datReq;
      SRngIncrWIndx(SS_RNG_BUF_DLRLC_TO_DLMAC_DAT_REQ);
      SsRngInfoTbl[SS_RNG_BUF_DLRLC_TO_DLMAC_DAT_REQ].pktRate++;
   }   
   else
   {
      printf("RLC DL DAT REQ RBUF is FULL!!! \n");
      SsRngInfoTbl[SS_RNG_BUF_DLRLC_TO_DLMAC_DAT_REQ].pktDrop++;
      ret1 = RFAILED;
   }
   RETVALUE(ret1);
} /* cmPkKwuDatReq */

#endif 

#ifdef __cplusplus
}
#endif /* __cplusplus */
  
/********************************************************************30**
         End of file
**********************************************************************/
