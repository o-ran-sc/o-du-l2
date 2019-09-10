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
 
        File:    pj_ul.x
  
*********************************************************************21*/
/** @file pj_ul.x
@brief PDCP Product Structures, prototypes
*/

#ifndef __PJ_UL_X__
#define __PJ_UL_X__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/** @struct PjRbCb 
RLC RB control block */
typedef struct pjUlUeCb PjUlUeCb;
typedef struct pjUlRbCb PjUlRbCb;               /* PDCP RB control block */

/** @addtogroup dldata */
/*@{*/

/**
 *  @brief
 *  PDCP SDU Status Information for a DRB mapped to RLC AM.
 */

typedef struct pjUlHoCfmInfo
{
   U8          rbId;                     /*!< RB ID */
   Bool        pres;                     /*!< Is handover info present for this RB */
   U8          dir;                      /*!< Direction of the RB */
   U8          res1;
   U8          numBits;                  /*!< Number of bits in UL bitmap */
   U8          res2[3];
   U8          *ulBitMap;                /*!< Status Report bitmap of PDUs */
   U32         count;                    /*!< UL Count value with FMS */
}PjUlHoCfmInfo;

/**
 *  @brief
 *  Handover information maintainned during processing of SduStaReq
 */
typedef struct pjUlHoInfo
{
   U32             transId;                   /*!< Transaction ID */
   UdxSduStaCfmInfo *staCfm;                  /*!< SduStaCfm received from DL */
   PjUlHoCfmInfo   *hoCfmInfo;                /*!< SDU Status Confirm */
}PjUlHoInfo;

/** @defgroup uldata UL Module Info 
*/
/** @addtogroup uldata */
/*@{*/

/** @struct PjRxEnt
PDCP Rx Entry - PDU received from RLC */
typedef struct pjRxEnt
{
   CmLList     lstEnt;                      /*!< List entry for PDU */
   CmLList     datPktEnt;                   /*!< List entry for PDU */
   U8          state;                       /*!< PDU state */
   Bool        discFlag;                    /*!< Whether entry can be discarded 
                                                 after processing */
   Bool        dupEntry;                    /*!< Duplicate Entry, These entries
                                                 are not inserted */
   Bool        res;                         /*!< Reserved */
   U32         count;                       /*!< COUNT associated with this PDU */
   Buffer      *mBuf;                       /*!< Buffer to store the pdu during processing */
#ifdef SS_RBUF 
   PjUlRbCb    *rbCb;
#endif
   Bool        isOutSeq;                    /*!< flag to tell whether this is a in-sequence packet or not */
}PjRxEnt;

/** @struct PjUlCb 
PDCP uplink control block */
typedef struct pjUlCb
{

   U32         rxNext;                       /*next expected pdcp pdu counter */
   U32         rxDeliv;                      /*next delivered pdu count to the upper layer*/
   U32         rxReord;                      /* Holds the count value of Data PDU which triggered t-Reordering */
   Bool        outOfOrderDelivery;           /*Out of Delivery is configured or not*/
   Bool        staRepReqd;                  /*!< Flag to indicate if status report 
                                                 is required */
   Bool        discReqd;                    /*!< Flag to indicate if the PDU can
                                                 be discarded during Integrity
                                                 failure */
   Bool        transCmp;                    /*!< Whether transmission of PDUs from RLC
                                                 during reestablishment is complete */ 
   U8          state;                       /*!< RB state for the UL entity */
   PjBuf       recBuf;                      /*!< Reception buffer */
   U32         nxtSubCnt;                   /*!< Count of next SDU to be submitted 
                                                 to the upper layer. Used in SRB, DRBUM */
   U32         nxtSubDeCmp;                 /*!< Count of next PDU to be submitted 
                                                 to the decompression unit */
   
   U32         obdPdu;                      /*!< Count of the Pdu sent for offboarding  */
   U32         firstReEstCnt;               /*!< Count of the first PDU received after
                                                 reestablishment was triggered */ 
#if (defined(PJ_SEC_ASYNC) || defined (PJ_CMP_ASYNC))
   CmTimer     obdTmr;                      /*!< Offboard timer */
   U32         obdCnt;                    /*!< OBD Count */
#endif /* (defined(PJ_SEC_ASYNC) || defined (PJ_CMP_ASYNC)) */
   CmLListCp   ulPktLst;                   /*!< Queues the message during
                                             Re-establishment/Handover */ 
   U32         fmsCount;                    /*!< To store the Count value (FMS + HFN) at the time Out-Of-Sequence */
   /* Added for UL UU  LOSS */
#ifdef LTE_L2_MEAS
   PjSn        nxtSubSn;                     /*!< PDCP SN to be submitted to UL next */
#endif
   CmTimer     tReordTmr;                      /*!< t-Reorder timer */
}PjUlCb;
/*@}*/


