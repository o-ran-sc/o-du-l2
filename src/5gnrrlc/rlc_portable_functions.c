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
  
        Name:    NR RLC Layer - Lower Interface
    
        Type:    C file
  
        Desc:    C source code for the lower interface of NR RLC
 
        File:    kw_ptli.c
  
*********************************************************************21*/

/** @file rlc_portable_functions.c
@brief RLC Lower Interface
*/


/* header (.h) include files */
#include "common_def.h"
#include "lkw.h"           /* LKW defines */
#include "ckw.h"           /* CKW defines */
#include "kwu.h"           /* KWU defines */
#include "rgu.h"           /* RGU defines */
#ifdef KW_PDCP
#include "cpj.h"           /* CPJ defines */
#include "pju.h"           /* PJU defines */
#include "lpj.h"           /* LPJ defines */
#endif
#include "rlc_err.h"
#include "rlc_env.h"        /* RLC environment options */

/* extern (.x) include files */
#include "lkw.x"           /* LKW */
#include "ckw.x"           /* CKW */
#include "kwu.x"           /* KWU */
#include "rgu.x"           /* RGU */
#ifdef KW_PDCP
#include "cpj.x"           /* CPJ defines */
#include "pju.x"           /* PJU defines */
#include "lpj.x"           /* LPJ defines */
#endif

#include "rlc_utils.h"            /* RLC defines */
/*
#if defined(MAC_RLC_UL_RBUF) || (defined(L2_L3_SPLIT) && defined(ICC_RECV_TSK_RBUF)) || defined(SS_RBUF)
#include "ss_rbuf.h"
#include "ss_rbuf.x"
#endif
*/
#include "ss_rbuf.h"
#include "ss_rbuf.x"
#if defined(L2_L3_SPLIT) && defined(ICC_RECV_TSK_RBUF)
#include "mt_plat_t33.h"
#include "mt_plat_t33.x"
#endif

#ifndef LCKWLIRGU
#define PTKWRGU
#endif

#ifndef RG
#define PTKWRGU
#endif

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* local defines */
#define MAXKWMI 2               /* max. layer management interfaces */

#ifdef RLC_MAC_DAT_REQ_RBUF
S16 rlcLiRguDatReqRbuf(Pst *Post,SpId spId,Void *datReq);
#endif

#ifdef RLC_MAC_STA_RSP_RBUF
S16 rlcLiRguStaRspRbuf(Pst *Post,SpId spId,Void  *staRsp);
#endif
#if defined(MAC_RLC_HARQ_STA_RBUF) && defined(LTE_L2_MEAS)
S16 RlcDlHarqStaBatchProc (Void);
#endif


/*********************************************************************
 *             Primitives for RGU interface 
 ********************************************************************/

/* RGU Bind Request primitive */

RguBndReq kwLiRguBndReqMt[] =
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

RguBndReq kwLiRguUbndReqMt[] =
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

/* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
#ifdef LTE_RLC_R9
/* RGU L2 Measurement Ul Ip Throughput Measurement Request primitive */

RguL2MUlThrpMeasReq kwLiRguL2MUlThrpMeasReqMt[] =
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

/*********************************************************************
 *             Primitives for LKW interface 
 ********************************************************************/
/* Configuration confirmation primitive */

static RlcConfigCfm rlcMiRlcConfigCfmMt[MAXKWMI] =
{
#ifdef LCKWMILKW
   packRlcConfigCfm,            /* 0 - loosely coupled - fc */
#endif /* LCRLMILKW */
#ifdef SM
   SmMiRlcConfigCfm,            /* 1 - tightly coupled layer management*/
#endif /* SM */
};

/* control confirmation primitives */

static LkwCntrlCfm kwMiLkwCntrlCfmMt[MAXKWMI] =
{
#ifdef LCKWMILKW 
   cmPkLkwCntrlCfm,          /* 0 - loosely coupled - fc */
#endif /* LCRLMILKW */
#ifdef SM
   SmMiLkwCntrlCfm,          /* 1 - tightly coupled layer management*/
#endif /* SM */
};

/* Status Indication primitive */

static LkwStaInd kwMiLkwStaIndMt[MAXKWMI] =
{
#ifdef LCKWMILKW 
   cmPkLkwStaInd,            /* 0 - loosely coupled  */
#endif /* LCKWMILKW */
#ifdef SM
   SmMiLkwStaInd,            /* 1 - tightly coupled, layer management */
#endif /* SM */
};

