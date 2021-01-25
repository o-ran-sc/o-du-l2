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
 
    Name:   LTE MAC layer
 
    Type:   X include file
 
    Desc:   Defines required by LTE MAC
 
    File:  rg.x 
 
**********************************************************************/
 
/** @file rg.x
@brief This file contains basic data structures for the Mac.
*/
 
#ifndef __RGX__
#define __RGX__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifdef EMTC_ENABLE
#include "rg_emtc.x"            /* typedefs for MAC */
#endif

/** @brief Logical Channel Id */
typedef RgPrgLteLcgId      LteLcgId;        
typedef RguDDatReqInfo     RgRguDedDatReq;
typedef RguCDatReqInfo     RgRguCmnDatReq;
typedef RguDStaRspInfo     RgRguDedStaRsp;
typedef RguCStaRspInfo     RgRguCmnStaRsp;
typedef RguDStaIndInfo     RgRguDedStaInd;
typedef RguCStaIndInfo     RgRguCmnStaInd;
typedef RguDDatIndInfo     RgRguDedDatInd;
typedef RguCDatIndInfo     RgRguCmnDatInd;
typedef RguLchStaInd       RgRguLcStaInd;
typedef TfuPdschDciInfo    RgTfuPdschDciInfo;
typedef TfuDatReqInfo      RgTfuDatReqInfo;
typedef TfuDatReqPduInfo   RgTfuDatReqPduInfo;
typedef RguDDatReqPerUe    RgRguDDatReqPerUe;
#ifdef L2_OPTMZ
typedef TfuDatReqTbInfo    RgTfuDatReqTbInfo;
#endif

#ifdef LTE_L2_MEAS
typedef struct rgL2MeasCb     RgL2MeasCb;
typedef struct rgQciCb        RgQciCb;

typedef RguL2MUlThrpMeasReqInfo RgRguL2MUlThrpMeasReq;

#endif /* LTE_L2_MEAS */
/* Forward declarations for some structures */
typedef struct rgUeCb         RgUeCb;
typedef struct rgErrInfo      RgErrInfo;
typedef struct rgCellCb       RgCellCb;
typedef struct rgUlHqProcCb   RgUlHqProcCb;
typedef struct rgDlHqProcCb   RgDlHqProcCb;
typedef struct rgLcgCb        RgLcgCb;
typedef struct rgDlHqEnt      RgDlHqEnt;
typedef struct _rgCb          RgCb;
typedef RgPrgUlLcInfo         RgUlLcCb;
typedef RgPrgDlLcInfo         RgDlLcCb;


#ifdef LTE_L2_MEAS
/* @brief UE Specific Uplink allocation info needed for measurements */
typedef struct rgUeUlAlloc
{
   CmLteRnti        rnti;      /*!< Ue Id */ 
   uint8_t          numPrb;    /*!< Number of total PRB's allocated for this UE */
} RgUeUlAlloc;
/**
 * @brief Structure hold uplink allocation information for sub frames.
 * */
typedef struct rgUlSf
{
   CmLteTimingInfo schdTime;       /*!< scheduled frame and subframe number */
   uint8_t         numUe;          /*!< Number of UE's allocated in this uplink  subframe */
   RgUeUlAlloc     *ueUlAllocInfo; /*!< Uplink alloc infor for this subframe */
} RgUlSf;

/** @brief
 *  L2 measurement control block.
 *  */
struct rgL2MeasCb
{
   CmLList            measLnk;        /*!< MeasCb node */
   RgInfL2MeasReq     measReq;        /*!< Request received for measurement */
   CmLteTimingInfo    startTime;      /*!< Time at which measurement started */
   uint32_t           sfnCycle;       /*!< Count of Num of SFN wraps */ 
};
/** @brief
 *  Control block to store Logical Channels with Qci.
 *  */
struct rgQciCb
{
   uint8_t          qci;      /*< QCI for measurement */
   uint32_t         prbCount; /*!< Cummulative PRB count */
   uint8_t          mask;     /*!< Measurement Active or Not*/
   CmLteTimingInfo  startTime;/*!< Time at which measurement started */
};
#endif /* LTE_L2_MEAS */
/**
  * @brief
  * Information about one PDCCH.
  */
typedef struct rgPdcch {
   CmLteRnti         rnti;    /*!< RNTI to who the PDCCH is allocated */
   RgTfuPdschDciInfo dci;     /*!< PDCCH format */
} RgPdcch;

typedef struct rgBcchTb
{
   RgPdcch        pdcch;
   Buffer         *tb;
   uint16_t       txPwrOffset; /*!< PDSCH Tx Pwr offset */
} RgBcchTb;

typedef struct rgPcchTb
{
   RgPdcch        pdcch;
   Buffer         *tb;
   uint16_t       txPwrOffset; /*!< PDSCH Tx Pwr offset */
} RgPcchTb;

typedef struct rgRaRspAlloc
{
   RgPdcch        pdcch;     /*!< NULLP if no Rsp allocation done for raRnti*/
   Buffer         *rar;
   uint16_t       txPwrOffset; /*!< PDSCH Tx Pwr offset */
}RgRaRspAlloc;

typedef struct rgBchTb
{
   Buffer         *tb;    /*!< BCH data for this frame */
}RgBchTb;


typedef struct rgDlSf
{
   CmLteTimingInfo schdTime;  /*!< scheduled frame and subframe number */
   Bool            txDone;     /*!< Flag to indicate transmission done */
/* ADD Changes for Downlink UE Timing Optimization */
#ifdef LTEMAC_DLUE_TMGOPTMZ  
   uint8_t         remDatReqCnt; /*!< Counter to maintain count of Ded data received.*/  
   /* Fix [ccpu00126310]: Tracks Data Requests from RLC for both loosely and tight coupled 
      RLC-MAC */
   Bool            statIndDone; /*!< Marks sending of all status indication done */
#endif
   RgBchTb         bch;        /*!< BCH data for this frame */
   RgBcchTb        bcch;       /*!< BCCH allocation for this frame */
#ifdef EMTC_ENABLE
   RgEmtcBcchTb    emtcBcch;   /*!< EMTC BCCH allocation for this frame */
#endif
   RgPcchTb        pcch;       /*!< PCCH allocation for this frame */
   CmLListCp       tbs;        /*!< TBs or DL HARQ processes */
   uint8_t         numRaRsp;   /*!< No.of RaRsp */
   RgRaRspAlloc    raRsp[RG_MAX_RA_RNTI]; /* Array of list of Ra Rsp 
                                       Allocations for each rarnti */
   uint8_t         numTbReqsSent;        /*!< Number of TB requests sent towards SMAC.
   Total numTbReqsSent will be equal to maximum secondary cells */                               
} RgDlSf;

