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
   U8               numPrb;    /*!< Number of total PRB's allocated for this UE */
} RgUeUlAlloc;
/**
 * @brief Structure hold uplink allocation information for sub frames.
 * */
typedef struct rgUlSf
{
   CmLteTimingInfo schdTime;       /*!< scheduled frame and subframe number */
   U8              numUe;          /*!< Number of UE's allocated in this uplink
                                      subframe */
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
   U32                sfnCycle;       /*!< Count of Num of SFN wraps */ 
};
/** @brief
 *  Control block to store Logical Channels with Qci.
 *  */
struct rgQciCb
{
   U8               qci;      /*< QCI for measurement */
   U32              prbCount; /*!< Cummulative PRB count */
   U8               mask;     /*!< Measurement Active or Not*/
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
   U16            txPwrOffset; /*!< PDSCH Tx Pwr offset */
} RgBcchTb;

typedef struct rgPcchTb
{
   RgPdcch        pdcch;
   Buffer         *tb;
   U16            txPwrOffset; /*!< PDSCH Tx Pwr offset */
} RgPcchTb;

typedef struct rgRaRspAlloc
{
   RgPdcch        pdcch;     /*!< NULLP if no Rsp allocation done for raRnti*/
   Buffer         *rar;
   U16            txPwrOffset; /*!< PDSCH Tx Pwr offset */
}RgRaRspAlloc;

typedef struct rgBchTb
{
   Buffer         *tb;    /*!< BCH data for this frame */
}RgBchTb;


typedef struct rgDlSf
{
   CmLteTimingInfo schdTime;  /*!< scheduled frame and subframe number */
   Bool           txDone;     /*!< Flag to indicate transmission done */
/* ADD Changes for Downlink UE Timing Optimization */
#ifdef LTEMAC_DLUE_TMGOPTMZ  
   U8            remDatReqCnt; /*!< Counter to maintain count of Ded data received.*/  
   /* Fix [ccpu00126310]: Tracks Data Requests from RLC for both loosely and tight coupled 
      RLC-MAC */
   Bool           statIndDone; /*!< Marks sending of all status indication done */
#endif
   RgBchTb        bch;        /*!< BCH data for this frame */
   RgBcchTb       bcch;       /*!< BCCH allocation for this frame */
#ifdef EMTC_ENABLE
   RgEmtcBcchTb   emtcBcch;   /*!< EMTC BCCH allocation for this frame */
#endif
   RgPcchTb       pcch;       /*!< PCCH allocation for this frame */
   CmLListCp      tbs;        /*!< TBs or DL HARQ processes */
   U8             numRaRsp;   /*!< No.of RaRsp */
   RgRaRspAlloc   raRsp[RG_MAX_RA_RNTI]; /* Array of list of Ra Rsp 
                                       Allocations for each rarnti */
   U8    numTbReqsSent;        /*!< Number of TB requests sent towards SMAC.
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
   U8              numBndRetries;   /*!< Number of Bind Retries */
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
   U8 shLen;  
   U8 shData[RG_MAX_SDU_SUB_HDR_LEN]; /* RRLCID/RFLCID + F-L (O) + L (O) */
} RgMUXSubHdr;


/** 
  * @brief
  * Cell Control block per cell 
  */
struct rgCellCb
{
   CmHashListEnt  cellLstEnt;       /*!< Hash list Entity for cell list */
   RgSchInstMap   schInstMap;      /*!< Sch Inst Info */
   CmLteCellId    cellId;           /*!< Cell ID */
   CmLteTimingInfo crntTime;
   U8             maxDlHqProcPerUe;  /*!< Store based on FDD/TDD(UL/DL Index) */
   CrgBwCfg        bwCfg;            /*!< Bandwidth Configuration */
   CrgRachCfg      rachCfg;          /*!< RACH Configuration */

