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
  
        Name:    LTE PDCP file 
    
        Type:    C include file
  
        Desc:    This file contains all the data structures and 
                 prototypes for LTE PDCP.
 
        File:    pj_dl.x
  
*********************************************************************21*/
/** @file pj_dl.x
@brief PDCP Product Structures, prototypes
*/

#ifdef TENB_STATS
#include "pj_tenb_stats.x"   
#endif


#ifndef __PJ_DL_X__
#define __PJ_DL_X__


/*
 *     The structures and variables declared in this file
 *     correspond to structures and variables used by
 *     the following TRILLIUM software:
 *
 *     part no.             description
 *     --------     ----------------------------------------------
 *     1000371      LTE-RLC
 *
*/


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct pjDlUeCb PjDlUeCb;
typedef struct pjDlRbCb PjDlRbCb;               /* PDCP RB control block */



/** @addtogroup dldata */
/*@{*/

/**
 *  @brief
 *  PDCP SDU Status Information for a DRB mapped to RLC AM.
 */

typedef struct pjDlHoCfmInfo
{
   U8         rbId;                     /*!< RB ID */
   Bool       pres;                     /*!< Is handover info present for this RB */
   U8         dir;                      /*!< Direction of the RB */
   U8         res1;
   U32        count;                    /*!< Count of the next SN to be assigned */
}PjDlHoCfmInfo;

/**
 *  @brief
 *  Handover information maintainned during processing of SduStaReq
 */
typedef struct pjDlHoInfo
{
   U32           transId;                   /*!< Transaction ID */
   PjDlHoCfmInfo   *hoCfmInfo;                /*!< SDU Status Confirm */
}PjDlHoInfo;

/** @struct PjTxEnt
PDCP Tx Entry - SDU received from Relay */
typedef struct pjTxEnt
{
   CmLList     lstEnt;                      /*!< List entry for SDU */
   CmLList     datCfmEnt;                   /*!< List entry for SDU */
   CmLList     sduSubmitEnt;                /*!< List entry for SDU */
   CmLList     reEstPktEnt;                /*!< List entry for SDU */
   U8          state;                       /*!< SDU state */
   U8          res;                         /*!< reserved */
   PjSn        sn;                          /*!< PDCP SN associated with this SDU */
   U32         count;                       /*!< COUNT associated with this SDU */  
   Data        *ciphDBuf;                   /*Pointer to flat buffer created for ciphering */
   Buffer      *pdu;                        /*!< Pdu Buffer */
   Buffer      *sdu;                        /*!< Sdu Buffer */
#ifdef FLAT_BUFFER_OPT
   FlatBuffer  fb;
#endif
   U32         sduId;                       /*!< SDU ID */
   PjDlRbCb      *rbCb;
   MsgLen         bufLen;
#ifdef LTE_L2_MEAS
   EpcTime     epcTime;                     /*!< Arrival Time of the SDU for dl delay */
#endif
}PjTxEnt;

/** @struct PjDlCb 
PDCP downlink control block */
typedef struct pjDlCb
{
   S16         discTmrInt;                  /*!< Discard timer Interval */
   U32         txNext;                       /*!<indicates the COUNT value of the next PDCP SDU to be transmitted */
   Bool        cfmReqd;                     /*!< Flag to indicate if
                                                 confirmation is required for the
                                                 SDUs delivered to the peer PDCP */
   U8          res[3];                      /*!< reserved */
   PjBuf       txBuf;                       /*!< Transmission buffer */
   U32         count;                       /*!< DL COUNT associated with the
                                                 recent SDU */
   PjSn        sn;                          /*<Sn for PDCP SDU */
   U32         nxtToSub;                    /*!< COUNT of the next PDU to be
                                                 submitted to RLC */
   U32         cfmExp;                      /*!< COUNT of the next PDU for which
                                                 confirmation is expected */
   CmTimer     discTmr;                     /*!< Discard timer */
#if (defined(PJ_SEC_ASYNC) || defined (PJ_CMP_ASYNC))
   CmTimer     obdTmr;                      /*!< Offboard timer */
   U32         obdCount;                    /*!< OBD Count */
#endif /* (defined(PJ_SEC_ASYNC) || defined (PJ_CMP_ASYNC)) */
   CmLListCp   dlPktQ;                   /*!< Queues the message during
                                             Re-establishment/Handover */ 
   Buffer     *staRep;
}PjDlCb;
/*@}*/


