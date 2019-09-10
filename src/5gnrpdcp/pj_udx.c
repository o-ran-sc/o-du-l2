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
  
        Name:    LTE PDCP Interface  
    
        Type:    C file
  
        Desc:    This file contains the pack/unpack functions for the
                 data structures at UDX Interface.
 
        File:    pj_udx.c
  
*********************************************************************21*/
#if(defined(LCUDX) || defined(LWLCUDX)) 
static const char* RLOG_MODULE_NAME="PDCP";
static int RLOG_MODULE_ID=1024;
static int RLOG_FILE_ID=224;
#endif

/* header include files (.h) */
  
#include "envopt.h"             /* Environment options */  
#include "envdep.h"             /* Environment dependent */
#include "envind.h"             /* Environment independent */
#include "gen.h"                /* General */
#include "ssi.h"                /* System services interface */
#include "cm_lte.h"             /* Common LTE header file */
#include "cm_llist.h"           /* Linked List header file */
#include "cm_tkns.h"            /* Common tokens header file */
#include "cm_hash.h"            /* Hash List header file */
#include "cpj.h"                /* CPJ Interface header file */
#include "lpj.h"
#include "pj_udx.h"



/* header/extern include files (.x) */

#include "gen.x"                /* General */
#include "ssi.x"                /* System services interface */
#include "cm_lib.x"             /* Common lib header file */
#include "cm_hash.x"            /* Hash List header file */
#include "cm_llist.x"           /* Linked List header file */
#include "cm_tkns.x"            /* Common tokens header file */
#include "cm_lte.x"             /* Common LTE header file */
#include "cpj.x"                /* CPJ Interface header file */
#include "pj.h"
#include "lpj.x"
#include "pj_udx.x"

#if(defined(LCUDX) || defined(LWLCUDX)) 
/* Packing/Unpacking Macros */

#ifdef ANSI
PUBLIC S16 cmPkPjUdxBndReq
(
Pst * pst,
SuId suId,
SpId spId
)
#else
PUBLIC S16 cmPkPjUdxBndReq(pst, suId, spId)
Pst * pst;
SuId suId;
SpId spId;
#endif
{
   Buffer *mBuf = NULLP;
   TRC3(cmPkPjUdxBndReq)

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EUDX001, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   if (SPkS16(spId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EUDX002, (ErrVal)0, "Packing failed");
#endif
      PJ_FREE_BUF(mBuf);
      RETVALUE(RFAILED);
   }
   if (SPkS16(suId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EUDX003, (ErrVal)0, "Packing failed");
#endif
      PJ_FREE_BUF(mBuf);
      RETVALUE(RFAILED);
   }
   pst->event = (Event) EVTUDXBNDREQ;
   RETVALUE(SPstTsk(pst,mBuf));
}

#ifdef ANSI
PUBLIC S16 cmUnpkPjUdxBndReq
(
UdxBndReq func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkPjUdxBndReq(func, pst, mBuf)
UdxBndReq func;
Pst *pst;
Buffer *mBuf;
#endif
{
   SuId suId;
   SpId spId;
   
   TRC3(cmUnpkPjUdxBndReq)

   if (SUnpkS16(&suId, mBuf) != ROK) {
      PJ_FREE_BUF(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EUDX004, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   if (SUnpkS16(&spId, mBuf) != ROK) {
      PJ_FREE_BUF(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EUDX005, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   PJ_FREE_BUF(mBuf);
   RETVALUE((*func)(pst, suId, spId));
}

#ifdef ANSI
PUBLIC S16 cmPkPjUdxBndCfm
(
Pst * pst,
SuId suId,
U8 status
)
#else
PUBLIC S16 cmPkPjUdxBndCfm(pst, suId, status)
Pst * pst;
SuId suId;
U8 status;
#endif
{
   Buffer *mBuf = NULLP;
   TRC3(cmPkPjUdxBndCfm)

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECPJ006, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   if (SPkU8(status, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECPJ007, (ErrVal)0, "Packing failed");
#endif
      PJ_FREE_BUF(mBuf);
      RETVALUE(RFAILED);
   }
   if (SPkS16(suId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECPJ008, (ErrVal)0, "Packing failed");
#endif
      PJ_FREE_BUF(mBuf);
      RETVALUE(RFAILED);
   }
   pst->event = (Event) EVTCPJBNDCFM;
   RETVALUE(SPstTsk(pst,mBuf));
}

#ifdef ANSI
PUBLIC S16 cmUnpkPjUdxBndCfm
(
UdxBndCfm func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkPjUdxBndCfm(func, pst, mBuf)
UdxBndCfm func;
Pst *pst;
Buffer *mBuf;
#endif
{
   SuId suId;
   U8 status;
   
   TRC3(cmUnpkPjUdxBndCfm)

   if (SUnpkS16(&suId, mBuf) != ROK) {
      PJ_FREE_BUF(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECPJ009, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   if (SUnpkU8(&status, mBuf) != ROK) {
      PJ_FREE_BUF(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECPJ010, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   PJ_FREE_BUF(mBuf);
   RETVALUE((*func)(pst, suId, status));
}

#ifdef ANSI
PUBLIC S16 cmPkPjUdxUbndReq
(
Pst * pst,
SpId spId,
Reason reason
)
#else
PUBLIC S16 cmPkPjUdxUbndReq(pst, spId, reason)
Pst * pst;
SpId spId;
Reason reason;
#endif
{
   Buffer *mBuf = NULLP;
   TRC3(cmPkPjUdxUbndReq)

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EUDX011, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }

   if (SPkS16(reason, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EUDX012, (ErrVal)0, "Packing failed");
#endif
      PJ_FREE_BUF(mBuf);
      RETVALUE(RFAILED);
   }
   if (SPkS16(spId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EUDX013, (ErrVal)0, "Packing failed");
#endif
      PJ_FREE_BUF(mBuf);
      RETVALUE(RFAILED);
   }
   pst->event = (Event) EVTUDXUBNDREQ;
   RETVALUE(SPstTsk(pst,mBuf));
}

#ifdef ANSI
PUBLIC S16 cmUnpkPjUdxUbndReq
(
UdxUbndReq func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkPjUdxUbndReq(func, pst, mBuf)
UdxUbndReq func;
Pst *pst;
Buffer *mBuf;
#endif
{
   SpId spId;
   Reason reason;
   
   TRC3(cmUnpkPjUdxUbndReq)

   if (SUnpkS16(&spId, mBuf) != ROK) {
      PJ_FREE_BUF(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EUDX014, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   if (SUnpkS16(&reason, mBuf) != ROK) {
      PJ_FREE_BUF(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EUDX015, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   PJ_FREE_BUF(mBuf);
   RETVALUE((*func)(pst, spId, reason));
}

#ifdef ANSI
PUBLIC S16 cmPkPjUdxCfgReq
(
Pst * pst,
SpId spId,
UdxCfgReqInfo * cfgReq
)
#else
PUBLIC S16 cmPkPjUdxCfgReq(pst, spId, cfgReq)
Pst * pst;
SpId spId;
UdxCfgReqInfo * cfgReq;
#endif
{
   Buffer *mBuf = NULLP;
   TRC3(cmPkPjUdxCfgReq)

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EUDX016, (ErrVal)0, "Packing failed");
#endif
      PJ_FREE_SHRABL_BUF_PST(pst->region, pst->pool, cfgReq, sizeof(UdxCfgReqInfo));
      RETVALUE(RFAILED);
   }

   switch(pst->selector)
   {
#ifdef LCUDX
      case UDX_SEL_LC:
         {

            if (cmPkPjUdxCfgReqInfo(pst, cfgReq, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
               SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                     __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                     (ErrVal)EUDX017, (ErrVal)0, "Packing failed");
#endif
               PJ_FREE_SHRABL_BUF_PST(pst->region, pst->pool, cfgReq, sizeof(UdxCfgReqInfo));
               PJ_FREE_BUF(mBuf);
               RETVALUE(RFAILED);
            }

            /* Free Request Structure after packing */
            PJ_FREE_SHRABL_BUF_PST(pst->region, pst->pool, cfgReq, sizeof(UdxCfgReqInfo));


            break;
         }
      case UDX_SEL_LWLC:
         {
            CMCHKPKLOG(cmPkPtr,(PTR)cfgReq,mBuf,EUDXXXX,pst);
            break;
         }
#endif /* LCUDX */
   }

    if (SPkS16(spId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EUDX019, (ErrVal)0, "Packing failed");
#endif
      PJ_FREE_SHRABL_BUF_PST(pst->region, pst->pool, cfgReq, sizeof(UdxCfgReqInfo));
      PJ_FREE_BUF(mBuf);
      RETVALUE(RFAILED);
   }
   pst->event = (Event) EVTUDXCFGREQ;

   RETVALUE(SPstTsk(pst,mBuf));
}

#ifdef ANSI
PUBLIC S16 cmPkPjUdxUlStaRep
(
Pst * pst,
SpId spId,
UdxUlStaRepInfo *staRep
)
#else
PUBLIC S16 cmPkPjUdxUlStaRep(pst, spId, staRep)
Pst * pst;
SpId spId;
UdxUlStaRepInfo *staRep;
#endif
{
   S16 ret;
   Buffer *mBuf;
   mBuf = NULLP;

   TRC3(cmPkPjUdxUlStaRep)

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EUDX016, (ErrVal)0, "Packing failed");
#endif
      PJ_FREE_BUF(mBuf);
      PJ_FREE_SHRABL_BUF_PST(pst->region, pst->pool, staRep, sizeof(UdxUlStaRepInfo));

      RETVALUE(RFAILED);
   }

   switch(pst->selector)
   {
#ifdef LCUDX
      case UDX_SEL_LC:
         {
            ret = cmPkPjUdxUlStaRepInfo( (staRep), pst, mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
            if(ret != ROK)
            {
               PJ_FREE_BUF(mBuf);
               PJ_FREE_BUF(staRep->sta);
               PJ_FREE_SHRABL_BUF_PST(pst->region, pst->pool, (Data *)staRep, sizeof(UdxUlStaRepInfo), 0);
               
               SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                     __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                     (ErrVal)EUDXXXX, (ErrVal)ret, "Packing failure");
               RETVALUE( ret );
            }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
            PJ_FREE_BUF(staRep->sta);
#ifndef XEON_SPECIFIC_CHANGES
            if (SPutStaticBuffer(pst->region, pst->pool, (Data *)staRep,
                     sizeof(UdxUlStaRepInfo), 0) != ROK)
#else
            if (SPutSBuf(pst->region, pst->pool, (Data *)staRep,
                     sizeof(UdxUlStaRepInfo)) != ROK)
#endif
            {
               PJ_FREE_BUF(mBuf);
               RETVALUE(RFAILED);
            }

            break;
         }
       case UDX_SEL_LWLC:
         {
            CMCHKPKLOG(cmPkPtr,(PTR)staRep,mBuf,EUDXXXX,pst);
            break;
         }
#endif /* LCUDX */
   }
   
   CMCHKPKLOG(SPkS16, spId, mBuf, EUDXXXX, pst);
   pst->event = (Event) EVTUDXULSTAREP;

   RETVALUE(SPstTsk(pst,mBuf));
}

#ifdef ANSI
PUBLIC S16 cmPkPjUdxDlStaRep
(
Pst * pst,
SpId spId,
UdxDlStaRepInfo *staRep
)
#else
PUBLIC S16 cmPkPjUdxDlStaRep(pst, spId, staRep)
Pst * pst;
SpId spId;
UdxDlStaRepInfo *staRep;
#endif
{
   S16 ret;
   Buffer *mBuf;
   mBuf = NULLP;

   TRC3(cmPkPjUdxDlStaRep)

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) 
   {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EUDX016, (ErrVal)0, "Packing failed");
#endif
      PJ_FREE_BUF(staRep->staPdu);
      PJ_FREE_SHRABL_BUF_PST(pst->region, pst->pool, staRep, sizeof(UdxDlStaRepInfo));

      RETVALUE(RFAILED);
   }

   switch(pst->selector)
   {
#ifdef LCUDX
      case UDX_SEL_LC:
         {
            ret = cmPkPjUdxDlStaRepInfo( (staRep), pst, mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
            if(ret != ROK)
            {
               PJ_FREE_BUF(mBuf);
               PJ_FREE_BUF(staRep->staPdu);
               PJ_FREE_SHRABL_BUF_PST(pst->region, pst->pool, staRep, sizeof(UdxDlStaRepInfo));
               
               SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                     __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                     (ErrVal)EUDXXXX, (ErrVal)ret, "Packing failure");
               RETVALUE( ret );
            }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
            PJ_FREE_BUF(staRep->staPdu);
#ifndef XEON_SPECIFIC_CHANGES
            if (SPutStaticBuffer(pst->region, pst->pool, (Data *)staRep,
                     sizeof(UdxDlStaRepInfo), 0) != ROK)
#else
            if (SPutSBuf(pst->region, pst->pool, (Data *)staRep,
                     sizeof(UdxDlStaRepInfo)) != ROK)
#endif
            {
               PJ_FREE_BUF(mBuf);
               RETVALUE(RFAILED);
            }

            break;
         }
       case UDX_SEL_LWLC:
         {
            CMCHKPKLOG(cmPkPtr,(PTR)staRep,mBuf,EUDXXXX,pst);
            break;
         }
#endif /* LCUDX */
   }
   
   CMCHKPKLOG(SPkS16, spId, mBuf, EUDXXXX, pst);
   pst->event = (Event) EVTUDXDLSTAREP;

   RETVALUE(SPstTsk(pst,mBuf));
}


#ifdef ANSI
PUBLIC S16 cmUnpkPjUdxCfgReq
(
UdxCfgReq func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkPjUdxCfgReq(func, pst, mBuf)
UdxCfgReq func;
Pst *pst;
Buffer *mBuf;
#endif
{
   S16   ret1;
   SpId spId;
   UdxCfgReqInfo *cfgReq;
   
   TRC3(cmUnpkPjUdxCfgReq)
   if (SUnpkS16(&spId, mBuf) != ROK) {
      PJ_FREE_BUF(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EUDX021, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }

   switch(pst->selector)
   {
#ifdef LCUDX
      case UDX_SEL_LC:
         {

            if((ret1 = SGetSBuf(pst->region, pst->pool, (Data **)&cfgReq,\
                        sizeof(UdxCfgReqInfo))) != ROK)
            {
#if (ERRCLASS & ERRCLS_ADD_RES)
               SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                     __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                     (ErrVal)EUDX040, (ErrVal)0, "SGetMsg() failed");
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
               RETVALUE(ret1);
            }

            if (cmUnpkPjUdxCfgReqInfo(pst, cfgReq, mBuf) != ROK) {
               PJ_FREE_SHRABL_BUF_PST(pst->region, pst->pool, cfgReq, sizeof(UdxCfgReqInfo);
               PJ_FREE_BUF(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
               SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                     __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                     (ErrVal)EUDX023, (ErrVal)0, "Packing failed");
#endif
               RETVALUE(RFAILED);
            }
            break;
         }
      case UDX_SEL_LWLC:
         {
            CMCHKUNPK(cmUnpkPtr,(PTR *) &cfgReq, mBuf);
            break;
         }
#endif /* LCUDX */
   }
   PJ_FREE_BUF(mBuf);
   RETVALUE((*func)(pst, spId, cfgReq));
}

#ifdef ANSI
PUBLIC S16 cmPkPjUdxCfgCfm
(
 Pst * pst,
 SuId suId,
 UdxCfgCfmInfo * cfgCfm
 )
#else
PUBLIC S16 cmPkPjUdxCfgCfm(pst, suId, cfgCfm)
        Pst * pst;
        SuId suId;
        UdxCfgCfmInfo * cfgCfm;
#endif
{
        Buffer *mBuf = NULLP;
        TRC3(cmPkPjUdxCfgCfm)

                if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
                        SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                                        __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                                        (ErrVal)ECPJ024, (ErrVal)0, "Packing failed");
#endif
                        PJ_FREE_SHRABL_BUF_PST(pst->region, pst->pool, cfgCfm, sizeof(UdxCfgCfmInfo));
                        RETVALUE(RFAILED);
                }
        switch(pst->selector)
        {
#ifdef LCUDX
           case UDX_SEL_LC:
              {

                 if (cmPkPjUdxCfgCfmInfo(cfgCfm, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
                    SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                          __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                          (ErrVal)ECPJ025, (ErrVal)0, "Packing failed");
#endif
                    PJ_FREE_SHRABL_BUF_PST(pst->region, pst->pool, cfgCfm, sizeof(UdxCfgCfmInfo));
                    PJ_FREE_BUF(mBuf);
                    RETVALUE(RFAILED);
                 }
                 /* cpj_c_001.main_3 */
                 /* Free Confirm Structure after packing */
                 PJ_FREE_SHRABL_BUF_PST(pst->region, pst->pool, cfgCfm, sizeof(UdxCfgCfmInfo));


                 break;
              }
           case UDX_SEL_LWLC:
              {
                 CMCHKPKLOG(cmPkPtr,(PTR)cfgCfm,mBuf,EUDXXXX,pst);
                 break;
              }
#endif /* LCUDX */

        }

        if (SPkS16(suId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
                SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                                __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                                (ErrVal)ECPJ027, (ErrVal)0, "Packing failed");
#endif
                PJ_FREE_SHRABL_BUF_PST(pst->region, pst->pool, cfgCfm, sizeof(UdxCfgCfmInfo));
                PJ_FREE_BUF(mBuf);
                RETVALUE(RFAILED);
        }
        pst->event = (Event) EVTCPJCFGCFM;
        RETVALUE(SPstTsk(pst,mBuf));
}

#ifdef ANSI
PUBLIC S16 cmUnpkPjUdxCfgCfm
(
 UdxCfgCfm func,
 Pst *pst,
 Buffer *mBuf
 )
#else
PUBLIC S16 cmUnpkPjUdxCfgCfm(func, pst, mBuf)
        UdxCfgCfm func;
        Pst *pst;
        Buffer *mBuf;
#endif
{
        S16   ret1;
        SuId suId;
        UdxCfgCfmInfo *cfgCfm;

        TRC3(cmUnpkPjUdxCfgCfm)
        if (SUnpkS16(&suId, mBuf) != ROK) {
                   PJ_FREE_BUF(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
                        SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                                        __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                                        (ErrVal)ECPJ029, (ErrVal)0, "Packing failed");
#endif
                        RETVALUE(RFAILED);
                }
        switch(pst->selector)
        {
#ifdef LCUDX
                case UDX_SEL_LC:
                        {


                                if((ret1 = SGetSBuf(pst->region, pst->pool, (Data **)&cfgCfm,\
                                                                sizeof(UdxCfgCfmInfo))) != ROK)
                                {
#if (ERRCLASS & ERRCLS_ADD_RES)
                                        SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                                                        __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                                                        (ErrVal)ECPJ040, (ErrVal)0, "SGetMsg() failed");
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
                                        RETVALUE(ret1);
                                }

                                if (cmUnpkPjUdxCfgCfmInfo(cfgCfm, mBuf) != ROK) {
                                        PJ_FREE_SHRABL_BUF_PST(pst->region, pst->pool, cfgCfm, sizeof(UdxCfgCfmInfo));
                                        PJ_FREE_BUF(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
                                        SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                                                        __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                                                        (ErrVal)ECPJ031, (ErrVal)0, "Packing failed");
#endif
                                        RETVALUE(RFAILED);
                                }
                                break;
                        }
                case UDX_SEL_LWLC:
                        {
                                CMCHKUNPK(cmUnpkPtr,(PTR *) &cfgCfm, mBuf);
                                break;
                        }
#endif /* LCUDX */
        }
        PJ_FREE_BUF(mBuf);
        RETVALUE((*func)(pst, suId, cfgCfm));
}


#ifdef ANSI
PUBLIC S16 cmPkPjUdxReEstReq
(
Pst * pst,
SpId spId,
UdxReEstReqInfo * reEstReq
)
#else
PUBLIC S16 cmPkPjUdxReEstReq(pst, spId, reEstReq)
Pst * pst;
SpId spId;
UdxReEstReqInfo * reEstReq;
#endif
{
   Buffer *mBuf = NULLP;
   TRC3(cmPkPjUdxReEstReq)

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK)
   {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EUDX032, (ErrVal)0, "Packing failed");
#endif
      PJ_FREE_SHRABL_BUF_PST(pst->region, pst->pool, reEstReq, sizeof(UdxReEstReqInfo));
      RETVALUE(RFAILED);
   }
   switch(pst->selector)
   {
#ifdef LCUDX
           case UDX_SEL_LC:
                   { 
                           if (cmPkPjUdxReEstReqInfo(reEstReq, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
                                   SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                                                   __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                                                   (ErrVal)EUDX033, (ErrVal)0, "Packing failed");
#endif
                                   PJ_FREE_SHRABL_BUF_PST(pst->region, pst->pool, reEstReq, sizeof(UdxReEstReqInfo));
                                   PJ_FREE_BUF(mBuf);
                                   RETVALUE(RFAILED);
                           }
                           /* Free Request Structure after packing */
                           PJ_FREE_SHRABL_BUF_PST(pst->region, pst->pool, reEstReq, sizeof(UdxReEstReqInfo));

                           break;
                   }
           case UDX_SEL_LWLC:
                   {
                           CMCHKPKLOG(cmPkPtr,(PTR)reEstReq,mBuf,EUDXXXX,pst);
                           break;
                   }
#endif /* LCUDX */
   }

   if (SPkS16(spId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EUDX035, (ErrVal)0, "Packing failed");
#endif
      PJ_FREE_SHRABL_BUF_PST(pst->region, pst->pool, reEstReq, sizeof(UdxReEstReqInfo));
      PJ_FREE_BUF(mBuf);
      RETVALUE(RFAILED);
   }

   pst->event = (Event) EVTUDXREESTREQ;
   RETVALUE(SPstTsk(pst,mBuf));
}

#ifdef ANSI
PUBLIC S16 cmUnpkPjUdxReEstReq
(
UdxReEstReq func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkPjUdxReEstReq(func, pst, mBuf)
UdxReEstReq func;
Pst *pst;
Buffer *mBuf;
#endif
{
   S16   ret1;
   SpId spId;
   UdxReEstReqInfo *reEstReq;
   
   TRC3(cmUnpkPjUdxReEstReq)

   if (SUnpkS16(&spId, mBuf) != ROK) {
      PJ_FREE_BUF(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EUDX037, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   switch(pst->selector)
   {
#ifdef LCUDX
           case UDX_SEL_LC:
                   { 

                           if((ret1 = SGetSBuf(pst->region, pst->pool, (Data **)&reEstReq,\
                                                           sizeof(UdxReEstReqInfo))) != ROK)
                           {
#if (ERRCLASS & ERRCLS_ADD_RES)
                                   SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                                                   __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                                                   (ErrVal)EUDX040, (ErrVal)0, "SGetMsg() failed");
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
                                   RETVALUE(ret1);
                           }

                           if (cmUnpkPjUdxReEstReqInfo(reEstReq, mBuf) != ROK) {
                                   PJ_FREE_SHRABL_BUF_PST(pst->region, pst->pool, reEstReq, sizeof(UdxReEstReqInfo));
                                   PJ_FREE_BUF(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
                                   SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                                                   __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                                                   (ErrVal)EUDX039, (ErrVal)0, "Packing failed");
#endif
                                   RETVALUE(RFAILED);
                           }
                           break;
                   }
           case UDX_SEL_LWLC:
                   {
                           CMCHKUNPK(cmUnpkPtr,(PTR *) &reEstReq, mBuf);
                           break;
                   }
#endif /* LCUDX */
}

   PJ_FREE_BUF(mBuf);
   RETVALUE((*func)(pst, spId, reEstReq));
}


#ifdef ANSI
PUBLIC S16 cmPkPjUdxReEstCfm
(
 Pst * pst,
 SuId suId,
 UdxReEstCfmInfo * reEstCfm
 )
#else
PUBLIC S16 cmPkPjUdxReEstCfm(pst, suId, reEstCfm)
        Pst * pst;
        SuId suId;
        UdxReEstCfmInfo * reEstCfm;
#endif
{
        Buffer *mBuf = NULLP;
        TRC3(cmPkPjUdxReEstCfm)

                if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
                        SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                                        __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                                        (ErrVal)ECPJ040, (ErrVal)0, "Packing failed");
#endif
                        PJ_FREE_SHRABL_BUF_PST(pst->region, pst->pool, reEstCfm, sizeof(UdxReEstCfmInfo));
                        RETVALUE(RFAILED);
                }
        switch(pst->selector)
        {
#ifdef LCUDX
                case UDX_SEL_LC:
                        { 

                                if (cmPkPjUdxReEstCfmInfo(reEstCfm, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
                                        SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                                                        __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                                                        (ErrVal)ECPJ041, (ErrVal)0, "Packing failed");
#endif
                                        PJ_FREE_SHRABL_BUF_PST(pst->region, pst->pool, reEstCfm, sizeof(UdxReEstCfmInfo));
                                        PJ_FREE_BUF(mBuf);
                                        RETVALUE(RFAILED);
                                }

                                /* Free Confirm Structure after packing */
                                PJ_FREE_SHRABL_BUF_PST(pst->region, pst->pool, reEstCfm, sizeof(UdxReEstCfmInfo));
                                break;
                        }
                case UDX_SEL_LWLC:
                        {
                                CMCHKPKLOG(cmPkPtr,(PTR)reEstCfm,mBuf,EUDXXXX,pst);
                                break;
                        }
#endif /* LCUDX */
        }
        if (SPkS16(suId, mBuf) != ROK)
        {
#if (ERRCLASS & ERRCLS_ADD_RES)
                SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                                __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                                (ErrVal)ECPJ043, (ErrVal)0, "Packing failed");
#endif
                PJ_FREE_SHRABL_BUF_PST(pst->region, pst->pool, reEstCfm, sizeof(UdxReEstCfmInfo));
                PJ_FREE_BUF(mBuf);
                RETVALUE(RFAILED);
        }


        pst->event = (Event) EVTCPJREESTCFM;
        RETVALUE(SPstTsk(pst,mBuf));
}

#ifdef ANSI
PUBLIC S16 cmUnpkPjUdxReEstCfm
(
 UdxReEstCfm func,
 Pst *pst,
 Buffer *mBuf
 )
#else
PUBLIC S16 cmUnpkPjUdxReEstCfm(func, pst, mBuf)
        UdxReEstCfm func;
        Pst *pst;
        Buffer *mBuf;
#endif
{
        S16   ret1;
        SuId suId;
        UdxReEstCfmInfo *reEstCfm;

        TRC3(cmUnpkPjUdxReEstCfm)

                if (SUnpkS16(&suId, mBuf) != ROK) {
                   PJ_FREE_BUF(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
                        SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                                        __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                                        (ErrVal)ECPJ045, (ErrVal)0, "Packing failed");
#endif
                        RETVALUE(RFAILED);
                }
        switch(pst->selector)
        {
#ifdef LCUDX
                case UDX_SEL_LC:
                        { 


                                if((ret1 = SGetSBuf(pst->region, pst->pool, (Data **)&reEstCfm,\
                                                                sizeof(UdxReEstCfmInfo))) != ROK)
                                {
#if (ERRCLASS & ERRCLS_ADD_RES)
                                        SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                                                        __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                                                        (ErrVal)ECPJ040, (ErrVal)0, "SGetMsg() failed");
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
                                        RETVALUE(ret1);
                                }

                                if (cmUnpkPjUdxReEstCfmInfo(reEstCfm, mBuf) != ROK) {
                                        PJ_FREE_SHRABL_BUF_PST(pst->region, pst->pool, reEstCfm, sizeof(UdxReEstCfmInfo));
                                        PJ_FREE_BUF(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
                                        SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                                                        __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                                                        (ErrVal)ECPJ047, (ErrVal)0, "Packing failed");
#endif
                                        RETVALUE(RFAILED);
                                }
                                break;
                        }
                case UDX_SEL_LWLC:
                        {
                                CMCHKUNPK(cmUnpkPtr,(PTR *) &reEstCfm, mBuf);
                                break;
                        }
#endif /* LCUDX */
        }

        PJ_FREE_BUF(mBuf);
        RETVALUE((*func)(pst, suId, reEstCfm));
}


#ifdef ANSI
PUBLIC S16 cmPkPjUdxSecCfgReq
(
Pst * pst,
SpId spId,
UdxSecCfgReqInfo * secCfgReq
)
#else
PUBLIC S16 cmPkPjUdxSecCfgReq(pst, spId, secCfgReq)
Pst * pst;
SpId spId;
UdxSecCfgReqInfo * secCfgReq;
#endif
{
   Buffer *mBuf = NULLP;
   TRC3(cmPkPjUdxSecCfgReq)

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EUDX048, (ErrVal)0, "Packing failed");
#endif
      PJ_FREE_SHRABL_BUF_PST(pst->region, pst->pool, secCfgReq, sizeof(UdxSecCfgReqInfo));
      RETVALUE(RFAILED);
   }

   switch(pst->selector)
   {
#ifdef LCUDX
           case UDX_SEL_LC:
                   {

                           if (cmPkPjUdxSecCfgReqInfo(secCfgReq, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
                                   SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                                                   __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                                                   (ErrVal)EUDX049, (ErrVal)0, "Packing failed");
#endif
                                   PJ_FREE_SHRABL_BUF_PST(pst->region, pst->pool, secCfgReq, sizeof(UdxSecCfgReqInfo));
                                   PJ_FREE_BUF(mBuf);
                                   RETVALUE(RFAILED);
                           }  
                           /* Free Request Structure after packing */
                           PJ_FREE_SHRABL_BUF_PST(pst->region, pst->pool, secCfgReq, sizeof(UdxSecCfgReqInfo));
                           break;

                   }
           case UDX_SEL_LWLC:
                   {
                           CMCHKPKLOG(cmPkPtr,(PTR)secCfgReq,mBuf,EUDXXXX,pst);
                           break;
                   }
#endif /* LCUDX */
   }

   if (SPkS16(spId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EUDX051, (ErrVal)0, "Packing failed");
#endif
      PJ_FREE_SHRABL_BUF_PST(pst->region, pst->pool, secCfgReq, sizeof(UdxSecCfgReqInfo));
      PJ_FREE_BUF(mBuf);
      RETVALUE(RFAILED);
   }

    pst->event = (Event) EVTUDXSECCFGREQ;
   RETVALUE(SPstTsk(pst,mBuf));
}

#ifdef ANSI
PUBLIC S16 cmUnpkPjUdxSecCfgReq
(
UdxSecCfgReq func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkPjUdxSecCfgReq(func, pst, mBuf)
UdxSecCfgReq func;
Pst *pst;
Buffer *mBuf;
#endif
{
   S16   ret1;
   SpId spId;
   UdxSecCfgReqInfo *secCfgReq;
   
   TRC3(cmUnpkPjUdxSecCfgReq)

   if (SUnpkS16(&spId, mBuf) != ROK) {
      PJ_FREE_BUF(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EUDX053, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   switch(pst->selector)
   {
#ifdef LCUDX
           case UDX_SEL_LC:
                   {

                           if((ret1 = SGetSBuf(pst->region, pst->pool, (Data **)&secCfgReq,\
                                                           sizeof(UdxSecCfgReqInfo))) != ROK)
                           {
#if (ERRCLASS & ERRCLS_ADD_RES)
                                   SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                                                   __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                                                   (ErrVal)EUDX040, (ErrVal)0, "SGetMsg() failed");
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
                                   RETVALUE(ret1);
                           }

                           if (cmUnpkPjUdxSecCfgReqInfo(secCfgReq, mBuf) != ROK) {
                                   PJ_FREE_SHRABL_BUF_PST(pst->region, pst->pool, secCfgReq, sizeof(UdxSecCfgReqInfo));
                                   PJ_FREE_BUF(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
                                   SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                                                   __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                                                   (ErrVal)EUDX055, (ErrVal)0, "Packing failed");
#endif
                                   RETVALUE(RFAILED);
                           }
                           break;

                   }
           case UDX_SEL_LWLC:
                   {
                           CMCHKUNPK(cmUnpkPtr,(PTR *) &secCfgReq,mBuf);
                           break;
                   }
#endif /* LCUDX */
   }
   PJ_FREE_BUF(mBuf);
   RETVALUE((*func)(pst, spId, secCfgReq));
}


#ifdef ANSI
PUBLIC S16 cmPkPjUdxSecCfgCfm
(
 Pst * pst,
 SuId suId,
 UdxSecCfgCfmInfo * secCfgCfm
 )
#else
PUBLIC S16 cmPkPjUdxSecCfgCfm(pst, suId, secCfgCfm)
        Pst * pst;
        SuId suId;
        UdxSecCfgCfmInfo * secCfgCfm;
#endif
{
        Buffer *mBuf = NULLP;
        TRC3(cmPkPjUdxSecCfgCfm)

                if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
                        SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                                        __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                                        (ErrVal)ECPJ056, (ErrVal)0, "Packing failed");
#endif
                        PJ_FREE_SHRABL_BUF_PST(pst->region, pst->pool, secCfgCfm, sizeof(UdxSecCfgCfmInfo));
                        RETVALUE(RFAILED);
                }
        switch(pst->selector)
        {
#ifdef LCUDX
                case UDX_SEL_LC:
                        {


                                if (cmPkPjUdxSecCfgCfmInfo(secCfgCfm, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
                                        SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                                                        __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                                                        (ErrVal)ECPJ057, (ErrVal)0, "Packing failed");
#endif
                                        PJ_FREE_SHRABL_BUF_PST(pst->region, pst->pool, secCfgCfm, sizeof(UdxSecCfgCfmInfo));
                                        PJ_FREE_BUF(mBuf);
                                        RETVALUE(RFAILED);
                                }
                                /* Free Confirm Structure after packing */
                                PJ_FREE_SHRABL_BUF_PST(pst->region, pst->pool, secCfgCfm, sizeof(UdxSecCfgCfmInfo));


                                break;

                        }
                case UDX_SEL_LWLC:
                        {
                                CMCHKPKLOG(cmPkPtr,(PTR)secCfgCfm,mBuf,EUDXXXX,pst);
                                break;
                        }
#endif /* LCUDX */
        }



        if (SPkS16(suId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
                SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                                __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                                (ErrVal)ECPJ059, (ErrVal)0, "Packing failed");
#endif
                PJ_FREE_SHRABL_BUF_PST(pst->region, pst->pool, secCfgCfm, sizeof(UdxSecCfgCfmInfo));
                PJ_FREE_BUF(mBuf);
                RETVALUE(RFAILED);
        }

        pst->event = (Event) EVTCPJSECCFGCFM;
        RETVALUE(SPstTsk(pst,mBuf));
}

#ifdef ANSI
PUBLIC S16 cmUnpkPjUdxSecCfgCfm
(
 UdxSecCfgCfm func,
 Pst *pst,
 Buffer *mBuf
 )
#else
PUBLIC S16 cmUnpkPjUdxSecCfgCfm(func, pst, mBuf)
        UdxSecCfgCfm func;
        Pst *pst;
        Buffer *mBuf;
#endif
{
        S16   ret1;
        SuId suId;
        UdxSecCfgCfmInfo *secCfgCfm;

        TRC3(cmUnpkPjUdxSecCfgCfm)

                if (SUnpkS16(&suId, mBuf) != ROK) {
                   PJ_FREE_BUF(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
                        SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                                        __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                                        (ErrVal)ECPJ061, (ErrVal)0, "Packing failed");
#endif
                        RETVALUE(RFAILED);
                }
        switch(pst->selector)
        {
#ifdef LCUDX
                case UDX_SEL_LC:
                        {


                                if((ret1 = SGetSBuf(pst->region, pst->pool, (Data **)&secCfgCfm,\
                                                                sizeof(UdxSecCfgCfmInfo))) != ROK)
                                {
#if (ERRCLASS & ERRCLS_ADD_RES)
                                        SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                                                        __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                                                        (ErrVal)ECPJ040, (ErrVal)0, "SGetMsg() failed");
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
                                        RETVALUE(ret1);
                                }

                                if (cmUnpkPjUdxSecCfgCfmInfo(secCfgCfm, mBuf) != ROK) {
                                        PJ_FREE_SHRABL_BUF_PST(pst->region, pst->pool, secCfgCfm, sizeof(UdxSecCfgCfmInfo));
                                        PJ_FREE_BUF(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
                                        SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                                                        __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                                                        (ErrVal)ECPJ063, (ErrVal)0, "Packing failed");
#endif
                                        RETVALUE(RFAILED);
                                }
                                break;

                        }
                case UDX_SEL_LWLC:
                        {
                                CMCHKUNPK(cmUnpkPtr,(PTR *)&secCfgCfm,mBuf);
                                break;
                        }
#endif /* LCUDX */
        }
        PJ_FREE_BUF(mBuf);
        RETVALUE((*func)(pst, suId, secCfgCfm));
}