   RgUpSapCb    *rguDlSap;            /*!< RGU SAP Control Block for RLC DL  */
   RgUpSapCb    *rguUlSap;           /*!< RGU SAP Control Block  for RLC UL*/
   Inst         macInst;            /*!< MAC Instance id for this cell */
   U8           cellActvState;      /*!< Bitmask indicating if cell is active:
                                       Cell is active only after BCCH, PCCH, DL
                                       CCCH, UL CCCH and scheduler config */
   CmLteLcId       ulCcchId;         /*!< LcId for uplink CCCH */
   CmLteLcId       dlCcchId;         /*!< LcId for downlink CCCH */ 
   RgBcchBchLcCb  bcchBchInfo;      /*!< BCCH mapped on BCH 
                                       logical channel control block */
   RgPcchLcCb      pcchInfo;        /*!< PCCH logical channel control block */
   U8              numBcchDlschInfo;
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
   CmLListCp         l2mList;                     /*!< List of all L2M requests
                                                     rgSchL2MeasCb */
   RgQciCb           qciArray[LRG_MAX_QCI];       /*!< List of all qci's configured
                                                      rgQciCb */
   RgUlSf            ulSf[RG_NUM_UL_SUB_FRAMES];  /*!< UL Sub frame */
   Bool              sndL2Meas;                   /*!< Send L2 Meas when No L2 Measuremnt timer is running*/

   U32               ttiCycle;             /*!< Count of Number of 10240 Cycles
                                             completed */

#endif /* LTE_L2_MEAS */
   RguFlowCntrlInd  *flowCntrlInd;        /*!<Pre Alloced buffer for flow control */
   /* RRM_SP1_START */
   U32             qcisUlPrbCnt[RG_MAX_QCI_REPORTS];         /*!< GBR Lc PRB count */
   /* RRM_SP1_END */
#ifdef LTE_ADV
  //RgLaaTbReq  *laaCellHqPLst[RG_MAX_SCELL_PER_UE]; /*!< LAA: List containing of TBs requested per
                                            //SCELL*/
  Void * laaCb;
#endif
#ifdef EMTC_ENABLE
    U8               emtcEnable;
#endif
};

/**
  * @brief
  * Uplink HARQ entity information per UE.
  */
typedef struct rgUlHqEnt
{
   U8           maxHqRetx;        /*!< Maximum number of harq
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
   U32 fSegTtiCycNum;               /*!< First Seg tti cycles*/
   U32 nSegTtiCycNum;               /*!< next seg tti cycles*/
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
   U8          lcCount;
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
   Bool            spsLcId[RG_MAX_LC_PER_UE];  /*!< Set to TRUE for SPS LCs */ 
   U8              implRelCntr;    /*!< Number of empty sps transmissions for
                                    implicit(?) release */
   U8              implRelCnt; /*!< Configured value of implicitRelAfter +
                                    RG_ADDL_SPS_OCC_FOR_EXP_REL */
   U8              explRelCntr;/*!< Counter to track explicit release */
   U8              explRelCnt; /*!< Count to trigger explicit release */
/*Added for explicit release - start*/
   U8              spsDatRcvd; /*!< Count to track SPS data for explicit 
                     release*/
   U32             totSpsDatRcvd; /*!< total number of SPS data recieved
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
   U32             schdSz;   /*!< Scheduled logical channel data */
} RgLchSchdData;

/** 
  * @brief
  * Contention Resolution ID. 
  */
typedef struct RgContResId
{
   U8        resId[RG_CRES_LEN];
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
   U8              numSchLch;
   RgLchSchdData   schdData[RG_MAX_NUM_DED_LC];
   TknU8           schdTa;  /*!< TA scheduled to be sent */
   /* Changed as a result of CR timer implementation*/
   U8              contResCe;   /*!< Contention Resolution CE scheduled 
                                   to be sent */
   RgContResId     *contResId;  /*!< Contention Resolution Identity */
#ifndef L2_OPTMZ
   Buffer          *tb;    /*!< Multiplexed Pdu */
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
   U32             tbSz;   /*!< TB size scheduled for the hqP */    
   RgPdcch         pdcch;  /*!< PDCCH allocated for this HQ Proc */
   /* Adding Array of sfLnk in case of repetitions because same lnk in mutiple
    * list causing issues*/
   RgTbSfLnkInfo   sfLnkInfo[2]; /*!< Used to queue it up on a subfrm */
   RgDlSf          *sf;
   CmLteTimingInfo timingInfo; /*!<This field holds the scheduled time */
   TknU32           doa;
   TfuTxMode        txMode;
   Bool              puschRptUsd; /*!< True if Precoding Info in PDCCH has to be
                                   in-accordance with the latest PUSCH report */
   TfuDlCqiPuschInfo puschPmiInfo;/*!< PUSCH report details for explicit PMI
                                       * information to PHY during a PDSCH */
   /* LTE_ADV_FLAG_REMOVED_START */
#ifdef TFU_UPGRADE
   U8            pa;                 /* DL Power control paramter P_A
                                     configured by higher layers
                                     Ref: RRC 36.331, 6.3.2,
                                     PDSCH-Config*/
#endif
   U8 isEnbSFR;
#ifdef LTE_ADV
   TknU8           sCellActCe;     /*!< SCell Act ce scheduled to be sent */
#endif
   /* LTE_ADV_FLAG_REMOVED_END */
}RgTbInfo;

