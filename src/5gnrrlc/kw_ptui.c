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
  
        Name:    LTE-RLC Layer - Upper Interface
    
        Type:    C file
  
        Desc:    C source code for the upper interface of LTE-RLC
 
        File:    kw_ptui.c
  
*********************************************************************21*/
static const char* RLOG_MODULE_NAME="UIM";
static int RLOG_MODULE_ID=2048;
static int RLOG_FILE_ID=199;

/** @file kw_ptui.c
@brief RLC Upper Interface
*/


/* header (.h) include files */
#include "common_def.h"
#include "lkw.h"           /* LKW defines */
#include "ckw.h"           /* CKW defines */
#include "kwu.h"           /* KWU defines */
#include "rgu.h"           /* RGU defines */

#include "kw_err.h"
#include "kw_env.h"        /* RLC environment options */
#include "kw.h"            /* RLC defines */

/* extern (.x) include files */
#include "lkw.x"           /* LKW */
#include "ckw.x"           /* CKW */
#include "kwu.x"           /* KWU */
#include "rgu.x"           /* RGU */
#if defined(MAC_RLC_UL_RBUF) || (defined(L2_L3_SPLIT) && defined(ICC_RECV_TSK_RBUF)) || defined(SS_RBUF)
#include "ss_rbuf.h"
#include "ss_rbuf.x"
#endif
#include "kw.x"
#if defined(L2_L3_SPLIT) && defined(ICC_RECV_TSK_RBUF)
#include "mt_plat_t33.h"
#include "mt_plat_t33.x"
#endif

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

  
/*********************************************************************
 *             Forward Declartion for KWU Porting Functions
 ********************************************************************/
#if defined(PDCP_RLC_DL_RBUF) || defined(SS_RBUF)
S16 rlcDlBatchProc ARGS ((Void));
S16 rlcUtlDlFreeRlcRBuf ARGS((Void));
void rlcUtlDlBatchProcHqStaInd ARGS ((Void));
Void rlcUtlFreeDlMem ARGS((Void));
SsRngBufCnt rngCb;
S16 rlcUtlDlBatchProcPkts ARGS((Void));
#endif

#if (defined(L2_L3_SPLIT) && defined(ICC_RECV_TSK_RBUF))
S16 rlcDlBatchProcSplit  ARGS((Void));
#endif

  
/*********************************************************************
 *             Primitives for KWU interface 
 ********************************************************************/

/* KWU Bind Confirm primitive */

KwuBndCfm kwUiKwuBndCfmMt[] =
{
#ifdef LCKWUIKWU
   cmPkKwuBndCfm,            /* 0 - loosely coupled */
#endif /* LCKWUIKWU */
#ifdef NH
   NhLiKwuBndCfm,            /* 1 - tightly coupled, RRC  */
#endif /* NH */
#ifndef KW_PDCP
#ifdef PJ
   PjLiKwuBndCfm,            /* 2 - tightly coupled, PDCP */
#endif /* NH */
#endif /* KW_PDCP */
#ifndef TENB_ACC
#ifdef LWLCKWUIKWU
   cmPkKwuBndCfm,      /* 3 - light weight loosely coupled */ 
#endif /* LCKWUIKWU */
#endif /*TENB_ACC*/
};

/* KWU Data Indication primitive */

KwuDatInd kwUiKwuDatIndMt[] =
{
#ifdef LCKWUIKWU
   cmPkKwuDatInd,            /* 0 - loosely coupled */
#endif /* LCKWUIKWU */
#ifdef NH
   NhLiKwuDatInd,            /* 1 - tightly coupled, RRC  */
#endif /* NH */
#ifdef KW_PDCP
#else
#ifdef PJ
   PjLiKwuDatInd,            /* 2 - tightly coupled, PDCP */
#endif /* NH */
#ifndef TENB_ACC
#ifdef LWLCKWUIKWU
   cmPkKwuDatInd,         /* 3 - light weight loosely coupled */ 
#endif /* LCKWUIKWU */
#endif /*TENB_ACC*/
#endif /* KW_PDCP */
};

#ifndef KW_PDCP
KwuDatCfm kwUiKwuDatCfmMt[] =
{
#ifdef LCKWUIKWU
   cmPkKwuDatCfm,            /* 0 - loosely coupled */
#endif /* LCKWUIKWU */
#ifdef PJ
   PjLiKwuDatCfm,            /* 2 - tightly coupled, PDCP */
#endif /* PJ */
#ifndef TENB_ACC
#ifdef LWLCKWUIKWU
   cmPkKwuDatCfm,         /* 3 - light weight loosely coupled */ 
#endif /* LCKWUIKWU */
#endif /*TENB_ACC*/
};

