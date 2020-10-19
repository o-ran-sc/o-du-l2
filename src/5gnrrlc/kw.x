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
  
        Name:    LTE RLC file 
    
        Type:    C include file
  
        Desc:    This file contains all the data structures and 
                 prototypes for LTE RLC.
 
        File:    kw.x

*********************************************************************21*/
/** @file kw.x
@brief RLC Product Structures, prototypes
*/

#ifndef __KWX__
#define __KWX__
 
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/** @brief Local typedefs */
typedef uint32_t    RlcSn;   /*!< Sequence Number length */

typedef RguDDatIndInfo KwDatIndInfo;

typedef RguDStaIndInfo KwDStaIndInfo;

typedef RguPduInfo KwPduInfo; /* kw002.201 : Aligning the structure with RGU */

typedef struct _amRlcStats
{
   uint32_t   numDLStaPduSent;
   uint32_t   numDLNacksInStaPdu;
   uint32_t   numDLBytesUnused;
   uint32_t   numDLPollTimerExpiresSrb;
   uint32_t   numDLPollTimerExpiresDrb;
   uint32_t   numDLMaxRetx;
   uint32_t   numDLRetransPdus;
   uint32_t   numULPdusDiscarded;
   uint32_t   numULReOrdTimerExpires;
   uint32_t   numULStaPduRcvd;
   uint32_t   numULNackInStaPduRcvd;
   uint32_t   numRlcAmCellSduTx; /* Count of SDUs transmitted in DL for all UEs */
   uint32_t   numRlcAmCellSduBytesTx; /*Total number of bytes transmitted in DL for all Ues */
   uint32_t   numRlcAmCellRetxPdu; /*Count of PDUs retransmitted for all Ues */
   uint32_t   numRlcAmMaxRetx; /*Total number of Max-RLC retransmissions hit for all the Ues */
   uint32_t   numRlcAmCellDupPduRx; /*Count of Duplicate PDUs detected for a UE in UL for all Ues */
   uint32_t   numRlcAmCellDropOutWinRx; /*Count of PDUs dropped due to Out of Window reception for all Ues */
   uint32_t   numRlcAmCellSduRx; /* Count of SDUs received in UL for all UEs*/
   uint32_t   numRlcAmCellSduBytesRx;/*Total number of bytes received in UL for all Ues*/
   uint32_t   numRlcAmCellNackRx; /*Total number of UL PDUs nacked for all the Ues*/
   uint32_t   numRlcAmCellWinStall; /*Number of window stalls detected for all the Ues */
}AMRLCStats;

typedef struct _umRlcStats
{
   uint32_t   numDLBytesUnused;
   uint32_t   numDLMaxRetx;
   uint32_t   numULPdusDiscarded;
   uint32_t   numULReOrdTimerExpires;
   uint32_t   numULPdusOutsideWindow;
}UMRLCStats;

typedef struct _rlcStats
{
   AMRLCStats   amRlcStats;
   UMRLCStats   umRlcStats;
}RLCStats;

EXTERN RLCStats gRlcStats;

/* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
typedef struct rlcSduSnMap RlcSduSnMap;
typedef RguLchMapInfo KwLchMapInfo;
#endif /*  LTE_L2_MEAS */


/** @defgroup ummode UM Module Info 
*/
/** 
 * @brief  Structure to hold an Unacknowledged Mode header
 *
 * @details
 *    - fi    : Framing Info
 *    - sn    : Sequence number
 *    - numLi : Number of length indicators in the following array (li)
 *    - li    : Length indicators
*/
typedef struct rlcUmHdr
{
   uint8_t     fi;              /*!< Framing Info */
   RlcSn   sn;              /*!< Sequence number */
   uint16_t    numLi;           /*!< Number of LIs */
   uint16_t    li[RLC_MAX_UL_LI];   /*!< Array of LIs */
}RlcUmHdr;

/** 
 * @brief  Structure to hold an Acknowledged Mode header
 *
 * @details
 *    - dc    : Data/Control PDU
 *    - rf    : Resegmentation flag
 *    - p     : Poll bit
 *    - fi    : Framing Info
 *    - e     : Extension bit
 *    - lsf   : Last segment flat
 *    - sn    : Sequence number
 *    - so    : Segment offset
 *    - numLi : Number of length indicators in the following array (li)
 *    - li    : Length indicators
*/
typedef struct rlcAmHdr
{
   uint8_t     dc;              /*!< Data/Control PDU */
   uint8_t     p;               /*!< Poll bit */
   uint8_t     si;              /*!< Segmentation Info: 5GNR */ 
   RlcSn   sn;              /*!< Sequence number */
   uint32_t    so;              /*!< Segment offset */
}RlcAmHdr;