#ifdef ANSI
PUBLIC S16 cmPkPjUdxUeIdChgReq
(
Pst * pst,
SpId spId,
U32 transId,
UdxUeInfo * oldUeInfo,
UdxUeInfo * newUeInfo
)
#else
PUBLIC S16 cmPkPjUdxUeIdChgReq(pst, spId, transId, oldUeInfo, newUeInfo)
Pst * pst;
SpId spId;
U32 transId;
UdxUeInfo * oldUeInfo;
UdxUeInfo * newUeInfo;
#endif
{
   Buffer *mBuf = NULLP;
   TRC3(cmPkPjUdxUeIdChgReq)

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EUDX064, (ErrVal)0, "Packing failed");
#endif
      PJ_FREE_SHRABL_BUF_PST(pst->region, pst->pool, oldUeInfo, sizeof(UdxUeInfo));
      PJ_FREE_SHRABL_BUF_PST(pst->region, pst->pool, newUeInfo, sizeof(UdxUeInfo));
      RETVALUE(RFAILED);
   }
   if (cmPkPjUdxUeInfo(newUeInfo, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)EUDX065, (ErrVal)0, "Packing failed");
#endif
         PJ_FREE_SHRABL_BUF_PST(pst->region, pst->pool, oldUeInfo, sizeof(UdxUeInfo));
         PJ_FREE_SHRABL_BUF_PST(pst->region, pst->pool, newUeInfo, sizeof(UdxUeInfo));
         PJ_FREE_BUF(mBuf);
         RETVALUE(RFAILED);
   }

   if (cmPkPjUdxUeInfo(oldUeInfo, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)EUDX067, (ErrVal)0, "Packing failed");
#endif
         PJ_FREE_SHRABL_BUF_PST(pst->region, pst->pool, oldUeInfo, sizeof(UdxUeInfo));
         PJ_FREE_SHRABL_BUF_PST(pst->region, pst->pool, newUeInfo, sizeof(UdxUeInfo));
         PJ_FREE_BUF(mBuf);
         RETVALUE(RFAILED);
   }

   if (SPkU32(transId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EUDX069, (ErrVal)0, "Packing failed");
#endif
      PJ_FREE_SHRABL_BUF_PST(pst->region, pst->pool, oldUeInfo, sizeof(UdxUeInfo));
      PJ_FREE_SHRABL_BUF_PST(pst->region, pst->pool, newUeInfo, sizeof(UdxUeInfo));
      PJ_FREE_BUF(mBuf);
      RETVALUE(RFAILED);
   }
   if (SPkS16(spId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EUDX070, (ErrVal)0, "Packing failed");
#endif
      PJ_FREE_SHRABL_BUF_PST(pst->region, pst->pool, oldUeInfo, sizeof(UdxUeInfo));
      PJ_FREE_SHRABL_BUF_PST(pst->region, pst->pool, newUeInfo, sizeof(UdxUeInfo));
      PJ_FREE_BUF(mBuf);
      RETVALUE(RFAILED);
   }

   /* Free Request Structure after packing */
   PJ_FREE_SHRABL_BUF_PST(pst->region, pst->pool, oldUeInfo, sizeof(UdxUeInfo));
   PJ_FREE_SHRABL_BUF_PST(pst->region, pst->pool, newUeInfo, sizeof(UdxUeInfo));

   pst->event = (Event) EVTUDXUEIDCHGREQ;
   RETVALUE(SPstTsk(pst,mBuf));
}

