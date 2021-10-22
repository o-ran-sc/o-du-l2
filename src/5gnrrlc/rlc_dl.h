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
  
        Name:     NR RLC file for downlink and non real time tasks 
    
        Type:     C include file
  
        Desc:     This file contains helper macros for RLC downlink
                  and non real time task
 
        File:     rlc_dl.h
  
*********************************************************************21*/
/** @file rlc_dl.h
@brief RLC Downlink helper macros 
*/

#ifndef __RLC_DLH__
#define __RLC_DLH__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
#ifdef TENB_STATS
#ifndef L2_l3_SPLIT
#include "pj_tenb_stats.x"
#else
#include "l2_tenb_stats.x"
#endif
#endif

#define RLC_DL_INST 1
#define RLC_MAX_RNG_DEQ_INDX 8

/* Below are the status used within RLC */
#define RLC_SDU_OPAT_NOUPDATE 0
#define RLC_SDU_OPAT_UPD_TX 1
#define RLC_SDU_OPAT_UPD_RETX   2

/* Below are the status which needs to be updated to MAC */
#define RLC_RGU_SDU_OPAT_NOUPDATE 0
#define RLC_RGU_SDU_OPAT_UPDATE   1
#define RLC_RGU_SDU_OPAT_BREACH   2

/* PDB Values for various QCI bearers : This value is considered 
   after taking into account a 20ms delay at the network */
#define RLC_PDB_VALUE_FOR_QCI1    80
#define RLC_PDB_VALUE_FOR_QCI2    130
#define RLC_PDB_VALUE_FOR_QCI3    30
#define RLC_PDB_VALUE_FOR_QCI4    280
#define RLC_PDB_VALUE_FOR_QCI5    80
#define RLC_PDB_VALUE_FOR_QCI6    280
#define RLC_PDB_VALUE_FOR_QCI7    80
#define RLC_PDB_VALUE_FOR_QCI8    280
#define RLC_PDB_VALUE_FOR_QCI9    280

#define RLC_DATA_BITMASK          0x80 /* Data bitmask is used to fill D/C bit for data*/
/**
 * @def RLC_MEAS_IS_DL_IP_MEAS_ON_FOR_RB
 *
 *    This macro is used to check if DL IP throughput measurement is ON
 *    or off for the passed rb
 *
 *    Returns TRUE (non-zero) if measurement is ON else FALSE (zero)
 *
 * @param[in] _gCb     RLC DL Cb
 * @param[in] _rbCb    RLC downlink control block
 *
*/ 
#ifdef LTE_L2_MEAS
#define RLC_MEAS_IS_DL_ANY_MEAS_ON_FOR_RB(_gCb, _rbCb)  \
     ((_rbCb->rlcId.rbType == CM_LTE_DRB) && \
            (_gCb->u.dlCb->rlcL2Cb.measOn[_rbCb->qci]))

#define RLC_MEAS_IS_DL_IP_MEAS_ON_FOR_RB(_gCb, _rbCb)  \
     ((_rbCb->rlcId.rbType == CM_LTE_DRB) && \
      (_gCb->u.dlCb->rlcL2Cb.measOn[_rbCb->qci] & LKW_L2MEAS_DL_IP) && \
      (_rbCb->rbL2Cb.measOn & LKW_L2MEAS_DL_IP))

#define RLC_MEAS_IS_DL_DELAY_MEAS_ON_FOR_RB(_gCb, _rbCb)  \
     ((_rbCb->rlcId.rbType == CM_LTE_DRB) && \
            (_gCb->u.dlCb->rlcL2Cb.measOn[_rbCb->qci] & LKW_L2MEAS_DL_DELAY))

#define RLC_MEAS_IS_DL_UU_LOSS_MEAS_ON_FOR_RB(_gCb, _rbCb)  \
     ((_rbCb->rlcId.rbType == CM_LTE_DRB) && \
            (_gCb->u.dlCb->rlcL2Cb.measOn[_rbCb->qci] & LKW_L2MEAS_UU_LOSS))
#define RLC_UPD_PDCP_L2_DLDELAY_STS(_rlcCb, _rlcRbCb, _delay)                     \
{                                                                              \
   if(((_rlcRbCb)->rlcId.rbType == CM_LTE_DRB) &&                               \
      ((_rlcCb)->u.dlCb->rlcL2Cb.measOn[_rlcRbCb->qci] & LKW_L2MEAS_DL_DELAY))    \
   {                                                                           \
      (_rlcRbCb)->rbL2Cb.l2Sts[RLC_L2MEAS_DL_DELAY]->dlPjSduDelay.sduDelay += _delay; \
      (_rlcRbCb)->rbL2Cb.l2Sts[RLC_L2MEAS_DL_DELAY]->dlPjSduDelay.numSdus++; \
   }                                                                    \
}

#define RLC_UPD_L2_UU_LOSS_PKTS(_rlcCb, _rlcRbCb, _val)                         \
{                                                                 \
   if(((_rlcRbCb)->rlcId.rbType == CM_LTE_DRB) &&                              \
      ((_rlcCb)->u.dlCb->rlcL2Cb.measOn[_rlcRbCb->qci] & LKW_L2MEAS_UU_LOSS))              \
   {                                                              \
      (_rlcRbCb)->rbL2Cb.l2Sts[RLC_L2MEAS_UU_LOSS]->uuLoss.dLoss += _val;     \
   }                                                              \
}
#define RLC_UPD_L2_UU_LOSS_POS_PKTS(_rlcCb,_rlcRbCb,_val)                         \
{                                                                 \
   if(((_rlcRbCb)->rlcId.rbType == CM_LTE_DRB) &&                              \
      ((_rlcCb)->u.dlCb->rlcL2Cb.measOn[_rlcRbCb->qci] & LKW_L2MEAS_UU_LOSS))              \
   {                                                              \
      (_rlcRbCb)->rbL2Cb.l2Sts[RLC_L2MEAS_UU_LOSS]->uuLoss.posPkts += _val;     \
   }                                                              \
}
/* Discard new changes starts */
#define RLC_UPD_L2_DL_DISC_SDU_STS(_rlcCb,_rlcRbCb)                         \
{                                                                 \
   if(((_rlcRbCb)->rlcId.rbType == CM_LTE_DRB) &&                              \
      ((_rlcCb)->u.dlCb->rlcL2Cb.measOn[_rlcRbCb->qci] & LKW_L2MEAS_DL_DISC))              \
   {                                                              \
      (_rlcRbCb)->rbL2Cb.l2Sts[RLC_L2MEAS_DL_DISC]->dlDisc.discSdus++;     \
   }                                                              \
}                                              