/** 
  * @brief
  * Downlink HARQ process information per UE.
  */
struct rgDlHqProcCb
{
   U8              procId;    /*!< HARQ process ID */
/* Changes for MIMO feature addition */
/* Removed dependency on MIMO compile-time flag */
   U8              numOfTBs;
   RgTbInfo        tbInfo[RG_MAX_TB_PER_UE];
   U16             saveId; // laa
#ifdef LTE_L2_MEAS

   U32           tbId[RGU_MAX_TB];          /*!< RLC's internal tbId */                                    
   U16           status[RGU_MAX_TB];        /*!< Status back to RLC ACK/NACK*/
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
   U8           numHqProcs;
   RgDlHqProcCb *procs[RG_MAX_DL_HARQ_NUM]; /*!< Downlink harq processes */
   CmLListCp    savedProcLst[RG_MAX_DL_HARQ_NUM]; /*!< laa: Saved TBs for the release Hq Procs*/
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
   U8              numSCells; /*!< Used to check if config cfm rcvd from all Scells*/ 
   U8              cfgCfgCount; /*!< Keep count of cfg confirms recvd */
   U8              mask; /*!< Cfg cfm mask */
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
   U8           tmrRes;           /*!< Timer resolution */
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
   U8           numRguSaps;                /*!< Numb of RGU SAP Control Block */
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
   U8 bufCnt;                              /*!< Count of buffers in buffersToFree[] */
   Buffer *bufToFree[RG_MAX_DFRD_FREE_BUFS];/*!< List of Shared Buffers to be freed */
};

/* Declaration for RgCb */
EXTERN RgCb rgCb[RG_MAX_INST];

/** 
  * @brief
  * Subheader of MAC SDU.
  */
typedef struct rgMacSdu
{
   CmLList  sduLstEnt;  /*!< List entry for MAC SDU subheader */
   CmLteLcId       lcId;       /*!< Logical channel ID */
   U16      len;        /*!< Length of the SDU */
   Buffer   *mBuf;      /*!< SDU Buffer */
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
   U8    errType;    /*!< Error Type */
   U16   errCause;   /*!< Cause of Error */
};

/** 
  * @brief
  * Carries the Information to build PDU.
  */
typedef struct rgBldPduInfo
{
   Void           *datReq;    /*!< Common/Dedicated Dat Request */
   U8             reqType;    /*!< EVTRGUCDATREQ/EVTRGUDDATREQ */
   MsgLen         schdTbSz;   /*!< Schedule TB size */
   U8             tbIndex;    /*!< Transport Block Index */
   TknU8          ta;         /*!< Timing Advance */
   RgContResId    *contResId; /*!< Contention Resolution ID */
#ifdef LTE_ADV
   TknU8          sCellActCe; /*!< SCell Act CE info */
#endif
} RgBldPduInfo;



/* APIs exposed by LIM */

#ifdef LTE_L2_MEAS /*LTE_L2_MEAS_PHASE2*/
EXTERN S16 rgAddToL2MeasPerQci ARGS ((RgCellCb  *cell,U8 qci));
#endif
EXTERN S16 rgLIMTfuBndReq ARGS((
         Inst                 inst,
         SuId                 suId,
         SpId                 spId));

EXTERN S16 rgLIMTfuUbndReq ARGS((
         Inst                 inst,
         SuId                 suId,
         Reason               reason));

EXTERN S16 rgLIMTfuDatReq ARGS((
         Inst          inst,
         TfuDatReqInfo *datReq));
#ifdef L2_OPTMZ
EXTERN S16 rgLIMTfuDelDatReq ARGS((
         Inst          inst,
         TfuDelDatReqInfo *datReq));
#endif /*L2_OPTMZ */
/* APIs exposed by TOM */

