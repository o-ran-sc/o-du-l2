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

/**********************************************************************
  
        Name:     NR RLC file for uplink and non real time tasks 
    
        Type:     C include file
  
        Desc:     This file contains helper macros for RLC uplink
                  and non real time tasks
 
        File:     rlc_ul.h

**********************************************************************/
/** 
 * @file rlc_ul.h
 * @brief RLC uplink helper macros
*/

#ifndef __RLC_ULH__
#define __RLC_ULH__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @def RLC_MEAS_IS_UL_IP_MEAS_ON_FOR_RB
 *
 *    This macro is used to check if UL IP throughput measurement is ON
 *    or off for the passed rb
 *
 *    Returns TRUE(non-zero) if measurement is ON else FALSE (zero)
 *
 * @param[in] _gCb     RLC UL Cb
 * @param[in] _rbCb    RLC uplink control block
 *
*/ 
#define RLC_MEAS_IS_UL_IP_MEAS_ON_FOR_RB(_gCb, _rbCb)  \
     ((_rbCb->rlcId.rbType == CM_LTE_DRB) && \
     (_gCb->u.ulCb->rlcL2Cb.measOn[_rbCb->qci] & LKW_L2MEAS_UL_IP) && \
     (_rbCb->rbL2Cb.measOn & LKW_L2MEAS_UL_IP))

typedef struct rlcUlUeCb RlcUlUeCb;

/** 
 * @brief  Structure to hold a RLC UM PDU segment
 *
 * @details
 *    - lstEnt : This is required for the linked list in which the segments
 *               are stored
 *    - seg    : Holds the segment data
 *    - segSz  : The length of the segment in bytes
 *    - soEnd  : SOEnd
 *    - umHdr  : The UM Header for the PDU segment
 *
*/
typedef struct rlcUmSeg
{
   CmLList   lstEnt;   /*!< List entry for PDU segment */
   Buffer    *seg;     /*!< PDU segment */
   MsgLen    segSz;    /*!< Buffer Size */
   uint16_t  soEnd;    /*!< Segment Offset End */
   RlcUmHdr  umHdr;    /*!<Um Header */
}RlcUmSeg;

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
   RlcSn       sn;            /*!< Sequence Number */
   CmLList     lnk;           /*!< Link to the receive buffer list */
   Bool        allSegRcvd;    /*!< Flag to check whether all seg are received */
   Bool        noMissingSeg;  /*!< Flag to check all the bytes are received before the last byte of segment */
   CmLListCp   segLst;        /*!< PDU Segments list */
   uint16_t    expSo;         /*!< Next expected seg offset */
   Bool        allRcvd;       /*!< All bytes received or not */
   RlcUmSeg    *expByteSeg;   /*!< Next expected byte segment */
   Buffer      *pdu;          /**< Buffer holding the UM PDU */
   RlcUmHdr    umHdr;         /**< UM PDU Header Information */
   MsgLen      pduSz;         /**< PDU Size */
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
   uint8_t       snLen;         /**< Sequence number length */
   uint8_t       reAsmblTmrInt; /**< Timer Interval */
   CmLListCp     *recBufLst;    /**!<Reception Buffer List */
   RlcSn         umWinSz;       /**< UM window size */
   uint16_t      modBitMask;    /**< Bitmask for modulus to wrap around variables */
   RlcSn         sn;            /**< Sequence number */
   RlcSn         vrUr;          /**< VR(UR) - Receive state variable */
   RlcSn         vrUh;          /**< VR(UH) - Highest received state variable */
   RlcSn         vrUx;          /**< VR(UX) - Reordering state variable */
   CmTimer       reAsmblTmr;    /**< Reordering Timer */
   Buffer        *assembleSdu;  /**< Assemble Sdu - Remains till the complete SDU is received */
   uint16_t      expSo;         /*!< Expected SO for reassembly */
   RlcSn         expSn;         /*!< Expected Sn */
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
   uint16_t  soEnd;    /**< Segment Offset End */
   RlcAmHdr  amHdr;    /**< AM header */
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
   RlcAmHdr    amHdr;         /**< AM header Info */
   CmLListCp   segLst;        /**< PDU Segments list */
   RlcSeg      *expByteSeg;   /**< Next expected byte segment */
   uint16_t    expSo;         /**< Next expected seg offset */
   Bool        allRcvd;       /**< All bytes received or not */
   Bool        isDelvUpperLayer; /**< Is it sent to upper layer */ 
   Bool        noMissingSeg;  /*!< Flag to check all the bytes are received before the last byte of segment */
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
 *    - reAsmblTmr         : The Reordering Timer
 *    - reAsmblTmrInt      : Re-ordering timer interval
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
   uint16_t          expSo;                 /**< Expected SO for reassembly */
   CmTimer      staProhTmr;            /**< T_status_prohibit Timer */
   uint16_t          staProhTmrInt;         /**< Timer Interval */
   CmTimer      reAsmblTmr;              /**< T_reordering Timer */
   uint8_t           reAsmblTmrInt;           /**< Timer Interval */
   Bool         gatherStaPduInfo;      /**< Gather STATUS PDU creation info*/
   Bool         isOutOfSeq;            /**< To identify whether packets are Out-Of-Seq or not */
   uint8_t           snLen;                 /*!< Sequence number length:12 bit or 18 bit : 5GNR RLC */
   uint32_t          snModMask;             /*!< (2 Pwr SnLen - 1): 5GNR RLC */
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
   uint8_t          qci;                 /**< qci of the RB */
   RlcL2MeasIpThruput l2MeasIpThruput;   /**< Holds related parameter for
                                             DL/Ul ip throughput>*/
