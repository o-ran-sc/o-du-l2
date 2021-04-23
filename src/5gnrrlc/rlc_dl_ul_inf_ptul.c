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

/************************************************************************
 
     Name:     NR RLC ULDL Interface
  
     Type:     C source file
  
     Desc:     C source code for INF Interface Module 
  
     File:     rlc_dl_ul_inf_ptul.c
  
**********************************************************************/

/** @file rlc_dl_ul_inf_ptul.c
*/

/* header include files (.h) */
#include "common_def.h"
#include "lkw.h"           /* LKW defines */
#include "ckw.h"           /* CKW defines */
#include "kwu.h"           /* KWU defines */
#include "rgu.h"           /* RGU defines */
#include "rlc_err.h"        /* Err defines */
#include "rlc_env.h"        /* RLC environment options */

/* extern (.x) include files */
#include "lkw.x"           /* LKW */
#include "ckw.x"           /* CKW */
#include "kwu.x"           /* KWU */
#include "rgu.x"           /* RGU */

#include "rlc_utils.h"            /* RLC defines */
#include "rlc_dl_ul_inf.h"
#include "rlc_dl.h"

#ifndef LCKWULUDX
static S16 PtUlUdxBndReq ARGS((Pst* pst, SuId suId,SpId  spId ));
static S16 PtUlUdxUbndReq ARGS((Pst* pst, SuId suId,Reason reason));
static S16 PtUlUdxCfgReq ARGS((Pst *pst, SpId spId, RlcCfgInfo *cfgInfo));
static S16 PtUlUdxStaUpdReq ARGS((Pst* pst,SpId spId,CmLteRlcId *rlcId,
                                RlcUdxStaPdu         *pStaPdu ));
static S16 PtUlUdxUeIdChgReq ARGS((Pst *pst, SpId spId, uint32_t transId, 
                                   CkwUeInfo *ueInfo, CkwUeInfo *newUeInfo));
static S16 PtUlUdxStaPduReq ARGS(( Pst* pst,SpId  spId, CmLteRlcId *rlcId,
                             RlcUdxDlStaPdu  *pStaPdu ));
static S16 PtUlUdxBndReq
(
Pst *pst,                       /* post structure */
SuId suId,                      /* Service User Id */
SpId spId                    /* Status */
)
{

   return ROK;

} /* end of rlcDlUdxBndReq */

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
static S16 PtUlUdxUbndReq
(
Pst *pst,                       /* post structure */
SpId spId,                      /* Service User Id */
Reason   reason                     /* Status */
)
{

   return ROK;

} /* end of PtDlUdxBndReq */

static S16 PtUlUdxCfgReq
(
Pst *pst,                       /* post structure */
SpId spId,                      /* Service User Id */
RlcCfgInfo *cfmInfo             /* Config Info */
)
{

   return ROK;

} /* end of rlcDlUdxCfgReq */

static S16 PtUlUdxUeIdChgReq
(
Pst *pst,                      /* post structure */
SpId spId,                     /* Service User Id */
uint32_t  transId,              /* transaction Id */
CkwUeInfo *ueInfo,             /* Config Info */
CkwUeInfo *newUeInfo           /* Config Info */
)
{

   return ROK;

} /* end of rlcDlUdxCfgReq */


static S16  PtUlUdxStaPduReq
(
Pst*                pst,
SpId                spId,
CmLteRlcId          *rlcId,
RlcUdxDlStaPdu       *pStaPdu
)
{

   return ROK;
}

/**
* @brief Request from ULM to DLM for UE Status
*
* @details
*
*     Function : KwUdxUdxStatUpd
*
*  @param[in]   Pst*           pst

*  @return   S16
*      -# ROK
**/
static S16  PtUlUdxStaUpdReq
(
Pst*                pst,
SpId                spId,
CmLteRlcId          *rlcId,
RlcUdxStaPdu       *pStaPdu
)
{

   return ROK;
} /* end of RlcUlmDlmStaUpd*/

#ifdef LTE_L2_MEAS
/**
* @brief Request from ULM to DLM for Measurement
*
* @details
*
*     Function : KwUdxUdxL2MeasReq
*
*  @param[in]   Pst*           pst

*  @return   S16
*      -# ROK
**/
static S16  PtUlUdxL2MeasReq(Pst* pst,RlcL2MeasReqEvt  *measReqEvt)
{

   return ROK;
} 