EXTERN S16 rgTOMDatInd ARGS((
         Inst          inst,
         TfuDatIndInfo *datInd));

EXTERN Void rgTOMRlsSf ARGS((Inst inst,RgDlSf *dlSf));

EXTERN S16 rgSchMacSfAllocReq ARGS((Pst *pst, RgInfSfAlloc *sfInfo));

/*
 * APIs exposed by LMM
 */
EXTERN S16 rgLMMStartTmr ARGS((Inst inst,S16 tmrEvnt, U32 tmrVal, PTR cb));
EXTERN S16 rgLMMStopTmr ARGS((Inst inst,S16 tmrEvnt, PTR cb));
EXTERN S16 rgLMMTmrExpiry ARGS((PTR cb, S16 tmrEvnt));
/* To send a Unsolicited Status Indication to Layer Manager */
EXTERN S16 rgLMMStaInd ARGS((Inst inst,U16 category, U16 event, U16 cause, RgUstaDgn *dgn));
/* To send a Trace Indication to Layer Manager.
 * srcMbuf is not freed. caller has to take care. */
EXTERN Void rgLMMTrcInd ARGS((Inst inst,Buffer *srcMbuf, U8 event));
/* LIM uses this to forward the BndCfm to LMM. This function invokes a
 * Control Confirmation to the LM. */
EXTERN S16 rgLMMBndCfm ARGS((Pst *pst, SuId suId, U8 status));
EXTERN S16 rgActvTmr ARGS((Ent ent, Inst inst ));
EXTERN S16 rgActvInit ARGS((Ent entity, Inst inst, Region 
       region, Reason reason));
EXTERN S16 rgActvTsk ARGS((Pst *pst, Buffer *mBuf));

/* 
 * APIs exposed by CFG module
 */
EXTERN S16 rgCFGVldtCrgCellCfg ARGS((Inst inst,CrgCellCfg *cellCfg, RgErrInfo *errInfo));
EXTERN S16 rgCFGVldtCrgUeCfg ARGS((Inst inst,CrgUeCfg  *ueCfg, RgCellCb  **cell, RgErrInfo
      *errInfo));
EXTERN S16 rgCFGVldtCrgLcCfg ARGS((Inst inst,CrgLchCfg *lcCfg, RgCellCb **cell, RgUeCb
      **ue, RgErrInfo  *errInfo));
EXTERN S16 rgCFGVldtCrgCellRecfg ARGS((Inst inst,CrgCellRecfg *cellRecfg, RgCellCb **cell,
      RgErrInfo *errInfo));
EXTERN S16 rgCFGVldtCrgUeRecfg ARGS((Inst inst,CrgUeRecfg *ueRecfg, RgCellCb **cell,
      RgUeCb **ue, RgErrInfo *errInfo));
EXTERN S16 rgCFGVldtCrgLcRecfg ARGS((Inst inst,CrgLchRecfg *lcRecfg, RgCellCb **cell,
      RgUeCb **ue, RgUlLcCb **ulLc, RgErrInfo *errInfo));
EXTERN S16 rgCFGVldtCrgUeReset ARGS ((Inst inst,CrgRst *reset,RgCellCb **cell,RgUeCb **ue,
         RgErrInfo   *errInfo));

EXTERN S16 rgCFGCrgCellCfg ARGS((Inst inst,CrgCellCfg *cellCfg, RgErrInfo *errInfo));
EXTERN S16 rgCFGCrgUeCfg ARGS((Inst inst,RgCellCb  *cell, CrgUeCfg  *ueCfg, RgErrInfo
      *errInfo));

#ifdef LTE_ADV
EXTERN S16 rgFillAndAddSCellCfg ARGS((Inst inst, RgCellCb  *cell, 
         CrgUeRecfg *ueRecfg, CrgCfgTransId transId, Bool *isCfmRqrd));
EXTERN S16 rgDelUeFrmAllSCell ARGS(( RgCellCb       *cell, RgUeCb         *ue));
EXTERN S16 rgUtlVltdAddSCellCfg ARGS ((RgPrgUeSCellCfgInfo *ueSCellCb,
                                       RgCellCb    *cell,
                                       Inst        inst
                                     ));
EXTERN S16 rgCfgAddUeSCellCfg ARGS ((Inst        dstMacInst,
                                    RgPrgUeSCellCfgInfo *ueSCellCb,
                                    RgCellCb    *cell
                                  ));