/* Status confirm primitive */

static LkwStaCfm kwMiLkwStaCfmMt[MAXKWMI] =
{
#ifdef LCKWMILKW 
   cmPkLkwStaCfm,            /* 0 - loosely coupled  */
#endif /* LCKWMILKW */
#ifdef SM
   SmMiLkwStaCfm,            /* 1 - tightly coupled, layer management */
#endif /* SM */
};

/* Statistics confirm primitive */

static LkwStsCfm kwMiLkwStsCfmMt[MAXKWMI] =
{
#ifdef LCKWMILKW
   cmPkLkwStsCfm,            /* 0 - loosely coupled  */
#endif /* LCRLMILKW */
#ifdef SM
   SmMiLkwStsCfm,            /* 1 - tightly coupled, layer management */
#endif /* SM */
};

/* Trace indication primitive */

static LkwTrcInd kwMiLkwTrcIndMt[MAXKWMI] =
{
#ifdef LCKWMILKW
   cmPkLkwTrcInd,            /* 0 - loosely coupled  */
#endif /* LCKWMILKW */
#ifdef SM
   SmMiLkwTrcInd,            /* 1 - tightly coupled, layer management */
#endif /* SM */
};

/* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
static const LkwL2MeasCfm rlcMiLkwL2MeasCfmMt[] =
{
#ifdef LCKWMILKW
   cmPkLkwL2MeasCfm,
#endif
#ifdef SM
   SmMiLkwL2MeasCfm,
#endif
};
static const LkwL2MeasStopCfm RlcMiLkwL2MeasStopCfmMt[] =
{
#ifdef LCKWMILKW
   cmPkLkwL2MeasStopCfm,
#endif
#ifdef SM
   SmMiLkwL2MeasStopCfm,
#endif
};
#endif /*  LTE_L2_MEAS */

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
 *                         LKW Interface Mt functions
 ***************************************************************************/
/**
   @brief
   This function is called by the RlcMiRlcConfigReq function for responding
   to configuration requests.The cfm field in the RlcMngmt  structure contains
 the response value.

   - This function calls the mapping matrix for sending the configuration
     confirmation.
   - The actual function called depends on the coupling at the LKW interface.
   - For a loosely coupled interface, a common packing function is called.
   - The packing function packs the parameter in a message buffer and posts
     the message to the target task.
   - For a tightly coupled interface, the actual function called depends on
     the layer manager API provided.

*/
S16 RlcMiRlcConfigCfm
(
Pst        *pst,                /* post structure */
RlcMngmt    *cfm                 /* Layer Management structure */
)
{

   /* jump to specific primitive depending on configured selector */
   (*rlcMiRlcConfigCfmMt[pst->selector])(pst, cfm);
   
   return ROK;
}


/**
   @brief
   This function is called by the RlcMiLkwCntrlReq function to send a control confirm to the layer management module.

   - This function calls the mapping matrix for sending the control confirmation.
   - Actual function called depends on the coupling of the LKW interface.
   - For a loosely coupled interface, a common packing function is called.
   - The packing function packs the parameter in a message buffer and posts the
     message to the target task.
   - For a tightly coupled interface, the actual function called depends on the
     layer manager API provided.

*/
S16 RlcMiLkwCntrlCfm
(
Pst *pst,                    /* post structure */
RlcMngmt *cfm                 /* configure */
)
{

   /* jump to specific primitive depending on configured selector */
   (*kwMiLkwCntrlCfmMt[pst->selector])(pst, cfm);

   return ROK;

} /* end of RlcMiLkwCntrlCfm */

/**
   @brief
   Description:
   - This function can be used by RLC to send unsolicited status information
     to the layer manager, when the unsolicited status flag is enabled by the
     layer manager through a previous control request.

   - This function calls the mapping matrix for sending the unsolicited status
     indication.The actual function called depends on the coupling of the
     LKW interface.

   - For a loosely coupled interface, a common packing function is called. The
     packing function packs the parameter in a message buffer and posts the
     message to the target task.

   - For a tightly coupled interface, the actual function called depends on
     the layer manager API provided.
*/
S16 RlcMiLkwStaInd
(
Pst     *pst,                /* post structure */
RlcMngmt *usta                /* unsolicited status */
)
{
   /* jump to specific primitive depending on configured selector */
   (*kwMiLkwStaIndMt[pst->selector])(pst, usta);

   return (ROK);
} /* end of RlcMiLkwStaInd */