#ifdef ANSI
PUBLIC S16 cmUnpkPjUdxUeIdChgReq
(
UdxUeIdChgReq func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkPjUdxUeIdChgReq(func, pst, mBuf)
UdxUeIdChgReq func;
Pst *pst;
Buffer *mBuf;
#endif
{
   S16 ret1;
   SpId spId;
   U32 transId;
   UdxUeInfo *oldUeInfo;
   UdxUeInfo *newUeInfo;
   
   TRC3(cmUnpkPjUdxUeIdChgReq)

   if (SUnpkS16(&spId, mBuf) != ROK) {
      PJ_FREE_BUF(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EUDX073, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   if (SUnpkU32(&transId, mBuf) != ROK) {
      PJ_FREE_BUF(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EUDX074, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }

    if((ret1 = SGetSBuf(pst->region, pst->pool, (Data **)&oldUeInfo,\
                sizeof(UdxUeInfo))) != ROK)
    {
#if (ERRCLASS & ERRCLS_ADD_RES)
       SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
             __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
             (ErrVal)EUDX040, (ErrVal)0, "SGetMsg() failed");
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
       RETVALUE(ret1);
    }

   if (cmUnpkPjUdxUeInfo(oldUeInfo, mBuf) != ROK) {
      PJ_FREE_SHRABL_BUF_PST(pst->region, pst->pool, oldUeInfo, sizeof(UdxUeInfo));
      PJ_FREE_BUF(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EUDX076, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }

   if((ret1 = SGetSBuf(pst->region, pst->pool, (Data **)&newUeInfo,\
               sizeof(UdxUeInfo))) != ROK)
   {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)EUDX040, (ErrVal)0, "SGetMsg() failed");
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
      RETVALUE(ret1);
   }

   if (cmUnpkPjUdxUeInfo(newUeInfo, mBuf) != ROK)
   {
      PJ_FREE_SHRABL_BUF_PST(pst->region, pst->pool, oldUeInfo, sizeof(UdxUeInfo));
      PJ_FREE_SHRABL_BUF_PST(pst->region, pst->pool, newUeInfo, sizeof(UdxUeInfo));
      PJ_FREE_BUF(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EUDX078, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   PJ_FREE_BUF(mBuf);
   RETVALUE((*func)(pst, spId, transId, oldUeInfo, newUeInfo));
}


#ifdef ANSI
PUBLIC S16 cmPkPjUdxUeIdChgCfm
(
Pst * pst,
SuId suId,
U32 transId,
UdxUeInfo  * ueInfo,
CmStatus status
)
#else
PUBLIC S16 cmPkPjUdxUeIdChgCfm(pst, suId, transId, ueInfo, status)
Pst * pst;
SuId suId;
U32 transId;
UdxUeInfo  * ueInfo;
CmStatus status;
#endif
{
   Buffer *mBuf = NULLP;
   TRC3(cmPkPjUdxUeIdChgCfm)

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECPJ079, (ErrVal)0, "Packing failed");
#endif
      PJ_FREE_SHRABL_BUF_PST(pst->region, pst->pool, ueInfo, sizeof(UdxUeInfo));
      RETVALUE(RFAILED);
   }
   if (cmPkCmStatus(&status, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECPJ080, (ErrVal)0, "Packing failed");
#endif
      PJ_FREE_SHRABL_BUF_PST(pst->region, pst->pool, ueInfo, sizeof(UdxUeInfo));
      PJ_FREE_BUF(mBuf);
      RETVALUE(RFAILED);
   }
   if (cmPkPjUdxUeInfo(ueInfo, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ECPJ081, (ErrVal)0, "Packing failed");
#endif
         PJ_FREE_SHRABL_BUF_PST(pst->region, pst->pool, ueInfo, sizeof(UdxUeInfo));
         PJ_FREE_BUF(mBuf);
         RETVALUE(RFAILED);
   }

   if (SPkU32(transId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECPJ083, (ErrVal)0, "Packing failed");
#endif
      PJ_FREE_SHRABL_BUF_PST(pst->region, pst->pool, ueInfo, sizeof(UdxUeInfo));
      PJ_FREE_BUF(mBuf);
      RETVALUE(RFAILED);
   }
   if (SPkS16(suId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECPJ084, (ErrVal)0, "Packing failed");
#endif
      PJ_FREE_SHRABL_BUF_PST(pst->region, pst->pool, ueInfo, sizeof(UdxUeInfo));
      PJ_FREE_BUF(mBuf);
      RETVALUE(RFAILED);
   }

   /* Free Confirm Structure after packing */
   PJ_FREE_SHRABL_BUF_PST(pst->region, pst->pool, ueInfo, sizeof(UdxUeInfo));

   pst->event = (Event) EVTCPJUEIDCHGCFM;
   RETVALUE(SPstTsk(pst,mBuf));
}

#ifdef ANSI
PUBLIC S16 cmUnpkPjUdxUeIdChgCfm
(
UdxUeIdChgCfm func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkPjUdxUeIdChgCfm(func, pst, mBuf)
UdxUeIdChgCfm func;
Pst *pst;
Buffer *mBuf;
#endif
{
   S16   ret1;
   SuId suId;
   U32 transId;
   UdxUeInfo *ueInfo;
   CmStatus status;
   
   TRC3(cmUnpkPjUdxUeIdChgCfm)

   if (SUnpkS16(&suId, mBuf) != ROK) {
      PJ_FREE_BUF(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECPJ086, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   if (SUnpkU32(&transId, mBuf) != ROK) {
      PJ_FREE_BUF(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECPJ087, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }

   if((ret1 = SGetSBuf(pst->region, pst->pool, (Data **)&ueInfo,\
               sizeof(UdxUeInfo))) != ROK)
   {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ECPJ040, (ErrVal)0, "SGetMsg() failed");
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
      RETVALUE(ret1);
   }

   if (cmUnpkPjUdxUeInfo(ueInfo, mBuf) != ROK) {
      PJ_FREE_SHRABL_BUF_PST(pst->region, pst->pool, ueInfo, sizeof(UdxUeInfo));
      PJ_FREE_BUF(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECPJ089, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   if (cmUnpkCmStatus(&status, mBuf) != ROK) {
      PJ_FREE_SHRABL_BUF_PST(pst->region, pst->pool, ueInfo, sizeof(UdxUeInfo));
      PJ_FREE_BUF(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECPJ090, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   PJ_FREE_BUF(mBuf);
   RETVALUE((*func)(pst, suId, transId, ueInfo, status));
}

#ifdef ANSI
PUBLIC S16 cmPkPjUdxCountReq
(
Pst * pst,
SpId spId,
UdxCountReqInfo * countReq
)
#else
PUBLIC S16 cmPkPjUdxCountReq(pst, spId, countReq)
Pst * pst;
SpId spId;
UdxCountReqInfo * countReq;
#endif
{
   Buffer *mBuf = NULLP;
   TRC3(cmPkPjUdxCountReq)

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EUDX091, (ErrVal)0, "Packing failed");
#endif
      PJ_FREE_SHRABL_BUF_PST(pst->region, pst->pool, countReq, sizeof(UdxCountReqInfo));
      RETVALUE(RFAILED);
   }

   switch(pst->selector)
   {
#ifdef LCUDX
           case UDX_SEL_LC:
                   {

                           if (cmPkPjUdxCountReqInfo(countReq, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
                                   SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                                                   __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                                                   (ErrVal)EUDX092, (ErrVal)0, "Packing failed");
#endif
                                   PJ_FREE_SHRABL_BUF_PST(pst->region, pst->pool, countReq, sizeof(UdxCountReqInfo));
                                   PJ_FREE_BUF(mBuf);
                                   RETVALUE(RFAILED);
                           }
                           /* Free Request Structure after packing */
                           PJ_FREE_SHRABL_BUF_PST(pst->region, pst->pool, countReq, sizeof(UdxCountReqInfo));
                           break;
                   }
           case UDX_SEL_LWLC:
                   {
                           CMCHKPKLOG(cmPkPtr,(PTR)countReq,mBuf,EUDXXXX,pst);
                           break;
                   }
#endif /* LCUDX */
   }

   if (SPkS16(spId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EUDX094, (ErrVal)0, "Packing failed");
#endif
      PJ_FREE_SHRABL_BUF_PST(pst->region, pst->pool, countReq, sizeof(UdxCountReqInfo));
      PJ_FREE_BUF(mBuf);
      RETVALUE(RFAILED);
   }

   pst->event = (Event) EVTUDXCOUNTREQ;
   RETVALUE(SPstTsk(pst,mBuf));
}

#ifdef ANSI
PUBLIC S16 cmUnpkPjUdxCountReq
(
UdxCountReq func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkPjUdxCountReq(func, pst, mBuf)
UdxCountReq func;
Pst *pst;
Buffer *mBuf;
#endif
{
   S16   ret1;
   SpId spId;
   UdxCountReqInfo *countReq;
   
   TRC3(cmUnpkPjUdxCountReq)

   if (SUnpkS16(&spId, mBuf) != ROK) {
      PJ_FREE_BUF(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EUDX096, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   switch(pst->selector)
   {
#ifdef LCUDX
           case UDX_SEL_LC:
                   {

                           if((ret1 = SGetSBuf(pst->region, pst->pool, (Data **)&countReq,\
                                                           sizeof(UdxCountReqInfo))) != ROK)
                           {
#if (ERRCLASS & ERRCLS_ADD_RES)
                                   SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                                                   __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                                                   (ErrVal)EUDX040, (ErrVal)0, "SGetMsg() failed");
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
                                   RETVALUE(ret1);
                           }

                           if (cmUnpkPjUdxCountReqInfo(countReq, mBuf) != ROK) {
                                   PJ_FREE_SHRABL_BUF_PST(pst->region, pst->pool, countReq, sizeof(UdxCountReqInfo));
                                   PJ_FREE_BUF(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
                                   SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                                                   __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                                                   (ErrVal)EUDX098, (ErrVal)0, "Packing failed");
#endif
                                   RETVALUE(RFAILED);
                           }
                           break;
                   }
           case UDX_SEL_LWLC:
                   {
                           CMCHKUNPK(cmUnpkPtr,(PTR *) &countReq,mBuf);
                           break;
                   }
#endif /* LCUDX */
}

PJ_FREE_BUF(mBuf);
   RETVALUE((*func)(pst, spId, countReq));
}


#ifdef ANSI
PUBLIC S16 cmPkPjUdxCountCfm
(
Pst * pst,
SuId suId,
UdxCountCfmInfo * countCfm
)
#else
PUBLIC S16 cmPkPjUdxCountCfm(pst, suId, countCfm)
Pst * pst;
SuId suId;
UdxCountCfmInfo * countCfm;
#endif
{
   Buffer *mBuf = NULLP;
   TRC3(cmPkPjUdxCountCfm)

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECPJ099, (ErrVal)0, "Packing failed");
#endif
      PJ_FREE_SHRABL_BUF_PST(pst->region, pst->pool, countCfm, sizeof(UdxCountCfmInfo));
      RETVALUE(RFAILED);
   }
   switch(pst->selector)
   {
#ifdef LCUDX
           case UDX_SEL_LC:
                   {

   if (cmPkPjUdxCountCfmInfo(countCfm, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ECPJ100, (ErrVal)0, "Packing failed");
#endif
         PJ_FREE_SHRABL_BUF_PST(pst->region, pst->pool, countCfm, sizeof(UdxCountCfmInfo));
         PJ_FREE_BUF(mBuf);
         RETVALUE(RFAILED);
    }
   /* Free Confirm Structure after packing */
   PJ_FREE_SHRABL_BUF_PST(pst->region, pst->pool, countCfm, sizeof(UdxCountCfmInfo));

   break;
  }
  case UDX_SEL_LWLC:
                   {
                           CMCHKPKLOG(cmPkPtr,(PTR)countCfm,mBuf,EUDXXXX,pst);
                           break;
                   }
#endif /* LCUDX */
   }

   if (SPkS16(suId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECPJ102, (ErrVal)0, "Packing failed");
#endif
      PJ_FREE_SHRABL_BUF_PST(pst->region, pst->pool, countCfm, sizeof(UdxCountCfmInfo));
      PJ_FREE_BUF(mBuf);
      RETVALUE(RFAILED);
   }

   pst->event = (Event) EVTCPJCOUNTCFM;
   RETVALUE(SPstTsk(pst,mBuf));
}

#ifdef ANSI
PUBLIC S16 cmUnpkPjUdxCountCfm
(
UdxCountCfm func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkPjUdxCountCfm(func, pst, mBuf)
UdxCountCfm func;
Pst *pst;
Buffer *mBuf;
#endif
{
   S16   ret1;
   SuId suId;
   UdxCountCfmInfo *countCfm = NULLP;
   
   TRC3(cmUnpkPjUdxCountCfm)

   if (SUnpkS16(&suId, mBuf) != ROK) {
      PJ_FREE_BUF(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECPJ104, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }

   switch(pst->selector)
   {
#ifdef LCUDX
           case UDX_SEL_LC:
                   {


   if((ret1 = SGetSBuf(pst->region, pst->pool, (Data **)&countCfm,\
               sizeof(UdxCountCfmInfo))) != ROK)
   {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ECPJ040, (ErrVal)0, "SGetMsg() failed");
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
      RETVALUE(ret1);
   }

   if (cmUnpkPjUdxCountCfmInfo(countCfm, mBuf) != ROK) {
      PJ_FREE_SHRABL_BUF_PST(pst->region, pst->pool, countCfm, sizeof(UdxCountCfmInfo));
      PJ_FREE_BUF(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECPJ106, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
         break;
                   }
           case UDX_SEL_LWLC:
                   {
                           CMCHKUNPK(cmUnpkPtr,(PTR *) &countCfm,mBuf);
                           break;
                   }
#endif /* LCUDX */
}

PJ_FREE_BUF(mBuf);
   RETVALUE((*func)(pst, suId, countCfm));
}


#ifdef ANSI
PUBLIC S16 cmPkPjUdxSduStaReq
(
Pst * pst,
SpId spId,
UdxSduStaReqInfo * staReq
)
#else
PUBLIC S16 cmPkPjUdxSduStaReq(pst, spId, staReq)
Pst * pst;
SpId spId;
UdxSduStaReqInfo * staReq;
#endif
{
   Buffer *mBuf = NULLP;
   TRC3(cmPkPjUdxSduStaReq)

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EUDX107, (ErrVal)0, "Packing failed");
#endif
      PJ_FREE_SHRABL_BUF_PST(pst->region, pst->pool, staReq, sizeof(UdxSduStaReqInfo));
      RETVALUE(RFAILED);
   }

   switch(pst->selector)
   {
#ifdef LCUDX
           case UDX_SEL_LC:
                   {

                           if (cmPkPjUdxSduStaReqInfo(staReq, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
                                   SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                                                   __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                                                   (ErrVal)EUDX108, (ErrVal)0, "Packing failed");
#endif
                                   PJ_FREE_SHRABL_BUF_PST(pst->region, pst->pool, staReq, sizeof(UdxSduStaReqInfo));
                                   PJ_FREE_BUF(mBuf);
                                   RETVALUE(RFAILED);
                           }
                           /* Free Request Structure after packing */
                           PJ_FREE_SHRABL_BUF_PST(pst->region, pst->pool, staReq, sizeof(UdxSduStaReqInfo));


                           break;
                   }
           case UDX_SEL_LWLC:
                   {
                           CMCHKPKLOG(cmPkPtr,(PTR)staReq,mBuf,EUDXXXX,pst);
                           break;
                   }
#endif /* LCUDX */
}
   if (SPkS16(spId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EUDX110, (ErrVal)0, "Packing failed");
#endif
      PJ_FREE_SHRABL_BUF_PST(pst->region, pst->pool, staReq, sizeof(UdxSduStaReqInfo));
      PJ_FREE_BUF(mBuf);
      RETVALUE(RFAILED);
   }

   pst->event = (Event) EVTUDXSDUSTAREQ;
   RETVALUE(SPstTsk(pst,mBuf));
}

#ifdef ANSI
PUBLIC S16 cmUnpkPjUdxSduStaReq
(
UdxSduStaReq func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkPjUdxSduStaReq(func, pst, mBuf)
UdxSduStaReq func;
Pst *pst;
Buffer *mBuf;
#endif
{
   S16   ret1;
   SpId spId;
   UdxSduStaReqInfo *staReq;
   
   TRC3(cmUnpkPjUdxSduStaReq)

   if (SUnpkS16(&spId, mBuf) != ROK) {
      PJ_FREE_BUF(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EUDX112, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   switch(pst->selector)
   {
#ifdef LCUDX
           case UDX_SEL_LC:
                   {

                           if((ret1 = SGetSBuf(pst->region, pst->pool, (Data **)&staReq,\
                                                           sizeof(UdxSduStaReqInfo))) != ROK)
                           {
#if (ERRCLASS & ERRCLS_ADD_RES)
                                   SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                                                   __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                                                   (ErrVal)EUDX040, (ErrVal)0, "SGetMsg() failed");
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
                                   RETVALUE(ret1);
                           }

                           if (cmUnpkPjUdxSduStaReqInfo(staReq, mBuf) != ROK) {
                                   PJ_FREE_SHRABL_BUF_PST(pst->region, pst->pool, staReq, sizeof(UdxSduStaReqInfo));
                                   PJ_FREE_BUF(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
                                   SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                                                   __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                                                   (ErrVal)EUDX114, (ErrVal)0, "Packing failed");
#endif
                                   RETVALUE(RFAILED);
                           }
                           break;
                   }
           case UDX_SEL_LWLC:
                   {
                           CMCHKUNPK(cmUnpkPtr,(PTR *) &staReq,mBuf);
                           break;
                   }
#endif /* LCUDX */
}
PJ_FREE_BUF(mBuf);
   RETVALUE((*func)(pst, spId, staReq));
}


#ifdef ANSI
PUBLIC S16 cmPkPjUdxSduStaCfm
(
 Pst * pst,
 SuId suId,
 UdxSduStaCfmInfo * staCfm
 )
#else
PUBLIC S16 cmPkPjUdxSduStaCfm(pst, suId, staCfm)
        Pst * pst;
        SuId suId;
        UdxSduStaCfmInfo * staCfm;
#endif
{
        Buffer *mBuf = NULLP;
        TRC3(cmPkPjUdxSduStaCfm)

                if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
                        SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                                        __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                                        (ErrVal)ECPJ115, (ErrVal)0, "Packing failed");
#endif
                        PJ_FREE_SHRABL_BUF_PST(pst->region, pst->pool, staCfm, sizeof(UdxSduStaCfmInfo));
                        RETVALUE(RFAILED);
                }
        switch(pst->selector)
        {
#ifdef LCUDX
                case UDX_SEL_LC:
                        {


                                if (cmPkPjUdxSduStaCfmInfo(pst, staCfm, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
                                        SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                                                        __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                                                        (ErrVal)ECPJ116, (ErrVal)0, "Packing failed");
#endif
                                        PJ_FREE_SHRABL_BUF_PST(pst->region, pst->pool, staCfm, sizeof(UdxSduStaCfmInfo));
                                        PJ_FREE_BUF(mBuf);
                                        RETVALUE(RFAILED);
                                }
                                break;
                        }
                case UDX_SEL_LWLC:
                        {
                                CMCHKPKLOG(cmPkPtr,(PTR)staCfm,mBuf,EUDXXXX,pst);
                                break;
                        }
#endif /* LCUDX */
        }

        if (SPkS16(suId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
                SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                                __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                                (ErrVal)ECPJ118, (ErrVal)0, "Packing failed");
#endif
                PJ_FREE_SHRABL_BUF_PST(pst->region, pst->pool, staCfm, sizeof(UdxSduStaCfmInfo));
                PJ_FREE_BUF(mBuf);
                RETVALUE(RFAILED);
        }

        /* Free Confirm Structure after packing */
        /*SPutSBuf(pst->region, pst->pool, (Data *)staCfm, sizeof(UdxSduStaCfmInfo));*/

        pst->event = (Event) EVTUDXSDUSTACFM;
        RETVALUE(SPstTsk(pst,mBuf));
}

#ifdef ANSI
PUBLIC S16 cmUnpkPjUdxSduStaCfm
(
 UdxSduStaCfm func,
 Pst *pst,
 Buffer *mBuf
 )
#else
PUBLIC S16 cmUnpkPjUdxSduStaCfm(func, pst, mBuf)
        UdxSduStaCfm func;
        Pst *pst;
        Buffer *mBuf;
#endif
{
        S16   ret1;
        SuId suId;
        UdxSduStaCfmInfo *staCfm;

        TRC3(cmUnpkPjUdxSduStaCfm)

                if (SUnpkS16(&suId, mBuf) != ROK) {
                   PJ_FREE_BUF(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
                        SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                                        __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                                        (ErrVal)ECPJ120, (ErrVal)0, "Packing failed");
#endif
                        RETVALUE(RFAILED);
                }
        switch(pst->selector)
        {
#ifdef LCUDX
                case UDX_SEL_LC:
                {
                    if((ret1 = SGetSBuf(pst->region, pst->pool, (Data **)&staCfm,\
                                        sizeof(UdxSduStaCfmInfo))) != ROK)
                    {
#if (ERRCLASS & ERRCLS_ADD_RES)
                        SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                                  __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                                 (ErrVal)ECPJ040, (ErrVal)0, "SGetMsg() failed");
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
                        RETVALUE(ret1);
                    }

                    if (cmUnpkPjUdxSduStaCfmInfo(pst, staCfm, mBuf) != ROK) {
                            PJ_FREE_SHRABL_BUF_PST(pst->region, pst->pool, staCfm, sizeof(UdxSduStaCfmInfo));
                            PJ_FREE_BUF(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
                            SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                                            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                                            (ErrVal)ECPJ122, (ErrVal)0, "Packing failed");
#endif
                            RETVALUE(RFAILED);
                    }
                }
                break;

                case UDX_SEL_LWLC:
                {
                        CMCHKUNPK(cmUnpkPtr,(PTR *) &staCfm,mBuf);
                }
                break;
        }
#endif /* LCUDX */


        PJ_FREE_BUF(mBuf);
        RETVALUE((*func)(pst, suId, staCfm));
}

#ifdef ANSI
PUBLIC S16 cmPkPjUdxDatResumeCfm
(
 Pst * pst,
 SuId suId,
 UdxDatResumeCfmInfo * datResumeCfm
 )
#else
PUBLIC S16 cmPkPjUdxDatResumeCfm(pst, suId, datResumeCfm)
   Pst * pst;
   SuId suId;
   UdxDatResumeCfmInfo * datResumeCfm;
#endif
{
   Buffer *mBuf = NULLP;
   TRC3(cmPkPjUdxDatResumeCfm)

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ECPJ115, (ErrVal)0, "Packing failed");
#endif
      PJ_FREE_SHRABL_BUF_PST(pst->region, pst->pool, datResumeCfm, sizeof(UdxDatResumeCfmInfo));
      RETVALUE(RFAILED);
   }
   switch(pst->selector)
   {
#ifdef LCUDX
      case UDX_SEL_LC:
         {
            if (cmPkPjUdxDatResumeCfmInfo(pst, datResumeCfm, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
               SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                     __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                     (ErrVal)ECPJ116, (ErrVal)0, "Packing failed");
#endif
               PJ_FREE_SHRABL_BUF_PST(pst->region, pst->pool, datResumeCfm, sizeof(UdxDatResumeCfmInfo));
               PJ_FREE_BUF(mBuf);
               RETVALUE(RFAILED);
            }
            break;
         }
      case UDX_SEL_LWLC:
         {
            CMCHKPKLOG(cmPkPtr,(PTR)datResumeCfm,mBuf,EUDXXXX,pst);
            break;
         }
#endif /* LCUDX */
   }

   if (SPkS16(suId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ECPJ118, (ErrVal)0, "Packing failed");
#endif
      PJ_FREE_SHRABL_BUF_PST(pst->region, pst->pool, datResumeCfm, sizeof(UdxDatResumeCfmInfo));
      PJ_FREE_BUF(mBuf);
      RETVALUE(RFAILED);
   }

   /* Free Confirm Structure after packing */
   /*SPutSBuf(pst->region, pst->pool, (Data *)datResumeCfm, sizeof(UdxSduStaCfmInfo));*/

   pst->event = (Event) EVTUDXDATRESUMECFM;
   RETVALUE(SPstTsk(pst,mBuf));
}

/*
*
*    Fun:    cmPkPjUdxDatForwardCfm
*
*    Ret:    ROK  -ok
*
*    Notes:  None
*
*
*/
#ifdef ANSI
PUBLIC S16 cmPkPjUdxDatForwardCfm
(
 Pst * pst,
 SuId suId,
 UdxDatFwdCfmInfo  * datFwdCfm
 )
#else
PUBLIC S16 cmPkPjUdxDatForwardCfm(pst, suId, datFwdCfm)
   Pst * pst;
   SuId suId;
   UdxDatFwdCfmInfo  * datFwdCfm;
#endif
{

   Buffer *mBuf = NULLP;
   TRC3(cmPkPjUdxDatForwardCfm)

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ECPJ115, (ErrVal)0, "Packing failed");
#endif
      PJ_FREE_SHRABL_BUF_PST(pst->region, pst->pool, datFwdCfm, sizeof(UdxDatFwdCfmInfo));
      RETVALUE(RFAILED);
   }

   switch(pst->selector)
   {
#ifdef LCUDX
      case UDX_SEL_LC:
         {
            if (cmPkPjUdxDatForwardCfmInfo(pst, datFwdCfm, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
               SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                     __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                     (ErrVal)ECPJ116, (ErrVal)0, "Packing failed");
#endif
               PJ_FREE_SHRABL_BUF_PST(pst->region, pst->pool, datFwdCfm, sizeof(UdxDatFwdCfmInfo));
               PJ_FREE_BUF(mBuf);
               RETVALUE(RFAILED);
            }
            break;
         }
      case UDX_SEL_LWLC:
         {
            CMCHKPKLOG(cmPkPtr,(PTR)datFwdCfm,mBuf,EUDXXXX,pst);
            break;
         }
#endif /* LCUDX */
   }

   if (SPkS16(suId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ECPJ118, (ErrVal)0, "Packing failed");
#endif
      PJ_FREE_SHRABL_BUF_PST(pst->region, pst->pool, datFwdCfm, sizeof(UdxDatFwdCfmInfo));
      PJ_FREE_BUF(mBuf);
      RETVALUE(RFAILED);
   }

   /* Free Confirm Structure after packing */
   /*SPutSBuf(pst->region, pst->pool, (Data *)datFwdCfm, sizeof(UdxDatFwdCfmInfo));*/

   pst->event = (Event) EVTUDXDATFRWDCFM;
   RETVALUE(SPstTsk(pst,mBuf));
}

#ifdef ANSI
PUBLIC S16 cmUnpkPjUdxDatResumeCfm
(
 UdxDatResumeCfm func,
 Pst *pst,
 Buffer *mBuf
 )
#else
PUBLIC S16 cmUnpkPjUdxDatResumeCfm(func, pst, mBuf)
   UdxDatResumeCfm func;
   Pst *pst;
   Buffer *mBuf;
#endif
{
   S16   ret1;
   SuId suId;
   UdxDatResumeCfmInfo *datResumeCfm = NULLP;

   TRC3(cmUnpkPjUdxDatResumeCfm)

   RLOG0(L_DEBUG," Unpacking UdxDatResumeCfm");
   if (SUnpkS16(&suId, mBuf) != ROK) {
      PJ_FREE_BUF(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ECPJ120, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   switch(pst->selector)
   {
#ifdef LCUDX
      case UDX_SEL_LC:
         {
            if((ret1 = SGetSBuf(pst->region, pst->pool, (Data **)&datResumeCfm,\
                        sizeof(UdxDatResumeCfmInfo))) != ROK)
            {
#if (ERRCLASS & ERRCLS_ADD_RES)
               SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                     __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                     (ErrVal)ECPJ040, (ErrVal)0, "SGetMsg() failed");
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
               RETVALUE(ret1);
            }

            if (cmUnpkPjUdxDatResumeCfmInfo(pst, datResumeCfm, mBuf) != ROK) {
               PJ_FREE_SHRABL_BUF_PST(pst->region, pst->pool, datResumeCfm, sizeof(UdxDatResumeCfmInfo));
               PJ_FREE_BUF(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
               SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                     __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                     (ErrVal)ECPJ122, (ErrVal)0, "Packing failed");
#endif
               RETVALUE(RFAILED);
            }
         }
         break;

      case UDX_SEL_LWLC:
         {
            CMCHKUNPK(cmUnpkPtr,(PTR *) &datResumeCfm,mBuf);
         }
         break;
   }
#endif /* LCUDX */


   PJ_FREE_BUF(mBuf);
   RETVALUE((*func)(pst, suId, datResumeCfm));
}

#ifdef ANSI
PUBLIC S16 cmUnpkPjUdxDatForwardCfm
(
 UdxDatForwardCfm func,
 Pst *pst,
 Buffer *mBuf
 )
#else
PUBLIC S16 cmUnpkPjUdxDatForwardCfm(func, pst, mBuf)
   UdxDatForwardCfm func;
   Pst *pst;
   Buffer *mBuf;
#endif
{
   S16   ret1;
   SuId suId;
   UdxDatFwdCfmInfo *datFwdCfm;

   TRC3(cmUnpkPjUdxDatForwardCfm)

   if (SUnpkS16(&suId, mBuf) != ROK) {
      PJ_FREE_BUF(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ECPJ120, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   switch(pst->selector)
   {
#ifdef LCUDX
      case UDX_SEL_LC:
         {
            if((ret1 = SGetSBuf(pst->region, pst->pool, (Data **)&datFwdCfm,\
                        sizeof(UdxDatFwdCfmInfo))) != ROK)
            {
#if (ERRCLASS & ERRCLS_ADD_RES)
               SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                     __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                     (ErrVal)ECPJ040, (ErrVal)0, "SGetMsg() failed");
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
               RETVALUE(ret1);
            }

            if (cmUnpkPjUdxDatForwardCfmInfo(pst, datFwdCfm, mBuf) != ROK) {
               PJ_FREE_SHRABL_BUF_PST(pst->region, pst->pool, datFwdCfm, sizeof(UdxDatFwdCfmInfo));
               PJ_FREE_BUF(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
               SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                     __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                     (ErrVal)ECPJ122, (ErrVal)0, "Packing failed");
#endif
               RETVALUE(RFAILED);
            }
         }
         break;

      case UDX_SEL_LWLC:
         {
            CMCHKUNPK(cmUnpkPtr,(PTR *) &datFwdCfm,mBuf);
         }
         break;
   }
#endif /* LCUDX */

   PJ_FREE_BUF(mBuf);
   RETVALUE((*func)(pst, suId, datFwdCfm));
}



/***********************************************************
*
*     Func : cmPkPjUdxRlcAm
*
*
*     Desc :  * RLC parameter information in AM mode.
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
PUBLIC S16 cmPkPjUdxRlcAm
(
UdxRlcAm *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmPkPjUdxRlcAm(param, mBuf)
UdxRlcAm *param;
Buffer *mBuf;
#endif
{

   TRC3(cmPkPjUdxRlcAm)

   CMCHKPK(SPkU8, param->statusRepReqd, mBuf);
   RETVALUE(ROK);
}



/***********************************************************
*
*     Func : cmUnpkPjUdxRlcAm
*
*
*     Desc :  * RLC parameter information in AM mode.
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
PUBLIC S16 cmUnpkPjUdxRlcAm
(
UdxRlcAm *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkPjUdxRlcAm(param, mBuf)
UdxRlcAm *param;
Buffer *mBuf;
#endif
{

   TRC3(cmUnpkPjUdxRlcAm)

   CMCHKUNPK(SUnpkU8, &param->statusRepReqd, mBuf);
   RETVALUE(ROK);
}



/***********************************************************
*
*     Func : cmPkPjUdxRlcUm
*
*
*     Desc : * RLC parameter information in UM mode.
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
PUBLIC S16 cmPkPjUdxRlcUm
(
UdxRlcUm *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmPkPjUdxRlcUm(param, mBuf)
UdxRlcUm *param;
Buffer *mBuf;
#endif
{

   TRC3(cmPkPjUdxRlcUm)

   CMCHKPK(SPkU8, param->pdcpSNSize, mBuf);
   RETVALUE(ROK);
}



/***********************************************************
*
*     Func : cmUnpkPjUdxRlcUm
*
*
*     Desc : * RLC parameter information in UM mode.
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
PUBLIC S16 cmUnpkPjUdxRlcUm
(
UdxRlcUm *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkPjUdxRlcUm(param, mBuf)
UdxRlcUm *param;
Buffer *mBuf;
#endif
{

   TRC3(cmUnpkPjUdxRlcUm)

   CMCHKUNPK(SUnpkU8, &param->pdcpSNSize, mBuf);
   RETVALUE(ROK);
}



/***********************************************************
*
*     Func : cmPkPjUdxRlcInfo
*
*
*     Desc : * Mapped RLC entity information in RLC for the PDCP entity
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
PUBLIC S16 cmPkPjUdxRlcInfo
(
UdxRlcInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmPkPjUdxRlcInfo(param, mBuf)
UdxRlcInfo *param;
Buffer *mBuf;
#endif
{

   TRC3(cmPkPjUdxRlcInfo)

      switch(param->mode) {
         case CM_LTE_MODE_UM:
            CMCHKPK(cmPkPjUdxRlcUm, &param->m.rlcUm, mBuf);
            break;
         case CM_LTE_MODE_AM:
            CMCHKPK(cmPkPjUdxRlcAm, &param->m.rlcAm, mBuf);
            break;
         default :
            RETVALUE(RFAILED);
      }
   CMCHKPK(SPkU8, param->mode, mBuf);
   CMCHKPK(SPkU8, param->dir, mBuf);
   RETVALUE(ROK);
}



/***********************************************************
*
*     Func : cmUnpkPjUdxRlcInfo
*
*
*     Desc : * Mapped RLC entity information in RLC for the PDCP entity
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
PUBLIC S16 cmUnpkPjUdxRlcInfo
(
UdxRlcInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkPjUdxRlcInfo(param, mBuf)
UdxRlcInfo *param;
Buffer *mBuf;
#endif
{

   TRC3(cmUnpkPjUdxRlcInfo)

   CMCHKUNPK(SUnpkU8, &param->dir, mBuf);
   CMCHKUNPK(SUnpkU8, &param->mode, mBuf);
      switch(param->mode) {
         case CM_LTE_MODE_AM:
            CMCHKUNPK(cmUnpkPjUdxRlcAm, &param->m.rlcAm, mBuf);
            break;
         case CM_LTE_MODE_UM:
            CMCHKUNPK(cmUnpkPjUdxRlcUm, &param->m.rlcUm, mBuf);
            break;
         default :
            RETVALUE(RFAILED);
      }
   RETVALUE(ROK);
}



/***********************************************************
*
*     Func : cmPkPjUdxCipherInfo
*
*
*     Desc : * Ciphering information to be configured per UE in PDCP.
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
PUBLIC S16 cmPkPjUdxCipherInfo
(
UdxCipherInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmPkPjUdxCipherInfo(param, mBuf)
UdxCipherInfo *param;
Buffer *mBuf;
#endif
{

   S32 i;
   TRC3(cmPkPjUdxCipherInfo)

   CMCHKPK(SPkU8, param->algoType, mBuf);
   for (i=UDX_CIPHER_KEY_LEN-1; i >= 0; i--) {
      CMCHKPK(SPkU8, param->usrplnKey[i], mBuf);
   }
   for (i=UDX_CIPHER_KEY_LEN-1; i >= 0; i--) {
      CMCHKPK(SPkU8, param->ctrlplnKey[i], mBuf);
   }
   RETVALUE(ROK);
}



/***********************************************************
*
*     Func : cmUnpkPjUdxCipherInfo
*
*
*     Desc : * Ciphering information to be configured per UE in PDCP.
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
PUBLIC S16 cmUnpkPjUdxCipherInfo
(
UdxCipherInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkPjUdxCipherInfo(param, mBuf)
UdxCipherInfo *param;
Buffer *mBuf;
#endif
{

   S32 i;
   TRC3(cmUnpkPjUdxCipherInfo)

   for (i=0; i<UDX_CIPHER_KEY_LEN; i++) {
      CMCHKUNPK(SUnpkU8, &param->ctrlplnKey[i], mBuf);
   }
   for (i=0; i<UDX_CIPHER_KEY_LEN; i++) {
      CMCHKUNPK(SUnpkU8, &param->usrplnKey[i], mBuf);
   }
   CMCHKUNPK(SUnpkU8, &param->algoType, mBuf);
   RETVALUE(ROK);
}



/***********************************************************
*
*     Func : cmPkPjUdxIntegInfo
*
*
*     Desc : * Integrity protection paramater information.
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
PUBLIC S16 cmPkPjUdxIntegInfo
(
UdxIntegInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmPkPjUdxIntegInfo(param, mBuf)
UdxIntegInfo *param;
Buffer *mBuf;
#endif
{

   S32 i;
   TRC3(cmPkPjUdxIntegInfo)

   for (i=UDX_INTG_KEY_LEN-1; i >= 0; i--) {
      CMCHKPK(SPkU8, param->intKey[i], mBuf);
   }
   CMCHKPK(SPkU8, param->algoType, mBuf);
   RETVALUE(ROK);
}



/***********************************************************
*
*     Func : cmUnpkPjUdxIntegInfo
*
*
*     Desc : * Integrity protection paramater information.
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
PUBLIC S16 cmUnpkPjUdxIntegInfo
(
UdxIntegInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkPjUdxIntegInfo(param, mBuf)
UdxIntegInfo *param;
Buffer *mBuf;
#endif
{

   S32 i;
   TRC3(cmUnpkPjUdxIntegInfo)

   CMCHKUNPK(SUnpkU8, &param->algoType, mBuf);
   for (i=0; i<UDX_INTG_KEY_LEN; i++) {
      CMCHKUNPK(SUnpkU8, &param->intKey[i], mBuf);
   }
   RETVALUE(ROK);
}


/***********************************************************
*
*     Func : cmPkPjUdxSecCfg
*
*
*     Desc : * PDCP Security configuration parameter.
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
PUBLIC S16 cmPkPjUdxSecCfg
(
UdxSecCfg *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmPkPjUdxSecCfg(param, mBuf)
UdxSecCfg *param;
Buffer *mBuf;
#endif
{

   TRC3(cmPkPjUdxSecCfg)

   CMCHKPK(cmPkPjUdxCipherInfo, &param->cipherInfo, mBuf);
   CMCHKPK(cmPkPjUdxIntegInfo, &param->integInfo, mBuf);
   CMCHKPK(SPkU8, param->selSecAct, mBuf);
   CMCHKPK(SPkU8, param->isHo, mBuf);
   RETVALUE(ROK);
}



/***********************************************************
*
*     Func : cmUnpkPjUdxSecCfg
*
*
*     Desc : * PDCP Security configuration parameter.
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
PUBLIC S16 cmUnpkPjUdxSecCfg
(
UdxSecCfg *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkPjUdxSecCfg(param, mBuf)
UdxSecCfg *param;
Buffer *mBuf;
#endif
{

   TRC3(cmUnpkPjUdxSecCfg)

   CMCHKUNPK(SUnpkU8, &param->isHo, mBuf);
   CMCHKUNPK(SUnpkU8, &param->selSecAct, mBuf);
   CMCHKUNPK(cmUnpkPjUdxIntegInfo, &param->integInfo, mBuf);
   CMCHKUNPK(cmUnpkPjUdxCipherInfo, &param->cipherInfo, mBuf);
   RETVALUE(ROK);
}


/***********************************************************
*
*     Func : cmPkPjUdxRohcInfo
*
*
*     Desc : * ROHC compression protocol information structure.
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
PUBLIC S16 cmPkPjUdxRohcInfo
(
UdxRohcInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmPkPjUdxRohcInfo(param, mBuf)
UdxRohcInfo *param;
Buffer *mBuf;
#endif
{

   S32 i;
   TRC3(cmPkPjUdxRohcInfo)

   for (i=UDX_MAX_ROHC_PROF_LIST-1; i >= 0; i--) {
      CMCHKPK(SPkU8, param->profLst[i], mBuf);
   }
   CMCHKPK(SPkU16, param->maxCid, mBuf);
   RETVALUE(ROK);
}



/***********************************************************
*
*     Func : cmUnpkPjUdxRohcInfo
*
*
*     Desc : * ROHC compression protocol information structure.
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
PUBLIC S16 cmUnpkPjUdxRohcInfo
(
UdxRohcInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkPjUdxRohcInfo(param, mBuf)
UdxRohcInfo *param;
Buffer *mBuf;
#endif
{

   S32 i;
   TRC3(cmUnpkPjUdxRohcInfo)

   CMCHKUNPK(SUnpkU16, &param->maxCid, mBuf);
   for (i=0; i<UDX_MAX_ROHC_PROF_LIST; i++) {
      CMCHKUNPK(SUnpkU8, &param->profLst[i], mBuf);
   }
   RETVALUE(ROK);
}



/***********************************************************
*
*     Func : cmPkPjUdxHdrCompCfg
*
*
*     Desc : * ROHC configuration information for PDCP entity.
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
PUBLIC S16 cmPkPjUdxHdrCompCfg
(
UdxHdrCompCfg *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmPkPjUdxHdrCompCfg(param, mBuf)
UdxHdrCompCfg *param;
Buffer *mBuf;
#endif
{

   TRC3(cmPkPjUdxHdrCompCfg)

   CMCHKPK(cmPkPjUdxRohcInfo, &param->rohcInfo, mBuf);
   CMCHKPK(SPkU8, param->hdrCompUsed, mBuf);
   RETVALUE(ROK);
}



/***********************************************************
*
*     Func : cmUnpkPjUdxHdrCompCfg
*
*
*     Desc : * ROHC configuration information for PDCP entity.
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
PUBLIC S16 cmUnpkPjUdxHdrCompCfg
(
UdxHdrCompCfg *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkPjUdxHdrCompCfg(param, mBuf)
UdxHdrCompCfg *param;
Buffer *mBuf;
#endif
{

   TRC3(cmUnpkPjUdxHdrCompCfg)

   CMCHKUNPK(SUnpkU8, &param->hdrCompUsed, mBuf);
   CMCHKUNPK(cmUnpkPjUdxRohcInfo, &param->rohcInfo, mBuf);
   RETVALUE(ROK);
}



/***********************************************************
*
*     Func : cmPkPjUdxHoInfo
*
*
*     Desc : * Handover Information for RB.
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
PUBLIC S16 cmPkPjUdxHoInfo
(
Pst *pst,
UdxHoInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmPkPjUdxHoInfo(pst, param, mBuf)
Pst *pst;
UdxHoInfo *param;
Buffer *mBuf;
#endif
{
   S16   cnt, idx;

   TRC3(cmPkPjUdxHoInfo)

   if (param->numBits % 8 == 0)
      cnt = param->numBits/8;
   else
      cnt = param->numBits/8 + 1;

   for (idx = cnt-1; idx >= 0; idx--)
   {
      CMCHKPK(SPkU8, param->ulBitMap[idx], mBuf);
   }

   /* Free ulBitMap after packing */
   if (cnt)
      PJ_FREE_SHRABL_BUF_PST(pst->region, pst->pool, param->ulBitMap, cnt * sizeof(U8));

   CMCHKPK(SPkU16, param->numBits, mBuf);
   CMCHKPK(SPkU32, param->ulCount, mBuf);
   CMCHKPK(SPkU32, param->dlCount, mBuf);
   CMCHKPK(SPkU8, param->isHoPres, mBuf);
   RETVALUE(ROK);
}



/***********************************************************
*
*     Func : cmUnpkPjUdxHoInfo
*
*
*     Desc : * Handover Information for RB.
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
PUBLIC S16 cmUnpkPjUdxHoInfo
(
Pst *pst,
UdxHoInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkPjUdxHoInfo(pst, param, mBuf)
Pst *pst;
UdxHoInfo *param;
Buffer *mBuf;
#endif
{
   S16   cnt, idx;
   S16   ret1 = ROK;

   TRC3(cmUnpkPjUdxHoInfo)

   CMCHKUNPK(SUnpkU8, &param->isHoPres, mBuf);
   CMCHKUNPK(SUnpkU32, &param->dlCount, mBuf);
   CMCHKUNPK(SUnpkU32, &param->ulCount, mBuf);
   CMCHKUNPK(SUnpkU16, &param->numBits, mBuf);

   if (param->numBits % 8 == 0)
      cnt = param->numBits/8;
   else
      cnt = param->numBits/8 + 1;

   if(cnt && (ret1 = SGetStaticBuffer(pst->region, pst->pool, (Data **)&param->ulBitMap,\
               cnt * sizeof(U8), 0)) != ROK)
   {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)EUDX040, (ErrVal)0, "SGetMsg() failed");
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
      RETVALUE(ret1);
   }

   if (cnt == 0)
      param->ulBitMap = NULLP;

   for (idx = 0; idx < cnt; idx++)
   {
      CMCHKUNPK(SUnpkU8, &param->ulBitMap[idx], mBuf);
   }

   RETVALUE(ROK);
}



/***********************************************************
*
*     Func : cmPkPjUdxCfgEnt
*
*
*     Desc : * Configuration structure for PDCP entity.
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
PUBLIC S16 cmPkPjUdxCfgEnt
(
Pst *pst,
UdxCfgEnt *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmPkPjUdxCfgEnt(pst, param, mBuf)
Pst *pst;
UdxCfgEnt *param;
Buffer *mBuf;
#endif
{
   TRC3(cmPkPjUdxCfgEnt)

      switch(param->cfgType) {
         case UDX_CFG_MODIFY:
            if (cmPkPjUdxModCfgEnt(pst, &param->m.modCfg, mBuf) != ROK)
               RETVALUE(RFAILED);
            break;
         case UDX_CFG_ADD:
            CMCHKPK(cmPkPjUdxAddCfgEnt, &param->m.addCfg, mBuf);
            break;
      }
   CMCHKPK(SPkU8, param->cfgType, mBuf);
   CMCHKPK(SPkU8, param->rbType, mBuf);
   CMCHKPK(SPkU8, param->rbId, mBuf);
   RETVALUE(ROK);
}



/***********************************************************
*
*     Func : cmUnpkPjUdxCfgEnt
*
*
*     Desc : * Configuration structure for PDCP entity.
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
PUBLIC S16 cmUnpkPjUdxCfgEnt
(
Pst *pst,
UdxCfgEnt *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkPjUdxCfgEnt(pst, param, mBuf)
Pst *pst;
UdxCfgEnt *param;
Buffer *mBuf;
#endif
{
   TRC3(cmUnpkPjUdxCfgEnt)

   CMCHKUNPK(SUnpkU8, &param->rbId, mBuf);
   CMCHKUNPK(SUnpkU8, &param->rbType, mBuf);
   CMCHKUNPK(SUnpkU8, &param->cfgType, mBuf);
      switch(param->cfgType) {
         case UDX_CFG_ADD:
            CMCHKUNPK(cmUnpkPjUdxAddCfgEnt, &param->m.addCfg, mBuf);
            break;
         case UDX_CFG_MODIFY:
            if (cmUnpkPjUdxModCfgEnt(pst, &param->m.modCfg, mBuf) != ROK)
               RETVALUE(RFAILED);
            break;
      }
   RETVALUE(ROK);
}


#ifdef ANSI
PUBLIC S16 cmPkPjUdxAddCfgEnt
(
UdxAddCfgEnt *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmPkPjUdxAddCfgEnt(param, mBuf)
UdxAddCfgEnt *param;
Buffer *mBuf;
#endif
{

   TRC3(cmPkPjUdxAddCfgEnt)

   CMCHKPK(cmPkPjUdxHdrCompCfg, &param->hdrCompCfg, mBuf);
   CMCHKPK(SPkS16, param->discardTmr, mBuf);
   CMCHKPK(cmPkPjUdxRlcInfo, &param->rlcInfo, mBuf);
   /* cpj_c_001.main_4 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
   CMCHKPK(SPkU8, param->qci, mBuf);
#endif /* LTE_L2_MEAS */
   CMCHKPK(SPkU8, param->cfmReqd, mBuf);
   CMCHKPK(SPkU8, param->discReqd, mBuf);
   RETVALUE(ROK);
}


#ifdef ANSI
PUBLIC S16 cmUnpkPjUdxAddCfgEnt
(
UdxAddCfgEnt *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkPjUdxAddCfgEnt(param, mBuf)
UdxAddCfgEnt *param;
Buffer *mBuf;
#endif
{

   TRC3(cmUnpkPjUdxAddCfgEnt)

   CMCHKUNPK(SUnpkU8, &param->discReqd, mBuf);
   CMCHKUNPK(SUnpkU8, &param->cfmReqd, mBuf);
   /* cpj_c_001.main_4 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
   CMCHKUNPK(SUnpkU8, &param->qci, mBuf);
#endif /* LTE_L2_MEAS */
   CMCHKUNPK(cmUnpkPjUdxRlcInfo, &param->rlcInfo, mBuf);
   CMCHKUNPK(SUnpkS16, &param->discardTmr, mBuf);
   CMCHKUNPK(cmUnpkPjUdxHdrCompCfg, &param->hdrCompCfg, mBuf);
   RETVALUE(ROK);
}


#ifdef ANSI
PUBLIC S16 cmPkPjUdxModCfgEnt
(
Pst *pst,
UdxModCfgEnt *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmPkPjUdxModCfgEnt(pst, param, mBuf)
Pst *pst;
UdxModCfgEnt *param;
Buffer *mBuf;
#endif
{
   TRC3(cmPkPjUdxModCfgEnt)

   if (cmPkPjUdxHoInfo(pst, &param->hoInfo, mBuf) != ROK)
      RETVALUE(RFAILED);
   CMCHKPK(cmPkPjUdxHdrCompCfg, &param->hdrCompCfg, mBuf);
   CMCHKPK(SPkU8, param->cfmReqd, mBuf);
   CMCHKPK(SPkU8, param->discReqd, mBuf);
   CMCHKPK(SPkU8, param->bitFlag, mBuf);
   RETVALUE(ROK);
}


#ifdef ANSI
PUBLIC S16 cmUnpkPjUdxModCfgEnt
(
Pst *pst,
UdxModCfgEnt *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkPjUdxModCfgEnt(pst, param, mBuf)
Pst *pst;
UdxModCfgEnt *param;
Buffer *mBuf;
#endif
{
   TRC3(cmUnpkPjUdxModCfgEnt)

   CMCHKUNPK(SUnpkU8, &param->bitFlag, mBuf);
   CMCHKUNPK(SUnpkU8, &param->discReqd, mBuf);
   CMCHKUNPK(SUnpkU8, &param->cfmReqd, mBuf);
   CMCHKUNPK(cmUnpkPjUdxHdrCompCfg, &param->hdrCompCfg, mBuf);
   if (cmUnpkPjUdxHoInfo(pst, &param->hoInfo, mBuf) != ROK)
      RETVALUE(RFAILED);
   RETVALUE(ROK);
}


/***********************************************************
*
*     Func : cmPkPjUdxCfgReqInfo
*
*
*     Desc : * PDCP Configuration structure. Parameters are filled by RRC 
   layer while configuring the PDCP layer.
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
PUBLIC S16 cmPkPjUdxCfgReqInfo
(
Pst *pst,
UdxCfgReqInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmPkPjUdxCfgReqInfo(pst, param, mBuf)
Pst *pst;
UdxCfgReqInfo *param;
Buffer *mBuf;
#endif
{
   S32 i;

   TRC3(cmPkPjUdxCfgReqInfo)

   for (i=param->numEnt-1; i >= 0; i--) {
      if (cmPkPjUdxCfgEnt(pst, &param->cfgEnt[i], mBuf) != ROK)
         RETVALUE(RFAILED);
   }
#ifdef TENB_MULT_CELL_SUPPRT
   CMCHKPK(SPkS16, param->rlcDlSapId, mBuf);
   CMCHKPK(SPkS16, param->rlcUlSapId, mBuf);
#endif

   CMCHKPK(SPkU8, param->numEnt, mBuf);
   CMCHKPK(SPkU32, param->transId, mBuf);
   CMCHKPK(cmPkLteCellId, param->cellId, mBuf);
   CMCHKPK(cmPkLteRnti, param->ueId, mBuf);
   RETVALUE(ROK);
}

/*
*
*    Fun:    cmPkPjUdxUlStaRepInfo
*
*    Desc:    pack the structure UdxUlStaRepInfo
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     pj_udx.c
*
*/
#ifdef ANSI
PUBLIC S16 cmPkPjUdxUlStaRepInfo
(
UdxUlStaRepInfo  *param,
Pst            *pst,
Buffer         *mBuf
)
#else
PUBLIC S16 cmPkPjUdxUlStaRepInfo(param, pst, mBuf)
UdxUlStaRepInfo  *param;
Pst            *pst;
Buffer         *mBuf;
#endif
{
   S16 msgLen;
   TRC3(cmPkPjUdxUlStaRepInfo)

   CMCHKPK(SPkU8, param->pdcpId.rbType, mBuf);
   CMCHKPK(cmPkLteRbId, param->pdcpId.rbId, mBuf);
   CMCHKPK(cmPkLteRnti, param->pdcpId.ueId, mBuf);
   CMCHKPK(cmPkLteCellId, param->pdcpId.cellId, mBuf);

   if (SFndLenMsg(param->sta, &msgLen) != ROK)
      RETVALUE(RFAILED);
   if (SCatMsg(mBuf, param->sta, M1M2) != ROK)
      RETVALUE(RFAILED);
   PJ_FREE_BUF(mBuf);
   CMCHKPK(SPkS16, msgLen, mBuf);

   RETVALUE(ROK);
} /* cmPkPjUdxUlStaRepInfo */

#ifdef ANSI
PUBLIC S16 cmUnpkPjUdxUlStaRepInfo
(
Pst *pst,
UdxUlStaRepInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkPjUdxUlStaRepInfo(pst, param, mBuf)
Pst *pst;
UdxUlStaRepInfo *param;
Buffer *mBuf;
#endif
{
   MsgLen msgLen=0;
   MsgLen totalMsgLen;

   TRC3(cmUnpkPjUdxUlStaRepInfo);

   CMCHKUNPK(SUnpkU8, &param->pdcpId.rbType, mBuf);
   CMCHKUNPK(cmUnpkLteRbId, &param->pdcpId.rbId, mBuf);
   CMCHKUNPK(cmUnpkLteRnti, &param->pdcpId.ueId, mBuf);
   CMCHKUNPK(cmUnpkLteCellId, &param->pdcpId.cellId, mBuf);

   if (SFndLenMsg(mBuf, &totalMsgLen) != ROK)
      RETVALUE(RFAILED);

   if (SSegMsg(mBuf, totalMsgLen-msgLen, &param->sta) != ROK)
      RETVALUE(RFAILED);   

   RETVALUE(ROK);
}

#ifdef ANSI
PUBLIC S16 cmUnpkPjUdxUlFdbkPktInfo
(
Pst *pst,
UdxUlFdbkPktInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkPjUdxUlFdbkPktInfo(pst, param, mBuf)
Pst *pst;
UdxUlFdbkPktInfo *param;
Buffer *mBuf;
#endif
{
   MsgLen msgLen=0;
   MsgLen totalMsgLen;

   TRC3(cmUnpkPjUdxUlFdbkPktInfo);

   CMCHKUNPK(SUnpkU8, &param->pdcpId.rbType, mBuf);
   CMCHKUNPK(cmUnpkLteRbId, &param->pdcpId.rbId, mBuf);
   CMCHKUNPK(cmUnpkLteRnti, &param->pdcpId.ueId, mBuf);
   CMCHKUNPK(cmUnpkLteCellId, &param->pdcpId.cellId, mBuf);

   if (SFndLenMsg(mBuf, &totalMsgLen) != ROK)
      RETVALUE(RFAILED);

   if (SSegMsg(mBuf, totalMsgLen-msgLen, &param->fbPkt) != ROK)
      RETVALUE(RFAILED);   

   RETVALUE(ROK);
}

#ifdef ANSI
PUBLIC S16 cmUnpkPjUdxDlFdbkPktInfo
(
Pst *pst,
UdxDlFdbkPktInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkPjUdxDlFdbkPktInfo(pst, param, mBuf)
Pst *pst;
UdxDlFdbkPktInfo *param;
Buffer *mBuf;
#endif
{
   MsgLen msgLen=0;
   MsgLen totalMsgLen;

   TRC3(cmUnpkPjUdxDlFdbkPktInfo);

   CMCHKUNPK(SUnpkU8, &param->pdcpId.rbType, mBuf);
   CMCHKUNPK(cmUnpkLteRbId, &param->pdcpId.rbId, mBuf);
   CMCHKUNPK(cmUnpkLteRnti, &param->pdcpId.ueId, mBuf);
   CMCHKUNPK(cmUnpkLteCellId, &param->pdcpId.cellId, mBuf);

   if (SFndLenMsg(mBuf, &totalMsgLen) != ROK)
      RETVALUE(RFAILED);

   if (SSegMsg(mBuf, totalMsgLen-msgLen, &param->fbPkt) != ROK)
      RETVALUE(RFAILED);   

   RETVALUE(ROK);
}

/*
*
*    Fun:    cmPkPjUdxDlStaRepInfo
*
*    Desc:    pack the structure UdxDlStaRepInfo
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     pj_udx.c
*
*/
#ifdef ANSI
PUBLIC S16 cmPkPjUdxDlStaRepInfo
(
UdxDlStaRepInfo  *param,
Pst            *pst,
Buffer         *mBuf
)
#else
PUBLIC S16 cmPkPjUdxDlStaRepInfo(param, pst, mBuf)
UdxDlStaRepInfo  *param;
Pst            *pst;
Buffer         *mBuf;
#endif
{
   S16 msgLen;
   TRC3(cmPkPjUdxDlStaRepInfo)

   CMCHKPK(SPkU8, param->pdcpId.rbType, mBuf);
   CMCHKPK(cmPkLteRbId, param->pdcpId.rbId, mBuf);
   CMCHKPK(cmPkLteRnti, param->pdcpId.ueId, mBuf);
   CMCHKPK(cmPkLteCellId, param->pdcpId.cellId, mBuf);
   CMCHKPK(SPkU32, param->fmc, mBuf);
   
   if (SFndLenMsg(param->staPdu, &msgLen) != ROK)
      RETVALUE(RFAILED);
   if (SCatMsg(mBuf, param->staPdu, M1M2) != ROK)
      RETVALUE(RFAILED);
   PJ_FREE_BUF(mBuf);
   CMCHKPK(SPkS16, msgLen, mBuf);

   RETVALUE(ROK);
} /* cmPkPjUdxUlStaRepInfo */

#ifdef ANSI
PUBLIC S16 cmUnpkPjUdxDlStaRepInfo
(
Pst *pst,
UdxDlStaRepInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkPjUdxDlStaRepInfo(pst, param, mBuf)
Pst *pst;
UdxDlStaRepInfo *param;
Buffer *mBuf;
#endif
{
   MsgLen msgLen=0;
   MsgLen totalMsgLen;

   TRC3(cmUnpkPjUdxDlStaRepInfo);

   CMCHKUNPK(SUnpkU8, &param->pdcpId.rbType, mBuf);
   CMCHKUNPK(cmUnpkLteRbId, &param->pdcpId.rbId, mBuf);
   CMCHKUNPK(cmUnpkLteRnti, &param->pdcpId.ueId, mBuf);
   CMCHKUNPK(cmUnpkLteCellId, &param->pdcpId.cellId, mBuf);
   CMCHKUNPK(SUnpkU32, &param->fmc, mBuf);

   if (SFndLenMsg(mBuf, &totalMsgLen) != ROK)
      RETVALUE(RFAILED);

   if (SSegMsg(mBuf, totalMsgLen-msgLen, &param->staPdu) != ROK)
      RETVALUE(RFAILED);   

   RETVALUE(ROK);
}



/***********************************************************
*
*     Func : cmUnpkPjUdxCfgReqInfo
*
*
*     Desc : * PDCP Configuration structure. Parameters are filled by RRC 
   layer while configuring the PDCP layer.
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
PUBLIC S16 cmUnpkPjUdxCfgReqInfo
(
Pst *pst,
UdxCfgReqInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkPjUdxCfgReqInfo(pst, param, mBuf)
Pst *pst;
UdxCfgReqInfo *param;
Buffer *mBuf;
#endif
{
   S32 i;

   TRC3(cmUnpkPjUdxCfgReqInfo)

   CMCHKUNPK(cmUnpkLteRnti, &param->ueId, mBuf);
   CMCHKUNPK(cmUnpkLteCellId, &param->cellId, mBuf);
   CMCHKUNPK(SUnpkU32, &param->transId, mBuf);
   CMCHKUNPK(SUnpkU8, &param->numEnt, mBuf);
#ifdef TENB_MULT_CELL_SUPPRT
   CMCHKUNPK(SUnpkS16, &param->rlcUlSapId, mBuf);
   CMCHKUNPK(SUnpkS16, &param->rlcDlSapId, mBuf);
#endif
   for (i=0; i<param->numEnt; i++) {
      if (cmUnpkPjUdxCfgEnt(pst, &param->cfgEnt[i], mBuf) != ROK)
         RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
}


/***********************************************************
*
*     Func : cmPkPjUdxCfmEnt
*
*
*     Desc : * PDCP confirm parameter structure for PDCP entity.
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
PUBLIC S16 cmPkPjUdxCfmEnt
(
UdxCfmEnt *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmPkPjUdxCfmEnt(param, mBuf)
UdxCfmEnt *param;
Buffer *mBuf;
#endif
{

   TRC3(cmPkPjUdxCfmEnt)

   CMCHKPK(SPkS16, param->reason, mBuf);
   CMCHKPK(SPkS16, param->status, mBuf);
   CMCHKPK(SPkU8, param->rbType, mBuf);
   CMCHKPK(SPkU8, param->rbId, mBuf);
   RETVALUE(ROK);
}



/***********************************************************
*
*     Func : cmUnpkPjUdxCfmEnt
*
*
*     Desc : * PDCP confirm parameter structure for PDCP entity.
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
PUBLIC S16 cmUnpkPjUdxCfmEnt
(
UdxCfmEnt *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkPjUdxCfmEnt(param, mBuf)
UdxCfmEnt *param;
Buffer *mBuf;
#endif
{

   TRC3(cmUnpkPjUdxCfmEnt)

   CMCHKUNPK(SUnpkU8, &param->rbId, mBuf);
   CMCHKUNPK(SUnpkU8, &param->rbType, mBuf);
   CMCHKUNPK(SUnpkS16, &param->status, mBuf);
   CMCHKUNPK(SUnpkS16, &param->reason, mBuf);
   RETVALUE(ROK);
}



/***********************************************************
*
*     Func : cmPkPjUdxCfgCfmInfo
*
*
*     Desc : * PDCP configuration confirm structure.
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
PUBLIC S16 cmPkPjUdxCfgCfmInfo
(
UdxCfgCfmInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmPkPjUdxCfgCfmInfo(param, mBuf)
UdxCfgCfmInfo *param;
Buffer *mBuf;
#endif
{

   S32 i;
   TRC3(cmPkPjUdxCfgCfmInfo)

   for (i=param->numEnt-1; i >= 0; i--) {
      CMCHKPK(cmPkPjUdxCfmEnt, &param->cfmEnt[i], mBuf);
   }
   CMCHKPK(SPkU8, param->numEnt, mBuf);
   CMCHKPK(SPkU32, param->transId, mBuf);
   CMCHKPK(cmPkLteCellId, param->cellId, mBuf);
   CMCHKPK(cmPkLteRnti, param->ueId, mBuf);
   RETVALUE(ROK);
}



/***********************************************************
*
*     Func : cmUnpkPjUdxCfgCfmInfo
*
*
*     Desc : * PDCP configuration confirm structure.
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
PUBLIC S16 cmUnpkPjUdxCfgCfmInfo
(
UdxCfgCfmInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkPjUdxCfgCfmInfo(param, mBuf)
UdxCfgCfmInfo *param;
Buffer *mBuf;
#endif
{

   S32 i;
   TRC3(cmUnpkPjUdxCfgCfmInfo)

   CMCHKUNPK(cmUnpkLteRnti, &param->ueId, mBuf);
   CMCHKUNPK(cmUnpkLteCellId, &param->cellId, mBuf);
   CMCHKUNPK(SUnpkU32, &param->transId, mBuf);
   CMCHKUNPK(SUnpkU8, &param->numEnt, mBuf);
   for (i=0; i<param->numEnt; i++) {
      CMCHKUNPK(cmUnpkPjUdxCfmEnt, &param->cfmEnt[i], mBuf);
   }
   RETVALUE(ROK);
}


/***********************************************************
*
*     Func : cmPkPjUdxReEstReqInfo
*
*
*     Desc : * Reestabishment request structure for SRB1 of an UE.
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
PUBLIC S16 cmPkPjUdxReEstReqInfo
(
UdxReEstReqInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmPkPjUdxReEstReqInfo(param, mBuf)
UdxReEstReqInfo *param;
Buffer *mBuf;
#endif
{

   S32 i;
   TRC3(cmPkPjUdxReEstReqInfo)

   for (i=UDX_CIPHER_KEY_LEN-1; i >= 0; i--) {
      CMCHKPK(SPkU8, param->usrplnKey[i], mBuf);
   }
   for (i=UDX_CIPHER_KEY_LEN-1; i >= 0; i--) {
      CMCHKPK(SPkU8, param->ctrlplnKey[i], mBuf);
   }
   for (i=UDX_INTG_KEY_LEN-1; i >= 0; i--) {
      CMCHKPK(SPkU8, param->intKey[i], mBuf);
   }
   CMCHKPK(SPkU32, param->transId, mBuf);

   CMCHKPK(cmPkLtePdcpId, &param->pdcpId, mBuf);
   RETVALUE(ROK);
}



/***********************************************************
*
*     Func : cmUnpkPjUdxReEstReqInfo
*
*
*     Desc : * Reestabishment request structure for SRB1 of an UE.
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
PUBLIC S16 cmUnpkPjUdxReEstReqInfo
(
UdxReEstReqInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkPjUdxReEstReqInfo(param, mBuf)
UdxReEstReqInfo *param;
Buffer *mBuf;
#endif
{

   S32 i;
   TRC3(cmUnpkPjUdxReEstReqInfo)

   CMCHKUNPK(cmUnpkLtePdcpId, &param->pdcpId, mBuf);
   CMCHKUNPK(SUnpkU32, &param->transId, mBuf);
   for (i=0; i<UDX_INTG_KEY_LEN; i++) {
      CMCHKUNPK(SUnpkU8, &param->intKey[i], mBuf);
   }
   for (i=0; i<UDX_CIPHER_KEY_LEN; i++) {
      CMCHKUNPK(SUnpkU8, &param->ctrlplnKey[i], mBuf);
   }
   for (i=0; i<UDX_CIPHER_KEY_LEN; i++) {
      CMCHKUNPK(SUnpkU8, &param->usrplnKey[i], mBuf);
   }
   RETVALUE(ROK);
}


/***********************************************************
*
*     Func : cmPkPjUdxReEstCfmInfo
*
*
*     Desc : * Reestablishment confirmation structure for an UE.
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
PUBLIC S16 cmPkPjUdxReEstCfmInfo
(
UdxReEstCfmInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmPkPjUdxReEstCfmInfo(param, mBuf)
UdxReEstCfmInfo *param;
Buffer *mBuf;
#endif
{

   TRC3(cmPkPjUdxReEstCfmInfo)

   CMCHKPK(SPkS16, param->status, mBuf);
   CMCHKPK(SPkU32, param->transId, mBuf);
   CMCHKPK(cmPkLteCellId, param->cellId, mBuf);
   CMCHKPK(cmPkLteRnti, param->ueId, mBuf);
   RETVALUE(ROK);
}



/***********************************************************
*
*     Func : cmUnpkPjUdxReEstCfmInfo
*
*
*     Desc : * Reestablishment confirmation structure for an UE.
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
PUBLIC S16 cmUnpkPjUdxReEstCfmInfo
(
UdxReEstCfmInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkPjUdxReEstCfmInfo(param, mBuf)
UdxReEstCfmInfo *param;
Buffer *mBuf;
#endif
{

   TRC3(cmUnpkPjUdxReEstCfmInfo)

   CMCHKUNPK(cmUnpkLteRnti, &param->ueId, mBuf);
   CMCHKUNPK(cmUnpkLteCellId, &param->cellId, mBuf);
   CMCHKUNPK(SUnpkU32, &param->transId, mBuf);
   CMCHKUNPK(SUnpkS16, &param->status, mBuf);
   RETVALUE(ROK);
}


/***********************************************************
*
*     Func : cmPkPjUdxSecCfgReqInfo
*
*
*     Desc : * PDCP Security Configuration structure. Parameters are filled by RRC
 * while configuring the security parameters.
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
PUBLIC S16 cmPkPjUdxSecCfgReqInfo
(
UdxSecCfgReqInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmPkPjUdxSecCfgReqInfo(param, mBuf)
UdxSecCfgReqInfo *param;
Buffer *mBuf;
#endif
{

   TRC3(cmPkPjUdxSecCfgReqInfo)

   CMCHKPK(cmPkPjUdxSecCfg, &param->secCfg, mBuf);
   CMCHKPK(SPkU32, param->transId, mBuf);
   CMCHKPK(cmPkLteCellId, param->cellId, mBuf);
   CMCHKPK(cmPkLteRnti, param->ueId, mBuf);
   RETVALUE(ROK);
}



/***********************************************************
*
*     Func : cmUnpkPjUdxSecCfgReqInfo
*
*
*     Desc : * PDCP Security Configuration structure. Parameters are filled by RRC
 * while configuring the security parameters.
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
PUBLIC S16 cmUnpkPjUdxSecCfgReqInfo
(
UdxSecCfgReqInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkPjUdxSecCfgReqInfo(param, mBuf)
UdxSecCfgReqInfo *param;
Buffer *mBuf;
#endif
{

   TRC3(cmUnpkPjUdxSecCfgReqInfo)

   CMCHKUNPK(cmUnpkLteRnti, &param->ueId, mBuf);
   CMCHKUNPK(cmUnpkLteCellId, &param->cellId, mBuf);
   CMCHKUNPK(SUnpkU32, &param->transId, mBuf);
   CMCHKUNPK(cmUnpkPjUdxSecCfg, &param->secCfg, mBuf);
   RETVALUE(ROK);
}


/***********************************************************
*
*     Func : cmPkPjUdxSecCfgCfmInfo
*
*
*     Desc : * PDCP Security configuration confirm structure.
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
PUBLIC S16 cmPkPjUdxSecCfgCfmInfo
(
UdxSecCfgCfmInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmPkPjUdxSecCfgCfmInfo(param, mBuf)
UdxSecCfgCfmInfo *param;
Buffer *mBuf;
#endif
{

   TRC3(cmPkPjUdxSecCfgCfmInfo)

   CMCHKPK(SPkS16, param->reason, mBuf);
   CMCHKPK(SPkS16, param->status, mBuf);
   CMCHKPK(SPkU32, param->transId, mBuf);
   CMCHKPK(cmPkLteCellId, param->cellId, mBuf);
   CMCHKPK(cmPkLteRnti, param->ueId, mBuf);
   RETVALUE(ROK);
}



/***********************************************************
*
*     Func : cmUnpkPjUdxSecCfgCfmInfo
*
*
*     Desc : * PDCP Security configuration confirm structure.
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
PUBLIC S16 cmUnpkPjUdxSecCfgCfmInfo
(
UdxSecCfgCfmInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkPjUdxSecCfgCfmInfo(param, mBuf)
UdxSecCfgCfmInfo *param;
Buffer *mBuf;
#endif
{

   TRC3(cmUnpkPjUdxSecCfgCfmInfo)

   CMCHKUNPK(cmUnpkLteRnti, &param->ueId, mBuf);
   CMCHKUNPK(cmUnpkLteCellId, &param->cellId, mBuf);
   CMCHKUNPK(SUnpkU32, &param->transId, mBuf);
   CMCHKUNPK(SUnpkS16, &param->status, mBuf);
   CMCHKUNPK(SUnpkS16, &param->reason, mBuf);
   RETVALUE(ROK);
}


/***********************************************************
*
*     Func : cmPkPjUdxUeInfo
*
*
*     Desc : *  UE ID information
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
PUBLIC S16 cmPkPjUdxUeInfo
(
UdxUeInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmPkPjUdxUeInfo(param, mBuf)
UdxUeInfo *param;
Buffer *mBuf;
#endif
{

   TRC3(cmPkPjUdxUeInfo)

   CMCHKPK(cmPkLteRnti, param->ueId, mBuf);
   CMCHKPK(cmPkLteCellId, param->cellId, mBuf);
   RETVALUE(ROK);
}



/***********************************************************
*
*     Func : cmUnpkPjUdxUeInfo
*
*
*     Desc : *  UE ID information
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
PUBLIC S16 cmUnpkPjUdxUeInfo
(
UdxUeInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkPjUdxUeInfo(param, mBuf)
UdxUeInfo *param;
Buffer *mBuf;
#endif
{

   TRC3(cmUnpkPjUdxUeInfo)

   CMCHKUNPK(cmUnpkLteCellId, &param->cellId, mBuf);
   CMCHKUNPK(cmUnpkLteRnti, &param->ueId, mBuf);
   RETVALUE(ROK);
}



/***********************************************************
*
*     Func : cmPkPjUdxCountReqInfo
*
*
*     Desc : *  PDCP Count Request Information from RRC.
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
PUBLIC S16 cmPkPjUdxCountReqInfo
(
UdxCountReqInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmPkPjUdxCountReqInfo(param, mBuf)
UdxCountReqInfo *param;
Buffer *mBuf;
#endif
{

   TRC3(cmPkPjUdxCountReqInfo)

   CMCHKPK(SPkU32, param->transId, mBuf);
   CMCHKPK(cmPkLteCellId, param->cellId, mBuf);
   CMCHKPK(cmPkLteRnti, param->ueId, mBuf);
   RETVALUE(ROK);
}



/***********************************************************
*
*     Func : cmUnpkPjUdxCountReqInfo
*
*
*     Desc : *  PDCP Count Request Information from RRC.
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
PUBLIC S16 cmUnpkPjUdxCountReqInfo
(
UdxCountReqInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkPjUdxCountReqInfo(param, mBuf)
UdxCountReqInfo *param;
Buffer *mBuf;
#endif
{

   TRC3(cmUnpkPjUdxCountReqInfo)

   CMCHKUNPK(cmUnpkLteRnti, &param->ueId, mBuf);
   CMCHKUNPK(cmUnpkLteCellId, &param->cellId, mBuf);
   CMCHKUNPK(SUnpkU32, &param->transId, mBuf);
   RETVALUE(ROK);
}


/***********************************************************
*
*     Func : cmPkPjUdxCountInfo
*
*
*     Desc : *  PDCP Count Information for a DRB.
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
PUBLIC S16 cmPkPjUdxCountInfo
(
UdxCountInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmPkPjUdxCountInfo(param, mBuf)
UdxCountInfo *param;
Buffer *mBuf;
#endif
{

   TRC3(cmPkPjUdxCountInfo)

   CMCHKPK(SPkU32, param->dlCount, mBuf);
   CMCHKPK(SPkU32, param->ulCount, mBuf);
   CMCHKPK(SPkU8, param->dir, mBuf);
   CMCHKPK(SPkU8, param->rbId, mBuf);
   RETVALUE(ROK);
}



/***********************************************************
*
*     Func : cmUnpkPjUdxCountInfo
*
*
*     Desc : *  PDCP Count Information for a DRB.
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
PUBLIC S16 cmUnpkPjUdxCountInfo
(
UdxCountInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkPjUdxCountInfo(param, mBuf)
UdxCountInfo *param;
Buffer *mBuf;
#endif
{

   TRC3(cmUnpkPjUdxCountInfo)

   CMCHKUNPK(SUnpkU8, &param->rbId, mBuf);
   CMCHKUNPK(SUnpkU8, &param->dir, mBuf);
   CMCHKUNPK(SUnpkU32, &param->ulCount, mBuf);
   CMCHKUNPK(SUnpkU32, &param->dlCount, mBuf);
   RETVALUE(ROK);
}



/***********************************************************
*
*     Func : cmPkPjUdxCountCfmInfo
*
*
*     Desc : *  PDCP Count Request Confirmation to RRC.
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
PUBLIC S16 cmPkPjUdxCountCfmInfo
(
UdxCountCfmInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmPkPjUdxCountCfmInfo(param, mBuf)
UdxCountCfmInfo *param;
Buffer *mBuf;
#endif
{

   S32 i;
   TRC3(cmPkPjUdxCountCfmInfo)

   CMCHKPK(SPkS16, param->reason, mBuf);
   CMCHKPK(SPkS16, param->status, mBuf);
   for (i=param->numRb-1; i >= 0; i--) {
      CMCHKPK(cmPkPjUdxCountInfo, &param->countInfo[i], mBuf);
   }
   CMCHKPK(SPkU8, param->numRb, mBuf);
   CMCHKPK(SPkU32, param->transId, mBuf);
   CMCHKPK(cmPkLteCellId, param->cellId, mBuf);
   CMCHKPK(cmPkLteRnti, param->ueId, mBuf);
   RETVALUE(ROK);
}



/***********************************************************
*
*     Func : cmUnpkPjUdxCountCfmInfo
*
*
*     Desc : *  PDCP Count Request Confirmation to RRC.
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
PUBLIC S16 cmUnpkPjUdxCountCfmInfo
(
UdxCountCfmInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkPjUdxCountCfmInfo(param, mBuf)
UdxCountCfmInfo *param;
Buffer *mBuf;
#endif
{

   S32 i;
   TRC3(cmUnpkPjUdxCountCfmInfo)

   CMCHKUNPK(cmUnpkLteRnti, &param->ueId, mBuf);
   CMCHKUNPK(cmUnpkLteCellId, &param->cellId, mBuf);
   CMCHKUNPK(SUnpkU32, &param->transId, mBuf);
   CMCHKUNPK(SUnpkU8, &param->numRb, mBuf);
   for (i=0; i<param->numRb; i++) {
      CMCHKUNPK(cmUnpkPjUdxCountInfo, &param->countInfo[i], mBuf);
   }
   CMCHKUNPK(SUnpkS16, &param->status, mBuf);
   CMCHKUNPK(SUnpkS16, &param->reason, mBuf);
   RETVALUE(ROK);
}


/***********************************************************
*
*     Func : cmPkPjUdxSduStaReqInfo
*
*
*     Desc : *  PDCP SDU Status Request Information from RRC - Used for reestablishment
 *  during handover.
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
PUBLIC S16 cmPkPjUdxSduStaReqInfo
(
UdxSduStaReqInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmPkPjUdxSduStaReqInfo(param, mBuf)
UdxSduStaReqInfo *param;
Buffer *mBuf;
#endif
{

   TRC3(cmPkPjUdxSduStaReqInfo)

   CMCHKPK(SPkU32, param->transId, mBuf);
   CMCHKPK(cmPkLteCellId, param->cellId, mBuf);
   CMCHKPK(cmPkLteRnti, param->ueId, mBuf);
   RETVALUE(ROK);
}



/***********************************************************
*
*     Func : cmUnpkPjUdxSduStaReqInfo
*
*
*     Desc : *  PDCP SDU Status Request Information from RRC - Used for reestablishment
 *  during handover.
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
PUBLIC S16 cmUnpkPjUdxSduStaReqInfo
(
UdxSduStaReqInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkPjUdxSduStaReqInfo(param, mBuf)
UdxSduStaReqInfo *param;
Buffer *mBuf;
#endif
{

   TRC3(cmUnpkPjUdxSduStaReqInfo)

   CMCHKUNPK(cmUnpkLteRnti, &param->ueId, mBuf);
   CMCHKUNPK(cmUnpkLteCellId, &param->cellId, mBuf);
   CMCHKUNPK(SUnpkU32, &param->transId, mBuf);
   RETVALUE(ROK);
}


/***********************************************************
*
*     Func : cmPkPjUdxDlSduStaInfo
*
*
*     Desc : *  PDCP DL SDU Status Information for a DRB mapped to RLC AM.
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
PUBLIC S16 cmPkPjUdxDlSduStaInfo
(
UdxDlSduStaInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmPkPjUdxDlSduStaInfo(param, mBuf)
UdxDlSduStaInfo *param;
Buffer *mBuf;
#endif
{

   TRC3(cmPkPjUdxDlSduStaInfo)
                                 
   CMCHKPK(cmPkBool, param->hoPres, mBuf);  
   CMCHKPK(SPkU32, param->count, mBuf);
   RETVALUE(ROK);
}



/***********************************************************
*
*     Func : cmUnpkPjUdxDlSduStaInfo
*
*
*     Desc : *  PDCP DL SDU Status Information for a DRB mapped to RLC AM.
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
PUBLIC S16 cmUnpkPjUdxDlSduStaInfo
(
UdxDlSduStaInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkPjUdxDlSduStaInfo(param, mBuf)
UdxDlSduStaInfo *param;
Buffer *mBuf;
#endif
{

   TRC3(cmUnpkPjUdxDlSduStaInfo)
   
   CMCHKUNPK(cmUnpkBool, &param->hoPres, mBuf); 
   CMCHKUNPK(SUnpkU32, &param->count, mBuf);
   RETVALUE(ROK);
}



/***********************************************************
*
*     Func : cmPkPjUdxSduStaInfo
*
*
*     Desc : *  PDCP SDU Status Information for a DRB mapped to RLC AM.
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
PUBLIC S16 cmPkPjUdxSduStaInfo
(
Pst *pst,
UdxSduStaInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmPkPjUdxSduStaInfo(pst, param, mBuf)
Pst *pst;
UdxSduStaInfo *param;
Buffer *mBuf;
#endif
{
   TRC3(cmPkPjUdxSduStaInfo)

   CMCHKPK(cmPkPjUdxDlSduStaInfo, &param->dlSduStaInfo, mBuf);
   CMCHKPK(SPkU8, param->dir, mBuf);
   CMCHKPK(SPkU8, param->rbId, mBuf);
   RETVALUE(ROK);
}



/***********************************************************
*
*     Func : cmUnpkPjUdxSduStaInfo
*
*
*     Desc : *  PDCP SDU Status Information for a DRB mapped to RLC AM.
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
PUBLIC S16 cmUnpkPjUdxSduStaInfo
(
Pst *pst,
UdxSduStaInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkPjUdxSduStaInfo(pst, param, mBuf)
Pst *pst;
UdxSduStaInfo *param;
Buffer *mBuf;
#endif
{
   TRC3(cmUnpkPjUdxSduStaInfo)

   CMCHKUNPK(SUnpkU8, &param->rbId, mBuf);
   CMCHKUNPK(SUnpkU8, &param->dir, mBuf);

   CMCHKUNPK(cmUnpkPjUdxDlSduStaInfo, &param->dlSduStaInfo, mBuf);
   RETVALUE(ROK);
}



/***********************************************************
*
*     Func : cmPkPjUdxSduStaCfmInfo
*
*
*     Desc : *  PDCP SDU Status Request Confirmation for an UE.
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
PUBLIC S16 cmPkPjUdxSduStaCfmInfo
(
Pst *pst,
UdxSduStaCfmInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmPkPjUdxSduStaCfmInfo(pst, param, mBuf)
Pst *pst;
UdxSduStaCfmInfo *param;
Buffer *mBuf;
#endif
{
   S32 i;

   TRC3(cmPkPjUdxSduStaCfmInfo)

   CMCHKPK(SPkS16, param->reason, mBuf);
   CMCHKPK(SPkS16, param->status, mBuf);
   for (i=param->numRb-1; i >= 0; i--) {
      if (cmPkPjUdxSduStaInfo(pst, &param->sduStaInfo[i], mBuf) != ROK)
         RETVALUE(RFAILED);
   }
   CMCHKPK(SPkU8, param->numRb, mBuf);
   CMCHKPK(SPkU32, param->transId, mBuf);
   CMCHKPK(cmPkLteCellId, param->cellId, mBuf);
   CMCHKPK(cmPkLteRnti, param->ueId, mBuf);
   RETVALUE(ROK);
}


/***********************************************************
*
*     Func : cmPkPjUdxDatResumeCfmInfo
*
*
*     Desc : *  PDCP SDU Status Request Confirmation for an UE.
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
PUBLIC S16 cmPkPjUdxDatResumeCfmInfo
(
Pst *pst,
UdxDatResumeCfmInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmPkPjUdxDatResumeCfmInfo(pst, param, mBuf)
Pst *pst;
UdxDatResumeCfmInfo *param;
Buffer *mBuf;
#endif
{

   TRC3(cmPkPjUdxDatResumeCfmInfo)

   CMCHKPK(SPkS16, param->reason, mBuf);
   CMCHKPK(SPkS16, param->status, mBuf);
   CMCHKPK(SPkU32, param->transId, mBuf);
   CMCHKPK(cmPkLteCellId, param->cellId, mBuf);
   CMCHKPK(cmPkLteRnti, param->ueId, mBuf);
   RETVALUE(ROK);
}

#ifdef ANSI
PUBLIC S16 cmPkPjUdxDatForwardCfmInfo
(
Pst *pst,
UdxDatFwdCfmInfo *param,
Buffer *mBuf
 )
#else
PUBLIC S16 cmPkPjUdxDatForwardCfmInfo(pst, param, mBuf)
Pst *pst;
UdxDatFwdCfmInfo *param;
Buffer *mBuf;
#endif
{
   TRC3(cmPkPjUdxDatForwardCfmInfo)

   CMCHKPK(SPkS16, param->reason, mBuf);
   CMCHKPK(SPkS16, param->status, mBuf);
   CMCHKPK(cmPkLteCellId, param->cellId, mBuf);
   CMCHKPK(cmPkLteRnti, param->ueId, mBuf);
   RETVALUE(ROK);
}
/*
*
*    Fun:    cmPkPjUdxDlFdbkPktInfo
*
*    Desc:    pack the structure UdxDlFdbPktInfo
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     pj_udx.c
*
*/
#ifdef ANSI
PUBLIC S16 cmPkPjUdxDlFdbkPktInfo
(
UdxDlFdbkPktInfo  *param,
Pst            *pst,
Buffer         *mBuf
)
#else
PUBLIC S16 cmPkPjUdxDlFdbkPktInfo(param, pst, mBuf)
UdxDlFdbkPktInfo  *param;
Pst            *pst;
Buffer         *mBuf;
#endif
{
   S16 msgLen;
   TRC3(cmPkPjUdxDlFdbkPktInfo)

   CMCHKPK(SPkU8, param->pdcpId.rbType, mBuf);
   CMCHKPK(cmPkLteRbId, param->pdcpId.rbId, mBuf);
   CMCHKPK(cmPkLteRnti, param->pdcpId.ueId, mBuf);
   CMCHKPK(cmPkLteCellId, param->pdcpId.cellId, mBuf);

   if (SFndLenMsg(param->fbPkt, &msgLen) != ROK)
      RETVALUE(RFAILED);
   if (SCatMsg(mBuf, param->fbPkt, M1M2) != ROK)
      RETVALUE(RFAILED);
   PJ_FREE_BUF(mBuf);
   CMCHKPK(SPkS16, msgLen, mBuf);

   RETVALUE(ROK);
} /* cmPkPjUdxDlFdbkPktInfo */


/***********************************************************
*
*     Func : cmUnpkPjUdxSduStaCfmInfo
*
*
*     Desc : *  PDCP SDU Status Request Confirmation for an UE.
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
PUBLIC S16 cmUnpkPjUdxSduStaCfmInfo
(
Pst *pst,
UdxSduStaCfmInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkPjUdxSduStaCfmInfo(pst, param, mBuf)
Pst *pst;
UdxSduStaCfmInfo *param;
Buffer *mBuf;
#endif
{

   S32 i;
   TRC3(cmUnpkPjUdxSduStaCfmInfo)

   CMCHKUNPK(cmUnpkLteRnti, &param->ueId, mBuf);
   CMCHKUNPK(cmUnpkLteCellId, &param->cellId, mBuf);
   CMCHKUNPK(SUnpkU32, &param->transId, mBuf);
   CMCHKUNPK(SUnpkU8, &param->numRb, mBuf);
   for (i=0; i<param->numRb; i++) {
      if (cmUnpkPjUdxSduStaInfo(pst, &param->sduStaInfo[i], mBuf) != ROK)
         RETVALUE(RFAILED);
   }
   CMCHKUNPK(SUnpkS16, &param->status, mBuf);
   CMCHKUNPK(SUnpkS16, &param->reason, mBuf);
   RETVALUE(ROK);
}


/***********************************************************
*
*     Func : cmUnpkPjUdxDatResumeCfmInfo
*
*
*     Desc : *  PDCP SDU Status Request Confirmation for an UE.
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
PUBLIC S16 cmUnpkPjUdxDatResumeCfmInfo
(
Pst *pst,
UdxDatResumeCfmInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkPjUdxDatResumeCfmInfo(pst, param, mBuf)
Pst *pst;
UdxDatResumeCfmInfo *param;
Buffer *mBuf;
#endif
{

   TRC3(cmUnpkPjUdxDatResumeCfmInfo)

   CMCHKUNPK(cmUnpkLteRnti, &param->ueId, mBuf);
   CMCHKUNPK(cmUnpkLteCellId, &param->cellId, mBuf);
   CMCHKUNPK(SUnpkU32, &param->transId, mBuf);
   CMCHKUNPK(SUnpkS16, &param->status, mBuf);
   CMCHKUNPK(SUnpkS16, &param->reason, mBuf);
   RETVALUE(ROK);
}
/***********************************************************
*
*     Func : cmUnpkPjUdxDatForwardCfm
*
*
*     Desc : *  PDCP Data Forward Confirmation for an UE.
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
PUBLIC S16 cmUnpkPjUdxDatForwardCfmInfo
(
Pst *pst,
UdxDatFwdCfmInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkPjUdxDatForwardCfmInfo(pst, param, mBuf)
Pst *pst;
UdxDatFwdCfmInfo *param;
Buffer *mBuf;
#endif
{

   TRC3(cmUnpkPjUdxDatForwardCfmInfo)

   CMCHKUNPK(cmUnpkLteRnti, &param->ueId, mBuf);
   CMCHKUNPK(cmUnpkLteCellId, &param->cellId, mBuf);
   CMCHKUNPK(SUnpkS16, &param->status, mBuf);
   CMCHKUNPK(SUnpkS16, &param->reason, mBuf);
   RETVALUE(ROK);
}


#ifdef ANSI
PUBLIC S16 cmPkPjUdxUlFdbkPkt
(
Pst * pst,
SpId spId,
UdxUlFdbkPktInfo *fdbkPktInfo
)
#else
PUBLIC S16 cmPkPjUdxUlFdbkPkt(pst, spId, fdbkPktInfo)
Pst * pst;
SpId spId;
UdxUlFdbkPktInfo *fdbkPktInfo;
#endif
{
   S16 ret;
   Buffer *mBuf;
   mBuf = NULLP;

   TRC3(cmPkPjUdxUlFdbkPkt)

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EUDX016, (ErrVal)0, "Packing failed");
#endif
      PJ_FREE_BUF(mBuf);
      PJ_FREE_SHRABL_BUF_PST(pst->region, pst->pool, fdbkPktInfo, sizeof(UdxUlFdbkPktInfo));

      RETVALUE(RFAILED);
   }

   switch(pst->selector)
   {
#ifdef LCUDX
      case UDX_SEL_LC:
         {
            ret = cmPkPjUdxUlFdbkPktInfo( (fdbkPktInfo), pst, mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
            if(ret != ROK)
            {
               PJ_FREE_BUF(mBuf);
               PJ_FREE_BUF(fdbkPktInfo->fbPkt);
               PJ_FREE_SHRABL_BUF_PST(pst->region, pst->pool, fdbkPktInfo, sizeof(UdxUlFdbkPktInfo));
               
               SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                     __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                     (ErrVal)EUDXXXX, (ErrVal)ret, "Packing failure");
               RETVALUE( ret );
            }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
            PJ_FREE_BUF(fdbkPktInfo->fbPkt);
#ifndef XEON_SPECIFIC_CHANGES
            if (SPutStaticBuffer(pst->region, pst->pool, (Data *)fdbkPktInfo,
                     sizeof(UdxUlFdbkPktInfo), 0) != ROK)
#else
            if (SPutSBuf(pst->region, pst->pool, (Data *)fdbkPktInfo,
                     sizeof(UdxUlFdbkPktInfo)) != ROK)
#endif
            {
               PJ_FREE_BUF(mBuf);
               RETVALUE(RFAILED);
            }

            break;
         }
       case UDX_SEL_LWLC:
         {
            CMCHKPKLOG(cmPkPtr,(PTR)fdbkPktInfo,mBuf,EUDXXXX,pst);
            break;
         }
#endif /* LCUDX */
   }
   
   CMCHKPKLOG(SPkS16, spId, mBuf, EUDXXXX, pst);
   pst->event = (Event) EVTUDXULROHCFDB;

   RETVALUE(SPstTsk(pst,mBuf));
}

#ifdef ANSI
PUBLIC S16 cmPkPjUdxDlFdbkPkt
(
Pst * pst,
SpId spId,
UdxDlFdbkPktInfo *fdbkPktInfo
)
#else
PUBLIC S16 cmPkPjUdxDlFdbkPkt(pst, spId, fdbkPktInfo)
Pst * pst;
SpId spId;
UdxDlFdbkPktInfo *fdbkPktInfo;
#endif
{
   S16 ret;
   Buffer *mBuf;
   mBuf = NULLP;

   TRC3(cmPkPjUdxDlFdbkPkt)

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EUDX016, (ErrVal)0, "Packing failed");
#endif
      PJ_FREE_BUF(mBuf);
      PJ_FREE_SHRABL_BUF_PST(pst->region, pst->pool, fdbkPktInfo, sizeof(UdxUlFdbkPktInfo));

      RETVALUE(RFAILED);
   }

   switch(pst->selector)
   {
#ifdef LCUDX
      case UDX_SEL_LC:
         {
            ret = cmPkPjUdxDlFdbkPktInfo( (fdbkPktInfo), pst, mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
            if(ret != ROK)
            {
               PJ_FREE_BUF(mBuf);
               PJ_FREE_BUF(fdbkPktInfo->fbPkt);
               PJ_FREE_SHRABL_BUF_PST(pst->region, pst->pool, fdbkPktInfo, sizeof(UdxUlFdbkPktInfo));
               
               SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                     __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                     (ErrVal)EUDXXXX, (ErrVal)ret, "Packing failure");
               RETVALUE( ret );
            }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
            PJ_FREE_BUF(fdbkPktInfo->fbPkt);
#ifndef XEON_SPECIFIC_CHANGES
            if (SPutStaticBuffer(pst->region, pst->pool, (Data *)fdbkPktInfo,
                     sizeof(UdxDlFdbkPktInfo), 0) != ROK)
#else
            if (SPutSBuf(pst->region, pst->pool, (Data *)fdbkPktInfo,
                     sizeof(UdxDlFdbkPktInfo)) != ROK)
#endif
            {
               PJ_FREE_BUF(mBuf);
               RETVALUE(RFAILED);
            }

            break;
         }
       case UDX_SEL_LWLC:
         {
            CMCHKPKLOG(cmPkPtr,(PTR)fdbkPktInfo,mBuf,EUDXXXX,pst);
            break;
         }
#endif /* LCUDX */
   }
   
   CMCHKPKLOG(SPkS16, spId, mBuf, EUDXXXX, pst);
   pst->event = (Event) EVTUDXDLROHCFDB;

   RETVALUE(SPstTsk(pst,mBuf));
}

/*
*
*    Fun:    cmPkPjUdxUlFdbkPktInfo
*
*    Desc:    pack the structure UdxUlFdbPktInfo
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     pj_udx.c
*
*/
#ifdef ANSI
PUBLIC S16 cmPkPjUdxUlFdbkPktInfo
(
UdxUlFdbkPktInfo  *param,
Pst            *pst,
Buffer         *mBuf
)
#else
PUBLIC S16 cmPkPjUdxUlFdbkPktInfo(param, pst, mBuf)
UdxUlFdbkPktInfo  *param;
Pst            *pst;
Buffer         *mBuf;
#endif
{
   S16 msgLen;
   TRC3(cmPkPjUdxUlFdbkPktInfo)

   CMCHKPK(SPkU8, param->pdcpId.rbType, mBuf);
   CMCHKPK(cmPkLteRbId, param->pdcpId.rbId, mBuf);
   CMCHKPK(cmPkLteRnti, param->pdcpId.ueId, mBuf);
   CMCHKPK(cmPkLteCellId, param->pdcpId.cellId, mBuf);

   if (SFndLenMsg(param->fbPkt, &msgLen) != ROK)
      RETVALUE(RFAILED);
   if (SCatMsg(mBuf, param->fbPkt, M1M2) != ROK)
      RETVALUE(RFAILED);
   PJ_FREE_BUF(param->fbPkt);
   CMCHKPK(SPkS16, msgLen, mBuf);

   RETVALUE(ROK);
} /* cmPkPjUdxUlFdbkPktInfo */


#ifdef ANSI
PUBLIC S16 cmUnpkPjUdxDlFdbkPkt
(
UdxDlFdbkPkt func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkPjUdxDlFdbkPkt(func, pst, mBuf)
UdxDlFdbkPkt func;
Pst *pst;
Buffer *mBuf;
#endif
{
   S16   ret1;
   SpId  spId;
   UdxDlFdbkPktInfo *fdbkPkt;

   TRC3(cmUnpkPjUdxDlFdbkPkt)
   
   if (SUnpkS16(&spId, mBuf) != ROK) {
      PJ_FREE_BUF(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EUDX021, (ErrVal)0, "Unpacking failed");
#endif
      RETVALUE(RFAILED);
   }

   switch(pst->selector)
   {
#ifdef LCUDX
       case UDX_SEL_LC:
       {        
          if((ret1 = SGetSBuf(pst->region, pst->pool, (Data **)&fdbkPkt,\
                      sizeof(UdxDlFdbkPktInfo))) != ROK)
          {
#if (ERRCLASS & ERRCLS_ADD_RES)
              SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                    __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                    (ErrVal)EUDX040, (ErrVal)0, "SGetMsg() failed");
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */

              PJ_FREE_BUF(mBuf);
             RETVALUE(ret1);
          }

          if (cmUnpkPjUdxDlFdbkPktInfo(pst, fdbkPkt, mBuf) != ROK) 
          {
             PJ_FREE_SHRABL_BUF_PST(pst->region, pst->pool, fdbkPkt, sizeof(UdxDlFdbkPktInfo));
             PJ_FREE_BUF(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
             SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                (ErrVal)EUDX023, (ErrVal)0, "Unpacking failed");
#endif
             RETVALUE(RFAILED);
          }

          break;
       }

       case UDX_SEL_LWLC:
       {
          CMCHKUNPK(cmUnpkPtr,(PTR *) &fdbkPkt, mBuf);
          break;
       }
#endif /* LCUDX */
   }

   PJ_FREE_BUF(mBuf);

   RETVALUE((*func)(pst, spId, fdbkPkt));  
}


#ifdef ANSI
PUBLIC S16 cmUnpkPjUdxDlFdbPktInfo
(
Pst *pst,
UdxDlFdbkPktInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkPjUdxDlFdbPktInfo(pst, param, mBuf)
Pst *pst;
UdxDlFdbkPktInfo *param;
Buffer *mBuf;
#endif
{
   MsgLen msgLen=0;
   MsgLen totalMsgLen;

   TRC3(cmUnpkPjUdxDlFdbPktInfo);

   CMCHKUNPK(SUnpkU8, &param->pdcpId.rbType, mBuf);
   CMCHKUNPK(cmUnpkLteRbId, &param->pdcpId.rbId, mBuf);
   CMCHKUNPK(cmUnpkLteRnti, &param->pdcpId.ueId, mBuf);
   CMCHKUNPK(cmUnpkLteCellId, &param->pdcpId.cellId, mBuf);

   if (SFndLenMsg(mBuf, &totalMsgLen) != ROK)
      RETVALUE(RFAILED);

   if (SSegMsg(mBuf, totalMsgLen-msgLen, &param->fbPkt) != ROK)
      RETVALUE(RFAILED);   

   RETVALUE(ROK);
}


#ifdef ANSI
PUBLIC S16 cmUnpkPjUdxUlStaRep
(
UdxUlStaRep func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkPjUdxUlStaRep(func, pst, mBuf)
UdxUlStaRep func;
Pst *pst;
Buffer *mBuf;
#endif
{
   S16   ret1;
   SpId  spId;
   UdxUlStaRepInfo *staRep;
   
   TRC3(cmUnpkPjUdxUlStaRep)

   if (SUnpkS16(&spId, mBuf) != ROK) {
      PJ_FREE_BUF(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EUDX021, (ErrVal)0, "Unpacking failed");
#endif
      RETVALUE(RFAILED);
   }

    switch(pst->selector)
    {
#ifdef LCUDX
       case UDX_SEL_LC:
       {
          if((ret1 = SGetSBuf(pst->region, pst->pool, (Data **)&staRep,\
                      sizeof(UdxUlStaRepInfo))) != ROK)
          {
#if (ERRCLASS & ERRCLS_ADD_RES)
              SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                    __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                    (ErrVal)EUDX040, (ErrVal)0, "SGetMsg() failed");
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */

              PJ_FREE_BUF(mBuf);
             RETVALUE(ret1);
          }

          if (cmUnpkPjUdxUlStaRepInfo(pst, staRep, mBuf) != ROK) 
          {
             PJ_FREE_SHRABL_BUF_PST(pst->region, pst->pool, staRep, sizeof(UdxUlStaRepInfo));
             PJ_FREE_BUF(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
             SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                (ErrVal)EUDX023, (ErrVal)0, "Unpacking failed");
#endif
             RETVALUE(RFAILED);
          }

          break;
       }

       case UDX_SEL_LWLC:
       {
          CMCHKUNPK(cmUnpkPtr,(PTR *) &staRep, mBuf);
          break;
       }
#endif /* LCUDX */
   }

    PJ_FREE_BUF(mBuf);

   RETVALUE((*func)(pst, spId, staRep));
}

#ifdef ANSI
PUBLIC S16 cmUnpkPjUdxDlStaRep
(
UdxDlStaRep func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkPjUdxDlStaRep(func, pst, mBuf)
UdxDlStaRep func;
Pst *pst;
Buffer *mBuf;
#endif
{
   S16   ret1;
   SpId  spId;
   UdxDlStaRepInfo *staRep;
   
   TRC3(cmUnpkPjUdxDlStaRep)

   if (SUnpkS16(&spId, mBuf) != ROK) {
      PJ_FREE_BUF(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EUDX021, (ErrVal)0, "Unpacking failed");
#endif
      RETVALUE(RFAILED);
   }

    switch(pst->selector)
    {
#ifdef LCUDX
       case UDX_SEL_LC:
       {
          if((ret1 = SGetSBuf(pst->region, pst->pool, (Data **)&staRep,\
                      sizeof(UdxDlStaRepInfo))) != ROK)
          {
#if (ERRCLASS & ERRCLS_ADD_RES)
              SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                    __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                    (ErrVal)EUDX040, (ErrVal)0, "SGetMsg() failed");
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */

              PJ_FREE_BUF(mBuf);
             RETVALUE(ret1);
          }

          if (cmUnpkPjUdxDlStaRepInfo(pst, staRep, mBuf) != ROK) 
          {
             PJ_FREE_SHRABL_BUF_PST(pst->region, pst->pool, staRep, sizeof(UdxDlStaRepInfo));
             PJ_FREE_BUF(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
             SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                (ErrVal)EUDX023, (ErrVal)0, "Unpacking failed");
#endif
             RETVALUE(RFAILED);
          }

          break;
       }

       case UDX_SEL_LWLC:
       {
          CMCHKUNPK(cmUnpkPtr,(PTR *) &staRep, mBuf);
          break;
       }
#endif /* LCUDX */
   }

    PJ_FREE_BUF(mBuf);

   RETVALUE((*func)(pst, spId, staRep));
}

#ifdef ANSI
PUBLIC S16 cmUnpkPjUdxUlFdbkPkt
(
UdxUlFdbkPkt func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkPjUdxUlFdbkPkt(func, pst, mBuf)
UdxUlFdbkPkt func;
Pst *pst;
Buffer *mBuf;
#endif
{
   S16   ret1;
   SpId  spId;
   UdxUlFdbkPktInfo *fdbkPkt;

   TRC3(cmUnpkPjUdxUlFdbkPkt)
   
   if (SUnpkS16(&spId, mBuf) != ROK) {
      PJ_FREE_BUF(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EUDX021, (ErrVal)0, "Unpacking failed");
#endif
      RETVALUE(RFAILED);
   }

   switch(pst->selector)
   {
#ifdef LCUDX
       case UDX_SEL_LC:
       {        
          if((ret1 = SGetSBuf(pst->region, pst->pool, (Data **)&fdbkPkt,\
                      sizeof(UdxUlFdbkPktInfo))) != ROK)
          {
#if (ERRCLASS & ERRCLS_ADD_RES)
              SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                    __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                    (ErrVal)EUDX040, (ErrVal)0, "SGetMsg() failed");
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */

              PJ_FREE_BUF(mBuf);
             RETVALUE(ret1);
          }

          if (cmUnpkPjUdxUlFdbkPktInfo(pst, fdbkPkt, mBuf) != ROK) 
          {
             PJ_FREE_SHRABL_BUF_PST(pst->region, pst->pool, fdbkPkt, sizeof(UdxUlFdbkPktInfo));
             PJ_FREE_BUF(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
             SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                (ErrVal)EUDX023, (ErrVal)0, "Unpacking failed");
#endif
             RETVALUE(RFAILED);
          }

          break;
       }

       case UDX_SEL_LWLC:
       {
          CMCHKUNPK(cmUnpkPtr,(PTR *) &fdbkPkt, mBuf);
          break;
       }
#endif /* LCUDX */
   }

   PJ_FREE_BUF(mBuf);

   RETVALUE((*func)(pst, spId, fdbkPkt));  
}

#ifdef LTE_L2_MEAS
/*
*
*    Fun:    cmPkPjUdxL2MeasReq
*
*    Ret:    ROK  -ok
*
*    Notes:  None
*
*
*/
#ifdef ANSI
PUBLIC S16 cmPkPjUdxL2MeasReq
(
Pst               *pst,
PjL2MeasReqEvt    *measReqEvt 
)
#else
PUBLIC S16 cmPkPjUdxL2MeasReq(pst, measReqEvt)
Pst               *pst;
PjL2MeasReqEvt    *measReqEvt; 
#endif
{
    S16    ret1;
    Buffer *mBuf = NULLP;

    TRC3(cmPkPjUdxL2MeasReq)

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

       RETVALUE(ret1);
    }
    
    switch (pst->selector)
    {
       case UDX_SEL_LC:
       {
       }
       case UDX_SEL_LWLC:
       {
         CMCHKPK(cmPkPtr,(PTR) measReqEvt, mBuf);
         break;
       }
    }
    pst->event = (Event) PJ_UDX_EVT_L2MEAS_REQ;

    RETVALUE(SPstTsk(pst, mBuf));

} /* cmPkPjUdxStaUpdReq */

/*
*
*    Fun:    cmPkPjUdxL2MeasSendReq
*
*    Ret:    ROK  -ok
*
*    Notes:  None
*
*
*/
#ifdef ANSI
PUBLIC S16 cmPkPjUdxL2MeasSendReq
(
Pst              *pst,
U8               measType
)
#else
PUBLIC S16 cmPkPjUdxL2MeasSendReq(pst, measReqEvt)
Pst               *pst;
U8                measType
#endif
{
    S16    ret1;
    Buffer *mBuf = NULLP;

    TRC3(cmPkPjUdxL2MeasSendReq)

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

       RETVALUE(ret1);
    }
    
    switch (pst->selector)
    {
       case UDX_SEL_LC:
       {
       }
       case UDX_SEL_LWLC:
       {
          CMCHKPKLOG(SPkU8, measType, mBuf, EUDXXXX, pst);
          break;
       }
    }
    pst->event = (Event) PJ_UDX_EVT_L2MEAS_SEND_REQ;

    RETVALUE(SPstTsk(pst, mBuf));

} 

/*
*
*    Fun:    cmPkPjUdxL2MeasStopReq
*
*    Ret:    ROK  -ok
*
*    Notes:  None
*
*
*/
#ifdef ANSI
PUBLIC S16 cmPkPjUdxL2MeasStopReq
(
Pst              *pst,
U8               measType
)
#else
PUBLIC S16 cmPkPjUdxL2MeasStopReq(pst, measType)
Pst               *pst;
U8                measType
#endif
{
    S16    ret1;
    Buffer *mBuf = NULLP;

    TRC3(cmPkPjUdxL2MeasStopReq)

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

       RETVALUE(ret1);
    }
    
    switch (pst->selector)
    {
       case UDX_SEL_LC:
       {
       }
       case UDX_SEL_LWLC:
       {
          CMCHKPKLOG(SPkU8, measType, mBuf, EUDXXXX, pst);
          break;
       }
    }
    pst->event = (Event) PJ_UDX_EVT_L2MEAS_STOP_REQ;

    RETVALUE(SPstTsk(pst, mBuf));

} 

/*

 
*    Fun:    cmUnpkPjUdxL2MeasReq
*
*    Ret:    ROK  -ok
*
*    Notes:  None
*
*
*/
#ifdef ANSI
PUBLIC S16 cmUnpkPjUdxL2MeasReq
(
UdxL2MeasReq     func,
Pst              *pst,
Buffer           *mBuf
)
#else
PUBLIC S16 cmUnpkPjUdxL2MeasReq(func, pst, mBuf)
UdxL2MeasReq     func;
Pst              *pst;
Buffer           *mBuf;
#endif
{
   // S16    ret1;
    PjL2MeasReqEvt    *measReqEvt; 

    TRC3(cmUnpkPjUdxL2MeasReq)

    switch (pst->selector)
    {
       case UDX_SEL_LC:
       {
       }
       case UDX_SEL_LWLC:
       {
         CMCHKUNPK(cmUnpkPtr, (PTR *)&measReqEvt,mBuf);
         break;
       }
    }

    SPutMsg(mBuf);

    RETVALUE((*func)(pst, measReqEvt));
} /* cmUnpkPjUdxL2MeasReq */

/*
*
*    Fun:    cmUnpkPjUdxL2MeasSendReq
*
*    Ret:    ROK  -ok
*
*    Notes:  None
*
*
*/
#ifdef ANSI
PUBLIC S16 cmUnpkPjUdxL2MeasSendReq
(
UdxL2MeasSendReq     func,
Pst                  *pst,
Buffer               *mBuf
)
#else
PUBLIC S16 cmUnpkPjUdxL2MeasSendReq(func, pst, mBuf)
UdxL2MeasSendReq     func;
Pst                  *pst;
Buffer               *mBuf;
#endif
{
    //S16    ret1;
    U8     measType;

    TRC3(cmUnpkPjUdxL2MeasSendReq)

    switch (pst->selector)
    {
       case UDX_SEL_LC:
       {
       }
       case UDX_SEL_LWLC:
       {
          CMCHKUNPK(SUnpkU8, &measType, mBuf);
         break;
       }
    }

    SPutMsg(mBuf);

    RETVALUE((*func)(pst, measType));
} /* cmUnpkPjUdxL2MeasReq */

/*
*
*    Fun:    cmUnpkPjUdxL2MeasStopReq
*
*    Ret:    ROK  -ok
*
*    Notes:  None
*
*
*/
#ifdef ANSI
PUBLIC S16 cmUnpkPjUdxL2MeasStopReq
(
UdxL2MeasStopReq     func,
Pst                  *pst,
Buffer               *mBuf
)
#else
PUBLIC S16 cmUnpkPjUdxL2MeasStopReq(func, pst, mBuf)
UdxL2MeasStopReq     func;
Pst                  *pst;
Buffer               *mBuf;
#endif
{
    U8     measType;

    TRC3(cmUnpkPjUdxL2MeasStopReq)

    switch (pst->selector)
    {
       case UDX_SEL_LC:
       {
       }
       case UDX_SEL_LWLC:
       {
          CMCHKUNPK(SUnpkU8, &measType, mBuf);
         break;
       }
    }

    SPutMsg(mBuf);

    RETVALUE((*func)(pst, measType));
} /* cmUnpkPjUdxL2MeasStopReq */
#endif

/*
*
*    Fun:    cmPkPjUdxDatResumeReq
*
*    Ret:    ROK  -ok
*
*    Notes:  None
*
*
*/
#ifdef ANSI
PUBLIC S16 cmPkPjUdxDatResumeReq
(
Pst           *pst,
UdxDatResumeInfo  *udxDatResumeInfo 
)
#else
PUBLIC S16 cmPkPjUdxDatResumeReq(pst, cellId, ueId)
Pst           *pst;
UdxDatResumeInfo  *udxDatResumeInfo;
#endif
{
    S16    ret1;
    Buffer *mBuf = NULLP;

    TRC3(cmPkPjUdxDatResume)

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
       PJ_FREE_SHRABL_BUF_PST(pst->region, pst->pool, (Data *)udxDatResumeInfo,sizeof(UdxDatResumeInfo));
       RETVALUE(ret1);
    }
    
    switch (pst->selector)
    {
       case UDX_SEL_LC:
       {
       }
       case UDX_SEL_LWLC:
       {
          CMCHKPKLOG(cmPkPtr,(PTR)udxDatResumeInfo,mBuf,EUDXXXX,pst);
          break;
       }
    }
   pst->event = (Event) EVTUDXDATRESUME;

    RETVALUE(SPstTsk(pst, mBuf));

} /* cmPkPjUdxDatResumeReq */

/*
*
*    Fun:    cmUnpkPjUdxDatResumeReq
*
*    Ret:    ROK  -ok
*
*    Notes:  None
*
*
*/
#ifdef ANSI
PUBLIC S16 cmUnpkPjUdxDatResumeReq
(
UdxDatResumeReq  func,
Pst              *pst,
Buffer           *mBuf
)
#else
PUBLIC S16 cmUnpkPjUdxDatResumeReq(func, pst, mBuf)
UdxDatResumeReq  func;
Pst              *pst;
Buffer           *mBuf;
#endif
{
   UdxDatResumeInfo  *datResumeInfo;

    TRC3(cmUnpkPjUdxDatResumeReq)

    switch (pst->selector)
    {
       case UDX_SEL_LC:
       {
       }
       case UDX_SEL_LWLC:
       {
          CMCHKUNPK(cmUnpkPtr,(PTR *) &datResumeInfo, mBuf);
         break;
       }
    }

    SPutMsg(mBuf);

    RETVALUE((*func)(pst, datResumeInfo));
} /* cmUnpkPjUdxDatResumeReq */

/*
*
*    Fun:    cmPkPjUdxDatForwardReq
*
*    Ret:    ROK  -ok
*
*    Notes:  None
*
*
*/
#ifdef ANSI
PUBLIC S16 cmPkPjUdxDatForwardReq
(
Pst           *pst,
UdxDatForwardInfo   *datFwdInfo
)
#else
PUBLIC S16 cmPkPjUdxDatForwardReq(pst, datFwdInfo)
Pst           *pst;
UdxDatForwardInfo   *datFwdInfo;
#endif
{
    S16    ret1;
    Buffer *mBuf = NULLP;

    TRC3(cmPkPjUdxDatForwardReq)

    RLOG0(L_DEBUG,"Packing UdxDatForwadReq");
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

       RETVALUE(ret1);
    }
    
    switch (pst->selector)
    {
       case UDX_SEL_LC:
       {
       }
       case UDX_SEL_LWLC:
       {
          CMCHKPKLOG(cmPkPtr,(PTR)datFwdInfo,mBuf,EUDXXXX,pst);
          break;
       }
    }
   pst->event = (Event) EVTUDXDATFRWD;

    RETVALUE(SPstTsk(pst, mBuf));

} /* cmPkPjUdxDatForwardReq */

/*
*
*    Fun:    cmUnpkPjUdxDatForwardReq
*
*    Ret:    ROK  -ok
*
*    Notes:  None
*
*
*/
#ifdef ANSI
PUBLIC S16 cmUnpkPjUdxDatForwardReq
(
UdxDatForwardReq  func,
Pst              *pst,
Buffer           *mBuf
)
#else
PUBLIC S16 cmUnpkPjUdxDatForwardReq(func, pst, mBuf)
UdxDatForwardReq  func;
Pst              *pst;
Buffer           *mBuf;
#endif
{
   UdxDatForwardInfo   *datFwdInfo;

    TRC3(cmUnpkPjUdxDatForwardReq)

    RLOG0(L_DEBUG,"Unpacking UdxDatForwadReq");
    switch (pst->selector)
    {
       case UDX_SEL_LC:
       {
       }
       case UDX_SEL_LWLC:
       {
          CMCHKUNPK(cmUnpkPtr,(PTR *) &datFwdInfo, mBuf);
         break;
       }
    }

    SPutMsg(mBuf);

    RETVALUE((*func)(pst, datFwdInfo));
} /* cmUnpkPjUdxDatForwardReq */
#endif /* end of #if(defined(LCUDX) || defined(LWLCUDX)) */

/********************************************************************30**
  
         End of file
**********************************************************************/
