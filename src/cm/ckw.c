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
  
        Name:    CKW RRC Control Interface
    
        Type:    C file
  
        Desc:    This file Contains the packing and unpacking functions for
                 CKW Interface
 
        File:    ckw.c

*********************************************************************21*/

#include "common_def.h"
#include "ckw.h"
#include "ckw.x"

/* extern (.x) include files */


#ifdef __cplusplus
EXTERN "C" {
#endif  /*for extern "C"*/

#ifdef LCCKW
/*
*
*    Fun:    cmPkCkwBndReq
*
*    Desc:    pack the primitive CkwBndReq
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     ckw.c
*
*/
#ifdef ANSI
S16 cmPkCkwBndReq
(
Pst *pst,
SuId suId,
SpId spId
)
#else
S16 cmPkCkwBndReq(pst, suId, spId)
Pst *pst;
SuId suId;
SpId spId;
#endif
{
    S16 ret1;
    Buffer *mBuf;
    mBuf = NULLP;
    TRC3(cmPkCkwBndReq)

    if((ret1 = SGetMsg(pst->region, pst->pool, &mBuf)) != ROK)
    {
#if (ERRCLASS & ERRCLS_ADD_RES)
       if(ret1 != ROK)
       {
          SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
               (ErrVal)ECKW001, (ErrVal)0, "SGetMsg() failed");
       }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
       return (ret1);
    }
    CMCHKPKLOG(SPkS16, spId, mBuf, ECKW002, pst);
    CMCHKPKLOG(SPkS16, suId, mBuf, ECKW003, pst);
    pst->event = (Event) CKW_EVT_BND_REQ;

    return (SPstTsk(pst,mBuf));
} /*end of function cmPkCkwBndReq*/

/*
*
*    Fun:    cmPkCkwUbndReq
*
*    Desc:    pack the primitive CkwUbndReq
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     ckw.c
*
*/
#ifdef ANSI
S16 cmPkCkwUbndReq
(
Pst *pst,
SpId spId,
Reason reason
)
#else
S16 cmPkCkwUbndReq(pst, spId, reason)
Pst *pst;
SpId spId;
Reason reason;
#endif
{
    S16 ret1;
    Buffer *mBuf;
    mBuf = NULLP;
    TRC3(cmPkCkwUbndReq)

    if((ret1 = SGetMsg(pst->region, pst->pool, &mBuf)) != ROK)
    {
#if (ERRCLASS & ERRCLS_ADD_RES)
       if(ret1 != ROK)
       {
          SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
               (ErrVal)ECKW004, (ErrVal)0, "SGetMsg() failed");
       }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
       return (ret1);
    }
    CMCHKPKLOG(SPkS16, reason, mBuf, ECKW005, pst);
    CMCHKPKLOG(SPkS16, spId, mBuf, ECKW006, pst);
    pst->event = (Event) CKW_EVT_UBND_REQ;

    return (SPstTsk(pst,mBuf));
} /*end of function cmPkCkwUbndReq*/

/*
*
*    Fun:    cmPkCkwBndCfm
*
*    Desc:    pack the primitive CkwBndCfm
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     ckw.c
*
*/
#ifdef ANSI
S16 cmPkCkwBndCfm
(
Pst *pst,
SuId suId,
U8 status
)
#else
S16 cmPkCkwBndCfm(pst, suId, status)
Pst *pst;
SuId suId;
U8 status;
#endif
{
    S16 ret1;
    Buffer *mBuf;
    mBuf = NULLP;
    TRC3(cmPkCkwBndCfm)

    if((ret1 = SGetMsg(pst->region, pst->pool, &mBuf)) != ROK)
    {
#if (ERRCLASS & ERRCLS_ADD_RES)
       if(ret1 != ROK)
       {
          SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
               (ErrVal)ECKW007, (ErrVal)0, "SGetMsg() failed");
       }

#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
       return (ret1);
    }
    CMCHKPKLOG(oduUnpackUInt8, status, mBuf, ECKW008, pst);
    CMCHKPKLOG(SPkS16, suId, mBuf, ECKW009, pst);
    pst->event = (Event) CKW_EVT_BND_CFM;

    return (SPstTsk(pst,mBuf));
} /*end of function cmPkCkwBndCfm*/


/*
*
*    Fun:    cmPkRlcTmInfo
*
*    Desc:    pack the structure RlcTmInfo
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     ckw.c
*
*/
#ifdef ANSI
S16 cmPkRlcTmInfo
(
RlcTmInfo      *param,
Buffer         *mBuf
)
#else
S16 cmPkRlcTmInfo(param, mBuf)
RlcTmInfo      *param;
Buffer         *mBuf;
#endif
{
    TRC3(cmPkRlcTmInfo)

    CMCHKPK(oduUnpackUInt32, param->dl.buffSize, mBuf);

    return ROK;
} /* cmPkRlcTmInfo */