#define RLC_UPD_L2_DL_TOT_SDU_STS(_rlcCb,_rlcRbCb)                             \
{                                                                 \
   if(((_rlcRbCb)->rlcId.rbType == CM_LTE_DRB) &&                              \
      ((_rlcCb)->u.dlCb->rlcL2Cb.measOn[_rlcRbCb->qci] & LKW_L2MEAS_DL_DISC))   \
   {                                                              \
      (_rlcRbCb)->rbL2Cb.l2Sts[RLC_L2MEAS_DL_DISC]->dlDisc.totSdus++;      \
   }                                                              \
}

#define RLC_UPD_L2_DECR_NONIP_PER_QCI_RB_COUNT(_rlcCb, _rlcRbCb)                 \
{                                                                 \
   if(((_rlcRbCb)->rlcId.rbType == CM_LTE_DRB) &&                    \
      ((_rlcCb)->u.dlCb->rlcL2Cb.measOn[_rlcRbCb->qci] & LKW_L2MEAS_DL_DISC))   \
   {                                                              \
      uint32_t idx1;                                                    \
      for (idx1 = 0; idx1 < LKW_MAX_L2MEAS; idx1++)                  \
      {                                                           \
         if(_rlcCb->u.dlCb->rlcL2Cb.rlcL2EvtCb[idx1].measCb.measType & LKW_L2MEAS_DL_DISC)                 \
         {                                                        \
            if(_rlcCb->u.dlCb->rlcL2Cb.rlcL2EvtCb[idx1].measCb.val.nonIpThMeas.measData[(_rlcRbCb)->qci].totDrbsPerQci > 0) \
            {                                                                                               \
               _rlcCb->u.dlCb->rlcL2Cb.rlcL2EvtCb[idx1].measCb.val.nonIpThMeas.measData[(_rlcRbCb)->qci].totDrbsPerQci--;   \
               if (_rlcCb->u.dlCb->rlcL2Cb.rlcL2EvtCb[idx1].measCb.val.nonIpThMeas.measData[(_rlcRbCb)->qci].totDrbsPerQci == 0) \
               {                                                                                                 \
                  _rlcCb->u.dlCb->rlcL2Cb.rlcL2EvtCb[idx1].measCb.val.nonIpThMeas.qci[(_rlcRbCb)->qci] = 0;                      \
                  memset(&_rlcCb->u.dlCb->rlcL2Cb.rlcL2EvtCb[idx1].measCb.val.nonIpThMeas.measData[(_rlcRbCb)->qci], 0,  \
                     sizeof(_rlcCb->u.dlCb->rlcL2Cb.rlcL2EvtCb[idx1].measCb.val.nonIpThMeas.measData[(_rlcRbCb)->qci]));            \
               }                                                                                                 \
            }                                                                                                    \
         }                                                                                                       \
      }                                                                                                          \
   }                                                                                                             \
}
#else
#define RLC_UPD_PDCP_L2_DLDELAY_STS(_rlcCb, _rlcRbCb, _delay)
#define RLC_MEAS_IS_DL_IP_MEAS_ON_FOR_RB(_gCb, _rbCb)
#define RLC_MEAS_IS_DL_DELAY_MEAS_ON_FOR_RB(_gCb, _rbCb)
#define RLC_UPD_L2_DL_DISC_SDU_STS(_rlcCb,_rlcRbCb)
#define RLC_UPD_L2_DL_TOT_SDU_STS(_rlcCb, _rlcRbCb)
#define RLC_UPD_L2_DECR_NONIP_PER_QCI_RB_COUNT(_rlcCb, _rlcRbCb)
#endif


typedef struct rlcDlUeCb RlcDlUeCb;

/** 
 * @brief  Structure to hold the SN of the PDU onto which a SDU is mapped
 *
 * @details
 *    - sn : Sequence number of the AM pdu
*/
typedef struct rlcPduMap
{
  RlcSn   sn;     /*!< Mapped PDU's SN */
}RlcPduMap;

