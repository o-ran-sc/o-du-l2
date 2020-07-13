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
PRIVATE S16 PtUlUdxCfgReq ARGS((Pst *pst, SpId spId, CkwCfgInfo *cfgInfo));
PRIVATE S16 PtUlUdxStaUpdReq ARGS((Pst* pst,SpId spId,CmLteRlcId *rlcId,
                                KwUdxStaPdu         *pStaPdu ));
PRIVATE S16 PtUlUdxUeIdChgReq ARGS((Pst *pst, SpId spId, U32 transId, 
                                   CkwUeInfo *ueInfo, CkwUeInfo *newUeInfo));
PRIVATE S16 PtUlUdxStaPduReq ARGS(( Pst* pst,SpId  spId, CmLteRlcId *rlcId,
                             KwUdxDlStaPdu  *pStaPdu ));
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

   RETVALUE(ROK);

} /* end of KwDlUdxBndReq */

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

   RETVALUE(ROK);

} /* end of PtDlUdxBndReq */

#ifdef ANSI
PRIVATE S16 PtUlUdxCfgReq
(
Pst *pst,                       /* post structure */
SpId spId,                      /* Service User Id */
CkwCfgInfo *cfmInfo             /* Config Info */
)
#else
PRIVATE S16 PtUlUdxCfgReq(pst, spId, status)
Pst *pst;                       /* post structure */
SpId spId;                      /* Service User Id */
CkwCfgInfo *cfmInfo;              /* Config Info */
#endif
{
   TRC3(PtUlUdxCfgReq)

   RETVALUE(ROK);

} /* end of KwDlUdxCfgReq */

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


   RETVALUE(ROK);

} /* end of KwDlUdxCfgReq */


PRIVATE S16  PtUlUdxStaPduReq
(
Pst*                pst,
SpId                spId,
CmLteRlcId          *rlcId,
KwUdxDlStaPdu       *pStaPdu
)
{
   TRC3(PtUlUdxStaPduReq);

   RETVALUE(ROK);
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
KwUdxStaPdu       *pStaPdu
)
#else
PRIVATE S16 PtUlUdxStaUpdReq(pst, rlcId, pStaPdu)
Pst*                pst;
SpId                spId;
CmLteRlcId          *rlcId;
KwUdxDlStaPdu       *pStaPdu;
#endif
{

   TRC3(PtUlUdxStaUpdReq);

   RETVALUE(ROK);
} /* end of KwUlmDlmStaUpd*/

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
KwL2MeasReqEvt      *measReqEvt
)
#else
PRIVATE S16 PtUlUdxL2MeasReq(pst, measReqEvt )
Pst*                pst;
KwL2MeasReqEvt      *measReqEvt;
#endif
{

   TRC3(PtUlUdxL2MeasReq);

   RETVALUE(ROK);
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

   RETVALUE(ROK);
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

   RETVALUE(ROK);
}
#endif
#endif

PRIVATE UdxBndReq kwUlUdxBndReqMt[] =
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
   KwDlUdxBndReq,            /* 2 - tightly coupled, RRC  */
#else
   PtUlUdxBndReq,            /* 2 - tightly coupled, portable */
#endif /* KW */
};

PRIVATE UdxUbndReq kwUlUdxUbndReqMt[] =
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
   KwDlUdxUbndReq,            /* 2 - tightly coupled, RRC  */
#else
   PtUlUdxUbndReq,            /* 2 - tightly coupled, portable */
#endif /* KW */
};


PRIVATE UdxCfgReq kwUlUdxCfgReqMt[] =
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
   KwDlUdxCfgReq,            /* 2 - tightly coupled, RRC  */
#else
   PtUlUdxCfgReq,            /* 2 - tightly coupled, portable */
#endif /* KW */
};

PRIVATE UdxUeIdChgReq kwUlUdxUeIdChgReqMt[] =
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
   KwDlUdxUeIdChgReq,            /* 2 - tightly coupled, RRC  */
#else
   PtUlUdxUeIdChgReq,            /* 2 - tightly coupled, portable */
#endif /* KW */
};