/** @struct PjRbCb 
PDCP RB control block */
struct pjDlRbCb
{
   CmLteRbId   rbId;                        /*!< RB Id for PDCP Cb */
   U8          rbType;                      /*!< RB Type : 0 - PJ_SRB, 1 - PJ_DRB */
   CmLteRlcMode mode;                       /*!< Mapped RLC entity's mode */
   U8          dir;                         /*!< Direction for UL/DL/biderctional */
   U8          snLen;                       /*!< Sequence number length */
   U8          state;                       /*!< RB state - Used only for DRBs*/
   PjSn        firstSn;                     /*!< SN of the first message sent
                                                 for Integrity Protection */
   PjRohc      rohc;                        /*!< ROHC Information */ 
   PjCmpCxt    cmpCxt;                      /*!< Context for ROHC */
   PTR         cmpCxtId;                    /*!< Context Id for ROHC */ 
   Bool        firstDlMsg;                  /*!< Flag for firstDlMsg 
                                                 Set to FALSE at start */
   Bool        dropOnlyOne;              /*!< Indicates whether too many packets 
                                                 are waiting on this RB */
   U32         pktAdmitCnt;              /*!< Indicates how many packets 
                                                 can be admitted */
   U32         pktCnt;                   /*!< counts number of packets 
                                                 admitted */
/* pj005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
   U8          qci;                         /*!< QCI value associated with rb */
   PjL2MeasRbCb rbL2Cb;                     /*!< L2 Meas Cb for RB */
#endif /* LTE_L2_MEAS */
/** @defgroup dldata DL Module Info 
*/
   PjDlCb      dlCb;                        /*!< Downlink PDCP RbCb */
   PjDlUeCb    *ueCb;                       /*!< Pointer to the UE in which RB is present */
   U32         maxTxSn;                     /*!< Sequence number length */
   PjUeKey     key;                       /*!< Hash list key for UeCb */
   CmLListCp   dlPktLst;                    /*!< List of DL packets */
   U32         rbOverloadCnt;              /*!< Count to track consecutive PDB breach report on an RB*/ 
};

/** @brief SDU Confirmation Structure from RLC */
typedef struct pjDatCfm
{
   CmLteRlcId        rlcId;                 /*!< RLC identifier */
   U32               numSdu;                /*!< Number of SDUs */
   U32               sduId[PJ_MAX_DAT_CFM]; /*!< SDU Id Array */
}PjDatCfm;


/** @struct PjUeCb
UE control block */
struct pjDlUeCb
{
   CmHashListEnt ueHlEnt;                   /*!< Hash list entry for UeCb */
   Inst          inst;
   PjUeKey       key;                       /*!< Hash list key for UeCb */ 
   U8            numSrbs;
   U8            numDrbs;
   PjDlRbCb     *srbCb[PJ_MAX_SRB_PER_UE]; /*!< SRB RbCbs within a UE */ 
   PjDlRbCb     *drbCb[PJ_MAX_DRB_PER_UE]; /*!< DRB RbCbs within a UE */ 
   PjSec         secInfo;                   /*!< Security Info */
   PjDlHoInfo      *hoInfo;                   /*!< SDU Status Confirm */
   PjLibInfo     libInfo;                   /*!< Hook Info - to wait for
                                                 Init Cfm */
   U32            waitForReEstTrig;

/* pj005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
   U16           numActRb[LPJ_MAX_QCI]; /* number of RBs Active */
#endif /* LTE_L2_MEAS */
#ifdef TENB_STATS
   TSL2UeStatsCb   *tenbStats; /*!< UE Stats Holder */
#endif
};

typedef struct pjDlPkt
{
   CmLList lnk;
   U8      type;
   U32     sduId;
   PjSn    sn;
   Buffer  *pdu;
#ifdef FLAT_BUFFER_OPT
   FlatBuffer  fb;
#endif
} PjDlPkt;