/** @struct PjRbCb 
PDCP RB control block */
struct pjUlRbCb
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
/* pj005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
   U8          qci;                         /*!< QCI value associated with rb */
   PjL2MeasRbCb rbL2Cb;                     /*!< L2 Meas Cb for RB */
#endif /* LTE_L2_MEAS */
/** @defgroup dldata DL Module Info 
*/
   PjUlCb      ulCb;                        /*!< Uplink PDCP RbCb */ 
   PjUlUeCb      *ueCb;                       /*!< Pointer to the UE in which RB is present */
   PjSec         secInfo;                   /*!< Security Info */
   U16           reOrdrTmrVal;              /* t-Reordering timer value in ms*/
};


/** @struct PjUeCb
UE control block */
struct pjUlUeCb
{
   CmHashListEnt ueHlEnt;                   /*!< Hash list entry for UeCb */
   Inst          inst; 
   PjUeKey       key;                       /*!< Hash list key for UeCb */
   U8            numSrbs;
   U8            numDrbs;
   PjUlRbCb        *srbCb[PJ_MAX_SRB_PER_UE]; /*!< SRB RbCbs within a UE */ 
   PjUlRbCb        *drbCb[PJ_MAX_DRB_PER_UE]; /*!< DRB RbCbs within a UE */ 
   PjSec         secInfo;                   /*!< Security Info */
   PjUlHoInfo     *hoInfo;                   /*!< SDU Status Confirm */
   PjLibInfo     libInfo;                   /*!< Hook Info - to wait for
                                                 Init Cfm */
/* pj005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
   U16           numActRb[LPJ_MAX_QCI]; /* number of RBs Active */
#endif /* LTE_L2_MEAS */
   Bool          isUlDatFwdReqd;       /*!< To identify whether App rqstd for UL Data Fwding or not */
};

typedef struct pjUlPkt
{
   CmLList lnk;
   PjSn    sn;
   Buffer  *pdu;
   U8      type;
} PjUlPkt;

/* CFG Module */
EXTERN S16 pjCfgAddUlRb ARGS ((PjCb *gCb,CmLteRnti  ueId,CmLteCellId  cellId,U8 isHo,
             CpjCfgEnt  *entCfg,PjUlUeCb **ueCb, CpjCfmEnt  *entCfm ));

EXTERN S16 pjCfgReCfgUlRb ARGS ((PjCb *gCb,PjUlUeCb *ueCb,
                          CpjCfgEnt *entCfg, CpjCfmEnt *entCfm));

EXTERN S16 pjCfgDelUlRb ARGS ((PjCb *gCb,PjUlUeCb *ueCb,
                        CpjCfgEnt *entCfg,CpjCfmEnt *entCfm));

EXTERN S16 pjCfgReEstUlRb ARGS ((
PjCb             *gCb,
PjUlUeCb         *ueCb,
CpjCfgEnt         *entCfg,
CpjCfmEnt         *entCfm,
UdxCfmEnt         *entDlCfgCfm 
));

EXTERN S16 pjCfgDelUlUe ARGS ((
PjCb             *gCb,
PjUlUeCb         *ueCb,
CpjCfgEnt         *entCfg,
CpjCfmEnt         *entCfm
));

EXTERN S16 pjCfgPrcSecUlCfg ARGS ((
PjCb             *gCb,
CpjSecCfgReqInfo *secCfg,
CpjSecCfgCfmInfo *cfgCfm
));

EXTERN S16 pjCfgUlUeIdChng ARGS((
PjCb      *gCb,
CpjUeInfo *ueInfo,
CpjUeInfo *newUeInfo,
CmStatus  *status
));