/* KWU AM Status Indication primitive */

KwuStaInd kwUiKwuStaIndMt[] =
{
#ifdef LCKWUIKWU
   cmPkKwuStaInd,            /* 0 - loosely coupled */
#endif /* LCKWUIKWU */
#ifdef PJ
   PjLiKwuStaInd,            /* 2 - tightly coupled, PDCP */
#endif /* PJ */
#ifndef TENB_ACC
#ifdef LWLCKWUIKWU
   cmPkKwuStaInd,         /* 3 - light weight loosely coupled */ 
#endif /* LCKWUIKWU */
#endif /*TENB_ACC*/
};

KwuReEstCmpInd kwUiKwuReEstCmpIndMt[] =
{
#ifdef LCKWUIKWU
   cmPkKwuReEstCmpInd,            /* 0 - loosely coupled */
#endif /* LCKWUIKWU */
#ifdef PJ
   PjLiKwuReEstCmpInd,            /* 2 - tightly coupled, PDCP */
#else
#endif /* PJ */
#ifndef TENB_ACC
#ifdef LWLCKWUIKWU
   cmPkKwuReEstCmpInd,    /* 3 - light weight loosely coupled */ 
#endif /* LCKWUIKWU */
#endif /*TENB_ACC*/
};

/* kw005.201 added support for L2 measurement */
KwuDiscSduCfm kwUiKwuDiscSduCfmMt[] =
{
#ifdef LCKWUIKWU
   cmPkKwuDiscSduCfm,            /* 0 - loosely coupled */
#endif /* LCKWUIKWU */
#ifdef PJ
   PjLiKwuDiscSduCfm,            /* 2 - tightly coupled, PDCP */
#endif
#ifndef TENB_ACC
#ifdef LWLCKWUIKWU
   cmPkKwuDiscSduCfm,    /* 3 - light weight loosely coupled */    
#endif /* PJ */
#endif /*TENB_ACC*/
};
KwuFlowCntrlInd kwUiKwuFlowCntrlIndMt[] =
{
#ifdef LCKWUIKWU
   cmPkKwuFlowCntrlInd,            /* 0 - loosely coupled */
#endif /* LCKWUIKWU */
#ifdef PJ
   PjLiKwuFlowCntrlInd,            /* 2 - tightly coupled, PDCP */
#endif /* PJ */
#ifdef LCKWUIKWU
   cmPkKwuFlowCntrlInd,            /* 3 - light weight loosely coupled */
#endif /* LCKWUIKWU */
};
#ifdef LTE_L2_MEAS
KwuDatAckInd kwUiKwuDatAckIndMt[] =
{
#ifdef LCKWUIKWU
   cmPkKwuDatAckInd,            /* 0 - loosely coupled */
#endif /* LCKWUIKWU */
#ifdef PJ
   PjLiKwuDatAckInd,            /* 2 - tightly coupled, PDCP */
#endif /* PJ */
#ifndef TENB_ACC
#ifdef LWLCKWUIKWU
   cmPkKwuDatAckInd,     /* 3 - light weight loosely coupled, portable */          
#endif /* PJ */
#endif /*TENB_ACC*/
};
#endif /* LTE_L2_MEAS */
/* KWU AM Data confirm primitive */

#endif /* KW_PDCP */

  
/****************************************************************************
 *                         KWU Interface Mt functions
 ***************************************************************************/
/**
 *
 * @brief 
 *
 *        Handler for confirming  the bind request received from KWU
 *        interface.
 *
 * @b Description:
 *
 *        This function send the bind confirm primitive to the RLC user,
 *        when it receives a bind request from its service user.
 *
 *  @param[in] pst     Post structure  
 *  @param[in] suId    Service user SAP ID 
 *  @param[in] status  Status of Confirmation 
 *
 *  @return  S16
 *      -# ROK 
 *
 */
S16 RlcUiKwuBndCfm
(
Pst     *pst,                       /* post structure */
SuId    suId,                      /* Service User Id */
uint8_t status                     /* Status */
)
{

   /* jump to specific primitive depending on configured selector */
   (*kwUiKwuBndCfmMt[pst->selector])(pst, suId, status);

   return ROK;

} /* end of RlcUiKwuBndCfm */


