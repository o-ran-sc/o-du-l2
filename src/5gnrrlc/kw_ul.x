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
  
        Name:    LTE RLC file for uplink and non real time tasks
    
        Type:    C include file
  
        Desc:    This file contains all the data structures and 
                 prototypes for LTE RLC in the uplink.
 
        File:    kw_ul.x
  
*********************************************************************21*/
/** 
 * @file kw_ul.x
 * @brief RLC uplink structures, prototypes
*/

#ifndef __KW_ULX__
#define __KW_ULX__
 
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct rlcUlUeCb RlcUlUeCb;

/** @defgroup um_uplink UM Uplink Module
*/

/** 
 * @brief  Structure to hold a  UM PDU
 *
 * @details
 *    - pdu : Buffer holding the UM PDU data
 *    - umHdr : UM PDU Header Information
 *    - pduSz : Length of the PDU excluding the header
*/
typedef struct rlcUmRecBuf
{
   Buffer    *pdu;    /**< Buffer holding the UM PDU */
   RlcUmHdr   umHdr;   /**< UM PDU Header Information */
   MsgLen    pduSz;   /**< PDU Size */
}RlcUmRecBuf;

/** 
 * @brief  Structure to hold uplink information in UM mode for a particular RB
 *
 * @details
 *    - snLen : The sequence number length can be 5 bits or 10 bits. 
 *              Here it is stored as 1 or 2 (as the number of bytes)
 *    - recBuf : Holds all the received PDUs. PDU's are removed from this 
 *               after a SDU is formed or during restablishment
 *    - umWinSz : The window size is 512 for 10 bits sequence number and 16 
 *                for 5 bits sequence number
 *    - partialSdu : This is used to store the partially completed SDU. 
 *                   It remains till complete SDU is received
*/
typedef struct rlcUmUl
{
   U8           snLen;         /**< Sequence number length */
   U8           reOrdTmrInt;   /**< Timer Interval */
   RlcUmRecBuf   **recBuf;      /**< Reception buffer */
   RlcSn         umWinSz;       /**< UM window size */
   U16          modBitMask;    /**< Bitmask for modulus to wrap around variables */
   RlcSn         sn;            /**< Sequence number */
   RlcSn         vrUr;          /**< VR(UR) - Receive state variable */
   RlcSn         vrUh;          /**< VR(UH) - Highest received state variable */
   RlcSn         vrUx;          /**< VR(UX) - Reordering state variable */
   CmTimer      reOrdTmr;      /**< Reordering Timer */
   Buffer       *partialSdu;   /**< Partial SDU - Remains till the complete SDU
                                                               is received */
}RlcUmUl;
/*@}*/

/** 
 * @brief  Structure to hold a RLC AM PDU segment
 *
 * @details
 *    - lstEnt : This is required for the linked list in which the segments
 *               are stored
 *    - seg    : Holds the segment data
 *    - segSz  : The length of the segment in bytes
 *    - soEnd  : SOEnd
 *    - amHdr  : The AM Header for the PDU segment
 *
*/
typedef struct rlcSeg
{
   CmLList   lstEnt;   /**< List entry for PDU segment */
   Buffer    *seg;     /**< PDU segment */
   MsgLen    segSz;    /**< Buffer Size */
   U16       soEnd;    /**< Segment Offset End */
   RlcAmHdr   amHdr;    /**< AM header */
}RlcSeg;

/*@}*/

/** 
 * @brief  Structure to hold a received AM PDU or segments of a PDU
 *
 * @details
 *    - pdu         : Holds the PDU data
 *    - pduSz       : Length of the PDU in bytes
 *    - amHdr       : The AM Header for the PDU  
 *    - segLst      : The length of the segment in bytes
 *    - expByteSeg  : The next expected segment for re-ordering
 *    - expSo       : The next expected SO so to be in sequence
 *    - allRcvd     : Whether all the segments for this PDU has been recevied
 *
*/
typedef struct rlcAmRecBuf
{
   CmLList     lnk;           /**< Link to the receive buffer list */
   Buffer      *pdu;          /**< PDU buffer */
   MsgLen      pduSz;         /**< Buffer Size */
   RlcAmHdr     amHdr;         /**< AM header Info */
   CmLListCp   segLst;        /**< PDU Segments list */
   RlcSeg       *expByteSeg;   /**< Next expected byte segment */
   U16         expSo;         /**< Next expected seg offset */
   Bool        allRcvd;       /**< All bytes received or not */
   Bool        isDelvUpperLayer; /**< Is it sent to upper layer */ 
}RlcAmRecBuf;