/* CFG Module */
EXTERN S16 pjCfgAddDlRb ARGS ((PjCb *gCb,CmLteRnti  ueId,CmLteCellId  cellId,
            U8 isHo, UdxCfgEnt  *entCfg, PjDlUeCb **ueCb,UdxCfmEnt  *entCfm ));

EXTERN S16 pjCfgReCfgDlRb ARGS ((PjCb *gCb,PjDlUeCb *ueCb,
                          UdxCfgEnt *entCfg, UdxCfmEnt *entCfm));

EXTERN S16 pjCfgDelDlRb ARGS ((PjCb *gCb,PjDlUeCb *ueCb,
                        UdxCfgEnt *entCfg,UdxCfmEnt *entCfm));

EXTERN S16 pjCfgReEstDlRb ARGS ((
PjCb             *gCb,
PjDlUeCb         *ueCb,
UdxCfgEnt         *entCfg,
UdxCfmEnt         *entCfm
));

EXTERN S16 pjCfgDelDlUe ARGS ((
PjCb             *gCb,
PjDlUeCb         *ueCb,
UdxCfgEnt         *entCfg,
UdxCfmEnt         *entCfm
));

EXTERN S16 pjCfgPrcSecDlCfg ARGS ((
PjCb             *gCb,
UdxSecCfgReqInfo *secCfg,
UdxSecCfgCfmInfo *cfgCfm
));

EXTERN S16 pjCfgDlUeIdChng ARGS((
PjCb      *gCb,
UdxUeInfo *ueInfo,
UdxUeInfo *newUeInfo,
CmStatus  *status
));

EXTERN S16 pjCfgPrcDlCount ARGS ((
PjCb                  *gCb,
UdxCountReqInfo       *countReq,
UdxCountCfmInfo       *countCfm
));

EXTERN S16  pjCfgDelPjDlRbCb ARGS ((
PjCb                  *gCb,
PjDlUeCb               *ueCb,
PjDlRbCb                 *rbCb
));


EXTERN  S16 pjCfgDlReEstReq ARGS ((
PjCb                  *gCb,
UdxReEstReqInfo *reEstReq,
UdxReEstCfmInfo   *reEstCfm
));
/* DB Module */
EXTERN S16 pjDbmInsTxEnt  ARGS ((
PjCb        *gCb,
PjBuf       *buf,             
PjTxEnt     *txEnt       
));
EXTERN S16 pjDbmDlBufInit ARGS ((  
PjCb        *gCb,
PjBuf       *buf,            /* !< Rx/Tx Buffer */
U8          numBins          /* !< number of Bins */
));

EXTERN PjTxEnt* pjDbmGetTxEnt ARGS ((
PjCb        *gCb,
PjBuf       *buf,        
U32          count      
));
EXTERN PjTxEnt* pjDbmGetTxEntSn ARGS ((
PjCb        *gCb,
PjBuf       *buf,      
U16          sn          
));
EXTERN S16 pjDbmDelTxEnt  ARGS ((
PjCb        *gCb,
PjBuf       *buf,    
U32         count     
));
EXTERN S16 pjDbmTxDelAll ARGS((
PjCb        *gCb,
PjBuf *buf       
));
EXTERN S16 pjDbmTxDeInit ARGS ((
PjCb        *gCb,
PjBuf *buf       
));

EXTERN S16 pjDbmFetchDlUeCb ARGS ((
PjCb        *gCb,
CmLteRnti         ueId,       /* UE Identifier */
CmLteCellId       cellId,     /* Cell Identifier */
PjDlUeCb            **ueCb       /* UE Control Block */
));

EXTERN S16 pjDbmCreateDlUeCb ARGS ((
PjCb        *gCb,
CmLteRnti         ueId,       /* UE Identifier */
CmLteCellId       cellId,     /* Cell Identifier */
PjDlUeCb            **ueCb       /* UE Control Block */
));
EXTERN  PjDlRbCb* pjDbmCreateDlRbCb ARGS ((
PjCb        *gCb,
U8      rbId,
U8      rbType,
PjDlUeCb  *ueCb,
U8      rb
));
EXTERN  S16 pjDbmDelDlRbCb ARGS ((
PjCb        *gCb,
U8      rbId,
U8      rbType,
PjDlRbCb   **rbCb,
U8      rb,
PjDlUeCb  *ueCb
));

