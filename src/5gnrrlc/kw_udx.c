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
  
        Name:    UDX RRC Control Interface
    
        Type:    C file
  
        Desc:    This file Contains the packing and unpacking functions for
                 UDX Interface
 
        File:    kw_udx.c
  
*********************************************************************21*/
#include "common_def.h"
#include "lkw.h"
#include "kw_udx.h"           /* UDX defines */
#include "kw.h"

/* extern (.x) include files */
#include "lkw.x"
#include "kw_udx.x"


#ifdef LCUDX

/*
*
*    Fun:    cmPkUdxBndReq
*
*    Desc:    pack the primitive UdxBndReq
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     ckw.c
*
*/
#ifdef ANSI
PRIVATE S16 cmUnpkUdxStruct
(
Buffer *srcMBuf,
U32  offset,
U8     *dstBuf,
U32    size
)
#else
PRIVATE S16 cmUnpkUdxStruct(dstMBuf,srcBuf,size)
Buffer *srcMBuf;
U32  offset;
U8     *dstBuf;
MsgLen    size;
#endif
{
    MsgLen tmpLen;

    return (SCpyMsgFix(srcMBuf,offset,size,dstBuf,&tmpLen));

} /*end of function cmPkUdxBndReq*/

/*
*
*    Fun:    cmPkUdxBndReq
*
*    Desc:    pack the primitive UdxBndReq
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     ckw.c
*
*/
#ifdef ANSI
PRIVATE S16 cmPkUdxStruct
(
U8     *srcBuf,
MsgLen    size,
Buffer *dstMBuf
)
#else
PRIVATE S16 cmPkUdxStruct(dstMBuf,srcBuf,size)
U8     *srcBuf;
MsgLen    size;
Buffer *dstMBuf;
#endif
{

    return (SAddPstMsgMult(srcBuf,size,dstMBuf));

} /*end of function cmPkUdxBndReq*/
/*
*
*    Fun:    cmPkUdxBndReq
*
*    Desc:    pack the primitive UdxBndReq
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     ckw.c
*
*/
#ifdef ANSI
S16 cmPkUdxBndReq
(
Pst *pst,
SuId suId,
SpId spId
)
#else
S16 cmPkUdxBndReq(pst, suId, spId)
Pst *pst;
SuId suId;
SpId spId;
#endif
{
    S16 ret1;
    Buffer *mBuf;
    mBuf = NULLP;

    if((ret1 = SGetMsg(pst->region, pst->pool, &mBuf)) != ROK)
    {
#if (ERRCLASS & ERRCLS_ADD_RES)
       if(ret1 != ROK)
       {
          SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
               (ErrVal)EUDXXXX, (ErrVal)0, "SGetMsg() failed");
       }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
       return (ret1);
    }
    CMCHKPKLOG(SPkS16, spId, mBuf, EUDXXXX, pst);
    CMCHKPKLOG(SPkS16, suId, mBuf, EUDXXXX, pst);
    pst->event = (Event) UDX_EVT_BND_REQ;

    return (SPstTsk(pst,mBuf));
} /*end of function cmPkUdxBndReq*/

/*
*
*    Fun:    cmPkUdxUbndReq
*
*    Desc:    pack the primitive UdxUbndReq
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     ckw.c
*
*/
#ifdef ANSI
S16 cmPkUdxUbndReq
(
Pst *pst,
SpId spId,
Reason reason
)
#else
S16 cmPkUdxUbndReq(pst, spId, reason)
Pst *pst;
SpId spId;
Reason reason;
#endif
{
    S16 ret1;
    Buffer *mBuf;
    mBuf = NULLP;

    if((ret1 = SGetMsg(pst->region, pst->pool, &mBuf)) != ROK)
    {
#if (ERRCLASS & ERRCLS_ADD_RES)
       if(ret1 != ROK)
       {
          SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
               (ErrVal)EUDXXXX, (ErrVal)0, "SGetMsg() failed");
       }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
       return (ret1);
    }
    CMCHKPKLOG(SPkS16, reason, mBuf, EUDXXXX, pst);
    CMCHKPKLOG(SPkS16, spId, mBuf, EUDXXXX, pst);
    pst->event = (Event) UDX_EVT_UBND_REQ;

    return (SPstTsk(pst,mBuf));
} /*end of function cmPkUdxUbndReq*/

/*
*
*    Fun:    cmPkUdxBndCfm
*
*    Desc:    pack the primitive UdxBndCfm
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     ckw.c
*
*/
#ifdef ANSI
S16 cmPkUdxBndCfm
(
Pst *pst,
SuId suId,
U8 status
)
#else
S16 cmPkUdxBndCfm(pst, suId, status)
Pst *pst;
SuId suId;
U8 status;
#endif
{
    S16 ret1;
    Buffer *mBuf;
    mBuf = NULLP;

    if((ret1 = SGetMsg(pst->region, pst->pool, &mBuf)) != ROK)
    {
#if (ERRCLASS & ERRCLS_ADD_RES)
       if(ret1 != ROK)
       {
          SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
               (ErrVal)EUDXXXX, (ErrVal)0, "SGetMsg() failed");
       }

#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
       return (ret1);
    }
    CMCHKPKLOG(oduUnpackUInt8, status, mBuf, EUDXXXX, pst);
    CMCHKPKLOG(SPkS16, suId, mBuf, EUDXXXX, pst);
    pst->event = (Event) UDX_EVT_BND_CFM;

    return (SPstTsk(pst,mBuf));
} /*end of function cmPkUdxBndCfm*/