#endif /* LTE_ADV */

EXTERN S16 rgCFGCrgLcCfg ARGS((Inst inst,RgCellCb *cell, RgUeCb *ue, CrgLchCfg
      *lcCfg, RgErrInfo *errInfo,Bool *isCfmRqrd,CrgCfgTransId   transId));
EXTERN S16 rgCFGCrgCellRecfg ARGS((Inst inst,RgCellCb *cell, CrgCellRecfg *cellRecfg,
      RgErrInfo *errInfo));
EXTERN S16 rgCFGCrgUeRecfg ARGS((Inst inst,RgCellCb *cell, RgUeCb *ue, CrgUeRecfg
      *ueRecfg, RgErrInfo *errInfo));
EXTERN S16 rgCFGCrgLcRecfg ARGS((Inst inst,RgCellCb *cell, RgUeCb *ue, RgUlLcCb
      *ulLc, CrgLchRecfg *lcRecfg, RgErrInfo *errInfo, Bool *isCfmRqrd));
EXTERN S16 rgCFGCrgCellDel ARGS((Inst inst,CrgDel *cellDelInfo, RgErrInfo *errInfo));
EXTERN S16 rgCFGCrgUeDel ARGS((Inst inst,CrgDel *ueDelInfo, RgErrInfo *errInfo));
EXTERN S16 rgCFGCrgLcDel ARGS((Inst inst,CrgDel *lcDelInfo, RgErrInfo
         *errInfo,Bool *isCfmRqrd,CrgCfgTransId transId));
EXTERN Void rgCFGFreeCellCb ARGS((RgCellCb *cell));
EXTERN Void rgCFGFreeInactvCellCb ARGS((RgCellCb *cell));
EXTERN S16 rgSchMacCellRegReq ARGS((Pst* pst,RgInfCellReg* regReq));
EXTERN S16 rgCFGCrgUeReset ARGS((RgCellCb *cell,RgUeCb *ue,CrgRst *reset,
         RgErrInfo *errInfo));

/* 
 * APIs exposed by COM module
 */
EXTERN S16 rgCOMCfgReq ARGS((Inst inst,CrgCfgTransId transId, CrgCfgReqInfo
         *cfgReqInfo));

/* APIs Exposed by UIM */
EXTERN S16 rgUIMRguBndCfm ARGS ((Inst inst,SuId suId, U8 status));
EXTERN S16 rgUIMSndDedStaInd ARGS ((Inst inst,RgUpSapCb *rguDlSap,RgRguDedStaInd  *staInd));
EXTERN S16 rgUIMSndCmnStaInd ARGS ((Inst inst,RgUpSapCb *rguDlSap,RgRguCmnStaInd  *staInd));
EXTERN S16 rgUIMSndDedDatInd ARGS ((Inst inst,RgUpSapCb *rguDlSap,RgRguDedDatInd  *datInd));
#ifdef LTE_L2_MEAS
EXTERN S16 rgL2mCreateMeasCb ARGS((
         RgCellCb       *cell,
         RgInfL2MeasReq *measInfo, 
         U8              measType,
         RgErrInfo      *err
));
EXTERN  S16 rgL2mMeasReq ARGS((
         RgCellCb       *cell,
         RgInfL2MeasReq *measInfo,
         RgErrInfo      *err
));
EXTERN S16 rgL2Meas ARGS ((
         RgCellCb  *cell
));
#endif /* LTE_L2_MEAS */
EXTERN S16 rgUIMSndCmnDatInd ARGS ((Inst inst,RgUpSapCb *rguSap,RgRguCmnDatInd  *datInd));
EXTERN S16 rgUIMCrgBndCfm ARGS ((Inst inst,SuId suId,U8 status));
EXTERN S16 rgUIMCrgCfgCfm ARGS ((Inst inst,CrgCfgTransId transId,U8 status));


/* 
 * APIs exposed by ROM module
 */
EXTERN S16 rgROMDedDatReq ARGS((Inst inst,RgRguDedDatReq *datReq));
EXTERN S16 rgROMCmnDatReq ARGS((Inst inst,RgRguCmnDatReq *datReq));
EXTERN S16 rgROMDedStaRsp ARGS((Inst inst,RgRguDedStaRsp *staRsp));
EXTERN S16 rgROMCmnStaRsp ARGS((Inst inst,RgRguCmnStaRsp *staRsp));
#ifdef LTE_L2_MEAS