EXTERN S16 pjDbmDelDlUeCb  ARGS((
PjCb        *gCb,
PjDlUeCb      *ueCb,       /* UE Identifier */
Bool        abortFlag    /* Abort Flag */
));

EXTERN S16 pjDbmFetchDlRbCb ARGS((
PjCb           *gCb,
CmLteRlcId     rlcId,      /* RLC Identifier */
PjDlRbCb       **rbCb       /* RB Cb */
));

EXTERN S16 PjDbmDelAllDlUe ARGS((PjCb *gCb));
EXTERN S16 pjDbmDelAllDlRb ARGS((
PjCb        *gCb,
PjDlRbCb         **rbCbLst,              /* RB Cb list */
U8             numRbCb                 /* Number of rbCbs */
));

EXTERN S16 pjDbmDlInit  ARGS (( PjCb *gCb));
EXTERN S16 pjDbmDlDeInit  ARGS (( PjCb *gCb));
EXTERN S16 pjDbmDlShutdown ARGS((PjCb *gCb));

EXTERN S16 pjDlUtlCmpReset ARGS(( 
PjCb        *gCb,
PjDlRbCb      *pjRbCb                 /* Context to be reset for compression */
));

/* UTL Module */
EXTERN S16 pjUtlDlCmpInit ARGS((
PjCb        *gCb,
PjDlRbCb   *pjRbCb                  /* PDCP RbCb */
));
EXTERN S16 pjUtlDlIntInit ARGS((
PjCb        *gCb,
PjDlUeCb   *ueCb                   /* UE CB Ptr */
));
EXTERN S16 pjUtlDlCpInit ARGS((
PjCb        *gCb,
PjDlUeCb   *ueCb                    /* UE CB Ptr */
));
EXTERN S16 pjUtlDlUpInit ARGS((
PjCb        *gCb,
PjDlUeCb   *ueCb                    /* UE CB Ptr */
));
EXTERN S16 pjUtlDlCmpReset ARGS((
PjCb        *gCb,
PjDlRbCb      *pjRbCb                 /* Context to be reset for compression */
));
EXTERN S16 pjUtlCmpReq ARGS((
PjCb        *gCb,
PjDlRbCb   *pjRbCb,                 /* PDCP RbCb */
Buffer   *mBuf,                   /* SDU to be compressed */
Buffer   **opSdu,                 /* Compressed SDU */
U32      count                    /* COUNT - transaction Id */
));
EXTERN S16 pjUtlIntProtReq ARGS((
PjCb        *gCb,
PjDlRbCb   *pjRbCb,                 /* PDCP RbCb */
PjSecInp secInp ,                 /* Input parameters for integrity */ 
Buffer   **mBuf                   /* SDU to be compressed */
));
EXTERN S16 pjUtlCipherReq ARGS((
PjCb        *gCb,
PjDlRbCb   *pjRbCb,               /* PDCP RbCb */
U32      count,                   /* COUNT associated with SDU/PDU */ 
Buffer   *mBuf,                   /* Data to be ciphered */
Buffer   **opSdu                  /* ciphered SDU */
));

#ifdef FLAT_BUFFER_OPT
EXTERN S16 pjUtlCipherReqFB ARGS((
PjCb       *gCb,
PjDlRbCb   *pjRbCb,                 /* PDCP RbCb */
U32        count,                   /* COUNT associated with SDU/PDU */ 
FlatBuffer *mBuf,                   /* Data to be deciphered */
Buffer   **opSdu                    /* Deciphered SDU */ 
));
#endif

EXTERN S16 pjUtlDlCmpClose ARGS((
PTR      cmpCxtId                 /* ROHC Context to be closed */
));
EXTERN S16 pjUtlDlIntClose ARGS((
PjCb     *gCb,
Void     *intCxtId            /* Handle to be closed */
));