/* structures used for encoding/decoding the headers */
typedef struct rlcCntrlInfo
{
   uint16_t  val;
   uint8_t   len;
   uint16_t  idx;
   uint8_t   emtBits;
   uint16_t  e1Idx;
   uint16_t  e2Idx;   
   uint8_t   e1eb;
}RlcCntrlInfo;

typedef struct rlcHdrInfo
{
   uint32_t  val;
   uint8_t   len;
   uint8_t   eb;
   uint8_t   *hdr;
   uint16_t  idx;
   uint8_t   pEb;
   uint8_t   pLen;
}RlcHdrInfo;

typedef struct rlcExtHdr
{
   uint32_t val;
   uint16_t len;
   uint8_t  hdr;
   uint8_t  pLen;
}RlcExtHdr;

/** 
 * @brief  Structure to hold information about a Logical channel
 *
 * @details
 *    - lChId    : Logical channel Id
 *    - lChType  : Logical channel type 
*/ 
typedef struct rlcLchInfo
{
   CmLteLcId     lChId;     /*!< Logical channel Id */
   CmLteLcType   lChType;   /*!< Logical channel type */
}RlcLchInfo;

/* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS

/** @struct RlcL2Cntr
 * RLC L2 Counter  */
typedef struct rlcL2Cntr
{
   struct
   {
      uint32_t  numActvUe;        /*!< number of active Ue */
      uint32_t  sampOc;           /*!< Total number of sampling occasion */
   }actUe;
   struct
   {
      uint32_t  dLoss;            /*!< Total number of lost packets */  
      uint32_t  posPkts;          /*!< Total number of positively acknowlegded 
                                  packets */
   }uuLoss;
   struct                    /*!< For DL IP throughput */
   {
      uint32_t volSummation;      /*!< Sum of data in bytes */
      uint32_t timeSummation;     /*!< Sum of time difference in milli sec*/
   }dlIpThruput;
   struct                    /*!< For UL IP throughput */
   {
      uint32_t volSummation;      /*!< Sum of data in bytes */
      uint32_t timeSummation;     /*!< Sum of time difference in milli sec*/
   }ulIpThruput;
   /* Discard new changes starts */
   struct                    /*!< For UL IP throughput */
   {
      uint32_t discSdus;          /*!< Total RLC SDUs discarded */
      uint32_t totSdus;           /*!< Total RLC SDUs received */
   }dlDisc;
   struct                    /*!< For UL IP throughput */
   {
      uint64_t sduDelay;          /*!< Total SDUs delay */
      uint32_t numSdus;
   }dlPjSduDelay;
   uint32_t    totDrbsPerQci;     /*!< Total Count of DRB's for this QCI */
}RlcL2Cntr;

struct rlcSduSnMap
{
   CmLList     lstEnt;
   Bool        failMarked;
   Bool        fullySent;
   uint32_t         sduId;
   uint16_t         numSn;
   uint16_t         snList[RLC_MAX_PDU_MAP];
   uint16_t         harqAck;
   uint16_t         reqSent;
   uint16_t         rspRcvd;
};

typedef struct rlcSnSduMap
{
   uint16_t         sn;
   CmLteLcId   lChId;              /*!< Logical channel Id */
   uint16_t         numSdu;
#ifdef LTE_RLC_R9
   Bool        isBurstSplitted;    /*!< true: burst for this LCH is splitted */
#endif /* LTE_RLC_R9 */
   RlcSduSnMap  *sduList[RLC_MAX_DL_LI];
}RlcSnSduMap;

typedef struct rlcTbSnMap
{
   CmHashListEnt  hlTbEnt;
   uint32_t            tbId;
   uint16_t            prevNumSn;
   uint16_t            numSn;
   RlcSnSduMap     snSduMap[RGU_MAX_PDU * RGU_MAX_LC];
}RlcTbSnMap;