/*
*
*    Fun:    cmPkRlcUmInfo
*
*    Desc:    pack the structure RlcUmInfo
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     ckw.c
*
*/
#ifdef ANSI
S16 cmPkRlcUmInfo
(
RlcEntCfgInfo  *param,
Buffer         *mBuf
)
#else
S16 cmPkRlcUmInfo(param, mBuf)
RlcEntCfgInfo  *param;
Buffer         *mBuf;
#endif
{
    TRC3(cmPkRlcUmInfo)

    switch(param->dir)
    {
       case CKW_CFG_DIR_DL:
          {
             CMCHKPK(oduUnpackUInt8, param->m.umInfo.dl.snLen, mBuf);
             break;
          }
       case CKW_CFG_DIR_UL:
          {
             CMCHKPK(oduUnpackUInt8, param->m.umInfo.ul.reOrdTmr, mBuf);
             CMCHKPK(oduUnpackUInt8, param->m.umInfo.ul.snLen, mBuf);
             break;
          }
       /* Patch ckw_c_001.main_3 */
       case CKW_CFG_DIR_BOTH:
          {
             CMCHKPK(oduUnpackUInt8, param->m.umInfo.dl.snLen, mBuf);
             CMCHKPK(oduUnpackUInt8, param->m.umInfo.ul.reOrdTmr, mBuf);
             CMCHKPK(oduUnpackUInt8, param->m.umInfo.ul.snLen, mBuf);
             break;
          }
    }

    return ROK;
} /* cmPkRlcUmInfo */


/*
*
*    Fun:    cmPkRlcAmInfo
*
*    Desc:    pack the structure RlcAmInfo
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     ckw.c
*
*/
#ifdef ANSI
S16 cmPkRlcAmInfo
(
RlcAmInfo      *param,
Buffer         *mBuf
)
#else
S16 cmPkRlcAmInfo(param, mBuf)
RlcAmInfo      *param;
Buffer         *mBuf;
#endif
{
    TRC3(cmPkRlcAmInfo)

    /* UP LINK */
    CMCHKPK(oduUnpackUInt8, param->ul.reOrdTmr, mBuf);
    CMCHKPK(oduUnpackUInt16, param->ul.staProhTmr, mBuf);
    CMCHKPK(oduUnpackUInt8, param->ul.snLen, mBuf);

    /* DOWN LINK */
    CMCHKPK(oduUnpackUInt8, param->dl.maxRetx, mBuf);
    CMCHKPK(SPkS32, param->dl.pollByte, mBuf);
    CMCHKPK(SPkS16, param->dl.pollPdu, mBuf);
    CMCHKPK(oduUnpackUInt16, param->dl.pollRetxTmr, mBuf);
    CMCHKPK(oduUnpackUInt8, param->dl.snLen, mBuf);

    return ROK;
} /* cmPkRlcAmInfo */


/*
*
*    Fun:    cmPkCkwLInfo
*
*    Desc:    pack the structure CkwLChInfo
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     ckw.c
*
*/
#ifdef ANSI
S16 cmPkCkwLChInfo
(
CkwLChInfo     *param,
Buffer         *mBuf
)
#else
S16 cmPkCkwLChInfo(param, mBuf)
CkwLChInfo     *param;
Buffer         *mBuf;
#endif
{
    TRC3(cmPkCkwLChInfo)

    CMCHKPK(oduUnpackUInt8, param->type, mBuf);
    CMCHKPK(oduUnpackUInt8, param->lChId, mBuf);

    return ROK;
} /* cmPkCkwLChInfo */


/*
*
*    Fun:    cmPkRlcEntCfgInfo
*
*    Desc:    pack the structure RlcEntCfgInfo
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     ckw.c
*
*/
#ifdef ANSI
S16 cmPkRlcEntCfgInfo
(
RlcEntCfgInfo  *param,
Buffer         *mBuf
)
#else
S16 cmPkRlcEntCfgInfo(param, mBuf)
RlcEntCfgInfo  *param;
Buffer         *mBuf;
#endif
{
    TRC3(cmPkRlcEntCfgInfo)

    switch(param->entMode)
    {
       case CM_LTE_MODE_TM:
          {
             CMCHKPK(cmPkRlcTmInfo, &(param->m.tmInfo), mBuf);
             break;
          }
       case CM_LTE_MODE_UM:
          {
             CMCHKPK(cmPkRlcUmInfo, param, mBuf);
             break;
          }
       case CM_LTE_MODE_AM:
          {
             CMCHKPK(cmPkRlcAmInfo, &(param->m.amInfo), mBuf);
             break;
          }
    }

    /* Logical Channel Information */
    switch(param->entMode)
    {
       case CM_LTE_MODE_TM:
          {
             CMCHKPK(cmPkCkwLChInfo, &(param->lCh[0]), mBuf);
             break;
          }
       /* ccpu00128195: Packing lCh[0] and lCh[1] for UM bearer as well */
       case CM_LTE_MODE_UM:
       case CM_LTE_MODE_AM:
          {
             CMCHKPK(cmPkCkwLChInfo, &(param->lCh[1]), mBuf);
             CMCHKPK(cmPkCkwLChInfo, &(param->lCh[0]), mBuf);
             break;
          }
    }

#ifdef TENB_MULT_CELL_SUPPRT
    CMCHKPK(SPkS16, param->rguSapId, mBuf);
#endif
    CMCHKPK(SPkS16, param->discardTmr, mBuf);
    CMCHKPK(oduUnpackUInt8, param->dir, mBuf);
    CMCHKPK(cmPkLteRlcMode, param->entMode, mBuf);
    CMCHKPK(oduUnpackUInt8, param->qci, mBuf);
    CMCHKPK(oduUnpackUInt8, param->rbType, mBuf);
    CMCHKPK(oduUnpackUInt8, param->rbId, mBuf);
    CMCHKPK(oduUnpackUInt8, param->cfgType, mBuf);

    return ROK;
} /* cmPkRlcEntCfgInfo */


