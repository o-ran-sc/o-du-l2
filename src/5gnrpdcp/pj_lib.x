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
  
        Name:     LTE PDCP file 
    
        Type:     C include file
  
        Desc:     This file contain the hash definations for PDCP 
 
        File:     pj_lib.x

*********************************************************************21*/

#ifndef __PJ_LIB_X__
#define __PJ_LIB_X__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifdef PJ_CMP_ASYNC

typedef S16 (*ObdCmpInitCfm) ARGS((
   Pst *                pst,
   PjCmpCxt             cmpCxt,
   PTR                  ctxId));

typedef S16 (*ObdCmpCfm) ARGS((
   Pst *                pst,
   PTR                  cmpCxtId,
   PjLibTrans           libTrans,
   Buffer *             opSdu));

typedef S16 (*ObdDecmpCfm) ARGS((
   Pst *                pst,
   PTR                  cmpCxtId,
   PjLibTrans           libTrans,
   Buffer *             opSdu,
   Buffer *             fbPkt
));

/* Library function externs */
EXTERN S16 PjLibObdCmpInitCfm ARGS((
Pst      *pst,                    /* post structure */
PjCmpCxt cmpCxt,                  /* Context for ROHC */
PTR      ctxId                    /* Compression Context ID */
));
EXTERN S16 PjLibObdCmpCfm ARGS((
Pst        *pst,        /* post structure             */
PTR         cmpCxtId,   /* Context Id for compression */
PjLibTrans  libTrans,   /* Transaction Id             */
Buffer     *opSdu       /* compressed SDU             */
));
EXTERN S16 PjLibObdDecmpCfm ARGS((
Pst         *pst,                 /* post structure */
PTR         cmpCxtId,             /* Context Id for decompression */
PjLibTrans  libTrans,             /* Transaction Id */
Buffer      *opSdu,               /* decompressed SDU */
Buffer      *fbPkt                /* Feedback pkt */
));
EXTERN S16 PjLibObdCmpInitReq ARGS((
Pst * pst,
PjCmpCxt cmpCxt,
PjRohc rohc
));
EXTERN S16 PjLibObdCmpReq ARGS((
Pst * pst,
PTR cmpCxtId,
PjLibTrans libTrans,
Buffer * mBuf
));
EXTERN S16 PjLibObdDecmpReq ARGS((
Pst * pst,
PTR cmpCxtId,
PjLibTrans libTrans,
Buffer * mBuf
));
EXTERN S16 PjLibObdCmpCloseReq ARGS((
Pst * pst,
PTR cmpCxtId
));
EXTERN S16 PjLibObdCmpResetReq ARGS((
Pst * pst,
PTR cmpCxtId
));
EXTERN S16 PjLibObdCmpFbReq  ARGS((
Pst      *pst,                    /* post structure */
PTR      cmpCxtId,                /* Context Id for compression */
PjLibTrans libTrans,             /* Transaction Id */
Buffer   *feedback                /* SDU to be decompressed */
));

EXTERN S16 cmPkLibObdCmpInitCfm ARGS((
Pst * pst,
PjCmpCxt cmpCxt,
PTR ctxId
));

EXTERN S16 cmUnpkLibObdCmpInitCfm ARGS((
ObdCmpInitCfm func,
Pst *pst,
Buffer *mBuf
));

EXTERN S16 cmPkLibObdCmpCfm ARGS((
   Pst *                pst,
   PTR                  cmpCxtId,
   PjLibTrans           libTrans,
   Buffer *             opSdu
));

EXTERN S16 cmUnpkLibObdCmpCfm ARGS((
   ObdCmpCfm            func,
   Pst *                pst,
   Buffer               *mBuf
));

EXTERN S16 cmPkLibObdDecmpCfm ARGS((
   Pst *                pst,
   PTR                  cmpCxtId,
   PjLibTrans           libTrans,
   Buffer *             opSdu,
   Buffer *             fbPkt
));

EXTERN S16 cmUnpkLibObdDecmpCfm ARGS((
   ObdDecmpCfm          func,
   Pst *                pst,
   Buffer               *mBuf
));