EXTERN S16 rgROML2MUlThrpMeasReq ARGS((Inst inst,RgRguL2MUlThrpMeasReq *measReq));

#endif

/* 
 * APIs exposed by RAM module
 */
EXTERN RgUeCb* rgRAMCreateUeCb ARGS((RgCellCb *cell,
                     CmLteRnti tmpCrnti,Bool insert, RgErrInfo *err));
EXTERN S16 rgRAMFreeCell ARGS((RgCellCb    *cell));
EXTERN Void rgRAMFreeUeCb ARGS((Inst inst,RgUeCb *ueCb));
EXTERN S16 rgHndlRaResp ARGS((RgCellCb *cell,CmLteTimingInfo timingInfo,
            RgInfRarInfo  *rarInfo,RgErrInfo *err));
EXTERN S16 rgSchMacRlsRntiReq ARGS((Pst *pst,RgInfRlsRnti* rlsRnti));
/* 
 * APIs exposed by UHM module
 */
EXTERN Void rgUHMCrgUeCfg ARGS(( RgCellCb *cellCb, RgUeCb *ueCb, 
                                 CrgUeCfg *ueCfg));
EXTERN Void rgUHMCrgUeRecfg ARGS(( RgCellCb *cellCb, RgUeCb *ueCb, 
                              CrgUeRecfg *ueRecfg));
/* 
 * APIs exposed by DUX module
 */
EXTERN S16 rgDUXDemuxData ARGS((Inst inst,RgMacPdu *pdu, RgInfCeInfo   *ceInfo,
                                Buffer **mBuf, RgErrInfo *err)); 
EXTERN S16 rgDUXExtSduLen ARGS
((
Inst        inst,
Buffer      *mBuf,
U16         *sduLen,
RgErrInfo   *err
));

/* 
 * APIs exposed by DBM module
 */

EXTERN S16 rgDBMInitInactvCellLst ARGS((U16 numBins));
EXTERN S16 rgDBMDeInitInactvCellLst ARGS((Void));
EXTERN S16 rgDBMInsInactvCellCb ARGS(( RgCellCb *cellCb));
EXTERN RgCellCb* rgDBMGetInactvCellCb ARGS(( CmLteCellId cellId));
EXTERN S16 rgDBMDelInactvCellCb ARGS(( RgCellCb *cellCb));
EXTERN RgCellCb* rgDBMGetNextInactvCellCb ARGS((RgCellCb *cellCb));

EXTERN S16 rgDBMInitCellLst ARGS(( U16 numBins));
EXTERN S16 rgDBMDeInitCellLst ARGS((Void));
EXTERN S16 rgDBMInsCellCb ARGS(( RgCellCb *cellCb));
EXTERN RgCellCb* rgDBMGetCellCb ARGS(( CmLteCellId cellId));
EXTERN S16 rgDBMDelCellCb ARGS(( RgCellCb *cellCb));
EXTERN RgCellCb* rgDBMGetNextCellCb ARGS((RgCellCb *cellCb));
EXTERN S16 rgDBMInitCell ARGS((RgCellCb *cellCb));

EXTERN S16 rgDBMDeInitUeCbLst ARGS(( RgCellCb *cellCb));
EXTERN S16 rgDBMInsUeCb ARGS(( RgCellCb *cellCb, RgUeCb *ueCb));
EXTERN RgUeCb* rgDBMGetUeCb ARGS(( RgCellCb *cellCb, CmLteRnti ueId));
EXTERN RgUeCb* rgDBMGetNextUeCb ( RgCellCb *cellCb, RgUeCb *ueCb);
EXTERN S16 rgDBMDelUeCb ARGS(( RgCellCb *cellCb, RgUeCb *ueCb));
EXTERN Void rgDBMInitUe ARGS((RgUeCb *ueCb));

#ifdef LTE_L2_MEAS
EXTERN Void rgDBMInsUlDedLcCb ARGS((RgUeCb *ueCb, CmLteLcId idx, LteLcgId  gId,
                        U8  qci));
#else
EXTERN Void rgDBMInsUlDedLcCb ARGS((RgUeCb *ueCb, 
                  CmLteLcId idx, LteLcgId gId));