/*
*
*    Fun:    cmPkRlcCfgInfo
*
*    Desc:    pack the structure RlcCfgInfo
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     ckw.c
*
*/
#ifdef ANSI
S16 cmPkRlcCfgInfo
(
RlcCfgInfo     *param,
Pst            *pst,
Buffer         *mBuf
)
#else
S16 cmPkRlcCfgInfo(param, pst, mBuf)
RlcCfgInfo     *param;
Pst            *pst;
Buffer         *mBuf;
#endif
{
   S16 idx;

   TRC3(cmPkRlcCfgInfo)

    for (idx = param->numEnt-1; idx >= 0; idx--)
    {
       CMCHKPK(cmPkRlcEntCfgInfo, &(param->entCfg[idx]), mBuf);
    }

    CMCHKPK(oduUnpackUInt8, param->numEnt, mBuf);
    CMCHKPK(cmPkLteCellId, param->cellId, mBuf);
    CMCHKPK(cmPkLteRnti, param->ueId, mBuf);
    CMCHKPKLOG(oduUnpackUInt32, param->transId, mBuf, ECKW010, pst);

    return ROK;
} /* cmPkRlcCfgInfo */


/*
*
*    Fun:    cmPkRlcEntCfgCfmInfo
*
*    Desc:    pack the structure RlcEntCfgInfo
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     ckw.c
*
*/
#ifdef ANSI
S16 cmPkRlcEntCfgCfmInfo
(
RlcEntCfgCfmInfo  *param,
Buffer            *mBuf
)
#else
S16 cmPkRlcEntCfgCfmInfo(param, mBuf)
RlcEntCfgCfmInfo  *param;
Buffer            *mBuf;
#endif
{
    TRC3(cmPkRlcEntCfgCfmInfo)

    CMCHKPK(cmPkCmStatus, &(param->status), mBuf);
    CMCHKPK(oduUnpackUInt8, param->rbType, mBuf);
    CMCHKPK(oduUnpackUInt8, param->rbId, mBuf);

    return ROK;
} /* cmPkRlcEntCfgCfmInfo */


/*
*
*    Fun:    cmPkRlcCfgCfmInfo
*
*    Desc:    pack the structure RlcCfgCfmInfo
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     ckw.c
*
*/
#ifdef ANSI
S16 cmPkRlcCfgCfmInfo
(
RlcCfgCfmInfo  *param,
Pst            *pst,
Buffer         *mBuf
)
#else
S16 cmPkRlcCfgCfmInfo(param, pst, mBuf)
RlcCfgCfmInfo  *param;
Pst            *pst;
Buffer         *mBuf;
#endif
{
    S16         idx;

    TRC3(cmPkRlcCfgCfmInfo)

    for (idx = param->numEnt-1; idx >= 0; idx--)
    {
       CMCHKPK(cmPkRlcEntCfgCfmInfo, &(param->entCfgCfm[(U8)idx]), mBuf);
    }

    CMCHKPK(oduUnpackUInt8, param->numEnt, mBuf);
    CMCHKPK(cmPkLteCellId, param->cellId, mBuf);
    CMCHKPK(cmPkLteRnti, param->ueId, mBuf);
    CMCHKPKLOG(oduUnpackUInt32, param->transId, mBuf, ECKW014, pst);

    return ROK;
} /* cmPkRlcCfgCfmInfo */


/*
*
*    Fun:    cmPkRlcCfgCfm
*
*    Desc:    pack the primitive RlcUiRlcCfgCfm
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     ckw.c
*
*/
#ifdef ANSI
S16 cmPkRlcCfgCfm
(
Pst               *pst,
SuId              suId,
RlcCfgCfmInfo     *cfgCfmInfo
)
#else
S16 cmPkRlcCfgCfm(pst, suId, cfgCfmInfo)
Pst               *pst;
SuId              suId;
RlcCfgCfmInfo     *cfgCfmInfo;
#endif
{
    S16 ret1;
    Buffer *mBuf;
    mBuf = NULLP;
    TRC3(cmPkRlcCfgCfm)

    if((ret1 = SGetMsg(pst->region, pst->pool, &mBuf)) != ROK)
    {
#if (ERRCLASS & ERRCLS_ADD_RES)
       if(ret1 != ROK)
       {
          SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
               (ErrVal)ECKW015, (ErrVal)0, "SGetMsg() failed");
       }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
       return (ret1);
    }

    switch(pst->selector)
    {
#ifdef LCCKW
       case ODU_SELECTOR_LC:
          {
             ret1 = cmPkRlcCfgCfmInfo( (cfgCfmInfo), pst, mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
             if(ret1 != ROK)
             {
                SPutMsg(mBuf);
                SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                      __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                      (ErrVal)ECKW016, (ErrVal)ret1, "Packing failure");
                return ( ret1 );
             }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
             if (SPutSBuf(pst->region, pst->pool, (Data *)cfgCfmInfo,
                      sizeof(RlcCfgCfmInfo)) != ROK)
             {
                SPutMsg(mBuf);
                return RFAILED;
             }

             break;
          }
#endif /* LCCKW */
    }

    CMCHKPKLOG(SPkS16, suId, mBuf, ECKW017, pst);
    pst->event = (Event) CKW_EVT_CFG_CFM;

    return (SPstTsk(pst,mBuf));
} /* cmPkRlcCfgCfm */