/** 
 * @brief  Structure to hold information regarding a SDU received from PDCP
 *
 * @details
 *    - lstEnt : Used for linking ( we have linked list of SDUs) which forms
 *               the SDU queue
 *    - mBuf   : Pointer to the SDU data
 *    - sduSz  : Size of the mBuf above
 *    - actSz  : Length of the SDU when received from PDCP, this might be 
 *               different from sduSz, in case when the SDU is split
 *    - mode   : Depending on the type, holds AM, UM or TM mode information
 *      - tm
 *        - sfn      : System Frame number for BCCH/PCCH
 *        - subframe : Subframe number
 *        - rnti     : RNTI for CCCH
 *      - um
 *        - sduId       : Unique identity for the SDU
 *        - isSegmented : Whether this SDU is segmented into multiple PDUs
 *      - am
 *        - sduId       : Unique identity for the SDU
 *        - rcvdSz      : Length of this SDU which is received by the peer
 *        - isSegmented : Whether this SDU is segmented into multiple PDUs
*/
typedef struct rlcSdu                        
{
   CmLList   lstEnt;     /*!< List entry for SDU */
   Buffer    *mBuf;      /*!< SDU buffer */ 
   MsgLen    sduSz;      /*!< Buffer Size */
   MsgLen    actSz;      /*!< Actual buffer Size */
/* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
   RlcSduSnMap  *snMap;   /*!< Sdu and the list of SN it maps to */
#endif /* LTE_L2_MEAS */

   union
   {
      struct   _t 
      {
         uint16_t    sfn;        /*!< Sub frame number for BCCH/PCCH */
         uint16_t    slot;   /*!< Subframe number */
         CmLteRnti   rnti;       /*!< RNTI for CCCH */
#ifdef EMTC_ENABLE
         uint8_t          emtcDiReason; /*!< DI reason */
         uint8_t          pnb;       /*!<narrow band on which Ue performs Paging*/
#endif
      }tm;   /*!< SDU for TM mode */

      struct   _u
      {
         uint32_t  sduId;         /*!< SDU Identifier */
         Bool      isSegmented;   /*!< Flag to indicate if the SDU is segmented */
      }um;   /*!< SDU for UM mode */

      struct   _a
      {
         uint32_t   sduId;                    /*!< SDU Identifier */
         MsgLen     rcvdSz;                   /*!< Recvd SDU Size from peer */
         Bool       isSegmented;              /*!< Is SDU segmented */
         RlcSn        sn;                       /*!< SN assigned to this SDU*/
      }am;   /*!< SDU for AM mode */
   }mode;   /*!< RLC mode specific SDUs */
   /* Discard new changes starts */
   Ticks               arrTime;     /*!< Arrival time of the PDCP PDU at RLC */
   /* Discard new changes ends */

}RlcSdu;

/** @addtogroup tmmode */
/*@{*/

/** 
 * @brief  Structure to hold Transparent Mode information for a RB
 *
 * @details
 *    - sduQ : Queue of SDUs
*/
typedef struct rlcTm
{
   CmLListCp sduQ; /*!< SDU queue for TM */
}RlcTm;
/*@}*/

/** 
 * @brief  Structure to hold buffer occupancy report sent to MAC
 *
 * @details
 *    - bo         : Size of the buffers at RLC
 *    - estHdrSz   : Size for all the headers as estimated by RLC
 *    - staPduPrst : Whether RLC wants to send a STATUS PDU or not
*/
typedef struct rlcBoRep
{
   int32_t  bo;            /*!< Buffer occupancy */
   uint16_t estHdrSz;      /*!< Estimated header size for the bo */
   bool     staPduPrsnt;   /*!< Indicate control bo present or not */
   uint32_t staPduBo;     /*!< Indicate bo for control PDU */
   uint32_t oldestSduArrTime;
} RlcBoRep;

/** 
 * @brief  Structure to hold information sent to MAC in a Data Request
 *
 * @details
 *    - pduSz   : Sum total of sizes of all the PDUs being sent
 *    - pduInfo : Information about each PDU
 *    - boRep   : Current buffer occupancy report
*/
typedef struct rlcDatReq
{
   MsgLen      pduSz;         /*!< PDU(s) size indicated by MAC */ 
   KwPduInfo   pduInfo;       /*!< PDU Information for lChs within UE */
   RlcBoRep     boRep;         /*!< BO report */ 
/* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
   KwLchMapInfo lchMapInfo;   /*!< Logical channel mapping info */
   uint32_t          totMacGrant;
#endif /* LTE_L2_MEAS */
}RlcDatReq;

/** @defgroup ummode UM Module Info 
*/

/** @addtogroup ummode */
/*@{*/
/** 
 * @brief  Structure to hold Unacknowledged mode downlink information for a RB
 *
 * @details
 *    - sduQ        : Control point for the queue of SDUs
 *    - snLen       : length of the squence number. Here it is stored 
 *                    as 1 for 5 bit sequence numbers and 
 *                    2 for 10 bit sequence numbers
 *    - bo          : Current buffer occupancy for this RB
 *    - TX_Next     : SN to be allocated to next SDU
 *    - modBitMask  : Bitmask used to do modulus operations to wrap around 
 *                    state variables, value is 0x3f or 
 *                    0xfff for 6-bit and 12-bit sequence numbers respectively
*/
typedef struct rlcUmDl
{
   CmLListCp   sduQ;            /*!< SDU queue for UM */
   uint8_t     snLen;           /*!< Sequence number length */
   int32_t     bo;              /*!< Buffer occupancy */
   RlcSn       txNext;          /*!< TX_Next */
   uint16_t    modBitMask;      /*!< Bitmask for modulus to wrap around vars*/   
}RlcUmDl;

/** 
 * @brief  Structure to hold information about which SDUs a PDU/segment carries
 *
 * @details
 *    - sduSz  : Size of the SDU or part of it
 *    - sdu    : Pointer to the SDU
*/
typedef struct rlcSduMap
{
   /*MsgLen   actSz;*/         /*!< SDU Size */
   MsgLen   sduSz;         /*!< SDU Size */
   /*Bool     isSegmented;*/   /*!< Flag to indicate if the SDU is segmented */
   RlcSdu*   sdu;           /*!< Pointer to the associated SDU */
}RlcSduMap;