/** 
  * @brief
  * Logical channel control block for BCCH and PCCH channels. 
  */
typedef struct rgBcchDlschLcCb
{
   CmLteLcId       lcId;                     /*!< Logical channel ID */
   Buffer          *tb;
} RgBcchDlschLcCb;

typedef struct rgBcchBchLcCb
{
   CmLteLcId       lcId;                     /*!< Logical channel ID */
} RgBcchBchLcCb;

typedef struct rgPcchLcCb
{
   CmLteLcId       lcId;                     /*!< Logical channel ID */
} RgPcchLcCb;

/** 
  * @brief
  * Random access information per cell. 
  */
typedef struct rgRaInfoCb
{
   CmLListCp       ueRachLst;                   /*!< List of RaCbs */
} RgRaInfoCb;

typedef struct rgSchInstMap
{
   S16          cellSapId; /*!< SAP ID of the cell in scheduler instance */
   Inst          schInst;  /*!< Scheduler instance that is serving this cell*/
} RgSchInstMap;

/** 
  * @brief
  * Configuration Information for Upper SAPs at RGU,CRG and RGR interfaces.
  */
typedef struct rgUpSapCfgInfo
{
   Pst  sapPst;              /*!< Post information associated with SAP */
   SpId spId;                /*!< SpId associated with SAP */
   SuId suId;                /*!< SuId associated with SAP */
}RgUpSapCfgInfo;

/** 
  * @brief
  * Configuration Information for Lower SAP at TFU interface
  */
typedef struct rgLowSapCfgInfo
{
   Pst    sapPst;            /*!< Post information associated with SAP */
   SpId   spId;              /*!< SpId associated with SAP */
   SuId   suId;              /*!< SuId associated with SAP */
   TmrCfg bndTmr;            /*!< Bind Timer Value */
}RgLowSapCfgInfo;

/** 
  * @brief
  * Control Block structure for Upper SAPs at RGU,CRG and RGR interfaces.
  */
typedef struct rgUpSapCb
{
   RgUpSapCfgInfo sapCfg;    /*!< Configuration information */
   RgSapSts     sapSts;      /*!< Statistics related to SAP */
   RgSapSta     sapSta;      /*!< SAP Status */
}RgUpSapCb;

/** 
  * @brief
  * Control Block structure for Lower SAP at TFU interface.
  */
typedef struct rgLowSapCb
{
   RgLowSapCfgInfo sapCfg;          /*!< SAP configuration information */
   RgSapSts        sapSts;          /*!< Statistics related to SAP */
   RgSapSta        sapSta;          /*!< SAP Status */
   uint8_t         numBndRetries;   /*!< Number of Bind Retries */
}RgLowSapCb;

/**
  * @brief
  * MAC sub-header control block for 5G-NR MAC
  * sub-hdr for fixed size MAC CE - 1 octat(R/R/LCID)
  * sub-hdr for variable size MAC CE or MAC SDU -2/3 octat(R/F/LCID/L(8/16 bits))
  * 
  */
typedef struct RgMUXSubHdr
{
   uint8_t shLen;  
   uint8_t shData[RG_MAX_SDU_SUB_HDR_LEN]; /* RRLCID/RFLCID + F-L (O) + L (O) */
} RgMUXSubHdr;


/** 
  * @brief
  * Cell Control block per cell 
  */
struct rgCellCb
{
   CmHashListEnt   cellLstEnt;       /*!< Hash list Entity for cell list */
   RgSchInstMap    schInstMap;      /*!< Sch Inst Info */
   CmLteCellId     cellId;           /*!< Cell ID */
   CmLteTimingInfo crntTime;
   uint8_t         maxDlHqProcPerUe;  /*!< Store based on FDD/TDD(UL/DL Index) */
   CrgBwCfg        bwCfg;            /*!< Bandwidth Configuration */
   CrgRachCfg      rachCfg;          /*!< RACH Configuration */

   RgUpSapCb       *rguDlSap;            /*!< RGU SAP Control Block for RLC DL  */
   RgUpSapCb       *rguUlSap;           /*!< RGU SAP Control Block  for RLC UL*/
   Inst            macInst;            /*!< MAC Instance id for this cell */
   uint8_t         cellActvState;      /*!< Bitmask indicating if cell is active:
                                       Cell is active only after BCCH, PCCH, DL
                                       CCCH, UL CCCH and scheduler config */
   CmLteLcId       ulCcchId;         /*!< LcId for uplink CCCH */
   CmLteLcId       dlCcchId;         /*!< LcId for downlink CCCH */ 
   RgBcchBchLcCb   bcchBchInfo;      /*!< BCCH mapped on BCH 
                                       logical channel control block */
   RgPcchLcCb      pcchInfo;        /*!< PCCH logical channel control block */
   uint8_t         numBcchDlschInfo;
   RgBcchDlschLcCb bcchDlschInfo[RG_MAX_BCCH_DLSCH]; /*!< BCCH mapped on DLSCH 
                                             logical channel control block */
   CmHashListCp    ueLst;            /*!< Hash list of UE control 
                                                   blocks: RgUeCb */
   /*Added support for SPS*/
#ifdef LTEMAC_SPS
   CmHashListCp    spsUeLst;         /*!< Hash list of UECbs with SPS-Rnti
                                                  : Element is RgUeCb */
#endif
   RgRaInfoCb      raInfo;           /*!< Random access related information for
                                          cell */
   RgDlSf          subFrms[RG_NUM_SUB_FRAMES];
#ifdef LTE_L2_MEAS
   CmLListCp       l2mList;                     /*!< List of all L2M requests
                                                     rgSchL2MeasCb */
   RgQciCb         qciArray[LRG_MAX_QCI];       /*!< List of all qci's configured
                                                      rgQciCb */
   RgUlSf          ulSf[RG_NUM_UL_SUB_FRAMES];  /*!< UL Sub frame */
   Bool            sndL2Meas;                   /*!< Send L2 Meas when No L2 Measuremnt timer is running*/