/** @addtogroup ammode */
/*@{*/

/** 
 * @brief  Structure to hold information about an uplink AM Radio Bearer
 *
 * @details
 *    - recBuf            : Reception buffer
 *    - rxNext            : RX_Next - Receive state variable 
 *    - rxNextHighestRcvd : RX_Next_Highest_Rcvd - Highest received state variable
 *    - rxNextStatusTrig  : RX_Next_Status_Trigger - reorderig state variable
 *    - vrMr              : VR(MR) - Maximum acceptable receive state variable
 *    - rxHighestStatus   : RX_Highest_Status - Maximum STATUS transmit state variable
 *    - staTrg           : Flag to indicate if status trigger occured
 *    - partialSdu       : Partial SDU - Remains till the complete SDU 
 *                         is received
 *    - expSn            : The expected sequence number for reassembly  
 *    - expSo            : The expected SO for reassembly
 *    - staProhTmr       : The Status Probihit Timer
 *    - staProhTmrInt    : Status Prohibit Timer interval (in ??)
 *    - reOrdTmr         : The Reordering Timer
 *    - reOrdTmrInt      : Re-ordering timer interval
 *    - gatherStaPduInfo : Whether to gather information required to create 
 *                         the STATUS PDU
 *
*/
typedef struct rlcAmUl
{
#ifndef LTE_TDD 
   CmLListCp   *recBufLst;
#else
//   RlcAmRecBuf   *recBuf[1024];              /**< Reception buffer */
#endif
   RlcSn         rxNext;                /**< RX_Next:Equvalent to VR(R) in 4G */
   RlcSn         rxNextHighestRcvd;     /**< RX_Next_Highest_Rcvd: Equvalent to VR(H) in 4G */ 
   RlcSn         rxNextStatusTrig;      /**< rxNextStatusTrig: Equvalent to VR(X) in 4G*/
   RlcSn         vrMr;                  /**< VR(MR) */ 
   RlcSn         rxHighestStatus;       /**< rxHighestStatus: Eqvalent to VR(MS) in 4G*/
   Bool         staTrg;                /**< Whether status trigger occured */
   Buffer       *partialSdu;           /**< Partially received SDU */
   RlcSn         expSn;                 /**< Expected SN for reassembly */
   U16          expSo;                 /**< Expected SO for reassembly */
   CmTimer      staProhTmr;            /**< T_status_prohibit Timer */
   U16          staProhTmrInt;         /**< Timer Interval */
   CmTimer      reOrdTmr;              /**< T_reordering Timer */
   U8           reOrdTmrInt;           /**< Timer Interval */
   Bool         gatherStaPduInfo;      /**< Gather STATUS PDU creation info*/
   Bool         isOutOfSeq;            /**< To identify whether packets are Out-Of-Seq or not */
   U8           snLen;                 /*!< Sequence number length:12 bit or 18 bit : 5GNR RLC */
   U32          snModMask;             /*!< (2 Pwr SnLen - 1): 5GNR RLC */
}RlcAmUl;

/*@}*/

/** 
 * @brief  Structure to hold uplink information about a Radio Bearer
 *
 * @details
 *    - rlcId    : RLC identifier, uniquely identifies a Radio Bearer
 *    - lch      : Information (type and id) of the logical channel associated 
 *                 with this Radio  Bearer.
 *    - mode     : The mode of the Radio Bearer; UM or AM
 *    - dir      : The direction of the Radio Bearer, downlink or uplink or both
 *    - inst     : Id of RLC instance where this Radio Bearer is present. Used
 *                 to find the instance from the Radio Bearer for memory needs 
 *                 as different instances might have different memory.
 *    - k1wuSapId : KWU SAP identifier
 *    - udxSapId : UDX SAP idenrifier
 *    - transId  : Stores the transaction identifier used to communicate 
 *                 with MAC, the same value as sent by MAC is passed back 
 *                 for it to be able to corelate
 *    - m        : Mode of the RB (TM/UM/AM)
 *      - umDl   : Unacknowledged Mode downlink information
 *      - amDl   : Acknowledged Mode downlink information
*/
typedef struct _rlcUlRbCb
{
/* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
   RlcL2MeasRbCb    rbL2Cb;              /**< RB measurement L2 Cb */
   RlcUlUeCb        *ueCb;              /*!< Pointer to UeCb  */
   U8              qci;                 /**< qci of the RB */
   RlcL2MeasIpThruput l2MeasIpThruput;   /**< Holds related parameter for
                                             DL/Ul ip throughput>*/
#endif /* LTE_L2_MEAS */
   CmLteRlcId     rlcId;      /**< RLC Identifier */
   RlcLchInfo      lch;        /**< Logical Channel Info */
   CmLteRlcMode   mode;       /**< Entity Mode */
   U8             dir;        /**< Direction for UL/DL */
   Inst           inst;       /**< Tapa where Rb created Instance id */
   SpId           k1wuSapId;   /**< KWU sap Id, to get the KwuSapCb */
   SpId           udxSapId;   /**< KWU sap Id, to get the KwuSapCb */
   U32            transId;    /**< Transaction Id for RLC */
   union          
   {
      RlcUmUl   umUl;   /**< UM  mode Ul elements */
      RlcAmUl   amUl;   /**< AM mode uplink elements */
   }m;   /**< RLC mode specific Info */
}RlcUlRbCb;