#endif /* LTE_L2_MEAS */
   CmLteRlcId     rlcId;      /**< RLC Identifier */
   RlcLchInfo     lch;        /**< Logical Channel Info */
   Snssai         *snssai;    /**< Snssai Info */
   CmLteRlcMode   mode;       /**< Entity Mode */
   uint8_t        dir;        /**< Direction for UL/DL */
   Inst           inst;       /**< Tapa where Rb created Instance id */
   SpId           k1wuSapId;   /**< KWU sap Id, to get the KwuSapCb */
   SpId           udxSapId;   /**< KWU sap Id, to get the KwuSapCb */
   uint32_t       transId;    /**< Transaction Id for RLC */
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
* @brief  Structure to hold ue delete information 
*
* @details
*    - pst      :  Pst 
*    - ueDelTmr :  Ue delete timer
*/
typedef struct rlcUeDeleteInfo
{
   Pst        pst;        /*Pst */ 
   CmTimer    ueDelTmr;   /*Ue delete timer*/
}RlcUeDeleteInfo;

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
   CmLteRnti       ueId;     /*!< UE Id */
   CmLteCellId     cellId;   /*!< Cell Id */
   RlcUlRbCb       *srbCb[RLC_MAX_SRB_PER_UE]; /**< SRB RbCbs within an UE */ 
   RlcUlRbCb       *drbCb[RLC_MAX_DRB_PER_UE]; /**< DRB RbCbs within an UE */ 
   RlcUlLch        lCh[RLC_MAX_LCH_PER_UE];    /**< Logical channels of an UE*/
/* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
   uint32_t        firstPacketTTI;            /*!< is first packet of the burst */
   uint16_t        numActRb[LKW_MAX_QCI];     /**< number of RBs Active */
   Bool            isUlBurstActive;   /*!<Has the Burst started for UL IP Thrpt meas */
#endif /* LTE_L2_MEAS */
   RlcUeDeleteInfo ueDeleteInfo;    /*!<Ue Delete Info */
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
   uint32_t              transId;                     /**< Locally generated Transaction Id */
   uint32_t              uprLyrTransId;               /**< Transaction Id generated by upper layer. 
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
 *                      Declarations
 ***************************************************************************/
/****************************************************************************
 *                    Configuration Functions 
 ***************************************************************************/