/** 
 * @brief  Structure to hold info about a PDU/segment to be re-transmitted
 *
 * @details
 *    - lstEnt           : Control point to link the re-transmit (the 
 *                         re-transmit PDUs  ,segments are stored as a 
 *                         link list) nodes
 *    - seg              : Pointer to the data
 *    - segSz            : Size of the PDU/segment including the header
 *    - hdrSz            : Size of the AM header for this in bytes
 *    - amHdr            : The header for this PDU/segment
 *    - soEnd            : SO End
 *    - retxCnt          : Number of times this has been re-transmitted
 *    - yetToConst       : Whether the PDU/segment has been constructed (headers
 *                         appended) or not
 *    - pendingReTrans   : TRUE if this node is to be re-transmitted else FALSE
 *    - numSdu           : Number of SDUs this PDU/segment carries
 *    - sduMap           : Infomation about the SDUs this node carries
*/
typedef struct rlcRetx
{
   CmLList    lstEnt;              /*!< List entry for Retransmission PDU */
   Buffer     *seg;                /*!< Buffer of PDU or Segment */
   MsgLen     segSz;               /*!< Segment Size */
   MsgLen     hdrSz;               /*!< Header size for the segment */
   RlcAmHdr   amHdr;               /*!< AM header Info */
   uint16_t   soEnd;               /*!< Segment Offset End */
   RlcSn      retxCnt;             /*!< Retransmission count */
   Bool       yetToConst;          /*!< Yet to construct seg flag */
   Bool       pendingReTrans;      /*!< Is node waiting for re-transmission */
   RlcSduMap  sduMap;              /*!< SDU mapped with PDU */
}RlcRetx;

/** 
 * @brief  Structure to hold a RLC AM PDU Information
 *
 * @details
 *    - lstEnt : This is required for the linked list in which pdu is stored
 *    - pdu    : Holds the pdu
 *    - pduSz  : The length of the segment in bytes
 *    - hdrSz  : Header size of this PDU
 *    - amHdr  : The AM Header for the PDU segment
 *    - RlcSduMap :SDU map info for this PDU 
 *
*/
typedef struct rlcDlPduInfo
{
   CmLList   lstEnt;               /*!< List entry for PDU list */
   Buffer     *pdu;                /*!< Buffer of PDU */
   MsgLen     pduSz;               /*!< PDU Size */
   MsgLen     hdrSz;               /*!< Header size for the segment */
   RlcAmHdr    amHdr;               /*!< AM header Info */
   RlcSduMap   sduMap;              /*!< SDU info for this PDU */

}RlcDlPduInfo;

/** 
 * @brief  Structure to hold info about a PDU that has been transmitted
 *
 * @details
 *    - lnk    : Link to the TX buffer list
 *    - sn     : SN of this RLC SDU
 *    - rlsLnk : Link to store on release list, used for
 *    - pduLst : It contains all PDUs of a particular SN
*/
typedef struct rlcTx
{
   CmLList     lnk;                /**< Link to the TX buffer list*/
   RlcSn        sn;                /*!< SN of this RLC SDU */
   CmLList    rlsLnk;             /*!< Lnk to store on release lst */
   CmLListCp  pduLst;             /*!< It contains all PDUs of a particular SN*/
}RlcTx;

/** @addtogroup ammode */
/*@{*/

/** 
 * @brief  Structure to hold Acknowledged Mode Downlink information for a RB
 *
 * @details
 *    - sduQ           : SDU queue
 *    - nxtTx          : Pointer to the next SDU to be transmitted
 *    - pollRetxTmr    : Poll re-transmission timer
 *    - pollRetxTmrInt : Timer interval (?? secs) of poll re-transmission timer
 *    - pollPdu        : Number of PDUs to send before sending a poll bit
 *    - pollByte       : Number of bytes to send before sending a poll bit
 *    - maxRetx        : Maximum number of re-transmissions to be done for a 
 *                       PDU/segment
 *    - cntrlBo        : Size of the STATUS PDU if it is to be sent, else 0
 *    - retxBo         : Bo of the PDUs/segments to be re-transmitted
 *    - estHdrSz       : Extimated header size of all the SDUs in the queue
 *    - bo             : Bo of the PDUs in the transmission buffer
 *    - txBufLst       : Transmission buffer lists. It's contiguous index by sequence
 *                       number
 *    - rextLst        : Re-transmission buffer. It is a doubly linked list
 *    - nxtRetx        : Next node to be re-transmitted
 *    - txNext         : 5GNR - TX_Next - Send state variable,( in LTE VT(S)) 
 *    - txNextAck      : 5GNR - TX_Next_Ack- Acknowledgement state variable
                       : (in LTE VT(A))     
 *    - pduWoPoll      : Number of PDUs sent without sending a poll bit
 *    - byteWoPoll     : Number of bytes sent without sending a poll bit
 *    - pollSn         : The sequence number on which the last poll bit was sent
 *    - mBuf           : The encoded STATUS pdu to be sent
 *    - maxReTxReached : Whether maximum number of re-transmissions has been
 *                       reached or not
 *    - pStaPdu        : Stores the control pdu (information required to
 *                       create the STATUS PDU) passed from AM uplink
*/
typedef struct rlcAmDl
{
   CmLListCp        sduQ;             /*!< SDU queue */
   RlcSdu           *nxtTx;           /*!< Next SDU to be transmitted */
   CmTimer          pollRetxTmr;      /*!< T_poll_retransmit Timer */
   uint16_t         pollRetxTmrInt;   /*!< Timer Interval */
   int16_t          pollPdu;          /*!< Poll_PDU */
   int32_t          pollByte;         /*!< Poll_Byte */
   uint8_t          maxRetx;          /*!< Max_Retx_Threshold */
   int32_t          cntrlBo;          /*!< BO of control pdu */ 
   int32_t          retxBo;           /*!< BO of Retransmission PDUs */ 
   uint32_t         estHdrSz;         /*!< Estimated header size for the BO */
   int32_t          bo;               /*!< BO of new PDUs */
#ifndef LTE_TDD 
   CmLListCp        *txBufLst;       /*!< Transmission buffer list of type RlcTx */
#else
//   RlcTx             *txBuf[1024];          /*!< Transmission buffer */
#endif
   CmLListCp        retxLst;          /*!< Retransmission PDU list */
   RlcRetx          *nxtRetx;         /*!< Next node to be retransmitted */
   RlcSn            txNext;           /*!< 5GNR: TX_Next, in LTE  VT(S) */
   RlcSn            txNextAck;        /*!< 5GNR: TX_Next_Ack, in LTE VT(A)*/ 
   int16_t          pduWoPoll;        /*!< PDU without poll bit counter */
   int32_t          byteWoPoll;       /*!< Byte without poll bit counter */
   RlcSn            pollSn;           /*!< Poll SN */
   Buffer           *mBuf;            /*!< this can be removed later 
                                           Stores the Control Pdu */
   Bool             maxReTxReached;   /*!< Set when the DL MAX Retx exceeded */
   RlcUdxDlStaPdu*  pStaPdu;          /*!< The Control Pdu passed from UL*/
   uint8_t          snLen;           /*!< Sequence number length:12 bit or 18 bit : 5GNR RLC */
   uint32_t         snModMask;       /*!< (2 Pwr SnLen - 1): 5GNR RLC */
}RlcAmDl;