/** 
 * @brief  Structure to hold mapping between logical channel and Radio Bearer
 *
 * @details
 *    - ulRbCb   : Pointer to the uplink Radio Bearer
*/
typedef struct rlcUlLch
{
   RlcUlRbCb *ulRbCb;   /**< Pointer to Uplink RbCb */
}RlcUlLch;                                  

/** 
 * @brief  Structure to hold uplink information about the Cells
 *
 * @details
 *    - cellHlEnt : Information about cells are stored in a hash table. This is
 *                  required for that.
 *    - cellId    : Identity of the cell
 *    - rbCb      : Radio Bearers in the cell
 *    - lCh       : Logical Channels in the cell
 *    - selfPstUl : Pst structure for sending messages to self
*/
typedef struct rlcUlCellCb
{
   CmHashListEnt   cellHlEnt;                 /**< Hash list entry for CellCb */
   CmLteCellId     cellId;                    /**< Cell Id */
   RlcUlRbCb        *rbCb[RLC_MAX_RB_PER_CELL]; /**< RbCbs within a Cell */
   RlcUlLch         lCh[RLC_MAX_LCH_PER_CELL];  /**< Logical channels in a cell */
   Pst             selfPstUl;
}RlcUlCellCb;

/** 
 * @brief  Structure to hold uplink information about the UEs
 *
 * @details
 *    - ueHlEnt : Information about cells are stored in a hash table. This is
 *                required for that.
 *    - key     : Key to store/find the UE in the hashtable
 *    - srbCb   : Signaling Radio Bearers configured for the UE
 *    - drbCb   : Data Radio Bearers configured for the UE
 *    - lCh     : Logical Channels in the UE
*/
struct rlcUlUeCb
{
   CmHashListEnt   ueHlEnt;                   /**< Hash list entry for UeCb */
   CmLteRnti     ueId;     /*!< UE Id */
   CmLteCellId   cellId;   /*!< Cell Id */
   RlcUlRbCb        *srbCb[RLC_MAX_SRB_PER_UE]; /**< SRB RbCbs within an UE */ 
   RlcUlRbCb        *drbCb[RLC_MAX_DRB_PER_UE]; /**< DRB RbCbs within an UE */ 
   RlcUlLch         lCh[RLC_MAX_LCH_PER_UE];    /**< Logical channels of an UE*/
/* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
   U32             firstPacketTTI;            /*!< is first packet of the burst */
   U16             numActRb[LKW_MAX_QCI];     /**< number of RBs Active */
   Bool              isUlBurstActive;   /*!<Has the Burst started for UL IP Thrpt meas */
#endif /* LTE_L2_MEAS */
};

/** 
 * @brief  Structure to hold temporary data of configuration 
 *
 * @details
 *    - entUlCfgCfm      :  Holds the UL configuration status  
 *    - rbCb             :  Rb Block
 */
typedef struct rlcUlEntTmpData
{
   RlcEntCfgCfmInfo   entUlCfgCfm; /**< Ul Configuration status*/ 
   RlcUlRbCb           *rbCb;       /**< Rb Block */
}RlcUlEntTmpData;