/*
*
*    Fun:    cmPkCkwUeInfo
*
*    Desc:   pack the primitive KwUiCkwUeInfo
*
*    Ret:    ROK  -ok
*
*    Notes:  None
*
*    File:   ckw.c
*
*/
#ifdef ANSI
S16 cmPkCkwUeInfo
(
CkwUeInfo *param,
Buffer    *mBuf
)
#else
S16 cmPkCkwUeInfo(param, mBuf)
CkwUeInfo *param;
Buffer    *mBuf;
#endif
{
   TRC2(cmPkCkwUeInfo)

   CMCHKPK(cmPkLteCellId, param->cellId, mBuf);
   CMCHKPK(cmPkLteRnti, param->ueId, mBuf);

   return ROK;
}

/*
*
*    Fun:    cmPkCkwUeIdChgReq
*
*    Desc:   pack the primitive RlcUiCkwUeIdChgReq
*
*    Ret:    ROK  -ok
*
*    Notes:  None
*
*    File:   ckw.c
*
*/
#ifdef ANSI
S16 cmPkCkwUeIdChgReq
(
Pst               *pst,
SpId              spId,
U32               transId,
CkwUeInfo         *ueInfo,
CkwUeInfo         *newUeInfo
)
#else
S16 cmPkCkwUeIdChgReq(pst, spId, transId, ueInfo, newUeInfo)
Pst               *pst;
SpId              spId;
U32               transId;
CkwUeInfo         *ueInfo;
CkwUeInfo         *newUeInfo;
#endif
{
    S16    ret1;
    Buffer *mBuf = NULLP;

    TRC3(cmPkCkwUeIdChgReq)

    if((ret1 = SGetMsg(pst->region, pst->pool, &mBuf)) != ROK)
    {
#if (ERRCLASS & ERRCLS_ADD_RES)
       if(ret1 != ROK)
       {
          SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
               (ErrVal)ECKW018, (ErrVal)0, "SGetMsg() failed");
       }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */

       return (ret1);
    }

    CMCHKPK(cmPkCkwUeInfo, newUeInfo, mBuf);
    if (SPutSBuf(pst->region, pst->pool, (Data *)newUeInfo,
             sizeof(CkwUeInfo)) != ROK)
    {
       SPutMsg(mBuf);
       return RFAILED;
    }
    CMCHKPK(cmPkCkwUeInfo, ueInfo, mBuf);
    if (SPutSBuf(pst->region, pst->pool, (Data *)ueInfo,
             sizeof(CkwUeInfo)) != ROK)
    {
       SPutMsg(mBuf);
       return RFAILED;
    }
    CMCHKPKLOG(oduUnpackUInt32, transId, mBuf, ECKW019, pst);
    CMCHKPKLOG(SPkS16, spId, mBuf, ECKW020, pst);
    pst->event = (Event) CKW_EVT_UEIDCHG_REQ;

    return (SPstTsk(pst, mBuf));

} /* cmPkCkwUeIdChgReq */

/*
*
*    Fun:    cmPkCkwUeIdChgCfm
*
*    Desc:   pack the primitive RlcUiCkwUeIdChgCfm
*
*    Ret:    ROK  -ok
*
*    Notes:  None
*
*    File:   ckw.c
*
*/
#ifdef ANSI
S16 cmPkCkwUeIdChgCfm
(
Pst               *pst,
SuId              suId,
U32               transId,
CkwUeInfo         *ueInfo,
CmStatus          status
)
#else
S16 cmPkCkwUeIdChgCfm(pst, suId, transId, ueInfo, status)
Pst               *pst;
SuId              suId;
U32               transId;
CkwUeInfo         *ueInfo;
CmStatus          status;
#endif
{
    S16    ret1;
    Buffer *mBuf = NULLP;

    TRC3(cmPkCkwUeIdChgCfm)

    if((ret1 = SGetMsg(pst->region, pst->pool, &mBuf)) != ROK)
    {
#if (ERRCLASS & ERRCLS_ADD_RES)
       if(ret1 != ROK)
       {
          SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
               (ErrVal)ECKW021, (ErrVal)0, "SGetMsg() failed");
       }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */

       return (ret1);
    }
    
    CMCHKPK(cmPkCmStatus, &status, mBuf); 
    CMCHKPK(cmPkCkwUeInfo, ueInfo, mBuf);
    if (SPutSBuf(pst->region, pst->pool, (Data *)ueInfo,
             sizeof(CkwUeInfo)) != ROK)
    {
       SPutMsg(mBuf);
       return RFAILED;
    }
    CMCHKPKLOG(oduUnpackUInt32, transId, mBuf, ECKW022, pst);
    CMCHKPKLOG(SPkS16, suId, mBuf, ECKW023, pst);
    pst->event = (Event) CKW_EVT_UEIDCHG_CFM;

    return (SPstTsk(pst, mBuf));

} /* cmPkCkwUeIdChgCfm */


/******************************************************************************
 *                            UNPACK FUNCTIONS
 *****************************************************************************/

/*
*
*    Fun:    cmUnpkCkwBndReq
*
*    Desc:    unpack the primitive CkwBndReq
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     ckw.c
*
*/
#ifdef ANSI
S16 cmUnpkCkwBndReq
(
CkwBndReq      func,
Pst            *pst,
Buffer         *mBuf
)
#else
S16 cmUnpkCkwBndReq(func, pst, mBuf)
CkwBndReq      func;
Pst            *pst;
Buffer         *mBuf;
#endif
{
    SuId       suId = 0;
    SpId       spId = 0;
    
    TRC3(cmUnpkCkwBndReq)

    CMCHKUNPKLOG(SUnpkS16, &suId, mBuf, ECKW024, pst);
    CMCHKUNPKLOG(SUnpkS16, &spId, mBuf, ECKW025, pst);
    SPutMsg(mBuf);

    return ((*func)(pst, suId, spId));
} /*end of function cmUnpkCkwBndReq*/