S16 rlcValidateRbCfgParams ARGS ((RlcCb *gCb,
                                        CmLteRnti   ueId,
                                        CmLteCellId cellId,
                                        RlcEntCfgInfo  *cfgToValidate,
                                        CmStatus        *status));
S16 rlcCfgValidateUlRb ARGS (( RlcCb *gCb,
                                    RlcEntCfgInfo *cfgToValidate,
                                    RlcUlEntTmpData *cfgInfo, 
                                    RlcUlCfgTmpData *cfg));

S16 rlcCfgRollBackUlRb ARGS ((RlcCb *gCb,
                                    CmLteRnti ueId,
                                    RlcEntCfgInfo *cfgToValidate,
                                    RlcUlEntTmpData *cfgTempData));

Void rlcCfgApplyUlRb ARGS ((RlcCb *gCb,
                                  RlcEntCfgInfo *cfgToAply,
                                  RlcUlEntTmpData *cfgTmpData,
                                  RlcUlCfgTmpData *cfgTmpInfo));

S16 rlcCfgValidateReEstRb ARGS ((RlcCb *gCb,
                                       CmLteRnti  ueId,
                                       CmLteCellId cellId,
                                       RlcEntCfgInfo *cfgToValidate,
                                       RlcUlEntTmpData   *cfgTmpData));

Void rlcCfgApplyReEstUlRb ARGS ((RlcCb *gCb,
                                      CmLteRnti ueId,
                                      CmLteCellId cellId,
                                      Bool sndReEstInd,
                                      RlcUlEntTmpData *cfgTmpData));

Void rlcCfgApplyDelUlCell ARGS ((RlcCb *gCb,
                                      RlcUlCfgTmpData *cfgTmpData));

S16 rlcCfgValidateDelUlCell ARGS ((RlcCb *gCb,
                                         CmLteCellId cellId,
                                         RlcEntCfgInfo *cfgToValidate,
                                         RlcUlEntTmpData   *cfgTmpData,
                                         RlcUlCfgTmpData *cfgInfo));

S16 rlcCfgValidateDelUlUe ARGS ((RlcCb *gCb,
                                       RlcEntCfgInfo *cfgToValidate,
                                       RlcUlEntTmpData *cfgTmpData,
                                       RlcUlCfgTmpData *cfgInfo));

Void rlcCfgApplyDelUlUe ARGS ((RlcCb *gCb,
                                    RlcUlCfgTmpData *cfgTmpData));

Void rlcUlHdlCfgReq ARGS ((RlcCb *gCb,
                                     RlcUlCfgTmpData *cfgInfo,
                                     RlcCfgInfo *cfg));

Void rlcCfgApplyUlUeIdChng ARGS ((RlcCb *gCb,
                                        CkwUeInfo *ueInfo,
                                        CkwUeInfo *newUeInfo,
                                        RlcUlCfgTmpData *cfgTmpInfo));

S16 rlcCfgValidateUeIdChng ARGS ((RlcCb *gCb,
                                        CkwUeInfo *ueInfo,
                                        CkwUeInfo *newUeInfo,
                                        RlcUlCfgTmpData *cfgTmpInfo));

/****************************************************************************
 *                    DBM module Functions 
 ***************************************************************************/
S16 rlcDbmUlInit ARGS ((RlcCb *gCb));

Void rlcDbmUlDeInit ARGS ((RlcCb *gCb));

S16 rlcDbmAddUlUeCb ARGS ((RlcCb *gCb,
                                    CmLteRnti ueId,
                                    CmLteCellId cellId,
                                    RlcUlUeCb *ueCb));

uint8_t rlcDbmFetchUlUeCb ARGS ((RlcCb *gCb,
                                   CmLteRnti ueId,
                                   CmLteCellId cellId,
                                   RlcUlUeCb **ueCb));

Void rlcDbmDelUlUeCb ARGS ((RlcCb *gCb,   
                                  RlcUlUeCb *ueCb,
                                  Bool abortFlag));

Void rlcDbmDelAllUlUe ARGS ((RlcCb *gCb));

S16 rlcDbmAddUlCellCb ARGS ((RlcCb *gCb,
                                      CmLteCellId cellId,
                                      RlcUlCellCb *cellCb));

