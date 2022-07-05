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
 
    Type:   C include file
 
    Desc:   Defines required by LTE MAC
 
    File:   lrg.x
 
**********************************************************************/
/**
  @file lrg.x
  @brief Structure declarations and definitions for LRG interface.
  */
#ifndef __LRGX__
#define __LRGX__


/* ccpu00121930 : ADD: c++ support*/
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/** 
 * @brief
   This structure holds configuration parameters for MAC General Configuration. */
typedef struct rgGenCfg
{
   Pst      lmPst;      /*!< Layer manager post structure */
   MemoryId mem;        /*!< Region and pool ID */
   uint8_t       tmrRes;     /*!< Timer resolution for Low SAP Bind Timer */
   uint8_t       numRguSaps; /*!< Number of RGU SAP's */
   uint8_t       startCellId;              /*!< Start cell id value */
#ifdef LTE_ADV
   Bool     forceCntrlSrbBoOnPCel;   /*!< value 1 means scheduler will schedule
                                          RLC control BO and SRBs only on PCEll
                                          else it will schedule on both PCell
                                          and SCell */
   Bool     isSCellActDeactAlgoEnable; /*!< TRUE will enable activation/deactivation algo at Schd */
#endif
}RgGenCfg;

/** 
 * @brief
   This structure holds configuration parameters for MAC Upper SAP Configuration. */
typedef struct rgUpSapCfg
{
   Selector selector;   /*!< Selector */
   MemoryId mem;        /*!< Region and pool ID */
   Priority prior;      /*!< Priority */
   ProcId   procId;     /*!< Processor ID */
   Ent      ent;        /*!< Entity ID */
   Inst     inst;       /*!< Instance ID */
   Route    route;      /*!< Route */
   SpId     spId;       /*!< Service provider ID */
   SuId     suId;       /*!< Service user ID */
}RgUpSapCfg;

/** 
 * @brief
   This structure holds configuration parameters for MAC Lower SAP Configuration. */
typedef struct rgLowSapCfg
{
   Selector selector;   /*!< Selector */
   MemoryId mem;        /*!< Region and pool ID */
   Priority prior;      /*!< Priority */
   ProcId   procId;     /*!< Processor ID */
   Ent      ent;        /*!< Entity ID */
   Inst     inst;       /*!< Instance ID */
   Route    route;      /*!< Route */
   SpId     spId;       /*!< Service provider ID */
   SuId     suId;       /*!< Service user ID */
   TmrCfg   bndTmr;     /*!< Bind Timer Value */
}RgLowSapCfg;

#ifdef MAC_SCH_STATS
/* Data structures */
typedef struct rgAckNack
{
   uint8_t           mcs;
   uint16_t          numOfNacks;
   uint16_t          numOfAcks;
} RgAckNack;

typedef struct rgSchNackAckStats
{
   RgAckNack dlCqiStat[15];   /*!< DL Stats */
   RgAckNack ulCqiStat[15];   /*!< UL stats */
} RgSchNackAckStats;

typedef struct rgSchHqNumRetx
{
   uint8_t           mcs;
   uint16_t          numOfHQ_1;
   uint16_t          numOfHQ_2;
   uint16_t          numOfHQ_3;
   uint16_t          numOfHQ_4;
   uint32_t          totalTx;
} RgSchHqNumRetx;

typedef struct rgSchHqRetxStats
{
   RgSchHqNumRetx    dlCqiStat[15];   /*!< DL Stats */
   RgSchHqNumRetx    ulCqiStat[15];   /*!< UL stats */
} RgSchHqRetxStats;

RgSchNackAckStats hqFailStats;
RgSchHqRetxStats  hqRetxStats;
#endif /* MAC_SCH_STATS */

/** 
 * @brief
   This structure holds General statistical information of MAC. */
typedef struct rgGenSts
{
   uint32_t numHarqFail;      /*!< Number of HARQ failures */
   uint32_t numUeCfg;         /*!< Number of UEs configured */
   uint16_t numCellCfg;       /*!< Number of Cells configured */
#ifdef MAC_SCH_STATS
   RgSchNackAckStats nackAckStats;
   RgSchHqRetxStats  hqRetxStats;
#endif /* MAC_SCH_STATS */
}RgGenSts;

/** 
 * @brief
   This structure holds statistical information of a SAP in MAC. */
typedef struct rgSapSts
{
   uint32_t numPduRcvd;       /*!< Number of PDUs received. At TFU, PDU refers to TB,
                              and at RGU it is SDU. */
   uint32_t numPduTxmit;      /*!< Number of PDUs transmitted. This field is 
                              applicable to TFU. */
   uint32_t numPduDrop;       /*!< Number of PDUs dropped. At TFU, PDU refers to TB,
                              and at RGU it is SDU. */
}RgSapSts;

typedef struct rgSchInstCfg
{
   uint8_t          instId;    /*!< Sheduler instance ID */
   RgGenCfg    genCfg;    /*!< General Configuration for the scheduler instance */
   uint8_t          numSaps;   /*!< Number of RGR and TFU SAPs must be same */
   RgUpSapCfg  rgrSap[LRG_MAX_SAPS_PER_INST];  /*!< RGR interface SAPS */
   RgUpSapCfg  rgmSap[LRG_MAX_SAPS_PER_INST];  /*!< RGR interface SAPS */
   RgLowSapCfg tfuSap[LRG_MAX_SAPS_PER_INST];  /*!< TFU interface SAPS */
} RgSchInstCfg;

/** 
 * @brief
   This structure holds configuration parameters for MAC. */