   uint32_t        ttiCycle;             /*!< Count of Number of 10240 Cycles
                                             completed */

#endif /* LTE_L2_MEAS */
   RguFlowCntrlInd  *flowCntrlInd;        /*!<Pre Alloced buffer for flow control */
   /* RRM_SP1_START */
   uint32_t             qcisUlPrbCnt[RG_MAX_QCI_REPORTS];         /*!< GBR Lc PRB count */
   /* RRM_SP1_END */
#ifdef LTE_ADV
  //RgLaaTbReq  *laaCellHqPLst[RG_MAX_SCELL_PER_UE]; /*!< LAA: List containing of TBs requested per
                                            //SCELL*/
  Void * laaCb;
#endif
#ifdef EMTC_ENABLE
    uint8_t               emtcEnable;
#endif
};

/**
  * @brief
  * Uplink HARQ entity information per UE.
  */
typedef struct rgUlHqEnt
{
   uint8_t           maxHqRetx;        /*!< Maximum number of harq
                                   * re-transmissions */ 
} RgUlHqEnt;
#ifdef LTE_L2_MEAS

/**
  * @brief Structure to hold Uplink throughtput info for L2 measurement.
  *
  * @details
  *   - outStndngBs     : outstanding buffer status
  *   - firstDatSegRcvd : first dat req seg recvd
  *   - frstSegTimInfo  : fisrt seg time information
  *   - nextSegTimInfo  : next seg time information
  *   - segInd          : UL thrgouthput indication
  *   - fSegTtiCycNum   : First Seg tti cycles
  *   - nSegTtiCycNum   : next seg tti cycles
  */
typedef struct rgL2MeasUlThrP
{ 
   S32 outStndngBs;                 /*!< outstanding buffer status*/
   Bool firstDatSegRcvd;            /*!< first dat req seg recvd*/
   CmLteTimingInfo frstSegTimInfo;  /*!< fisrt seg time information*/
   CmLteTimingInfo nextSegTimInfo;  /*!< next seg time information*/
   RguL2MUlThrpSegInd segInd;       /*!< UL thrgouthput indication*/
   uint32_t fSegTtiCycNum;               /*!< First Seg tti cycles*/
   uint32_t nSegTtiCycNum;               /*!< next seg tti cycles*/
} RgL2MeasUlThrP;
#endif

/** 
  * @brief
  * Logical channel group
  */
struct rgLcgCb
{
   /* Right now not keeping associated logical channels, searching for
    * associated channels needed only during config */
   LteLcgId    lcgId;  /*!< Group ID */
   uint8_t     lcCount;
#ifdef LTE_L2_MEAS

   RgL2MeasUlThrP lcgBsInfo;

#endif
   Bool        isGbr;  /*!< Indicate if LCG is GBR LCG */
};




/**
  * @brief
  * Uplink control block for UE specific information
  */
typedef struct rgUeUlCb
{
   RgUlHqEnt  hqEnt;      /*!< Uplink HARQ information for the UE */
   RgUlLcCb   lcCb[RG_MAX_LC_PER_UE]; /*!< Dedicated Uplink logical channel 
                                           information */
   RgLcgCb    lcgArr[RG_MAX_LCG_PER_UE]; /*!< Logical channel groups */
   /* Added support for SPS*/
#ifdef LTEMAC_SPS
   Bool                 spsLcId[RG_MAX_LC_PER_UE];  /*!< Set to TRUE for SPS LCs */ 
   uint8_t              implRelCntr;    /*!< Number of empty sps transmissions for
                                    implicit(?) release */
   uint8_t              implRelCnt; /*!< Configured value of implicitRelAfter +
                                    RG_ADDL_SPS_OCC_FOR_EXP_REL */
   uint8_t              explRelCntr;/*!< Counter to track explicit release */
   uint8_t              explRelCnt; /*!< Count to trigger explicit release */
/*Added for explicit release - start*/
   uint8_t              spsDatRcvd; /*!< Count to track SPS data for explicit 
                     release*/
   uint32_t             totSpsDatRcvd; /*!< total number of SPS data recieved
                     from activation*/
#endif
} RgUeUlCb;

/** 
  * @brief
  * Logical channel data as being scheduled by the scheduler.
  */
typedef struct rgLchSchdData
{
   CmLteLcId       lcId;       /*!< Logical channel ID */
   uint32_t        schdSz;   /*!< Scheduled logical channel data */
} RgLchSchdData;

/** 
  * @brief
  * Contention Resolution ID. 
  */
typedef struct RgContResId
{
   uint8_t        resId[RG_CRES_LEN];
}RgContResId;

/** 
  * @brief
  * Used to keep sf and SfLnk in Tb. 
  */
typedef struct rgTbSfLnkInfo
{
   RgDlSf          *sf;   /*!< Sf pointer to check if SfAlloc came for Tb in a SF */
   CmLList         sfLnk; /*!< Used to queue it up on a subfrm */
}RgTbSfLnkInfo;

/** 
  * @brief
  * Transport Block Info scheduled by the scheduler.
  */