EXTERN S16 pjUtlDlCipherClose ARGS((
PjCb     *gCb,
Void     *cpCxtId                  /* Context Id for Ciphering to be closed */
));
EXTERN S16 pjUtlDlReEstSrb1 ARGS((
PjCb        *gCb,
PjDlRbCb      *pjRbCb                    /* PDCP Control Block Pointer */
));
EXTERN S16 pjUtlDlReEstStart ARGS((
PjCb        *gCb,
PjDlUeCb         *ueCb,          /* UE Control Block Pointer */
UdxCfgReqInfo  *cfgReq         /* CfgReq for Reestablishment */
));
EXTERN S16 pjUtlDlReEstHOStart ARGS((
PjCb        *gCb,
PjDlUeCb      *ueCb,           /* Number of RBs undergoing reestablishment*/ 
U32         transId,          /* Transaction id of Sdu Status cfm */
U32       waitForHoTrig
));
EXTERN S16 pjUtlDlReEstReconfig ARGS((
PjCb        *gCb,
PjDlUeCb       *ueCb           /* UE Control Block */ 
));
EXTERN S16 pjUtlDlReEstDl ARGS((
PjCb        *gCb,
PjDlUeCb       *ueCb           /* UE Control Block */ 
));
EXTERN S16 pjUtlDlSndReEstCfgCfm ARGS((
PjCb        *gCb,
PjDlUeCb       *ueCb           /* UE Control Block */ 
));
EXTERN S16 pjUtlDlSndSduStaCfm ARGS((
PjCb        *gCb,
PjDlUeCb       *ueCb           /* UE Control Block */ 
));
EXTERN S16 pjUtlDlShutdown ARGS((
PjCb        *gCb
));
EXTERN Void pjUtlDlFreeRb ARGS((
PjCb        *gCb,
PjDlRbCb *pjRbCb
));
/* kw005.201 added function to send data forward indication mesage */
EXTERN S16 pjUtlDlSndDatFwdInd ARGS((
PjCb        *gCb,
PjDlRbCb           *pjRbCb,
PjuDatFwdIndInfo *datFwdInd
));

EXTERN S16 pjUtlDlHdlObdInitCfm ARGS((
PjCb        *gCb,
PjDlUeCb    *ueCb,           /* Pointer to UeCb */ 
U16         txIdx,           /* Transaction Index for UeCb */
U8          cfmType,         /* Confirm type */
U8          maskVal          /* mask value */
));
EXTERN S16 pjUtlDlHdlInitObdTmrExp ARGS((
PjCb      *gCb,
PjDlUeCb *ueCb
));

EXTERN S16 pjCfgPrcDlSduSta ARGS((
PjCb        *gCb,
UdxSduStaReqInfo *staReq   
));

EXTERN S16 pjUtlDlSaveCfmInfo  ARGS((
PjCb        *gCb,
PjDlUeCb      *ueCb,           /* Pointer to UeCb */ 
U8          cfmType,         /* Confirm type */
Bool        startTmr,        /* Whether to start timer */
U8          entity,          /* Confirm type */
PTR         cfmPtr,          /* Pointer to the structure for confirm */
PTR         cfgPtr           /* Pointer to the structure for request */
));

/* DL Module */
EXTERN S16 pjDlmProcessSdus ARGS ((
PjCb        *gCb,
PjDlRbCb *pjRbCb, 
Buffer *sdu,
U32    sduId,
U32    count
));

EXTERN S16 pjDlmProcDrb ARGS ((
PjCb        *gCb,
PjDlRbCb  *pjRbCb,
PjTxEnt *txEnt
));

#ifdef FLAT_BUFFER_OPT
EXTERN S16 pjDlmProcDrbFB ARGS ((
PjCb        *gCb,
PjDlRbCb  *pjRbCb,
PjTxEnt *txEnt
));
#endif

EXTERN S16 pjDlmProcSrb ARGS ((
PjCb        *gCb,
PjDlRbCb  *pjRbCb,
PjTxEnt *txEnt
));
EXTERN Void pjDlmReEstSrb ARGS ((
PjCb        *gCb,
PjDlRbCb *pjRbCb
));

