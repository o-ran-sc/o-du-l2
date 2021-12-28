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

/* Defines APIs exchanged between du_app and cl module of RLC */
#ifndef __RLC_INF_H__
#define __RLC_INF_H__

/* Events */
#define EVENT_RLC_UE_CREATE_REQ  210
#define EVENT_RLC_UE_CREATE_RSP 211    /*!< Config Confirm */
#define EVENT_UL_RRC_MSG_TRANS_TO_DU  212
#define EVENT_DL_RRC_MSG_TRANS_TO_RLC 213
#define EVENT_RRC_DELIVERY_MSG_TRANS_TO_DU 214
#define EVENT_RLC_UE_RECONFIG_REQ  215
#define EVENT_RLC_UE_RECONFIG_RSP  216
#define EVENT_DL_RRC_MSG_RSP_TO_DU 217
#define EVENT_DL_USER_DATA_TRANS_TO_RLC 218
#define EVENT_UL_USER_DATA_TRANS_TO_DU 219
#define EVENT_RLC_UE_DELETE_REQ 220
#define EVENT_RLC_UE_DELETE_RSP 221
#define EVENT_RLC_SLICE_PM_TO_DU 222

#define RB_ID_SRB 0
#define RB_ID_DRB 1

#define RB_TYPE_SRB 0        /* Signalling Radio Bearer */
#define RB_TYPE_DRB 1        /* Data Radio Bearer */

/* RLC Direction Config */
#define RLC_CFG_DIR_UL        1     /*!< Uplink direction */
#define RLC_CFG_DIR_DL        2     /*!< Downlink direction */
#define RLC_CFG_DIR_BOTH      3     /*!< Both Downlink and Uplink Direction*/


/* Logical Channel Type */
#define LCH_BCCH      1      /*!< BCCH Logical Channel */
#define LCH_PCCH      2      /*!< PCCH Logical Channel */
#define LCH_CCCH      3      /*!< CCCH Logical Channel */
#define LCH_DTCH      4      /*!< DTCH Logical Channel */
#define LCH_DCCH      5      /*!< DCCH Logical Channel */

typedef enum
{
   TRANSMISSION_IN_PROGRESS,
   TRANSMISSION_COMPLETE,
   TRANSMISSION_FAILED
}DlMsgState;

typedef enum
{
   RLC_CFG_REAS_NONE            ,  /*!< No Failure */
   RLC_CFG_REAS_RB_CREAT_FAIL   ,  /*!< RB CB creation Failure */
   RLC_CFG_REAS_UE_CREAT_FAIL   ,  /*!< UE CB creation Failure */
   RLC_CFG_REAS_CELL_CREAT_FAIL ,  /*!< CELL CB creation Failure */
   RLC_CFG_REAS_RB_PRSNT        ,  /*!< RB CB already present */
   RLC_CFG_REAS_LCHTYPE_MIS     ,  /*!< Logical channel type mismatch */
   RLC_CFG_REAS_RB_UNKWN        ,  /*!< RB Cb unknown */
   RLC_CFG_REAS_UE_UNKWN        ,  /*!< UE Cb unknown */
   RLC_CFG_REAS_CELL_UNKWN      ,  /*!< Cell Cb unknown */
   RLC_CFG_REAS_RB_DEL_FAIL     ,  /*!< RB Cb Deletion Failure */
   RLC_CFG_REAS_UE_DEL_FAIL     ,  /*!< UE Cb Deletion Failure */
   RLC_CFG_REAS_RB_REEST_FAIL   ,  /*!< RB Cb Re establish Failure */
   RLC_CFG_REAS_RB_MODE_MIS     ,  /*!< RB Cb Mode mismatch */
   RLC_CFG_REAS_REEST_FAIL      ,  /*!< RB Cb Re-establishment Failed */
   RLC_CFG_REAS_RECFG_FAIL      ,  /*!< RB Cb Re-configuration Failed */
   RLC_CFG_REAS_INVALID_CFG     ,  /*!< Invalid Configuration Type */
   RLC_CFG_REAS_SAME_UEID       ,  /*!< UE Ids match in ChngUeId Request */
   RLC_CFG_REAS_CELL_DEL_FAIL   ,  /*!< Cell Cb Deletion Failure */
   RLC_CFG_REAS_INVALID_LCHID   ,  /*!< Invalid LChId */
   RLC_CFG_REAS_INVALID_DIR     ,  /*!< Invalid Direction */
   RLC_CFG_REAS_UE_EXISTS       ,  /*!< UE ID already exists */
   RLC_CFG_REAS_INVALID_SNLEN   ,  /*!< Invalid SN Length */
   RLC_CFG_REAS_SNLEN_MIS       ,  /*!< SN Length mismatch */
   RLC_CFG_REAS_INVALID_RGUSAP     /*!< Invalid RGU SAP ID */
}FailureReason;