/*@}*/

/** 
 * @brief  Structure to hold information about a Radio Bearer as required by RLC
 *
 * @details
 *    - rlcId          : RLC identifier, uniquely identifies a Radio Bearer
 *    - lch            : Information (type and id) of the logical channel 
 *                       associated with this Radio  Bearer.
 *    - mode           : The mode of the Radio Bearer TM or UM or AM
 *    - dir            : The direction of the Radio Bearer, downlink or 
 *                       uplink or both
 *    - inst           : Id of RLC instance where this Radio Bearer is present. 
 *                       Used to find the instance from the Radio Bearer for 
 *                       memory needs as different instances might have 
 *                       different memory.
 *    - k1wuSapId       : KWU SAP identifier
 *    - udxSapId       : UDX SAP idenrifier
 *    - transId        : Stores the transaction identifier used to communicate 
 *                       with MAC, the same value as sent by MAC is passed back 
 *                       for it to be able to corelate
 *    - reestInProgress: Whether re-establishment is going on for this RB
 *    - m              : Mode of the RB (TM/UM/AM)
 *      - tm           : Transparent Mode information
 *      - umDl         : Unacknowledged Mode downlink information
 *      - amDl         : Acknowledged Mode downlink information
 *    - rlsLnk         : Link list used to free the RbCb
 *    - cleanupStarted : Whether cleanup of RB is in progress or not
*/
typedef struct _rlcDlRbCb
{
   uint8_t              qci;                /*!< qci of the RB */
#ifdef LTE_L2_MEAS
   RlcDlUeCb        *ueCb;              /*!< Pointer to UeCb  */
   RlcL2MeasRbCb    rbL2Cb;             /*!< RB measurement L2 Cb */
   CmLListCp       sduSnMapQ;          /*!< SDU SN map queue for UM */

   RlcL2MeasIpThruput l2MeasIpThruput;  /*!< Holds related parameter for
                                                 DL/UL ip throughput>*/
#endif                                                 

   /* Discard new changes starts */
   S16             discTmrInt;        /*!< The discard time in milli 
                                           seconds for an PDCP SDU for this RB */
   /* Discard new changes ends */
   CmLteRlcId     rlcId;           /*!< RLC Identifier */
   RlcLchInfo     lch;             /*!< Logical Channel Info  */
   Snssai         *snssai;
   CmLteRlcMode   mode;            /*!< Entity Mode */
   uint8_t        dir;             /*!< Direction for UL/DL */
   Inst           inst;            /*!< Tapa Instance where rb created */
   SpId           k1wuSapId;        /*!< KWU sap Id, to get the KwuSapCb */
   SpId           udxSapId;        /*!< UDX sap Id, to get the UdxSapCb */
   uint32_t       transId;         /*!< Transaction Id for RLC */
   Bool           reestInProgress; /*!< Is reestablishment in progress */
   SpId           rguSapId;        /*!< RGU Instance SAP corresponding to this RB */
   union          
   {
      RlcTm     tm;     /*!< TM mode specific elements */
      RlcUmDl   umDl;   /*!< UM downlink elements */
      RlcAmDl   amDl;   /*!< AM downlink elements */
   }m;   /*!< RLC mode specific Info */
   CmLList   rlsLnk;   /*!< Link to add to release list */
   Bool      cleanupStarted; /*!< Whether cleanup of RB is in progress or not */
   uint32_t  lastRprtdBoToMac; /*!< Last Reported BO to MAC Layer */
   uint32_t  boUnRprtdCnt; /*!< Count to keep track of periodic BO Update */
}RlcDlRbCb;

/** 
 * @brief  Structure to hold mapping between logical channel and Radio Bearer
 *
 * @details
 *    - dlRbCb   : Pointer to the downlink Radio Bearer
*/
typedef struct rlcDlLch
{
   RlcDlRbCb *dlRbCb;   /*!< Pointer to Downlink RbCb */
}RlcDlLch;                                  

/** 
 * @brief  Structure to hold information about the Cells
 *
 * @details
 *    - cellHlEnt : Information about cells are stored in a hash table. This is
 *                  required for that.
 *    - cellId    : Identity of the cell
 *    - rbCb      : Radio Bearers in the cell
 *    - lCh       : Logical Channels in the cell
*/
typedef struct rlcDlCellCb
{
   CmHashListEnt   cellHlEnt;                 /*!< Hash list entry for CellCb */
   CmLteCellId     cellId;                    /*!< Cell Id */
   RlcDlRbCb        *rbCb[RLC_MAX_RB_PER_CELL]; /*!< RbCbs within a Cell */
   RlcDlLch         lCh[RLC_MAX_LCH_PER_CELL];  /*!< Array of Logical channels */
}RlcDlCellCb;

#ifdef LTE_L2_MEAS
/** @struct RlcContSduLst
 * Structure contains list of rbId and index to KwOutStngSduLst
 */