/**
 *
 * @brief 
 *
 *        Handler for sending the data(SDU) from upper layer. 
 *
 * @b Description:
 *
 *        This function is used to transfer a SDU received from the peer
 *        RLC entity to the service user(RRC/PDCP).
 *
 *  @param[in] pst         Post structure  
 *  @param[in] suId        Service User SAP ID 
 *  @param[in] datIndInfo  Data Request Information
 *  @param[in] mBuf         Data Buffer (SDU) 
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 */
S16 rlcSendUlDataToDu
(
Pst               *pst,
KwuDatIndInfo     *datInd,
Buffer            *mBuf
)
{

   /* jump to specific primitive depending on configured selector */
   (*kwUiKwuDatIndMt[pst->selector])(pst, datInd, mBuf);

   return ROK;

} /* end of rlcSendUlDataToDu */

   int rlcDatCfmsSent = 0;

#ifndef KW_PDCP
/**
 *
 * @brief 
 *
 *        Handler for sending the data confirmation to upper layer. 
 *
 * @b Description:
 *
 *        This function is used to send a confirmation to the service
 *        user about the data received by the peer RLC entity. 
 *
 *  @param[in] pst         Post structure  
 *  @param[in] suId        Service User SAP ID 
 *  @param[in] datCfmInfo  Data Confirmation Information
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 */
S16 RlcUiKwuDatCfm
(
Pst            *pst,
SuId           suId,
KwuDatCfmInfo  *datCfm
)
{
   rlcDatCfmsSent++;

   /* jump to specific primitive depending on configured selector */
   (*kwUiKwuDatCfmMt[pst->selector])(pst, suId, datCfm);

   return ROK;

} /* end of RlcUiKwuDatCfm */


/**
 *
 * @brief 
 *
 *        Handler for sending the Status Indication to the upper layer. 
 *
 * @b Description:
 *
 *        This function is used only by RLC AM entity.It send status 
 *        indication to the upper layer about the maximum number of 
 *        re-transmissions reached for a RLC AM entity. 
 *
 *  @param[in] pst         Post structure  
 *  @param[in] suId        Service User SAP ID 
 *  @param[in] staInd      RLC Entity Id  
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 */
S16 RlcUiKwuStaInd
(
Pst               *pst,
SuId              suId,
KwuStaIndInfo     *staInd
)
{

   /* jump to specific primitive depending on configured selector */
   (*kwUiKwuStaIndMt[pst->selector])(pst, suId, staInd);

   return ROK;

} /* end of RlcUiKwuStaInd */


/**
 *
 * @brief 
 *
 *        Handler for sending the Status Indication to the upper layer. 
 *
 * @b Description:
 *
 *        This function is used only by RLC AM entity.It send status 
 *        indication to the upper layer about the maximum number of 
 *        re-transmissions reached for a RLC AM entity. 
 *
 *  @param[in] pst         Post structure  
 *  @param[in] suId        Service User SAP ID 
 *  @param[in] staInd      RLC Entity Id  
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 */
S16 RlcUiKwuReEstCmpInd
(
Pst               *pst,
SuId              suId,
CmLteRlcId        rlcId
)
{
   RLOG0(L_DEBUG, "In RlcUiKwuReEstCmpInd");

   /* jump to specific primitive depending on configured selector */
   (*kwUiKwuReEstCmpIndMt[pst->selector])(pst, suId, rlcId);

   return ROK;

} /* end of RlcUiKwuReEstCmpInd */
/* kw005.201 added support for L2 Measurement */

/**
 *
 * @brief 
 *
 *        Handler for sending the Sdu Disc Cfm  to the upper layer. 
 *
 * @b Description:
 *       
 *       This function confirms the discard of an SDU .
 *
 *  @param[in] pst         Post structure  
 *  @param[in] suId        Service User SAP ID 
 *  @param[in] discCfm     Disc information.
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 */
S16 RlcUiKwuDiscSduCfm
(
Pst               *pst,
SuId              suId,
KwuDiscSduInfo    *discCfm
)
{

   /* jump to specific primitive depending on configured selector */
   (*kwUiKwuDiscSduCfmMt[pst->selector])(pst, suId, discCfm);

   return ROK;

} /* end of RlcUiKwuDiscSduCfm */ 