typedef struct rlcL2MeasCbUeMeasInfo
{
   CmLteRnti   ueId;                    /*!< UE ID (Used only for IP Throughput
                                             in UL/DL */
   CmLteCellId cellId;                  /*!< UE ID (Used only for IP Throughput
                                             in UL/DL */
   Bool        isValid;                 /*! < is this UE entry valid */
   uint8_t          numLcId;                 /*!< Holds the number of LCh for which Ul Ip
                                             measurement is ON */
   uint8_t          lcId[RLC_MAX_LCH_PER_UE]; /*!< Holds the list of LCh for which Ul ip
                                             measurement is ON */
   RlcL2Cntr    measData[LKW_MAX_QCI];
   uint16_t         numQci;                  /*!< number of valid qcI */
   uint8_t          qci[LKW_MAX_QCI];        /*!< list of valid qcI */
}RlcL2MeasCbUeMeasInfo;

typedef struct rlcL2MeasCbIpThMeas
{
   uint8_t                   numUes;
   uint8_t                   totNumQci;
   uint8_t                   totQci[LKW_MAX_QCI];
   RlcL2MeasCbUeMeasInfo ueInfoLst[LKW_MAX_UE]; /*Added for handling meas for multiple ues*/ 
}RlcL2MeasCbIpThMeas;

typedef struct rlcL2MeasCbNonIpThMeas
{
   uint16_t        numSamples;              /*!< Number of samples to take on numActUe */
   uint16_t        numQci;                  /*!< number of valid qcI */
   uint8_t         qci[LKW_MAX_QCI];        /*!< list of valid qcI */
   RlcL2Cntr   measData[LKW_MAX_QCI];   /*!< Measurement CB */
}RlcL2MeasCbNonIpThMeas;

typedef union rlcL2MeasCbIpNonIpThMeasVal
{
   RlcL2MeasCbIpThMeas    ipThMeas;
   RlcL2MeasCbNonIpThMeas nonIpThMeas;
}RlcL2MeasCbIpNonIpThMeasVal;

/** @struct RlcL2MeasCb
 * RLC L2 Measurement CB */
typedef struct rlcL2MeasCb
{
   uint8_t               measType;        /*!< Bit-wise set measurement types */
   RlcL2MeasCbIpNonIpThMeasVal val;   /* Union of IP tpt or non-ip tpt */
}RlcL2MeasCb;

/** @struct RlcL2MeasEvtCb
 * RLC L2 Measurement Evt CB */
typedef struct rlcL2MeasEvtCb
{
   uint32_t           transId;                /*!< TransId of Measurement Req */
   uint32_t           cbIdx;                  /*!< TransId of Measurement Req */
   CmTimer       l2Tmr; /* NOT USED */                 /*!< L2 Timer per request */
   TmrCfg        l2TmrCfg; /* NOT USED */               /*!< Time period of measurement */
   RlcL2MeasCb    measCb;                 /*!< Measurement CB */ 
   EpcTime       startTime; /* NOT USED */            /*!<  start time when meas starts*/ 
}RlcL2MeasEvtCb;

/** @struct RlcL2MeasRbCb
 * RLC L2 Measurement Rb CB */
typedef struct rlcL2MeasRbCb
{
   uint8_t            measOn;                      /*!< Measurements that are running */ 
   RlcL2Cntr      *l2Sts[RLC_MAX_L2MEAS_EVT];  /*!< L2 Mesurement statistics */     
}RlcL2MeasRbCb;

/** @struct RlcL2Cb
 * RLC L2  CB */
typedef struct rlcL2Cb
{
   uint16_t            rlcNumMeas;                   /*!< Number of measurements going on */
   RlcL2MeasEvtCb  rlcL2EvtCb[LKW_MAX_L2MEAS];  /*!< Pointers to Measurement Cb */
   uint8_t             measOn[LKW_MAX_QCI];          /*!< Measurement on */
   uint32_t            numActUe[LKW_MAX_QCI];       /*!< Measurement on */
}RlcL2Cb;


typedef enum _dlIpThrputState
{
   KW_DL_IPTHRU_RESET = 0,
   KW_DL_IPTHRU_BURST_STARTED,
   KW_DL_IPTHRU_BURST_CONTINUE,
   KW_DL_IPTHRU_BURST_COMPLETED
}DlIpThrputState;

/** 
* @struct rlcL2MeasSduLst
* Structure to hold parameters of 
* burst sdus in DL for a RB */
typedef struct rlcOutStngSduInfo
{
   uint32_t       sduId;            /*!< SDU Id of sdu */
   MsgLen    sduLen;           /*!< Size of sdu */
   uint32_t       numTb;            /*!< Hold the number of TBs for this sdu in DL */
}RlcOutStngSduInfo;