/** 
 * @brief  Structure to hold transaction information in configuration request 
 *
 * @details
 *    - transHlEnt   : List entry for transaction block. 
 *    - transId      : Transaction Id
 *    - ueId         : ue Id
 *    - cellId       : cell Id
 *    - ueCb         : Ue Block
 *    - cellCb       : Cell Block
 *    - cfgInfo      : Configuration Information 
 *    - ueInfo       : UE Information
 *    - newUeInfo    : New Ue Information
 *    - cfgTmpData   : Temporary data per Configuration entity
*/
typedef struct rlcUlCfgTmpData
{
   CmHashListEnt    transHlEnt;                  /**< List Entry of 
                                                   Transaction*/
   U32              transId;                     /**< Locally generated Transaction Id */
   U32              uprLyrTransId;               /**< Transaction Id generated by upper layer. 
                                                      This is used while sending confirm to the User Layer */
   CmLteCellId      cellId;                      /**< Cell Id */
   CmLteRnti        ueId;                        /**< Ue Id */
   RlcUlUeCb         *ueCb;                       /**< Ue Block */
   RlcUlCellCb       *cellCb;                     /**< Cell Block */
   RlcCfgInfo       *cfgInfo;                    /**< Config Information*/
   CkwUeInfo        *ueInfo;                     /**< Ue Information */
   CkwUeInfo        *newUeInfo;                  /**< New Ue Information */
   RlcUlEntTmpData   cfgEntData[CKW_MAX_ENT_CFG]; /**< Entity Data */
} RlcUlCfgTmpData;
/****************************************************************************
 *                      EXTERN Declarations
 ***************************************************************************/
/****************************************************************************
 *                    Configuration Functions 
 ***************************************************************************/

EXTERN S16 rlcValidateRbCfgParams ARGS ((RlcCb *gCb,
                                        CmLteRnti   ueId,
                                        CmLteCellId cellId,
                                        RlcEntCfgInfo  *cfgToValidate,
                                        CmStatus        *status));
EXTERN S16 rlcCfgValidateUlRb ARGS (( RlcCb *gCb,
                                    RlcEntCfgInfo *cfgToValidate,
                                    RlcUlEntTmpData *cfgInfo, 
                                    RlcUlCfgTmpData *cfg));

EXTERN S16 rlcCfgRollBackUlRb ARGS ((RlcCb *gCb,
                                    CmLteRnti ueId,
                                    RlcEntCfgInfo *cfgToValidate,
                                    RlcUlEntTmpData *cfgTempData));

EXTERN Void rlcCfgApplyUlRb ARGS ((RlcCb *gCb,
                                  RlcEntCfgInfo *cfgToAply,
                                  RlcUlEntTmpData *cfgTmpData,
                                  RlcUlCfgTmpData *cfgTmpInfo));

EXTERN S16 rlcCfgValidateReEstRb ARGS ((RlcCb *gCb,
                                       CmLteRnti  ueId,
                                       CmLteCellId cellId,
                                       RlcEntCfgInfo *cfgToValidate,
                                       RlcUlEntTmpData   *cfgTmpData));

EXTERN Void rlcCfgApplyReEstUlRb ARGS ((RlcCb *gCb,
                                      CmLteRnti ueId,
                                      CmLteCellId cellId,
                                      Bool sndReEstInd,
                                      RlcUlEntTmpData *cfgTmpData));

EXTERN Void rlcCfgApplyDelUlCell ARGS ((RlcCb *gCb,
                                      RlcUlCfgTmpData *cfgTmpData));

EXTERN S16 rlcCfgValidateDelUlCell ARGS ((RlcCb *gCb,
                                         CmLteCellId cellId,
                                         RlcEntCfgInfo *cfgToValidate,
                                         RlcUlEntTmpData   *cfgTmpData,
                                         RlcUlCfgTmpData *cfgInfo));

EXTERN S16 rlcCfgValidateDelUlUe ARGS ((RlcCb *gCb,
                                       RlcEntCfgInfo *cfgToValidate,
                                       RlcUlEntTmpData *cfgTmpData,
                                       RlcUlCfgTmpData *cfgInfo));

EXTERN Void rlcCfgApplyDelUlUe ARGS ((RlcCb *gCb,
                                    RlcUlCfgTmpData *cfgTmpData));

EXTERN Void rlcUlHdlCfgReq ARGS ((RlcCb *gCb,
                                     RlcUlCfgTmpData *cfgInfo,
                                     RlcCfgInfo *cfg));

EXTERN Void rlcCfgApplyUlUeIdChng ARGS ((RlcCb *gCb,
                                        CkwUeInfo *ueInfo,
                                        CkwUeInfo *newUeInfo,
                                        RlcUlCfgTmpData *cfgTmpInfo));

EXTERN S16 rlcCfgValidateUeIdChng ARGS ((RlcCb *gCb,
                                        CkwUeInfo *ueInfo,
                                        CkwUeInfo *newUeInfo,
                                        RlcUlCfgTmpData *cfgTmpInfo));