/*
*
*    Fun:    cmPkUdxCfgReq
*
*    Desc:    pack the primitive KwUiUdxCfgReq
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     ckw.c
*
*/
#ifdef ANSI
S16 cmPkUdxCfgReq
(
Pst               *pst,
SpId              spId,
RlcCfgInfo        *cfgInfo
)
#else
S16 cmPkUdxCfgReq(pst, spId, cfgInfo)
Pst               *pst;
SpId              spId;
RlcCfgInfo        *cfgInfo;
#endif
{
    S16 ret1;
    Buffer *mBuf;
    mBuf = NULLP;

    if((ret1 = SGetMsg(pst->region, pst->pool, &mBuf)) != ROK)
    {
#if (ERRCLASS & ERRCLS_ADD_RES)
       if(ret1 != ROK)
       {
          SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
               (ErrVal)EUDXXXX, (ErrVal)0, "SGetMsg() failed");
       }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
       return (ret1);
    }

    switch(pst->selector)
    {
#ifdef LCUDX
       case UDX_SEL_LC:
          {
             cmPkUdxStruct((U8 *)cfgInfo, sizeof(RlcCfgInfo),mBuf);
             /* Need Not free CfgInfo here as it is stored 
                in UL */
             break;
          }
        case UDX_SEL_LWLC:
          {
             CMCHKPKLOG(oduPackPointer,(PTR)cfgInfo,mBuf,EUDXXXX,pst);
             break;
          }
#endif /* LCUDX */
    }

    CMCHKPKLOG(SPkS16, spId, mBuf, EUDXXXX, pst);
    pst->event = (Event) UDX_EVT_CFG_REQ;

    return (SPstTsk(pst,mBuf));
} /* cmPkUdxCfgReq */


/*
*
*    Fun:    cmPkUdxCfgCfm
*
*    Desc:    pack the primitive KwUiUdxCfgCfm
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     ckw.c
*
*/
#ifdef ANSI
S16 cmPkUdxCfgCfm
(
Pst               *pst,
SuId              suId,
RlcCfgCfmInfo     *cfgCfmInfo
)
#else
S16 cmPkUdxCfgCfm(pst, suId, cfgCfmInfo)
Pst               *pst;
SuId              suId;
RlcCfgCfmInfo     *cfgCfmInfo;
#endif
{
    S16 ret1;
    Buffer *mBuf;
    mBuf = NULLP;

    if((ret1 = SGetMsg(pst->region, pst->pool, &mBuf)) != ROK)
    {
#if (ERRCLASS & ERRCLS_ADD_RES)
       if(ret1 != ROK)
       {
          SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
               (ErrVal)EUDXXXX, (ErrVal)0, "SGetMsg() failed");
       }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
       return (ret1);
    }

    switch(pst->selector)
    {
#ifdef LCUDX
       case UDX_SEL_LC:
          {
             cmPkUdxStruct((U8 *)cfgCfmInfo, sizeof(RlcCfgCfmInfo),mBuf);
             /* Need to free the cfgCfmInfo here as it is allocated 
                                        buffer call SPutStaticBuffer */
             SPutStaticBuffer(pst->region,pst->pool,(Data *) cfgCfmInfo,
                          sizeof(RlcCfgCfmInfo),0);
             break;
          }
        case UDX_SEL_LWLC:
          {
             CMCHKPKLOG(oduPackPointer,(PTR)cfgCfmInfo,mBuf,EUDXXXX,pst);
             break;
          }
#endif /* LCUDX */
    }

    CMCHKPKLOG(SPkS16, suId, mBuf, EUDXXXX, pst);
    pst->event = (Event) UDX_EVT_CFG_CFM;

    return (SPstTsk(pst,mBuf));
} /* cmPkUdxCfgCfm */


/*
*
*    Fun:    cmPkUdxUeIdChgReq
*
*    Desc:   pack the primitive KwUiUdxUeIdChgReq
*
*    Ret:    ROK  -ok
*
*    Notes:  None
*
*    File:   ckw.c
*
*/
#ifdef ANSI
S16 cmPkUdxUeIdChgReq
(
Pst               *pst,
SpId              spId,
U32               transId,
CkwUeInfo         *ueInfo,
CkwUeInfo         *newUeInfo
)
#else
S16 cmPkUdxUeIdChgReq(pst, spId, transId, ueInfo, newUeInfo)
Pst               *pst;
SpId              spId;
U32               transId;
CkwUeInfo         *ueInfo;
CkwUeInfo         *newUeInfo;
#endif
{
    S16    ret1;
    Buffer *mBuf = NULLP;


    if((ret1 = SGetMsg(pst->region, pst->pool, &mBuf)) != ROK)
    {
#if (ERRCLASS & ERRCLS_ADD_RES)
       if(ret1 != ROK)
       {
          SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
               (ErrVal)EUDXXXX, (ErrVal)0, "SGetMsg() failed");
       }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */

       return (ret1);
    }

    switch(pst->selector)
    {
#ifdef LCUDX
       case UDX_SEL_LC:
       {
            cmPkUdxStruct((U8 *)newUeInfo, sizeof(CkwUeInfo),mBuf);
            cmPkUdxStruct((U8 *)ueInfo, sizeof(CkwUeInfo),mBuf);
            /* No need to free ueInfo here as it is stored */
            break;
       }
       case UDX_SEL_LWLC:
          {
             CMCHKPKLOG(oduPackPointer,(PTR)newUeInfo,mBuf,EUDXXXX,pst);
             CMCHKPKLOG(oduPackPointer,(PTR)ueInfo,mBuf,EUDXXXX,pst);
             break;
          }
       default:
       {
          printf("cmPkUdxUeIdChgReq()- selector =%d not supported \n", pst->selector);
          exit(1);
       }
#endif
    }
    CMCHKPKLOG(oduUnpackUInt32, transId, mBuf, EUDXXXX, pst);
    CMCHKPKLOG(SPkS16, spId, mBuf, EUDXXXX, pst);
    pst->event = (Event) UDX_EVT_UEIDCHG_REQ;

    return (SPstTsk(pst, mBuf));

} /* cmPkUdxUeIdChgReq */

