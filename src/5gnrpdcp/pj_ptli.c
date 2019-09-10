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
  
        Name:    RLC service user (PDCP) lower interface
    
        Type:    C file
  
        Desc:    This file Contains the RLC service user lower interface
                  primitive implementain
 
        File:    pj_ptli.c
  
*********************************************************************21*/

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
#include "kwu.h"           /* KWU defines */
#include "pj_err.h"        

/* extern (.x) include files */
#include "gen.x"           /* general */
#include "ssi.x"           /* system services */
//#if defined(PDCP_RLC_DL_RBUF)
#include "ss_rbuf.h"
#include "ss_rbuf.x"
#include "kwu.h"                /* PJU */
#include "ss_queue.h"
#include "ss_queue.x"
#include "ss_task.h"
#include "ss_task.x"
#include "ss_timer.x"           /* System services */
#include "ss_msg.h"           /* System services */
#include "ss_msg.x"           /* System services */
#include "ss_mem.h"           /* System services */
#include "ss_mem.x"           /* System services */
#include "ss_drvr.x"
#include "ss_gen.h"
#include "ss_gen.x"
//#endif

#include "cm5.x"           /* common timer library */
#include "cm_tkns.x"       /* common tokens */
#include "cm_mblk.x"       /* common memory allocation */
#include "cm_llist.x"      /* common link list */
#include "cm_hash.x"       /* common hash list */
#include "cm_lte.x"        /* common LTE includes */
#include "cm_lib.x"        /* common memory allocation library */
#include "kwu.x"           /* KWU */


PUBLIC U8 SUpdMbufMemInfo (Pst    *pst,Buffer *mBuf);
#if defined(PDCP_RLC_DL_RBUF) && !defined(SS_RBUF)
PUBLIC S16 pjKwuRbufDatReq(Pst * pst,SpId spId,KwuDatReqInfo* datReq,Buffer * mBuf);
#endif

EXTERN S16 PtLiKwuBndReq ARGS ((
Pst         *post,
SuId        suId,
SpId        spId
));

EXTERN S16 PtLiKwuUbndReq ARGS ((
Pst         *post,
SpId        spId,
Reason      reason
));

EXTERN S16 PtLiKwuDiscSduReq ARGS ((
Pst               *post,
SpId              spId,
KwuDiscSduInfo    *discSdu
));

EXTERN S16 PtLiKwuDatReq ARGS ((
Pst            *post,
SpId           spId,
KwuDatReqInfo  *datReq,
Buffer         *mBuf
));

/************************************************************************
                             KWU Interface Matrices
************************************************************************/

PUBLIC KwuBndReq pjLiKwuBndReqMt [] =
{
#ifdef LCKWUIKWU
   cmPkKwuBndReq,        /* 0 - loosely coupled */
#else
   PtLiKwuBndReq,        /* 0 - loosely coupled, portable */
#endif
#ifdef KW
   KwUiKwuBndReq,        /* 1 - tightly coupled, stub layer */
#else
   PtLiKwuBndReq,        /* 1 - tightly coupled, portable */
#endif
#ifdef LWLCKWUIKWU
   cmPkKwuBndReq,        /* 3 - light weight loosely coupled */
#else
   PtLiKwuBndReq,        /* 3 - light weight loosely coupled, portable */
#endif
};

PUBLIC KwuUbndReq pjLiKwuUbndReqMt [] =
{
#ifdef LCKWUIKWU 
   cmPkKwuUbndReq,        /* 0 - loosely coupled */
#else
   PtLiKwuUbndReq,        /* 0 - loosely coupled, portable */
#endif
#ifdef KW
   KwUiKwuUbndReq,        /* 1 - tightly coupled, stub layer */
#else
   PtLiKwuUbndReq,        /* 1 - tightly coupled, portable */
#endif
#ifdef LWLCKWUIKWU 
   cmPkKwuUbndReq,        /* 3 - light weight loosely coupled */
#else
   PtLiKwuUbndReq,        /* 3 - light weight loosely coupled, portable */
#endif
};

PUBLIC KwuDatReq pjLiKwuDatReqMt [] =
{
#ifdef LCKWUIKWU
   cmPkKwuDatReq,        /* 0 - loosely coupled */
#else
   PtLiKwuDatReq,        /* 0 - loosely coupled, portable */
#endif
#ifdef KW
   KwUiKwuDatReq,        /* 1 - tightly coupled, stub layer */
#else
   PtLiKwuDatReq,        /* 1 - tightly coupled, portable */
#endif
#ifdef LWLCKWUIKWU
   cmPkKwuDatReq,        /* 3 - light weight loosely coupled */
#else
   PtLiKwuDatReq,        /* 3 - light weight loosely coupled, portable */
#endif
};