/** 
* @struct rlcL2MeasDlIpTh
* Structure to hold parameters for DL ip 
* throughput for a RB */
typedef struct rlcL2MeasDlIpTh
{
   Bool                 isBurstAct;            /*!< Set to TRUE when burst is active in DL */
   uint64_t             burstStartTime;        /*!< Holds the starting time of the burst */
   uint32_t             burstEndSduId;         /*!< Sdu ID when burst ends */
   uint8_t              lastSduIdx;            /*!< Holds the index of last outStanding sdu */
   RlcOutStngSduInfo  outStngSduArr[RLC_L2MEAS_MAX_OUTSTNGSDU];/*!< Hold the burst sdu information */
}RlcL2MeasDlIpTh;

/** 
* @struct rlcL2MeasIpThruput
* Structure to hold parameters for UL/DL ip 
* throughput for a RB */
typedef struct rlcL2MeasIpThruput
{
   uint32_t             dataVol;                 /*!< Holds volume of new data in bytes
                                              for UL IP throughput */
   uint32_t             ttiCnt;                  /*!< Holds ttiCnt received from MAC in UL */
   uint32_t             prevTtiCnt;        /*!< Holds previous ttiCnt received from MAC in UL */
   RlcL2MeasDlIpTh  dlIpTh;
}RlcL2MeasIpThruput;

#endif /* LTE_L2_MEAS */

/** 
 * @brief  Structure to hold an UE key for the UE hast lists
 *
 * @details
 *    - ueId    : UE Id
 *    - cellId  : Cell Id 
*/
typedef struct rlcUeKey
{
   CmLteRnti     ueId;     /*!< UE Id */
   CmLteCellId   cellId;   /*!< Cell Id */
}RlcUeKey;

/** 
 * @brief  Structure to hold an information about the CKW SAP
 *
 * @details
 *    - pst   : Service user post structure
 *    - spId  : Service provider Id
 *    - suId  : Service user Id
 *    - state : State of the SAP
 *    - sts   : SAP specific statistics 
*/
typedef struct rlcCkwSapCb
{
   Pst           pst;     /*!< Service user post structure */
   SpId          spId;    /*!< Service provider Id */
   SuId          suId;    /*!< Service user Id */
   uint8_t       state;   /*!< Sap Status */
   RlcCkwCntSts   sts;     /*!< Statistics */
}RlcCkwSapCb;

/** 
 * @brief  Structure to hold an information about the KWU SAP
 *
 * @details
 *    - pst   : Service user post structure
 *    - spId  : Service provider Id
 *    - suId  : Service user Id
 *    - state : State of the SAP
 *    - sts   : SAP specific statistics 
*/
typedef struct rlcKwuSapCb
{
   Pst           pst;     /*!< Service user post structure */
   SpId          spId;    /*!< Service provider Id */
   SuId          suId;    /*!< Service user Id */
   uint8_t            state;   /*!< Sap Status */
   RlcKwuSapSts   sts;     /*!< Statistics */
}RlcKwuSapCb;

/** 
 * @brief  Structure to hold an information about the RGU SAP
 *
 * @details
 *    - pst       : Service user post structure
 *    - spId      : Service provider Id
 *    - suId      : Service user Id
 *    - state     : State of the SAP
 *    - bndTmr    : Bind Timer
 *    - bndTmrInt : Timer Interval
 *    - retryCnt  : Bind Retry Count
*/
typedef struct rlcRguSapCb
{
   Pst       pst;         /*!< Service user post structure */
   SpId      spId;        /*!< Service provider Id */
   SuId      suId;        /*!< Service user Id */
   uint8_t        state;       /*!< Sap Status */
   CmTimer   bndTmr;      /*!< Bind Timer */
   uint16_t       bndTmrInt;   /*!< Timer Interval */
   uint8_t        retryCnt;    /*!< Bind Retry Count */
}RlcRguSapCb;

/** 
 * @brief  Structure to hold an information about the UDX UL SAP
 *
 * @details
 *    - pst       : Service user post structure
 *    - spId      : Service provider Id
 *    - suId      : Service user Id
 *    - state     : State of the SAP
 *    - bndTmr    : Bind Timer
 *    - bndTmrInt : Timer Interval
 *    - retryCnt  : Bind Retry Count
*/
typedef struct rlcUdxUlSapCb
{
   Pst       pst;         /*!< Service user post structure */
   SpId      spId;        /*!< Service provider Id */
   SuId      suId;        /*!< Service user Id */
   uint8_t        state;       /*!< Sap Status */
   CmTimer   bndTmr;      /*!< Bind Timer */
   uint16_t       bndTmrInt;   /*!< Timer Interval */
   uint8_t        retryCnt;    /*!< Bind Retry Count */
}RlcUdxUlSapCb;