EXTERN S16 pjCfgPrcUlCount ARGS ((
PjCb                  *gCb,
CpjCountReqInfo       *countReq,
CpjCountCfmInfo       *countCfm,
UdxCountCfmInfo       *cntCfm
));

EXTERN S16  pjCfgDelPjUlRbCb ARGS ((
PjCb                  *gCb,
PjUlUeCb               *ueCb,
PjUlRbCb                 *rbCb
));

EXTERN S16 pjUtlUlCipherClose ARGS((
PjCb     *gCb,
Void     *cpCxtId                  /* Context Id for Ciphering to be closed */
));

EXTERN S16 pjUtlUlIntClose ARGS((
PjCb     *gCb,
Void     *intCxtId                 /* Integration CxtId to be closed */
));

EXTERN  S16 pjCfgUlReEstReq ARGS ((
PjCb                  *gCb,
CpjReEstReqInfo *reEstReq,
CpjReEstCfmInfo   *reEstCfm
));
/* DB Module */
EXTERN S16 pjDbmInsRxEnt  ARGS ((
PjCb        *gCb,
PjBuf       *buf,             
PjRxEnt     *rxEnt,
Bool        dupFlag
));
EXTERN S16 pjDbmBufInit ARGS ((
PjCb        *gCb,
PjBuf       *buf,          
U8          numBins          
));
EXTERN PjRxEnt* pjDbmGetRxEnt ARGS ((
PjCb        *gCb,
PjBuf       *buf,        
U32          count      
));
EXTERN PjRxEnt* pjDbmGetRxEntSn ARGS ((
PjCb        *gCb,
PjBuf       *buf,      
U16          sn          
));
EXTERN S16 pjDbmDelRxEnt  ARGS ((
PjCb        *gCb,
PjBuf       *buf,    
U32         count     
));
EXTERN S16 pjDbmRxDelAll ARGS((
PjCb        *gCb,
PjBuf *buf       
));
EXTERN S16 pjDbmRxDeInit ARGS ((
PjCb        *gCb,
PjBuf *buf       
));

EXTERN S16 pjDbmUlDeInit ARGS ((
PjCb        *gCb
));

EXTERN S16 pjDbmFetchUlUeCb ARGS ((
PjCb        *gCb,
CmLteRnti         ueId,       /* UE Identifier */
CmLteCellId       cellId,     /* Cell Identifier */
PjUlUeCb            **ueCb       /* UE Control Block */
));

EXTERN S16 pjDbmCreateUlUeCb ARGS ((
PjCb        *gCb,
CmLteRnti         ueId,       /* UE Identifier */
CmLteCellId       cellId,     /* Cell Identifier */
PjUlUeCb            **ueCb       /* UE Control Block */
));
EXTERN  PjUlRbCb* pjDbmCreateUlRbCb ARGS ((
PjCb        *gCb,
U8      rbId,
U8      rbType,
PjUlUeCb  *ueCb,
U8      rb
));

EXTERN S16 pjDbmFetchUlRbCb ARGS ((
PjCb           *gCb,
CmLteRlcId     rlcId,      /* RLC Identifier */
PjUlRbCb       **rbCb       /* RB Cb */
));

EXTERN  S16 pjDbmDelUlRbCb ARGS ((
PjCb        *gCb,
U8      rbId,
U8      rbType,
PjUlRbCb   **rbCb,
U8      rb,
PjUlUeCb      *ueCb
));

EXTERN S16 pjDbmDelUlUeCb  ARGS((
PjCb        *gCb,
PjUlUeCb      *ueCb,       /* UE Identifier */
Bool        abortFlag    /* Abort Flag */
));

EXTERN S16 PjDbmDelAllUlUe ARGS((PjCb *gCb));
EXTERN S16 pjDbmDelAllUlRb ARGS((
PjCb        *gCb,
PjUlRbCb         **rbCbLst,              /* RB Cb list */
U8             numRbCb                 /* Number of rbCbs */
));
EXTERN S16 pjDbmDelAllUlUe ARGS ((  
PjCb  *gCb
));

EXTERN S16 pjDbmUlInit  ARGS (( PjCb *gCb));
EXTERN S16 pjDbmUlShutdown ARGS((PjCb *gCb));
EXTERN S16 pjDbmAddUlTransIdLst ARGS((PjCb *gCb,PjCfgInfo  *cfg));
EXTERN S16 pjDbmFindUlTransIdLst ARGS((PjCb *gCb,U32 transId,
                                      PjCfgInfo   **cfg));