/**
   @brief
   - This function is called by the RlcMiLkwStaReq function to send
      the requested status information to the layer manager.

   - This function calls the mapping matrix for sending the status
      confirmation. The actual function called depends on the coupling
      of the LKW interface.

   - For a loosely coupled interface, a common packing function is called.
      The packing function packs the parameter in a message buffer and
      posts the message to the target task.

   - For a tightly coupled interface, the actual function called depends
      on the layer manager API provided.

*/
S16 RlcMiLkwStaCfm
(
Pst *pst,                    /* post structure */
RlcMngmt *cfm                 /* solicited status confirmation */
)
{

   /* jump to specific primitive depending on configured selector */
   (*kwMiLkwStaCfmMt[pst->selector])(pst, cfm);

   return ROK;

} /* end of RlcMiLkwStaCfm */


/**
   @brief
   - This function is called by the RlcMiLkwStsReq function for responding
      to statistics requests.

   - This function calls the mapping matrix for sending the statistics
      confirmation. The actual function called depends on the coupling
      of the LKW interface.

   - For a loosely coupled interface, a common packing function is called.
      The packing function packs the parameter in a message buffer and
      posts the message to the target task.

   - For a tightly coupled interface, the actual function called depends
      on the layer manager API provided.

*/
S16 RlcMiLkwStsCfm
(
Pst *pst,                    /* post structure */
Action action,               /* action */
RlcMngmt *cfm                 /* statistics confirmation */
)
{
   /* jump to specific primitive depending on configured selector */
   (*kwMiLkwStsCfmMt[pst->selector])(pst, action, cfm);

   return ROK;

} /* end of RlcMiLkwStsCfm */

/**
   @brief
   - This function can be used by RLC module to send unsolicited trace
      indications to the layer manager, when tracing is enabled by the
      layer manager through a previous control request.

   - This function calls the mapping matrix for sending the trace indication.
      The actual function called depends on the coupling of the LKW interface.

   - For a loosely coupled interface, a common packing function is called.
      The packing function packs the parameter in a message buffer and posts
      the message to the target task.

   - For a tightly coupled interface, the actual function called depends on
      the layer manager API provided.

*/
S16 RlcMiLkwTrcInd
(
Pst *pst,                    /* post structure */
RlcMngmt *trc,                /* trace indication */
Buffer *mBuf                 /* message buffer */
)
{
   /* jump to specific primitive depending on configured selector */
   (*kwMiLkwTrcIndMt[pst->selector])(pst, trc, mBuf);

   return ROK;

} /* end of RlcMiLkwTrcInd */


/* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
S16 RlcMiLkwL2MeasCfm(Pst * pst,RlcL2MeasCfmEvt *measEvt)
{

   (*rlcMiLkwL2MeasCfmMt[pst->selector])(pst, measEvt);

   return ROK;

}
S16 RlcMiLkwL2MeasStopCfm(Pst *pst,uint8_t measType,uint8_t status)
{

   (*RlcMiLkwL2MeasStopCfmMt[pst->selector])(pst, measType,status);

   return ROK;

}
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

S16 RlcLiRguBndReq
(
Pst  *post,                       /* post structure */
SuId suId,                      /* Service User Id */
SpId spId                       /* Service Provider Id */
)
{

   /* jump to specific primitive depending on configured selector */
   (*kwLiRguBndReqMt[post->selector])(post, suId, spId);

   return ROK;

} /* end of RlcLiRguBndReq */


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

S16 RlcLiRguUbndReq(Pst *post,SpId spId,Reason reason)
{

   /* jump to specific primitive depending on configured selector */
   (*kwLiRguUbndReqMt[post->selector])(post, spId, reason);

   return ROK;

} /* end of RlcLiRguUbndReq */

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
S16 RlcLiRguL2MUlThrpMeasReq(Pst *post,SpId spId,RguL2MUlThrpMeasReqInfo *l2mUlThrpMeasReq)
{

   /* jump to specific primitive depending on configured selector */
   (*kwLiRguL2MUlThrpMeasReqMt[post->selector])(post, spId, l2mUlThrpMeasReq);

   return ROK;

} /* end of RlcLiRguL2MUlThrpMeasReq */

#endif /* LTE_RLC_R9 */
#endif /* LTE_L2_MEAS */