/****************************************************************************
 *                    DBM module Functions 
 ***************************************************************************/
EXTERN S16 rlcDbmUlInit ARGS ((RlcCb *gCb));

EXTERN Void rlcDbmUlDeInit ARGS ((RlcCb *gCb));

EXTERN S16 rlcDbmAddUlUeCb ARGS ((RlcCb *gCb,
                                    CmLteRnti ueId,
                                    CmLteCellId cellId,
                                    RlcUlUeCb *ueCb));

EXTERN S16 rlcDbmFetchUlUeCb ARGS ((RlcCb *gCb,
                                   CmLteRnti ueId,
                                   CmLteCellId cellId,
                                   RlcUlUeCb **ueCb));

EXTERN Void rlcDbmDelUlUeCb ARGS ((RlcCb *gCb,   
                                  RlcUlUeCb *ueCb,
                                  Bool abortFlag));

EXTERN Void rlcDbmDelAllUlUe ARGS ((RlcCb *gCb));

EXTERN S16 rlcDbmAddUlCellCb ARGS ((RlcCb *gCb,
                                      CmLteCellId cellId,
                                      RlcUlCellCb *cellCb));

EXTERN Void rlcDbmFetchUlCellCb ARGS ((RlcCb *gCb, 
                                      CmLteCellId cellId,
                                      RlcUlCellCb **cellCb));

EXTERN Void rlcDbmDelUlCellCb ARGS ((RlcCb *gCb,  
                                   RlcUlCellCb *cellCb));

EXTERN Void rlcDbmDelAllUlCell ARGS ((RlcCb *gCb));

EXTERN Void rlcDbmFetchUlRbCbByRbId ARGS ((RlcCb *gCb, 
                                          CmLteRlcId *rlcId, 
                                          RlcUlRbCb **rbCb));

EXTERN Void rlcDbmFetchUlRbCbFromLchId ARGS ((RlcCb *gCb, 
                                             CmLteRnti ueId, 
                                             CmLteCellId cellId, 
                                             CmLteLcId lcId,  
                                             RlcUlRbCb **rbCb));

EXTERN Void rlcDbmDelAllUlRb ARGS ((RlcCb *gCb, 
                                   RlcUlRbCb **rbCbLst, 
                                   U8 numRbCb));

EXTERN S16 rlcDbmAddUlTransaction ARGS((RlcCb *gCb, RlcUlCfgTmpData *cfg));

EXTERN S16 rlcDbmFindUlTransaction ARGS((RlcCb *gCb, 
                                        U32 transId, 
                                        RlcUlCfgTmpData **cfg));

EXTERN S16 rlcDbmDelUlTransaction ARGS((RlcCb *gCb, RlcUlCfgTmpData *cfg));

EXTERN S16 rlcDbmDelAllUlTransactions ARGS((RlcCb *gCb));

EXTERN Void rlcDbmUlShutdown ARGS ((RlcCb *gCb));

/****************************************************************************
 *                    Transparent Mode Functions 
 ***************************************************************************/
#ifdef CCPU_OPT
EXTERN Void rlcTmmRcvFrmLi ARGS ((RlcCb *gCb, 
                                       RlcUlRbCb *rbCb,
                                       CmLteRnti tCrnti, 
                                       Buffer *pdu));
#else /* CCPU_OPT */
EXTERN Void rlcTmmRcvFrmLi ARGS ((RlcCb *gCb,
                                       RlcUlRbCb *rbCb,
                                       Buffer *pdu));
#endif /* CCPU_OPT */

EXTERN Void rlcTmmUlReEstablish ARGS ((RlcCb *gCb, RlcUlRbCb *rbCb));

/****************************************************************************
 *                    Unacknowledged Mode Functions 
 ***************************************************************************/
#ifdef LTE_L2_MEAS
EXTERN Void rlcUmmProcessPdus ARGS((RlcCb *gCb,
                                  RlcUlRbCb *rbCb, 
                                  KwPduInfo *pduInfo,
                                  U32 ttiCnt));
#else 
EXTERN Void rlcUmmProcessPdus ARGS ((RlcCb *gCb, 
                                    RlcUlRbCb *rbCb,
                                    KwPduInfo *pduInfo));
#endif 
EXTERN Void rlcUmmUlReEstablish ARGS ((RlcCb *gCb, 
                                     CmLteRlcId *rlcId, 
                                     RlcUlRbCb *rbCb));