/*
*
*    Fun:    cmPkUdxUeIdChgCfm
*
*    Desc:   pack the primitive KwUiUdxUeIdChgCfm
*
*    Ret:    ROK  -ok
*
*    Notes:  None
*
*    File:   ckw.c
*
*/
#ifdef ANSI
S16 cmPkUdxUeIdChgCfm
(
Pst               *pst,
SuId              suId,
U32               transId,
CmStatus          status
)
#else
S16 cmPkUdxUeIdChgCfm(pst, suId, transId, status)
Pst               *pst;
SuId              suId;
U32               transId;
CmStatus          status;
#endif
{
    S16    ret1;
    Buffer *mBuf = NULLP;

    if((ret1 = SGetMsg(pst->region, pst->pool, &mBuf)) != ROK)
    {
#if (ERRCLASS & ERRCLS_ADD_RES)
       if(ret1 != ROK)
       {
          SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
               (ErrVal)EUDXXXX, (ErrVal)0, "SGetMsg() failed");
       }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */

       return (ret1);
    }
    
    CMCHKPK(cmPkCmStatus, &status, mBuf); 
    CMCHKPKLOG(oduUnpackUInt32, transId, mBuf, EUDXXXX, pst);
    CMCHKPKLOG(SPkS16, suId, mBuf, EUDXXXX, pst);
    pst->event = (Event) UDX_EVT_UEIDCHG_CFM;

    return (SPstTsk(pst, mBuf));

} /* cmPkUdxUeIdChgCfm */

/*
*
*    Fun:    cmPkUdxStaUpdCfm
*
*    Desc:   pack the primitive KwUiUdxUeIdChgCfm
*
*    Ret:    ROK  -ok
*
*    Notes:  None
*
*    File:   ckw.c
*
*/
#ifdef ANSI
S16 cmPkUdxStaUpdCfm
(
Pst               *pst,
SuId              suId,
CmLteRlcId        *rlcId,
RlcUdxBufLst       *pStaPdu
)
#else
S16 cmPkUdxStaUpdCfm(pst, suId, rlcId,pStaPdu)
Pst               *pst;
SuId              suId;
CmLteRlcId        *rlcId;
RlcUdxBufLst       *pStaPdu;
#endif
{
    S16    ret1;
    Buffer *mBuf = NULLP;

    if((ret1 = SGetMsg(pst->region, pst->pool, &mBuf)) != ROK)
    {
#if (ERRCLASS & ERRCLS_ADD_RES)
       if(ret1 != ROK)
       {
          SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
               (ErrVal)EUDXXXX, (ErrVal)0, "SGetMsg() failed");
       }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */

       return (ret1);
    }
    
    switch (pst->selector)
    {
       case UDX_SEL_LC:
       {
         cmPkUdxStruct((U8 *)pStaPdu, sizeof(RlcUdxBufLst),mBuf);
         cmPkUdxStruct((U8 *)rlcId, sizeof(CmLteRlcId),mBuf);
         break;
       }
       case UDX_SEL_LWLC:
       {
         CMCHKPK(oduPackPointer,(PTR) pStaPdu, mBuf);
         CMCHKPK(oduPackPointer,(PTR) rlcId, mBuf); 
         break;
       }
    }
    CMCHKPKLOG(SPkS16, suId, mBuf, EUDXXXX, pst);
    pst->event = (Event) UDX_EVT_STA_UPD_CFM;

    return (SPstTsk(pst, mBuf));

} /* cmPkUdxStaUpdCfm */

/*
*
*    Fun:    cmPkUdxStaProhTmrStart
*
*    Desc:   pack the primitive
*
*    Ret:    ROK  -ok
*
*    Notes:  None
*
*    File:   Kw_udx.c
*
*/
#ifdef ANSI
S16 cmPkUdxStaProhTmrStart
(
Pst               *pst,
SpId              spId,
CmLteRlcId        *rlcId
)
#else
S16 cmPkUdxStaProhTmrStart(pst, suId, rlcId)
Pst               *pst;
SpId              spId;
CmLteRlcId        *rlcId;
#endif
{
    S16    ret1;
    Buffer *mBuf = NULLP;

    if((ret1 = SGetMsg(pst->region, pst->pool, &mBuf)) != ROK)
    {
#if (ERRCLASS & ERRCLS_ADD_RES)
       if(ret1 != ROK)
       {
          SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
               (ErrVal)EUDXXXX, (ErrVal)0, "SGetMsg() failed");
       }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */

       return (ret1);
    }
    
    switch (pst->selector)
    {
       case UDX_SEL_LC:
       {
         cmPkUdxStruct((U8 *)rlcId, sizeof(CmLteRlcId),mBuf);
         break;
       }
       case UDX_SEL_LWLC:
       {
         CMCHKPK(oduPackPointer,(PTR) rlcId, mBuf); 
         break;
       }
    }
    CMCHKPKLOG(SPkS16, spId, mBuf, EUDXXXX, pst);
    pst->event = (Event) UDX_EVT_STA_PHBT_TMR_START;

    return (SPstTsk(pst, mBuf));

} /* cmPkUdxStaProhTmrStart */

