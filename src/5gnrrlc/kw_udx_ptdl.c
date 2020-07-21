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
  
     File:     kw_udx_ptdl.c
  
**********************************************************************/

/** @file kw_udx_ptdl.c
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


#define UDX_MAX_SEL   3

#ifndef LCKWULUDX
PRIVATE S16 PtDlUdxBndCfm ARGS(( Pst *pst,SuId suId,U8 status ));
PRIVATE S16 PtDlUdxCfgCfm ARGS((Pst *pst, SuId suId, CkwCfgCfmInfo *cfmInfo));
PRIVATE S16 PtDlUdxUeIdChgCfm ARGS(( Pst *pst, SuId suId,U32 transId, 
                                     CmStatus status));
PRIVATE S16  PtDlUdxStaUpdCfm ARGS(( Pst* pst,SuId suId,CmLteRlcId *rlcId,
                              KwUdxBufLst *pBufLst));

/* UDX Bind Confirm primitive */
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
PRIVATE S16 PtDlUdxBndCfm
(
Pst *pst,                       /* post structure */
SuId suId,                      /* Service User Id */
U8   status                     /* Status */
)
#else
PRIVATE S16 PtDlUdxBndCfm(pst, suId, status)
Pst *pst;                       /* post structure */
SuId suId;                      /* Service User Id */
U8   status;                    /* Status */
#endif
{
   TRC3(PtDlUdxBndCfm)

   RETVALUE(ROK);

} /* end of KwDlUdxBndCfm */



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
PRIVATE S16 PtDlUdxCfgCfm
(
Pst *pst,                       /* post structure */
SuId suId,                      /* Service User Id */
CkwCfgCfmInfo *cfmInfo                    /* Status */
)
#else
PRIVATE S16 PtDlUdxCfgCfm(pst, suId, status)
Pst           *pst;            /* post structure */
SuId          suId;            /* Service User Id */
CkwCfgCfmInfo *cfmInfo;        /* Status */
#endif
{
   TRC3(PtDlUdxCfgCfm)

   RETVALUE(ROK);

} /* end of KwDlUdxBndCfm */

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
PRIVATE S16 PtDlUdxUeIdChgCfm
(
Pst *pst,                       /* post structure */
SuId suId,                      /* Service User Id */
U32       transId,
CmStatus      status
)
#else
PRIVATE S16 PtDlUdxUeIdChgCfm(pst, suId, status)
Pst           *pst;            /* post structure */
SuId          suId;            /* Service User Id */
U32           transId;
CmStatus      status;
#endif
{
   TRC3(PtDlUdxUeIdChgCfm)

   RETVALUE(ROK);

} /* end of KwDlUdxBndCfm */



PRIVATE S16  PtDlUdxStaUpdCfm
(
Pst*                pst,
SuId                suId,
CmLteRlcId          *rlcId,
KwUdxBufLst         *pBufLst
)
{
   TRC3(PtDlUdxStaUpdCfm);
   RETVALUE(ROK);
}

PRIVATE S16  PtDlUdxStaProhTmrStart
(
Pst*                pst,
SuId                suId,
CmLteRlcId          *rlcId
)
{
   TRC3(PtDlUdxStaProhTmrStart);
   RETVALUE(ROK);
}
#endif

PRIVATE UdxBndCfm kwDlUdxBndCfmMt[UDX_MAX_SEL] =
{
#ifdef LCKWULUDX
   cmPkUdxBndCfm,            /* 0 - loosely coupled */
#else
   PtDlUdxBndCfm,            /* 0 - loosely coupled, portable  */
#endif /* LCKWUIKWU */
#ifdef LWLCKWULUDX
   cmPkUdxBndCfm,            /* 1 - loosely coupled */
#else
   PtDlUdxBndCfm,            /* 1 - loosely coupled, portable  */
#endif /* LCKWUIKWU */
#ifdef KW
   KwUlUdxBndCfm,            /* 2 - tightly coupled, RRC  */
#else
   PtDlUdxBndCfm,            /* 2 - tightly coupled, portable */
#endif /* KW */
};

PUBLIC UdxCfgCfm kwDlUdxCfgCfmMt[UDX_MAX_SEL] =
{
#ifdef LCKWULUDX
   cmPkUdxCfgCfm,            /* 0 - loosely coupled */
#else
   PtDlUdxCfgCfm,            /* 0 - loosely coupled, portable  */
#endif /* LCKWUIKWU */
#ifdef LWLCKWULUDX
   cmPkUdxCfgCfm,            /* 1 - loosely coupled */
#else
   PtDlUdxCfgCfm,            /* 1 - loosely coupled, portable  */
#endif /* LCKWUIKWU */
#ifdef KW
   KwUlUdxCfgCfm,            /* 2 - tightly coupled, RRC  */
#else
   PtDlUdxCfgCfm,            /* 2 - tightly coupled, portable */
#endif /* KW */
};