/**
* @brief Request from ULM to DLM for Measurement
*
* @details
*
*     Function : KwUdxUdxL2MeasSendReq
*
*  @param[in]   Pst*           pst

*  @return   S16
*      -# ROK
**/
static S16  PtUlUdxL2MeasSendReq(Pst*  pst,uint8_t status)
{

   return ROK;
}

/**
* @brief Request from ULM to DLM for Measurement
*
* @details
*
*     Function : KwUdxUdxL2MeasStopReq
*
*  @param[in]   Pst*           pst

*  @return   S16
*      -# ROK
**/
static S16  PtUlUdxL2MeasStopReq(Pst*      pst,uint8_t   status)
{

   return ROK;
}
#endif
#endif

static UdxBndReq rlcUlUdxBndReqMt[] =
{
#ifdef LCKWULUDX
   cmPkUdxBndReq,            /* 0 - loosely coupled */
#else
   PtUlUdxBndReq,            /* 0 - loosely coupled, portable  */
#endif /* LCKWUIKWU */
#ifdef LWLCKWULUDX
   cmPkUdxBndReq,            /* 1 - loosely coupled */
#else
   PtUlUdxBndReq,            /* 1 - loosely coupled, portable  */
#endif /* LCKWUIKWU */
#ifdef KW
   rlcDlUdxBndReq,            /* 2 - tightly coupled, RRC  */
#else
   PtUlUdxBndReq,            /* 2 - tightly coupled, portable */
#endif /* KW */
};

static UdxUbndReq rlcUlUdxUbndReqMt[] =
{
#ifdef LCKWULUDX
   cmPkUdxUbndReq,            /* 0 - loosely coupled */
#else
   PtUlUdxUbndReq,            /* 0 - loosely coupled, portable  */
#endif /* LCKWUIKWU */
#ifdef LWLCKWULUDX
   cmPkUdxUbndReq,            /* 1 - loosely coupled */
#else
   PtUlUdxUbndReq,            /* 1 - loosely coupled, portable  */
#endif /* LCKWUIKWU */
#ifdef KW
   rlcDlUdxUbndReq,            /* 2 - tightly coupled, RRC  */
#else
   PtUlUdxUbndReq,            /* 2 - tightly coupled, portable */
#endif /* KW */
};


static UdxCfgReq rlcUlUdxCfgReqMt[] =
{
#ifdef LCKWULUDX
   cmPkUdxCfgReq,            /* 0 - loosely coupled */
#else
   PtUlUdxCfgReq,            /* 0 - loosely coupled, portable  */
#endif /* LCKWUIKWU */
#ifdef LWLCKWULUDX
   cmPkUdxCfgReq,            /* 1 - loosely coupled */
#else
   PtUlUdxCfgReq,            /* 1 - loosely coupled, portable  */
#endif /* LCKWUIKWU */
#ifdef KW
   rlcDlUdxCfgReq,            /* 2 - tightly coupled, RRC  */
#else
   PtUlUdxCfgReq,            /* 2 - tightly coupled, portable */
#endif /* KW */
};

static UdxUeIdChgReq rlcUlUdxUeIdChgReqMt[] =
{
#ifdef LCKWULUDX
   cmPkUdxUeIdChgReq,            /* 0 - loosely coupled */
#else
   PtUlUdxUeIdChgReq,            /* 0 - loosely coupled, portable  */
#endif /* LCKWUIKWU */
#ifdef LWLCKWULUDX
   cmPkUdxUeIdChgReq,            /* 1 - loosely coupled */
#else
   PtUlUdxUeIdChgReq,            /* 1 - loosely coupled, portable  */
#endif /* LCKWUIKWU */
#ifdef KW
   rlcDlUdxUeIdChgReq,            /* 2 - tightly coupled, RRC  */
#else
   PtUlUdxUeIdChgReq,            /* 2 - tightly coupled, portable */
#endif /* KW */
};