/*
*
*    Fun:    cmPkUdxStaUpdReq
*
*    Desc:   pack the primitive KwUiUdxUeIdChgCfm
*
*    Ret:    ROK  -ok
*
*    Notes:  None
*
*    File:   ckw.c
*
*/
#ifdef ANSI
S16 cmPkUdxStaUpdReq
(
Pst               *pst,
SpId              spId,
CmLteRlcId        *rlcId,
RlcUdxStaPdu      *pStaPdu
)
#else
S16 cmPkUdxStaUpdReq(pst, suId, rlcId,pStaPdu)
Pst               *pst;
SpId              spId;
CmLteRlcId        *rlcId;
RlcUdxStaPdu       *pStaPdu;
#endif
{
    S16    ret1;
    Buffer *mBuf = NULLP;

    if((ret1 = SGetMsg(pst->region, pst->pool, &mBuf)) != ROK)
    {
#if (ERRCLASS & ERRCLS_ADD_RES)
       if(ret1 != ROK)
       {
          SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
               (ErrVal)EUDXXXX, (ErrVal)0, "SGetMsg() failed");
       }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */

       return (ret1);
    }
    
    switch (pst->selector)
    {
       case UDX_SEL_LC:
       {
         cmPkUdxStruct((U8 *)pStaPdu, sizeof(RlcUdxStaPdu),mBuf);
         cmPkUdxStruct((U8 *)rlcId, sizeof(CmLteRlcId),mBuf);
         SPutStaticBuffer(pst->region,pst->pool,(Data *) pStaPdu,
                          sizeof(RlcUdxStaPdu),0);

         break;
       }
       case UDX_SEL_LWLC:
       {
         CMCHKPK(oduPackPointer,(PTR) pStaPdu, mBuf);
         CMCHKPK(oduPackPointer,(PTR) rlcId, mBuf); 
         break;
       }
    }
    CMCHKPKLOG(SPkS16, spId, mBuf, EUDXXXX, pst);
    pst->event = (Event) UDX_EVT_STA_UPD_REQ;

    return (SPstTsk(pst, mBuf));

} /* cmPkUdxStaUpdReq */

/*
*
*    Fun:    cmPkUdxStaPduReq
*
*    Desc:   pack the primitive KwUiUdxUeIdChgCfm
*
*    Ret:    ROK  -ok
*
*    Notes:  None
*
*    File:   ckw.c
*
*/
#ifdef ANSI
S16 cmPkUdxStaPduReq
(
Pst               *pst,
SpId              spId,
CmLteRlcId        *rlcId,
RlcUdxDlStaPdu     *pStaPdu
)
#else
S16 cmPkUdxStaPduReq(pst, suId, rlcId,pStaPdu)
Pst               *pst;
SpId              spId;
CmLteRlcId        *rlcId;
RlcUdxDlStaPdu     *pStaPdu;
#endif
{
    S16    ret1;
    Buffer *mBuf = NULLP;

    if((ret1 = SGetMsg(pst->region, pst->pool, &mBuf)) != ROK)
    {
#if (ERRCLASS & ERRCLS_ADD_RES)
       if(ret1 != ROK)
       {
          SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
               (ErrVal)EUDXXXX, (ErrVal)0, "SGetMsg() failed");
       }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */

       return (ret1);
    }
    
    switch (pst->selector)
    {
       case UDX_SEL_LC:
       {
         cmPkUdxStruct((U8 *)pStaPdu, sizeof(RlcUdxDlStaPdu),mBuf);
         cmPkUdxStruct((U8 *)rlcId, sizeof(CmLteRlcId),mBuf);
         /* Free status Pdu here for LC */
         SPutStaticBuffer(pst->region,pst->pool,(Data *) pStaPdu,
                          sizeof(RlcUdxDlStaPdu),0);
         break;
       }
       case UDX_SEL_LWLC:
       {
         CMCHKPK(oduPackPointer,(PTR) pStaPdu, mBuf);
         CMCHKPK(oduPackPointer,(PTR) rlcId, mBuf); 
         break;
       }
    }
    CMCHKPKLOG(SPkS16, spId, mBuf, EUDXXXX, pst);
    pst->event = (Event) UDX_EVT_STA_PDU_REQ;

    return (SPstTsk(pst, mBuf));

} /* cmPkUdxStaUpdReq */

#ifdef LTE_L2_MEAS
/*
*
*    Fun:    cmPkUdxL2MeasReq
*
*    Ret:    ROK  -ok
*
*    Notes:  None
*
*
*/
#ifdef ANSI
S16 cmPkUdxL2MeasReq
(
Pst               *pst,
RlcL2MeasReqEvt    *measReqEvt 
)
#else
S16 cmPkUdxL2MeasReq(pst, measReqEvt)
Pst               *pst;
RlcL2MeasReqEvt    *measReqEvt; 
#endif
{
    S16    ret1;
    Buffer *mBuf = NULLP;

    if((ret1 = SGetMsg(pst->region, pst->pool, &mBuf)) != ROK)
    {
#if (ERRCLASS & ERRCLS_ADD_RES)
       if(ret1 != ROK)
       {
          SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
               (ErrVal)EUDXXXX, (ErrVal)0, "SGetMsg() failed");
       }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */

       return (ret1);
    }
    
    switch (pst->selector)
    {
       case UDX_SEL_LC:
       {
         cmPkUdxStruct((U8 *)measReqEvt, sizeof(RlcL2MeasReqEvt),mBuf);
         break;
       }
       case UDX_SEL_LWLC:
       {
         CMCHKPK(oduPackPointer,(PTR) measReqEvt, mBuf);
         break;
       }
    }
    pst->event = (Event) UDX_EVT_L2MEAS_REQ;

    return (SPstTsk(pst, mBuf));

} /* cmPkUdxStaUpdReq */

/*
*
*    Fun:    cmPkUdxL2MeasReq
*
*    Ret:    ROK  -ok
*
*    Notes:  None
*
*
*/
#ifdef ANSI
S16 cmPkUdxL2MeasSendReq
(
Pst              *pst,
U8               measType
)
#else
S16 cmPkUdxL2MeasSendReq(pst, measReqEvt)
Pst               *pst;
U8                measType
#endif
{
    S16    ret1;
    Buffer *mBuf = NULLP;

    if((ret1 = SGetMsg(pst->region, pst->pool, &mBuf)) != ROK)
    {
#if (ERRCLASS & ERRCLS_ADD_RES)
       if(ret1 != ROK)
       {
          SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
               (ErrVal)EUDXXXX, (ErrVal)0, "SGetMsg() failed");
       }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */

       return (ret1);
    }
    
    switch (pst->selector)
    {
       case UDX_SEL_LC:
       case UDX_SEL_LWLC:
       {
          CMCHKPKLOG(oduUnpackUInt8, measType, mBuf, EUDXXXX, pst);
          break;
       }
    }
    pst->event = (Event) UDX_EVT_L2MEAS_SEND_REQ;

    return (SPstTsk(pst, mBuf));

} 

