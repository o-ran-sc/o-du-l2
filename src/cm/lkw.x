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
  
        Name:    LKW RLC Layer Manager Interface
    
        Type:    C include file
  
        Desc:    This file Contains the Data structures and prototypes
                 for LKW Interface
 
        File:    lkw.x
  
*********************************************************************21*/

#ifndef __LKW_X__
#define __LKW_X__

#ifdef __cplusplus
EXTERN "C" {
#endif  /* __cplusplus */


/** @brief
    General Configuration Structure. */
typedef struct rlcGenCfg
{
   Pst               lmPst;         /*!< Post structure for communicating
                                      with LM. */
   U32               maxUe;         /*!< Maximum number of UEs supported
                                      by RLC. */
   U16               maxKwuSaps;    /*!< Maximum KWU SAPs. */
/* Supported by SPLIT Architecture */
   U16               maxUdxSaps;    /*!< Maximum Udx SAPs. */
/* Supported by SPLIT Architecture ends */
   Ticks             timeRes;       /*!< Time resolution. */
/* Supported by SPLIT Architecture */
   U8                rlcMode;       /*!< RLC_DL or RLC_UL */
/* Supported by SPLIT Architecture ends */
   U16               maxRguSaps;    /*!< Maximum RGU SAPs. */
}RlcGenCfg;

/** @brief
    SAP Configuration Structure */
typedef struct rlcSapCfg
{
   Selector          selector;      /*!< Selector for LC/TC. */
   MemoryId          mem;           /*!< Region and pool. */
   ProcId            procId;        /*!< Processor ID. */
   Ent               ent;           /*!< Entity ID. */
   Inst              inst;          /*!< Instance ID. */
   SpId              sapId;         /*!< SAP ID. */
   U16               bndTmrIntvl;   /*!< Bind timer interval. */
   Priority          priority;      /*!< Priority. */
   Route             route;         /*!< Route. */
}RlcSapCfg;

/** @brief 
*   Configuration Structure 
*/
typedef struct rlcCfg
{
   union
   {
      RlcGenCfg       gen;           /*!< General configuraton. */
      RlcSapCfg       sap;           /*!< SAP configuration. */
   }s;
}RlcCfg;

/** @brief 
    General Statistics Structure */
typedef struct rlcGenSts
{
   CntrSts           numUe;         /*!< Total number of UEs. */
   CntrSts           pdusRecv;      /*!< Number of PDUs received. */
   CntrSts           pdusSent;      /*!< Number of PDUs sent. */
   CntrSts           pdusRetx;      /*!< Number of PDUs retransmitted. */
   CntrSts           bytesRecv;     /*!< Number of bytes received. */
   CntrSts           bytesSent;     /*!< Number of bytes sent. */
   CntrSts           unexpPdusRecv; /*!< Unexpected PDU received. */
   CntrSts           errorPdusRecv; /*!< Format error pdus received. */
   CntrSts           protTimeOut;   /*!< Number of protocol time outs leading
                                         to retransmission. */
   CntrSts           numOfRb;       /*!< Total number of RBs in RLC. */
   CntrSts           numSduDisc;    /*!< Number of SDUs discarded. */
}RlcGenSts;

/** @brief 
    RLC Upper SAP statistics */
typedef struct rlcKwuSapSts 
{
   /* lkw_x_001.main_2, changed from suId to spId */
   SpId              spId;          /*!< Service  Provider ID. */
   CntrSts           sduRx;         /*!< Number of SDUs received. */
   CntrSts           sduTx;         /*!< Number of tranxmitted SDUs. */
}RlcKwuSapSts;

/** @brief 
           RRC Control SAP Statistics */
typedef struct rlcCkwCntSts
{
   CntrSts           statMsgs;      /*!< Number of Status Messages send. */
}RlcCkwCntSts;

/** @brief 
    Statistics Structure */
typedef struct rlcSts
{
   DateTime dt;                     /*!< Date and Time structure. */
   union
   {
      RlcGenSts       gen;           /*!< General Statistics. */
      RlcKwuSapSts    rlckwuSap;        /*!< RLC upper SAP statistics. */
      RlcCkwCntSts    ckwSap;        /*!< RRC control SAP. */
   }s;
}RlcSts;

/** @brief 
            MAC Upper SAP Status Structure */
typedef struct rlcRguSapSta
{
   SuId              suId;          /*!< Service user ID. */
   State             state;         /*!< State of the SAP. */
}RlcRguSapSta;

/** @brief 
    RLC Upper SAP Status Structure */
typedef struct rlcKwuSapSta
{
   SpId              spId;          /*!< Service provider ID. */
   State             state;         /*!< State of the SAP. */
}RlcKwuSapSta;

/** @brief 
    RRC Control SAP Status Structure */
typedef struct rlcCkwCntSapSta
{
   SpId              spId;          /*!< Service provider ID. */
   State             state;         /*!< State of the SAP. */
}RlcCkwCntSapSta;

/** @brief 
    Status Structure */
typedef struct rlcSSta
{
   DateTime          dt;            /*!< Date and Time structure. */
   union
   {
      SystemId       sysId;         /*!< System ID. */
      RlcRguSapSta    rguSap;        /*!< RLC lower SAP (MAC) status. */
      RlcKwuSapSta    rlckwuSap;        /*!< RLC Upper SAP status. */
      RlcCkwCntSapSta ckwSap;        /*!< RRC Control SAP Status. */
   }s;
}RlcSSta;

/** @brief 
    Trace Control Structure */
typedef struct rlcTrcCntrl
{
   U8                trcMask;       /*!< Trace mask. */
   S16               trcLen;        /*!< Trace length. */
}RlcTrcCntrl;

/** @brief 
    Debug Control Structure */
typedef struct rlcDbgCntrl
{
   U32               dbgMask;       /*!< Debug mask. Assign non zero value to enable 
                                         and zero to disable debug */
}RlcDbgCntrl;

/** @brief 
    SAP Control Structure */
typedef struct rlcSapCntrl
{
   SuId              suId;          /*!< Service user ID. */
   SpId              spId;          /*!< Service provider ID. */
}RlcSapCntrl;

/** @brief 
    Control Structure */
typedef struct rlcCntrl
{
   DateTime          dt;            /*!< Date and Time structure. */
   U8                action;        /*!< Action. */
   U8                subAction;     /*!< Sub action. */
   union
   {
      RlcTrcCntrl     trcCntrl;      /*!< Trace Control Structure. */
      RlcDbgCntrl     dbgCntrl;      /*!< Debug Control Structure. */
      RlcSapCntrl     sapCntrl;      /*!< SAP Control Structure. */
#ifdef SS_DIAG
      U32            logMask;       /*!< Logging Control Structure. */
#endif
   }s;
}RlcCntrl;

/** @brief 
    Unsolicited Status Structure */
typedef struct rlcUSta
{
   DateTime          dt;            /*!< Date and Time structure. */
   CmAlarm           alarm;         /*!< Alarm. */
   SuId              suId;          /*!< Service user ID. */
   U32               ueId;          /*!< Urnti UE ID. */
  /* lkw_x_001.main_2, added support for L2 measurement */
#ifdef LTE_L2_MEAS
   U8                qci;           /*!< Qci value */
#endif
}RlcUSta;

/** @brief 
    Trace Structure */
typedef struct rlcTrc
{
   DateTime          dt;            /*!< Date and Time structure. */
   U16               event;         /*!< Event. Events defined in the differenct RLC
                                         interfaces are pssible values here.*/
}RlcTrc;

/** @brief 
    Layer Management Structure */
typedef struct rlcMngmt
{
   Header            hdr;           /*!< Common header. */
   CmStatus          cfm;           /*!< Status of confirmation. */
   union 
   {
      RlcCfg          cfg;           /*!< General Configuration. */
      RlcCntrl        cntrl;         /*!< Control Structure. */
      RlcSts          sts;           /*!< Statistics. */
      RlcSSta         ssta;          /*!< Status. */
      RlcUSta         usta;          /*!< Unsolicited Status. */
      RlcTrc          trc;           /*!< Trace Structre. */
   }t;
}RlcMngmt;

/* lkw_x_001.main_2, added support for L2 measurement */
#ifdef LTE_L2_MEAS

/** @brief Measurement Request Params Structure. */
typedef struct rlcL2MeasReqInfo
{
   U8        measType;          /*!< Measurement type, bit 1 to 4 (LSB nibble) 
                                  will be used for non IP Throughput and 
                                  bit 5 and 6 will be used for DL and UL 
                                  Ipthroughput respectively */
   union
   {
      struct 
      {
         U16          numSamples;       /*!<Sampling period: */
         U8           numQci;           /*!<number of qCI to take measurement for */
         U8           qci[LKW_MAX_QCI]; /*!<QCI for the measurement */
      }nonIpThMeas;

      struct 
      {
         U16 numUes;
         struct 
         {
            U8           numQci;           /*!<number of qCI to take measurement for */
            U8           qci[LKW_MAX_QCI]; /*!<QCI for the measurement */
            CmLteRnti    ueId;             /*!< UE ID (Used only for IP Throughput in UL/DL */
            CmLteCellId  cellId;           /*!< Cell ID (Used only for IP Throughput in UL/DL */
         }ueInfoLst[LKW_MAX_UE];
      }ipThMeas;

   }val;
}RlcL2MeasReqInfo;

/** @brief Measurement Request Exvent  Structure. */
typedef struct rlcL2MeasReqEvt
{
   U32              transId;     /*!< Transaction Identifier */
   U16              measPeriod;  /*!< Measurement Period */
   RlcL2MeasReqInfo  measReq;     /*!< Measurement request structure */
}rlcL2MeasReqEvt;

/** @brief Measurement Confirm Params Structure. */
typedef struct rlcL2MeasCfmInfo
{
   U8              qci;     /*!< QCI value */
   union
   {
      struct
      {
         U32   numActUe;   /*!< num of Active UEs*/
         U32   uuLoss;     /*!< Uu Loss Rate */
         /* Discard new changes starts */
         U32   dlDiscRate; /*!< DL Discard rate to be reported */
         /* Discard new changes ends */
         U32  dlSduDelay;
      }nonIpThrput;

      struct              /*!< Structure for IP throughput*/
      {
         F32   dlIpThPut; /*!< Scheduled DL IP throughput */
         F32   ulIpThPut; /*!< Scheduled UL IP throughput */
      }ipThrput;

   }val;
}RlcL2MeasCfmInfo;

typedef struct rlcL2MeasCfmUeInfoLst
{
   CmLteRnti       ueId;    /*!< UE Id used for DL/UL Ipthroughput*/
   CmLteCellId     cellId;  /*!< Cell Id used for DL/UL Ipthroughput*/
   RlcL2MeasCfmInfo measCfm[LKW_MAX_QCI]; /*!< Measurement confirm structure*/
   U16             numCfm; /*!< Number of Measurement confirm*/
}RlcL2MeasCfmUeInfoLst;

typedef struct rlcL2MeasCfmIpThMeas
{
   U16                  numUes; /*!< Number of Ue Info*/
   RlcL2MeasCfmUeInfoLst ueInfoLst[LKW_MAX_UE]; /*!< UE info list*/
}RlcL2MeasCfmIpThMeas;

typedef struct rlcL2MeasCfmNonIpThMeas
{
   U16             numCfm; /*!< Number of confirm */
   RlcL2MeasCfmInfo measCfm[LKW_MAX_QCI]; /*!< Confirmation info */
}RlcL2MeasCfmNonIpThMeas;

typedef union kwL2MeasCfmIpNonIp
{
   RlcL2MeasCfmIpThMeas    ipThMeas;    /*!< Ip thrupt measurements*/
   RlcL2MeasCfmNonIpThMeas nonIpThMeas; /*!< Non-Ip thrupt measurements*/
}RlcL2MeasCfmIpNonIp;

/** @brief Measurement Confirm Event  Structure. */
typedef struct rlcL2MeasCfmEvt
{
   U32             transId; /*!< Transaction id */
   CmStatus        status;  /*!< Status is for all QCIs */
   U8              measType; /*!< Measurement type, bit 1 to 4 (LSB nibble) 
                               will be used for non IP Throughput and 
                               bit 5 and 6 will be used for DL and UL 
                               Ipthroughput respectively. It is same for
                               all qci in this Cfm event. */
   RlcL2MeasCfmIpNonIp val;
}RlcL2MeasCfmEvt;

#endif /* LTE_L2_MEAS */

/* Control primitives towards LTE RLC */
/* Pointer functions to pack/unpack primitives */
typedef S16 (*RlcConfigReq)   ARGS((Pst *pst, RlcMngmt *cfg));
typedef S16 (*RlcConfigCfm)   ARGS((Pst *pst, RlcMngmt *cfgCfm));

typedef S16 (*LkwCntrlReq) ARGS((Pst *pst, RlcMngmt *cntl));
typedef S16 (*LkwCntrlCfm) ARGS((Pst *pst, RlcMngmt *cntlCfm));

typedef S16 (*LkwStaInd)   ARGS((Pst *pst, RlcMngmt *staInd)); 
typedef S16 (*LkwStaReq)   ARGS((Pst *pst, RlcMngmt *staReq));
typedef S16 (*LkwStaCfm)   ARGS((Pst *pst, RlcMngmt *staCfm));

typedef S16 (*LkwStsReq)   ARGS((Pst *pst, Action, RlcMngmt *sts));
typedef S16 (*LkwStsCfm)   ARGS((Pst *pst, Action, RlcMngmt *stsCfm));

typedef S16 (*LkwTrcInd)   ARGS((Pst *pst, RlcMngmt *trc, Buffer *mBuf));

/* lkw_x_001.main_2, added support for L2 measurement */
#ifdef LTE_L2_MEAS
typedef S16 (*LkwL2MeasReq)   ARGS((Pst *pst, rlcL2MeasReqEvt *measEvt));
typedef S16 (*LkwL2MeasCfm)   ARGS((Pst *pst, RlcL2MeasCfmEvt *measEvt));
typedef S16 (*LkwL2MeasSendReq)   ARGS((Pst *pst, U8 measType));
typedef S16 (*LkwL2MeasStopReq)   ARGS((Pst *pst, U8 measType));
typedef S16 (*LkwL2MeasStopCfm)   ARGS((Pst *pst, U8 measType,U8 status));
#endif /*  LTE_L2_MEAS */

/* Layer primitives */
/** @details This function processes configuration requests received from the layer manager.The configuration requests are general configuration, or configuration of RGU, KWU, and CKW SAPs. The type of the configuration request is determined from the elmId field in the header of the layer manager message.This function is called by the layer manager to configure RLC.

@param[in] pst  - Pointer to post structure
@param[in] cfg  - Configuration management structure
@return ROK if success , RFAILED if failure

*/
EXTERN S16 RlcMiRlcConfigReq ARGS((Pst *pst, RlcMngmt *cfg));

/**
@details This function is called by the RlcMiRlcConfigReq function for responding to configuration requests.The cfm field in the RlcMngmt  structure contains the response value.

@param[in] pst  - Pointer to post structure
@param[in] cfm  - Configuration management structure
@return ROK if success , RFAILED if failure
*/
EXTERN S16 RlcMiRlcConfigCfm ARGS((Pst *pst, RlcMngmt *cfgCfm));

/**
@details
The layer manager initiates and uses the management-control procedure to control
RLC elements.The RLC control request primitive (RlcMiLkwCntrlReq) is called
more than once and at any time after the management-configuration procedure.The
control request primitive is confirmed by a RlcMiLkwCntrlCfm primitive.

@param[in] pst  - Pointer to post structure
@param[in] cntl - Configuration management structure
@return ROK if success , RFAILED if failure
*/
EXTERN S16 RlcMiLkwCntrlReq ARGS((Pst *pst, RlcMngmt *cntl));

/**
@details
This function is called by the RlcMiLkwCntrlReq function to send a control confirm to the layer management module.

@param[in] pst  - Pointer to post structure
@param[in] cfm  - Configuration management structure
@return ROK if success , RFAILED if failure
*/
EXTERN S16 RlcMiLkwCntrlCfm ARGS((Pst *pst, RlcMngmt *cntlCfm));

/**
@details
This function processes solicited status requests received from the layer manager.The layer manager can request status information regarding the system ID, RGSA
P,KWSAP, or CKSAP.

@param[in] pst  - Pointer to post structure
@param[in] cfg  - Configuration management structure
@return ROK if success , RFAILED if failure
*/
EXTERN S16 RlcMiLkwStaReq ARGS((Pst *pst, RlcMngmt *staInd));

/**
@details
This function is called by the RlcMiLkwStaReq function to send the requested stat
us information to the layer manager.

@param[in] pst   - Pointer to post structure
@param[in] stacfm - Configuration management structure
@return ROK if success , RFAILED if failure
*/
EXTERN S16 RlcMiLkwStaCfm ARGS((Pst *pst, RlcMngmt *staCfm));

/**
@details
This function processes statistics requests received from the layer manager. After collecting the statistics, this function calls the statistics confirm function to send the statistics to the layer manager.

@param[in] pst   - Pointer to post structure  
@param[in] acton - action 
@param[in] sts   - Configuration management structure
@return ROK if success , RFAILED if failure
*/
EXTERN S16 RlcMiLkwStsReq ARGS((Pst *pst, Action, RlcMngmt *sts));
/**
@details
This function is called by the RlcMiLkwStsReq function for responding to statisti
cs requests.

@param[in] pst   - Pointer to post structure
@param[in] action - action 
@param[in] cfg  - Configuration management structure
@return ROK if success , RFAILED if failure
*/
EXTERN S16 RlcMiLkwStsCfm ARGS((Pst *pst, Action action, RlcMngmt *stsCfm));
/**
@details
This function is used by RLC module to send unsolicited trace indications to
the layer manager, when tracing is enabled by the layer manager through a previous control request.

@param[in] pst  - Pointer to post structure
@param[in] cfg  - Configuration management structure
@param[in] buf  - buffer 
@return ROK if success , RFAILED if failure
*/
EXTERN S16 RlcMiLkwTrcInd ARGS ((Pst *pst, RlcMngmt *trc, Buffer *mBuf));

/**
@details This function is used by RLC to send unsolicited status information to the layer manager, when the unsolicited status flag is enabled by the layer manager through a previous control request.

@param[in] pst  - Pointer to post structure
@param[in] staReq - Configuration management structure
@return ROK if success , RFAILED if failure
*/
/* lkw_x_001.main_2, added support for L2 measurement */
#ifdef LTE_L2_MEAS
/**
@details This function is called by the Layer Manger function to request 
for Measurement  responding to statisti cs requests.

@param[in] pst  - Pointer to post structure
@param[in] measEvt - Measurement Request structure
@return ROK if success , RFAILED if failure
*/
EXTERN S16 RlcMiLkwL2MeasReq ARGS((Pst *pst, rlcL2MeasReqEvt *measEvt));
EXTERN S16 RlcMiLkwL2MeasStopReq ARGS((Pst *pst,U8 measType));
EXTERN S16 RlcMiLkwL2MeasSendReq ARGS((Pst *pst,U8 measType));
/**
@details This function can be used by PDCP to send the measurement values
after doing L2 measurement that was requested by the layer manager.

@param[in] pst  - Pointer to post structure
@param[in] measEvt - Measurement confirmation structure
@return ROK if success , RFAILED if failure
*/
EXTERN S16 RlcMiLkwL2MeasCfm ARGS((Pst *pst, RlcL2MeasCfmEvt *measCfm));
EXTERN S16 RlcMiLkwL2MeasStopCfm ARGS((Pst *pst, U8 measType, U8 status));
#endif /*  LTE_L2_MEAS */

EXTERN S16 RlcMiLkwStaInd ARGS((Pst *pst, RlcMngmt *staReq));

EXTERN Void rlcSendLmAlarm ARGS ((
      U16 category,
      U16  event,
      U16 cause,
      SuId suId,
      U32 ueId));

#ifdef SM
EXTERN S16 SmMiRlcConfigCfm   ARGS((
       Pst *,
       RlcMngmt *
       ));

EXTERN S16 SmMiLkwCntrlCfm ARGS((
       Pst *,
       RlcMngmt *
       ));

EXTERN S16 SmMiLkwStaInd   ARGS((
       Pst *,
       RlcMngmt *
       ));

EXTERN S16 SmMiLkwStaCfm   ARGS((
       Pst *,
       RlcMngmt *
       ));

EXTERN S16 SmMiLkwStsCfm   ARGS((
       Pst *,
       Action,
       RlcMngmt *
       ));

EXTERN S16 SmMiLkwTrcInd   ARGS((
       Pst *,
       RlcMngmt *,
       Buffer *
       ));

EXTERN S16 SmMiLkwStsReq   ARGS((
       Pst *,
       Action,
       RlcMngmt *
       ));

EXTERN S16 SmMiLkwStaReq   ARGS((
       Pst *,
       RlcMngmt *
       ));

EXTERN S16 SmMiLkwCntrlReq ARGS((
       Pst *,
       RlcMngmt *
       ));

EXTERN S16 SmMiRlcConfigReq   ARGS((
       Pst *,
       RlcMngmt *
       ));

/* lkw_x_001.main_2, added support for L2 measurement */
#ifdef LTE_L2_MEAS
EXTERN S16 SmMiLkwL2MeasReq   ARGS((
         Pst *,
         rlcL2MeasReqEvt *
         ));
EXTERN S16 SmMiLkwL2MeasStopReq ARGS((
       Pst            *pst,
       U8             measType
      ));

EXTERN S16 SmMiLkwL2MeasSendReq ARGS((
         Pst            *pst,
         U8             measType
         ));
EXTERN S16 SmMiLkwL2MeasCfm   ARGS((
         Pst *,
         RlcL2MeasCfmEvt *
         ));
EXTERN S16 SmMiLkwL2MeasStopCfm ARGS((
         Pst     *pst,          
         U8      measType,
         U8      status
         ));
#endif /*  LTE_L2_MEAS */
#endif

EXTERN S16 smKwActvInit    ARGS((Ent ent, Inst inst, Region region, Reason reason));
EXTERN S16 smKwActvTsk     ARGS((Pst *pst, Buffer *mBuf));

/* pack/unpack functions */
#ifdef LCLKW 
EXTERN S16 packRlcConfigReq     ARGS((
       Pst *,
       RlcMngmt *
       ));

EXTERN S16 packRlcConfigCfm     ARGS((
       Pst *,
       RlcMngmt *
       ));


EXTERN S16 cmPkLkwCntrlReq   ARGS((
       Pst *,
       RlcMngmt *
       ));

EXTERN S16 cmPkLkwCntrlCfm   ARGS((
       Pst *,
       RlcMngmt *
       ));

EXTERN S16 cmPkLkwStaInd     ARGS((
       Pst *,
       RlcMngmt *
       ));

EXTERN S16 cmPkLkwStaReq     ARGS((
       Pst *,
       RlcMngmt *
       ));

EXTERN S16 cmPkLkwStaCfm     ARGS((
       Pst *,
       RlcMngmt *
       ));

EXTERN S16 cmPkLkwStsReq     ARGS((
       Pst *,
       Action,
       RlcMngmt *
       ));

EXTERN S16 cmPkLkwStsCfm     ARGS((
       Pst *,
       Action,
       RlcMngmt *
       ));

EXTERN S16 cmPkLkwTrcInd     ARGS((
       Pst *,
       RlcMngmt *,
       Buffer *
       ));

EXTERN S16 unpackRlcConfigReq   ARGS((
       RlcConfigReq,
       Pst *,
       Buffer *
       ));

EXTERN S16 unpackRlcConfigCfm   ARGS((
       RlcConfigCfm,
       Pst *,
       Buffer *
       ));

EXTERN S16 cmUnpkLkwCntrlReq ARGS((
       RlcConfigReq,
       Pst *,
       Buffer *
       ));

EXTERN S16 cmUnpkLkwCntrlCfm ARGS((
       RlcConfigCfm,
       Pst *,
       Buffer *
       ));

EXTERN S16 cmUnpkLkwStaInd   ARGS((
       LkwStaInd,
       Pst *,
       Buffer *
       ));

EXTERN S16 cmUnpkLkwStaReq   ARGS((
       LkwStaReq,
       Pst *,
       Buffer *
       ));

EXTERN S16 cmUnpkLkwStaCfm   ARGS((
       LkwStaCfm,
       Pst *,
       Buffer *
       ));

EXTERN S16 cmUnpkLkwStsReq   ARGS((
       LkwStsReq,
       Pst *,
       Buffer *
       ));

EXTERN S16 cmUnpkLkwStsCfm   ARGS((
       LkwStsCfm,
       Pst *, Buffer *
       ));


EXTERN S16 cmUnpkLkwTrcInd   ARGS((
       LkwTrcInd,
       Pst *,
       Buffer *));

/* lkw_x_001.main_2, added support for L2 measurement */
#ifdef LTE_L2_MEAS
EXTERN S16 cmPkLkwL2MeasReq ARGS((
         Pst * pst,
         rlcL2MeasReqEvt * measReqEvt
         ));
EXTERN S16 cmUnpkLkwL2MeasReq ARGS((
         LkwL2MeasReq func,
         Pst *pst,
         Buffer *mBuf
         ));
EXTERN S16 cmPkLkwL2MeasCfm ARGS((
         Pst * pst,
         RlcL2MeasCfmEvt *measCfmEvt
         ));
EXTERN S16 cmUnpkLkwL2MeasCfm ARGS((
         LkwL2MeasCfm func,
         Pst *pst,
         Buffer *mBuf
         ));
EXTERN S16 cmPkRlcL2MeasReqInfo ARGS((
         RlcL2MeasReqInfo * measReqInfo,
         Buffer *mBuf
         ));
EXTERN S16 cmUnpkRlcL2MeasReqInfo ARGS((
         RlcL2MeasReqInfo * measReqInfo,
         Buffer *mBuf
         ));
EXTERN S16 cmPkRlcL2MeasCfmInfo ARGS((
         RlcL2MeasCfmInfo * measCfmInfo,
         Buffer *mBuf,
         U8 measType
         ));
EXTERN S16 cmUnpkRlcL2MeasCfmInfo ARGS((
         RlcL2MeasCfmInfo * measCfmInfo,
         Buffer *mBuf,
         U8 measType
         ));
EXTERN S16 cmPkLkwL2MeasStopReq ARGS((
         Pst * pst,
         U8 measType
         ));
EXTERN S16 cmUnpkLkwL2MeasStopReq ARGS((
         LkwL2MeasStopReq func,
         Pst *pst,
         Buffer *mBuf
         ));
EXTERN S16 cmPkLkwL2MeasSendReq ARGS((
         Pst * pst,
         U8 measType
         ));
EXTERN S16 cmUnpkLkwL2MeasSendReq ARGS((
         LkwL2MeasSendReq func,
         Pst *pst,
         Buffer *mBuf
         ));
EXTERN S16 cmPkLkwL2MeasStopCfm ARGS((
         Pst * pst,
         U8 measType,
         U8 status
         ));
EXTERN S16 cmUnpkLkwL2MeasStopCfm ARGS((
         LkwL2MeasStopCfm func,
         Pst *pst,
         Buffer *mBuf
         ));
#endif /*  LTE_L2_MEAS */

#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __LKW_X__ */

  
/********************************************************************30**
         End of file
**********************************************************************/