/*
*
*    Fun:    cmUnpkCkwUbndReq
*
*    Desc:    unpack the primitive CkwUbndReq
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     ckw.c
*
*/
#ifdef ANSI
S16 cmUnpkCkwUbndReq
(
CkwUbndReq     func,
Pst            *pst,
Buffer         *mBuf
)
#else
S16 cmUnpkCkwUbndReq(func, pst, mBuf)
CkwUbndReq     func;
Pst            *pst;
Buffer         *mBuf;
#endif
{
    SpId       spId = 0;
    Reason     reason = 0;
    
    TRC3(cmUnpkCkwUbndReq)

    CMCHKUNPKLOG(SUnpkS16, &spId, mBuf, ECKW026, pst);
    CMCHKUNPKLOG(SUnpkS16, &reason, mBuf, ECKW027, pst);
    SPutMsg(mBuf);
    return ((*func)(pst, spId, reason));
} /*end of function cmUnpkCkwUbndReq*/

/*
*
*    Fun:    cmUnpkCkwBndCfm
*
*    Desc:    unpack the primitive CkwBndCfm
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     ckw.c
*
*/
#ifdef ANSI
S16 cmUnpkCkwBndCfm
(
CkwBndCfm      func,
Pst            *pst,
Buffer         *mBuf
)
#else
S16 cmUnpkCkwBndCfm(func, pst, mBuf)
CkwBndCfm      func;
Pst            *pst;
Buffer         *mBuf;
#endif
{
    SuId       suId = 0;
    U8         status = 0;
    
    TRC3(cmUnpkCkwBndCfm)

    CMCHKUNPKLOG(SUnpkS16, &suId, mBuf, ECKW028, pst);
    CMCHKUNPKLOG(oduPackUInt8, &status, mBuf, ECKW029, pst);
    SPutMsg(mBuf);

    return ((*func)(pst, suId, status));
} /*end of function cmUnpkCkwBndCfm*/


/*
*
*    Fun:    cmUnpkRlcTmInfo
*
*    Desc:    pack the structure RlcTmInfo
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     ckw.c
*
*/
#ifdef ANSI
S16 cmUnpkRlcTmInfo
(
RlcTmInfo      *param,
Buffer         *mBuf
)
#else
S16 cmUnpkRlcTmInfo(param, mBuf)
RlcTmInfo      *param;
Buffer         *mBuf;
#endif
{
    TRC3(cmUnpkRlcTmInfo)

    CMCHKUNPK(oduPackUInt32, &(param->dl.buffSize), mBuf);

    return ROK;
} /* cmUnpkRlcTmInfo */


/*
*
*    Fun:    cmUnpkRlcUmInfo
*
*    Desc:    pack the structure RlcUmInfo
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     ckw.c
*
*/
#ifdef ANSI
S16 cmUnpkRlcUmInfo
(
RlcEntCfgInfo  *param,
Buffer         *mBuf
)
#else
S16 cmUnpkRlcUmInfo(param, mBuf)
RlcEntCfgInfo  *param;
Buffer         *mBuf;
#endif
{
    TRC3(cmUnpkRlcUmInfo)

    switch(param->dir)
    {
       case CKW_CFG_DIR_DL:
          {
             CMCHKUNPK(oduPackUInt8, &(param->m.umInfo.dl.snLen), mBuf);
             break;
          }
       case CKW_CFG_DIR_UL:
          {
             CMCHKUNPK(oduPackUInt8, &(param->m.umInfo.ul.snLen), mBuf);
             CMCHKUNPK(oduPackUInt8, &(param->m.umInfo.ul.reOrdTmr), mBuf);
             break;
          }
       /* Patch ckw_c_001.main_3 */
       case CKW_CFG_DIR_BOTH:
          {
             CMCHKUNPK(oduPackUInt8, &(param->m.umInfo.ul.snLen), mBuf);
             CMCHKUNPK(oduPackUInt8, &(param->m.umInfo.ul.reOrdTmr), mBuf);
             CMCHKUNPK(oduPackUInt8, &(param->m.umInfo.dl.snLen), mBuf);
             break;
          }
    }

    return ROK;
} /* cmUnpkRlcUmInfo */


/*
*
*    Fun:    cmUnpkRlcAmInfo
*
*    Desc:    pack the structure RlcAmInfo
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     ckw.c
*
*/
#ifdef ANSI
S16 cmUnpkRlcAmInfo
(
RlcAmInfo      *param,
Buffer         *mBuf
)
#else
S16 cmUnpkRlcAmInfo(param, mBuf)
RlcAmInfo      *param;
Buffer         *mBuf;
#endif
{
    TRC3(cmUnpkRlcAmInfo)

    /* DOWN LINK */
    CMCHKUNPK(oduPackUInt8, &(param->dl.snLen), mBuf);
    CMCHKUNPK(oduPackUInt16, &(param->dl.pollRetxTmr), mBuf);
    CMCHKUNPK(SUnpkS16, &(param->dl.pollPdu), mBuf);
    CMCHKUNPK(SUnpkS32, &(param->dl.pollByte), mBuf);
    CMCHKUNPK(oduPackUInt8, &(param->dl.maxRetx), mBuf);

    /* UP LINK */
    CMCHKUNPK(oduPackUInt8, &(param->ul.snLen), mBuf);
    CMCHKUNPK(oduPackUInt16, &(param->ul.staProhTmr), mBuf);
    CMCHKUNPK(oduPackUInt8, &(param->ul.reOrdTmr), mBuf);

    return ROK;
} /* cmUnpkRlcAmInfo */