EXTERN S16 pjDbmDelUlTransIdLst ARGS((PjCb *gCb,PjCfgInfo  **cfg));
EXTERN S16 pjDbmDelAllUlTransIdLst ARGS((PjCb  *gCb));

EXTERN S16 pjUtlUlUpdUpSecKeys ARGS(( PjCb *gCb, PjUlUeCb *ueCb));
EXTERN S16 pjUtlUlUpdCpSecKeys ARGS(( PjCb *gCb, PjUlUeCb *ueCb));
EXTERN S16 pjUtlUlHdlRbReEstComplete ARGS((PjCb *gCb, PjUlRbCb *pjRbCb));


EXTERN S16 pjUtlUlCmpReset ARGS(( 
PjCb        *gCb,
PjUlRbCb      *pjRbCb                 /* Context to be reset for compression */
));

/* UTL Module */
EXTERN S16 pjUtlUlCmpInit ARGS((
PjCb        *gCb,
PjUlRbCb   *pjRbCb                  /* PDCP RbCb */
));
EXTERN S16 pjUtlUlIntInit ARGS((
PjCb        *gCb,
PjUlUeCb   *ueCb                   /* UE CB Ptr */
));
EXTERN S16 pjUtlUlCpInit ARGS((
PjCb        *gCb,
PjUlUeCb   *ueCb                    /* UE CB Ptr */
));
EXTERN S16 pjUtlUlUpInit ARGS((
PjCb        *gCb,
PjUlUeCb   *ueCb                    /* UE CB Ptr */
));

EXTERN S16 pjUtlUlCmpReq ARGS((
PjCb        *gCb,
PjUlRbCb   *pjRbCb,                 /* PDCP RbCb */
Buffer   *mBuf,                   /* SDU to be compressed */
Buffer   **opSdu,                 /* Compressed SDU */
U32      count                    /* COUNT - transaction Id */
));
EXTERN S16 pjUtlUlIntProtReq ARGS((
PjCb        *gCb,
PjUlRbCb   *pjRbCb,                 /* PDCP RbCb */
PjSecInp secInp ,                 /* Input parameters for integrity */ 
Buffer   *mBuf,                   /* SDU to be compressed */
U32      *macI                    /* Message authentication code for the SDU */
));
EXTERN S16 pjUtlDeCmpReq ARGS((
PjCb        *gCb,
PjUlRbCb   *pjRbCb,                 /* PDCP RbCb */
U32      count,                   /* COUNT - transaction Id */
Buffer   *mBuf,                   /* SDU to be compressed */
Buffer   **opSdu                  /* Compressed SDU */
));
EXTERN S16 pjUtlIntVerReq ARGS((
PjCb        *gCb,
PjUlRbCb   *pjRbCb,                 /* PDCP RbCb */
PjSecInp secInp ,                 /* Input parameters for integrity */ 
Buffer   *mBuf,                   /* SDU to be compressed */
U32      macI,                    /* MAC-I to be verified with */
Status   *status                  /* Integrity verification status */
));
EXTERN S16 pjUtlDecipherReq ARGS((
PjCb        *gCb,
PjUlRbCb   *pjRbCb,                 /* PDCP RbCb */
U32        count,                  /* count of packet */ 
Buffer     *mBuf,                   /* Data to be deciphered */
Buffer   **opSdu                  /* Deciphered SDU */
));
EXTERN S16 pjUtlSndFeedback ARGS((
PjCb        *gCb,
PjUlRbCb   *pjRbCb,                 /* PDCP RbCb */
Buffer   *feedback                /* ROHC Feedback */
));
EXTERN S16 pjUtlUlReEstSrb1 ARGS((
PjCb        *gCb,
PjUlRbCb      *pjRbCb                    /* PDCP Control Block Pointer */
));
EXTERN S16 pjUtlUlReEstStart ARGS((
PjCb        *gCb,
PjUlUeCb         *ueCb,          /* UE Control Block Pointer */
CpjCfgReqInfo  *cfgReq         /* CfgReq for Reestablishment */
));
EXTERN S16 pjUtlUlReEstHOStart ARGS((
PjCb        *gCb,
PjUlUeCb      *ueCb,           /* Number of RBs undergoing reestablishment*/ 
U32         transId,        /* Transaction id of Sdu Status cfm */
U32         *waitForHoTrig 
));
EXTERN S16 pjUtlUlReEstReconfig ARGS((
PjCb        *gCb,
PjUlUeCb       *ueCb           /* UE Control Block */ 
));
EXTERN S16 pjUtlUlReEstUl ARGS((
PjCb        *gCb,
PjUlUeCb       *ueCb           /* UE Control Block */ 
));
EXTERN S16 pjUtlUlSndReEstCfgCfm ARGS((
PjCb        *gCb,
PjUlUeCb       *ueCb           /* UE Control Block */ 
));
EXTERN S16 pjUtlUlSndSduStaCfm ARGS((
PjCb        *gCb,
PjUlUeCb       *ueCb         /* UE Control Block */ 
));
EXTERN S16 pjUtlUlShutdown ARGS((
PjCb        *gCb
));
EXTERN Void pjUtlUlFreeRb ARGS((
PjCb        *gCb,
PjUlRbCb *pjRbCb
));
/* kw005.201 added function to send data forward indication mesage */
EXTERN S16 pjUtlUlSndDatFwdInd ARGS((
PjCb        *gCb,
PjUlRbCb           *pjRbCb,
PjuDatFwdIndInfo *datFwdInd
));
EXTERN S16 pjUtlUlSndUlStaRep ARGS ((  
PjCb             *gCb,
PjUlRbCb         *pjRbCb,
Buffer           *sta
));