EXTERN S16 cmPkPjRohc ARGS((
   PjRohc               *param,
   Buffer               *mBuf
));
EXTERN S16 cmUnpkPjRohc ARGS((
   PjRohc               *param,
   Buffer               *mBuf
));
EXTERN S16 cmPkPjCmpCxt ARGS((
   PjCmpCxt             *param,
   Buffer               *mBuf
));
EXTERN S16 cmUnpkPjCmpCxt ARGS((
   PjCmpCxt             *param,
   Buffer               *mBuf
));

#else

EXTERN S16 pjLibCmpInitReq  ARGS((
PjCmpCxt cmpCxt,                  
PjRohc   rohc,                    
PTR      *cmpCxtId                
));
EXTERN S16 pjLibCmpReq  ARGS((
PTR      cmpCxtId,                
Buffer   *mBuf,                   
Buffer   **opSdu                  
));
EXTERN S16 pjLibDecmpReq  ARGS((
PTR      cmpCxtId,                
Buffer   *mBuf,                   
Buffer   **opSdu,                 
Buffer   **feedback               
));
EXTERN S16 pjLibCmpCloseReq ARGS((
PTR      cmpCxtId                 
));
EXTERN S16 pjLibCmpResetReq ARGS((
PTR      cmpCxtId                 
));
EXTERN S16 pjLibCmpFbReq ARGS((
PTR      cmpCxtId,
Buffer   *feedback
));

#endif /* PJ_CMP_ASYNC */

#ifdef PJ_SEC_ASYNC

typedef S16 (*ObdIntInitCfm) ARGS((
   Pst *                pst,
   PjSecCxt             secCtx,
   PTR                  ctxId));

typedef S16 (*ObdCpInitCfm) ARGS((
   Pst *                pst,
   PjSecCxt             secCxt,
   PTR                  ctxId));

typedef S16 (*ObdUpInitCfm) ARGS((
   Pst *                pst,
   PjSecCxt             secCxt,
   PTR                  ctxId));

typedef S16 (*ObdIntProtCfm) ARGS((
   Pst *                pst,
   PTR                  cxtId,
   PjLibTrans           libTrans,
   U32                  macI));

typedef S16 (*ObdCipherCfm) ARGS((
   Pst *                pst,
   PTR                  cxtId,
   PjLibTrans           libTrans,
   Buffer *             opSdu));

typedef S16 (*ObdDecipherCfm) ARGS((
   Pst *                pst,
   PTR                  cxtId,
   PjLibTrans           libTrans,
   Buffer *             opSdu));

typedef S16 (*ObdIntVerCfm) ARGS((
   Pst *                pst,
   PTR                  cxtId,
   PjLibTrans           libTrans,
   Status               status));

EXTERN S16 PjLibObdIntInitCfm ARGS((
Pst      *pst,                    /* post structure */
PjSecCxt secCxt,                  /* Secutiry Context */
PTR      ctxId                    /* Integrity Context ID */
));
EXTERN S16 PjLibObdCpInitCfm ARGS((
Pst      *pst,                    /* post structure */
PjSecCxt secCxt,                  /* Secutiry Context */
PTR      ctxId                    /* Ciphering Context ID */
));
EXTERN S16 PjLibObdUpInitCfm ARGS((
Pst      *pst,                    /* post structure */
PjSecCxt secCxt,                  /* Secutiry Context */
PTR      ctxId                    /* Ciphering Context ID */
));
EXTERN S16 PjLibObdIntProtCfm ARGS((
Pst      *pst,                    /* post structure */
PTR      cxtId,                   /* Context Id for Integrity protection*/
PjLibTrans libTrans,              /* Transaction Id */
U32      macI                     /* Message authentication code */
));
EXTERN S16 PjLibObdCipherCfm ARGS((
Pst      *pst,                    /* post structure */
PTR      cxtId,                   /* Context Id for Ciphering */
PjLibTrans libTrans,              /* Transaction Id */
Buffer   *opSdu                   /* ciphered SDU */
));
EXTERN S16 PjLibObdDecipherCfm ARGS((
Pst         *pst,                 /* post structure */
PTR         cxtId,                /* Context Id for Integrity protection*/
PjLibTrans  libTrans,             /* Transaction Id */
Buffer      *opPdu                /* deciphered SDU */
));
EXTERN S16 PjLibObdIntVerCfm ARGS((
Pst      *pst,                    /* post structure */
PTR      cxtId,                   /* Context Id for Integrity protection*/
PjLibTrans libTrans,              /* Transaction Id */
Status   status                   /* Integrity verification status */
));
EXTERN S16 PjLibObdIntInitReq ARGS((
Pst * pst,
PjSecCxt secCtx,
PjIntInfo intInfo
));
EXTERN S16 PjLibObdCpInitReq ARGS((
Pst * pst,
PjSecCxt secCxt,
U8 algoType,
U8 *cpKey
));
EXTERN S16 PjLibObdUpInitReq ARGS((
Pst * pst,
PjSecCxt secCxt,
U8 algoType,
U8 *upKey
));
EXTERN S16 PjLibObdIntProtReq ARGS((
Pst * pst,
PTR intCxtId,
PjSecInp secInp,
PjLibTrans libTrans,
Buffer ** mBuf
));
EXTERN S16 PjLibObdCipherReq ARGS((
Pst * pst,
PTR cxtId,
PjSecInp secInp,
PjLibTrans libTrans,
Buffer * mBuf
));
EXTERN S16 PjLibObdIntVerReq ARGS((
Pst * pst,
PTR intCxtId,
PjSecInp secInp,
PjLibTrans libTrans,
Buffer * mBuf,
U32 macI
));
EXTERN S16 PjLibObdDecipherReq ARGS((
Pst * pst,
PTR cxtId,
PjSecInp secInp,
PjLibTrans libTrans,
Buffer * mBuf
));
EXTERN S16 PjLibObdIntCloseReq ARGS((
Pst * pst,
PTR intCxtId
));
EXTERN S16 PjLibObdCipherCloseReq ARGS((
Pst * pst,
PTR cpCxtId
));