/*
*
*    Fun:    cmPkUdxL2MeasStopReq
*
*    Ret:    ROK  -ok
*
*    Notes:  None
*
*
*/
#ifdef ANSI
S16 cmPkUdxL2MeasStopReq
(
Pst              *pst,
U8               measType
)
#else
S16 cmPkUdxL2MeasStopReq(pst, measType)
Pst               *pst;
U8                measType
#endif
{
    S16    ret1;
    Buffer *mBuf = NULLP;

    if((ret1 = SGetMsg(pst->region, pst->pool, &mBuf)) != ROK)
    {
#if (ERRCLASS & ERRCLS_ADD_RES)
       if(ret1 != ROK)
       {
          SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
               (ErrVal)EUDXXXX, (ErrVal)0, "SGetMsg() failed");
       }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */

       return (ret1);
    }
    
    switch (pst->selector)
    {
       case UDX_SEL_LC:
       case UDX_SEL_LWLC:
       {
          CMCHKPKLOG(oduUnpackUInt8, measType, mBuf, EUDXXXX, pst);
          break;
       }
    }
    pst->event = (Event) UDX_EVT_L2MEAS_STOP_REQ;

    return (SPstTsk(pst, mBuf));

} 
#endif
/******************************************************************************
 *                            UNPACK FUNCTIONS
 *****************************************************************************/

/*
*
*    Fun:    cmUnpkUdxBndReq
*
*    Desc:    unpack the primitive UdxBndReq
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     ckw.c
*
*/
#ifdef ANSI
S16 cmUnpkUdxBndReq
(
UdxBndReq      func,
Pst            *pst,
Buffer         *mBuf
)
#else
S16 cmUnpkUdxBndReq(func, pst, mBuf)
UdxBndReq      func;
Pst            *pst;
Buffer         *mBuf;
#endif
{
    SuId       suId = 0;
    SpId       spId = 0;
    
    CMCHKUNPKLOG(SUnpkS16, &suId, mBuf, EUDXXXX, pst);
    CMCHKUNPKLOG(SUnpkS16, &spId, mBuf, EUDXXXX, pst);
    SPutMsg(mBuf);

    return ((*func)(pst, suId, spId));
} /*end of function cmUnpkUdxBndReq*/

/*
*
*    Fun:    cmUnpkUdxUbndReq
*
*    Desc:    unpack the primitive UdxUbndReq
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     ckw.c
*
*/
#ifdef ANSI
S16 cmUnpkUdxUbndReq
(
UdxUbndReq     func,
Pst            *pst,
Buffer         *mBuf
)
#else
S16 cmUnpkUdxUbndReq(func, pst, mBuf)
UdxUbndReq     func;
Pst            *pst;
Buffer         *mBuf;
#endif
{
    SpId       spId = 0;
    Reason     reason = 0;
    
    CMCHKUNPKLOG(SUnpkS16, &spId, mBuf, EUDXXXX, pst);
    CMCHKUNPKLOG(SUnpkS16, &reason, mBuf, EUDXXXX, pst);
    SPutMsg(mBuf);
    return ((*func)(pst, spId, reason));
} /*end of function cmUnpkUdxUbndReq*/

/*
*
*    Fun:    cmUnpkUdxBndCfm
*
*    Desc:    unpack the primitive UdxBndCfm
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     ckw.c
*
*/
#ifdef ANSI
S16 cmUnpkUdxBndCfm
(
UdxBndCfm      func,
Pst            *pst,
Buffer         *mBuf
)
#else
S16 cmUnpkUdxBndCfm(func, pst, mBuf)
UdxBndCfm      func;
Pst            *pst;
Buffer         *mBuf;
#endif
{
    SuId       suId = 0;
    U8         status = 0;
    
    CMCHKUNPKLOG(SUnpkS16, &suId, mBuf, EUDXXXX, pst);
    CMCHKUNPKLOG(oduPackUInt8, &status, mBuf, EUDXXXX, pst);
    SPutMsg(mBuf);

    return ((*func)(pst, suId, status));
} /*end of function cmUnpkUdxBndCfm*/


/*
*
*    Fun:    cmUnpkUdxCfgReq
*
*    Desc:    unpack the primitive KwUiUdxCfgReq
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     ckw.c
*
*/
#ifdef ANSI
S16 cmUnpkUdxCfgReq
(
UdxCfgReq         func,
Pst               *pst,
Buffer            *mBuf
)
#else
S16 cmUnpkUdxCfgReq(func, pst, mBuf)
UdxCfgReq         func;
Pst               *pst;
Buffer            *mBuf;
#endif
{
#if(ERRCLASS & ERRCLS_DEBUG)
    S16           ret1;
#endif /* ERRCLASS & ERRCLS_DEBUG */
    SpId          spId = 0;
    RlcCfgInfo     tmpCfgInfo;
    RlcCfgInfo    *cfgInfo; /*stack Variable because it is not freed */
    
    CMCHKUNPK(SUnpkS16, &(spId), mBuf);
    switch(pst->selector)
    {
#ifdef LCUDX
       case UDX_SEL_LC:
       {
#if(ERRCLASS & ERRCLS_DEBUG)
          ret1 = cmUnpkUdxStruct(mBuf,0,(U8 *)&tmpCfgInfo,sizeof(RlcCfgInfo));
          if(ret1 != ROK)
          {
             SPutMsg(mBuf);
             SLogError(pst->dstEnt, pst->dstInst, pst->dstProcId,
                   __FILE__, __LINE__, (ErrCls)ERRCLS_DEBUG,
                  (ErrVal)EUDXXXX, (ErrVal)ret1, "Unpacking failure");
             return ( ret1 );
          }
#else
         cmUnpkUdxStruct(mBuf,0,(U8 *)&tmpCfgInfo,sizeof(RlcCfgInfo));
#endif /* ERRCLASS & ERRCLS_DEBUG */
          cfgInfo = &tmpCfgInfo;
          break;
       }
       case UDX_SEL_LWLC:
       {
          CMCHKUNPK(oduUnpackPointer,(PTR *) &cfgInfo, mBuf);
          break;
       }
#endif /* LCUDX */
    }
    SPutMsg(mBuf);

    return ((*func)(pst, spId, cfgInfo));
} /* cmUnpkUdxCfgReq */