PRIVATE CONSTANT UdxStaUpdReq kwUlUdxStaUpdReqMt[KW_MAX_UDX] =
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
   KwDlUdxStaUpdReq,            /* 2 - tightly coupled, RRC  */
#else
   PtUlUdxStaUpdReq,            /* 2 - tightly coupled, portable */
#endif /* KW */
};

PRIVATE CONSTANT UdxStaPduReq kwUlUdxStaPduReqMt[KW_MAX_UDX] =
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
   KwDlUdxStaPduReq,            /* 2 - tightly coupled, RRC  */
#else
   PtUlUdxStaPduReq,            /* 2 - tightly coupled, portable */
#endif /* KW */
};
#ifdef LTE_L2_MEAS
PRIVATE CONSTANT UdxL2MeasReq kwUlUdxL2MeasReqMt[KW_MAX_UDX] =
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
   KwDlUdxL2MeasReq,            /* 2 - tightly coupled, RRC  */
#else
   PtUlUdxL2MeasReq,            /* 2 - tightly coupled, portable */
#endif /* KW */
};

PRIVATE CONSTANT UdxL2MeasSendReq kwUlUdxL2MeasSendReqMt[KW_MAX_UDX] =
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
   KwDlUdxL2MeasSendReq,            /* 2 - tightly coupled, RRC  */
#else
   PtUlUdxL2MeasSendReq,            /* 2 - tightly coupled, portable */
#endif /* KW */
};

PRIVATE CONSTANT UdxL2MeasStopReq kwUlUdxL2MeasStopReqMt[KW_MAX_UDX] =
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
   KwDlUdxL2MeasStopReq,            /* 2 - tightly coupled, RRC  */
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
PUBLIC S16 KwUlUdxBndReq
(
Pst *pst,                       /* post structure */
SuId suId,                      /* Service User Id */
SpId spId                     /* Status */
)
#else
PUBLIC S16 KwUlUdxBndReq(pst, suId, status)
Pst *pst;                       /* post structure */
SuId suId;                      /* Service User Id */
SpId spId;                    /* Status */
#endif
{
   TRC3(KwUlUdxBndReq)

   /* jump to specific primitive depending on configured selector */
   (*kwUlUdxBndReqMt[pst->selector])(pst, suId, spId);

   RETVALUE(ROK);

} /* end of KwDlUdxBndReq */

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
PUBLIC S16 KwUlUdxUbndReq
(
Pst *pst,                       /* post structure */
SpId spId,                      /* Service User Id */
Reason   reason                     /* Status */
)
#else
PUBLIC S16 KwUlUdxUbndReq(pst, suId, status)
Pst *pst;                       /* post structure */
SpId spId;                      /* Service User Id */
Reason Reason;                    /* Status */
#endif
{
   TRC3(KwUlUdxUbndReq)

   /* jump to specific primitive depending on configured selector */
   (*kwUlUdxUbndReqMt[pst->selector])(pst, spId,reason );

   RETVALUE(ROK);

} /* end of KwDlUdxBndReq */

#ifdef ANSI
PUBLIC S16 KwUlUdxCfgReq
(
Pst *pst,                       /* post structure */
SpId spId,                      /* Service User Id */
CkwCfgInfo *cfmInfo             /* Config Info */
)
#else
PUBLIC S16 KwUlUdxCfgReq(pst, suId, status)
Pst *pst;                       /* post structure */
SuId spId;                      /* Service User Id */
CkwCfgInfo *cfmInfo;              /* Config Info */
#endif
{
   TRC3(KwUlUdxCfgReq)

   /* jump to specific primitive depending on configured selector */
   (*kwUlUdxCfgReqMt[pst->selector])(pst, spId, cfmInfo);

   RETVALUE(ROK);

} /* end of KwDlUdxCfgReq */