#ifdef MAC_RLC_UL_RBUF
S16 rlcUlBatchProc ARGS ((Void));
Void rlcUtlFreeUlRBuf ARGS((void));

S16 rlcUlBatchProc(Void)
{
/* Read from Ring Buffer and process PDCP packets */
   RguDDatIndInfo   *datInd;
   Void *elmIndx = NULLP;
   static Pst rlcUlRbfuPst={1,1,ENTRLC,0,ENTMAC,0,PRIOR0,RTESPEC,EVTRLCULDAT,0,0,0,0};
/* Read from Ring Buffer and process PDCP packets */

#ifndef SS_RBUF
   RguDedDatInd1 *rguDatInd = NULLP;
   uint8_t rngBufDeqIndx = 0;

   elmIndx = SRngGetRIndx(SS_RNG_BUF_ULMAC_TO_ULRLC);
   while(NULLP != elmIndx)
   {
      rguDatInd = (RguDedDatInd1 *)elmIndx;
      datInd = (RguDDatIndInfo*) rguDatInd->msg; 
      SsRngInfoTbl[SS_RNG_BUF_ULMAC_TO_ULRLC].nPktProc++;//Number of pkt processed in tti
      if(datInd != NULLP)
      {
         RlcProcDedLcUlData(&rlcUlRbfuPst, 0, datInd);
      }
      else
      {
         DU_LOG("\nERROR  -->  RLC UL : Received NULL buffer");
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
      RlcProcDedLcUlData(&rlcUlRbfuPst, 0, datInd);

      elmIndx = NULLP;
      datInd = NULLP;
      SRngIncrRIndx(SS_RNG_BUF_ULMAC_TO_ULRLC);

      if((elmIndx = SRngGetRIndx(SS_RNG_BUF_ULMAC_TO_ULRLC)) == NULLP)
      	break;
   }
#endif
   return ROK;

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
Void rlcUtlFreeUlRBuf(void)
{
   RguDDatIndInfo *datInd;
   PTR            elem;
   uint8_t        numLch;
   uint8_t        numPdu;

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
               ODU_PUT_MSG_BUF(datInd->lchData[numLch].pdu.mBuf[numPdu]);
            }
         }
      }
      RLC_PST_FREE(0, 0, datInd, sizeof(RguDDatIndInfo)); 
   }
}
#endif
#ifdef RLC_MAC_STA_RSP_RBUF
S16 rlcLiRguStaRspRbuf(Pst *post,SpId spId,Void *staRsp)
{
   S16 ret1 = ROK;
   
   Void *elem = NULLP;

   RguDStaRspInfo  *staRspInfo = NULL;
   elem = SRngGetWIndx(SS_RNG_BUF_DLRLC_TO_DLMAC);
   if (NULLP != elem)
   {
      staRspInfo = (RguDStaRspInfo *)elem;
      memcpy(staRspInfo, staRsp, sizeof(RguDStaRspInfo)); 
      staRspInfo->post = *post;
      SRngIncrWIndx(SS_RNG_BUF_DLRLC_TO_DLMAC);
      SsRngInfoTbl[SS_RNG_BUF_DLRLC_TO_DLMAC].pktRate++;
   }   
   else
   {
      DU_LOG("\nERROR  -->  RLC_DL : RLC DL STA RSP RBUF is FULL!!! ");
      SsRngInfoTbl[SS_RNG_BUF_DLRLC_TO_DLMAC].pktDrop++;
      ret1 = RFAILED;
   }
   return (ret1);
} /* cmPkRlcDatReq */

#endif
#ifdef RLC_MAC_DAT_REQ_RBUF
S16 rlcLiRguDatReqRbuf(Pst *post,SpId spId,Void *datReq)
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
      DU_LOG("\nERROR  -->  RLC_DL : RLC DL DAT REQ RBUF is FULL!!! \n");
      SsRngInfoTbl[SS_RNG_BUF_DLRLC_TO_DLMAC_DAT_REQ].pktDrop++;
      ret1 = RFAILED;
   }
   return (ret1);
} /* cmPkRlcDatReq */

#endif

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
   DU_LOG("\nINFO   --> RLC : In RlcUiKwuReEstCmpInd");

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
         DU_LOG("\nERROR  --> RLC : mBuf is NULL");
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
   RlcDatReqInfo datReq;

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
      ODU_PUT_MSG_BUF(kwuDatReqDetl->mBuf);
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

/************************************************************************
         End of file
**********************************************************************/