typedef struct rlcContSduLst
{
   CmLteLcId   lcId;    /*!< Holds the lcId of rbCb */
   uint8_t     numSdus; /*!< Number sdus in sduIdx array */
   uint8_t     sduIdx[RLC_L2MEAS_MAX_OUTSTNGSDU];  /*!< Hold the index to KwOutStngSduLst */
}RlcContSduLst;
typedef struct rlcSduInfo
{
   Bool  isRetxPdu;
   Ticks arvlTime; /* stores the the arrival time of each PDCP PDU */
}RlcSduInfo;

typedef struct rlclchInfo
{
   CmLteLcId   lcId;    /*!< Holds the lcId of rbCb */
   uint8_t     numSdus; /*!< Number sdus belonged to the this LC */
   RlcSduInfo  sduInfo[RLC_L2MEAS_SDUIDX];
}RlclchInfo;

/** @struct RlcL2MeasTb
 * Structure containes list of rbId and  index to KwOutStngSduLst
 */
typedef struct rlcL2MeasTb
{
   uint8_t          numLcId;       /* number of logical channels in this TbCb */
   RlcContSduLst    sduInfo[RLC_MAX_ACTV_DRB]; /*!< Contained sduLst for 
                                               outstanding sdu */
   uint8_t          numLchInfo;
   RlclchInfo       lchInfo[RLC_MAX_ACTV_DRB]; /* Holds the LC info details 
                                                  used for DL delay L2 Meas*/
   uint32_t         txSegSduCnt; /*!< This holds the no.of partially 
                                     transmitted SDU per TB which can be used for UU loss Meas*/
}RlcL2MeasTb;
#endif

/** 
 * @brief  Structure to hold information about the UEs
 *
 * @details
 *    - ueHlEnt : Information about cells are stored in a hash table. This is
 *                required for that.
 *    - key     : Key to store/find the UE in the hashtable
 *    - srbCb   : Signaling Radio Bearers configured for the UE
 *    - drbCb   : Data Radio Bearers configured for the UE
 *    - lCh     : Logical Channels in the UE
*/
struct rlcDlUeCb
{
   CmHashListEnt     ueHlEnt;                   /*!< Hash list entry for UeCb */
   CmLteRnti         ueId;     /*!< UE Id */
   CmLteCellId       cellId;   /*!< Cell Id */
   RlcDlRbCb         *srbCb[RLC_MAX_SRB_PER_UE]; /*!< SRB RbCbs within a UE */ 
   RlcDlRbCb         *drbCb[RLC_MAX_DRB_PER_UE]; /*!< DRB RbCbs within a UE */ 
   RlcDlLch          lCh[RLC_MAX_LCH_PER_UE];    /*!< Array of Logical channels */
/* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
   uint16_t           numActRb[LKW_MAX_QCI];     /*!< number of RBs Active */
   uint8_t            tbIdx;                /*!< TBIDx for  measTbCb */
   RlcL2MeasTb        *l2MeasTbCb[RLC_MAX_TB_PER_UE]; /*!< Array of Measurement tbCb */
#endif /* LTE_L2_MEAS */
#ifdef TENB_STATS
   TSL2UeStatsCb   *tenbStats; /*!< UE Stats Holder */
#endif
};

/****************************************************************************
 *                      Declarations
 ***************************************************************************/
/****************************************************************************
 *                    Configuration Functions 
 ***************************************************************************/ 
S16 rlcCfgAddDlRb ARGS ((RlcCb *gCb, 
                              CmLteRnti ueId, 
                              CmLteCellId cellId, 
                              RlcEntCfgInfo *entCfg, 
                              RlcEntCfgCfmInfo *entCfm));

S16 rlcCfgReCfgDlRb ARGS ((RlcCb *gCb, 
                                 CmLteRnti ueId, 
                                 CmLteCellId cellId, 
                                 RlcEntCfgInfo *entCfg, 
                                 RlcEntCfgCfmInfo *entCfm));

S16 rlcCfgDelDlRb ARGS ((RlcCb *gCb, 
                               CmLteRnti ueId, 
                               CmLteCellId cellId, 
                               RlcEntCfgInfo *entCfg, 
                               RlcEntCfgCfmInfo *entCfm));

S16 rlcCfgDelDlUe ARGS ((RlcCb *gCb, 
                               CmLteRnti ueId, 
                               CmLteCellId cellId, 
                               RlcEntCfgInfo *entCfg, 
                               RlcEntCfgCfmInfo *entCfm));

S16 rlcCfgReEstDlRb ARGS ((RlcCb *gCb, 
                                 CmLteRnti ueId, 
                                 CmLteCellId cellId, 
                                 Bool sndReEst,
                                 RlcEntCfgInfo *entCfg, 
                                 RlcEntCfgCfmInfo *entCfm));

S16 rlcCfgDelDlCell ARGS ((RlcCb *gCb, 
                                 CmLteCellId cellId, 
                                 RlcEntCfgInfo *entCfg,
                                 RlcEntCfgCfmInfo *entCfm));

S16 rlcCfgDlUeIdChng ARGS ((RlcCb *gCb, 
                                  CkwUeInfo *ueInfo, 
                                  CkwUeInfo *newUeInfo, 
                                  CmStatus  *status));

/****************************************************************************
 *                    DBM module Functions 
 ***************************************************************************/
S16 rlcDbmDlInit ARGS ((RlcCb *gCb));

Void rlcDbmDlDeInit ARGS ((RlcCb *gCb));

S16 rlcDbmCreateDlUeCb ARGS ((RlcCb *gCb,     
                                    CmLteRnti ueId,   
                                    CmLteCellId cellId,
                                    RlcDlUeCb **ueCb));

uint8_t rlcDbmFetchDlUeCb ARGS ((RlcCb *gCb,        
                                   CmLteRnti ueId,      
                                   CmLteCellId cellId,     
                                   RlcDlUeCb **ueCb));