typedef enum
{
   SUCCESSFUL            ,  /*!< No Failure */
   INVALID_CELLID      ,  /*!< CellId not present */
   INVALID_UEID        ,  /*!< UEId not present */
}UeDeleteResult;

typedef enum
{
   RLC_AM,                    //Acknowledged Mode
   RLC_UM_BI_DIRECTIONAL,     //UnAcknowledged Mode
   RLC_UM_UNI_DIRECTIONAL_UL,
   RLC_UM_UNI_DIRECTIONAL_DL
}RlcMode;

typedef enum
{
   AM_SIZE_12 = 1,
   AM_SIZE_18
}SnLenAm;

typedef enum
{
   UM_SIZE_6 = 1,
   UM_SIZE_12
}SnLenUm;

typedef enum
{
   RLC_DU_APP_RSP_OK = 1,
   RLC_DU_APP_RSP_NOK
}RlcRsp;

typedef struct ulAmCfg
{
   SnLenAm        snLenUl;              /* Sequence Number length in bits. Allowed values are 12 and 18 */
   int8_t         reAssemTmr;           /* T_reassembling Timer in msec*/
   int16_t        statProhTmr;          /* T_status_prohibit Timer in msec*/

}UlAmCfg;

typedef struct dlAmCfg
{
   SnLenAm      snLenDl;             /* Sequence Number length in bits. Allowed values are 12 and 18 */
   uint16_t     pollRetxTmr;         /* T_poll_retransmit Timer in msec */
   int16_t      pollPdu;             /* Used to trigger a poll for every pollPdu.*/
   int32_t      pollByte;            /* Poll_Byte in bytes. */
   uint8_t      maxRetxTh;           /* Max_Retx_Threshold */

}DlAmCfg;

typedef struct dlUmCfg
{
   SnLenUm        snLenDlUm;             /* Sequence Number length in bits. Allowed values are 6 and 12 */

}DlUmCfg;

typedef struct ulUmCfg
{
   SnLenUm        snLenUlUm;             /* Sequence Number length in bits. Allowed values are 6 and 12 */
   int8_t         reAssemTmr;            /* T_reassembling Timer in msec*/

}UlUmCfg;

typedef struct amBearerCfg
{
   UlAmCfg  ulAmCfg;
   DlAmCfg  dlAmCfg;
}AmBearerCfg;

typedef struct umBiDirBearerCfg
{
   UlUmCfg  ulUmCfg;
   DlUmCfg  dlUmCfg;
}UmBiDirBearerCfg;

typedef struct umUniDirUlBearerCfg
{
   UlUmCfg  ulUmCfg;
}UmUniDirUlBearerCfg;

typedef struct umUniDirDlBearerCfg
{
   DlUmCfg  dlUmCfg;
}UmUniDirDlBearerCfg;