#ifdef ANSI
PUBLIC S16 KwUlUdxUeIdChgReq
(
Pst *pst,                      /* post structure */
SpId spId,                     /* Service User Id */
U32      transId,              /* transaction Id */
CkwUeInfo *ueInfo,             /* Config Info */
CkwUeInfo *newUeInfo           /* Config Info */
)
#else
PUBLIC S16 KwUlUdxUeIdChgReq(pst, spId,transId, ueInfo, newUeInfo)
Pst *pst;                      /* post structure */
SpId spId;                     /* Service User Id */
U32      transId;              /* transaction Id */
CkwUeInfo *ueInfo;             /* Config Info */
CkwUeInfo *newUeInfo;          /* Config Info */
#endif
{
   TRC3(KwUlUdxUeIdChgReq)

   /* jump to specific primitive depending on configured selector */
   (*kwUlUdxUeIdChgReqMt[pst->selector])(pst, spId,transId,ueInfo,newUeInfo);

   RETVALUE(ROK);

} /* end of KwDlUdxCfgReq */



PUBLIC S16  KwUlUdxStaPduReq
(
Pst*                pst,
SpId                spId,
CmLteRlcId          *rlcId,
KwUdxDlStaPdu       *pStaPdu
)
{
   TRC3(KwUlUdxStaPduReq);

   RETVALUE((*kwUlUdxStaPduReqMt[pst->selector])(pst,spId, rlcId, pStaPdu));
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
PUBLIC S16  KwUlUdxStaUpdReq
(
Pst*                pst,
SpId                spId,
CmLteRlcId          *rlcId,
KwUdxStaPdu         *pStaPdu
)
#else
PUBLIC S16 KwUlUdxStaUpdReq(pst, rlcId, pStaPdu)
Pst*                pst;
SpId                spId;
CmLteRlcId          *rlcId;
KwUdxStaPdu         *pStaPdu;
#endif
{

   TRC3(KwUlUdxStaUpdReq);

   RETVALUE((*kwUlUdxStaUpdReqMt[pst->selector])(pst,spId, rlcId, pStaPdu));
} /* end of KwUlmDlmStaUpd*/

#ifdef LTE_L2_MEAS
/**
* @brief Request from ULM to DLM for MeasReq 
*
* @details
*
*     Function : KwUlUdxL2MeasReq
*
*  @param[in]   Pst*           pst

*  @return   S16
*      -# ROK
**/
#ifdef ANSI
PUBLIC S16  KwUlUdxL2MeasReq
(
Pst*                pst,
KwL2MeasReqEvt      *measReqEvt 
)
#else
PUBLIC S16 KwUlUdxL2MeasReq(pst,measReqEvt)
Pst*                pst;
KwL2MeasReqEvt      *measReqEvt; 
#endif
{

   TRC3(KwUlUdxStaUpdReq);

   RETVALUE((*kwUlUdxL2MeasReqMt[pst->selector])(pst,measReqEvt));
} /* end of KwUlUdxL2MeasReq*/

/**
* @brief Request from ULM to DLM for MeasSendReq 
*
* @details
*
*     Function : KwUlUdxMeasSendReq
*
*  @param[in]   Pst*           pst

*  @return   S16
*      -# ROK
**/
#ifdef ANSI
PUBLIC S16  KwUlUdxL2MeasSendReq
(
Pst*                pst,
U8                  measType
)
#else
PUBLIC S16 KwUlUdxL2MeasSendReq(pst,measReqEvt)
Pst*                pst;
U8                  measType;
#endif
{

   TRC3(KwUlUdxStaUpdReq);

   RETVALUE((*kwUlUdxL2MeasSendReqMt[pst->selector])(pst,measType));
} /* end of KwUlUdxMesReq*/

/**
* @brief Request from ULM to DLM for MeasStopReq 
*
* @details
*
*     Function : KwUlUdxL2MeasStopReq
*
*  @param[in]   Pst*           pst

*  @return   S16
*      -# ROK
**/
#ifdef ANSI
PUBLIC S16  KwUlUdxL2MeasStopReq
(
Pst*                pst,
U8                  measType
)
#else
PUBLIC S16 KwUlUdxL2MeasStopReq(pst,measReqEvt)
Pst*                pst;
U8                  measType;
#endif
{

   TRC3(KwUlUdxStaUpdReq);

   RETVALUE((*kwUlUdxL2MeasStopReqMt[pst->selector])(pst,measType));
} /* end of KwUlUdxMesReq*/
#endif
/**********************************************************************
 
         End of file
**********************************************************************/