PUBLIC KwuDiscSduReq pjLiKwuDiscSduReqMt [] =
{
#ifdef LCKWUIKWU
   cmPkKwuDiscSduReq,        /* 0 - loosely coupled */
#else
   PtLiKwuDiscSduReq,        /* 0 - loosely coupled, portable */
#endif
#ifdef KW
   KwUiKwuDiscSduReq,        /* 1 - tightly coupled, stub layer */
#else
   PtLiKwuDiscSduReq,        /* 1 - tightly coupled, portable */
#endif
#ifdef LWLCKWUIKWU
   cmPkKwuDiscSduReq,        /* 3 - light weight loosely coupled */
#else
   PtLiKwuDiscSduReq,        /* 3 - light weight loosely coupled, portable */
#endif
};


/****************************************************************************
 *                         KWU Interface Mt functions
 ***************************************************************************/
/**
 *
 * @brief 
 *
 *        PjLiKwuBndReq - KWU SAP bind Request
 *
 *  @param[in] pst   - Post structure  
 *  @param[in] suId  - Service user SAP ID 
 *  @param[in] spId  - Service provider ID
 *
 *  @return  S16
 *      -# ROK 
 */
#ifdef ANSI
PUBLIC S16 PjLiKwuBndReq
(
Pst *pst,                       /* post structure */
SuId suId,                      /* Service User Id */
SpId spId                       /* Service Provider Id */
)
#else
PUBLIC S16 PjLiKwuBndReq(pst, suId, spId)
Pst *pst;                       /* post structure */
SuId suId;                      /* Service User Id */
SpId spId;                      /* Service Provider Id */
#endif
{
   TRC3(PjLiKwuBndReq)

   /* jump to specific primitive depending on configured selector */
   (*pjLiKwuBndReqMt[pst->selector])(pst, suId, spId);

   RETVALUE(ROK);

} /* end of PjLiKwuBndReq */


/**
 *
 * @brief 
 *
 *        PjLiKwuUbndReq - KWU SAP unbind Request
 *
 *  @param[in] pst     - Post structure  
 *  @param[in] spId    - Service provider SAP ID 
 *  @param[in] reason  - Reason
 *
 *  @return  S16
 *      -# ROK 
 */
#ifdef ANSI
PUBLIC S16 PjLiKwuUbndReq
(
Pst         *pst,
SpId        spId,
Reason      reason
)
#else
PUBLIC S16 PjLiKwuUbndReq(pst, spId, reason)
Pst         *pst;
SpId        spId;
Reason      reason;
#endif
{
   TRC3(PjLiKwuUbndReq)

   /* jump to specific primitive depending on configured selector */
   (*pjLiKwuUbndReqMt[pst->selector])(pst, spId, reason);

   RETVALUE(ROK);

} /* end of PjLiKwuUbndReq */


/**
 *
 * @brief 
 *
 *        PjLiKwuDatReq - KWU Data Request
 *
 *  @param[in] pst     - Post structure  
 *  @param[in] spId    - Service provider SAP ID 
 *  @param[in] datReq  - Data Request
 *
 *  @return  S16
 *      -# ROK 
 */
#ifdef ANSI
PUBLIC S16 PjLiKwuDatReq
(
Pst            *pst,
SpId           spId,
KwuDatReqInfo  *datReq,
Buffer         *mBuf
)
#else
PUBLIC S16 PjLiKwuDatReq(pst, spId, datReq, mBuf)
Pst            *pst;
SpId           spId;
KwuDatReqInfo  *datReq;
Buffer         *mBuf;
#endif
{
   S16 ret = ROK;

   TRC3(PjLiKwuDatReq)
#if defined(PDCP_RLC_DL_RBUF) && !defined(SS_RBUF)
   if(datReq->lcType == CM_LTE_LCH_DTCH)
   {
      if((pjKwuRbufDatReq(pst,spId, datReq, mBuf)) != ROK)
      {
         /* Posting the message as ring buffer write failure */
         ret = RFAILED;
      }
   }
   else
#endif
   {   
      /* jump to specific primitive depending on configured selector */
      ret = (*pjLiKwuDatReqMt[pst->selector])(pst, spId, datReq, mBuf);
   }
   RETVALUE(ret);

} /* end of PjLiKwuDatReq */


/**
 *
 * @brief 
 *
 *        PjLiKwuDiscSduReq - KWU Discard SDU Request
 *
 *  @param[in] pst     - Post structure  
 *  @param[in] spId    - Service provider SAP ID 
 *  @param[in] datReq  - Data Request
 *
 *  @return  S16
 *      -# ROK 
 */
