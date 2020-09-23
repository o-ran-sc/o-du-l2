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
 
     Name:     LTE-RLC ULDL Interface
  
     Type:     C source file
  
     Desc:     C source code for INF Interface Module 
  
     File:     kw_udx_ptul.c
  
**********************************************************************/

/** @file kw_udx_ptul.c
*/

/* header include files (.h) */
#include "common_def.h"
#include "lkw.h"           /* LKW defines */
#include "ckw.h"           /* CKW defines */
#include "kwu.h"           /* KWU defines */
#include "rgu.h"           /* RGU defines */
#include "kw_err.h"        /* Err defines */
#include "kw_env.h"        /* RLC environment options */

#include "kw.h"            /* RLC defines */
#include "kw_udx.h"
#include "kw_dl.h"

/* extern (.x) include files */
#include "lkw.x"           /* LKW */
#include "ckw.x"           /* CKW */
#include "kwu.x"           /* KWU */
#include "rgu.x"           /* RGU */

#include "kw.x"
#include "kw_udx.x"
#include "kw_dl.x"


#ifndef LCKWULUDX
PRIVATE S16 PtUlUdxBndReq ARGS((Pst* pst, SuId suId,SpId  spId ));
PRIVATE S16 PtUlUdxUbndReq ARGS((Pst* pst, SuId suId,Reason reason));
PRIVATE S16 PtUlUdxCfgReq ARGS((Pst *pst, SpId spId, RlcCfgInfo *cfgInfo));
PRIVATE S16 PtUlUdxStaUpdReq ARGS((Pst* pst,SpId spId,CmLteRlcId *rlcId,
                                RlcUdxStaPdu         *pStaPdu ));
PRIVATE S16 PtUlUdxUeIdChgReq ARGS((Pst *pst, SpId spId, U32 transId, 
                                   CkwUeInfo *ueInfo, CkwUeInfo *newUeInfo));
PRIVATE S16 PtUlUdxStaPduReq ARGS(( Pst* pst,SpId  spId, CmLteRlcId *rlcId,
                             RlcUdxDlStaPdu  *pStaPdu ));