/** 
 * @brief  Structure to hold an information about the UDX DL SAP
 *
 * @details
 *    - pst       : Service user post structure
 *    - spId      : Service provider Id
 *    - suId      : Service user Id
 *    - state     : State of the SAP
*/
typedef struct rlcUdxDlSapCb
{
   Pst    pst;     /*!< Service user post structure */
   SpId   spId;    /*!< Service provider Id */
   SuId   suId;    /*!< Service user Id */
   uint8_t     state;   /*!< Sap Status */
}RlcUdxDlSapCb;

/** 
 * @brief  Structure to hold info about memory to be freed
 *
 * @details
 *    - sduLst  : The SDU queues are appended to this queue, used 
 *                for the UM SDU queues
 *    - txLst   : Stores to be released AM Mode TX PDUs
 *    - reTxLst : Stores to be released AM Re TX PDU's
 *    - rbLst   : List of AM DL RBs to be freed 
*/
typedef struct rlcDlDataToBeFreed
{
   CmLListCp   sduLst;     /*!< Queue of SDU's to be freed  */
   CmLListCp   txLst;     /*!< Stores to be released TX PDUs */
   CmLListCp   reTxLst;   /*!< Stores to be released ReTX PDUs */
   CmLListCp   rbLst;     /*!< List of AM DL RBs to be freed */
}RlcDlDataToBeFreed;

/** 
 * @brief  Structure to hold an information about DL RLC instance
 *
 * @details
 *    - numKwuSaps        : Number of RLC KWU Saps
 *    - numUdxSaps        : Number of RLC UDX Saps
 *    - rlcKwuDlSap          : Pointer to the array of KWU SAPS
 *    - udxDlSap          : Pointer to the array of UDX SAPS
 *    - rguDlSap          : RGU Sap Control Block
 *    - cellLstCp         : Hashlist of CellCb
 *    - ueLstCp           : Hashlist of UeCb 
 *    - toBeFreed         : Pointer to data to be freed
 *    - shutdownReveived  : Request for shutdown recevied or not
 *    - eventInQueue      : Event for cleanup exists in queue or not
 */
typedef struct rlcDlCb
{
   uint8_t                  numKwuSaps;         /*!< Number of RLC Data Saps */
   uint8_t                  numUdxSaps;         /*!< Number of RLC Data Saps */
   RlcKwuSapCb          *rlcKwuDlSap;          /*!< KWU Sap Control Block */
   RlcUdxDlSapCb        *udxDlSap;          /*!< UDX DL Sap Control Block */
   RlcRguSapCb          *rguDlSap;          /*!< RGU Sap Control Block */
   CmHashListCp        cellLstCp;          /*!< Hashlist of CellCb */
   CmHashListCp        ueLstCp;            /*!< Hashlist of UeCb */
   RlcDlDataToBeFreed   toBeFreed;          /*!< Pointer to data to be freed */        
   Pst                 selfPst;            /*!< Pst to post events to self */
   Buffer              *selfPstMBuf;       /*!< Buffer used for self post */
   Bool                shutdownReceived;   /*!< Request for shutdown recevied */
   Bool                eventInQueue;       /*!< Event exists in queue or not */
#ifdef LTE_L2_MEAS
   RlcL2Cb              rlcL2Cb; /*!< Control Block for L2 Measurements in RLC */
#endif /* LTE_L2_MEAS */
}RlcDlCb;

/** 
 * @brief  Structure to hold an information about UL RLC instance
 *
 * @details
 *    - ckwSap       : CKW Sap Conrol Block
 *    - numKwuSaps   : Number of RLC KWU Saps
 *    - numUdxSaps   : Number of RLC UDX Saps
 *    - udxUlSap     : Pointer to the array of UDX SAPS 
 *    - rlcKwuUlSap     : Pointer to the array of KWU SAPS
 *    - rguUlSap     : RGU Sap Control Block
 *    - cellLstCp    : Hashlist of CellCb
 *    - ueLstCp      : Hashlist of UeCb 
 *    - transIdLstCp : Hashlist of cfg trans
 */