/**
 *
 * @brief 
 *
 *        Handler for sending Flow indication to the upper layer. 
 *
 * @b Description:
 *       
 *       This function indicates to PDCP if packets need to be stopped or
 *       started for a particular RB
 *
 *  @param[in] pst                Post structure  
 *  @param[in] suId               Service User SAP ID 
 *  @param[in] flowCntrlIndInfo   Flow control information.
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 */
S16 RlcUiKwuFlowCntrlInd(Pst *pst,SuId suId,KwuFlowCntrlIndInfo *flowCntrlIndInfo)
{

   /* jump to specific primitive depending on configured selector */
   (*kwUiKwuFlowCntrlIndMt[pst->selector])(pst, suId, flowCntrlIndInfo);

   return ROK;

} /* end of RlcUiKwuFlowCntrlInd */ 
#ifdef LTE_L2_MEAS

/**
 *
 * @brief 
 *
 *        Handler for sending the Data ack indication  to the upper layer. 
 *
 * @b Description:
 *       
 *       This function confirms the succesfull transmission of SDU
 *
 *  @param[in] pst         Post structure  
 *  @param[in] suId        Service User SAP ID 
 *  @param[in] datAckInd   DatAckInd
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 */
S16 RlcUiKwuDatAckInd(Pst *pst,SuId suId,KwuDatAckInfo *datAckInd)
{

   /* jump to specific primitive depending on configured selector */
   (*kwUiKwuDatAckIndMt[pst->selector])(pst, suId, datAckInd);

   return ROK;

} /* end of RlcUiKwuDatAckInd */ 
#endif /* LTE_L2_MEAS */
#endif /* KW_PDCP */


  
/*********************************************************************
 *             Primitives for CKW interface 
 ********************************************************************/

/* CKW Bind Confirm primitive */

CkwBndCfm kwUiCkwBndCfmMt[] =
{
#ifdef LCKWUICKW
   cmPkCkwBndCfm,            /* 0 - loosely coupled */
#endif /* LCCKUICKW */
#ifdef NH
   NhLiCkwBndCfm,            /* 1 - tightly coupled, RRC  */
#endif /* NH */
};

/* CKW Configuration confirm primitive */

RlcCfgCfm kwUiRlcCfgCfmMt[] =
{
#ifdef LCKWUICKW
   cmPkRlcCfgCfm,            /* 0 - loosely coupled */
#endif /* LCKWUICKW */
#ifdef NH
   NhLiRlcCfgCfm,            /* 1 - tightly coupled, RRC  */
#endif /* NH */
};

CkwUeIdChgCfm kwUiCkwUeIdChgCfmMt[] =
{
#ifdef LCKWUICKW
   cmPkCkwUeIdChgCfm,            /* 0 - loosely coupled */
#endif /* LCKWUICKW */
#ifdef NH
   NhLiCkwUeIdChgCfm,            /* 1 - tightly coupled, RRC  */
#endif /* NH */
};


  
/****************************************************************************
 *                         CKW Interface Mt functions
 ***************************************************************************/
/**
 *
 * @brief 
 *
 *        Handler for confirming  the bind request received from CKW
 *        interface.
 *
 * @b Description:
 *
 *        This function send the bind confirm primitive to the RLC user,
 *        when it receives a bind request from its service user.
 *
 *  @param[in] pst      - Post structure  
 *  @param[in] suId     - Service user SAP ID 
 *  @param[in] status   - Status
 *
 *  @return  S16
 *      -# ROK 
 */
S16 RlcUiCkwBndCfm
(
Pst     *pst,                       /* post structure */
SuId    suId,                      /* Service User Id */
uint8_t status                     /* Status */
)
{

   /* jump to specific primitive depending on configured selector */
   (*kwUiCkwBndCfmMt[pst->selector])(pst, suId, status);

   return ROK;

} /* end of RlcUiCkwBndCfm */


/**
 *
 * @brief 
 *
 *        Handler for sending a configuration confirm to RRC.
 *
 * @b Description:
 *
 *        This function is used by RLC user to send a configuration
 *        confirmation to RRC after configuring(add/delete/modify)
 *        the RLC entities. 
 *
 *  @param[in] pst      Post structure  
 *  @param[in] transId  Transaction Id 
 *  @param[in] cfmInfo  Config Confirmation Info 
 *
 *  @return  S16
 *      -# ROK 
 *
 */
S16 RlcUiRlcCfgCfm
(
Pst            *pst,                      /* post structure */
SuId           suId,                      /* Service User Id */
RlcCfgCfmInfo  *cfmInfo                    /* Configuration Confirm */
)
{

   /* jump to specific primitive depending on configured selector */
   (*kwUiRlcCfgCfmMt[pst->selector])(pst, suId, cfmInfo);

   return ROK;

} /* end of RlcUiRlcCfgCfm */