EXTERN S16 cmPkLibObdIntInitCfm ARGS((
   Pst *                pst,
   PjSecCxt             secCtx,
   PTR                  ctxId
));

EXTERN S16 cmUnpkLibObdIntInitCfm ARGS((
   ObdIntInitCfm        func,
   Pst *                pst,
   Buffer               *mBuf
));

EXTERN S16 cmPkLibObdCpInitCfm ARGS((
   Pst *                pst,
   PjSecCxt             secCxt,
   PTR                  ctxId
));

EXTERN S16 cmUnpkLibObdCpInitCfm ARGS((
   ObdCpInitCfm         func,
   Pst *                pst,
   Buffer               *mBuf
));

EXTERN S16 cmPkLibObdUpInitCfm ARGS((
   Pst *                pst,
   PjSecCxt             secCxt,
   PTR                  ctxId
));

EXTERN S16 cmUnpkLibObdUpInitCfm ARGS((
   ObdUpInitCfm         func,
   Pst *                pst,
   Buffer               *mBuf
));

EXTERN S16 cmPkLibObdIntProtCfm ARGS((
   Pst *                pst,
   PTR                  cxtId,
   PjLibTrans           libTrans,
   U32                  macI
));

EXTERN S16 cmUnpkLibObdIntProtCfm ARGS((
   ObdIntProtCfm        func,
   Pst *                pst,
   Buffer               *mBuf
));

EXTERN S16 cmPkLibObdCipherCfm ARGS((
   Pst *                pst,
   PTR                  cxtId,
   PjLibTrans           libTrans,
   Buffer *             opSdu
));

EXTERN S16 cmUnpkLibObdCipherCfm ARGS((
   ObdCipherCfm         func,
   Pst *                pst,
   Buffer               *mBuf
));

EXTERN S16 cmPkLibObdDecipherCfm ARGS((
   Pst *                pst,
   PTR                  cxtId,
   PjLibTrans           libTrans,
   Buffer *             opSdu
));

EXTERN S16 cmUnpkLibObdDecipherCfm ARGS((
   ObdDecipherCfm       func,
   Pst *                pst,
   Buffer               *mBuf
));

EXTERN S16 cmPkLibObdIntVerCfm ARGS((
   Pst *                pst,
   PTR                  cxtId,
   PjLibTrans           libTrans,
   Status               status
));

EXTERN S16 cmUnpkLibObdIntVerCfm ARGS((
   ObdIntVerCfm         func,
   Pst *                pst,
   Buffer               *mBuf
));