Void rlcDbmFetchUlCellCb ARGS ((RlcCb *gCb, 
                                      CmLteCellId cellId,
                                      RlcUlCellCb **cellCb));

Void rlcDbmDelUlCellCb ARGS ((RlcCb *gCb,  
                                   RlcUlCellCb *cellCb));

Void rlcDbmDelAllUlCell ARGS ((RlcCb *gCb));

Void rlcDbmFetchUlRbCbByRbId ARGS ((RlcCb *gCb, 
                                          CmLteRlcId *rlcId, 
                                          RlcUlRbCb **rbCb));

Void rlcDbmFetchUlRbCbFromLchId ARGS ((RlcCb *gCb, 
                                             CmLteRnti ueId, 
                                             CmLteCellId cellId, 
                                             CmLteLcId lcId,  
                                             RlcUlRbCb **rbCb));

Void rlcDbmDelAllUlRb ARGS ((RlcCb *gCb, 
                                   RlcUlRbCb **rbCbLst, 
                                   uint8_t numRbCb));

S16 rlcDbmAddUlTransaction ARGS((RlcCb *gCb, RlcUlCfgTmpData *cfg));

S16 rlcDbmFindUlTransaction ARGS((RlcCb *gCb, 
                                        uint32_t transId, 
                                        RlcUlCfgTmpData **cfg));

S16 rlcDbmDelUlTransaction ARGS((RlcCb *gCb, RlcUlCfgTmpData *cfg));

S16 rlcDbmDelAllUlTransactions ARGS((RlcCb *gCb));

Void rlcDbmUlShutdown ARGS ((RlcCb *gCb));

/****************************************************************************
 *                    Transparent Mode Functions 
 ***************************************************************************/
#ifdef CCPU_OPT
void rlcTmmRcvFrmMac ARGS ((RlcCb *gCb,
                            RlcUlRbCb *rbCb,
			    CmLteRnti   tCrnti,
                            Buffer *pdu));

#else
void rlcTmmRcvFrmMac ARGS ((RlcCb *gCb,
                            RlcUlRbCb *rbCb,
                            Buffer *pdu));
#endif				       

Void rlcTmmUlReEstablish ARGS ((RlcCb *gCb, RlcUlRbCb *rbCb));

/****************************************************************************
 *                    Unacknowledged Mode Functions 
 ***************************************************************************/
#ifdef LTE_L2_MEAS
Void rlcUmmProcessPdus ARGS((RlcCb *gCb,
                                  RlcUlRbCb *rbCb, 
                                  KwPduInfo *pduInfo,
                                  uint32_t ttiCnt));
#else 
Void rlcUmmProcessPdus ARGS ((RlcCb *gCb, 
                                    RlcUlRbCb *rbCb,
                                    KwPduInfo *pduInfo));
#endif 
Void rlcUmmUlReEstablish ARGS ((RlcCb *gCb, 
                                     CmLteRlcId *rlcId, 
                                     RlcUlRbCb *rbCb));

Void rlcUmmReAsmblTmrExp ARGS((RlcCb *gCb, RlcUlRbCb  *rbCb));


Void rlcUmmFreeUlRbCb ARGS ((RlcCb *gCb, RlcUlRbCb *rbCb)); 

/****************************************************************************
 *                    Acknowledged Mode Functions 
 ***************************************************************************/
Void rlcAmmUlReEstablish ARGS((RlcCb *gCb, 
                                     CmLteRlcId rlcId, 
                                     Bool sndReEst,
                                     RlcUlRbCb *rbCb));
#ifdef LTE_L2_MEAS
Void rlcAmmProcessPdus ARGS((RlcCb *gCb,
                                  RlcUlRbCb *rbCb, 
                                  KwPduInfo *pduInfo,
                                  uint32_t ttiCnt));
#else
Void rlcAmmProcessPdus ARGS((RlcCb *gCb, 
                                  RlcUlRbCb *rbCb,
                                  KwPduInfo *pduInfo));
#endif 

