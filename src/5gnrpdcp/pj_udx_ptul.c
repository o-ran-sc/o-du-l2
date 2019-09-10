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
  
        Name:    LTE-PDCP Layer - Upper Interface
    
        Type:    C file
  
        Desc:    C source code for the upper interface of LTE-PDCP
 
        File:    pj_udx_ptul.c
  
*********************************************************************21*/

/** @file pj_udx_ptul.c
@brief PDCP Upper Interface
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
#include "cpj.h"           /* UDX defines */
#include "pju.h"           /* PJU defines */
#include "lpj.h"
#include "pj_udx.h"

#include "pj_err.h"
#include "pj_env.h"        /* RLC environment options */

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
#include "cpj.x"           /* UDX defines */
#include "pju.x"           /* PJU defines */
#include "lpj.x"           /* LPJ defines */
#include "pj.h"
#include "pj.x"
#include "pj_udx.x"
#include "pj_ul.x"


#if !(defined(LCPJULUDX)  && defined(PJ) && defined(LWLCPJULUDX))
#define PTPJULUDX
#endif

#ifdef __cplusplus
EXTERN "C" {
#endif /* __cplusplus */

#ifdef PTPJULUDX
PRIVATE S16 PtUlUdxBndReq ARGS((Pst *post, SuId suId, SpId spId));
PRIVATE S16 PtUlUdxUbndReq ARGS((Pst *post, SpId spId, Reason reason));
PRIVATE S16 PtUlUdxCfgReq ARGS((Pst *post, SpId spId, UdxCfgReqInfo *pdCfgReq));
PRIVATE S16 PtUlUdxUeIdChgReq ARGS((Pst *post, SpId spId,U32 transId, 
                                    UdxUeInfo *oldUeInfo,
                                    UdxUeInfo *newUeInfo));

PRIVATE S16 PtUlUdxSecCfgReq ARGS((Pst *post, SpId spId, 
                                   UdxSecCfgReqInfo *pdcpSecCfgReq));
PRIVATE S16 PtUlUdxCountReq ARGS((Pst *post, SpId spId, 
                                   UdxCountReqInfo *pdcpCountReq));
PRIVATE S16 PtUlUdxReEstReq ARGS(( Pst *post, SpId spId,
                                   UdxReEstReqInfo *restbInfo));
PRIVATE S16 PtUlUdxSduStaReq ARGS(( Pst *post, SpId spId,
                                   UdxSduStaReqInfo *statReqInfo));

PRIVATE S16 PtUlUdxUlStaRep ARGS ((
         Pst *post,                      /* Post structure */
         SpId spId,                      /* Service provider ID */
         UdxUlStaRepInfo *pdStaRep         /* PDCP UL ot PDCP DL Reestablishment trigger */
         ));
PRIVATE S16 PtUlUdxDlStaRep ARGS ((
         Pst *post,                      /* Post structure */
         SpId spId,                      /* Service provider ID */
         UdxDlStaRepInfo *pdStaRep       /* Status report from PDCP UL to PDCP DL */
         ));

PRIVATE S16 PtUlUdxUlFdbkPktInfo ARGS ((
         Pst *post,                      /* Post structure */
         SpId spId,                      /* Service provider ID */
         UdxUlFdbkPktInfo  *fdbkPktInfo    /* feedback Packet Info */
            ));

PRIVATE S16 PtUlUdxDlFdbkPktInfo ARGS ((
         Pst *post,                      /* Post structure */
         SpId spId,                      /* Service provider ID */
         UdxDlFdbkPktInfo  *fdbkPktInfo    /* feedback Packet Info */
            ));
#ifdef LTE_L2_MEAS
PRIVATE S16  PtUlUdxL2MeasReq ARGS ((
         Pst *pst,
         PjL2MeasReqEvt *measReqEvt
         ));
PRIVATE S16  PtUlUdxL2MeasSendReq ARGS((
         Pst *pst,
         U8 status
         ));
PRIVATE S16  PtUlUdxL2MeasStopReq ARGS((
         Pst *pst,
         U8  status
         ));
#endif
PRIVATE S16  PtUlUdxDatResumeReq ARGS((
         Pst           *pst,
         UdxDatResumeInfo  *udxDatResumeInfo
         ));
PRIVATE S16  PtUlUdxDatForwardReq ARGS((
         Pst           *pst,
         UdxDatForwardInfo  *udxDatForwardInfo
         ));
#endif /* PTPJULUDX */


/**************************************************************************
 * UDX Interface Matrices
 *************************************************************************/

/* UDX Interface Bind Request primitive */
PRIVATE CONSTANT UdxBndReq PjUlUdxBndReqMt [PJ_MAX_UDX] =
{
#ifdef LCPJULUDX
   cmPkPjUdxBndReq,          /* 0 - loosely coupled */
#else /* LCPJULUDX */
   PtUlUdxBndReq,          /* 0 - loosely coupled, portable */
#endif /* LCPJULUDX */
#ifdef LWLCPJULUDX
   cmPkPjUdxBndReq,          /* 1 - loosely coupled */
#else /* LCPJULUDX */
   PtUlUdxBndReq,          /* 1 - loosely coupled, portable */
#endif /* LCPJULUDX */
#ifdef PJ
   PjDlUdxBndReq,          /* 2 - tightly coupled, CTC */
#else /* PJ */
   PtUlUdxBndReq,          /* 2 - tightly coupled, portable */
#endif /* PJ */
};

/* UDX Interface Unbind Request primitive */
PRIVATE CONSTANT UdxUbndReq PjUlUdxUbndReqMt [PJ_MAX_UDX] =
{
#ifdef LCPJULUDX
 cmPkPjUdxUbndReq,          /* 0 - loosely coupled */
#else /* LCPJULUDX */
 PtUlUdxUbndReq,          /* 0 - loosely coupled, portable */
#endif /* LCPJULUDX */
#ifdef LWLCPJULUDX
 cmPkPjUdxUbndReq,          /* 1 - loosely coupled */
#else /* LCPJULUDX */
 PtUlUdxUbndReq,          /* 1 - loosely coupled, portable */
#endif /* LCPJULUDX */
#ifdef PJ 
 PjDlUdxUbndReq,          /* 2 - tightly coupled, UDX */
#else /* PJ */
 PtUlUdxUbndReq,          /* 2 - tightly coupled, portable */
#endif /* PJ */
};

/* UDX Interface Config Request primitive */
PRIVATE CONSTANT UdxCfgReq PjUlUdxCfgReqMt [PJ_MAX_UDX] =
{
#ifdef LCPJULUDX
 cmPkPjUdxCfgReq,          /* 0 - loosely coupled */
#else /* LCPJULUDX */
 PtUlUdxCfgReq,          /* 0 - loosely coupled, portable */
#endif /* LCPJULUDX */
#ifdef LWLCPJULUDX
 cmPkPjUdxCfgReq,          /* 1 - loosely coupled */
#else /* LCPJULUDX */
 PtUlUdxCfgReq,          /* 1 - loosely coupled, portable */
#endif /* LCPJULUDX */
#ifdef PJ
 PjDlUdxCfgReq,          /* 2 - tightly coupled, UDX */
#else /* PJ */
 PtUlUdxCfgReq,          /* 2 - tightly coupled, portable */
#endif /* PJ */
};

/* UDX Interface Status Report from PDCP-UL to PDCP-DL primitive */
PRIVATE CONSTANT UdxUlStaRep PjUlUdxUlStaRepMt [PJ_MAX_UDX] =
{
#ifdef LCPJULUDX
 cmPkPjUdxUlStaRep,          /* 0 - loosely coupled */
#else /* LCPJULUDX */
 PtUlUdxUlStaRep,          /* 0 - loosely coupled, portable */
#endif /* LCPJULUDX */
#ifdef LWLCPJULUDX
 cmPkPjUdxUlStaRep,          /* 1 - loosely coupled */
#else /* LCPJULUDX */
 PtUlUdxUlStaRep,          /* 1 - loosely coupled, portable */
#endif /* LCPJULUDX */
#ifdef PJ
 PjDlUdxUlStaRep,          /* 2 - tightly coupled, UDX */
#else /* PJ */
 PtUlUdxUlStaRep,          /* 2 - tightly coupled, portable */
#endif /* PJ */
};

/* UDX Interface Status Report from PDCP-UL to PDCP-DL primitive */
PRIVATE CONSTANT UdxDlStaRep PjUlUdxDlStaRepMt [PJ_MAX_UDX] =
{
#ifdef LCPJULUDX
 cmPkPjUdxDlStaRep,          /* 0 - loosely coupled */
#else /* LCPJULUDX */
 PtUlUdxDlStaRep,          /* 0 - loosely coupled, portable */
#endif /* LCPJULUDX */
#ifdef LWLCPJULUDX
 cmPkPjUdxDlStaRep,          /* 1 - loosely coupled */
#else /* LCPJULUDX */
 PtUlUdxDlStaRep,          /* 1 - loosely coupled, portable */
#endif /* LCPJULUDX */
#ifdef PJ
 PjDlUdxDlStaRep,          /* 2 - tightly coupled, UDX */
#else /* PJ */
 PtUlUdxDlStaRep,          /* 2 - tightly coupled, portable */
#endif /* PJ */
};

/* UDX Interface Status Report from PDCP-UL to PDCP-DL primitive */
PRIVATE CONSTANT UdxDlFdbkPkt PjUlUdxDlFdbkPktInfoMt [PJ_MAX_UDX] =
{
#ifdef LCPJULUDX
 cmPkPjUdxDlFdbkPkt,          /* 0 - loosely coupled */
#else /* LCPJULUDX */
 PtUlUdxDlFdbkPktInfo,          /* 0 - loosely coupled, portable */
#endif /* LCPJULUDX */
#ifdef LWLCPJULUDX
 cmPkPjUdxDlFdbkPkt,          /* 1 - loosely coupled */
#else /* LCPJULUDX */
 PtUlUdxDlFdbkPktInfo,          /* 1 - loosely coupled, portable */
#endif /* LCPJULUDX */
#ifdef PJ
 PjDlUdxDlFdbkPktInfo,          /* 2 - tightly coupled, UDX */
#else /* PJ */
 PtUlUdxDlFdbkPktInfo,          /* 2 - tightly coupled, portable */
#endif /* PJ */
};


/* UDX Interface Feedback Pkt Info from PDCP-UL to PDCP-DL primitive */
PRIVATE CONSTANT UdxUlFdbkPkt PjUlUdxUlFdbkPktInfoMt [3] =
{
#ifdef LCPJULUDX
    cmPkPjUdxUlFdbkPkt,          /* 0 - loosely coupled */
#else /* LCPJULUDX */
    PtUlUdxUlFdbkPktInfo,      /* 0 - loosely coupled, portable */
#endif /* LCPJULUDX */
#ifdef LWLCPJULUDX
    cmPkPjUdxUlFdbkPkt,          /* 1 - loosely coupled */
#else /* LCPJULUDX */
    PtUlUdxUlFdbkPktInfo,      /* 1 - loosely coupled, portable */
#endif /* LCPJULUDX */
#ifdef PJ
    PjDlUdxUlFdbkPktInfo,        /* 2 - tightly coupled, UDX */
#else /* PJ */
    PtUlUdxUlFdbkPktInfo,      /* 2 - tightly coupled, portable */
#endif /* PJ */
};

/* UDX Interface UE Id Change Request primitive */
PRIVATE CONSTANT UdxUeIdChgReq PjUlUdxUeIdChgReqMt [PJ_MAX_UDX] =
{
#ifdef LCPJULUDX
   cmPkPjUdxUeIdChgReq,          /* 0 - loosely coupled */
#else /* LCPJULUDX */
   PtUlUdxUeIdChgReq,          /* 0 - loosely coupled, portable */
#endif /* LCPJULUDX */
#ifdef LWLCPJULUDX
   cmPkPjUdxUeIdChgReq,          /* 1 - loosely coupled */
#else /* LCPJULUDX */
   PtUlUdxUeIdChgReq,          /* 1 - loosely coupled, portable */
#endif /* LCPJULUDX */
#ifdef PJ
   PjDlUdxUeIdChgReq,          /* 2 - tightly coupled, UDX */
#else /* PJ */
   PtUlUdxUeIdChgReq,          /* 2 - tightly coupled, portable */
#endif /* PJ */
};

/* UDX Interface Security Config Request primitive */
PRIVATE CONSTANT UdxSecCfgReq PjUlUdxSecCfgReqMt [PJ_MAX_UDX] =
{
#ifdef LCPJULUDX
   cmPkPjUdxSecCfgReq,          /* 0 - loosely coupled */
#else /* LCPJULUDX */
   PtUlUdxSecCfgReq,          /* 0 - loosely coupled, portable */
#endif /* LCPJULUDX */
#ifdef LWLCPJULUDX
   cmPkPjUdxSecCfgReq,          /* 1 - loosely coupled */
#else /* LCPJULUDX */
   PtUlUdxSecCfgReq,          /* 1 - loosely coupled, portable */
#endif /* LCPJULUDX */
#ifdef PJ
   PjDlUdxSecCfgReq,          /* 2 - tightly coupled, UDX */
#else /* PJ */
   PtUlUdxSecCfgReq,          /* 2 - tightly coupled, portable */
#endif/* PJ */
};

/* UDX Interface COUNT Request primitive */
PRIVATE CONSTANT UdxCountReq PjUlUdxCountReqMt [PJ_MAX_UDX] =
{
#ifdef LCPJULUDX
   cmPkPjUdxCountReq,          /* 0 - loosely coupled */
#else /* LCPJULUDX */
   PtUlUdxCountReq,          /* 0 - loosely coupled, portable */
#endif /* LCPJULUDX */
#ifdef LWLCPJULUDX
   cmPkPjUdxCountReq,          /* 1 - loosely coupled */
#else /* LCPJULUDX */
   PtUlUdxCountReq,          /* 1 - loosely coupled, portable */
#endif /* LCPJULUDX */
#ifdef PJ
   PjDlUdxCountReq,          /* 2 - tightly coupled, UDX */
#else /* PJ */
   PtUlUdxCountReq,          /* 2 - tightly coupled, portable */
#endif /* PJ */
};

/* UDX Interface PDCP Status Request primitive */
PRIVATE CONSTANT UdxSduStaReq PjUlUdxSduStaReqMt [PJ_MAX_UDX] =
{
#ifdef LCPJULUDX
   cmPkPjUdxSduStaReq,          /* 0 - loosely coupled */
#else /* LCPJULUDX */
   PtUlUdxSduStaReq,          /* 0 - loosely coupled, portable */
#endif /* LCPJULUDX */
#ifdef LWLCPJULUDX
   cmPkPjUdxSduStaReq,          /* 1 - loosely coupled */
#else /* LCPJULUDX */
   PtUlUdxSduStaReq,          /* 1 - loosely coupled, portable */
#endif /* LCPJULUDX */
#ifdef PJ
   PjDlUdxSduStaReq,          /* 2 - tightly coupled, UDX */
#else /* PJ */
   PtUlUdxSduStaReq,          /* 2 - tightly coupled, portable */
#endif /* PJ */
};

/* UDX Interface Re-Establishment Request primitive */
PRIVATE CONSTANT UdxReEstReq PjUlUdxReEstReqMt [PJ_MAX_UDX] =
{
#ifdef LCPJULUDX
   cmPkPjUdxReEstReq,          /* 0 - loosely coupled */
#else /* LCPJULUDX */
   PtUlUdxReEstReq,          /* 0 - loosely coupled, portable */
#endif /* LCPJULUDX */
#ifdef LWLCPJULUDX
   cmPkPjUdxReEstReq,          /* 1 - loosely coupled */
#else /* LCPJULUDX */
   PtUlUdxReEstReq,          /* 1 - loosely coupled, portable */
#endif /* LCPJULUDX */
#ifdef PJ
   PjDlUdxReEstReq,          /* 2 - tightly coupled, UDX */
#else /* PJ */
   PtUlUdxReEstReq,          /* 2 - tightly coupled, portable */
#endif /* PJ */
};

#ifdef LTE_L2_MEAS
PRIVATE CONSTANT UdxL2MeasReq pjUlUdxL2MeasReqMt[PJ_MAX_UDX] =
{
#ifdef LCPJULUDX
   cmPkPjUdxL2MeasReq,            /* 0 - loosely coupled */
#else
   PtUlUdxL2MeasReq,            /* 0 - loosely coupled, portable  */
#endif /* LCKWUIKWU */
#ifdef LWLCPJULUDX
   cmPkPjUdxL2MeasReq,            /* 1 - loosely coupled */
#else
   PtUlUdxL2MeasReq,            /* 1 - loosely coupled, portable  */
#endif /* LCKWUIKWU */
#ifdef PJ
   PjDlUdxL2MeasReq,            /* 2 - tightly coupled, RRC  */
#else
   PtUlUdxL2MeasReq,            /* 2 - tightly coupled, portable */
#endif /* KW */
};

PRIVATE CONSTANT UdxL2MeasSendReq pjUlUdxL2MeasSendReqMt[PJ_MAX_UDX] =
{
#ifdef LCPJULUDX
   cmPkPjUdxL2MeasSendReq,            /* 0 - loosely coupled */
#else
   PtUlUdxL2MeasSendReq,            /* 0 - loosely coupled, portable  */
#endif /* LCKWUIKWU */
#ifdef LWLCPJULUDX
   cmPkPjUdxL2MeasSendReq,            /* 1 - loosely coupled */
#else
   PtUlUdxL2MeasSendReq,            /* 1 - loosely coupled, portable  */
#endif /* LCKWUIKWU */
#ifdef PJ
   PjDlUdxL2MeasSendReq,            /* 2 - tightly coupled, RRC  */
#else
   PtUlUdxL2MeasSendReq,            /* 2 - tightly coupled, portable */
#endif /* PJ */
};

PRIVATE CONSTANT UdxL2MeasStopReq pjUlUdxL2MeasStopReqMt[PJ_MAX_UDX] =
{
#ifdef LCPJULUDX
   cmPkPjUdxL2MeasStopReq,            /* 0 - loosely coupled */
#else
   PtUlUdxL2MeasStopReq,            /* 0 - loosely coupled, portable  */
#endif /* LCKWUIKWU */
#ifdef LWLCPJULUDX
   cmPkPjUdxL2MeasStopReq,            /* 1 - loosely coupled */
#else
   PtUlUdxL2MeasStopReq,            /* 1 - loosely coupled, portable  */
#endif /* LCKWUIKWU */
#ifdef PJ
   PjDlUdxL2MeasStopReq,            /* 2 - tightly coupled, RRC  */
#else
   PtUlUdxL2MeasStopReq,            /* 2 - tightly coupled, portable */
#endif /* PJ */
};
#endif


/* UDX Interface PDCP Status Request primitive */
PRIVATE CONSTANT UdxDatResumeReq pjUlUdxDatResumeReqMt [PJ_MAX_UDX] =
{
#ifdef LCPJULUDX
   cmPkPjUdxDatResumeReq,          /* 0 - loosely coupled */
#else /* LCPJULUDX */
   PtUlUdxDatResumeReq,          /* 0 - loosely coupled, portable */
#endif /* LCPJULUDX */
#ifdef LWLCPJULUDX
   cmPkPjUdxDatResumeReq,          /* 1 - loosely coupled */
#else /* LCPJULUDX */
   PtUlUdxDatResumeReq,          /* 1 - loosely coupled, portable */
#endif /* LCPJULUDX */
#ifdef PJ
   PjDlUdxDatResumeReq,          /* 2 - tightly coupled, UDX */
#else /* PJ */
   PtUlUdxDatResumeReq,          /* 2 - tightly coupled, portable */
#endif /* PJ */
};


/* UDX Interface PDCP Start Data Forward primitive */
PRIVATE CONSTANT UdxDatForwardReq pjUlUdxDatForwardReqMt [PJ_MAX_UDX] =
{
#ifdef LCPJULUDX
   cmPkPjUdxDatForwardReq,          /* 0 - loosely coupled */
#else /* LCPJULUDX */
   PtUlUdxDatForwardReq,          /* 0 - loosely coupled, portable */
#endif /* LCPJULUDX */
#ifdef LWLCPJULUDX
   cmPkPjUdxDatForwardReq,          /* 1 - loosely coupled */
#else /* LCPJULUDX */
   PtUlUdxDatForwardReq,          /* 1 - loosely coupled, portable */
#endif /* LCPJULUDX */
#ifdef PJ
   PjDlUdxDatForwardReq,          /* 2 - tightly coupled, UDX */
#else /* PJ */
   PtUlUdxDatForwardReq,          /* 2 - tightly coupled, portable */
#endif /* PJ */
};

/**************************************************************************
 * The UDX Interface
 *************************************************************************/
/**
 *
 *   @brief  This function binds the RRC service user with the PDCP 
 *             service provider.
 *
 *   @details  This function binds the RRC service user with the PDCP 
 *             service provider. The PDCP registers this new service 
 *             user and allocates a Service Access Point (SAP) for 
 *             this bind. The service user specifies the reference 
 *             number that is used for the duration of this bind.
 */
#ifdef ANSI
PUBLIC S16 PjUlUdxBndReq
(
Pst *post,                       /* Post structure */
SuId suId,                      /* Service user ID */
SpId spId                       /* Service provider ID */
)
#else /* ANSI */
PUBLIC S16 PjUlUdxBndReq(post, suId, spId)
Pst *post;                       /* Post structure */
SuId suId;                      /* Service user ID */
SpId spId;                      /* Service provider ID */
#endif /* ANSI */
{

   TRC3(PjUlUdxBndReq);

   RETVALUE((*PjUlUdxBndReqMt[post->selector])(post, suId, spId));

} /* end of function PjUlUdxBndReq */


/*
*      @brief  This function unbinds the RRC service user with the PDCP 
*             service provider. 
*
*      @details This function unbinds the RRC service user with the PDCP 
*             service provider. 
*
*/
#ifdef ANSI
PUBLIC S16 PjUlUdxUbndReq
(
Pst *post,                       /* Post structure */
SpId spId,                      /* Service provider ID */
Reason reason                   /* Reason */
)
#else /* ANSI */
PUBLIC S16 PjUlUdxUbndReq(post, spId, reason)
Pst *post;                       /* Post structure */
SpId spId;                      /* Service provider ID */
Reason reason;                  /* Reason */
#endif /* ANSI */
{

   TRC3(PjUlUdxUbndReq);

   RETVALUE((*PjUlUdxUbndReqMt[post->selector])(post, spId, reason));

} /* end of function PjUlUdxUbndReq */


/**
*
*      @brief  This function is used to pass Signalling Info to PDCP related 
*             to PDCP configuration.
*
*      @details This function is used to pass signaling Info to PDCP related 
*             to PDCP configuration.
*
*/
#ifdef ANSI
PUBLIC S16 PjUlUdxCfgReq
(
Pst *post,                       /* Post structure */
SpId spId,                      /* Service provider ID */
UdxCfgReqInfo *pdCfgReq              /* PDCP Config parameters */
)
#else /* ANSI */
PUBLIC S16 PjUlUdxCfgReq(post, spId, pdCfgReq)
Pst *post;                       /* Post structure */
SpId spId;                      /* Service provider ID */
UdxCfgReqInfo *pdCfgReq;             /* PDCP Config parameters */
#endif /* ANSI */
{

  TRC3(PjUlUdxCfgReq);


  RETVALUE((*PjUlUdxCfgReqMt[post->selector])(post, spId, pdCfgReq));

} /* end of function PjUlUdxCfgReq */


/**
*
*      @brief  This function is used to sent the PDCP STATUS report to PDCP-DL during the Reestablishment.
*      @details This function is used to sent the PDCP STATUS report to PDCP-DL during the Reestablishment
*
*/
#ifdef ANSI
PUBLIC S16 PjUlUdxUlStaRep
(
Pst *pst,                       /* Post structure */
SpId spId,                      /* Service provider ID */
UdxUlStaRepInfo *staRep           /*PDCP Status Report*/
)
#else /* ANSI */
PUBLIC S16 PjUlUdxUlStaRep (pst, spId, staRep)
Pst *pst;                       /* Post structure */
SpId spId;                      /* Service provider ID */
UdxUlStaRepInfo *staRep;          /* PDCP Status Report*/
#endif /* ANSI */
{

  TRC3(PjUlUdxUlStaRep);

  RETVALUE((*PjUlUdxUlStaRepMt[pst->selector])(pst, spId, staRep));

} /* end of function PjUlUdxUlStaRep */

/**
*
*      @brief  This function is used to sent the PDCP STATUS report received from peer RLC to PDCP-DL.
*      @details This function is used to sent the PDCP STATUS report received from peer RLC to PDCP-DL
*
*/
#ifdef ANSI
PUBLIC S16 PjUlUdxDlStaRep
(
Pst *pst,                       /* Post structure */
SpId spId,                      /* Service provider ID */
UdxDlStaRepInfo *staRep           /*PDCP Status Report*/
)
#else /* ANSI */
PUBLIC S16 PjUlUdxDlStaRep (pst, spId, staRep)
Pst *pst;                       /* Post structure */
SpId spId;                      /* Service provider ID */
UdxDlStaRepInfo *staRep;          /* PDCP Status Report*/
#endif /* ANSI */
{

  TRC3(PjUlUdxDlStaRep);

  RETVALUE((*PjUlUdxDlStaRepMt[pst->selector])(pst, spId, staRep));

} /* end of function PjUlUdxUlStaRep */

/**
*
*      @brief  This function is used to sent the ROHC feedback report received from peer PDCP to PDCP-DL
*      @details This function is used to sent the ROHC feedback report received from peer PDCP to PDCP-DL
*
*/
#ifdef ANSI
PUBLIC S16 PjUlUdxDlFdbkPktInfo
(
Pst *pst,                       /* Post structure */
SpId spId,                      /* Service provider ID */
UdxDlFdbkPktInfo *fdbk           /*ROHC feedback*/
)
#else /* ANSI */
PUBLIC S16 PjUlUdxDlFdbkPktInfo (pst, spId, fdbk)
Pst *pst;                       /* Post structure */
SpId spId;                      /* Service provider ID */
UdxDlFdbkPktInfo *fdbk;          /* ROHC feedback*/
#endif /* ANSI */
{

  TRC3(PjUlUdxDlFdbkPktInfo);

  RETVALUE((*PjUlUdxDlFdbkPktInfoMt[pst->selector])(pst, spId, fdbk));

} /* end of function PjUlUdxDlFdbkPktInfo */

/**
 *
 *      @brief  This function is used to sent the Feedback pkt info to DL-PDCP.
 *      @details This function is used to sent the Feedback pkt info to DL-PDCP.
 *
 */
#ifdef ANSI
PUBLIC S16 PjUlUdxUlFdbkPktInfo
(
Pst *pst,                       /* Post structure */
SpId spId,                      /* Service provider ID */
UdxUlFdbkPktInfo *fdbkPktInfo           /*Feedback Packet Info*/
)
#else /* ANSI */
PUBLIC S16 PjUlUdxUlFdbkPktInfo (pst, spId, staRep)
Pst *pst;                       /* Post structure */
SpId spId;                      /* Service provider ID */
UdxUlFdbkPktInfo *fdbkPktInfo;          /* Feedback Packet Info*/
#endif /* ANSI */
{

    TRC3(PjUlUdxUlFdbkPktInfo);

    RETVALUE((*PjUlUdxUlFdbkPktInfoMt[pst->selector])(pst, spId, fdbkPktInfo));

} /* end of function PjUlUdxUlFdbkPktInfo */


/**
 *
 *      @brief  Sends an UE Id Change Indication to PDCP on Ckw interface
 *      @details Sends an UE Id Change Indication to PDCP on Ckw interface
 *
 */
#ifdef ANSI
PUBLIC S16 PjUlUdxUeIdChgReq
(
Pst       *post,          /* post structure */
SpId       spId,         /* service provider */
U32        transId,      /* RRC Transaction Id */
UdxUeInfo *oldUeInfo,    /* current UE Id */
UdxUeInfo *newUeInfo     /* new UE Id */
)
#else /* ANSI */
PUBLIC S16 PjUlUdxUeIdChgReq(post, spId, transId, oldUeInfo, newUeInfo)
Pst       *post;              /* post structure */
SpId       spId;             /* service provider */
U32        transId;          /* RRC Transaction Id */
UdxUeInfo *oldUeInfo;        /* current UE Id */
UdxUeInfo *newUeInfo;        /* new UE Id */
#endif /* ANSI */
{
  TRC3(PjUlUdxUeIdChgReq);

  RETVALUE((*PjUlUdxUeIdChgReqMt[post->selector])(post, spId, transId,
            (UdxUeInfo *)oldUeInfo, (UdxUeInfo *)newUeInfo ));

} /* end of function PjUlUdxUeIdChgReq */

/**
*
*      @brief  This function is used to configure PDCP security parameters 
*              (parametrs of Integrity protection and ciphering)
*
*      @details  This function is used to configure PDCP security parameters 
*              (parametrs of Integrity protection and ciphering)
*
*/
#ifdef ANSI
PUBLIC S16 PjUlUdxSecCfgReq
(
Pst *post,                                /* Post structure */
SpId spId,                               /* Service provider ID */
UdxSecCfgReqInfo *cpjSecConfigReqInfo    /* PDCP Security Config parameters */
)
#else /* ANSI */
PUBLIC S16 PjUlUdxSecCfgReq(post, spId, cpjSecConfigReqInfo)
Pst *post;                                /* Post structure */
SpId spId;                               /* Service provider ID */
UdxSecCfgReqInfo *cpjSecConfigReqInfo;   /* PDCP Security Config parameters */
#endif /* ANSI */
{

  TRC3(PjUlUdxSecCfgReq);

  RETVALUE((*PjUlUdxSecCfgReqMt[post->selector])(post, spId, 
                            cpjSecConfigReqInfo));

} /* end of function PjUlUdxSecCfgReq */

/**
*
*      @brief  This function is used to get COUNT values of each established DRB 
*
*      @details  This function is used to get COUNT values of each established DRB 
*
*/
#ifdef ANSI
PUBLIC S16 PjUlUdxCountReq
(
Pst *post,                       /* Post structure */
SpId spId,                      /* Service provider ID */
UdxCountReqInfo *pdcpCountReq              /* PDCP COUNT request paramters */
)
#else /* ANSI */
PUBLIC S16 PjUlUdxCountReq(post, spId, pdcpCountReq)
Pst *post;                       /* Post structure */
SpId spId;                      /* Service provider ID */
UdxCountReqInfo *pdcpCountReq;              /* PDCP COUNT request paramters */
#endif /* ANSI */
{

  TRC3(PjUlUdxCountReq);

  RETVALUE((*PjUlUdxCountReqMt[post->selector])(post, spId, pdcpCountReq));

} /* end of function PjUlUdxCountReq */

/**
*
*      @brief  This function is used to get PDCP Sdu Status for all DRBs.
*
*      @details  This function is used to get PDCP Sdu Status for all DRBs
*                from PDCP at source during HO.
*
*/
#ifdef ANSI
PUBLIC S16 PjUlUdxSduStaReq
(
Pst *post,                       /* Post structure */
SpId spId,                      /* Service provider ID */
UdxSduStaReqInfo *pdcpSduStaReq     /* PDCP SDU status request parameters */
)
#else /* ANSI */
PUBLIC S16 PjUlUdxSduStaReq(post, spId, pdcpSduStaReq)
Pst *post;                       /* Post structure */
SpId spId;                      /* Service provider ID */
UdxSduStaReqInfo *pdcpSduStaReq;    /* PDCP SDU status request parameters */
#endif /* ANSI */
{

  TRC3(PjUlUdxSduStaReq);

  RETVALUE((*PjUlUdxSduStaReqMt[post->selector])(post, spId, pdcpSduStaReq));

} /* end of function PjUlUdxSduStaReq */


/**
 *
 *      @brief  Sends a Re-establish request to PDCP on UDX interface
 *      @details Sends a Re-establish request to PDCP on UDX interface 
 *
 */
#ifdef ANSI
PUBLIC S16 PjUlUdxReEstReq
(
Pst       *post,          /* post structure */
SpId       spId,         /* service provider */
UdxReEstReqInfo *restbInfo     /* Re-establishment Info */
)
#else /* ANSI */
PUBLIC S16 PjUlUdxReEstReq(post, spId, restbInfo)
Pst       *post;              /* post structure */
SpId       spId;             /* service provider */
UdxReEstReqInfo *restbInfo;        /* new UE Id */
#endif /* ANSI */
{
  TRC3(PjUlUdxReEstReq);

  RETVALUE((*PjUlUdxReEstReqMt[post->selector])(post, spId, restbInfo));

} /* end of function PjUlUdxReEstReq */

#ifdef LTE_L2_MEAS
/**
* @brief Request from ULM to DLM for MeasReq 
*
* @details
*
*     Function : PjUlUdxL2MeasReq
*
*  @param[in]   Pst*           pst

*  @return   S16
*      -# ROK
**/
#ifdef ANSI
PUBLIC S16  PjUlUdxL2MeasReq
(
Pst*                pst,
PjL2MeasReqEvt      *measReqEvt 
)
#else
PUBLIC S16 PjUlUdxL2MeasReq(pst,measReqEvt)
Pst*                pst;
PjL2MeasReqEvt      *measReqEvt; 
#endif
{

   TRC3(PjUlUdxStaUpdReq);

   RETVALUE((*pjUlUdxL2MeasReqMt[pst->selector])(pst,measReqEvt));
} /* end of PjUlUdxMesReq*/

/**
* @brief Request from ULM to DLM for MeasSendReq 
*
* @details
*
*     Function : PjUlUdxMeasSendReq
*
*  @param[in]   Pst*           pst

*  @return   S16
*      -# ROK
**/
#ifdef ANSI
PUBLIC S16  PjUlUdxL2MeasSendReq
(
Pst*                pst,
U8                  measType
)
#else
PUBLIC S16 PjUlUdxL2MeasSendReq(pst,measReqEvt)
Pst*                pst;
U8                  measType;
#endif
{

   TRC3(PjUlUdxL2MeasSendReq);

   RETVALUE((*pjUlUdxL2MeasSendReqMt[pst->selector])(pst,measType));
} /* end of PjUlUdxMesReq*/

/**
* @brief Request from ULM to DLM for MeasStopReq 
*
* @details
*
*     Function : PjUlUdxL2MeasStopReq
*
*  @param[in]   Pst*           pst

*  @return   S16
*      -# ROK
**/
#ifdef ANSI
PUBLIC S16  PjUlUdxL2MeasStopReq
(
Pst*                pst,
U8                  measType
)
#else
PUBLIC S16 PjUlUdxL2MeasStopReq(pst,measReqEvt)
Pst*                pst;
U8                  measType;
#endif
{

   TRC3(PjUlUdxL2MeasStopReq);

   RETVALUE((*pjUlUdxL2MeasStopReqMt[pst->selector])(pst,measType));
} /* end of PjUlUdxMesReq*/
#endif



/**
* @brief Request from ULM to DLM for MeasStopReq 
*
* @details
*
*     Function : PjUlUdxDatResumeReq
*
*  @param[in]   Pst*           pst

*  @return   S16
*      -# ROK
**/
#ifdef ANSI
PUBLIC S16  PjUlUdxDatResumeReq
(
Pst*          pst,
UdxDatResumeInfo  *udxDatResumeInfo
)
#else
PUBLIC S16 PjUlUdxDatResumeReq(pst, udxDatResumeInfo)
Pst*          pst;
UdxDatResumeInfo  *udxDatResumeInfo; 
#endif
{

   TRC3(PjUlUdxDatResumeReq);

   RETVALUE((*pjUlUdxDatResumeReqMt[pst->selector])(pst, udxDatResumeInfo));
} /* end of PjUlUdxMesReq*/


/**
* @brief 
*
* @details
*
*     Function : PjUlUdxDatForwardReq
*
*  @param[in]   Pst*           pst

*  @return   S16
*      -# ROK
**/
#ifdef ANSI
PUBLIC S16  PjUlUdxDatForwardReq
(
Pst*          pst,
UdxDatForwardInfo  *udxDatForwardInfo
)
#else
PUBLIC S16 PjUlUdxDatForwardReq(pst, udxDatForwardInfo)
Pst*          pst;
UdxDatForwardInfo  *udxDatForwardInfo; 
#endif
{

   TRC3(PjUlUdxDatForwardReq);

   RETVALUE((*pjUlUdxDatForwardReqMt[pst->selector])(pst, udxDatForwardInfo));
} /* end of PjUlUdxMesReq*/
/**************************************************************************
 * The UDX (Portable) Interface
 *************************************************************************/
#ifdef PTPJULUDX
/**************************************************************************
 *
 *      Fun:   PtUlUdxBndReq
 *
 *      Desc:  Portable UdxBndReq primitive
 *
 *      Ret:   ROK 
 *
 *      Notes: 
 *
 *      File:  nh_ptli.c
 *
 *************************************************************************/
#ifdef ANSI
PRIVATE S16 PtUlUdxBndReq
(
Pst *post,                      /* Post structure */
SuId suId,                      /* Service user ID */
SpId spId                       /* Service provider ID */
)
#else /* ANSI */
PRIVATE S16 PtUlUdxBndReq(post, suId, spId)
Pst *post;                      /* Post structure */
SuId suId;                      /* Service user ID */
SpId spId;                      /* Service provider ID */
#endif /* ANSI */
{

   TRC3(PtUlUdxBndReq);

   UNUSED(post);
   UNUSED(suId);
   UNUSED(spId);

   RETVALUE(ROK);

} /* end of function PtUlUdxBndReq */


/**************************************************************************
*
*      Fun:   PtUlUdxUbndReq
*
*      Desc:  Portable UdxUbndReq primitive
*
*      Ret:   ROK 
*
*      Notes: 
*
*      File:  nh_ptli.c
*
*************************************************************************/
#ifdef ANSI
PRIVATE S16 PtUlUdxUbndReq
(
Pst *post,                      /* Post structure */
SpId spId,                      /* Service provider ID */
Reason reason                   /* Reason */
)
#else /* ANSI */
PRIVATE S16 PtUlUdxUbndReq(post, spId, reason)
Pst *post;                      /* Post structure */
SpId spId;                      /* Service provider ID */
Reason reason;                  /* Reason */
#endif /* ANSI */
{

   TRC3(PtUlUdxUbndReq);

   UNUSED(post);
   UNUSED(spId);
   UNUSED(reason);

   RETVALUE(ROK);

} /* end of function PtUlUdxUbndReq */


/**************************************************************************
*
*      Fun:   PtUlUdxCfgReq
*
*      Desc:  Portable UdxCfgReq primitive
*
*      Ret:   ROK 
*
*      Notes: 
*
*      File:  nh_ptli.c
*
*************************************************************************/
#ifdef ANSI
PRIVATE S16 PtUlUdxCfgReq
(
Pst *post,                      /* Post structure */
SpId spId,                      /* Service provider ID */
UdxCfgReqInfo *pdCfgReq         /* PDCP Config parameters */
)
#else /* ANSI */
PRIVATE S16 PtUlUdxCfgReq(post, spId, pdCfgReq)
Pst *post;                      /* Post structure */
SpId spId;                      /* Service provider ID */
UdxCfgReqInfo *pdCfgReq;        /* PDCP Config parameters */
#endif /* ANSI */
{

   TRC3(PtUlUdxCfgReq);

   UNUSED(post);
   UNUSED(spId);
   UNUSED(pdCfgReq);


   RETVALUE(ROK);

} /* end of function PtUlUdxCfgReq */

/**************************************************************************
*
*      Fun:   PtUlUdxUlStaRep
*
*      Desc:  Portable UdxUlStaRep primitive
*
*      Ret:   ROK 
*
*      Notes: 
*
*      File:  pj_udx_ptul.c
*
*************************************************************************/
#ifdef ANSI
PRIVATE S16 PtUlUdxUlStaRep
(
Pst *post,                      /* Post structure */
SpId spId,                      /* Service provider ID */
UdxUlStaRepInfo *pdStaRep         /* PDCP UL ot PDCP DL Reestablishment trigger */
)
#else /* ANSI */
PRIVATE S16 PtUlUdxUlStaRep(post, spId, pdStaRep)
Pst *post;                      /* Post structure */
SpId spId;                      /* Service provider ID */
UdxUlStaRepInfo *pdStaRep;        /* PDCP UL ot PDCP DL Reestablishment trigger */
#endif /* ANSI */
{

   TRC3(PtUlUdxUlStaRep);

   UNUSED(post);
   UNUSED(spId);
   UNUSED(pdStaRep);


   RETVALUE(ROK);

} /* end of function  */

/**************************************************************************
*
*      Fun:   PtUlUdxUlStaRep
*
*      Desc:  Portable UdxUlStaRep primitive
*
*      Ret:   ROK 
*
*      Notes: 
*
*      File:  pj_udx_ptul.c
*
*************************************************************************/
#ifdef ANSI
PRIVATE S16 PtUlUdxDlStaRep
(
Pst *post,                      /* Post structure */
SpId spId,                      /* Service provider ID */
UdxDlStaRepInfo *pdStaRep       /* Status report from PDCP UL to PDCP DL */
)
#else /* ANSI */
PRIVATE S16 PtUlUdxDlStaRep(post, spId, pdStaRep)
Pst *post;                      /* Post structure */
SpId spId;                      /* Service provider ID */
UdxDlStaRepInfo *pdStaRep;      /* Status report from PDCP UL to PDCP DL */
#endif /* ANSI */
{

   TRC3(PtUlUdxDlStaRep);

   UNUSED(post);
   UNUSED(spId);
   UNUSED(pdStaRep);


   RETVALUE(ROK);

} /* end of function  */


/**************************************************************************
 *
 *      Fun:   PtUlUdxUeIdChgReq
 *
 *      Desc:  Portable UdxUeIdChgReq primitive
 *
 *      Ret:   ROK 
 *
 *      Notes: 
 *
 *      File:  nh_ptli.c
 *
 *************************************************************************/
#ifdef ANSI
PRIVATE S16 PtUlUdxUeIdChgReq
(
Pst       *post,           /* post structure */
SpId      spId,            /* service provider */
U32       transId,         /* TransId */
UdxUeInfo *oldUeInfo,      /* current UE Id */
UdxUeInfo *newUeInfo       /* new UE Id */
)
#else /* ANSI */
PRIVATE S16 PtUlUdxUeIdChgReq(post, spId, transId, oldUeInfo, newUeInfo)
Pst       *post;           /* post structure */
SpId       spId;           /* service provider */
U32        transId;        /* TransId */
UdxUeInfo *oldUeInfo;      /* current UE Id */
UdxUeInfo *newUeInfo;      /* new UE Id */
#endif /* ANSI */
{
  TRC3(PtUlUdxUeIdChgReq);

  UNUSED(post);
  UNUSED(spId);
  UNUSED(transId);
  UNUSED(oldUeInfo);
  UNUSED(newUeInfo);

  RETVALUE(ROK);

} /* end of function PtUlCkwUeIdChgReq */

/**************************************************************************
 *
 *      Fun:   PtUlUdxSecCfgReq
 *
 *      Desc:  Portable UdxSecCfgReq primitive
 *
 *      Ret:   ROK 
 *
 *      Notes: 
 *
 *      File:  nh_ptli.c
 *
 *************************************************************************/
#ifdef ANSI
PRIVATE S16 PtUlUdxSecCfgReq
(
Pst       *post,                  /* post structure */
SpId      spId,                   /* service provider */
UdxSecCfgReqInfo *secCfgReq       /* PDCP Security Config paramters */
)
#else /* ANSI */
PRIVATE S16 PtUlUdxSecCfgReq(post, spId, secCfgReq)
Pst       *post;                  /* post structure */
SpId       spId;                  /* service provider */
UdxSecCfgReqInfo *secCfgReq;      /* PDCP Security Config paramters */
#endif /* ANSI */
{
  TRC3(PtUlUdxSecCfgReq);

  UNUSED(post);
  UNUSED(spId);
  UNUSED(secCfgReq);

  RETVALUE(ROK);

} /* end of function PtUlUdxSecCfgReq */

/**************************************************************************
 *
 *      Fun:   PtUlUdxCountReq
 *
 *      Desc:  Portable UdxCountReq primitive
 *
 *      Ret:   ROK 
 *
 *      Notes: 
 *
 *      File:  nh_ptli.c
 *
 *************************************************************************/
#ifdef ANSI
PRIVATE S16 PtUlUdxCountReq
(
Pst       *post,                /* post structure */
SpId      spId,                 /* service provider */
UdxCountReqInfo *countReq       /* PDCP Security Config paramters */
)
#else /* ANSI */
PRIVATE S16 PtUlUdxCountReq(post, spId, countReq)
Pst       *post;                /* post structure */
SpId       spId;                /* service provider */
UdxCountReqInfo *countReq;      /* Count value paramters */
#endif /* ANSI */
{
  TRC3(PtUlUdxCountReq);

  UNUSED(post);
  UNUSED(spId);
  UNUSED(countReq);

  RETVALUE(ROK);

} /* end of function PtUlUdxCountReq */

/**************************************************************************
 *
 *      Fun:   PtUlUdxSduStaReq
 *
 *      Desc:  Portable UdxSduStaReq primitive
 *
 *      Ret:   ROK 
 *
 *      Notes: 
 *
 *      File:  nh_ptli.c
 *
 *************************************************************************/
#ifdef ANSI
PRIVATE S16 PtUlUdxSduStaReq
(
Pst          *post,               /* post structure */
SpId         spId,                /* service provider */
UdxSduStaReqInfo *sduStaReq       /* PDCP SDU status parameters */
)
#else /* ANSI */
PRIVATE S16 PtUlUdxSduStaReq(post, spId, sduStaReq)
Pst          *post;               /* post structure */
SpId         spId;                /* service provider */
UdxSduStaReqInfo *sduStaReq;      /* PDCP SDU status parameters */
#endif /* ANSI */
{
  TRC3(PtUlUdxSduStaReq);

  UNUSED(post);
  UNUSED(spId);
  UNUSED(sduStaReq);


  RETVALUE(ROK);

} /* end of function PtUlUdxSduStaReq */


/**************************************************************************
 *
 *      Fun:   PtUlUdxReEstReq
 *
 *      Desc:  Portable UdxReEstReq primitive
 *
 *      Ret:   ROK 
 *
 *      Notes: 
 *
 *      File:  nh_ptli.c
 *
 *************************************************************************/
#ifdef ANSI
PRIVATE S16 PtUlUdxReEstReq
(
Pst       *post,                 /* post structure */
SpId      spId,                  /* service provider */
UdxReEstReqInfo *restbInfo       /* PDCP Re-establishment Config paramters */
)
#else /* ANSI */
PRIVATE S16 PtUlUdxReEstReq(post, spId, restbInfo)
Pst       *post;                 /* post structure */
SpId       spId;                 /* service provider */
UdxReEstReqInfo *restbInfo;      /* PDCP Re-establishment Config paramters */
#endif /* ANSI */
{
  TRC3(PtUlUdxReEstReq);

  UNUSED(post);
  UNUSED(spId);
  UNUSED(restbInfo);

  RETVALUE(ROK);

} /* end of function PtUlUdxReEstReq */
/**************************************************************************
 *
 *      Fun:   PtUlUdxUlFdbkPktInfo
 *
 *      Desc:  Portable PtUlUdxUlFdbkPktInfo primitive
 *
 *      Ret:   ROK
 *
 *      Notes:
  *
 *      File :  nh_ptli.c
 *
 *************************************************************************/
#ifdef ANSI
PRIVATE S16 PtUlUdxUlFdbkPktInfo
(
Pst          *post,               /* post structure */
SpId         spId,                /* service provider */
UdxUlFdbkPktInfo  *fdbkPktInfo    /* feedback Packet Info */
)
#else /* ANSI */
PRIVATE S16 PtUlUdxUlFdbkPktInfo(post, spId, fdbkPktInfo)
Pst          *post;               /* post structure */
SpId         spId;                /* service provider */
UdxUlFdbkPktInfo  *fdbkPktInfo    /* feedback Packet Info */
#endif /* ANSI */
{
    TRC3(PtUlUdxUlFdbkPktInfo);

    UNUSED(post);
    UNUSED(spId);
    UNUSED(fdbkPktInfo);


    RETVALUE(ROK);

} /* end of function PtUlUdxUlFdbkPktInfo */


/**************************************************************************
 *
 *      Fun:   PtUlUdxDlFdbkPktInfo
 *
 *      Desc:  Portable PtUlUdxDlFdbkPktInfo primitive
 *
 *      Ret:   ROK
 *
 *      Notes:
  *
 *      File :  nh_ptli.c
 *
 *************************************************************************/
#ifdef ANSI
PRIVATE S16 PtUlUdxDlFdbkPktInfo
(
Pst          *post,               /* post structure */
SpId         spId,                /* service provider */
UdxDlFdbkPktInfo  *fdbkPktInfo    /* feedback Packet Info */
)
#else /* ANSI */
PRIVATE S16 PtUlUdxDlFdbkPktInfo(post, spId, fdbkPktInfo)
Pst          *post;               /* post structure */
SpId         spId;                /* service provider */
UdxDlFdbkPktInfo  *fdbkPktInfo    /* feedback Packet Info */
#endif /* ANSI */
{
    TRC3(PtUlUdxDlFdbkPktInfo);

    UNUSED(post);
    UNUSED(spId);
    UNUSED(fdbkPktInfo);


    RETVALUE(ROK);

} /* end of function PtUlUdxDlFdbkPktInfo */




#ifdef LTE_L2_MEAS
/**
* @brief Request from ULM to DLM for Measurement
*
* @details
*
*     Function : PjUdxUdxL2MeasReq
*
*  @param[in]   Pst*           pst

*  @return   S16
*      -# ROK
**/
#ifdef ANSI
PRIVATE S16  PtUlUdxL2MeasReq
(
Pst*                pst,
PjL2MeasReqEvt      *measReqEvt
)
#else
PRIVATE S16 PtUlUdxL2MeasReq(pst, measReqEvt )
Pst*                pst;
PjL2MeasReqEvt      *measReqEvt;
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
*     Function : PjUdxUdxL2MeasSendReq
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
*     Function : PjUdxUdxL2MeasStopReq
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
/**
* @brief Request from ULM to DLM for ResumeReq 
*
* @details
*
*     Function : PtUlUdxDatResumeReq
*
*  @param[in]   Pst*           pst

*  @return   S16
*      -# ROK
**/
#ifdef ANSI
PRIVATE S16  PtUlUdxDatResumeReq
(
Pst*          pst,
UdxDatResumeInfo  *udxDatResumeInfo
)
#else
PRIVATE S16 PtUlUdxDatResumeReq(pst, udxDatResumeInfo )
Pst*          pst;
UdxDatResumeInfo  *udxDatResumeInfo;
#endif
{

   TRC3(PtUlUdxDatResumeReq);

   RETVALUE(ROK);
}

/**
* @brief  
*
* @details
*
*     Function : PtUlUdxDatForwardReq
*
*  @param[in]   Pst*           pst

*  @return   S16
*      -# ROK
**/
#ifdef ANSI
PRIVATE S16  PtUlUdxDatForwardReq
(
Pst*          pst,
UdxDatForwardInfo  *udxDatForwardInfo
)
#else
PRIVATE S16 PtUlUdxDatForwardReq(pst, udxDatForwardInfo )
Pst*          pst;
UdxDatForwardInfo  *udxDatForwardInfo;
#endif
{

   TRC3(PtUlUdxDatForwardReq);

   RETVALUE(ROK);
}

#endif /* #ifdef PTPJULUDX */

#ifdef __cplusplus
}
#endif /* __cplusplus */
/********************************************************************30**
         End of file
**********************************************************************/