/**
 *
 * @brief 
 *
 *        Handler for sending a configuration for UE ID change.
 *
 * @b Description:
 *
 *        This function is used by RLC to send a configuration
 *        confirm for UE ID change.
 *
 *  @param[in] pst      Post structure  
 *  @param[in] suId     Service User Id
 *  @param[in] transId  Transaction Id 
 *  @param[in] cfmInfo  Config Confirmation Info 
 *
 *  @return  S16
 *      -# ROK 
 *
 */
S16 RlcUiCkwUeIdChgCfm 
(
Pst            *pst,                      /* post structure */
SuId           suId,                      /* Service User Id */
uint32_t       transId,
CkwUeInfo      *ueInfo,
CmStatus       status
)
{

   /* jump to specific primitive depending on configured selector */
   (*kwUiCkwUeIdChgCfmMt[pst->selector])(pst, suId, transId, ueInfo, status);

   return ROK;

} /* end of RlcUiRlcCfgCfm */


#if (defined(L2_L3_SPLIT) && defined(ICC_RECV_TSK_RBUF))
/**
 *
 * @brief 
 *
 *        rlcDlBatchProcSplit- process rbug messages
 *
 *  @return  S16
 *      -# ROK 
 */
S16 rlcDlBatchProcSplit(Void)
{

/* Read from Ring Buffer and process PDCP packets */
   Void *elmIndx = NULLP;
   RxmBufReq *datReq = NULLP;
#ifdef LTE_ADV
   uint32_t dlPktCount = 0;
#endif
   uint8_t rngBufDeqIndx = 0;
   uint32_t rngBufDeqMaxCnt;
   uint32_t rngBufCurrCnt;

   rngBufDeqMaxCnt = SS_RNG_MAX_DLPDCP_TO_DLRLC_DQ_CNT;
#ifdef TENB_T2K3K_SPECIFIC_CHANGES
   rngBufCurrCnt = SGetNumElemInRng(SS_RNG_BUF_RX_TO_DLRLC);
   if ( rngBufCurrCnt > (3 * SS_RNG_MAX_DLPDCP_TO_DLRLC_DQ_CNT))
   {
      if ( rngBufCurrCnt > (6 * SS_RNG_MAX_DLPDCP_TO_DLRLC_DQ_CNT))
      {
         /* Restablishment scenario */
         rngBufDeqMaxCnt = (4 * SS_RNG_MAX_DLPDCP_TO_DLRLC_DQ_CNT);
      }
      else
      {
         rngBufDeqMaxCnt = (3 * SS_RNG_MAX_DLPDCP_TO_DLRLC_DQ_CNT)/2;
      }
   }
#endif
   elmIndx = SRngGetRIndx(SS_RNG_BUF_RX_TO_DLRLC);
   while(NULLP != elmIndx)
   {
      datReq = (RxmBufReq *)elmIndx;
      if(datReq->mBuf != NULLP)
      {
        cmUnpkKwuDatReq(rlcProcDlData, &datReq->pst, datReq->mBuf);
      }
      else
      {
         RLOG0(L_ERROR, "mBuf is NULL");
         if(datReq->mBuf)
          cmUnpkKwuDatReq(rlcProcDlData, &datReq->pst, datReq->mBuf);

      }
      SsRngInfoTbl[SS_RNG_BUF_RX_TO_DLRLC].nPktProc++;//Number of pkt processed in tti
      datReq->mBuf = NULLP;
      SRngIncrRIndx(SS_RNG_BUF_RX_TO_DLRLC);
      rngBufDeqIndx++;
      if(rngBufDeqIndx >= rngBufDeqMaxCnt)
      {
         break;
      }
#ifdef LTE_ADV
      {
        dlPktCount++;
        if(dlPktCount > 75)
        {
           break;
        }
      }
#endif
      if((elmIndx = SRngGetRIndx(SS_RNG_BUF_RX_TO_DLRLC)) == NULLP)
      {
         break;
      }
   }

   return ROK;
}
#endif