#endif
EXTERN Void rgDBMUpdUlDedLcCb ARGS((RgUeCb *ueCb, 
               RgUlLcCb *ulLcCb, LteLcgId gId));

EXTERN Void rgDBMInsDlDedLcCb ARGS((RgUeCb *ueCb, CmLteLcId idx));
EXTERN Void rgDBMDelUlDedLcCb ARGS((RgUeCb *ueCb, RgUlLcCb *ulLcCb));
EXTERN Void rgDBMDelDlDedLcCb ARGS((RgUeCb *ueCb, RgDlLcCb *dlLcCb));
EXTERN RgDlLcCb* rgDBMGetDlDedLcCb ARGS(( RgUeCb *ueCb, CmLteLcId idx));
EXTERN RgUlLcCb* rgDBMGetUlDedLcCb ARGS(( RgUeCb *ueCb, CmLteLcId idx));
EXTERN Void rgDBMFreeCmnLcLst ARGS((RgCellCb *cellCb));
EXTERN S16 rgDBMChkCmnLcCb ARGS(( RgCellCb *cellCb, CmLteLcId lcId));
EXTERN RgBcchBchLcCb* rgDBMGetBcchOnBch ARGS(( RgCellCb *cellCb ));
EXTERN RgBcchDlschLcCb* rgDBMGetBcchOnDlsch ARGS(( RgCellCb *cellCb, 
                                                   CmLteLcId lcId));
EXTERN RgPcchLcCb* rgDBMGetPcch ARGS(( RgCellCb *cellCb));
EXTERN Void rgDBMInsBcchOnBch ARGS(( RgCellCb *cellCb, CmLteLcId idx));
EXTERN Void rgDBMInsBcchOnDlsch ARGS(( RgCellCb *cellCb, CmLteLcId idx));
EXTERN Void rgDBMInsPcch ARGS(( RgCellCb *cellCb, CmLteLcId idx));

EXTERN Void rgDBMInsUeCbInRachLst ARGS((RgCellCb *cellCb,RgUeCb *ueCb));
EXTERN RgUeCb* rgDBMGetUeCbFromRachLst ARGS((RgCellCb *cellCb,CmLteRnti key));
EXTERN RgUeCb* rgDBMGetNextUeCbFromRachLst ARGS((RgCellCb *cellCb,
                                                      RgUeCb *ueCb));
EXTERN Void rgDBMDelUeCbFromRachLst ARGS((RgCellCb *cellCb,RgUeCb *ueCb));
/* Added support for SPS*/
#ifdef LTEMAC_SPS
EXTERN S16 rgDBMDeInitSpsUeCbLst ARGS((RgCellCb *cellCb));
EXTERN S16 rgDBMInsSpsUeCb ARGS((RgCellCb *cellCb, RgUeCb *ueCb));
EXTERN RgUeCb* rgDBMGetSpsUeCb ARGS((RgCellCb *cellCb, CmLteRnti ueId));
EXTERN RgUeCb* rgDBMGetNextSpsUeCb ARGS((RgCellCb *cellCb, RgUeCb *ueCb));
EXTERN S16 rgDBMDelSpsUeCb ARGS((RgCellCb *cellCb,RgUeCb *ueCb));
#endif /* LTEMAC_SPS */
                        
/* 
 * APIs exposed by DHM module
 */
EXTERN Void rgCFGFreeUeCb ARGS((RgCellCb *cell, RgUeCb *ue));
EXTERN S16 rgDHMHqEntInit ARGS((Inst inst,RgDlHqEnt *hqE, U8 maxHqProcs));
EXTERN Void rgDHMRlsHqProc ARGS((RgDlHqProcCb *hqP));
EXTERN Void rgDHMUeReset ARGS ((RgCellCb *cell, RgDlHqEnt *hqE));
EXTERN Void rgDHMHdlBufFree ARGS((Inst inst, Buffer **mBuf));
EXTERN Void rgDHMFreeTbBufs ARGS((Inst inst));
EXTERN S16 rgDHMRlsHqProcTB ARGS((RgCellCb *cell, RgDlHqProcCb *hqP,U8 tbIndex));
EXTERN S16 rgDHMGetHqProcFrmId ARGS((RgUeCb *ue, U8 idx, RgDlHqProcCb **hqP));
/* Changes for MIMO feature addition */
/* Removed dependency on MIMO compile-time flag */
EXTERN S16 rgDHMSndDatReq ARGS((RgCellCb *cellCb, RgDlSf *dlSf, RgTfuDatReqInfo *datInfo, 
                                RgDlHqProcCb *hqProc, RgErrInfo *err));