typedef struct rgTbInfo{
   uint8_t              numSchLch;
   RgLchSchdData        schdData[RG_MAX_NUM_DED_LC];
   TknUInt8             schdTa;  /*!< TA scheduled to be sent */
   /* Changed as a result of CR timer implementation*/
   uint8_t              contResCe;   /*!< Contention Resolution CE scheduled 
                                   to be sent */
   RgContResId          *contResId;  /*!< Contention Resolution Identity */
#ifndef L2_OPTMZ
   Buffer               *tb;    /*!< Multiplexed Pdu */
#else
   /* L2 Optimization for mUe/Tti:Pre-allocated MAC Hdr, MAC CEs and padding
    * will be used to store and send to CL. These pointer will be allocated
    * during Harq Proc creation. CL shall not free these pointers.
    * These pointers will be freed duing harq proc deletion
    */
    RgTfuDatReqTbInfo     tb;   /* TB to be sent to CL/PHY*/
#endif
/* Changes for MIMO feature addition */
/* Removed dependency on MIMO compile-time flag */
   uint32_t             tbSz;   /*!< TB size scheduled for the hqP */    
   RgPdcch              pdcch;  /*!< PDCCH allocated for this HQ Proc */
   /* Adding Array of sfLnk in case of repetitions because same lnk in mutiple
    * list causing issues*/
   RgTbSfLnkInfo   sfLnkInfo[2]; /*!< Used to queue it up on a subfrm */
   RgDlSf          *sf;
   CmLteTimingInfo timingInfo; /*!<This field holds the scheduled time */
   TknUInt32       doa;
   TfuTxMode       txMode;
   Bool            puschRptUsd; /*!< True if Precoding Info in PDCCH has to be
                                   in-accordance with the latest PUSCH report */
   TfuDlCqiPuschInfo puschPmiInfo;/*!< PUSCH report details for explicit PMI
                                       * information to PHY during a PDSCH */
   /* LTE_ADV_FLAG_REMOVED_START */
#ifdef TFU_UPGRADE
   uint8_t            pa;                 /* DL Power control paramter P_A
                                     configured by higher layers
                                     Ref: RRC 36.331, 6.3.2,
                                     PDSCH-Config*/
#endif
   uint8_t isEnbSFR;
#ifdef LTE_ADV
   TknUInt8           sCellActCe;     /*!< SCell Act ce scheduled to be sent */
#endif
   /* LTE_ADV_FLAG_REMOVED_END */
}RgTbInfo;

/** 
  * @brief`
  * Downlink HARQ process information per UE.
  */
struct rgDlHqProcCb
{
   uint8_t         procId;    /*!< HARQ process ID */
/* Changes for MIMO feature addition */
/* Removed dependency on MIMO compile-time flag */
   uint8_t         numOfTBs;
   RgTbInfo        tbInfo[RG_MAX_TB_PER_UE];
   uint16_t             saveId; // laa
#ifdef LTE_L2_MEAS

   uint32_t        tbId[RGU_MAX_TB];          /*!< RLC's internal tbId */                                    
   uint16_t        status[RGU_MAX_TB];        /*!< Status back to RLC ACK/NACK*/
#endif /* LTE_L2_MEAS */
/* Changes for MIMO feature addition */
/* Removed dependency on MIMO compile-time flag */
};


/** 
  * @brief
  * Downlink HARQ entity information per UE.
  */
struct rgDlHqEnt
{
   uint8_t       numHqProcs;
   RgDlHqProcCb  *procs[RG_MAX_DL_HARQ_NUM]; /*!< Downlink harq processes */
   CmLListCp     savedProcLst[RG_MAX_DL_HARQ_NUM]; /*!< laa: Saved TBs for the release Hq Procs*/
};



/**
  * @brief
  * Downlink control block for UE specific information.
  */
typedef struct rgUeDlCb
{
   RgDlHqEnt  hqEnt;        /*!< Downlink HARQ information for the UE */          
   RgDlLcCb   lcCb[RG_MAX_LC_PER_UE];/*!< Dedicated Downlink logical channels in
UE */ 
} RgUeDlCb;                   

#ifdef LTE_ADV
typedef struct rgSCellAddCfgCfmInfo
{
   CrgCfgTransId   transId;   /*!< Scell add transaction id*/
   uint8_t         numSCells; /*!< Used to check if config cfm rcvd from all Scells*/ 
   uint8_t         cfgCfgCount; /*!< Keep count of cfg confirms recvd */
   uint8_t         mask; /*!< Cfg cfm mask */
}RgSCellAddCfgCfmInfo;

typedef struct rgSCelInfo
{
   Bool           isSCellAdded;     /*!< is Cell Added TRUE/FALSE */
   Inst           macInst;          /*!< MAC Instance of that 
                                         Cell Added */
   CmLteCellId    sCellId;          /*!< Secondary Cell ID */
}RgSCelInfo;

#endif

/** 
  * @brief
  * UE control block for UE specific information
  */
struct rgUeCb
{
   CmHashListEnt     ueLstEnt;   /*!< Hash List entity for UE List */
   /* Added suport for SPS*/
#ifdef LTEMAC_SPS
   CmHashListEnt     spsUeLstEnt;/*!< Hash List entity for SPS Rnti */
#endif
   CmLList           rachLstEnt; /*!< Link List entity for RACH UE List*/ 
   CmLteRnti         ueId;       /*!< UE identifier */
   /* Added suport for SPS*/
#ifdef LTEMAC_SPS
   CmLteRnti         spsRnti;    /*!< SPS-Rnti for the UE */
#endif
   RgContResId       contResId;  /*!< Contention Resolution Identity */
   RgUeUlCb          ul;        /*!< UE Specific Uplink information */
   RgUeDlCb          dl;        /*!< UE Specific Downlink information */
/* Changes for MIMO feature addition */
/* Removed dependency on MIMO compile-time flag */
   CrgTxMode         txMode;      /*!< UE Transmission mode Cfg */
   RgUpSapCb        *rguDlSap;    /*!< RGU SAP Control Block for RLC DL  */
   RgUpSapCb        *rguUlSap;    /*!< RGU SAP Control Block  for RLC UL*/
#ifdef LTE_ADV
   RgSCelInfo        sCelInfo[RG_MAX_SCELL_PER_UE]; /*!<sCell Information */
   RgSCellAddCfgCfmInfo cfgCfmInfo; /*!< Scell add cfg cfm info*/
#endif /* LTE_ADV */
};

/** 
  * @brief
  * structure holding Layer Manager Response information cached from control
  * request.
  */
typedef struct rgLmResponse
{
   TranId transId;           /*!< Transaction ID */
   Resp   response;          /*!< Response */
}RgLmResponse;