/*
*
*    Fun:    cmUnpkCkwLChInfo
*
*    Desc:    pack the structure CkwLChInfo
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     ckw.c
*
*/
#ifdef ANSI
S16 cmUnpkCkwLChInfo
(
CkwLChInfo     *param,
Buffer         *mBuf
)
#else
S16 cmUnpkCkwLChInfo(param, mBuf)
CkwLChInfo     *param;
Buffer         *mBuf;
#endif
{
    TRC3(cmUnpkCkwLChInfo)

    CMCHKUNPK(oduPackUInt8, &(param->lChId), mBuf);
    CMCHKUNPK(oduPackUInt8, &(param->type), mBuf);

    return ROK;
} /* cmUnpkCkwLChInfo */


/*
*
*    Fun:    cmUnpkRlcEntCfgCfmInfo
*
*    Desc:    unpack the structure RlcEntCfgInfo
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     ckw.c
*
*/
#ifdef ANSI
S16 cmUnpkRlcEntCfgCfmInfo
(
RlcEntCfgCfmInfo  *param,
Buffer            *mBuf
)
#else
S16 cmUnpkRlcEntCfgCfmInfo(param, mBuf)
RlcEntCfgCfmInfo  *param;
Buffer            *mBuf;
#endif
{
    TRC3(cmUnpkRlcEntCfgCfmInfo)

    CMCHKUNPK(oduPackUInt8, &(param->rbId), mBuf);
    CMCHKUNPK(oduPackUInt8, &(param->rbType), mBuf);
    CMCHKUNPK(cmUnpkCmStatus, &(param->status), mBuf);

    return ROK;
} /* cmUnpkRlcEntCfgCfmInfo */


/*
*
*    Fun:    cmUnpkRlcCfgCfmInfo
*
*    Desc:    pack the structure RlcCfgCfmInfo
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     ckw.c
*
*/
#ifdef ANSI
S16 cmUnpkRlcCfgCfmInfo
(
RlcCfgCfmInfo  *param,
Pst            *pst,
Buffer         *mBuf
)
#else
S16 cmUnpkRlcCfgCfmInfo(param, pst, mBuf)
RlcCfgCfmInfo  *param;
Pst            *pst;
Buffer         *mBuf;
#endif
{
    U8         idx;

    TRC3(cmUnpkRlcCfgCfmInfo)

    CMCHKUNPKLOG(oduPackUInt32, &(param->transId), mBuf, ECKW030, pst);
    CMCHKUNPK(cmUnpkLteRnti, &(param->ueId), mBuf);
    CMCHKUNPK(cmUnpkLteCellId, &(param->cellId), mBuf);
    CMCHKUNPK(oduPackUInt8, &(param->numEnt), mBuf);

    for (idx = 0; idx < param->numEnt; idx++)
    {
       CMCHKUNPK(cmUnpkRlcEntCfgCfmInfo, &(param->entCfgCfm[idx]), mBuf);
    }

    return ROK;
} /* cmUnpkRlcCfgCfmInfo */


/*
*
*    Fun:    cmUnpkRlcEntCfgInfo
*
*    Desc:    pack the structure RlcEntCfgCfmInfo
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     ckw.c
*
*/
#ifdef ANSI
S16 cmUnpkRlcEntCfgInfo
(
RlcEntCfgInfo  *param,
Buffer         *mBuf
)
#else
S16 cmUnpkRlcEntCfgInfo(param, mBuf)
RlcEntCfgInfo  *param;
Buffer         *mBuf;
#endif
{
    TRC3(cmUnpkRlcEntCfgInfo)

    CMCHKUNPK(oduPackUInt8, &(param->cfgType), mBuf);
    CMCHKUNPK(oduPackUInt8, &(param->rbId), mBuf);
    CMCHKUNPK(oduPackUInt8, &(param->rbType), mBuf);
    CMCHKUNPK(oduPackUInt8, &(param->qci), mBuf);
    CMCHKUNPK(cmUnpkLteRlcMode, &(param->entMode), mBuf);
    CMCHKUNPK(oduPackUInt8, &(param->dir), mBuf);
    CMCHKUNPK(SUnpkS16, &(param->discardTmr), mBuf);
#ifdef TENB_MULT_CELL_SUPPRT
    CMCHKUNPK(SUnpkS16, &(param->rguSapId), mBuf);
#endif

    /* Logical Channel Information */
    switch(param->entMode)
    {
       case CM_LTE_MODE_TM:
          {
             CMCHKUNPK(cmUnpkCkwLChInfo, &(param->lCh[0]), mBuf);
             break;
          }
       /* ccpu00128195: Unpacking lCh[0] and lCh[1] for UM bearer as well */
       case CM_LTE_MODE_UM:
       case CM_LTE_MODE_AM:
          {
             CMCHKUNPK(cmUnpkCkwLChInfo, &(param->lCh[0]), mBuf);
             CMCHKUNPK(cmUnpkCkwLChInfo, &(param->lCh[1]), mBuf);
             break;
          }
    }

    /* TM/UM/AM Info */
    switch(param->entMode)
    {
       case CM_LTE_MODE_TM:
          {
             CMCHKUNPK(cmUnpkRlcTmInfo, &(param->m.tmInfo), mBuf);
             break;
          }
       case CM_LTE_MODE_UM:
          {
             CMCHKUNPK(cmUnpkRlcUmInfo, param, mBuf);
             break;
          }
       case CM_LTE_MODE_AM:
          {
             CMCHKUNPK(cmUnpkRlcAmInfo, &(param->m.amInfo), mBuf);
             break;
          }
    }

    return ROK;
} /* cmUnpkRlcEntCfgInfo */