typedef struct rgCfg
{
   union
   {
      RgGenCfg    genCfg;  /*!< General Configuration */
      /* SAPs for the MAC layer */
      RgUpSapCfg  rguSap;  /*!< RGU interface SAP */
      RgUpSapCfg  crgSap;  /*!< CRG interface SAP */
      RgLowSapCfg tfuSap;  /*!< TFU interface SAP */
      RgSchInstCfg schInstCfg;/*!< General Configuration for scheduler instances */
   }s;
}RgCfg;

/** 
 * @brief
   This structure holds a SAP's status information. */
typedef struct rgSapSta
{
   uint8_t sapState;         /*!< SAP state */
}RgSapSta;

/** 
 * @brief
   This structure holds MAC's statistical information. */
typedef struct rgSts
{
   DateTime dt;          /*!< Date and Time. */
   uint8_t       sapInst;     /*!< SAP instance. */
   Action   action;      /*!< Action on Trafffic load related statistic values.
                              ARST: To reset the statistic values. */
   union
   {
      RgGenSts genSts;   /*!< General statistics. */
      RgSapSts rguSts;   /*!< RGU SAP statistics. */
      RgSapSts crgSts;   /*!< CRG SAP statistics. */
      RgSapSts rgrSts;   /*!< RGR SAP statistics. */
      RgSapSts tfuSts;   /*!< TFU SAP statistics. */
   }s;
}RgSts;

/** 
 * @brief
   This structure holds MAC's solicited status information. */
typedef struct rgSsta
{
   DateTime dt;             /*!< Date and time */
   uint8_t       sapInst;        /*!< SAP instance */
   union
   {
      SystemId sysId;       /*!< System information */
      RgSapSta rguSapSta;   /*!< RGU SAP state */
      RgSapSta crgSapSta;   /*!< CRG SAP state */
      RgSapSta rgrSapSta;   /*!< RGR SAP state */
      RgSapSta rgmSapSta;   /*!< RGM SAP state */
      RgSapSta tfuSapSta;   /*!< TFU SAP state */
   }s;
}RgSsta;

/** 
 * @brief
   Alarm diagnostics structure. */
typedef struct rgUstaDgn
{
   uint8_t          type;         /*!< Diagnostics Type */
   union
   {
   /*lrg_x_001.main_3 - Changed for documentation*/
      MemoryId mem;          /*!< Memory pool and region <BR> 
                             LRG_USTA_DGNVAL_MEM Dynamic memory allocation failure. <BR> 
                             LRG_USTS_DGNVAL_HARQ Harq Process is busy.
                              */
   }u;
} RgUstaDgn;

/** 
 * @brief
   This structure holds MAC's Unsolicited Status information. */
typedef struct rgUsta
{
   CmAlarm   cmAlarm;      /*!< Alarms */
   RgUstaDgn dgn;          /*!< Alarm diagnostics */
}RgUsta;

/** 
 * @brief
   This structure holds MAC's Trace Indication information. */
typedef struct rgTrc
{
   DateTime dt;          /*!< Date and time */
   /*lrg_x_001.main_3 - changed for documentation*/
   uint8_t       evnt;        /*!< Event <BR> 
                           EVTRGUDATREQ Trace for Dedicated channel Data Request. <BR> 
                           EVTRGUCDATREQ Trace for common channel data request.<BR> 
                          */
}RgTrc;

/** 
 * @brief
   This structure holds MAC's Debug Control information. */
typedef struct rgDbgCntrl
{
   uint32_t dbgMask;          /*!< iThe Layer Manager electively enables or disables various levels of Debug printing <BR>
                          Following are the values: <BR> 
                          DBGMASK_PRM Enable/Disable function parameter debug prints <BR>
                          DBGMASK_ERR Enable/Disable error prints <BR>
                          DBGMASK_INFO Enable/Disable informational prints.
                          */
}RgDbgCntrl;

/** 
 * @brief
   This structure holds MAC's SAP Control information. */
typedef struct rgSapCntrl
{
   SuId suId;            /*!< Service user ID */
   SpId spId;            /*!< Service provider ID */
}RgSapCntrl;

#ifdef PHY_ERROR_LOGING
typedef struct rgSchUlAllocCntrl
{
   uint8_t  mcs;
   uint16_t numOfRb;
   uint16_t rbStart;
   Bool testStart;
   Bool enaLog;
   uint16_t  logTime;
}RgSchUlAllocCntrl; 
#endif

/** 
 * @brief
   This structure holds MAC's Control information. */
typedef struct rgCntrl
{
   DateTime      dt;          /*!< Date and Time */
   uint8_t            action;      /*!< Action */
   uint8_t            subAction;   /*!< Sub-action */
   uint8_t            instId;      /*!< Scheduler instance ID */
   union
   {
      RgDbgCntrl rgDbgCntrl;  /*!< Debug Control */
      /*lrg_x_001.main_3 - Changed for documentation.*/
      S16        trcLen;      /*!< Trace Length <BR> 
                               i)LRG_FULL_TRACE Give full message <BR> 
                               ii)LRG_NO_TRACE Disable Trace<BR>
                               iii)Or any valid S16 value in case where only the specified number
                               of bytes, as indicated by trcLen, are to be sent.*/
      RgSapCntrl rgSapCntrl;  /*!< SAP Control */
      uint32_t        logMask;     /*!<  Logging control Mask */
#ifdef PHY_ERROR_LOGING
      RgSchUlAllocCntrl rgSchUlAllocCntrl; /* For setting MCS,Number of RB and RB start */
#endif
   }s;
}RgCntrl; 

/** 
 * @brief
   This structure holds MAC's Configuration and Control Management Information. */
typedef struct rgMngmt
{
   Header     hdr;       /*!< Header */
   CmStatus   cfm;       /*!< Confirmation */
   union
   {
      RgCfg   cfg;       /*!< Configuration */
      RgSts   sts;       /*!< Statistics */
      RgSsta  ssta;      /*!< Solicited Status */
      RgUsta  usta;      /*!< Unsolicited Status */
      RgTrc   trc;       /*!< Trace */
      RgCntrl cntrl;     /*!< Control */
   }t;
}RgMngmt;