static const UdxStaUpdReq rlcUlUdxStaUpdReqMt[RLC_MAX_UDX] =
{
#ifdef LCKWULUDX
   cmPkUdxStaUpdReq,            /* 0 - loosely coupled */
#else
   PtUlUdxStaUpdReq,            /* 0 - loosely coupled, portable  */
#endif /* LCKWUIKWU */
#ifdef LWLCKWULUDX
   cmPkUdxStaUpdReq,            /* 1 - loosely coupled */
#else
   PtUlUdxStaUpdReq,            /* 1 - loosely coupled, portable  */
#endif /* LCKWUIKWU */
#ifdef KW
   rlcDlUdxStaUpdReq,            /* 2 - tightly coupled, RRC  */
#else
   PtUlUdxStaUpdReq,            /* 2 - tightly coupled, portable */
#endif /* KW */
};

static const UdxStaPduReq rlcUlUdxStaPduReqMt[RLC_MAX_UDX] =
{
#ifdef LCKWULUDX
   cmPkUdxStaPduReq,            /* 0 - loosely coupled */
#else
   PtUlUdxStaPduReq,            /* 0 - loosely coupled, portable  */
#endif /* LCKWUIKWU */
#ifdef LWLCKWULUDX
   cmPkUdxStaPduReq,            /* 1 - loosely coupled */
#else
   PtUlUdxStaPduReq,            /* 1 - loosely coupled, portable  */
#endif /* LCKWUIKWU */
#ifdef KW
   rlcDlUdxStaPduReq,            /* 2 - tightly coupled, RRC  */
#else
   PtUlUdxStaPduReq,            /* 2 - tightly coupled, portable */
#endif /* KW */
};
#ifdef LTE_L2_MEAS
static const UdxL2MeasReq rlcUlUdxL2MeasReqMt[RLC_MAX_UDX] =
{
#ifdef LCKWULUDX
   cmPkUdxL2MeasReq,            /* 0 - loosely coupled */
#else
   PtUlUdxL2MeasReq,            /* 0 - loosely coupled, portable  */
#endif /* LCKWUIKWU */
#ifdef LWLCKWULUDX
   cmPkUdxL2MeasReq,            /* 1 - loosely coupled */
#else
   PtUlUdxL2MeasReq,            /* 1 - loosely coupled, portable  */
#endif /* LCKWUIKWU */
#ifdef KW
   rlcDlUdxL2MeasReq,            /* 2 - tightly coupled, RRC  */
#else
   PtUlUdxL2MeasReq,            /* 2 - tightly coupled, portable */
#endif /* KW */
};

static const UdxL2MeasSendReq rlcUlUdxL2MeasSendReqMt[RLC_MAX_UDX] =
{
#ifdef LCKWULUDX
   cmPkUdxL2MeasSendReq,            /* 0 - loosely coupled */
#else
   PtUlUdxL2MeasSendReq,            /* 0 - loosely coupled, portable  */
#endif /* LCKWUIKWU */
#ifdef LWLCKWULUDX
   cmPkUdxL2MeasSendReq,            /* 1 - loosely coupled */
#else
   PtUlUdxL2MeasSendReq,            /* 1 - loosely coupled, portable  */
#endif /* LCKWUIKWU */
#ifdef KW
   rlcDlUdxL2MeasSendReq,            /* 2 - tightly coupled, RRC  */
#else
   PtUlUdxL2MeasSendReq,            /* 2 - tightly coupled, portable */
#endif /* KW */
};

static const UdxL2MeasStopReq rlcUlUdxL2MeasStopReqMt[RLC_MAX_UDX] =
{
#ifdef LCKWULUDX
   cmPkUdxL2MeasStopReq,            /* 0 - loosely coupled */
#else
   PtUlUdxL2MeasStopReq,            /* 0 - loosely coupled, portable  */
#endif /* LCKWUIKWU */
#ifdef LWLCKWULUDX
   cmPkUdxL2MeasStopReq,            /* 1 - loosely coupled */
#else
   PtUlUdxL2MeasStopReq,            /* 1 - loosely coupled, portable  */
#endif /* LCKWUIKWU */
#ifdef KW
   rlcDlUdxL2MeasStopReq,            /* 2 - tightly coupled, RRC  */
#else
   PtUlUdxL2MeasStopReq,            /* 2 - tightly coupled, portable */
#endif /* KW */
};
#endif
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
S16 rlcUlUdxBndReq
(
Pst *pst,                       /* post structure */
SuId suId,                      /* Service User Id */
SpId spId                     /* Status */
)
{
   /* jump to specific primitive depending on configured selector */
   (*rlcUlUdxBndReqMt[pst->selector])(pst, suId, spId);

   return ROK;

} /* end of rlcDlUdxBndReq */

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
S16 rlcUlUdxUbndReq
(
Pst *pst,                       /* post structure */
SpId spId,                      /* Service User Id */
Reason   reason                     /* Status */
)
{
   /* jump to specific primitive depending on configured selector */
   (*rlcUlUdxUbndReqMt[pst->selector])(pst, spId,reason );

   return ROK;

} /* end of rlcDlUdxBndReq */