/*
*
*    Fun:    cmUnpkRlcCfgInfo
*
*    Desc:    unpack the structure RlcCfgInfo
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     ckw.c
*
*/
#ifdef ANSI
S16 cmUnpkRlcCfgInfo
(
RlcCfgInfo     *param,
Pst            *pst,
Buffer         *mBuf
)
#else
S16 cmUnpkRlcCfgInfo(param, pst, mBuf)
RlcCfgInfo     *param;
Pst            *pst;
Buffer         *mBuf;
#endif
{
    U8         idx;

    TRC3(cmUnpkRlcCfgInfo)

    CMCHKUNPKLOG(oduPackUInt32, &(param->transId), mBuf, ECKW031, pst);
    CMCHKUNPK(cmUnpkLteRnti, &(param->ueId), mBuf);
    CMCHKUNPK(cmUnpkLteCellId, &(param->cellId), mBuf);
    CMCHKUNPK(oduPackUInt8, &(param->numEnt), mBuf);

    for (idx = 0; idx < param->numEnt; idx++)
    {
       CMCHKUNPKLOG(cmUnpkRlcEntCfgInfo,&(param->entCfg[idx]), mBuf, ECKW032, pst);
    }

    return ROK;
} /* cmUnpkRlcCfgInfo */


/*
*
*    Fun:    cmUnpkRlcCfgCfm
*
*    Desc:    unpack the primitive RlcUiRlcCfgCfm
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     ckw.c
*
*/
#ifdef ANSI
S16 cmUnpkRlcCfgCfm
(
RlcCfgCfm         func,
Pst               *pst,
Buffer            *mBuf
)
#else
S16 cmUnpkRlcCfgCfm(func, pst, mBuf)
RlcCfgCfm         func;
Pst               *pst;
Buffer            *mBuf;
#endif
{
    S16 ret1;
    SuId             suId = 0;
    RlcCfgCfmInfo    *cfgCfmInfo = NULLP;
    
    TRC3(cmUnpkRlcCfgCfm)

    if((ret1 = SGetSBuf(pst->region, pst->pool, (Data **)&cfgCfmInfo,\
                sizeof(RlcCfgCfmInfo))) != ROK)
    {
#if (ERRCLASS & ERRCLS_ADD_RES)
       if(ret1 != ROK)
       {
          SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
               (ErrVal)ECKW035, (ErrVal)0, "SGetMsg() failed");
       }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
       return (ret1);
    }

    cmMemset((U8 *)cfgCfmInfo, 0, sizeof(RlcCfgCfmInfo));

    CMCHKUNPK(SUnpkS16, &suId, mBuf);

    switch(pst->selector)
    {
#ifdef LCCKW
       case ODU_SELECTOR_LC:
       {
          ret1 = cmUnpkRlcCfgCfmInfo( (cfgCfmInfo), pst, mBuf);
#if(ERRCLASS & ERRCLS_DEBUG)
          if(ret1 != ROK)
          {
             SPutMsg(mBuf);
             SLogError(pst->dstEnt, pst->dstInst, pst->dstProcId,
                   __FILE__, __LINE__, (ErrCls)ERRCLS_DEBUG,
                  (ErrVal)ECKW036, (ErrVal)ret1, "Unpacking failure");
             return ( ret1 );
          }
#endif /* ERRCLASS & ERRCLS_DEBUG */
          break;
       }
#endif /* LCCKW */
    }
    SPutMsg(mBuf);

    return ((*func)(pst, suId, cfgCfmInfo));
} /* cmUnpkRlcCfgCfm */

/*
*
*    Fun:    cmUnpkCkwUeInfo
*
*    Desc:   Unpack the primitive KwUiCkwUeInfo
*
*    Ret:    ROK  -ok
*
*    Notes:  None
*
*    File:   ckw.c
*
*/
#ifdef ANSI
S16 cmUnpkCkwUeInfo
(
CkwUeInfo *param,
Buffer    *mBuf
)
#else
S16 cmUnpkCkwUeInfo(param, mBuf)
CkwUeInfo *param;
Buffer    *mBuf;
#endif
{
   TRC2(cmUnpkCkwUeInfo)

   CMCHKUNPK(cmUnpkLteRnti, &(param->ueId), mBuf);
   CMCHKUNPK(cmUnpkLteCellId, &(param->cellId), mBuf);

   return ROK;
}