#if defined(PDCP_RLC_DL_RBUF) || defined(SS_RBUF)
S16 rlcDlBatchProc(Void)
{
/* Read from Ring Buffer and process PDCP packets */

   uint8_t rngBufDeqIndx = 0;
   uint32_t rngBufDeqMaxCnt;
#ifdef TENB_T2K3K_SPECIFIC_CHANGES
   uint32_t rngBufCurrCnt;
#endif
   /* Memory regions different for BRCM and Intel */
   /*dstProcId,srcProcId,dstEnt,dstInst,srcEnt,srcInst,prior,route,event,region,pool,selector*/
#ifdef SS_RBUF
   static Pst rlcDlRbfuPst ={1,1,ENTRLC,1,ENTPJ,1,PRIOR0,RTESPEC,KWU_EVT_DAT_REQ,1,1,0,0};
#else
   static Pst rlcDlRbfuPst ={1,1,ENTRLC,1,ENTPJ,1,PRIOR0,RTESPEC,KWU_EVT_DAT_REQ,2,1,0,0};
#endif
   Void *elmIndx = NULLP;
   KwuDatReqDetl *kwuDatReqDetl = NULLP;
   KwuDatReqInfo datReq;

   rngBufDeqMaxCnt = SS_RNG_MAX_DLPDCP_TO_DLRLC_DQ_CNT;
#ifdef TENB_T2K3K_SPECIFIC_CHANGES
   rngBufCurrCnt = SGetNumElemInRng(SS_RNG_BUF_DLPDCP_TO_DLRLC);
   if ( rngBufCurrCnt > (3 * SS_RNG_MAX_DLPDCP_TO_DLRLC_DQ_CNT))
   {
      if ( rngBufCurrCnt > (5 * SS_RNG_MAX_DLPDCP_TO_DLRLC_DQ_CNT))
      {
         /* Restablishment scenario */
         rngBufDeqMaxCnt = (4 * SS_RNG_MAX_DLPDCP_TO_DLRLC_DQ_CNT);
      }
      else
      {
         rngBufDeqMaxCnt = (2 * SS_RNG_MAX_DLPDCP_TO_DLRLC_DQ_CNT);
      }
   }
#endif
   elmIndx = SRngGetRIndx(SS_RNG_BUF_DLPDCP_TO_DLRLC);
   while(NULLP != elmIndx)
   {
      kwuDatReqDetl = (KwuDatReqDetl *)elmIndx;
      datReq.rlcId = kwuDatReqDetl->rlcId;
      datReq.sduId = kwuDatReqDetl->sduId;
      datReq.lcType = kwuDatReqDetl->lcType;
      SsRngInfoTbl[SS_RNG_BUF_DLPDCP_TO_DLRLC].nPktProc++;//Number of pkt processed in tti
      if(kwuDatReqDetl->mBuf != NULLP)
      {
         rlcProcDlData(&rlcDlRbfuPst, kwuDatReqDetl->spId, &datReq, kwuDatReqDetl->mBuf);
      }
      SRngIncrRIndx(SS_RNG_BUF_DLPDCP_TO_DLRLC);
      rngBufDeqIndx++;

      if(rngBufDeqIndx >= rngBufDeqMaxCnt)
      {
         break;
      }
      elmIndx = (Void *)SRngGetRIndx(SS_RNG_BUF_DLPDCP_TO_DLRLC);
   }

   return ROK;
}


S16 rlcUtlDlBatchProcPkts(Void)
{
    rlcDlBatchProc();
#ifdef SS_RBUF
#ifdef LTE_L2_MEAS
    rlcUtlDlBatchProcHqStaInd();
#endif
    rlcUtlFreeDlMem();
#endif
    return ROK;
}   


S16 rlcUtlDlFreeRlcRBuf(Void)
{
/* Read from Ring Buffer and process PDCP packets */
   Void *elmIndx = NULLP;
   KwuDatReqDetl *kwuDatReqDetl = NULLP;
   /* Free SS_RNG_BUF_DLPDCP_TO_DLRLC  */
   elmIndx = SRngGetRIndx(SS_RNG_BUF_DLPDCP_TO_DLRLC);
   while(NULLP != elmIndx)
   {
      kwuDatReqDetl = (KwuDatReqDetl *)elmIndx;
      SRngIncrRIndx(SS_RNG_BUF_DLPDCP_TO_DLRLC);
      SsRngInfoTbl[SS_RNG_BUF_DLPDCP_TO_DLRLC].nPktProc++;
      SPutMsg(kwuDatReqDetl->mBuf);
      elmIndx = NULLP;
      kwuDatReqDetl = NULLP;
      if((elmIndx = SRngGetRIndx(SS_RNG_BUF_DLPDCP_TO_DLRLC)) == NULLP)
      break;
   }
   return ROK;
}


#endif 
#ifdef __cplusplus
}
#endif /* __cplusplus */
/********************************************************************30**
         End of file
**********************************************************************/