/** 
  * @brief
  * Structure holding LTE MAC's General Configuration Information.
  */
typedef struct rgGenCb
{
   uint8_t      tmrRes;           /*!< Timer resolution */
   RgLmResponse trcResp;          /*!< Response value for Trace Indication */
   RgLmResponse ustaResp;         /*!< Respones value for Alarms */
   RgLmResponse bndCfmResp;       /*!< Respones value for Bind Confirm */
   Status       memUpperThr;      /*!< upper threshold of the memory pool */
   Status       memLowerThr;      /*!< lower threshold of the memory pool */
}RgGenCb;



/** 
  * @brief
  * Global Control block for LTE-MAC.
  */
struct _rgCb
{
   TskInit      rgInit;                    /*!< Task Init information */
   S16          trcLen;                    /*!< trace length */
   RgGenCb      genCfg;                    /*!< General Config information */
   uint8_t      numRguSaps;                /*!< Numb of RGU SAP Control Block */
   RgUpSapCb    *rguSap;                     /*!< RGU SAP Control Block */
   RgUpSapCb    crgSap;                    /*!< CRG SAP Control Block */
   RgLowSapCb   tfuSap;                    /*!< TFU SAP Control Block */
   RgGenSts     genSts;                    /*!< General Statistics */
   CmTqCp       tmrTqCp;                   /*!< Timer Task Queue Cntrl Point */
   CmTqType     tmrTq[RG_TQ_SIZE];         /*!< Timer Task Queue */
   CmTimer      tmrBlk[RG_MAX_TIMER];      /*!< Timer Block */
   RgCellCb *inactiveCell;                     /*!< Reference of last accessed cellCb*/
   RgCellCb *cell;                     /*!< Reference of last accessed cellCb*/
   RgCellCb *prevCell;                     /*!< Reference of last accessed cellCb*/
   uint8_t bufCnt;                              /*!< Count of buffers in buffersToFree[] */
   Buffer *bufToFree[RG_MAX_DFRD_FREE_BUFS];/*!< List of Shared Buffers to be freed */
};

/* Declaration for RgCb */
RgCb rgCb[RG_MAX_INST];

/** 
  * @brief
  * Subheader of MAC SDU.
  */
typedef struct rgMacSdu
{
   CmLList    sduLstEnt;  /*!< List entry for MAC SDU subheader */
   CmLteLcId  lcId;       /*!< Logical channel ID */
   uint16_t   len;        /*!< Length of the SDU */
   Buffer     *mBuf;      /*!< SDU Buffer */
} RgMacSdu;

/** 
  * @brief
  * MAC PDU Contents.
  */
typedef struct rgMacPdu
{
   CmMemListCp memCp;     /*!< Memory control point */
   CmLListCp   sduLst;    /*!< List of SDUs */
} RgMacPdu;

/** 
  * @brief
  * Carries the Error Information.
  */
struct rgErrInfo
{
   uint8_t    errType;    /*!< Error Type */
   uint16_t   errCause;   /*!< Cause of Error */
};

/** 
  * @brief
  * Carries the Information to build PDU.
  */
typedef struct rgBldPduInfo
{
   Void           *datReq;    /*!< Common/Dedicated Dat Request */
   uint8_t        reqType;    /*!< EVTRGUCDATREQ/EVTRGUDDATREQ */
   MsgLen         schdTbSz;   /*!< Schedule TB size */
   uint8_t        tbIndex;    /*!< Transport Block Index */
   TknUInt8       ta;         /*!< Timing Advance */
   RgContResId    *contResId; /*!< Contention Resolution ID */
#ifdef LTE_ADV
   TknUInt8       sCellActCe; /*!< SCell Act CE info */
#endif
} RgBldPduInfo;



/* APIs exposed by LIM */

#ifdef LTE_L2_MEAS /*LTE_L2_MEAS_PHASE2*/
S16 rgAddToL2MeasPerQci ARGS ((RgCellCb  *cell,uint8_t qci));
#endif
S16 rgLIMTfuBndReq ARGS((
         Inst                 inst,
         SuId                 suId,
         SpId                 spId));

S16 rgLIMTfuUbndReq ARGS((
         Inst                 inst,
         SuId                 suId,
         Reason               reason));

S16 rgLIMTfuDatReq ARGS((
         Inst          inst,
         TfuDatReqInfo *datReq));
#ifdef L2_OPTMZ
S16 rgLIMTfuDelDatReq ARGS((
         Inst          inst,
         TfuDelDatReqInfo *datReq));
#endif /*L2_OPTMZ */
/* APIs exposed by TOM */

S16 rgTOMDatInd ARGS((
         Inst          inst,
         TfuDatIndInfo *datInd));

Void rgTOMRlsSf ARGS((Inst inst,RgDlSf *dlSf));

S16 rgSchMacSfAllocReq ARGS((Pst *pst, RgInfSfAlloc *sfInfo));

/*
 * APIs exposed by LMM
 */
S16 rgLMMStartTmr ARGS((Inst inst,S16 tmrEvnt, uint32_t tmrVal, PTR cb));
S16 rgLMMStopTmr ARGS((Inst inst,S16 tmrEvnt, PTR cb));
S16 rgLMMTmrExpiry ARGS((PTR cb, S16 tmrEvnt));
/* To send a Unsolicited Status Indication to Layer Manager */
S16 rgLMMStaInd ARGS((Inst inst,uint16_t category, uint16_t event, uint16_t cause, RgUstaDgn *dgn));
/* To send a Trace Indication to Layer Manager.
 * srcMbuf is not freed. caller has to take care. */
Void rgLMMTrcInd ARGS((Inst inst,Buffer *srcMbuf, uint8_t event));
/* LIM uses this to forward the BndCfm to LMM. This function invokes a
 * Control Confirmation to the LM. */
S16 rgLMMBndCfm ARGS((Pst *pst, SuId suId, uint8_t status));
S16 rgActvTmr ARGS((Ent ent, Inst inst ));
S16 rgActvInit ARGS((Ent entity, Inst inst, Region 
       region, Reason reason));
S16 rgActvTsk ARGS((Pst *pst, Buffer *mBuf));