/* lrg_x_001.main_3 - ADD - Data structures for LTE_L2_MEAS */
#ifdef LTE_L2_MEAS
/**
 *  @brief
   To measure Average Number of PRB's used per QCI for a time period
   timePrd
*/
typedef struct lrgAvgPrbQCI   
{
   uint8_t        numQci;                 /*!< Numner of QCI's in requests */
   uint8_t        qci[LRG_MAX_QCI_PER_REQ];  /*!< QCI for which PRB has to be measured */
} LrgAvgPrbQCI;

/**
 *  @brief
   To measure number of Active UE's per QCI for the given time period
   timePrd 
**/
typedef struct lrgNmbActvUeQCI
{
   uint8_t       sampPrd;                   /*!< sampling prd for which active UE's measured
                                          Where sampling period is in milli seconds
                                          value can be at most 100ms */
   uint8_t        numQci;                   /*!< Numner of QCI's in requests */
   uint8_t        qci[LRG_MAX_QCI_PER_REQ]; /*!< QCI for which UE has to be considered */
} LrgNmbActvUeQCI;

/**
 * * @brief 
     This structure will be used by Layer Manager to L2 Measurement Request
     Information 
**/
typedef struct lrgSchMeasReqInfo
{
   Header           hdr;             /*!< Header */
   uint16_t              measType;        /*!< For type of measurement Following are the */
                                     /*!< allowed values */
                                     /*!< LRG_L2MEAS_AVG_PRB_DL , LRG_L2MEAS_AVG_PRB_UL*/
                                     /*!< LRG_L2MEAS_AVG_PRB_PER_QCI_DL  */
                                     /*!< LRG_L2MEAS_AVG_PRB_PER_QCI_UL */
                                     /*!< LRG_L2MEAS_RA_PREAMBLE  */
                                     /*!< LRG_L2MEAS_NMB_ACTV_UE_PER_QCI_UL */
                                     /*!< LRG_L2MEAS_NMB_ACTV_UE_PER_QCI_DL */
                                     /*!< LRG_L2MEAS_TB_TRANS_DL_COUNT */
                                     /*!< LRG_L2MEAS_TB_TRANS_DL_FAULTY_COUNT */
                                     /*!< LRG_L2MEAS_TB_TRANS_UL_COUNT */
                                     /*!< LRG_L2MEAS_TB_TRANS_UL_FAULTY_COUNT */
   uint32_t              timePrd;         /*!< Time period UNITS and value will differ depending
                                       on the action. Might be milli seconds/seconds. */
   CmLteCellId      cellId;          /*!< CELL Id for which measurement is Done */
   LrgAvgPrbQCI     avgPrbQciUl;     /*!< Average PRB usage per QCI in UL */
   LrgAvgPrbQCI     avgPrbQciDl;     /*!< Average PRB usage per QCI in DL */
   LrgNmbActvUeQCI  nmbActvUeQciUl;  /*!< Number of active UE's per QCI in UL */
   LrgNmbActvUeQCI  nmbActvUeQciDl;  /*!< Number of active UE's per QCI in DL */
} LrgSchMeasReqInfo;

typedef struct lrgSchMeasSndReqInfo
{
   Header           hdr;             /*!< Header */
   uint16_t              measType;        /*!< For type of measurement Following are the */
                                     /*!< allowed values */
                                     /*!< LRG_L2MEAS_AVG_PRB_DL , LRG_L2MEAS_AVG_PRB_UL*/
                                     /*!< LRG_L2MEAS_AVG_PRB_PER_QCI_DL  */
                                     /*!< LRG_L2MEAS_AVG_PRB_PER_QCI_UL */
                                     /*!< LRG_L2MEAS_RA_PREAMBLE  */
                                     /*!< LRG_L2MEAS_NMB_ACTV_UE_PER_QCI_UL */
                                     /*!< LRG_L2MEAS_NMB_ACTV_UE_PER_QCI_DL */
   uint32_t              timePrd;         /*!< Time period UNITS and value will differ depending
                                       on the action. Might be milli seconds/seconds. */
   CmLteCellId      cellId;          /*!< CELL Id for which measurement is Done */
 //  LrgAvgPrbQCI     avgPrbQciUl;     /*!< Average PRB usage per QCI in UL */
 //  LrgAvgPrbQCI     avgPrbQciDl;     /*!< Average PRB usage per QCI in DL */
 //  LrgNmbActvUeQCI  nmbActvUeQciUl;  /*!< Number of active UE's per QCI in UL */
 //  LrgNmbActvUeQCI  nmbActvUeQciDl;  /*!< Number of active UE's per QCI in DL */
} LrgSchMeasSndReqInfo;

typedef struct lrgSchMeasStopReqInfo
{
   Header           hdr;             /*!< Header */
   uint16_t              measType;        /*!< For type of measurement Following are the */
                                     /*!< allowed values */
                                     /*!< LRG_L2MEAS_AVG_PRB_DL , LRG_L2MEAS_AVG_PRB_UL*/
                                     /*!< LRG_L2MEAS_AVG_PRB_PER_QCI_DL  */
                                     /*!< LRG_L2MEAS_AVG_PRB_PER_QCI_UL */
                                     /*!< LRG_L2MEAS_RA_PREAMBLE  */
                                     /*!< LRG_L2MEAS_NMB_ACTV_UE_PER_QCI_UL */
                                     /*!< LRG_L2MEAS_NMB_ACTV_UE_PER_QCI_DL */
 /*  uint16_t              timePrd;  */       /*!< Time period UNITS and value will differ depending
                                       on the action. Might be milli seconds/seconds. */
   CmLteCellId      cellId;          /*!< CELL Id for which measurement is Done */
 /*  LrgAvgPrbQCI     avgPrbQciUl; */     /*!< Average PRB usage per QCI in UL */
 //  LrgAvgPrbQCI     avgPrbQciDl;     /*!< Average PRB usage per QCI in DL */
 //  LrgNmbActvUeQCI  nmbActvUeQciUl;  /*!< Number of active UE's per QCI in UL */
 //   LrgNmbActvUeQCI  nmbActvUeQciDl;  /*!< Number of active UE's per QCI in DL */
} LrgSchMeasStopReqInfo;