#ifdef ANSI
PUBLIC S16 PjLiKwuDiscSduReq
(
Pst               *pst,
SpId              spId,
KwuDiscSduInfo    *discSdu
)
#else
PUBLIC S16 PjLiKwuDiscSduReq(pst, spId, discSdu)
Pst               *pst;
SpId              spId;
KwuDiscSduInfo    *discSdu;
#endif
{
   TRC3(PjLiKwuDiscSduReq)

   /* jump to specific primitive depending on configured selector */
   (*pjLiKwuDiscSduReqMt[pst->selector])(pst, spId, discSdu);

   RETVALUE(ROK);

} /* end of PjLiKwuDiscSduReq */

/****************************************************************************
 *                         Porting Functions
 ***************************************************************************/
/**
 *
 * @brief 
 *
 *        PtLiKwuBndReq - portable bind request
 *
 *  @param[in] pst   - Post structure  
 *  @param[in] suId  - Service user SAP ID 
 *  @param[in] spId  - Service provider ID
 *
 *  @return  S16
 *      -# ROK 
 */

#ifdef ANSI
PUBLIC S16 PtLiKwuBndReq
(
Pst         *post,
SuId        suId,
SpId        spId
)
#else
PUBLIC S16 PtLiKwuBndReq(post, suId, spId)
Pst         *post;
SuId        suId;
SpId        spId;
#endif /* ANSI */
{
   TRC3(PtLiKwuBndReq)
#if (ERRCLASS & ERRCLS_DEBUG)
 /*  S16 ret1;    */
   SLogError(post->srcEnt, post->srcInst, post->srcProcId,
        __FILE__, __LINE__, (ErrCls)ERRCLS_DEBUG,
         (ErrVal)EPJXXX, (ErrVal)ERRZERO, "Improper selector for NhLiKwuBndReq\n");
   RETVALUE( RFAILED );
#endif /* (ERRCLASS & ERRCLS_DEBUG) */
   UNUSED(post);
   UNUSED(suId);
   UNUSED(spId);

   RETVALUE(ROK);
} /* end of PtLiKwuBndReq() */

  
/**
 *
 * @brief 
 *
 *        PtLiKwuUbndReq - portable unbind request
 *
 *  @param[in] pst     - Post structure  
 *  @param[in] suId    - Service provider SAP ID 
 *  @param[in] reason  - Reason
 *
 *  @return  S16
 *      -# ROK 
 */
#ifdef ANSI
PUBLIC S16 PtLiKwuUbndReq
(
Pst         *post,
SpId        spId,
Reason      reason
)
#else
PUBLIC S16 PtLiKwuUbndReq(post, spId, reason)
Pst         *post;
SpId        spId;
Reason      reason;
#endif /* ANSI */
{
   TRC3(PtLiKwuUbndReq)
#if (ERRCLASS & ERRCLS_DEBUG)
  /* S16 ret1; */
   SLogError(post->srcEnt, post->srcInst, post->srcProcId,
        __FILE__, __LINE__, (ErrCls)ERRCLS_DEBUG,
         (ErrVal)EPJXXX, (ErrVal)ERRZERO, "Improper selector for NhLiKwuUbndReq\n");
   RETVALUE( RFAILED );
#endif /* (ERRCLASS & ERRCLS_DEBUG) */
   UNUSED(post);
   UNUSED(spId);
   UNUSED(reason);
   
   RETVALUE(ROK);
} /* end of PtLiKwuUbndReq() */

/**
 *
 * @brief 
 *
 *        PtLiKwuDiscSduReq - portable discard SDU request
 *
 *  @param[in] pst     - Post structure  
 *  @param[in] spId    - Service provider SAP ID 
 *  @param[in] datReq  - Data Request
 *
 *  @return  S16
 *      -# ROK 
 */
#ifdef ANSI
PUBLIC S16 PtLiKwuDiscSduReq
(
Pst               *post,
SpId              spId,
KwuDiscSduInfo    *discSdu
)
#else
PUBLIC S16 PtLiKwuDiscSduReq(post, spId, discSdu)
Pst               *post;
SpId              spId;
KwuDiscSduInfo    *discSdu;
#endif /* ANSI */
{
   TRC3(PtLiKwuDiscSduReq)
#if (ERRCLASS & ERRCLS_DEBUG)
  /* S16 ret1; */
   SLogError(post->srcEnt, post->srcInst, post->srcProcId,
        __FILE__, __LINE__, (ErrCls)ERRCLS_DEBUG,
         (ErrVal)EKWU010, (ErrVal)ERRZERO, "Improper selector for NhLiKwuDiscSduReq\n");
   RETVALUE( RFAILED );

#endif /* (ERRCLASS & ERRCLS_DEBUG) */
   UNUSED(post);
   UNUSED(spId);
   UNUSED(discSdu);

   RETVALUE(ROK);
} /* end of PtLiKwuDiscSduReq() */