/* 
 * APIs exposed by CFG module
 */
S16 rgCFGVldtCrgCellCfg ARGS((Inst inst,CrgCellCfg *cellCfg, RgErrInfo *errInfo));
S16 rgCFGVldtCrgUeCfg ARGS((Inst inst,CrgUeCfg  *ueCfg, RgCellCb  **cell, RgErrInfo
      *errInfo));
S16 rgCFGVldtCrgLcCfg ARGS((Inst inst,CrgLchCfg *lcCfg, RgCellCb **cell, RgUeCb
      **ue, RgErrInfo  *errInfo));
S16 rgCFGVldtCrgCellRecfg ARGS((Inst inst,CrgCellRecfg *cellRecfg, RgCellCb **cell,
      RgErrInfo *errInfo));
S16 rgCFGVldtCrgUeRecfg ARGS((Inst inst,CrgUeRecfg *ueRecfg, RgCellCb **cell,
      RgUeCb **ue, RgErrInfo *errInfo));
S16 rgCFGVldtCrgLcRecfg ARGS((Inst inst,CrgLchRecfg *lcRecfg, RgCellCb **cell,
      RgUeCb **ue, RgUlLcCb **ulLc, RgErrInfo *errInfo));
S16 rgCFGVldtCrgUeReset ARGS ((Inst inst,CrgRst *reset,RgCellCb **cell,RgUeCb **ue,
         RgErrInfo   *errInfo));

S16 rgCFGCrgCellCfg ARGS((Inst inst,CrgCellCfg *cellCfg, RgErrInfo *errInfo));
S16 rgCFGCrgUeCfg ARGS((Inst inst,RgCellCb  *cell, CrgUeCfg  *ueCfg, RgErrInfo
      *errInfo));

#ifdef LTE_ADV
S16 rgFillAndAddSCellCfg ARGS((Inst inst, RgCellCb  *cell, 
         CrgUeRecfg *ueRecfg, CrgCfgTransId transId, Bool *isCfmRqrd));
S16 rgDelUeFrmAllSCell ARGS(( RgCellCb       *cell, RgUeCb         *ue));
S16 rgUtlVltdAddSCellCfg ARGS ((RgPrgUeSCellCfgInfo *ueSCellCb,
                                       RgCellCb    *cell,
                                       Inst        inst
                                     ));
S16 rgCfgAddUeSCellCfg ARGS ((Inst        dstMacInst,
                                    RgPrgUeSCellCfgInfo *ueSCellCb,
                                    RgCellCb    *cell
                                  ));
#endif /* LTE_ADV */

S16 rgCFGCrgLcCfg ARGS((Inst inst,RgCellCb *cell, RgUeCb *ue, CrgLchCfg
      *lcCfg, RgErrInfo *errInfo,Bool *isCfmRqrd,CrgCfgTransId   transId));
S16 rgCFGCrgCellRecfg ARGS((Inst inst,RgCellCb *cell, CrgCellRecfg *cellRecfg,
      RgErrInfo *errInfo));
S16 rgCFGCrgUeRecfg ARGS((Inst inst,RgCellCb *cell, RgUeCb *ue, CrgUeRecfg
      *ueRecfg, RgErrInfo *errInfo));
S16 rgCFGCrgLcRecfg ARGS((Inst inst,RgCellCb *cell, RgUeCb *ue, RgUlLcCb
      *ulLc, CrgLchRecfg *lcRecfg, RgErrInfo *errInfo, Bool *isCfmRqrd));
S16 rgCFGCrgCellDel ARGS((Inst inst,CrgDel *cellDelInfo, RgErrInfo *errInfo));
S16 rgCFGCrgUeDel ARGS((Inst inst,CrgDel *ueDelInfo, RgErrInfo *errInfo));
S16 rgCFGCrgLcDel ARGS((Inst inst,CrgDel *lcDelInfo, RgErrInfo
         *errInfo,Bool *isCfmRqrd,CrgCfgTransId transId));
Void rgCFGFreeCellCb ARGS((RgCellCb *cell));
Void rgCFGFreeInactvCellCb ARGS((RgCellCb *cell));
S16 rgSchMacCellRegReq ARGS((Pst* pst,RgInfCellReg* regReq));
S16 rgCFGCrgUeReset ARGS((RgCellCb *cell,RgUeCb *ue,CrgRst *reset,
         RgErrInfo *errInfo));

/* 
 * APIs exposed by COM module
 */
S16 rgCOMCfgReq ARGS((Inst inst,CrgCfgTransId transId, CrgCfgReqInfo
         *cfgReqInfo));

/* APIs Exposed by UIM */
S16 rgUIMRguBndCfm ARGS ((Inst inst,SuId suId, uint8_t status));
S16 rgUIMSndDedStaInd ARGS ((Inst inst,RgUpSapCb *rguDlSap,RgRguDedStaInd  *staInd));
S16 rgUIMSndCmnStaInd ARGS ((Inst inst,RgUpSapCb *rguDlSap,RgRguCmnStaInd  *staInd));
S16 rgUIMSndDedDatInd ARGS ((Inst inst,RgUpSapCb *rguDlSap,RgRguDedDatInd  *datInd));
#ifdef LTE_L2_MEAS
S16 rgL2mCreateMeasCb ARGS((
         RgCellCb       *cell,
         RgInfL2MeasReq *measInfo, 
         uint8_t         measType,
         RgErrInfo      *err
));
 S16 rgL2mMeasReq ARGS((
         RgCellCb       *cell,
         RgInfL2MeasReq *measInfo,
         RgErrInfo      *err
));
S16 rgL2Meas ARGS ((
         RgCellCb  *cell
));
#endif /* LTE_L2_MEAS */
S16 rgUIMSndCmnDatInd ARGS ((Inst inst,RgUpSapCb *rguSap,RgRguCmnDatInd  *datInd));
S16 rgUIMCrgBndCfm ARGS ((Inst inst,SuId suId,uint8_t status));
S16 rgUIMCrgCfgCfm ARGS ((Inst inst,CrgCfgTransId transId,uint8_t status));


/* 
 * APIs exposed by ROM module
 */