/*
*
*    Fun:    cmUnpkCkwUeIdChgReq
*
*    Desc:    unpack the primitive RlcUiCkwUeIdChgReq
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     ckw.c
*
*/
#ifdef ANSI
S16 cmUnpkCkwUeIdChgReq
(
CkwUeIdChgReq     func,
Pst               *pst,
Buffer            *mBuf
)
#else
S16 cmUnpkCkwUeIdChgReq(func, pst, mBuf)
CkwUeIdChgReq     func;
Pst               *pst;
Buffer            *mBuf;
#endif
{
    S16       ret1;
    SpId      spId = 0;
    U32       transId = 0;
    CkwUeInfo *ueInfo = NULLP;
    CkwUeInfo *newUeInfo = NULLP;
    
    TRC3(cmUnpkCkwUeIdChgReq)


    CMCHKUNPK(SUnpkS16, &(spId), mBuf);
    CMCHKUNPKLOG(oduPackUInt32, &transId, mBuf, ECKW037, pst);

    if((ret1 = SGetSBuf(pst->region, pst->pool, (Data **)&ueInfo,\
                sizeof(CkwUeInfo))) != ROK)
    {
#if (ERRCLASS & ERRCLS_ADD_RES)
      /*MBUF_FIXX*/
          SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
               (ErrVal)ECKW038, (ErrVal)0, "SGetMsg() failed");
      
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
       return (ret1);
    }

    cmMemset((U8 *)ueInfo, 0, sizeof(CkwUeInfo));

    ret1 = cmUnpkCkwUeInfo((ueInfo), mBuf);
#if(ERRCLASS & ERRCLS_DEBUG)
    if(ret1 != ROK)
    {
       SPutMsg(mBuf);
       SLogError(pst->dstEnt, pst->dstInst, pst->dstProcId,
             __FILE__, __LINE__, (ErrCls)ERRCLS_DEBUG,
             (ErrVal)ECKW039, (ErrVal)ret1, "Unpacking failure");
       return ( ret1 );
    }
#endif /* ERRCLASS & ERRCLS_DEBUG */

    if((ret1 = SGetSBuf(pst->region, pst->pool, (Data **)&newUeInfo,\
                sizeof(CkwUeInfo))) != ROK)
    {
#if (ERRCLASS & ERRCLS_ADD_RES)
       /*MBUF_FIXX*/
          SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
               (ErrVal)ECKW040, (ErrVal)0, "SGetMsg() failed");
       
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
       return (ret1);
    }

    cmMemset((U8 *)newUeInfo, 0, sizeof(CkwUeInfo));

    ret1 = cmUnpkCkwUeInfo((newUeInfo), mBuf);
#if(ERRCLASS & ERRCLS_DEBUG)
    if(ret1 != ROK)
    {
       SPutMsg(mBuf);
       SLogError(pst->dstEnt, pst->dstInst, pst->dstProcId,
             __FILE__, __LINE__, (ErrCls)ERRCLS_DEBUG,
             (ErrVal)ECKW041, (ErrVal)ret1, "Unpacking failure");
       return ( ret1 );
    }
#endif /* ERRCLASS & ERRCLS_DEBUG */

    SPutMsg(mBuf);

    return ((*func)(pst, spId, transId, ueInfo, newUeInfo));

} /* cmUnpkCkwUeIdChgReq */

/*
*
*    Fun:     cmUnpkCkwUeIdChgCfm
*
*    Desc:    unpack the primitive RlcUiCkwUeIdChgCfm
*
*    Ret:     ROK  -ok
*
*    Notes:   None
*
*    File:    ckw.c
*
*/
#ifdef ANSI
S16 cmUnpkCkwUeIdChgCfm
(
CkwUeIdChgCfm     func,
Pst               *pst,
Buffer            *mBuf
)
#else
S16 cmUnpkCkwUeIdChgCfm(func, pst, mBuf)
CkwUeIdChgCfm     func;
Pst               *pst;
Buffer            *mBuf;
#endif
{
    S16       ret1;
    SuId      suId = 0;
    U32       transId = 0;
    CkwUeInfo *ueInfo = NULLP;
    CmStatus  status;
    
    TRC3(cmUnpkCkwUeIdChgCfm)

    cmMemset((U8 *)&status, (U8)0, (PTR)sizeof(CmStatus));

    CMCHKUNPK(SUnpkS16, &suId, mBuf);
    CMCHKUNPKLOG(oduPackUInt32, &transId, mBuf, ECKW042, pst);

    if((ret1 = SGetSBuf(pst->region, pst->pool, (Data **)&ueInfo,\
                sizeof(CkwUeInfo))) != ROK)
    {
#if (ERRCLASS & ERRCLS_ADD_RES)
       if(ret1 != ROK)
       {
          SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
               (ErrVal)ECKW043, (ErrVal)0, "SGetMsg() failed");
       }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
       return (ret1);
    }

    cmMemset((U8 *)ueInfo, 0, sizeof(CkwUeInfo));

    ret1 = cmUnpkCkwUeInfo((ueInfo), mBuf);
#if(ERRCLASS & ERRCLS_DEBUG)
    if(ret1 != ROK)
    {
       SPutMsg(mBuf);
       SLogError(pst->dstEnt, pst->dstInst, pst->dstProcId,
             __FILE__, __LINE__, (ErrCls)ERRCLS_DEBUG,
             (ErrVal)ECKW044, (ErrVal)ret1, "Unpacking failure");
       return ( ret1 );
    }
#endif /* ERRCLASS & ERRCLS_DEBUG */

    CMCHKUNPK(cmUnpkCmStatus, &status, mBuf);

    SPutMsg(mBuf);

    return ((*func)(pst, suId, transId, ueInfo, status));

} /* cmUnpkCkwUeIdChgCfm */

#endif /* LCCKW */
#ifdef __cplusplus
}
#endif /* __cplusplus */

  
/**********************************************************************
         End of file
**********************************************************************/