typedef struct rlcUlCb
{
   RlcCkwSapCb     ckwSap;         /*!< CKW Sap Conrol Block */ 
   uint8_t             numKwuSaps;     /*!< Number of RLC Data Saps */
   uint8_t             numUdxSaps;     /*!< Number of RLC Data Saps */
   RlcUdxUlSapCb   *udxUlSap;      /*!< UDX DL Sap Control Block */
   RlcKwuSapCb     *rlcKwuUlSap;      /*!< KWU Sap Control Block */
   RlcRguSapCb     *rguUlSap;      /*!< RGU Sap Control Block */
   CmHashListCp   cellLstCp;      /*!< Hashlist of CellCb */
   CmHashListCp   ueLstCp;        /*!< Hashlist of UeCb */
   CmHashListCp   transIdLstCp;   /*!< Hashlist of cfg trans */
/* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
   RlcL2Cb        rlcL2Cb; /*!< Control Block for L2 Measurements in RLC */
#endif /* LTE_L2_MEAS */
}RlcUlCb;


/** 
 * @brief  Structure to hold an information about a RLC instance
 *
 * @details
 *    - init    : Task Initialization Info
 *    - genCfg  : General Configuration
 *    - genSts  : General Statistics
 *    - trcLen  : Trace Length
 *    - trcMask : Trace Mask
 *    - rlcTq    : Timer queue
 *    - rlcTqCp  : Timer queue control point
 *    - u       : Union depending on whether the instance is UL or DL
 *      - ulCb  : UL instance Control Block
 *      - dlCb  : DL instance Control Block
 */
typedef struct rlcCb
{
   TskInit    init;               /*!< Task Initialization Info */
   RlcGenCfg   genCfg;             /*!< General Configuration Structure */
   RlcGenSts   genSts;             /*!< General Statistics */
   S16        trcLen;             /*!< Trace Length */
   uint8_t         trcMask;            /*!< Trace Mask */
   CmTqType   rlcTq[RLC_TMR_LEN];   /*!< Timer queue */
   CmTqCp     rlcTqCp;             /*!< Timer queue control point */
   union 
   {
      RlcUlCb   *ulCb;   /*!< Ul Control Block */
      RlcDlCb   *dlCb;   /*!< Dl Control Block */
   } u;
   uint8_t    dlSduId;   /*!< Downlink SDU ID */
}RlcCb;

EXTERN RlcCb *rlcCb[MAX_RLC_INSTANCES];   /*!< RLC global control block */

/****************************************************************************
 *                      EXTERN Declarations
 ***************************************************************************/
S16 rlcGetSId ARGS((SystemId *s));

Void rlcTmrExpiry ARGS((PTR cb, S16 tmrEvnt));

S16 rlcLmmSendTrc ARGS ((RlcCb *gCb, Event event, Buffer *mBuf));

void rlcStartTmr ARGS((RlcCb *gCb, PTR cb, S16 tmrEvnt));

void rlcStopTmr  ARGS((RlcCb *gCb, PTR cb, uint8_t tmrType));

bool rlcChkTmr ARGS((RlcCb *gCb,PTR cb, S16 tmrEvnt));

#ifdef LTE_L2_MEAS
Void rlcLmmSendAlarm ARGS (( RlcCb *gCb,
                                   uint16_t category, 
                                   uint16_t event, 
                                   uint16_t cause, 
                                   SuId suId, 
                                   uint32_t ueId, 
                                   uint8_t qci));

S16 RlcMiRlcDlL2MeasReq ARGS (( Pst *pst, RlcL2MeasReqEvt *measReqEvt ));
S16 RlcMiRlcDlL2MeasSendReq ARGS((Pst *pst,uint8_t measType));
S16 RlcMiRlcDlL2MeasStopReq ARGS((Pst *pst,uint8_t measType));
S16 RlcMiRlcUlL2MeasReq ARGS (( Pst *pst, RlcL2MeasReqEvt *measReqEvt ));
S16 RlcMiRlcUlL2MeasSendReq ARGS((Pst *pst,uint8_t measType));
S16 RlcMiRlcUlL2MeasStopReq ARGS((Pst *pst,uint8_t measType));
Void rlcUtlPlcMeasDatInL2Sts ARGS((RlcL2Cntr *measData, 
                                         RlcL2MeasRbCb *rbL2Cb,
                                         uint8_t measType));
#else /* LTE_L2_MEAS */
Void rlcLmmSendAlarm ARGS ((RlcCb *gCb,
                                  uint16_t category, 
                                  uint16_t event, 
                                  uint16_t cause, 
                                  SuId suId, 
                                  uint32_t ueId));
#endif /* LTE_L2_MEAS */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __KWX__ */

  
/********************************************************************30**
  
         End of file
**********************************************************************/