/**
 *
 * @brief 
 *
 *        PtLiKwuDatReq - portable data request
 *
 *  @param[in] pst     - Post structure  
 *  @param[in] spId    - Service provider SAP ID 
 *  @param[in] datReq  - Data Request
 *
 *  @return  S16
 *      -# ROK 
 */
#ifdef ANSI
PUBLIC S16 PtLiKwuDatReq
(
Pst            *pst,
SpId           spId,
KwuDatReqInfo  *datReq,
Buffer         *mBuf
)
#else
PUBLIC S16 PtLiKwuDatReq(pst, spId, datReq, mBuf)
Pst            *pst;
SpId           spId;
KwuDatReqInfo  *datReq;
Buffer         *mBuf;
#endif /* ANSI */
{
   TRC3(PtLiKwuDatReq)
#if (ERRCLASS & ERRCLS_DEBUG)
  /* S16 ret1; */
   SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
        __FILE__, __LINE__, (ErrCls)ERRCLS_DEBUG,
         (ErrVal)EPJXXX, (ErrVal)ERRZERO, "Improper selector for NhLiKwuDatReq\n");
   RETVALUE( RFAILED );
#endif /* (ERRCLASS & ERRCLS_DEBUG) */
   UNUSED(pst);
   UNUSED(spId);
   UNUSED(datReq);
   UNUSED(mBuf);

   RETVALUE(ROK);
} /* end of PtLiKwuDatReq() */
/*
*
*       Fun:   SUpdMbufMemInfo
*
*       Desc:  This function is used to get the mem region info of a task.
*
*       Ret:   ROK      - ok
*              RFAILED  - failed, general (optional)
*
*
*       File:  ss_task.c
*
*/
#ifdef TENB_T2K3K_SPECIFIC_CHANGES
#ifdef ANSI
PUBLIC U8 SUpdMbufMemInfo 
(
Pst    *pst,
Buffer *mBuf
)
#else
PUBLIC U8 SUpdMbufMemInfo(pst,mBuf)
Pst    *pst;
Buffer *mBuf;
#endif
{
   SsMsgInfo *minfo;
   SsTTskEntry *tTsk;
   SsIdx       dstIdx;


   minfo = (SsMsgInfo*) mBuf->b_rptr;

   dstIdx = osCp.tTskIds[pst->dstEnt][pst->dstInst];
   tTsk = &osCp.tTskTbl[dstIdx];
#ifdef SS_MULTICORE_SUPPORT
   minfo->region = tTsk->sTsk->region;
#endif /* SS_MULTICORE_SUPPORT */
   RETVALUE(ROK);
}
#endif

#if defined(PDCP_RLC_DL_RBUF) && !defined(SS_RBUF)
#ifdef ANSI
PUBLIC S16 pjKwuRbufDatReq
(
Pst * pst,
SpId spId,
KwuDatReqInfo* datReq,
Buffer * mBuf
)
#else
PUBLIC S16 pjKwuRbufDatReq(pst, spId, datReq, mBuf)
Pst * pst;
SpId spId;
KwuDatReqInfo* datReq;
Buffer * mBuf;
#endif
{
   S16 ret1 = ROK;
   
   Void *elem = NULLP;
   KwuDatReqDetl *kwuDatReqDetl = NULLP;

   elem = SRngGetWIndx(SS_RNG_BUF_DLPDCP_TO_DLRLC);
   if (NULLP != elem)
   {
      kwuDatReqDetl = (KwuDatReqDetl *) elem;
      kwuDatReqDetl->spId = spId;
      kwuDatReqDetl->lcType = datReq->lcType;
      kwuDatReqDetl->sduId = datReq->sduId;
      kwuDatReqDetl->rlcId = datReq->rlcId;
      kwuDatReqDetl->mBuf = mBuf;
      /* Commenting below function because memory region mapped with threadId*/
      /* SUpdMbufMemInfo(pst, mBuf);*/
      SRngIncrWIndx(SS_RNG_BUF_DLPDCP_TO_DLRLC);
      SsRngInfoTbl[SS_RNG_BUF_DLPDCP_TO_DLRLC].pktRate++;
   }   
   else
   {
#if (ERRCLASS & ERRCLS_DEBUG)
       SLogError(ENTPJ, 0, SFndProcId(),
               __FILE__, __LINE__, (ErrCls)ERRCLS_DEBUG,
               (ErrVal)EPJXXX, (ErrVal)ERRZERO, "PDCP-DL RBUF is FULL!!!\n");
#endif /* (ERRCLASS & ERRCLS_DEBUG) */
      SsRngInfoTbl[SS_RNG_BUF_DLPDCP_TO_DLRLC].pktDrop++;
      ret1 = RFAILED;
   }
   RETVALUE(ret1);
} /* cmPkKwuDatReq */

#endif /* PDCP_RLC_DL_RBUF */

  
/********************************************************************30**
         End of file
**********************************************************************/