/**
 *@brief
 Structure to hold Received Random Access Preambles.
**/
typedef struct lrgRaPreambles
{
   uint16_t    dedPreambles;        /*!< Dedicated RA Preamble received */
   uint16_t    randSelPreLowRange;  /*!< Randomly selected preambles in low range */
   uint16_t    randSelPreHighRange; /*!< Randomly selected preambles in high range */
} LrgRaPreamblesCfm;

/**
 * @brief
 * Staructure to send Avg PRB for a given Time Period 
 * */
typedef struct lrgAvgPrbcfm
{
   uint8_t    prbPerc;      /*!< PRB usage in percentage for UL */
} LrgAvgPrbCfm;

/*LRG : Review Tag*/
typedef struct prbPercQci
{
	uint8_t qciValue;
	uint8_t prbPercQci;
}PrbPercQci;

typedef struct numActvUeQci
{
	uint8_t qciValue;
	uint8_t numActvUeQci;
}NumActvUeQci;
/*LRG : Review Tag*/
/**
 * @brief
 * Structure to send Avg PRB for a given period per QCI
 * */
typedef struct lrgAvgPrbQCICfm
{
   uint8_t    numQci;                             /*!< number of QCI */
/*LRG : Review Tag*/
   PrbPercQci    prbPercQci[LRG_MAX_QCI_PER_REQ];    /*!< PRB usage in percentage per QCI for UL/DL */
/*LRG : Review Tag*/
} LrgAvgPrbQCICfm;

/**
 *@brief
 Structure to send number of Active UE's per QCI
 */
typedef struct lrgNumActvUeQCICfm
{
   uint8_t     numQci;                            /*!< Numner of QCI's in requests */
/*LRG : Review Tag*/
   NumActvUeQci     numActvUeQci[LRG_MAX_QCI_PER_REQ]; /*!< Number of Active UE's in UL/DL per QCI */
/*LRG : Review Tag*/
} LrgNumActvUeQCICfm;
/**
  @brief
  Structure to send L2 Measurement confirm 
**/
typedef struct lrgSchMeasCfmInfo
{
   Header             hdr;               /*!< Header information */
   uint16_t                measType;          /*!< Type of measurement */
   CmStatus           cfm;               /*!< Confirmation possible Values when measType
                                              is invalid status -> NOK and reason -> INVALID
                                                */
   CmLteCellId        cellId;             /*!< Cell for which measurement is done */
   LrgAvgPrbCfm       avgPrbUl;           /*!< PRB usage in percentage per QCI for UL */
   LrgAvgPrbCfm       avgPrbDl;           /*!< PRB usage in percentage per QCI for DL */
   LrgAvgPrbQCICfm    avgPrbQciUlCfm;     /*!< PRB usage in percentage per QCI for UL */
   LrgAvgPrbQCICfm    avgPrbQciDlCfm;     /*!< PRB usage in percentage per QCI for DL */
   LrgRaPreamblesCfm  raPrmbsCfm;         /*!< Different received RA preambles */
   LrgNumActvUeQCICfm numUeQciUlCfm;      /*!< Number of Active UE's in UL per QCI */
   LrgNumActvUeQCICfm numUeQciDlCfm;      /*!< Number of Active UE's in DL per QCI */
   uint32_t                tbTransDlTotalCnt;  /*!< Count of DL TB transmitteed */
   uint32_t                tbTransDlFaulty;    /*!< Count of DL TB for wich NACK not recieved from UE */ 
   uint32_t                tbTransUlTotalCnt;  /*!< Count of UL TB received successfully */ 
   uint32_t                tbTransUlFaulty;    /*!< Count of UL TB not recieved successfully */ 
} LrgSchMeasCfmInfo;

#endif /* LTE_L2_MEAS */

/* 
   Function Prototype Typedefs 
. */
typedef S16 (*LrgCfgReq)     ARGS((
        Pst        *pst,               /* Post Structure */
        RgMngmt    *cfg                /* Management Structure */
     ));

typedef uint8_t (*LrgSchCfgReq)     ARGS((
        Pst        *pst,               /* Post Structure */
        RgMngmt    *cfg                /* Management Structure */
     ));

typedef uint8_t (*LrgCfgCfm)     ARGS((
        Pst        *pst,               /* Post Structure */
        RgMngmt    *cfg                /* Management Structure */
     ));

typedef uint8_t (*LrgSchCfgCfm)     ARGS((
        Pst        *pst,               /* Post Structure */
        RgMngmt    *cfg                /* Management Structure */
     ));

typedef S16 (*LrgCntrlReq)   ARGS((
        Pst        *pst,               /* Post Structure */
        RgMngmt    *cntrl              /* Management Structure */
     ));

typedef S16 (*LrgSchCntrlReq)   ARGS((
        Pst        *pst,               /* Post Structure */
        RgMngmt    *cntrl              /* Management Structure */
     ));

typedef S16 (*LrgCntrlCfm)   ARGS((
        Pst        *pst,               /* Post Structure */
        RgMngmt    *cntrl              /* Management Structure */
     ));