Void rlcDbmDelDlUeCb ARGS ((RlcCb *gCb,       
                                  RlcDlUeCb *ueCb, 
                                  Bool abortFlag));

Void rlcDbmDelAllDlUe ARGS ((RlcCb *gCb));

S16 rlcDbmCreateDlCellCb ARGS ((RlcCb *gCb,      
                                    CmLteCellId cellId,   
                                    RlcDlCellCb **cellCb));

uint8_t rlcDbmFetchDlCellCb ARGS ((RlcCb *gCb, 
                                     CmLteCellId cellId, 
                                     RlcDlCellCb **cellCb));

Void rlcDbmDelDlCellCb ARGS ((RlcCb *gCb, RlcDlCellCb *cellCb));

Void rlcDbmDelAllDlCell ARGS ((RlcCb *gCb));

Void rlcDbmFetchDlRbCbByRbId ARGS ((RlcCb *gCb, 
                                          CmLteRlcId *rlcId, 
                                          RlcDlRbCb **rbCb));

Void rlcDbmFetchDlRbCbFromLchId ARGS ((RlcCb *gCb,       
                                             CmLteRnti ueId,     
                                             CmLteCellId cellId,    
                                             CmLteLcId lcId,      
                                             RlcDlRbCb **rbCb));

Void rlcDbmDelAllDlRb ARGS ((RlcCb *gCb, RlcDlRbCb **rbCbLst, uint8_t numRbCb));

S16 rlcDbmDlShutdown ARGS ((RlcCb *gCb));

Void rlcUtlGetCurrTime ARGS((uint32_t *time));
Void rlcUtlTrigPdbFlowCntrl ARGS((RlcCb *gCb, RlcDlRbCb *rbCb, uint32_t pktAdmitCnt ));
#ifdef LTE_L2_MEAS
Void rlcDbmDelAllDlL2MeasTbFrmUe ARGS ((RlcCb *gCb, RlcDlUeCb *ueCb));

#endif

/****************************************************************************
 *                    Transparent Mode Functions 
 ***************************************************************************/
Void rlcTmmQSdu ARGS ((RlcCb *gCb,
                            RlcDlRbCb *rbCb,
                            RlcDatReqInfo *datReqInfo,
                            Buffer *mBuf));
Void rlcTmmSendToMac ARGS ((RlcCb     *gCb,
                                SuId     suId,
                                RlcDlRbCb *rbCb, 
                                RguCStaIndInfo *staInd));

Void rlcDlTmmReEstablish ARGS ((RlcCb *gCb, RlcDlRbCb *rbCb));

/****************************************************************************
 *                    Unacknowledged Mode Functions 
 ***************************************************************************/
Void rlcUmmQSdu ARGS ((RlcCb *gCb, 
                             RlcDlRbCb *rbCb,
                             RlcDatReqInfo *datReq, 
                             Buffer *mBuf));

Void rlcUmmDiscSdu ARGS ((RlcCb *gCb, RlcDlRbCb *rbCb, uint32_t sduId));

Void rlcDlUmmReEstablish ARGS ((RlcCb *gCb, 
                                      CmLteRlcId rlcId, 
                                      Bool sndReEst,
                                      RlcDlRbCb *rbCb));

Void rlcUmmProcessSdus ARGS ((RlcCb *gCb,RlcDlRbCb *rbCb,RlcDatReq *datReq));

Void rlcUmmFreeDlRbCb ARGS ((RlcCb *gCb, RlcDlRbCb *rbCb));

/****************************************************************************
 *                    Acknowledged Mode Functions 
 ***************************************************************************/

S32 rlcAmmCalculateBo ARGS ((RlcAmDl *amDl));

Void rlcAmmSendDedLcBoStatus ARGS ((RlcCb *gCb, RlcDlRbCb *rbCb, RlcAmDl *amDl));

Void rlcAmmQSdu ARGS((RlcCb *gCb,
                            RlcDlRbCb *rbCb,
                            Buffer *mBuf, 
                            RlcDatReqInfo *datReq));

void rlcAmmProcessSdus ARGS((RlcCb *gCb, 
                                   RlcDlRbCb *rbCb, 
                                   RlcDatReq *rlcDatReq,
                                   bool staPduPres));

Void rlcAmmDlReEstablish ARGS((RlcCb *gCb, 
                                     CmLteRlcId rlcId, 
                                     RlcDlRbCb *rbCb));

Void rlcAmmDlHndlStatusPdu ARGS ((RlcCb  *gCb,
                                        RlcDlRbCb  *rbCb,
                                        RlcUdxStaPdu *pStaPdu));

S16 rlcAmmDiscSdu ARGS((RlcCb *gCb, RlcDlRbCb *rbCb, uint32_t sduId));

Void rlcAmmPollRetxTmrExp ARGS((RlcCb *gCB, RlcDlRbCb *rbCb));

Void rlcAmmFreeDlRbCb ARGS ((RlcCb *gCb, RlcDlRbCb *rbCb));

/****************************************************************************
 *                    Utility Functions 
 ***************************************************************************/

Void rlcUtlStoreTxBuf ARGS ((CmLListCp        *txBufLst,
                                    RlcTx       *txBuf,
                                    RlcSn              sn
                                   ));
RlcTx* rlcUtlGetTxBuf ARGS ((CmLListCp        *txBufLst,
                                  RlcSn              sn
                                  ));
Void rlcUtlDelTxBuf ARGS ((CmLListCp        *txBufLst,
                                  RlcTx            *txBuf,
                                  RlcCb              *gCb
                                  ));
Void rlcUtlRemovTxBuf ARGS ((CmLListCp      *txBufLst,
                                  RlcTx            *txBuf,
                                  RlcCb              *gCb
                                  ));

uint8_t rlcUtlSendDedLcBoStatus ARGS ((RlcCb *gCb,
                                  RlcDlRbCb *rbCb,
                                  int32_t bo,
				  int32_t estHdrSz,
                                  bool staPduPrsnt,
                                  uint32_t staPduBo));