/* Spec Ref: 38.331, 6.3.2 RLC-BearerConfig */
typedef struct rlcBearerCfg
{
   ConfigType   configType;
   uint8_t      rbId;
   uint8_t      rbType;
   uint8_t      lcId;
   uint8_t      lcType;
   Snssai       *snssai; 
   RlcMode      rlcMode;
   union
   {
      AmBearerCfg         *amCfg;
      UmBiDirBearerCfg    *umBiDirCfg;
      UmUniDirUlBearerCfg *umUniDirUlCfg;
      UmUniDirDlBearerCfg *umUniDirDlCfg;
   }u;
}RlcBearerCfg;

typedef struct rlcUeCfg
{
   uint16_t     cellId;
   uint8_t      ueId;
   uint8_t      numLcs;
   RlcBearerCfg rlcLcCfg[MAX_NUM_LC];
   UeCfgState rlcUeCfgState; /* InActive / Completed */
}RlcUeCfg;

typedef struct rlcUeCfgRsp
{
   uint16_t       cellId;
   uint16_t       ueId;
   RlcRsp         result;
   FailureReason  reason;
}RlcUeCfgRsp;

typedef struct rlcUeDelete
{
   uint16_t      cellId;
   uint8_t       ueId;
}RlcUeDelete;

typedef struct rlcUeDeleteRsp
{
   uint16_t       cellId;
   uint8_t        ueId;
   UeDeleteResult result;
}RlcUeDeleteRsp;

/* UL RRC Message from RLC to DU APP */
typedef struct ulRrcMsgInfo
{
   uint16_t   cellId;       /* Cell Id */
   uint16_t   ueId;         /* UE Id */
   uint8_t    lcId;         /* Logical channel Id */
   uint16_t   msgLen;       /* RRC message length (in bytes) */
   uint8_t    *rrcMsg;      /* RRC Message (UL-DCCH Message) */
}RlcUlRrcMsgInfo;

/* UL User Data from RLC to DU APP */
typedef struct ulUserDatInfo
{
   uint16_t   cellId;       /* Cell Id */
   uint16_t   ueId;         /* UE Id */
   uint8_t    rbId;
   uint16_t   msgLen;       /* User data length (in bytes) */
   uint8_t    *userData;    /* User data (UL-DTCH Message) */
}RlcUlUserDatInfo;

/* DL RRC Message from DU APP to RLC */
typedef struct dlRrcMsgInfo
{
   uint16_t   cellId;         /* Cell Id */
   uint16_t   ueId;           /* UE Id */
   uint8_t    rbType;         /* Radio Bearer Type */
   uint8_t    rbId;           /* Radio Bearer Id */
   uint8_t    lcType;         /* Logical channel type */
   uint8_t    lcId;           /* Logical channel Id */
   bool       execDup;        /* Execution duplication enabled */
   bool       deliveryStaRpt;  /* Reporting of delivery status by RLC to DU-APP enabled */
   uint16_t   msgLen;         /* RRC Message length */
   uint8_t    *rrcMsg;        /* RRC Message (DL-DCCH Message) */
}RlcDlRrcMsgInfo;

/* DL RRC Message Rsp From RLC to DU APP */
typedef struct dlRrcMsgRsp
{
   uint16_t   cellId;         /* Cell Id */
   uint16_t   crnti;          /* UE index */
   DlMsgState state;         /* Dl RRC Msg State */
}RlcDlRrcMsgRsp;

/* RRC delivery message from RLC to DU APP */
typedef struct rrcDeliveryStatus
{
   uint16_t  deliveryStatus;
   uint16_t  triggeringMessage;
}RrcDeliveryStatus;

typedef struct
{
  uint32_t sd:24;
  uint8_t sst;
}SliceIdentifier;

/*Pm Metric for NW Slicing from RLC to DUAPP*/
typedef struct slicePm
{
  SliceIdentifier networkSliceIdentifier;
  double ThpDl;
  double ThpUl;
}SlicePm;

typedef struct slicePmList
{
   uint8_t numSlice;
   SlicePm *sliceRecord;
}SlicePmList;

typedef struct rrcDeliveryReportInfo
{
   uint16_t  cellId;
   uint16_t  ueId;
   uint8_t   srbId;
   RrcDeliveryStatus  rrcDeliveryStatus;
}RrcDeliveryReport;