typedef S16 (*LrgSchCntrlCfm)   ARGS((
        Pst        *pst,               /* Post Structure */
        RgMngmt    *cntrl              /* Management Structure */
     ));

/* lrg_x_001.main_3 - ADD - Data structures for LTE_L2_MEAS */
#ifdef LTE_L2_MEAS
typedef S16 (*LrgSchL2MeasReq)   ARGS((
        Pst        *pst,               /* Post Structure */
        LrgSchMeasReqInfo *meas       /* L2 Measurement structure */
     ));
typedef S16 (*LrgSchL2MeasStopReq)   ARGS((
        Pst        *pst,               /* Post Structure */
        LrgSchMeasStopReqInfo *meas       /* L2 Measurement structure */
     ));
typedef S16 (*LrgSchL2MeasSendReq)   ARGS((
        Pst        *pst,               /* Post Structure */
        LrgSchMeasSndReqInfo *meas       /* L2 Measurement structure */
     ));
#endif
typedef S16 (*LrgStaReq)     ARGS((
        Pst        *pst,               /* Post Structure */
        RgMngmt    *sta                /* Management Structure */
     ));

typedef S16 (*LrgStaCfm)     ARGS((
        Pst        *pst,               /* Post Structure */
        RgMngmt    *sta                /* Management Structure */
     ));

typedef S16 (*LrgStaInd)     ARGS((
        Pst        *pst,               /* Post Structure */
        RgMngmt    *sta                /* Management Structure */
     ));

typedef S16 (*LrgSchStaInd)     ARGS((
        Pst        *pst,               /* Post Structure */
        RgMngmt    *sta                /* Management Structure */
     ));

typedef S16 (*LrgStsReq)     ARGS((
        Pst        *pst,               /* Post Structure */
        RgMngmt    *sts                /* Management Structure */
     ));

typedef S16 (*LrgStsCfm)     ARGS((
        Pst        *pst,               /* Post Structure */
        RgMngmt    *sts                /* Management Structure */
     ));

/* lrg_x_001.main_3 - ADD - Data structures for LTE_L2_MEAS */
#ifdef LTE_L2_MEAS
typedef S16 (*LrgSchL2MeasCfm)     ARGS((
        Pst            *pst,              /* Post Structure */
        LrgSchMeasCfmInfo  *schL2MeasCfm     /* Measurement Info */
     ));
typedef S16 (*LrgSchL2MeasStopCfm)     ARGS((
        Pst            *pst,              /* Post Structure */
        LrgSchMeasCfmInfo  *schL2MeasCfm     /* Measurement Info */
     ));
#endif /* LTE_L2_MEAS */
/* 
   Function Prototypes.
 */
#ifdef RG
/* lrg_x_001.main_3 - ADD -  Added the comments for the following function */
 /** @brief This primitive is sent from Layer manager to MAC. It carries
  * configuration information towards MAC.
  * @details This primitive can be used by layer manager to configure the
  * following entities at MAC.
  * -# General configuration
  * -# RGU SAP i.e. SAP towards RLC
  * -# CRG SAP i.e. SAP towards RRC
  * -# TFU SAP i.e. SAP towards Physical layer
  * 
  * @param pst pointer to Pst
  * @param cfg pointer to RgMngmt
  * @return ROK/RFAILED
  */
S16 RgMiLrgCfgReq ARGS((Pst *pst, RgMngmt *cfg));

 /** @brief This primitive carries the Configuration Request
  * for Scheduler sent from the DU APP to MAC.
  * @details This primitive is used by MAC to sned the configuration
  * to scheduler
  * @param pst Pointer to the post structure.
  * @param cfm pointer to RgMngmt 
  * @return ROK/RFAILED
  */
uint8_t MacSchGenCfgReq ARGS((Pst *pst, RgMngmt *cfg));

 /** @brief This primitive carries the Confirmation for a Configuration Request
  * sent from the layer manager to MAC.
  * @details This primitive is used by MAC to inform Layer manager about the
  * status of a Configuration Request.
  * @param pst Pointer to the post structure.
  * @param cfm pointer to RgMngmt 
  * @return ROK/RFAILED
  */
S16 RgMiLrgCfgCfm ARGS((Pst *pst, RgMngmt *cfm));

 /** @brief This primitive is sent from Layer manager to Scheduler. It carries
  * configuration information towards MAC.
  * @details This primitive can be used by layer manager to configure the
  * following entities at Scheduler.
  * -# General configuration
  * -# RGR SAP i.e. SAP towards RRM
  * -# TFU SAP i.e. SAP towards Physical layer
  * 
  * @param pst pointer to Pst
  * @param cfg pointer to RgMngmt
  * @return ROK/RFAILED
  */
uint8_t  SchProcGenCfgReq ARGS((Pst *pst, RgMngmt *cfg));

 /** @brief This primitive carries the Confirmation for a Configuration Request
  * sent from the layer manager to Scheduler.
  * @details This primitive is used by Scheduler to inform Layer manager about the
  * status of a Configuration Request.
  * @param pst Pointer to the post structure.
  * @param cfm pointer to RgMngmt 
  * @return ROK/RFAILED
  */
S16 RgMiLrgSchCfgCfm ARGS((Pst *pst, RgMngmt *cfm));

 /** @brief This primitive carries the Request for statistics from MAC layer
  * sent from the layer manager.
  * @details This primitive is used by layer manager to request for statistics
  * on the following
  * -# General statistics
  * -# RGU SAP statistics
  * -# CRG SAP statistics
  * -# TFU SAP statistics
  * @param pst Pointer to the post structure.
  * @param sts pointer to RgMngmt 
  * @return ROK/RFAILED
  */