#ifdef LTE_L2_MEAS_RLC
Void rlcUtlEmptySduQ ARGS ((RlcCb *gCb, RlcDlRbCb *rbCb, CmLListCp *sduQ));
#else /* LTE_L2_MEAS */
Void rlcUtlEmptySduQ ARGS ((RlcCb *gCb,CmLListCp *sduQ));
#endif /* LTE_L2_MEAS */

uint8_t rlcUtlSendToMac ARGS ((RlcCb *gCb, SuId suId, KwDStaIndInfo *staIndInfo));

Void rlcUtlIncrementKwuStsSduTx ARGS((RlcKwuSapCb *rlckwuSap));

Void rlcUtlIncrementGenStsBytesAndPdusSent ARGS((RlcGenSts *genSts, 
                                                       Buffer *pdu));

Void rlcUtlFreeDlMemory ARGS ((RlcCb *gCb));

Void rlcUtlInitToBeFreed ARGS ((RlcCb *gCb, RlcDlDataToBeFreed *toBeFreed));

Void rlcUtlInitializeSelfPst ARGS((RlcCb *gCb));

Void rlcUtlRaiseDlCleanupEvent ARGS((RlcCb *gCb));

Void rlcUtlAddSduToBeFreedQueue ARGS((RlcCb *gCb, RlcSdu *sdu));

Void rlcUtlAddReTxPduToBeFreedQueue ARGS((RlcCb *gCb, RlcRetx *retx));

Void rlcUtlAddTxPduToBeFreedQueue ARGS((RlcCb *gCb, RlcTx *pdu));

#ifdef LTE_L2_MEAS
S16 rlcUtlL2MeasDlInit ARGS((RlcCb *gCb));
#endif

/****************************************************************************
 *                    Debug Functions 
 ***************************************************************************/
Void ResetRLCStats ARGS((Void));

Void PrintRLCStats ARGS((Void));

Void DumpRLCDlDebugInformation ARGS((Void));

/****************************************************************************
 *                    Activation Functions 
 ***************************************************************************/
S16 rlcDlActvInit ARGS ((Ent ent,Inst inst,Region region,Reason reason)); 

S16 rlcDlActvTsk ARGS ((Pst *pst, Buffer *mBuf));

bool rlcDlUtlIsReestInProgress ARGS ((RlcDlRbCb *rbCb));

Void rlcDlUtlResetReestInProgress ARGS ((RlcDlRbCb *rbCb));

Void rlcDlUtlResetReestInProgress ARGS (( RlcDlRbCb *rbCb));

Void rlcDlUtlSetReestInProgressForAllRBs ARGS ((RlcCb *gCb, RlcDlUeCb
         *ueCb));
Void rlcDlUtlSetReestInProgressForRB ARGS (( RlcCb *gCb, RlcDlRbCb *rbCb));

#ifdef LTE_L2_MEAS
Void rlcUtlUpdateContainedSduLst ARGS ((
uint8_t  sduIdx, 
RlcContSduLst  *contSduLst
));
Void rlcUtlUpdateOutStandingSduLst ARGS ((
RlcL2MeasDlIpTh   *dlIpThPut, 
uint8_t           sduIdx, 
MsgLen            sduLen, 
uint32_t          sduId,
Bool              newIdx
));
Void rlcUtlUpdateBurstSdus ARGS((
RlcCb            *gCb,         
RlcDlRbCb        *rbCb,
RlcContSduLst    *contSduLst,
S32              dataVol,
uint32_t         schPduSz
));

RlcL2MeasTb * rlcUtlGetCurMeasTb ARGS((
RlcCb     *gCb,
RlcDlRbCb *rbCb
));

S16 rlcUtlSndDlL2MeasNCfm ARGS((RlcCb *gCb,
                                      RlcL2MeasReqEvt *measReqEvt,
                                      RlcL2MeasCfmEvt *measCfmEvt));

S16 rlcUtlSndDlL2MeasCfm  ARGS ((RlcCb *gCb, RlcL2MeasEvtCb *measEvtCb));

S16 rlcUtlProcHarqInd  ARGS (( RlcCb *gCb, RguHarqStatusInd *staInd, RlcDlUeCb *ueCb, 
                                     uint8_t tbIdx));
Void rlcUtlResetDlL2MeasInRlcRb ARGS ((RlcCb *gCb,
                                            RlcL2MeasCb *measCb,
                                            uint8_t measType));

S16 RlcMiLkwL2MeasStopCfm ARGS((Pst *pst, uint8_t  measType,uint8_t  status));

S16 rlcUtlValidateIpThL2Meas ARGS((RlcL2MeasReqEvt *measReqEvt, 
                                     RlcL2MeasCfmEvt *measCfmEvt)); 
                                    
S16 rlcFetchLchInfo ARGS ((RlcL2MeasReqEvt *measReqEvt,
                                 RlcL2MeasCfmEvt *measCfmEvt,
                                 uint16_t             ueId,
                                 CmLteLcId *lChId,
                                 uint8_t *numLch));

S16 rlcUtlNotifyMacUlIp ARGS ((RlcL2MeasCb *measCb,uint16_t ueIdx, Bool enable,
                                    CmLteLcId  *lChId, uint8_t *numLCh));
#endif

uint8_t rlcProcDlData(Pst *pst, RlcDatReqInfo* datReq, Buffer *buf);
uint8_t rlcProcCommLcSchedRpt(Pst *pst, SuId suId, RguCStaIndInfo  *staInd);
uint8_t rlcProcDedLcSchedRpt(Pst *pst, SuId suId, RguDStaIndInfo  *staInd);

Void rlcUtlFreeDlMem ARGS(( Void));
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __RLC_DLH__ */
  
/********************************************************************30**
  
         End of file
**********************************************************************/