EXTERN S16 cmPkPjSecCxt ARGS((
   PjSecCxt             *param,
   Buffer               *mBuf
));
EXTERN S16 cmUnpkPjSecCxt ARGS((
   PjSecCxt             *param,
   Buffer               *mBuf
));
EXTERN S16 cmPkPjIntInfo ARGS((
   PjIntInfo            *param,
   Buffer               *mBuf
));
EXTERN S16 cmUnpkPjIntInfo ARGS((
   PjIntInfo            *param,
   Buffer               *mBuf
));
EXTERN S16 cmPkPjSecInp ARGS((
   PjSecInp             *param,
   Buffer               *mBuf
));
EXTERN S16 cmUnpkPjSecInp ARGS((
   PjSecInp             *param,
   Buffer               *mBuf
));

#else

EXTERN S16 PjLibObdUlInitReq ARGS ((
PjCb    *gCb
));

EXTERN S16 PjLibObdDlInitReq ARGS ((
PjCb    *gCb
));

EXTERN S16 pjLibObdPrcKenbCfg ARGS ((
PjCb                   *gCb,
CpjAsKeyInfo*       kenbInf,
CpjAskeyCfmInfo     *kdfCfm
));

EXTERN S16 pjLibObdPrcKenbStarCfg ARGS ((
PjCb                   *gCb,
CpjKenbStarInfo*       kenbInf,
CpjKenbStarCfmInfo     *kdfCfm
));

#ifdef INTEL_QAT_DP
EXTERN S16 PjLibObdDlCloseReq ARGS ((Void *));
#else
EXTERN S16 PjLibObdDlCloseReq ARGS ((Void));
#endif

#ifdef INTEL_QAT_DP
EXTERN S16 PjLibObdUlCloseReq ARGS ((Void *));
#else
EXTERN S16 PjLibObdUlCloseReq ARGS ((Void));
#endif

EXTERN S16 PjLibObdIntInitReq ARGS ((
PjSec *secInp
));

EXTERN Void PjUlSpaccTrigger ARGS ((Void));

EXTERN Void PjDlSpaccTrigger ARGS ((Void));

#ifdef INTEL_SW_INTEG
EXTERN S16 PjLibObdIntProtReq ARGS ((
PjCb *gCb,
PjLibTrans  *libTrans, 
Buffer **mBuf
));
#else
EXTERN S16 PjLibObdIntProtReq ARGS ((
PjLibTrans  *libTrans, 
Buffer **mBuf
));
#endif



#ifdef TENB_AS_SECURITY
EXTERN S16 PjLibObdDlInitReq ARGS ((
PjCb    *gCb
));

EXTERN S16 PjLibObdUlInitReq ARGS ((
PjCb    *gCb
));
#ifdef INTEL_QAT_DP
EXTERN S16 PjLibObdDlCloseReq ARGS ((Void *));
#else
EXTERN S16 PjLibObdDlCloseReq ARGS ((Void));
#endif

#ifdef INTEL_QAT_DP
EXTERN S16 PjLibObdUlCloseReq ARGS ((Void *));
#else
EXTERN S16 PjLibObdUlCloseReq ARGS ((Void));
#endif
#endif

#ifdef TENB_AS_SECURITY
#ifdef INTEL_SW_INTEG
EXTERN S16 PjLibObdIntVerReq ARGS ((
PjCb *gCb,
PjLibTrans  *libTrans,
Buffer * mBuf,
U32   macI
));
#else
EXTERN S16 PjLibObdIntVerReq ARGS ((
PjLibTrans  *libTrans,
Buffer * mBuf,
U32   macI
));
#endif
#else
EXTERN S16 PjLibObdIntVerReq ARGS((
Pst * pst,
PTR intCxtId,
PjSecInp secInp,
PjLibTrans libTrans,
Buffer * mBuf,
U32 macI
));

#endif

#ifdef INTEL_QAT_DP
EXTERN S16 PjLibObdIntCloseReq ARGS ((
Void *instHndl, 				 /* Instance Handler */
Void *intCxtId
));
#else
EXTERN S16 PjLibObdIntCloseReq ARGS ((
Void *intCxtId
));
#endif

EXTERN S16 PjLibObdCpInitReq ARGS ((
PjSec *secInp
));

EXTERN S16 PjLibObdUpInitReq ARGS ((
PjSec *secInp
));


EXTERN S16 PjLibObdCipherReq ARGS ((
PjLibTrans  *libTrans,
Buffer      *mBuf,
Buffer      **opSdu
));