PUBLIC UdxUeIdChgCfm kwDlUdxUeIdChgCfmMt[UDX_MAX_SEL] =
{
#ifdef LCKWULUDX
   cmPkUdxUeIdChgCfm,            /* 0 - loosely coupled */
#else
   PtDlUdxUeIdChgCfm,            /* 0 - loosely coupled, portable  */
#endif /* LCKWUIKWU */
#ifdef LWLCKWULUDX
   cmPkUdxUeIdChgCfm,            /* 1 - loosely coupled */
#else
   PtDlUdxUeIdChgCfm,            /* 1 - loosely coupled, portable  */
#endif /* LCKWUIKWU */
#ifdef KW
   KwUlUdxUeIdChgCfm,            /* 2 - tightly coupled, RRC  */
#else
   PtDlUdxUeIdChgCfm,            /* 2 - tightly coupled, portable */
#endif /* KW */
};


PRIVATE CONSTANT UdxStaProhTmrStart KwDlUdxStaProhTmrStartMt[UDX_MAX_SEL] =
{
#ifdef LCKWULUDX
   cmPkUdxStaProhTmrStart,
#else
   PtDlUdxStaProhTmrStart,
#endif
#ifdef LWLCKWULUDX
   cmPkUdxStaProhTmrStart,            /* 1 - loosely coupled */
#else
   PtDlUdxStaProhTmrStart,            /* 1 - loosely coupled, portable  */
#endif /* LCKWUIKWU */
#ifdef KW
   KwUlUdxStaProhTmrStart,            /* 2 - tightly coupled, RRC  */
#else
   PtDlUdxStaProhTmrStart,            /* 2 - tightly coupled, portable */
#endif /* KW */
};

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
PUBLIC S16 KwDlUdxBndCfm
(
Pst *pst,                       /* post structure */
SuId suId,                      /* Service User Id */
U8   status                     /* Status */
)
#else
PUBLIC S16 KwDlUdxBndCfm(pst, suId, status)
Pst *pst;                       /* post structure */
SuId suId;                      /* Service User Id */
U8   status;                    /* Status */
#endif
{
   TRC3(KwDlUdxBndCfm)

   /* jump to specific primitive depending on configured selector */
   (*kwDlUdxBndCfmMt[pst->selector])(pst, suId, status);

   RETVALUE(ROK);

} /* end of KwDlUdxBndCfm */



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
PUBLIC S16 KwDlUdxCfgCfm
(
Pst *pst,                       /* post structure */
SuId suId,                      /* Service User Id */
CkwCfgCfmInfo *cfmInfo                    /* Status */
)
#else
PUBLIC S16 KwDlUdxCfgCfm(pst, suId, status)
Pst           *pst;            /* post structure */
SuId          suId;            /* Service User Id */
CkwCfgCfmInfo *cfmInfo;        /* Status */
#endif
{
   TRC3(KwDlUdxCfgCfm)

   /* jump to specific primitive depending on configured selector */
   (*kwDlUdxCfgCfmMt[pst->selector])(pst, suId, cfmInfo);

   RETVALUE(ROK);

} /* end of KwDlUdxBndCfm */

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
PUBLIC S16 KwDlUdxUeIdChgCfm
(
Pst *pst,                       /* post structure */
SuId suId,                      /* Service User Id */
U32           transId,
CmStatus      status
)
#else
PUBLIC S16 KwDlUdxUeIdChgCfm(pst, suId, status)
Pst           *pst;            /* post structure */
SuId          suId;            /* Service User Id */
U32           transId;
CmStatus      status;
#endif
{
   TRC3(KwDlUdxUeIdChgCfm)

   /* jump to specific primitive depending on configured selector */
   (*kwDlUdxUeIdChgCfmMt[pst->selector])(pst, suId,transId,status);

   RETVALUE(ROK);

} /* end of KwDlUdxBndCfm */



#ifdef ANSI
PUBLIC S16 KwDlUdxStaProhTmrStart
(
Pst                 *pst,                       /* post structure */
SuId                suId,                      /* Service User Id */
CmLteRlcId          *rlcId
)
#else
PUBLIC S16 KwDlUdxStaProhTmrStart(pst, suId, rlcId)
Pst           *pst;            /* post structure */
SuId          suId;            /* Service User Id */
CmLteRlcId    *rlcId;
#endif
{
   TRC3(KwDlUdxStaProhTmrStart)

   /* jump to specific primitive depending on configured selector */
   (*KwDlUdxStaProhTmrStartMt[pst->selector])(pst, suId, rlcId);

   RETVALUE(ROK);

} /* end of KwDlUdxBndCfm */

/**********************************************************************
 
         End of file
**********************************************************************/