EXTERN S16 pjUtlUlHdlObdInitCfm ARGS((
PjCb        *gCb,
PjUlUeCb    *ueCb,           /* Pointer to UeCb */ 
U16         txIdx,           /* Transaction Index for UeCb */
U8          cfmType,         /* Confirm type */
U8          maskVal          /* mask value */
));
EXTERN S16 pjUtlUlHdlInitObdTmrExp ARGS((
PjCb      *gCb,
PjUlUeCb *ueCb
));

EXTERN S16 pjCfgPrcUlSduSta ARGS((
PjCb        *gCb,
CpjSduStaReqInfo *staReq,
UdxSduStaCfmInfo *staCfm
));

EXTERN S16 pjUtlUlSaveCfmInfo  ARGS((
PjCb        *gCb,
PjUlUeCb      *ueCb,           /* Pointer to UeCb */ 
U8          cfmType,         /* Confirm type */
Bool        startTmr,        /* Whether to start timer */
U8          entity,          /* Confirm type */
PTR         cfmPtr,          /* Pointer to the structure for confirm */
PTR         cfgPtr           /* Pointer to the structure for request */
));

/* UL Module */
EXTERN S16 pjUlmHdlSrbPkt ARGS((
PjCb        *gCb,
PjUlRbCb *pjRbCb,               /* !< PDCP Control Block */  
Buffer *pdu                   /* !< PDU Buffer */ 
));
EXTERN S16 pjUlmHndlDlStaRep ARGS ((    
PjCb     *gCb,
PjUlRbCb   *pjRbCb,
PjSn     fmc,
Buffer   *staPdu
));

#ifndef XEON_SPECIFIC_CHANGES
EXTERN U32 pjMsCheckSpaccQueue ARGS((
Bool isUl
));
#else
EXTERN U32 pjMsCheckSpaccQueue ARGS((
         Void
         ));
#endif