/* DL Data Message from DU APP to RLC */
typedef struct dlDataMsgInfo
{
   uint16_t   cellId;         /* Cell Id */
   uint16_t   ueId;           /* UE Id */
   uint8_t    rbId;           /* Radio Bearer Id */
   uint16_t   msgLen;         /* Message length */
   Buffer     *dlMsg;         /* DL Data */
}RlcDlUserDataInfo;

/* Function Pointers */
/* UE create Request from DU APP to RLC*/
typedef uint8_t (*DuRlcUeCreateReq) ARGS((
   Pst           *pst,
   RlcUeCfg      *ueCfg ));

/* UE Cfg Response from RLC to DU APP*/
typedef uint8_t (*RlcDuUeCfgRsp) ARGS((
   Pst          *pst,
   RlcUeCfgRsp  *ueCfgRsp));

/* UE Delete Response from RLC to DU APP*/
typedef uint8_t (*RlcDuUeDeleteRsp) ARGS((
   Pst          *pst,
   RlcUeDeleteRsp  *ueDelRsp));

/* UL RRC Message from RLC to DU APP */
typedef uint8_t (*RlcUlRrcMsgToDuFunc) ARGS((
   Pst           *pst,
   RlcUlRrcMsgInfo *ulRrcMsgInfo));

/* DL RRC Message from DU APP to RLC */
typedef uint8_t (*DuDlRrcMsgToRlcFunc) ARGS((
   Pst           *pst,
   RlcDlRrcMsgInfo *dlRrcMsgInfo));

/* RRC delivery message from RLC to DU APP */
typedef uint8_t (*RlcRrcDeliveryReportToDuFunc) ARGS((
   Pst           *pst,
   RrcDeliveryReport *rrcDeliveryReport));

/* UE Reconfig Request from DU APP to RLC */
typedef uint8_t (*DuRlcUeReconfigReq) ARGS((
   Pst           *pst,
   RlcUeCfg      *ueCfg ));

/* UE Delete Request from DU APP to RLC */
typedef uint8_t (*DuRlcUeDeleteReq) ARGS((
   Pst           *pst,
   RlcUeDelete   *ueDelete));

/* DL RRC Message from RLC to DU APP */
typedef uint8_t (*RlcDlRrcMsgRspToDuFunc) ARGS((
   Pst           *pst,
   RlcDlRrcMsgRsp *dlRrcMsgRsp));

/* UL User data from RLC to DU APP */
typedef uint8_t (*RlcUlUserDataToDuFunc) ARGS((
   Pst           *pst,
   RlcUlUserDatInfo *ulUserData));

/* DL User Data from DU APP to RLC */
typedef uint8_t (*DuRlcDlUserDataToRlcFunc) ARGS((
   Pst           *pst,
   RlcDlUserDataInfo *dlDataMsg));

/* Slice Metrics from RLC to DU APP */
typedef uint8_t (*RlcSlicePmToDuFunc) ARGS((
   Pst           *pst,
   SlicePmList *sliceStats));