#ifdef FLAT_BUFFER_OPT
EXTERN S16 PjLibObdCipherReqFB ARGS ((
PjLibTrans  *libTrans,
FlatBuffer  *mBuf,
Buffer      **opSdu
));
#endif

EXTERN S16 PjLibObdDecipherReq ARGS ((
PjLibTrans  *libTrans,
Buffer      *mBuf,
Buffer      **opSdu
));

#ifdef INTEL_QAT_DP
EXTERN S16 PjLibObdCipherCloseReq ARGS ((
Void *instHndl,                                  /* Instance Handler */
Void *cpCxtId
));
#else
EXTERN S16 PjLibObdCipherCloseReq ARGS ((
Void *cpCxtId
));
#endif

#ifdef INTEL_QAT_DP
EXTERN S16 pjLibIntInitReq ARGS (( 
PjCb  *gCb,
PjIntInfo intInfo,                /* Integrity Cfg Info */
Void      **cxtId                  /* FD */
));
#else
EXTERN S16 pjLibIntInitReq ARGS (( 
PjSecCxt secCxt,                  /* Context for Intg protection/verification */
PjIntInfo intInfo,                /* Integrity Cfg Info */
Void      **cxtId                  /* FD */
));
#endif

#ifdef INTEL_QAT_DP
EXTERN S16 pjLibCpInitReq ARGS ((
PjCb     *gCb,
PjSecCxt secCxt,                  /* Context for Intg protection/verificatino */
U8       algoType,                /* Type of the algorithm */
U8       *cpKey,                  /* Control plane ciphering key */
Void     **cxtId                   /* FD */
));
#else
EXTERN S16 pjLibCpInitReq ARGS ((
PjSecCxt secCxt,                  /* Context for Intg protection/verificatino */
U8       algoType,                /* Type of the algorithm */
U8       *cpKey,                  /* Control plane ciphering key */
Void     **cxtId                   /* FD */
));
#endif

#ifdef INTEL_QAT_DP
EXTERN S16 pjLibUpInitReq ARGS ((
PjCb     *gCb,
PjSecCxt secCxt,                  /* Context for Intg protection/verificatino */
U8       algoType,                /* Type of the algorithm */
U8       *upKey,                  /* User plane ciphering key */
Void     **cxtId                   /* FD */
));
#else
EXTERN S16 pjLibUpInitReq ARGS ((
PjSecCxt secCxt,                  /* Context for Intg protection/verificatino */
U8       algoType,                /* Type of the algorithm */
U8       *upKey,                  /* User plane ciphering key */
Void     **cxtId                   /* FD */
));
#endif
EXTERN S16 pjLibDecipherReq ARGS((
PTR         intCxtId,           /* Context Id for compression */
PjSecInp    secInp,             /* Input for Deciphering */
Buffer      *mBuf,              /* SDU to be deciphered */
Buffer      **opSdu             /* deciphered SDU */
));
EXTERN S16 pjLibIntVerReq  ARGS((
PTR        intCxtId,                /* Context Id for compression */
PjSecInp   secInp,                  /* Input for Integrity protection */                  
Buffer     *mBuf,                   /* SDU to be compressed */
U32        macI,                    /* Message authentication code for the SDU */
Status     *status                  /* Status of the Integrity verification */
));
#endif

#if defined(PJ_CMP_ASYNC) || defined(PJ_SEC_ASYNC)
EXTERN S16 cmPkPjLibTrans ARGS((
   PjLibTrans           *param,
   Buffer               *mBuf
));
EXTERN S16 cmUnpkPjLibTrans ARGS((
   PjLibTrans           *param,
   Buffer               *mBuf
));

#endif /* PJ_CMP_ASYNC || PJ_SEC_ASYNC */
/* kw003.201 */

/* kw004.201 Guarded under PTPJLIB */
#ifdef PTPJLIB 
EXTERN S16 pjLibPktAdmCtrl ARGS((
   PjDlRbCb             *pjRbCb,  
   Buffer               *pkt
));
#endif
#if defined(L2_L3_SPLIT) && defined(ICC_RECV_TSK_RBUF)
EXTERN S16 pjUlBatchProcSplit ARGS ((Void));
#endif
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __PJ_LIB_X__ */
  
/********************************************************************30**
  
         End of file
**********************************************************************/