EXTERN S16 pjUlmHdlDrbPkt ARGS((
PjCb        *gCb,
PjUlRbCb *pjRbCb,               /* !< PDCP Control Block */      
Buffer *pdu,                   /* !< PDU Buffer */ 
Bool    isOutOfSeq               /* !< To indicate whether this packet is in-sequence or not */
));
EXTERN S16 pjUlmProcessSrb ARGS((
PjCb        *gCb,
PjUlRbCb   *pjRbCb,           /* !< PDCP Control Block */   
PjSn     sn,               /* !< SN value of PDU */           
Buffer   *pdu              /* !< PDU message buffer */           
));
EXTERN S16 pjUlmProcessDrbUm ARGS((
PjCb        *gCb,
PjUlRbCb   *pjRbCb,          /* !< PDCP Control Block */   
PjSn     sn,               /* !< SN value of PDU */           
Buffer   *pdu              /* !< PDU message buffer */           
));
EXTERN S16 pjUlmProcessDrbAm ARGS((
PjCb        *gCb,
PjUlRbCb   *pjRbCb,           /* !< PDCP Control Block */   
PjSn     sn,               /* !< SN value of PDU */           
Buffer   *pdu,              /* !< PDU message buffer */           
Bool     isOutOfSeq           /* !< To indicate whether packet received is in-sequence or not */
));
EXTERN S16 pjUlmHdlDecipher ARGS((
PjCb        *gCb,
PjUlRbCb     *pjRbCb,        /* !< PDCP Control Block */  
PjRxEnt    *rxEnt          /* !< Rx Entry */
));
EXTERN S16 pjUlmHdlIntVer ARGS((
PjCb        *gCb,
PjUlRbCb     *pjRbCb,        /* !< PDCP Control Block */  
PjRxEnt    *rxEnt          /* !< Rx Entry */
));
EXTERN S16 pjUlmHdlDeCmp ARGS((
PjCb        *gCb,
PjUlRbCb     *pjRbCb,        /* !< PDCP Control Block */  
PjRxEnt    *rxEnt          /* !< Rx Entry */
));
EXTERN S16 pjUlmDeliverSrb ARGS((
PjCb        *gCb,
PjUlRbCb *pjRbCb,          /* !< PDCP Control Block */     
PjRxEnt *rxEnt           /* !< Recption Buffer Entry   */            
));
EXTERN S16 pjUlmDeliverDrbUm ARGS((
PjCb        *gCb,
PjUlRbCb *pjRbCb,        /* !< PDCP Control Block */                  
PjRxEnt *rxEnt         /* !< Recption Buffer Entry   */             
));
EXTERN S16 pjUlmDeliverDrbAm ARGS((
PjCb        *gCb,
PjUlRbCb *pjRbCb,           /* !< PDCP Control Block */                  
PjRxEnt *rxEnt            /* !< Recption Buffer Entry   */             
));
EXTERN S16 pjUlmReEstSrb ARGS((
PjCb        *gCb,
PjUlRbCb *pjRbCb       /*!< PDCP control block  */
));
EXTERN S16 pjUlmReEstDrbAm ARGS((
PjCb        *gCb,
PjUlRbCb *pjRbCb       /*!< PDCP control block  */
));
EXTERN S16 pjUlmBldStaRep ARGS((
PjCb        *gCb,
PjUlRbCb    *pjRbCb,             /* !< PDCP Control Block */  
PjSn         fmc,                /* !< First Missing count */  
U8          *staRep,             /* !< Status Report */       
U16         bMapLen              /* !< Bitmap Length */
));
EXTERN S16 pjUlmHdlDatFwdReq ARGS((
PjCb        *gCb,
PjUlRbCb         *pjRbCb,        /* !< PDCP ID */        
PjuDatFwdReqInfo   *datFwdReq    /* !< DatFwdReq Info */
));                                             
EXTERN S16 pjUlmSubmitForDecmp ARGS((
PjCb       *gCb,
PjUlRbCb   *pjRbCb,        /* !< PDCP Control Block */  
PjRxEnt    *rxEnt          /* Ptr To Rx Entry */
));

EXTERN Void pjUlmHdlObdTmrExpiry ARGS(( 
PjCb       *gCb,
PjUlRbCb     *pjRbCb         /* !< PDCP Control Block */
));

EXTERN Void pjUlmHdlErrUpdates ARGS((
PjCb       *gCb,
PjUlRbCb     *pjRbCb,        /* !< PDCP Control Block */  
U32         errCnt         /* !< Count of PDU that raises the error */
));

EXTERN Void pjLimDatInd ARGS((
PjCb        *gCb,
PjUlRbCb      *pjRbCb,
Buffer      *mBuf,
Bool        isOutOfSeq
));

EXTERN Void pjLimReEstCmpInd ARGS((
PjUlRbCb      *pjRbCb         
));

#ifdef LTE_L2_MEAS
S16 pjUtlL2MeasUlInit(PjCb *gCb);

