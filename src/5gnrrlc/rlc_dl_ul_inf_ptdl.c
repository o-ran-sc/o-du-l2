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
  
     File:     rlc_dl_ul_inf_ptdl.c
  
**********************************************************************/

/** @file rlc_dl_ul_inf_ptdl.c
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

#define UDX_MAX_SEL   3

#ifndef LCKWULUDX
static S16 PtDlUdxBndCfm ARGS(( Pst *pst,SuId suId,uint8_t status ));
static S16 PtDlUdxCfgCfm ARGS((Pst *pst, SuId suId, RlcCfgCfmInfo *cfmInfo));
static S16 PtDlUdxUeIdChgCfm ARGS(( Pst *pst, SuId suId,uint32_t transId, 
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
static S16 PtDlUdxBndCfm
(
Pst *pst,                       /* post structure */
SuId suId,                      /* Service User Id */
uint8_t   status                     /* Status */
)
{

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
static S16 PtDlUdxCfgCfm
(
Pst *pst,                       /* post structure */
SuId suId,                      /* Service User Id */
RlcCfgCfmInfo *cfmInfo                    /* Status */
)
{

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
static S16 PtDlUdxUeIdChgCfm
(
Pst *pst,                       /* post structure */
SuId suId,                      /* Service User Id */
uint32_t transId,
CmStatus status
)
{

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
   return ROK;
}

static S16  PtDlUdxStaProhTmrStart
(
Pst*                pst,
SuId                suId,
CmLteRlcId          *rlcId
)
{
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


static const UdxStaProhTmrStart rlcDlUdxStaProhTmrStartMt[UDX_MAX_SEL] =
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
S16 rlcDlUdxBndCfm
(
Pst *pst,                       /* post structure */
SuId suId,                      /* Service User Id */
uint8_t status                     /* Status */
)
{

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
S16 rlcDlUdxCfgCfm
(
Pst *pst,                       /* post structure */
SuId suId,                      /* Service User Id */
RlcCfgCfmInfo *cfmInfo                    /* Status */
)
{

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
S16 rlcDlUdxUeIdChgCfm
(
Pst *pst,                       /* post structure */
SuId suId,                      /* Service User Id */
uint32_t      transId,
CmStatus      status
)
{

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