S16 rgROMDedDatReq ARGS((Inst inst,RgRguDedDatReq *datReq));
S16 rgROMCmnDatReq ARGS((Inst inst,RgRguCmnDatReq *datReq));
S16 rgROMDedStaRsp ARGS((Inst inst,RgRguDedStaRsp *staRsp));
S16 rgROMCmnStaRsp ARGS((Inst inst,RgRguCmnStaRsp *staRsp));
#ifdef LTE_L2_MEAS

S16 rgROML2MUlThrpMeasReq ARGS((Inst inst,RgRguL2MUlThrpMeasReq *measReq));

#endif

/* 
 * APIs exposed by RAM module
 */
RgUeCb* rgRAMCreateUeCb ARGS((RgCellCb *cell,
                     CmLteRnti tmpCrnti,Bool insert, RgErrInfo *err));
S16 rgRAMFreeCell ARGS((RgCellCb    *cell));
Void rgRAMFreeUeCb ARGS((Inst inst,RgUeCb *ueCb));
S16 rgHndlRaResp ARGS((RgCellCb *cell,CmLteTimingInfo timingInfo,
            RgInfRarInfo  *rarInfo,RgErrInfo *err));
S16 rgSchMacRlsRntiReq ARGS((Pst *pst,RgInfRlsRnti* rlsRnti));
/* 
 * APIs exposed by UHM module
 */
Void rgUHMCrgUeCfg ARGS(( RgCellCb *cellCb, RgUeCb *ueCb, 
                                 CrgUeCfg *ueCfg));
Void rgUHMCrgUeRecfg ARGS(( RgCellCb *cellCb, RgUeCb *ueCb, 
                              CrgUeRecfg *ueRecfg));
/* 
 * APIs exposed by DUX module
 */
S16 rgDUXDemuxData ARGS((Inst inst,RgMacPdu *pdu, RgInfCeInfo   *ceInfo,
                                Buffer **mBuf, RgErrInfo *err)); 
S16 rgDUXExtSduLen ARGS
((
Inst        inst,
Buffer      *mBuf,
uint16_t    *sduLen,
RgErrInfo   *err
));

/* 
 * APIs exposed by DBM module
 */

S16 rgDBMInitInactvCellLst ARGS((uint16_t numBins));
S16 rgDBMDeInitInactvCellLst ARGS((Void));
S16 rgDBMInsInactvCellCb ARGS(( RgCellCb *cellCb));
RgCellCb* rgDBMGetInactvCellCb ARGS(( CmLteCellId cellId));
S16 rgDBMDelInactvCellCb ARGS(( RgCellCb *cellCb));
RgCellCb* rgDBMGetNextInactvCellCb ARGS((RgCellCb *cellCb));

S16 rgDBMInitCellLst ARGS(( uint16_t numBins));
S16 rgDBMDeInitCellLst ARGS((Void));
S16 rgDBMInsCellCb ARGS(( RgCellCb *cellCb));
RgCellCb* rgDBMGetCellCb ARGS(( CmLteCellId cellId));
S16 rgDBMDelCellCb ARGS(( RgCellCb *cellCb));
RgCellCb* rgDBMGetNextCellCb ARGS((RgCellCb *cellCb));
S16 rgDBMInitCell ARGS((RgCellCb *cellCb));

S16 rgDBMDeInitUeCbLst ARGS(( RgCellCb *cellCb));
S16 rgDBMInsUeCb ARGS(( RgCellCb *cellCb, RgUeCb *ueCb));
RgUeCb* rgDBMGetUeCb ARGS(( RgCellCb *cellCb, CmLteRnti ueId));
RgUeCb* rgDBMGetNextUeCb ( RgCellCb *cellCb, RgUeCb *ueCb);
S16 rgDBMDelUeCb ARGS(( RgCellCb *cellCb, RgUeCb *ueCb));
Void rgDBMInitUe ARGS((RgUeCb *ueCb));

#ifdef LTE_L2_MEAS
Void rgDBMInsUlDedLcCb ARGS((RgUeCb *ueCb, CmLteLcId idx, LteLcgId  gId,
                        uint8_t  qci));
#else
Void rgDBMInsUlDedLcCb ARGS((RgUeCb *ueCb, 
                  CmLteLcId idx, LteLcgId gId));
#endif
Void rgDBMUpdUlDedLcCb ARGS((RgUeCb *ueCb, 
               RgUlLcCb *ulLcCb, LteLcgId gId));

Void rgDBMInsDlDedLcCb ARGS((RgUeCb *ueCb, CmLteLcId idx));
Void rgDBMDelUlDedLcCb ARGS((RgUeCb *ueCb, RgUlLcCb *ulLcCb));
Void rgDBMDelDlDedLcCb ARGS((RgUeCb *ueCb, RgDlLcCb *dlLcCb));
RgDlLcCb* rgDBMGetDlDedLcCb ARGS(( RgUeCb *ueCb, CmLteLcId idx));
RgUlLcCb* rgDBMGetUlDedLcCb ARGS(( RgUeCb *ueCb, CmLteLcId idx));
Void rgDBMFreeCmnLcLst ARGS((RgCellCb *cellCb));
S16 rgDBMChkCmnLcCb ARGS(( RgCellCb *cellCb, CmLteLcId lcId));
RgBcchBchLcCb* rgDBMGetBcchOnBch ARGS(( RgCellCb *cellCb ));
RgBcchDlschLcCb* rgDBMGetBcchOnDlsch ARGS(( RgCellCb *cellCb, 
                                                   CmLteLcId lcId));
RgPcchLcCb* rgDBMGetPcch ARGS(( RgCellCb *cellCb));
Void rgDBMInsBcchOnBch ARGS(( RgCellCb *cellCb, CmLteLcId idx));
Void rgDBMInsBcchOnDlsch ARGS(( RgCellCb *cellCb, CmLteLcId idx));
Void rgDBMInsPcch ARGS(( RgCellCb *cellCb, CmLteLcId idx));