S16 RgMiLrgStsReq ARGS((Pst *pst, RgMngmt *sts));

 /** @brief This primitive carries the Statistics for a Statistics Request
  * sent from the layer manager to MAC.
  * @details This primitive is used by MAC to inform Layer manager about the
  * Statistics requested earlier.
  * @param pst Pointer to the post structure.
  * @param cfm pointer to RgMngmt 
  * @return ROK/RFAILED
  */
S16 RgMiLrgStsCfm ARGS((Pst *pst, RgMngmt *cfm));

 /** @brief This primitive carries the Unsolicited status indications from MAC
  * to the layer manager i.e. Alarms.
  * @details This primitive is used by MAC to inform Layer manager about some
  * error conditions or bind confirmations.
  * @param pst Pointer to the post structure.
  * @param usta pointer to RgMngmt 
  * @return ROK/RFAILED
  */
S16 RgMiLrgStaInd ARGS((Pst *pst, RgMngmt *usta));
 /** @brief This primitive carries the Unsolicited status indications from
  * scheduler to the layer manager i.e. Alarms.
  * @details This primitive is used by Scheduler to inform Layer manager about some
  * error conditions or bind confirmations.
  * @param pst Pointer to the post structure.
  * @param usta pointer to RgMngmt 
  * @return ROK/RFAILED
  */
S16 RgMiLrgSchStaInd ARGS((Pst *pst, RgMngmt *usta));

#ifdef LTE_L2_MEAS
 /** @brief This primitive is to enable L2 (layer 2) measurements at the MAC
  * layer.
  * 
  * @details Enables the L2 Measurements.
  * 
  * @param pst Pointer to the post structure.
  * @param schL2MeasInfo pointer to LrgSchMeasReqInfo 
  * @return ROK/RFAILED
  */
S16 RgMiLrgSchL2MeasReq ARGS((Pst *pst, LrgSchMeasReqInfo *schL2MeasInfo));
 /** @brief This primitive carries the results of the L2 Measurements gathered
  * by MAC to the layer manager.
  * 
  * @details Carries the Measurements gathered by MAC.
  * 
  * @param pst Pointer to the post structure.
  * @param schL2MeasCfm pointer to LrgSchMeasCfmInfo 
  * @return ROK/RFAILED
  */
S16 RgMiLrgSchL2MeasCfm ARGS((Pst *pst, LrgSchMeasCfmInfo *schL2MeasCfm));
 /** @brief This primitive is to Stop L2 (layer 2) measurements at the MAC
  * layer.
  *
  * @details Enables the L2 Measurements.
  *
  * @param pst Pointer to the post structure.
  * @param schL2MeasInfo pointer to LrgSchMeasReqInfo
  * @return ROK/RFAILED
  */

S16 RgMiLrgSchL2MeasStopReq ARGS((Pst *pst, LrgSchMeasStopReqInfo *schL2MeasInfo));
 /** @brief This primitive is to used to send  L2 (layer 2) measurements at the MAC
  * layer.
  *
  * @details Enables the L2 Measurements.
  *
  * @param pst Pointer to the post structure.
  * @param schL2MeasInfo pointer to LrgSchMeasReqInfo
  * @return ROK/RFAILED
  */

S16 RgMiLrgSchL2MeasSendReq ARGS((Pst *pst,LrgSchMeasSndReqInfo *schL2MeasInfo));
 /** @brief This primitive carries the confrmratoin  of the L2 Measurements gathered
  * by MAC to the layer manager.
  *
  * @details Carries the Measurements gathered by MAC.
  *
  * @param pst Pointer to the post structure.
  * @param schL2MeasCfm pointer to LrgSchMeasCfmInfo
  * @return ROK/RFAILED
  */

S16 RgMiLrgSchL2MeasStopCfm ARGS((Pst *pst, LrgSchMeasCfmInfo *schL2MeasCfm));
#endif /* LTE_L2_MEAS */
#endif /* RG. */

#ifdef SM 
S16 smRgActvInit ARGS((Ent ent,Inst inst, Region region,Reason reason));
S16 smRgActvTsk ARGS((Pst *pst, Buffer *mBuf));
S16 SmMiLrgCfgReq ARGS((Pst *pst, RgMngmt *cfg));
S16 SmMiLrgCfgCfm ARGS((Pst *pst, RgMngmt *cfm));
S16 SmMiLrgSchCfgReq ARGS((Pst *pst, RgMngmt *cfg));
S16 SmMiLrgSchCfgCfm ARGS((Pst *pst, RgMngmt *cfm));
S16 SmMiLrgStsReq ARGS((Pst *pst, RgMngmt *sts));
S16 SmMiLrgStsCfm ARGS((Pst *pst, RgMngmt *cfm));
S16 SmMiLrgStaReq ARGS((Pst *pst, RgMngmt *sta));
S16 SmMiLrgStaInd ARGS((Pst *pst, RgMngmt *usta));
S16 SmMiLrgCntrlReq ARGS((Pst *pst, RgMngmt *cntrl));
S16 SmMiLrgSchStaInd ARGS((Pst *pst, RgMngmt *usta));
S16 SmMiLrgCntrlCfm ARGS(( Pst *pst, RgMngmt *cfm));
S16 SmMiLrgSchCntrlReq ARGS((Pst *pst, RgMngmt *cntrl));
/* lrg_x_001.main_3 - ADD -  Added the following functions for LTE_L2_MEAS */
#ifdef LTE_L2_MEAS
S16 SmMiLrgSchL2MeasStartReq ARGS((Pst *pst, LrgSchMeasReqInfo *schL2MeasReq));
S16 SmMiLrgSchL2MeasCfm ARGS((Pst *pst, LrgSchMeasCfmInfo *schL2MeasCfm));
S16 SmMiLrgSchL2MeasStopReq ARGS((Pst *pst, LrgSchMeasStopReqInfo *schL2MeasReq));
S16 SmMiLrgSchL2MeasSendReq ARGS((Pst *pst, LrgSchMeasSndReqInfo *schL2MeasReq));
S16 SmMiLrgSchL2MeasStopCfm ARGS((Pst *pst, LrgSchMeasCfmInfo *schL2MeasCfm));
S16 SmMiLrgSchL2MeasReq ARGS((Pst *pst, LrgSchMeasReqInfo *meas));
S16 SmMiLrgSchL2SendMeasReq ARGS((Pst *pst, LrgSchMeasSndReqInfo *meas));
S16 SmMiLrgSchL2StopMeasReq ARGS((Pst *pst, LrgSchMeasStopReqInfo *meas));
#endif /* LTE_L2_MEAS */
#endif /* SM. */