EXTERN Void pjUtlResetUlL2MeasCntr ARGS((
PjCb         *gCb,
PjL2MeasCb   *measCb,
U8           measType
));

EXTERN S16 pjUtlSndUlL2MeasNCfm ARGS ((PjCb *gCb, 
                                PjL2MeasCfmEvt *measCfmEvt));

EXTERN Void pjUtlPlcMeasDatInUlL2Sts ARGS((PjL2Cntr *, PjL2MeasRbCb *, U8));
#endif

EXTERN  S16 pjDbmFetchPjUlRbCb ARGS ((
PjUlUeCb   *ueCb,
U8         rbId,
U8         rbType,
PjUlRbCb   **pjRbCb         
));

EXTERN S16 pjUtlUlSndUlRohcFdbk ARGS ((
PjCb        *gCb,
PjUlRbCb           *pjRbCb,
Buffer      *fbPkt
));

EXTERN S16 pjUtlUlResetBuffrdPkts ARGS ((
PjCb        *gCb,
PjUlRbCb    *pjRbCb
));

EXTERN  S16 pjUtlReEstProcUlRb ARGS ((
PjCb            *tPjCb,
PjUlUeCb        *ueCb,
CpjCfgReqInfo   *cfg
));

EXTERN S16 pjUtlCheckOnlySrbPresent ARGS((
PjCb        *tPjCb,            /* Pdcp UL control block */
CmLtePdcpId pdcpId,            /* PDCP Id*/
U32         *waitForReEstTrig /* wait for reestablishment trigger */
));

EXTERN Void pjUlmReEstCmpInd ARGS((
PjCb   *tPjCb,
PjUlRbCb *tRbCb        /* !< RB control block */  
));

EXTERN S16 PjUlUdxDlStaRep ARGS((  
Pst *pst,                       /* Post structure */
SpId spId,                      /* Service provider ID */
UdxDlStaRepInfo *staRep           /*PDCP Status Report*/
));

EXTERN S16 PjUlUdxUlStaRep ARGS ((  
Pst *pst,                       /* Post structure */
SpId spId,                      /* Service provider ID */
UdxUlStaRepInfo *staRep           /*PDCP Status Report*/
));

EXTERN S16 pjUtlStartUlReEst ARGS ((
PjCb            *gCb,
CpjReEstReqInfo *reEstReq,
CpjReEstCfmInfo *reEstCfm
));

EXTERN S16 pjUtlUlPrcsDatResume ARGS ((
PjCb       *gCb,
CpjDatResumeReqInfo  *datResReq
));

EXTERN S16 pjUlmStartDataFrwdPerRb ARGS((
PjCb       *gCb,
PjUlRbCb    *pjRbCb
));

EXTERN Void pjUlmProcessUlPktQ ARGS ((
PjCb       *gCb,
PjUlRbCb   *pjRbCb           /* !< PDCP Control Block */
));

EXTERN S16 pjUtlUlGetRbCb ARGS ((
PjCb          *gCb,
CmLtePdcpId   *pdcpId,
PjUlRbCb      **rbCb,
U8            *retCause
));

EXTERN Void pjUtlEmptyUlPktList ARGS((
PjCb *gCb, 
PjUlRbCb *pjRbCb
));

EXTERN S16 pjUtlUlReEstHO ARGS((
PjCb          *gCb,
PjUlUeCb      *ueCb         
));

EXTERN S16 pjDbmFetchUlUeCb ARGS((
PjCb              *gCb,
CmLteRnti         ueId,    
CmLteCellId       cellId, 
PjUlUeCb          **ueCb  
));
EXTERN S16 pjUlmHndlDatFwdReq ARGS((
PjCb               *gCb,
PjUlRbCb           *pjRbCb,
PjuDatFwdReqInfo   *datFwdReq
));
EXTERN Void dumpPDCPUlRbInformation ARGS ((
PjUlRbCb* ulRbCb,
U16 ueId
));

EXTERN void DumpPDCPUlDebugInformation ARGS ((
Void
));

EXTERN Void pjDeInitDeCipherQ(Void);

EXTERN  Void pjUlmHdlReordTmrExpiry ARGS((
PjCb          *tPjCb,/*pjCb structure */
PjUlRbCb     *pjRbCb
));


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __PJX__ */

  
/********************************************************************30**
  
         End of file
**********************************************************************/