Void rgDBMInsUeCbInRachLst ARGS((RgCellCb *cellCb,RgUeCb *ueCb));
RgUeCb* rgDBMGetUeCbFromRachLst ARGS((RgCellCb *cellCb,CmLteRnti key));
RgUeCb* rgDBMGetNextUeCbFromRachLst ARGS((RgCellCb *cellCb,
                                                      RgUeCb *ueCb));
Void rgDBMDelUeCbFromRachLst ARGS((RgCellCb *cellCb,RgUeCb *ueCb));
/* Added support for SPS*/
#ifdef LTEMAC_SPS
S16 rgDBMDeInitSpsUeCbLst ARGS((RgCellCb *cellCb));
S16 rgDBMInsSpsUeCb ARGS((RgCellCb *cellCb, RgUeCb *ueCb));
RgUeCb* rgDBMGetSpsUeCb ARGS((RgCellCb *cellCb, CmLteRnti ueId));
RgUeCb* rgDBMGetNextSpsUeCb ARGS((RgCellCb *cellCb, RgUeCb *ueCb));
S16 rgDBMDelSpsUeCb ARGS((RgCellCb *cellCb,RgUeCb *ueCb));
#endif /* LTEMAC_SPS */
                        
/* 
 * APIs exposed by DHM module
 */
Void rgCFGFreeUeCb ARGS((RgCellCb *cell, RgUeCb *ue));
S16 rgDHMHqEntInit ARGS((Inst inst,RgDlHqEnt *hqE, uint8_t maxHqProcs));
Void rgDHMRlsHqProc ARGS((RgDlHqProcCb *hqP));
Void rgDHMUeReset ARGS ((RgCellCb *cell, RgDlHqEnt *hqE));
Void rgDHMHdlBufFree ARGS((Inst inst, Buffer **mBuf));
Void rgDHMFreeTbBufs ARGS((Inst inst));
S16 rgDHMRlsHqProcTB ARGS((RgCellCb *cell, RgDlHqProcCb *hqP,uint8_t tbIndex));
S16 rgDHMGetHqProcFrmId ARGS((RgUeCb *ue, uint8_t idx, RgDlHqProcCb **hqP));
/* Changes for MIMO feature addition */
/* Removed dependency on MIMO compile-time flag */
S16 rgDHMSndDatReq ARGS((RgCellCb *cellCb, RgDlSf *dlSf, RgTfuDatReqInfo *datInfo, 
                                RgDlHqProcCb *hqProc, RgErrInfo *err));
S16 rgDHMHndlDedDatReq ARGS((Inst inst,RgDlHqProcCb *hqProc, 
                              RgRguDDatReqPerUe *datReq, RgDlSf *dlSf, RgErrInfo *err));
/* Changed for contention resolution timer implementation*/
S16 rgDHMHndlCmnDatReq ARGS((Inst inst,RgDlHqProcCb *hqProc, 
                RgRguCmnDatReq *datReq, RgErrInfo *err));
/* Changes for MIMO feature addition */
/* Removed dependency on MIMO compile-time flag */

S16 rgDHMSndConsolidatedStaInd ARGS ((RgCellCb *cell,RgInfUeInfo *ueInfo,
      CmLteTimingInfo timingInfo, RgErrInfo *err));

Void rgDHMFreeUe ARGS((Inst inst,RgDlHqEnt *hqE));
S16 rgSchMacRlsHqReq ARGS((Pst *pst, RgInfRlsHqInfo *rlshqUeInfo));

/* 
 * APIs exposed by MUX module
 */
#ifndef L2_OPTMZ
S16 rgMUXBldPdu ARGS((Inst inst, RgBldPduInfo *bldPdu, Buffer **txPdu, 
         RgErrInfo *err));
#else
S16 rgMUXBldPdu ARGS((Inst inst, RgBldPduInfo *bldPdu, RgTfuDatReqTbInfo *tb, 
         RgErrInfo *err));
Bool RgUtlIsTbMuxed ARGS((TfuDatReqTbInfo *tb));
#endif
S16 rgMUXBldRarPdu ARGS((RgCellCb *cell, RgInfRaRntiInfo *alloc,
         Buffer **txPdu, RgErrInfo *err));

/*
 * Utility APIs
 */
S16  rgAllocSBuf     ARGS((Inst inst,Data **pData, Size size));
S16  rgGetMsg        ARGS((Inst inst,Buffer **mBuf));
/*ccpu00117052 - MOD  Passing double pointer for proper NULLP
                      assignment */
Void rgFreeSBuf      ARGS((Inst inst,Data **data, Size size));
Void rgFillDgnParams ARGS((Inst inst,RgUstaDgn *dgn,uint8_t dgnType));
Void rgUpdtRguDedSts ARGS((Inst inst,RgUpSapCb *rguSap,uint8_t stsType, RgRguDedDatReq *datReq));
Void rgUpdtRguCmnSts ARGS((Inst inst,RgUpSapCb *rguSap,uint8_t stsType));
Void rgUpdtCellCnt   ARGS((Inst inst,uint8_t updtType));
Void rgUpdtUeCnt     ARGS((Inst inst,uint8_t updtType));
Void rgGetPstToInst ARGS((Pst *pst,Inst srcInst, Inst dstInst));
S16 rgAllocEventMem ARGS((Inst inst,Ptr *memPtr,Size memSize));
S16 rgGetEventMem ARGS((Inst inst,Ptr *ptr,Size len,Ptr memCp));
S16 rgAllocShrablSBuf ARGS((Inst inst,Data **pData, Size size));
Void rgPrintfSubFrameInfo ARGS((RgDlSf *dlSf));
Void printMacCellInfo ARGS((Void));
Void rgFreeSharableSBuf ARGS((Inst inst,Data **data, Size size)); 
void MacSendCellCfgCfm(uint8_t response);


Void rgDHMFreeAllTbBufs ARGS
((
Inst inst
));

S16 rgMUXAddPadd ARGS((
Inst           inst,
MsgLen         *schdTbSz,
Buffer         *sduBuf,
Bool           isRar,
RgErrInfo      *err
));
uint16_t rgMUXCalcRiv ARGS
((
uint8_t           bw,
uint8_t           rbStart,
uint8_t           numRb
));



#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __RGX__ */


/**********************************************************************
         End of file
**********************************************************************/