/*
*
*    Fun:    cmUnpkUdxCfgCfm
*
*    Desc:    unpack the primitive KwUiUdxCfgCfm
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     ckw.c
*
*/
#ifdef ANSI
S16 cmUnpkUdxCfgCfm
(
UdxCfgCfm         func,
Pst               *pst,
Buffer            *mBuf
)
#else
S16 cmUnpkUdxCfgCfm(func, pst, mBuf)
UdxCfgCfm         func;
Pst               *pst;
Buffer            *mBuf;
#endif
{
    S16 ret1;
    SuId             suId = 0;
    RlcCfgCfmInfo    *cfgCfmInfo = NULLP;
    
    CMCHKUNPK(SUnpkS16, &suId, mBuf);

    switch(pst->selector)
    {
#ifdef LCUDX
       case UDX_SEL_LC:
       {
           if((ret1 = SGetStaticBuffer(pst->region, pst->pool, (Data **)&cfgCfmInfo,\
                       sizeof(RlcCfgCfmInfo),0)) != ROK)
           {
#if (ERRCLASS & ERRCLS_ADD_RES)
              if(ret1 != ROK)
              {
                 SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                       __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                      (ErrVal)EUDXXXX, (ErrVal)0, "SGetMsg() failed");
              }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
              return (ret1);
           }

          ret1 = cmUnpkUdxStruct(mBuf,0,(U8 *)cfgCfmInfo, sizeof(RlcCfgCfmInfo));
#if(ERRCLASS & ERRCLS_DEBUG)
          if(ret1 != ROK)
          {
             SPutMsg(mBuf);
             SLogError(pst->dstEnt, pst->dstInst, pst->dstProcId,
                   __FILE__, __LINE__, (ErrCls)ERRCLS_DEBUG,
                  (ErrVal)EUDXXXX, (ErrVal)ret1, "Unpacking failure");
             return ( ret1 );
          }
#endif /* ERRCLASS & ERRCLS_DEBUG */
          break;
       }
       case UDX_SEL_LWLC:
       {
          CMCHKUNPK(oduUnpackPointer,(PTR *) &cfgCfmInfo, mBuf);
          break;
       }
#endif /* LCUDX */
    }
    SPutMsg(mBuf);

    return ((*func)(pst, suId, cfgCfmInfo));
} /* cmUnpkUdxCfgCfm */

/*
*
*    Fun:    cmUnpkUdxUeIdChgReq
*
*    Desc:    unpack the primitive KwUiUdxUeIdChgReq
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     ckw.c
*
*/
#ifdef ANSI
S16 cmUnpkUdxUeIdChgReq
(
UdxUeIdChgReq     func,
Pst               *pst,
Buffer            *mBuf
)
#else
S16 cmUnpkUdxUeIdChgReq(func, pst, mBuf)
UdxUeIdChgReq     func;
Pst               *pst;
Buffer            *mBuf;
#endif
{
    SpId      spId = 0;
    U32       transId = 0;
    CkwUeInfo tmpUeInfo;
    CkwUeInfo tmpNewUeInfo;
    CkwUeInfo *ueInfo;
    CkwUeInfo *newUeInfo;
    
    CMCHKUNPK(SUnpkS16, &(spId), mBuf);
    CMCHKUNPKLOG(oduPackUInt32, &transId, mBuf, EUDXXXX, pst);

    switch(pst->selector)
    {
#ifdef LCUDX
       case UDX_SEL_LC:
       {
          cmUnpkUdxStruct(mBuf,0,(U8 *)&tmpNewUeInfo, sizeof(CkwUeInfo));
          cmUnpkUdxStruct(mBuf,sizeof(CkwUeInfo),(U8 *)&tmpUeInfo, sizeof(CkwUeInfo));

          ueInfo =  &tmpUeInfo;
          newUeInfo =  &tmpNewUeInfo;
          break;
       }
       case UDX_SEL_LWLC:
       {
          CMCHKUNPK(oduUnpackPointer,(PTR *) &ueInfo, mBuf);
          CMCHKUNPK(oduUnpackPointer,(PTR *) &newUeInfo, mBuf);
          break;
       }
       default:
       {
          printf("cmUnpkUdxUeIdChgReq()- selector =%d not supported \n", pst->selector);
          exit(1);
       }

#endif /* LCUDX */
    }
    SPutMsg(mBuf);

    return ((*func)(pst, spId, transId, ueInfo, newUeInfo));

} /* cmUnpkUdxUeIdChgReq */