/*
   Function Prototypes for Packing and Unpacking the primitives.
 */
#if (defined(LCLRG))
/** @brief This API is used to send a 
Config Request from LM to MAC.*/
S16 cmPkLrgCfgReq ARGS((
   Pst *                pst,
   RgMngmt *            cfg
));
/** @brief This API is used to send a 
Configuration Request from LM to MAC. */
S16 cmUnpkLrgCfgReq ARGS((
   LrgCfgReq            func,
   Pst *                pst,
   Buffer               *mBuf
));
/** @brief This API is used to send a 
Configuration Request from LM to SCH. */
S16 cmPkLrgSchCfgReq ARGS((
   Pst *                pst,
   RgMngmt *            cfg
));
/** @brief This API is used to send a 
Configuration Request from LM to SCH. */
S16 cmUnpkLrgSchCfgReq ARGS((
   LrgSchCfgReq         func,
   Pst *                pst,
   Buffer               *mBuf
));
/* lrg_x_001.main_3 - ADD -  Added the following pack/unpack functions for LTE_L2_MEAS */
#ifdef LTE_L2_MEAS
/** @brief This API is used to send a 
L2 Measurement Request from LM to SCH. */
S16 cmPkLrgSchL2MeasReq ARGS((
   Pst *                pst,
   LrgSchMeasReqInfo *  meas
));
/** @brief This API is used to send a 
L2 Measurement Request from LM to SCH. */
S16 cmUnpkLrgSchL2MeasReq ARGS((
   LrgSchL2MeasReq         func,
   Pst *                 pst,
   Buffer               *mBuf
));
S16 cmPkLrgSchL2MeasCfm ARGS((
    Pst * pst,
    LrgSchMeasCfmInfo  *measInfo
));
S16 cmUnpkLrgSchL2MeasCfm ARGS((
    LrgSchL2MeasCfm func,
    Pst *pst,
    Buffer *mBuf
));
S16 cmPkLrgSchL2MeasReq ARGS((
   Pst *                pst,
   LrgSchMeasReqInfo *  meas
));
/** @brief This API is used to send a 
L2 Measurement Request from LM to SCH. */
S16 cmUnpkLrgSchL2MeasReq ARGS((
   LrgSchL2MeasReq         func,
   Pst *                 pst,
   Buffer               *mBuf
));
/** @brief This API is used to send a
L2 Measurement Stop Request from LM to SCH. */
S16 cmPkLrgSchL2MeasSendReq ARGS((
   Pst *                pst,
   LrgSchMeasSndReqInfo *  meas
));
/** @brief This API is used to send a 
L2 Measurement Request from LM to SCH. */
S16 cmUnpkLrgSchL2MeasSendReq ARGS((
   LrgSchL2MeasSendReq         func,
   Pst *                 pst,
   Buffer               *mBuf
));
/** @brief This API is used to send a
L2 Measurement Stop Request from LM to SCH. */
S16 cmPkLrgSchL2MeasStopReq ARGS((
   Pst *                pst,
   LrgSchMeasStopReqInfo *  meas
));
/** @brief This API is used to send a 
L2 Measurement Request from LM to SCH. */
S16 cmUnpkLrgSchL2MeasStopReq ARGS((
   LrgSchL2MeasStopReq         func,
   Pst *                 pst,
   Buffer               *mBuf
));
/** @brief This API is used to carry
L2 Measurement Cfm  from SCH. */
S16 cmPkLrgSchL2MeasStopCfm ARGS((
    Pst * pst,
    LrgSchMeasCfmInfo  *measInfo
));
/** @brief This API is used to carrya
L2 Measurement Cfm  from SCH. */
S16 cmUnpkLrgSchL2MeasStopCfm ARGS((
    LrgSchL2MeasStopCfm func,
    Pst *pst,
    Buffer *mBuf
));
#endif
/** @brief This API is used to send a 
Configuration Confirm from MAC to LM. */
uint8_t cmPkLrgCfgCfm ARGS((
   Pst *                pst,
   RgMngmt *            cfm
));
/** @brief This API is used to send a 
Configuration Confirm from MAC to LM. */
S16 cmUnpkLrgCfgCfm ARGS((
   LrgCfgCfm            func,
   Pst *                pst,
   Buffer               *mBuf
));
/** @brief This API is used to send a 
Configuration Confirm from SCH to LM. */
 uint8_t cmPkLrgSchCfgCfm ARGS((
   Pst *                pst,
   RgMngmt *            cfg
));
/** @brief This API is used to send a 
Configuration Confirm from SCH to LM. */
S16 cmUnpkLrgSchCfgCfm ARGS((
   LrgSchCfgCfm         func,
   Pst *                pst,
   Buffer               *mBuf
));
/** @brief This API is used to send a 
Statistics Request from LM to MAC. */
 S16 cmPkLrgStsReq ARGS((
   Pst *                pst,
   RgMngmt *            sts
));
/** @brief This API is used to send a 
Statistics Request from LM to MAC. */
S16 cmUnpkLrgStsReq ARGS((
   LrgStsReq            func,
   Pst *                pst,
   Buffer               *mBuf
));
/** @brief This API is used to send a 
Statistics Confirm from MAC to LM. */
S16 cmPkLrgStsCfm ARGS((
   Pst *                pst,
   RgMngmt *            cfm
));
/** @brief This API is used to send a 
Statistics Confirm from MAC to LM. */
S16 cmUnpkLrgStsCfm ARGS((
   LrgStsCfm            func,
   Pst *                pst,
   Buffer               *mBuf
));
/** @brief This API is used to send a 
Status Indication from MAC to LM. */
S16 cmPkLrgStaInd ARGS((
   Pst *                pst,
   RgMngmt *            usta
));
/** @brief This API is used to send a 
Status Indication from MAC to LM. */
S16 cmUnpkLrgStaInd ARGS((
   LrgStaInd            func,
   Pst *                pst,
   Buffer               *mBuf
));
/** @brief This API is used to send a 
Status Indication from SCH to LM. */
S16 cmPkLrgSchStaInd ARGS((
   Pst *                pst,
   RgMngmt *            sta
));
/** @brief This API is used to send a 
Status Indication from SCH to LM. */
S16 cmUnpkLrgSchStaInd ARGS((
   LrgSchStaInd         func,
   Pst *                pst,
   Buffer               *mBuf
));
S16 cmPkRgGenCfg ARGS((
   RgGenCfg             *param,
   Buffer               *mBuf
));
S16 cmUnpkRgGenCfg ARGS((
   RgGenCfg             *param,
   Buffer               *mBuf
));
S16 cmPkRgUpSapCfg ARGS((
   RgUpSapCfg           *param,
   Buffer               *mBuf
));
S16 cmUnpkRgUpSapCfg ARGS((
   RgUpSapCfg           *param,
   Buffer               *mBuf
));
S16 cmPkRgLowSapCfg ARGS((
   RgLowSapCfg          *param,
   Buffer               *mBuf
));
S16 cmUnpkRgLowSapCfg ARGS((
   RgLowSapCfg          *param,
   Buffer               *mBuf
));
S16 cmPkRgGenSts ARGS((
   RgGenSts             *param,
   Buffer               *mBuf
));