#ifdef ANSI
PRIVATE S16 PtUlUdxBndReq
(
Pst *pst,                       /* post structure */
SuId suId,                      /* Service User Id */
SpId spId                    /* Status */
)
#else
PRIVATE S16 PtUlUdxBndReq(pst, suId,spId)
Pst *pst;                       /* post structure */
SuId suId;                      /* Service User Id */
SpId spId;                    /* Status */
#endif
{
   TRC3(PtUlUdxBndReq)

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
#ifdef ANSI
PRIVATE S16 PtUlUdxUbndReq
(
Pst *pst,                       /* post structure */
SpId spId,                      /* Service User Id */
Reason   reason                     /* Status */
)
#else
PRIVATE S16 PtUlUdxUbndReq(pst, suId, status)
Pst *pst;                       /* post structure */
SpId spId;                      /* Service User Id */
Reason Reason;                    /* Status */
#endif
{
   TRC3(PtUlUdxUbndReq)

   return ROK;

} /* end of PtDlUdxBndReq */

#ifdef ANSI
PRIVATE S16 PtUlUdxCfgReq
(
Pst *pst,                       /* post structure */
SpId spId,                      /* Service User Id */
RlcCfgInfo *cfmInfo             /* Config Info */
)
#else
PRIVATE S16 PtUlUdxCfgReq(pst, spId, status)
Pst *pst;                       /* post structure */
SpId spId;                      /* Service User Id */
RlcCfgInfo *cfmInfo;              /* Config Info */
#endif
{
   TRC3(PtUlUdxCfgReq)

   return ROK;

} /* end of rlcDlUdxCfgReq */

#ifdef ANSI
PRIVATE S16 PtUlUdxUeIdChgReq
(
Pst *pst,                      /* post structure */
SpId spId,                     /* Service User Id */
U32      transId,              /* transaction Id */
CkwUeInfo *ueInfo,             /* Config Info */
CkwUeInfo *newUeInfo           /* Config Info */
)
#else
PRIVATE S16 PtUlUdxUeIdChgReq(pst, spId,transId, ueInfo, newUeInfo)
Pst *pst;                      /* post structure */
SpId spId;                     /* Service User Id */
U32      transId;              /* transaction Id */
CkwUeInfo *ueInfo;             /* Config Info */
CkwUeInfo *newUeInfo;          /* Config Info */
#endif
{
   TRC3(PtUlUdxUeIdChgReq)


   return ROK;

} /* end of rlcDlUdxCfgReq */


PRIVATE S16  PtUlUdxStaPduReq
(
Pst*                pst,
SpId                spId,
CmLteRlcId          *rlcId,
RlcUdxDlStaPdu       *pStaPdu
)
{
   TRC3(PtUlUdxStaPduReq);

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
#ifdef ANSI
PRIVATE S16  PtUlUdxStaUpdReq
(
Pst*                pst,
SpId                spId,
CmLteRlcId          *rlcId,
RlcUdxStaPdu       *pStaPdu
)
#else
PRIVATE S16 PtUlUdxStaUpdReq(pst, rlcId, pStaPdu)
Pst*                pst;
SpId                spId;
CmLteRlcId          *rlcId;
RlcUdxDlStaPdu       *pStaPdu;
#endif
{

   TRC3(PtUlUdxStaUpdReq);

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
#ifdef ANSI
PRIVATE S16  PtUlUdxL2MeasReq
(
Pst*                pst,
RlcL2MeasReqEvt      *measReqEvt
)
#else
PRIVATE S16 PtUlUdxL2MeasReq(pst, measReqEvt )
Pst*                pst;
RlcL2MeasReqEvt      *measReqEvt;
#endif
{

   TRC3(PtUlUdxL2MeasReq);

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
#ifdef ANSI
PRIVATE S16  PtUlUdxL2MeasSendReq
(
Pst*                pst,
U8                  status
)
#else
PRIVATE S16 PtUlUdxL2MeasSendReq(pst, status )
Pst*                pst;
U8                  status
#endif
{

   TRC3(PtUlUdxL2MeasSendReq);

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
#ifdef ANSI
PRIVATE S16  PtUlUdxL2MeasStopReq
(
Pst*                pst,
U8                  status
)
#else
PRIVATE S16 PtUlUdxL2MeasStopReq(pst, status )
Pst*                pst;
U8                  status
#endif
{

   TRC3(PtUlUdxL2MeasStopReq);

   return ROK;
}
#endif
#endif

PRIVATE UdxBndReq rlcUlUdxBndReqMt[] =
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

PRIVATE UdxUbndReq rlcUlUdxUbndReqMt[] =
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


PRIVATE UdxCfgReq rlcUlUdxCfgReqMt[] =
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

PRIVATE UdxUeIdChgReq rlcUlUdxUeIdChgReqMt[] =
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


PRIVATE CONSTANT UdxStaUpdReq rlcUlUdxStaUpdReqMt[RLC_MAX_UDX] =
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

PRIVATE CONSTANT UdxStaPduReq rlcUlUdxStaPduReqMt[RLC_MAX_UDX] =
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
PRIVATE CONSTANT UdxL2MeasReq rlcUlUdxL2MeasReqMt[RLC_MAX_UDX] =
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

PRIVATE CONSTANT UdxL2MeasSendReq rlcUlUdxL2MeasSendReqMt[RLC_MAX_UDX] =
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

PRIVATE CONSTANT UdxL2MeasStopReq rlcUlUdxL2MeasStopReqMt[RLC_MAX_UDX] =
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
#ifdef ANSI
S16 rlcUlUdxBndReq
(
Pst *pst,                       /* post structure */
SuId suId,                      /* Service User Id */
SpId spId                     /* Status */
)
#else
S16 rlcUlUdxBndReq(pst, suId, status)
Pst *pst;                       /* post structure */
SuId suId;                      /* Service User Id */
SpId spId;                    /* Status */
#endif
{
   TRC3(rlcUlUdxBndReq)

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
#ifdef ANSI
S16 rlcUlUdxUbndReq
(
Pst *pst,                       /* post structure */
SpId spId,                      /* Service User Id */
Reason   reason                     /* Status */
)
#else
S16 rlcUlUdxUbndReq(pst, suId, status)
Pst *pst;                       /* post structure */
SpId spId;                      /* Service User Id */
Reason Reason;                    /* Status */
#endif
{
   TRC3(rlcUlUdxUbndReq)

   /* jump to specific primitive depending on configured selector */
   (*rlcUlUdxUbndReqMt[pst->selector])(pst, spId,reason );

   return ROK;

} /* end of rlcDlUdxBndReq */

#ifdef ANSI
S16 rlcUlUdxCfgReq
(
Pst *pst,                       /* post structure */
SpId spId,                      /* Service User Id */
RlcCfgInfo *cfmInfo             /* Config Info */
)
#else
S16 rlcUlUdxCfgReq(pst, suId, status)
Pst *pst;                       /* post structure */
SuId spId;                      /* Service User Id */
RlcCfgInfo *cfmInfo;              /* Config Info */
#endif
{
   TRC3(rlcUlUdxCfgReq)

   /* jump to specific primitive depending on configured selector */
   (*rlcUlUdxCfgReqMt[pst->selector])(pst, spId, cfmInfo);

   return ROK;

} /* end of rlcDlUdxCfgReq */

#ifdef ANSI
S16 rlcUlUdxUeIdChgReq
(
Pst *pst,                      /* post structure */
SpId spId,                     /* Service User Id */
U32      transId,              /* transaction Id */
CkwUeInfo *ueInfo,             /* Config Info */
CkwUeInfo *newUeInfo           /* Config Info */
)
#else
S16 rlcUlUdxUeIdChgReq(pst, spId,transId, ueInfo, newUeInfo)
Pst *pst;                      /* post structure */
SpId spId;                     /* Service User Id */
U32      transId;              /* transaction Id */
CkwUeInfo *ueInfo;             /* Config Info */
CkwUeInfo *newUeInfo;          /* Config Info */
#endif
{
   TRC3(rlcUlUdxUeIdChgReq)

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
#ifdef ANSI
S16  rlcUlUdxL2MeasReq
(
Pst*                pst,
RlcL2MeasReqEvt      *measReqEvt 
)
#else
S16 rlcUlUdxL2MeasReq(pst,measReqEvt)
Pst*                pst;
RlcL2MeasReqEvt      *measReqEvt; 
#endif
{

   TRC3(rlcUlUdxStaUpdReq);

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
#ifdef ANSI
S16  rlcUlUdxL2MeasSendReq
(
Pst*                pst,
U8                  measType
)
#else
S16 rlcUlUdxL2MeasSendReq(pst,measReqEvt)
Pst*                pst;
U8                  measType;
#endif
{

   TRC3(rlcUlUdxStaUpdReq);

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
#ifdef ANSI
S16  rlcUlUdxL2MeasStopReq
(
Pst*                pst,
U8                  measType
)
#else
S16 rlcUlUdxL2MeasStopReq(pst,measReqEvt)
Pst*                pst;
U8                  measType;
#endif
{

   TRC3(rlcUlUdxStaUpdReq);

   return ((*rlcUlUdxL2MeasStopReqMt[pst->selector])(pst,measType));
} /* end of rlcUlUdxMesReq*/
#endif
/**********************************************************************
 
         End of file
**********************************************************************/