/* Pack/Unpack function declarations */
uint8_t packDuRlcUeCreateReq(Pst *pst, RlcUeCfg *ueCfg);
uint8_t unpackRlcUeCreateReq(DuRlcUeCreateReq func, Pst *pst, Buffer *mBuf);
uint8_t packRlcDuUeCfgRsp(Pst *pst, RlcUeCfgRsp *ueCfgRsp);
uint8_t unpackRlcUeCfgRsp(RlcDuUeCfgRsp func, Pst *pst, Buffer *mBuf);
uint8_t packRlcUlRrcMsgToDu(Pst *pst, RlcUlRrcMsgInfo *ulRrcMsgInfo);
uint8_t unpackRlcUlRrcMsgToDu(RlcUlRrcMsgToDuFunc func, Pst *pst, Buffer *mBuf);
uint8_t packDlRrcMsgToRlc(Pst *pst, RlcDlRrcMsgInfo *dlRrcMsgInfo);
uint8_t unpackDlRrcMsgToRlc(DuDlRrcMsgToRlcFunc func, Pst *pst, Buffer *mBuf);
uint8_t packRrcDeliveryReportToDu(Pst *pst, RrcDeliveryReport *rrcDeliveryReport);
uint8_t unpackRrcDeliveryReportToDu(RlcRrcDeliveryReportToDuFunc func,Pst *pst, Buffer *mBuf);
uint8_t packDuRlcUeReconfigReq(Pst *pst, RlcUeCfg *ueCfg);
uint8_t unpackRlcUeReconfigReq(DuRlcUeReconfigReq func, Pst *pst, Buffer *mBuf);
uint8_t packRlcDlRrcMsgRspToDu(Pst *pst, RlcDlRrcMsgRsp *dlRrcMsgRsp);
uint8_t unpackRlcDlRrcMsgRspToDu(RlcDlRrcMsgRspToDuFunc func, Pst *pst, Buffer *mBuf);
uint8_t packRlcUlUserDataToDu(Pst *pst, RlcUlUserDatInfo *ulUserData);
uint8_t unpackRlcUlUserDataToDu(RlcUlUserDataToDuFunc func, Pst *pst, Buffer *mBuf);
uint8_t packRlcDlUserDataToRlc(Pst *pst, RlcDlUserDataInfo *dlDataMsg);
uint8_t unpackRlcDlUserDataToRlc(DuRlcDlUserDataToRlcFunc func, Pst *pst, Buffer *mBuf);
uint8_t packDuRlcUeDeleteReq(Pst *pst, RlcUeDelete *ueDelete);
uint8_t unpackRlcUeDeleteReq(DuRlcUeDeleteReq func, Pst *pst, Buffer *mBuf);
uint8_t packRlcDuUeDeleteRsp(Pst *pst, RlcUeDeleteRsp *ueDeleteRsp);
uint8_t unpackRlcUeDeleteRsp(RlcDuUeDeleteRsp func, Pst *pst, Buffer *mBuf);
uint8_t packRlcDuSlicePm(Pst *pst, SlicePmList *sliceStats);
uint8_t unpackRlcSlicePm(RlcSlicePmToDuFunc func, Pst *pst, Buffer *mBuf);

/* Event Handler function declarations */
uint8_t RlcProcUeCreateReq(Pst *pst, RlcUeCfg *ueCfg);
uint8_t DuProcRlcUeCfgRsp(Pst *pst, RlcUeCfgRsp *cfgRsp);
uint8_t DuProcRlcUlRrcMsgTrans(Pst *pst, RlcUlRrcMsgInfo *ulRrcMsgInfo);
uint8_t RlcProcDlRrcMsgTransfer(Pst *pst, RlcDlRrcMsgInfo *dlRrcMsgInfo);
uint8_t DuProcRlcRrcDeliveryReport(Pst *pst, RrcDeliveryReport *rrcDeliveryReport);
uint8_t RlcProcUeReconfigReq(Pst *pst, RlcUeCfg *ueCfg);
uint8_t DuProcRlcDlRrcMsgRsp(Pst *pst, RlcDlRrcMsgRsp *dlRrcMsg);
uint8_t DuProcRlcUlUserDataTrans(Pst *pst, RlcUlUserDatInfo *ulUserData);
uint8_t RlcProcDlUserDataTransfer(Pst *pst, RlcDlUserDataInfo *dlDataMsgInfo);
uint8_t RlcProcUeDeleteReq(Pst *pst, RlcUeDelete *ueDelete);
uint8_t DuProcRlcUeDeleteRsp(Pst *pst, RlcUeDeleteRsp *delRsp);
uint8_t DuProcRlcSliceMetrics(Pst *pst, SlicePmList *sliceStats);
#endif /* RLC_INF_H */

/**********************************************************************
         End of file
**********************************************************************/