EXTERN S16 pjDlmReEstDrbUm ARGS ((
PjCb        *gCb,
PjDlRbCb *pjRbCb
));

EXTERN S16 pjDlmReEstDrbAm ARGS ((
PjCb        *gCb,
PjDlRbCb *pjRbCb
));
EXTERN S16 pjDlmProcessCfm ARGS ((
PjCb        *gCb,
PjDlRbCb        *pjRbCb,
PjDatCfm      *pjDatCfm,
U8            cfmType
));
EXTERN S16 pjDlmHndlStaRep ARGS ((
PjCb        *gCb,
PjDlRbCb   *pjRbCb,
PjSn     fms,
Buffer   *staPdu
));
EXTERN S16 pjDlmReEstHoDrbAm ARGS ((
PjCb        *gCb,
PjDlRbCb *pjRbCb
));
EXTERN S16 pjDlmHdlDatFwdReq ARGS ((
PjCb        *gCb,
PjDlRbCb  *pjRbCb,
PjuDatFwdReqInfo *datFwdReq
));
EXTERN S16 pjDlmDeliverPdu ARGS ((
PjCb        *gCb,
PjDlRbCb     *pjRbCb,
PjTxEnt    *txEnt
));
EXTERN S16 pjDlmObdTmrExp ARGS ((
PjCb        *gCb,
PjDlRbCb     *pjRbCb
));
EXTERN  S16 pjDlmHdlIntProt ARGS ((
PjCb        *gCb,
PjDlRbCb  *pjRbCb,
PjTxEnt *txEnt
));
EXTERN S16 pjDlmHdlCmp ARGS (( 
PjCb        *gCb,
PjDlRbCb  *pjRbCb,
PjTxEnt *txEnt
));

#ifdef FLAT_BUFFER_OPT
EXTERN S16 pjDlmHdlCmpFB ARGS (( 
PjCb        *gCb,
PjDlRbCb  *pjRbCb,
PjTxEnt *txEnt
));
#endif

EXTERN S16 pjDlmHdlCiph ARGS ((
PjCb        *gCb,
PjDlRbCb  *pjRbCb,
PjTxEnt *txEnt
));

#ifdef FLAT_BUFFER_OPT
EXTERN S16 pjDlmHdlCiphFB ARGS ((
PjCb        *gCb,
PjDlRbCb  *pjRbCb,
PjTxEnt *txEnt
));
#endif

EXTERN S16 pjDlmDiscTmrExp ARGS ((
PjCb        *gCb,
PjDlRbCb  *pjRbCb
));

EXTERN S16 pjDlmSendDatReq ARGS(( PjCb *gCb, PjDlRbCb *pjRbCb, U32 sduId, 
                                Buffer *mBuf));
EXTERN S16 pjDlmSendDiscSdu ARGS(( PjCb *gCb, PjDlRbCb  *pjRbCb, U32 sduId));

EXTERN Void pjLimDatCfm ARGS((
PjCb          *gCb,
PjDlRbCb      *pjRbCb,
PjDatCfm    *datCfm 
));
EXTERN S16 pjUtlReEstDl ARGS (( 
PjCb        *gCb,
PjDlUeCb       *ueCb           /* UE Control Block */
));

EXTERN Void pjLimStaInd  ARGS((
PjCb        *gCb,
PjDlRbCb    *pjRbCb,
PjDatCfm    *staInd
));

EXTERN S16 pjDbmFetchPjDlRbCb ARGS((
PjDlUeCb           *ueCb,
U8      rbId,
U8      rbType,
PjDlRbCb      **pjRbCb
));

EXTERN Void pjDlmProcessDlPktQ ARGS((
PjCb       *gCb,
PjDlRbCb   *pjRbCb           /* !< PDCP Control Block */
));

EXTERN Void pjDlmEnqueueDlPkt ARGS((
PjCb               *gCb, 
PjDlRbCb           *pjRbCb,          /* !< PDCP Control Block */
U8                 datType,
PjuDatFwdReqInfo   *datFwd,          /* !< SN value of PDU */
PjuSduId           sduId,
Buffer             *pdu              /* !< PDU message buffer */
));