EXTERN Void rlcUmmReOrdTmrExp ARGS((RlcCb *gCb, RlcUlRbCb  *rbCb));


EXTERN Void rlcUmmFreeUlRbCb ARGS ((RlcCb *gCb, RlcUlRbCb *rbCb)); 

/****************************************************************************
 *                    Acknowledged Mode Functions 
 ***************************************************************************/
EXTERN Void rlcAmmUlReEstablish ARGS((RlcCb *gCb, 
                                     CmLteRlcId rlcId, 
                                     Bool sndReEst,
                                     RlcUlRbCb *rbCb));
#ifdef LTE_L2_MEAS
EXTERN Void rlcAmmProcessPdus ARGS((RlcCb *gCb,
                                  RlcUlRbCb *rbCb, 
                                  KwPduInfo *pduInfo,
                                  U32 ttiCnt));
#else
EXTERN Void rlcAmmProcessPdus ARGS((RlcCb *gCb, 
                                  RlcUlRbCb *rbCb,
                                  KwPduInfo *pduInfo));
#endif 

EXTERN Void rlcAmmReOrdTmrExp ARGS((RlcCb *gCb, RlcUlRbCb *rbCb));

EXTERN Void rlcAmmStaProTmrExp ARGS((RlcCb *gCb, RlcUlRbCb *rbCb));

EXTERN Void rlcAmmFreeUlRbCb ARGS ((RlcCb *gCb, RlcUlRbCb *rbCb));

/****************************************************************************
 *                    Utility Functions 
 ***************************************************************************/
EXTERN Void rlcUtlStoreRecBuf ARGS ((CmLListCp        *recBufLst,
                                    RlcAmRecBuf       *recBuf,
                                    RlcSn              sn
                                   ));
EXTERN RlcAmRecBuf* rlcUtlGetRecBuf ARGS ((CmLListCp        *recBufLst,
                                  RlcSn              sn
                                  ));
EXTERN Void rlcUtlDelRecBuf ARGS ((CmLListCp        *recBufLst,
                                  RlcAmRecBuf       *recBuf,
                                  RlcCb              *gCb
                                  ));

EXTERN S16 rlcUtlRcvFrmLi ARGS ((RlcCb *gCb, KwDatIndInfo *datIndInfo));

EXTERN S16 rlcUtlSndDatInd ARGS ((RlcCb *gCb,RlcUlRbCb *rbCb, Buffer *sdu));

#ifdef LTE_L2_MEAS
EXTERN S16 rlcUtlHdlL2TmrExp   ARGS (( RlcCb *gCb, RlcL2MeasEvtCb *measEvtCb));

EXTERN  Void rlcUtlCalUlIpThrPutIncTTI ARGS ((RlcCb *gCb, 
                                             RlcUlRbCb  *rbCb,
                                             U32 ttiCnt));

EXTERN Void rlcUtlCalUlIpThrPut ARGS((RlcCb *gCb, 
                                     RlcUlRbCb *rbCb, 
                                     Buffer *pdu, 
                                     U32 ttiCnt));

EXTERN S16 rlcUtlSndUlL2MeasCfm  ARGS ((RlcCb *gCb, RlcL2MeasEvtCb *measEvtCb));

EXTERN S16 rlcUtlSndUlL2MeasNCfm ARGS ((RlcCb *gCb, 
                                       RlcL2MeasReqEvt *measReqEvt,
                                       RlcL2MeasCfmEvt *measCfmEvt));

EXTERN S16 rlcUtlL2MeasUlInit ARGS((RlcCb *gCb));

EXTERN Void rlcUtlResetUlL2MeasInRlcRb ARGS((RlcCb *gCb,
                                           RlcL2MeasCb *measCb,
                                           U8 measType));

EXTERN S16 rlcUtlValidateIpThL2Meas ARGS ((RlcL2MeasReqEvt *measReqEvt,
         RlcL2MeasCfmEvt *measCfmEvt));

#endif /*  LTE_L2_MEAS */
/****************************************************************************
 *                    Activation Functions 
 ***************************************************************************/
EXTERN S16 rlcUlActvInit ARGS ((Ent ent, 
                               Inst inst, 
                               Region region, 
                               Reason reason));

EXTERN S16 rlcUlActvTsk ARGS ((Pst *pst, Buffer *mBuf));

/****************************************************************************
 *                    Debug Functions 
 ***************************************************************************/
EXTERN Void DumpRLCUlDebugInformation ARGS((Void));

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __KW_ULX__ */

  
/********************************************************************30**
  
         End of file
**********************************************************************/