Void rlcAmmReAsmblTmrExp ARGS((RlcCb *gCb, RlcUlRbCb *rbCb));

Void rlcAmmStaProTmrExp ARGS((RlcCb *gCb, RlcUlRbCb *rbCb));

Void rlcAmmFreeUlRbCb ARGS ((RlcCb *gCb, RlcUlRbCb *rbCb));

/****************************************************************************
 *                    Utility Functions 
 ***************************************************************************/

void rlcUtlStoreUmRecBuf ARGS ((CmLListCp   *recBufLst,
                                RlcUmRecBuf *recBuf,
                                RlcSn        sn
                              ));
RlcUmRecBuf* rlcUtlGetUmRecBuf ARGS ((CmLListCp        *recBufLst,
                                      RlcSn              sn
                                    ));
void rlcUtlDelUmRecBuf(RlcCb *gCb, CmLListCp *recBufLst, RlcUmRecBuf  *recBuf);

Void rlcUtlStoreRecBuf ARGS ((CmLListCp        *recBufLst,
                                    RlcAmRecBuf       *recBuf,
                                    RlcSn              sn
                                   ));
RlcAmRecBuf* rlcUtlGetRecBuf ARGS ((CmLListCp        *recBufLst,
                                  RlcSn              sn
                                  ));
Void rlcUtlDelRecBuf ARGS ((CmLListCp        *recBufLst,
                                  RlcAmRecBuf       *recBuf,
                                  RlcCb              *gCb
                                  ));

uint8_t rlcUtlRcvFrmMac ARGS ((RlcCb *gCb, KwDatIndInfo *datIndInfo));

uint8_t rlcUtlSendUlDataToDu ARGS ((RlcCb *gCb,RlcUlRbCb *rbCb, Buffer *sdu));

#ifdef LTE_L2_MEAS
S16 rlcUtlHdlL2TmrExp   ARGS (( RlcCb *gCb, RlcL2MeasEvtCb *measEvtCb));

Void rlcUtlCalUlIpThrPutIncTTI ARGS ((RlcCb *gCb, 
                                             RlcUlRbCb  *rbCb,
                                             uint32_t ttiCnt));

Void rlcUtlCalUlIpThrPut ARGS((RlcCb *gCb, 
                                     RlcUlRbCb *rbCb, 
                                     Buffer *pdu, 
                                     uint32_t ttiCnt));

S16 rlcUtlSndUlL2MeasCfm  ARGS ((RlcCb *gCb, RlcL2MeasEvtCb *measEvtCb));

S16 rlcUtlSndUlL2MeasNCfm ARGS ((RlcCb *gCb, 
                                       RlcL2MeasReqEvt *measReqEvt,
                                       RlcL2MeasCfmEvt *measCfmEvt));

S16 rlcUtlL2MeasUlInit ARGS((RlcCb *gCb));

Void rlcUtlResetUlL2MeasInRlcRb ARGS((RlcCb *gCb,
                                           RlcL2MeasCb *measCb,
                                           uint8_t measType));

S16 rlcUtlValidateIpThL2Meas ARGS ((RlcL2MeasReqEvt *measReqEvt,
         RlcL2MeasCfmEvt *measCfmEvt));

#endif /*  LTE_L2_MEAS */
/****************************************************************************
 *                    Activation Functions 
 ***************************************************************************/
S16 rlcUlActvInit ARGS ((Ent ent, 
                               Inst inst, 
                               Region region, 
                               Reason reason));

S16 rlcUlActvTsk ARGS ((Pst *pst, Buffer *mBuf));

/****************************************************************************
 *                    Debug Functions 
 ***************************************************************************/
Void DumpRLCUlDebugInformation ARGS((Void));

uint8_t rlcProcCommLcUlData(Pst *pst, SuId suId, RguCDatIndInfo  *datInd);
uint8_t rlcProcDedLcUlData(Pst *pst, SuId suId, RguDDatIndInfo  *datInd);

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* __RLC_ULH__ */
/**********************************************************************
  
         End of file
**********************************************************************/