EXTERN S16 rgDHMHndlDedDatReq ARGS((Inst inst,RgDlHqProcCb *hqProc, 
                              RgRguDDatReqPerUe *datReq, RgDlSf *dlSf, RgErrInfo *err));
/* Changed for contention resolution timer implementation*/
EXTERN S16 rgDHMHndlCmnDatReq ARGS((Inst inst,RgDlHqProcCb *hqProc, 
                RgRguCmnDatReq *datReq, RgErrInfo *err));
/* Changes for MIMO feature addition */
/* Removed dependency on MIMO compile-time flag */

EXTERN S16 rgDHMSndConsolidatedStaInd ARGS ((RgCellCb *cell,RgInfUeInfo *ueInfo,
      CmLteTimingInfo timingInfo, RgErrInfo *err));

EXTERN Void rgDHMFreeUe ARGS((Inst inst,RgDlHqEnt *hqE));
EXTERN S16 rgSchMacRlsHqReq ARGS((Pst *pst, RgInfRlsHqInfo *rlshqUeInfo));

/* 
 * APIs exposed by MUX module
 */
#ifndef L2_OPTMZ
EXTERN S16 rgMUXBldPdu ARGS((Inst inst, RgBldPduInfo *bldPdu, Buffer **txPdu, 
         RgErrInfo *err));
#else
EXTERN S16 rgMUXBldPdu ARGS((Inst inst, RgBldPduInfo *bldPdu, RgTfuDatReqTbInfo *tb, 
         RgErrInfo *err));
EXTERN Bool RgUtlIsTbMuxed ARGS((TfuDatReqTbInfo *tb));
#endif
EXTERN S16 rgMUXBldRarPdu ARGS((RgCellCb *cell, RgInfRaRntiInfo *alloc,
         Buffer **txPdu, RgErrInfo *err));

/*
 * Utility APIs
 */
EXTERN S16  rgAllocSBuf     ARGS((Inst inst,Data **pData, Size size));
EXTERN S16  rgGetMsg        ARGS((Inst inst,Buffer **mBuf));
/*ccpu00117052 - MOD  Passing double pointer for proper NULLP
                      assignment */
EXTERN Void rgFreeSBuf      ARGS((Inst inst,Data **data, Size size));
EXTERN Void rgFillDgnParams ARGS((Inst inst,RgUstaDgn *dgn,U8 dgnType));
EXTERN Void rgUpdtRguDedSts ARGS((Inst inst,RgUpSapCb *rguSap,U8 stsType, RgRguDedDatReq *datReq));
EXTERN Void rgUpdtRguCmnSts ARGS((Inst inst,RgUpSapCb *rguSap,U8 stsType));
EXTERN Void rgUpdtCellCnt   ARGS((Inst inst,U8 updtType));
EXTERN Void rgUpdtUeCnt     ARGS((Inst inst,U8 updtType));
EXTERN Void rgGetPstToInst ARGS((Pst *pst,Inst srcInst, Inst dstInst));
EXTERN S16 rgAllocEventMem ARGS((Inst inst,Ptr *memPtr,Size memSize));
EXTERN S16 rgGetEventMem ARGS((Inst inst,Ptr *ptr,Size len,Ptr memCp));
EXTERN S16 rgAllocShrablSBuf ARGS((Inst inst,Data **pData, Size size));
EXTERN Void rgPrintfSubFrameInfo ARGS((RgDlSf *dlSf));
EXTERN Void printMacCellInfo ARGS((Void));
EXTERN Void rgFreeSharableSBuf ARGS((Inst inst,Data **data, Size size)); 
EXTERN void MacSendCellCfgCfm(uint8_t response);


EXTERN Void rgDHMFreeAllTbBufs ARGS
((
Inst inst
));

EXTERN S16 rgMUXAddPadd ARGS((
Inst           inst,
MsgLen         *schdTbSz,
Buffer         *sduBuf,
Bool           isRar,
RgErrInfo      *err
));
EXTERN U16 rgMUXCalcRiv ARGS
((
U8           bw,
U8           rbStart,
U8           numRb
));



#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __RGX__ */


/**********************************************************************
         End of file
**********************************************************************/
