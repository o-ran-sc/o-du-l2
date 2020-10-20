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
static S16 PtDlUdxBndCfm ARGS(( Pst *pst,SuId suId,U8 status ));
static S16 PtDlUdxCfgCfm ARGS((Pst *pst, SuId suId, RlcCfgCfmInfo *cfmInfo));
static S16 PtDlUdxUeIdChgCfm ARGS(( Pst *pst, SuId suId,U32 transId, 
                                     CmStatus status));
static S16  PtDlUdxStaUpdCfm ARGS(( Pst* pst,SuId suId,CmLteRlcId *rlcId,
                              RlcUdxBufLst *pBufLst));

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
static S16 PtDlUdxBndCfm
(
Pst *pst,                       /* post structure */
SuId suId,                      /* Service User Id */
U8   status                     /* Status */
)
#else
static S16 PtDlUdxBndCfm(pst, suId, status)
Pst *pst;                       /* post structure */
SuId suId;                      /* Service User Id */
U8   status;                    /* Status */
#endif
{
   TRC3(PtDlUdxBndCfm)

   return ROK;

} /* end of rlcDlUdxBndCfm */



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
static S16 PtDlUdxCfgCfm
(
Pst *pst,                       /* post structure */
SuId suId,                      /* Service User Id */
RlcCfgCfmInfo *cfmInfo                    /* Status */
)
#else
static S16 PtDlUdxCfgCfm(pst, suId, status)
Pst           *pst;            /* post structure */
SuId          suId;            /* Service User Id */
RlcCfgCfmInfo *cfmInfo;        /* Status */
#endif
{
   TRC3(PtDlUdxCfgCfm)

   return ROK;

} /* end of rlcDlUdxBndCfm */

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
static S16 PtDlUdxUeIdChgCfm
(
Pst *pst,                       /* post structure */
SuId suId,                      /* Service User Id */
U32       transId,
CmStatus      status
)
#else
static S16 PtDlUdxUeIdChgCfm(pst, suId, status)
Pst           *pst;            /* post structure */
SuId          suId;            /* Service User Id */
U32           transId;
CmStatus      status;
#endif
{
   TRC3(PtDlUdxUeIdChgCfm)

   return ROK;

} /* end of rlcDlUdxBndCfm */



static S16  PtDlUdxStaUpdCfm
(
Pst*                pst,
SuId                suId,
CmLteRlcId          *rlcId,
RlcUdxBufLst         *pBufLst
)
{
   TRC3(PtDlUdxStaUpdCfm);
   return ROK;
}

static S16  PtDlUdxStaProhTmrStart
(
Pst*                pst,
SuId                suId,
CmLteRlcId          *rlcId
)
{
   TRC3(PtDlUdxStaProhTmrStart);
   return ROK;
}
#endif

static UdxBndCfm rlcDlUdxBndCfmMt[UDX_MAX_SEL] =
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
   rlcUlUdxBndCfm,            /* 2 - tightly coupled, RRC  */
#else
   PtDlUdxBndCfm,            /* 2 - tightly coupled, portable */
#endif /* KW */
};

UdxCfgCfm rlcDlUdxCfgCfmMt[UDX_MAX_SEL] =
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
   rlcUlUdxCfgCfm,            /* 2 - tightly coupled, RRC  */
#else
   PtDlUdxCfgCfm,            /* 2 - tightly coupled, portable */
#endif /* KW */
};

UdxUeIdChgCfm rlcDlUdxUeIdChgCfmMt[UDX_MAX_SEL] =
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
   rlcUlUdxUeIdChgCfm,            /* 2 - tightly coupled, RRC  */
#else
   PtDlUdxUeIdChgCfm,            /* 2 - tightly coupled, portable */
#endif /* KW */
};


static CONSTANT UdxStaProhTmrStart rlcDlUdxStaProhTmrStartMt[UDX_MAX_SEL] =
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
   rlcUlUdxStaProhTmrStart,            /* 2 - tightly coupled, RRC  */
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
S16 rlcDlUdxBndCfm
(
Pst *pst,                       /* post structure */
SuId suId,                      /* Service User Id */
U8   status                     /* Status */
)
#else
S16 rlcDlUdxBndCfm(pst, suId, status)
Pst *pst;                       /* post structure */
SuId suId;                      /* Service User Id */
U8   status;                    /* Status */
#endif
{
   TRC3(rlcDlUdxBndCfm)

   /* jump to specific primitive depending on configured selector */
   (*rlcDlUdxBndCfmMt[pst->selector])(pst, suId, status);

   return ROK;

} /* end of rlcDlUdxBndCfm */



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
S16 rlcDlUdxCfgCfm
(
Pst *pst,                       /* post structure */
SuId suId,                      /* Service User Id */
RlcCfgCfmInfo *cfmInfo                    /* Status */
)
#else
S16 rlcDlUdxCfgCfm(pst, suId, status)
Pst           *pst;            /* post structure */
SuId          suId;            /* Service User Id */
RlcCfgCfmInfo *cfmInfo;        /* Status */
#endif
{
   TRC3(rlcDlUdxCfgCfm)

   /* jump to specific primitive depending on configured selector */
   (*rlcDlUdxCfgCfmMt[pst->selector])(pst, suId, cfmInfo);

   return ROK;

} /* end of rlcDlUdxBndCfm */

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
S16 rlcDlUdxUeIdChgCfm
(
Pst *pst,                       /* post structure */
SuId suId,                      /* Service User Id */
U32           transId,
CmStatus      status
)
#else
S16 rlcDlUdxUeIdChgCfm(pst, suId, status)
Pst           *pst;            /* post structure */
SuId          suId;            /* Service User Id */
U32           transId;
CmStatus      status;
#endif
{
   TRC3(rlcDlUdxUeIdChgCfm)

   /* jump to specific primitive depending on configured selector */
   (*rlcDlUdxUeIdChgCfmMt[pst->selector])(pst, suId,transId,status);

   return ROK;

} /* end of rlcDlUdxBndCfm */


uint8_t rlcDlUdxStaProhTmrStart(Pst *pst, SuId suId, CmLteRlcId *rlcId)
{
   /* jump to specific primitive depending on configured selector */
   (*rlcDlUdxStaProhTmrStartMt[pst->selector])(pst, suId, rlcId);

   return ROK;

} /* end of rlcDlUdxBndCfm */

/**********************************************************************
 
         End of file
**********************************************************************/