#ifdef FLAT_BUFFER_OPT
EXTERN Void pjDlmEnqueueDlPktFB ARGS((
PjCb               *gCb, 
PjDlRbCb           *pjRbCb,          /* !< PDCP Control Block */
U8                 datType,
PjuDatFwdReqInfo   *datFwd,          /* !< SN value of PDU */
PjuSduId           sduId,
FlatBuffer         *pdu             /* !< Flat Buffer PDU */ 
));

EXTERN Void pjDlmRbDataResumeFB ARGS((
PjCb   *gCb,
PjDlRbCb *pjRbCb
));

EXTERN Void pjDlmProcessDlPktQFB ARGS((
PjCb       *gCb,
PjDlRbCb   *pjRbCb           /* !< PDCP Control Block */
));

EXTERN S16 pjDlmStartDataFrwdPerRbFB ARGS((
PjCb       *gCb,
PjDlRbCb   *pjRbCb
));

#endif

EXTERN S16 pjDlmStartDataFrwdPerRb ARGS((
PjCb       *gCb,
PjDlRbCb   *pjRbCb
));

EXTERN S16 pjDlmHndlDatFwdReq ARGS((
PjCb               *gCb,
PjDlRbCb           *pjRbCb,
PjuDatFwdReqInfo   *datFwdReq
));

EXTERN S16 pjUtlDlUpdUpSecKeys ARGS((
PjCb       *gCb,
PjDlUeCb   *ueCb           /* UE Control Block */ 
));

EXTERN S16 pjUtlDlUpdCpSecKeys ARGS((
PjCb       *gCb,
PjDlUeCb   *ueCb           /* UE Control Block */ 
));

EXTERN S16 pjUtlDlReEstHO ARGS((
PjCb              *gCb,
PjDlUeCb          *ueCb           /* Number of RBs undergoing reestablishment*/ 
));

EXTERN Void pjUtlEmptyDlPktList ARGS((
PjCb *gCb, 
PjDlRbCb *pjRbCb));

EXTERN Void pjDlmRbDataResume ARGS((
PjCb   *gCb,
PjDlRbCb *pjRbCb
));

EXTERN PUBLIC S16 pjDlmHdlNullCiph ARGS((
PjCb       *gCb,
PjDlRbCb     *pjRbCb,
PjTxEnt    *txEnt
));

#ifdef FLAT_BUFFER_OPT
EXTERN PUBLIC S16 pjDlmProcessSdusFB ARGS((
PjCb   *gCb,
PjDlRbCb *pjRbCb,                   
FlatBuffer *sdu,                      
U32    sduId,                      
U32        count
));

EXTERN PUBLIC S16 pjUtlConvFbToBuf ARGS((
PjCb       *gCb,
FlatBuffer *fb,
Buffer     **mBuf
));

EXTERN PUBLIC S16 pjUtlCopyFbToBuf ARGS((
PjCb       *gCb,
FlatBuffer *fb,
Buffer     **mBuf
));
#endif

#ifdef LTE_L2_MEAS
S16 pjUtlL2MeasDlInit(PjCb *gCb);

EXTERN Void pjUtlResetDlL2MeasCntr ARGS((
PjCb         *gCb,
PjL2MeasCb   *measCb,
U8           measType
));

EXTERN Void pjUtlPlcMeasDatInDlL2Sts ARGS((
PjL2Cntr       *measData, 
PjL2MeasRbCb   *rbL2Cb,
U8             measType
));
S16 pjUtlSndDlL2MeasCfm  ARGS ((PjCb *gCb, PjL2MeasEvtCb *measEvtCb));

#endif


void DumpPDCPDlDebugInformation(void);
void dumpPDCPDlRbInformation(PjDlRbCb* dlRbCb, U16 ueId);
void ResetPDCPStats(void);
void PrintPDCPStats(void);
EXTERN Void pjDeInitCiphQ(Void);
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __PJX__ */

  
/********************************************************************30**
  
         End of file
**********************************************************************/