#ifdef MAC_SCH_STATS
S16 cmPkRgSchHqRetxStats ARGS((
   RgSchHqRetxStats     *param,
   Buffer               *mBuf
));
S16 cmPkRgSchNackAckStats ARGS((
   RgSchNackAckStats    *param,
   Buffer               *mBuf
));
S16 cmPkRgHqNumRetx ARGS((
   RgSchHqNumRetx       *param,
   Buffer               *mBuf
));
S16 cmPkRgAckNack ARGS((
   RgAckNack            *param,
   Buffer               *mBuf
));

S16 cmUnpkRgSchHqRetxStats ARGS((
   RgSchHqRetxStats     *param,
   Buffer               *mBuf
));
S16 cmUnpkRgSchNackAckStats ARGS((
   RgSchNackAckStats    *param,
   Buffer               *mBuf
));
S16 cmUnpkRgHqNumRetx ARGS((
   RgSchHqNumRetx       *param,
   Buffer               *mBuf
));
S16 cmUnpkRgAckNack ARGS((
   RgAckNack            *param,
   Buffer               *mBuf
));
#endif /* MAC_SCH_STATS */

S16 cmUnpkRgGenSts ARGS((
   RgGenSts             *param,
   Buffer               *mBuf
));
S16 cmPkRgSapSts ARGS((
   RgSapSts             *param,
   Buffer               *mBuf
));
S16 cmUnpkRgSapSts ARGS((
   RgSapSts             *param,
   Buffer               *mBuf
));
S16 cmPkRgSchInstCfg ARGS((
   RgSchInstCfg         *param,
   Buffer               *mBuf
));
S16 cmUnpkRgSchInstCfg ARGS((
   RgSchInstCfg         *param,
   Buffer               *mBuf
));
S16 cmPkRgCfg ARGS((
   RgCfg                *param,
   S16                  elmnt,
   Buffer               *mBuf
));
S16 cmUnpkRgCfg ARGS((
   RgCfg                *param,
   S16                  elmnt,
   Buffer               *mBuf
));
S16 cmPkRgSapSta ARGS((
   RgSapSta             *param,
   Buffer               *mBuf
));
S16 cmUnpkRgSapSta ARGS((
   RgSapSta             *param,
   Buffer               *mBuf
));
S16 cmPkRgSts ARGS((
   RgSts                *param,
   S16                  elmnt,
   Buffer               *mBuf
));
S16 cmUnpkRgSts ARGS((
   RgSts                *param,
   S16                  elmnt,
   Buffer               *mBuf
));
/* lrg_x_001.main_3 - MODIFY -  Modified the below function to hold the event type */
S16 cmPkRgUstaDgn ARGS((
   RgUstaDgn            *param,
   Buffer               *mBuf
));
S16 cmUnpkRgUstaDgn ARGS((
   RgUstaDgn            *param,
   Buffer               *mBuf
));
S16 cmPkRgUsta ARGS((
   RgUsta               *param,
   Buffer               *mBuf
));
S16 cmUnpkRgUsta ARGS((
   RgUsta               *param,
   Buffer               *mBuf
));
S16 cmPkRgMngmt ARGS((
   Pst *pst,
   RgMngmt *param,
   uint8_t eventType,
   Buffer *mBuf
));
S16 cmUnpkRgMngmt ARGS((
   Pst *pst,
   RgMngmt *param,
   uint8_t eventType,
   Buffer *mBuf
));

/* lrg_x_001.main_4 ccpu00117036 - C++ support */
#ifdef __cplusplus
}
#endif
#endif

#endif /* __LRGX__. */


/**********************************************************************
   End of file
**********************************************************************/