/*
*
*    Fun:     cmUnpkUdxUeIdChgCfm
*
*    Desc:    unpack the primitive KwUiUdxUeIdChgCfm
*
*    Ret:     ROK  -ok
*
*    Notes:   None
*
*    File:    ckw.c
*
*/
#ifdef ANSI
S16 cmUnpkUdxUeIdChgCfm
(
UdxUeIdChgCfm     func,
Pst               *pst,
Buffer            *mBuf
)
#else
S16 cmUnpkUdxUeIdChgCfm(func, pst, mBuf)
UdxUeIdChgCfm     func;
Pst               *pst;
Buffer            *mBuf;
#endif
{
    SuId      suId = 0;
    U32       transId = 0;
    CmStatus  status;
    
    memset((U8 *)&status, (U8)0, (PTR)sizeof(CmStatus));

    CMCHKUNPK(SUnpkS16, &suId, mBuf);
    CMCHKUNPKLOG(oduPackUInt32, &transId, mBuf, EUDXXXX, pst);

    CMCHKUNPK(cmUnpkCmStatus, &status, mBuf);

    SPutMsg(mBuf);

    return ((*func)(pst, suId, transId, status));

} /* cmUnpkUdxUeIdChgCfm */

/*
*
*    Fun:     cmUnpkUdxStaUpdCfm
*
*    Desc:    unpack the primitive KwUiUdxUeIdChgCfm
*
*    Ret:     ROK  -ok
*
*    Notes:   None
*
*    File:    ckw.c
*
*/
#ifdef ANSI
S16 cmUnpkUdxStaUpdCfm
(
UdxStaUpdCfm     func,
Pst               *pst,
Buffer            *mBuf
)
#else
S16 cmUnpkUdxStaUpdCfm(func, pst, mBuf)
UdxStaUpdCfm     func;
Pst               *pst;
Buffer            *mBuf;
#endif
{
    SuId      suId = 0;
    CmLteRlcId *rlcId = NULLP;  /* KW_FIX */
    RlcUdxBufLst *pBufLst = NULLP; /* KW_FIX*/
    
    CMCHKUNPK(SUnpkS16, &suId, mBuf);
    switch (pst->selector)
    {
       case UDX_SEL_LC:
       {
	  break;
       }
       case UDX_SEL_LWLC:
       {
          CMCHKUNPK(oduUnpackPointer, (PTR *)&rlcId,mBuf);
          CMCHKUNPK(oduUnpackPointer, (PTR *)&pBufLst,mBuf);
          break;
       }
    } 

    SPutMsg(mBuf);

    return ((*func)(pst, suId, rlcId, pBufLst));

} /* cmUnpkUdxUeIdChgCfm */

/*
*
*    Fun:     cmUnpkUdxStaUpdReq
*
*    Desc:    unpack the primitive KwUiUdxUeIdChgCfm
*
*    Ret:     ROK  -ok
*
*    Notes:   None
*
*    File:    ckw.c
*
*/
#ifdef ANSI
S16 cmUnpkUdxStaUpdReq
(
UdxStaUpdReq     func,
Pst               *pst,
Buffer            *mBuf
)
#else
S16 cmUnpkUdxStaUpdReq(func, pst, mBuf)
UdxStaUpdReq     func;
Pst               *pst;
Buffer            *mBuf;
#endif
{
    SpId      spId = 0;
    CmLteRlcId *rlcId = NULLP; /* KW_FIX */ 
    RlcUdxStaPdu *pStaPdu = NULLP; /* KW_FIX */
    S16 ret1;
    CmLteRlcId tmpRlcId;
    
    CMCHKUNPK(SUnpkS16, &spId, mBuf);
    switch (pst->selector)
    {
       case UDX_SEL_LC:
       {
           if((ret1 = SGetStaticBuffer(pst->region, pst->pool, (Data **)&pStaPdu,
                       sizeof(RlcUdxStaPdu),0)) != ROK)
           {
#if (ERRCLASS & ERRCLS_ADD_RES)
              if(ret1 != ROK)
              {
                 SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                       __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                      (ErrVal)EUDXXXX, (ErrVal)0, "SGetMsg() failed");
              }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
              return (ret1);
           }
          ret1 = cmUnpkUdxStruct(mBuf,0,(U8 *)pStaPdu, sizeof(RlcUdxStaPdu));
          ret1 = cmUnpkUdxStruct(mBuf,sizeof(RlcUdxStaPdu),(U8 *)&tmpRlcId,sizeof(CmLteRlcId));
          rlcId = &tmpRlcId; 
          break;
       }
       case UDX_SEL_LWLC:
       {
          CMCHKUNPK(oduUnpackPointer, (PTR *)&rlcId,mBuf);
          CMCHKUNPK(oduUnpackPointer, (PTR *)&pStaPdu,mBuf);
          break;
       }
    } 

    SPutMsg(mBuf);

    return ((*func)(pst, spId, rlcId, pStaPdu));

} /* cmUnpkUdxUeIdChgCfm */

/*
*
*    Fun:     cmUnpkUdxStaPduReq
*
*    Desc:    unpack the primitive KwUiUdxUeIdChgCfm
*
*    Ret:     ROK  -ok
*
*    Notes:   None
*
*    File:    ckw.c
*
*/
#ifdef ANSI
S16 cmUnpkUdxStaPduReq
(
UdxStaPduReq     func,
Pst               *pst,
Buffer            *mBuf
)
#else
S16 cmUnpkUdxStaPduReq(func, pst, mBuf)
UdxStaPduReq     func;
Pst               *pst;
Buffer            *mBuf;
#endif
{
    S16       ret1;
    SpId      spId = 0;
    CmLteRlcId tmpRlcId;
    CmLteRlcId *rlcId = NULLP;  /* KW_FIX */
    RlcUdxDlStaPdu *pStaPdu = NULLP; /* KW_FIX */
    
    CMCHKUNPK(SUnpkS16, &spId, mBuf);
    switch (pst->selector)
    {
       case UDX_SEL_LC:
       {
           if((ret1 = SGetStaticBuffer(pst->region, pst->pool, (Data **)&pStaPdu,
                       sizeof(RlcUdxDlStaPdu),0)) != ROK)
           {
#if (ERRCLASS & ERRCLS_ADD_RES)
              if(ret1 != ROK)
              {
                 SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                       __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                      (ErrVal)EUDXXXX, (ErrVal)0, "SGetMsg() failed");
              }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
              return (ret1);
           }
          ret1 = cmUnpkUdxStruct(mBuf,0, (U8 *)pStaPdu, sizeof(RlcUdxDlStaPdu));
          ret1 = cmUnpkUdxStruct(mBuf,sizeof(RlcUdxDlStaPdu),(U8 *)&tmpRlcId,sizeof(CmLteRlcId));
          rlcId = &tmpRlcId; 
           break;
       }
       case UDX_SEL_LWLC:
       {
          CMCHKUNPK(oduUnpackPointer, (PTR *)&rlcId,mBuf);
          CMCHKUNPK(oduUnpackPointer, (PTR *)&pStaPdu,mBuf);
          break;
       }
    } 

    SPutMsg(mBuf);
    return ((*func)(pst, spId, rlcId, pStaPdu));

} /* cmUnpkUdxUeIdChgCfm */