S16 rlcUlUdxCfgReq
(
Pst *pst,                       /* post structure */
SpId spId,                      /* Service User Id */
RlcCfgInfo *cfmInfo             /* Config Info */
)
{

   /* jump to specific primitive depending on configured selector */
   (*rlcUlUdxCfgReqMt[pst->selector])(pst, spId, cfmInfo);

   return ROK;

} /* end of rlcDlUdxCfgReq */

S16 rlcUlUdxUeIdChgReq
(
Pst *pst,                      /* post structure */
SpId spId,                     /* Service User Id */
uint32_t  transId,              /* transaction Id */
CkwUeInfo *ueInfo,             /* Config Info */
CkwUeInfo *newUeInfo           /* Config Info */
)
{

   /* jump to specific primitive depending on configured selector */
   (*rlcUlUdxUeIdChgReqMt[pst->selector])(pst, spId,transId,ueInfo,newUeInfo);

   return ROK;

} /* end of rlcDlUdxCfgReq */



uint8_t rlcUlUdxStaPduReq(Pst* pst, SpId spId, CmLteRlcId *rlcId, RlcUdxDlStaPdu *pStaPdu)
{
   return ((*rlcUlUdxStaPduReqMt[pst->selector])(pst,spId, rlcId, pStaPdu));
}

/**
* @brief Request from ULM to DLM for UE Status
*
* @details
*
*     Function : KwUdxUdxStatUpd
*
*  @param[in]   Pst*           pst

*  @return   S16
*      -# ROK
**/
uint8_t rlcUlUdxStaUpdReq(Pst* pst, SpId spId, CmLteRlcId *rlcId, RlcUdxStaPdu *pStaPdu)
{
   return ((*rlcUlUdxStaUpdReqMt[pst->selector])(pst,spId, rlcId, pStaPdu));
} /* end of RlcUlmDlmStaUpd*/

#ifdef LTE_L2_MEAS
/**
* @brief Request from ULM to DLM for MeasReq 
*
* @details
*
*     Function : rlcUlUdxL2MeasReq
*
*  @param[in]   Pst*           pst

*  @return   S16
*      -# ROK
**/
S16  rlcUlUdxL2MeasReq(Pst* pst,RlcL2MeasReqEvt *measReqEvt )
{

   return ((*rlcUlUdxL2MeasReqMt[pst->selector])(pst,measReqEvt));
} /* end of rlcUlUdxL2MeasReq*/

/**
* @brief Request from ULM to DLM for MeasSendReq 
*
* @details
*
*     Function : rlcUlUdxMeasSendReq
*
*  @param[in]   Pst*           pst

*  @return   S16
*      -# ROK
**/
S16  rlcUlUdxL2MeasSendReq(Pst* pst,uint8_t measType)
{

   return ((*rlcUlUdxL2MeasSendReqMt[pst->selector])(pst,measType));
} /* end of rlcUlUdxMesReq*/

/**
* @brief Request from ULM to DLM for MeasStopReq 
*
* @details
*
*     Function : rlcUlUdxL2MeasStopReq
*
*  @param[in]   Pst*           pst

*  @return   S16
*      -# ROK
**/
S16  rlcUlUdxL2MeasStopReq(Pst*  pst,uint8_t  measType)
{

   return ((*rlcUlUdxL2MeasStopReqMt[pst->selector])(pst,measType));
} /* end of rlcUlUdxMesReq*/
#endif
/**********************************************************************
 
         End of file
**********************************************************************/