/*
*
*    Fun:     cmUnpkUdxStaProhTmrStart
*
*    Desc:    unpack the Status prohibit timer start Msg
*
*    Ret:     ROK  -ok
*
*    Notes:   None
*
*    File:    kw_udx.c
*
*/
#ifdef ANSI
S16 cmUnpkUdxStaProhTmrStart
(
UdxStaProhTmrStart     func,
Pst               *pst,
Buffer            *mBuf
)
#else
S16 cmUnpkUdxStaProhTmrStart(func, pst, mBuf)
UdxStaProhTmrStart     func;
Pst               *pst;
Buffer            *mBuf;
#endif
{
    SpId      spId = 0;
    CmLteRlcId tmpRlcId;
    CmLteRlcId *rlcId = NULLP; /* KW_FIX */
    
    CMCHKUNPK(SUnpkS16, &spId, mBuf);
    switch (pst->selector)
    {
       case UDX_SEL_LC:
       {
          cmUnpkUdxStruct(mBuf,0,(U8 *)&tmpRlcId,sizeof(CmLteRlcId));
          rlcId = &tmpRlcId;
          break;
       }
       case UDX_SEL_LWLC:
       {
          CMCHKUNPK(oduUnpackPointer, (PTR *)&rlcId,mBuf);
          break;
       }
    } 

    SPutMsg(mBuf);

    return ((*func)(pst, spId, rlcId));

} /* cmUnpkUdxStaProhTmrStart */

#ifdef LTE_L2_MEAS
/*

 
*    Fun:    cmUnpkUdxL2MeasReq
*
*    Ret:    ROK  -ok
*
*    Notes:  None
*
*
*/
#ifdef ANSI
S16 cmUnpkUdxL2MeasReq
(
UdxL2MeasReq     func,
Pst              *pst,
Buffer           *mBuf
)
#else
S16 cmUnpkUdxL2MeasReq(func, pst, mBuf)
UdxL2MeasReq     func;
Pst              *pst;
Buffer           *mBuf;
#endif
{
    RlcL2MeasReqEvt    tmpMeasReqEvt;
    RlcL2MeasReqEvt    *measReqEvt = NULLP;  /* KW_FIX */

    switch (pst->selector)
    {
       case UDX_SEL_LC:
       {
          cmUnpkUdxStruct(mBuf,0,(U8 *)&tmpMeasReqEvt,sizeof(RlcL2MeasReqEvt));
          measReqEvt = &tmpMeasReqEvt;
          break;
       }
       case UDX_SEL_LWLC:
       {
         CMCHKUNPK(oduUnpackPointer, (PTR *)&measReqEvt,mBuf);
         break;
       }
    }

    SPutMsg(mBuf);

    return ((*func)(pst, measReqEvt));
} /* cmUnpkUdxL2MeasReq */

/*
*
*    Fun:    cmUnpkUdxL2MeasSendReq
*
*    Ret:    ROK  -ok
*
*    Notes:  None
*
*
*/
#ifdef ANSI
S16 cmUnpkUdxL2MeasSendReq
(
UdxL2MeasSendReq   func,
Pst                *pst,
Buffer             *mBuf
)
#else
S16 cmUnpkUdxL2MeasSendReq(func, pst, mBuf)
UdxL2MeasSendReq   func;
Pst                *pst;
Buffer             *mBuf;
#endif
{
    U8     measType = 0;  /* KW_FIX */

    switch (pst->selector)
    {
       case UDX_SEL_LC:
       case UDX_SEL_LWLC:
       {
          CMCHKUNPK(oduPackUInt8, &measType, mBuf);
         break;
       }
    }

    SPutMsg(mBuf);

    return ((*func)(pst, measType));
} /* cmUnpkUdxL2MeasReq */

/*
*
*    Fun:    cmUnpkUdxL2MeasStopReq
*
*    Ret:    ROK  -ok
*
*    Notes:  None
*
*
*/
#ifdef ANSI
S16 cmUnpkUdxL2MeasStopReq
(
UdxL2MeasStopReq   func,
Pst                *pst,
Buffer             *mBuf
)
#else
S16 cmUnpkUdxL2MeasStopReq(func, pst, mBuf)
UdxL2MeasSendReq     func;
Pst                  *pst;
Buffer               *mBuf;
#endif
{
    U8     measType = 0; /* KW_FIX */

    switch (pst->selector)
    {
       case UDX_SEL_LC:
       case UDX_SEL_LWLC:
       {
          CMCHKUNPK(oduPackUInt8, &measType, mBuf);
         break;
       }
    }

    SPutMsg(mBuf);

    return ((*func)(pst, measType));
} /* cmUnpkUdxL2MeasStopReq */
#endif
#endif /* LCUDX */

  
/********************************************************************30**
         End of file
**********************************************************************/
